#include "common.h"

bool dummyFunction()
{
    return true;
}

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{

  string signalType = "T2bw-025";

  string signalLabel;
  if (signalType == "T2tt")     signalLabel = "#tilde{t} #rightarrow t #tilde{#chi}^{0}";
  if (signalType == "T2bw-025") signalLabel = "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}, x = 0.25";
  if (signalType == "T2bw-050") signalLabel = "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}, x = 0.50";
  if (signalType == "T2bw-075") signalLabel = "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}, x = 0.75";

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
     SonicScrewdriver s;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     s.AddVariable("mStop",          "m_{#tilde{t}}",         "GeV",    28,112.5,812.5,  &(myEvent.mStop),         "");
     s.AddVariable("mNeutralino",    "m_{#tilde{#chi}^{0}}",  "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),   "noOverflowInLastBin");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass(signalType, signalLabel,            "signal",     kSpring-1);
          s.AddDataset(signalType, signalType,   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     // Preselection
     s.AddRegion("preselection",  "Preselection efficiency",  &goesInPreselection);

     s.AddChannel("singleLepton", "e/#mu-channels",           &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
  	 s.Add2DHisto("mStop","mNeutralino");

     // Schedule plots
     s.SchedulePlots("2D");

     // Config plots

     s.SetGlobalStringOption("Plot", "infoTopRight", "CMS Simulation");
     s.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV");

     s.SetGlobalBoolOption("Plot", "exportPdf", true);
     s.SetGlobalBoolOption("Plot", "exportEps", false);
     s.SetGlobalBoolOption("Plot", "exportPng", false);


  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  s.GetDatasetList(&datasetsList);

  cout << "   > Reading datasets... " << endl;
  cout << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = s.GetProcessClass(currentDataset);

     sampleName = currentDataset;
     sampleType = s.GetProcessClassType(currentProcessClass);

     // Open the tree
     TFile f((string(FOLDER_BABYTUPLES)+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple");

     intermediatePointers pointers;
     InitializeBranchesForReading(theTree,&myEvent,&pointers);

     myEvent.mStop       = -1;
     myEvent.mNeutralino = -1;

  // ########################################
  // ##        Run over the events         ##
  // ########################################

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          //float weight = getWeight();
          float weight = 1.0 / myEvent.numberOfInitialEvents;

          s.AutoFillProcessClass(currentProcessClass,weight);
      }

      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }

  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  s.MakePlots();

  TFile fOutput(("./plots/"+signalType+"/efficiencyMap.root").c_str(),"RECREATE");
  TH2F* efficiencyMap  = s.Get2DHistoClone("mStop","mNeutralino",signalType,"preselection","singleLepton");
  formatAndWriteEfficiencyMap(&s,efficiencyMap,signalType,"./plots/"+signalType+"/");
  fOutput.Close();

  //cout << "   > Saving plots..." << endl;
  //s.WritePlots("./plots/"+signalType+"/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  // ...

  printBoxedMessage("Program done.");
  return (0);
}


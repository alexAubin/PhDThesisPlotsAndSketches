#include "common.h"

bool dummyFunction()
{
    return true;
}

float passID_(unsigned int tauIndex)
{
    if (myEvent.tauCandidates_IDMVA2[tauIndex] !=  1.0) return false;

    return true;
}

int isSecondLepton_(unsigned int tauIndex)
{

    int sign = myEvent.leadingLeptonPDGId > 0 ? -1 : 1;
    if (myEvent.tauCandidates_charge[tauIndex] == sign) return false;
    if (myEvent.tauCandidates[tauIndex].DeltaR(myEvent.leadingLepton) < 0.4) return false;

    return true;
}

float candidateDeltaRToGenLepton(TLorentzVector candidate)
{
    float minDeltaR = 999;

    for (unsigned int i = 0 ; i < myEvent.genParticles.size() ; i++)
    {
        int pdgid = abs(myEvent.genParticlesPDGId[i]);
        if (((pdgid) != 11) && ((pdgid) != 13) && ((pdgid) != 15)) continue;

        if (minDeltaR > myEvent.genParticles[i].DeltaR(candidate))
            minDeltaR = myEvent.genParticles[i].DeltaR(candidate);
    }

    return minDeltaR;
}

// #########################################################################
//                              Main function
// #########################################################################

int main (int argc, char *argv[])
{
  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
     SonicScrewdriver s;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     float pT;
     s.AddVariable("pT",    "Tau candidate p_T",    "GeV",    20,0,100,     &(pT),    "");
     int   isSecondLepton;
     s.AddVariable("isSecondLepton",    "isSecondLepton",  "",    2,0,1,     &(isSecondLepton),    "");
     int   passID;
     s.AddVariable("ID",    "Pass ID",             "",    2,0,1,     &(passID),    "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("fakes",       "Fakes",                                    "background",     kRed-7);
     s.AddProcessClass("matched",     "Matched to gen lep. (e/#mu/#tau)",         "background",     kCyan-3);
        s.AddDataset("ttbarnew", "",   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     // Preselection
     s.AddRegion("prepreselection", "",  &goesInPreVetoSelection   );
        s.AddRegion("isSecondLepton", "", "prepreselection", {Cut("isSecondLepton",'>',0)}  );
            s.AddRegion("passID", "",     "isSecondLepton",  {Cut("ID",'>',0)}  );
     s.AddChannel("singleLepton",   "",  &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
     s.Add2DHisto("pT","ID");

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     //s.SchedulePlots("1DFigureOfMerit","var=pT,cutType=keepLowValues");
     s.SchedulePlots("1DFrom2DProjection","varX=pT,varY=ID,projectionType=mean,labelY=Tagging efficiency");

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
     TFile f(("./"+currentDataset+".root").c_str());
     TTree* theTree = (TTree*) f.Get("babyTuple");

     intermediatePointers pointers;
     InitializeBranchesForReading(theTree,&myEvent,&pointers);


  // ########################################
  // ##        Run over the events         ##
  // ########################################

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries / 10; i++)
      //for (int i = 0 ; i < 10 ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          for (unsigned int tau = 0 ; tau < myEvent.tauCandidates.size() ; tau++)
          {
              float deltaR = candidateDeltaRToGenLepton(myEvent.tauCandidates[tau]);
              pT = myEvent.tauCandidates[tau].Pt();
              passID = passID_(tau);
              isSecondLepton = isSecondLepton_(tau);

              if (deltaR < 0.15)     s.AutoFillProcessClass("matched");
              else if (deltaR > 0.5) s.AutoFillProcessClass("fakes");
          }
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
  cout << "   > Saving plots..." << endl;
  s.WritePlots("./plots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}



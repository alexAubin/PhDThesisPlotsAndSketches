#include "../../backgroundEstimation/common/common.h"


#define FOLDER_BABYTUPLES "../../store/babyTuples_0603_withBDT/"

bool goesInDileptonControlRegion_() { return goesInDileptonControlRegion(); }

// #########################################################################
//                              Main function
// #########################################################################

bool goesInPreselection0()
{
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    if (myEvent.nJets < NJET_CUT)  return false;
    return true;
}

bool goesInPreselection1()
{
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    if (myEvent.nJets < NJET_CUT)  return false;
    if (myEvent.nBTag < NBJET_CUT)  return false;

    return true;
}

bool goesInPreselection2()
{
    if (myEvent.MET < MET_CUT) return false;
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    if (myEvent.nJets < NJET_CUT)  return false;
    if (myEvent.nBTag < NBJET_CUT)  return false;

    return true;
}
int main (int argc, char *argv[])
{

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################

     // Create a sonic Screwdriver
      SonicScrewdriver screwdriver;

     // ##########################
     // ##   Create Variables   ##
     // ##########################

     screwdriver.AddVariable("MET",            "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                   "logY");
     screwdriver.AddVariable("METPhi",         "METPhi",                  "GeV",    16,50,530,      &(myEvent.METPhi),                "");

     screwdriver.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                    "logY");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

        screwdriver.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            #ifdef USING_TTBAR_POWHEG
                screwdriver.AddDataset("ttbar_powheg",                "1ltop",  0, 0);
            #endif
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_1l",             "1ltop",  0, 0);
            #endif
            //screwdriver.AddDataset("ttbar_madgraph_scaledown",    "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_scaleup",      "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingdown", "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_matchingup",   "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass166-5",    "1ltop",  0, 0);
            //screwdriver.AddDataset("ttbar_madgraph_mass178-5",    "1ltop",  0, 0);

            screwdriver.AddDataset("singleTop_st",                  "1ltop",  0, 0);


     screwdriver.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);
            #ifdef USING_TTBAR_MADGRAPH
                screwdriver.AddDataset("ttbar_madgraph_2l",   "ttbar_2l",  0, 0);
            #endif


     screwdriver.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             screwdriver.AddDataset("W+jets",    "W+jets", 0, 0);

     screwdriver.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             screwdriver.AddDataset("rare",   "rare", 0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     screwdriver.AddRegion("1lep4jets",                "Preselection",               &goesInPreselection0          );
     screwdriver.AddRegion("1lep4jets1btag",                "Preselection",               &goesInPreselection1          );
     screwdriver.AddRegion("1lep4jets1btagMET80",                "Preselection",               &goesInPreselection2          );
     screwdriver.AddRegion("preselection",                "Preselection",               &goesInPreselection          );
     screwdriver.AddRegion("preselectionMTtail",                "Preselection",               &goesInPreselectionMTtail          );
     screwdriver.AddRegion("preselectionMTtail120",                "Preselection",               "preselectionMTtail", { Cut("MT",'>', 120) } );

     screwdriver.AddRegion("MTpeak",                "Preselection;M_{T} peak Control Region",               &goesInPreselectionMTpeak          );
     screwdriver.AddRegion("0btag_MTtail",          "Preselection;0 b-tag Control Region",                  &goesIn0BtagControlRegionMTtail    );
     screwdriver.AddRegion("2leptons_noMTCut",      "Preselection;2 leptons Control Region, no M_{T} cut",  &goesInDileptonControlRegion_      );
     screwdriver.AddRegion("reversedVeto_noMTCut",  "Preselection;Reversed 2nd lepton veto, no M_{T} cut",  &goesInVetosControlRegion          );



     // ##########################
     // ##   Create Channels    ##
     // ##########################

     screwdriver.AddChannel("singleLepton", "e/#mu-channels",            &goesInSingleLeptonChannel);
     screwdriver.AddChannel("doubleLepton", "ee/e#mu/#mu#mu-channels",   &goesInDoubleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     screwdriver.Create1DHistos();

     // Schedule plots

     // Config plots

     screwdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "stack");
     screwdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );

     screwdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Preliminary");
     screwdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

     screwdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     screwdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     screwdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##       Run over the datasets        ##
  // ########################################

  vector<string> datasetsList;
  screwdriver.GetDatasetList(&datasetsList);

  cout << "   > Reading datasets... " << endl;
  cout << endl;

  for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
  {
     string currentDataset = datasetsList[d];
     string currentProcessClass = screwdriver.GetProcessClass(currentDataset);

     sampleName = currentDataset;
     sampleType = screwdriver.GetProcessClassType(currentProcessClass);

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

     bool ttbarDatasetToBeSplitted = false;
      if (findSubstring(currentDataset,"ttbar")
      && (currentDataset != "ttbar_madgraph_1l")
      && (currentDataset != "ttbar_madgraph_2l"))
          ttbarDatasetToBeSplitted = true;

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
              currentProcessClass_ = "ttbar_2l";

          screwdriver.AutoFillProcessClass(currentProcessClass_,weight);
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
  screwdriver.MakePlots();
  cout << "   > Saving plots..." << endl;
  screwdriver.WritePlots("./plots/preselectionControlPlots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  TableDataMC(&screwdriver,{"1lep4jets", "1lep4jets1btag", "1lep4jets1btagMET80", "preselection", "preselectionMTtail", "preselectionMTtail120"}, "singleLepton", "").Print("table.tab",2);
  TableDataMC(&screwdriver,{"MTpeak", "0btag_MTtail", "2leptons_noMTCut", "reversedVeto_noMTCut" },"singleLepton", "").Print("table2.tab",2);
  TableDataMC(&screwdriver,{"2leptons_noMTCut", "reversedVeto_noMTCut"}, "doubleLepton", "").Print("table3.tab",2);
  printBoxedMessage("Program done.");
  return (0);
}


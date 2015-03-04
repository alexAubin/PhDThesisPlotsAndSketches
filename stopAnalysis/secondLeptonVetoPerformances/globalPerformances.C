#include "common.h"

bool dummyFunction()
{
    return true;
}

bool oneLeptonThreeJets()
{
    if (myEvent.numberOfLepton != 1) return false;
    if (myEvent.nJets           < 3)  return false;

    return true;
}

float leadingTauPt_()
{

    float leadingTauPt = -1;

    int sign = myEvent.leadingLeptonPDGId > 0 ? -1 : 1;

    for (unsigned int i = 0 ; i < myEvent.tauCandidates.size() ; i++)
    {
        if (myEvent.tauCandidates_IDMVA2[i] !=  1.0) continue;
        if (myEvent.tauCandidates_charge[i] == sign) continue;
        if (myEvent.tauCandidates[i].DeltaR(myEvent.leadingLepton) < 0.4) continue;

        if (leadingTauPt < myEvent.tauCandidates[i].Pt())
            leadingTauPt = myEvent.tauCandidates[i].Pt();

    }

    return leadingTauPt;
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

     int isolatedTrackVeto;
     s.AddVariable("isoTrackVeto",    "simpleIsoTrackVeto",    "",    2,0,1,     &(isolatedTrackVeto),  "");
     int tauVeto;
     s.AddVariable("tauVeto",         "simpleTauVeto",         "",    2,0,1,     &(tauVeto),            "");
     int leadingTauPt;
     s.AddVariable("leadingTauPt",    "p_T(lead. #tau)",       "",    20,0,100,  &(leadingTauPt),               "");
     s.AddVariable("MT",              "MT",                    "",    35,0,350,  &(myEvent.MT),                 "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("ttbar1l",            "1#ell",                                    "background",     kRed-2);
     s.AddProcessClass("ttbar2lUnknown",     "2nd #ell unknown",                         "background",     kRed-2);
     s.AddProcessClass("ttbar2lNotAccept",   "2nd #ell not accept",                      "background",     kRed-2);
     s.AddProcessClass("ttbar2lElOrMu",      "2nd #ell (e/mu)",                          "background",     kRed-2);
     s.AddProcessClass("ttbar2lTauToElOrMu", "2nd #ell (#tau #rightarrow #e/#mu)",       "background",     kRed-2);
     s.AddProcessClass("ttbar2lOneCharHadr", "2nd #ell (#tau #rightarrow  1 charg. h)",  "background",     kRed-2);
     s.AddProcessClass("ttbar2lMulCharHadr", "2nd #ell (#tau #rightarrow >1 charg. h)",  "background",     kRed-2);
     s.AddProcessClass("ttbar2lTotal",       "2nd #ell",                                 "background",     kRed-2);

        s.AddDataset("ttbarnew", "ttbar",   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     // Preselection
     s.AddRegion("1leptonThreeJets",   "Baseline selection efficiency",  &oneLeptonThreeJets       );
     s.AddRegion("prepreselection",    "Baseline selection efficiency",  &goesInPreVetoSelection   );
        s.AddRegion("isoTrackVeto",    "Simple isotrack veto",  "prepreselection", {Cut("isoTrackVeto",'=',1)});
        s.AddRegion("tauVeto",         "Simple tau veto",       "prepreselection", {Cut("tauVeto",     '=',1)});
        s.AddRegion("withMTtail",      "With MT cut",           "prepreselection", {Cut("MT",          '>',100)});
     s.AddRegion("preselection",           "Baseline selection efficiency",  &goesInPreselection   );
     s.AddChannel("singleLepton", "e/#mu-channels",                 &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("1DStack");

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

          SecondGeneratedLeptonType secondGeneratedLepton = secondGeneratedLeptonType();
          string currentProcessClass_;
               if (secondGeneratedLepton == UNKNOWN                   ) currentProcessClass_ = "ttbar2lUnknown"    ;
          else if (secondGeneratedLepton == NO_SECOND_LEPTON          ) currentProcessClass_ = "ttbar1l"           ;
          else if (secondGeneratedLepton == NOT_IN_ACCEPTANCE         ) currentProcessClass_ = "ttbar2lNotAccept"  ;
          else if (secondGeneratedLepton == ELEC_OR_MUON              ) currentProcessClass_ = "ttbar2lElOrMu"     ;
          else if (secondGeneratedLepton == TAU_TO_ELEC_OR_MUON       ) currentProcessClass_ = "ttbar2lTauToElOrMu";
          else if (secondGeneratedLepton == TAU_TO_ONE_PRONG          ) currentProcessClass_ = "ttbar2lOneCharHadr";
          else if (secondGeneratedLepton == TAU_TO_MORE_THAN_ONE_PRONG) currentProcessClass_ = "ttbar2lMulCharHadr";


          isolatedTrackVeto = myEvent.isolatedTrackVeto;
          tauVeto           = myEvent.tauVeto          ;
          leadingTauPt = leadingTauPt_();

          float weight = getWeight();

          s.AutoFillProcessClass(currentProcessClass_,weight);

          if (myEvent.numberOfGenLepton >= 2)
            s.AutoFillProcessClass("ttbar2lTotal",weight);
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

  // ...

  TableDataMC(&s,{"1leptonThreeJets", "prepreselection", "isoTrackVeto", "tauVeto", "withMTtail", "preselection" },"singleLepton", "").Print("table.tab",4);

  printBoxedMessage("Program done.");
  return (0);
}



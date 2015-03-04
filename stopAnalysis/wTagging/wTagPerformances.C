#include "common.h"

bool dummyFunction() { return true; }

TLorentzVector genW()
{
    for (unsigned int i = 0 ; i < myEvent.genParticles.size() ; i++)
    {
        int pdgid = abs(myEvent.genParticlesPDGId[i]);
        if (pdgid >= 5) continue;

        if (abs(myEvent.genParticlesPDGId[myEvent.genParticlesMother[i]]) != 24) continue;

        return myEvent.genParticles[myEvent.genParticlesMother[i]];
    }

    return TLorentzVector(0,0,0,0);
}

float genWPt() { return genW().Pt(); }

float deltaRWgen_(TLorentzVector candidate)
{
    TLorentzVector genW_ = genW();
    if (genW_.Pt() == 0) return 99;

    return candidate.DeltaR(genW());
}

int candIsTagged(TLorentzVector cand, float tau2OverTau1, float deltaRLepton)
{
    if (cand.M()  < 70) return false;
    if (deltaRLepton < 1.5) return false;
    if (tau2OverTau1 > 0.5) return false;

    return true;
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

     float genpT;
     s.AddVariable("genpT",          "Gen W p_{T}",                       "GeV",     20,0,400,  &(genpT),          "noUnderflowInFirstBin");

     float pT;
     s.AddVariable("pT",             "W candidate p_{T}",                 "GeV",     30,100,400,&(pT),             "");

     float M;
     s.AddVariable("M",              "W candidate mass",                  "GeV",     40,0,200,  &(M),              "");

     float deltaRLepton;
     s.AddVariable("deltaRLepton",   "#Delta R(W cand.,lepton)",          "",        30,0,5,  &(deltaRLepton),     "");

     float tau2OverTau1;
     s.AddVariable("tau2OverTau1",   "W candidate #tau_{2} / #tau_{1}",   "",        30,0,1.5,  &(tau2OverTau1),   "");

     int tagged;
     s.AddVariable("tagged",         "W candidate is tagged",             "",        2,0,1,     &(tagged),         "");

     float deltaRWgen;
     s.AddVariable("deltaRWgen",     "#Delta R (reco,gen)",               "",        30,0,5,    &(deltaRWgen),     "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("matched",     "Matched to gen. W",                     "background",     kRed-7);
     s.AddProcessClass("fakes",       "Fakes",                                 "background",     kCyan-3);

        s.AddDataset("ttbar", "",   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     //s.AddRegion("preselection", "",  &goesInPreselection   );
     s.AddRegion("noSelection",   "", { }   );
     s.AddRegion("basicSelection",   "", { Cut("pT", '>', 150 ), Cut("deltaRLepton",'>', 1.5) }   );
     s.AddRegion("refinedSelection", "",  "basicSelection", { Cut("tau2OverTau1",'<',0.5), Cut("M", '>', 70) });

     s.AddChannel("singleLepton",   "",  &dummyFunction);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
     s.Add2DHisto("genpT","M");
     s.Add2DHisto("genpT","pT");
     s.Add2DHisto("M","tau2OverTau1");
     s.Add2DHisto("pT","tagged");
     s.Add2DHisto("pT","deltaRLepton");

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("1DStack");
     s.SchedulePlots("1DFrom2DProjection","varX=pT,varY=tagged,projectionType=mean,labelY=Tagging efficiency,tagY=tagEff");

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
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          if (myEvent.MET < 80)            continue;
          if (myEvent.numberOfLepton != 1) continue;
          if (myEvent.nBTag < 1)           continue;
          if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) continue;

          for (unsigned int cand = 0 ; cand < myEvent.WCand.size() ; cand++)
          {
              deltaRWgen = deltaRWgen_(myEvent.WCand[cand]);
              genpT = genWPt();
              pT = myEvent.WCand[cand].Pt();
              M  = myEvent.WCand[cand].M();
              deltaRLepton = myEvent.leadingLepton.DeltaR(myEvent.WCand[cand]);
              tau2OverTau1 = myEvent.WCandTau2[cand] / myEvent.WCandTau1[cand];
              tagged = candIsTagged(myEvent.WCand[cand],tau2OverTau1, deltaRLepton);

              if (deltaRWgen < 0.4) s.AutoFillProcessClass("matched");
              if (deltaRWgen > 2)   s.AutoFillProcessClass("fakes");
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



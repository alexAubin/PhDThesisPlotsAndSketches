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

bool checkMassPoint()
{
    for (unsigned int i = 0 ; i < myEvent.genParticles.size() ; i++)
    {
        int pdgid = abs(myEvent.genParticlesPDGId[i]);
        if (pdgid == 1000006)
        {
            if (abs(myEvent.genParticles[i].M() - 600) > 5) return false;
        }

        if (pdgid == 1000022)
        {
            if (abs(myEvent.genParticles[i].M() - 1) > 5) return false;
        }
    }

    return true;
}

float genDeltaR()
{
    TLorentzVector q1 = -1;
    TLorentzVector q2 = -1;
    for (unsigned int i = 0 ; i < myEvent.genParticles.size() ; i++)
    {
        int pdgid = abs(myEvent.genParticlesPDGId[i]);
        if (pdgid >= 5) continue;
        if (abs(myEvent.genParticlesPDGId[myEvent.genParticlesMother[i]]) != 24) continue;

        if (myEvent.genParticlesPDGId[i] < 0) q1 = myEvent.genParticles[i];
        else                                  q2 = myEvent.genParticles[i];

    }

    return q1.DeltaR(q2);
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
     s.AddVariable("genpT",          "p_{T}(gen. W_{hadr.})",              "GeV",     50,0,500,  &(genpT),          "noUnderflowInFirstBin");

     float genDeltaR_;
     s.AddVariable("genDeltaR",      "Gen. #Delta R (q,#bar{q})",          "",        40,0,5,    &(genDeltaR_),     "");

     int nJets;
     s.AddVariable("nJets",          "Number of selected jets",            "",        5,3,7,    &(nJets),   "");

     int onlyThreeJetsSelected;
     s.AddVariable("onlyThreeJetsSelected",          "Event has only three jets selected",            "",        2,0,1,    &(onlyThreeJetsSelected),   "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     //s.AddProcessClass("T2tt",       "T2tt (600/0)",                                 "background",     kRed);
     //   s.AddDataset("T2tt", "T2tt",   0, 0);
     s.AddProcessClass("ttbar",       "",                                 "background",     kRed);
        s.AddDataset("ttbar", "ttbar",   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     s.AddRegion("noSel",   "", &dummyFunction   );

     s.AddChannel("singleLepton",   "",  &dummyFunction);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
     s.Add2DHisto("genpT","genDeltaR");
     s.Add2DHisto("genpT","nJets");
     s.Add2DHisto("genpT","onlyThreeJetsSelected");

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("2D");
     s.SchedulePlots("1DFrom2DProjection","varX=genpT,varY=genDeltaR,projectionType=mean,labelY=Mean gen. #DeltaR(q,#bar{q}),tagY=meanDeltaR");
     s.SchedulePlots("1DFrom2DProjection","varX=genpT,varY=nJets,projectionType=mean,labelY=Mean number of jets,tagY=meanNJets");
     s.SchedulePlots("1DFrom2DProjection","varX=genpT,varY=onlyThreeJetsSelected,projectionType=mean,labelY=Fraction of evt with only 3 jets,tagY=frac3jets");

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
      for (int i = 0 ; i < nEntries ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          if (myEvent.numberOfGenLepton != 1) continue;
          //if (!checkMassPoint()) continue;

          nJets = myEvent.nJets;
          onlyThreeJetsSelected = (nJets == 3) ? 1 : 0;
          genpT = genW().Pt();
          genDeltaR_ = genDeltaR();

          s.AutoFillProcessClass(currentProcessClass);
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



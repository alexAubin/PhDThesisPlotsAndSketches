#include "common.h"

void computeGenInfo()
{

    myEvent.genMET          = 0;
    myEvent.genTopPt        = 0;
    myEvent.genWPt          = 0;
    myEvent.genbPt          = 0;
    myEvent.genWM           = 0;
    myEvent.genCharginoPt   = 0;
    myEvent.genNeutralinoPt = 0;
    myEvent.genCharginoMass = 0;

    myEvent.genCharginoId1  = -1;
    myEvent.genCharginoId2  = -1;

    TLorentzVector genMETVect(0,0,0,0);

    for (int i = 0 ; i < myEvent.genParticles.size() ; i++)
    {
        int pdgid =  abs(myEvent.genParticlesPDGId[i]);
        if ((pdgid == 1000022) || (pdgid == 12) || (pdgid == 14) || (pdgid == 16))
        {
            genMETVect += myEvent.genParticles[i];
        }

        if (pdgid == 5)
        {
            myEvent.genbPt += myEvent.genParticles[i].Pt();
        }

        if (pdgid == 6)
        {
            myEvent.genTopPt += myEvent.genParticles[i].Pt();
        }

        if (pdgid == 24)
        {
            myEvent.genWPt += myEvent.genParticles[i].Pt();
            myEvent.genWM  += myEvent.genParticles[i].M();
        }

        if (pdgid == 1000022)
        {
            myEvent.genNeutralinoPt += myEvent.genParticles[i].Pt();
        }

        if (pdgid == 1000024)
        {
            myEvent.genCharginoPt   += myEvent.genParticles[i].Pt();
            myEvent.genCharginoMass += myEvent.genParticles[i].M();
            if (myEvent.genCharginoId1 == -1) myEvent.genCharginoId1 = i;
            else                              myEvent.genCharginoId2 = i;
        }

    }

    myEvent.genMET          = genMETVect.Pt();
    myEvent.genTopPt        /= 2;
    myEvent.genbPt          /= 2;
    myEvent.genWPt          /= 2;
    myEvent.genWM           /= 2;
    myEvent.genCharginoPt   /= 2;
    myEvent.genNeutralinoPt /= 2;
    myEvent.genCharginoMass /= 2;
}

bool dummyFunction()
{
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

     s.AddVariable("mStop",            "m_{#tilde{t}}",         "GeV",    28,112.5,812.5,  &(myEvent.mStop),             "");
     s.AddVariable("mNeutralino",      "m_{#tilde{#chi}^{0}}",  "GeV",    16,-12.5,387.5,  &(myEvent.mNeutralino),       "noOverflowInLastBin");
     s.AddVariable("genMET",           "gen. MET",              "GeV",    50,0,1000,       &(myEvent.genMET),            "");
     s.AddVariable("genTopPt",         "gen. top p_T",          "GeV",    50,0,1000,       &(myEvent.genTopPt),          "");
     s.AddVariable("genbPt",           "gen. b p_T",            "GeV",    50,0,1000,       &(myEvent.genbPt),            "");
     s.AddVariable("genWPt",           "gen. W   p_T",          "GeV",    50,0,1000,       &(myEvent.genWPt),            "");
     s.AddVariable("genWM",            "gen. W   mass",         "GeV",    50,0,100,        &(myEvent.genWM),             "");
     s.AddVariable("genCharginoPt",    "gen. Chargino p_T",     "GeV",    50,0,1000,       &(myEvent.genCharginoPt),     "");
     s.AddVariable("genCharginoMass",  "gen. Chargino mass",    "GeV",    50,0,500,        &(myEvent.genCharginoMass),   "");
     s.AddVariable("genNeutralinoPt",  "gen. Neutralino p_T",   "GeV",    50,0,1000,       &(myEvent.genNeutralinoPt),   "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("T2tt",   "#tilde{t} #rightarrow t #tilde{#chi}^{0}",                     "signal",     kSpring-1);
             s.AddDataset("T2tt",   "T2tt", 0, 0);

/*
     s.AddProcessClass("T2bw-050",   "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}",                     "signal",     kSpring-1);
             s.AddDataset("T2bw-050","T2bw-050", 0, 0);

     s.AddProcessClass("T2bw-075",   "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}",                     "signal",     kSpring-1);
             s.AddDataset("T2bw-075","T2bw-075", 0, 0);
*/
/*
     s.AddProcessClass("T2bw-025",   "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}",                     "signal",     kSpring-1);
             s.AddDataset("T2bw-025","T2bw-025", 0, 0);
*/
     // ##########################
     // ##    Create Regions    ##
     // ##########################

     // Preselection
     s.AddRegion("preselection",  "#geq 1 e/#mu, #geq 3 jets",  vector<Cut>());

     s.AddChannel("singleLepton", "", &dummyFunction);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
  	 s.Add3DHisto("mStop","mNeutralino","genMET");
  	 s.Add3DHisto("mStop","mNeutralino","genTopPt");
  	 s.Add3DHisto("mStop","mNeutralino","genbPt");
  	 s.Add3DHisto("mStop","mNeutralino","genWPt");
  	 s.Add3DHisto("mStop","mNeutralino","genWM");
  	 s.Add3DHisto("mStop","mNeutralino","genCharginoPt");
  	 s.Add3DHisto("mStop","mNeutralino","genNeutralinoPt");
  	 s.Add3DHisto("mStop","mNeutralino","genCharginoMass");

     // Schedule plots
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genMET,projectionType=mean,labelZ=Mean gen. MET [GeV],tagZ=meanGenMET");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genTopPt,projectionType=mean,labelZ=Mean gen. top p_{T} [GeV],tagZ=meanGenTopPt");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genbPt,projectionType=mean,labelZ=Mean gen. b p_{T} [GeV],tagZ=meanGenbPt");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genWPt,projectionType=mean,labelZ=Mean gen. W p_{T} [GeV],tagZ=meanGenWPt");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genWM,projectionType=mean,labelZ=Mean gen. W mass [GeV],tagZ=meanGenWM");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genCharginoPt,projectionType=mean,labelZ=Mean gen. chargino p_{T} [GeV],tagZ=meanGenCharginoPt");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genCharginoMass,projectionType=mean,labelZ=Mean gen. chargino mass [GeV],tagZ=meanGenCharginoMass");
     s.SchedulePlots("2DFrom3DProjection","varX=mStop,varY=mNeutralino,varZ=genNeutralinoPt,projectionType=mean,labelZ=Mean gen. neutralino p_{T} [GeV],tagZ=meanGenNeutralinoPt");

     // Config plots

     s.SetGlobalBoolOption("1DSuperimposed",    "includeSignal",          true   );

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
      //for (int i = 0 ; i < 10 ; i++)
      {
          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          computeGenInfo();

          //if (abs(myEvent.genParticles[myEvent.genCharginoId1].Pt() - 200) > 20) continue;
          //if (abs(myEvent.genParticles[myEvent.genCharginoId2].Pt() - 200) > 20) continue;

          float weight = 1.0;

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
  cout << "   > Saving plots..." << endl;
  s.WritePlots("./plots/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  // ...

  printBoxedMessage("Program done.");
  return (0);
}


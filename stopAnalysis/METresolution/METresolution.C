#include "common.h"

float Phi_mpi_pi(float x) {
    while (x >= 3.1415) x -= 2*3.1415;
    while (x <  3.1415) x += 2*3.1415;
    return x;
}


void computeGenInfo()
{
    myEvent.genMET = 0;
    myEvent.genMETdPhi = 0;

    TLorentzVector genLeadingLeptonVect(0,0,0,0);
    TLorentzVector genMETVect(0,0,0,0);

    for (int i = 0 ; i < myEvent.genParticles.size() ; i++)
    {
        int pdgid =  abs(myEvent.genParticlesPDGId[i]);
        if ((pdgid == 1000022) || (pdgid == 12) || (pdgid == 14) || (pdgid == 16))
        {
            genMETVect += myEvent.genParticles[i];
        }
        if ((pdgid == 11) || (pdgid == 13) || (pdgid == 15))
        {
            if (genLeadingLeptonVect.Pt() < myEvent.genParticles[i].Pt())
                genLeadingLeptonVect      = myEvent.genParticles[i];
        }
    }

    myEvent.genMET     = genMETVect.Pt();
    myEvent.genMETdPhi = abs(Phi_mpi_pi(genMETVect.Phi() - genLeadingLeptonVect.Phi()));
}

bool goesInDileptonControlRegion_() { return goesInDileptonControlRegion(); }


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

     s.AddVariable("MET",            "MET",                     "GeV",    16,50,530,      &(myEvent.MET),                  "logY");
     s.AddVariable("METPhi",         "METPhi",                  "GeV",    16,50,530,      &(myEvent.METPhi),               "");

     s.AddVariable("MT",             "M_{T}",                   "GeV",    40,0,400,       &(myEvent.MT),                   "");
    /*
     s.AddVariable("MTpeak",         "M_{T}",                   "GeV",    20,0,100,       &(myEvent.MT),                   "noOverflowInLastBin");
     s.AddVariable("MTtail",         "M_{T}",                   "GeV",    30,100,400,     &(myEvent.MT),                   "logY,noUnderflowInFirstBin");
     s.AddVariable("deltaPhiMETJets","#Delta#Phi(MET,j_{1,2})", "rad",    16,0,3.2,       &(myEvent.deltaPhiMETJets),      "");
     s.AddVariable("MT2W",           "M_{T2}^{W}",              "GeV",    20,0,500,       &(myEvent.MT2W),                 "");
     s.AddVariable("HTratio",        "H_{T}^{ratio}",           "",       20,0,1.2,       &(myEvent.HTRatio),              "");
     s.AddVariable("HadronicChi2",   "Hadronic #chi^{2}",       "",       20,0,20,        &(myEvent.hadronicChi2),         "logY");
     s.AddVariable("leadingBPt",     "p_{T}(leading b-jet)",    "GeV",    20,0,400,       &(myEvent.leadingBPt),           "");
     s.AddVariable("leadingJetPt",   "p_{T}(leading jet)",      "GeV",    20,0,600,       &(myEvent.leadingJetPt),         "");
     s.AddVariable("leptonPt",       "p_{T}(lepton)",           "GeV",    28,20,300,      &(myEvent.leadingLeptonPt),      "");
     s.AddVariable("Mlb",            "M'(lb)",                  "GeV",    20,0,500,       &(myEvent.Mlb),                  "");
     s.AddVariable("Mlb_hemi",       "M(lb)",             "GeV",          20,0,500,       &(myEvent.Mlb_hemi),             "");
     s.AddVariable("M3b",            "M3b",                     "GeV",    20,0,1000,      &(myEvent.M3b),                  "");
     s.AddVariable("deltaRLeptonB",  "#DeltaR(l,leading b)",    "",       20,0,5,         &(myEvent.deltaRLeptonLeadingB), "");
     s.AddVariable("HTLeptonPtMET",  "HT + MET + p_{T}(lepton)","GeV",    20,100,2100,    &(myEvent.HTPlusLeptonPtPlusMET),"");
     */
     s.AddVariable("HT",             "H_{T}",                   "",       46,120,1500,    &(myEvent.HT),                   "");
     s.AddVariable("METoverSqrtHT",  "MET / #sqrt{H_{T}}",      "",       32,0,32,        &(myEvent.METoverSqrtHT),        "");

     float leadingNonBPtN4;
     s.AddVariable("leadingNonBPt",     "p_{T}(leading nonb-jet)",    "GeV",    20,0,500,       &(leadingNonBPtN4),           "");
     float leadingNonBPtN5;
     s.AddVariable("leadingNonBPtN5",     "p_{T}(leading nonb-jet) (Njet>4)",    "GeV",    20,0,500,       &(leadingNonBPtN5),           "noUnderflowInFirstBin");

     int nJets, nBtag;

     s.AddVariable("nJets",          "Number of selected jets",               "",       11,0,10,        &(nJets),              "");
     s.AddVariable("nBtag",          "Number of selected b-tagged jets",      "",       5, 0,4,         &(nBtag),              "");
     s.AddVariable("numberOfPrimaryVertices",   "Number of primary vertices", "",       41, 0,40,       &(myEvent.numberOfPrimaryVertices),              "");

     float METPtReso;
     float METdPhiReso;
     s.AddVariable("METPtReso",      "MET p_{T} rel. resolution",         "GeV",    50, 0,5,      &(METPtReso),       "");
     s.AddVariable("METdPhiReso",     "MET #Delta #Phi rel. resolution",          "rad",    50, 0,2,      &(METdPhiReso),      "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

        s.AddProcessClass("1ltop", "1l top",                             "background",kRed-7);
            s.AddDataset("ttbar_powheg",                "1ltop",  0, 0);

            s.AddDataset("singleTop_st",                  "1ltop",  0, 0);

     s.AddProcessClass("ttbar_2l", "t#bar{t} #rightarrow l^{+}l^{-}", "background",kCyan-3);

     s.AddProcessClass("W+jets",   "W+jets",                          "background",kOrange-2);
             s.AddDataset("W+jets",    "W+jets", 0, 0);

     s.AddProcessClass("rare",   "rare",                              "background",kMagenta-5);
             s.AddDataset("rare",   "rare", 0, 0);
     /*
     s.AddProcessClass("T2tt", "T2tt (450, 50)",  "signal", COLORPLOT_GREEN );
            s.AddDataset("T2tt", "T2tt",   0, 0);

     s.AddProcessClass("T2bw", "T2bw, x = 0.50 (450, 50)",  "signal", COLORPLOT_MAGENTA );
            s.AddDataset("T2bw-050", "T2bw",   0, 0);
     */
     /*
     s.AddProcessClass("data",   "data",                              "data",COLORPLOT_BLACK);
            s.AddDataset("SingleElec",   "data", 0, 0);
            s.AddDataset("SingleMuon",   "data", 0, 0);
            s.AddDataset("DoubleElec",   "data", 0, 0);
            s.AddDataset("DoubleMuon",   "data", 0, 0);
            s.AddDataset("MuEl",         "data", 0, 0);
     */

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     s.AddRegion("preselection",                  "Preselection",               &goesInPreselection          );
     s.AddRegion("preselection_MTtail",           "Preselection;M_{T} tail",    &goesInPreselectionMTtail    );

     // ##########################
     // ##   Create Channels    ##
     // ##########################

     s.AddChannel("singleLepton", "e/#mu-channels",            &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
     s.Add2DHisto("MET","HT");
     s.Add2DHisto("MT","METPtReso");
     s.Add2DHisto("MT","METdPhiReso");
     s.Add2DHisto("MT","numberOfPrimaryVertices");
     s.Add2DHisto("numberOfPrimaryVertices","METdPhiReso");
     s.Add2DHisto("numberOfPrimaryVertices","METPtReso");

     // Schedule plots
     //s.SchedulePlots("1DDataMCComparison");
     s.SchedulePlots("1DStack");
     s.SchedulePlots("1DSuperimposed");
     s.SchedulePlots("2D");
     s.SchedulePlots("1DFrom2DProjection","varX=MT,varY=METPtReso,projectionType=mean,labelY=MET Pt rel. resolution");
     s.SchedulePlots("1DFrom2DProjection","varX=MT,varY=METdPhiReso,projectionType=mean,labelY=MET #Delta #Phi rel. resolution");
     s.SchedulePlots("1DFrom2DProjection","varX=MT,varY=numberOfPrimaryVertices,projectionType=mean,labelY=Number of vertices");
     s.SchedulePlots("1DFrom2DProjection","varX=numberOfPrimaryVertices,varY=METdPhiReso,projectionType=mean,labelY=MET #Delta #Phi rel. resolution");
     s.SchedulePlots("1DFrom2DProjection","varX=numberOfPrimaryVertices,varY=METPtReso,projectionType=mean,labelY=MET Pt rel. resolution");

     // Config plots
     s.SetGlobalBoolOption  ("1DSuperimposed",    "includeSignal",    true   );

     s.SetGlobalStringOption("DataMCComparison",  "includeSignal",    "superimposed");
     s.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",     100.0    );

     s.SetGlobalStringOption("1DStack",           "includeSignal",    "superimposed");
     s.SetGlobalFloatOption ("1DStack",           "factorSignal",     100.0    );

     s.SetGlobalFloatOption ("DataMCRatio",       "min",              0.0    );
     s.SetGlobalFloatOption ("DataMCRatio",       "max",              2.0    );

     s.SetGlobalStringOption("Plot",              "infoTopRight",     "CMS Preliminary");
     s.SetGlobalStringOption("Plot",              "infoTopLeft",      "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");

     s.SetGlobalBoolOption  ("Plot",              "exportPdf",        true);
     s.SetGlobalBoolOption  ("Plot",              "exportEps",        false);
     s.SetGlobalBoolOption  ("Plot",              "exportPng",        false);

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

     if (sampleType == "signal")
     {
        theTree->SetBranchAddress("mStop",       &(myEvent.mStop));
        theTree->SetBranchAddress("mNeutralino", &(myEvent.mNeutralino));
     }
     else
     {
         myEvent.mStop       = -1;
         myEvent.mNeutralino = -1;
     }

  // ########################################
  // ##        Run over the events         ##
  // ########################################

     bool ttbarDatasetToBeSplitted = false;
      if (findSubstring(currentDataset,"ttbar"))
          ttbarDatasetToBeSplitted = true;

      int nEntries = theTree->GetEntries();
      for (int i = 0 ; i < nEntries ; i++)
      {
          //if (i > float(nEntries) * 0.1) break;

          if (i % (nEntries / 50) == 0) printProgressBar(i,nEntries,currentDataset);

          // Get the i-th entry
          ReadEvent(theTree,i,&pointers,&myEvent);

          nJets = myEvent.nJets;
          nBtag = myEvent.nBTag;

          float weight = getWeight();

          // Split 1-lepton ttbar and 2-lepton ttbar
          string currentProcessClass_ = currentProcessClass;
          if (ttbarDatasetToBeSplitted && (myEvent.numberOfGenLepton == 2))
              currentProcessClass_ = "ttbar_2l";

          #ifdef LEADING_NON_B_PT_ALREADY_COMPUTED
          leadingNonBPtN4= myEvent.leadingNonBPt;
          #else
          leadingNonBPtN4= leadingNonBPt();
          #endif
          leadingNonBPtN5=-1;
          if (nJets>4) leadingNonBPtN5= leadingNonBPtN4;

          if ((currentProcessClass_ == "T2tt") && ((myEvent.mStop != 450) || (myEvent.mNeutralino != 50))) continue;
          if ((currentProcessClass_ == "T2bw") && ((myEvent.mStop != 450) || (myEvent.mNeutralino != 50))) continue;

          computeGenInfo();
          METPtReso   = myEvent.MET    / myEvent.genMET;
          METdPhiReso = abs(Phi_mpi_pi(myEvent.METPhi - myEvent.leadingLepton.Phi())) / myEvent.genMETdPhi;

          s.AutoFillProcessClass(currentProcessClass_,weight);
      }

      printProgressBar(nEntries,nEntries,currentDataset);
      cout << endl;
      f.Close();

  }
/*
  // #############################
  // ##   Apply scale factors   ##
  // #############################

    Table scaleFactors = Table("../prediction/scaleFactors/preselection.tab");

    Figure SF_pre           = scaleFactors.Get("value","SF_pre");
    Figure SF_post          = scaleFactors.Get("value","SF_post");
    Figure SF_0btag         = scaleFactors.Get("value","SF_0btag");
    Figure SF_vetopeak      = scaleFactors.Get("value","SF_vetopeak");
    Figure SF_MTtail_1ltop  = scaleFactors.Get("value","SF_MTtail_1ltop");
    Figure SF_MTtail_Wjets  = scaleFactors.Get("value","SF_MTtail_Wjets");

    s.ApplyScaleFactor("ttbar_2l", "preselection_MTtail_withCorr",  "singleLepton", SF_pre);
    s.ApplyScaleFactor("1ltop",    "preselection_MTtail_withCorr",  "singleLepton", SF_post * SF_MTtail_1ltop);
    s.ApplyScaleFactor("W+jets",   "preselection_MTtail_withCorr",  "singleLepton", SF_post * SF_MTtail_Wjets);
*/
  // ###################################
  // ##   Make plots and write them   ##
  // ###################################

  cout << endl;
  cout << "   > Making plots..." << endl;
  s.MakePlots();
  cout << "   > Saving plots..." << endl;
  s.WritePlots("./plots/METresolution/");

  printBoxedMessage("Plot generation completed");

  // #############################
  // ##   Post-plotting tests   ##
  // #############################

  printBoxedMessage("Program done.");
  return (0);
}


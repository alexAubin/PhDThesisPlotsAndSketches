#include "common.h"

bool dummyFunction()
{
    return true;
}

bool basicID(unsigned int index)
{
    if (myEvent.pfCandidates[index].DeltaR(myEvent.leadingLepton) < 0.1) return false;

    int id = abs(myEvent.pfCandidates_id[index]);
    int sign = myEvent.leadingLeptonPDGId > 0 ? -1 : 1;

    if ((id == 11) || (id == 13))
    {
        if (myEvent.pfCandidates[index].Pt() < 5) return false;
    }
    else
    {
        if (myEvent.pfCandidates[index].Pt() < 10) return false;
        if (myEvent.pfCandidates_charge[index] == sign) return false;
    }

    return true;
}

pair<float,float> relIsoAndDz(unsigned int index)
{
    int id = abs(myEvent.pfCandidates_id[index]);

    float dz = 99;
    float reliso = 99;

    if (id == 11)
    {
        dz = abs(myEvent.pfCandidates_gsfdz[index]);
        reliso = myEvent.pfCandidates_iso[index] / myEvent.pfCandidates_gsfPt[index];
    }
    else if (id == 13)
    {
        dz = abs(myEvent.pfCandidates_dz[index]);
        reliso = myEvent.pfCandidates_iso[index] / myEvent.pfCandidates[index].Pt();
    }
    else
    {
        dz = abs(myEvent.pfCandidates_dz[index]);
        reliso = myEvent.pfCandidates_iso[index] / myEvent.pfCandidates[index].Pt();
    }

    return pair<float,float>(reliso,dz);
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
     s.AddVariable("pT",             "PF candidate pT",         "",     60,0,30,    &(pT),             "logY");
     float relIso;
     s.AddVariable("relIso",         "PF candidate rel. iso",   "",     40,0,0.25,  &(relIso),          "logY");
     float dz;
     s.AddVariable("dz",             "PF candidate dz",        "cm",    40,0,0.25,  &(dz),              "logY");
     int   passID;
     s.AddVariable("ID",             "Pass ID",                 "",     2,0,1,      &(passID),          "");

     // #########################################################
     // ##   Create ProcessClasses (and associated datasets)   ##
     // #########################################################

     s.AddProcessClass("ElMufakes",     "e/#mu fakes",                              "background",     kRed-7);
     s.AddProcessClass("NIfakes",       "NI fakes",                                 "background",     kRed-9);
     s.AddProcessClass("ElMumatched",   "e/#mu cand. matched to gen e/#mu/#tau",    "background",     kCyan-3);
     s.AddProcessClass("NImatched",     "NI cand. matched to gen e/#mu/#tau",       "background",     kCyan-5);
        s.AddDataset("ttbarnew", "",   0, 0);

     // ##########################
     // ##    Create Regions    ##
     // ##########################

     // Preselection
     s.AddRegion("prepreselection", "",  &goesInPreVetoSelection   );
            s.AddRegion("passID", "",     "prepreselection",  {Cut("ID",'>',0)}  );
     s.AddChannel("singleLepton",   "",  &goesInSingleLeptonChannel);

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################

     // Create histograms
     s.Create1DHistos();
     //s.Add2DHisto("relIso","ID");

     // Schedule plots
     s.SchedulePlots("1DSuperimposed");
     //s.SchedulePlots("1DFrom2DProjection","varX=relIso,varY=ID,projectionType=mean,labelY=Tagging efficiency");

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

          for (unsigned int pf = 0 ; pf < myEvent.pfCandidates.size() ; pf++)
          {
              float deltaR = candidateDeltaRToGenLepton(myEvent.pfCandidates[pf]);
              int id = abs(myEvent.pfCandidates_id[pf]);
              pair<float,float> relIsoAndDz_ = relIsoAndDz(pf);
              pT = myEvent.pfCandidates[pf].Pt();
              relIso = relIsoAndDz_.first;
              dz     = relIsoAndDz_.second;
              passID = basicID(pf);
              if (deltaR < 0.15)
              {
                  if ((id == 11) || (id == 13)) s.AutoFillProcessClass("ElMumatched");
                  else                          s.AutoFillProcessClass("NImatched");
              }
              else if (deltaR > 0.5)
              {
                  if ((id == 11) || (id == 13)) s.AutoFillProcessClass("ElMufakes");
                  else                          s.AutoFillProcessClass("NIfakes");
              }
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



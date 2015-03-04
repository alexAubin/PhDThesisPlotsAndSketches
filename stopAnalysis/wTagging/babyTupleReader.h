#ifndef babyFormat
#define babyFormat

typedef struct
{
    // ------------
    // General info
    // ------------

    Int_t run;                                  // Run number
    Int_t lumi;                                 // Lumiblock number
    Int_t event;                                // Event number

    // ------------
    // Trigger info
    // ------------

    Bool_t triggerMuon;                         // Single-Muon trigger has been fired
    Bool_t xtriggerMuon;                        // Muon cross-trigger  has been fired
    Bool_t triggerElec;                         // Single-Elec trigger has been fired

    // ------------
    // Leptons info
    // ------------

    Short_t        numberOfLepton;              // Number of selected leptons

    TLorentzVector leadingLepton;               // p4 of the leading selected lepton
    Short_t        leadingLeptonPDGId;          // pdgid of the leading selected lepton
    float          leadingLeptonIdEfficiency;   // lepton ID efficiency weight
    float          leadingLeptonIsoScaleFactor; // lepton isolation scale factor

    TLorentzVector secondLepton;                // p4 of the second lepton
    Short_t        secondLeptonPDGId;           // pdgid of the second lepton
    float          secondLeptonIdEfficiency;    // lepton ID efficiency weight
    float          secondLeptonIsoScaleFactor;  // lepton isolation scale factor

    Bool_t         isolatedTrackVeto;           // Event pass/fail the isolated track veto
    Bool_t         tauVeto;                     // Event pass/fail the tau veto

    // ------------
    // Jets info
    // ------------

    Short_t                nJets;               // Number of selected jets
    Short_t                nBTag;               // Number of selected jets b-tagged
    vector<TLorentzVector> jets;                // p4 of the selected jets
    vector<Float_t>        jets_CSV_raw;        // CSV value of the selected jets, before reshaping
    vector<Float_t>        jets_CSV_reshaped;   // CSV value of the selected jets, after reshaping
    vector<Int_t>          jets_partonFlav;     // Parton flavour of the selected jets

    // ------------
    // "High-level" /BDT variables
    // ------------

    Float_t MET;                                // Type-1 + phi-corrected PF MET
    Float_t MT;                                 // transverse mass of leading lepton + MET

    // ------------
    // Generator-level infos
    // ------------

    Int_t   numberOfGenLepton;                  // Number of MC leptons (e/mu/tau, leptons from tau decay are not taken into account)

    vector<TLorentzVector> genParticles;        // p4 of status=3 particles
    vector<Int_t>          genParticlesPDGId;   // PDG of status=3 particles
    vector<Int_t>          genParticlesMother;  // Mother index of the status=3 particles

    // ------------
    // Weights and infos related to weights
    // ------------

    Int_t   numberOfInitialEvents;              // Number of events for the given dataset or signal mass point, before any skimming/selection applied
    Float_t crossSection;                       // Cross section for the given dataset or signal mass point
    Int_t   numberOfTruePU;                     // Number of true pile-up (MC, used in weightPileup)
    Int_t   numberOfPrimaryVertices;            // Number of primary vertices, used to check pile-up data/MC aggreement

    Float_t weightCrossSection;                 // = (crossSection / numberOfInitialEvents) ; user should multiply this weight by the desired lumi
    Float_t weightPileUp;                       // Weight for PU-reweighting
    Float_t weightISRmodeling;                  // Weight for ISR modeling
    Float_t weightTopPt;                        // Weight for top Pt reweighting
    Float_t weightTriggerEfficiency;            // Weight for singleLepton trigger efficiency

        // Non selected leptons (with pT > 10, eta < 2.4, and electron/muon ID applied)

    vector<TLorentzVector> nonSelectedLeptons;              // p4 of the non selected leptons
    vector<Short_t>        nonSelectedLeptonsPDGId;         // pdgid of the non selected leptons

        // Raw MET (used as a cross check for when applying MET filters after production)

    Float_t rawPFMET;                           // Raw MET from PF-based algorithm

        // W-tagging informations

    vector<TLorentzVector> WCand;   // p4 of the W candidates
    vector<float> WCandTau1;        // tau_1 of the W candidates
    vector<float> WCandTau2;        // tau_2 of the W candidates
    vector<float> WCandTau3;        // tau_3 of the W candidates

} babyEvent;

typedef struct
{

    TLorentzVector*         pointerToLeadingLepton;
    TLorentzVector*         pointerToSecondLepton;
    vector<TLorentzVector>* pointerToGenParticles;
    vector<Int_t>*          pointerToGenParticlesPDGId;
    vector<Int_t>*          pointerToGenParticlesMother;
    vector<TLorentzVector>* pointerToWCand;
    vector<float>*          pointerToWCandTau1;
    vector<float>*          pointerToWCandTau2;
    vector<float>*          pointerToWCandTau3;

} intermediatePointers;

void ReadEvent(TTree* theTree, long int i, intermediatePointers* pointers, babyEvent* myEvent)
{
      theTree->GetEntry(i);

      myEvent->leadingLepton         = *(pointers->pointerToLeadingLepton);
      myEvent->secondLepton          = *(pointers->pointerToSecondLepton);
      myEvent->genParticles          = *(pointers->pointerToGenParticles);
      myEvent->genParticlesPDGId     = *(pointers->pointerToGenParticlesPDGId);
      myEvent->genParticlesMother    = *(pointers->pointerToGenParticlesMother);
      myEvent->WCand                 = *(pointers->pointerToWCand);
      myEvent->WCandTau1             = *(pointers->pointerToWCandTau1);
      myEvent->WCandTau2             = *(pointers->pointerToWCandTau2);
      myEvent->WCandTau3             = *(pointers->pointerToWCandTau3);

}

void InitializeBranchesForReading(TTree* theTree, babyEvent* myEvent,intermediatePointers* pointers)
{
    theTree->SetBranchAddress("run",                                          &(myEvent->run));
    theTree->SetBranchAddress("lumi",                                         &(myEvent->lumi));
    theTree->SetBranchAddress("event",                                        &(myEvent->event));

    theTree->SetBranchAddress("triggerMuon",                                  &(myEvent->triggerMuon));
    theTree->SetBranchAddress("xtriggerMuon",                                 &(myEvent->xtriggerMuon));
    theTree->SetBranchAddress("triggerElec",                                  &(myEvent->triggerElec));

    theTree->SetBranchAddress("numberOfLepton",                               &(myEvent->numberOfLepton));
    pointers->pointerToLeadingLepton = 0;
    theTree->SetBranchAddress("leadingLepton",                                &(pointers->pointerToLeadingLepton));
    theTree->SetBranchAddress("leadingLeptonPDGId",                           &(myEvent->leadingLeptonPDGId));
    theTree->SetBranchAddress("leadingLeptonIdEfficiency",                    &(myEvent->leadingLeptonIdEfficiency));
    theTree->SetBranchAddress("leadingLeptonIsoScaleFactor",                  &(myEvent->leadingLeptonIsoScaleFactor));
    pointers->pointerToSecondLepton = 0;
    theTree->SetBranchAddress("secondLepton",                                 &(pointers->pointerToSecondLepton));
    theTree->SetBranchAddress("secondLeptonPDGId",                            &(myEvent->secondLeptonPDGId));
    theTree->SetBranchAddress("secondLeptonIdEfficiency",                     &(myEvent->secondLeptonIdEfficiency));
    theTree->SetBranchAddress("secondLeptonIsoScaleFactor",                   &(myEvent->secondLeptonIsoScaleFactor));
    theTree->SetBranchAddress("isolatedTrackVeto",                            &(myEvent->isolatedTrackVeto));
    theTree->SetBranchAddress("tauVeto",                                      &(myEvent->tauVeto));

    theTree->SetBranchAddress("nJets",                                        &(myEvent->nJets));
    theTree->SetBranchAddress("nBTag",                                        &(myEvent->nBTag));

    theTree->SetBranchAddress("MET",                                          &(myEvent->MET));
    theTree->SetBranchAddress("MT",                                           &(myEvent->MT));

    theTree->SetBranchAddress("numberOfGenLepton",                            &(myEvent->numberOfGenLepton));

    pointers->pointerToGenParticles = 0;
    theTree->SetBranchAddress("genParticles",                                 &(pointers->pointerToGenParticles));
    pointers->pointerToGenParticlesPDGId = 0;
    theTree->SetBranchAddress("genParticlesPDGId",                            &(pointers->pointerToGenParticlesPDGId));
    pointers->pointerToGenParticlesMother = 0;
    theTree->SetBranchAddress("genParticlesMother",                           &(pointers->pointerToGenParticlesMother));

    theTree->SetBranchAddress("numberOfInitialEvents",                        &(myEvent->numberOfInitialEvents));
    theTree->SetBranchAddress("weightCrossSection",                           &(myEvent->weightCrossSection));
    theTree->SetBranchAddress("weightPileUp",                                 &(myEvent->weightPileUp));
    theTree->SetBranchAddress("weightISRmodeling",                            &(myEvent->weightISRmodeling));
    theTree->SetBranchAddress("weightTopPt",                                  &(myEvent->weightTopPt));
    theTree->SetBranchAddress("weightTriggerEfficiency",                      &(myEvent->weightTriggerEfficiency));
    /*
    pointers->pointerToNonSelectedLeptons           = 0;
    theTree->SetBranchAddress("nonSelectedLeptons",                           &(pointers->pointerToNonSelectedLeptons));
    //pointers->pointerToNonSelectedLeptonsPDGId      = 0;
    //theTree->SetBranchAddress("nonSelectedLeptonsPDGId",                      &(pointers->pointerToNonSelectedLeptonsPDGId));
    */

    pointers->pointerToWCand = 0;
    theTree->SetBranchAddress("WCand",                                        &(pointers->pointerToWCand));
    pointers->pointerToWCandTau1 = 0;
    theTree->SetBranchAddress("WCandTau1",                                    &(pointers->pointerToWCandTau1));
    pointers->pointerToWCandTau2 = 0;
    theTree->SetBranchAddress("WCandTau2",                                    &(pointers->pointerToWCandTau2));
    pointers->pointerToWCandTau3 = 0;
    theTree->SetBranchAddress("WCandTau3",                                    &(pointers->pointerToWCandTau3));

}
#endif


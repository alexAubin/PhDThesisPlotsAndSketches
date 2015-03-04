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

        // Phi of the (corrected) MET

    Float_t METPhi;                             // Type-1 + phi-corrected PF MET

        // Isolated track veto infos

    vector<TLorentzVector> pfCandidates;        // Comment your code, bro
    vector<Int_t>          pfCandidates_id;     //
    vector<Float_t>        pfCandidates_dz;     //
    vector<Float_t>        pfCandidates_gsfPt;  //
    vector<Float_t>        pfCandidates_gsfdz;  // Duuuuude
    vector<Float_t>        pfCandidates_iso;    //
    vector<Float_t>        pfCandidates_charge; //

        // Taus

    vector<TLorentzVector> tauCandidates;        //
    vector<Int_t>          tauCandidates_charge; //  STAHP
    vector<Int_t>          tauCandidates_IDMVA2; //


} babyEvent;

typedef struct
{

    TLorentzVector*         pointerToLeadingLepton;
    TLorentzVector*         pointerToSecondLepton;
    vector<TLorentzVector>* pointerToGenParticles;
    vector<Int_t>*          pointerToGenParticlesPDGId;
    vector<Int_t>*          pointerToGenParticlesMother;
    vector<TLorentzVector>* pointerToPFCandidates;
    vector<Int_t>*          pointerToPFCandidates_id;
    vector<Float_t>*        pointerToPFCandidates_dz;
    vector<Float_t>*        pointerToPFCandidates_gsfPt;
    vector<Float_t>*        pointerToPFCandidates_gsfdz;
    vector<Float_t>*        pointerToPFCandidates_iso;
    vector<Float_t>*        pointerToPFCandidates_charge;
    vector<TLorentzVector>* pointerToTauCandidates;
    vector<Int_t>*          pointerToTauCandidates_charge;
    vector<Int_t>*          pointerToTauCandidates_IDMVA2;

} intermediatePointers;

void ReadEvent(TTree* theTree, long int i, intermediatePointers* pointers, babyEvent* myEvent)
{
      theTree->GetEntry(i);

      myEvent->leadingLepton         = *(pointers->pointerToLeadingLepton);
      myEvent->secondLepton          = *(pointers->pointerToSecondLepton);
      myEvent->genParticles          = *(pointers->pointerToGenParticles);
      myEvent->genParticlesPDGId     = *(pointers->pointerToGenParticlesPDGId);
      myEvent->genParticlesMother    = *(pointers->pointerToGenParticlesMother);
      myEvent->pfCandidates          = *(pointers->pointerToPFCandidates);
      myEvent->pfCandidates_id       = *(pointers->pointerToPFCandidates_id);
      myEvent->pfCandidates_dz       = *(pointers->pointerToPFCandidates_dz);
      myEvent->pfCandidates_gsfPt    = *(pointers->pointerToPFCandidates_gsfPt);
      myEvent->pfCandidates_gsfdz    = *(pointers->pointerToPFCandidates_gsfdz);
      myEvent->pfCandidates_iso      = *(pointers->pointerToPFCandidates_iso);
      myEvent->pfCandidates_charge   = *(pointers->pointerToPFCandidates_charge);
      myEvent->tauCandidates         = *(pointers->pointerToTauCandidates);
      myEvent->tauCandidates_charge  = *(pointers->pointerToTauCandidates_charge);
      myEvent->tauCandidates_IDMVA2  = *(pointers->pointerToTauCandidates_IDMVA2);

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

    pointers->pointerToPFCandidates = 0;
    theTree->SetBranchAddress("pfCandidates",                                 &(pointers->pointerToPFCandidates));
    pointers->pointerToPFCandidates_id = 0;
    theTree->SetBranchAddress("pfCandidates_id",                              &(pointers->pointerToPFCandidates_id));
    pointers->pointerToPFCandidates_dz = 0;
    theTree->SetBranchAddress("pfCandidates_dz",                              &(pointers->pointerToPFCandidates_dz));
    pointers->pointerToPFCandidates_gsfPt = 0;
    theTree->SetBranchAddress("pfCandidates_gsfPt",                           &(pointers->pointerToPFCandidates_gsfPt));
    pointers->pointerToPFCandidates_gsfdz = 0;
    theTree->SetBranchAddress("pfCandidates_gsfdz",                           &(pointers->pointerToPFCandidates_gsfdz));
    pointers->pointerToPFCandidates_iso = 0;
    theTree->SetBranchAddress("pfCandidates_iso",                             &(pointers->pointerToPFCandidates_iso));
    pointers->pointerToPFCandidates_charge = 0;
    theTree->SetBranchAddress("pfCandidates_charge",                          &(pointers->pointerToPFCandidates_charge));

    pointers->pointerToTauCandidates = 0;
    theTree->SetBranchAddress("tauCandidates",                                &(pointers->pointerToTauCandidates));
    pointers->pointerToTauCandidates_charge = 0;
    theTree->SetBranchAddress("tauCandidates_charge",                         &(pointers->pointerToTauCandidates_charge));
    pointers->pointerToTauCandidates_IDMVA2 = 0;
    theTree->SetBranchAddress("tauCandidates_IDMVA2",                         &(pointers->pointerToTauCandidates_IDMVA2));

}
#endif


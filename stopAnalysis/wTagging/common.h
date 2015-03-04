#ifndef WTAGGING_COMMON
#define WTAGGING_COMMON

#include "../../common.h"
bool enableMTCut = true;
bool disableMTCut = false;

// ###############################
// #  Sonic screwdriver headers  #
// ###############################

#include "interface/Table.h"
#include "interface/SonicScrewdriver.h"
#include "interface/tables/TableBackgroundSignal.h"
#include "interface/tables/TableDataMC.h"
using namespace theDoctor;

#include <TCanvas.h>
#include <TH1F.h>

// ###################################
// #  BabyTuple format and location  #
// ###################################

#define FOLDER_BABYTUPLES "../../store/babyTuples_0603_withBDT_skim/1lepton4jetsMET80/"
#include "babyTupleReader.h"

#define BDT_OUTPUT_AVAILABLE
#define SECOND_LEPTON_IN_ACCEPTANCE_ALREADY_COMPUTED
#define ISR_JET_ALREADY_COMPUTED
#define LEADING_NON_B_PT_ALREADY_COMPUTED

// #################################################
// #  Analysis, cut-and-count and BDT definitions  #
// #################################################

#define MT_CUT    100
#define MET_CUT   80
#define NJET_CUT  4
#define NBJET_CUT 1
#define NLEP_CUT  1
#define MET_CUTLL 50

babyEvent myEvent;
string sampleName;
string sampleType;

bool goesInMTpeak()     { if ((myEvent.MT > 50) && (myEvent.MT < 80)) return true; else return false; }
bool goesInMTtail()     { if (myEvent.MT > MT_CUT)                    return true; else return false; }
bool goesInMTinverted() { if (myEvent.MT < MT_CUT)                    return true; else return false; }

bool goesInPreVetoSelection()
{
    if (myEvent.MET < MET_CUT) return false;
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    if (myEvent.nJets < NJET_CUT)  return false;
    if (myEvent.nBTag < NBJET_CUT)  return false;

    return true;
}

bool goesInPreselection()
{
    if (myEvent.MET < MET_CUT) return false;
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    if (myEvent.nJets < NJET_CUT)  return false;
    if (myEvent.nBTag < NBJET_CUT)  return false;
    if ((!myEvent.isolatedTrackVeto) || (!myEvent.tauVeto)) return false;

    return true;
}


#include "../../common/secondGeneratedLeptonType.h"

bool goesInSingleElecChannel()
{
    // Keep only events with numberOfLepton == 1
    if (myEvent.numberOfLepton != NLEP_CUT) return false;
    // For data, keep only events from SingleElec dataset that fired the trigger
    if (sampleType == "data")
    {
        if ((sampleName != "SingleElec") || (!myEvent.triggerElec)) return false;
    }

    // Remove electrons with pT < 30 GeV
    if (myEvent.leadingLepton.Pt() < 30)  return false;

    // Keep only events with an electron as leading lepton
    return (abs(myEvent.leadingLeptonPDGId) == 11);
}


bool goesInSingleMuonChannel()
{
    // Keep only events with numberOfLepton == 1
    if (myEvent.numberOfLepton != 1) return false;
    // For data, keep only events from SingleMuon dataset that fired the trigger
    if (sampleType == "data")
    {
        if ((sampleName != "SingleMuon") || ((!myEvent.triggerMuon) && (!myEvent.xtriggerMuon))) return false;

        // Take care of the splitting due to x-trigger
        if ((myEvent.leadingLepton.Pt() >= 26) && (!myEvent.triggerMuon))  return false;
        if ((myEvent.leadingLepton.Pt() <  26) && (!myEvent.xtriggerMuon)) return false;
    }

    // Keep only events with a muon as leading lepton
    return (abs(myEvent.leadingLeptonPDGId) == 13);
}


bool goesInSingleLeptonChannel()
{
    // Single-lepton channel is the union of e-channel + mu-channel
    bool test = (goesInSingleElecChannel()
              || goesInSingleMuonChannel());
    return test;
}


float getLumi()
{
         if (goesInSingleElecChannel())  return 19508.0;
    else if (goesInSingleMuonChannel())  return 19514.0;
    else                                 return 0.0;
}


float getWeight()
{
    float weight = 1.0;

    // Get the lumi
    float lumi = getLumi();

    // Normalize to cross section times lumi
    weight *= myEvent.weightCrossSection * lumi;

    // Apply trigger efficiency weights for singleLepton channels
    if (myEvent.numberOfLepton == 1)
    {
        weight *= myEvent.weightTriggerEfficiency;
    }

    // Apply lepton ID efficiency and isolation scale factor to singleLepton channels
    if (myEvent.numberOfLepton == 1)
    {
        weight *= myEvent.leadingLeptonIdEfficiency * myEvent.leadingLeptonIsoScaleFactor;
    }
    // TODO not sure about this, to be confirmed
    else if (myEvent.numberOfLepton == 2)
    {
        weight *= myEvent.leadingLeptonIdEfficiency * myEvent.leadingLeptonIsoScaleFactor;
        weight *= myEvent.secondLeptonIdEfficiency  * myEvent.secondLeptonIsoScaleFactor;
    }

    // Apply pile-up weight
    // TODO : Do we confirm we'll use also this for signal ?
        weight *= myEvent.weightPileUp;

    // For ttbar only, apply topPt reweighting
    if (sampleName.find("ttbar") != string::npos)
        weight *= myEvent.weightTopPt;

    return weight;
}


#endif

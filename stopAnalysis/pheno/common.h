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

#define FOLDER_BABYTUPLES "../../store/babyTuples_0603/"
#include "babyTupleReader.h"

#define BDT_OUTPUT_AVAILABLE
#define SECOND_LEPTON_IN_ACCEPTANCE_ALREADY_COMPUTED
#define ISR_JET_ALREADY_COMPUTED
#define LEADING_NON_B_PT_ALREADY_COMPUTED

// #################################################
// #  Analysis, cut-and-count and BDT definitions  #
// #################################################

#include "../../common/selectionDefinitions.h"
#include "../../common/cutAndCountDefinitions.h"
#include "../../common/signalRegionDefinitions.h"
#include "../../common/secondGeneratedLeptonType.h"

#endif

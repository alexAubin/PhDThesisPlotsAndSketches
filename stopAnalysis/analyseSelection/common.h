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

#include "../../common/selectionDefinitions.h"
#include "../../common/cutAndCountDefinitions.h"
#include "../../common/signalRegionDefinitions.h"
#include "../../common/secondGeneratedLeptonType.h"

void formatAndWriteEfficiencyMap(SonicScrewdriver* screwdriver, TH2F* theHisto, string name, string pathExport)
{
    float lineOffset = -999;
    string signalLabel = "";

    if (name == "T2tt"    ) { lineOffset = 172; signalLabel = "#tilde{t} #rightarrow t #tilde{#chi}^{0}";             }
    if (name == "T2bw-025") { lineOffset = 320; signalLabel = "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}, x = 0.25"; }
    if (name == "T2bw-050") { lineOffset = 160; signalLabel = "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}, x = 0.50"; }
    if (name == "T2bw-075") { lineOffset = 105; signalLabel = "#tilde{t} #rightarrow b #tilde{#chi}^{#pm}, x = 0.75"; }

    Plot thePlot(name,"custom",screwdriver->GetGlobalOptions());
    thePlot.SetParameter("name",name);
    thePlot.AddToInPlotInfo(signalLabel+";Preselection efficiency; ");

    Plot2D::ApplyAxisStyle(&thePlot,theHisto,"m_{#tilde{t}} [GeV]", "m_{#tilde{#chi}^{0}} [GeV]",screwdriver->GetGlobalOptions());
    PlotDefaultStyles::SetSmoothPalette("soft");
    thePlot.SetLogZ(false);
    thePlot.getCanvas()->SetRightMargin(0.1);
    theHisto->SetMaximum(0.14);
    theHisto->Draw("COLZ");
    PlotDefaultStyles::ApplyDefaultStylePalette(theHisto,thePlot.getCanvas());


    TLine* line1 = new TLine(lineOffset-12.5,-12.5,lineOffset+387.5,387.5);
    line1->SetLineWidth(2);
    line1->SetLineStyle(2);
    line1->Draw();
    thePlot.Write("./plots/"+name+"/","custom",screwdriver->GetGlobalOptions());
}


#endif

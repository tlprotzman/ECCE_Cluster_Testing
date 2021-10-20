#include <TROOT.h>

#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <TCanvas.h>

void drawAndSave(TObject *obj, const char *path, const char *drawOpts="",
                 bool logX=false, bool logY=false, bool logZ=false) {
    
    TCanvas *canvas = new TCanvas("", "", 1000, 1000);
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15);
    obj->Draw(drawOpts);
    if (logX) (canvas->SetLogz());
    if (logY) (canvas->SetLogy());
    if (logZ) (canvas->SetLogz());
    canvas->SaveAs(path);
}

int drawPlots(const char *inFile = "eemc_test.root") {
    TFile f = TFile(inFile, "READ");
    TH1 *eScale = (TH1*)f.Get("eScale");
    TH2 *eScaleEta = (TH2*)f.Get("eScaleEta");
    TH2 *eScaleGe = (TH2*)f.Get("eScaleGe");

    drawAndSave(eScale, "plots/eeemc/eScale.png");
    drawAndSave(eScaleEta, "plots/eeemc/eScaleEta.png", "colz", false, false, false);
    drawAndSave(eScaleGe, "plots/eeemc/eScaleGe.png", "colz", false, false, false);
    return 0;
}

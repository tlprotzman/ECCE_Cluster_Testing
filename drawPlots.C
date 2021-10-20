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

void draw2AndSave(TObject *obj1, TObject *obj2, const char *path, const char *drawOpts="",
                 bool logX=false, bool logY=false, bool logZ=false) {
    
    TCanvas *canvas = new TCanvas("", "", 2000, 1000);
    canvas->Divide(2, 1);
    gPad->SetMargin(0.15, 0.15, 0.15, 0.15);
    canvas->cd(1);
    obj1->Draw(drawOpts);
    if (logX) (canvas->SetLogz());
    if (logY) (canvas->SetLogy());
    if (logZ) (canvas->SetLogz());
    canvas->cd(2);
    obj2->Draw(drawOpts);
    if (logX) (canvas->SetLogz());
    if (logY) (canvas->SetLogy());
    if (logZ) (canvas->SetLogz());
    canvas->SaveAs(path);
}

int drawPlots(const char *detector) {
    const char *inFile = Form("%s_test.root", detector);
    TFile f = TFile(inFile, "READ");
    TH1 *eScale = (TH1*)f.Get("eScale");
    TH2 *eScaleEta = (TH2*)f.Get("eScaleEta");
    TH2 *eScaleGe = (TH2*)f.Get("eScaleGe");

    drawAndSave(eScale, Form("plots/%s/eScale.png", detector));
    drawAndSave(eScaleEta, Form("plots/%s/eScaleEta.png", detector), "colz", false, false, false);
    drawAndSave(eScaleGe, Form("plots/%s/eScaleGe.png", detector), "colz", false, false, false);
    draw2AndSave(eScale, eScaleGe, Form("plots/%s/eScaleSplit.png", detector), "colz", false, false, false);
    return 0;
}

#include "MCExerciseS2.h"
#include "MCMultiLingual.h"
#include "MCMultiView.h"
#include "MCVSDReader.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <TEveBrowser.h>
#include <TEveGValuators.h>
#include <TEveManager.h>
#include <TF1.h>
#include <TFile.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGMsgBox.h>
#include <TGDoubleSlider.h>
#include <TGTab.h>
#include <TGButton.h>
#include "TH1D.h"
#include "TLatex.h"
#include <TPad.h>
#include <TPRegexp.h>
#include <TRootBrowser.h>
#include <TStyle.h>

//=MCExerciseS2========================================================================
MCExerciseS2::MCExerciseS2(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
    fDataSetFileDir = "Data-Masterclass/events/Strangeness/Part2/";
    fDataSetFileName = fDataSetFileDir + "AliVSD_MasterClass_5.root";

    if (!TFile::Open(fDataSetFileName))
    {
        TString msgText(Form("Event file %s not found! \nCheck your installation", fDataSetFileName.Data()));
        new TGMsgBox(gClient->GetRoot(), 0, "MCExerciseS2:Data", msgText, kMBIconExclamation, kMBOk);
        gApplication->Terminate();
    }
    fVecListHistos.resize(fkNpart);
    fResults.resize(fkNpart);
    for (Int_t iPart = 0; iPart < fkNpart; iPart++)
    {
        fVecListHistos[iPart].resize(kNbSystemsAndEvtActivity);
        fResults[iPart].resize(kNbSystemsAndEvtActivity);
    }
    fVecListHistos[P_KAON][kpp_MB] = "K0s_pp_MB";
    fVecListHistos[P_KAON][kPbPb_MB] = "K0s_PbPb_MB";
    fVecListHistos[P_KAON][kPbPb_0010] = "K0s_PbPb_0010";
    fVecListHistos[P_KAON][kPbPb_1020] = "K0s_PbPb_1020";
    fVecListHistos[P_KAON][kPbPb_2030] = "K0s_PbPb_2030";
    fVecListHistos[P_KAON][kPbPb_3040] = "K0s_PbPb_3040";
    fVecListHistos[P_KAON][kPbPb_4050] = "K0s_PbPb_4050";
    fVecListHistos[P_KAON][kPbPb_5060] = "K0s_PbPb_5060";
    fVecListHistos[P_KAON][kPbPb_6070] = "K0s_PbPb_6070";
    fVecListHistos[P_KAON][kPbPb_7080] = "K0s_PbPb_7080";
    fVecListHistos[P_KAON][kPbPb_8090] = "K0s_PbPb_8090";
    fVecListHistos[P_KAON][kPbPb_90100] = "K0s_PbPb_90100";
    fVecListHistos[P_LAMBDA][kpp_MB] = "Lambda_pp_MB";
    fVecListHistos[P_LAMBDA][kPbPb_MB] = "Lambda_PbPb_MB";
    fVecListHistos[P_LAMBDA][kPbPb_0010] = "Lambda_PbPb_0010";
    fVecListHistos[P_LAMBDA][kPbPb_1020] = "Lambda_PbPb_1020";
    fVecListHistos[P_LAMBDA][kPbPb_2030] = "Lambda_PbPb_2030";
    fVecListHistos[P_LAMBDA][kPbPb_3040] = "Lambda_PbPb_3040";
    fVecListHistos[P_LAMBDA][kPbPb_4050] = "Lambda_PbPb_4050";
    fVecListHistos[P_LAMBDA][kPbPb_5060] = "Lambda_PbPb_5060";
    fVecListHistos[P_LAMBDA][kPbPb_6070] = "Lambda_PbPb_6070";
    fVecListHistos[P_LAMBDA][kPbPb_7080] = "Lambda_PbPb_7080";
    fVecListHistos[P_LAMBDA][kPbPb_8090] = "Lambda_PbPb_8090";
    fVecListHistos[P_LAMBDA][kPbPb_90100] = "Lambda_PbPb_90100";
    fVecListHistos[P_ALAMBDA][kpp_MB] = "AntiLambda_pp_MB";
    fVecListHistos[P_ALAMBDA][kPbPb_MB] = "AntiLambda_PbPb_MB";
    fVecListHistos[P_ALAMBDA][kPbPb_0010] = "AntiLambda_PbPb_0010";
    fVecListHistos[P_ALAMBDA][kPbPb_1020] = "AntiLambda_PbPb_1020";
    fVecListHistos[P_ALAMBDA][kPbPb_2030] = "AntiLambda_PbPb_2030";
    fVecListHistos[P_ALAMBDA][kPbPb_3040] = "AntiLambda_PbPb_3040";
    fVecListHistos[P_ALAMBDA][kPbPb_4050] = "AntiLambda_PbPb_4050";
    fVecListHistos[P_ALAMBDA][kPbPb_5060] = "AntiLambda_PbPb_5060";
    fVecListHistos[P_ALAMBDA][kPbPb_6070] = "AntiLambda_PbPb_6070";
    fVecListHistos[P_ALAMBDA][kPbPb_7080] = "AntiLambda_PbPb_7080";
    fVecListHistos[P_ALAMBDA][kPbPb_8090] = "AntiLambda_PbPb_8090";
    fVecListHistos[P_ALAMBDA][kPbPb_90100] = "AntiLambda_PbPb_90100";

    fSysActName[kpp_MB] = "pp MB";
    fSysActName[kPbPb_MB] = "PbPb MB";
    fSysActName[kPbPb_0010] = "PbPb 0-10\%";
    fSysActName[kPbPb_1020] = "PbPb 10-20\%";
    fSysActName[kPbPb_2030] = "PbPb 20-30\%";
    fSysActName[kPbPb_3040] = "PbPb 30-40\%";
    fSysActName[kPbPb_4050] = "PbPb 40-50\%";
    fSysActName[kPbPb_5060] = "PbPb 50-60\%";
    fSysActName[kPbPb_6070] = "PbPb 60-70\%";
    fSysActName[kPbPb_7080] = "PbPb 70-80\%";
    fSysActName[kPbPb_8090] = "PbPb 80-90\%";
    fSysActName[kPbPb_90100] = "PbPb 90-100\%";

    Start(RO_NONE);
}

//=MCExerciseS2========================================================================
MCExerciseS2::~MCExerciseS2()
{
    delete fMinvHisto;
    delete fFitGauss;
    delete fLabel;
    if (!fNeedsInit)
        DeleteHistograms();
    fSMF->UnmapWindow();
    if (fMF)
        fMF->CloseWindow();
    if (fLabel)
        delete fLabel;
    if (fFitGauss)
        delete fFitGauss;
    if (fFitPoly)
        delete fFitPoly;
}

//=MCExerciseS2========================================================================
void MCExerciseS2::CustomizeBrowser()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    TEveBrowser *browser = gEve->GetBrowser();
    browser->GetMainFrame()->SetWindowName(ml.tr("Student"));
    browser->GetMainFrame()->Move(500, 0);

    browser->StartEmbedding(TRootBrowser::kLeft);

    fSMF = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    fSMF->SetWindowName("XX GUI");
    fSMF->SetCleanup(kDeepCleanup);

    //-------------------New GUI-------------------------
    // tab widget to select histograms to be fitted

    TGTab *lTabsV0s = new TGTab(fSMF, 128, 280);
    TGCompositeFrame *lCompositeFrame[fkNpart];
    // container of "Tab1"
    lCompositeFrame[P_KAON] = lTabsV0s->AddTab("K0s");
    lCompositeFrame[P_KAON]->SetLayoutManager(new TGVerticalLayout(lCompositeFrame[P_KAON]));

    // container of "Tab2"
    lCompositeFrame[P_LAMBDA] = lTabsV0s->AddTab("Lambda");
    lCompositeFrame[P_LAMBDA]->SetLayoutManager(new TGVerticalLayout(lCompositeFrame[P_LAMBDA]));

    // container of "Tab3"
    lCompositeFrame[P_ALAMBDA] = lTabsV0s->AddTab("AntiLambda");
    lCompositeFrame[P_ALAMBDA]->SetLayoutManager(new TGVerticalLayout(lCompositeFrame[P_ALAMBDA]));

    for (Short_t iPart = 0; iPart < fkNpart; iPart++)
    {
        for (Int_t iHist = 0; iHist < kNbSystemsAndEvtActivity; iHist++)
        {
            TString lStrID(fVecListHistos[iPart][iHist].Data());
            TString lStrIDLabel = FormatText(lStrID, kFALSE);
            fTabCheckButton[lStrID.Data()] = new TGCheckButton(lCompositeFrame[iPart], lStrIDLabel.Data(), iPart * 100 + iHist);
            fTabCheckButton[lStrID.Data()]->Connect("Clicked()", "MCExerciseS2", this, TString::Format("LoadHisto(=%d,%d)", iPart, iHist));
            fTabCheckButton[lStrID.Data()]->SetTextJustify(36);
            fTabCheckButton[lStrID.Data()]->SetMargins(0, 0, 0, 0);
            fTabCheckButton[lStrID.Data()]->SetWrapLength(-1);
            lCompositeFrame[iPart]->AddFrame(fTabCheckButton[lStrID.Data()], new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2));
        }
    }
    lTabsV0s->SetTab(1);
    lTabsV0s->Resize(lTabsV0s->GetDefaultSize());
    fSMF->AddFrame(lTabsV0s, new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2));
    lTabsV0s->MoveResize(8, 8, 128, 280);
    fSMF->SetMWMHints(kMWMDecorAll, kMWMFuncAll, kMWMInputModeless);
    fSMF->MapSubwindows();
    fSMF->Resize(fSMF->GetDefaultSize());
    fSMF->MapWindow();

    TGGroupFrame *gf = new TGGroupFrame(fSMF, ml.tr("Fit"));
    TGVerticalFrame *vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
    {
        TGHorizontalFrame *hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

        hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        {
            fFitRangePoly = new TEveGDoubleValuator(hf, ml.tr("Bckg range"), 40, 0);
            fFitRangePoly->SetNELength(5);
            fFitRangePoly->SetLabelWidth(80);
            fFitRangePoly->Build();
            fFitRangePoly->GetSlider()->SetWidth(190);
            fFitRangePoly->SetLimits(0.25, 2.0, TGNumberFormat::kNESRealThree);
            fFitRangePoly->SetValues(0.25, 2.0, TGNumberFormat::kNESRealThree);
            hf->AddFrame(fFitRangePoly, new TGLayoutHints(kLHintsExpandX));
        }
        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 4, 4, 4, 4));

        hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        {
            fFitRangeGauss = new TEveGDoubleValuator(hf, ml.tr("Peak range"), 40, 0);
            fFitRangeGauss->SetNELength(5);
            fFitRangeGauss->SetLabelWidth(80);
            fFitRangeGauss->Build();
            fFitRangeGauss->GetSlider()->SetWidth(190);
            fFitRangeGauss->SetLimits(0.35, 1.5, TGNumberFormat::kNESRealThree);
            fFitRangeGauss->SetValues(0.35, 1.5, TGNumberFormat::kNESRealThree);
            hf->AddFrame(fFitRangeGauss, new TGLayoutHints(kLHintsExpandX));
        }
        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 4, 4, 4, 4));

        hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        {
            TGTextButton *b = new TGTextButton(hf, ml.tr("Fit peak+bckg"));
            b->Connect("Clicked()", "MCExerciseS2", this, "FitInvariantMass()");
            hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
            TGTextButton *bs = new TGTextButton(hf, ml.tr("Save"));
            bs->Connect("Clicked()", "MCExerciseS2", this, "SaveResult()");
            hf->AddFrame(bs, new TGLayoutHints(kLHintsExpandX));
        }
        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));
    }
    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(gf);

    gf = new TGGroupFrame(fSMF, ml.tr("Table of Results"));
    vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
    TGTextButton *b = new TGTextButton(vf, ml.tr("Display"));
    b->Connect("Clicked()", "MCExerciseS2", this, "DisplayResults()");
    vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(gf),

        fSMF->MapSubwindows();
    fSMF->Resize();
    fSMF->MapWindow();

    browser->StopEmbedding(" ");

    if (browser->GetTabRight()->GetTabTab(2))
        browser->GetTabRight()->RemoveTab(2);

    browser->StartEmbedding(TRootBrowser::kRight);
    fPad = new TCanvas();
    browser->StopEmbedding(Form("%s S2", ml.tr("Mass")));

    browser->GetTabRight()->SetTab(2);
}

//=MCExerciseS2========================================================================
void MCExerciseS2::DeleteHistograms()
{
    delete fMinvHistK;
    delete fMinvHistX;
    delete fMinvHistL;
    delete fMinvHistAL;
    delete fMinvHistALL;
}

//=MCExerciseS2========================================================================
void MCExerciseS2::DisplayResults()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    if (fMF)
        fMF->CloseWindow();
    fMF = new TGMainFrame(gClient->GetRoot(), 2000, 50);
    fMF->DontCallClose();
    for (Int_t ipart = 0; ipart < fkNpart; ipart++)
    {
        TGGroupFrame *gf = new TGGroupFrame(fMF, fPartName[ipart]);
        TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 2000, 50);
        for (Int_t ihist = kNbSystemsAndEvtActivity - 1; ihist >= 0; ihist--)
        {
            TGVerticalFrame *vf = new TGVerticalFrame(hf, 20, 20);
            TGLabel *label = new TGLabel(vf, fSysActName[ihist]);
            vf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            label = new TGLabel(vf, TString::Format("%6i", fResults[ipart][ihist]));
            label->SetTextColor(kGreen);
            vf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            hf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsRight));
        }
        gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        fMF->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
    }

    TGHorizontalFrame *hf = new TGHorizontalFrame(fMF, 200, 50);
    TGTextButton *btext = new TGTextButton(hf, ml.tr("Refresh"));
    btext->Connect("Clicked()", "MCExerciseS2", this, "RefreshResults()");
    hf->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    btext = new TGTextButton(hf, ml.tr("Close"));
    btext->Connect("Clicked()", "TGMainFrame", fMF, "UnmapWindow()");
    hf->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    fMF->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 100, 100, 20, 20));
    fMF->SetWindowName(ml.tr("Table of Results"));
    fMF->MapSubwindows();
    fMF->Resize(fMF->GetDefaultSize());
    fMF->MapWindow();
}

//=MCExerciseS2========================================================================
void MCExerciseS2::FitInvariantMass()
{
    Double_t minRange = TMath::Min(fFitRangePoly->GetMin(), fFitRangeGauss->GetMin());
    Double_t maxRange = TMath::Max(fFitRangePoly->GetMax(), fFitRangeGauss->GetMax());

    if (!fFitGauss)
        fFitGauss = new TF1("fitGauss", "[0]*TMath::Gaus(x,[1],[2])+[3]*x*x+[4]*x+[5]", minRange, maxRange);
    else
        fFitGauss->SetRange(minRange, maxRange);
    fFitGauss->SetParameter(0, 80.);
    fFitGauss->SetParameter(1, 0.5 * (fFitRangeGauss->GetMin() + fFitRangeGauss->GetMax()));
    fFitGauss->SetParameter(2, 1.);
    fFitGauss->SetParLimits(0, 0., TMath::Power(10., 9));
    fFitGauss->SetParLimits(1, fFitRangeGauss->GetMin(), fFitRangeGauss->GetMax());
    fFitGauss->SetParLimits(2, 0., (fFitRangeGauss->GetMax() - fFitRangeGauss->GetMin()) / 2.);
    fFitGauss->SetLineColor(kGreen + 1);
    fMinvHisto->Fit(fFitGauss, "rime");

    if (fLabel)
    {
        delete fLabel;
        fMinvHisto->Draw();
    }
    fLabel = new TLatex();
    fLabel->SetTextSize(0.045);
    fLabel->SetTextColor(kBlack);
    fLabel->SetNDC(kTRUE);
    Double_t labelX = 0.5;
    Double_t labelY = 0.75;
    Double_t labelYStep = 0.06;

    Int_t total_Fit = (Int_t)(fFitGauss->Integral(fFitRangeGauss->GetMin(), fFitRangeGauss->GetMax()) / fMinvHisto->GetBinWidth(1));

    Double_t maxMmin = fFitRangeGauss->GetMax() - fFitRangeGauss->GetMin();
    Double_t maxPmin = fFitRangeGauss->GetMax() + fFitRangeGauss->GetMin();
    Double_t background_Integral = maxMmin * (fFitGauss->GetParameter(5) + fFitGauss->GetParameter(4) * maxPmin / 2.) +
                                   (fFitGauss->GetParameter(3) * (TMath::Power(fFitRangeGauss->GetMax(), 3) - TMath::Power(fFitRangeGauss->GetMin(), 3)) / 3.);
    Int_t bck_Fit = (Int_t)(background_Integral / fMinvHisto->GetBinWidth(1));
    fFitResult = total_Fit - bck_Fit;
    fLabel->SetTextSize(0.03);
    fLabel->DrawLatex(labelX, labelY, Form("Total = %d", total_Fit));
    fLabel->DrawLatex(labelX, labelY - 1.0 * labelYStep, Form("%s = %d", MCMultiLingual::Instance().tr("Background"), bck_Fit));
    fLabel->DrawLatex(labelX, labelY - 2.0 * labelYStep, Form("#color[%d]{#rightarrow Signal: %d}", kGreen + 1, fFitResult));
    fLabel->DrawLatex(labelX, labelY - 3.0 * labelYStep, Form("#mu_{Gauss} : %6.4f #pm %6.4f GeV/c^{2}", fFitGauss->GetParameter(1), fFitGauss->GetParError(1)));
    fLabel->SetTextSize(0.03);
    // fLabel->DrawLatex(labelX, labelY - 4.0 * labelYStep, Form("     #color[%d]{ NOTE : %s}", kGray + 1, fMassInfo.Data()));
    fLabel->DrawLatex(labelX, labelY - 4.0 * labelYStep, Form("#sigma_{Gauss} : %5.3f #pm %5.3f  MeV/c^{2}", fFitGauss->GetParameter(2) * 1000, fFitGauss->GetParError(1) * 1000));

    if (!fFitPoly)
        fFitPoly = new TF1("fitPoly", "[0]*x*x+[1]*x+[2]", minRange, maxRange);
    else
        fFitPoly->SetRange(minRange, maxRange);
    fFitPoly->SetParameter(0, fFitGauss->GetParameter(3));
    fFitPoly->SetParameter(1, fFitGauss->GetParameter(4));
    fFitPoly->SetParameter(2, fFitGauss->GetParameter(5));
    fFitPoly->SetLineColor(kBlue + 1);
    fFitPoly->Draw("SAME");

    TLatex *lLaTeX_Mass = new TLatex();
    lLaTeX_Mass->SetTextSize(0.03);
    lLaTeX_Mass->SetTextColor(kGray + 2);
    lLaTeX_Mass->DrawLatexNDC(0.59, 0.85, fMassInfo.Data());

    fPad->Update();
}

//=MCExerciseS2========================================================================
TString MCExerciseS2::FormatText(const TString &text, Bool_t tit)
{
    TString rv(text);
    if (tit)
    {
        rv.ReplaceAll("K0s_", "[ K^{0}s, ");               // K0s_pp_MB  --> K0s, pp_MB
        rv.ReplaceAll("AntiLambda_", "[ #bar{#Lambda}, "); // NOTE : replace first AntiLambda over Lambda ... to avoid undesirable replacement
        rv.ReplaceAll("Lambda_", "[ #Lambda, ");
    }
    else
    {
        rv.ReplaceAll("K0s_", "K0s, ");           // K0s_pp_MB  --> K0s, pp_MB
        rv.ReplaceAll("AntiLambda_", "anti-L, "); // NOTE : replace first AntiLambda over Lambda ... to avoid undesirable replacement
        rv.ReplaceAll("Lambda_", "L, ");
    }
    rv.ReplaceAll("_", " "); // K0s, pp_MB --> K0s, pp MB
    rv.ReplaceAll("pp", "p-p");
    rv.ReplaceAll("PbPb", "Pb-Pb");
    TPRegexp lRegex("([0-9]{2})([0-9]{2})"); // regex pattern : locate 2 groups of 2 numbers 0010 -> (00)(10)
    lRegex.Substitute(rv, "$1-$2");          // add an hyphen in between : 00-10
    if (!rv.Contains("MB"))
        rv.Append("% ]"); // for Pb-Pb centrality
    else
        rv.Append(" ]");
    return rv;
}

//=MCExerciseS2========================================================================
void MCExerciseS2::LoadHisto(Int_t iPart, Int_t iHist)
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    fCurrentPart = iPart;
    fCurrentAct = iHist;
    UntickButtons();
    TString lStrID(fVecListHistos[iPart][iHist].Data());
    fTabCheckButton[lStrID.Data()]->SetOn(kTRUE);
    TString lHistTitle(ml.tr("Invariant Mass")), lXTitle(""), lYTitle("");
    if (fMinvHisto)
        delete fMinvHisto;
    fMinvHisto = new TH1D("MinvHisto", ml.tr("Invariant Mass"), 2000, 0., 2.);
    fMinvHisto->Rebin(2);
    fMinvHisto->SetStats(kFALSE);
    fMinvHisto->SetLineWidth(2);
    lXTitle = ml.tr("Invariant Mass (GeV/c^{2})");
    lYTitle = TString::Format("%s %4.2f MeV/c^{2}",
                              ml.tr("Counts per bin of "),
                              fMinvHisto->GetXaxis()->GetBinWidth(1) * 1000.);
    fCurrentLabel = FormatText(lStrID);
    lHistTitle = lHistTitle + fCurrentLabel;

    Double_t lZoomXmin = 0.0;
    Double_t lZoomXmax = 2.0;
    Color_t lColor = 1;
    if (lStrID.BeginsWith("K0s"))
    {
        lZoomXmin = 0.25;
        lZoomXmax = 1.0;
        lXTitle = lXTitle + " m(#pi^{+}#pi^{-}) (GeV/c^{2})";
        lColor = kAzure + 2;
        fMassInfo = "m_{PDG}( K^{0}s ) #approx 0.498 GeV/c^{2}";
    }
    else if (lStrID.BeginsWith("Lambda"))
    {
        lZoomXmin = 1.0;
        lZoomXmax = 2.0;
        lXTitle = lXTitle + " m(p^{+}#pi^{-}) (GeV/c^{2})";
        lColor = kRed + 1;
        fMassInfo = "m_{PDG}( #Lambda ) #approx 1.116 GeV/c^{2}";
    }
    else if (lStrID.BeginsWith("AntiLambda"))
    {
        lZoomXmin = 1.0;
        lZoomXmax = 2.0;
        lXTitle = lXTitle + " m(#bar{p}^{#kern[+0.2]{-}}#pi^{+}) (GeV/c^{2})";
        lColor = kOrange + 1;
        fMassInfo = "m_{PDG}( #bar{#Lambda} ) #approx 1.116 GeV/c^{2}";
    }
    fMinvHisto->SetTitle(lHistTitle.Data());
    fMinvHisto->GetXaxis()->SetTitle(lXTitle.Data());
    fMinvHisto->GetYaxis()->SetTitle(lYTitle.Data());
    fMinvHisto->GetXaxis()->SetRangeUser(lZoomXmin, lZoomXmax);
    fMinvHisto->SetFillColor(0);
    fMinvHisto->SetLineColor(lColor);

    ReadHistoFile(TString::Format("dataset_%s.txt", lStrID.Data()));
    fMinvHisto->Draw();

    TLatex *lLaTeX_Mass = new TLatex();
    lLaTeX_Mass->SetTextSize(0.03);
    lLaTeX_Mass->SetTextColor(kGray + 2);
    lLaTeX_Mass->DrawLatexNDC(0.59, 0.85, fMassInfo.Data());

    fPad->Update();
}

//=MCExerciseS1========================================================================
void MCExerciseS2::MakeHistograms()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    gStyle->GetOptStat();
    gStyle->SetOptFit(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetPalette(1);
    if (!fMinvHistK)
    {
        // Kaons
        fMinvHistK = new TH1D("hMinvHistK", "Kaons", 50, 0.4, 0.6);
        fMinvHistK->SetLineColor(2);
        fMinvHistK->GetXaxis()->SetTitle(ml.tr("Invariant Mass (GeV/c^{2})"));
        fMinvHistK->GetYaxis()->SetTitle(ml.tr("Momentum"));
        fMinvHistK->SetLineColor(2);
        fMinvHistK->SetFillColor(0);

        // Xi
        fMinvHistX = new TH1D("hMinvHistX", "Xis", 50, 1.2, 1.4);
        fMinvHistX->SetLineColor(2);
        fMinvHistX->GetXaxis()->SetTitle(ml.tr("Invariant Mass (GeV/c^{2})"));
        fMinvHistX->GetYaxis()->SetTitle(ml.tr("Momentum"));
        fMinvHistX->SetLineColor(3);
        fMinvHistX->SetFillColor(0);

        // Lambda
        fMinvHistL = new TH1D("hMinvHistL", "Lambdas", 50, 1.0, 1.2);
        fMinvHistL->SetLineColor(2);
        fMinvHistL->GetXaxis()->SetTitle(ml.tr("Invariant Mass (GeV/c^{2})"));
        fMinvHistL->GetYaxis()->SetTitle(ml.tr("Momentum"));
        fMinvHistL->SetLineColor(4);
        fMinvHistL->SetFillColor(0);

        // Anti-Lambda
        fMinvHistAL = new TH1D("hMinvHistAL", "Anti-Lambdas", 50, 1.0, 1.2);
        fMinvHistAL->SetLineColor(2);
        fMinvHistAL->GetXaxis()->SetTitle(ml.tr("Invariant Mass (GeV/c^{2})"));
        fMinvHistAL->GetYaxis()->SetTitle(ml.tr("Momentum"));
        fMinvHistAL->SetLineColor(9);
        fMinvHistAL->SetFillColor(0);

        // All
        fMinvHistALL = new TH1D("hMinvHistALL", ml.tr("Invariant Mass - final result"), 20, 0.0, 2.0);
        fMinvHistALL->GetXaxis()->SetTitle(ml.tr("Invariant Mass (GeV/c^{2})"));
        fMinvHistALL->GetYaxis()->SetTitle(ml.tr("Momentum"));
        fMinvHistALL->SetFillColor(0);
    }
    else
    {
        fMinvHistK->Reset();
        fMinvHistX->Reset();
        fMinvHistL->Reset();
        fMinvHistAL->Reset();
        fMinvHistALL->Reset();
    }
}

//=MCExerciseS2========================================================================
void MCExerciseS2::ReadHistoFile(const char *fileName)
{
    TString fn(fDataSetFileDir + fileName + "?filetype=raw");
    TFile *file = nullptr;
    if (!(file = TFile::Open(fn)))
    {
        TString msgText(Form("Event file %s not found! \nCheck your installation", fn.Data()));
        new TGMsgBox(gClient->GetRoot(), 0, "MCExerciseS1:Data", msgText, kMBIconExclamation, kMBOk);
        gApplication->Terminate();
    }
    Long64_t buffSize = 1024;
    Long64_t fileSize = file->GetSize();
    if (fileSize < buffSize)
        buffSize = fileSize;
    char *cbuf = new char[buffSize];
    while (buffSize && !file->ReadBuffer(cbuf, buffSize))
    {
        TString temp;
        for (Int_t index = 0; index < buffSize; index++)
        {
            if (cbuf[index] != '\n')
                temp.Append(cbuf[index]);
            else
            {
                fMinvHisto->Fill(temp.Atof());
                temp.Clear();
            }
            Long64_t bytesLeft = fileSize - file->GetBytesRead();
            if (bytesLeft < buffSize)
                buffSize = (int)bytesLeft;
        }
    }
    file->Close();
    delete[] cbuf;
}

//=MCExerciseS2========================================================================
void MCExerciseS2::RefreshResults()
{
    fMF->UnmapWindow();
    DisplayResults();
}

//=MCExerciseS2========================================================================
void MCExerciseS2::SaveResult()
{
    fResults[fCurrentPart][fCurrentAct] = fFitResult;
    if (fMF)
        RefreshResults();
}

//=MCExerciseS2========================================================================
void MCExerciseS2::StartExercise()
{
    if (fNeedsInit)
    {
        fNeedsInit = kFALSE;
        InitViewer(kTRUE);
        MakeHistograms();
    }
    CustomizeBrowser();
}

//=MCExerciseS2========================================================================
void MCExerciseS2::UntickButtons()
{
    for (Short_t iPart = 0; iPart < fkNpart; iPart++)
    {
        for (Int_t iHist = 0; iHist < kNbSystemsAndEvtActivity; iHist++)
        {
            TString lStrID(fVecListHistos[iPart][iHist].Data());
            fTabCheckButton[lStrID.Data()]->SetOn(kFALSE);
        }
    }
}

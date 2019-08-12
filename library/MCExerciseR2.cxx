#include "MCExerciseR2.h"
#include "MCMultiLingual.h"

#include <TCanvas.h>
#include <TEveManager.h>
#include <TFile.h>
#include <TGaxis.h>
#include <TEveBrowser.h>
#include <TGButton.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGMsgBox.h>
#include <TGNumberEntry.h>
#include <TGProgressBar.h>
#include <TGTab.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TLegend.h>
#include <TPaveLabel.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TTree.h>

//=MCExerciseR2========================================================================
MCExerciseR2::MCExerciseR2(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
    fDataSetFileDir = "Data-Masterclass/events/RAA/Part2/";
    fCollSystem = "PbPb";
    fDataSetFileName = fDataSetFileDir + "MasterClassesTree_LHC10h_Run139036.root";
    fGuiLogoName = "images/RAA.png";
    MakeGui(kFALSE);
}

//=MCExerciseR2========================================================================
MCExerciseR2::~MCExerciseR2()
{
    DeleteHistograms();
}

//=MCExerciseR2========================================================================
void MCExerciseR2::ContinueExercise()
{
    //called by SetCentralityBin
    // fCentralitySelector->UnmapWindow();

    if (fRole == RO_STUDENT)
    {
        DemoSet();
    }
    else if (fRole == RO_DEMO)
    {
        StudentSet();
    }
}

//=MCExerciseR2========================================================================
void MCExerciseR2::DeleteHistograms()
{
    for (Int_t centbin = 0; centbin < kNBCENTBINS; centbin++)
    {
        delete fHNTPC[centbin];
        delete fHTrackPt[centbin];
    }
    delete fHNTPCvsCent;
    delete fHDeltaPt;
}

//=MCExerciseR2========================================================================
void MCExerciseR2::DemoSet()
{
    StudentSet();
}

//=MCExerciseR2========================================================================
void MCExerciseR2::MakeHistograms()
{
    //*********************************************************************************
    // Defintion of histograms:
    // * to be filled with trackpt and number of charged tracks in the TPC (TH1*)
    // * correlation between centrality an nTracks TPC (TH2F)
    //*********************************************************************************

    MCMultiLingual &ml = MCMultiLingual::Instance();
    for (Int_t centbin = 0; centbin < kNBCENTBINS; centbin++)
    {
        TString name = Form("hNTPC_%i-%i", fCentCheckValL[centbin], fCentCheckValH[centbin]);
        TString title = Form("%s %i-%i", ml.tr("Number of TPC Tracks"),
                             fCentCheckValL[centbin], fCentCheckValH[centbin]);
        fHNTPC[centbin] = new TH1F(name, title, 200, 0, 2000);
        // fHNTPC[centbin]->GetYaxis()->SetTitleOffset(1.3);
        fHNTPC[centbin]->SetXTitle(ml.tr("Number of TPC Tracks"));
        fHNTPC[centbin]->SetYTitle(ml.tr("Counts/evt"));

        name = Form("hTrackPt_%i-%i", fCentCheckValL[centbin], fCentCheckValH[centbin]);
        title = Form("%s pT%i-%i", ml.tr("Tracks"),
                     fCentCheckValL[centbin], fCentCheckValH[centbin]);
        fHTrackPt[centbin] = new TH1D(name, "track pt", fNBinsPt, fBinsPt);
        fHTrackPt[centbin]->SetXTitle("pt (GeV/c)");
        fHTrackPt[centbin]->SetYTitle("N/tracks");
        // fHTrackPt[centbin]->GetYaxis()->SetTitleOffset(1.3);
    }
    fHNTPCvsCent = new TH2F("hNTPCvsCent", ml.tr("Number of TPC Tracks"), 200, 0, 2000, 100, 0, 100);
    fHNTPCvsCent->SetXTitle(ml.tr("Number of TPC Tracks"));
    fHNTPCvsCent->SetYTitle(ml.tr("Centrality"));
    // fHNTPCvsCent->GetYaxis()->SetTitleOffset(1.3);

    //*********************************************************************************
    // Definition of correction histogram in order to normalise for the binwidth in
    // Pt
    //*********************************************************************************
    fHDeltaPt = new TH1D("deltaPt", "", fNBinsPt, fBinsPt);
    for (Int_t iPt = 1; iPt < fNBinsPt + 1; iPt++)
    {
        fHDeltaPt->SetBinContent(iPt, fBinsPt[iPt] - fBinsPt[iPt - 1]);
        fHDeltaPt->SetBinError(iPt, 0);
    }
}

//=MCExerciseR2========================================================================
void MCExerciseR2::MakeRAA()
{
    // Attaching and Reading pp-reference file
    TFile *fileInputPP = new TFile(fDataSetFileDir + "PP_2760GeV_BaseLine.root");

    TH1D *hTrackPtpp = (TH1D *)fileInputPP->Get("trackPt_pp");
    SetHistoMarkerAndColor(hTrackPtpp, 20, 0.1, kBlue, kBlue);
    hTrackPtpp->SetStats(kFALSE);
    hTrackPtpp->SetXTitle("pt (GeV/c)");

    TLegend *legendTrackPt = new TLegend(0.55, 0.70, 0.95, 0.9);
    legendTrackPt->SetTextSize(0.03);
    legendTrackPt->SetFillColor(0);
    legendTrackPt->SetLineColor(0);
    legendTrackPt->SetNColumns(2);
    legendTrackPt->AddEntry(hTrackPtpp, "pp");

    fPad->cd(1)->SetLogy(1);
    hTrackPtpp->Draw("e1");
    for (Int_t cenbin = 0; cenbin < kNBCENTBINS - 1; cenbin++)
    {
        legendTrackPt->AddEntry(fHTrackPt[cenbin], Form("PbPb %i-%i %%", fCentCheckValL[cenbin], fCentCheckValH[cenbin]));
        SetHistoMarkerAndColor(fHTrackPt[cenbin], 20, 0.1, kRed - cenbin, kRed - cenbin);
        fHTrackPt[cenbin]->SetStats(kFALSE);
        fHTrackPt[cenbin]->Draw("same, e1");
    }
    legendTrackPt->Draw();
    fPad->cd(1)->Update();

    TH1F *hNTracksTPCpp = (TH1F *)fileInputPP->Get("nTracksTPC_pp");
    SetHistoMarkerAndColor(hNTracksTPCpp, 20, 0.1, kBlue, kBlue);
    hNTracksTPCpp->SetStats(kFALSE);

    TLegend *legendTrackTPC = new TLegend(0.55, 0.70, 0.95, 0.9);
    legendTrackTPC->SetTextSize(0.03);
    legendTrackTPC->SetFillColor(0);
    legendTrackTPC->SetLineColor(0);
    legendTrackTPC->SetNColumns(2);
    legendTrackTPC->AddEntry(hNTracksTPCpp, "pp");

    fPad->cd(2)->SetLogy(1);
    hNTracksTPCpp->Draw("e1");
    for (Int_t cenbin = 0; cenbin < kNBCENTBINS - 1; cenbin++)
    {
        legendTrackTPC->AddEntry(fHNTPC[cenbin], Form("PbPb %i-%i %%", fCentCheckValL[cenbin], fCentCheckValH[cenbin]));
        SetHistoMarkerAndColor(fHNTPC[cenbin], 20, 0.1, kRed - cenbin, kRed - cenbin);
        fHNTPC[cenbin]->SetStats(kFALSE);
        fHNTPC[cenbin]->Draw("same, e1");
    }
    legendTrackTPC->Draw();
    fPad->cd(2)->Update();

    TLegend *legendRAA = new TLegend(0.55, 0.70, 0.95, 0.9);
    legendTrackTPC->SetTextSize(0.03);
    legendTrackTPC->SetFillColor(0);
    legendTrackTPC->SetLineColor(0);
    legendTrackTPC->SetNColumns(2);

    fPad->cd(3);
    TH1D *hRAA[kNBCENTBINS - 1];
    hRAA[kNBCENTBINS - 2] = (TH1D *)fHTrackPt[0]->Clone(Form("RAA_vs_pt_%i-%i", fCentCheckValL[kNBCENTBINS - 2], fCentCheckValH[kNBCENTBINS - 2]));
    hRAA[kNBCENTBINS - 2]->SetYTitle("R_{AA}");
    hRAA[kNBCENTBINS - 2]->Divide(hTrackPtpp);
    hRAA[kNBCENTBINS - 2]->SetMaximum(2.0);
    hRAA[kNBCENTBINS - 2]->Draw("e1");
    for (Int_t cenbin = 1; cenbin < kNBCENTBINS - 2; cenbin++)
    {
        legendRAA->AddEntry(fHNTPC[cenbin], Form("PbPb %i-%i %%", fCentCheckValL[cenbin], fCentCheckValH[cenbin]));
        hRAA[cenbin] = (TH1D *)fHTrackPt[cenbin]->Clone(Form("RAA_vs_pt_%i-%i", fCentCheckValL[cenbin], fCentCheckValH[cenbin]));
        hRAA[cenbin]->SetYTitle("R_{AA}");
        hRAA[cenbin]->Divide(hTrackPtpp);
        hRAA[cenbin]->Draw("same, e1");
    }
    legendRAA->AddEntry(fHNTPC[kNBCENTBINS - 2], Form("PbPb %i-%i %%", fCentCheckValL[kNBCENTBINS - 2], fCentCheckValH[kNBCENTBINS - 2]));
    legendRAA->Draw();
    fPad->cd(3)->Update();

    fPad->cd(4);
    TPaveLabel *label = new TPaveLabel(0.1, 0.1, 0.9, 0.9,
                                       "R_{AA} = #frac{dN^{AA}/dp_{t}}{<N_{coll}> dN^{pp}/dp_{t}}");
    label->SetFillColor(24);
    label->SetTextSize(0.05);
    label->Draw();
    fPad->cd(4)->Update();
}

//=MCExerciseR2========================================================================
void MCExerciseR2::SetCentBin(Int_t low, Int_t high, Int_t binid)
{
    fCentralityLow = low;
    fCentralityHigh = high;
    fCurrentCentBin = binid;
    fCentCheck[binid]->SetDisabledAndSelected(kTRUE);
    fCentCheck[binid]->SetTextColor(kRed);
    if (!(fRole == RO_DEMO))
        ContinueExercise();
}

//=MCExerciseR2========================================================================
void MCExerciseR2::SetHistoMarkerAndColor(TH1 *histo, Style_t markerStyle, Size_t markerSize, Color_t markerColor, Color_t lineColor)
{
    histo->SetMarkerStyle(markerStyle);
    histo->SetMarkerSize(markerSize);
    histo->SetMarkerColor(markerColor);
    histo->SetLineColor(lineColor);
}

//=MCExerciseR2========================================================================
void MCExerciseR2::SetStatistics()
{
    fEntriesTestE = fEntriesTestEntryE->GetNumberEntry()->GetIntNumber();
    fEntriesTestT = fEntriesTestEntryT->GetNumberEntry()->GetIntNumber();
}

//=MCExerciseR2========================================================================
void MCExerciseR2::StartExercise()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    //Reset ROOT and connect tree file
    gROOT->Reset();
    //-------------------New GUI--------------------------

    //*********************************************************************************
    //	Definition of canvas
    //*********************************************************************************

    // TCanvas *c1 = new TCanvas("c1", "Pt");
    // TCanvas *c2 = new TCanvas("c2", "nTracksTPC");
    // TCanvas *c3 = new TCanvas("c3", "nTracksTPCvsCent");
    //*********************************************************************************
    // * Axis labeling for multiplicity histogram and plotting it to an eps file with
    //   the name dependent on the collision system and centrality
    //*********************************************************************************
    TEveManager::Create(kTRUE, "FV");
    StartBrowser();

    gEve->GetBrowser()->GetMainFrame()->SetWindowName(ml.tr("Student"));
    gEve->GetBrowser()->GetMainFrame()->Move(500, 0);
    gEve->GetBrowser()->CloseTab(0);

    gEve->GetBrowser()->StartEmbedding(TRootBrowser::kLeft);

    fSMF = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    fSMF->SetWindowName("XX GUI");
    fSMF->SetCleanup(kDeepCleanup);
    TGGroupFrame *gf = new TGGroupFrame(fSMF, "Centrality Bins");
    TGVerticalFrame *vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
    for (Int_t centbin = 0; centbin < kNBCENTBINS; centbin++)
    {
        fCentCheck[centbin] = new TGCheckButton(vf, fCentCheckName[centbin]);
        fCentCheck[centbin]->Connect("Clicked()", "MCExerciseR2", this, Form("SetCentBin(=%i, %i, %i)", fCentCheckValL[centbin], fCentCheckValH[centbin], centbin));
        fCentCheck[centbin]->SetTextJustify(36);
        fCentCheck[centbin]->SetMargins(0, 0, 0, 0);
        fCentCheck[centbin]->SetWrapLength(-1);
        vf->AddFrame(fCentCheck[centbin], new TGLayoutHints(kLHintsLeft | kLHintsTop, 2, 2, 2, 2));
    }
    fCentCheck[kNBCENTBINS - 1]->SetState(kButtonDown);
    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(gf);

    if (fRole == RO_DEMO)
    {
        gf = new TGGroupFrame(fSMF, "Statistics");
        vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
        TGHorizontalFrame *hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        TGLabel *la = new TGLabel(hf, "Events");
        fEntriesTestEntryE = new TGNumberEntry(hf, 1000, 4, 999,
                                               TGNumberFormat::kNESInteger,
                                               TGNumberFormat::kNEANonNegative,
                                               TGNumberFormat::kNELLimitMinMax,
                                               0, 10000);
        fEntriesTestEntryE->Connect("ValueSet(Long_t)", "MCExerciseR2", this, "SetStatistics()");
        (fEntriesTestEntryE->GetNumberEntry())->Connect("ReturnPressed()", "MCExerciseR2", this, "SetStatistics()");

        hf->AddFrame(la, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 1, 1, 1, 1));
        hf->AddFrame(fEntriesTestEntryE, new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));
        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsRight));

        hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
        la = new TGLabel(hf, "tracks");
        fEntriesTestEntryT = new TGNumberEntry(hf, 10000, 4, 999,
                                               TGNumberFormat::kNESInteger,
                                               TGNumberFormat::kNEANonNegative,
                                               TGNumberFormat::kNELLimitMinMax,
                                               0, 10000);
        fEntriesTestEntryT->Connect("ValueSet(Long_t)", "MCExerciseR2", this, "SetStatistics()");
        (fEntriesTestEntryT->GetNumberEntry())->Connect("ReturnPressed()", "MCExerciseR2", this, "SetStatistics()");
        hf->AddFrame(la, new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 1, 1, 1, 1));
        hf->AddFrame(fEntriesTestEntryT, new TGLayoutHints(kLHintsCenterY | kLHintsLeft | kLHintsExpandX, 4, 4, 4, 4));

        vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsRight));
        gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
        fSMF->AddFrame(gf);

        TGTextButton *b = new TGTextButton(fSMF, "&GO");
        b->Connect("Clicked()", "MCExerciseR2", this, "ContinueExercise()");
        fSMF->AddFrame(b, new TGLayoutHints(kLHintsCenterX, 4, 4, 4, 4));
    }
    fMakeRAAButton = new TGTextButton(fSMF, "&Make RAA");
    fMakeRAAButton->Connect("Clicked()", "MCExerciseR2", this, "MakeRAA()");
    fMakeRAAButton->SetEnabled(kFALSE);
    fSMF->AddFrame(fMakeRAAButton, new TGLayoutHints(kLHintsCenterX, 4, 4, 4, 4));

    fSMF->MapSubwindows();
    fSMF->Resize();
    fSMF->MapWindow();
    gEve->GetBrowser()->StopEmbedding(" ");

    gEve->GetBrowser()->StartEmbedding(TRootBrowser::kRight);
    fPad = new TCanvas();
    fPad->Divide(2, 2);
    gEve->GetBrowser()->StopEmbedding(ml.tr("Mass"));

    gEve->GetBrowser()->GetTabRight()->SetTab(2);

    if (fNeedsInit)
    {
        fNeedsInit = kFALSE;
        MakeHistograms();
        StyleSettings();
    }
}

//=MCExerciseR2========================================================================
void MCExerciseR2::StudentSet()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    TString text(Form("%s: \t %s",
                      ml.tr("Analysing file"),
                      fDataSetFileName.Data()));
    text.Append(Form("\n %s : \t %s", ml.tr("Collision system"), fCollSystem.Data()));
    text.Append(Form("\n %s : \t\t %i-%i%%", ml.tr("Centrality"), fCentralityLow, fCentralityHigh));

    //*********************************************************************************
    //Declaration of leaves types for track tree
    //*********************************************************************************
    Float_t trackCentrality; // variable for the centrality in the track tree
    Double_t trackPt;        // variable for the transverse momentum of the track

    //*********************************************************************************
    //Declaration of leaves types for event tree
    //*********************************************************************************
    Float_t eventCentrality; // variable for the centrality in the event tree
    Int_t eventMult;         // variable for the multiplicity in the event

    //*********************************************************************************
    // Definition of input trees
    //*********************************************************************************
    if (!fTrack)
    {
        fTrack = (TTree *)gDirectory->Get("Track"); // stores track properties
        fEvent = (TTree *)gDirectory->Get("Event"); // stores event properties
    }

    // Set branch addresses for track tree
    fTrack->SetBranchAddress("centrality", &trackCentrality);
    fTrack->SetBranchAddress("trackpt", &trackPt);
    // Set branch addresses for event tree
    fEvent->SetBranchAddress("nTPCAccepted", &eventMult);
    fEvent->SetBranchAddress("centrality", &eventCentrality);

    //*********************************************************************************
    // maximum number of files for testing
    //*********************************************************************************

    //*********************************************************************************
    // Getting the number of entries in the track tree
    //*********************************************************************************
    ULong64_t nEntriesTrack = fTrack->GetEntries();
    ULong64_t nEntriesEvent = fEvent->GetEntries();

    text.Append(Form("\n%s :\t %llu", ml.tr("Number of available tracks"), nEntriesTrack));
    text.Append(Form("\n%s :\t %llu", ml.tr("Number of available events"), nEntriesEvent));
    if (fRole == RO_DEMO)
    {
        text.Append(Form("\n %s :\t %llu", ml.tr("Number of tracks to analyze"), fEntriesTestT));
        text.Append(Form("\n %s :\t %llu", ml.tr("Number of events to analyze"), fEntriesTestE));
        if (nEntriesTrack > fEntriesTestT)
            nEntriesTrack = fEntriesTestT;
        if (nEntriesEvent > fEntriesTestE)
            nEntriesEvent = fEntriesTestE;
    }
    new TGMsgBox(gClient->GetRoot(), 0, "MCExerciseR2::StudentSet", text, kMBIconAsterisk, kMBOk, 0, kVerticalFrame, kTextLeft | kTextCenterY);

    //*********************************************************************************
    // * Reading the entries form the event tree (extract nTracksTPC) and filling
    //   it in the multiplicity histograms (hNTPC,hNTPCvsCent)
    // * Distinction between PbPb and pp as for PbPb you need to fill them with
    //   restriction in centrality
    // * nEntriesPerCent will give you the normalization value for the different
    //	  centralities
    //*********************************************************************************

    ULong_t nEntriesPerCent = 0;
    ULong64_t nbytes2 = 0;

    TGTransientFrame *tf = new TGTransientFrame(gClient->GetRoot(), 0, 600, 300);
    tf->SetCleanup(kDeepCleanup);
    tf->ChangeOptions((tf->GetOptions() & ~kVerticalFrame) | kHorizontalFrame);
    TGHorizontalFrame *hf = new TGHorizontalFrame(tf, 0, 0, 0);
    TGHProgressBar *pb = new TGHProgressBar(hf, TGProgressBar::kFancy, 300);
    pb->SetBarColor("lightblue");
    pb->ShowPosition(kTRUE, kFALSE, "%.0f events");
    hf->Resize(300, 300);
    hf->AddFrame(pb, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 5, 10, 5, 5));
    tf->AddFrame(hf, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 0, 0, 0, 0));
    tf->SetWindowName("Events");
    TGDimension size = tf->GetDefaultSize();
    tf->Resize(size);
    tf->MapSubwindows();
    tf->MapWindow();
    tf->Move(1000, 500);

    pb->SetMax(nEntriesEvent);
    fHNTPCvsCent->Reset();
    for (ULong_t i = 0; i < nEntriesEvent; i++)
    {
        nbytes2 += fEvent->GetEvent(i);
        if (eventCentrality > fCentralityLow && eventCentrality < fCentralityHigh)
        {
            fHNTPC[fCurrentCentBin]->Fill(eventMult);
            fHNTPCvsCent->Fill(eventMult, eventCentrality);
            nEntriesPerCent++;
        }
        if (i % 10 == 0)
        {
            pb->Increment(10);
            gSystem->ProcessEvents();
        }
    }
    if (nEntriesPerCent == 0)
    {
        new TGMsgBox(gClient->GetRoot(), 0, "MCExerciseR2::StudentSet", "increase mumber of test events", kMBIconStop, kMBOk, 0, kVerticalFrame, kTextLeft | kTextCenterY);
        return;
    }

    //*********************************************************************************
    // * Reading the entries form the track tree (extract pt values) and filling it
    //   in the pt-histogram
    // * Distinction between PbPb and pp as for PbPb you need to fill them with
    //   restriction in centrality
    //*********************************************************************************
    ULong64_t nbytes = 0;
    ULong_t nTracksPerCent = 0;
    tf->SetWindowName("Progress tracks");
    pb->Reset();
    pb->SetMax(nEntriesTrack);
    pb->ShowPosition(kTRUE, kFALSE, "%.0f tracks");

    for (ULong_t i = 0; i < nEntriesTrack; i++)
    {
        nbytes += fTrack->GetEvent(i);
        if (trackCentrality > fCentralityLow && trackCentrality < fCentralityHigh)
        {
            fHTrackPt[fCurrentCentBin]->Fill(trackPt);
            nTracksPerCent++;
        }
        ///!!!!!!!!!!!!!!!! To do: here you need to add something
        if (i % 10000 == 0)
        {
            pb->Increment(10000);
            gSystem->ProcessEvents();
        }
    }
    tf->DeleteWindow();

    fHNTPC[fCurrentCentBin]->Sumw2();
    fHNTPC[fCurrentCentBin]->Scale(1. / nEntriesPerCent);
    fPad->cd(1)->SetLogy(1);
    SetHistoMarkerAndColor(fHNTPC[fCurrentCentBin], 20, 1., kBlue, kBlue);
    fHNTPC[fCurrentCentBin]->Draw("e1,p");
    fPad->cd(1)->Update();

    fPad->cd(3)->SetLogy(0);
    fPad->cd(3)->SetLogz(1);
    fPad->cd(3)->SetRightMargin(0.1);
    fHNTPCvsCent->Draw("colz");
    fPad->cd(3)->Update();

    //*********************************************************************************
    // * Normalisation of the pt-histogram to the number of entries in this
    //   centrality bin and to the bin with in pt by deviding through the created
    //   histogram for deltaPt (fDeltaPt)
    // * Labeling of the histograms and plotting it to an eps file with the name
    //   dependent on the collision system and the centralities
    //*********************************************************************************

    fHTrackPt[fCurrentCentBin]->Sumw2();
    fHTrackPt[fCurrentCentBin]->Divide(fHDeltaPt);
    fHTrackPt[fCurrentCentBin]->Scale(1. / nEntriesPerCent);
    fPad->cd(2)->SetLogy(1);
    SetHistoMarkerAndColor(fHTrackPt[fCurrentCentBin], 20, 1., kRed, kRed);
    fHTrackPt[fCurrentCentBin]->Draw("e1,p");
    fPad->cd(2)->Update();

    // fPad->cd(1)->SaveAs(Form("nTracksTPC_%i-%i.eps",fCentralityLow, fCentralityHigh));
    // fPad->cd(2)->SaveAs(Form("nTracksPt_%i-%i.eps", fCentralityLow, fCentralityHigh));
    // fPad->cd(3)->SaveAs(Form("nTracksTPCvsCent_%i-%i.eps", fCentralityLow, fCentralityHigh));

    // TFile* fileOutput = new TFile("RAABaseOutput.root","UPDATE");
    // fHNTPC->Write(Form("nTracksTPC_%s_%i-%i", fCollSystem.Data(), fCentralityLow ,fCentralityHigh), TObject::kOverwrite);
    // /// To do: uncomment this as soon as you are ready
    // fHTrackPt->Write(Form("trackPt_%s_%i-%i", fCollSystem.Data(), fCentralityLow, fCentralityHigh), TObject::kOverwrite);
    // fileOutput->Write();
    // fileOutput->Close();

    // scaling the pT spectrum by corresponding number of Collisions (nColl)
    fHTrackPt[fCurrentCentBin]->Scale(1.0 / fNColl[fCurrentCentBin]);
    Bool_t allDone = kTRUE;
    for (Int_t cenbin = 0; cenbin < kNBCENTBINS - 1; cenbin++)
        if (!fCentCheck[cenbin]->IsDisabledAndSelected())
            allDone = kFALSE;
    if (allDone)
        fMakeRAAButton->SetEnabled(kTRUE);
}

//=MCExerciseR2========================================================================
void MCExerciseR2::StyleSettings()
{
    gStyle->SetOptDate(0); //show day and time
    gStyle->SetOptStat(1); //show statistic
    gStyle->SetPalette(1, 0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetTitleFillColor(0);
    gStyle->SetTextSize(0.5);
    gStyle->SetLabelSize(0.03, "xyz");
    gStyle->SetLabelOffset(0.002, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetTitleOffset(1.7, "y");
    gStyle->SetTitleOffset(0.7, "x");
    gStyle->SetCanvasColor(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetLineWidth(2);

    gStyle->SetPadTopMargin(0.06);
    gStyle->SetPadBottomMargin(0.09);
    gStyle->SetPadRightMargin(0.04);
    gStyle->SetPadLeftMargin(0.1);

    TGaxis::SetMaxDigits(3);
}

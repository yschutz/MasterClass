#include "MCAliExtractor.h"
#include "MCAliSelector.h"
#include "MCExerciseJ1.h"
#include "MCMultiLingual.h"
#include "MCMultiView.h"
#include "MCVSDReader.h"

#include <fstream>
#include <iostream>

#include <TCanvas.h>
#include <TColor.h>
#include <TEveBrowser.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveVSD.h>
#include <TEveWindowManager.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGLabel.h>
#include <TGMsgBox.h>
#include <TGNumberEntry.h>
#include <TGProgressBar.h>
#include <TGTab.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLine.h>
#include <TRootHelpDialog.h>
#include <TSystem.h>
#include <TTree.h>

//=MCExerciseJ1========================================================================
MCExerciseJ1::MCExerciseJ1(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
  fDataSetFileDir = "Data-Masterclass/events/JPsi/Part1/";
  fDataTemplate = "events_x.root";
  fGuiLogoName = "images/charmonium.png";
  MakeGui();
  fDemo->SetEnabled(kFALSE);
  fTutor->SetEnabled(kFALSE);
  fData->RemoveAll();
  fData->AddEntry("pp collisions", 0);
  fData->AddEntry("p-Pb collisions", 1);
}

//=MCExerciseJ1========================================================================
MCExerciseJ1::~MCExerciseJ1()
{
  if (!fNeedsInit)
    DeleteHistograms();
  if (fInstructions)
    fInstructions->CloseWindow(); 
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::ApplyTrackCuts()
{
  fApplyTrackCuts = kTRUE;
  GotoEvent(MCVSDReader::Instance().CurEv());
  fButtonApplyTrackCuts->SetState(kButtonDown);
  if (MCAliSelector::Instance().IsStarted())
  {
    fPad2->cd();
    PlotPIDLines();
    fPad2->cd()->Update();
  }
}
//=MCExerciseJ1========================================================================
void MCExerciseJ1::BeginAgain()
{
  ClearHisto();
  fPMin = 0.;
  fPMax = 10.;
  fDeMin = 20.;
  fDeMax = 140.;

  fEsdTracksLoaded = kFALSE;
  fApplyTrackCuts = kFALSE;
  fEnableQuickAnalysis = kFALSE;

  fCheckESDTracks->SetEnabled(kTRUE);
  fCheckESDTracks->SetState(kButtonUp);
  fButtonFillPidHistos->SetEnabled(kTRUE);
  fButtonDefineTrackCuts->SetEnabled(kTRUE);
  fButtonApplyTrackCuts->SetEnabled(kTRUE);
  fButtonFillInvMass->SetEnabled(kTRUE);
  fButtonJumpEvents->SetEnabled(kFALSE);
  GotoEvent(0);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::CheckNLoop()
{
  if (fNLoop->GetNumber() > MCVSDReader::Instance().CurEv() + MCVSDReader::Instance().MaxEv())
    fNLoop->SetNumber(MCVSDReader::Instance().MaxEv() - MCVSDReader::Instance().CurEv());
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::ClearHisto()
{
  fPad->cd(1);
  fhMassInv->Reset();
  fhMassInv->Draw();
  fPad->cd(1)->Update();

  fPad2->cd();
  fhEnergyLoss->Reset();
  fhEnergyLoss->Draw();
  fPad2->cd()->Update();

  fPad->cd(3);
  fhEleEle->Reset();
  fhEleEle->Draw();
  fPad->cd(3)->Update();

  fPad->cd(4);
  fhPosPos->Reset();
  fhPosPos->Draw();
  fPad->cd(4)->Update();

  fPad->cd(2);
  fhJPsi->Reset();
  fhJPsi->Draw();
  fPad->cd(2)->Update();
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::CustomizeBrowser()
{
  MCMultiLingual &ml = MCMultiLingual::Instance();
  TEveBrowser *browser = gEve->GetBrowser();
  browser->GetMainFrame()->DontCallClose();
  browser->GetMainFrame()->SetWindowName("ALICE J/psi Master Class");

  browser->GetMainFrame()->Move(500, 0);

  if (browser->GetTabRight()->GetTabTab(3))
    browser->GetTabRight()->RemoveTab(3);
  if (browser->GetTabRight()->GetTabTab(2))
    browser->GetTabRight()->RemoveTab(2);
  if (browser->GetTabLeft()->GetTabTab(0))
    browser->GetTabLeft()->RemoveTab(0);
  if (browser->GetTabBottom()->GetTabTab(0))
    browser->GetTabBottom()->RemoveTab(0);

  browser->StartEmbedding(TRootBrowser::kLeft);
  fSMF = new TGMainFrame(gClient->GetRoot(), 1000, 600);
  fSMF->SetWindowName("XX GUI");
  fSMF->SetCleanup(kDeepCleanup);

  TGTextButton *btext = nullptr;
  TGHorizontalFrame *horiF = nullptr;
  TGLabel *label = nullptr;
  TGPictureButton *bpict = nullptr;

  TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")));
  TGGroupFrame *groupF = new TGGroupFrame(fSMF, ml.tr("STUDENT MODE INSTRUCTIONS"));
  TGVerticalFrame *vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
  {
    const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
    FontStruct_t buttonFont = font->GetFontStruct();
    TGTextButton *btext = new TGTextButton(vertF, "Instructions");
    btext->SetTextColor(TColor::Number2Pixel(kRed + 2));
    btext->SetFont(buttonFont);
    vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    btext->Connect("Clicked()", "MCExerciseJ1", this, "Instructions()");
  }
  groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  fSMF->AddFrame(groupF);

  groupF = new TGGroupFrame(fSMF, ml.tr("Events Navigation"));
  vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
  {
    horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
    {
      label = new TGLabel(horiF, ml.tr("Previous"));
      horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));

      label = new TGLabel(horiF, ml.tr("Current"));
      horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      label = new TGLabel(horiF, ml.tr("Next"));
      horiF->AddFrame(label, new TGLayoutHints(kLHintsRight, 1, 1, 1, 1));
    }
    vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
    {
      bpict = new TGPictureButton(horiF, gClient->GetPicture(icondir + "GoBack.gif"));
      horiF->AddFrame(bpict, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
      bpict->Connect("Clicked()", "MCExerciseJ1", this, "PrevEvent()");
      fEventNumber = new TGLabel(horiF);
      fEventNumber->SetText(1);
      horiF->AddFrame(fEventNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      bpict = new TGPictureButton(horiF, gClient->GetPicture(icondir + "GoForward.gif"));
      horiF->AddFrame(bpict, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
      bpict->Connect("Clicked()", "MCExerciseJ1", this, "NextEvent()");
    }
    vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    // horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
    // {
    //     label = new TGLabel(horiF, ml.tr(EVTDA));
    //     horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
    //     fEventAnalysed = new TGLabel(horiF);
    //     fEventAnalysed->SetText("0");
    //     horiF->AddFrame(fEventAnalysed, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    // }
    // vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  }
  groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  fSMF->AddFrame(groupF);

  groupF = new TGGroupFrame(fSMF, ml.tr("Analysis tool"));
  {
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
      fButtonDefineTrackCuts = new TGTextButton(vertF, ml.tr("Define Track Cuts"));
      fButtonDefineTrackCuts->Connect("Pressed()", "MCExerciseJ1", this, "Selector()");
      vertF->AddFrame(fButtonDefineTrackCuts, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
      {
        fButtonApplyTrackCuts = new TGTextButton(horiF, ml.tr("Tracks Selection On/Off"));
        fButtonApplyTrackCuts->SetEnabled(kFALSE);
        fButtonApplyTrackCuts->Connect("Clicked()", "MCExerciseJ1", this, "SwapApplyTrackCuts()");
        fButtonApplyTrackCuts->SetState(kButtonUp);
        horiF->AddFrame(fButtonApplyTrackCuts, new TGLayoutHints(kLHintsExpandX));
      }
      vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      fButtonFillPidHistos = new TGTextButton(vertF, ml.tr("Update PID Histogram"));
      fButtonFillPidHistos->Connect("Pressed()", "MCExerciseJ1", this, "FillEnergyLossHisto()");
      vertF->AddFrame(fButtonFillPidHistos, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

      fButtonFillInvMass = new TGTextButton(vertF, ml.tr("Update Invariant Mass Histograms"));
      fButtonFillInvMass->Connect("Pressed()", "MCExerciseJ1", this, "FillInvariantMassHistos()");
      vertF->AddFrame(fButtonFillInvMass, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

      btext = new TGTextButton(vertF, ml.tr("Extract Signal"));
      btext->Connect("Pressed()", "MCExerciseJ1", this, "Extractor()");
      vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      btext = new TGTextButton(vertF, ml.tr("Restart"));
      btext->Connect("Pressed()", "MCExerciseJ1", this, "BeginAgain()");
      vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  }
  fSMF->AddFrame(groupF);

  groupF = new TGGroupFrame(fSMF, "Visu");
  {
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
      horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
      {
        fButtonLoadTracks = new TGTextButton(horiF, ml.tr("Load tracks"));
        fButtonLoadTracks->Connect("Clicked()", "MCExerciseJ1", this, "DisplayTracks()");
        fCheckESDTracks = new TGCheckButton(horiF, "", 10);
        fCheckESDTracks->SetEnabled(kFALSE),
            horiF->AddFrame(fButtonLoadTracks, new TGLayoutHints(kLHintsExpandX | kLHintsLeft));
        horiF->AddFrame(fCheckESDTracks, new TGLayoutHints(kLHintsExpandX | kLHintsRight, 80, 1, 1, 1));
      }
      vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      btext = new TGTextButton(vertF, ml.tr("Geometry"));
      btext->Connect("Clicked()", "MCExerciseJ1", this, "ChangeGeometry()");
      vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      btext = new TGTextButton(vertF, ml.tr("Screen Background"));
      btext->Connect("Clicked()", "MCExerciseJ1", this, "ChangeBackgroundColor()");
      vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      btext = new TGTextButton(vertF, ml.tr("Clear Histograms"));
      btext->Connect("Pressed()", "MCExerciseJ1", this, "ClearHisto()");
      vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  }
  fSMF->AddFrame(groupF);

  groupF = new TGGroupFrame(fSMF, ml.tr("Quick Analysis"));
  {
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
      horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
      {
        fButtonQuickAnalysis = new TGTextButton(horiF, ml.tr("Select"));
        fButtonQuickAnalysis->Connect("Clicked()", "MCExerciseJ1", this, "SwapQuickAnalysis()");
        horiF->AddFrame(fButtonQuickAnalysis, new TGLayoutHints(kLHintsExpandX));
      }
      vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    {
      horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
      {
        TGLabel *loopNumber = new TGLabel(horiF, Form("N %s =", ml.tr("Events")));
        fNLoop = new TGNumberEntryField(horiF);
        fNLoop->Connect("ReturnPressed()", "MCExerciseJ1", this, "CheckNLoop()");
        fNLoop->SetEnabled(kFALSE);
        horiF->AddFrame(loopNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        horiF->AddFrame(fNLoop, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      }
      vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    {
      horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
      {
        fButtonJumpEvents = new TGTextButton(horiF, "GO");
        fButtonJumpEvents->Connect("Clicked()", "MCExerciseJ1", this, "EventsJump()");
        fButtonJumpEvents->SetEnabled(kFALSE);
        horiF->AddFrame(fButtonJumpEvents, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      }
      vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
  }
  fSMF->AddFrame(groupF);

  fSMF->MapSubwindows();
  fSMF->Resize();
  fSMF->MapWindow();

  browser->StopEmbedding("Tools");

  browser->StartEmbedding(TRootBrowser::kRight);

  fPad2 = new TCanvas();
  gPad->SetLogx(1);
  fhEnergyLoss->Draw();

  browser->StopEmbedding("Particle Identification");

  browser->GetTabRight()->SetTab(1);

  browser->StopEmbedding(" ");
  browser->StartEmbedding(TRootBrowser::kRight);

  fPad = new TCanvas();
  fPad->Divide(2, 2);

  fPad->cd(1);
  fhMassInv->Draw("ep");

  fPad->cd(2);
  fhJPsi->Draw("ep");

  fPad->cd(3);
  fhEleEle->Draw("ep");

  fPad->cd(4);
  fhPosPos->Draw("ep");

  browser->StopEmbedding("Invariant Mass");
  browser->GetTabRight()->SetTab(1);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::DrawELoss()
{
  fPad2->cd();
  fhEnergyLoss->Draw("colz");
  if (fApplyTrackCuts && MCAliSelector::Instance().IsStarted())
    PlotPIDLines();
  fPad2->cd()->Update();
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::DeleteHistograms()
{
  delete fhElectrons;
  delete fhPositrons;
  delete fhEleEle;
  delete fhPosPos;
  delete fhJPsi;
  delete fhEnergyLoss;
  delete fhMassInv;
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::DrawInvMass()
{
  fPad->cd(1);
  Double_t yMax = fhMassInv->GetBinContent(fhMassInv->GetMaximumBin());
  yMax += TMath::Sqrt(yMax);
  yMax *= 1.05;
  fhMassInv->GetYaxis()->SetRangeUser(0, yMax);
  fhMassInv->Draw();
  fPad->cd(2);
  fhJPsi->Draw();
  fPad->cd(3);
  fhEleEle->GetYaxis()->SetRangeUser(0, yMax);
  fhEleEle->Draw();
  fPad->cd(4);
  fhPosPos->GetYaxis()->SetRangeUser(0, yMax);
  fhPosPos->Draw();
  fPad->cd(1)->Update();
  fPad->cd(2)->Update();
  fPad->cd(3)->Update();
  fPad->cd(4)->Update();
  fhJPsi->Add(fhElectrons, -1);
  fhJPsi->Add(fhPositrons, -1);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::EnableTrackCutsButton()
{
  fButtonApplyTrackCuts->SetEnabled(kTRUE);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::EventsJump()
{
  MCMultiLingual &ml = MCMultiLingual::Instance();
  if (fEnableQuickAnalysis)
  {
    Int_t nLoop = fNLoop->GetNumber();
    if (nLoop + MCVSDReader::Instance().CurEv() > MCVSDReader::Instance().MaxEv())
    {
      new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseJ1::EventsJump", ml.tr("Too many events selected"), kMBIconExclamation, kMBOk);
      return;
    }

    TGTransientFrame *transientF = new TGTransientFrame(gClient->GetRoot(), 0, 600, 300);
    transientF->SetCleanup(kDeepCleanup);
    transientF->ChangeOptions((transientF->GetOptions() & ~kVerticalFrame) | kHorizontalFrame);
    TGHorizontalFrame *horiF = new TGHorizontalFrame(transientF, 0, 0, 0);
    TGHProgressBar *progressbar = new TGHProgressBar(horiF, TGProgressBar::kFancy, 300);
    progressbar->SetBarColor("lightblue");
    progressbar->ShowPosition(kTRUE, kFALSE, "%.0f events");
    horiF->Resize(300, 300);
    horiF->AddFrame(progressbar, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 5, 10, 5, 5));
    transientF->AddFrame(horiF, new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandY, 0, 0, 0, 0));
    transientF->SetWindowName("Events");
    TGDimension size = transientF->GetDefaultSize();
    transientF->Resize(size);
    transientF->MapSubwindows();
    transientF->MapWindow();
    transientF->Move(1000, 500);
    progressbar->SetMax(nLoop);

    for (Int_t i = 0; i < nLoop; i++)
    {
      NextEvent();
      if (i % 10 == 0)
      {
        progressbar->Increment(10);
        gSystem->ProcessEvents();
      }
    }
    transientF->DeleteWindow();
    DrawELoss();
    DrawInvMass();
  }
  else
    new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseJ1::EventsJump", ml.tr("Enable Quick Analysis first"), kMBIconExclamation, kMBOk);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::Extractor()
{
  MCAliExtractor::Instance().Start(this);
  MCAliExtractor::Instance().SetAll(fMMin, fMMax);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::FillEnergyLossHisto()
{
  if (fEsdTracksLoaded)
  {
    if (fNSelectedTracks > 0)
    {
      for (Int_t i = 0; i < fNSelectedTracks; i++)
        fhEnergyLoss->Fill(fArrP[i], fArrdEdx[i]);
      if (!fEnableQuickAnalysis)
        DrawELoss();
      ResetDedxArray();
    }
  }
  else
    new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseJ1::FillEnergyLossHisto", "Load tracks before filling energy loss histos!", kMBIconExclamation, kMBOk);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::FillInvariantMassHistos()
{
  if (fEsdTracksLoaded)
  {
    fNEvents++;
    for (Int_t i = 0; i < fNElectrons - 1; i++)
    {
      for (Int_t j = i + 1; j < fNElectrons; j++)
      {
        Double_t mass = TMath::Sqrt((fElectrons[i][0] + fElectrons[j][0]) *
                                        (fElectrons[i][0] + fElectrons[j][0]) -
                                    (fElectrons[i][1] + fElectrons[j][1]) *
                                        (fElectrons[i][1] + fElectrons[j][1]) -
                                    (fElectrons[i][2] + fElectrons[j][2]) *
                                        (fElectrons[i][2] + fElectrons[j][2]) -
                                    (fElectrons[i][3] + fElectrons[j][3]) *
                                        (fElectrons[i][3] + fElectrons[j][3]));
        fhEleEle->Fill(mass);
        fhElectrons->Fill(mass);
      }
    }
    for (Int_t i = 0; i < fNPositrons - 1; i++)
    {
      for (Int_t j = i + 1; j < fNPositrons; j++)
      {
        Double_t mass = TMath::Sqrt((fPositrons[i][0] + fPositrons[j][0]) *
                                        (fPositrons[i][0] + fPositrons[j][0]) -
                                    (fPositrons[i][1] + fPositrons[j][1]) *
                                        (fPositrons[i][1] + fPositrons[j][1]) -
                                    (fPositrons[i][2] + fPositrons[j][2]) *
                                        (fPositrons[i][2] + fPositrons[j][2]) -
                                    (fPositrons[i][3] + fPositrons[j][3]) *
                                        (fPositrons[i][3] + fPositrons[j][3]));
        fhPosPos->Fill(mass);
        fhPositrons->Fill(mass);
      }
    }

    for (Int_t i = 0; i < fNElectrons; i++)
    {
      for (Int_t j = 0; j < fNPositrons; j++)
      {
        Double_t mass = TMath::Sqrt((fElectrons[i][0] + fPositrons[j][0]) *
                                        (fElectrons[i][0] + fPositrons[j][0]) -
                                    (fElectrons[i][1] + fPositrons[j][1]) *
                                        (fElectrons[i][1] + fPositrons[j][1]) -
                                    (fElectrons[i][2] + fPositrons[j][2]) *
                                        (fElectrons[i][2] + fPositrons[j][2]) -
                                    (fElectrons[i][3] + fPositrons[j][3]) *
                                        (fElectrons[i][3] + fPositrons[j][3]));
        fhMassInv->Fill(mass);
        fhJPsi->Fill(mass);
      }
    }
    if (!fEnableQuickAnalysis)
      DrawInvMass();
    ResetMatrices();
  }
  else
    new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseJ1::FillInvariantMassHistos", "Load tracks before filling energy loss histos!", kMBIconExclamation, kMBOk);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::Instructions()
{
  MCMultiLingual &ml = MCMultiLingual::Instance();

  fInstructions = new TRootHelpDialog(gClient->GetRoot(), " ", 620, 400);
  // fInstructions->DontCallClose();
  fInstructions->Popup();
  switch (fRole)
  {
  case RO_DEMO:
    break;
  case RO_STUDENT:
    fInstructions->SetWindowName(ml.tr("INSTRUCTIONS MODE ETUDIANT"));
    fInstructions->SetText(ml.tr("\
Your task is to reconstruct signals of the production of the J/Psi particle in data recorded in\n\
proton-proton or proton-lead collisions with the ALICE detector at the LHC at CERN.\n\
The J/psi particle can be produced in the collision and then immediately decay into a pair\n\
of an electron and a positron. These \"daughter particles we can detect in the detector. \n\
In order to reconstruct the particle they decayed from, the \"mother particle\" we can make use\n\
of the \"invariant mass\". For a single particle, this is just is rest mass (3.1 GeV/c^2 for a J/Psi).\n\
If the particle decays, this quantity is conserved and can  be calculated from the momenta and the\n\
masses of the decay daughters. \n\n\
So in order to reconstruct J/psi particles from collision data we have to do the following:\n\n\
  1.) Select only electrons and positrons out of the many particles that are produced in a collision\n\
    (e.g. pions, kaons, protons,...) For this we can use the particle identification capabilities \n\
    of the detector, based on the specific energy loss in the time projection chamber \n\
    (see attached plots). But even if we only select electrons, they can still come from many\n\
    different sources, for example a conversion of a photon to an electron-positron pair or \n\
    direct production of electron-positron pairs. \n\
    To reject electrons from other sources one possibility is to select only tracks that have a high\n\
    momentum (for kinetic reasons, the average momentum will be somewhere around half the J/psi mass).\n\
  2.)Combine all electrons with all positrons and calculate for each pair the invariant mass.\n\
     The signal of the J/psi will sit on a background from random combinations of electrons and \n\
     positrons. In order to subtract this background and obtain the pure signal, we must be able\n\
     to describe it. One method is to make combinations of electrons with electrons and of positrons\n\
     with positrons and calculate the invariant mass of these pairs.\n\
    \
  3.) Do the same for a large number of collisions. (For publications, we typically analyze some \n\
  hundred million collisions.) \n\
\n\n\
For your task you have several tools at hand, which can be found on the left side of the program:\n\
  Event Navigation: section you can browser through the recorded collisions events.\n\
  Analysis Tools:\n\
    Here you can \n\
      - Load the tracks of the selected event\n\
      - Plot for the tracks the specific energy loss in the TPC as a function of momentum\n\
      - Define cuts to be applied on the tracks \n\
      - Apply the defined cuts on your track selection\n\
      - Fill the invariant mass distributions for the selected tracks for electron-electron,\n\
        positron-positron and electron-positron pairs\n\
      - extract the signal by defining a mass range and counting the entries in the background \n\
        subtracted invariant mass histogramm\n\
  Steering:\n\
    Here you can choose whether to show the ALICE detectors, change the background color, clear all\n\
    filled histograms or restart the analysis from event 1\n\
  Quick Analysis:\n\
  Quick analysis: Once you have found satisfying track cuts and tested them on some events,\n\
    you can analyze more events more quickly by automatically looping over a given number\n\
    of events and filling the histograms automatically.\n\
  On the main part of the window you find several plots and visualizations, grouped in 3 tabs:\n\
  Multi View:\n\
    In this tab you find a 3D representation of the ALICE detector , together with 2 seperate\n\
    2D projections. With your mouse you can zoom, rotate and shift the representations. \n\
    \
  Particle Identification:\n\
    In this tab you find a 2 dimensional histogram which shows you for each track its specific energy\n\
    loss in the TPC as a function of its momentum. Lines indicate which tracks you are selecting with\n\
    your current cut settings.\n\
  Invariant Mass:\n\
    In this tab you find the invariant mass distributions for combinations of \n\
      - electrons with electron\n\
      - positrons with positrons\n\
      - electrons with positrons\n\
    and a plot where the electron-electron and positron positron distribution is subtracted from the \n\
    electron-positron distribution."));
    break;
  case RO_TUTOR:
    break;
  default:
    break;
  }
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::LoadEsdTracks(Int_t maxR)
{
  if (!fTrackList)
  {
    fTrackList = new TEveTrackList("ESD Tracks");
    fTrackList->SetMainColor(kBlue);
  }
  else
    fTrackList->DestroyElements();
  TEveTrackPropagator *trkProp = fTrackList->GetPropagator();
  trkProp->SetMaxR(maxR);

  ResetMatrices();
  ResetDedxArray();
  if (MCVSDReader::Instance().GetEveVSD()->fTreeR)
  {
    for (Int_t i = 0; i < MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntries(); ++i)
    {
      MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(i);
      TEveTrack *track = new TEveTrack(&MCVSDReader::Instance().GetEveVSD()->fR, trkProp);
      Double_t px = track->GetMomentum().fX;
      Double_t py = track->GetMomentum().fY;
      Double_t pz = track->GetMomentum().fZ;
      Double_t dedx = track->GetStatus();
      Int_t charge = track->GetCharge();
      Double_t pSquared = px * px + py * py + pz * pz;
      if (fApplyTrackCuts && (dedx >= fDeMax || dedx < fDeMin || pSquared >= fPMax * fPMax || pSquared <= fPMin * fPMin))
      {
        track->Destroy();
        continue;
      }
      fNSelectedTracks++;
      fArrP[i] = TMath::Sqrt(px * px + py * py + pz * pz);
      fArrdEdx[i] = dedx;
      Double_t massSquared = dedx > 62 ? 0. : 0.019;
      Double_t e = TMath::Sqrt(pSquared + massSquared);
      if (charge > 0)
      {
        fPositrons[fNPositrons][0] = e;
        fPositrons[fNPositrons][1] = px;
        fPositrons[fNPositrons][2] = py;
        fPositrons[fNPositrons][3] = pz;
        fNPositrons++;
      }
      else
      {
        fElectrons[fNElectrons][0] = e;
        fElectrons[fNElectrons][1] = px;
        fElectrons[fNElectrons][2] = py;
        fElectrons[fNElectrons][3] = pz;
        fNElectrons++;
      }
      track->SetAttLineAttMarker(fTrackList);
      fTrackList->AddElement(track);
    }
  }
  if (!fEnableQuickAnalysis)
  {
    fTrackList->MakeTracks();
    gEve->AddElement(fTrackList);
    gEve->Redraw3D(kFALSE);
  }
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::MakeHistograms()
{
  MCMultiLingual &ml = MCMultiLingual::Instance();

  fhElectrons = new TH1F("fhElectrons", "e^{-} e^{-} Distribution", 60, 0., 6);
  fhElectrons->Sumw2();

  fhPositrons = new TH1F("fhPositrons", "e^{+} e^{+} Distribution", 60, 0., 6);
  fhPositrons->Sumw2();

  Int_t nbinsX = 200;
  Int_t nbinsY = 120;
  Double_t *binLimitsX = new Double_t[nbinsX + 1];
  Double_t *binLimitsY = new Double_t[nbinsX + 1];
  Double_t firstX = 0.1;
  Double_t lastX = 20.;
  Double_t firstY = 20.;
  Double_t lastY = 140.;
  Double_t expMax = TMath::Log(lastX / firstX);
  for (Int_t i = 0; i < nbinsX + 1; ++i)
    binLimitsX[i] = firstX * TMath::Exp(expMax / nbinsX * (Double_t)i);
  for (Int_t i = 0; i < nbinsY + 1; ++i)
    binLimitsY[i] = firstY + i * (lastY - firstY) / nbinsY;
  fhEnergyLoss = new TH2F("fhEnergyLoss", ml.tr("Specific energy loss"), nbinsX, binLimitsX, nbinsY, binLimitsY);
  fhEnergyLoss->GetXaxis()->SetTitle(Form("%s (GeV/c)", ml.tr("Momentum")));
  fhEnergyLoss->GetXaxis()->SetTitleOffset(1.5);
  fhEnergyLoss->GetYaxis()->SetTitle("TPC dE/dx (a.u.)");
  fhEnergyLoss->SetStats(0);

  fhMassInv = new TH1F("fhMassInv", Form("%s: electron + positron", ml.tr("Invariant Mass Distribution")), 60, 0., 6);
  fhMassInv->SetLineColor(2);
  fhMassInv->SetMarkerColor(2);
  fhMassInv->GetXaxis()->SetTitle("m_{e^{+}e^{-}} (GeV/c^{2})");
  fhMassInv->GetYaxis()->SetTitle(ml.tr("Counts"));
  fhMassInv->Sumw2();

  fhPosPos = new TH1F("fhPosPos", Form("%s: positron + positron", ml.tr("Invariant Mass Distribution")), 60, 0., 6);
  fhPosPos->SetLineColor(416);
  fhPosPos->SetMarkerColor(416);
  fhPosPos->GetXaxis()->SetTitle("m_{e^{+}e^{+}} (GeV/c^{2})");
  fhPosPos->GetYaxis()->SetTitle(ml.tr("Counts"));
  fhPosPos->Sumw2();

  fhEleEle = new TH1F("fhEleEle", Form("%s: electron + electron", ml.tr("Invariant Mass Distribution")), 60, 0., 6);
  fhEleEle->SetLineColor(416);
  fhEleEle->SetMarkerColor(416);
  fhEleEle->GetXaxis()->SetTitle("m_{e^{-}e^{-}} (GeV/c^{2})");
  fhEleEle->GetYaxis()->SetTitle(ml.tr("Counts"));
  fhEleEle->Sumw2();

  fhJPsi = new TH1F("fhJPsi", "Difference: EP - (EE + PP) ", 60, 0., 6);
  fhJPsi->SetLineColor(6);
  fhJPsi->SetMarkerColor(6);
  fhJPsi->GetXaxis()->SetTitle("m_{e^{+}e^{-}} (GeV/c^{2})");
  fhJPsi->GetYaxis()->SetTitle(ml.tr("Counts"));
  fhJPsi->Sumw2();
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::NextEvent()
{
  MCVSDReader &reader = MCVSDReader::Instance();
  if (reader.CurEv() >= reader.MaxEv() - 1)
  {
    TString msgText = Form("%s %d", MCMultiLingual::Instance().tr("Last event"), reader.MaxEv());
    new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseS1:Data", msgText, kMBIconExclamation, kMBOk);
    return;
  }
  else
  {
    ResetMatrices();
    GotoEvent(reader.CurEv() + 1);
  }
}

//=MCExerciseJ1========================================================================
Bool_t MCExerciseJ1::GotoEvent(Int_t evt)
{
  fTrackList = nullptr;
  Bool_t rv = MCVSDReader::Instance().GotoEvent(evt);
  if (fEventNumber)
    fEventNumber->SetText(TString::Format("Evt %i / %i",
                                          MCVSDReader::Instance().CurEv() + 1,
                                          MCVSDReader::Instance().MaxEv()));
  if (fEsdTracksLoaded)
    LoadEsdTracks(kfMaxRadius);
  if (fEnableQuickAnalysis)
  {
    FillInvariantMassHistos();
    FillEnergyLossHisto();
  }
  else
  {
    TEveElement *top = gEve->GetCurrentEvent();
    if (MCMultiView::Instance().IsSet())
    {
      MCMultiView::Instance().DestroyEventRPhi();
      MCMultiView::Instance().ImportEventRPhi(top);
      MCMultiView::Instance().DestroyEventRhoZ();
      MCMultiView::Instance().ImportEventRhoZ(top);
    }
  }
  return kTRUE;
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::PlotPIDLines()
{
  fPad2->cd();
  fhEnergyLoss->Draw("colz");
  for (Int_t i = 0; i < 4; i++)
    fLine[i]->Draw();
  fPad2->cd()->Update();
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::PrevEvent()
{
  Int_t currentEvent = MCVSDReader::Instance().CurEv();
  if (currentEvent == 0)
    return;
  else
  {
    ResetMatrices();
    GotoEvent(currentEvent - 1);
  }
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::ResetDedxArray()
{
  for (int i = 0; i < fNSelectedTracks; i++)
  {
    fArrP[i] = 0;
    fArrdEdx[i] = 0;
  }
  fNSelectedTracks = 0;
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::ResetMatrices()
{
  for (int i = 0; i < 100; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      fElectrons[i][j] = 0;
      fPositrons[i][j] = 0;
    }
  }
  fNPositrons = 0;
  fNElectrons = 0;
  fhElectrons->Reset();
  fhPositrons->Reset();
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::Selector()
{
  MCAliSelector::Instance().Start(this);
  MCAliSelector::Instance().SetAll(fPMin, fPMax, fDeMin, fDeMax);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::SetPIDCutValues()
{
  fPMin = MCAliSelector::Instance().GetPMin();
  fPMax = MCAliSelector::Instance().GetPMax();
  fDeMin = MCAliSelector::Instance().GetDeMin();
  fDeMax = MCAliSelector::Instance().GetDeMax();

  for (Int_t i = 0; i < 4; i++)
  {
    if (fLine[i])
      delete fLine[i];
    fLine[i] = new TLine();
  }
  fLine[0]->SetX1(fPMin);
  fLine[0]->SetY1(fDeMin);
  fLine[0]->SetX2(fPMin);
  fLine[0]->SetY2(fDeMax);
  fLine[0]->SetLineColor(2);
  fLine[0]->SetLineStyle(5);

  fLine[1]->SetX1(fPMax);
  fLine[1]->SetY1(fDeMin);
  fLine[1]->SetX2(fPMax);
  fLine[1]->SetY2(fDeMax);
  fLine[1]->SetLineColor(2);
  fLine[1]->SetLineStyle(5);

  fLine[2]->SetX1(fPMin);
  fLine[2]->SetY1(fDeMin);
  fLine[2]->SetX2(fPMax);
  fLine[2]->SetY2(fDeMin);
  fLine[2]->SetLineColor(2);
  fLine[2]->SetLineStyle(5);

  fLine[3]->SetX1(fPMin);
  fLine[3]->SetY1(fDeMax);
  fLine[3]->SetX2(fPMax);
  fLine[3]->SetY2(fDeMax);
  fLine[3]->SetLineColor(2);
  fLine[3]->SetLineStyle(5);
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::StartExercise()
{
  static Bool_t groupFirst = kTRUE;
  if (fNeedsInit)
  {
    fNeedsInit = kFALSE;
    InitViewer();
    MakeHistograms();
  }
  ResetHistograms(); 
  switch (fRole)
  {
  case RO_TUTOR:
    TutorSet();
    break;
  case RO_STUDENT:
    StudentSet();
    GotoEvent(0);
    break;
  case RO_DEMO:
    StudentSet();
    GotoEvent(0);
    break;
  default:
    break;
  }
  gEve->GetWindowManager()->HideAllEveDecorations();
  gEve->Redraw3D(kTRUE);  
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::StudentSet()
{
  Instructions();
  CustomizeBrowser();
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::SwapApplyTrackCuts()
{
  fApplyTrackCuts = !fApplyTrackCuts;
  if (!fApplyTrackCuts)
  {
    fApplyTrackCuts = kFALSE;
    GotoEvent(MCVSDReader::Instance().CurEv());
    fButtonApplyTrackCuts->SetState(kButtonUp);
    fPad2->cd();
    fhEnergyLoss->Draw("colz");
    fPad2->cd()->Update();
  }
  else
  {
    fButtonApplyTrackCuts->SetState(kButtonDown);
    ApplyTrackCuts();
  }
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::SwapQuickAnalysis()
{
  Bool_t enable = !fEnableQuickAnalysis;
  fButtonLoadTracks->SetEnabled(!enable);
  fButtonFillPidHistos->SetEnabled(!enable);
  fButtonDefineTrackCuts->SetEnabled(!enable);
  fButtonApplyTrackCuts->SetEnabled(!enable);
  fButtonFillInvMass->SetEnabled(!enable);

  fEnableQuickAnalysis = enable;
  fButtonJumpEvents->SetEnabled(enable);
  fNLoop->SetEnabled(enable);

  if (enable)
  {
    fButtonQuickAnalysis->SetState(kButtonDown);
    fEsdTracksLoaded = kTRUE;
    fCheckESDTracks->SetDisabledAndSelected(kTRUE);
  }
  else
  {
    fButtonQuickAnalysis->SetState(kButtonUp);
  }
}

//=MCExerciseJ1========================================================================
void MCExerciseJ1::WrapUp()
{
  fMMin = MCAliExtractor::Instance().GetMMin();
  fMMax = MCAliExtractor::Instance().GetMMax();
  Int_t massBin1 = fhJPsi->FindBin(fMMin + 0.01);
  Int_t massBin2 = fhJPsi->FindBin(fMMax - 0.01);
  fNJPsi = fhJPsi->Integral(massBin1, massBin2);
  Double_t bg = (fhEleEle->Integral(massBin1, massBin2)) + (fhPosPos->Integral(massBin1, massBin2));
  if (fNJPsi + bg > 0.)
    fSignificance = fNJPsi / (TMath::Sqrt(fNJPsi + bg));
  if (bg > 0.0)
    fSoB = 1. * fNJPsi / bg;
  else if (fNJPsi > 0.0)
    fSoB = 9999.;

  if (fLine[4])
    delete fLine[4];
  if (fLine[5])
    delete fLine[5];
  fLine[4] = new TLine;
  fLine[4]->SetX1(fMMin);
  fLine[4]->SetY1(-100);
  fLine[4]->SetX2(fMMin);
  fLine[4]->SetY2(100);
  fLine[4]->SetLineColor(1);
  fLine[4]->SetLineStyle(5);

  fLine[5] = new TLine;
  fLine[5]->SetX1(fMMax);
  fLine[5]->SetY1(-100);
  fLine[5]->SetX2(fMMax);
  fLine[5]->SetY2(100);
  fLine[5]->SetLineColor(1);
  fLine[5]->SetLineStyle(5);

  fPad->cd(2);
  fLine[4]->Draw();
  fLine[5]->Draw();
  fPad->cd(2)->Update();

  MCAliExtractor::Instance().SetAll(fMMin, fMMax, fNJPsi, fSignificance, fSoB);

  std::ofstream myresult;
  TString smyresult;
  myresult.open("masterclass.save", std::ofstream::out | std::ofstream::app);
  myresult << "Momentum region:       " << fPMin << " < p (GeV/c) < " << fPMax << std::endl;
  smyresult.Append(Form("\n Momentum region:          %4.1f < p (GeV/c) < %4.1f", fPMin, fPMax));
  myresult << "Specific energy loss:  " << fDeMin << " <   dE/dx   < " << fDeMax << std::endl;
  smyresult.Append(Form("\n Specific energy loss:     %4.1f  <   dE/dx    < %4.1f", fDeMin, fDeMax));
  myresult << "Invariant mass window: " << fMMin << " < m (GeV/c2) < " << fMMax << std::endl;
  smyresult.Append(Form("\n Invariant mass window: %4.1f < m (GeV/c2) < %4.1f", fMMin, fMMax));
  myresult << "Number of events:      " << fNEvents << std::endl;
  smyresult.Append(Form("\n Number of events:      %d", fNEvents));
  myresult << "Number of Jpsi's:      " << fNJPsi << std::endl;
  smyresult.Append(Form("\n Number of Jpsi's:      %d", fNJPsi));
  myresult << "Signal/Background:     " << fSoB << std::endl;
  smyresult.Append(Form("\n Signal/Background:     %4.1f", fSoB));
  myresult << "Significance:          " << fSignificance << std::endl;
  smyresult.Append(Form("\n Significance:           %4.1f", fSignificance));
  myresult << std::endl;
  myresult.close();
  new TGMsgBox(gClient->GetRoot(), fSMF, "My Analysis result", smyresult, kMBIconAsterisk, kMBOk, 0, kVerticalFrame, kTextLeft);
}

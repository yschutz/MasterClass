#include "MCAliCalculator.h"
#include "MCMain.h"
#include "MCExerciseS1.h"
#include "MCMultiLingual.h"
#include "MCMultiView.h"
#include "MCVSDReader.h"

#include <TApplication.h>
#include <TCanvas.h>
#include <TEveBrowser.h>
#include <TEveElement.h>
#include <TEveEventManager.h>
#include <TEveGeoShape.h>
#include <TEveGeoShapeExtract.h>
#include <TEveLine.h>
#include <TEveManager.h>
#include <TEvePointSet.h>
#include <TEveScene.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveVSD.h>
#include <TEveViewer.h>
#include <TEveWindowManager.h>
#include <TFile.h>
#include <TG3DLine.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGFileDialog.h>
#include <TGLViewer.h>
#include <TGLOverlayButton.h>
#include <TGMsgBox.h>
#include <TGNumberEntry.h>
#include <TGeoUniformMagField.h>
#include <TGTab.h>
#include <TH1D.h>
#include <TList.h>
#include <TObjString.h>
#include <TRandom.h>
#include <TPRegexp.h>
#include <TROOT.h>
#include <TRootHelpDialog.h>
#include <TStyle.h>
#include <TSystem.h>
#include <TSystemDirectory.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

//=MCExerciseS1========================================================================
MCExerciseS1::MCExerciseS1(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
    fDataSetFileDir = "Data-Masterclass/events/Strangeness/Part1/";
    fDataTemplate = "AliVSD_MasterClass_pp_x.root";
    fGuiLogoName = "images/strangeness.png";
    MakeGui();
}

//=MCExerciseS1========================================================================
MCExerciseS1::~MCExerciseS1()
{
    fDataSetFileDir = "";
    fDataTemplate = "";
    fGuiLogoName = "";
    if (!fNeedsInit)
        DeleteHistograms();
    if (MCAliCalculator::Instance().IsStarted())
        MCAliCalculator::Instance().DoExit();
    if (fTrackListCascadeNeg)
    {
        fTrackListCascadeNeg->Delete();
        fTrackListCascadePos->Delete();
        fTrackListCascadeMother->Delete();
        fTrackListCascade->Delete();
        delete fPsCascade;
    }
    // if (fTrackList)
    //     delete fTrackList;
    if (fTrackListV0Neg)
    {
        fTrackListV0Neg->Delete();
        fTrackListV0Pos->Delete();
        fTrackListV0Mother->Delete();
        delete fPsV0;
    }

    if (fTIW)
        fTIW->UnmapWindow();
    if (fCSP)
        fCSP->UnmapWindow();
    if (fCSP2)
        fCSP2->UnmapWindow();
    // if (fInstructions)
    //     fInstructions->CloseWindow();
    if (fADI)
        fADI->UnmapWindow();
    if (fPAI)
        fPAI->UnmapWindow();
    if (fPAI2)
        fPAI2->UnmapWindow();
    if (fPAIW)
        fPAIW->UnmapWindow();
    if (fTIW)
        fTIW->UnmapWindow();
    if (fTMF)
        fTMF->UnmapWindow();
    if (fSMF)
        fSMF->UnmapWindow();
}

//=MCExerciseS1========================================================================
void MCExerciseS1::AutoSave()
{

    std::ofstream myresult(TString::Format("autosave_AfterEvt%02dinSet%02d.masterclass",
                                           MCVSDReader::Instance().CurEv() + 1, fDataSet));
    myresult << "Kaons" << std::endl;
    myresult << fCountKaons << std::endl;
    for (Int_t i = 0; i < fCountKaons; i++)
        myresult << fMinvKaon[i] << std::endl;

    myresult << "Lambdas" << std::endl;
    myresult << fCountLambdas << std::endl;
    for (Int_t i = 0; i < fCountLambdas; i++)
        myresult << fMinvLambda[i] << std::endl;

    myresult << "AntiLambdas" << std::endl;
    myresult << fCountALambdas << std::endl;
    for (Int_t i = 0; i < fCountALambdas; i++)
        myresult << fMinvALambda[i] << std::endl;

    myresult << "Xis" << std::endl;
    myresult << fCountXis << std::endl;
    for (Int_t i = 0; i < fCountXis; i++)
        myresult << fMinvXi[i] << std::endl;

    myresult << "Omegas" << std::endl;
    myresult << fCountOmegas << std::endl;
    for (Int_t i = 0; i < fCountOmegas; i++)
        myresult << fMinvOmega[i] << std::endl;

    myresult << "Others" << std::endl;
    myresult << 0 << std::endl;

    myresult.close();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::Calculator()
{
    // if (!MCAliCalculator::Instance().IsStarted())
    MCAliCalculator::Instance().Start(this);
    if (fRole == RO_STUDENT)
        MCAliCalculator::Instance().BlockButtons(kTRUE);
    else if (fRole == RO_DEMO)
        MCAliCalculator::Instance().BlockButtons(kFALSE);
}

//=MCExerciseS1========================================================================
void MCExerciseS1::CountStrangeParticles()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();

    fCSP = new TGMainFrame(gClient->GetRoot(), 100, 100);
    TGGroupFrame *groupF = new TGGroupFrame(fCSP, ml.tr("Strange Particles Statistics"));

    TGLabel *label = nullptr;

    Int_t nloads = 1;
    if (fLoads)
        nloads = fLoads;

    TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, ml.tr("Particle"));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, ml.tr("Monte-Carlo Data"));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, ml.tr("Real Data"));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupF->AddFrame(horiF);
    // kaons
    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "Kaons");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%4.1f", 13.4 * nloads));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%d", fCountKaons));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupF->AddFrame(horiF);
    // lambdas
    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "Lambdas");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%4.1f", 3.9 * nloads));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%d", fCountLambdas));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupF->AddFrame(horiF);
    // anti lambdas
    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "antiLambdas");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%4.1f", 3.5 * nloads));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%d", fCountALambdas));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupF->AddFrame(horiF);
    // Xis
    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "Xis");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%4.1f", 5.5 * nloads));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(horiF, TString::Format("%d", fCountXis));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupF->AddFrame(horiF);

    TGHorizontal3DLine *separator = new TGHorizontal3DLine(groupF);
    groupF->AddFrame(separator, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    groupF->AddFrame(horiF);

    TGTextButton *btext = new TGTextButton(groupF, ml.tr("Close"));
    btext->Connect("Clicked()", "TGMainFrame", fCSP, "UnmapWindow()");
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    fCSP->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    fCSP->SetWindowName(ml.tr("Strange Particles Statistics"));
    fCSP->MapSubwindows();
    fCSP->Resize(fCSP->GetDefaultSize());
    fCSP->MapWindow();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::CountStrangeParticles2()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();

    fCSP2 = new TGMainFrame(gClient->GetRoot(), 100, 100);
    fCSP2->DontCallClose();

    TGGroupFrame *groupF = new TGGroupFrame(fCSP2, ml.tr("Strange Particles Statistics"));
    Int_t nLoads = 1;
    if (fLoads)
        nLoads = fLoads;
    TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    TGLabel *label = new TGLabel(horiF, ml.tr("Particle"));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    label = new TGLabel(horiF, ml.tr("Real Data"));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "Kaons");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    label = new TGLabel(horiF, TString::Format("%d", fCountKaons));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "Lambdas");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    label = new TGLabel(horiF, TString::Format("%d", fCountLambdas));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "antiLambdas");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    label = new TGLabel(horiF, TString::Format("%d", fCountALambdas));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    horiF = new TGHorizontalFrame(groupF, 450, 20, kFixedWidth);
    label = new TGLabel(horiF, "Xis");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    label = new TGLabel(horiF, TString::Format("%d", fCountXis));
    horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    TGTextButton *btext = new TGTextButton(groupF, ml.tr("Close"));
    btext->Connect("Clicked()", "TGMainFrame", fCSP2, "UnmapWindow()");
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    fCSP2->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
    fCSP2->SetWindowName(ml.tr("Strange Particles Statistics"));
    fCSP2->MapSubwindows();
    fCSP2->Resize(fCSP2->GetDefaultSize());
    fCSP2->MapWindow();
}

//=MCExerciseS1========================================================================
void MCExerciseS1::CloseWindow()
{
}
//=MCExerciseS1========================================================================
void MCExerciseS1::CopyToCalc(Int_t i)
{
    if (MCAliCalculator::Instance().IsStarted())
        switch (i)
        {
        case 1:
            MCAliCalculator::Instance().Set1(fEntryX->GetNumber(), fEntryY->GetNumber(), fEntryZ->GetNumber(),
                                             fEntryM->GetNumber());
            break;
        case 2:
            MCAliCalculator::Instance().Set2(fEntryX->GetNumber(), fEntryY->GetNumber(), fEntryZ->GetNumber(),
                                             fEntryM->GetNumber());
            break;
        case 3:
            MCAliCalculator::Instance().Set3(fEntryX->GetNumber(), fEntryY->GetNumber(), fEntryZ->GetNumber(),
                                             fEntryM->GetNumber());
            break;
        default:
            break;
        }
    else
    {
        TString msgText = MCMultiLingual::Instance().tr("Start the calculator");
        new TGMsgBox(gClient->GetRoot(), fTIW, "MCExerciseS1::CopyToCalc1", msgText, kMBIconExclamation, kMBOk);
    }
}
//=MCExerciseS1========================================================================
void MCExerciseS1::CountEvents()
{
    fEventAnalysed->SetText(TString::Format("%02i", fCount + 1));
    fCount++;
    AutoSave();
    NextEvent();
}

//=MCExerciseS1========================================================================
void MCExerciseS1::CustomizeBrowser()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    TEveBrowser *browser = gEve->GetBrowser();

    if (fRole == RO_DEMO)
        browser->GetMainFrame()->SetWindowName(ml.tr("Demo"));
    else
        browser->GetMainFrame()->SetWindowName(ml.tr("Student"));

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
        btext->Connect("Clicked()", "MCExerciseS1", this, "Instructions()");
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
            bpict->Connect("Clicked()", "MCExerciseS1", this, "PrevEvent()");
            fEventNumber = new TGLabel(horiF);
            fEventNumber->SetText("hello");
            horiF->AddFrame(fEventNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

            bpict = new TGPictureButton(horiF, gClient->GetPicture(icondir + "GoForward.gif"));
            horiF->AddFrame(bpict, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
            bpict->Connect("Clicked()", "MCExerciseS1", this, "NextEvent()");
        }
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

        horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
        {
            btext = new TGTextButton(horiF, ml.tr("Event analysed!"));
            btext->Connect("Clicked()", "MCExerciseS1", this, "CountEvents()");
            horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

        horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, ml.tr("Events already analyzed:"));
            horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
            fEventAnalysed = new TGLabel(horiF);
            fEventAnalysed->SetText("0");
            horiF->AddFrame(fEventAnalysed, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fSMF, ml.tr("Strange Particles"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
        {
            btext = new TGTextButton(horiF, "V0s");
            btext->Connect("Clicked()", "MCExerciseS1", this, "DisplayV0s()");
            fCheckV0s = new TGCheckButton(horiF, "", 10);
            fCheckV0s->SetEnabled(kFALSE);
            horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
            horiF->AddFrame(fCheckV0s);
        }
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
        {
            btext = new TGTextButton(horiF, "Cascades");
            btext->Connect("Clicked()", "MCExerciseS1", this, "DisplayCascades()");
            fCheckCascades = new TGCheckButton(horiF, "", 10);
            fCheckCascades->SetEnabled(kFALSE);
            horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
            horiF->AddFrame(fCheckCascades);
        }
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fSMF, ml.tr("Calculator"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
        FontStruct_t buttonFont = font->GetFontStruct();
        btext = new TGTextButton(vertF, ml.tr("Calculator"));
        btext->SetTextColor(TColor::Number2Pixel(kGreen + 2));
        btext->SetFont(buttonFont);
        btext->Connect("Clicked()", "MCExerciseS1", this, "Calculator()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
        {
            btext = new TGTextButton(horiF, ml.tr("Table of Results"));
            btext->Connect("Clicked()", "MCExerciseS1", this, "CountStrangeParticles2()");
            horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fSMF, ml.tr("Display"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        // !!! VERTEX info not available in data !!!!

        // horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
        // btext = new TGTextButton(horiF, "Vertex");
        // btext->Connect("Clicked()", "MCExerciseS1", this, "DisplayVertex()");
        // fCheckVertex = new TGCheckButton(horiF, "", 10);
        // fCheckVertex->SetEnabled(kFALSE),
        // horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
        // horiF->AddFrame(fCheckVertex);
        // vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        // !!! CLUSTER info not available in data !!!!

        // horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
        // btext = new TGTextButton(horiF, ml.tr(CLUSTER));
        // btext->Connect("Clicked()", "MCExerciseS1", this, "DisplayClusters()");
        // fCheckClusters = new TGCheckButton(horiF, "", 10);
        // fCheckClusters->SetEnabled(kFALSE),
        // horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
        // horiF->AddFrame(fCheckClusters);
        // vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
        btext = new TGTextButton(horiF, ml.tr("Tracks"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "DisplayTracks()");
        fCheckESDTracks = new TGCheckButton(horiF, "", 10);
        fCheckESDTracks->SetEnabled(kFALSE),
            horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
        horiF->AddFrame(fCheckESDTracks);
        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
        btext = new TGTextButton(horiF, ml.tr("Geometry"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "ChangeGeometry()");
        horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(horiF, "Axes");
        btext->Connect("Clicked()", "MCExerciseS1", this, "DisplayAxes()");
        horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(vertF, ml.tr("Screen Background"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "ChangeBackgroundColor()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fSMF, ml.tr("Encyclopaedia"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        btext = new TGTextButton(vertF, ml.tr("ALICE Detector"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "DetectorInfo()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(vertF, ml.tr("V0 Pattern"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "PatternInfo()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(vertF, "2.76 TeV PbPb");
        btext->Connect("Clicked()", "MCExerciseS1", this, "PatternInfo2()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fSMF->AddFrame(groupF);

    fSMF->MapSubwindows();
    fSMF->Resize();
    fSMF->MapWindow();
    browser->StopEmbedding(ml.tr("Student"));

    browser->StartEmbedding(TRootBrowser::kRight);
    // if (fStudentSetCanvas)
    //     delete fStudentSetCanvas;
    fStudentSetCanvas = new TCanvas();
    fStudentSetCanvas->Divide(2, 2);
    fStudentSetCanvas->cd(P_KAON + 1);
    fMinvHistK->Draw();

    fStudentSetCanvas->cd(P_XI + 1);
    fMinvHistX->Draw();

    fStudentSetCanvas->cd(P_LAMBDA + 1);
    fMinvHistL->Draw();

    fStudentSetCanvas->cd(P_ALAMBDA + 1);
    fMinvHistAL->Draw();

    browser->StopEmbedding(Form("%s S1", ml.tr("Mass")));
    browser->HideBottomTab();
    browser->GetTabRight()->SetTab(1);
}

//=MCExerciseS1========================================================================
void MCExerciseS1::DeleteHistograms()
{
    delete fMinvHistK;
    delete fMinvHistX;
    delete fMinvHistL;
    delete fMinvHistAL;
    delete fMinvHistALL;
}

//=MCExerciseS1========================================================================
void MCExerciseS1::DisplayCascades()
{
    if (fCascadesLoaded)
    {
        fCascadesLoaded = kFALSE;
        GotoEvent(MCVSDReader::Instance().CurEv());
        fCheckCascades->SetDisabledAndSelected(kFALSE);
    }
    else
    {
        fCascadesLoaded = kTRUE;
        GotoEvent(MCVSDReader::Instance().CurEv());
        fCheckCascades->SetDisabledAndSelected(kTRUE);
    }
}
//=MCExerciseS1========================================================================
void MCExerciseS1::DisplayV0s()
{
    if (fV0sLoaded)
    {
        fV0sLoaded = kFALSE;
        GotoEvent(MCVSDReader::Instance().CurEv());
        fCheckV0s->SetDisabledAndSelected(kFALSE);
    }
    else
    {
        fV0sLoaded = kTRUE;
        GotoEvent(MCVSDReader::Instance().CurEv());
        fCheckV0s->SetDisabledAndSelected(kTRUE);
    }
}

//=MCExerciseS1========================================================================
void MCExerciseS1::EveSelection(TEveElement *el)
{
    if (el->GetElementName() == TString::Format("ESD_Track"))
    {
        TEveTrack *track = dynamic_cast<TEveTrack *>(el);
        TrackInfoWindo(track->GetMomentum().fX, track->GetMomentum().fY, track->GetMomentum().fZ,
                       kfMassPion, track->GetCharge());
        return;
    }
    for (Int_t n = 0; n < fV0Tracks; n++)
    {
        if (el->GetElementName() == TString::Format("V0_Track_Neg_%d", n))
        {
            TrackInfoWindo(fPxNeg[n], fPyNeg[n], fPzNeg[n], fMassNeg[n], -1);
            return;
        }
        if (el->GetElementName() == TString::Format("V0_Track_Pos_%d", n))
        {
            TrackInfoWindo(fPxPos[n], fPyPos[n], fPzPos[n], fMassPos[n], 1);
            return;
        }
    }
    for (Int_t n = 0; n < fCascadeTracks; n++)
    {
        if (el->GetElementName() == TString::Format("Cascade_Track_Neg_%d", n))
        {
            TrackInfoWindo(fPxNegCas[n], fPyNegCas[n], fPzNegCas[n], fMassNegCas[n], -1);
            return;
        }
        if (el->GetElementName() == TString::Format("Cascade_Track_Pos_%d", n))
        {
            TrackInfoWindo(fPxPosCas[n], fPyPosCas[n], fPzPosCas[n], fMassPosCas[n], 1);
            return;
        }
        if (el->GetElementName() == TString::Format("Cascade_Track_%d", n))
        {
            TrackInfoWindo(fPxNegCas[n], fPyNegCas[n], fPzNegCas[n], fMassNegCas[n], -1);
            TrackInfoWindo(fPxPosCas[n], fPyPosCas[n], fPzPosCas[n], fMassPosCas[n], 1);
            return;
        }
    }
}

//=MCExerciseS1========================================================================
void MCExerciseS1::FoundPart(E_PATId part)
{
    if (!MCAliCalculator::Instance().HasChanged())
        return;
    Double_t minv = MCAliCalculator::Instance().GetInvMass();
    switch (part)
    {
    case P_KAON:

        fMinvHistK->Fill(minv);
        fMinvKaon[fCountKaons] = minv;
        fCountKaons++;
        break;
    case P_LAMBDA:
        fMinvHistL->Fill(MCAliCalculator::Instance().GetInvMass());
        fMinvLambda[fCountLambdas] = minv;
        fCountLambdas++;
        break;
    case P_ALAMBDA:
        fMinvHistAL->Fill(MCAliCalculator::Instance().GetInvMass());
        fMinvALambda[fCountALambdas] = minv;
        fCountALambdas++;
        break;
    case P_XI:
        fMinvHistX->Fill(MCAliCalculator::Instance().GetInvMass());
        fMinvXi[fCountXis] = minv;
        fCountXis++;
        break;
    default:
        break;
    }
    MCAliCalculator::Instance().SetChanged(kFALSE);
    MCAliCalculator::Instance().SetZero();
}

//=MCExerciseS1========================================================================
Bool_t MCExerciseS1::GotoEvent(Int_t evt)
{
    fTrackList = nullptr;
    MCAliCalculator::Instance().SetChanged(kTRUE);

    Bool_t rv = MCVSDReader::Instance().GotoEvent(evt);
    if (fEventNumber)
        fEventNumber->SetText(TString::Format("Evt %02i / %s %i",
                                              MCVSDReader::Instance().CurEv() + 1,
                                              MCMultiLingual::Instance().tr("Set"),
                                              fDataSet));
    if (fVertexLoaded)
        LoadVertex();
    if (fAxesLoaded)
        LoadAxes();
    if (fITSClustersLoaded)
        LoadClusters(fITSClusters, "ITS", 0);
    if (fTPCClustersLoaded)
        LoadClusters(fITSClusters, "TPC", 0);
    if (fTRDClustersLoaded)
        LoadClusters(fTRDClusters, "TRD", 0);
    if (fTOFClustersLoaded)
        LoadClusters(fTOFClusters, "TOF", 0);
    if (fEsdTracksLoaded)
        LoadEsdTracks(kfMaxRadius);
    if (fV0sLoaded)
        LoadV0s();
    if (fCascadesLoaded)
        LoadCascades();

    if (fOverlayButton && MCMultiView::Instance().IsSet())
        MCMultiView::Instance().Get3DView()->GetGLViewer()->RemoveOverlayElement((TGLOverlayElement *)fOverlayButton);
    if (fRole == RO_DEMO && MCMultiView::Instance().IsSet())
    {
        fOverlayButton = new TGLOverlayButton(MCMultiView::Instance().Get3DView()->GetGLViewer(), "", 0, 0, 200, 30);

        if (fEventNumber->GetText()->GetString() == TString::Format("1"))
            fOverlayButton->SetText("Kaon");
        if (fEventNumber->GetText()->GetString() == TString::Format("2"))
            fOverlayButton->SetText("Lambda");
        if (fEventNumber->GetText()->GetString() == TString::Format("3"))
            fOverlayButton->SetText("antiLambda");
        if (fEventNumber->GetText()->GetString() == TString::Format("4"))
            fOverlayButton->SetText("Xi");
    }
    if (MCMultiView::Instance().IsSet())
    {
        fOverlayButton1->SetPosition(0, MCMultiView::Instance().Get3DView()->GetEveFrame()->GetHeight() - 25);
        fOverlayButton2->SetPosition(0, MCMultiView::Instance().Get3DView()->GetEveFrame()->GetHeight() - 45);
        fOverlayButton3->SetPosition(0, MCMultiView::Instance().Get3DView()->GetEveFrame()->GetHeight() - 65);
    }

    TEveElement *top = gEve->GetCurrentEvent();

    if (MCMultiView::Instance().IsSet())
    {
        MCMultiView::Instance().DestroyEventRPhi();
        MCMultiView::Instance().ImportEventRPhi(top);
        MCMultiView::Instance().DestroyEventRhoZ();
        MCMultiView::Instance().ImportEventRhoZ(top);
    }

    return kTRUE;
}
// //=MCExerciseS1========================================================================
// void MCExerciseS1::HandleMenu(Int_t id)
// {
//     switch (id)
//     {
//     // case M_GO_EXIT:
//     // {
//     //     Int_t retval;
//     //     TString msgText = (ml.tr(S1_MSGEXIT));
//     //     new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseS1:Exit", msgText, kMBIconQuestion, kMBYes | kMBCancel, &retval);
//     //     if (retval == 1)
//     //         CloseWindow();
//     //     break;
//     // }
//     case M_GO_HELP:
//     {
//         TRootHelpDialog *ins = new TRootHelpDialog(gClient->GetRoot(), "", 750, 180);
//         ins->SetWindowName(ml.tr(S1_INTIT));
//         ins->SetText(ml.tr(S1_INTXT));
//         ins->Popup();
//         break;
//     }
//     default:
//     {
//         printf("Menu item %d selected\n", id);
//         break;
//     }
//     }
// }
//=MCExerciseS1========================================================================
void MCExerciseS1::Instructions()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    fInstructions = new TRootHelpDialog(gClient->GetRoot(), " ", 620, 400);
    fInstructions->DontCallClose();
    fInstructions->Popup();
    switch (fRole)
    {
    case RO_DEMO:
        fInstructions->SetWindowName(ml.tr("DEMO MODE INSTRUCTIONS"));
        fInstructions->SetText(ml.tr("\
Welcome to the <Demo> mode, here you see all the tools you need to analyse\n\
the ALICE Events. In this mode, there are only 4 events to analyze, and each of\n\
them contains a strange particle. The name of the particle is displayed in the\n\
bottom of the 3D View on the right-hand side\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side you have your steering board which is divided into segments:\n\
\
Instructions:\n\
   Instructions          - general instructions of how to use the application\n\
   Analysis Instructions - instructions for analysis\n\
\n\n\
\
Event Navigation:\n\
   Click on the arrows to go to the previous or to the next event.\n\
   The current event number and number of analysed events are also displayed.\n\n\
\
Strange Particles - this allows you to display or hide tracks from decays\n\
of strange particles:\n\
   V0s      - tracks produced from a V0 decays you will be looking for\n\
   Cascades - tracks from the cascade decays you will be looking for\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Calculator:\n\
   Calculator       - allows you to calculate the properties of strange particles like their\n\
   invariant mass given the daughter particles propreties\n\
   Table of Results - table with strange particle statistics \n\
   \n\
\
Display - this allows you to display or hide elements like:\n\
   Vertex     - the point where the collision took place\n\
   Clusters   - reconstructed points in the detectors showing where the particle was\n\
   Tracks     - reconstructed trajectories of the particles traversing the detectors\n\
   Geometry   - drawing of ALICE detectors\n\
   Axes       - reference system of ALICE Experiment\n\
   Background - choose the colour of the display panel\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Encyclopedia:\n\
   ALICE detector    - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)"));
        break;
    case RO_STUDENT:
        fInstructions->SetText(ml.tr("\
the ALICE Events.\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side there is your steering board which is divided into segments:\n\
\
Instructions:\n\
   Global Instructions   - general instructions of how to use the application\n\
   Analysis Instructions - instructions for analysis\n\
 \n\n\
\
Event Navigation:\n\
   Click on the arrows to go to the previous or to the next event.\n\
   The current event number and number of analysed events are also displayed.\n\n\
\
Strange Particles - this allows you to display or hide tracks from decays\n\
of strange particles:\n\
   V0s      - tracks produced from a V0 decays you will be looking for\n\
   Cascades - tracks from the cascade decays you will be looking for\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Calculator:\n\
   Calculator       - allows you to calculate the properties of strange particles like\n\
   invariant mass given the daughter particles characteristics\n\
   Table of Results - table with strange particle statistics \n\
  \n\
\
Display - this allows you to display or hide elements like:\n\
   Vertex     - the point where the collision took place\n\
   Clusters   - reconstucted points in the detectors showing where the particle was\n\
   Tracks     - reconstructed trajectories of the particles traversing the detectors\n\
   Geometry   - drawing of ALICE detectors\n\
   Background - choose the colour of the display panel\n\
   Axes       - reference system of ALICE Experiment\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Encyclopedia:\n\
   ALICE detector - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)"));
        fInstructions->SetWindowName(ml.tr("STUDENT MODE INSTRUCTIONS"));
        break;
    case RO_TUTOR:
        fInstructions->SetText(ml.tr("\
Welcome in the <Teacher> mode, here you can see the tools to merge and analyse\n\
your students' results\n\n\
\
On the left hand side panel you can see four panels, called <Teacher Instructions>,\n\
<Teacher Controls>, <Results> and <Encyclopaedia>:\n\n\
\
   Instructions - desribes how to operate the application in the <Teacher> mode\n\
   Teacher controls - allows you to merge and analayze students' results.\n\
      Get Files - this allows to get your students' results\n\
      Save Histograms - saves histograms showing your students' results\n\
   Results\n\
      Table of Results - shows and interprets the analysis results\n\
    \n\
Encyclopedia:\n\
   ALICE detector - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)\n\n\
\
Event animation:\n\
   Play/Stop - Watch the charged particles propagation through the detector\n\n\
\
\
In the bottom, there are main application control buttons:\n\
   Student Mode - starts the application in the Student mode,\n\
   Exit\n\n\
\
How to merge and analyze the students' results\n\n\
\
   1. Click the button <Get Files> and a window <Open> will appear.\n\
   2. Find the result (filetype *.masterclass) and click <Open>. The result\n\
      will be loaded. Repeat it for all the students' files\n\
   3. Click on the tab <Merged Invariant Mass> (right-hand side) to see\n\
      the invariant mass of the strange particles found by your students.\n\
      Update the histograms by clicking on each of them.\n\
   4. Click <Save Histograms>, give the title of the pictures and <Save>\n\
   5. Click <Table of Results>. A table called <Strange Particle Statistics> appears\n\
   6. Compare the pre-calculated results from Monte Carlo simulations with the\n\
      results of your students. Do the results agree? Yes? No? What Does it mean?\n\
   7. Close <Strange Particle Statistics> by clicking <Close>\n\
 \n\
   8. Click <Close>. The analysis is done!\n\n\
\
    "));
        fInstructions->SetWindowName(ml.tr("TUTOR MODE INSTRUCTIONS"));
        break;
    default:
        break;
    }
}
//=MCExerciseS1========================================================================
void MCExerciseS1::InstructionsLevel1()
{
    fInstructions = new TRootHelpDialog(gClient->GetRoot(), "MCExerciseS1::InstructionsLevel1", 620, 400);
    fInstructions->SetText(MCMultiLingual::Instance().tr("Analysis Instructions"));
    // fInstructions->DontCallClose();
    fInstructions->Popup();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::LoadCascades()
{
    if (MCVSDReader::Instance().GetEveVSD()->fTreeV0)
    {
        if (!fTrackListCascadeNeg)
        {
            fTrackListCascadeNeg = new TEveTrackList("Cascade Tracks Neg");
            fTrackListCascadeNeg->SetMainColor(kGreen - 4);
            fTrackListCascadeNeg->SetMarkerColor(kYellow);
            fTrackListCascadeNeg->SetMarkerStyle(10);
            fTrackListCascadeNeg->SetMarkerSize(5);
            fTrackListCascadeNeg->IncDenyDestroy();
        }
        else
            fTrackListCascadeNeg->DestroyElements();

        if (!fTrackListCascadePos)
        {
            fTrackListCascadePos = new TEveTrackList("Cascade Tracks Pos");
            fTrackListCascadePos->SetMainColor(2);
            fTrackListCascadePos->SetMarkerColor(kYellow);
            fTrackListCascadePos->SetMarkerStyle(10);
            fTrackListCascadePos->SetMarkerSize(5);
            fTrackListCascadePos->IncDenyDestroy();
        }
        else
            fTrackListCascadePos->DestroyElements();

        if (!fTrackListCascadeMother)
        {
            fTrackListCascadeMother = new TEveTrackList("Cascade Mother Tracks");
            fTrackListCascadeMother->SetMainColor(kSpring + 6);
            fTrackListCascadeMother->SetMarkerColor(kSpring + 6);
            fTrackListCascadeMother->SetMarkerStyle(10);
            fTrackListCascadeMother->SetMarkerSize(5);

            fTrackListCascadeMother->IncDenyDestroy();
        }
        else
            fTrackListCascadeNeg->DestroyElements();
        if (fPsCascade == 0)
        {
            fPsCascade = new TEvePointSet();
            fPsCascade->SetMainColor(1);
            fPsCascade->SetMarkerSize(2);
            fPsCascade->SetMarkerStyle(2);
            fPsCascade->IncDenyDestroy();
        }
        else
            fPsCascade->Reset();
        if (fTrackListCascade == 0)
        {
            fTrackListCascade = new TEveTrackList("Cascade Bachelor Tracks");
            fTrackListCascade->SetMainColor(kBlue);
            fTrackListCascade->SetMarkerColor(kYellow);
            fTrackListCascade->SetMarkerStyle(4);
            fTrackListCascade->SetMarkerSize(0.5);

            fTrackListCascade->IncDenyDestroy();
        }
        else
            fTrackListCascade->DestroyElements();
        TEveTrackPropagator *trkProp = fTrackListCascadeNeg->GetPropagator();
        trkProp->SetMagField(kfMafField);
        trkProp->SetMaxR(600); //R[cm]

        TEveTrackPropagator *trkProp2 = fTrackListCascadeMother->GetPropagator();
        trkProp2->SetMagField(kfMafField);

        TEveTrackPropagator *trkProp3 = fTrackListCascade->GetPropagator();
        trkProp3->SetMagField(kfMafField);
        trkProp3->SetMaxR(kfMaxRadius); //R[cm]

        MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(0);
        fCascadeTracks = MCVSDReader::Instance().GetEveVSD()->fR.fIndex;
        TEvePointSelector ss(MCVSDReader::Instance().GetEveVSD()->fTreeV0, fPsCascade, "fVCa.fX:fVCa.fY:fVCa.fZ");
        ss.Select();
        fPsCascade->SetTitle("Cascade_CA_Points");
        Int_t n = 0;
        for (Int_t i = 0; i < fCascadeTracks; i++)
        {
            MCVSDReader::Instance().GetEveVSD()->fTreeV0->GetEntry(n);
            Int_t pdg = MCVSDReader::Instance().GetEveVSD()->fV0.fPdg;
            Double_t massN, massP;
            switch (pdg)
            {
            case 3312:
                massN = kfMassPion;
                massP = kfMassProton;
                break;
            case -3312:
                massN = kfMassProton;
                massP = kfMassPion;
                break;
            default:
                massN = kfMassPion;
                massP = kfMassProton;
                break;
            }
            TEveRecTrack rcNeg;
            rcNeg.fP.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fPNeg);
            rcNeg.fV.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fVNeg);
            rcNeg.fStatus = MCVSDReader::Instance().GetEveVSD()->fV0.fStatus;
            rcNeg.fLabel = MCVSDReader::Instance().GetEveVSD()->fV0.fDLabel[0];
            Double_t momentum = MCVSDReader::Instance().GetEveVSD()->fV0.fPNeg.Mag();
            rcNeg.fBeta = momentum / TMath::Sqrt(momentum * momentum + TMath::C() * TMath::C() * massN * massN);
            rcNeg.fSign = -1;
            TEveRecTrack rcPos;
            rcPos.fP.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fPPos);
            rcPos.fV.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fVPos);
            rcPos.fStatus = MCVSDReader::Instance().GetEveVSD()->fV0.fStatus;
            rcPos.fLabel = MCVSDReader::Instance().GetEveVSD()->fV0.fDLabel[1];
            momentum = MCVSDReader::Instance().GetEveVSD()->fV0.fPPos.Mag();
            rcPos.fBeta = momentum / TMath::Sqrt(momentum * momentum + TMath::C() * TMath::C() * massP * massP);
            rcPos.fSign = 1;
            TEveRecTrack rcMother;
            rcMother.fP.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fPNeg + MCVSDReader::Instance().GetEveVSD()->fV0.fPPos);
            rcMother.fV.Set(0.0, 0.0, 0.0);
            rcMother.fStatus = MCVSDReader::Instance().GetEveVSD()->fV0.fStatus;
            rcMother.fLabel = MCVSDReader::Instance().GetEveVSD()->fV0.fLabel;
            rcMother.fBeta = 0.5;
            rcMother.fSign = 0;
            TEveTrack *track1 = new TEveTrack(&rcNeg, trkProp);
            track1->SetName(TString::Format("Cascade_Track_Neg_%d", n));
            track1->SetAttLineAttMarker(fTrackListCascadeNeg);
            fPxNegCas[n] = track1->GetMomentum().fX;
            fPyNegCas[n] = track1->GetMomentum().fY;
            fPzNegCas[n] = track1->GetMomentum().fZ;
            fMassNegCas[n] = massN;
            TEveTrack *track2 = new TEveTrack(&rcPos, trkProp);
            track2->SetName(TString::Format("Cascade_Track_Pos_%d", n));
            track2->SetAttLineAttMarker(fTrackListCascadePos);
            fPxPosCas[n] = track2->GetMomentum().fX;
            fPyPosCas[n] = track2->GetMomentum().fY;
            fPzPosCas[n] = track2->GetMomentum().fZ;
            fMassPosCas[n] = massP;
            fPcaXCas[n] = MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.fX;
            fPcaYCas[n] = MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.fY;
            fPcaZCas[n] = MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.fZ;
            trkProp2->SetMaxR(MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.Mag()); //R[cm]
            TEveTrack *track3 = new TEveTrack(&rcMother, trkProp2);
            track3->SetName(TString::Format("Cascade_Track_%d", n));
            fTrackListCascadeMother->AddElement(track3);
            fTrackListCascadePos->AddElement(track2);
            fTrackListCascadeNeg->AddElement(track1);
            MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(n);
            TEveTrack *track = new TEveTrack(&MCVSDReader::Instance().GetEveVSD()->fR, trkProp3);
            track->SetName(Form("Cascade_Bachelor_Track_%d", n));
            fPxBacCas[n] = track->GetMomentum().fX;
            fPyBacCas[n] = track->GetMomentum().fY;
            fPzBacCas[n] = track->GetMomentum().fZ;
            fPcaXBacCas[n] = track->GetVertex().fX;
            fPcaYBacCas[n] = track->GetVertex().fY;
            fPcaZBacCas[n] = track->GetVertex().fZ;
            fMassBacCas[n] = kfMassPion;
            fChargeBacCas[n] = -1;
            track->SetStdTitle();
            track->SetAttLineAttMarker(fTrackListCascade);
            fTrackListCascade->AddElement(track);
            momentum = TMath::Sqrt((fPxNegCas[n] + fPxPosCas[n] + fPxBacCas[n]) * (fPxNegCas[n] + fPxPosCas[n] + fPxBacCas[n]) +
                                   (fPyNegCas[n] + fPyPosCas[n] + fPyBacCas[n]) * (fPyNegCas[n] + fPyPosCas[n] + fPyBacCas[n]) +
                                   (fPyNegCas[n] + fPyPosCas[n] + fPzBacCas[n]) * (fPyNegCas[n] + fPyPosCas[n] + fPzBacCas[n]));
            Double_t direction[3] = {(fPxNegCas[n] + fPxPosCas[n] + fPxBacCas[n]) / momentum,
                                     (fPyNegCas[n] + fPyPosCas[n] + fPyBacCas[n]) / momentum,
                                     (fPzNegCas[n] + fPzPosCas[n] + fPzBacCas[n]) / momentum};
            fPointingLine1[n] = new TEveLine(TString::Format("Cascade_Line_1_%d", n));
            fPointingLine1[n]->SetPoint(0, 0.0, fPcaYBacCas[n] - fPcaXBacCas[n] * direction[1] / direction[0],
                                        fPcaZBacCas[n] - fPcaXBacCas[n] * direction[2] / direction[0]);
            fPointingLine1[n]->SetPoint(1, fPcaXBacCas[n], fPcaYBacCas[n], fPcaZBacCas[n]);
            fPointingLine1[n]->SetLineColor(kSpring + 6);
            fPointingLine1[n]->SetLineWidth(2);
            fPointingLine1[n]->SetLineStyle(2);
            gEve->AddElement(fPointingLine1[n]);
            n++;
        }
        fTrackListCascadeMother->MakeTracks();
        fTrackListCascadePos->MakeTracks();
        fTrackListCascadeNeg->MakeTracks();
        fTrackListCascade->MakeTracks();

        gEve->AddElement(fTrackListCascadePos);
        gEve->AddElement(fTrackListCascadeNeg);
        gEve->AddElement(fTrackListCascade);

        TEveElement *top = gEve->GetCurrentEvent();

        MCMultiView::Instance().DestroyEventRPhi();
        MCMultiView::Instance().ImportEventRPhi(top);

        MCMultiView::Instance().DestroyEventRhoZ();
        MCMultiView::Instance().ImportEventRhoZ(top);
        gEve->Redraw3D(kFALSE, kTRUE);
    }
}

//=MCExerciseS1========================================================================
void MCExerciseS1::LoadSavedResults()
{
    TGFileInfo fi;
    fi.fFileTypes = fMasterClassOutputData;
    fi.fIniDir = StrDup("");
    fi.fFileTypeIdx = 0;
    fi.fOverwrite = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(), 0, kFDOpen, &fi);
    if (!fi.fFilename)
        return;
    TPMERegexp filere(".*/([^/]+$)");
    if (filere.Match(fi.fFilename) != 2)
        return;
    gSystem->ChangeDirectory(fi.fIniDir);
    std::ifstream data(fi.fFilename);
    if (!data.is_open())
        return;
    Int_t nParticles = 0;
    std::string line;
    //Kaons
    getline(data, line);
    data >> nParticles;
    getline(data, line);
    fCountKaons += nParticles;
    Double_t minv = 0.0;
    for (Int_t i = 0; i < nParticles; i++)
    {
        data >> minv;
        getline(data, line);
        fMinvHistK->Fill(minv);
    }
    // Lambdas
    getline(data, line);
    data >> nParticles;
    getline(data, line);
    fCountLambdas += nParticles;
    for (Int_t i = 0; i < nParticles; i++)
    {
        data >> minv;
        getline(data, line);
        fMinvHistL->Fill(minv);
    }
    // Antilambdas
    getline(data, line);
    data >> nParticles;
    getline(data, line);
    fCountALambdas += nParticles;
    for (Int_t i = 0; i < nParticles; i++)
    {
        data >> minv;
        getline(data, line);
        fMinvHistAL->Fill(minv);
    }
    // Xis
    getline(data, line);
    data >> nParticles;
    getline(data, line);
    fCountXis += nParticles;
    for (Int_t i = 0; i < nParticles; i++)
    {
        data >> minv;
        getline(data, line);
        fMinvHistX->Fill(minv);
    }
    data.close();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::LoadEsdTracks(Int_t maxR)
{
    if (!fTrackList)
    {
        fTrackList = new TEveTrackList("ESD Tracks");
        fTrackList->SetMainColor(40);
        fTrackList->SetMarkerColor(kYellow);
        fTrackList->SetMarkerStyle(4);
        fTrackList->SetMarkerSize(0.5);
        fTrackList->IncDenyDestroy();
    }
    else
        fTrackList->DestroyElements();
    TEveTrackPropagator *trkProp = fTrackList->GetPropagator();
    trkProp->SetMagField(kfMafField);
    trkProp->SetMaxR(maxR);

    MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(0);

    fCascadeTracks = MCVSDReader::Instance().GetEveVSD()->fR.fIndex;
    Int_t n = 0;
    for (Int_t i = fCascadeTracks; i < MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntries(); ++i)
    {
        MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(n);
        TEveTrack *track = new TEveTrack(&MCVSDReader::Instance().GetEveVSD()->fR, trkProp);
        track->SetName("ESD_Track");
        track->SetTitle("ESD_Track");
        track->SetAttLineAttMarker(fTrackList);
        fTrackList->AddElement(track);
        n++;
    }
    fTrackList->MakeTracks();
    gEve->AddElement(fTrackList);
}

//=MCExerciseS1========================================================================
void MCExerciseS1::LoadV0s()
{
    if (MCVSDReader::Instance().GetEveVSD()->fTreeV0)
    {
        if (!fTrackListV0Neg)
        {
            fTrackListV0Neg = new TEveTrackList("V0 Tracks Neg");
            fTrackListV0Neg->SetMainColor(kGreen - 4);
            fTrackListV0Neg->SetMarkerColor(kYellow);
            fTrackListV0Neg->SetMarkerStyle(10);
            fTrackListV0Neg->SetMarkerSize(5);
            fTrackListV0Neg->IncDenyDestroy();
        }
        else
            fTrackListV0Neg->DestroyElements();
        if (!fTrackListV0Pos)
        {
            fTrackListV0Pos = new TEveTrackList("V0 Tracks Pos");
            fTrackListV0Pos->SetMainColor(2);
            fTrackListV0Pos->SetMarkerColor(kYellow);
            fTrackListV0Pos->SetMarkerStyle(10);
            fTrackListV0Pos->SetMarkerSize(5);
            fTrackListV0Pos->IncDenyDestroy();
        }
        else
            fTrackListV0Pos->DestroyElements();
        if (!fTrackListV0Mother)
        {
            fTrackListV0Mother = new TEveTrackList("V0 Mother Tracks");
            fTrackListV0Mother->SetMainColor(kSpring + 6);
            fTrackListV0Mother->SetMarkerColor(kSpring + 6);
            fTrackListV0Mother->SetMarkerStyle(10);
            fTrackListV0Mother->SetMarkerSize(5);
            fTrackListV0Mother->IncDenyDestroy();
        }
        else
            fTrackListV0Mother->DestroyElements();
        if (fPsV0 == 0)
        {
            fPsV0 = new TEvePointSet();
            fPsV0->SetMainColor(14);
            fPsV0->SetMarkerSize(0.5);
            fPsV0->SetMarkerStyle(2);
            fPsV0->IncDenyDestroy();
        }
        else
            fPsV0->Reset();

        TEveTrackPropagator *trkProp = fTrackListV0Neg->GetPropagator();
        trkProp->SetMagField(0.5);
        trkProp->SetMaxR(600); //R[cm]

        TEveTrackPropagator *trkProp2 = fTrackListV0Mother->GetPropagator();
        trkProp2->SetMagField(0.5);

        fV0Tracks = MCVSDReader::Instance().GetEveVSD()->fTreeV0->GetEntries();
        fCascadeTracks = MCVSDReader::Instance().GetEveVSD()->fR.fIndex;

        TEvePointSelector ss(MCVSDReader::Instance().GetEveVSD()->fTreeV0, fPsV0, "fVCa.fX:fVCa.fY:fVCa.fZ");
        ss.Select();
        fPsV0->SetTitle("V0_CA_Points");
        Int_t n = 0;
        for (Int_t i = fCascadeTracks; i < fV0Tracks; i++)
        {
            MCVSDReader::Instance().GetEveVSD()->fTreeV0->GetEntry(n);
            Int_t pdg = MCVSDReader::Instance().GetEveVSD()->fV0.fPdg;
            Double_t massN = 0.0, massP = 0.0;
            switch (pdg)
            {
            case 0:
                massN = kfMassElectron;
                massP = kfMassElectron;
                break;
            case 310:
                massN = kfMassPion;
                massP = kfMassPion;
                break;
            case 3122:
                massN = kfMassPion;
                massP = kfMassProton;
                break;
            case -3122:
                massN = kfMassProton;
                massP = kfMassPion;
                break;
            default:
                massN = kfMassPion;
                massP = kfMassPion;
                break;
            }
            TEveRecTrack rcPos, rcNeg, rcMother, rcPointing;
            rcNeg.fP.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fPNeg);
            rcNeg.fV.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fVNeg);
            rcNeg.fStatus = MCVSDReader::Instance().GetEveVSD()->fV0.fStatus;
            rcNeg.fLabel = MCVSDReader::Instance().GetEveVSD()->fV0.fDLabel[0];
            Double_t momentum = MCVSDReader::Instance().GetEveVSD()->fV0.fPNeg.Mag();
            rcNeg.fBeta = momentum / TMath::Sqrt(momentum * momentum + TMath::C() * TMath::C() * massN * massN);
            rcNeg.fSign = -1;

            rcPos.fP.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fPPos);
            rcPos.fV.Set(MCVSDReader::Instance().GetEveVSD()->fV0.fVPos);
            rcPos.fStatus = MCVSDReader::Instance().GetEveVSD()->fV0.fStatus;
            rcPos.fLabel = MCVSDReader::Instance().GetEveVSD()->fV0.fDLabel[1];
            momentum = MCVSDReader::Instance().GetEveVSD()->fV0.fPPos.Mag();
            rcPos.fBeta = momentum / TMath::Sqrt(momentum * momentum + TMath::C() * TMath::C() * massP * massP);
            rcPos.fSign = 1;

            TEveTrack *track1 = new TEveTrack(&rcNeg, trkProp);
            track1->SetName(TString::Format("V0_Track_Neg_%d", n));
            track1->SetAttLineAttMarker(fTrackListV0Neg);

            fPxNeg[n] = track1->GetMomentum().fX;
            fPyNeg[n] = track1->GetMomentum().fY;
            fPzNeg[n] = track1->GetMomentum().fZ;
            fMassNeg[n] = massN;

            TEveTrack *track2 = new TEveTrack(&rcPos, trkProp);
            track2->SetName(TString::Format("V0_Track_Pos_%d", n));
            track2->SetAttLineAttMarker(fTrackListV0Pos);

            fPxPos[n] = track2->GetMomentum().fX;
            fPyPos[n] = track2->GetMomentum().fY;
            fPzPos[n] = track2->GetMomentum().fZ;
            fMassPos[n] = massP;

            fPcaX[n] = MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.fX;
            fPcaY[n] = MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.fY;
            fPcaZ[n] = MCVSDReader::Instance().GetEveVSD()->fV0.fVCa.fZ;

            momentum = TMath::Sqrt((fPxNeg[n] + fPxPos[n]) * (fPxNeg[n] + fPxPos[n]) + (fPyNeg[n] + fPyPos[n]) * (fPyNeg[n] + fPyPos[n]) + (fPyNeg[n] + fPyPos[n]) * (fPyNeg[n] + fPyPos[n]));
            Double_t direction[3] = {(fPxNeg[n] + fPxPos[n]) / momentum,
                                     (fPyNeg[n] + fPyPos[n]) / momentum,
                                     (fPzNeg[n] + fPzPos[n]) / momentum};
            fPointingLine[n] = new TEveLine(TString::Format("V0_Pointing_Line_%d", n));
            fPointingLine[n]->SetPoint(0, 0.0,
                                       fPcaY[n] - fPcaX[n] * direction[1] / direction[0],
                                       fPcaZ[n] - fPcaX[n] * direction[2] / direction[0]);
            fPointingLine[n]->SetPoint(1, fPcaX[n], fPcaY[n], fPcaZ[n]);
            fPointingLine[n]->SetLineColor(kSpring + 6);
            fPointingLine[n]->SetLineWidth(2);
            fPointingLine[n]->SetLineStyle(2);
            gEve->AddElement(fPointingLine[n]);
            fTrackListV0Pos->AddElement(track2);
            fTrackListV0Neg->AddElement(track1);
            n++;
        }
        fTrackListV0Pos->MakeTracks();
        fTrackListV0Neg->MakeTracks();

        gEve->AddElement(fPsV0);
        gEve->AddElement(fTrackListV0Pos);
        gEve->AddElement(fTrackListV0Neg);
        TEveEventManager *top = gEve->GetCurrentEvent();

        if (MCMultiView::Instance().IsSet())
        {
            MCMultiView::Instance().DestroyEventRPhi();
            MCMultiView::Instance().ImportEventRPhi(top);

            MCMultiView::Instance().DestroyEventRhoZ();
            MCMultiView::Instance().ImportEventRhoZ(top);
        }
        gEve->Redraw3D(kFALSE, kTRUE);
    }
}

//=MCExerciseS1========================================================================
void MCExerciseS1::MakeBiggerDet()
{
    fADI = new TGMainFrame(gClient->GetRoot(), 100, 100);
    fADI->DontCallClose();
    TGGroupFrame *groupF = nullptr;
    TGHorizontalFrame *horiF = nullptr;
    fPictButton = nullptr;

    switch (fDetector)
    {
    case D_ALICE:
        groupF = new TGGroupFrame(fADI, MCMultiLingual::Instance().tr("ALICE Detector"));
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/ALICE.png"));
        fADI->SetWindowName("ALICE Info");
        break;
    case D_ITS:
        groupF = new TGGroupFrame(fADI, "ITS");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/ITS.png"));
        fADI->SetWindowName("ITS Info");
        break;
    case D_TPC:
        groupF = new TGGroupFrame(fADI, "TPC");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/TPC.png"));
        fADI->SetWindowName("TPC Info");
        break;
    case D_TRDTOF:
        groupF = new TGGroupFrame(fADI, "TRD & TOF");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/TRDTOF.png"));
        fADI->SetWindowName("TRD & TOF Info");
        break;
    case D_PHOS:
        groupF = new TGGroupFrame(fADI, "PHOS");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/PHOS.png"));
        fADI->SetWindowName("PHOS Info");
        break;
    case D_HMPID:
        groupF = new TGGroupFrame(fADI, "HMPID");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/HMPID.png"));
        fADI->SetWindowName("HMPID Info");
        break;
    default:
        groupF = new TGGroupFrame(fADI, MCMultiLingual::Instance().tr("ALICE Detector"));
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/ALICE.png"));
        fADI->SetWindowName("ALICE Info");
        break;
    }
    fPictButton->Connect("Clicked()", "TGMainFrame", fADI, "UnmapWindow()");
    horiF->AddFrame(fPictButton, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);
    fADI->AddFrame(groupF);
    fADI->MapSubwindows();
    fADI->Resize(fADI->GetDefaultSize());
    fADI->MapWindow();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::MakeBiggerPat()
{
    fPAIW = new TGMainFrame(gClient->GetRoot(), 100, 100);
    fPictButton = nullptr;
    TGGroupFrame *groupF = nullptr;
    TGHorizontalFrame *horiF = nullptr;
    switch (fPattern)
    {
    case P_KAON:
        groupF = new TGGroupFrame(fPAIW, "Kaon");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/kaon.png"));
        fPAIW->SetWindowName("Kaon");
        break;
    case P_LAMBDA:
        groupF = new TGGroupFrame(fPAIW, "Lambda");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/lambda.png"));
        fPAIW->SetWindowName("Lambda");
        break;
    case P_ALAMBDA:
        groupF = new TGGroupFrame(fPAIW, "anti Lambda");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/antilambda.png"));
        fPAIW->SetWindowName("anti Lambda");
        break;
    case P_XI:
        groupF = new TGGroupFrame(fPAIW, "Xi");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/xi.png"));
        fPAIW->SetWindowName("Xi");
        break;
    default:
        groupF = new TGGroupFrame(fPAIW, "Kaon");
        horiF = new TGHorizontalFrame(groupF, 250, 250);
        fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/kaon.png"));
        fPAIW->SetWindowName("Kaon");
        break;
    }
    fPictButton->Connect("Clicked()", "TGMainFrame", fPAIW, "UnmapWindow()");
    horiF->AddFrame(fPictButton, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);
    fPAIW->AddFrame(groupF);
    fPAIW->MapSubwindows();
    fPAIW->Resize(fPAIW->GetDefaultSize());
    fPAIW->MapWindow();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::MakeBiggerPat2()
{
}

//=MCExerciseS1========================================================================
void MCExerciseS1::MakeHistograms()
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

//=MCExerciseS1========================================================================
void MCExerciseS1::NextEvent()
{
    MCVSDReader &reader = MCVSDReader::Instance();
    MCMultiLingual &ml = MCMultiLingual::Instance();

    if (reader.CurEv() == reader.MaxEv() - 1)
    {
        TString msgText = Form("%s %d", ml.tr("Last event"), reader.MaxEv());
        new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseS1:Data", msgText, kMBIconExclamation, kMBOk);
        return;
    }
    else
        GotoEvent(reader.CurEv() + 1);
    if (MCAliCalculator::Instance().IsStarted())
        MCAliCalculator::Instance().SetZero();
}
// //=MCExerciseS1========================================================================
// Bool_t MCExerciseS1::OpenDataFile()
// {
//     Bool_t rv = kFALSE;
//     if (fRole == RO_STUDENT)
//     {
//         fDataSet = fData->GetSelected() + 1;
//         TString dataName(fDataTemplate);
//         if (fDataSet == 101)
//             dataName = "AliVSD_MasterClass_PbPb_101.root";
//         else
//             // dataName = Form("AliVSD_MasterClass_pp_%i.root", fDataSet);
//             dataName.ReplaceAll("x", Form("%i", fDataSet));

//         fDataSetFileName = fDataSetFileDir + dataName;
//     }
//     else if (fRole == RO_DEMO)
//     {
//         fDataSetFileName = fDataSetFileDir + "AliVSD_example.root";
//     }
//     if (!TFile::Open(fDataSetFileName))
//     {
//         TString msgText(Form("Event file %s not found! \nCheck your installation", fDataSetFileName.Data()));
//         new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseS1:Data", msgText, kMBIconExclamation, kMBOk);
//         gApplication->Terminate();
//     }
//     else
//         rv = kTRUE;
//     return rv;
// }
//=MCExerciseS1========================================================================
void MCExerciseS1::PatternInfo()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    fPAI = new TGMainFrame(gClient->GetRoot(), 100, 100);
    fPAI->DontCallClose();
    TGGroupFrame *groupF = new TGGroupFrame(fPAI, ml.tr("Patterns"));
    TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 250, 250);
    fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/kaon_small.png"));
    fPictButton->Connect("Clicked()", "MCExerciseS1", this, "MakeBiggerPat()");
    horiF->AddFrame(fPictButton, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    horiF = new TGHorizontalFrame(groupF);
    TGTextButton *btxt = nullptr;
    {
        btxt = new TGTextButton(horiF, "Kaon");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat(Int_t pat = P_KAON");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btxt = new TGTextButton(horiF, "Lambda");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat(Int_t pat = P_LAMBDA");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btxt = new TGTextButton(horiF, "anti Lambda");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat(Int_t pat = P_ALAMBDA");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btxt = new TGTextButton(horiF, "Xi");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat(Int_t pat = P_XI");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, kLHintsCenterY));

    horiF = new TGHorizontalFrame(groupF);
    {
        btxt = new TGTextButton(horiF, ml.tr("Close"));
        btxt->Connect("Clicked()", "TGMainFrame", fPAI, "UnmapWindow()");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fPAI->AddFrame(groupF);
    fPAI->SetWindowName(ml.tr("Patterns Info"));
    fPAI->MapSubwindows();
    fPAI->Resize(fPAI->GetDefaultSize());
    fPAI->MapWindow();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::PatternInfo2()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    fPAI2 = new TGMainFrame(gClient->GetRoot(), 100, 100);
    fPAI2->DontCallClose();
    TGGroupFrame *groupF = new TGGroupFrame(fPAI2, ml.tr("Patterns"));
    TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 250, 250);
    fPictButton = new TGPictureButton(horiF, gClient->GetPicture("images/lambda_2.png"));
    fPictButton->Connect("Clicked()", "MCExerciseS1", this, "MakeBiggerPat2()");
    horiF->AddFrame(fPictButton, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    groupF->AddFrame(horiF);

    horiF = new TGHorizontalFrame(groupF);
    TGTextButton *btxt = nullptr;
    {
        btxt = new TGTextButton(horiF, "Kaon");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat2(bpic, Int_t pat = P_KAON");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btxt = new TGTextButton(horiF, "Lambda");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat2(Int_t pat = P_LAMBDA");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btxt = new TGTextButton(horiF, "Xi");
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowPat2(Int_t pat = P_XI");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, kLHintsCenterY));

    horiF = new TGHorizontalFrame(groupF);
    {
        btxt = new TGTextButton(horiF, ml.tr("Close"));
        btxt->Connect("Clicked()", "TGMainFrame", fPAI2, "UnmapWindow()");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fPAI2->AddFrame(groupF);
    fPAI2->SetWindowName(ml.tr("Patterns Info"));
    fPAI2->MapSubwindows();
    fPAI2->Resize(fPAI2->GetDefaultSize());
    fPAI2->MapWindow();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::PrevEvent()
{
    Int_t currentEvent = MCVSDReader::Instance().CurEv();
    if (currentEvent == 0)
        return;
    else
        GotoEvent(currentEvent - 1);
    if (MCAliCalculator::Instance().IsStarted())
        MCAliCalculator::Instance().SetZero();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::SaveResults()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();

    if (fCountKaons + fCountLambdas + fCountALambdas + fCountXis == 0)
    {
        TString msgText = ml.tr("Nothing to save");
        new TGMsgBox(gClient->GetRoot(), 0, "MCExerciseS1:SaveResults", msgText, kMBIconExclamation, kMBOk);
        return;
    }
    // TGFileInfo fi;
    // fi.fFileTypes   = fMasterClassOutputData;
    // fi.fIniDir      =  StrDup("");
    // fi.fFileTypeIdx = 0;
    // fi.fOverwrite   = kTRUE;
    // new TGFileDialog(gClient->GetDefaultRoot(), 0, kFDOpen, &fi);
    // if (!fi.fFilename)
    //     return;
    TString outputFilename(Form("DataSet_%d", fDataSet));
    // TPMERegexp filere(".*/([^/]+$)");
    // if (filere.Match(outputFilename) != 2)
    //     return;
    gSystem->ChangeDirectory(StrDup(""));
    std::ofstream myresult(TString::Format("%s.masterclass", outputFilename.Data()));
    // Kaons
    myresult << "Kaons" << std::endl;
    myresult << fCountKaons << std::endl;
    for (Int_t i = 0; i < fCountKaons; i++)
        myresult << fMinvKaon[i] << " " << 0. << std::endl;

    // Lambdas
    myresult << "Lambdas" << std::endl;
    myresult << fCountLambdas << std::endl;
    for (Int_t i = 0; i < fCountLambdas; i++)
        myresult << fMinvLambda[i] << " " << 0. << std::endl;

    // AntiLambdas
    myresult << "AntiLambdas" << std::endl;
    myresult << fCountALambdas << std::endl;
    for (Int_t i = 0; i < fCountALambdas; i++)
        myresult << fMinvALambda[i] << " " << 0. << std::endl;

    // Xis
    myresult << "Xis" << std::endl;
    myresult << fCountXis << std::endl;
    for (Int_t i = 0; i < fCountXis; i++)
        myresult << fMinvXi[i] << " " << 0. << std::endl;

    myresult.close();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::SaveHistograms()
{
    TGFileInfo infoF;
    infoF.fFileTypes = fMasterClassOutputPicture;
    infoF.fIniDir = StrDup("");
    infoF.fFileTypeIdx = 0;
    infoF.fOverwrite = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(), gEve->GetMainWindow(), kFDSave, &infoF);
    if (!infoF.fFilename)
        return;

    TPMERegexp filere(".*/([^/]+$)");
    if (filere.Match(infoF.fFilename) != 2)
    {
        Warning("AliEvePopupHandler", "file '%s' bad.", infoF.fFilename);
        return;
    }
    gSystem->ChangeDirectory(infoF.fIniDir);
    if (fTutorSetCanvas)
        fTutorSetCanvas->SaveAs(TString::Format("%s_Invariant_Mass.png", infoF.fFilename));
}

//=MCExerciseS1========================================================================
void MCExerciseS1::ShowDet(Int_t det)
{
    TString image;
    switch (det)
    {
    case D_ALICE:
        image = "images/ALICE_small2.png";
        fDetector = D_ALICE;
        break;
    case D_ITS:
        image = "images/ITS_small.png";
        fDetector = D_ITS;
        break;
    case D_TPC:
        image = "images/TPC_small.png";
        fDetector = D_TPC;
        break;
    case D_TRDTOF:
        image = "images/TRDTOF_small.png";
        fDetector = D_TRDTOF;
        break;
    case D_PHOS:
        image = "images/PHOS_small.png";
        fDetector = D_PHOS;
        break;
    case D_HMPID:
        image = "images/HMPID_small.png";
        fDetector = D_HMPID;
        break;
    default:
        break;
    }
    fPictButton->SetPicture(gClient->GetPicture(image));
    fPictButton->Draw();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::ShowPat(Int_t pat)
{
    TString image;
    switch (pat)
    {
    case P_KAON:
        image = "images/kaon_small.png";
        fPattern = P_KAON;
        break;
    case P_LAMBDA:
        image = "images/lambda_small.png";
        fPattern = P_LAMBDA;
        break;
    case P_ALAMBDA:
        image = "images/antilambda_small.png";
        fPattern = P_ALAMBDA;
        break;
    case P_XI:
        image = "images/xi_small.png";
        fPattern = P_XI;
        break;
    default:
        break;
    }
    fPictButton->SetPicture(gClient->GetPicture(image));
    fPictButton->Draw();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::ShowPat2(Int_t pat)
{
    TString image;
    switch (pat)
    {
    case P_KAON:
        image = "images/k0_2.png";
        fPattern = P_KAON;
        break;
    case P_LAMBDA:
        image = "images/lambda_2.png";
        fPattern = P_LAMBDA;
        break;
    case P_XI:
        image = "images/xi_2.png";
        fPattern = P_XI;
        break;
    default:
        break;
    }
    fPictButton->SetPicture(gClient->GetPicture(image));
    fPictButton->Draw();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::StartExercise()
{
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
//=MCExerciseS1========================================================================
void MCExerciseS1::StudentSet()
{
    fAxesLoaded = kFALSE;
    fCascadesLoaded = kFALSE;
    fClustersLoaded = kFALSE;
    fEsdTracksLoaded = kFALSE;
    fGeometrySet = kTRUE;
    fITSClustersLoaded = kFALSE;
    fTOFClustersLoaded = kFALSE;
    fTPCClustersLoaded = kFALSE;
    fTRDClustersLoaded = kFALSE;
    fV0sLoaded = kFALSE;
    fVertexLoaded = kFALSE;

    Instructions();
    CustomizeBrowser();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::TrackInfoWindo(Double_t px, Double_t py, Double_t pz, Double_t mass, Int_t charge,
                                  Bool_t isTrack, Bool_t isBachelor)
{
    MCMultiLingual &ml = MCMultiLingual::Instance();

    if (fTIW)
        fTIW->UnmapWindow();
    fTIW = new TGMainFrame(gClient->GetRoot(), 10, 10, kVerticalFrame);
    fTIW->SetCleanup(kDeepCleanup);
    fTIW->DontCallClose();
    TString text = ml.tr("Momentum");

    TGGroupFrame *groupF = new TGGroupFrame(fTIW, text + " X");
    TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF);
    TGLabel *label = new TGLabel(horiF, "[GeV/c]");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fEntryX = new TGNumberEntryField(groupF);
    groupF->AddFrame(fEntryX, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTIW->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
    fEntryX->SetNumber(px);

    groupF = new TGGroupFrame(fTIW, text + " Y");
    horiF = new TGHorizontalFrame(groupF);
    label = new TGLabel(horiF, "[GeV/c]");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fEntryY = new TGNumberEntryField(groupF);
    groupF->AddFrame(fEntryY, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTIW->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
    fEntryY->SetNumber(py);

    groupF = new TGGroupFrame(fTIW, text + " Z");
    horiF = new TGHorizontalFrame(groupF);
    label = new TGLabel(horiF, "[GeV/c]");
    horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fEntryZ = new TGNumberEntryField(groupF);
    groupF->AddFrame(fEntryZ, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTIW->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
    fEntryZ->SetNumber(pz);

    if (!isTrack)
    {
        groupF = new TGGroupFrame(fTIW, ml.tr("Mass"));
        horiF = new TGHorizontalFrame(groupF);
        label = new TGLabel(horiF, "[GeV/c**2]");
        horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
        fEntryM = new TGNumberEntryField(groupF);
        groupF->AddFrame(fEntryM, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
        fTIW->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
        fEntryM->SetNumber(mass);
    }
    else
    {
        groupF = new TGGroupFrame(fTIW, "Charge");
        horiF = new TGHorizontalFrame(groupF);
        label = new TGLabel(horiF, "");
        horiF->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
        fEntryC = new TGNumberEntryField(groupF);
        groupF->AddFrame(fEntryC, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
        fTIW->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
        fEntryC->SetNumber(charge);
    }
    TGTextButton *btxt = nullptr;
    if (charge < 0)
    {
        fTIW->SetWindowName(Form("(-) %s", ml.tr("Particle")));
        btxt = new TGTextButton(fTIW, ml.tr("Copy to calculator"));
        fTIW->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 5, 5));
        if (isBachelor)
            btxt->Connect("Clicked()", "MCExerciseS1", this, "CopyToCalc(Int_t i = 3)");
        else
            btxt->Connect("Clicked()", "MCExerciseS1", this, "CopyToCalc(Int_t i = 1)");
        btxt->Connect("Clicked()", "TGMainFrame", fTIW, "UnmapWindow()");
    }
    if (charge > 0)
    {
        fTIW->SetWindowName(Form("(+) %s", ml.tr("Particle")));
        btxt = new TGTextButton(fTIW, ml.tr("Copy to calculator"));
        fTIW->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 5, 5));
        if (isBachelor)
            btxt->Connect("Clicked()", "MCExerciseS1", this, "CopyToCalc(Int_t i = 3)");
        else
            btxt->Connect("Clicked()", "MCExerciseS1", this, "CopyToCalc(Int_t i = 2)");
        btxt->Connect("Clicked()", "TGMainFrame", fTIW, "UnmapWindow()");
    }
    btxt = new TGTextButton(fTIW, ml.tr("Close"));
    btxt->Connect("Clicked()", "TGMainFrame", fTIW, "UnmapWindow()");
    fTIW->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 5, 5));

    fTIW->MapSubwindows();
    fTIW->Resize();
    fTIW->SetWMSizeHints(fTIW->GetDefaultWidth(), fTIW->GetDefaultHeight(), 1000, 1000, 0, 0);
    fTIW->MapRaised();
    fTIW->Move(100, 1000);
}

//=MCExerciseS1========================================================================
void MCExerciseS1::TutorSet()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();

    Instructions();

    TEveBrowser *browser = gEve->GetBrowser();
    browser->GetMainFrame()->SetWindowName(ml.tr("ALICE MASTER CLASS - TUTOR MODE"));
    if (browser->GetTabRight()->GetTabTab(3))
        browser->GetTabRight()->RemoveTab(3);
    if (browser->GetTabRight()->GetTabTab(2))
        browser->GetTabRight()->RemoveTab(2);
    if (browser->GetTabLeft()->GetTabTab(0))
        browser->GetTabLeft()->RemoveTab(0);
    if (browser->GetTabBottom()->GetTabTab(0))
        browser->GetTabBottom()->RemoveTab(0);

    browser->StartEmbedding(TRootBrowser::kRight);
    if (fTutorSetCanvas)
        delete fTutorSetCanvas;
    fTutorSetCanvas = new TCanvas();
    fTutorSetCanvas->Divide(2, 2);

    fTutorSetCanvas->cd(P_KAON + 1);
    fMinvHistK->Draw();

    fTutorSetCanvas->cd(P_XI + 1);
    fMinvHistX->Draw();

    fTutorSetCanvas->cd(P_LAMBDA + 1);
    fMinvHistL->Draw();

    fTutorSetCanvas->cd(P_ALAMBDA + 1);
    fMinvHistAL->Draw();
    browser->StopEmbedding(ml.tr("Merged Invariant Mass"));

    browser->StartEmbedding(TRootBrowser::kLeft);
    fTMF = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    fTMF->SetWindowName("XX GUI");
    fTMF->SetCleanup(kDeepCleanup);

    TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")));
    fPictButton = nullptr;
    TGTextButton *btext = nullptr;
    TGGroupFrame *groupF = nullptr;
    TGVerticalFrame *vertF = nullptr;

    groupF = new TGGroupFrame(fTMF, ml.tr("TUTOR MODE INSTRUCTIONS"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
        FontStruct_t buttonFont = font->GetFontStruct();
        btext = new TGTextButton(vertF, ml.tr("Global Instructions"));
        btext->SetTextColor(TColor::Number2Pixel(kRed + 2));
        btext->SetFont(buttonFont);
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        btext->Connect("Clicked()", "MCExerciseS1", this, "Instructions()");
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fTMF, ml.tr("Tutor Control"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        btext = new TGTextButton(vertF, ml.tr("Get Files"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "UploadResults()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(vertF, ml.tr("Save Files"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "SaveHistograms()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fTMF, ml.tr("Results"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        btext = new TGTextButton(vertF, ml.tr("Table of Results"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "CountStrangeParticles()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsCenterX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTMF->AddFrame(groupF);

    groupF = new TGGroupFrame(fTMF, ml.tr("Encyclopaedia"));
    vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
    {
        btext = new TGTextButton(vertF, ml.tr("ALICE Detector"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "DetectorInfo()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(vertF, ml.tr("V0 Pattern"));
        btext->Connect("Clicked()", "MCExerciseS1", this, "PatternInfo()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

        btext = new TGTextButton(vertF, "2.76 TeV PbPb");
        btext->Connect("Clicked()", "MCExerciseS1", this, "PatternInfo2()");
        vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
    fTMF->AddFrame(groupF);

    // vertF = new TGVerticalFrame(fTMF, 150, 150, kFixedWidth);
    // {
    //     fPictButton = new TGPictureButton(vertF, gClient->GetPicture("images/2011-Nov-24-ALICE_logo_scaled.png"));
    //     vertF->AddFrame(fPictButton, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    // }
    // fTMF->AddFrame(vertF, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
    fTMF->MapSubwindows();
    fTMF->Resize();
    fTMF->MapWindow();

    browser->StopEmbedding(ml.tr("Tutor"));
    browser->GetTabRight()->SetTab(1);
    // browser->StartEmbedding(TRootBrowser::kBottom);

    // fTMF = new TGMainFrame(gClient->GetRoot(), 1000, 600, kHorizontalFrame);
    // fTMF->SetWindowName("XX GUI");
    // fTMF->SetCleanup(kDeepCleanup);

    // const TGFont *font = gClient->GetFont("-*-helvetica-bold-r-normal-*-20-*-*-*-*-*-*-*");
    // FontStruct_t buttonFont = font->GetFontStruct();

    // btext = new TGTextButton(fTMF, ml.tr(S1_MODS));
    // btext->SetTextColor(TColor::Number2Pixel(kAzure + 2));
    // btext->SetFont(buttonFont);
    // btext->Connect("Clicked()", "MCExerciseS1", this, "ToStudent()");
    // fTMF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));

    // btext = new TGTextButton(fTMF, ml.tr(S1_EXIT));
    // btext->SetTextColor(TColor::Number2Pixel(kRed + 2));
    // btext->SetFont(buttonFont);
    // btext->Connect("Clicked()", "TApplication", gApplication, "Terminate()");
    // fTMF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 1, 1, 1, 1));

    // fTMF->MapSubwindows();
    // fTMF->Resize();
    // fTMF->MapWindow();

    // browser->StopEmbedding(" ");
    browser->HideBottomTab();
}
//=MCExerciseS1========================================================================
void MCExerciseS1::UploadResults()
{
    fLoads++;
    TGFileInfo infoF;
    infoF.fFileTypes = fMasterClassOutputData;
    infoF.fIniDir = StrDup("");
    infoF.fFileTypeIdx = 0;
    infoF.fOverwrite = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(), gEve->GetMainWindow(), kFDOpen, &infoF);
    if (!infoF.fFilename)
        return;
    TPMERegexp filere(".*/([^/]+$)");
    if (filere.Match(infoF.fFilename) != 2)
        return;
    gSystem->ChangeDirectory(infoF.fIniDir);

    TString file1(filere[1]);
    std::ifstream data(infoF.fFilename);
    std::string line;
    Double_t minv = 0.0;
    Int_t nparticles = 0;

    if (data.is_open())
    {
        getline(data, line);
        data >> nparticles;
        getline(data, line);
        fCountKaons += nparticles;
        for (Int_t i = 0; i < nparticles; i++)
        {
            data >> minv;
            getline(data, line);
            fMinvHistK->Fill(minv);
        }
        getline(data, line);
        data >> nparticles;
        getline(data, line);
        fCountLambdas += nparticles;
        for (Int_t i = 0; i < nparticles; i++)
        {
            data >> minv;
            getline(data, line);
            fMinvHistL->Fill(minv);
        }
        getline(data, line);
        data >> nparticles;
        getline(data, line);
        fCountALambdas += nparticles;
        for (Int_t i = 0; i < nparticles; i++)
        {
            data >> minv;
            getline(data, line);
            fMinvHistAL->Fill(minv);
        }
        getline(data, line);
        data >> nparticles;
        getline(data, line);
        fCountXis += nparticles;
        for (Int_t i = 0; i < nparticles; i++)
        {
            data >> minv;
            getline(data, line);
            fMinvHistX->Fill(minv);
        }
        data.close();

        Double_t maxMinv[4] = {fMinvHistK->GetMaximum(), fMinvHistX->GetMaximum(), fMinvHistL->GetMaximum(), fMinvHistAL->GetMaximum()};
        fMinvHistALL->GetYaxis()->SetRangeUser(0, 1.2 * TMath::MaxElement(4, maxMinv));
    }
}
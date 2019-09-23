#include "MCAliCalculator.h"
#include "MCExercise.h"
#include "MCMultiLingual.h"
#include "MCMultiView.h"
#include "MCVSDReader.h"

#include <iostream>

#include <TApplication.h>
#include <TEveBrowser.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveGeoShape.h>
#include <TEveGeoShapeExtract.h>
#include <TEveLine.h>
#include <TEvePointSet.h>
#include <TEveSelection.h>
#include <TEveTrack.h>
#include <TEveViewer.h>
#include <TEveVSD.h>
#include <TFile.h>
#include <TGButton.h>
#include <TGComboBox.h>
#include <TGDockableFrame.h>
#include <TGImageMap.h>
#include <TGLabel.h>
#include <TGLViewer.h>
#include <TGMenu.h>
#include <TGMsgBox.h>
#include <TGLOverlayButton.h>
#include <TGTab.h>
#include <TGWindow.h>
#include <TH1D.h>
#include <TRootHelpDialog.h>
#include <TStyle.h>

//=MCExercise========================================================================
MCExercise::MCExercise(const TGWindow *p, UInt_t w, UInt_t h)  
{
    fPartName[P_KAON]    = "K0s"; 
    fPartName[P_LAMBDA]  = "Lambda"; 
    fPartName[P_ALAMBDA] = "Antilambda"; 
    fPartName[P_XI]      = "Xi"; 

    fMain = new TGVerticalFrame(p, w, h);
    fMain->Move(50, 200);
    fMain->SetCleanup(kDeepCleanup);

    // create dock
    fMenuDock = new TGDockableFrame(fMain);
    fMenuDock->EnableUndock(kTRUE);
    fMenuDock->EnableHide(kTRUE);

    // add dock to main frame
    fMain->AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));

    fMenuOpt = new TGPopupMenu(gClient->GetRoot());
    fMenuOpt->AddEntry("&Help", M_GO_HELP);
    fMenuOpt->AddSeparator();
    // fMenuOpt->AddEntry("E&xit", M_GO_EXIT);
    fMenuOpt->Connect("Activated(Int_t)", "MCExercise", this, "HandleMenu(Int_t)");

    // menu bar
    TGLayoutHints *menuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
    fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
    fMenuBar->AddPopup("&Options", fMenuOpt, menuBarLayout);

    // add menu bar to dock
    fMenuDock->AddFrame(fMenuBar, menuBarLayout);

    // add role button
    fDemo = new TGTextButton(fMain, "", RO_DEMO);
    fDemo->Connect("Clicked()", "MCExercise", this, "Start(E_ROLE ro = RO_DEMO)");

    fStudent = new TGTextButton(fMain, "");
    fStudent->Connect("Clicked()", "MCExercise", this, "Start(E_ROLE ro = RO_STUDENT)");

    fTutor = new TGTextButton(fMain, "");
    fTutor->Connect("Clicked()", "MCExercise", this, "Start(E_ROLE ro = RO_TUTOR)");

    // add the data scroll list
    fDataFrame = new TGCompositeFrame(fMain, w * 0.9, 20, kHorizontalFrame);
    Pixel_t red;
    gClient->GetColorByName("red", red);
    TGGC redtext = TGButton::GetDefaultGC();
    fDataCheck = new TGLabel(fDataFrame, TGString("Data"), redtext());
    fDataCheck->ChangeBackground(red);
    fDataFrame->AddFrame(fDataCheck, new TGLayoutHints(kLHintsLeft | kLHintsCenterY, 3, 5, 0, 0));
    fData = new TGComboBox(fDataFrame);

    for (Int_t index = 0; index < 20; index++)
        fData->AddEntry("", index);

    fData->AddEntry("", 101);
    // fData->Select(0, kTRUE);
    fData->Resize(w * 0.5, 20),
    fData->Connect("Selected(Int_t, Int_t)", "MCExerciseS1", this, "DataSelected()");
    fDataFrame->AddFrame(fData, new TGLayoutHints(kLHintsExpandX));
}

//=MCExercise========================================================================
MCExercise::~MCExercise()  
{
//    if (fDataFile->IsOpen())
//       fDataFile->Close();    

    // if (fInstructions)
    //     fInstructions->CloseWindow(); 
    delete fMain; 
}

//=MCExerciseS1========================================================================
void MCExercise::ChangeBackgroundColor()
{
    gEve->GetViewers()->SwitchColorSet(); 
}

//=MCExercise========================================================================
void MCExercise::ChangeGeometry()
{
    if (fGeometrySet)
    {
        DestroyGeometry(); 
        fGeometrySet = kFALSE;
    }
    else
    {
        GeometryDefault();
        fGeometrySet = kTRUE; 
    }  
}

//=MCExerciseS1========================================================================
void MCExercise::DataSelected()
{
    Pixel_t green;
    gClient->GetColorByName("green", green);
    TGGC redtext = TGButton::GetDefaultGC();
    fDataCheck->ChangeBackground(green);
}

//=MCExerciseS1========================================================================
void MCExercise::DestroyGeometry()
{
    TEveSceneList *sceneList1 = gEve->GetScenes(); 
    TEveElement::List_i i = sceneList1->BeginChildren(); 
    TEveSceneList *sceneList2 = (TEveSceneList * )*i; 
    sceneList2->RemoveElements(); 
    i++;
    i++;
    TEveSceneList *sceneList3 = (TEveSceneList * )*i; 
    sceneList3->RemoveElements(); 
    i++;
    TEveSceneList *sceneList4 = (TEveSceneList * )*i; 
    MCMultiView &mv = MCMultiView::Instance(); 
    mv.SetDepth(-10);
    mv.DestroyEventRPhi(); 
    mv.DestroyEventRhoZ(); 
    mv.SetDepth(0);
    mv.SetRPhiAxes(); 
    mv.SetRhoZAxes(); 
}

//=MCExercise========================================================================
void MCExercise::DetectorInfo()
{
    MCMultiLingual &ml = MCMultiLingual::Instance(); 
    fDEI = new TGMainFrame(gClient->GetRoot(), 100, 100); 
    fDEI->DontCallClose(); 
    TGTextButton    *btxt = nullptr; 

    TGGroupFrame      *groupF = new TGGroupFrame(fDEI, ml.tr("ALICE Detector")); 
    TGHorizontalFrame *horiF  = new TGHorizontalFrame(groupF, 250, 250); 
    
    fPictButton   = new TGPictureButton(horiF, gClient->GetPicture("images/ALICE_small2.png")); 
    fPictButton->Connect("Clicked()", "MCExerciseS1", this, "MakeBiggerDet()");  
    horiF->AddFrame(fPictButton, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    groupF->AddFrame(horiF); 

    horiF = new TGHorizontalFrame(groupF); 
    {
        btxt = new TGTextButton(horiF, "ALICE"); 
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowDet(Int_t det = D_ALICE)"); 
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 

        btxt = new TGTextButton(horiF, "ITS"); 
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowDet(Int_t det = D_ITS)"); 
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 

        btxt = new TGTextButton(horiF, "TPC"); 
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowDet(Int_t det = D_TPC)"); 
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 

        btxt = new TGTextButton(horiF, "TRD & TOF"); 
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowDet(Int_t det = D_TRDTOF)"); 
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 

        btxt = new TGTextButton(horiF, "PHOS"); 
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowDet(Int_t det = D_PHOS)"); 
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 

        btxt = new TGTextButton(horiF, "HMPID"); 
        btxt->Connect("Clicked()", "MCExerciseS1", this, "ShowDet(Int_t det = D_HMPID)"); 
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY)); 

    horiF = new TGHorizontalFrame(groupF); 
    {
        btxt = new TGTextButton(horiF, ml.tr("Close")); 
        btxt->Connect("Clicked()", "TGMainFrame", fDEI, "UnmapWindow()");
        horiF->AddFrame(btxt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY)); 
    fDEI->AddFrame(groupF); 
    fDEI->SetWindowName(ml.tr("ALICE Detector"));
    fDEI->MapSubwindows();
    fDEI->Resize(fDEI->GetDefaultSize()); 
    fDEI->MapWindow(); 
}

//=MCExerciseS1========================================================================
void MCExercise::DisplayAxes()
{
    if (fAxesLoaded)
        fAxesLoaded = kFALSE;
    else 
        fAxesLoaded = kTRUE;
    GotoEvent(MCVSDReader::Instance().CurEv());  
}

//=MCExercise========================================================================
void MCExercise::DisplayClusters()
{
    if (fClustersLoaded)
    {
        fClustersLoaded    = kFALSE; 
 		fITSClustersLoaded = kFALSE;
 		fTPCClustersLoaded = kFALSE;
 		fTRDClustersLoaded = kFALSE;
 		fTOFClustersLoaded = kFALSE; 
        GotoEvent(MCVSDReader::Instance().CurEv()); 
        fCheckClusters->SetDisabledAndSelected(kFALSE);
    }
    else 
    {
        fClustersLoaded    = kTRUE; 
 		fITSClustersLoaded = kTRUE;
 		fTPCClustersLoaded = kTRUE;
 		fTRDClustersLoaded = kTRUE;
 		fTOFClustersLoaded = kTRUE; 
        GotoEvent(MCVSDReader::Instance().CurEv()); 
        fCheckClusters->SetDisabledAndSelected(kTRUE);
    }
}

//=MCExercise========================================================================
void MCExercise::DisplayTracks()
{
    if (fEsdTracksLoaded)
    {
        fTrackList->DestroyElements(); 
        fTrackList = nullptr; 
        fEsdTracksLoaded = kFALSE; 
        GotoEvent(MCVSDReader::Instance().CurEv()); 
        fCheckESDTracks->SetDisabledAndSelected(kFALSE);
    }
    else 
    {
        fEsdTracksLoaded = kTRUE; 
        GotoEvent(MCVSDReader::Instance().CurEv()); 
        fCheckESDTracks->SetDisabledAndSelected(kTRUE);
    }
}

//=MCExercise========================================================================
void MCExercise::DisplayVertex()
{
    if (fVertexLoaded)
    {
        fVertexLoaded = kFALSE;
        GotoEvent(MCVSDReader::Instance().CurEv()); 
        fCheckVertex->SetDisabledAndSelected(kFALSE);
    }
    else 
    {
        fVertexLoaded = kTRUE; 
        GotoEvent(MCVSDReader::Instance().CurEv()); 
        fCheckVertex->SetDisabledAndSelected(kTRUE);
    }
}

//=MCExercise========================================================================
void MCExercise::GeometryDefault()
{
    TEveSceneList *sceneList1 = gEve->GetScenes(); 
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList *sceneList2 = (TEveSceneList *)*i; 
    sceneList2->RemoveElements(); 
    i++;
    i++;
    TEveSceneList *sceneList3 = (TEveSceneList *)*i; 
    sceneList3->RemoveElements(); 
    i++;
    TEveSceneList *sceneList4 = (TEveSceneList *)*i; 
    sceneList4->RemoveElements(); 

    TFile *geom = TFile::Open(fGeometryFileName); 
    if (!geom)
        return; 
    TEveGeoShapeExtract *gse = (TEveGeoShapeExtract *)geom->Get("Gentle"); 
    TEveGeoShape *gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0); 
    geom->Close(); 
    delete geom; 

    gentle_geom->SetRnrSelf(kFALSE);

    //ITS
    i = gentle_geom->BeginChildren(); 
    TEveGeoShape *lvl1 = (TEveGeoShape *) *i; 
    //TPC
    i++; 
    //TRD & TOF
    i++;
    lvl1 = (TEveGeoShape *) *i; 
    lvl1->SetRnrSelf(kFALSE); 
    TEveElement::List_i j = lvl1->BeginChildren();
    TEveGeoShape* trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;
        trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    j++;
    j++;
    j++;
    j++;
        trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    j++;
    j++;
    j++;
    j++;
    j++;

    trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;

    trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainTransparency(80);
    j++;
    //PHOS
    /*
    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainTransparency(20);
      }

    //HMPID

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
      }
    */
    gEve->AddGlobalElement(gentle_geom);

    MCMultiView &mv = MCMultiView::Instance(); 
    mv.SetDepth(-10);
    mv.DestroyGeomRPhi();
    mv.ImportGeomRPhi(gentle_geom);
    mv.DestroyGeomRhoZ();
    mv.ImportGeomRhoZ(gentle_geom);
    mv.SetDepth(0);
    mv.SetRPhiAxes();
    mv.SetRhoZAxes();
} 

//=MCExercise========================================================================
void MCExercise::HandleMenu(Int_t id)
{
    MCMultiLingual &ml = MCMultiLingual::Instance(); 

    switch (id)
    {
    // case M_GO_EXIT:
    // {
    //     Int_t retval;
    //     TString msgText = (ml.tr("Do you want really to quit?"));
    //     new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseS1:Exit", msgText, kMBIconQuestion, kMBYes | kMBCancel, &retval);
    //     if (retval == 1)
    //         CloseWindow();
    //     break;
    // }
        case M_GO_HELP:
        {
            TRootHelpDialog *ins = new TRootHelpDialog(gClient->GetRoot(), "", 750, 180);
            ins->SetWindowName(ml.tr("Instructions"));
            ins->SetText(ml.tr("\
Welcome to ALICE Masterclass!\n\n\
\
You see the window <MasterClass>.There are 3 buttons to your disposal\n\
Using these buttons, one can choose the mode:\n\n\
  Demo    - starts a demo of MasterClass exercise with 4 events to analyse\n\
\
  Student - starts MasterClass with 100 real events for you to analyze\n\
\
  Tutor   - starts Masterclass with Tutor tools that allow to summarise\n\
            the students results and draw conclusions from the analysis"));
            ins->Popup();
            break;
        }
        default:
        {
            printf("Menu item %d selected\n", id);
            break;
        }
    }
}

//=MCExercise========================================================================
void MCExercise::InitViewer(Bool_t withGeometry)
{
    MCMultiLingual &ml = MCMultiLingual::Instance(); 

    TFile::SetCacheFileDir(".");
    TEveVSD::DisableTObjectStreamersForVSDStruct();
    TEveManager::Create(kTRUE, "FV");
    if (withGeometry)
    {
    // Get the geometry from file
        TFile *geom = TFile::Open(fGeometryFileName);
        if (!geom)
        {
            TString msgText(Form("Geometry file %s not found! \nCheck your installation", fGeometryFileName.Data()));
            new TGMsgBox(gClient->GetRoot(), fMain, "MCExercise:InitViewer", msgText, kMBIconExclamation, kMBOk);
            gApplication->Terminate();
        }
        TEveGeoShapeExtract *gse  = (TEveGeoShapeExtract *)geom->Get("Gentle");
        TEveGeoShape *gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
        geom->Close();
        delete geom; 
        gentle_geom->SetRnrSelf(kFALSE);
        // ITS
        TEveElement::List_i index = gentle_geom->BeginChildren();
        TEveGeoShape *lvl1 = (TEveGeoShape *)*index;
        //TPC
        index++;
        //TRD & TOF
        index++;

        lvl1 = (TEveGeoShape *)*index;
        lvl1->SetRnrSelf(kFALSE);
        TEveElement::List_i jndex = lvl1->BeginChildren();

        TEveGeoShape *trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        jndex++;
        jndex++;
        jndex++;
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        jndex++;
        jndex++;
        jndex++;
        jndex++;
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;

        trd1 = (TEveGeoShape *)*jndex;
        trd1->SetRnrSelf(kTRUE);
        trd1->SetMainTransparency(80);
        jndex++;
        /* 
        //PHOS
        i++;

        lvl1 = (TEveGeoShape*) *i;
        lvl1->SetRnrSelf(kFALSE);

        for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
          {
        lvl2 = (TEveGeoShape*) *j;
        lvl2->SetRnrSelf(kTRUE);
        lvl2->SetMainTransparency(20);
          }

        //HMPID

        i++;

        lvl1 = (TEveGeoShape*) *i;
        lvl1->SetRnrSelf(kFALSE);

        for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
          {
        lvl2 = (TEveGeoShape*) *j;
        lvl2->SetRnrSelf(kTRUE);
          }
        */
        gEve->AddGlobalElement(gentle_geom);

        // Standard multi-view
        //=====================

        MCMultiView &mv = MCMultiView::Instance();
        mv.Get3DView()->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);
        mv.Get3DView()->GetGLViewer()->SetStyle(0);
        mv.SetDepth(-10);
        mv.ImportGeomRPhi(gentle_geom);
        mv.ImportGeomRhoZ(gentle_geom);
        mv.SetDepth(0);

        // Final stuff
        //=====================

        gEve->GetViewers()->SwitchColorSet();
        gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);
        TString text; 
        if (mv.IsSet())
        {
            text = ml.tr("Use ROLLER to zoom in/out");
            fOverlayButton1 = new TGLOverlayButton(mv.Get3DView()->GetGLViewer(), text, 0, mv.Get3DView()->GetEveFrame()->GetHeight()-20, 250, 20);
            text = ml.tr("Click LEFT button to rotate");
            fOverlayButton2 = new TGLOverlayButton(mv.Get3DView()->GetGLViewer(), text, 0, mv.Get3DView()->GetEveFrame()->GetHeight()-40, 250, 20);
            text = ml.tr("Press ROLLER to move");
            fOverlayButton3 = new TGLOverlayButton(mv.Get3DView()->GetGLViewer(), text, 0, mv.Get3DView()->GetEveFrame()->GetHeight()-60, 250, 20);
        }
        else 
        {
            text = ml.tr("Use ROLLER to zoom in/out");
            fOverlayButton1 = new TGLOverlayButton(mv.Get3DView()->GetGLViewer(), text, 0, 0, 250, 20);
            text = ml.tr("Click LEFT button to rotate");
            fOverlayButton2 = new TGLOverlayButton(mv.Get3DView()->GetGLViewer(), text, 0, 0, 250, 20);
            text = ml.tr("Press ROLLER to move");
            fOverlayButton3 = new TGLOverlayButton(mv.Get3DView()->GetGLViewer(), text, 0, 0, 250, 20);  
        }
        fOverlayButton1->SetAlphaValues(0.05, 0.8);
        fOverlayButton2->SetAlphaValues(0.05, 0.8);
        fOverlayButton3->SetAlphaValues(0.05, 0.8);
        (gEve->GetSelection())->Connect("SelectionAdded(TEveElement*)", "MCExerciseS1", this, "EveSelection(TEveElement*)"); 
    }    
    StartBrowser(); 
}

//=MCExercise========================================================================
void MCExercise::LoadAxes()
{
    for (Int_t i = 0; i < 12; i++)
    {
        fAxesX[i] = new TEveLine(" "); 
        fAxesX[i]->SetLineColor(kRed); 
        fAxesX[i]->SetLineWidth(1); 
        fAxesX[i]->SetLineStyle(1); 
    }
    for (Int_t i = 0; i < 14; i++)
    {
        fAxesY[i] = new TEveLine(" "); 
        fAxesY[i]->SetLineColor(kGreen); 
        fAxesY[i]->SetLineWidth(1); 
        fAxesY[i]->SetLineStyle(1); 

        fAxesZ[i] = new TEveLine(" "); 
        fAxesZ[i]->SetLineColor(kBlue); 
        fAxesZ[i]->SetLineWidth(1); 
        fAxesZ[i]->SetLineStyle(1); 
    }
    //line X
    fAxesX[0]->SetPoint(0, -100, 0, 0); 
    fAxesX[0]->SetPoint(1,  100, 0, 0); 

    //arrows X
    fAxesX[1]->SetPoint(0, -100,  0, 0); 
    fAxesX[1]->SetPoint(1,  -95,  5, 0); 
    fAxesX[2]->SetPoint(0, -100,  0, 0); 
    fAxesX[2]->SetPoint(1,  -95, -5, 0); 
    fAxesX[3]->SetPoint(0,  100,  0, 0); 
    fAxesX[3]->SetPoint(1,   95,  5, 0); 
    fAxesX[4]->SetPoint(0,  100,  0, 0); 
    fAxesX[4]->SetPoint(1,   95, -5, 0); 

    //+X
    fAxesX[5]->SetPoint(0,   95,   10, 0); 
    fAxesX[5]->SetPoint(1,   90,   10, 0); 
    fAxesX[6]->SetPoint(0, 92.5,  7.5, 0); 
    fAxesX[6]->SetPoint(1, 92.5, 12.5, 0); 
    fAxesX[7]->SetPoint(0,   90,  6, 0); 
    fAxesX[7]->SetPoint(1,   86, 14, 0); 
    fAxesX[8]->SetPoint(0,   90, 14, 0); 
    fAxesX[8]->SetPoint(1,   86,  6, 0); 

    //-X
    fAxesX[9]->SetPoint(0,   95,   10, 0); 
    fAxesX[9]->SetPoint(1,   90,   10, 0); 
    fAxesX[10]->SetPoint(0, 92.5,  7.5, 0); 
    fAxesX[10]->SetPoint(1, 92.5, 12.5, 0); 
    fAxesX[11]->SetPoint(0,   90,  6, 0); 
    fAxesX[11]->SetPoint(1,   86, 14, 0); 
    fAxesX[12]->SetPoint(0,   90, 14, 0); 
    fAxesX[12]->SetPoint(1,   86,  6, 0); 

    //line Y
    fAxesY[0]->SetPoint(0, 0, -100, 0); 
    fAxesY[0]->SetPoint(1, 0,  100, 0); 

    //arrows Y
    fAxesY[1]->SetPoint(0, 0, -100,  0); 
    fAxesY[1]->SetPoint(1, 0,  -95,  5); 
    fAxesY[2]->SetPoint(0, 0, -100,  0); 
    fAxesY[2]->SetPoint(1, 0,  -95, -5); 
    fAxesY[3]->SetPoint(0, 0,  100,  0); 
    fAxesY[3]->SetPoint(1, 0,   95,  5); 
    fAxesY[4]->SetPoint(0, 0,  100,  0); 
    fAxesY[4]->SetPoint(1, 0,   95, -5); 

    //+Y
    fAxesY[5]->SetPoint(0, 0,   95,   10); 
    fAxesY[5]->SetPoint(1, 0,   90,   10); 
    fAxesY[6]->SetPoint(0, 0, 92.5,  7.5); 
    fAxesY[6]->SetPoint(1, 0, 92.5, 12.5); 
    fAxesY[7]->SetPoint(0, 0, 96.5, 12.5); 
    fAxesY[7]->SetPoint(1, 0, 92.5, 14.5); 
    fAxesY[8]->SetPoint(0, 0, 96.5, 16.5); 
    fAxesY[8]->SetPoint(1, 0, 92.5, 14.5); 
    fAxesY[9]->SetPoint(0, 0, 92.5, 14.5); 
    fAxesY[9]->SetPoint(1, 0, 88.5, 14.5); 

    //-Y
    fAxesY[10]->SetPoint(0, 0, -92.5,  7.5);
    fAxesY[10]->SetPoint(1, 0, -92.5, 12.5);

    fAxesY[11]->SetPoint(0,  0, -88.5,  12.5);
    fAxesY[11]->SetPoint(1,  0, -92.5,  14.5);

    fAxesY[12]->SetPoint(0,  0, -88.5,  16.5);
    fAxesY[12]->SetPoint(1,  0, -92.5, 14.5);

    fAxesY[13]->SetPoint(0,  0, -92.5,  14.5);
    fAxesY[13]->SetPoint(1,  0, -96.5, 14.5);

    //line Z
    fAxesZ[0]->SetPoint(0,  0,  0, -100);
    fAxesZ[0]->SetPoint(1,  0,  0,  100);

    //Z arrows
    fAxesZ[1]->SetPoint(0,  0,  0, -100);
    fAxesZ[1]->SetPoint(1,  8,  0, -92);

    fAxesZ[2]->SetPoint(0,  0,  0, -100);
    fAxesZ[2]->SetPoint(1, -8,  0, -92);

    fAxesZ[3]->SetPoint(0,  0,  0,  100);
    fAxesZ[3]->SetPoint(1,  8,  0,  92);

    fAxesZ[4]->SetPoint(0,  0,  0,  100);
    fAxesZ[4]->SetPoint(1, -8,  0,  92);

    //+Z
    fAxesZ[5]->SetPoint(0, 0, 10, 90);
    fAxesZ[5]->SetPoint(1, 0, 10, 85);

    fAxesZ[6]->SetPoint(0, 0, 7.5, 87.5);
    fAxesZ[6]->SetPoint(1, 0, 12.5, 87.5);

    fAxesZ[7]->SetPoint(0,  0, 14, 95);
    fAxesZ[7]->SetPoint(1,  0, 14, 90);

    fAxesZ[8]->SetPoint(0,  0, 14, 95);
    fAxesZ[8]->SetPoint(1,  0, 6, 90);

    fAxesZ[9]->SetPoint(0,  0, 6, 90);
    fAxesZ[9]->SetPoint(1,  0, 6, 95);

    //-Z
    fAxesZ[10]->SetPoint(0, 0, 10, -90);
    fAxesZ[10]->SetPoint(1, 0, 10, -95);

    fAxesZ[11]->SetPoint(0,  0, 14, -90);
    fAxesZ[11]->SetPoint(1,  0, 14, -85);

    fAxesZ[12]->SetPoint(0,  0, 14, -85);
    fAxesZ[12]->SetPoint(1,  0, 6, -90);

    fAxesZ[13]->SetPoint(0,  0, 6, -85);
    fAxesZ[13]->SetPoint(1,  0, 6, -90);

    for (Int_t i = 0; i < 12; i++) 
        gEve->AddElement(fAxesX[i]); 
    for (Int_t i = 0; i < 14; i++) 
    {
        gEve->AddElement(fAxesY[i]); 
        gEve->AddElement(fAxesZ[i]); 
    }
}

//=MCExercise========================================================================
void MCExercise::LoadClusters(TEvePointSet*& ps, const TString& det_name, Int_t det_id)
{
    if (ps == nullptr)
    {
        ps = new TEvePointSet(det_name);
        ps->SetMainColor((Color_t)(det_id + 2));
        ps->SetMarkerSize(0.5);
        ps->SetMarkerStyle(2);
        ps->IncDenyDestroy();
    }
    else 
        ps->Reset(); 
    TEvePointSelector ss(MCVSDReader::Instance().GetEveVSD()->fTreeC, ps, 
                        "fV.fX:fV.fX:fV.fZ", TString::Format("fDetId==%d", det_id)); 
    ss.Select();
    ps->SetTitle(det_name + MCMultiLingual::Instance().tr("Clusters"));
    gEve->AddElement(ps);  
}

//=MCExercise========================================================================
void MCExercise::LoadVertex()
{
    if (!fPsVertex)
    {
        fPsVertex = new TEvePointSet("Vertex"); 
        fPsVertex->SetMainColor(kRed); 
        fPsVertex->SetMarkerSize(2); 
        fPsVertex->SetMarkerStyle(2); 
        fPsVertex->IncDenyDestroy(); 
    }
    else 
        fPsVertex->Reset(); 

    TEvePointSelector ss(MCVSDReader::Instance().GetEveVSD()->fTreeR, fPsVertex, "fPVX:fPVY:fPVZ"); 
    ss.Select();
    fPsVertex->SetTitle(MCMultiLingual::Instance().tr("Primary Vertex")); 
    gEve->AddElement(fPsVertex);     
}

//=MCExercise========================================================================
void MCExercise::MakeGui(Bool_t dataOpt)
{
    // add elements to the main window
    TGImageMap *AliceLogo = new TGImageMap(fMain, gClient->GetPicture(fGuiLogoName));
    fMain->AddFrame(AliceLogo, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY | kLHintsExpandX | kLHintsExpandY));
    TGLayoutHints *layout = new TGLayoutHints(kLHintsExpandX, 5, 5, 5, 5);
    fMain->AddFrame(fDemo,      layout);
    fMain->AddFrame(fStudent,   layout);
    fMain->AddFrame(fTutor,     layout);
    if (dataOpt)
        fMain->AddFrame(fDataFrame, layout);

    //everything is done
    fMain->SetWindowName("");
    fMain->MapSubwindows();
    fMain->Resize();
    fMain->MapWindow();

    SetText();
}

//=MCExercise========================================================================
Bool_t MCExercise::OpenDataFile()
{
    Bool_t rv = kFALSE; 
    if (fDataSetFileName.IsNull())
    {
        if (fRole == RO_STUDENT) 
        {
            fDataSet = fData->GetSelected() + 1;
            if (fDataSet == 0)
            {
                TString msgText(MCMultiLingual::Instance().tr("Select a data set"));
                new TGMsgBox(gClient->GetRoot(), fMain, "MCExercise:Data", msgText, kMBIconExclamation, kMBOk);
                return rv;
            }
            TString dataName(fDataTemplate); 
            if (fDataSet == 101)
                dataName = "AliVSD_MasterClass_PbPb_101.root";
            else 
                dataName.ReplaceAll("x", Form("%i", fDataSet));

            fDataSetFileName = fDataSetFileDir + dataName;
        } 
        else if (fRole == RO_DEMO) 
        {
            fDataSet = -1; 
            fDataSetFileName = fDataSetFileDir + "AliVSD_example.root";
        }
    }
    if (! (fDataFile = TFile::Open(fDataSetFileName)))
    {
        TString msgText(Form("Event file %s not found! \nCheck your installation", fDataSetFileName.Data()));
        new TGMsgBox(gClient->GetRoot(), fMain, "MCExercise:Data", msgText, kMBIconExclamation, kMBOk);
        gApplication->Terminate();
    }
    else 
        rv = kTRUE; 
    
    return rv; 
}

//=MCExercise========================================================================
void MCExercise::ResetHistograms()
{
    TList *hl = TDirectory::CurrentDirectory()->GetList(); 
    for (const auto&& obj : *hl)    
        ((TH1*)obj)->Reset();  
}

//=MCExercise========================================================================
void MCExercise::Start(E_ROLE role)
{
    fRole = role;
    if (fRole == RO_STUDENT || fRole == RO_DEMO)
    {
        fDataSetFileName.Clear(); 
        if (!OpenDataFile()) 
            return;  
        MCVSDReader::Instance().Init(fDataSetFileName);
    }
    if (MCAliCalculator::Instance().IsStarted())
    {
        if (fRole == RO_STUDENT)
        MCAliCalculator::Instance().BlockButtons(kTRUE);
        else if (fRole == RO_DEMO)
        MCAliCalculator::Instance().BlockButtons(kFALSE);
    }
    StartExercise(); 
 
}

//=MCExercise========================================================================
void MCExercise::SetText()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();

    // fMain->SetWindowName(ml.tr("Master Class: Strangeness Exercice 1"));
    fDemo->SetText(ml.tr("Demo"));
    fStudent->SetText(ml.tr("Student"));
    fTutor->SetText(ml.tr("Tutor"));
    char name[50];
    for (Int_t index = 0; index < 20; index++)
    {
        TGTextLBEntry *te = ((TGTextLBEntry *)fData->GetListBox()->GetEntry(index));
        if (!te)
            break;
        else
        {
            sprintf(name, "pp - %s %d", ml.tr("Set"), index + 1);
            te->SetText(new TGString(name));
        }
    }
    TGTextLBEntry *te = ((TGTextLBEntry *)fData->GetListBox()->GetEntry(101));
    if (te)
    {    
        sprintf(name, "Pb-Pb - %s %d", ml.tr("Set"), 101);
        te->SetText(new TGString(name));
    }
}

//=MCExercise========================================================================
void MCExercise::StartBrowser()  
{
    TEveBrowser *browser = gEve->GetBrowser();
    browser->GetTabLeft()->RemoveTab(1);
    browser->GetTabLeft()->RemoveTab(0);
    
    TEveEventManager *evm = gEve->GetCurrentEvent(); 
    if (evm) 
        delete evm; 
    gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event")); 
}

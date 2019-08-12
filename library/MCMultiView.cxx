// Multi-view (3d, rphi, rhoz) service class using EVE Window Manager.
// Author: Matevz Tadel 2009
#include "MCMultiView.h"
#include "MCMultiLingual.h"
#include <TGLViewer.h>
#include <TEveBrowser.h>
#include <TEveManager.h>
#include <TEveProjectionAxes.h>
#include <TEveViewer.h>
#include <TEveWindow.h>
#include <TList.h>
#include <TObjString.h>

#include <iostream>

MCMultiView MCMultiView::fInstance = MCMultiView();

//=MCMultiView========================================================================
void MCMultiView::Init()
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    // Constructor --- creates required scenes, projection managers
    // and GL viewers.

    // Scenes
    //========
    fRPhiGeomScene = gEve->SpawnNewScene("RPhi Geometry",
                                         "Scene holding projected geometry for the RPhi view.");
    fRhoZGeomScene = gEve->SpawnNewScene("RhoZ Geometry",
                                         "Scene holding projected geometry for the RhoZ view.");
    fRPhiEventScene = gEve->SpawnNewScene("RPhi Event Data",
                                          "Scene holding projected event-data for the RPhi view.");
    fRhoZEventScene = gEve->SpawnNewScene("RhoZ Event Data",
                                          "Scene holding projected event-data for the RhoZ view.");
    // Projection managers
    //=====================
    fRPhiMgr = new TEveProjectionManager(TEveProjection::kPT_RPhi);
    gEve->AddToListTree(fRPhiMgr, kFALSE);
    {
        TEveProjectionAxes *a = new TEveProjectionAxes(fRPhiMgr);
        a->SetMainColor(kWhite);
        a->SetTitle("R-Phi");
        a->SetTitleSize(0.05);
        a->SetTitleFont(102);
        a->SetLabelSize(0.025);
        a->SetLabelFont(102);
        fRPhiGeomScene->AddElement(a);
    }
    fRhoZMgr = new TEveProjectionManager(TEveProjection::kPT_RhoZ);
    gEve->AddToListTree(fRhoZMgr, kFALSE);
    {
        TEveProjectionAxes *a = new TEveProjectionAxes(fRhoZMgr);
        a->SetMainColor(kWhite);
        a->SetTitle("Rho-Z");
        a->SetTitleSize(0.05);
        a->SetTitleFont(102);
        a->SetLabelSize(0.025);
        a->SetLabelFont(102);
        fRhoZGeomScene->AddElement(a);
    }
    // Viewers
    //=========
    TEveWindowSlot *slot = TEveWindow::CreateWindowInTab(gEve->GetBrowser()->GetTabRight());
    TEveWindowPack *pack = slot->MakePack();
    pack->SetElementName(ml.tr("Multi View"));
    pack->SetHorizontal();
    pack->SetShowTitleBar(kFALSE);
    pack->NewSlot()->MakeCurrent();
    f3DView = gEve->SpawnNewViewer(ml.tr("3D View"));
    f3DView->AddScene(gEve->GetGlobalScene());
    f3DView->AddScene(gEve->GetEventScene());
    pack = pack->NewSlot()->MakePack();
    pack->SetShowTitleBar(kFALSE);
    pack->NewSlot()->MakeCurrent();
    fRPhiView = gEve->SpawnNewViewer(ml.tr("R-Phi View"), "");
    fRPhiView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    fRPhiView->AddScene(fRPhiGeomScene);
    fRPhiView->AddScene(fRPhiEventScene);
    pack = pack->NewSlot()->MakePack();
    pack->SetShowTitleBar(kFALSE);
    pack->NewSlot()->MakeCurrent();
    fRhoZView = gEve->SpawnNewViewer(ml.tr("Rho-Z View"), "");
    fRhoZView->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    fRhoZView->AddScene(fRhoZGeomScene);
    fRhoZView->AddScene(fRhoZEventScene);
    fMVSet = kTRUE;
}

//=MCMultiView========================================================================
MCMultiView &MCMultiView::Instance()
{
    if (!fInstance.IsSet())
        fInstance.Init();
    return fInstance;
}

//=MCMultiView========================================================================
void MCMultiView::SetDepth(Float_t d)
{
    // Set current depth on all projection managers.

    fRPhiMgr->SetCurrentDepth(d);
    fRhoZMgr->SetCurrentDepth(d);
}

//=MCMultiView========================================================================
void MCMultiView::SetRPhiAxes()
{
    gEve->AddToListTree(fRPhiMgr, kFALSE);
    {
        TEveProjectionAxes *a = new TEveProjectionAxes(fRPhiMgr);
        a->SetMainColor(kWhite);
        a->SetTitle("Rho-Z");
        a->SetTitleSize(0.05);
        a->SetTitleFont(102);
        a->SetLabelSize(0.025);
        a->SetLabelFont(102);
        fRPhiGeomScene->AddElement(a);
    }
}

//=MCMultiView========================================================================
void MCMultiView::SetRhoZAxes()
{
    gEve->AddToListTree(fRhoZMgr, kFALSE);
    {
        TEveProjectionAxes *a = new TEveProjectionAxes(fRhoZMgr);
        a->SetMainColor(kWhite);
        a->SetTitle("Rho-Z");
        a->SetTitleSize(0.05);
        a->SetTitleFont(102);
        a->SetLabelSize(0.025);
        a->SetLabelFont(102);
        fRhoZGeomScene->AddElement(a);
    }
}
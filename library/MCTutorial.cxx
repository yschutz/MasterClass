#include "MCTutorial.h"
#include "MCMultiView.h"

#include <iostream>

#include <TEveBrowser.h>
#include <TEveElement.h>
#include <TEveLine.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveVector.h>
#include <TRandom.h>
#include <TTimer.h>

//=MCTutorial========================================================================
MCTutorial::MCTutorial(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
    InitViewer(); 
    CustomizeBrowser(); 
    gRandom->SetSeed(0); 
    fTimer = new TTimer(); 
    fTimer->Connect("Timeout()", "MCTutorial", this, "DrawTrackSegment()"); 
    DrawTrack(); 
}

//=MCTutorial========================================================================
void MCTutorial::CustomizeBrowser()
{
    TEveBrowser *browser = gEve->GetBrowser();

}

//=MCTutorial========================================================================
void MCTutorial::DrawTrack()
{
    fTrack = new TEveLine("track"); 
    fTrack->SetLineColor(kRed); 
    fTrack->SetLineWidth(3); 
    fTrack->SetLineStyle(1); 
    fTrackDirection.SetXYZ(1., 0., 0.);
    fTrackDirection.SetMag(kfMaxRadius/10); 
    Double_t theta = gRandom->Rndm() * 180.; 
    Double_t phi   = gRandom->Rndm() * 360.; 
    fTrackDirection.SetTheta(theta * TMath::DegToRad()); 
    fTrackDirection.SetPhi(phi * TMath::DegToRad()); 
    fTrack->SetPoint(0, 0, 0, 0);

    fTimer->Start(300, kFALSE); 
}

//=MCTutorial========================================================================
void MCTutorial::DrawTrackSegment()
{
    if (fTrack->CalculateLineLength() > kfMaxRadius)
    {
        fTimer->Stop(); 
        fTrackSegmentStep = 1; 
    }
    Float_t x = fTrack->GetLineEnd().fX;   
    Float_t y = fTrack->GetLineEnd().fY;   
    Float_t z = fTrack->GetLineEnd().fZ;   
    fTrack->SetPoint(fTrackSegmentStep, fTrackDirection.X() + x, fTrackDirection.Y() + y, fTrackDirection.Z() + z); 
    gEve->AddElement(fTrack);
    TEveElement *top = gEve->GetCurrentEvent();
    if (MCMultiView::Instance().IsSet())
    {
        MCMultiView::Instance().DestroyEventRPhi();
        MCMultiView::Instance().ImportEventRPhi(top);
        MCMultiView::Instance().DestroyEventRhoZ();
        MCMultiView::Instance().ImportEventRhoZ(top);
    }
    gEve->Redraw3D(kFALSE, kTRUE);
    fTrackSegmentStep++; 
}
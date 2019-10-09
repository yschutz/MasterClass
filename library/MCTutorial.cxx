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

    fNSteps = 100; 
    fBz     = 0.5;                    // Tesla
    fMass   = 130.0 * 1.78E-30;       // kg 
    fCharge = 1.0 * 1.602E-19;        // Coulomb 
    fPt     = 200.;                  // MeV 

    fOmega  = fBz * fCharge / fMass; 
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
    fTrack->SetSmooth(kTRUE); 

    Double_t theta = 0. ; //gRandom->Rndm() * 180.; 
    Double_t phi   = 0.; //gRandom->Rndm() * 360.; 
    fTrackMomentum.SetPtThetaPhi(fPt, theta, phi);  
    fRadius = fTrackMomentum.Pt() / fBz;
    std::cout << "px py pz " << fTrackMomentum.Z() << " " << fTrackMomentum.Z() << std::endl;  
    fTracklet.SetXYZ(fTrackMomentum.X(), fTrackMomentum.Y(), fTrackMomentum.Z());
    fTracklet.SetMag(kfMaxRadius / fNSteps); 

    fTrack->SetPoint(0, 0, 0, 0); 
    fTimer->Start(300, kFALSE); 
}

//=MCTutorial========================================================================
void MCTutorial::DrawTrackSegment()
{
    Double_t r = TMath::Sqrt(fTrack->GetLineEnd().fX * fTrack->GetLineEnd().fX + fTrack->GetLineEnd().fY * fTrack->GetLineEnd().fY + fTrack->GetLineEnd().fZ * fTrack->GetLineEnd().fZ); 
    if (r > kfMaxRadius)
    {
        fTimer->Stop(); 
        fTrackSegmentStep = 1; 
        return; 
    }
    Float_t x, y, z;
    TrackStep(x, y, z); 
    fTrack->SetPoint(fTrackSegmentStep, x, y, z); 
    std::cout << "xyz " << fTrackSegmentStep/100. << " " << x << " " << y << " " << z << std::endl;  
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

//=MCTutorial========================================================================
void MCTutorial::TrackStep(Float_t &x, Float_t &y, Float_t &z)
{
    if (fBz == 0.0)     
    {   // linear 
        x = fTracklet.X() + fTrack->GetLineEnd().fX; 
        y = fTracklet.Y() + fTrack->GetLineEnd().fY; 
        z = fTracklet.Z() + fTrack->GetLineEnd().fZ; 
    }
    else
    {
        x = fRadius * TMath::Sin(fOmega * fTrackSegmentStep / (Double_t)fNSteps);
        y = fRadius * TMath::Cos(fOmega * fTrackSegmentStep / (Double_t)fNSteps); 
        z = fTrackMomentum.Z() / (Double_t)fNSteps + fTrack->GetLineEnd().fZ; 
    }
    
}
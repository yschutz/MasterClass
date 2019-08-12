// to display information on selected ALICE detector
// Yves Schutz June 2019

#include "MCAliceDetectorInfoWindow.h"

#include <TGButton.h>
#include <TString.h>


MCAliceDetectorInfoWindow MCAliceDetectorInfoWindow::fInstance = MCAliceDetectorInfoWindow(); 
//=MCAliceDetectorInfoWindow========================================================================
MCAliceDetectorInfoWindow::MCAliceDetectorInfoWindow()
{
}

//=MCAliceDetectorInfoWindow========================================================================
MCAliceDetectorInfoWindow::~MCAliceDetectorInfoWindow()
{
}

//=MCAliceDetectorInfoWindow========================================================================
void MCAliceDetectorInfoWindow::Init(const TGWindow *p, UInt_t w, UInt_t h, Int_t detector)
{
    fMainFrame = new TGMainFrame(p, w, h);
    fMainFrame->DontCallClose();
    TGGroupFrame      *gf = nullptr; 
    TGHorizontalFrame *hf = nullptr; 
    TString           filename("");

    switch (detector)
    {
    case 0:
        gf = new TGGroupFrame(fMainFrame, "ALICE Detector");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/ALICE.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("ALICE Detector Info");
        break;
    case 1:
        gf = new TGGroupFrame(fMainFrame, "ITS Detector");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/ITS.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("ITS Detector Info");
        break;    
    case 2:
        gf = new TGGroupFrame(fMainFrame, "TPC Detector");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/TPC.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("TPC Detector Info");
        break;   
    case 3:
        gf = new TGGroupFrame(fMainFrame, "TRD & TOF Detectors");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/TRDTOF.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("TRD & TOF Detectors Info");
        break;    
    case 4:
        gf = new TGGroupFrame(fMainFrame, "PHOS Detector");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/PHOS.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("PHOS Detector Info");
        break;    
    case 5:
        gf = new TGGroupFrame(fMainFrame, "HMPID Detector");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/HMPID.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("HMPID Detector Info");
        break;         
    default:
        gf = new TGGroupFrame(fMainFrame, "ALICE Detector");
        hf = new TGHorizontalFrame(gf, 250, 250);
        filename = "../images/ALICE.png";
        fPB = new TGPictureButton(hf, gClient->GetPicture(filename));
        fMainFrame->SetWindowName("ALICE Detector Info");
        break;
     }
    fPB->Connect("Clicked()", "TGMainFrame", fMainFrame, "UnmapWindow()");
    hf->AddFrame(fPB, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    gf->AddFrame(hf);
    fMainFrame->AddFrame(gf);
    fMainFrame->MapSubwindows();
    fMainFrame->Resize(fMainFrame->GetDefaultSize());
    fMainFrame->MapWindow();
}
//=MCAliceDetectorInfoWindow========================================================================
MCAliceDetectorInfoWindow & MCAliceDetectorInfoWindow::Instance()
{
    return fInstance; 
}
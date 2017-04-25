#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <TGFileDialog.h>
#include <TRootHelpDialog.h>

#include "TImage.h"
#include "TRecorder.h"

#include <TGClient.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>

#include <TGLOverlayButton.h>

#include <TG3DLine.h>
#include <TAxis3D.h>
#include <TStyle.h>
#include <TLegend.h>

#include <TApplication.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveWindowManager.h>
#include <TEveVSD.h>
#include <TEveVSDStructs.h>

#include <TCanvas.h>

#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveGeoShape.h>

#include <TEveSelection.h>

#include <TGLEmbeddedViewer.h>
#include <TCanvas.h>
#include <TParticlePDG.h>

#include <TH1D.h>

#include <TGTextEntry.h>

#include <TGLabel.h>

#include <TGTab.h>
#include <TGButton.h>
#include <TTimer.h>

#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TPRegexp.h>

#include <TGNumberEntry.h>
#include <TGLWidget.h>

#include "MultiView.C"

using namespace std;

namespace
{
  const char *gMasterClassOutputData[] = {"MasterClass Output Data", "*.masterclass", 0, 0}; //for loading results
}

namespace
{
  const char *gMasterClassOutputPictue[] = {"MasterClass Histograms", "*.png", 0, 0}; //for loading results
}

MultiView* gMultiView = 0;

TH1D* gKRapHist = 0;
TH1D* gXRapHist = 0;
TH1D* gLRapHist = 0;
TH1D* gALRapHist = 0;
TH1D* gAllRapHist = 0;

TH1D* gKMinvHist = 0;
TH1D* gXMinvHist = 0;
TH1D* gLMinvHist = 0;
TH1D* gALMinvHist = 0;
TH1D* gAllMinvHist = 0;

TCanvas* pad1 = 0;
TCanvas* pad2 = 0;

TGCheckButton* gCheckVertex = 0;
TGCheckButton* gCheckClusters = 0;
TGCheckButton* gCheckITSClusters = 0;
TGCheckButton* gCheckTPCClusters = 0;
TGCheckButton* gCheckTRDClusters = 0;
TGCheckButton* gCheckTOFClusters = 0;
TGCheckButton* gCheckESDTracks = 0;
TGCheckButton* gCheckV0s = 0;
TGCheckButton* gCheckCascades = 0;

TGLabel* gEventNumber = 0;
TGLabel* gEventAnalysed = 0;

TEveLine* pointingLine[10] = {0};
TEveLine* pointingLine1[10] = {0};
TEveLine* pointingLine2[10] = {0};
TEveLine* guideX[12] = {0};
TEveLine* guideY[14] = {0};
TEveLine* guideZ[14] = {0};

Int_t globalChoice = 0;
Int_t globalMode = 0;
Int_t globalDataset = 0;
Int_t gLoads = 0;

Double_t minvLambda[1000] = {0};
Double_t minvALambda[1000] = {0};
Double_t minvKaon[1000] = {0};
Double_t minvXi[1000] = {0};
Double_t minvOmega[1000] = {0};

Double_t rapLambda[1000] = {0};
Double_t rapALambda[1000] = {0};
Double_t rapKaon[1000] = {0};
Double_t rapXi[1000] = {0};
Double_t rapOmega[1000] = {0};

Int_t nminvLambda = 0;
Int_t nminvALambda = 0;
Int_t nminvKaon = 0;
Int_t nminvXi = 0;

Int_t countLambdas = 0;
Int_t countALambdas = 0;
Int_t countKaons = 0;
Int_t countXis = 0;
Int_t countOmegas = 0;

Int_t nV0Tracks = 0;
Int_t nCascadeTracks = 0;

Double_t pxPos[10] = {0};
Double_t pyPos[10] = {0};
Double_t pzPos[10] = {0};
Double_t massPos[10] = {0};

Double_t pxNeg[10] = {0};
Double_t pyNeg[10] = {0};
Double_t pzNeg[10] = {0};
Double_t massNeg[10] = {0};

Double_t pxPosCas[10] = {0};
Double_t pyPosCas[10] = {0};
Double_t pzPosCas[10] = {0};
Double_t massPosCas[10] = {0};

Double_t pxNegCas[10] = {0};
Double_t pyNegCas[10] = {0};
Double_t pzNegCas[10] = {0};
Double_t massNegCas[10] = {0};

Double_t pxBacCas[10] = {0};
Double_t pyBacCas[10] = {0};
Double_t pzBacCas[10] = {0};
Double_t massBacCas[10] = {0};
Double_t chargeBacCas[10] = {0};

Double_t pxPosFound = 0;
Double_t pyPosFound = 0;
Double_t pzPosFound = 0;
Double_t massPosFound = 0;

Double_t pxNegFound = 0;
Double_t pyNegFound = 0;
Double_t pzNegFound = 0;
Double_t massNegFound = 0;

Int_t chargeNegFound = -1;
Int_t chargePosFound = 1;

Bool_t foundNeg = kFALSE;
Bool_t foundPos = kFALSE;
Bool_t teacher = kFALSE;

Double_t pcaX[10] = {0};
Double_t pcaY[10] = {0};
Double_t pcaZ[10] = {0};

Double_t pcaXCas[10] = {0};
Double_t pcaYCas[10] = {0};
Double_t pcaZCas[10] = {0};

Double_t pcaXBacCas[10] = {0};
Double_t pcaYBacCas[10] = {0};
Double_t pcaZBacCas[10] = {0};

Int_t detector = 0;
Int_t geometry = 0;

Int_t fCount = 0;

Bool_t change = kTRUE;

TString gMiniGeom = gSystem->Getenv("GEOPATH") + TString("alice_mini_geom.root");


//--------------------------------------------------------------
//additional classes
//--------------------------------------------------------------

	//--------------------------------------------------------------
class PatternsInfoWindow : public TGMainFrame
{

  TGPictureButton *b1;

public:

	//--------------------------------------------------------------
  PatternsInfoWindow(const TGWindow *p, UInt_t w, UInt_t h, Int_t detector) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

    b1 = 0;
    TGGroupFrame* groupFrame = 0;
    TGHorizontalFrame *hframe00 = 0;

    switch (detector)
      {
      case 0:
	groupFrame = new TGGroupFrame(this, "Kaon");
	hframe00 = new TGHorizontalFrame(groupFrame, 250, 250);
	b1 = new TGPictureButton(hframe00, gClient->GetPicture("eve_files/kaon.png"));
	SetWindowName("Kaon Pattern");
	break;

      case 1:
	groupFrame = new TGGroupFrame(this, "Lambda");
	hframe00 = new TGHorizontalFrame(groupFrame, 250, 250);
	b1 = new TGPictureButton(hframe00, gClient->GetPicture("eve_files/lambda.png"));
	SetWindowName("Lambda Pattern");
	break;

      case 2:
	groupFrame = new TGGroupFrame(this, "AntiLambda");
	hframe00 = new TGHorizontalFrame(groupFrame, 250, 250);
	b1 = new TGPictureButton(hframe00, gClient->GetPicture("eve_files/antilambda.png"));
	SetWindowName("AntiLambda Pattern");
	break;

      case 3:
	groupFrame = new TGGroupFrame(this, "Xi");
	hframe00 = new TGHorizontalFrame(groupFrame, 250, 250);
	b1 = new TGPictureButton(hframe00, gClient->GetPicture("eve_files/xi.png"));
	SetWindowName("Xi Pattern");
	break;

      default:
	groupFrame = new TGGroupFrame(this, "Kaon");
	hframe00 = new TGHorizontalFrame(groupFrame, 250, 250);
	b1 = new TGPictureButton(hframe00, gClient->GetPicture("eve_files/kaon.png"));
	SetWindowName("Kaon Pattern");
	break;
      }

    b1->Connect("Clicked()", "PatternsInfoWindow", this, "UnmapWindow()");

    hframe00->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupFrame->AddFrame(hframe00);

    AddFrame(groupFrame);

    MapSubwindows();

    Resize(GetDefaultSize());

    MapWindow();

  }

  ClassDef(PatternsInfoWindow, 0);

};

//--------------------------------------------------------------
PatternsInfoWindow* gPatternsInfoWindow = 0;

class AlicePatternInfo : public TGMainFrame
{

protected:


  TGPictureButton *b1;

public:

	//--------------------------------------------------------------
  AlicePatternInfo(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

    b1 = 0;

    TGTextButton *b2 = 0;

    TGGroupFrame* gf = new TGGroupFrame(this, "Patterns");

    TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 250, 250);
	
	TString fileName("kaon_small.png");			

    b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));

    b1->Connect("Clicked()", "AlicePatternInfo", this, "MakeBigger()");

    hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf);
    {

      b2 = new TGTextButton(hf, "Kaon");
      b2->Connect("Clicked()", "AlicePatternInfo", this, "ShowALICE()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "Lambda");
      b2->Connect("Clicked()", "AlicePatternInfo", this, "ShowITS()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "AntiLambda");
      b2->Connect("Clicked()", "AlicePatternInfo", this, "ShowTPC()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "Xi");
      b2->Connect("Clicked()", "AlicePatternInfo", this, "ShowTRDTOF()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    hf = new TGHorizontalFrame(gf);
    {
		
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b2 = new TGTextButton(hf, "Close");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b2 = new TGTextButton(hf, "Fermer");
		b2->Connect("Clicked()", "AlicePatternInfo", this, "UnmapWindow()");
		hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    AddFrame(gf);

    SetWindowName("Pattern Info");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();

  }

  //--------------------------------------------------------------
  void ShowALICE()
  {

	  TString fileName("kaon_small.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 0;

  }

  //--------------------------------------------------------------
  void ShowITS()
  {

	  TString fileName("lambda_small.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 1;

  }

  //--------------------------------------------------------------
  void ShowTPC()
  {

	  TString fileName("antilambda_small.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH")))); 
	  b1->Draw();

	  detector = 2;

  }

  //--------------------------------------------------------------
  void ShowTRDTOF()
  {

	  TString fileName("xi_small.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH")))); 
	  b1->Draw();

	  detector = 3;

  }

	//--------------------------------------------------------------
  void MakeBigger()
  {

    gPatternsInfoWindow = new PatternsInfoWindow(gClient->GetRoot(),100,100, detector);

  }

  ClassDef(AlicePatternInfo, 0);

};

AlicePatternInfo* gAlicePatternInfo = 0;


//----------------KASIA--------------
class AlicePatternInfo2 : public TGMainFrame
{

protected:


 TGPictureButton *b1;

public:

	//--------------------------------------------------------------
  AlicePatternInfo2(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

    b1 = 0;

    TGTextButton *b2 = 0;

    TGGroupFrame* gf = new TGGroupFrame(this, "Patterns");

    TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 250, 250);
	
	TString fileName("lambda_2.png");			

    b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	

    b1->Connect("Clicked()", "AlicePatternInfo2", this, "MakeBigger()");

    hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf);
    {

      b2 = new TGTextButton(hf, "Kaon");
      b2->Connect("Clicked()", "AlicePatternInfo2", this, "ShowALICE2()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "Lambda");
      b2->Connect("Clicked()", "AlicePatternInfo2", this, "ShowITS2()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "Xi");
      b2->Connect("Clicked()", "AlicePatternInfo2", this, "ShowTRDTOF2()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    hf = new TGHorizontalFrame(gf);
    {

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b2 = new TGTextButton(hf, "Close");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b2 = new TGTextButton(hf, "Fermer");
		b2->Connect("Clicked()", "AlicePatternInfo2", this, "UnmapWindow()");
		hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    AddFrame(gf);

  SetWindowName("Pattern Info");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();

  }

  //--------------------------------------------------------------
  void ShowALICE2()
  {

	  TString fileName("k0_2.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH")))); 

	  b1->Draw();

	  detector = 0;

  }

  //--------------------------------------------------------------
  void ShowITS2()
  {

	  TString fileName("lambda_2.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH")))); 
	  b1->Draw();

	  detector = 1;

  }

 

  //--------------------------------------------------------------
  void ShowTRDTOF2()
  {

	  TString fileName("xi_2.png");			

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH")))); 
	  b1->Draw();

	  detector = 3;

  }

	//--------------------------------------------------------------
  void MakeBigger()
  {

//   gPatternsInfoWindow = new PatternsInfoWindow(gClient->GetRoot(),100,100, detector);

  }

  ClassDef(AlicePatternInfo2, 0);

};

AlicePatternInfo2* gAlicePatternInfo2 = 0;


//----------------KASIA-------------------


class AliceDetectorInfoWindow : public TGMainFrame
{

  TGPictureButton *b1;

public:

	//--------------------------------------------------------------
  AliceDetectorInfoWindow(const TGWindow *p, UInt_t w, UInt_t h, Int_t detector) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

    b1 = 0;

    TGGroupFrame* gf = 0;
    TGHorizontalFrame *hf = 0;

    TString fileName;			

    switch (detector)
      {
      case 0:
	gf = new TGGroupFrame(this, "ALICE Detector");
	hf = new TGHorizontalFrame(gf, 250, 250);

    fileName = "ALICE.png";			

	
	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("ALICE Detector Info");
	break;

      case 1:
	gf = new TGGroupFrame(this, "ITS Detector");
	hf = new TGHorizontalFrame(gf, 250, 250);
    fileName = "ITS.png";			

	
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("ITS Detector Info");
	break;

      case 2:
	gf = new TGGroupFrame(this, "TPC Detector");
	hf = new TGHorizontalFrame(gf, 250, 250);
    fileName = "TPC.png";			

	
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("TPC Detector Info");
	break;

      case 3:
	gf = new TGGroupFrame(this, "TRD and TOF Detectors");
	hf = new TGHorizontalFrame(gf, 250, 250);
    fileName = "TRDTOF.png";				
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("TRD and TOF Detectors Info");
	break;

      case 4:
	gf = new TGGroupFrame(this, "PHOS Detector");
	hf = new TGHorizontalFrame(gf, 250, 250);
    fileName = "PHOS.png";				
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("PHOS Detector Info");
	break;

      case 5:
	gf = new TGGroupFrame(this, "HMPID Detector");
	hf = new TGHorizontalFrame(gf, 250, 250);
    fileName = "HMPID.png";				
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("HMPID Detector Info");
	break;

      default:
	gf = new TGGroupFrame(this, "ALICE Detector");
	hf = new TGHorizontalFrame(gf, 250, 250);
    fileName = "ALICE.png";				
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	SetWindowName("ALICE Detector Info");
	break;
      }

    b1->Connect("Clicked()", "AliceDetectorInfoWindow", this, "UnmapWindow()");

    hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    AddFrame(gf);

    MapSubwindows();

    Resize(GetDefaultSize());

    MapWindow();

  }

  ClassDef(AliceDetectorInfoWindow, 0);

};

AliceDetectorInfoWindow* gAliceDetectorInfoWindow = 0;

	//--------------------------------------------------------------
class AliceDetectorInfo : public TGMainFrame
{

protected:

  TGPictureButton *b1;

public:

	//--------------------------------------------------------------
  AliceDetectorInfo(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

    b1 = 0;

    TGTextButton *b2 = 0;

    TGGroupFrame* gf = new TGGroupFrame(this, "ALICE Detector");

    TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 250, 250);

    TString fileName("ALICE_small2.png");				
  	b1 = new TGPictureButton(hf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));

    b1->Connect("Clicked()", "AliceDetectorInfo", this, "MakeBigger()");

    hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf);
    {

      b2 = new TGTextButton(hf, "ALICE");
      b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowALICE()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "ITS");
      b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowITS()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "TPC");
      b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowTPC()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "TRD+TOF");
      b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowTRDTOF()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "PHOS");
      b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowPHOS()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b2 = new TGTextButton(hf, "HMPID");
      b2->Connect("Clicked()", "AliceDetectorInfo", this, "ShowHMPID()");
      hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    hf = new TGHorizontalFrame(gf);
    {

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b2 = new TGTextButton(hf, "Close");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b2 = new TGTextButton(hf, "Fermer");
		b2->Connect("Clicked()", "AliceDetectorInfo", this, "UnmapWindow()");
		hf->AddFrame(b2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    AddFrame(gf);

    SetWindowName("ALICE Detector Info");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();

  }

  //--------------------------------------------------------------
  void ShowALICE()
  {

	  TString fileName("ALICE_small2.png");				

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 0;

  }

  //--------------------------------------------------------------
  void ShowITS()
  {

	  TString fileName("ITS_small.png");				

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 1;

  }

  //--------------------------------------------------------------
  void ShowTPC()
  {

	  TString fileName("TPC_small.png");				

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 2;

  }

  //--------------------------------------------------------------
  void ShowTRDTOF()
  {

	  TString fileName("TRDTOF_small.png");				

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 3;

  }

  //--------------------------------------------------------------
  void ShowPHOS()
  {

	  TString fileName("PHOS_small.png");				

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 4;

  }

  //--------------------------------------------------------------
  void ShowHMPID()
  {

	  TString fileName("HMPID_small.png");				

	  b1->SetPicture(gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
	  b1->Draw();

	  detector = 5;

  }

	//--------------------------------------------------------------
  void MakeBigger()
  {

    gAliceDetectorInfoWindow = new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, detector);

  }


  ClassDef(AliceDetectorInfo, 0);

};

AliceDetectorInfo* gAliceDetectorInfo = 0;

	//--------------------------------------------------------------
class AliceCalculator : public TGMainFrame
{

protected:

  TGNumberEntryField *fpx1;
  TGNumberEntryField *fpx2;
  TGNumberEntryField *fpx3;
  TGNumberEntryField *fpy1;
  TGNumberEntryField *fpy2;
  TGNumberEntryField *fpy3;
  TGNumberEntryField *fpz1;
  TGNumberEntryField *fpz2;
  TGNumberEntryField *fpz3;
  TGNumberEntryField *fmass1;
  TGNumberEntryField *fmass2;
  TGNumberEntryField *fmass3;
  TGNumberEntryField *frapiditycalc;
  TGNumberEntryField *finvariant_masscalc;

public:

	//--------------------------------------------------------------
  AliceCalculator(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
  {
    // Create main frame

    DontCallClose();

    TGGroupFrame* gf = nullptr; 
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    gf = new TGGroupFrame(this, "Calculator Instructions");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    gf = new TGGroupFrame(this, "Instructions Calculateurs");

    TGHorizontalFrame* hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);

    {
      const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
      FontStruct_t buttonFont = font->GetFontStruct();
      ULong_t buttonRedColor;
      gClient->GetColorByName("red", buttonRedColor);
      TGTextButton *b = new TGTextButton(hf,"Instructions");
      b->SetTextColor(buttonRedColor);
      b->SetFont(buttonFont);
      hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b->Connect("Clicked()", "AliceCalculator", this, "Instructions()");
    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    gf = new TGGroupFrame(this, "Particle Table");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    gf = new TGGroupFrame(this, "Tableau des Particules");

    TGLabel *val1 = 0;
    TGLabel *val2 = 0;

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
	    val1 = new TGLabel(hf, "Particle type");
	    val2 = new TGLabel(hf, "Mass [GeV/c2]");
	}
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
	    val1 = new TGLabel(hf, "Type de Particules");
		val2 = new TGLabel(hf, "Masse [GeV/c2]");
	}
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
    val1 = new TGLabel(hf, "Electron");
    val2 = new TGLabel(hf, "0.000511");
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
    val1 = new TGLabel(hf, "Pion");
    val2 = new TGLabel(hf, "0.139");
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    val1 = new TGLabel(hf, "Neutral Kaon");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    val1 = new TGLabel(hf, "Kaon Neutre");
    val2 = new TGLabel(hf, "0.497");
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
    val1 = new TGLabel(hf, "Proton");
    val2 = new TGLabel(hf, "0.938");
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
    val1 = new TGLabel(hf, "Lambda");
    val2 = new TGLabel(hf, "1.115");
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    val1 = new TGLabel(hf, "Charged Xi");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    val1 = new TGLabel(hf, "Xi Charge");
    val2 = new TGLabel(hf, "1.321");
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(val2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    gf = new TGGroupFrame(this, "Calculator");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    gf = new TGGroupFrame(this, "Calculateur");

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    val1 = new TGLabel(hf, "    ");
    val1->SetTextJustify(kTextCenterX);
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    val1 = new TGLabel(hf, "(-)");
    val1->SetTextJustify(kTextCenterX);
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    val1 = new TGLabel(hf, "(+)");
    val1->SetTextJustify(kTextCenterX);
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    val1 = new TGLabel(hf, "Bachelor");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    val1 = new TGLabel(hf, "Celibataire");
    val1->SetTextJustify(kTextCenterX);
    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

  hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    val1 = new TGLabel(hf, "px");
    fpx1 = new TGNumberEntryField(hf);
    fpx2 = new TGNumberEntryField(hf);
    fpx3 = new TGNumberEntryField(hf);

    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpx1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpx2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpx3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    val1 = new TGLabel(hf, "py");
    fpy1 = new TGNumberEntryField(hf);
    fpy2 = new TGNumberEntryField(hf);
    fpy3 = new TGNumberEntryField(hf);

    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpy1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpy2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpy3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    val1 = new TGLabel(hf, "pz");
    fpz1 = new TGNumberEntryField(hf);
    fpz2 = new TGNumberEntryField(hf);
    fpz3 = new TGNumberEntryField(hf);

    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpz1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpz2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fpz3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    val1 = new TGLabel(hf, "mass");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    val1 = new TGLabel(hf, "masse");
    fmass1 = new TGNumberEntryField(hf);
    fmass2 = new TGNumberEntryField(hf);
    fmass3 = new TGNumberEntryField(hf);

    hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fmass1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fmass2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
    hf->AddFrame(fmass3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    TGTextButton* b; 
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(hf, "Invariant Mass");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(hf, "Masse Invariante");

    b->Connect("Pressed()", "AliceCalculator", this, "CalculateInvariantMass()");

    hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    finvariant_masscalc= new TGNumberEntryField(hf);

    finvariant_masscalc->SetEnabled(kFALSE);

    hf->AddFrame(finvariant_masscalc, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------
/*
    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    b = new TGTextButton(hf, "Rapidity");

    b->Connect("Pressed()", "AliceCalculator", this, "CalculateRapidity()");

    hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
*/
    //---------------------------------------------------------------------------
/*
    hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);

    frapiditycalc = new TGNumberEntryField(hf);

    frapiditycalc->SetEnabled(kFALSE);

    hf->AddFrame(frapiditycalc, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
*/
    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "That's a Kaon!");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Ceci est un Kaon !");
    b->Connect("Pressed()", "AliceCalculator", this, "FoundKaon()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "That's a Lambda!");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Ceci est un Lambda !");
    b->Connect("Pressed()", "AliceCalculator", this, "FoundLambda()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "That's a anti-Lambda!");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Ceci est un anti-Lambda !");
    b->Connect("Pressed()", "AliceCalculator", this, "FoundALambda()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "That's a Xi!");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Ceci est un Xi !");
    b->Connect("Pressed()", "AliceCalculator", this, "FoundXi()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "That's background!");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Ceci est du bruit de fond !");
    b->Connect("Pressed()", "AliceCalculator", this, "Background()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "Clear");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Effacer");
    b->Connect("Pressed()", "AliceCalculator", this, "SetZero()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));


//--------------------------------------------
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "Load");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Charger");
    b->Connect("Pressed()", "AliceCalculator", this, "Load()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    b = new TGTextButton(gf, "Save");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    b = new TGTextButton(gf, "Sauvegarder");
    b->Connect("Pressed()", "AliceCalculator", this, "SaveResults()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		b = new TGTextButton(gf, "Close");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		b = new TGTextButton(gf, "Fermer");
	b->Connect("Pressed()", "AliceCalculator", this, "DoExit()");

    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    //---------------------------------------------------------------------------

    AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    // Set a name to the main frame      
    SetWindowName("Calculator");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();

  }

	//--------------------------------------------------------------
  void DoExit()
  {
    this->UnmapWindow();
  }

	//--------------------------------------------------------------
  void SetZero()
  {

    fpx1->SetNumber(0);
    fpy1->SetNumber(0);
    fpz1->SetNumber(0);
    fmass1->SetNumber(0);

    fpx2->SetNumber(0);
    fpy2->SetNumber(0);
    fpz2->SetNumber(0);
    fmass2->SetNumber(0);

    fpx3->SetNumber(0);
    fpy3->SetNumber(0);
    fpz3->SetNumber(0);
    fmass3->SetNumber(0);

  }

	//--------------------------------------------------------------
  void Instructions(){

    TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "CALCULATOR INSTRUCTIONS", 700, 400);

    instructions->SetText("\
Calculator Instructions \n\n\
\
The Calculator is provided to allow you to calculate rapidity and invariant mass\n\
of the mother partice given the daughter particles characteristics. Is is divided\n\
into 3 segments:\n\n\
\
   Instructions\n\n\
\
   Particle Table - shows various types of particles and their masses, use the table\n\
   to check the identity of the tracks from V0 and Cascade decays\n\n\
\
   Calculator - with:n\n\
      12 Number Fields (4 rows: px, py, pz and mass, 3 columns: (-), (+), Bachelor) - \n\
      the parameters of chosen track will be copied here\n\
      Invariant mass - calculates the invariant mass of the mother particle, the\n\
      result is displayed below the button\n\
      That's a Kaon/Lambda/antiLambda/Xi!- If you have found a particle click on its button\n\
      Clear - Clear all Number Fields \n\
      Load- Load your results\n\
      Save- Save your results (autosave at the end of each event)\n\
      Close- close the Calculator\n\n\
\
      ");

    instructions->Popup();

  }

	//--------------------------------------------------------------
  void Set1(Double_t px1, Double_t py1, Double_t pz1, Double_t mass1)
  {
    fpx1->SetNumber(px1);
    fpy1->SetNumber(py1);
    fpz1->SetNumber(pz1);
    fmass1->SetNumber(mass1);

    change = kTRUE;

  }

	//--------------------------------------------------------------
  void Set2(Double_t px2, Double_t py2, Double_t pz2, Double_t mass2)
  {
    fpx2->SetNumber(px2);
    fpy2->SetNumber(py2);
    fpz2->SetNumber(pz2);
    fmass2->SetNumber(mass2);

    change = kTRUE;

  }

	//--------------------------------------------------------------
  void Set3(Double_t px3, Double_t py3, Double_t pz3, Double_t mass3)
  {
    fpx3->SetNumber(px3);
    fpy3->SetNumber(py3);
    fpz3->SetNumber(pz3);
    fmass3->SetNumber(mass3);

    change = kTRUE;

  }

	//--------------------------------------------------------------
  void CalculateRapidity()
  {

    Double_t px1, py1, pz1, mass1, E1, px2, py2, pz2, mass2, E2, px3, py3, pz3, mass3, E3, minv, E, rap;

    px1 = fpx1->GetNumber();
    py1 = fpy1->GetNumber();
    pz1 = fpz1->GetNumber();
    mass1 = fmass1->GetNumber();
    E1 = TMath::Sqrt(px1*px1+py1*py1+pz1*pz1+mass1*mass1);

    px2 = fpx2->GetNumber();
    py2 = fpy2->GetNumber();
    pz2 = fpz2->GetNumber();
    mass2 = fmass2->GetNumber();
    E2 = TMath::Sqrt(px2*px2+py2*py2+pz2*pz2+mass2*mass2);

    px3 = fpx3->GetNumber();
    py3 = fpy3->GetNumber();
    pz3 = fpz3->GetNumber();
    mass3 = fmass3->GetNumber();
    E3 = TMath::Sqrt(px3*px3+py3*py3+pz3*pz3+mass3*mass3);

    minv = TMath::Sqrt((E1+E2+E3)*(E1+E2+E3)-(px1+px2+px3)*(px1+px2+px3)-(py1+py2+py3)*(py1+py2+py3)-(pz1+pz2+pz3)*(pz1+pz2+pz3));

    E = TMath::Sqrt((px1+px2+px3)*(px1+px2+px3)+(py1+py2+py3)*(py1+py2+py3)+(pz1+pz2+pz3)*(pz1+pz2+pz3)+minv*minv);

    rap = 0.5*TMath::Log((E+pz1+pz2+pz3)/(E-pz1-pz2-pz3));

    frapiditycalc->SetNumber(rap);

  }

	//--------------------------------------------------------------
  void CalculateInvariantMass()
  {
    Double_t px1, py1, pz1, mass1, E1, px2, py2, pz2, mass2, E2, px3, py3, pz3, mass3, E3, minv;//, minv2, E4;

    px1 = fpx1->GetNumber();
    py1 = fpy1->GetNumber();
    pz1 = fpz1->GetNumber();
    mass1 = fmass1->GetNumber();
    E1 = TMath::Sqrt(px1*px1+py1*py1+pz1*pz1+mass1*mass1);

    px2 = fpx2->GetNumber();
    py2 = fpy2->GetNumber();
    pz2 = fpz2->GetNumber();
    mass2 = fmass2->GetNumber();
    E2 = TMath::Sqrt(px2*px2+py2*py2+pz2*pz2+mass2*mass2);

    px3 = fpx3->GetNumber();
    py3 = fpy3->GetNumber();
    pz3 = fpz3->GetNumber();
    mass3 = fmass3->GetNumber();
    E3 = TMath::Sqrt(px3*px3+py3*py3+pz3*pz3+mass3*mass3);

    minv = TMath::Sqrt((E1+E2+E3)*(E1+E2+E3)-(px1+px2+px3)*(px1+px2+px3)-(py1+py2+py3)*(py1+py2+py3)-(pz1+pz2+pz3)*(pz1+pz2+pz3));

    finvariant_masscalc->SetNumber(minv);

  }

	//--------------------------------------------------------------
  void FillHistogram(TH1D* minvhist, TH1D* raphist, Double_t minv, Double_t rap)
  {
    minvhist->Fill(minv);
    raphist->Fill(rap);
  }

	//--------------------------------------------------------------
  void FoundLambda()
  {

    if(change)
    {
      FillHistogram(gLMinvHist, gLRapHist, finvariant_masscalc->GetNumber(), 0/*frapiditycalc->GetNumber()*/);
      minvLambda[countLambdas] = finvariant_masscalc->GetNumber();
      //rapLambda[countLambdas] = frapiditycalc->GetNumber();
      countLambdas++;
    }

    change = kFALSE;

  }

	//--------------------------------------------------------------
  void FoundALambda()
  {
    if(change)
    {
      FillHistogram(gALMinvHist, gALRapHist, finvariant_masscalc->GetNumber(), 0/*frapiditycalc->GetNumber()*/);
      minvALambda[countALambdas] = finvariant_masscalc->GetNumber();
      //rapALambda[countALambdas] = frapiditycalc->GetNumber();
      countALambdas++;
    }

    change = kFALSE;

  }

	//--------------------------------------------------------------
  void FoundKaon()
  {
    if(change)
    {

      FillHistogram(gKMinvHist, gKRapHist, finvariant_masscalc->GetNumber(), 0/*frapiditycalc->GetNumber()*/);
      minvKaon[countKaons] = finvariant_masscalc->GetNumber();
      //rapKaon[countKaons] = frapiditycalc->GetNumber();
      countKaons++;
    }

    change = kFALSE;

  }

	//--------------------------------------------------------------
  void FoundXi()
  {
    if(change)
    {

      FillHistogram(gXMinvHist, gXRapHist, finvariant_masscalc->GetNumber(), 0/*frapiditycalc->GetNumber()*/);
      minvXi[countXis] = finvariant_masscalc->GetNumber();
      //rapXi[countXis] = frapiditycalc->GetNumber();
      countXis++;
    }

    change = kFALSE;

  }
//-------------------!!!!!!!!!!!!!!!--------------------------------
void Background()
  {

  }

	//--------------------------------------------------------------
  void SaveResults()
  {

    TGFileInfo fi;
    fi.fFileTypes   = gMasterClassOutputData;
    fi.fIniDir      = StrDup(""); // current directory
    fi.fFileTypeIdx = 0;
    fi.fOverwrite   = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(),gEve->GetMainWindow(), kFDSave, &fi);
    if (!fi.fFilename) return;

    TPMERegexp filere(".*/([^/]+$)");
    if (filere.Match(fi.fFilename) != 2)
      {
//        Warning("AliEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }

    gSystem->ChangeDirectory(fi.fIniDir);

    ofstream myresult(TString::Format("%s.masterclass",fi.fFilename));

    myresult << "Kaons" << endl;
    myresult << countKaons << endl;
    for(Int_t i=0; i<countKaons; i++)
      myresult << minvKaon[i] << " " << rapKaon[i] <<endl;

    myresult << "Lambas" << endl;
    myresult << countLambdas << endl;
    for(Int_t i=0; i<countLambdas; i++)
      myresult << minvLambda[i] << " " << rapLambda[i]  <<endl;

    myresult << "AntiLambas" << endl;
    myresult << countALambdas << endl;
    for(Int_t i=0; i<countALambdas; i++)
      myresult << minvALambda[i] << " " << rapALambda[i]  <<endl;

    myresult << "Xis" << endl;
    myresult << countXis << endl;
    for(Int_t i=0; i<countXis; i++)
      myresult << minvXi[i] << " " << rapXi[i]  <<endl;

    myresult << "Omegas" << endl;
    myresult << countOmegas << endl;
    for(Int_t i=0; i<countOmegas; i++)
      myresult << minvOmega[i] << " " << rapOmega[i]  <<endl;

    myresult << "Others" << endl;
    myresult << 0 << endl;

    myresult.close();

  }

	//--------------------------------------------------------------
  void Load()
  {

    TGFileInfo fi;
    fi.fFileTypes   = gMasterClassOutputData;
    fi.fIniDir      = StrDup("");
    fi.fFileTypeIdx = 0;
    fi.fOverwrite   = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(),gEve->GetMainWindow(), kFDOpen, &fi);
    if (!fi.fFilename) return;

    TPMERegexp filere(".*/([^/]+$)");
    if (filere.Match(fi.fFilename) != 2)
      {
//        Warning("AliEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }

    gSystem->ChangeDirectory(fi.fIniDir);

    string file1(filere[1]);

    ifstream data(fi.fFilename);

    string line;
    Double_t minv = 0;
    Double_t rapidity = 0;
    Int_t nparticles = 0;
  
    if(data.is_open()){

      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countKaons = countKaons+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
         
	  gKMinvHist->Fill(minv);
//	  gKRapHist->Fill(rapidity);

	}

      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countLambdas = countLambdas+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
         
	  gLMinvHist->Fill(minv);
	  gLRapHist->Fill(rapidity);

	}


      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countALambdas = countALambdas+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
         
	  gALMinvHist->Fill(minv);
//	  gALRapHist->Fill(rapidity);

	}

      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countXis = countXis+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
	  gXMinvHist->Fill(minv);
//	  gXRapHist->Fill(rapidity);

	}

      data.close();

//      Double_t maxRap[4] = {gKRapHist->GetMaximum(), gXRapHist->GetMaximum(), gLRapHist->GetMaximum(), gALRapHist->GetMaximum()};
      Double_t maxMinv[4] = {gKMinvHist->GetMaximum(), gXMinvHist->GetMaximum(), gLMinvHist->GetMaximum(), gALMinvHist->GetMaximum()};

      gAllMinvHist->GetYaxis()->SetRangeUser(0,1.2*TMath::MaxElement(4,maxMinv));
//      gAllRapHist->GetYaxis()->SetRangeUser(0,1.2*TMath::MaxElement(4,maxRap));

    }
  }


  ClassDef(AliceCalculator, 0);

};

AliceCalculator* gAliceCalculator = 0;

//---------------------------------------------------------------------------

class GroupBox : public TGGroupFrame {
private:
  TGNumberEntryField *fEntry;

public:
  GroupBox(const TGWindow *p, const char *name, const char *title);
  TGNumberEntryField *GetEntry() const { return fEntry; }
  Double_t GetNumber() const { return fEntry->GetNumber(); }

  ClassDef(GroupBox, 0)
};

	//--------------------------------------------------------------
GroupBox::GroupBox(const TGWindow *p, const char *name, const char *title) :
  TGGroupFrame(p, name) 
{

  TGHorizontalFrame *hf = new TGHorizontalFrame(this);

  TGLabel *label = new TGLabel(hf, title);
  hf->AddFrame(label, new TGLayoutHints(kLHintsLeft | kLHintsCenterY));

  AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  fEntry = new TGNumberEntryField(this);
  AddFrame(fEntry, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

}


//---------------------------------------------------------------------------

/*
class Views {

protected:
  TGMainFrame *mf;
  TGCompositeFrame* evf;
  TGCompositeFrame* evf2;


public:
  virtual ~Views() { delete mf; }
  Views(Int_t n)
  {
    // Main test window.
   
    mf = new TGMainFrame(gClient->GetRoot(), 500, 1100, kVerticalFrame);

    // recusively delete all subframes on exit
    mf->SetCleanup(kDeepCleanup);

    mf->SetWindowName(TString::Format("V0 View %d", n));

    evf = new TGCompositeFrame(mf, 500, 500);
    mf->AddFrame(evf, new TGLayoutHints(kLHintsNormal | kLHintsExpandX | kLHintsExpandY));

    TGLabel *label1 = new TGLabel(evf,"R-Phi View");
    evf->AddFrame(label1, new TGLayoutHints(kLHintsNormal | kLHintsExpandX));

    TGLEmbeddedViewer* ev = new TGLEmbeddedViewer(evf);
    evf->AddFrame(ev->GetFrame(), new TGLayoutHints(kLHintsNormal | kLHintsExpandX | kLHintsExpandY));

    TEveViewer* eve_v = new TEveViewer("XY View");
    eve_v->SetGLViewer(ev, ev->GetFrame());
    eve_v->GetGLViewer()->SwitchColorSet();
    eve_v->AddScene(gEve->GetEventScene());
    eve_v->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
    TGLCamera& dpCam = eve_v->GetGLViewer()->CurrentCamera();
    dpCam.Truck(pcaX[n],pcaY[n]);

    evf2 = new TGCompositeFrame(mf, 500, 500);
    mf->AddFrame(evf2, new TGLayoutHints(kLHintsNormal | kLHintsExpandX | kLHintsExpandY));

    TGLabel *label2 = new TGLabel(evf2,"Rho-Z View");
    evf2->AddFrame(label2, new TGLayoutHints(kLHintsNormal | kLHintsExpandX));

    TGLEmbeddedViewer* ev2 = new TGLEmbeddedViewer(evf2);
    evf2->AddFrame(ev2->GetFrame(), new TGLayoutHints(kLHintsNormal | kLHintsExpandX | kLHintsExpandY));

    TEveViewer* eve_v2 = new TEveViewer("RhoZ View");
    eve_v2->SetGLViewer(ev2, ev2->GetFrame());
    eve_v2->GetGLViewer()->SwitchColorSet();
    eve_v2->AddScene(gEve->GetEventScene());
    eve_v2->GetGLViewer()->SetCurrentCamera(TGLViewer::kCameraOrthoZOY);
    TGLCamera& dpCam2 = eve_v2->GetGLViewer()->CurrentCamera();
    dpCam2.Truck(pcaZ[n],pcaY[n]);

    TGPictureButton* b = 0;
    TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
    b = new TGPictureButton(mf, gClient->GetPicture(icondir + "ed_delete.png"));
    mf->AddFrame(b, new TGLayoutHints(kLHintsNormal |kLHintsExpandX));
    b->Connect("Clicked()", "TGMainFrame", mf, "UnmapWindow()");

    mf->MapSubwindows();
    mf->Resize();
    mf->MapRaised();

  }

  ClassDef(Views, 0);

};

*/

//---------------------------------------------------------------------------

class TextEntryWindow {

protected:
  TGMainFrame *fMain;     // main frame
  GroupBox    *fEcho;     // echo mode (echo, password, no echo)
  GroupBox    *fAlign;    // alignment (left, right, center)
  GroupBox    *fAccess;   // read-only mode
  GroupBox    *fBorder;   // border mode
  GroupBox    *fMinv;   // border mode

public:
  virtual ~TextEntryWindow() { delete fMain; }
	//--------------------------------------------------------------
  TextEntryWindow(Double_t px, Double_t py, Double_t pz, Double_t mass, Int_t charge, Bool_t isTrack, Bool_t isBachelor)
  {
    // Main test window.
   
    TGNumberEntryField *entry;

    fMain = new TGMainFrame(gClient->GetRoot(), 10, 10, kVerticalFrame);

    // recusively delete all subframes on exit
    fMain->SetCleanup(kDeepCleanup);

    fMain->DontCallClose();
   
    {
  	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	      fEcho = new GroupBox(fMain, "MomentumX:", "[GeV/c]");
      else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	      fEcho = new GroupBox(fMain, "Quantite de Mouvement X:", "[GeV/c]");
      fMain->AddFrame(fEcho, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
      entry = fEcho->GetEntry();
      entry->SetNumber(px);

  	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	      fAlign = new GroupBox(fMain, "MomentumY:", "[GeV/c]");
      else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	      fAlign = new GroupBox(fMain, "Quantite de Mouvement Y:", "[GeV/c]");
      fMain->AddFrame(fAlign, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
      entry = fAlign->GetEntry();
      entry->SetNumber(py);

  	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	      fAccess = new GroupBox(fMain, "MomentumZ:", "[GeV/c]");
      else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	      fAccess = new GroupBox(fMain, "Quantite de Mouvement Z:", "[GeV/c]");
      fMain->AddFrame(fAccess, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
      entry = fAccess->GetEntry();
      entry->SetNumber(pz);

      if(!isTrack)
	{
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			fBorder = new GroupBox(fMain, "Mass:", "[GeV/c^{2}]");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			fBorder = new GroupBox(fMain, "Masse:", "[GeV/c^{2}]");
	  fMain->AddFrame(fBorder, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
	  entry = fBorder->GetEntry();
	  entry->SetNumber(mass);
	}
      else
	{
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	  	  fBorder = new GroupBox(fMain, "Charge:", "");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	  	  fBorder = new GroupBox(fMain, "Charge:", "");
	  fMain->AddFrame(fBorder, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 5, 5));
	  entry = fBorder->GetEntry();
	  entry->SetNumber(charge);

	}

      TGTextButton* b = 0;

      if(charge < 0)
	{
  	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  fMain->SetWindowName("(-) Particle");
  	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  fMain->SetWindowName("(-) Particule");

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  b = new TGTextButton(fMain, "          Copy to calculator          ");
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  b = new TGTextButton(fMain, "          Copier vers calculateur          ");
	  fMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 5, 5));

	  if(isBachelor)
            b->Connect("Clicked()", "TextEntryWindow", this, "Copy3()");
	  else
            b->Connect("Clicked()", "TextEntryWindow", this, "Copy1()");

	  b->Connect("Clicked()", "TGMainFrame", fMain, "UnmapWindow()");

	}

      if(charge > 0)
	{
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			fMain->SetWindowName("(+) Particle");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			fMain->SetWindowName("(-) Particule");

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b = new TGTextButton(fMain, "          Copy to calculator          ");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b = new TGTextButton(fMain, "          Copier vers calculateur          ");
		fMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 5, 5));

		if(isBachelor)
			b->Connect("Clicked()", "TextEntryWindow", this, "Copy3()");
		else
			b->Connect("Clicked()", "TextEntryWindow", this, "Copy2()");

		b->Connect("Clicked()", "TGMainFrame", fMain, "UnmapWindow()");

	}

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
        b = new TGTextButton(fMain, "Close");
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
        b = new TGTextButton(fMain, "Fermer");
      fMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 5, 5));
      b->Connect("Clicked()", "TGMainFrame", fMain, "UnmapWindow()");

    }

    fMain->MapSubwindows();
    fMain->Resize();

    // set minimum width, height
    fMain->SetWMSizeHints(fMain->GetDefaultWidth(), fMain->GetDefaultHeight(),
			  1000, 1000, 0, 0);

    fMain->MapRaised();

  }

	//--------------------------------------------------------------
  void Copy1()
  {

    if(gAliceCalculator)
      gAliceCalculator->Set1(fEcho->GetNumber(), fAlign->GetNumber(), fAccess->GetNumber(), fBorder->GetNumber());
    else
      {

	TRootHelpDialog* startMessage = new TRootHelpDialog(gClient->GetRoot(), "Warning", 250, 100);

	startMessage->SetText("\
Start the Calculator first!\
");
	startMessage->Popup();

      }

    return;

  }

	//--------------------------------------------------------------
  void Copy2()
  {

    if(gAliceCalculator)
      gAliceCalculator->Set2(fEcho->GetNumber(), fAlign->GetNumber(), fAccess->GetNumber(), fBorder->GetNumber());
    else
      {

	TRootHelpDialog* startMessage = new TRootHelpDialog(gClient->GetRoot(), "Warning", 250, 100);

	startMessage->SetText("\
Start the Calculator first!\
");
	startMessage->Popup();

      }
    return;

  }

	//--------------------------------------------------------------
  void Copy3()
  {

    if(gAliceCalculator)
      gAliceCalculator->Set3(fEcho->GetNumber(), fAlign->GetNumber(), fAccess->GetNumber(), fBorder->GetNumber());
    else
      {

	TRootHelpDialog* startMessage = new TRootHelpDialog(gClient->GetRoot(), "warning", 250, 100);

	startMessage->SetText("\
Start the Calculator first!\
");
	startMessage->Popup();

      }

    return;

  }

  ClassDef(TextEntryWindow, 0);

};

//------------------------------------------------------

class SigTestSpitter
{
  TEveSelection *fSel;
  TString        fPrefix;

public:
	//--------------------------------------------------------------
  SigTestSpitter(TEveSelection* sel, const TString& prefix) :
    fSel(sel), fPrefix(prefix)
  {
    fSel->Connect("SelectionAdded(TEveElement*)", "SigTestSpitter", this, "Added(TEveElement*)");
  }

  // ----------------------------------------------------------------

  void Added(TEveElement* el)
  {

    if(el->GetElementName() == TString::Format("ITS"))
      {

	new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 1);

	return;
      }

    if(el->GetElementName() == TString::Format("TPC"))
      {

	new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 2);

	return;
      }

    if(el->GetElementName() == TString::Format("TRD") || el->GetElementName() == TString::Format("TOF"))
      {

	new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 3);

	return;
      }

    if(el->GetElementName() == TString::Format("ESD_Track"))
      {

	TEveTrack *track = (dynamic_cast<TEveTrack*>(el));

	new TextEntryWindow(track->GetMomentum().fX, track->GetMomentum().fY, track->GetMomentum().fZ, 0.13957, track->GetCharge(), kFALSE, kFALSE);

	return;
      }

    for(Int_t n=0; n<nV0Tracks; n++)
      {

	if(el->GetElementName() == TString::Format("V0_Track_Neg_%d", n))
	  {
            new TextEntryWindow(pxNeg[n], pyNeg[n], pzNeg[n], massNeg[n], -1, kFALSE, kFALSE);

            return;
	  }


	if(el->GetElementName() == TString::Format("V0_Track_Pos_%d", n))
	  {
            new TextEntryWindow(pxPos[n], pyPos[n], pzPos[n], massPos[n], 1, kFALSE, kFALSE);

            return;
	  }

	if(el->GetElementName() == TString::Format("V0_Pointing_Line_%d", n))
	  {
            new TextEntryWindow(pxNeg[n], pyNeg[n], pzNeg[n], massNeg[n], -1, kFALSE, kFALSE);

            new TextEntryWindow(pxPos[n], pyPos[n], pzPos[n], massPos[n], 1, kFALSE, kFALSE);

            TGLViewer *glv = (dynamic_cast<TEveViewer*>(gEve->GetViewers()->FindChild("RPhi View")))->GetGLViewer();
            glv->CurrentCamera().Reset();
            glv->CurrentCamera().Truck(pcaX[n]-39.8,pcaY[n]+44.0);
            glv->CurrentCamera().Dolly(200000, kTRUE, kTRUE);
            glv->DoDraw();

            glv = (dynamic_cast<TEveViewer*>(gEve->GetViewers()->FindChild("RhoZ View")))->GetGLViewer();
            glv->CurrentCamera().Reset();
            glv->CurrentCamera().Truck(pcaZ[n], TMath::Sqrt(pcaX[n]*pcaX[n]+pcaY[n]*pcaY[n])+10.0);
            glv->CurrentCamera().Dolly(200000, kTRUE, kTRUE);
            glv->DoDraw();

            return;

	  }

      }

    for(Int_t n=0; n<nCascadeTracks; n++)
      {

	if(el->GetElementName() == TString::Format("Cascade_Track_Neg_%d", n))
	  {
            new TextEntryWindow(pxNegCas[n], pyNegCas[n], pzNegCas[n], massNegCas[n], -1, kFALSE, kFALSE);

            return;
	  }


	if(el->GetElementName() == TString::Format("Cascade_Track_Pos_%d", n))
	  {
            new TextEntryWindow(pxPosCas[n], pyPosCas[n], pzPosCas[n], massPosCas[n], 1, kFALSE, kFALSE);

            return;
	  }

	if(el->GetElementName() == TString::Format("Cascade_Track_%d", n))
	  {
            new TextEntryWindow(pxNegCas[n], pyNegCas[n], pzNegCas[n], massNegCas[n], -1, kFALSE, kFALSE);

            new TextEntryWindow(pxPosCas[n], pyPosCas[n], pzPosCas[n], massPosCas[n], 1, kFALSE, kFALSE);

            return;
	  }

	if(el->GetElementName() == TString::Format("Cascade_Line_1_%d", n))
	  {
            new TextEntryWindow(pxBacCas[n], pyBacCas[n], pzBacCas[n], massBacCas[n], chargeBacCas[n], kFALSE, kFALSE);

            TGLViewer *glv = (dynamic_cast<TEveViewer*>(gEve->GetViewers()->FindChild("RPhi View")))->GetGLViewer();
            glv->CurrentCamera().Reset();
            glv->CurrentCamera().Truck(pcaXBacCas[n]-39.8,pcaYBacCas[n]+44.0);
            glv->CurrentCamera().Dolly(200000, kTRUE, kTRUE);
            glv->DoDraw();

            glv = (dynamic_cast<TEveViewer*>(gEve->GetViewers()->FindChild("RhoZ View")))->GetGLViewer();
            glv->CurrentCamera().Reset();
            glv->CurrentCamera().Truck(pcaZBacCas[n], TMath::Sqrt(pcaXBacCas[n]*pcaXBacCas[n]+pcaYBacCas[n]*pcaYBacCas[n])+10.0);
            glv->CurrentCamera().Dolly(200000, kTRUE, kTRUE);
            glv->DoDraw();

            return;
	  }

	if(el->GetElementName() == TString::Format("Cascade_Line_2_%d", n))
	  {
            new TextEntryWindow(pxNegCas[n], pyNegCas[n], pzNegCas[n], massNegCas[n], -1, kFALSE, kFALSE);

            new TextEntryWindow(pxPosCas[n], pyPosCas[n], pzPosCas[n], massPosCas[n], 1, kFALSE, kFALSE);

            TGLViewer *glv = (dynamic_cast<TEveViewer*>(gEve->GetViewers()->FindChild("RPhi View")))->GetGLViewer();
            glv->CurrentCamera().Reset();
            glv->CurrentCamera().Truck(pcaXCas[n]-39.8,pcaYCas[n]+44.0);
            glv->CurrentCamera().Dolly(200000, kTRUE, kTRUE);
            glv->DoDraw();

            glv = (dynamic_cast<TEveViewer*>(gEve->GetViewers()->FindChild("RhoZ View")))->GetGLViewer();
            glv->CurrentCamera().Reset();
            glv->CurrentCamera().Truck(pcaZCas[n], TMath::Sqrt(pcaXCas[n]*pcaXCas[n]+pcaYCas[n]*pcaYCas[n])+10.0);
            glv->CurrentCamera().Dolly(200000, kTRUE, kTRUE);
            glv->DoDraw();

            return;
	  }

	if(el->GetElementName() == TString::Format("Cascade_Bachelor_Track_%d", n))
	  {

            new TextEntryWindow(pxBacCas[n], pyBacCas[n], pzBacCas[n], massBacCas[n], chargeBacCas[n], kFALSE, kTRUE);

            return;

	  }

      }

  }

};

SigTestSpitter* gSigTestSpitter = 0;

//---------------------------------------------------------------------------


class StrangeStatisticsPopup : public TGMainFrame
{

protected:

public:

	//--------------------------------------------------------------
  StrangeStatisticsPopup(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

	TGGroupFrame* gf = nullptr; 
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		gf = new TGGroupFrame(this, "Strange Particle Statistics");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		gf = new TGGroupFrame(this, "Statistique Particules Etranges");
	new TGGroupFrame(this, "Strange Particle Statistics");

	TGLabel *label = 0;

    Int_t nLoads = 1;

    if(gLoads)
      nLoads = gLoads;

    TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    label = new TGLabel(hf, "Particle");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    label = new TGLabel(hf, "Particule");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    label = new TGLabel(hf, "MC Data");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    label = new TGLabel(hf, "Donnees MC");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    label = new TGLabel(hf, "Real Data");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    label = new TGLabel(hf, "Donnees Reelles");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Kaons");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%4.1f",13.4*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%d", countKaons));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Lambdas");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%3.1f",3.9*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%d", countLambdas));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "antiLambdas");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%3.1f",3.5*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%d", countALambdas));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Xis");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%3.1f",5.5*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%d", countXis));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    TGHorizontal3DLine* separator = new TGHorizontal3DLine(gf);
    gf->AddFrame(separator, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

//-------------------------------------------------------------------------------------
/*
     label = new TGLabel(hf, "Particle Ratio");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, "MC Data");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, "Real Data");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Kaons/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.094"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countKaons/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Lambdas/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.026"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countLambdas/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "antiLambdas/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.024"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countALambdas/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Xis/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.037"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%6.4f", 1.0*countXis/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);
*/
//-------------------------------------------------------------------------------------
/*
    label = new TGLabel(hf, "DO THEY AGREE?");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("YES!"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("NO!"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    gf->AddFrame(hf);


	TGTextButton *b = nullptr; 
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		b = new TGTextButton(gf, "Close");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		b = new TGTextButton(gf, "Fermer");
	b->Connect("Clicked()", "StrangeStatisticsPopup", this, "UnmapWindow()");
    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    // Set a name to the main frame      
    SetWindowName("Strange Particle Statistics");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();

  }

  ClassDef(StrangeStatisticsPopup, 0);

};

StrangeStatisticsPopup* gStrangeStatisticsPopup = 0;

//---------------------------------------------------------------------------


class ParticleRatioPopup : public TGMainFrame
{

protected:

public:

	//--------------------------------------------------------------
  ParticleRatioPopup(const TGWindow *p, UInt_t w, UInt_t h) :
    TGMainFrame(p, w, h)
  {

    DontCallClose();

  //  TGGroupFrame* gf = new TGGroupFrame(this, "Particle Ratios");
	TGGroupFrame* gf = new TGGroupFrame(this, "Strange Particle Statistics");

    TGLabel *label = 0;

    Int_t nPions = 148;

    Int_t nLoads = 1;

    if(gLoads)
      nLoads = gLoads;

    TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);
/*
    label = new TGLabel(hf, "Particle Ratio");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, "MC Data");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, "Real Data");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

//	label = new TGLabel(hf, "????");
//    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Kaons/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.094"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countKaons/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

//	label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countKaons/(nPions*fCount)));
//    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Lambdas/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.026"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countLambdas/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

//	label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countKaons/(nPions*fCount)));
//    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "antiLambdas/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.024"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countALambdas/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

//	label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countKaons/(nPions*fCount)));
//    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Xis/Pions(+)");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("0.037"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("%6.4f", 1.0*countXis/(nPions*nLoads)));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

//	label = new TGLabel(hf, TString::Format("%5.3f", 1.0*countKaons/(nPions*fCount)));
//    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    gf->AddFrame(hf);

    TGHorizontal3DLine* separator = new TGHorizontal3DLine(gf);
    gf->AddFrame(separator, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);
*/
//------------Kasia

    label = new TGLabel(hf, "Particle");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
    label = new TGLabel(hf, "MC Data");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    label = new TGLabel(hf, "Real Data");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
  label = new TGLabel(hf, "Event Analysed");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Kaons");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
    label = new TGLabel(hf, TString::Format("%4.1f",13.4*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    label = new TGLabel(hf, TString::Format("%d", countKaons));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
label = new TGLabel(hf, TString::Format("%4.3f",13.4*nLoads*countKaons/100));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));	
*/
    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Lambdas");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
    label = new TGLabel(hf, TString::Format("%3.1f",3.9*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    label = new TGLabel(hf, TString::Format("%d", countLambdas));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
label = new TGLabel(hf, TString::Format("%3.3f",3.9*nLoads*countLambdas/100));
   hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));	
*/
    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "antiLambdas");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
    label = new TGLabel(hf, TString::Format("%3.1f",3.5*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    label = new TGLabel(hf, TString::Format("%d", countALambdas));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
label = new TGLabel(hf, TString::Format("%3.3f",3.5*nLoads*countALambdas/100));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));	
*/
    gf->AddFrame(hf);


    hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

    label = new TGLabel(hf, "Xis");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
    label = new TGLabel(hf, TString::Format("%3.1f",5.5*nLoads));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    label = new TGLabel(hf, TString::Format("%d", countXis));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
/*
label = new TGLabel(hf, TString::Format("%3.3f",5.5*nLoads*countXis/100));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));	
*/
    gf->AddFrame(hf);

hf = new TGHorizontalFrame(gf, 450, 20, kFixedWidth);

/*
    label = new TGLabel(hf, "DO THEY AGREE?");
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("YES!"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    label = new TGLabel(hf, TString::Format("NO!"));
    hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
*/
    gf->AddFrame(hf);

	TGTextButton *b = nullptr; 

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		b = new TGTextButton(gf, "Close");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		b = new TGTextButton(gf, "Fermer");
	b->Connect("Clicked()", "StrangeStatisticsPopup", this, "UnmapWindow()");
    gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    // Set a name to the main frame      
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
	    SetWindowName("Strange Particle Statistics");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    SetWindowName("Statistique des Particules Etranges");
    MapSubwindows();

    // Initialize the layout algorithm via Resize()
    Resize(GetDefaultSize());

    // Map main frame
    MapWindow();

  }

  ClassDef(ParticleRatioPopup, 0);

};

ParticleRatioPopup* gParticleRatioPopup = 0;


//---------------------------------------------------------------------------

class TVSDReader
{
public:
  // ----------------------------------------------------------
  // File / Event Data
  // ----------------------------------------------------------

  TFile      *fFile;
  TDirectory *fDirectory;

  TObjArray  *fEvDirKeys;

  TEveVSD    *fVSD;

  Int_t       fMaxEv, fCurEv, fMaxR, fRAnim;

  Bool_t      fVertexLoaded, fITSClustersLoaded, fTPCClustersLoaded, fTRDClustersLoaded, fTOFClustersLoaded,  fClustersLoaded, fEsdTracksLoaded, fV0sLoaded, fCascadesLoaded, fGuidesLoaded, fGeometrySet, fInDemoMode;

  // ----------------------------------------------------------
  // Event visualization structures
  // ----------------------------------------------------------

  TEveTrackList *fTrackList;
  TEveTrackList *fTrackListV0Neg;
  TEveTrackList *fTrackListV0Pos;
  TEveTrackList *fTrackListV0Mother;
  TEveTrackList *fTrackListCascade;
  TEveTrackList *fTrackListCascadeNeg;
  TEveTrackList *fTrackListCascadePos;
  TEveTrackList *fTrackListCascadeBachelor;
  TEveTrackList *fTrackListCascadeMother;
  TEveTrackList *fTrackListPointingV0;

  TEvePointSet  *fPsVertex;
  TEvePointSet  *fITSClusters;
  TEvePointSet  *fTPCClusters;
  TEvePointSet  *fTRDClusters;
  TEvePointSet  *fTOFClusters;
  TEvePointSet  *fPsV0;
  TEvePointSet  *fPsCascade;

  TTimer        *fTimer;

  TGLOverlayButton*	obutton1;
  TGLOverlayButton*	obutton2;
  TGLOverlayButton*	obutton3;
  TGLOverlayButton*	overlayButton;

public:
  TVSDReader(const char* file_name) :
    fFile(0), fDirectory(0), fEvDirKeys(0), 
    fVSD(0),

    fMaxEv(-1), fCurEv(-1), fMaxR(400), fRAnim(0), 

    fVertexLoaded(kFALSE), fITSClustersLoaded(kFALSE), fTPCClustersLoaded(kFALSE), fTRDClustersLoaded(kFALSE),
    fTOFClustersLoaded(kFALSE), fClustersLoaded(kFALSE), fEsdTracksLoaded(kFALSE), fV0sLoaded(kFALSE), fCascadesLoaded(kFALSE), fGuidesLoaded(kFALSE),
    fGeometrySet(kTRUE), fInDemoMode(kFALSE),

    fTrackList(0), fTrackListV0Neg(0), fTrackListV0Pos(0), fTrackListV0Mother(0),
    fTrackListCascade(0), fTrackListCascadeNeg(0), fTrackListCascadePos(0), fTrackListCascadeBachelor(0), fTrackListCascadeMother(0),
    fTrackListPointingV0(0),
    fPsVertex(0), fITSClusters(0), fTPCClusters(0), fTRDClusters(0), fTOFClusters(0),
    fPsV0(0), fPsCascade(0), fTimer(0),
    obutton1(0), obutton2(0), obutton3(0), overlayButton(0)

  {
    fFile = TFile::Open(file_name);
    if (!fFile)
      {
	Error("VSD_Reader", "Can not open file '%s' ... terminating.", file_name);
	gSystem->Exit(1);
      }

    fEvDirKeys = new TObjArray;
    TPMERegexp name_re("Event\\d+");
    TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
      {
	if (name_re.Match(lnk->GetObject()->GetName()))
	  {
            fEvDirKeys->Add(lnk->GetObject());
	  }
	lnk = lnk->Next();
      }

    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
      {
	Error("VSD_Reader", "No events to show ... terminating.");
	gSystem->Exit(1);
      }

    fVSD = new TEveVSD;

    if(gMultiView)
      {
	obutton1 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "use ROLLER to zoom in/out", 0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-20, 250, 20);
	obutton2 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "click LEFT button to rotate", 0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-40, 250, 20);
	obutton3 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "press ROLLER to move", 0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-60, 250, 20);
      }
    else
      {
	obutton1 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "use ROLLER to zoom in/out", 0, 0, 250, 20);
	obutton2 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "click LEFT button to rotate", 0, 0, 250, 20);
	obutton3 = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "press ROLLER to move", 0, 0, 250, 20);
      }

    obutton1->SetAlphaValues(0.05, 0.8);
    obutton2->SetAlphaValues(0.05, 0.8);
    obutton3->SetAlphaValues(0.05, 0.8);

  }

	//--------------------------------------------------------------
  virtual ~TVSDReader()
  {
    // Destructor.

    DropEvent();

    delete fVSD;
    delete fEvDirKeys;

    fFile->Close();
    delete fFile;
  }

	//--------------------------------------------------------------
  void ToTeacher(){

    globalChoice = 2;
    globalMode = 3;

    fInDemoMode = kFALSE;

    fVertexLoaded = kFALSE;
    fITSClustersLoaded = kFALSE;
    fTPCClustersLoaded = kFALSE;
    fTRDClustersLoaded = kFALSE;
    fTOFClustersLoaded = kFALSE;
    fClustersLoaded = kFALSE;
    fEsdTracksLoaded = kFALSE;
    fV0sLoaded = kFALSE;
    fCascadesLoaded = kFALSE;
    fGuidesLoaded = kFALSE;
    fGeometrySet = kTRUE;

    TeacherSet();

  }

	//--------------------------------------------------------------
  void ToDemo(){

    DropEvent();

    fInDemoMode = kTRUE;

    globalChoice = 1;
    globalMode = 1;

    delete fVSD;
    delete fEvDirKeys;

    fFile->Close();
    delete fFile;
	
	TString fileName("AliVSD_example.root"); 
	fileName.Prepend(gSystem->Getenv("EVTPATH")); 

    fFile = TFile::Open(fileName);
    if (!fFile)
      {
	Error("VSD_Reader", "Can not open the demo file ... terminating.");
	gSystem->Exit(1);
      }

    fEvDirKeys = new TObjArray;
    TPMERegexp name_re("Event\\d+");
    TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
      {
	if (name_re.Match(lnk->GetObject()->GetName()))
	  {
            fEvDirKeys->Add(lnk->GetObject());
	  }
	lnk = lnk->Next();
      }

    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
      {
	Error("VSD_Reader", "No events to show ... terminating.");
	gSystem->Exit(1);
      }


    fVertexLoaded = kFALSE;
    fITSClustersLoaded = kFALSE;
    fTPCClustersLoaded = kFALSE;
    fTRDClustersLoaded = kFALSE;
    fTOFClustersLoaded = kFALSE;
    fClustersLoaded = kFALSE;
    fEsdTracksLoaded = kTRUE;
    fV0sLoaded = kFALSE;
    fCascadesLoaded = kFALSE;
    fGuidesLoaded = kFALSE;
    fGeometrySet = kTRUE;


    fVSD = new TEveVSD;
    fTimer = new TTimer();
    fTimer->Connect("Timeout()", "TVSDReader", this, "AnimateEsdTracks()");

    StudentSet();

    //      GotoEvent(0);

    gEve->FullRedraw3D();
      
  }

	//--------------------------------------------------------------
  void ToStudent(){


    DropEvent();

    fInDemoMode = kFALSE;

    globalChoice = 2;
    globalMode = 2;

    delete fVSD;
    delete fEvDirKeys;

    fFile->Close();
    delete fFile;

	TString fileName(Form("AliVSD_MasterClass_%i.root", globalDataset)); 
	fileName.Prepend(gSystem->Getenv("EVTPATH")); 

    fFile = TFile::Open(fileName);
/*    switch(globalDataset)
    {

    case 1:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_1.root");
      break;

    case 2:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_2.root");
      break;

    case 3:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_3.root");
      break;

    case 4:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_4.root");
      break;

    case 5:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_5.root");
      break;

    case 6:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_6.root");
      break;

    case 7:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_7.root");
      break;

    case 8:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_8.root");
      break;

    case 9:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_9.root");
      break;

    case 10:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_10.root");
      break;

    case 11:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_11.root");
      break;

    case 12:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_12.root");
      break;

    case 13:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_13.root");
      break;

    case 14:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_14.root");
      break;

    case 15:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_15.root");
      break;

    case 16:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_16.root");
      break;

    case 17:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_17.root");
      break;

    case 18:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_18.root");
      break;

    default:
      fFile = TFile::Open("eve_files/data/AliVSD_MasterClass_1.root");
      break;

    }
*/
//    fFile = TFile::Open("AliVSD.root");

    if (!fFile)
      {
	Error("VSD_Reader", "Can not open the demo file ... terminating.");
	gSystem->Exit(1);
      }

    fEvDirKeys = new TObjArray;
    TPMERegexp name_re("Event\\d+");
    TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
      {
	if (name_re.Match(lnk->GetObject()->GetName()))
	  {
            fEvDirKeys->Add(lnk->GetObject());
	  }
	lnk = lnk->Next();
      }

    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
      {
	Error("VSD_Reader", "No events to show ... terminating.");
	gSystem->Exit(1);
      }


    fVertexLoaded = kFALSE;
    fITSClustersLoaded = kFALSE;
    fTPCClustersLoaded = kFALSE;
    fTRDClustersLoaded = kFALSE;
    fTOFClustersLoaded = kFALSE;
    fClustersLoaded = kFALSE;
    fEsdTracksLoaded = kTRUE;
    fV0sLoaded = kFALSE;
    fCascadesLoaded = kFALSE;
    fGuidesLoaded = kFALSE;
    fGeometrySet = kTRUE;


    fVSD = new TEveVSD;
    fTimer = new TTimer();
    fTimer->Connect("Timeout()", "TVSDReader", this, "AnimateEsdTracks()");


    StudentSet();

    //      GotoEvent(0);

    gEve->FullRedraw3D();
      
  }

	//--------------------------------------------------------------
  void Exit(){

    gEve->GetBrowser()->UnmapWindow();
    gSystem->CleanCompiledMacros();

  }

	//--------------------------------------------------------------
  void Calculator(){

    gAliceCalculator = new AliceCalculator(gClient->GetRoot(), 400, 200);

  }

	//--------------------------------------------------------------
  void DetectorInfo(){

    detector = 0;

    gAliceDetectorInfo = new AliceDetectorInfo(gClient->GetRoot(), 100, 100);

  }

	//--------------------------------------------------------------
  void PatternInfo(){

    detector = 0;

    gAlicePatternInfo = new AlicePatternInfo(gClient->GetRoot(),100,100);

  }

	//--------------------------------------------------------------
  void PatternInfo2(){

    detector = 0;

    gAlicePatternInfo2 = new AlicePatternInfo2(gClient->GetRoot(),100,100);

  }

	//--------------------------------------------------------------
  void CountStrangeParticles(){

    gStrangeStatisticsPopup = new StrangeStatisticsPopup(gClient->GetRoot(), 100, 100);

  }

	//--------------------------------------------------------------
  void CountStrangeParticles2(){

    gParticleRatioPopup = new ParticleRatioPopup(gClient->GetRoot(), 100, 100);

  }

	//--------------------------------------------------------------
  void InstructionsLevel1(){

	  TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "ANALYSIS INSTRUCTIONS", 700, 400);

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  instructions->SetText("\
Analysis aim:\n\n\
\
Strange particles are rare and they are not produced in every event\n\
You are given 100 events; go through each one carefully and count\n\
how many strange particles of each species you can find (how many Kaons,\n\
Lambdas, antiLambdas, Xis) and compare them to MC data.\n\n\
\
How to analyse an event:\n\n\
\
   Click on <Calculator>\n\n\
\
   1. Use the following buttons to display objects of the ALICE data.\n\
\
   1.1 Click on <V0> and <Cascades> in <Strange Particles>\n\
   1.2 (Option) Click on <Vertex>, <Clusters> and <Tracks> in <Display>\n\
   The tracks from strange particle decays have been coloured in red, green and blue,\n\
   while tracks of other particles (mostly pions) have been coloured grey.\n\n\
\
   2. Look carefully at the event display. Do you recognize any interesting pattern\n\
      that looks like the decay of V0s or Cascades?\n\
      If it is difficult for you to spot the strange particle tracks, take away the others.\n\
   2.1 If you recognize a V0 decay pattern, click on each one of the two tracks\n\
       that were produced from the decay. Then you get a window with the momentum,\n\
       mass and charge of this particle.\n\
   2.2 If you recognize a Cascade decay pattern, click on each one of the three tracks\n\
       that were produced from the decay. Then you get a window with the momentum,\n\
       mass and charge of this particle.\n\n\
\
   3. Click <Copy to Calculator>. The momentum and mass of each particle is copied\n\
      to the Calculator.\n\n\
\
   Now, check the masses of the particles. What are they?\n\
   Do you remember it from the lecture? If not, you will find them at the top\n\
   of the Calculator panel in the <Particle Table>.\n\n\
\
   Then, click <Invariant Mass>. Then, the invariant mass\n\
   of the mother particle is calculated. Does the invariant mass corresponds to any value\n\
   in the particle table? If so, you have found a strange particle! (If not, it's background).\n\n\
\
   Confirm it by clicking <That's XXX>, where XXX is the name of the particle you have found.\n\n\
\
   When you find a particle, click on tab over the event display <Invariant mass>.\n\
   To update your results click on the historgams\n\n\
\
   4. When you finish with one event, click <Event analysed!> to count analysed events.\n\
   Then go to the next one by clicking the right arrow in <Event Navigation> section.\n\n\
\
   5. When you finish all the events, click <Table of Results>. There are two sets of strange\n\
      particle statistics there, one as predicted by theoretical models and one with your\n\
      analysis results. Do they agree? Then the theory descibes reality, and the\n\
      'standard physics' processes mechanism described in MC is verified.\n\n\
\
      REMEMBER:\n\
      Strange particles are rare. If we had given you the events we analyse (real + MC),\n\
      you should look at 100 to find one Lambda and 1000 to find one Xi. To make it easier for you\n\
      we threw away  some of the events without strange particles in the controlled way.\n\
      To get the final numbers you have to multiply your results with some factors:\n\
      (a) Kaon x 100\n\
      (b) Lambda x 100\n\
      (c) antiLambda x 100\n\
      (d) Xi x 1000\n\n\
\
   6. Click <Save> in the calculator.\n\n\
\
   7. Pass the file with the results to your teacher: Copy the file with your results\n\
      to the memory stick\n\n\
\
   ");

	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		instructions->SetText("\
But de l'analyse :\n\n\
\
La production de particules etranges est rare et elles ne sont pas produites dans chaque evenement\n\
Vous avez 100 evenemets ; examinez chacun d'eux et comptez\n\
combien de particules etranges de chaque espece vous pouvez trouver (combien de Kaons,\n\
Lambdas, antiLambdas, Xis) et comparez vos resultats aux donnes MC.\n\n\
\
Comment analyser un evenement :\n\n\
\
   Cliquez <Calculateur>\n\n\
\
   1. Utilisez les boutons suivants pour afficher les objets des donnees ALICE.\n\
\
     1.1 cliquez <V0> et <Cascades> dans la rubique <Pricules Etranges>\n\
     1.2 (Option) Cliquez <Vertex>, >Clusters> et <Traces> dans la rubique <Visualisation>\n\
     les traces des particules de decroissance des particules sont marquees en rouge, vert et bleu,\n\
     et les traves des autres particules (principalement des pions) sont marquees en gris.\n\n\
\
   2. Regardez attentivement la visualisation de l'evenement. Reconnaissez-vous un motif interessant\n\
      qui ressemble à une decroissance V0 ou cascade ?\n\
      Si vous avez des difficultes pour identifier les traves des particules etranges, supprimer l'affichage des autres particules.\n\
     2.1 Si vous avez trouve une topologie V0, cliquant chacune des deux traces\n\
         resultant de la decroissance. Vous obtientrez alors une fenetre avec la quantite de mouvement,\n\
         la masse et la charge de cette particule.\n\
	 2.2 Si vous avez trouve une topologie cascade, cliquez chacune des trois traces\n\
         resultant de la decroissance. Vous obtientrez alors une fenetre avec la quantite de mouvement,\n\
         la masse et la charge de cette particule.\n\
\
   2. Cliquez <Copy to Calculator>. la quantite de mouvement et la masse de chaque  particule seront copiees\n\
      dans le Calculateur.\n\n\
\
   Maintenant, verifiez la masse des partcules. Quelles sont-elles ?\n\
   Vous le rappelez vous du cours ? Si non, vous les trouverez en haut\n\
   du panneau du Calculateur dans <Tableau des Particules>.\n\n\
\
   Cliquez ensuite <Masse Invariante>. La masse invariante\n\
   de la particule mere sera calculee. Est-ce que la masse invariante correspon à l'une\n\
   des masses dans le tableau des particules ?Si oui, vous avez trouve une particule etrange ! (Si non, c'est du bruit de fond).\n\n\
\
   Confirmez en cliquant <Ceci est un XXX>, ou XXX est le nom de la particule que vous avez trouvee.\n\n\
\
   Lorsque vous avez trouve une particule, cliquez l'onglet <Masse Invariante> au-dessus de la visualition de l'evenemet.\n\
   Pour mettre a jour vos resultats dans les histogrammes\n\n\
\
   4. Lorsque vous avez termine l'analyse de l'evenement, cliquez <Evenement analyse !> pour compter les evenements analyses.\n\
   Continuez ensuite avec l'evenement suivant en cliquant la fleche vers la droite dans la rubrique <Vavigation Evenement>.\n\n\
\
   5. Lorsque vous aurez analyse tous les evenements, cliquez <Tableau des Resultats>. Il y a deux series de statistique\n\
      de particules etranges, l'une est celle predite par les modeles theoriques et l'autre\n\
	  est le resultat de votre analyse. Est-ce que les deux sont en accord ? Si oui, la theorie decrit bien la realite et\n\
	  les mecanismes de la 'physique standart' decrite dans le MC sont valides.\n\n\
\
	  ATTENTION :\n\
      Les particules etranges sont rares. Si nous vous avions donne les evenements que nous analysons d'habitude (reels + MC),\n\
      vous auriez du examiner 100 evenements pour trouver un Lambda et 1000 pour trouver un Xi. Pour vous simplifier la tache\n\
      nous avons elimine des evenements ne contenant aucune particule etrange.\n\
      Pour obtenir les nombres finals vous devez multiplier vos resultats par les facteurs suivants :\n\
      (a) Kaon x 100\n\
      (b) Lambda x 100\n\
      (c) antiLambda x 100\n\
      (d) Xi x 1000\n\n\
\
   6. Cliquez <Sauvegarder> dans l'outil Calculateur.\n\n\
\
   7. Transmettez le fichier avec vos rsultats a votre tuteur : Copier le fichier avec vos resultats\n\
      sur la cle memoire\n\n\
\
      ");	

	  instructions->DontCallClose();
	  instructions->Popup();

  }

	//--------------------------------------------------------------
  void InstructionsLevel2(){

    TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "INSTRUCTIONS LEVEL 2", 700, 400);

  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
    instructions->SetText("\
Level 2 analysis aim:\n\n\
\
You are given 100 events, go through each one carefully and count the number\n\
of strange particles of each species (Kaons, Lambdas, antiLambdas and Xis), then\n\
calculate the ratios of strange/non-strange particles (Kaons/Pions, Lambdas/Pions,\n\
antiLambdas/Pions and Xis/Pions). Compare them to the same ratios for Monte Carlo data.\n\n\
\
\
How to analyse an event in Level 2:\n\n\
\
   Click on <Calculator>\n\n\
\
   1. Use the following buttons to display objects of the ALICE data.\n\
\
   1.1 Click on <V0> and <Cascades> in <Strange Particles>\n\
   1.2 Click on <Vertex>, <Clusters> and <Tracks> in <Display>\n\
       The tracks from strange particle decays have been coloured in red, green and magenta,\n\
       while tracks of other particles (mostly pions) have been coloured grey.\n\n\
\
   2. Look carefully at the event display. Do you recognize any interesting pattern\n\
      that looks like the decay of V0s or Cascades?\n\
      If it is difficult for you to spot the strange particle tracks, take away the others.\n\
   2.1 If you recognize a V0 decay pattern, click on each one of the two tracks\n\
       that were produced from the decay. Then you get a window with the momentum,\n\
       mass and charge of this particle.\n\
   2.2 If you recognize a Cascade decay pattern, click on each one of the three tracks\n\
       that were produced from the decay. Then you get a window with the momentum,\n\
       mass and charge of this particle.\n\n\
\
   3. Click <Copy to Calculator>. The momentum and mass of each particle is copied\n\
      to the Calculator.\n\n\
\
   Now, check the masses of the particles. What are they?\n\
   Do you remember it from the lecture? If not, you will find them at the top\n\
   of the Calculator panel in the <Particle Table>.\n\n\
\
   Then, click <Invariant Mass>. Then, the invariant mass\n\
   of the mother particle is calculated. Does the invariant mass corresponds to any value\n\
   in the particle table? If so, you have found a strange particle! (If not, it's background).\n\n\
\
   Confirm it by clicking <That's XXX>, where XXX is the name of the particle you have found.\n\n\
\
   When you find a particle, click on the tab over the event display <Invariant mass>.\n\
   To update your results click on the historgams\n\n\
\
   4. When you finish with one event, go to the next one by clicking the right arrow\n\
      in <Event Navigation> section.\n\n\
\
   5. When you finish all the events, click <Table 2>. The numers of strange particles you\n\
      have found are now divided by the number of Pions in 100 events. Then, there are two sets\n\
      of ratios strange/non-strange, one predicted by theoretical model and one from your analysis.\n\n\
\
      REMEMBER:\n\
      Strange particles are rare. If we had given you the events we analyse (real + MC),\n\
      you should look at 100 to find one Lambda and 1000 to find one Xi. To make it easier for you\n\
      we threw away  some of the events without strange particles in the controlled way.\n\
      To get the final numbers you have to multiply your results with some factors:\n\
       (a) Kaon x 100\n\
       (b) Lambda x 100\n\
       (c) antiLambda x 100\n\
       (d) Xi x 1000\n\n\
\
      Are the ratios strange/non-strange higher in real data?\n\
      Then, you have found strangeness enhancement, a hint that reality is not as 'trivial\n\
      as MC models and something interesting may happen.\n\n\
\
   6. Click <Save> in the calculator.\n\n\
\
   7. Pass the file with the results to your teacher: Copy the file with your results\n\
      to the memory stick\n\n\
\
   ");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
	    instructions->SetText("\
But de l'analyse niveau 2 :\n\n\
\
Vous avez 100 evenemets ; examinez chacun d'eux et comptez\n\
de particules etranges de chaque espece (Kaons, Lambdas, antiLambdas and Xis), ensuite\n\
calculez les rapports des particules étranges / particules non étranges (Kaons/Pions, Lambdas/Pions,\n\
antiLambdas/Pions et Xis/Pions). Comparez les rapports obtenus à ceux obtenus a partir des donnees Monte Carlo.\n\n\
\
Comment analyser un evenement :\n\n\
\
   Cliquez <Calculateur>\n\n\
\
   1. Utilisez les boutons suivants pour afficher les objets des donnees ALICE.\n\
\
     1.1 cliquez <V0> et <Cascades> dans la rubique <Pricules Etranges>\n\
     1.2 (Option) Cliquez <Vertex>, >Clusters> et <Traces> dans la rubique <Visualisation>\n\
          les traces des particules de decroissance des particules sont marquees en rouge, vert et bleu,\n\
          et les traves des autres particules (principalement des pions) sont marquees en gris.\n\n\
\
   2. Regardez attentivement la visualisation de l'evenement. Reconnaissez-vous un motif interessant\n\
      qui ressemble à une decroissance V0 ou cascade ?\n\
      Si vous avez des difficultes pour identifier les traves des particules etranges, supprimer l'affichage des autres particules.\n\
     2.1 Si vous avez trouve une topologie V0, cliquant chacune des deux traces\n\
        resultant de la decroissance. Vous obtientrez alors une fenetre avec la quantite de mouvement,\n\
        la masse et la charge de cette particule.\n\
	 2.2 Si vous avez trouve une topologie cascade, cliquez chacune des trois traces\n\
         resultant de la decroissance. Vous obtientrez alors une fenetre avec la quantite de mouvement,\n\
         la masse et la charge de cette particule.\n\
\
   2. Cliquez <Copy to Calculator>. la quantite de mouvement et la masse de chaque  particule seront copiees\n\
      dans le Calculateur.\n\n\
\
   Maintenant, verifiez la masse des partcules. Quelles sont-elles ?\n\
   Vous le rappelez vous du cours ? Si non, vous les trouverez en haut\n\
   du panneau du Calculateur dans <Tableau des Particules>.\n\n\
\
   Cliquez ensuite <Masse Invariante>. La masse invariante\n\
   de la particule mere sera calculee. Est-ce que la masse invariante correspon à l'une\n\
   des masses dans le tableau des particules ?Si oui, vous avez trouve une particule etrange ! (Si non, c'est du bruit de fond).\n\n\
\
   Confirmez en cliquant <Ceci est un XXX>, ou XXX est le nom de la particule que vous avez trouvee.\n\n\
\
   Lorsque vous avez trouve une particule, cliquez l'onglet <Masse Invariante> au-dessus de la visualition de l'evenemet.\n\
   Pour mettre a jour vos resultats dans les histogrammes\n\n\
\
   4. Lorsque vous avez termine l'analyse de l'evenement, cliquez <Evenement analyse !> pour compter les evenements analyses.\n\
   Continuez ensuite avec l'evenement suivant en cliquant la fleche vers la droite dans la rubrique <Vavigation Evenement>.\n\n\
\
   5. Lorsque vous aurez analyse tous les evenements, cliquez <Tableau 2>. Les nombres de particules étranges que vous\n\
	  avet trouvees sont maintenenat divises par le nombre de Pions dans 100 evenements. Ensuite, il y a deux series\n\
      de rapports etrange/non-etrange, l'une predite par les modeles theoriques et l'autre de votre analyse.\n\n\
\
    ATTENTION :\n\
	Les particules etranges sont rares. Si nous vous avions donne les evenements que nous analysons d'habitude (reels + MC),\n\
	vous auriez du examiner 100 evenements pour trouver un Lambda et 1000 pour trouver un Xi. Pour vous simplifier la tache\n\
	nous avons elimine des evenements ne contenant aucune particule etrange.\n\
    Pour obtenir les nombres finals vous devez multiplier vos resultats par les facteurs suivants :\n\
     (a) Kaon x 100\n\
     (b) Lambda x 100\n\
	 (c) antiLambda x 100\n\
	 (d) Xi x 1000\n\n\
\
   Est-ce que les rapports les rapports etrange/non-etrange sont plus eleves dans les donnees reelles ?\n\
   Si oui, alors vous avez decouvert une augmentation de l'etrangete, une indication que la realite n'est pas aussi evidente\n\
   que le predise les modeles MC et que quelquechose d'interessant peut avoir lieu.\n\n\
\
   6. Cliquez <Sauvegarder> dans la section calculateur.\n\n\
\
   7. Donnez le fichier avec les resultats a votre tuteur : Copier le fichier avec vos resultats\n\
      sur la cle memoire\n\n\
\
		   ");
  
    instructions->DontCallClose();
    instructions->Popup();

  }

	//--------------------------------------------------------------
  void Instructions(){

    if(globalMode == 1 && fInDemoMode)
      {
	TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "DEMO MODE INSTRUCTIONS", 620, 400);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		instructions->SetText("\
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
   information)\n\n\
\
In the bottom, there are main application control buttons:\n\
   I'm ready! Start Exercise - starts the application in the student mode\n\
   Exit\n\n\
\
   ");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		instructions->SetText("\
Bienvenu.e dans le mode <Demo>, vous y trouverez les outils necessaires a l'analyse des\n\
evenements ALICE. Dans ce mode il n'y aura que 4 evenements pour l'analyse et chacun\n\
d'eux contient une particule etrange. Le nom de la particule est affiche dans le\n\
bas de la vue 3D se trouvant a droite\n\n\
\
Sur la droite sont affiches les evenements vus de trois facons differentes.\n\
Sur la gauche se trouve votre poste de pilotage divise en plusieurs parties :\n\
\
Navigation Evenements :\n\
   Cliquez les fleches pour selectioner l'evenement precedent ou suivant.\n\
   Le numero de l'evenement courant et le nombre d'evenements analyses sont egalement affiches.\n\n\
\
Particules Etranges - ceci vous permet d'afficher ou non les traces issues de la decroissance \n\
de particules etranges :\n\
   V0s      - traces produites a partir d'une decroissance V0 qu'il vous est demande de rechercher\n\
   Cascades - traces d'une decroissance cascade qu'il vous est demande de rechercher\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
Calculateur:\n\
   Calculateur           - vous permet de calculer les proprietes des particules etranges telles que\n\
   la masse invariante a partir des caracteristiques des particules filles\n\
   Tableau des Resultats - tableau resumant la statistique des particules etranges \n\
\n\
\
Visualisation - vous permet de visualiser ou non des elements tels que :\n\
   Vertex     - le point ou s'est produit la collision\n\
   Grappes    - les points de passage des particules dans les detecteurs\n\
   Traces     - la trajectoire reconstruite des particules traversant les detecteurs\n\
   Geometrie  - pour afficher les detecteurs ALICE\n\
   Fond       - pour choisir la couleur d'arriere plan du panneau de visualisation\n\
   Axes       - systeme de reference de l'experience ALICE\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
Encyclopedie :\n\
   Detecteur ALICE              - description de ALICE et de ses principaux composants;\n\
   Topologie de decroissance V0 - topologie de decroissance des particules a rechercher;\n\
   (NOTE: cliquer les images des detecteurs/decroissances pour obtenir \n\
	d'avantage d'informations)\n\n\
\
Dans la partie basse, vous trouverez les boutons de controle principaux de l'application :\n\
    Mode Etudiant - demarre l'application en mode <Etudiant>,\n\
    Sortie\n\n\
\
   ");
    instructions->DontCallClose();
	instructions->Popup();

      }

    if(globalMode == 2)
      {
	TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "STUDENT MODE INSTRUCTIONS", 700, 400);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		instructions->SetText("\
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
   information)\n\n\
\
In the bottom, there are main application control buttons:\n\
   Back to Demo - starts the application in the Demo mode,\n\
   Exit\n\n\
\
   ");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		instructions->SetText("\
Bienvenu.e dans le mode <Etudiant>, vous y trouverez les outils necessaires a l'analyse des\n\
evenements ALICE.\n\n\
\
Sur la droite sont affiches les evenements vus de trois facons differentes.\n\
Sur la gauche se trouve votre poste de pilotage divise en plusieurs parties :\n\
\
Instructions :\n\
   Instructions Globales - instructions generales pour l'utilisation de l'application\n\
   Instructions Analyse  - instructions pour l'analyse\n\
 \n\n\
\
Navigation Evenements :\n\
   Cliquez les fleches pour selectioner l'evenement precedent ou suivant.\n\
   Le numero de l'evenement courant et le nombre d'evenements analyses sont egalement affiches.\n\n\
\
Particules Etranges - ceci vous permet d'afficher ou non les traces issues de la decroissance \n\
de particules etranges :\n\
   V0s      - traces produites a partir d'une decroissance V0 qu'il vous est demande de rechercher\n\
   Cascades - traces d'une decroissance cascade qu'il vous est demande de rechercher\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
Calculateur:\n\
   Calculateur - vous permet de calculer les proprietes des particules etranges telles que\n\
   la masse invariante a partir des caracteristiques des particules filles\n\
   Tableau des Resultats - tableau resumant la statistique des particules etranges \n\
  \n\
\
Visualisation - vous permet de visualiser ou non des elements tels que :\n\
   Vertex     - le point ou s'est produit la collision\n\
   Grappes    - les points de passage des particules dans les detecteurs\n\
   Traces     - la trajectoire reconstruite des particules traversant les detecteurs\n\
   Geometrie  - pour afficher les detecteurs ALICE\n\
   Fond       - pour choisir la couleur d'arriere plan du panneau de visualisation\n\
   Axes       - systeme de reference de l'experience ALICE\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
 Encyclopedie :\n\
	Detecteur ALICE - description de ALICE et de ses principaux composants;\n\
	Topologie de decroissance V0 - topologie de decroissance des particules a rechercher;\n\
	(NOTE: cliquer les images des detecteurs/decroissances pour obtenir \n\
	d'avantage d'informations)\n\n\
\
Dans la partie basse, vous trouverez les boutons de controle principaux de l'application :\n\
    Mode Etudiant - demarre l'application en mode <Etudiant>,\n\
	Sortie\n\n\
\
   ");
	
	instructions->DontCallClose();
	instructions->Popup();

      }

    if(globalMode == 3)
      {

	TRootHelpDialog* instructions = new TRootHelpDialog(gClient->GetRoot(), "TEACHER MODE INSTRUCTIONS", 700, 600);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		instructions->SetText("\
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
");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		instructions->SetText("\
Bienvenu.e dans le mode <Tuteur>, vous y trouverez les outils pour fusionner les résultats et faire l'analyse\n\
de vos résultats obtenus en mode <Etudiant>\n\n\
\
Dans le panneau de gauche, vous voyez quatre autres panneaux appelés <Instructions Tuteurs>,\n\
<Controles Tuteur>, <Resultats> and <Encyclopedie> :\n\n\
\
   Instructions - decrit comment utiliser l'application en mode <Tuteur> mode\n\
   Controles Tuteur - vous permet de fusionner et analyser les resultats obtenus en mode <Etudiant>.\n\
      Obtenir les Fichiers - vous permet de recuperer les fichiers des resultats obtenus en mode <Etudiant>\n\
      Enregistrer les Histogrammes - enregistre les histogrammes montrant les resultats obtenus en mode <Etudiant>\n\
   Resultats\n\
      Tableau des Resultats - affiche et interprete les resultats de l'analyse\n\
    \n\
Encyclopedie :\n\
   Detecteur ALICE - description de ALICE et de ses principaux composants;\n\
   Topologie de decroissance V0 - topologie de decroissance des particules a rechercher;\n\
   (NOTE: cliquer les images des detecteurs/decroissances pour obtenir \n\
   d'avantage d'informations)\n\n\
\
Animation de l'Evenement :\n\
   Play/Stop - Observer la propagation des particules traversant les detecteurs\n\n\
\
\
Dans la partie basse, vous trouverez les boutons de controle principaux de l'application :\n\
   Mode Etudiant - demarre l'application en mode <Etudiant>,\n\
   Sortie\n\n\
\
Comment fusionner et analyser les resultats obtenus en mode <Etudiant>\n\n\
\
   1. En cliquant le bouton <Obtenir les Fichiers> une fenetre <Ouvrir> s'affiche.\n\
   2. Trouvez le resultat (type de fichier *.masterclass) et cliquez <Ouvrir>. Le resultat\n\
      will be loaded. Repeat it for all the students' files\n\
   3. Cliquez l'onglet  <Masse Invariante Fusionnee> (à droite) pour afficher\n\
      la masse invariante des particules etranges decouvertes par votre etudiant.\n\
      Ajoutez à l'histogramme en cliquant chacune d'elles.\n\
   4. Cliquez <Enregistrez les Histogrammes>, donnez un nom à l'image et <Save>\n\
   5. Cliquez <Tableau des Resultats>. Un tableau appele <Statistique des Particules Etrange> s'affichera\n\
   6. Comparez les resultats calcules avec des simulations Monte-Carlo avec les \n\
      resultats obtenus en mode <Eudiant>. Est-ce-que les resultats concordent ? Oui ? Non ? Que cela signifie-t-il ?\n\
   7. Fermer <Statistiques des Particules Etranges> en cliquant <Fermer>\n\
 \n\
   8. Cliquez <Fermer>. L'analyse est finie !\n\n\
\
");	
    instructions->DontCallClose();
	instructions->Popup();

      }


  }

	//--------------------------------------------------------------
  void ShowInvariantMass(){

    TCanvas* pad2 = new TCanvas("Invariant Mass - pp 900 GeV");

    gAllMinvHist->Draw();
    gKMinvHist->Draw("same");
    gXMinvHist->Draw("same");
    gLMinvHist->Draw("same");
    gALMinvHist->Draw("same");

    TLegend *leg1 = new TLegend(0.9,0.125,0.55,0.325);
    leg1->AddEntry(gKMinvHist,"Kaon","l");
    leg1->AddEntry(gXMinvHist,"Xi","l");
    leg1->AddEntry(gLMinvHist,"Lambda","l");
    leg1->AddEntry(gALMinvHist,"AntiLambda","l");
    leg1->Draw();

    leg1->SetX1(0.7);
    leg1->SetX2(1.0);
    leg1->SetY1(0.85);
    leg1->SetY2(1.0);

    pad2->Draw();

  }

	//--------------------------------------------------------------
  void ShowRapidity(){

    TCanvas* pad2 = new TCanvas("Rapidity - pp 900 GeV");

    pad2->Divide(2, 2);

    pad2->cd(1);

    gKRapHist->Draw();

    pad2->cd(2);

    gXRapHist->Draw();

    pad2->cd(3);

    gLRapHist->Draw();

    pad2->cd(4);

    gALRapHist->Draw();

    pad2->Draw();

  }

	//--------------------------------------------------------------
  void StudentSet(){
	  
	  TString text(""); 

	  if(globalMode == 3)
	  {

		  globalMode = 2;
		  globalChoice = 2;

	  }

	  if(globalMode == 1 && !fInDemoMode)
	  {
		  ToDemo();
		  return;
	  }

	  Instructions();

	  TEveBrowser* browser = gEve->GetBrowser();

	  if(globalChoice == 1) {
		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "ALICE MasterClass - DEMO MODE";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "MasterClass ALICE - MODE DEMO";
		  browser->GetMainFrame()->SetWindowName(text);
	  }
	  else {
		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "ALICE MasterClass - STUDENT MODE";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "MasterClass ALICE - MODE ETUDIANT";
		  browser->GetMainFrame()->SetWindowName("ALICE MasterClass - STUDENT MODE");
	  }

	  if(browser->GetTabRight()->GetTabTab(3))
		  browser->GetTabRight()->RemoveTab(3);

	  if(browser->GetTabRight()->GetTabTab(2))
		  browser->GetTabRight()->RemoveTab(2);

	  if(browser->GetTabLeft()->GetTabTab(0))
		  browser->GetTabLeft()->RemoveTab(0);

	  if(browser->GetTabBottom()->GetTabTab(0))
		  browser->GetTabBottom()->RemoveTab(0);

	  browser->StartEmbedding(TRootBrowser::kLeft);

	  TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
	  frmMain->SetWindowName("XX GUI");
	  frmMain->SetCleanup(kDeepCleanup);

	  TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
	  TGPictureButton* b1 = 0;
	  TGTextButton* b = 0;
	  TGLabel* label = 0;
	  TGGroupFrame* gf = 0;
	  TGVerticalFrame* vf = 0;
	  TGHorizontalFrame* hf = 0;
	  ULong_t buttonRedColor, buttonBlueColor;
	  
	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Student Instructions";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Instructions Etudiant";
	  gf = new TGGroupFrame(frmMain, text);

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

	  {
		  const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
		  FontStruct_t buttonFont = font->GetFontStruct();
		  gClient->GetColorByName("red", buttonRedColor);
		  b = new TGTextButton(vf,"Instructions");
		  b->SetTextColor(buttonRedColor);
		  b->SetFont(buttonFont);
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "Instructions()");

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
		  {
			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Analysis Instructions";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Instructions Analyse";
			  b = new TGTextButton(hf, text);
			  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  b->Connect("Clicked()", "TVSDReader", this, "InstructionsLevel1()");

			  // GUZIK DO LEVEL 2!!!!!!!!!!!!

			  //	b = new TGTextButton(hf,"Level 2");
			  //	hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  //	b->Connect("Clicked()", "TVSDReader", this, "InstructionsLevel2()");

		  }

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		  /*
		  if(globalChoice != 1)
		  {
		  b = new TGTextButton(vf,"Back To Demo");
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "ToDemo()");
		  }
		  */

	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);

	  //------------------------------------------------------

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Event Navigation";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Navigation Evenements";
	  gf = new TGGroupFrame(frmMain, text);

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
	  {

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
		  {
			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Previous";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Précédent";

			  label = new TGLabel(hf, text);
			  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Current";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Courant";
			  label = new TGLabel(hf, text);
			  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Next";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Suivant";

			  label = new TGLabel(hf, "Next");
			  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  }

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
		  {

			  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoBack.gif"));
			  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  b1->Connect("Clicked()", "TVSDReader", this, "PrevEvent()");

			  gEventNumber = new TGLabel(hf);
			  gEventNumber->SetText(1);
			  hf->AddFrame(gEventNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoForward.gif"));
			  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  b1->Connect("Clicked()", "TVSDReader", this, "NextEvent()");
		  }


		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));


		  //--ZLICZANIE EVENTOW  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
		  {
	
			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Event analysed!";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Evenement analysé !";
			  b = new TGTextButton(hf, text);
			  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  b->Connect("Clicked()", "TVSDReader", this, "CountEvents()");

		  }
		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
		  {

			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Event done:";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Evenements traites :";
			  label = new TGLabel(hf, text);
			  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));	

			  gEventAnalysed = new TGLabel(hf);
			  gEventAnalysed->SetText(0);
	
			  hf->AddFrame(gEventAnalysed, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  }
		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		  //---------------------
	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Strange Particles";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Particules Etranges";
	  gf = new TGGroupFrame(frmMain, text);

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

	  {

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
      
		  b = new TGTextButton(hf, "V0s");
		  b->Connect("Clicked()", "TVSDReader", this, "DisplayV0s()");
		  gCheckV0s = new TGCheckButton(hf, "", 10);
		  gCheckV0s->SetEnabled(kFALSE);

		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
		  hf->AddFrame(gCheckV0s);

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
      
		  b = new TGTextButton(hf, "Cascades");
		  b->Connect("Clicked()", "TVSDReader", this, "DisplayCascades()");
		  gCheckCascades = new TGCheckButton(hf, "", 10);
		  gCheckCascades->SetEnabled(kFALSE);

		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
		  hf->AddFrame(gCheckCascades);

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Calculator";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Calculateur";
	  gf = new TGGroupFrame(frmMain, text);

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
	  {

		  const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
		  FontStruct_t buttonFont = font->GetFontStruct();
		  gClient->GetColorByName("blue", buttonBlueColor);
		  b = new TGTextButton(vf, text);
		  b->SetTextColor(buttonBlueColor);
		  b->SetFont(buttonFont);
		  b->Connect("Clicked()", "TVSDReader", this, "Calculator()");
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);

		  {
			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  text = "Table of Results";
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  text = "Tableau des Resultats";

			  b = new TGTextButton(hf, text);
			  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  //	b->Connect("Clicked()", "TVSDReader", this, "CountStrangeParticles()");
			  b->Connect("Clicked()", "TVSDReader", this, "CountStrangeParticles2()");

			  /*
			  b = new TGTextButton(hf, "Table 2");
			  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			  b->Connect("Clicked()", "TVSDReader", this, "CountStrangeParticles2()");
			  */
		  }

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Display";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Visualisation";
	  gf = new TGGroupFrame(frmMain, text);

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
	  {

		  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);

		  b = new TGTextButton(hf,"Vertex");
		  b->Connect("Clicked()", "TVSDReader", this, "DisplayVertex()");
		  gCheckVertex = new TGCheckButton(hf, "", 10);
		  gCheckVertex->SetEnabled(kFALSE);

		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
		  hf->AddFrame(gCheckVertex);

		  //      vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "Clusters";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Grappes";
		  b = new TGTextButton(hf, text);
		  b->Connect("Clicked()", "TVSDReader", this, "DisplayClusters()");
		  gCheckClusters = new TGCheckButton(hf, "", 10);
		  gCheckClusters->SetEnabled(kFALSE);

		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
		  hf->AddFrame(gCheckClusters);

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
      
		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "Tracks";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Traces";
		  b = new TGTextButton(hf, text);
		  b->Connect("Clicked()", "TVSDReader", this, "DisplayEsdTracks()");
		  gCheckESDTracks = new TGCheckButton(hf, "", 10);
		  gCheckESDTracks->SetEnabled(kFALSE);

		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
		  hf->AddFrame(gCheckESDTracks);

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "Geometry";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Geometrie";
		  b = new TGTextButton(hf, text);
		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "ChangeGeometry()");

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "Axes";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Axes";
		  b = new TGTextButton(hf, text);
		  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "DisplayGuides()");

		  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "Background";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Fond";
		  b = new TGTextButton(vf, text);
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "ChangeBackgroundColor()");

	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);
	  /*
	  gf = new TGGroupFrame(frmMain, "Animate event");

	  hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
	  {

	  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "ed_execute.png"));
	  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
	  b1->Connect("Clicked()", "TVSDReader", this, "StartAnimation()");

	  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "ed_interrupt.png"));
	  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
	  b1->Connect("Clicked()", "TVSDReader", this, "StopAnimation()");

	  }

	  gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);

	  */

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Encyvlopaedia";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Encyclopedie";
	  gf = new TGGroupFrame(frmMain, text);

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
	  {

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "ALICE Detector";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Detecteur ALICE";
		  b = new TGTextButton(vf, text);
		  b->Connect("Clicked()", "TVSDReader", this, "DetectorInfo()");
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "V0 Patterns";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Topologie V0";
		  b = new TGTextButton(vf, text);
		  b->Connect("Clicked()", "TVSDReader", this, "PatternInfo()");
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "2.76 TeV PbPb";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "2,76 TeV PbPb";
		  b = new TGTextButton(vf, text);
		  b->Connect("Clicked()", "TVSDReader", this, "PatternInfo2()");
		  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);

	  /*
	  gf = new TGGroupFrame(frmMain, "");

	  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
	  {

	  if(teacher)
	  {
	  b = new TGTextButton(vf, "Teacher Mode");
	  b->Connect("Clicked()", "TVSDReader", this, "ToTeacher()");
	  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
	  }

	  b = new TGTextButton(vf, "Exit");
	  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
	  b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

	  }

	  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

	  frmMain->AddFrame(gf);
	  */
	  vf = new TGVerticalFrame(frmMain, 150, 150, kFixedWidth);
	  {

		TString fileName("2011-Nov-24-ALICE_logo_scaled.eps");			
		b1 = new TGPictureButton(vf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));

	  }

	  vf->AddFrame(b1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));
	  frmMain->AddFrame(vf, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

	  frmMain->MapSubwindows();
	  frmMain->Resize();
	  frmMain->MapWindow();
	  
	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Student";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Etudiant";

	  browser->StopEmbedding(text);

	  browser->StartEmbedding(TRootBrowser::kBottom);

	  frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600, kHorizontalFrame);
	  frmMain->SetWindowName("XX GUI");
	  frmMain->SetCleanup(kDeepCleanup);

	  //   hf = new TGHorizontalFrame(frmMain);

	  const TGFont *font = gClient->GetFont("-*-helvetica-bold-r-normal-*-20-*-*-*-*-*-*-*");
	  FontStruct_t buttonFont = font->GetFontStruct();

	  if(globalChoice == 1)
	  {

		  gClient->GetColorByName("navy", buttonRedColor);
		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  text = "I'm ready! Start Exercise";
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  text = "Je suis pret ! Demarrer l'exercice";
		  b = new TGTextButton(frmMain, text);
		  b->SetTextColor(buttonRedColor);
		  b->SetFont(buttonFont);
		  //     hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "ToStudent()");

		  frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

	  }
	  else
	  {

		  gClient->GetColorByName("navy", buttonRedColor);
		  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			  b = new TGTextButton(frmMain, "Back To Demo");
		  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			  b = new TGTextButton(frmMain, "Retour Mode Demo");
		  b->SetTextColor(buttonRedColor);
		  b->SetFont(buttonFont);
		  //      hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
		  b->Connect("Clicked()", "TVSDReader", this, "ToDemo()");

		  frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

		  if(teacher && globalChoice != 1)
		  {

			  gClient->GetColorByName("navy", buttonRedColor);
			  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
				  b = new TGTextButton(frmMain,"Teacher Mode");
			  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
				  b = new TGTextButton(frmMain,"Mode Tuteur");
			  b->SetTextColor(buttonRedColor);
			  b->SetFont(buttonFont);
			  //         hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
			  b->Connect("Clicked()", "TVSDReader", this, "ToTeacher()");

			  frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

		  }

	  }

	  gClient->GetColorByName("red", buttonRedColor);
	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Exit";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Sortie";  
	  b = new TGTextButton(frmMain, text);
	  b->SetTextColor(buttonRedColor);
	  b->SetFont(buttonFont);
	  //   hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
	  b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

	  frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

	  frmMain->MapSubwindows();
	  frmMain->Resize();
	  frmMain->MapWindow();

	  browser->StopEmbedding(" ");

	  browser->StartEmbedding(TRootBrowser::kRight);

	  TCanvas* pad = new TCanvas();
	  pad->Divide(2, 2);

	  pad->cd(1);
	  gKMinvHist->Draw();

	  pad->cd(2);
	  gXMinvHist->Draw();

	  pad->cd(3);
	  gLMinvHist->Draw();

	  pad->cd(4);

	  gALMinvHist->Draw();

	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		  text = "Invariant Mass";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		  text = "Masse Invariante";
	  browser->StopEmbedding(text);
	  /*
	  browser->StartEmbedding(TRootBrowser::kRight);
   
	  pad = new TCanvas();
	  pad->Divide(2,2);

	  pad->cd(1);
	  gKRapHist->Draw();

	  pad->cd(2);
	  gXRapHist->Draw();

	  pad->cd(3);
	  gLRapHist->Draw();

	  pad->cd(4);
	  gALRapHist->Draw();

	  browser->StopEmbedding("Rapidity");

	  pad->Draw();
	  */
	  GotoEvent(0);

	  browser->GetTabRight()->SetTab(1);

  }

	//--------------------------------------------------------------
  void TeacherSet(){

/*
    DropEvent();

    fInDemoMode = kFALSE;

    globalChoice = 2;
    globalMode = 3;

    delete fVSD;
    delete fEvDirKeys;

    fFile->Close();
    delete fFile;

    fFile = TFile::Open("eve_files/data/AliVSD_example.root");
    if (!fFile)
      {
	Error("VSD_Reader", "Can not open the demo file ... terminating.");
	gSystem->Exit(1);
      }

    fEvDirKeys = new TObjArray;
    TPMERegexp name_re("Event\\d+");
    TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
      {
	if (name_re.Match(lnk->GetObject()->GetName()))
	  {
            fEvDirKeys->Add(lnk->GetObject());
	  }
	lnk = lnk->Next();
      }

    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
      {
	Error("VSD_Reader", "No events to show ... terminating.");
	gSystem->Exit(1);
      }

    fVertexLoaded = kFALSE;
    fITSClustersLoaded = kFALSE;
    fTPCClustersLoaded = kFALSE;
    fTRDClustersLoaded = kFALSE;
    fTOFClustersLoaded = kFALSE;
    fClustersLoaded = kFALSE;
    fEsdTracksLoaded = kFALSE;
    fV0sLoaded = kFALSE;
    fCascadesLoaded = kFALSE;
    fGuidesLoaded = kFALSE;
    fGeometrySet = kTRUE;

    fVSD = new TEveVSD;
    fTimer = new TTimer();
    fTimer->Connect("Timeout()", "TVSDReader", this, "AnimateEsdTracks()");
*/
//    LoadEsdTracks(0);

	 TString text; 
    Instructions();
    TEveBrowser* browser = gEve->GetBrowser();

    browser->GetMainFrame()->SetWindowName("ALICE MasterClass - TEACHER MODE");

    if(browser->GetTabRight()->GetTabTab(3))
      browser->GetTabRight()->RemoveTab(3);

    if(browser->GetTabRight()->GetTabTab(2))
      browser->GetTabRight()->RemoveTab(2);

    if(browser->GetTabLeft()->GetTabTab(0))
      browser->GetTabLeft()->RemoveTab(0);

    if(browser->GetTabBottom()->GetTabTab(0))
      browser->GetTabBottom()->RemoveTab(0);

    browser->StartEmbedding(TRootBrowser::kRight);
   
    pad1 = new TCanvas();
    pad1->Divide(2, 2);

    pad1->cd(1);
    gKMinvHist->Draw();

    pad1->cd(2);
    gXMinvHist->Draw();

    pad1->cd(3);
    gLMinvHist->Draw();

    pad1->cd(4);
    gALMinvHist->Draw();

    pad1->Draw();

    browser->StopEmbedding("Merged Invariant Mass");
/*
    browser->StartEmbedding(TRootBrowser::kRight);

    pad2 = new TCanvas();
    pad2->Divide(2, 2);

    pad2->cd(1);
    gKRapHist->Draw();

    pad2->cd(2);
    gXRapHist->Draw();

    pad2->cd(3);
    gLRapHist->Draw();

    pad2->cd(4);
    gALRapHist->Draw();

    pad2->Draw();


    browser->StopEmbedding("Merged Rapidity");
*/
    browser->StartEmbedding(TRootBrowser::kLeft);
 
    TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
    frmMain->SetWindowName("XX GUI");
    frmMain->SetCleanup(kDeepCleanup);

    TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
    TGPictureButton* b1 = nullptr;
    TGTextButton* b = nullptr;
    TGGroupFrame* gf = nullptr;
    TGVerticalFrame* vf = nullptr;
    ULong_t buttonRedColor;

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Teacher Instructions";
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Instructions Tuteur";
    gf = new TGGroupFrame(frmMain, text);

    vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

    {

      const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
      FontStruct_t buttonFont = font->GetFontStruct();
      ULong_t buttonRedColor;
      gClient->GetColorByName("red", buttonRedColor);
	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Global Instructions";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Instructions Globales";
      b = new TGTextButton(vf, text);
      b->SetTextColor(buttonRedColor);
      b->SetFont(buttonFont);
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b->Connect("Clicked()", "TVSDReader", this, "Instructions()");

    }

    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    frmMain->AddFrame(gf);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Teacher Controls";
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Controles Tuteur";
    gf = new TGGroupFrame(frmMain, text);

    vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

    {
	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			text = "Get Files";
	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			text = "Obtenir les Fichiers";
      b = new TGTextButton(vf, text);
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b->Connect("Clicked()", "TVSDReader", this, "UploadResults()");

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Save Histograms";
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Enregistrer les Histogrammes";
      b = new TGTextButton(vf, text);
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b->Connect("Clicked()", "TVSDReader", this, "SaveHistograms()");

    }

    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    frmMain->AddFrame(gf);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Results";
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Resultats";
    gf = new TGGroupFrame(frmMain, text);

    vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

    {
	  TString text;	
  	  if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
  		  text = "Table of Results";
  	  else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
  		  text = "Tableau des Resultats";
      b = new TGTextButton(vf, text);
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b->Connect("Clicked()", "TVSDReader", this, "CountStrangeParticles()");


//LEVEL 2 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
/*      b = new TGTextButton(vf, "Level 2");
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b->Connect("Clicked()", "TVSDReader", this, "CountStrangeParticles2()");
*/
    }

    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    frmMain->AddFrame(gf);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		text = "Encyclopaedia";
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		text = "Encyclopedie";
    gf = new TGGroupFrame(frmMain, text);

    vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
    {

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b = new TGTextButton(vf, "ALICE Detector");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b = new TGTextButton(vf, "Detecteur ALICE");
		b->Connect("Clicked()", "TVSDReader", this, "DetectorInfo()");
		vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b = new TGTextButton(vf, "V0 Patterns");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b = new TGTextButton(vf, "Topologie V0");
		b->Connect("Clicked()", "TVSDReader", this, "PatternInfo()");
		vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
			b = new TGTextButton(vf, "2.76 TeV PbPb");
		else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
			b = new TGTextButton(vf, "2,76 TeV PbPb");
		b->Connect("Clicked()", "TVSDReader", this, "PatternInfo2()");
		vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
	}

    gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    frmMain->AddFrame(gf);
/*
    gf = new TGGroupFrame(frmMain, "Animate event");

    TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
    {

      b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "ed_execute.png"));
      hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b1->Connect("Clicked()", "TVSDReader", this, "StartAnimation()");

      b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "ed_interrupt.png"));
      hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
      b1->Connect("Clicked()", "TVSDReader", this, "StopAnimation()");

    }

    gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

    frmMain->AddFrame(gf);
*/
    /*
      gf = new TGGroupFrame(frmMain, "");

      vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
      {

      b = new TGTextButton(vf, "Student Mode");
      b->Connect("Clicked()", "TVSDReader", this, "StudentSet()");
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      b = new TGTextButton(vf, "Exit");
      b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");
      vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

      }

      gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

      frmMain->AddFrame(gf);
    */

    vf = new TGVerticalFrame(frmMain, 150, 150, kFixedWidth);
    {
		TString fileName("2011-Nov-24-ALICE_logo_scaled.eps");			
		b1 = new TGPictureButton(vf, gClient->GetPicture(fileName.Prepend(gSystem->Getenv("IMPATH"))));
		vf->AddFrame(b1, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    }

    frmMain->AddFrame(vf, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY));

    frmMain->MapSubwindows();
    frmMain->Resize();
    frmMain->MapWindow();

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		browser->StopEmbedding("Teacher");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		browser->StopEmbedding("Tuteur");

	browser->GetTabRight()->SetTab(1);


    browser->StartEmbedding(TRootBrowser::kBottom);

    frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600, kHorizontalFrame);
    frmMain->SetWindowName("XX GUI");
    frmMain->SetCleanup(kDeepCleanup);

    const TGFont *font = gClient->GetFont("-*-helvetica-bold-r-normal-*-20-*-*-*-*-*-*-*");
    FontStruct_t buttonFont = font->GetFontStruct();

	gClient->GetColorByName("navy", buttonRedColor);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0)
		b = new TGTextButton(frmMain, "Student Mode");
	else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0)
		b = new TGTextButton(frmMain, "Mode Etudiant");
	b->SetTextColor(buttonRedColor);
	b->SetFont(buttonFont);
	b->Connect("Clicked()", "TVSDReader", this, "ToStudent()");

    frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

    gClient->GetColorByName("red", buttonRedColor);
    b = new TGTextButton(frmMain,"Exit");
    b->SetTextColor(buttonRedColor);
    b->SetFont(buttonFont);
    b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

    frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));
    frmMain->MapSubwindows();
    frmMain->Resize();
    frmMain->MapWindow();

    browser->StopEmbedding(" ");

  }

	//--------------------------------------------------------------
  void SaveHistograms()
  {

    TGFileInfo fi;
    fi.fFileTypes   = gMasterClassOutputPictue;
    fi.fIniDir      = StrDup(""); // current directory
    fi.fFileTypeIdx = 0;
    fi.fOverwrite   = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(),gEve->GetMainWindow(), kFDSave, &fi);
    if (!fi.fFilename) return;

    TPMERegexp filere(".*/([^/]+$)");
    if (filere.Match(fi.fFilename) != 2)
      {
        Warning("AliEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }

    gSystem->ChangeDirectory(fi.fIniDir);

    if(pad1)
      pad1->SaveAs(TString::Format("%s_Invariant_Mass.png",fi.fFilename));
/*
    if(pad2)
      pad2->SaveAs(TString::Format("%s_Rapidity.png",fi.fFilename));
*/

  }

	//--------------------------------------------------------------
  void Autosave()
  {

    ofstream myresult(TString::Format("autosave.masterclass%i",fCurEv+1));

    myresult << "Kaons" << endl;
    myresult << countKaons << endl;
    for(Int_t i=0; i<countKaons; i++)
      myresult << minvKaon[i] << " " << rapKaon[i] <<endl;

    myresult << "Lambas" << endl;
    myresult << countLambdas << endl;
    for(Int_t i=0; i<countLambdas; i++)
      myresult << minvLambda[i] << " " << rapLambda[i]  <<endl;

    myresult << "AntiLambas" << endl;
    myresult << countALambdas << endl;
    for(Int_t i=0; i<countALambdas; i++)
      myresult << minvALambda[i] << " " << rapALambda[i]  <<endl;

    myresult << "Xis" << endl;
    myresult << countXis << endl;
    for(Int_t i=0; i<countXis; i++)
      myresult << minvXi[i] << " " << rapXi[i]  <<endl;

    myresult << "Omegas" << endl;
    myresult << countOmegas << endl;
    for(Int_t i=0; i<countOmegas; i++)
      myresult << minvOmega[i] << " " << rapOmega[i]  <<endl;

    myresult << "Others" << endl;
    myresult << 0 << endl;

    myresult.close();

  }

	//--------------------------------------------------------------
  void UploadResults(){

    gLoads++;

    TGFileInfo fi;
    fi.fFileTypes   = gMasterClassOutputData;
    fi.fIniDir      = StrDup("");
    fi.fFileTypeIdx = 0;
    fi.fOverwrite   = kTRUE;
    new TGFileDialog(gClient->GetDefaultRoot(),gEve->GetMainWindow(), kFDOpen, &fi);
    if (!fi.fFilename) return;

    TPMERegexp filere(".*/([^/]+$)");
/*
    if (filere.Match(fi.fFilename) != 2)
      {
        Warning("AliEvePopupHandler", "file '%s' bad.", fi.fFilename);
        return;
      }
*/
    gSystem->ChangeDirectory(fi.fIniDir);

    string file1(filere[1]);

    ifstream data(fi.fFilename);

    string line;
    Double_t minv = 0;
    Double_t rapidity = 0;
    Int_t nparticles = 0;
  
    if(data.is_open()){

      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countKaons = countKaons+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
         
	  gKMinvHist->Fill(minv);
	  gKRapHist->Fill(rapidity);

	}

      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countLambdas = countLambdas+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
         
	  gLMinvHist->Fill(minv);
	  gLRapHist->Fill(rapidity);

	}


      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countALambdas = countALambdas+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
         
	  gALMinvHist->Fill(minv);
	  gALRapHist->Fill(rapidity);

	}

      getline(data,line);

      data >> nparticles;

      getline(data,line);

      countXis = countXis+nparticles;

      for(Int_t i = 0; i< nparticles; i++)
	{

	  data >> minv;
	  data >> rapidity;
	  getline(data,line);
	  gXMinvHist->Fill(minv);
	  gXRapHist->Fill(rapidity);

	}

      data.close();

      Double_t maxRap[4] = {gKRapHist->GetMaximum(), gXRapHist->GetMaximum(), gLRapHist->GetMaximum(), gALRapHist->GetMaximum()};
      Double_t maxMinv[4] = {gKMinvHist->GetMaximum(), gXMinvHist->GetMaximum(), gLMinvHist->GetMaximum(), gALMinvHist->GetMaximum()};

      gAllMinvHist->GetYaxis()->SetRangeUser(0,1.2*TMath::MaxElement(4,maxMinv));
      gAllRapHist->GetYaxis()->SetRangeUser(0,1.2*TMath::MaxElement(4,maxRap));

    }
  }

	//--------------------------------------------------------------
  void DestroyGeometry(){

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    gMultiView->SetDepth(-10);
    gMultiView->DestroyGeomRPhi();
    gMultiView->DestroyGeomRhoZ();
    gMultiView->SetDepth(0);

    gMultiView->SetRPhiAxes();
    gMultiView->SetRhoZAxes();


  }

	//--------------------------------------------------------------
  void GeometryDefault(){

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    TEveGeoShape *gentle_geom = 0;

    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    gentle_geom->SetRnrSelf(kFALSE);

    //ITS

    i = gentle_geom->BeginChildren();

    TEveGeoShape* lvl1 = (TEveGeoShape*) *i;

    //TPC

    i++;

    //TRD+TOF

    i++;

    lvl1 = (TEveGeoShape*) *i;
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

/*
    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    TEveGeoShape* trd1 = (TEveGeoShape*) gentle_geom->FindChild("BSEGMO17_1");
    trd1->SetRnrSelf(kTRUE);

    gEve->AddGlobalElement(gentle_geom);

    gMultiView->SetDepth(-10);
    gMultiView->DestroyGeomRPhi();
    gMultiView->ImportGeomRPhi(gentle_geom);
    gMultiView->DestroyGeomRhoZ();
    gMultiView->ImportGeomRhoZ(gentle_geom);
    gMultiView->SetDepth(0);

    gMultiView->SetRPhiAxes();
    gMultiView->SetRhoZAxes();

  }

	//--------------------------------------------------------------
  void Geometry1(){

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    TEveGeoShape *gentle_geom = 0;

    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    gentle_geom->SetRnrSelf(kFALSE);

    i = gentle_geom->BeginChildren();

    //ITS

    TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    TEveElement::List_i j = lvl1->BeginChildren();

    TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    TEveElement::List_i k = lvl2->BeginChildren();

    TEveGeoShape* its1 = (TEveGeoShape*) *k;
    its1->SetRnrSelf(kTRUE);
    its1->SetMainColor(kMagenta-5);
    its1->SetMainTransparency(60);
    k++;

    TEveGeoShape* its2 = (TEveGeoShape*) *k;
    its2->SetRnrSelf(kTRUE);
    its2->SetMainColor(kCyan-5);
    its2->SetMainTransparency(60);
    k++;

    TEveGeoShape* its3 = (TEveGeoShape*) *k;
    its3->SetRnrSelf(kTRUE);
    its3->SetMainColor(kGreen-5);
    its3->SetMainTransparency(60);

    //TPC

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    k = lvl2->BeginChildren();

    TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
    lvl3->SetRnrSelf(kFALSE);
    lvl3->SetMainColor(1);
    lvl3->SetMainTransparency(70);

    TEveElement::List_i l = lvl3->BeginChildren();

    TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
    lvl4->SetRnrSelf(kFALSE);
    TEveElement::List_i m = lvl4->BeginChildren();

    TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
    tpc1->SetRnrSelf(kTRUE);
    tpc1->SetMainColor(1);
    tpc1->SetMainTransparency(70);
    m++;

    TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
    tpc2->SetRnrSelf(kTRUE);
    tpc2->SetMainColor(1);
    tpc2->SetMainTransparency(70);
    m++;

    TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
    tpc3->SetRnrSelf(kTRUE);
    tpc3->SetMainColor(1);
    tpc3->SetMainTransparency(70);
    m++;

    //TRD+TOF

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    TEveGeoShape* trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainColor(kMagenta-8);
    trd1->SetMainTransparency(80);
    j++;

    TEveGeoShape* tof1 = (TEveGeoShape*) *j;
    tof1->SetRnrSelf(kTRUE);
    tof1->SetMainColor(kMagenta-1);
    tof1->SetMainTransparency(80);

    //PHOS

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainColor(kGreen-8);
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
	lvl2->SetMainColor(kBlue-5);
	lvl2->SetMainTransparency(20);
      }


    gEve->AddGlobalElement(gentle_geom);
*/

    gMultiView->SetDepth(-10);
    gMultiView->DestroyGeomRPhi();
    gMultiView->ImportGeomRPhi(gentle_geom);
    gMultiView->DestroyGeomRhoZ();
    gMultiView->ImportGeomRhoZ(gentle_geom);
    gMultiView->SetDepth(0);

    gMultiView->SetRPhiAxes();
    gMultiView->SetRhoZAxes();


  }

	//--------------------------------------------------------------
  void Geometry2(){

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    TEveGeoShape *gentle_geom = 0;

    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    gentle_geom->SetRnrSelf(kFALSE);
    i = gentle_geom->BeginChildren();

    //ITS

    TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    TEveElement::List_i j = lvl1->BeginChildren();

    TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    TEveElement::List_i k = lvl2->BeginChildren();

    TEveGeoShape* its1 = (TEveGeoShape*) *k;
    its1->SetRnrSelf(kTRUE);
    its1->SetMainColor(kYellow-4);
    its1->SetMainTransparency(50);
    k++;

    TEveGeoShape* its2 = (TEveGeoShape*) *k;
    its2->SetRnrSelf(kTRUE);
    its2->SetMainColor(kYellow-7);
    its2->SetMainTransparency(50);
    k++;

    TEveGeoShape* its3 = (TEveGeoShape*) *k;
    its3->SetRnrSelf(kTRUE);
    its3->SetMainColor(kYellow-9);
    its3->SetMainTransparency(50);

    //TPC

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    k = lvl2->BeginChildren();

    TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
    lvl3->SetRnrSelf(kTRUE);
    lvl3->SetMainColor(kGray);
    lvl3->SetMainTransparency(80);
    TEveElement::List_i l = lvl3->BeginChildren();

    TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
    lvl4->SetRnrSelf(kFALSE);
    TEveElement::List_i m = lvl4->BeginChildren();

    TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
    tpc1->SetRnrSelf(kTRUE);
    tpc1->SetMainColor(kGray+2);
    tpc1->SetMainTransparency(80);
    m++;

    TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
    tpc2->SetMainColor(kGray);
    tpc2->SetMainColor(kGray+2);
    tpc2->SetMainTransparency(80);
    m++;

    TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
    tpc3->SetRnrSelf(kTRUE);
    tpc3->SetMainColor(kGray+2);
    tpc3->SetMainTransparency(80);
    m++;

    //TRD+TOF

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kFALSE);
	lvl2->SetMainColor(0);
	lvl2->SetMainTransparency(80);
      }

    //PHOS

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainTransparency(30);
      }


    gEve->AddGlobalElement(gentle_geom);

    gMultiView->SetDepth(-10);
    gMultiView->DestroyGeomRPhi();
    gMultiView->ImportGeomRPhi(gentle_geom);
    gMultiView->DestroyGeomRhoZ();
    gMultiView->ImportGeomRhoZ(gentle_geom);
    gMultiView->SetDepth(0);

    gMultiView->SetRPhiAxes();
    gMultiView->SetRhoZAxes();

  }


	//--------------------------------------------------------------
  void Geometry3(){

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    TEveGeoShape *gentle_geom = 0;

    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    gentle_geom->SetRnrSelf(kFALSE);

    i = gentle_geom->BeginChildren();
    //ITS

    TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    TEveElement::List_i j = lvl1->BeginChildren();

    TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    TEveElement::List_i k = lvl2->BeginChildren();

    TEveGeoShape* its1 = (TEveGeoShape*) *k;
    its1->SetRnrSelf(kTRUE);
    its1->SetMainColor(0);
    its1->SetMainTransparency(80);
    k++;

    TEveGeoShape* its2 = (TEveGeoShape*) *k;
    its2->SetRnrSelf(kTRUE);
    its2->SetMainColor(0);
    its2->SetMainTransparency(80);
    k++;

    TEveGeoShape* its3 = (TEveGeoShape*) *k;
    its3->SetRnrSelf(kTRUE);
    its3->SetMainColor(0);
    its3->SetMainTransparency(80);

    //TPC

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    k = lvl2->BeginChildren();

    TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
    lvl3->SetRnrSelf(kTRUE);
    lvl3->SetMainColor(0);
    lvl3->SetMainTransparency(80);
    TEveElement::List_i l = lvl3->BeginChildren();

    TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
    lvl4->SetRnrSelf(kFALSE);
    TEveElement::List_i m = lvl4->BeginChildren();

    TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
    tpc1->SetRnrSelf(kTRUE);
    tpc1->SetMainColor(0);
    tpc1->SetMainTransparency(80);
    m++;

    TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
    tpc2->SetMainColor(kGray);
    tpc2->SetMainColor(0);
    tpc2->SetMainTransparency(80);
    m++;

    TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
    tpc3->SetRnrSelf(kTRUE);
    tpc3->SetMainColor(0);
    tpc3->SetMainTransparency(80);
    m++;

    //TRD+TOF

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kTRUE);
    lvl2->SetMainColor(0);
    lvl2->SetMainTransparency(80);
    j++;

    lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kTRUE);
    lvl2->SetMainColor(0);
    lvl2->SetMainTransparency(80);

    //PHOS

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainColor(0);
	lvl2->SetMainTransparency(80);
      }

    //HMPID

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainColor(0);
	lvl2->SetMainTransparency(80);
      }

    gEve->AddGlobalElement(gentle_geom);

    gMultiView->SetDepth(-10);
    gMultiView->DestroyGeomRPhi();
    gMultiView->ImportGeomRPhi(gentle_geom);
    gMultiView->DestroyGeomRhoZ();
    gMultiView->ImportGeomRhoZ(gentle_geom);
    gMultiView->SetDepth(0);

    gMultiView->SetRPhiAxes();
    gMultiView->SetRhoZAxes();

  }

	//--------------------------------------------------------------
  void Geometry4(){

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    TEveGeoShape *gentle_geom = 0;

    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    gentle_geom->SetRnrSelf(kFALSE);

    i = gentle_geom->BeginChildren();

    //ITS

    TEveGeoShape* lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    TEveElement::List_i j = lvl1->BeginChildren();

    TEveGeoShape* lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    TEveElement::List_i k = lvl2->BeginChildren();

    TEveGeoShape* its1 = (TEveGeoShape*) *k;
    its1->SetRnrSelf(kTRUE);
    its1->SetMainColor(1);
    its1->SetMainTransparency(80);
    k++;

    TEveGeoShape* its2 = (TEveGeoShape*) *k;
    its2->SetRnrSelf(kTRUE);
    its2->SetMainColor(1);
    its2->SetMainTransparency(80);
    k++;

    TEveGeoShape* its3 = (TEveGeoShape*) *k;
    its3->SetRnrSelf(kTRUE);
    its3->SetMainColor(1);
    its3->SetMainTransparency(80);

    //TPC

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    lvl2 = (TEveGeoShape*) *j;
    lvl2->SetRnrSelf(kFALSE);
    k = lvl2->BeginChildren();

    TEveGeoShape* lvl3 = (TEveGeoShape*) *k;
    lvl3->SetRnrSelf(kFALSE);
    lvl3->SetMainColor(1);
    lvl3->SetMainTransparency(80);

    TEveElement::List_i l = lvl3->BeginChildren();

    TEveGeoShape* lvl4 = (TEveGeoShape*) *l;
    lvl4->SetRnrSelf(kFALSE);
    TEveElement::List_i m = lvl4->BeginChildren();

    TEveGeoShape* tpc1 = (TEveGeoShape*) *m;
    tpc1->SetRnrSelf(kTRUE);
    tpc1->SetMainColor(1);
    tpc1->SetMainTransparency(80);
    m++;

    TEveGeoShape* tpc2 = (TEveGeoShape*) *m;
    tpc2->SetRnrSelf(kTRUE);
    tpc2->SetMainColor(1);
    tpc2->SetMainTransparency(80);
    m++;

    TEveGeoShape* tpc3 = (TEveGeoShape*) *m;
    tpc3->SetRnrSelf(kTRUE);
    tpc3->SetMainColor(1);
    tpc3->SetMainTransparency(80);
    m++;

    //TRD+TOF

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);
    j = lvl1->BeginChildren();

    TEveGeoShape* trd1 = (TEveGeoShape*) *j;
    trd1->SetRnrSelf(kTRUE);
    trd1->SetMainColor(1);
    trd1->SetMainTransparency(80);
    j++;

    TEveGeoShape* tof1 = (TEveGeoShape*) *j;
    tof1->SetRnrSelf(kTRUE);
    tof1->SetMainColor(1);
    tof1->SetMainTransparency(80);

    //PHOS

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainColor(1);
	lvl2->SetMainTransparency(80);
      }

    //HMPID

    i++;

    lvl1 = (TEveGeoShape*) *i;
    lvl1->SetRnrSelf(kFALSE);

    for(j = lvl1->BeginChildren(); j != lvl1->EndChildren(); ++j)
      {
	lvl2 = (TEveGeoShape*) *j;
	lvl2->SetRnrSelf(kTRUE);
	lvl2->SetMainColor(1);
	lvl2->SetMainTransparency(20);
      }


    gEve->AddGlobalElement(gentle_geom);

    gMultiView->SetDepth(-10);
    gMultiView->DestroyGeomRPhi();
    gMultiView->ImportGeomRPhi(gentle_geom);
    gMultiView->DestroyGeomRhoZ();
    gMultiView->ImportGeomRhoZ(gentle_geom);
    gMultiView->SetDepth(0);

    gMultiView->SetRPhiAxes();
    gMultiView->SetRhoZAxes();


  }

	//--------------------------------------------------------------
  void ChangeGeometry(){

    if(fGeometrySet)
      {
	DestroyGeometry();
	fGeometrySet = kFALSE;
      }
    else
      {
	GeometryDefault();
	fGeometrySet = kTRUE;
      }

    /*
      geometry++;

      if(geometry == 5)
      geometry = 0;

      switch (geometry)
      {
      case 0:
      GeometryDefault();
      break;

      case 1:
      Geometry1();
      break;

      case 2:
      Geometry2();
      break;

      case 3:
      Geometry3();
      break;

      case 4:
      Geometry4();
      break;

      default:
      GeometryDefault();
      break;
      }
    */
  }

	//--------------------------------------------------------------
  void ChangeBackgroundColor(){
      
    gEve->GetViewers()->SwitchColorSet();

  }

	//--------------------------------------------------------------
  void AttachEvent()
  {
    // Attach event data from current directory.

    fVSD->LoadTrees();
    fVSD->SetBranchAddresses();
  }

	//--------------------------------------------------------------
  void DropEvent()
  {
    // Drop currently held event data, release current directory.

    // Drop old visualization structures.

    gEve->GetViewers()->DeleteAnnotations();
    gEve->GetCurrentEvent()->DestroyElements();

    // Drop old event-data.

    fVSD->DeleteTrees();
    delete fDirectory;
    fDirectory = 0;
  }

  //---------------------------------------------------------------------------
  // Event navigation
  //---------------------------------------------------------------------------

	//--------------------------------------------------------------
  void NextEvent()
  {
    if(fCurEv == fMaxEv-1)
      GotoEvent(0);
    else
      GotoEvent(fCurEv + 1);

    if(gAliceCalculator)
      gAliceCalculator->SetZero();

    Autosave();

  }

	//--------------------------------------------------------------
  void PrevEvent()
  {
    if(fCurEv == 0)
      GotoEvent(fMaxEv-1);
    else
      GotoEvent(fCurEv - 1);

    if(gAliceCalculator)
      gAliceCalculator->SetZero();
  }

	//--------------------------------------------------------------
  Bool_t GotoEvent(Int_t ev)
  {
      
    change = kTRUE;

    if (ev < 0 || ev >= fMaxEv)
      {
	Warning("GotoEvent", "fMaxEv = fEvDirKeys->GetEntriesFast();Invalid event id %d.", ev);
	return kFALSE;
      }

    if(fTimer)
      fTimer->Stop();

    if(fRAnim)
      fRAnim=0;

    DropEvent();

    // Connect to new event-data.

    fCurEv = ev;
    fDirectory = (TDirectory*) ((TKey*) fEvDirKeys->At(fCurEv))->ReadObj();

    //      if(fVSD && fDirectory)
    fVSD->SetDirectory(fDirectory);

    AttachEvent();

    //      if(gEventNumber)
    gEventNumber->SetText(TString::Format("%i / %i",fCurEv+1,globalDataset));

    // Load event data into visualization structures.

    if(fVertexLoaded)
      LoadVertex();

    if(fGuidesLoaded)
      LoadGuides();

    if(fITSClustersLoaded)
      LoadClusters(fITSClusters, "ITS", 0);

    if(fTPCClustersLoaded)
      LoadClusters(fTPCClusters, "TPC", 1);

    if(fTRDClustersLoaded)
      LoadClusters(fTRDClusters, "TRD", 2);

    if(fTOFClustersLoaded)
      LoadClusters(fTOFClusters, "TOF", 3);

    if(fEsdTracksLoaded)
      LoadEsdTracks(fMaxR);

    if(fV0sLoaded)
      LoadV0s();

    if(fCascadesLoaded)
      LoadCascades();

    if(overlayButton && gMultiView)
      gMultiView->Get3DView()->GetGLViewer()->RemoveOverlayElement((TGLOverlayElement*)overlayButton);

    if(globalMode == 1 && gMultiView)
      {

	overlayButton = new TGLOverlayButton(gMultiView->Get3DView()->GetGLViewer(),  "", 0, 0, 200, 30);

	if(gEventNumber->GetText()->GetString() == TString::Format("1"))
	  overlayButton->SetText("Kaon");

	if(gEventNumber->GetText()->GetString() == TString::Format("2"))
	  overlayButton->SetText("Lambda");

	if(gEventNumber->GetText()->GetString() == TString::Format("3"))
	  overlayButton->SetText("antiLambda");

	if(gEventNumber->GetText()->GetString() == TString::Format("4"))
	  overlayButton->SetText("Xi");

	overlayButton->SetAlphaValues(0.1, 0.8);
      }

    if(gMultiView)
      {
	obutton1->SetPosition(0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-25);
	obutton2->SetPosition(0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-45);
	obutton3->SetPosition(0, gMultiView->Get3DView()->GetEveFrame()->GetHeight()-65);
      }

    TEveElement* top = gEve->GetCurrentEvent();

    if(gMultiView)
      {
	gMultiView->DestroyEventRPhi();
	gMultiView->ImportEventRPhi(top);

	gMultiView->DestroyEventRhoZ();
	gMultiView->ImportEventRhoZ(top);

	gEve->Redraw3D(kFALSE, kTRUE);
      }

    return kTRUE;

  }


//____________________________ZLiczanie Eventow!!!!!!!!!!!!!!!

void CountEvents()
  {
 
		
		gEventAnalysed->SetText(TString::Format("%i",fCount+1));
	fCount++;		
//gEventAnalysed++;
   Autosave();

  }



  //---------------------------------------------------------------------------
  // Display Clusters and Tracks
  //---------------------------------------------------------------------------

  void Message()
  {
    printf("done.\n");
  }

	//--------------------------------------------------------------
  void DisplayGuides(){

    if(fGuidesLoaded)
      {
	fGuidesLoaded = kFALSE;
	GotoEvent(fCurEv);
      }
    else
      {
	fGuidesLoaded = kTRUE;
	GotoEvent(fCurEv);
      }

  }

	//--------------------------------------------------------------
  void DisplayVertex()
  {

    if(fVertexLoaded)
      {

	fVertexLoaded=kFALSE;
	GotoEvent(fCurEv);
	gCheckVertex->SetDisabledAndSelected(kFALSE);
      }

    else
      {

	fVertexLoaded=kTRUE;
	printf("Loading Vertex...\n");
	GotoEvent(fCurEv);
	printf("Vertex loaded.\n");
	gCheckVertex->SetDisabledAndSelected(kTRUE);
         
      }

  }

	//--------------------------------------------------------------
  void DisplayClusters()
  {

    if(fClustersLoaded)
      {

	fClustersLoaded = kFALSE;
	fITSClustersLoaded=kFALSE;
	fTPCClustersLoaded=kFALSE;
	fTRDClustersLoaded=kFALSE;
	fTOFClustersLoaded=kFALSE;
	GotoEvent(fCurEv);
	gCheckClusters->SetDisabledAndSelected(kFALSE);
      }

    else
      {
	fClustersLoaded = kTRUE;
	fITSClustersLoaded=kTRUE;
	fTPCClustersLoaded=kTRUE;
	fTRDClustersLoaded=kTRUE;
	fTOFClustersLoaded=kTRUE;
	printf("Loading Clusters...\n");
	GotoEvent(fCurEv);
	printf("Clusters loaded.\n");
	gCheckClusters->SetDisabledAndSelected(kTRUE);
      }

  }

	//--------------------------------------------------------------
  void DisplayITSClusters()
  {

    if(fITSClustersLoaded)
      {

	fITSClustersLoaded=kFALSE;
	GotoEvent(fCurEv);
	gCheckITSClusters->SetDisabledAndSelected(kFALSE);

      }

    else
      {

	fITSClustersLoaded=kTRUE;
	printf("Loading ITS Clusters...\n");
	GotoEvent(fCurEv);
	printf("ITS Clusters loaded.\n");
	gCheckITSClusters->SetDisabledAndSelected(kTRUE);
      }

  }

	//--------------------------------------------------------------
  void DisplayTPCClusters()
  {

    if(fTPCClustersLoaded)
      {

	fTPCClustersLoaded=kFALSE;
	GotoEvent(fCurEv);
	gCheckTPCClusters->SetDisabledAndSelected(kFALSE);

      }

    else
      {

  
	fTPCClustersLoaded=kTRUE;
	printf("Loading TPC Clusters...\n");
	GotoEvent(fCurEv);
	printf("TPC Clusters loaded.\n");
	gCheckTPCClusters->SetDisabledAndSelected(kTRUE);
         
      }

  }

	//--------------------------------------------------------------
  void DisplayTRDClusters()
  {

    if(fTRDClustersLoaded)
      {

	fTRDClustersLoaded=kFALSE;
	GotoEvent(fCurEv);
	gCheckTRDClusters->SetDisabledAndSelected(kFALSE);
      }

    else
      {

	fTRDClustersLoaded=kTRUE;
	printf("Loading TRD Clusters...\n");
	GotoEvent(fCurEv);
	printf("TRD Clusters loaded.\n");
	gCheckTRDClusters->SetDisabledAndSelected(kTRUE);         
      }

  }

	//--------------------------------------------------------------
  void DisplayTOFClusters()
  {

    if(fTOFClustersLoaded)
      {

	fTOFClustersLoaded=kFALSE;
	GotoEvent(fCurEv);

	gCheckTOFClusters->SetDisabledAndSelected(kFALSE);
      }

    else
      {

	fTOFClustersLoaded=kTRUE;
	printf("Loading TOF Clusters...\n");
	GotoEvent(fCurEv);
	printf("TOF Clusters loaded.\n");
         
	gCheckTOFClusters->SetDisabledAndSelected(kTRUE);
      }

  }

	//--------------------------------------------------------------
  void DisplayEsdTracks()
  {

    if(fEsdTracksLoaded)
      {

	fEsdTracksLoaded=kFALSE;
	GotoEvent(fCurEv);

	if(gCheckESDTracks)
	  gCheckESDTracks->SetDisabledAndSelected(kFALSE);
      }

    else
      {

	fEsdTracksLoaded=kTRUE;
	printf("Loading Esd Tracks...\n");
	GotoEvent(fCurEv);
	printf("Esd Tracks loaded.\n");
	gCheckESDTracks->SetDisabledAndSelected(kTRUE);
         
      }

  }

	//--------------------------------------------------------------
  void DisplayV0s()
  {

    if(fV0sLoaded)
      {

	fV0sLoaded=kFALSE;
	GotoEvent(fCurEv);

	gCheckV0s->SetDisabledAndSelected(kFALSE);
      }

    else
      {

	fV0sLoaded=kTRUE;
	printf("Loading V0s...\n");
	GotoEvent(fCurEv);
	printf("V0s loaded.\n");
	gCheckV0s->SetDisabledAndSelected(kTRUE);
         
      }

  }

	//--------------------------------------------------------------
  void DisplayCascades()
  {

    if(fCascadesLoaded)
      {

	fCascadesLoaded=kFALSE;
	gCheckCascades->SetDisabledAndSelected(kFALSE);

	GotoEvent(fCurEv);

      }

    else
      {

	fCascadesLoaded=kTRUE;
	printf("Loading Cascades...\n");
	GotoEvent(fCurEv);
	printf("Cascades loaded.\n");
	gCheckCascades->SetDisabledAndSelected(kTRUE);
         
      }

  }

  //---------------------------------------------------------------------------
  // Tracks animating
  //---------------------------------------------------------------------------

  void AnimateEsdTracks()
  {

    TEveTrackPropagator* trkProp = fTrackList->GetPropagator();
    trkProp->SetMaxR(fRAnim); //R[cm] 

    fRAnim+=10;

    gEve->AddElement(fTrackList);

    TEveElement* top = gEve->GetCurrentEvent();

    gMultiView->DestroyEventRPhi();
    gMultiView->ImportEventRPhi(top);

    gMultiView->DestroyEventRhoZ();
    gMultiView->ImportEventRhoZ(top);

    gEve->Redraw3D(kFALSE, kTRUE);

    if(fRAnim>fMaxR)
      fTimer->Stop();

  }

	//--------------------------------------------------------------
  void StartAnimation()
  {

    if(fVertexLoaded)
      {
	fVertexLoaded=kFALSE;
	gCheckVertex->SetDisabledAndSelected(kFALSE);
      }

    if(fClustersLoaded)
      {
	fClustersLoaded=kFALSE;
	gCheckClusters->SetDisabledAndSelected(kFALSE);
      }

    if(fITSClustersLoaded)
      {
	fITSClustersLoaded=kFALSE;
      }

    if(fTPCClustersLoaded)
      {
	fTPCClustersLoaded=kFALSE;
      }

    if(fTRDClustersLoaded)
      {
	fTRDClustersLoaded=kFALSE;
      }

    if(fTOFClustersLoaded)
      {
	fTOFClustersLoaded=kFALSE;
      }

    if(fEsdTracksLoaded)
      {
	fEsdTracksLoaded=kFALSE;
	gCheckESDTracks->SetDisabledAndSelected(kFALSE);
      }

    if(fV0sLoaded)
      {
	fV0sLoaded=kFALSE;
	gCheckV0s->SetDisabledAndSelected(kFALSE);
      }

    if(fCascadesLoaded)
      {
	fCascadesLoaded=kFALSE;
	gCheckCascades->SetDisabledAndSelected(kFALSE);
      }

    if(fGeometrySet)
      {
	fGeometrySet=kFALSE;
	DestroyGeometry();
      }

    GotoEvent(fCurEv);

    if(fEsdTracksLoaded)
      {
	fEsdTracksLoaded=kFALSE;
	GotoEvent(fCurEv);
      }
    else
      {
	LoadEsdTracks(fMaxR);
	fEsdTracksLoaded=kTRUE;
      }  

    fTimer->Start(5,kFALSE);

  }

	//--------------------------------------------------------------
  void StopAnimation()
  {

    fTimer->Stop();

  }

  //---------------------------------------------------------------------------
  // Guides
  //---------------------------------------------------------------------------

  void LoadGuides()
  {

    for(Int_t i=0; i<12; i++)
      {
	guideX[i] = new TEveLine(" ");
	guideX[i]->SetLineColor(kRed);
	guideX[i]->SetLineWidth(1);
	guideX[i]->SetLineStyle(1);
      }

    for(Int_t i=0; i<14; i++)
      {
	guideY[i] = new TEveLine(" ");
	guideY[i]->SetLineColor(kGreen);
	guideY[i]->SetLineWidth(1);
	guideY[i]->SetLineStyle(1);

	guideZ[i] = new TEveLine(" ");
	guideZ[i]->SetLineColor(kBlue);
	guideZ[i]->SetLineWidth(1);
	guideZ[i]->SetLineStyle(1);
      }

    //line X

    guideX[0]->SetPoint(0, -100,  0,  0);
    guideX[0]->SetPoint(1,  100,  0,  0);

    //arrows X
    guideX[1]->SetPoint(0, -100,  0,  0);
    guideX[1]->SetPoint(1, -95,  5,  0);

    guideX[2]->SetPoint(0, -100,  0,  0);
    guideX[2]->SetPoint(1, -95, -5,  0);

    guideX[3]->SetPoint(0,  100,  0,  0);
    guideX[3]->SetPoint(1,  95,  5,  0);

    guideX[4]->SetPoint(0,  100,  0,  0);
    guideX[4]->SetPoint(1,  95, -5,  0);

    //+X
    guideX[5]->SetPoint(0, 95,  10,  0);
    guideX[5]->SetPoint(1, 90,  10,  0);

    guideX[6]->SetPoint(0, 92.5,  7.5,  0);
    guideX[6]->SetPoint(1, 92.5, 12.5,  0);

    guideX[7]->SetPoint(0,  90,  6,  0);
    guideX[7]->SetPoint(1,  86,  14,  0);

    guideX[8]->SetPoint(0,  90,  14,  0);
    guideX[8]->SetPoint(1,  86, 6,  0);

    //-X
    guideX[9]->SetPoint(0, -85,  10,  0);
    guideX[9]->SetPoint(1, -90,  10,  0);

    guideX[10]->SetPoint(0,  -95,  6,  0);
    guideX[10]->SetPoint(1,  -91,  14,  0);

    guideX[11]->SetPoint(0,  -95,  14,  0);
    guideX[11]->SetPoint(1,  -91, 6,  0);

    //line Y
    guideY[0]->SetPoint(0,  0, -100,  0);
    guideY[0]->SetPoint(1,  0,  100,  0);

    //arrows Y
    guideY[1]->SetPoint(0,  0, -100,  0);
    guideY[1]->SetPoint(1,  0, -95,  5);

    guideY[2]->SetPoint(0,  0, -100,  0);
    guideY[2]->SetPoint(1,  0, -95, -5);

    guideY[3]->SetPoint(0,  0,  100,  0);
    guideY[3]->SetPoint(1,  0,  95,  5);

    guideY[4]->SetPoint(0,  0,  100,  0);
    guideY[4]->SetPoint(1,  0,  95, -5);

    //+Y
    guideY[5]->SetPoint(0, 0, 95,  10);
    guideY[5]->SetPoint(1, 0, 90,  10);

    guideY[6]->SetPoint(0, 0, 92.5,  7.5);
    guideY[6]->SetPoint(1, 0, 92.5, 12.5);

    guideY[7]->SetPoint(0,  0, 96.5,  12.5);
    guideY[7]->SetPoint(1,  0, 92.5,  14.5);

    guideY[8]->SetPoint(0,  0, 96.5,  16.5);
    guideY[8]->SetPoint(1,  0, 92.5, 14.5);

    guideY[9]->SetPoint(0,  0, 92.5,  14.5);
    guideY[9]->SetPoint(1,  0, 88.5, 14.5);


    //-Y
    guideY[10]->SetPoint(0, 0, -92.5,  7.5);
    guideY[10]->SetPoint(1, 0, -92.5, 12.5);

    guideY[11]->SetPoint(0,  0, -88.5,  12.5);
    guideY[11]->SetPoint(1,  0, -92.5,  14.5);

    guideY[12]->SetPoint(0,  0, -88.5,  16.5);
    guideY[12]->SetPoint(1,  0, -92.5, 14.5);

    guideY[13]->SetPoint(0,  0, -92.5,  14.5);
    guideY[13]->SetPoint(1,  0, -96.5, 14.5);

    //line Z
    guideZ[0]->SetPoint(0,  0,  0, -100);
    guideZ[0]->SetPoint(1,  0,  0,  100);

    //Z arrows
    guideZ[1]->SetPoint(0,  0,  0, -100);
    guideZ[1]->SetPoint(1,  8,  0, -92);

    guideZ[2]->SetPoint(0,  0,  0, -100);
    guideZ[2]->SetPoint(1, -8,  0, -92);

    guideZ[3]->SetPoint(0,  0,  0,  100);
    guideZ[3]->SetPoint(1,  8,  0,  92);

    guideZ[4]->SetPoint(0,  0,  0,  100);
    guideZ[4]->SetPoint(1, -8,  0,  92);

    //+Z
    guideZ[5]->SetPoint(0, 0, 10, 90);
    guideZ[5]->SetPoint(1, 0, 10, 85);

    guideZ[6]->SetPoint(0, 0, 7.5, 87.5);
    guideZ[6]->SetPoint(1, 0, 12.5, 87.5);

    guideZ[7]->SetPoint(0,  0, 14, 95);
    guideZ[7]->SetPoint(1,  0, 14, 90);

    guideZ[8]->SetPoint(0,  0, 14, 95);
    guideZ[8]->SetPoint(1,  0, 6, 90);

    guideZ[9]->SetPoint(0,  0, 6, 90);
    guideZ[9]->SetPoint(1,  0, 6, 95);

    //-Z
    guideZ[10]->SetPoint(0, 0, 10, -90);
    guideZ[10]->SetPoint(1, 0, 10, -95);

    guideZ[11]->SetPoint(0,  0, 14, -90);
    guideZ[11]->SetPoint(1,  0, 14, -85);

    guideZ[12]->SetPoint(0,  0, 14, -85);
    guideZ[12]->SetPoint(1,  0, 6, -90);

    guideZ[13]->SetPoint(0,  0, 6, -85);
    guideZ[13]->SetPoint(1,  0, 6, -90);

    for(Int_t i=0; i<12; i++)
      {
	gEve->AddElement(guideX[i]);
      }

    for(Int_t i=0; i<14; i++)
      {
	gEve->AddElement(guideY[i]);
	gEve->AddElement(guideZ[i]);
      }


  }

  //---------------------------------------------------------------------------
  // Primary Vertex
  //---------------------------------------------------------------------------

  void LoadVertex()
  {
    if (fPsVertex == 0)
      {
	fPsVertex = new TEvePointSet("Vertex");
	fPsVertex->SetMainColor(14);
	fPsVertex->SetMarkerSize(2);
	fPsVertex->SetMarkerStyle(2);
	fPsVertex->IncDenyDestroy();
      }
    else
      {
	fPsVertex->Reset();
      }

    TEvePointSelector ss(fVSD->fTreeH, fPsVertex, "fV.fX:fV.fX:fV.fZ");
    ss.Select();

    fPsVertex->SetTitle("Primary Vertex");

    gEve->AddElement(fPsVertex);

    TEveLine* pointingLine = new TEveLine(TString::Format(" "));
    pointingLine->SetPoint(0, 0, 0, 0);
    pointingLine->SetPoint(1, 0, 0, 0);
    pointingLine->SetLineColor(0);
    pointingLine->SetLineWidth(0);
    pointingLine->SetLineStyle(0);
    gEve->AddElement(pointingLine);



  }

  //---------------------------------------------------------------------------
  // Cluster loading
  //---------------------------------------------------------------------------

  void LoadClusters(TEvePointSet*& ps, const TString& det_name, Int_t det_id)
  {
    if (ps == 0)
      {
	ps = new TEvePointSet(det_name);
	ps->SetMainColor((Color_t)(det_id + 2));
	ps->SetMarkerSize(0.5);
	ps->SetMarkerStyle(2);
	ps->IncDenyDestroy();
      }
    else
      {
	ps->Reset();
      }

    TEvePointSelector ss(fVSD->fTreeC, ps, "fV.fX:fV.fY:fV.fZ",
			 TString::Format("fDetId==%d", det_id));
    ss.Select();
    ps->SetTitle(det_name + " Clusters");

    gEve->AddElement(ps);

  }

  //---------------------------------------------------------------------------
  // Track loading
  //---------------------------------------------------------------------------

  enum ESDTrackFlags
    {
      kITSin=0x0001,kITSout=0x0002,kITSrefit=0x0004,kITSpid=0x0008,
      kTPCin=0x0010,kTPCout=0x0020,kTPCrefit=0x0040,kTPCpid=0x0080,
      kTRDin=0x0100,kTRDout=0x0200,kTRDrefit=0x0400,kTRDpid=0x0800,
      kTOFin=0x1000,kTOFout=0x2000,kTOFrefit=0x4000,kTOFpid=0x8000,
      kHMPIDpid=0x20000,
      kEMCALmatch=0x40000,
      kTRDbackup=0x80000,
      kTRDStop=0x20000000,
      kESDpid=0x40000000,
      kTIME=0x80000000
    };

	//--------------------------------------------------------------
  Bool_t trackIsOn(TEveTrack* t, Int_t mask)
  {
    // Check is track-flag specified by mask are set.

    return (t->GetStatus() & mask) > 0;
  }

  void LoadEsdTracks(Int_t maxR)
  {
    // Read reconstructed tracks from current event.

    if (fTrackList == 0)
      {
	fTrackList = new TEveTrackList("ESD Tracks"); 
	fTrackList->SetMainColor(40);
	fTrackList->SetMarkerColor(kYellow);
	fTrackList->SetMarkerStyle(4);
	fTrackList->SetMarkerSize(0.5);

	fTrackList->IncDenyDestroy();
      }
    else
      {
	fTrackList->DestroyElements();
      }

    TEveTrackPropagator* trkProp = fTrackList->GetPropagator();
    trkProp->SetMagField(0.5);
    trkProp->SetMaxR(maxR); //R[cm]

    Int_t nTracks = fVSD->fTreeR->GetEntries();

    fVSD->fTreeR->GetEntry(0);
    Int_t nCascades = fVSD->fR.fIndex;

    nCascadeTracks = nCascades;

    Int_t n = 0;

    for (Int_t i = nCascades; i < nTracks; ++i)
      {
	fVSD->fTreeR->GetEntry(n);
	TEveTrack* track = new TEveTrack(&fVSD->fR, trkProp);
	track->SetName("ESD_Track");
	track->SetTitle("ESD_Track");
	track->SetAttLineAttMarker(fTrackList);
	fTrackList->AddElement(track);

	n++;

      }

    fTrackList->MakeTracks();
    gEve->AddElement(fTrackList);
  }

	//--------------------------------------------------------------
  void LoadV0s()
  {

//if( !nCascadeTracks)
//{
    if(fVSD->fTreeV0)
      {

	TEveRecTrack  rcPos;
	TEveRecTrack  rcNeg;
	TEveRecTrack  rcMother;
	TEveRecTrack  rcPointing;
	Int_t pdg;
	Double_t massN, massP, momentum;

	if (fTrackListV0Neg == 0)
	  {
	    fTrackListV0Neg = new TEveTrackList("V0 Tracks Neg"); 
	    fTrackListV0Neg->SetMainColor(kGreen-4);
	    fTrackListV0Neg->SetMarkerColor(kYellow);
	    fTrackListV0Neg->SetMarkerStyle(10);
	    fTrackListV0Neg->SetMarkerSize(5);

	    fTrackListV0Neg->IncDenyDestroy();
	  }
	else
	  {
	    fTrackListV0Neg->DestroyElements();
	  }

	if (fTrackListV0Pos == 0)
	  {
	    fTrackListV0Pos = new TEveTrackList("V0 Tracks Pos"); 
	    fTrackListV0Pos->SetMainColor(2);
	    fTrackListV0Pos->SetMarkerColor(kYellow);
	    fTrackListV0Pos->SetMarkerStyle(10);
	    fTrackListV0Pos->SetMarkerSize(5);

	    fTrackListV0Pos->IncDenyDestroy();
	  }
	else
	  {
	    fTrackListV0Pos->DestroyElements();
	  }

	if (fTrackListV0Mother == 0)
	  {
	    fTrackListV0Mother = new TEveTrackList("V0 Mother Tracks"); 
	    fTrackListV0Mother->SetMainColor(kSpring+6);
	    fTrackListV0Mother->SetMarkerColor(kSpring+6);
	    fTrackListV0Mother->SetMarkerStyle(10);
	    fTrackListV0Mother->SetMarkerSize(5);

	    fTrackListV0Mother->IncDenyDestroy();
	  }
	else
	  {
	    fTrackListV0Mother->DestroyElements();
	  }

	if (fPsV0 == 0)
	  {
	    fPsV0 = new TEvePointSet();
	    fPsV0->SetMainColor(14);
	    fPsV0->SetMarkerSize(0.5);
	    fPsV0->SetMarkerStyle(2);
	    fPsV0->IncDenyDestroy();
	  }
	else
	  {
	    fPsV0->Reset();
	  }

	TEveTrackPropagator* trkProp = fTrackListV0Neg->GetPropagator();
	trkProp->SetMagField(0.5);
	trkProp->SetMaxR(600); //R[cm]

	TEveTrackPropagator* trkProp2 = fTrackListV0Mother->GetPropagator();
	trkProp2->SetMagField(0.5);

	Int_t nV0s = fVSD->fTreeV0->GetEntries();
	nV0Tracks = nV0s;

	fVSD->fTreeR->GetEntry(0);
	Int_t nCascades = fVSD->fR.fIndex;//fVSD->fTreeR->GetEntry(0);001-0.985750-4.389611

	nCascadeTracks = nCascades;

	Int_t n = 0;

	TEvePointSelector ss(fVSD->fTreeV0, fPsV0, "fVCa.fX:fVCa.fY:fVCa.fZ");
	ss.Select();
	fPsV0->SetTitle("V0_CA_Points");

	for(Int_t i=nCascades; i<nV0s; i++)
	  {

	    fVSD->fTreeV0->GetEntry(n);

	    pdg = fVSD->fV0.fPdg;

	    switch (pdg)
	      {
	      case 0:
		massN = 0.000510;
		massP = 0.000510;
		break;
	      case 310:
		massN = 0.13957018;
		massP = 0.13957018;
		break;
	      case 3122:
		massN = 0.13957018;
		massP = 0.938272;
		break;
	      case -3122:
		massN = 0.938272;
		massP = 0.13957018;
		break;
	      default:
		massN = 0.13957018;
		massP = 0.13957018;
		break;
	      }

	    rcNeg.fP.Set(fVSD->fV0.fPNeg);
	    rcNeg.fV.Set(fVSD->fV0.fVNeg);
	    rcNeg.fStatus = fVSD->fV0.fStatus;
	    rcNeg.fLabel = fVSD->fV0.fDLabel[0];
	    momentum = fVSD->fV0.fPNeg.Mag();
	    rcNeg.fBeta = momentum/TMath::Sqrt(momentum*momentum + TMath::C()*TMath::C()*massN*massN);
	    rcNeg.fSign = -1;

	    rcPos.fP.Set(fVSD->fV0.fPPos);
	    rcPos.fV.Set(fVSD->fV0.fVPos);
	    rcPos.fStatus = fVSD->fV0.fStatus;
	    rcPos.fLabel = fVSD->fV0.fDLabel[1];
	    momentum = fVSD->fV0.fPPos.Mag();
	    rcPos.fBeta = momentum/TMath::Sqrt(momentum*momentum + TMath::C()*TMath::C()*massP*massP);
	    rcPos.fSign = 1;

	    TEveTrack* track1 = new TEveTrack(&rcNeg, trkProp);
	    track1->SetName(TString::Format("V0_Track_Neg_%d", n));
	    track1->SetAttLineAttMarker(fTrackListV0Neg);

	    pxNeg[n]=track1->GetMomentum().fX;
	    pyNeg[n]=track1->GetMomentum().fY;
	    pzNeg[n]=track1->GetMomentum().fZ;
	    massNeg[n]=massN;

	    TEveTrack* track2 = new TEveTrack(&rcPos, trkProp);
	    track2->SetName(TString::Format("V0_Track_Pos_%d", n));
	    track2->SetAttLineAttMarker(fTrackListV0Pos);

	    pxPos[n]=track2->GetMomentum().fX;
	    pyPos[n]=track2->GetMomentum().fY;
	    pzPos[n]=track2->GetMomentum().fZ;
	    massPos[n]=massP;

	    pcaX[n] = fVSD->fV0.fVCa.fX;
	    pcaY[n] = fVSD->fV0.fVCa.fY;
	    pcaZ[n] = fVSD->fV0.fVCa.fZ;

	    Double_t momentum = TMath::Sqrt((pxNeg[n]+pxPos[n])*(pxNeg[n]+pxPos[n])
					    +(pyNeg[n]+pyPos[n])*(pyNeg[n]+pyPos[n])+(pyNeg[n]+pyPos[n])*(pyNeg[n]+pyPos[n]));
	    Double_t direction[3] = {(pxNeg[n]+pxPos[n])/momentum, (pyNeg[n]+pyPos[n])/momentum, (pzNeg[n]+pzPos[n])/momentum};

	    pointingLine[n] = new TEveLine(TString::Format("V0_Pointing_Line_%d", n));
	    pointingLine[n]->SetPoint(0, 0.0, pcaY[n]-pcaX[n]*direction[1]/direction[0], pcaZ[n]-pcaX[n]*direction[2]/direction[0]);
	    pointingLine[n]->SetPoint(1, pcaX[n], pcaY[n], pcaZ[n]);
	    pointingLine[n]->SetLineColor(kSpring+6);
	    pointingLine[n]->SetLineWidth(2);
	    pointingLine[n]->SetLineStyle(2);
	    gEve->AddElement(pointingLine[n]);

	    fTrackListV0Pos->AddElement(track2);
	    fTrackListV0Neg->AddElement(track1);

	    n++;

	  }

	fTrackListV0Pos->MakeTracks();
	fTrackListV0Neg->MakeTracks();

	gEve->AddElement(fPsV0);
	gEve->AddElement(fTrackListV0Pos);
	gEve->AddElement(fTrackListV0Neg);

	TEveElement* top = gEve->GetCurrentEvent();

	if(gMultiView)
	  {
	    gMultiView->DestroyEventRPhi();
	    gMultiView->ImportEventRPhi(top);

	    gMultiView->DestroyEventRhoZ();
	    gMultiView->ImportEventRhoZ(top);
	  }

	gEve->Redraw3D(kFALSE, kTRUE);

      }
//}

  }

	//--------------------------------------------------------------
  void LoadCascades()
  {

    if(fVSD->fTreeV0)
      {

	TEveRecTrack  rcPos;
	TEveRecTrack  rcNeg;
	TEveRecTrack  rcMother;
	Int_t pdg;
	Double_t massN, massP, momentum;

	if (fTrackListCascadeNeg == 0)
	  {
	    fTrackListCascadeNeg = new TEveTrackList("Cascade Tracks Neg"); 
	    fTrackListCascadeNeg->SetMainColor(kGreen-4);
	    fTrackListCascadeNeg->SetMarkerColor(kYellow);
	    fTrackListCascadeNeg->SetMarkerStyle(10);
	    fTrackListCascadeNeg->SetMarkerSize(5);

	    fTrackListCascadeNeg->IncDenyDestroy();
	  }
	else
	  {
	    fTrackListCascadeNeg->DestroyElements();
	  }

	if (fTrackListCascadePos == 0)
	  {
	    fTrackListCascadePos = new TEveTrackList("Cascade Tracks Pos"); 
	    fTrackListCascadePos->SetMainColor(2);
	    fTrackListCascadePos->SetMarkerColor(kYellow);
	    fTrackListCascadePos->SetMarkerStyle(10);
	    fTrackListCascadePos->SetMarkerSize(5);

	    fTrackListCascadePos->IncDenyDestroy();
	  }
	else
	  {
	    fTrackListCascadePos->DestroyElements();
	  }

	if (fTrackListCascadeMother == 0)
	  {
	    fTrackListCascadeMother = new TEveTrackList("Cascade Mother Tracks"); 
	    fTrackListCascadeMother->SetMainColor(kSpring+6);
	    fTrackListCascadeMother->SetMarkerColor(kSpring+6);
	    fTrackListCascadeMother->SetMarkerStyle(10);
	    fTrackListCascadeMother->SetMarkerSize(5);

	    fTrackListCascadeMother->IncDenyDestroy();
	  }
	else
	  {
	    fTrackListCascadeNeg->DestroyElements();
	  }

	if (fPsCascade == 0)
	  {
	    fPsCascade = new TEvePointSet();
	    fPsCascade->SetMainColor(1);
	    fPsCascade->SetMarkerSize(2);
	    fPsCascade->SetMarkerStyle(2);
	    fPsCascade->IncDenyDestroy();
	  }
	else
	  {
	    fPsCascade->Reset();
	  }

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
	  {
	    fTrackListCascade->DestroyElements();
	  }

	TEveTrackPropagator* trkProp = fTrackListCascadeNeg->GetPropagator();
	trkProp->SetMagField(0.5);
	trkProp->SetMaxR(600); //R[cm]

	TEveTrackPropagator* trkProp2 = fTrackListCascadeMother->GetPropagator();
	trkProp2->SetMagField(0.5);

	TEveTrackPropagator* trkProp3 = fTrackListCascade->GetPropagator();
	// !!!! Need to store field on file !!!!
	// Can store TEveMagField ?
	trkProp3->SetMagField(0.5);
	trkProp3->SetMaxR(400); //R[cm]

	fVSD->fTreeR->GetEntry(0);
	Int_t nCascades = fVSD->fR.fIndex;

	nCascadeTracks = nCascades;

	TEvePointSelector ss(fVSD->fTreeV0, fPsCascade, "fVCa.fX:fVCa.fY:fVCa.fZ");
	ss.Select();
	fPsCascade->SetTitle("Cascade_CA_Points");

	Int_t n = 0;

	for(Int_t i=0; i<nCascades; i++)
	  {

	    fVSD->fTreeV0->GetEntry(n);

	    pdg = fVSD->fV0.fPdg;

	    switch (pdg)
	      {
	      case 3312:
		massN = 0.13957018;
		massP = 0.938272;
		break;
	      case -3312:
		massN = 0.938272;
		massP = 0.13957018;
		break;
		/*
		  case -3332:
		  massN = 0.938272;
		  massP = 0.13957018;
		  break;
		  case 3332:
		  massN = 0.13957018;
		  massP = 0.938272;
		  break;
		*/
	      default:
		massN = 0.13957018;
		massP = 0.938272;
		break;
	      }

	    rcNeg.fP.Set(fVSD->fV0.fPNeg);
	    rcNeg.fV.Set(fVSD->fV0.fVNeg);
	    rcNeg.fStatus = fVSD->fV0.fStatus;
	    rcNeg.fLabel = fVSD->fV0.fDLabel[0];
	    momentum = fVSD->fV0.fPNeg.Mag();
	    rcNeg.fBeta = momentum/TMath::Sqrt(momentum*momentum + TMath::C()*TMath::C()*massN*massN);
	    rcNeg.fSign = -1;

	    rcPos.fP.Set(fVSD->fV0.fPPos);
	    rcPos.fV.Set(fVSD->fV0.fVPos);
	    rcPos.fStatus = fVSD->fV0.fStatus;
	    rcPos.fLabel = fVSD->fV0.fDLabel[1];
	    momentum = fVSD->fV0.fPPos.Mag();
	    rcPos.fBeta = momentum/TMath::Sqrt(momentum*momentum + TMath::C()*TMath::C()*massP*massP);
	    rcPos.fSign = 1;

	    rcMother.fP.Set(fVSD->fV0.fPNeg+fVSD->fV0.fPPos);
	    rcMother.fV.Set(0.0,0.0,0.0);
	    rcMother.fStatus = fVSD->fV0.fStatus;
	    rcMother.fLabel = fVSD->fV0.fLabel;
	    rcMother.fBeta = 0.5;
	    rcMother.fSign = 0;

	    TEveTrack* track1 = new TEveTrack(&rcNeg, trkProp);
	    track1->SetName(TString::Format("Cascade_Track_Neg_%d", n));
	    track1->SetAttLineAttMarker(fTrackListCascadeNeg);

	    pxNegCas[n]=track1->GetMomentum().fX;
	    pyNegCas[n]=track1->GetMomentum().fY;
	    pzNegCas[n]=track1->GetMomentum().fZ;

	    massNegCas[n]=massN;

	    TEveTrack* track2 = new TEveTrack(&rcPos, trkProp);
	    track2->SetName(TString::Format("Cascade_Track_Pos_%d", n));

	    track2->SetAttLineAttMarker(fTrackListCascadePos);

	    pxPosCas[n]=track2->GetMomentum().fX;
	    pyPosCas[n]=track2->GetMomentum().fY;
	    pzPosCas[n]=track2->GetMomentum().fZ;
	    massPosCas[n]=massP;

	    pcaXCas[n] = fVSD->fV0.fVCa.fX;
	    pcaYCas[n] = fVSD->fV0.fVCa.fY;
	    pcaZCas[n] = fVSD->fV0.fVCa.fZ;         

	    trkProp2->SetMaxR(fVSD->fV0.fVCa.Mag()); //R[cm]

	    TEveTrack* track3 = new TEveTrack(&rcMother, trkProp2);
	    track3->SetName(TString::Format("Cascade_Track_%d", n));

	    fTrackListCascadeMother->AddElement(track3);
	    fTrackListCascadePos->AddElement(track2);
	    fTrackListCascadeNeg->AddElement(track1);

	    fVSD->fTreeR->GetEntry(n);

	    TEveTrack* track = new TEveTrack(&fVSD->fR, trkProp3);
	    track->SetName(Form("Cascade_Bachelor_Track_%d", n));

	    pxBacCas[n]=track->GetMomentum().fX;
	    pyBacCas[n]=track->GetMomentum().fY;
	    pzBacCas[n]=track->GetMomentum().fZ;

	    pcaXBacCas[n]=track->GetVertex().fX;
	    pcaYBacCas[n]=track->GetVertex().fY;
	    pcaZBacCas[n]=track->GetVertex().fZ;

	    massBacCas[n]=0.13957018;

	    if(massBacCas[n]==0.13957018)
	      chargeBacCas[n] = -1;
	    else
	      chargeBacCas[n] = 1;

	    track->SetStdTitle();
	    track->SetAttLineAttMarker(fTrackListCascade);
	    fTrackListCascade->AddElement(track);

	    Double_t momentum = TMath::Sqrt((pxNegCas[n]+pxPosCas[n]+pxBacCas[n])*(pxNegCas[n]+pxPosCas[n]+pxBacCas[n])
					    +(pyNegCas[n]+pyPosCas[n]+pyBacCas[n])*(pyNegCas[n]+pyPosCas[n]+pyBacCas[n])
					    +(pyNegCas[n]+pyPosCas[n]+pzBacCas[n])*(pyNegCas[n]+pyPosCas[n]+pzBacCas[n]));
	    Double_t direction[3] = {(pxNegCas[n]+pxPosCas[n]+pxBacCas[n])/momentum,
				     (pyNegCas[n]+pyPosCas[n]+pyBacCas[n])/momentum,
				     (pzNegCas[n]+pzPosCas[n]+pzBacCas[n])/momentum};

	    pointingLine1[n] = new TEveLine(TString::Format("Cascade_Line_1_%d", n));
	    pointingLine1[n]->SetPoint(0, 0.0, pcaYBacCas[n]-pcaXBacCas[n]*direction[1]/direction[0], pcaZBacCas[n]-pcaXBacCas[n]*direction[2]/direction[0]);
	    pointingLine1[n]->SetPoint(1, pcaXBacCas[n], pcaYBacCas[n], pcaZBacCas[n]);
	    pointingLine1[n]->SetLineColor(kSpring+6);
	    pointingLine1[n]->SetLineWidth(2);
	    pointingLine1[n]->SetLineStyle(2);

	    gEve->AddElement(pointingLine1[n]);

	    pointingLine2[n] = new TEveLine(TString::Format("Cascade_Line_2_%d", n));
	    pointingLine2[n]->SetPoint(0, pcaXBacCas[n], pcaYBacCas[n], pcaZBacCas[n]);
	    pointingLine2[n]->SetPoint(1, pcaXCas[n], pcaYCas[n], pcaZCas[n]);
	    pointingLine2[n]->SetLineColor(kSpring-6);
	    pointingLine2[n]->SetLineWidth(2);
	    pointingLine2[n]->SetLineStyle(3);

	    gEve->AddElement(pointingLine2[n]);

	    n++;

	  }

	fTrackListCascadeMother->MakeTracks();
	fTrackListCascadePos->MakeTracks();
	fTrackListCascadeNeg->MakeTracks();
	fTrackListCascade->MakeTracks();

	gEve->AddElement(fTrackListCascadePos);
	gEve->AddElement(fTrackListCascadeNeg);
	gEve->AddElement(fTrackListCascade);

	TEveElement* top = gEve->GetCurrentEvent();

	gMultiView->DestroyEventRPhi();
	gMultiView->ImportEventRPhi(top);

	gMultiView->DestroyEventRhoZ();
	gMultiView->ImportEventRhoZ(top);

	gEve->Redraw3D(kFALSE, kTRUE);

      }

  }
 
  ClassDef(TVSDReader, 0);
};

TVSDReader* gVSDReader = 0;
/*
  class TVSDModeSwitch {
  public:

  const char* file1;
  const char* file2;

  TVSDModeSwitch()
  {  

  file1 = "eve_files/data/AliVSD_example.root";
  file2 = "eve_files/data/AliVSD.root";
  }

  ~TVSDModeSwitch(){}

  void DemoToStudent()
  {

  gVSDReader = new TVSDReader(file2);
  gVSDReader->fTimer = new TTimer();
  gVSDReader->fTimer->Connect("Timeout()", "TVSDReader", gVSDReader, "AnimateEsdTracks()");

  gVSDReader->GotoEvent(0);

  gEve->Redraw3D(kTRUE);

  }

  void StudentToDemo()
  {

  gVSDReader = new TVSDReader(file1);

  gVSDReader->fTimer = new TTimer();
  gVSDReader->fTimer->Connect("Timeout()", "TVSDReader", gVSDReader, "AnimateEsdTracks()");
  gVSDReader->GotoEvent(0);

  gEve->Redraw3D(kTRUE);

  }

  ClassDef(TVSDModeSwitch, 0);

  };

  TVSDModeSwitch* gVSDModeSwitch = 0;
*/

// Forward declaration.
//void make_gui(Int_t mode);

//______________________________________________________________________________
void alice_vsd(Int_t choice, Int_t mode, Int_t dataset)
{

  TString vsd_file_name;
  
  vsd_file_name = Form("AliVSD_MasterClass_%i.root", dataset);
  vsd_file_name.Prepend(gSystem->Getenv("PART")); 
  vsd_file_name.Prepend(gSystem->Getenv("EVTPATH")); 

 // vsd_file_name = "AliVSD.root";

  globalChoice = choice;
  globalMode = mode;
  globalDataset = dataset;

  // Main function, initializes the application.
  //
  // 1. Load the auto-generated library holding ESD classes and ESD dictionaries.
  // 2. Open ESD data-files.
  // 3. Load cartoon geometry.
  // 4. Spawn simple GUI.
  // 5. Load first event.

  TFile::SetCacheFileDir(".");

  TEveVSD::DisableTObjectStreamersForVSDStruct();

  TEveManager::Create(kTRUE,"FV");

  TEveGeoShape *gentle_geom = 0;
/*
  TFile* geom = TFile::Open(gMiniGeom);

  //   gAliceCalculator = new AliceCalculator(gClient->GetRoot(), 400, 200);

  if (!geom)
    return;
  TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
  gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
  geom->Close();
  delete geom;

  gEve->AddGlobalElement(gentle_geom);

    TEveSceneList* sceneList1=gEve->GetScenes();
    TEveElement::List_i i = sceneList1->BeginChildren();
    TEveSceneList* sceneList2 = (TEveSceneList*) *i;
    sceneList2->RemoveElements();
    i++;
    i++;
    TEveSceneList* sceneList3 = (TEveSceneList*) *i;
    sceneList3->RemoveElements();
    i++;
    TEveSceneList* sceneList4 = (TEveSceneList*) *i;
    sceneList4->RemoveElements();

    TEveGeoShape *gentle_geom = 0;
*/
    TFile* geom = TFile::Open(gMiniGeom);
    if (!geom)
      return;
    TEveGeoShapeExtract* gse = (TEveGeoShapeExtract*) geom->Get("Gentle");
    gentle_geom = TEveGeoShape::ImportShapeExtract(gse, 0);
    geom->Close();
    delete geom;

    gentle_geom->SetRnrSelf(kFALSE);

    //ITS

    TEveElement::List_i i = gentle_geom->BeginChildren();

    TEveGeoShape* lvl1 = (TEveGeoShape*) *i;

    //TPC

    i++;

    //TRD+TOF

    i++;

    lvl1 = (TEveGeoShape*) *i;
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

  // Standard multi-view
  //=====================

  gMultiView = new MultiView();
  gMultiView->f3DView->GetGLViewer()->SetStyle(TGLRnrCtx::kOutline);
  gMultiView->f3DView->GetGLViewer()->SetStyle(0);
  gMultiView->SetDepth(-10);
  gMultiView->ImportGeomRPhi(gentle_geom);
  gMultiView->ImportGeomRhoZ(gentle_geom);
  gMultiView->SetDepth(0);

  // Final stuff
  //=============

  gEve->GetViewers()->SwitchColorSet();
  gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);

  //   make_gui(mode);

  gVSDReader = new TVSDReader(vsd_file_name);

  gVSDReader->fTimer = new TTimer();
  gVSDReader->fTimer->Connect("Timeout()", "TVSDReader", gVSDReader, "AnimateEsdTracks()");

  TEveBrowser* browser = gEve->GetBrowser();

  browser->GetTabLeft()->RemoveTab(1);
  browser->GetTabLeft()->RemoveTab(0);
 
  //   browser->StartEmbedding(TRootBrowser::kRight);

  gStyle->SetOptStat(110);
  gStyle->SetCanvasColor(0);
  gStyle->SetPalette(1);

  //   TCanvas* pad = new TCanvas();
  //   pad->Divide(2, 2);

  //   pad->cd(1);
  gKMinvHist = new TH1D("Statistics","Kaons",50,0.4,0.6);
  gKMinvHist->SetLineColor(2);
  gKMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gKMinvHist->GetYaxis()->SetTitle("Counts");
  gKMinvHist->SetFillColor(0);
  //   gKMinvHist->Draw();

  //   pad->cd(2);
  gXMinvHist = new TH1D("Statistics","Xis",50,1.2,1.4);
  gXMinvHist->SetLineColor(3);
  gXMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gXMinvHist->GetYaxis()->SetTitle("Counts");
  gXMinvHist->SetFillColor(0);
  //   gXMinvHist->Draw();

  //   pad->cd(3);
  gLMinvHist = new TH1D("Statistics","Lambdas",50,1.0,1.2);
  gLMinvHist->SetLineColor(4);
  gLMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gLMinvHist->GetYaxis()->SetTitle("Counts");
  gLMinvHist->SetFillColor(0);
  //   gLMinvHist->Draw();

  //   pad->cd(4);
  gALMinvHist = new TH1D("Statistics","Anti-Lambdas",50,1.0,1.2);
  gALMinvHist->SetLineColor(9);
  gALMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gALMinvHist->GetYaxis()->SetTitle("Counts");
  gALMinvHist->SetFillColor(0);
  //   gALMinvHist->Draw();

  //   browser->StopEmbedding("Invariant Mass");

  //   browser->StartEmbedding(TRootBrowser::kRight);
   
  //   pad = new TCanvas();
  //   pad->Divide(2,2);

  //   pad->cd(1);
  gKRapHist = new TH1D("Statistics","Kaons",20,-1.5,1.5);
  gKRapHist->SetLineColor(2);
  gKRapHist->GetXaxis()->SetTitle("Rapidity");
  gKRapHist->GetYaxis()->SetTitle("Counts");
  gKRapHist->SetFillColor(0);
  //   gKRapHist->Draw();

  //   pad->cd(2);
  gXRapHist = new TH1D("Statistics","Xis",20,-1.5,1.5);
  gXRapHist->SetLineColor(3);
  gXRapHist->GetXaxis()->SetTitle("Rapidity");
  gXRapHist->GetYaxis()->SetTitle("Counts");
  gXRapHist->SetFillColor(0);
  //   gXRapHist->Draw();

  //   pad->cd(3);
  gLRapHist = new TH1D("Statistics","Lambdas",20,-1.5,1.5);
  gLRapHist->SetLineColor(4);
  gLRapHist->GetXaxis()->SetTitle("Rapidity");
  gLRapHist->GetYaxis()->SetTitle("Counts");
  gLRapHist->SetFillColor(0);
  //   gLRapHist->Draw();

  //   pad->cd(4);
  gALRapHist = new TH1D("Statistics","Anti-Lambdas",20,-1.5,1.5);
  gALRapHist->SetLineColor(9);
  gALRapHist->GetXaxis()->SetTitle("Rapidity");
  gALRapHist->GetYaxis()->SetTitle("Counts");
  gALRapHist->SetFillColor(0);
  //   gALRapHist->Draw();

  //   browser->StopEmbedding("Rapidity");

  //   pad->Draw();

  gAllMinvHist = new TH1D("Statistics","Invariant Mass - final result",20,0.0,2.0);
  gAllMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
  gAllMinvHist->GetYaxis()->SetTitle("Counts");
  gAllMinvHist->SetFillColor(0);

  gAllRapHist = new TH1D("Statistics","Rapidity - final result",20,-1.5,1.5);
  gAllRapHist->GetXaxis()->SetTitle("Rapidity");
  gAllRapHist->GetYaxis()->SetTitle("Counts");
  gAllRapHist->SetFillColor(0);

  gSigTestSpitter = new SigTestSpitter(gEve->GetSelection(), "Selection");

  gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

  if(mode == 3)
    {
      teacher = kTRUE;
      gVSDReader->TeacherSet();
    }
  else
    {
      gVSDReader->StudentSet();
      gVSDReader->GotoEvent(0);
    }

  gEve->GetWindowManager()->HideAllEveDecorations();

  gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.

}


//______________________________________________________________________________

/*
  void make_gui(Int_t mode)
  {
  // Create minimal GUI for event navigation.

  TEveBrowser* browser = gEve->GetBrowser();
  browser->StartEmbedding(TRootBrowser::kLeft);

  TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
  frmMain->SetWindowName("XX GUI");
  frmMain->SetCleanup(kDeepCleanup);

  TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
  TGPictureButton* b1 = 0;
  TGTextButton* b = 0;
  TGLabel* label = 0;
  TGGroupFrame* gf = 0;
  TGVerticalFrame* vf = 0;
  TGHorizontalFrame* hf = 0;
  ULong_t buttonRedColor, buttonBlueColor;

  gf = new TGGroupFrame(frmMain, "Student Instructions");

  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

  {
  const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
  FontStruct_t buttonFont = font->GetFontStruct();
  gClient->GetColorByName("red", buttonRedColor);
  b = new TGTextButton(vf,"Instructions");
  b->SetTextColor(buttonRedColor);
  b->SetFont(buttonFont);
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "Instructions()");

  b = new TGTextButton(vf,"Back To Demo");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "ToDemo()");

  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  //------------------------------------------------------

  gf = new TGGroupFrame(frmMain, "Event Navigation");

  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
  {

  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
  {

  label = new TGLabel(hf, "Previous");
  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  label = new TGLabel(hf, "Current");
  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  label = new TGLabel(hf, "Next");
  hf->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  }

  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
  {

  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoBack.gif"));
  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b1->Connect("Clicked()", "TVSDReader", gVSDReader, "PrevEvent()");

  gEventNumber = new TGLabel(hf);
  gEventNumber->SetText(1);
  hf->AddFrame(gEventNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "GoForward.gif"));
  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b1->Connect("Clicked()", "TVSDReader", gVSDReader, "NextEvent()");
  }


  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  gf = new TGGroupFrame(frmMain, "Display Event");

  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);

  {

  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);

  b = new TGTextButton(hf,"Vertex");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DisplayVertex()");
  gCheckVertex = new TGCheckButton(hf, "", 10);
  gCheckVertex->SetEnabled(kFALSE);

  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
  hf->AddFrame(gCheckVertex);

  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);

  b = new TGTextButton(hf,"Clusters");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DisplayClusters()");
  gCheckClusters = new TGCheckButton(hf, "", 10);
  gCheckClusters->SetEnabled(kFALSE);

  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
  hf->AddFrame(gCheckClusters);

  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
      
  b = new TGTextButton(hf, "Tracks");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DisplayEsdTracks()");
  gCheckESDTracks = new TGCheckButton(hf, "", 10);
  gCheckESDTracks->SetEnabled(kFALSE);

  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
  hf->AddFrame(gCheckESDTracks);

  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
      
  b = new TGTextButton(hf, "V0s");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DisplayV0s()");
  gCheckV0s = new TGCheckButton(hf, "", 10);
  gCheckV0s->SetEnabled(kFALSE);

  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
  hf->AddFrame(gCheckV0s);

  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  hf = new TGHorizontalFrame(vf, 450, 20, kFixedWidth);
      
  b = new TGTextButton(hf, "Cascades");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DisplayCascades()");
  gCheckCascades = new TGCheckButton(hf, "", 10);
  gCheckCascades->SetEnabled(kFALSE);

  hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
  hf->AddFrame(gCheckCascades);

  vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  //   frmMain->AddFrame(gf);

  gf = new TGGroupFrame(frmMain, "Calculator");

  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
  {

  const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
  FontStruct_t buttonFont = font->GetFontStruct();
  gClient->GetColorByName("blue", buttonBlueColor);
  b = new TGTextButton(vf, "Calculator");
  b->SetTextColor(buttonBlueColor);
  b->SetFont(buttonFont);
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "Calculator()");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  gf = new TGGroupFrame(frmMain, "Set Geometry");

  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
  {

  b = new TGTextButton(vf, "Geometry");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "ChangeGeometry()");

  b = new TGTextButton(vf, "Background");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "ChangeBackgroundColor()");

  b = new TGTextButton(vf, "Axes");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DisplayGuides()");

  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  gf = new TGGroupFrame(frmMain, "Animate event");

  hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
  {

  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "ed_execute.png"));
  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b1->Connect("Clicked()", "TVSDReader", gVSDReader, "StartAnimation()");

  b1 = new TGPictureButton(hf, gClient->GetPicture(icondir + "ed_interrupt.png"));
  hf->AddFrame(b1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b1->Connect("Clicked()", "TVSDReader", gVSDReader, "StopAnimation()");

  }

  gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  gf = new TGGroupFrame(frmMain, "Others");

  vf = new TGVerticalFrame(gf, 150, 150, kFixedWidth);
  {

  b = new TGTextButton(vf, "ALICE Detector");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "DetectorInfo()");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  b = new TGTextButton(vf, "V0 Patterns");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "PatternInfo()");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  gf = new TGGroupFrame(frmMain, "");

  vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
  {

  if(mode == 3)
  {
  b = new TGTextButton(vf, "Teacher Mode");
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "TeacherSet()");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  }

  b = new TGTextButton(vf, "Exit");
  vf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
  b->Connect("Clicked()", "TVSDReader", gVSDReader, "Exit()");

  }

  gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

  frmMain->AddFrame(gf);

  vf = new TGVerticalFrame(frmMain, 150, 150, kFixedWidth);
  {

  b1 = new TGPictureButton(vf, gClient->GetPicture("eve_files/ALICE_logo_scaled.png"));

  }

  vf->AddFrame(b1);
  frmMain->AddFrame(vf);

  frmMain->MapSubwindows();
  frmMain->Resize();
  frmMain->MapWindow();

  browser->StopEmbedding("Student");

  }
*/

#include "MCAliCounter.h"
#include "MCExerciseR1.h"
#include "MCMultiLingual.h"
#include "MCMultiView.h"
#include "MCVSDReader.h"

#include <iostream>

#include <TCanvas.h>
#include <TEveBrowser.h>
#include <TEveElement.h>
#include <TEveEventManager.h>
#include <TEveManager.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveVSD.h>
#include <TEveViewer.h>
#include <TEveWindowManager.h>
#include <TGComboBox.h>
#include <TGImageMap.h>
#include <TGLabel.h>
#include <TGMsgBox.h>
#include <TGLOverlayButton.h>
#include <TGTab.h>
#include <TGLViewer.h>
#include <TH1D.h>
#include <TRootHelpDialog.h>
#include <TStyle.h>
#include <TSystem.h>

//=MCExerciseR1========================================================================
MCExerciseR1::MCExerciseR1(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
	fDataSetFileDir = "Data-Masterclass/events/RAA/Part1/";
	fDataTemplate = "AliVSD_MasterClass_x.root";
	fGuiLogoName = "images/RAA.png";
	MakeGui();
	fTutor->SetEnabled(kFALSE);
}

//=MCExerciseR1========================================================================
MCExerciseR1::~MCExerciseR1()
{
	if (!fNeedsInit)
		DeleteHistograms();
	if (fInstructions)
    	fInstructions->CloseWindow(); 
}

//=MCExerciseR1========================================================================
void MCExerciseR1::Counter()
{
	if (fCollSys == 1)
		new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseS1:Data", MCMultiLingual::Instance().tr("You do not need to count tracks"), kMBIconExclamation, kMBOk);
	else
		MCAliCounter::Instance().Start(this, fCollSys);
}

//=MCExerciseR1========================================================================
void MCExerciseR1::CountEvents()
{
	if (fCollSys == 2)
		PublishMult();
	fEventAnalysed->SetText(TString::Format("%i", fCount + 1));
	fCount++;
}

//=MCExerciseR1========================================================================
void MCExerciseR1::CustomizeBrowser()
{
	MCMultiLingual &ml = MCMultiLingual::Instance();
	TEveBrowser *browser = gEve->GetBrowser();
	TString text;
	text = ml.tr("Student");
	browser->GetMainFrame()->SetWindowName(text);
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
		btext->Connect("Clicked()", "MCExerciseR1", this, "Instructions()");
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
			bpict->Connect("Clicked()", "MCExerciseR1", this, "PrevEvent()");
			fEventNumber = new TGLabel(horiF);
			fEventNumber->SetText("hello");
			horiF->AddFrame(fEventNumber, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

			bpict = new TGPictureButton(horiF, gClient->GetPicture(icondir + "GoForward.gif"));
			horiF->AddFrame(bpict, new TGLayoutHints(kLHintsLeft, 1, 1, 1, 1));
			bpict->Connect("Clicked()", "MCExerciseR1", this, "NextEvent()");
		}
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
		{
			btext = new TGTextButton(horiF, ml.tr("Event analysed!"));
			btext->Connect("Clicked()", "MCExerciseR1", this, "CountEvents()");
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

	groupF = new TGGroupFrame(fSMF, ml.tr("Analysis tool"));
	vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
	{
		horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
		{
			fDataSetLabel = new TGLabel(horiF);
			fDataSetLabel->SetText("pp, 7 TeV, B=0 T");
			horiF->AddFrame(fDataSetLabel, new TGLayoutHints(kLHintsExpandX));
		}
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
		horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
		{
			btext = new TGTextButton(horiF, ml.tr("Counter"));
			horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			btext->Connect("Clicked()", "MCExerciseR1", this, "Counter()");
		}
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
	}
	groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
	fSMF->AddFrame(groupF);

	groupF = new TGGroupFrame(fSMF, ml.tr("Display"));
	vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
	{
		horiF = new TGHorizontalFrame(vertF, 200, 20, kFixedWidth);
		btext = new TGTextButton(horiF, "Vertex");
		btext->Connect("Clicked()", "MCExerciseR1", this, "DisplayVertex()");
		fCheckVertex = new TGCheckButton(horiF, "", 10);
		fCheckVertex->SetEnabled(kFALSE);
		horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
		horiF->AddFrame(fCheckVertex);
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
		btext = new TGTextButton(horiF, ml.tr("Clusters"));
		btext->Connect("Clicked()", "MCExerciseR1", this, "DisplayClusters()");
		fCheckClusters = new TGCheckButton(horiF, "", 10);
		fCheckClusters->SetEnabled(kFALSE);
		horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
		horiF->AddFrame(fCheckClusters);
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);

		btext = new TGTextButton(horiF, ml.tr("Tracks"));
		btext->Connect("Clicked()", "MCExerciseR1", this, "DisplayTracks()");
		fCheckESDTracks = new TGCheckButton(horiF, "", 10);
		fCheckESDTracks->SetEnabled(kFALSE);
		horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX));
		horiF->AddFrame(fCheckESDTracks);
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
		fPrimaryTracksButton = new TGTextButton(horiF, ml.tr("Show primary tracks only"));
		fPrimaryTracksButton->Connect("Clicked()", "MCExerciseR1", this, "DisplayPrimaryTracks()");
		horiF->AddFrame(fPrimaryTracksButton, new TGLayoutHints(kLHintsExpandX));
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

		horiF = new TGHorizontalFrame(vertF, 450, 20, kFixedWidth);
		btext = new TGTextButton(horiF, ml.tr("Geometry"));
		horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		btext->Connect("Clicked()", "MCExerciseR1", this, "ChangeGeometry()");
		btext = new TGTextButton(horiF, "Axes");
		horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		btext->Connect("Clicked()", "MCExerciseR1", this, "DisplayAxes()");
		vertF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		btext = new TGTextButton(vertF, ml.tr("Screen Background"));
		vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		btext->Connect("Clicked()", "MCExerciseR1", this, "ChangeBackgroundColor()");
	}
	groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
	fSMF->AddFrame(groupF);

	groupF = new TGGroupFrame(fSMF, ml.tr("Encyclopaedia"));
	vertF = new TGVerticalFrame(groupF, 200, 20, kFixedWidth);
	{
		btext = new TGTextButton(vertF, ml.tr("ALICE Detector"));
		btext->Connect("Clicked()", "MCExerciseR1", this, "DetectorInfo()");
		vertF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
	}
	groupF->AddFrame(vertF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
	fSMF->AddFrame(groupF);

	fSMF->MapSubwindows();
	fSMF->Resize();
	fSMF->MapWindow();
	browser->StopEmbedding(ml.tr("Student"));

	browser->StartEmbedding(TRootBrowser::kRight);
	fPad = new TCanvas();
	fPad->Divide(2, 2);
	fPad->cd(1);
	fMultHist->Draw();
	fPad->cd(2);
	fMultHistMinPt->Draw();
	fPad->cd(3);
	fPtHist->Draw();
	fPad->cd(4);
	fChargeHist->Draw();
	browser->StopEmbedding(ml.tr("Event Characteristics"));
	browser->HideBottomTab();
	browser->GetTabRight()->SetTab(1);
}

//=MCExerciseR1========================================================================
void MCExerciseR1::DeleteHistograms()
{
	delete fMultHist;
	delete fMultHistMinPt;
	delete fPtHist;
	delete fChargeHist;
}

//=MCExerciseR1========================================================================
void MCExerciseR1::DisplayPrimaryTracks()
{
	if (fPrimaryTracksLoaded)
	{
		fPrimaryTracksLoaded = kFALSE;
		fPrimaryTracksButton->SetState(kButtonUp);
	}
	else
	{
		fPrimaryTracksLoaded = kTRUE;
		fPrimaryTracksButton->SetState(kButtonDown);
	}
	GotoEvent(MCVSDReader::Instance().CurEv());
}

//=MCExerciseR1========================================================================
void MCExerciseR1::EveSelection(TEveElement *el)
{
	MCMultiLingual &ml = MCMultiLingual::Instance();

	if (el->GetElementName() == TString::Format("ESD_Track"))
	{
		TEveTrack *track = (dynamic_cast<TEveTrack *>(el));
		if (MCAliCounter::Instance().IsStarted())
		{
			if (track->GetLineColor() == kRed)
				new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::EveSelection", ml.tr("This track has already been counted"), kMBIconExclamation, kMBOk);
			else
			{
				track->SetLineColor(kRed);
				if (fCollSys == 2)
				{
					MCAliCounter::Instance().Set1(track->GetMomentum().fX, track->GetMomentum().fY, track->GetMomentum().fZ, track->GetCharge());
					fPtHist->Fill(MCAliCounter::Instance().GetPt());
					fChargeHist->Fill(MCAliCounter::Instance().GetCharge());
					fPad->cd(3);
					fPtHist->Draw();
					fPad->cd(3)->Update();
					fPad->cd(4);
					fChargeHist->Draw();
					fPad->cd(4)->Update();
					MCAliCounter::Instance().IncreaseMult();
				}
				else if (fCollSys == 3 || fCollSys == 4 || fCollSys == 5)
					MCAliCounter::Instance().IncreaseMult2(track->GetMomentum().fX, track->GetMomentum().fY);
				else
					MCAliCounter::Instance().Set2(track->GetMomentum().fX, track->GetMomentum().fY, track->GetMomentum().fZ);
			}
		}
		else
			new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::EveSelection", ml.tr("Start the Counter first"), kMBIconExclamation, kMBOk);
	}
	return;
}

//=MCExerciseR1========================================================================
Bool_t MCExerciseR1::GotoEvent(Int_t evt)
{
	MCMultiLingual &ml = MCMultiLingual::Instance();

	MCAliCounter::Instance().SetChanged(kTRUE);
	TString text;
	if (evt == 31)
	{
		MCAliCounter::Instance().DoExit();
		text = "PbPb, peripheral";
		fDataSetLabel->SetText(text);
		text.Prepend(ml.tr("New event type"));
		new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::GotoEvent", text, kMBIconExclamation, kMBOk);
		fCollSys = 3;
		MCAliCounter::Instance().Start(this, fCollSys);
	}
	else if (evt == 32)
	{
		MCAliCounter::Instance().DoExit();
		text = "PbPb, semicentral";
		fDataSetLabel->SetText(text);
		text.Prepend(ml.tr("New event type"));
		new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::GotoEvent", text, kMBIconExclamation, kMBOk);
		fCollSys = 4;
		MCAliCounter::Instance().Start(this, fCollSys);
	}
	else if (evt == 33)
	{
		MCAliCounter::Instance().DoExit();
		text = "PbPb, central";
		fDataSetLabel->SetText(text);
		text.Prepend(ml.tr("New event type"));
		new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::GotoEvent", text, kMBIconExclamation, kMBOk);
		fCollSys = 5;
		MCAliCounter::Instance().Start(this, fCollSys);
	}
	else if (evt == 0)
	{
		fDataSetLabel->SetText("pp, 7 TeV, B=0 T");
		fCollSys = 1;
	}
	else
	{
		text = "pp, 2.76 TeV, B=0.5 T";
		fDataSetLabel->SetText(text);
		text.Prepend(ml.tr("New event type"));
		fCollSys = 2;
		if (!MCAliCounter::Instance().IsStarted())
		{
			text.Prepend(ml.tr("New event type"));
			new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::GotoEvent", text, kMBIconExclamation, kMBOk);
			text.Prepend(ml.tr("New event type"));
			MCAliCounter::Instance().Start(this, fCollSys);
		}
	}
	fTrackList = nullptr;
	Bool_t rv = MCVSDReader::Instance().GotoEvent(evt);
	fEventNumber->SetText(TString::Format("Evt %02i / %s %i",
										  MCVSDReader::Instance().CurEv() + 1,
										  ml.tr("Set"),
										  fDataSet));

	if (fVertexLoaded)
		LoadVertex();
	if (fAxesLoaded)
		LoadAxes();
	if (fITSClustersLoaded)
		LoadClusters(fITSClusters, "ITS", 0);
	if (fTPCClustersLoaded)
		LoadClusters(fTPCClusters, "TPC", 1);
	if (fTRDClustersLoaded)
		LoadClusters(fTRDClusters, "TRD", 2);
	if (fTOFClustersLoaded)
		LoadClusters(fTOFClusters, "TOF", 3);
	if (fEsdTracksLoaded)
		LoadEsdTracks(kfMaxRadius);
	if (fPrimaryTracksLoaded)
		LoadPrimaryTracks(kfMaxRadius);

	if (fOverlayButton && MCMultiView::Instance().IsSet())
		MCMultiView::Instance().Get3DView()->GetGLViewer()->RemoveOverlayElement((TGLOverlayElement *)fOverlayButton);

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

//=MCExerciseR1========================================================================
void MCExerciseR1::Instructions()
{
	MCMultiLingual &ml = MCMultiLingual::Instance();

	fInstructions = new TRootHelpDialog(gClient->GetRoot(), " ", 620, 400);
	// fInstructions->DontCallClose();
	fInstructions->Popup();
	fInstructions->SetWindowName(ml.tr("STUDENT MODE INSTRUCTIONS"));
	fInstructions->SetText(ml.tr("\
Welcome to the ALICE event-diplay, here you see all the tools you need to analyse\n\
the ALICE Events.\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side there is your steering board which is divided into segments:\n\
Event Navigation:\n\
Click on the arrows to go to the previous or to the next event.\n\
The current event number and number of analysed events are also displayed.\n\n\
\
\
Counter:\n\
  Counter - allows you to count the charged tracks in the event and publishes it to histograms\n\
  in addition it will display the properties of the track like:\n\
  Momentum in x,y,z as well as the charge and the transverse momentum \n\
\n\
\
Display - this allows you to display or hide elements like:\n\
  Vertex: \t\t\tthe point where the collision took place\n\
  Clusters: \t\t\treconstucted points in the detectors showing where the particle was\n\
  Tracks: \t\t\treconstructed trajectories of the particles traversing the detectors\n\
  Show primary tracks only: \twill reduce the number of tracks shown to the ones originating\n\
  \t\t\t\tin the primary vertex\n\
  Geometry: \t\t\tdrawing of ALICE detectors\n\
  Background: \t\t\tchoose the colour of the display panel\n\
  Axes: \t\t\treference system of ALICE Experiment\n\
  (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
  on the right)\n\n\
\
Encyclopedia:\n\
  ALICE detector - description of ALICE and its main components;\n\
  (NOTE: click on the pictures of the detectors to get further\n\
  information)\n\n\
\
In the bottom, there are main application control buttons:\n\
  Exit\n\n\n\
YOUR TASK:  \n\
In this part of the master class programm you are supposed to evaluate the mean multiplicity\n\
in pp (first 30 events), peripheral (31st event), semicentral (32nd event) and central (33rd \n\
event) Pb-Pb-collisions and determine the integrated nuclear modification factor for these 3 \n\
Pb-Pb events.\n \n\
But first you need to make yourself familiar with this tool. \n\
In the event-display you have the possibilty to display the clusters (reconstructed space points)\n\
in the different detectors. From these clusters the paths on which a particles went through the \n\
detector are reconstructed - so called tracks. Based on the tracks one can reconstruct the \n\
collision point (primary vertex) from which most of the tracks should come. However as the \n\
some of the detectors are very large one might see several points in which a lot of tracks \n\
originate. The primary collision however should have taken place close to the center of our \n\
coordinate system (X~0, Y~0, Z~0), as then all detectors are able to see the collision and \n\
the tracks can be much better reconstructed. The tracks originating in the other vertices are \n\
called pile-up and do not need to be counted for the multiplicity of the event. Therefore you \n\
can just show the primary tracks (originating in the primary vertex) by clicking on the button \n\
\"Show primary tracks only\". If you click it again you will see the full event again.\n\n\
To achieve the final goal you should use the \"Counter\". This tool will help you count the \n\
number of primary tracks. It will increase the multiplicity in this event by one if you click on \n\
a track, which will then turn red to avoid double counting.\n\
After having clicked the button \"Event analysed\" the multiplicity of this event will be published\n\
to the corresponding histogram in the header \"Event Characteristics\". \n\
In the header \"Event Characteristics\" the histograms are aranged as follows: \n \
top left:\t\t multiplicity per event or as one could also say number of tracks\n \
top right:\t\t multiplicity per event, for tracks with a transverse momentum (pt)\n \
\t\t\t bigger 1.0 GeV/c \n \
bottom left:\t\t transverse momentum distribution for every analysed track \n \
bottom right:\t sign of the charge of every analysed track\n\
The mean multiplicity can be extracted either with a pt-threshold or without and can be seen in the \n\
statistics box of the corresponding histogram in the top row.\
						"));
}

//=MCExerciseS1========================================================================
void MCExerciseR1::LoadEsdTracks(Int_t maxR)
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

	Int_t nTracksInList = MCVSDReader::Instance().GetEveVSD()->fR.fIndex;
	Int_t n = 0;
	for (Int_t i = nTracksInList; i < MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntries(); ++i)
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

//=MCExerciseR1========================================================================
void MCExerciseR1::LoadPrimaryTracks(Int_t maxR)
{
	if (fTrackList == nullptr)
	{
		fTrackList = new TEveTrackList("ESD Tracks");
		fTrackList->SetMainColor(kBlue);
		fTrackList->SetMarkerColor(kYellow);
		fTrackList->SetMarkerStyle(4);
		fTrackList->SetMarkerSize(0.5);
		fTrackList->IncDenyDestroy();
	}
	else
		fTrackList->DestroyElements();

	TEveTrackPropagator *trkProp = fTrackList->GetPropagator();
	if (fCollSys == 1)
		trkProp->SetMagField(0.);
	else
		trkProp->SetMagField(0.5);
	trkProp->SetMaxR(maxR); //R[cm]

	MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(0);

	Int_t nTracksInList = MCVSDReader::Instance().GetEveVSD()->fR.fIndex;
	Int_t n = 0;
	fNPrim = 0;
	fNPrimMinPt = 0;
	for (Int_t i = nTracksInList; i < MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntries(); ++i)
	{
		MCVSDReader::Instance().GetEveVSD()->fTreeR->GetEntry(n);
		TEveTrack *track = new TEveTrack(&MCVSDReader::Instance().GetEveVSD()->fR, trkProp);
		Double_t dcaXY = MCVSDReader::Instance().GetEveVSD()->fR.fDcaXY;
		Double_t dcaZ = MCVSDReader::Instance().GetEveVSD()->fR.fDcaZ;
		if (TMath::Abs(dcaXY) < 0.5 && TMath::Abs(dcaZ) < 1.)
		{
			track->SetLineColor(kBlue);
			track->SetName("ESD_Track");
			track->SetTitle("ESD_Track");
			track->SetAttLineAttMarker(fTrackList);
			fTrackList->AddElement(track);
			fNPrim++;
			Double_t ptTrack = TMath::Sqrt(track->GetMomentum().fX * track->GetMomentum().fX + track->GetMomentum().fY * track->GetMomentum().fY);
			if (ptTrack > 1.0)
				fNPrimMinPt++;
		}
		n++;
	}
	fTrackList->MakeTracks();
	gEve->Redraw3D();
}

//=MCExerciseR1========================================================================
void MCExerciseR1::MakeHistograms()
{
	gStyle->SetOptStat(110);
	gStyle->SetPalette(1, 0);
	gStyle->SetFrameBorderMode(0);
	gStyle->SetFrameFillColor(0);
	gStyle->SetTitleFillColor(0);
	gStyle->SetTextSize(0.5);
	gStyle->SetLabelSize(0.03, "xyz");
	gStyle->SetLabelOffset(0.002, "xyz");
	gStyle->SetTitleFontSize(0.04);
	gStyle->SetTitleOffset(1, "y");
	gStyle->SetTitleOffset(0.7, "x");
	gStyle->SetCanvasColor(0);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);

	fMultHist = new TH1D("Statistics Mult", "Multiplicity distribution", 10, 0., 50.);
	fMultHist->SetLineColor(2);
	fMultHist->GetXaxis()->SetTitle("Multiplicity (# TPC tracks)");
	fMultHist->GetYaxis()->SetTitle("Counts");
	fMultHist->SetFillColor(0);
	// 	gMultHist->Draw();

	fMultHistMinPt = new TH1D("Statistics Mult pt > 1.0", "Multiplicity distribution pt > 1.0 GeV/c", 10, 0., 50.);
	fMultHistMinPt->SetLineColor(2);
	fMultHistMinPt->GetXaxis()->SetTitle("Multiplicity (# TPC tracks)");
	fMultHistMinPt->GetYaxis()->SetTitle("Counts");
	fMultHistMinPt->SetFillColor(0);
	// 	gMultHistMinPt->Draw();

	fPtHist = new TH1D("Statistics pt", "p_{t} distribution", 50, 0., 20.);
	fPtHist->SetLineColor(3);
	fPtHist->GetXaxis()->SetTitle("p_{t} (GeV/c)");
	fPtHist->GetYaxis()->SetTitle("Counts");
	fPtHist->SetFillColor(0);
	// 	gPtHist->Draw();

	fChargeHist = new TH1D("Statistics charge", "Charge distribution", 5, -2.5, 2.5);
	fChargeHist->SetLineColor(4);
	fChargeHist->GetXaxis()->SetTitle("Charge ");
	fChargeHist->GetYaxis()->SetTitle("Counts");
	fChargeHist->SetFillColor(0);
	// 	gChargeHist->Draw();
}

//=MCExerciseR1========================================================================
void MCExerciseR1::NextEvent()
{
	MCVSDReader &reader = MCVSDReader::Instance();
	if (reader.CurEv() == reader.MaxEv() - 1)
	{
		TString msgText = Form("%s %d", MCMultiLingual::Instance().tr("Last event"), reader.MaxEv());
		new TGMsgBox(gClient->GetRoot(), fSMF, "MCExerciseS1:Data", msgText, kMBIconExclamation, kMBOk);
		return;
	}
	else
		GotoEvent(reader.CurEv() + 1);
	if (MCAliCounter::Instance().IsStarted())
	{
		if (fCollSys == 2)
			MCAliCounter::Instance().SetZero();
		else if (fCollSys == 3 || fCollSys == 4 || fCollSys == 5)
			MCAliCounter::Instance().SetZero2();
	}
}

//=MCExerciseR1========================================================================
void MCExerciseR1::PrevEvent()
{
	Int_t currentEvent = MCVSDReader::Instance().CurEv();

	if (currentEvent == 0)
		return;
	else
		GotoEvent(currentEvent - 1);
	if (MCAliCounter::Instance().IsStarted())
	{
		if (fCollSys == 2)
			MCAliCounter::Instance().SetZero();
		else if (fCollSys == 3 || fCollSys == 4 || fCollSys == 5)
			MCAliCounter::Instance().SetZero2();
	}
}

//=MCExerciseR1========================================================================
void MCExerciseR1::PublishMult()
{
	fMultHist->Fill(MCAliCounter::Instance().GetMult());
	fPad->cd(1);
	fMultHist->Draw();
	fPad->cd(1)->Update();

	fMultHistMinPt->Fill(MCAliCounter::Instance().GetMultMinPt());
	fPad->cd(2);
	fMultHistMinPt->Draw();
	fPad->cd(2)->Update();

	MCAliCounter::Instance().SetZero();
}

//=MCExerciseR1========================================================================
void MCExerciseR1::StartExercise()
{
	if (fRole == RO_TUTOR)
		new TGMsgBox(gClient->GetRoot(), fMain, "MCExerciseR1::StartExercise", "Not implemented", kMBIconExclamation, kMBOk);
	else
	{
		if (fNeedsInit)
		{
			fNeedsInit = kFALSE;
			InitViewer();
			MakeHistograms();
		}
		StudentSet();
        GotoEvent(0);
		gEve->GetWindowManager()->HideAllEveDecorations();
		gEve->Redraw3D(kTRUE);
	}
}

//=MCExerciseR1========================================================================
void MCExerciseR1::StudentSet()
{
	Instructions();
	CustomizeBrowser();
}
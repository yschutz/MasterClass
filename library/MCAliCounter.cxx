// ALICE Counter for Master Class exercise RAA
#include "MCAliCounter.h"
#include "MCExerciseR1.h"
#include "MCMultiLingual.h"

#include <iostream>

#include <TGButton.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TMath.h>
#include <TRootHelpDialog.h>

MCAliCounter MCAliCounter::fInstance = MCAliCounter();
//=MCAliCounter========================================================================
MCAliCounter::MCAliCounter() : fChange(kTRUE), fMainFrame(nullptr), fStarted(kFALSE)
{
}

//=MCAliCounter========================================================================
void MCAliCounter::CountAutomatic()
{
	fAutomaticCount->SetText(TString::Format("%i", fMother->GetPrimaries()));
	fAutomaticCountMinPt->SetText(TString::Format("%i", fMother->GetPrimariesMinPt()));
}

//=MCAliCounter========================================================================
void MCAliCounter::IncreaseMult()
{
	fMult->SetNumber(fMult->GetNumber() + 1);
	if (GetPt() > 1.0)
		fMultMinPt->SetNumber(fMultMinPt->GetNumber() + 1);
	fChange = kTRUE;
}

//=MCAliCounter========================================================================
void MCAliCounter::IncreaseMult2(Double_t px, Double_t py)
{
	fMult->SetNumber(fMult->GetNumber() + 1);
	Double_t pt = TMath::Sqrt(px * px + py * py);
	if (pt > 1.0)
		fMultMinPt->SetNumber(fMultMinPt->GetNumber() + 1);
	fChange = kTRUE;
}

//=MCAliCounter========================================================================
void MCAliCounter::Instructions()
{
	MCMultiLingual &ml = MCMultiLingual::Instance();
	TRootHelpDialog *instructions = new TRootHelpDialog(gClient->GetRoot(), ml.tr("Counter Instructions"), 600, 200);
	instructions->SetText(ml.tr("\
Counter Instructions \n\n\
\
// The Counter is the tool to help you count the primary tracks in the event-display.\n\
By clicking on the tracks the multiplicity is increased by 1 and the properties\
of the track are displayed. \n\
The following properties will be evaluated:\n\
\t *px \t- momentum of the particle in x direction\n\
\t *py \t- momentum of the particle in y direction\n\
\t *pz \t- momentum of the particle in z direction\n\
\t *pt \t- momentum of the particle in the transverse plane (sqrt(px^2 + py^2))\n\
\t *charge \t - sign of the charge of the particle\n\
The transverse momentum (pt) and the charge will be automatically published to a histogram \n\
in the header \"Events Characteristics\".\n\n\
The two fields below the particle properties will give you the multiplicity in the event \n\
(with or without a minimum pt)\n\n\
The \"Clear\"-button - this button will set all the entries to 0, however the entries \n\
in the histograms remain, as they need to be accumulated over several events.\n\n\
The button \"Publish Multiplicity\"- will publish the multiplicity to the corresponding histograms. \n\n\
With the \"Close\"-button you can close the Counter\n"));
	instructions->Popup();
}

//=MCAliCounter========================================================================
void MCAliCounter::SetZero()
{
	fCharge->SetNumber(0);
	fPx->SetNumber(0);
	fPy->SetNumber(0);
	fPz->SetNumber(0);
	fPt->SetNumber(0);
	fMult->SetNumber(0);
	fMultMinPt->SetNumber(0);
}

//=MCAliCounter========================================================================
void MCAliCounter::Set1(Double_t px, Double_t py, Double_t pz, Int_t charge)
{
	Set2(px, py, pz);
	fPx->SetNumber(px);
	fPy->SetNumber(py);
	fPz->SetNumber(pz);
	fCharge->SetNumber(charge);
	fPt->SetNumber(TMath::Sqrt(px * px + py * py));
}

//=MCAliCounter========================================================================
void MCAliCounter::Set2(Double_t px, Double_t py, Double_t pz)
{
	fPx->SetNumber(px);
	fPy->SetNumber(py);
	fPz->SetNumber(pz);
	fChange = kTRUE;
}

//=MCAliCounter========================================================================
void MCAliCounter::SetZero2()
{
	fAutomaticCount->SetText("");
	fAutomaticCountMinPt->SetText("");
	fMult->SetNumber(0);
	fMultMinPt->SetNumber(0);
}

//=MCAliCounter========================================================================
void MCAliCounter::Start(MCExerciseR1 *mother, Int_t collSys)
{
	MCMultiLingual &ml = MCMultiLingual::Instance();
	if (fMainFrame)
		fMainFrame->UnmapWindow();
	fMother = mother;
	fMainFrame = new TGMainFrame(gClient->GetRoot(), 400, 200);
	fMainFrame->DontCallClose();

	if (collSys == 2)
	{
		TGGroupFrame *gf = new TGGroupFrame(fMainFrame, ml.tr("Counter Instructions"));
		TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
		{
			const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
			FontStruct_t buttonFont = font->GetFontStruct();
			ULong_t buttonRedColor;
			gClient->GetColorByName("red", buttonRedColor);
			TGTextButton *b = new TGTextButton(hf, "Instructions");
			b->SetTextColor(buttonRedColor);
			b->SetFont(buttonFont);
			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			b->Connect("Clicked()", "MCAliCounter", this, "Instructions()");
		}
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		gf = new TGGroupFrame(fMainFrame, ml.tr("Particle properties"));
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		TGLabel *val1 = new TGLabel(hf, ml.tr("Properties"));
		val1->SetTextJustify(kTextCenterX);
		hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//---------------------------------------------------------------------------
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		val1 = new TGLabel(hf, "px (GeV/c)");
		fPx = new TGNumberEntryField(hf);
		hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		hf->AddFrame(fPx, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//---------------------------------------------------------------------------
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		val1 = new TGLabel(hf, "py (GeV/c)");
		fPy = new TGNumberEntryField(hf);
		hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		hf->AddFrame(fPy, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//---------------------------------------------------------------------------
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		val1 = new TGLabel(hf, "pz (GeV/c)");
		fPz = new TGNumberEntryField(hf);
		hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		hf->AddFrame(fPz, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//---------------------------------------------------------------------------
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		val1 = new TGLabel(hf, "pt (GeV/c)");
		fPt = new TGNumberEntryField(hf);
		hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		hf->AddFrame(fPt, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//----------------------------------------------------------------------------
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		val1 = new TGLabel(hf, "charge");
		fCharge = new TGNumberEntryField(hf);
		hf->AddFrame(val1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		hf->AddFrame(fCharge, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//---------------------------------------------------------------------------
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		gf = new TGGroupFrame(fMainFrame, ml.tr("Multiplicity"));
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		fMult = new TGNumberEntryField(hf);
		fMult->SetEnabled(kFALSE);
		hf->AddFrame(fMult, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		gf = new TGGroupFrame(fMainFrame, ml.tr("Multiplicity for pT > 1.0 GeV/c"));
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		fMultMinPt = new TGNumberEntryField(hf);
		fMultMinPt->SetEnabled(kFALSE);
		hf->AddFrame(fMultMinPt, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		gf = new TGGroupFrame(fMainFrame, "Options");
		TGTextButton *b = 0;
		b = new TGTextButton(gf, ml.tr("Clear"));
		b->Connect("Pressed()", "MCAliCounter", this, "SetZero()");
		gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		b = new TGTextButton(gf, ml.tr("Publish to Mult histogram"));
		b->Connect("Pressed()", "MCExerciseR1", fMother, "PublishMult()");
		gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		b = new TGTextButton(gf, ml.tr("Close"));
		b->Connect("Pressed()", "MCAliCounter", this, "DoExit()");
		gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->SetWindowName("proton-proton");
		fStarted = kTRUE;
	}
	else if (collSys == 3 || collSys == 4 || collSys == 5)
	{
		TGGroupFrame *gf = new TGGroupFrame(fMainFrame, ml.tr("Counter Instructions"));
		TGHorizontalFrame *hf = new TGHorizontalFrame(gf, 200, 20, kFixedWidth);
		{
			const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
			FontStruct_t buttonFont = font->GetFontStruct();
			ULong_t buttonRedColor;
			gClient->GetColorByName("red", buttonRedColor);
			TGTextButton *b = new TGTextButton(hf, "Instructions");
			b->SetTextColor(buttonRedColor);
			b->SetFont(buttonFont);
			hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
			b->Connect("Clicked()", "MCAliCounter", this, "Instructions()");
		}
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		TGTextButton *b = 0;
		//---------------------------------------------------------------------------
		gf = new TGGroupFrame(fMainFrame, ml.tr("Multiplicity"));
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		fMult = new TGNumberEntryField(hf);
		fMult->SetEnabled(kFALSE);
		hf->AddFrame(fMult, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		gf = new TGGroupFrame(fMainFrame, ml.tr("Multiplicity for pT > 1.0 GeV/c"));
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		fMultMinPt = new TGNumberEntryField(hf);
		fMultMinPt->SetEnabled(kFALSE);
		hf->AddFrame(fMultMinPt, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		// Automatic counting
		gf = new TGGroupFrame(fMainFrame, "Options");
		b = new TGTextButton(gf, ml.tr("Count all primary tracks"));
		b->Connect("Pressed()", "MCAliCounter", this, "CountAutomatic()");
		gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		//---------------------------------------------------------------------------
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		TGLabel *explain = new TGLabel(hf);
		explain->SetText(ml.tr("Multiplicity"));
		hf->AddFrame(explain, new TGLayoutHints(kLHintsExpandX));
		fAutomaticCount = new TGLabel(hf);
		fAutomaticCount->SetText("");
		hf->AddFrame(fAutomaticCount, new TGLayoutHints(kLHintsExpandX));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		hf = new TGHorizontalFrame(gf, 250, 20, kFixedWidth);
		TGLabel *explainMinPt = new TGLabel(hf);
		explainMinPt->SetText(" Mult. pt > 1 GeV/c:  ");
		hf->AddFrame(explainMinPt, new TGLayoutHints(kLHintsExpandX));
		fAutomaticCountMinPt = new TGLabel(hf);
		fAutomaticCountMinPt->SetText("");
		hf->AddFrame(fAutomaticCountMinPt, new TGLayoutHints(kLHintsExpandX));
		gf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

		gf = new TGGroupFrame(fMainFrame, "Options");
		b = new TGTextButton(gf, ml.tr("Clear"));
		b->Connect("Pressed()", "MCAliCounter", this, "SetZero2()");
		gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		b = new TGTextButton(gf, ml.tr("Close"));
		b->Connect("Pressed()", "MCAliCounter", this, "DoExit()");
		gf->AddFrame(b, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->AddFrame(gf, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
		fMainFrame->SetWindowName("Pb-Pb");
		fStarted = kTRUE;
	}
	fMainFrame->MapSubwindows();
	fMainFrame->Resize(fMainFrame->GetDefaultSize());
	fMainFrame->MapWindow();
	fMainFrame->Move(1000, 500);
}
// ALICE Calculator for Master Class exercise Strangeness

#include "MCAliCalculator.h"
#include "MCMultiLingual.h"
#include <TColor.h>
#include <TGClient.h>
#include <TGButton.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TMath.h>
#include <TRootHelpDialog.h>

MCAliCalculator MCAliCalculator::fInstance = MCAliCalculator(); 

//=MCAliCalculator========================================================================
MCAliCalculator::MCAliCalculator() : fChange(kTRUE), fMainFrame(nullptr), fStarted(kFALSE)
{
}

//=MCAliCalculator========================================================================
void MCAliCalculator::CalculateInvariantMass()
{
    Double_t px1   = fPx1->GetNumber(); 
    Double_t py1   = fPy1->GetNumber(); 
    Double_t pz1   = fPz1->GetNumber(); 
    Double_t mass1 = fMass1->GetNumber(); 
    Double_t E1    = TMath::Sqrt(px1 * px1 + py1 * py1 + pz1 * pz1 + mass1 * mass1);

    Double_t px2   = fPx2->GetNumber(); 
    Double_t py2   = fPy2->GetNumber(); 
    Double_t pz2   = fPz2->GetNumber(); 
    Double_t mass2 = fMass2->GetNumber(); 
    Double_t E2    = TMath::Sqrt(px2 * px2 + py2 * py2 + pz2 * pz2 + mass2 * mass2);

    Double_t px3   = fPx3->GetNumber(); 
    Double_t py3   = fPy3->GetNumber(); 
    Double_t pz3   = fPz3->GetNumber(); 
    Double_t mass3 = fMass3->GetNumber(); 
    Double_t E3    = TMath::Sqrt(px3 * px3 + py3 * py3 + pz3 * pz3 + mass3 * mass3);

    Double_t minv = TMath::Sqrt((E1 + E2 + E3) * (E1 + E2 + E3) - 
                                (px1 + px2 + px3) * (px1 + px2 + px3) - 
                                (py1 + py2 + py3) * (py1 + py2 + py3) - 
                                (pz1 + pz2 + pz3) * (pz1 + pz2 + pz3));

    fInvariant_masscalc->SetNumber(minv);
}

//=MCAliCalculator========================================================================
Double_t MCAliCalculator::GetInvMass() const
{
    return fInvariant_masscalc->GetNumber(); 
}

//=MCAliCalculator========================================================================
void MCAliCalculator::Instructions()
{
    TRootHelpDialog *instructions = new TRootHelpDialog(gClient->GetRoot(), MCMultiLingual::Instance().tr("Calculator Instructions"), 100, 100);
    instructions->SetText(MCMultiLingual::Instance().tr("\
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

//=MCAliCalculator========================================================================
void MCAliCalculator::Start(MCExerciseS1 *mother)
{
    MCMultiLingual &ml = MCMultiLingual::Instance(); 
    if (fMainFrame)
        fMainFrame->UnmapWindow(); 
    fMother = mother; 
    fMainFrame = new TGMainFrame(gClient->GetRoot(), 400, 200); 
    fMainFrame->DontCallClose();
    TGGroupFrame *groupF = new TGGroupFrame(fMainFrame, ml.tr("Calculator Instructions"));
    TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 200, 20, kFixedWidth); 
    {
        const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
        FontStruct_t buttonFont = font->GetFontStruct();
        TGTextButton *btext = new TGTextButton(horiF, "Instructions"); 
        btext->SetTextColor(TColor::Number2Pixel(kRed + 2)); 
        btext->SetFont(buttonFont); 
        btext->Connect("Clicked()", "MCAliCalculator", this, "Instructions()"); 
        horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    }
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY)); 
    fMainFrame->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    groupF = new TGGroupFrame(fMainFrame, ml.tr("Particles Table")); 
    horiF  = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    TGLabel *lab1 = new TGLabel(horiF, ml.tr("Particle Type")); 
    TGLabel *lab2 = new TGLabel(horiF, ml.tr("Mass [GeV/c2]")); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    groupF->AddFrame(horiF);

    // electron
    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1  = new TGLabel(horiF, "Electron"); 
    lab2  = new TGLabel(horiF, "0.000511"); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    groupF->AddFrame(horiF);

    // pion
    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1  = new TGLabel(horiF, "Pion"); 
    lab2  = new TGLabel(horiF, "0.139"); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    groupF->AddFrame(horiF);

    // Kaon
    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1  = new TGLabel(horiF, "Kaon"); 
    lab2  = new TGLabel(horiF, "0.497"); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    groupF->AddFrame(horiF);

    // proton
    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1  = new TGLabel(horiF, "Proton"); 
    lab2  = new TGLabel(horiF, "0.938"); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    groupF->AddFrame(horiF);

    // lambda
    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1  = new TGLabel(horiF, "Lambda"); 
    lab2  = new TGLabel(horiF, "1.115"); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    groupF->AddFrame(horiF);

    // Xi
    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1  = new TGLabel(horiF, "Xi"); 
    lab2  = new TGLabel(horiF, "1.321"); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    horiF->AddFrame(lab2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));  
    groupF->AddFrame(horiF);

    fMainFrame->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    groupF = new TGGroupFrame(fMainFrame, ml.tr("Calculator")); 
    horiF  = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    
    lab1   = new TGLabel(horiF, "    "); 
    lab1->SetTextJustify(kTextCenterX); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    lab1   = new TGLabel(horiF, "(-)"); 
    lab1->SetTextJustify(kTextCenterX); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    lab1   = new TGLabel(horiF, "(+)"); 
    lab1->SetTextJustify(kTextCenterX); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    lab1   = new TGLabel(horiF, ml.tr("Bachelor")); 
    lab1->SetTextJustify(kTextCenterX); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));

    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth); 
    lab1 = new TGLabel(horiF, "px"); 
    fPx1 = new TGNumberEntryField(horiF); 
    fPx2 = new TGNumberEntryField(horiF); 
    fPx3 = new TGNumberEntryField(horiF); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPx1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPx2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPx3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth);
    lab1 = new TGLabel(horiF, "py"); 
    fPy1 = new TGNumberEntryField(horiF); 
    fPy2 = new TGNumberEntryField(horiF); 
    fPy3 = new TGNumberEntryField(horiF); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPy1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPy2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPy3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth);
    lab1 = new TGLabel(horiF, "pz"); 
    fPz1 = new TGNumberEntryField(horiF); 
    fPz2 = new TGNumberEntryField(horiF); 
    fPz3 = new TGNumberEntryField(horiF); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPz1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPz2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fPz3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth);
    lab1 = new TGLabel(horiF, ml.tr("Mass")); 
    fMass1 = new TGNumberEntryField(horiF); 
    fMass2 = new TGNumberEntryField(horiF); 
    fMass3 = new TGNumberEntryField(horiF); 
    horiF->AddFrame(lab1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fMass1, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fMass2, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    horiF->AddFrame(fMass3, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1)); 
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth);
    TGTextButton *btext = new TGTextButton(horiF, ml.tr("Mass")); 
    btext->Connect("Clicked()", "MCAliCalculator", this, "CalculateInvariantMass()"); 
    horiF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4)); 
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));    

    horiF = new TGHorizontalFrame(groupF, 250, 20, kFixedWidth);
    fInvariant_masscalc = new TGNumberEntryField(horiF); 
    fInvariant_masscalc->SetEnabled(kFALSE); 
    horiF->AddFrame(fInvariant_masscalc, new TGLayoutHints(kLHintsExpandX, 1, 1, 3, 4));
    groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("That is a Kaon")); 
    btext->Connect("Clicked()", "MCExerciseS1", fMother, "FoundPart(E_PATId id = P_KAON)"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("That is a Lambda")); 
    btext->Connect("Clicked()", "MCExerciseS1", fMother, "FoundPart(E_PATId id = P_LAMBDA)"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("That is a anti-Lambda")); 
    btext->Connect("Clicked()", "MCExerciseS1", fMother, "FoundPart(E_PATId id = P_ALAMBDA)"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("That is a Xi")); 
    btext->Connect("Clicked()", "MCExerciseS1", fMother, "FoundPart(E_PATId id = P_XI)"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("That is background")); 
    btext->Connect("Clicked()", "MCAliCalculator", this, "Background()"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("Clear")); 
    btext->Connect("Clicked()", "MCAliCalculator", this, "SetZero()"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    fBtxtLR = new TGTextButton(groupF, ml.tr("Load")); 
    fBtxtLR->Connect("Clicked()", "MCExerciseS1", fMother, "LoadSavedResults()"); 
    groupF->AddFrame(fBtxtLR, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    fBtxtSR = new TGTextButton(groupF, ml.tr("Save")); 
    fBtxtSR->Connect("Clicked()", "MCExerciseS1", fMother, "SaveResults()"); 
    groupF->AddFrame(fBtxtSR, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    btext = new TGTextButton(groupF, ml.tr("Close")); 
    btext->Connect("Clicked()", "MCAliCalculator", this, "DoExit()"); 
    groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1)); 

    fMainFrame->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    fMainFrame->SetWindowName(ml.tr("Calculator")); 
    fMainFrame->MapSubwindows();
    fMainFrame->Resize(fMainFrame->GetDefaultSize()); 
    fMainFrame->MapWindow(); 
    fMainFrame->Move(1000, 500); 
    fStarted = kTRUE; 
}

//=MCAliCalculator========================================================================
void MCAliCalculator::BlockButtons(Bool_t state)
{
    fBtxtLR->SetEnabled(state); 
    fBtxtSR->SetEnabled(state); 
}

//=MCAliCalculator========================================================================
void MCAliCalculator::Set1(Double_t px, Double_t py, Double_t pz, Double_t mass)
{
    fPx1->SetNumber(px); 
    fPy1->SetNumber(py);
    fPz1->SetNumber(pz);
    fMass1->SetNumber(mass);

    fChange = kTRUE; 
}

//=MCAliCalculator========================================================================
void MCAliCalculator::Set2(Double_t px, Double_t py, Double_t pz, Double_t mass)
{
    fPx2->SetNumber(px); 
    fPy2->SetNumber(py);
    fPz2->SetNumber(pz);
    fMass2->SetNumber(mass);

    fChange = kTRUE; 
}

//=MCAliCalculator========================================================================
void MCAliCalculator::Set3(Double_t px, Double_t py, Double_t pz, Double_t mass)
{
    fPx3->SetNumber(px); 
    fPy3->SetNumber(py);
    fPz3->SetNumber(pz);
    fMass3->SetNumber(mass);

    fChange = kTRUE; 
}

//=MCAliCalculator========================================================================
void MCAliCalculator::SetZero()
{
    fPx1->SetNumber(0);
    fPy1->SetNumber(0);
    fPz1->SetNumber(0);
    fMass1->SetNumber(0);

    fPx2->SetNumber(0);
    fPy2->SetNumber(0);
    fPz2->SetNumber(0);
    fMass2->SetNumber(0);

    fPx3->SetNumber(0);
    fPy3->SetNumber(0);
    fPz3->SetNumber(0);
    fMass3->SetNumber(0);

    fInvariant_masscalc->SetNumber(0); 
}
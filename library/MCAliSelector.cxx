#include "MCAliSelector.h"
#include "MCMultiLingual.h"
#include "MCExerciseJ1.h"

#include <TGButton.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TRootHelpDialog.h>

MCAliSelector MCAliSelector::fInstance = MCAliSelector();

//=MCAliSelector========================================================================
MCAliSelector::MCAliSelector() : fChange(kTRUE), fMainFrame(nullptr), fStarted(kFALSE)
{
}


//=MCAliSelector========================================================================
void MCAliSelector::DoOK()
{
    fMother->SetPIDCutValues();
    fMother->PlotPIDLines();
    fMother->EnableTrackCutsButton();
    fMother->ApplyTrackCuts();
    DoExit();
}

//=MCAliSelector========================================================================
void MCAliSelector::Instructions()
{
    TRootHelpDialog *instructions = new TRootHelpDialog(gClient->GetRoot(), "INSTRUCTIONS", 700, 400);
    instructions->SetText(MCMultiLingual::Instance().tr("\
   Instructions: Track Cuts \n\n\
      The \"Track Cuts\" is the tool that allows you to select only tracks which are \n\
      candidates for being electrons and positrons, originating from a J/psi decay,\n\n\
      based on following quantities:\n\
      p     \t- momentum\n\
      dE/dx \t- specific energy loss of the particle inside the detector\n\
      Enter your desired minimum/maximum values and click on \"Engage\".\n\
      In the \"Specific Energy Loss\" histogram in the \"Particle Identification\" window\n\
      a rectangle which defines the selected area will be drawn. \n\
      If you want to see only the selected tracks in the 3D model, click\n\
      \"Selected Tracks\"in the menu \"Load\" on the left side."));
    instructions->Popup();
}

//=MCAliSelector========================================================================
void MCAliSelector::SetMinMax()
{
    if (fPMin->GetNumberEntry()->GetNumber() > fPMax->GetNumberEntry()->GetNumber())
        fPMin->SetNumber(fPMin->GetNumMin());
    if (fDeMin->GetNumberEntry()->GetNumber() > fDeMax->GetNumberEntry()->GetNumber())
        fDeMin->SetNumber(fDeMin->GetNumMin());
}

//=MCAliSelector========================================================================
void MCAliSelector::Start(MCExerciseJ1 *mother)
{
    if (fMainFrame)
        fMainFrame->UnmapWindow();
    fMother = mother;
    fMainFrame = new TGMainFrame(gClient->GetRoot(), 400, 200);
    fMainFrame->DontCallClose();

    TGTextButton *btext;
    TGGroupFrame *groupF = new TGGroupFrame(fMainFrame, "");
    {
        const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
        FontStruct_t buttonFont = font->GetFontStruct();
        ULong_t buttonRedColor;
        gClient->GetColorByName("red", buttonRedColor);
        btext = new TGTextButton(groupF, "Instructions");
        btext->SetTextColor(buttonRedColor);
        btext->SetFont(buttonFont);
        btext->Connect("Clicked()", "MCAliSelector", this, "Instructions()");
        groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 2));
        TGLabel *label;
        TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 200, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, "    ");
            label->SetTextJustify(kTextCenterX);
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            label = new TGLabel(horiF, "min");
            label->SetTextJustify(kTextCenterX);
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            label = new TGLabel(horiF, "max");
            label->SetTextJustify(kTextCenterX);
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        horiF = new TGHorizontalFrame(groupF, 200, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, "p (GeV/c)");
            fPMin = new TGNumberEntry(horiF, 0, 4, 1,
                                      TGNumberFormat::kNESReal,
                                      TGNumberFormat::kNEANonNegative,
                                      TGNumberFormat::kNELLimitMinMax,
                                      0., 10.);
            fPMin->Connect("ValueSet(Long_t)", "MCAliSelector", this, "SetMinMax()");
            (fPMin->GetNumberEntry())->Connect("ReturnPressed()", "MCAliSelector", this, "SetMinMax()");
            fPMax = new TGNumberEntry(horiF, 10, 4, 2,
                                      TGNumberFormat::kNESReal,
                                      TGNumberFormat::kNEANonNegative,
                                      TGNumberFormat::kNELLimitMinMax,
                                      0., 10.);
            fPMax->Connect("ValueSet(Long_t)", "MCAliSelector", this, "SetMinMax()");
            (fPMax->GetNumberEntry())->Connect("ReturnPressed()", "MCAliSelector", this, "SetMinMax()");
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            horiF->AddFrame(fPMin, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            horiF->AddFrame(fPMax, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 2));

        horiF = new TGHorizontalFrame(groupF, 200, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, "dE/dx");
            fDeMin = new TGNumberEntry(horiF, 20, 4, 3,
                                       TGNumberFormat::kNESReal,
                                       TGNumberFormat::kNEANonNegative,
                                       TGNumberFormat::kNELLimitMinMax,
                                       20., 140.);
            fDeMin->Connect("ValueSet(Long_t)", "MCAliSelector", this, "SetMinMax()");
            (fDeMin->GetNumberEntry())->Connect("ReturnPressed()", "MCAliSelector", this, "SetMinMax()");
            fDeMax = new TGNumberEntry(horiF, 140, 4, 4,
                                       TGNumberFormat::kNESReal,
                                       TGNumberFormat::kNEANonNegative,
                                       TGNumberFormat::kNELLimitMinMax,
                                       20., 140.);
            fDeMax->Connect("ValueSet(Long_t)", "MCAliSelector", this, "SetMinMax()");
            (fDeMax->GetNumberEntry())->Connect("ReturnPressed()", "MCAliSelector", this, "SetMinMax()");
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            horiF->AddFrame(fDeMin, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            horiF->AddFrame(fDeMax, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 2));

        btext = new TGTextButton(groupF, "OK");
        btext->Connect("Pressed()", "MCAliSelector", this, "DoOK()");
        groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 2));

        // btext = new TGTextButton(groupF, MCMultiLingual::Instance().GetText(MCMultiLingual::CLOSE));
        // btext->Connect("Pressed()", "MCAliSelector", this, "DoExit()");
        // groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 2));
    }
    fMainFrame->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

    fMainFrame->SetWindowName(MCMultiLingual::Instance().tr("Tracks Selector"));
    fMainFrame->MapSubwindows();
    fMainFrame->Resize(fMainFrame->GetDefaultSize());
    fMainFrame->MapWindow();
    fMainFrame->Move(1000, 500);
    fStarted = kTRUE;
}
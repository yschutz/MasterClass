#include "MCAliExtractor.h"
#include "MCExerciseJ1.h"
#include "MCMultiLingual.h"

#include <TGButton.h>
#include <TGFrame.h>
#include <TGLabel.h>
#include <TGNumberEntry.h>
#include <TRootHelpDialog.h>

MCAliExtractor MCAliExtractor::fInstance = MCAliExtractor();

//=MCAliExtractor========================================================================
MCAliExtractor::MCAliExtractor() : fStarted(kTRUE), fMainFrame(nullptr)
{
}

//=MCAliExtractor========================================================================
void MCAliExtractor::Instructions()
{
    TRootHelpDialog *instructions = new TRootHelpDialog(gClient->GetRoot(), "INSTRUCTIONS", 700, 400);
    instructions->SetText(MCMultiLingual::Instance().tr("\
Instructions: Signal Extraction\n\n\
     With this tool you can extract the J/psi signal from the Invariant mass distribution.\n\
     You can set an invariant mass range from which the signal should be extracted. \n\
     The tool will calculate in the chosen mass range \n\
      - the signal S by integrating the counts in the \"Unlike - Like Sign Mass Distribution\" \n\
      - the background B by integrating the counts in the \"Like Sign Mass Distribution\" \n\
      - the signal over background ratio S/B\n\
      - the significance, defined as S/sqrt(S+B)\n\
     It will also store your current values for the track cuts and the in variant mass window\n\
     together with the obtained signal, S/B and significance in a file called  \"masterclass.save\""));
    instructions->Popup();
}

//=MCAliExtractor========================================================================
void MCAliExtractor::SetMinMax()
{
    if (fMMin->GetNumberEntry()->GetNumber() > fMMax->GetNumberEntry()->GetNumber())
        fMMin->SetNumber(fMMin->GetNumMin());
}

//=MCAliExtractor========================================================================
void MCAliExtractor::Start(MCExerciseJ1 *mother)
{
    MCMultiLingual &ml = MCMultiLingual::Instance();
    if (fMainFrame)
        fMainFrame->UnmapWindow();
    fMother = mother;
    fMainFrame = new TGMainFrame(gClient->GetRoot(), 400, 200);
    fMainFrame->DontCallClose();

    TGTextButton *btext;
    TGLabel *label;
    TGGroupFrame *groupF = new TGGroupFrame(fMainFrame, "");
    {
        const TGFont *font = gClient->GetFont("-*-times-bold-r-*-*-16-*-*-*-*-*-*-*");
        FontStruct_t buttonFont = font->GetFontStruct();
        ULong_t buttonRedColor;
        gClient->GetColorByName("red", buttonRedColor);

        btext = new TGTextButton(groupF, "Instructions");
        btext->SetTextColor(buttonRedColor);
        btext->SetFont(buttonFont);
        btext->Connect("Clicked()", "MCAliExtractor", this, "Instructions()");
        groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

        TGHorizontalFrame *horiF = new TGHorizontalFrame(groupF, 120, 20, kFixedWidth);
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

        horiF = new TGHorizontalFrame(groupF, 240, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, ml.tr("Invariant Mass (GeV/c^{2})"));
            fMMin = new TGNumberEntry(horiF, 0, 4, 1,
                                      TGNumberFormat::kNESReal,
                                      TGNumberFormat::kNEANonNegative,
                                      TGNumberFormat::kNELLimitMinMax,
                                      0., 6.);
            fMMin->Connect("ValueSet(Long_t)", "MCAliExtractor", this, "SetMinMax()");
            (fMMin->GetNumberEntry())->Connect("ReturnPressed()", "MCAliExtractor", this, "SetMinMax()");

            fMMax = new TGNumberEntry(horiF, 6, 4, 2,
                                      TGNumberFormat::kNESReal,
                                      TGNumberFormat::kNEANonNegative,
                                      TGNumberFormat::kNELLimitMinMax,
                                      0., 6.);
            fMMax->Connect("ValueSet(Long_t)", "MCAliExtractor", this, "SetMinMax()");
            (fMMax->GetNumberEntry())->Connect("ReturnPressed()", "MCAliExtractor", this, "SetMinMax()");
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            horiF->AddFrame(fMMin, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
            horiF->AddFrame(fMMax, new TGLayoutHints(kLHintsExpandX, 1, 1, 1, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        btext = new TGTextButton(groupF, "OK");
        btext->Connect("Pressed()", "MCExerciseJ1", fMother, "WrapUp()");
        groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        horiF = new TGHorizontalFrame(groupF, 240, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, "# J/Psi");
            label->SetTextJustify(kTextCenterX);
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
            fJPsiCount = new TGNumberEntryField(horiF);
            fJPsiCount->SetEnabled(kFALSE);
            horiF->AddFrame(fJPsiCount, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        horiF = new TGHorizontalFrame(groupF, 240, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, Form("Signal/%s", ml.tr("Background")));
            label->SetTextJustify(kTextCenterX);
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
            fSoB = new TGNumberEntryField(horiF);
            fSoB->SetEnabled(kFALSE);
            horiF->AddFrame(fSoB, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        horiF = new TGHorizontalFrame(groupF, 240, 20, kFixedWidth);
        {
            label = new TGLabel(horiF, "Significance");
            label->SetTextJustify(kTextCenterX);
            horiF->AddFrame(label, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
            fSignificance = new TGNumberEntryField(horiF);
            fSignificance->SetEnabled(kFALSE);
            horiF->AddFrame(fSignificance, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));
        }
        groupF->AddFrame(horiF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        btext = new TGTextButton(groupF, ml.tr("Close"));
        btext->Connect("Pressed()", "MCAliExtractor", this, "DoExit()");
        groupF->AddFrame(btext, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        fMainFrame->AddFrame(groupF, new TGLayoutHints(kLHintsExpandX, 2, 2, 5, 1));

        fMainFrame->SetWindowName(ml.tr("Extract Signal"));

        fMainFrame->MapSubwindows();
        fMainFrame->Resize(fMainFrame->GetDefaultSize());
        fMainFrame->MapWindow();
    }
}
#include "MCTutorial.h"
//=MCTutorial========================================================================
MCTutorial::MCTutorial(const TGWindow *p, UInt_t w, UInt_t h) : MCExercise(p, w, h)
{
    fDataSetFileDir = "Data-Masterclass/events/Strangeness/Part1/";
    fDataTemplate = "AliVSD_MasterClass_pp_x.root";
    fGuiLogoName = "images/strangeness.png";
    MakeGui();
}
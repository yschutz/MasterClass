#include "MCMain.h"
#include "MCExerciseJ1.h"
#include "MCExerciseR1.h"
#include "MCExerciseR2.h"
#include "MCExerciseS1.h"
#include "MCExerciseS2.h"
#include "MCMultiLingual.h"
#include "MCVSDReader.h"

#include <TGCanvas.h>
#include <TGFrame.h>
#include <TApplication.h>
#include <TGDockableFrame.h>
#include <TGFrame.h>
#include <TGImageMap.h>
#include <TGMenu.h>
#include <TGMsgBox.h>
#include <TGPicture.h>
#include <TGWindow.h>
#include <TROOT.h>
#include <TSystem.h>

#include <iostream>
/*
   fMain (MCMain) -- > fMenuDock (TGDockableFrame) --> fMenuBar (TGMenuBar) --> fMenuGo (TGPopupMenu) --> Exit (Entry)
                       -- > fCanvasWindow (TGCanvas) -- > alicelogo (TGFrame) 
                       
                         
*/
//=MCMain========================================================================
MCMain::MCMain(const TGWindow *p, UInt_t w, UInt_t h, Int_t opt)
{
   // const TString kAliceLogo("images/alicelogo.png");
   const TString kAliceLogo("images/alicelogo.png");
   const TString kFrenchFlag("Images/français.png");
   const TString kEnglishFlag("Images/english.png");

   //create the main frame
   fMain = new TGMainFrame(p, w, h);
   fMain->SetCleanup(kDeepCleanup);
   fMain->Connect("CloseWindow()", "MCMain", this, "CloseWindow()");

   // create dock
   fMenuDock = new TGDockableFrame(fMain);
   fMenuDock->SetWindowName("Master Class Menu");
   fMenuDock->EnableUndock(kTRUE);
   fMenuDock->EnableHide(kTRUE);
   fMenuDock->Connect("Undocked()", "MCMain", this, "HandleMenu(ECommand id=M_VIEW_UNDOCK)");

   // add dock to main frame
   fMain->AddFrame(fMenuDock, new TGLayoutHints(kLHintsExpandX, 0, 0, 1, 0));

   // pop up language
   fMenuLang = new TGPopupMenu(gClient->GetRoot());
   fMenuLang->AddEntry("&FR", M_LAN_FR); //, 0, gClient->GetPicture(kFrenchFlag, 20, 20));
   fMenuLang->AddEntry("&EN", M_LAN_EN); //, 0, gClient->GetPicture(kEnglishFlag, 20, 20));
   fMenuLang->CheckEntry(M_LAN_FR);

   // pop up menu Go
   fMenuGo = new TGPopupMenu(gClient->GetRoot());
   fMenuGo->AddPopup("&Language", fMenuLang);
   fMenuGo->AddSeparator();
   fMenuGo->AddEntry("E&xit", M_GO_EXIT);
   fMenuGo->Connect("Activated(Int_t)", "MCMain", this, "HandleMenu(ECommand)");
   // fMenuGo->Connect("PoppedUp()", "MCMain", this, "HandlePopup()");
   // fMenuGo->Connect("PoppedDown()", "MCMain", this, "HandlePopdown()");

   // pop up menu view
   fMenuView = new TGPopupMenu(gClient->GetRoot());
   fMenuView->AddEntry("&Dock", M_VIEW_DOCK);
   fMenuView->AddEntry("&Undock", M_VIEW_UNDOCK);
   fMenuView->AddSeparator();
   fMenuView->AddEntry("Enable U&ndock", M_VIEW_ENBL_DOCK);
   fMenuView->AddEntry("Enable &Hide", M_VIEW_ENBL_HIDE);
   fMenuView->DisableEntry(M_VIEW_DOCK);
   fMenuView->CheckEntry(M_VIEW_ENBL_DOCK);
   fMenuView->CheckEntry(M_VIEW_ENBL_HIDE);
   fMenuView->Connect("Activated(Int_t", "MCMain", this, "HandleMenu(ECommand");
   
   TString datapath("Data-Masterclass/events/");

   // pop up menu Exercices Strangeness
   fMenuExSTRP = new TGPopupMenu(gClient->GetRoot());
   fMenuExSTRP->AddEntry("Part&1", M_EX_STRP1);
   fMenuExSTRP->AddEntry("Part&2", M_EX_STRP2);
   if (gSystem->AccessPathName(datapath + "Strangeness") == kTRUE)
   {      
      fMenuExSTRP->DisableEntry(M_EX_STRP1); 
      fMenuExSTRP->DisableEntry(M_EX_STRP2); 
   } 
   // pop up menu Exercices RAA
   fMenuExRAAP = new TGPopupMenu(gClient->GetRoot());
   fMenuExRAAP->AddEntry("Part&1", M_EX_RAAP1);
   fMenuExRAAP->AddEntry("Part&2", M_EX_RAAP2);
   if (gSystem->AccessPathName(datapath + "RAA") == kTRUE)
   {
      fMenuExRAAP->DisableEntry(M_EX_RAAP1);
      fMenuExRAAP->DisableEntry(M_EX_RAAP2);
   }

   // pop up menu Exercices JPsi
   fMenuExJPsP = new TGPopupMenu(gClient->GetRoot());
   fMenuExJPsP->AddEntry("Part&1", M_EX_JPSP1);
   // fMenuExJPsP->AddEntry("Part&2", M_EX_JPSP2);
   if (gSystem->AccessPathName(datapath + "JPsi") == kTRUE)
   {
      fMenuExJPsP->DisableEntry(M_EX_JPSP1);
      // fMenuExJPsP->DisableEntry(M_EX_JPSP2);
   }

   fMenuEx = new TGPopupMenu(gClient->GetRoot());
   fMenuEx->AddPopup("&Strangeness", fMenuExSTRP);
   fMenuEx->AddPopup("&RAA", fMenuExRAAP);
   fMenuEx->AddPopup("&JPsi", fMenuExJPsP);
   fMenuEx->Connect("Activated(Int_t)", "MCMain", this, "HandleMenu(ECommand)");

   // menu bar
   fMenuBarLayout = new TGLayoutHints(kLHintsTop | kLHintsExpandX);
   fMenuBar = new TGMenuBar(fMenuDock, 1, 1, kHorizontalFrame);
   fMenuBar->AddPopup("&Go", fMenuGo, fMenuBarLayout);
   fMenuBar->AddPopup("&View", fMenuView, fMenuBarLayout);
   fMenuBar->AddPopup("&Exercices", fMenuEx, fMenuBarLayout);

   // add menu bar to dock
   fMenuDock->AddFrame(fMenuBar, fMenuBarLayout);

   // add alice logo
   fAliceLogo = new TGImageMap(fMain, gClient->GetPicture(kAliceLogo));
   fMain->AddFrame(fAliceLogo, new TGLayoutHints(kLHintsCenterX | kLHintsCenterY | kLHintsExpandX | kLHintsExpandY));

   // everything is done
   fMain->SetWindowName("Master Class");
   fMain->SetIconName("ALICE Master Class");
   fMain->MapSubwindows(),
       fMain->Resize();
   fMain->MapWindow();
   // Connect("Created()", "MCMain", this, "Welcome()");

   MCMultiLingual::Instance().SetLanguage("FR");

   switch (opt)
   {
   case 11:
      fExS1 = new MCExerciseS1(gClient->GetRoot(), 400, 200);
      fMenuExSTRP->DisableEntry(M_EX_STRP1); 
      break;
   case 12:
      fExS2 = new MCExerciseS2(gClient->GetRoot(), 400, 200);
      fMenuExSTRP->DisableEntry(M_EX_STRP2); 
      break;
   case 21:
      fExR1 = new MCExerciseR1(gClient->GetRoot(), 400, 200);
      fMenuExRAAP->DisableEntry(M_EX_RAAP1); 
      break;
   case 22:
      fExR2 = new MCExerciseR2(gClient->GetRoot(), 400, 200);
      fMenuExRAAP->DisableEntry(M_EX_RAAP2); 
      break;
   case 31: 
      fExJ1 = new MCExerciseJ1(gClient->GetRoot(), 400, 200);
      fMenuExJPsP->DisableEntry(M_EX_JPSP1); 
      break;
   default:
      break;
   }
   // Created();
}

//=MCMain========================================================================
MCMain::~MCMain()
{
   fMain->CloseWindow();
   if (fExS1)
      delete fExS1;
   if (fExS2)
      delete fExS2;
   if (fExR1)
      delete fExR1;
   if (fExR2)
      delete fExR2;
   if (fExJ1)
      delete fExJ1;
}

//=MCMain========================================================================
void MCMain::CloseWindow()
{
   gApplication->Terminate(0);
}

//=MCMain========================================================================
void MCMain::HandleMenu(ECommand id)
{
   MCMultiLingual &ml = MCMultiLingual::Instance();
   switch (id)
   {
   case M_GO_EXIT:
   {
      Int_t retval;
      TString msgText(ml.tr("Do you want really to quit?"));
      new TGMsgBox(gClient->GetRoot(), fMain, "MCMain:Exit", msgText, kMBIconQuestion, kMBYes | kMBCancel, &retval);
      if (retval == 1)
         gApplication->Terminate(0);
      break;
   }
   case M_EX_STRP1:
   {
      if ((fExR1 || fExR2 || fExJ1) && MCVSDReader::Instance().IsInitialized())
         gApplication->Terminate(11); 
      fMenuExSTRP->EnableEntry(M_EX_STRP2); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP1); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP2); 
      fMenuExJPsP->EnableEntry(M_EX_JPSP1); 
      if (fExS2)
      {
         delete fExS2;
         fExS2 = nullptr;
      }
      if (fExR1)
      {
         delete fExR1;
         fExR1 = nullptr;
      }
      if (fExR2)
      {
         delete fExR2;
         fExR2 = nullptr;
      }      
      if (fExJ1)
      {
         delete fExJ1;
         fExJ1 = nullptr;
      }
      if (!fExS1) 
         fExS1 = new MCExerciseS1(gClient->GetRoot(), 400, 200);
      fMenuExSTRP->DisableEntry(M_EX_STRP1); 

      break;
   }
   case M_EX_STRP2:
   {
      if ((fExR1 || fExR2 || fExJ1) && MCVSDReader::Instance().IsInitialized())
         gApplication->Terminate(12); 
      fMenuExSTRP->EnableEntry(M_EX_STRP1); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP1); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP2); 
      fMenuExJPsP->EnableEntry(M_EX_JPSP1); 
      if (fExS1)
      {
         delete fExS1;
         fExS1 = nullptr;
      }
      if (fExR1)
      {
         delete fExR1;
         fExR1 = nullptr;
      }
      if (fExR2)
      {
         delete fExR2;
         fExR2 = nullptr;
      }
      if (fExJ1)
      {
         delete fExJ1;
         fExJ1 = nullptr;
      }
      if (!fExS2) 
         fExS2 = new MCExerciseS2(gClient->GetRoot(), 400, 200);
      break;
   }
   case M_EX_RAAP1:
   {
     if ((fExS1 || fExS2 || fExJ1) && MCVSDReader::Instance().IsInitialized())
         gApplication->Terminate(21); 
      fMenuExSTRP->EnableEntry(M_EX_STRP1); 
      fMenuExSTRP->EnableEntry(M_EX_STRP2); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP2); 
      fMenuExJPsP->EnableEntry(M_EX_JPSP1); 
      if (fExS1)
      {
         delete fExS1;
         fExS1 = nullptr;
      }
      if (fExS2)
      {
         delete fExS2;
         fExS2 = nullptr;
      }
      if (fExR2)
      {
         delete fExR2;
         fExR2 = nullptr;
      }
      if (fExJ1)
      {
         delete fExJ1;
         fExJ1 = nullptr;
      }
      if (!fExR1) 
         fExR1 = new MCExerciseR1(gClient->GetRoot(), 400, 200);
      fMenuExRAAP->DisableEntry(M_EX_RAAP1); 
      break;
   }
   case M_EX_RAAP2:
   {
      if ((fExS1 || fExS2 || fExJ1) && MCVSDReader::Instance().IsInitialized())
         gApplication->Terminate(22); 
      fMenuExSTRP->EnableEntry(M_EX_STRP1); 
      fMenuExSTRP->EnableEntry(M_EX_STRP2); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP1); 
      fMenuExJPsP->EnableEntry(M_EX_JPSP1); 
      if (fExS1)
      {
         delete fExS1;
         fExS1 = nullptr;
      }
      if (fExS2)
      {
         delete fExS2;
         fExS2 = nullptr;
      }
      if (fExR1)
      {
         delete fExR1;
         fExR1 = nullptr;
      }
      if (fExJ1)
      {
         delete fExJ1;
         fExJ1 = nullptr;
      }
      if (!fExR2) 
         fExR2 = new MCExerciseR2(gClient->GetRoot(), 400, 200);
      fMenuExRAAP->DisableEntry(M_EX_RAAP2); 
      break;
   }
   case M_EX_JPSP1:
   {
     if ((fExS1 || fExS2 || fExR1 || fExR2) && MCVSDReader::Instance().IsInitialized())
         gApplication->Terminate(31); 
      fMenuExSTRP->EnableEntry(M_EX_STRP1); 
      fMenuExSTRP->EnableEntry(M_EX_STRP2); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP1); 
      fMenuExRAAP->EnableEntry(M_EX_RAAP2); 
      if (fExS1)
      {
         delete fExS1;
         fExS1 = nullptr;
         fMenuExSTRP->EnableEntry(M_EX_STRP1); 
      }
      if (fExS2)
      {
         delete fExS2;
         fExS2 = nullptr;
      }
      if (fExR1)
      {
         delete fExR1;
         fExR1 = nullptr;
      }
      if (fExR2)
      {
         delete fExR2;
         fExR2 = nullptr;
      }
      if (!fExJ1) 
         fExJ1 = new MCExerciseJ1(gClient->GetRoot(), 400, 200);
      fMenuExJPsP->DisableEntry(M_EX_JPSP1); 
      break;
   }
   case M_VIEW_ENBL_DOCK:
   {
      fMenuDock->EnableUndock(!fMenuDock->EnableUndock());
      if (fMenuDock->EnableUndock())
      {
         fMenuView->CheckEntry(M_VIEW_ENBL_DOCK);
         fMenuView->EnableEntry(M_VIEW_UNDOCK);
      }
      else
      {
         fMenuView->UnCheckEntry(M_VIEW_ENBL_DOCK);
         fMenuView->DisableEntry(M_VIEW_UNDOCK);
      }
      break;
   }
   case M_VIEW_ENBL_HIDE:
   {
      fMenuDock->EnableHide(!fMenuDock->EnableHide());
      if (fMenuDock->EnableHide())
      {
         fMenuView->CheckEntry(M_VIEW_ENBL_HIDE);
      }
      else
      {
         fMenuView->UnCheckEntry(M_VIEW_ENBL_HIDE);
      }
      break;
   }
   case M_VIEW_DOCK:
   {
      fMenuDock->DockContainer();
      fMenuView->EnableEntry(M_VIEW_UNDOCK);
      fMenuView->DisableEntry(M_VIEW_DOCK);
      break;
   }
   case M_VIEW_UNDOCK:
   {
      fMenuDock->UndockContainer();
      fMenuView->EnableEntry(M_VIEW_DOCK);
      fMenuView->DisableEntry(M_VIEW_UNDOCK);
      break;
   }
   case M_LAN_FR:
   {
      ml.SetLanguage("FR");
      printf("Français sélectionné\n");
      if (fExS1)
         fExS1->SetText();
      if (fExR1)
         fExR1->SetText();
      fMenuLang->CheckEntry(M_LAN_FR);
      fMenuLang->UnCheckEntry(M_LAN_EN);
      break;
   }
   case M_LAN_EN:
   {
      ml.SetLanguage("EN");
      printf("English Selected\n");
      if (fExS1)
         fExS1->SetText();
      if (fExR1)
         fExR1->SetText();
      fMenuLang->CheckEntry(M_LAN_EN);
      fMenuLang->UnCheckEntry(M_LAN_FR);
      break;
   }
   default:
      break;
   }
}
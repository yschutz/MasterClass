#ifndef MCMain_h
#define MCMain_h

#include <RQ_OBJECT.h>

class TGCanvas;
class TGDockableFrame;
class TGFrame;
class TGImageMap;
class TGLayoutHints;
class TGMainFrame;
class TGMenuBar;
class TGPopupMenu;
class TGWindow;
class MCExerciseJ1;
class MCExerciseR1;
class MCExerciseR2;
class MCExerciseS1;
class MCExerciseS2;

enum ECommand
{
   M_GO_EXIT,
   M_GO_HELP,
   M_EX_STR,
   M_EX_RAA,
   M_EX_STRP1,
   M_EX_STRP2,
   M_EX_RAAP1,
   M_EX_RAAP2,
   M_EX_JPSP1,
   M_EX_JPSP2,
   M_VIEW_ENBL_DOCK,
   M_VIEW_ENBL_HIDE,
   M_VIEW_DOCK,
   M_VIEW_UNDOCK,
   M_LAN_FR,
   M_LAN_EN
};

class MCMain
{
   RQ_OBJECT("MCMain")

private:
   TGImageMap       *fAliceLogo;
   TGCanvas         *fCanvasWindow;
   TGFrame          *fContainer;
   MCExerciseJ1     *fExJ1{nullptr};
   MCExerciseR1     *fExR1{nullptr};
   MCExerciseR2     *fExR2{nullptr};
   MCExerciseS1     *fExS1{nullptr};
   MCExerciseS2     *fExS2{nullptr};
   TGMainFrame      *fMain; // the main frame
   TGMenuBar        *fMenuBar;
   TGLayoutHints    *fMenuBarLayout;
   TGDockableFrame  *fMenuDock;
   TGPopupMenu      *fMenuEx;
   TGPopupMenu      *fMenuExJPsP;
   TGPopupMenu      *fMenuExRAAP;
   TGPopupMenu      *fMenuExSTRP;
   TGPopupMenu      *fMenuGo;
   TGPopupMenu      *fMenuLang;
   TGPopupMenu      *fMenuView;

public:
   MCMain(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~MCMain();

   //slots
   void    CloseWindow();
   // void Created() { Emit("Created()"); } //*SIGNAL*
   void    HandleLang(ECommand id);
   void    HandleMenu(ECommand id);
   // void HandlePopup()
   // { /*printf("menu popped up\n"); */
   // }
   // void HandlePopdown()
   // { /*printf("menu popped down\n"); */
   // }
   // void Welcome()              {printf("Master Class GUI has been created \n");}

   ClassDef(MCMain, 1);
};
#endif

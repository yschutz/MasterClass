#ifndef MCTutorial_h
#define MCTutorial_h

#include "MCExercise.h"
#include <RQ_OBJECT.h>
#include <TVector3.h>

class TEveLine;
class TTimer;  
class MCTutorial: public MCExercise
{
    RQ_OBJECT("MCTutorial")
  private:
    TTimer   *fTimer            {nullptr}; 
    TEveLine *fTrack            {nullptr}; 
    Int_t    fTrackSegmentStep  {1}; 
    TVector3 fTrackDirection; 

    void   CustomizeBrowser(); 
    void   DeleteHistograms(){;} 
    void   DrawTrack();
    void   MakeHistograms(){;} 

  public:
    MCTutorial(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCTutorial(){;} 

    void   DrawTrackSegment();  


    ClassDef(MCTutorial, 1);
};
#endif
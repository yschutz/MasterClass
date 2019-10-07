#ifndef MCTutorial_h
#define MCTutorial_h

#include "MCExercise.h"
#include <RQ_OBJECT.h>
class MCTutorial: public MCExercise
{
    RQ_OBJECT("MCTutorial")
  private:
    void   DeleteHistograms(){;} 
    void   MakeHistograms(){;} 

  public:
    MCTutorial(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCTutorial(){;} 


    ClassDef(MCTutorial, 1);
};
#endif
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
    Double_t fBz                {0.0}; 
    Double_t fCharge; 
    Double_t fMass;  
    Int_t    fNSteps; 
    Double_t fOmega; 
    Double_t fPt                {2.0}; 
    Double_t fRadius; 
    TTimer   *fTimer            {nullptr}; 
    TEveLine *fTrack            {nullptr}; 
    Int_t    fTrackSegmentStep  {1}; 
    TVector3 fTracklet; 
    TVector3 fTrackMomentum; 
    Double_t fUnitNorm; 

    void   CustomizeBrowser(); 
    void   DeleteHistograms(){;} 
    void   DrawTrack();
    void   MakeHistograms(){;} 
    void   TrackStep(Float_t &x, Float_t &y, Float_t &z); 

  public:
    MCTutorial(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCTutorial(){;} 

    void   DrawTrackSegment();  


    ClassDef(MCTutorial, 1);
};
#endif
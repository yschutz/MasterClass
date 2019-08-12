#ifndef MCExerciseR1_h
#define MCExerciseR1_h

#include "MCExercise.h"
#include <RQ_OBJECT.h>

class TCanvas;
class TEveElement; 
class TGLabel; 
class TGTextButton; 
class TH1D; 
class MCExerciseR1 : public MCExercise
{
    RQ_OBJECT("MCExerciseR1")
private:
    TGLabel            *fDataSetLabel        { nullptr };
    TGLabel            *fEventAnalysed       { nullptr };
    TGLabel            *fEventNumber         { nullptr };
    Int_t              fCollSys;     
    Int_t              fCount                { 0 };     
    TH1D               *fMultHist            { nullptr }; 
    TH1D               *fMultHistMinPt       { nullptr };
    Int_t              fNPrim;
	Int_t              fNPrimMinPt;
    TCanvas*           fPad                  { nullptr };
    TGTextButton       *fPrimaryTracksButton { nullptr };             
    Bool_t             fPrimaryTracksLoaded  { kFALSE };             
    TH1D               *fPtHist              { nullptr }; 
    TH1D               *fChargeHist          { nullptr };
     
    void   CustomizeBrowser();  
    void   DeleteHistograms(); 
    void   LoadEsdTracks(Int_t maxR); 
    void   LoadPrimaryTracks(Int_t maxR);
    void   MakeHistograms(); 
    void   StartExercise();  
    void   StudentSet(); 

public:
    MCExerciseR1(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCExerciseR1(); 

    void   Counter(); 
    void   CountEvents(); 
    void   DisplayPrimaryTracks(); 
    void   EveSelection(TEveElement *el);
    Int_t  GetPrimaries() const           { return fNPrim; }
    Int_t  GetPrimariesMinPt() const      { return fNPrimMinPt; }
    Bool_t GotoEvent(Int_t evt); 
    void   Instructions();
    void   NextEvent(); 
    void   PrevEvent();
    void   PublishMult();  

    ClassDef(MCExerciseR1, 1);
}; 
#endif
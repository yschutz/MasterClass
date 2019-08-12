// ALICE Counter for Master Class exercise RAA

#ifndef MCAliCounter_h
#define MCAliCounter_h

#include <RQ_OBJECT.h>

#include <TGFrame.h>  
#include <TGNumberEntry.h>

class MCExerciseR1; 
class TGLabel; 
class MCAliCounter
{
RQ_OBJECT("MCAliCounter")

private:
    TGLabel                *fAutomaticCount      { nullptr }; 
    TGLabel                *fAutomaticCountMinPt { nullptr }; 
    Bool_t                 fChange;   
    TGNumberEntryField     *fCharge              { nullptr };
    static MCAliCounter    fInstance;       
    TGMainFrame            *fMainFrame;        
    MCExerciseR1           *fMother; 
    TGNumberEntryField     *fMult                { nullptr };
    TGNumberEntryField     *fMultMinPt           { nullptr };
    TGNumberEntryField     *fPt                  { nullptr };
    TGNumberEntryField     *fPx                  { nullptr };
    TGNumberEntryField     *fPy                  { nullptr };
    TGNumberEntryField     *fPz                  { nullptr };
    Bool_t                 fStarted; 


    MCAliCounter(); 
    MCAliCounter(const MCAliCounter &) {}
    virtual ~MCAliCounter()            { fStarted = kFALSE; }

public:
    void                 CountAutomatic(); 
    void                 DoExit()                 { fMainFrame->UnmapWindow(); }
    Int_t                GetCharge() const        { return fCharge->GetNumber(); }     
    Int_t                GetMult() const          { return fMult->GetNumber(); }                
    Int_t                GetMultMinPt() const     { return fMultMinPt->GetNumber(); }   
    Double_t             GetPt() const            { return fPt->GetNumber(); }     
    Bool_t               HasChanged() const       { return fChange; }    
    void                 IncreaseMult();
    void                 IncreaseMult2(Double_t px, Double_t py);
    static MCAliCounter  &Instance()              {	return fInstance; }
    void                 Instructions();
    Bool_t               IsStarted() const        { return fStarted; }
    void                 SetChanged(Bool_t ch)    { fChange = ch; }
    void                 Set1(Double_t px, Double_t py, Double_t pz, Int_t charge); 
    void                 Set2(Double_t px, Double_t py, Double_t pz); 
    void                 SetZero(); 
    void                 SetZero2(); 
    void                 Start(MCExerciseR1 * mother, Int_t collSys); 

ClassDef(MCAliCounter, 1);
};
#endif
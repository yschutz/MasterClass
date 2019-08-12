// ALICE Calculator for Master Class exercise Strangeness

#ifndef MCAliCalculator_h
#define MCAliCalculator_h

#include <RQ_OBJECT.h>

#include <TGFrame.h>  

class MCExerciseS1; 
class TGNumberEntryField;
class TGTextButton;  
class MCAliCalculator 
{
RQ_OBJECT("MCAliCalculator")
private:
    TGTextButton           *fBtxtLR; 
    TGTextButton           *fBtxtSR; 
    Bool_t                 fChange;   
    static MCAliCalculator fInstance;
    TGNumberEntryField     *fInvariant_masscalc;;  
    TGMainFrame            *fMainFrame; 
    MCExerciseS1           *fMother; 
    TGNumberEntryField     *fMass1;  
    TGNumberEntryField     *fMass2;  
    TGNumberEntryField     *fMass3;  
    TGNumberEntryField     *fPx1;  
    TGNumberEntryField     *fPx2;  
    TGNumberEntryField     *fPx3;  
    TGNumberEntryField     *fPy1;  
    TGNumberEntryField     *fPy2;  
    TGNumberEntryField     *fPy3;  
    TGNumberEntryField     *fPz1;  
    TGNumberEntryField     *fPz2;  
    TGNumberEntryField     *fPz3;  
    Bool_t                 fStarted; 
    MCAliCalculator(); 
    MCAliCalculator(const MCAliCalculator &) {}
    virtual ~MCAliCalculator()               { fStarted = kFALSE; }

public: 
    void                     BlockButtons(Bool_t state); 
    Double_t                 GetInvMass() const; 
    Bool_t                   HasChanged() const         { return fChange; }
    static MCAliCalculator & Instance()                 { return fInstance; } 
    Bool_t                   IsStarted() const          { return fStarted; }
    void                     Set1(Double_t px, Double_t py, Double_t pz, Double_t mass); 
    void                     Set2(Double_t px, Double_t py, Double_t pz, Double_t mass); 
    void                     Set3(Double_t px, Double_t py, Double_t pz, Double_t mass); 
    void                     SetChanged(Bool_t ch)      { fChange = ch; }
    void                     Start(MCExerciseS1 * mother); 

//slots
    void                     Background()               {}
    void                     CalculateInvariantMass(); 
    void                     DoExit()                   { fMainFrame->UnmapWindow(); }
    void                     Instructions();
    void                     SetZero(); 

ClassDef(MCAliCalculator, 1);
};
#endif
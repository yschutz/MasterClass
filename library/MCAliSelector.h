#ifndef MCAliSelector_h
#define MCAliSelector_h

#include <RQ_OBJECT.h>

#include <TGFrame.h>  
#include <TGNumberEntry.h>

class MCExerciseJ1; 
class TGMainFrame; 
class TGNumberEntryField; 
class MCAliSelector 
{
RQ_OBJECT("MCAliSelector")
private:
    Bool_t               fChange;   
    TGNumberEntry        *fDeMin; 
    TGNumberEntry        *fDeMax; 
    static MCAliSelector fInstance;
    TGMainFrame          *fMainFrame; 
    MCExerciseJ1         *fMother; 
    TGNumberEntry        *fPMin; 
    TGNumberEntry        *fPMax; 
    Bool_t               fStarted; 


    MCAliSelector(); 
    MCAliSelector(const MCAliSelector &) {}
    virtual ~MCAliSelector()             { fStarted = kFALSE; }

public:
    void                     DoExit()                   { fMainFrame->UnmapWindow(); }
    void                     DoOK(); 
    Double_t                 GetDeMin() const           { return fDeMin->GetNumber(); }
    Double_t                 GetDeMax() const           { return fDeMax->GetNumber(); }
    Double_t                 GetPMin() const            { return fPMin->GetNumber(); }
    Double_t                 GetPMax() const            { return fPMax->GetNumber(); }
    Bool_t                   HasChanged() const         { return fChange; }
    static MCAliSelector     &Instance()                { return fInstance; }; 
    void                     Instructions(); 
    Bool_t                   IsStarted() const          { return fStarted; }
    void                     SetAll(Double_t pmin, Double_t pmax, Double_t demin, Double_t demax) {
                                                                                fPMin->SetNumber(pmin); 
                                                                                fPMax->SetNumber(pmax); 
                                                                                fDeMin->SetNumber(demin); 
                                                                                fDeMax->SetNumber(demax);} 
    void                     SetChanged(Bool_t ch)      { fChange = ch; }
    void                     SetMinMax(); 
    void                     Start(MCExerciseJ1 * mother); 


ClassDef  (MCAliSelector, 1);
};
#endif
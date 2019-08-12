#ifndef MCAliExtractor_h
#define MCAliExtractor_h

#include <RQ_OBJECT.h>

#include <TGFrame.h>  
#include <TGNumberEntry.h>

class MCExerciseJ1; 
class TGNumberEntry; 
class TGNumberEntryField; 
class MCAliExtractor 
{
RQ_OBJECT("MCAliExtractor")
private:
    static MCAliExtractor fInstance;
    TGNumberEntryField   *fJPsiCount    { nullptr };
    TGMainFrame          *fMainFrame    { nullptr }; 
    TGNumberEntry        *fMMin         { nullptr }; 
    TGNumberEntry        *fMMax         { nullptr }; 
    MCExerciseJ1         *fMother       { nullptr }; 
    TGNumberEntryField   *fSignificance { nullptr };
    TGNumberEntryField   *fSoB         { nullptr };
    Bool_t               fStarted       { kFALSE }; 


    MCAliExtractor(); 
    MCAliExtractor(const MCAliExtractor &) {}
    virtual ~MCAliExtractor()              { fStarted = kFALSE; }

public:
    void                      DoExit()                           { fMainFrame->UnmapWindow(); }
    Double_t                  GetMMax() const                    { return fMMax->GetNumber(); }
    Double_t                  GetMMin() const                    { return fMMin->GetNumber(); }
    static MCAliExtractor     &Instance()                        { return fInstance; } 
    void                      Instructions(); 
    Bool_t                    IsStarted() const                  { return fStarted; }
    void                      SetAll(Double_t min, Double_t max, Int_t njpsi = 0, Double_t sig = 0.0, Double_t sb = 0.0) 
                                                                 { fMMin->SetNumber(min); 
                                                                   fMMax->SetNumber(max);
                                                                   fJPsiCount->SetNumber(njpsi);
                                                                   fSignificance->SetNumber(sig); 
                                                                   fSoB->SetNumber(sb); } 
    void                      SetMinMax();
    void                      Start(MCExerciseJ1 * mother); 

ClassDef(MCAliExtractor, 1);
};
#endif
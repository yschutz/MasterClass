#ifndef MCExerciseS2_h
#define MCExerciseS2_h

#include "MCExercise.h"
#include <TGButton.h>

#include <RQ_OBJECT.h>

class TEveGDoubleValuator;
class TF1; 
class TGTGCheckButton; 
class TGWindow; 
class TH1D; 
class TLatex; 
class TPad; 
class TString; 
enum E_SystemAndEvtActivity {
            kpp_MB = 0,
            kPbPb_MB,
            kPbPb_0010,
            kPbPb_1020,
            kPbPb_2030,
            kPbPb_3040,
            kPbPb_4050,
            kPbPb_5060,
            kPbPb_6070,
            kPbPb_7080,
            kPbPb_8090,
            kPbPb_90100,
            kNbSystemsAndEvtActivity                
};       
class MCExerciseS2 : public MCExercise
{
    RQ_OBJECT("MCExerciseS2")
private:
    TString                              fSysActName[kNbSystemsAndEvtActivity]; 
    Int_t                                fCurrentAct     { 0 }; 
    TString                              fCurrentLabel   { "" }; 
    Int_t                                fCurrentPart    { 0 }; 
    TF1                                  *fFitGauss      { nullptr }; 
    TF1                                  *fFitPoly       { nullptr }; 
    TEveGDoubleValuator                  *fFitRangeGauss { nullptr }; 
    TEveGDoubleValuator                  *fFitRangePoly  { nullptr }; 
    Int_t                                fFitResult      { 0 };
    TLatex                               *fLabel         { nullptr }; 
    TH1D                                 *fMinvHisto     { nullptr };
    TH1D                                 *fMinvHistAL    { nullptr };
    TH1D                                 *fMinvHistALL   { nullptr };
    TH1D                                 *fMinvHistK     { nullptr };
    TH1D                                 *fMinvHistL     { nullptr };
    TH1D                                 *fMinvHistX     { nullptr };
    TGMainFrame                          *fMF            { nullptr }; 
    const Int_t                          fkNpart         { 3 }; 
    TString                              fMassInfo       { "" }; 
    TPad                                 *fPad           { nullptr };
    std::vector<std::vector<Int_t>>      fResults;
    std::vector<std::vector<TString>>    fVecListHistos;
    std::map<TString,TGCheckButton*>     fTabCheckButton;

    void                                 DeleteHistograms(); 
    TString                              FormatText(const TString &text, Bool_t tit = kTRUE);

public:
    MCExerciseS2(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCExerciseS2();    

    void CustomizeBrowser(); 
    void DisplayResults(); 
    void FitInvariantMass(); 
    void LoadHisto(Int_t iPart, Int_t iHist); 
    void MakeHistograms(); 
    void ReadHistoFile(const char *fileMame); 
    void RefreshResults(); 
    void SaveResult(); 
    void StartExercise(); 
    void UntickButtons();

    ClassDef(MCExerciseS2, 1);
};
#endif

#ifndef MCExerciseJ1_h
#define MCExerciseJ1_h

#include "MCExercise.h"
#include <RQ_OBJECT.h>

class TCanvas; 
class TGCheckButton;
class TGNumberEntryField;
class TGTextButton; 
class TH1F; 
class TH2F; 
class TLine; 
class MCExerciseJ1 : public MCExercise
{
    RQ_OBJECT("MCExerciseJ1")

  private:

    Bool_t             fApplyTrackCuts          { kFALSE }; 
    Double_t           fArrP[300]               { 0.0}; 
    Double_t           fArrdEdx[300]            { 0.0}; 
    TGTextButton       *fButtonApplyTrackCuts   { nullptr };
    TGTextButton       *fButtonDefineTrackCuts  { nullptr };
    TGTextButton       *fButtonFillInvMass      { nullptr };
    TGTextButton       *fButtonFillPidHistos    { nullptr };
    TGTextButton       *fButtonJumpEvents       { nullptr };
    TGTextButton       *fButtonLoadTracks       { nullptr }; 
    TGTextButton       *fButtonQuickAnalysis    { nullptr }; 
    Double_t           fDeMax                   { 140. };
    Double_t           fDeMin                   { 20. };
    Double_t           fElectrons[100][4]       {{ 0. }};
    Bool_t             fEnableQuickAnalysis     { kFALSE };
    TH1F               *fhElectrons             { nullptr }; 
    TH1F               *fhEleEle                { nullptr }; 
    TH2F               *fhEnergyLoss            { nullptr }; 
    TH1F               *fhJPsi                  { nullptr }; 
    TH1F               *fhMassInv               { nullptr }; 
    TH1F               *fhPositrons             { nullptr }; 
    TH1F               *fhPosPos                { nullptr }; 
    TLine              *fLine[6]                { nullptr };
    Double_t           fMMax                    { 6. };  
    Double_t           fMMin                    { 0. };  
    Int_t              fNElectrons              { 0 }; 
    Int_t              fNEvents                 { 0 }; 
    Int_t              fNJPsi                   { 0 }; 
    TGNumberEntryField *fNLoop                  { nullptr };
    Int_t              fNPositrons              { 0 }; 
    Int_t              fNSelectedTracks         { 0 }; 
    Double_t           fPMax                    { 10. };
    Double_t           fPMin                    { 0. };
    TCanvas            *fPad                    { nullptr }; 
    TCanvas            *fPad2                   { nullptr }; 
    Double_t           fPositrons[100][4]       {{ 0. }};
    Double_t           fSignificance            { 0. }; 
    Double_t           fSoB                     { 0. }; 

    void CustomizeBrowser();  
    void DeleteHistograms(); 
    void DemoSet() {}
    void DrawELoss(); 
    void DrawInvMass(); 
    void LoadEsdTracks(Int_t maxR);
    void MakeHistograms();
    void ResetDedxArray(); 
    void ResetMatrices(); 
    void StudentSet(); 
    void TutorSet() {}

  public:
    MCExerciseJ1(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCExerciseJ1();

    void   ApplyTrackCuts();
    void   BeginAgain();
    void   CheckNLoop();   
    void   ClearHisto(); 
    void   Extractor(); 
    void   FillEnergyLossHisto(); 
    void   Instructions(); 
    void   EnableTrackCutsButton(); 
    void   EventsJump(); 
    void   FillInvariantMassHistos();   
    Bool_t GotoEvent(Int_t evt); 
    void   NextEvent(); 
    void   PlotPIDLines(); 
    void   PrevEvent(); 
    void   Selector(); 
    void   SetPIDCutValues(); 
    void   StartExercise(); 
    void   SwapApplyTrackCuts(); 
    void   SwapQuickAnalysis(); 
    void   WrapUp(); 

    ClassDef(MCExerciseJ1, 1);

};
#endif
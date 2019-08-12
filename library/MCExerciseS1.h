#ifndef MCExerciseS1_h
#define MCExerciseS1_h

#include "MCExercise.h"
#include <RQ_OBJECT.h>

#include <TGFrame.h>  
#include <TGLabel.h>

class MCMultiView;
class TCanvas; 
class TEveElement; 
class TEveLine; 
class TEvePointSet; 
class TEveTrackList; 
class TGCheckButton; 
class TGImageMap;
class TGLabel;
class TGLOverlayButton; 
class TGNumberEntryField; 
class TGWindow;
class TH1D;
class TList;
class TRootHelpDialog;
class TVSReader;

enum E_DETId
{
    D_ALICE, 
    D_ITS, 
    D_TPC, 
    D_TRDTOF,
    D_PHOS,
    D_HMPID   
};
class MCExerciseS1 : public MCExercise
{
    RQ_OBJECT("MCExerciseS1")

  private:
    TGMainFrame        *fADI                         { nullptr };
    TGMainFrame        *fCSP                         { nullptr };
    TGMainFrame        *fCSP2                        { nullptr };
    Bool_t             fCascadesLoaded               { kFALSE }; 
    Int_t              fCascadeTracks                { 0 }; 
    TGCheckButton      *fCheckCascades               { nullptr }; 
    TGCheckButton      *fCheckV0s                    { nullptr }; 
    Int_t              fCount                        { 0 };
    Int_t              fCountALambdas                { 0 }; 
    Int_t              fCountKaons                   { 0 };
    Int_t              fCountLambdas                 { 0 }; 
    Int_t              fCountOmegas                  { 0 }; 
    Int_t              fCountXis                     { 0 };
    // TString            fDataSetFileName              { "Data-Masterclass/events/Strangeness/Part1/" }; 
    Int_t              fDetector                     { 0 };
    TGNumberEntryField *fEntryC                      { nullptr }; 
    TGNumberEntryField *fEntryM                      { nullptr }; 
    TGNumberEntryField *fEntryX                      { nullptr }; 
    TGNumberEntryField *fEntryY                      { nullptr }; 
    TGNumberEntryField *fEntryZ                      { nullptr }; 
    Int_t              fLoads                        { 0 }; 
    const Double_t     kfMassElectron                { 0.5109989461 }; 
    const Double_t     kfMassPion                    { 0.13957061 }; 
    const Double_t     kfMassProton                  { 0.938272081 }; 
    TGLabel            *fLabel;
    const char         *fMasterClassOutputData[4]    { "MasterClass Output Data", "*.masterclass", 0, 0}; 
    const char         *fMasterClassOutputPicture[4] { "MasterClass Histograms",  "*.png",         0, 0};
    Double_t           fMassPos[10]                  {  };
    Double_t           fMassPosCas[10]               {  };
    Double_t           fMinvALambda[1000]            {  }; 
    Double_t           fMinvLambda[1000]             {  }; 
    Double_t           fMinvKaon[1000]               {  }; 
    Double_t           fMinvOmega[1000]              {  }; 
    Double_t           fMinvXi[1000]                 {  }; 
    TH1D               *fMinvHistAL                  { nullptr };
    TH1D               *fMinvHistALL                 { nullptr };
    TH1D               *fMinvHistK                   { nullptr };
    TH1D               *fMinvHistL                   { nullptr };
    TH1D               *fMinvHistX                   { nullptr };
    TCanvas            *fStudentSetCanvas            { nullptr }; 
    TGMainFrame        *fPAI                         { nullptr };  
    TGMainFrame        *fPAI2                        { nullptr };  
    TGMainFrame        *fPAIW                        { nullptr };  
    Int_t              fPattern                      { 0 };
    Double_t           fPcaX[10]                     {  };
    Double_t           fPcaY[10]                     {  };
    Double_t           fPcaZ[10]                     {  };
    Double_t           fPcaXCas[10]                  {  };
    Double_t           fPcaYCas[10]                  {  };
    Double_t           fPcaZCas[10]                  {  };
    Double_t           fPcaXBacCas[10]               {  };
    Double_t           fPcaYBacCas[10]               {  };
    Double_t           fPcaZBacCas[10]               {  };
    TEveLine           *fPointingLine[10]            {  };
    TEveLine           *fPointingLine1[10]           {  };
    TEveLine           *fPointingLine2[10]           {  };    
    TEvePointSet       *fPsV0                        { nullptr };
    TEvePointSet       *fPsCascade                   { nullptr };
    Double_t           fPxNeg[10]                    {  };
    Double_t           fPyNeg[10]                    {  };
    Double_t           fPzNeg[10]                    {  };
    Double_t           fMassNeg[10]                  {  };
    Double_t           fPxBacCas[10]                 {  };
    Double_t           fPyBacCas[10]                 {  };
    Double_t           fPzBacCas[10]                 {  };
    Double_t           fMassBacCas[10]               {  };
    Double_t           fChargeBacCas[10]             {  };
    Double_t           fPxNegCas[10]                 {  };
    Double_t           fPyNegCas[10]                 {  };
    Double_t           fPzNegCas[10]                 {  };
    Double_t           fMassNegCas[10]               {  };
    Double_t           fPxPos[10]                    {  };
    Double_t           fPyPos[10]                    {  };
    Double_t           fPzPos[10]                    {  };
    Double_t           fPxPosCas[10]                 {  };
    Double_t           fPyPosCas[10]                 {  };
    Double_t           fPzPosCas[10]                 {  };
    TGMainFrame        *fTIW                         { nullptr };
    TGMainFrame        *fTMF                         { nullptr };
    TEveTrackList      *fTrackListCascade            { nullptr }; 
    TEveTrackList      *fTrackListCascadeMother      { nullptr }; 
    TEveTrackList      *fTrackListCascadeNeg         { nullptr }; 
    TEveTrackList      *fTrackListCascadePos         { nullptr }; 
    TEveTrackList      *fTrackListV0Neg              { nullptr };
    TEveTrackList      *fTrackListV0Pos              { nullptr };
    TEveTrackList      *fTrackListV0Mother           { nullptr };     
    TCanvas            *fTutorSetCanvas              { nullptr }; 
    Bool_t             fV0sLoaded                    { kFALSE };     
    Int_t              fV0Tracks                     { 0 }; 

    void   TrackInfoWindo(Double_t px, Double_t py, Double_t pz, Double_t mass, Int_t charge, 
                          Bool_t isTrack = kFALSE, Bool_t isBachelor = kFALSE); 
    void   CloseWindow();                       
    void   CustomizeBrowser(); 
    void   DeleteHistograms(); 
    void   MakeHistograms(); 
    void   StartExercise();
    void   StudentSet(); 
    void   TutorSet();

  public:
    MCExerciseS1(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCExerciseS1(); 

    void AutoSave(); 
    void Instructions();
    void InstructionsLevel1();
    void LoadCascades(); 
    void LoadEsdTracks(Int_t maxR); 
    void LoadV0s(); 

    //slots
    void   Calculator(); 
    void   CopyToCalc(Int_t i); 
    void   CountEvents(); 
    void   CountStrangeParticles(); 
    void   CountStrangeParticles2(); 
    void   DestroyGeometry(); 
    void   DisplayCascades();
    void   DisplayV0s(); 
    void   EveSelection(TEveElement *el);
    void   FoundPart(E_PATId part); 
    Bool_t GotoEvent(Int_t evt); 
    void   LoadSavedResults(); 
    void   MakeBiggerDet();
    void   MakeBiggerPat();
    void   MakeBiggerPat2();
    void   NextEvent(); 
    void   PatternInfo();  
    void   PatternInfo2();  
    void   PrevEvent(); 
    void   SaveHistograms();
    void   SaveResults(); 
    void   ShowDet(Int_t det);  
    void   ShowPat(Int_t pat);  
    void   ShowPat2(Int_t pat);  
    void   ToDemo (); 
    void   ToStudent(); 
    void   UploadResults(); 

    ClassDef(MCExerciseS1, 1);
};
#endif
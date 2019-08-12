#ifndef MCExerciseR2_h
#define MCExerciseR2_h

#include "MCExercise.h"

#include <TString.h>

#include <RQ_OBJECT.h>

class TGCheckButton;
class TGMainFrame; 
class TGNumberEntry; 
class TCanvas; 
class TH1; 
class TH1D; 
class TH1F; 
class TH2F; 
class TTree; 
enum E_CENTBIN {
            kCENT_0005,
            kCENT_0010,
            kCENT_0510,
            kCENT_1020,
            kCENT_2030,
            kCENT_3040,
            kCENT_4050,
            kCENT_5060,
            kCENT_6070,
            kCENT_7080,
            kCENT_8090,
            kCENT_00100,
            kNBCENTBINS
};       

class MCExerciseR2 : public MCExercise
{
    RQ_OBJECT("MCExerciseR2")
private:

    Double_t      fBinsPt[55]                   { 0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
                                                  0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
                                                  1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
                                                  2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8,
                                                  4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9, 10,
                                                  11, 12, 13, 14, 15}; 
    Int_t         fCentralityHigh               { 100 };
    Int_t         fCentralityLow                { 0 }; 
    TGMainFrame   *fCentralitySelector;
    TGCheckButton *fCentCheck[kNBCENTBINS]; 
    TString       fCentCheckName[kNBCENTBINS]   {"00-05 %", "00-10 %", "05-10 %", "10-20 %", "20-30 %", "30-40 %",
                                                 "40-50 %", "50-60 %", "60-70 %", "70-80 %", "80-90 %", "0-90 %"}; 
    Int_t         fCentCheckValH[kNBCENTBINS]   { 5, 10, 10, 20, 30, 40, 50 ,60, 70, 80, 90, 90};
    Int_t         fCentCheckValL[kNBCENTBINS]   { 0, 0, 5, 10, 20, 30, 40, 50 ,60, 70, 80, 0 };
    TString       fCollSystem; 
    Int_t         fCurrentCentBin; 
    ULong64_t     fEntriesTestE                 { 10000 };
    ULong64_t     fEntriesTestT                 { 10000 };
    TGNumberEntry *fEntriesTestEntryE           { nullptr }; 
    TGNumberEntry *fEntriesTestEntryT           { nullptr }; 
    TTree         *fEvent                       { nullptr }; 
 
    TH1D          *fHDeltaPt;    
    TH1F          *fHNTPC[kNBCENTBINS]; 
    TH2F          *fHNTPCvsCent;
    TH1D          *fHTrackPt[kNBCENTBINS]; 
    TGTextButton  *fMakeRAAButton               { nullptr }; 
    Int_t         fNBinsPt                      { 54 };
    Double_t      fNColl[kNBCENTBINS]           { 1686.87, 1502.7, 1319.89, 923.26, 558.68,
                                                  321.20, 171.67, 85.13, 38.51, 15.78, 6.32, 5127.31};
    TCanvas       *fPad; 
    TTree         *fTrack                       { nullptr }; 

    void          DeleteHistograms(); 
    void          DemoSet(); 
    void          MakeHistograms();
    void          SetHistoMarkerAndColor(TH1* histo1, Style_t markerStyle, Size_t markerSize, Color_t markerColor, Color_t lineColor);
    void          StartExercise();
    void          StudentSet(); 
    void          StyleSettings(); 

public:
    MCExerciseR2(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCExerciseR2(); 

    void   ContinueExercise(); 
    void   MakeRAA(); 
    void   SetCentBin(Int_t low, Int_t high, Int_t binid); 
    void   SetStatistics(); 
    ClassDef(MCExerciseR2, 1);
}; 
#endif
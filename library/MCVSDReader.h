#ifndef MCVSDReader_h
#define MCVSDReader_h

#include <RQ_OBJECT.h>

class TDirectory;
class TEveTrackList;  
class TEveVSD; 
class TFile; 
class TGLOverlayButton; 
class TObjArray; 
class TTimer; 
class MCVSDReader
{
RQ_OBJECT("MCVSDReader")

private:
    Int_t              fCurEv                          { -1 };
    TDirectory         *fDirectory                     { nullptr };
    TObjArray          *fEvDirKeys                     { nullptr };
    TFile              *fFile                          { nullptr };
    static MCVSDReader fInstance;  
    Int_t              fMaxEv                          { -1 }; 
    Int_t              fMaxR                           { 400 };
    // Int_t              fRAnim                          { 0 }; 
    // TTimer             *fTimer                         { nullptr }; 
    // TEveTrackList      *fTrackList                     { nullptr }; 
    TEveVSD            *fVSD                           { nullptr }; 

    MCVSDReader()                     {} 
    MCVSDReader (const MCVSDReader &) {}
    virtual ~MCVSDReader(); 

public:
    Int_t                CurEv() const                  { return fCurEv; }
    void                 DropEvent(); 
    TEveVSD *            GetEveVSD() const              { return fVSD; }                
    Bool_t               GotoEvent(Int_t ev); 
    void                 Init(const TString &fileName); 
    static MCVSDReader & Instance()                     { return fInstance; } 
    Int_t                MaxEv() const                  { return fMaxEv; }
    void                 Reset(); 

//slots
// void                     AnimateEsdTracks(); 
}; 
#endif
// to display information on selected ALICE detector
// Yves Schutz June 2019
#ifndef MCAliceDetectorInfoWindow_h
#define MCAliceDetectorInfoWindow_h

#include <RQ_OBJECT.h>
#include <TGFrame.h>

class TGPictureButton; 

class MCAliceDetectorInfoWindow
{
RQ_OBJECT("MCAliceDetectorInfoWindow")
private:
    TGPictureButton                  *fPB       { nullptr };
    static MCAliceDetectorInfoWindow fInstance; //unique instance of the class
    TGMainFrame                      *fMainFrame; 

    MCAliceDetectorInfoWindow();
    MCAliceDetectorInfoWindow (const MCAliceDetectorInfoWindow &) {}
    virtual ~MCAliceDetectorInfoWindow();
    void Init(const TGWindow *p, UInt_t w, UInt_t h, Int_t detector);

public:
    static MCAliceDetectorInfoWindow & Instance(); 
 
ClassDef(MCAliceDetectorInfoWindow, 1); 
};
#endif
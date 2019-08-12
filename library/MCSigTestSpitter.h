// to display information on selected track
// Yves Schutz June 2019
#ifndef MCSigTestSpitter_h
#define MCSigTestSpitter_h

#include <RQ_OBJECT.h>
#include <TEveElement.h>

class TEveSelection;
class TString; 

class MCSigTestSpitter
{
RQ_OBJECT("MCSigTestSpitter")
private:
    static MCSigTestSpitter fInstance; //unique instance of the class
    TString                 fPrefix; 
    TEveSelection           *fSel      { nullptr }; 

    MCSigTestSpitter();
    MCSigTestSpitter (const MCSigTestSpitter &) {}
    virtual ~MCSigTestSpitter();

public:
    void                      Added(TEveElement * el); 
    static MCSigTestSpitter & Instance(); 
    void                      Set(TEveSelection *sel, const TString &prefix); 

ClassDef(MCSigTestSpitter, 1); 
};
#endif
// to display information on selected track
// Yves Schutz June 2019

#include "MCSigTestSpitter.h"
#include <TEveSelection.h>
#include <TString.h>

MCSigTestSpitter MCSigTestSpitter::fInstance = MCSigTestSpitter(); 
//=MCSigTestSpitter========================================================================
MCSigTestSpitter::MCSigTestSpitter()
{
}

//=MCSigTestSpitter========================================================================
MCSigTestSpitter::~MCSigTestSpitter()
{
}

//=MCSigTestSpitter========================================================================
void MCSigTestSpitter::Added(TEveElement* el)
{
    if (el->GetElementName() == TString::Format("ITS"))
       ;//new AliceDetectorInfoWindow(gClient->GetRoot(),100,100, 1);
}

//=MCSigTestSpitter========================================================================
MCSigTestSpitter & MCSigTestSpitter::Instance()
{
    return fInstance; 
}

//=MCSigTestSpitter========================================================================
void MCSigTestSpitter::Set(TEveSelection *sel, const TString &prefix)
{
    fSel    = sel;
    fPrefix = prefix; 
    fSel->Connect("SelectionAdded(TEveElement *)", "MCSigTestSpitter", this, "Added(TEveElement *)");
}
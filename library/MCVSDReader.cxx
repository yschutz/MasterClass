#include "MCExerciseS1.h"
#include "MCMain.h"
#include "MCMultiView.h"
#include "MCVSDReader.h"

#include <TApplication.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveVSD.h>
#include <TEveViewer.h>
#include <TFile.h>
#include <TGLOverlayButton.h>
#include <TGLViewer.h>
#include <TGMsgBox.h>
#include <TKey.h>
#include <TObjArray.h>
#include <TRootHelpDialog.h>
#include <TTime.h>
#include <TPRegexp.h>

#include <iostream>

MCVSDReader MCVSDReader::fInstance = MCVSDReader();
//=MCVSDReader========================================================================
MCVSDReader::~MCVSDReader()
{
    // DropEvent();
    if (fFile) 
        fFile->Close(); 
    // if (fEvDirKeys)
    // {
    //     delete fEvDirKeys;
    //     fEvDirKeys = nullptr;
    // }
}
//=MCVSDReader========================================================================
// void MCVSDReader::AnimateEsdTracks()
// {
//     TEveTrackPropagator *trkProp = fTrackList->GetPropagator();
//     trkProp->SetMaxR(fRAnim); // R[cm]
//     fRAnim +=10;
//     gEve->AddElement(fTrackList);

//     TEveElement *top = gEve->GetCurrentEvent();

//     MCMultiView::Instance().DestroyEventRPhi();
//     MCMultiView::Instance().ImportEventRPhi(top);

//     MCMultiView::Instance().DestroyEventRhoZ();
//     MCMultiView::Instance().ImportEventRhoZ(top);

//     gEve->Redraw3D(kFALSE, kTRUE);
//     if (fRAnim < fMaxR )
//         fTimer->Stop();

// }
//=MCVSDReader========================================================================
void MCVSDReader::DropEvent()
{
    // Drop currently held event data, release current directory.

    // Drop old visualization structures.
    if (gEve)
    {
        if (gEve->GetViewers())
            gEve->GetViewers()->DeleteAnnotations();
        if (gEve->GetCurrentEvent())
            gEve->GetCurrentEvent()->DestroyElements();
    }
    // Drop old event-data.
    if (fVSD)
    {
        fVSD->DeleteTrees();
        fDirectory = nullptr;
    }
}

//=MCVSDReader========================================================================
Bool_t MCVSDReader::GotoEvent(Int_t ev)
{
    if (ev < 0 || ev > fMaxEv)
    {
        TString msg(TString::Format("Invalid event number %d; max = %d\n", ev, fEvDirKeys->GetEntriesFast()));
        new TGMsgBox(gClient->GetRoot(), 0, "MCVSDReader::GotoEvent", msg, kMBIconExclamation, kMBOk);
        gApplication->Terminate();
    }
    // if (fTimer)
    //     fTimer->Stop();
    // if (fRAnim)
    //     fRAnim = 0;
    DropEvent();
    fCurEv = ev;
    fDirectory = (TDirectory *)((TKey *)fEvDirKeys->At(fCurEv))->ReadObj();
    fVSD->SetDirectory(fDirectory);
    fVSD->LoadTrees();
    fVSD->SetBranchAddresses();
    return kTRUE;
}

//=MCVSDReader========================================================================
void MCVSDReader::Reset()
{
    DropEvent();
    if (fVSD)
    {
        delete fVSD;
        fVSD = nullptr;
    }
    // if (fTimer)
    // {
    //     delete fTimer;
    //     fTimer = nullptr;
    // }
    fCurEv = -1;
    fMaxEv = -1;
    fMaxR = 400;
    if (fFile)
        fFile->Close(); 
    fInit = kFALSE; 
}

//=MCVSDReader========================================================================
void MCVSDReader::Init(const TString &fileName)
{
    Reset();
    fFile = TFile::Open(fileName);
    if (!fFile)
    {
        TString msg(Form("Cannot open file %s\n Check your installation", fileName.Data()));
        new TGMsgBox(0, 0, "MCVSDReader:ERROR", msg, kMBIconExclamation, kMBOk);
        gApplication->Terminate();
    }
    fEvDirKeys = new TObjArray();
    TPMERegexp name_re("Event\\d+");
    TObjLink *lnk = fFile->GetListOfKeys()->FirstLink();
    while (lnk)
    {
        if (name_re.Match(lnk->GetObject()->GetName()))
            fEvDirKeys->Add(lnk->GetObject());
        lnk = lnk->Next();
    }
    fMaxEv = fEvDirKeys->GetEntriesFast();
    if (fMaxEv == 0)
    {
        // TString msg(Form("No events to show!\n Check your installation"));
        // new TGMsgBox(gClient->GetRoot(), 0, "MCVSDReader:ERROR", msg, kMBIconExclamation, kMBOk);
        return;
    }
    fVSD  = new TEveVSD();
    fInit = kTRUE; 
    // fTimer = new TTimer();
    // fTimer->Connect("Timeout()", "MCVSDReader", this, "AnimateEsdTracks()");
}

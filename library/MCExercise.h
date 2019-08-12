#ifndef MCExercise_h
#define MCExercise_h

#include <RQ_OBJECT.h>

class TEveElement;
class TEveLine; 
class TEvePointSet;
class TEveTrackList; 
class TFile; 
class TGCheckButton; 
class TGComboBox;
class TGCompositeFrame; 
class TGDockableFrame; 
class TGLabel;
class TGLOverlayButton; 
class TGMainFrame; 
class TGMenuBar;
class TGPopupMenu;
class TGTextButton;
class TGVerticalFrame; 
class TGWindow; 
class TGPictureButton; 
class TRootHelpDialog;
class TString; 
enum E_ROLE
{
    RO_NONE,
    RO_DEMO,
    RO_STUDENT,
    RO_TUTOR
};
enum E_PATId
  {
    P_KAON,
    P_LAMBDA, 
    P_ALAMBDA, 
    P_XI
};
class MCExercise
{
    RQ_OBJECT("MCExercise")
    enum E_ButtonId
    {
        M_GO_EXIT,
        M_GO_HELP
    };
protected:
    TEveLine           *fAxesX[12]                   {  }; 
    TEveLine           *fAxesY[14]                   {  }; 
    TEveLine           *fAxesZ[14]                   {  }; 
    Bool_t             fAxesLoaded                   { kFALSE };
    TGCheckButton      *fCheckClusters               { nullptr };
    TGCheckButton      *fCheckESDTracks              { nullptr };
    TGCheckButton      *fCheckVertex                 { nullptr }; 
    TGComboBox         *fData                        { nullptr };
    TFile              *fDataFile                    { nullptr };     
    Bool_t             fClustersLoaded               { kFALSE };
    TGMainFrame        *fDEI                         { nullptr };
    TString            fDataSetFileDir               { "" };
    TString            fDataSetFileName              { "" };  
    TGLabel            *fDataCheck                   { nullptr };
    TGCompositeFrame   *fDataFrame                   { nullptr };
    Int_t              fDataSet                      { -1 }; 
    TString            fDataTemplate;
    TGTextButton       *fDemo                        { nullptr };
    Bool_t             fEsdTracksLoaded              { kFALSE };  
    TGLabel            *fEventAnalysed               { nullptr };  
    TGLabel            *fEventNumber                 { nullptr };         
    const TString      fGeometryFileName             { "Data-Masterclass/geometry/alice_mini_geom.root"};
    Bool_t             fGeometrySet                  { kFALSE }; 
    TString            fGuiLogoName;        
    TRootHelpDialog    *fInstructions                { nullptr }; 
    TEvePointSet       *fITSClusters                 { nullptr };
    Bool_t             fITSClustersLoaded            { kFALSE };
    const Double_t     kfMafField                    { 0.5 }; 
    TGVerticalFrame    *fMain                        { nullptr }; 
    const Double_t     kfMaxRadius                   { 400. }; 
    TGMenuBar          *fMenuBar                     { nullptr }; 
    TGDockableFrame    *fMenuDock                    { nullptr };
    TGPopupMenu        *fMenuOpt                     { nullptr }; 
    Bool_t             fNeedsInit                    { kTRUE };
    TGLOverlayButton   *fOverlayButton               { nullptr };
    TGLOverlayButton   *fOverlayButton1              { nullptr };
    TGLOverlayButton   *fOverlayButton2              { nullptr };
    TGLOverlayButton   *fOverlayButton3              { nullptr };
    TString            fPartName[P_XI + 1]; 
    TGPictureButton    *fPictButton                  { nullptr };
    TEvePointSet       *fPsVertex                    { nullptr };
    E_ROLE             fRole                         { RO_NONE };
    TGMainFrame        *fSMF                         { nullptr };
    TGTextButton       *fStudent                     { nullptr };
    TEvePointSet       *fTOFClusters                 { nullptr };
    Bool_t             fTOFClustersLoaded            { kFALSE };
    TEvePointSet       *fTPCClusters                 { nullptr };
    Bool_t             fTPCClustersLoaded            { kFALSE };
    TEvePointSet       *fTRDClusters                 { nullptr };
    Bool_t             fTRDClustersLoaded            { kFALSE }; 
    TEveTrackList      *fTrackList                   { nullptr };
    TGTextButton       *fTutor                       { nullptr };    
    Bool_t             fVertexLoaded                 { kFALSE };
 

    void           DestroyGeometry(); 
    void           InitViewer(Bool_t withGeometry = kTRUE); 
    virtual void   DeleteHistograms() = 0; 
    virtual Bool_t GotoEvent(Int_t evt) { return kFALSE;}
    void           LoadAxes(); 
    void           LoadClusters(TEvePointSet*& ps, const TString& det_name, Int_t det_id); 
    virtual void   LoadEsdTracks(Int_t maxR) {} 
    void           LoadVertex(); 
    void           MakeGui(Bool_t dataOpt = kTRUE); 
    virtual void   MakeHistograms() = 0; 
    Bool_t         OpenDataFile(); 
    void           ResetHistograms(); 

public:
    MCExercise(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MCExercise();

    void         SetText();
    void         Start(E_ROLE); 
    virtual void StartExercise() {} 
    void         StartBrowser(); 

//slots
    void         ChangeBackgroundColor();
    void         ChangeGeometry(); 
    void         DataSelected();
    void         DetectorInfo(); 
    void         DisplayAxes();  
    void         DisplayClusters(); 
    void         DisplayTracks(); 
    void         DisplayVertex(); 
    virtual void EveSelection(TEveElement *el) {}
    void         GeometryDefault(); 
    void         HandleMenu(Int_t id);
};
#endif
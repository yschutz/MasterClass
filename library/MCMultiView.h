// Multi-view (3d, rphi, rhoz) service class using EVE Window Manager.
// Author: Matevz Tadel 2009
#ifndef MCMultiView_h
#define MCMultiView_h

#include <RQ_OBJECT.h>

#include <TEveProjectionManager.h>
#include <TEveScene.h> 
#include <TEveElement.h> 
class TEveViewer;
class MCMultiView
{
RQ_OBJECT("MCMultiView")

private:
    static MCMultiView    fInstance; 
    TEveViewer            *f3DView                { nullptr };        
    TEveScene             *fRhoZGeomScene         { nullptr };
    TEveScene             *fRhoZEventScene        { nullptr };
    TEveScene             *fRPhiGeomScene         { nullptr };
    TEveScene             *fRPhiEventScene        { nullptr };
    TEveViewer            *fRPhiView              { nullptr };
    TEveProjectionManager *fRhoZMgr               { nullptr };
    TEveProjectionManager *fRPhiMgr               { nullptr };
    TEveViewer            *fRhoZView              { nullptr };
    Bool_t                fMVSet                  { kFALSE };

    MCMultiView()                     {} 
    MCMultiView (const MCMultiView &) {}
    virtual ~MCMultiView()            {}
    void    Init();      


public:
    void                    DestroyEventRhoZ()                         { fRhoZEventScene->DestroyElements(); } 
    void                    DestroyEventRPhi()                         { fRPhiEventScene->DestroyElements(); }
    void                    DestroyGeomRhoZ()                          { fRhoZGeomScene->DestroyElements(); }    
    void                    DestroyGeomRPhi()                          { fRPhiGeomScene->DestroyElements(); }
    TEveViewer *            Get3DView() const                          { return f3DView; }
    void                    ImportEventRhoZ(TEveElement *el)           { fRhoZMgr->ImportElements(el, fRhoZEventScene); }
    void                    ImportEventRPhi(TEveElement *el)           { fRPhiMgr->ImportElements(el, fRPhiEventScene); }
    void                    ImportGeomRhoZ(TEveElement *el)            { fRhoZMgr->ImportElements(el, fRhoZGeomScene); }
    void                    ImportGeomRPhi(TEveElement *el)            { fRPhiMgr->ImportElements(el, fRPhiGeomScene); } 
    static MCMultiView &    Instance();   
    Bool_t                  IsSet() const                              { return fMVSet; }
    void                    SetDepth(Float_t d); 
    void                    SetRPhiAxes();
    void                    SetRhoZAxes(); 
    void                    UnSet()                                    { fMVSet = kFALSE; }
; 
ClassDef(MCMultiView,1);
};
#endif
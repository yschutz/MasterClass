#include <TApplication.h>
#include <TGClient.h>
#include <TList.h>

class MasterClassFrame : public TGMainFrame {

public:

   MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~MasterClassFrame();
   void Start();
  
  
   ClassDef(MasterClassFrame, 0)
};

void MasterClassFrame::Start()
{

#if defined(__CINT__) && !defined(__MAKECINT__)
	{
		Info("alice_vsd.C", "Has to be run in compiled mode ... doing this for you.");
		gSystem->CompileMacro("alice_vsd.C");
		alice_vsd(1);
	}
#else
	alice_vsd(1);    
#endif

	UnmapWindow();

}

MasterClassFrame::MasterClassFrame(const TGWindow *p, UInt_t w, UInt_t h) :
   TGMainFrame(p, w, h)
{

 Start();

}

MasterClassFrame::~MasterClassFrame()
{

   Cleanup();

}

void masterclassExtended()
{

   gROOT->pwd();

   new MasterClassFrame(gClient->GetRoot(), 400, 200);



}

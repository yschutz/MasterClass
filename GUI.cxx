#include "library/MCMain.h"

#include <TApplication.h>
#include <TGClient.h>
#include <TROOT.h>
#include <TSystem.h>

//=================================================================================
void GUI()
{
   Int_t opt = TString(gSystem->Getenv("OPT")).Atoi();
   new MCMain(gClient->GetRoot(), 400, 220, opt);
}
//=================================================================================
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);
   GUI();
   theApp.Run();
   return 0;
}
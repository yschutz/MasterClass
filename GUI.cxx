#include "library/MCMain.h"

#include <TApplication.h>
#include <TGClient.h>
#include <TROOT.h>

//=================================================================================
void GUI()
{
   new MCMain(gClient->GetRoot(), 400, 220);
}
//=================================================================================
int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);
   GUI();
   theApp.Run();
   return 0;
}
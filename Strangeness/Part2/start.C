{
    gSystem->CompileMacro("alice_vsd.C");
    gSystem->Load("alice_vsd_C.so");
    gROOT->Macro("masterclassExtended.C");
}
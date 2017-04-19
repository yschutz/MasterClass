{
	TString path(gSystem->Getenv("EXPATH")); 
	path.Append(gSystem->Getenv("PART")); 
	path.Append(gSystem->Getenv("ROLE")); 
    gSystem->CompileMacro( path + "/alice_vsd.C");
    gSystem->Load("alice_vsd_C.so");
	path = gSystem->Getenv("EXPATH"); 
    gROOT->Macro(path + "masterclass.C");
}
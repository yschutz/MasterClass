{
TString file = "MasterClassesTree_LHC10h_Run139036.root";
file.Prepend(gSystem->Getenv("EVTPATH")); 	
gSystem->Setenv("FILENAME", file); 
TString collsys = "PbPb";  
gSystem->Setenv("COLLSYS", collsys); 
TString trfa = "kFALSE"; 
gSystem->CompileMacro("AnalyseTreeForRAAComplete.C"); 
AnalyseTreeForRAAComplete(0, 5);
}

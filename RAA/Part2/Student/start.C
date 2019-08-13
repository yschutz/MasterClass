{
TString file = "MasterClassesTree_LHC10h_Run139036.root";
TString collsys = "PbPb";  
TString trfa = "kFALSE"; 
gSystem->CompileMacro("AnalyseTreeForRAAStudents.C"); 
AnalyseTreeForRAAStudents(file, collsys, trfa, 0, 5);
}

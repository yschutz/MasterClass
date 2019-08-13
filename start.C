

void start(){
     TString lPath("");    
    lPath.Append( gSystem->Getenv("EXPATH") ); 
    lPath.Append( gSystem->Getenv("PART") ); 

    gSystem->CompileMacro( lPath + "alice_vsd.C", "fg"); 
        // "f" = force recompilation
        // "g" = compile with debug symbol
    gSystem->Load("alice_vsd_C.so");
    
    
    TString lExPart( gSystem->Getenv("PART") );
    if( lExPart.EqualTo("Part1/")) gROOT->Macro( lPath + "masterclass.C");
    else
    if( lExPart.EqualTo("Part2/")) gROOT->Macro( lPath + "masterclassExtended.C");
    else{
       Printf(" ");
       Printf("Pb, unknown part (%s ?) of exercise to be treated... Nothing to be done, exit", lExPart.Data());
    }
    
}



#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

#include <TGFileDialog.h>
#include <TRootHelpDialog.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGLayout.h>
#include <TGSplitter.h>

#include <TGLOverlayButton.h>

#include <TLatex.h>
#include <TVirtualFitter.h>

#include <TG3DLine.h>
#include <TAxis3D.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TF1.h>
#include <TEveGValuators.h>
#include <TGDoubleSlider.h>

#include <TApplication.h>
#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveWindowManager.h>
#include <TEveVSD.h>
#include <TEveVSDStructs.h>

#include <TCanvas.h>

#include <TGLEmbeddedViewer.h>
#include <TCanvas.h>
#include <TParticlePDG.h>

#include <TH1D.h>

#include <TGTextEntry.h>

#include <TGLabel.h>
#include <TGComboBox.h>

#include <TGTab.h>
#include <TGButton.h>
#include <TGButtonGroup.h>
#include <TTimer.h>

#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TPRegexp.h>

#include <TGNumberEntry.h>
#include <TGLWidget.h>

#include <TFileMerger.h>
#include <TSystemDirectory.h>

#include <TEveManager.h>

#include <TEveBrowser.h>
#include <TEveWindow.h>

#include <TAxis3D.h>

using namespace std;

namespace
{
	const char *gMasterClassOutputData[] = {"MasterClass Output Data", "*.masterclass", 0, 0}; //for loading results
}

namespace
{
	const char *gMasterClassOutputPictue[] = {"MasterClass Histograms", "*.png", 0, 0}; //for loading results
}

TH1D* gKMinvHist   = 0;
TH1D* gXMinvHist   = 0;
TH1D* gLMinvHist   = 0;
TH1D* gALMinvHist  = 0;
TH1D* gAllMinvHist = 0;

TCanvas* pad1 = 0;
TCanvas* pad2 = 0;

Int_t globalChoice  = 0;
Int_t globalMode    = 0;
Int_t globalDataset = 0;
Int_t gLoads        = 0;

Bool_t foundNeg = kFALSE;
Bool_t foundPos = kFALSE;

TLatex *gLabels2 = 0;

Int_t AC = 0;

class TVSDReader
{
public:
	// ----------------------------------------------------------
	// File / Event Data
	// ----------------------------------------------------------

	TFile                   *fFile;
	TObjArray               *fEvDirKeys;
	TEveVSD                 *fVSD;
	Int_t                   fMaxEv, fCurEv, fMaxR; 
	TTimer                  *fTimer;
	TH1D                    *fMinvHisto;
	TCanvas                 *fPad;
        std::map< string,TGCheckButton* > fTabCheckButton;
        vector<vector<TString> > fVecListHistos;
	TEveGDoubleValuator 	*fFitRangeGaus;
	TEveGDoubleValuator 	*fFitRangePoly;
	TF1                     *fFitGaus;
	TF1                     *fFitPolynomial;
        
        enum gkPartType{
            kK0s = 0,
            kLambda = 1,
            kALambda = 2,
            kNbPart
        };
        
        enum gkSystemAndEvtActivity{
            kpp_MB = 0,
            kPbPb_MB,
            kPbPb_0010,
            kPbPb_1020,
            kPbPb_2030,
            kPbPb_3040,
            kPbPb_4050,
            kPbPb_5060,
            kPbPb_6070,
            kPbPb_7080,
            kPbPb_8090,
            kPbPb_90100,
            kNbSystemsAndEvtActivity                
        };       
        
        

	// ----------------------------------------------------------
	TVSDReader(const char* file_name) :
	fFile(0), fEvDirKeys(0),
	fVSD(0), fMaxEv(-1), fCurEv(-1), fMaxR(400), 
	fTimer(0), fMinvHisto(0), fPad(0), fFitRangeGaus(0), fFitRangePoly(0),
	fFitGaus(0), fFitPolynomial(0)
	{
		fFile = TFile::Open(file_name);
		if (!fFile)
		{
			Error("VSD_Reader", "Can not open file '%s' ... terminating.", file_name);
			gSystem->Exit(1);
		}

		fEvDirKeys = new TObjArray;
		TPMERegexp name_re("Event\\d+");
		TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
		while (lnk)
		{
			if (name_re.Match(lnk->GetObject()->GetName()))
			{
				fEvDirKeys->Add(lnk->GetObject());
			}
			lnk = lnk->Next();
		}

		fMaxEv = fEvDirKeys->GetEntriesFast();
		if (fMaxEv == 0)
		{
			Error("VSD_Reader", "No events to show ... terminating.");
			gSystem->Exit(1);
		}

		fVSD = new TEveVSD;
                
                TH1::AddDirectory(kFALSE); // to avoid memory leak with the constant update of fMinvHisto
                               

                // NOTE : set up the size of the 2D TString array  (kNbPart x kNbSystemsAndEvtActivity)
                fVecListHistos.resize( kNbPart );
                for (Int_t iPart = 0; iPart < kNbPart ; ++iPart) {
                    fVecListHistos[iPart].resize( kNbSystemsAndEvtActivity );
                }// end set-up size of the 2D array
             
                fVecListHistos[kK0s    ][kpp_MB     ] = "K0s_pp_MB"     ;
                fVecListHistos[kK0s    ][kPbPb_MB   ] = "K0s_PbPb_MB"   ;
                fVecListHistos[kK0s    ][kPbPb_0010 ] = "K0s_PbPb_0010" ;
                fVecListHistos[kK0s    ][kPbPb_1020 ] = "K0s_PbPb_1020" ;
                fVecListHistos[kK0s    ][kPbPb_2030 ] = "K0s_PbPb_2030" ;
                fVecListHistos[kK0s    ][kPbPb_3040 ] = "K0s_PbPb_3040" ;
                fVecListHistos[kK0s    ][kPbPb_4050 ] = "K0s_PbPb_4050" ;
                fVecListHistos[kK0s    ][kPbPb_5060 ] = "K0s_PbPb_5060" ;
                fVecListHistos[kK0s    ][kPbPb_6070 ] = "K0s_PbPb_6070" ;
                fVecListHistos[kK0s    ][kPbPb_7080 ] = "K0s_PbPb_7080" ;
                fVecListHistos[kK0s    ][kPbPb_8090 ] = "K0s_PbPb_8090" ;
                fVecListHistos[kK0s    ][kPbPb_90100] = "K0s_PbPb_90100";
                
                fVecListHistos[kLambda ][kpp_MB     ] = "Lambda_pp_MB"     ;
                fVecListHistos[kLambda ][kPbPb_MB   ] = "Lambda_PbPb_MB"   ;
                fVecListHistos[kLambda ][kPbPb_0010 ] = "Lambda_PbPb_0010" ;
                fVecListHistos[kLambda ][kPbPb_1020 ] = "Lambda_PbPb_1020" ;
                fVecListHistos[kLambda ][kPbPb_2030 ] = "Lambda_PbPb_2030" ;
                fVecListHistos[kLambda ][kPbPb_3040 ] = "Lambda_PbPb_3040" ;
                fVecListHistos[kLambda ][kPbPb_4050 ] = "Lambda_PbPb_4050" ;
                fVecListHistos[kLambda ][kPbPb_5060 ] = "Lambda_PbPb_5060" ;
                fVecListHistos[kLambda ][kPbPb_6070 ] = "Lambda_PbPb_6070" ;
                fVecListHistos[kLambda ][kPbPb_7080 ] = "Lambda_PbPb_7080" ;
                fVecListHistos[kLambda ][kPbPb_8090 ] = "Lambda_PbPb_8090" ;
                fVecListHistos[kLambda ][kPbPb_90100] = "Lambda_PbPb_90100";
                
                fVecListHistos[kALambda][kpp_MB     ] = "AntiLambda_pp_MB"     ;
                fVecListHistos[kALambda][kPbPb_MB   ] = "AntiLambda_PbPb_MB"   ;
                fVecListHistos[kALambda][kPbPb_0010 ] = "AntiLambda_PbPb_0010" ;
                fVecListHistos[kALambda][kPbPb_1020 ] = "AntiLambda_PbPb_1020" ;
                fVecListHistos[kALambda][kPbPb_2030 ] = "AntiLambda_PbPb_2030" ;
                fVecListHistos[kALambda][kPbPb_3040 ] = "AntiLambda_PbPb_3040" ;
                fVecListHistos[kALambda][kPbPb_4050 ] = "AntiLambda_PbPb_4050" ;
                fVecListHistos[kALambda][kPbPb_5060 ] = "AntiLambda_PbPb_5060" ;
                fVecListHistos[kALambda][kPbPb_6070 ] = "AntiLambda_PbPb_6070" ;
                fVecListHistos[kALambda][kPbPb_7080 ] = "AntiLambda_PbPb_7080" ;
                fVecListHistos[kALambda][kPbPb_8090 ] = "AntiLambda_PbPb_8090" ;
                fVecListHistos[kALambda][kPbPb_90100] = "AntiLambda_PbPb_90100";


	}// end constructor

	// ----------------------------------------------------------
	virtual ~TVSDReader()
	{
		// Destructor.

                delete fFitPolynomial;
                delete fFitGaus;
                delete fFitRangePoly;
                delete fFitRangeGaus;
                delete fMinvHisto;
            
		delete fVSD;
		delete fEvDirKeys;
                delete fTimer;
                delete fPad;

		fFile->Close();
		delete fFile;
                
	}

	// ----------------------------------------------------------
	void ToStudent()
	{
		string fileIn;
		switch(globalDataset)
		{
			case 1:
			fileIn="dataset1.txt";
			break;
			default:
			fileIn= "dataset1.txt";
			break;
		}
		fEvDirKeys = new TObjArray;
		TPMERegexp name_re("Event\\d+");
		TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();

		while (lnk)
		{
			if (name_re.Match(lnk->GetObject()->GetName()))
			{
				fEvDirKeys->Add(lnk->GetObject());
			}
			lnk = lnk->Next();
		}

		fMaxEv = fEvDirKeys->GetEntriesFast();
		if (fMaxEv == 0)
		{
			Error("VSD_Reader", "No events to show ... terminating.");
			gSystem->Exit(1);
		}

		fVSD = new TEveVSD;
		fTimer = new TTimer();
		StudentSet();
	}

	// ----------------------------------------------------------
	void Exit(){


		gSystem->CleanCompiledMacros();

	}
	// ----------------------------------------------------------
	void Combine(const char * fileName, int type)
	{
		TString fn(fileName); 
		fn.Prepend(gSystem->Getenv("PART")); 
		fn.Prepend(gSystem->Getenv("EVTPATH")); 
		
		// ifstream file;
		TFile *file; 

		Double_t minv;
		// if(AC<4) {
		// 	AC=AC+1;
		// 	cout << AC << endl; 
		// } else  {
		// 	AC=1;
		// 	cout << AC <<endl; 
		// }	
	
		// fPad->cd(AC);

		fFitPolynomial = nullptr;
		fFitGaus       = nullptr;

      
		// file.open(fn);
		fn.Append("?filetype=raw");
		file = TFile::Open(fn);       
		Long64_t bufsize = 1024; 
		Long64_t filesize = file->GetSize(); 
		if (filesize < bufsize)
			bufsize = filesize;
		char *buf    = new char[bufsize];
       
		// if( file.good() == false )
		if (!file)
		{
			cout << "Cannot open file: " << fn << endl;
			return;
		}
		fMinvHisto->SetLineColor(2);
		float max = 1.0;
		float min = 0.25; 
		if (type == 1) {
			max = 2.0;
			min = 1.0;
		}
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			fMinvHisto = new TH1D("fMinvHisto","InvariantMass ",400,min,max);
			fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
			fMinvHisto->GetYaxis()->SetTitle("Counts");
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			fMinvHisto = new TH1D("fMinvHisto","Masse Invariante ",400,min,max);
			fMinvHisto->GetXaxis()->SetTitle("Masse Invariante (GeV/c^{2})");
			fMinvHisto->GetYaxis()->SetTitle("Coups");
		}
		fMinvHisto->SetFillColor(0);

		// while(!file.eof() )
		// {
		// 	file>>minv;
		// 	// cout<<"Invariant Mass : "<<minv<<endl;
		// 	fMinvHisto->Fill(minv);

		// }
		while (bufsize && !file->ReadBuffer(buf, bufsize)) {
			TString temp; 
			for (int index =0; index < bufsize; index++) { 
				if (buf[index] != '\n')
					temp.Append(buf[index]); 
				else {
					minv = temp.Atof();
					fMinvHisto->Fill(minv);
					temp.Clear(); 
				} 
			}
			Long64_t bytesleft = filesize - file->GetBytesRead(); 
			if (bytesleft < bufsize)
				bufsize = (int)bytesleft;
		} 
		fMinvHisto->Draw();
		fPad->Update();
		// file.close();
		file->Close();
		
	}
	
	void CheckButtonUpAndTurnDown(){
            
            for(Short_t iPart = 0; iPart < kNbPart; ++iPart){
                for(Int_t iHist = 0; iHist < kNbSystemsAndEvtActivity; ++iHist ){

                    TString lStrID( fVecListHistos[iPart][iHist].Data() );
                    
                    // See TGButtonGroup::ReleaseButtons(), 
                    // in https://root.cern.ch/doc/master/classTGButtonGroup.html

                    if( fTabCheckButton[ lStrID.Data() ]->IsOn() ){
                        Printf("Button [%s] was on... switching it off now.", fTabCheckButton[ lStrID.Data() ]->GetTitle() );    
                        fTabCheckButton[ lStrID.Data() ]->SetOn(kFALSE);
                    }
                    
                }// end for iHist            
            }// end for iPart
        }

        
        	// ----------------------------------------------------------
	void LoadHisto( Int_t iPart, Int_t iHist)
	{
            CheckButtonUpAndTurnDown();
            Printf("File to be open : dataset_%d_%d", iPart, iHist );
            
            
            TString lStrID( fVecListHistos[iPart][iHist].Data() );
            Printf("File to be open : dataset_%d_%d = dataset_%s", iPart, iHist, lStrID.Data()  );
            
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ lStrID.Data() ]->SetOn(kTRUE);
		const char * file = "pp-K0.txt";
		Combine(file, 0 );
                fMinvHisto->SetTitle( lStrID.Data()  );
            
	}
        

	// ----------------------------------------------------------
	void CombineK0s_pp_MB()
	{
            TString lStrID("K0s_pp_MB");
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ lStrID.Data() ]->SetOn(kTRUE);
		const char * file = "pp-K0.txt";
		Combine(file, 0);
                fMinvHisto->SetTitle( lStrID.Data()  );
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_MB()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_MB"  ]->SetOn(kTRUE);            
            	const char * file = "PbPb-K0.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_0010()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_0010"  ]->SetOn(kTRUE); 
		const char * file = "dataset11.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_1020()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_1020"  ]->SetOn(kTRUE);             
		const char * file = "dataset12.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_2030()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_2030"  ]->SetOn(kTRUE);             
		const char * file = "dataset13.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_3040()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_3040"  ]->SetOn(kTRUE);
		const char * file = "dataset14.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_4050()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_4050"  ]->SetOn(kTRUE);
		const char * file = "dataset15.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_5060()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_5060"  ]->SetOn(kTRUE);
		const char * file = "dataset16.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_6070()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_6070"  ]->SetOn(kTRUE);
		const char * file = "dataset17.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_7080()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_7080"  ]->SetOn(kTRUE);	
                const char * file = "dataset18.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_8090()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_8090"  ]->SetOn(kTRUE);
		const char * file = "dataset19.txt";
		Combine(file, 0); 
	}

	// ----------------------------------------------------------
	void CombineK0s_PbPb_90100()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "K0s_PbPb_90100"  ]->SetOn(kTRUE);
		const char * file = "dataset20.txt";
		Combine(file, 0); 
	}

	
	
	// ----------------------------------------------------------
	void CombineLambda_pp_MB()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_pp_MB"  ]->SetOn(kTRUE);
		const char * file = "pp-Lambda.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_MB()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_MB"  ]->SetOn(kTRUE);
		const char * file = "PbPb-Lambda.txt";
		Combine(file, 1); 
	}	
	
	// ----------------------------------------------------------
	void CombineLambda_PbPb_0010()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_0010"  ]->SetOn(kTRUE);
		const char * file = "dataset21.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_1020()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_1020"  ]->SetOn(kTRUE);
		const char * file = "dataset22.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_2030()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_2030"  ]->SetOn(kTRUE);
		const char * file = "dataset23.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_3040()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_3040"  ]->SetOn(kTRUE);
		const char * file = "dataset24.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_4050()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_4050"  ]->SetOn(kTRUE);
		const char * file = "dataset25.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_5060()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_5060"  ]->SetOn(kTRUE);
		const char * file = "dataset26.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_6070()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_6070"  ]->SetOn(kTRUE);
		const char * file = "dataset27.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_7080()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_7080"  ]->SetOn(kTRUE);
		const char * file = "dataset28.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_8090()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_8090"  ]->SetOn(kTRUE);
		const char * file = "dataset29.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineLambda_PbPb_90100()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "Lambda_PbPb_90100"  ]->SetOn(kTRUE);
		const char * file = "dataset30.txt";
		Combine(file, 1); 
	}

	
		// ----------------------------------------------------------
	void CombineAntiLambda_pp_MB()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_pp_MB"  ]->SetOn(kTRUE);
		const char * file = "pp-AntiLambda.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_MB()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_MB"  ]->SetOn(kTRUE);
		const char * file = "PbPb-AntiLambda.txt";
		Combine(file, 1); 
	}
	
	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_0010()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_0010"  ]->SetOn(kTRUE);
		const char * file = "dataset31.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_1020()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_1020"  ]->SetOn(kTRUE);
		const char * file = "dataset32.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_2030()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_2030"  ]->SetOn(kTRUE);
		const char * file = "dataset33.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_3040()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_3040"  ]->SetOn(kTRUE);
		const char * file = "dataset34.txt";
		Combine(file, 1); 
	}
  
	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_4050()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_4050"  ]->SetOn(kTRUE);
		const char * file = "dataset35.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_5060()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_5060"  ]->SetOn(kTRUE);
		const char * file = "dataset36.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_6070()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_6070"  ]->SetOn(kTRUE);
		const char * file = "dataset37.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_7080()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_7080"  ]->SetOn(kTRUE);
		const char * file = "dataset38.txt";
		Combine(file, 1); 
	}
  
	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_8090()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_8090"  ]->SetOn(kTRUE);
		const char * file = "dataset39.txt";
		Combine(file, 1); 
	}

	// ----------------------------------------------------------
	void CombineAntiLambda_PbPb_90100()
	{
            CheckButtonUpAndTurnDown();
            fTabCheckButton[ "AntiLambda_PbPb_90100"  ]->SetOn(kTRUE);
		const char * file = "dataset40.txt";
		Combine(file, 1);               
	}
	


  //______________________________________________________________________________
	void FitInvariantMass()
	{
    
		Double_t MinRange = TMath::Min(fFitRangePoly->GetMin(),fFitRangeGaus->GetMin());
		Double_t MaxRange = TMath::Max(fFitRangePoly->GetMax(),fFitRangeGaus->GetMax());

		if (!fFitGaus) 
			fFitGaus = new TF1("fitGaus","[0]*TMath::Gaus(x,[1],[2])+[3]*x*x+[4]*x+[5]",MinRange,MaxRange);
		else 
			fFitGaus->SetRange(MinRange,MaxRange);

		fFitGaus->SetParameter(0,80.);
		fFitGaus->SetParameter(1,0.5*(fFitRangeGaus->GetMin()+fFitRangeGaus->GetMax()));
		fFitGaus->SetParameter(2,1.);
		fFitGaus->SetParLimits(0,0.,TMath::Power(10.,9));    
		fFitGaus->SetParLimits(1,fFitRangeGaus->GetMin(),fFitRangeGaus->GetMax());    
		fFitGaus->SetParLimits(2,0.,(fFitRangeGaus->GetMax()-fFitRangeGaus->GetMin())/2.);

		fFitGaus->SetLineColor(kGreen+1);
		fMinvHisto->Fit(fFitGaus,"rime");

		if (gLabels2) {
			delete gLabels2;
			fMinvHisto->Draw();
		}
		gLabels2 = new TLatex();
		gLabels2->SetTextSize(0.045);
		gLabels2->SetTextColor(kBlack);
		gLabels2->SetNDC(kTRUE);
		Double_t LabelX = 0.5;
		Double_t LabelY = 0.8;
		Double_t LabelYstep = 0.05;
      
		Int_t Total_Fit = (Int_t)(fFitGaus->Integral(fFitRangeGaus->GetMin(),fFitRangeGaus->GetMax())/fMinvHisto->GetBinWidth(1));
		Int_t Bck_Fit = (Int_t)(Background_Integral(fFitRangeGaus->GetMin(),fFitRangeGaus->GetMax())/fMinvHisto->GetBinWidth(1));

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			gLabels2->DrawLatex(LabelX, LabelY,Form("Total: %d",Total_Fit));
			gLabels2->DrawLatex(LabelX, LabelY-LabelYstep,Form("Background: %d",Bck_Fit));
			gLabels2->DrawLatex(LabelX, LabelY-2.*LabelYstep,Form("Signal: %d",Total_Fit-Bck_Fit));
			gLabels2->DrawLatex(LabelX, LabelY-3.*LabelYstep,Form("Mean: %f#pm%f",fFitGaus->GetParameter(1)*1000., fFitGaus->GetParError(1)*1000.));
			gLabels2->DrawLatex(LabelX, LabelY-4.*LabelYstep,Form("#sigma: %f#pm%f",fFitGaus->GetParameter(2)*1000., fFitGaus->GetParError(2)*1000.));
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			gLabels2->DrawLatex(LabelX, LabelY,Form("Total: %d",Total_Fit));
			gLabels2->DrawLatex(LabelX, LabelY-LabelYstep,Form("Fond: %d",Bck_Fit));
			gLabels2->DrawLatex(LabelX, LabelY-2.*LabelYstep,Form("Signal: %d",Total_Fit-Bck_Fit));
			gLabels2->DrawLatex(LabelX, LabelY-3.*LabelYstep,Form("Moyenne: %f#pm%f",fFitGaus->GetParameter(1)*1000., fFitGaus->GetParError(1)*1000.));
			gLabels2->DrawLatex(LabelX, LabelY-4.*LabelYstep,Form("#sigma: %f#pm%f",fFitGaus->GetParameter(2)*1000., fFitGaus->GetParError(2)*1000.));
		}
    
		if(!fFitPolynomial) fFitPolynomial = new TF1("fitPoly","[0]*x*x+[1]*x+[2]",MinRange,MaxRange);
		else fFitPolynomial->SetRange(MinRange,MaxRange);
		fFitPolynomial->SetParameter(0,fFitGaus->GetParameter(3));
		fFitPolynomial->SetParameter(1,fFitGaus->GetParameter(4));
		fFitPolynomial->SetParameter(2,fFitGaus->GetParameter(5));
		fFitPolynomial->SetLineColor(kBlue+1);
		fFitPolynomial->Draw("SAME");
      
		fPad->Update();
	}
    
	//______________________________________________________________________________
	Double_t Background_Integral(Double_t xmin, Double_t xmax) {
    
		return (xmax-xmin)*(fFitGaus->GetParameter(5)+fFitGaus->GetParameter(4)*(xmax+xmin)/2.)+(fFitGaus->GetParameter(3)*(TMath::Power(xmax,3) - TMath::Power(xmin,3))/3.);
        
	}

	//______________________________________________________________________________
	void AutomaticMenu()
	{

		TEveBrowser* browser = gEve->GetBrowser();

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			browser->GetMainFrame()->SetWindowName(Form("ALICE MasterClass - STUDENT MODE - "));
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			browser->GetMainFrame()->SetWindowName(Form("ALICE MasterClass -  MODE Etudiant - "));
		}
		browser->CloseTab(0);
		browser->StartEmbedding(TRootBrowser::kLeft);

		TGMainFrame* frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
		frmMain->SetWindowName("XX GUI");
		frmMain->SetCleanup(kDeepCleanup);

		TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")) );
		TGTextButton* b       = nullptr;
		TGGroupFrame* gf      = nullptr;
		TGVerticalFrame* vf   = nullptr;
		TGHorizontalFrame* hf = nullptr;
		ULong_t buttonRedColor;

		//-------------------New GUI--------------------------
		// tab widget to select histograms to be fitted
                
                TGTab *lTabsV0s = new TGTab(frmMain,128,280);


                TGCompositeFrame *lCompositeFrame[ kNbPart ];
                
                // container of "Tab1"
                lCompositeFrame[ kK0s ] = lTabsV0s->AddTab("K0s");
                lCompositeFrame[ kK0s ]->SetLayoutManager(new TGVerticalLayout(lCompositeFrame[ kK0s ]));

                // container of "Tab2"
                lCompositeFrame[ kLambda ] = lTabsV0s->AddTab("Lambda");
                lCompositeFrame[ kLambda ]->SetLayoutManager(new TGVerticalLayout(lCompositeFrame[ kLambda ]));

                // container of "Tab3"
                lCompositeFrame[ kALambda ] = lTabsV0s->AddTab("AntiLambda");
                lCompositeFrame[ kALambda ]->SetLayoutManager(new TGVerticalLayout(lCompositeFrame[ kALambda ]));                
                
                                                   
                // FIXME A. Maire : Mutual Exclusive click on each button ...
                //  See /home/amaire/AliceSuite/sw/ubuntu1604_x86-64/ROOT/v6-12-06-2/tutorials/gui/buttongroupState.C
                //  Key TGBUttonGroup::SetRadioButtonExclusive(kTRUE)
                // Did not manage to make it work here in Tabs 
                
                // AN alternative to RadioButton or CheckButton would be TGComboBox.
                
                
                for(Short_t iPart = 0; iPart < kNbPart; ++iPart){
                    for(Int_t iHist = -1; iHist < kNbSystemsAndEvtActivity; ++iHist ){
                        
                        if(iHist == -1){                        
                            TGTextButton *button = 0;
                                if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
					button = new TGTextButton(lCompositeFrame[iPart], "Untick all");
				} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
					button = new TGTextButton(lCompositeFrame[iPart], "Tout decocher");
                                }
                            button->Connect("Clicked()", "TVSDReader", this, "CheckButtonUpAndTurnDown()");
                            lCompositeFrame[ iPart ]->AddFrame(button, new TGLayoutHints(kLHintsRight,2,2,2,2));                          
                        }
                        else{                      
                            TString lStrID( fVecListHistos[iPart][iHist].Data() );  // K0s_pp_MB
        
                            // Prepare label to be displayed for the button itself
                            TString lStrIDLabel( lStrID );
        
                            lStrIDLabel.ReplaceAll("K0s_","K0s, ");               // K0s_pp_MB  --> K0s, pp_MB
                            lStrIDLabel.ReplaceAll("AntiLambda_","anti-L, ");     // NOTE : replace first AntiLambda over Lambda ... to avoid undesirable replacement
                            lStrIDLabel.ReplaceAll("Lambda_","L, ");                    
                            lStrIDLabel.ReplaceAll("_"," ");                  // K0s, pp_MB --> K0s, pp MB
                            lStrIDLabel.ReplaceAll("pp"  ,"p-p"  );
                            lStrIDLabel.ReplaceAll("PbPb","Pb-Pb");
                            TPRegexp lRegex("([0-9]{2})([0-9]{2})");// regex pattern : locate 2 groups of 2 numbers 0010 -> (00)(10)
                            lRegex.Substitute(lStrIDLabel,"$1-$2");   // add an hyphen in between : 00-10                    
                            if( ! lStrIDLabel.Contains("MB") ) lStrIDLabel.Append("%"); // for Pb-Pb centrality

                                            
                            fTabCheckButton[ lStrID.Data() ] = new TGCheckButton(lCompositeFrame[ iPart ], lStrIDLabel.Data(), iPart*100 + iHist   );
                            // NOTE 1 :
                            //  the table of button is organised with a std::map to look like  fTabCheckButton[ "Lambda_PbPb_7080" ], 
                            //  i.e. identified via meaning string rather than a simple Int_t ID
                            
                            // NOTE 2:  
                            // Unique ID of the buttons = potentially needed for the mutual exclusion behaviour
                            //  (iPart=0-2)*100 + (iHist=0-11) should be sufficient to avoid overlap in ID ...
                            
                            // FIXME
                            fTabCheckButton[ lStrID.Data() ]->Connect("Clicked()", "TVSDReader", this, TString::Format("Combine%s()", lStrID.Data()) );
                            //fTabCheckButton[ lStrID.Data() ]->Connect("Clicked()", "TVSDReader", this, TString::Format("LoadHisto(=%d,%d)", iPart, iHist) );
                            fTabCheckButton[ lStrID.Data() ]->SetTextJustify(36);
                            fTabCheckButton[ lStrID.Data() ]->SetMargins(0,0,0,0);
                            fTabCheckButton[ lStrID.Data() ]->SetWrapLength(-1);
                            
                            lCompositeFrame[ iPart ]->AddFrame(fTabCheckButton[ lStrID.Data() ], new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
                        }
                        
                        //fButtonGroup->Show();
                        //lCompositeFrame[ iPart ]->AddFrame(fButtonGroup, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
                        
                    }// end loop over iHist, system + centrality
                }// end loop over iPart
                                
                

		lTabsV0s->SetTab(1);

		lTabsV0s->Resize(lTabsV0s->GetDefaultSize());
		frmMain->AddFrame(lTabsV0s, new TGLayoutHints(kLHintsLeft | kLHintsTop,2,2,2,2));
		lTabsV0s->MoveResize(8,8,128,280);

		frmMain->SetMWMHints(kMWMDecorAll,
		kMWMFuncAll,
		kMWMInputModeless);
		frmMain->MapSubwindows();

		frmMain->Resize(frmMain->GetDefaultSize());
		frmMain->MapWindow();
		//frmMain->Resize(490,372);

	
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			gf = new TGGroupFrame(frmMain, "Fit");
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			gf = new TGGroupFrame(frmMain, "Ajustage");
		}

		vf = new TGVerticalFrame(gf, 200, 20, kFixedWidth);
		{

			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{

				if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
					fFitRangePoly = new TEveGDoubleValuator(hf,"Bkg range:", 40, 0);
				} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
					fFitRangePoly = new TEveGDoubleValuator(hf,"Inter. fond :", 40, 0);
				}
				fFitRangePoly->SetNELength(5);
				fFitRangePoly->SetLabelWidth(70);
				fFitRangePoly->Build();
				fFitRangePoly->GetSlider()->SetWidth(190);
				fFitRangePoly->SetLimits(0.0, 2.0, TGNumberFormat::kNESRealThree);
				fFitRangePoly->SetValues(0.0, 2.0, TGNumberFormat::kNESRealThree);

				hf->AddFrame(fFitRangePoly, new TGLayoutHints(kLHintsExpandX));

			}

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{

				if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
					fFitRangeGaus = new TEveGDoubleValuator(hf,"Sig range:", 40, 0);
				} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
					fFitRangeGaus = new TEveGDoubleValuator(hf,"Inter. signal :", 40, 0);
				}
				fFitRangeGaus->SetNELength(5);
				fFitRangeGaus->SetLabelWidth(70);
				fFitRangeGaus->Build();
				fFitRangeGaus->GetSlider()->SetWidth(190);
				fFitRangeGaus->SetLimits(0.0, 2.0, TGNumberFormat::kNESRealThree);
				fFitRangeGaus->SetValues(0.0, 2.0, TGNumberFormat::kNESRealThree);

				hf->AddFrame(fFitRangeGaus, new TGLayoutHints(kLHintsExpandX));

			}

			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));

			hf = new TGHorizontalFrame(vf, 200, 20, kFixedWidth);
			{
				if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
					b = new TGTextButton(hf, "Fit signal+background");
				} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
					b = new TGTextButton(hf, "Ajustage signal+fond");
				}
				b->Connect("Clicked()", "TVSDReader", this, "FitInvariantMass()");
				hf->AddFrame(b, new TGLayoutHints(kLHintsExpandX));
			}
        
			vf->AddFrame(hf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY, 5, 5, 5, 5));
        
		}

		gf->AddFrame(vf, new TGLayoutHints(kLHintsExpandX | kLHintsCenterY));

		frmMain->AddFrame(gf);

		frmMain->MapSubwindows();
		frmMain->Resize();
		frmMain->MapWindow();

		browser->StopEmbedding(" ");

		browser->StartEmbedding(TRootBrowser::kBottom);

		frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600, kHorizontalFrame);
		frmMain->SetWindowName("XX GUI");
		frmMain->SetCleanup(kDeepCleanup);

		const TGFont *font = gClient->GetFont("-*-helvetica-bold-r-normal-*-20-*-*-*-*-*-*-*");
		FontStruct_t buttonFont = font->GetFontStruct();

		gClient->GetColorByName("red", buttonRedColor);
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			b = new TGTextButton(frmMain,"Exit");
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			b = new TGTextButton(frmMain,"Sortie");
		}
		b->SetTextColor(buttonRedColor);
		b->SetFont(buttonFont);
    
		b->Connect("Clicked()", "TApplication", gApplication, "Terminate()");

		frmMain->AddFrame(b, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY , 1, 1, 1, 1));

		frmMain->MapSubwindows();
		frmMain->Resize();
		frmMain->MapWindow();

		browser->StopEmbedding(" ");

		browser->StartEmbedding(TRootBrowser::kRight);

		fPad = new TCanvas();
		// fPad->Divide(2, 2);
		// fPad->cd(1);

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			fMinvHisto = new TH1D("Invariant Mass","Invariant Mass",400,0.0,2.0);
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			fMinvHisto = new TH1D("Masse Invariante","masse invariante",400,0.0,2.0);
		}

		// if(AC<=4) {
		// 	AC=AC+1; 
		// 	cout << AC << endl;
		// }

	
		// else {
		// 	AC=1; 
		// 	cout << AC << endl;
		// }

		// fPad->cd(AC);
		fMinvHisto->SetLineColor(2);
		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			fMinvHisto->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
			fMinvHisto->GetYaxis()->SetTitle("Counts");
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			fMinvHisto->GetXaxis()->SetTitle("masse invariante (GeV/c^{2})");
			fMinvHisto->GetYaxis()->SetTitle("Coups");
		}
		fMinvHisto->SetFillColor(0);

		if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
			browser->StopEmbedding("Invariant Mass Distibution");
		} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
			browser->StopEmbedding("Distribution Masse Invariante");
		}

		browser->GetTabRight()->SetTab(2);

	}

	//______________________________________________________________________________
	void StudentSet(){

		AutomaticMenu();

	}

	ClassDef(TVSDReader, 0);
};

TVSDReader* gVSDReader = 0;


//______________________________________________________________________________
void alice_vsd(Int_t dataset)
{

	TString vsd_file_name;


	switch(dataset)
	{

		case 1:
		vsd_file_name = "AliVSD_MasterClass_5.root";
		vsd_file_name.Prepend(gSystem->Getenv("PART")); 
		vsd_file_name.Prepend(gSystem->Getenv("EVTPATH")); 
		break;

	}


	TEveManager::Create(kTRUE,"FV");

	gVSDReader = new TVSDReader(vsd_file_name);

	gVSDReader->fTimer = new TTimer();
 
	TEveBrowser* browser = gEve->GetBrowser();

	browser->GetTabLeft()->RemoveTab(1);
	browser->GetTabLeft()->RemoveTab(0);
	gStyle->SetOptStat("e");
	//  gStyle->SetOptFit(1111);
	gStyle->SetOptFit(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetPalette(1);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
		gKMinvHist = new TH1D("Statistics","Kaons",50,0.0,2.0);
		gKMinvHist->GetXaxis()->SetTitle("K0s invariant Mass (GeV/c^{2})");
		gKMinvHist->GetYaxis()->SetTitle("Counts");
	} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
		gKMinvHist = new TH1D("Statistique","Kaons",50,0.0,2.0);
		gKMinvHist->GetXaxis()->SetTitle("Masse Invariante K0S (GeV/c^{2})");
		gKMinvHist->GetYaxis()->SetTitle("Coups");
	}
	gKMinvHist->SetLineColor(2);
	gKMinvHist->SetFillColor(0);
  
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
		gXMinvHist = new TH1D("Statistics","Xis",50,0.0,2.0);
		gXMinvHist->GetXaxis()->SetTitle("#Xi invariant Mass (GeV/c^{2})");
		gXMinvHist->GetYaxis()->SetTitle("Counts");
	} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
		gXMinvHist = new TH1D("Statistique","Xis",50,0.0,2.0);
		gXMinvHist->GetXaxis()->SetTitle("Masse invariante #Xi (GeV/c^{2})");
		gXMinvHist->GetYaxis()->SetTitle("Coups");
	}
	gXMinvHist->SetLineColor(3);
	gXMinvHist->SetFillColor(0);
 
	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
		gLMinvHist = new TH1D("Statistics","Lambdas",50,0.0,2.0);
		gLMinvHist->GetXaxis()->SetTitle("#Lambda invariant Mass (GeV/c^{2})");
		gLMinvHist->GetYaxis()->SetTitle("Counts");
	} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
		gLMinvHist = new TH1D("Statistique","Lambdas",50,0.0,2.0);
		gLMinvHist->GetXaxis()->SetTitle("Masse Invariante #Lambda (GeV/c^{2})");
		gLMinvHist->GetYaxis()->SetTitle("Coups");
	}
	gLMinvHist->SetLineColor(4);
	gLMinvHist->SetFillColor(0);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
		gALMinvHist = new TH1D("Statistics","Anti-Lambdas",50,0.0,2.0);
		gALMinvHist->GetXaxis()->SetTitle("#overline{#Lambda} invariant Mass (GeV/c^{2})");
		gALMinvHist->GetYaxis()->SetTitle("Counts");
	} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
		gALMinvHist = new TH1D("Statistique","Anti-Lambdas",50,0.0,2.0);
		gALMinvHist->GetXaxis()->SetTitle("Masse invariante #overline{#Lambda} (GeV/c^{2})");
		gALMinvHist->GetYaxis()->SetTitle("Coups");
	}
	gALMinvHist->SetLineColor(9);
	gALMinvHist->SetFillColor(0);

	if (strncmp(gSystem->Getenv("BLA"), "EN", 2) == 0) {
		gAllMinvHist = new TH1D("Statistics","Invariant Mass - final result",20,0.0,2.0);
		gAllMinvHist->GetXaxis()->SetTitle("Invariant Mass (GeV/c^{2})");
		gAllMinvHist->GetYaxis()->SetTitle("Counts");
	} else if (strncmp(gSystem->Getenv("BLA"), "FR", 2) == 0) {
		gAllMinvHist = new TH1D("Statistique","Masse Invariante - Resultat final",20,0.0,2.0);
		gAllMinvHist->GetXaxis()->SetTitle("Masse Invariante (GeV/c^{2})");
		gAllMinvHist->GetYaxis()->SetTitle("Coups");
	}
	gAllMinvHist->SetFillColor(0);

	gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

	gVSDReader->ToStudent();
   
}

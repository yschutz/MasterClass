#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <TROOT.h>
#include <TApplication.h>
#include <TPaveLabel.h>
#include <TSystem.h>
#include <TFrame.h>
#include <TStyle.h>
#include <TString.h>
#include "TGaxis.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TF1.h"
#include "TVirtualFitter.h"
#include "TObject.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TTree.h"
#include "TMinuit.h"
#include "TLatex.h"
#include "TMath.h"
#include "TPostScript.h"
#include "TGraphErrors.h"
#include "TArrow.h"
#include "TGraphAsymmErrors.h"
#include "TGaxis.h"
#include "TMarker.h"

//*********************************************************************************
// Function to set some general Styles in the root environment
//*********************************************************************************

void StyleSettings(){
    //gStyle->SetOptTitle(kFALSE);
    gStyle->SetOptDate(0);   //show day and time
    gStyle->SetOptStat(0);  //show statistic
    gStyle->SetPalette(1,0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetTitleFillColor(0);
    gStyle->SetTextSize(0.5);
    gStyle->SetLabelSize(0.03,"xyz");
    gStyle->SetLabelOffset(0.002,"xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetTitleOffset(1.0,"y");
    gStyle->SetTitleOffset(1.0,"x");
    gStyle->SetCanvasColor(0);
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
    gStyle->SetLineWidth(2);
    
    gStyle->SetPadTopMargin(0.06);
    gStyle->SetPadBottomMargin(0.09);
    gStyle->SetPadRightMargin(0.04);
    gStyle->SetPadLeftMargin(0.1	);
    
    TGaxis::SetMaxDigits(3);
}


//*********************************************************************************
// Function to improve style of histograms for plotting
// arguments:
// 	* histo1 - histograms which should be modified
//		* markerStyle - Style of the marker
//				- good marker styles are:
//						# 20 = filled circle, 21 = filled square, 22 = filled triangle up
//						# 23 = filled triangle down, 24 = open circle, 25 = open square
//						# 26 = open triangle up, 27 = open diamond, 28 = open cross
//						# 29 = filled star, 30 = open star
// 	* markerSize - Size of the marker, should be varied between 0.8 - 2.5 only
//		* markerColor - Color of the marker
//		* lineColor - Color of the errorbars
//				- good colors for line and marker are:
//						# kBlack, kGray +1 , kGray+2, kGray+3
//						# kBlue, kBlue+4,  kBlue-4, kBlue-9
//						# kGreen, kGreen+4, kGreen-2, kGreen-9
// 					# kRed, kRed+2, kRed-4, kRed-9
//						# kCyan, kCyan+3, kCyan-3, kCyan -9
//						# kYellow
//						# kOrange, kOrange+10, kOrange-3, kOrange+4
//						# kMagenta, kMagenta+3, kMagenta-7
//						# kViolet, kViolet-3, kViolet+6, kViolet+1
//*********************************************************************************
void HistoSetMarkerAndColor( TH1* histo1,
                            Style_t markerStyle, Size_t markerSize, Color_t markerColor, Color_t lineColor ) {
    histo1->SetMarkerStyle(markerStyle);
    histo1->SetMarkerSize(markerSize);
    histo1->SetMarkerColor(markerColor);
    histo1->SetLineColor(lineColor);
}

//*********************************************************************************
// Start of main function
// parameters:
//  * filename         - name of the tree-input file
//  * optionCollSystem - selection of collision system should be either "pp" or
//		                   "PbPb"
//  * optionTest       - test option will allow not to analyse the full data sample
//                       at first but just 100 events
//  * startCentrality  - gives the starting point of your centrality bin
//  * endCentrality 	  - gives the end point of your centrality bin
//*********************************************************************************
void AnalyseTreeForRAAComplete(Int_t startCentrality = 0., Int_t endCentrality = 100.){
    
    //  TString filename = "../../ALICE-DVD/Part2/MasterClassesTree_LHC10h_Run139036.root";
    TString filename = gSystem->Getenv("FILENAME");
    
    //  TString optionCollSystem = "PbPb";
    TString optionCollSystem = gSystem->Getenv("COLLSYS");
    
    TString optionTest= "kFalse";
    
    //Reset ROOT and connect tree file
    gROOT->Reset();
    StyleSettings();
    
    gStyle->SetOptStat(0);
    
    
    cout << "Analysing file " <<  filename.Data() << endl;
    cout << "Collision system chosen: " << optionCollSystem.Data() << endl;
    if (optionCollSystem.CompareTo("PbPb")==0){
        cout << "Centrality chosen between: " << startCentrality << " - " << endCentrality << endl;
    }
    
    //*********************************************************************************
    //Attaching the file
    //*********************************************************************************
    TFile *f = new TFile(filename.Data());
    
    //*********************************************************************************
    //Declaration of leaves types for track tree
    //*********************************************************************************
    Float_t         trackCentrality;  // variable for the centrality in the track tree
    Double_t        trackPt;			// variable for the transverse momentum of the track
    
    //*********************************************************************************
    //Declaration of leaves types for event tree
    //*********************************************************************************
    Float_t         eventCentrality;	// variable for the centrality in the event tree
    Int_t           eventMult;			// variable for the multiplicity in the event
    
    //*********************************************************************************
    // Definition of input trees
    //*********************************************************************************
    TTree *Track = (TTree*)gDirectory->Get("Track");  // stores track properties
    TTree *Event = (TTree*)gDirectory->Get("Event");  // stores event properties
    
    
    // Set branch addresses for track tree
    Track->SetBranchAddress("centrality",&trackCentrality);
    Track->SetBranchAddress("trackpt",&trackPt);
    // Set branch addresses for event tree
    Event->SetBranchAddress("nTPCAccepted",&eventMult);
    Event->SetBranchAddress("centrality",&eventCentrality);
    
    //*********************************************************************************
    // maximum number of files for testing
    //*********************************************************************************
    ULong64_t nEntriesTest = 1000;
    
    //*********************************************************************************
    // Getting the number of entries in the track tree
    //*********************************************************************************
    ULong64_t nEntriesTrack = Track->GetEntries();
    cout << "You have " << nEntriesTrack << " for the track tree" << endl;
    if (optionTest.CompareTo("kTRUE") == 0){
        cout << "You will at maximum analyse "<< nEntriesTest <<" tracks in the tree for testing "  << endl;
        if (nEntriesTrack > nEntriesTest){
            nEntriesTrack = nEntriesTest;
        }
    }
    
    //*********************************************************************************
    // Getting the number of entries in the event tree
    //*********************************************************************************
    ULong64_t nEntriesEvent = Event->GetEntries();
    cout << "You have " << nEntriesEvent << " for the event tree" << endl;
    if (optionTest.CompareTo("kTRUE") == 0){
        cout << "You will at maximum analyse "<< nEntriesTest <<" events in the tree for testing "  << endl;
        if (nEntriesEvent > nEntriesTest){
            nEntriesEvent = nEntriesTest;
        }
    }
    
    //*********************************************************************************
    // Definition of bins in transverse momentum pt, due to steply falling spectrum
    // and not enough statistics at high pt
    //*********************************************************************************
    Int_t fNBinsPt = 54;
    Double_t fBinsPt[55] = 	{0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
        0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
        1, 1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9,
        2, 2.2, 2.4, 2.6, 2.8, 3, 3.2, 3.4, 3.6, 3.8,
        4, 4.5, 5, 5.5, 6, 6.5, 7, 8, 9, 10,
        11, 12, 13, 14, 15};
    
    
    
    //*********************************************************************************
    // Defintion of histograms:
    // * to be filled with trackpt and number of charged tracks in the TPC (TH1*)
    // * correlation between centrality an nTracks TPC (TH2F)
    //*********************************************************************************
    
    TH1D* htrackPt = new TH1D("htrackPt","track pt",fNBinsPt,fBinsPt);
    TH1F *hNTPC = new TH1F("hNTPC","Number of TPC tracks ",200,0,2000);
    TH2F *hNTPCvsCent = new TH2F("hNTPCvsCent","Number of TPC tracks ",200,0,2000, 100, 0, 100);
    
    //*********************************************************************************
    // Definition of correction histogram in order to normalise for the binwidth in
    // Pt
    //*********************************************************************************
    TH1D* fDeltaPt = new TH1D("deltaPt","",fNBinsPt,fBinsPt);
    for(Int_t iPt=1;iPt<fNBinsPt+1;iPt++){
        fDeltaPt->SetBinContent(iPt,fBinsPt[iPt]-fBinsPt[iPt-1]);
        fDeltaPt->SetBinError(iPt,0);
    }
    
    //*********************************************************************************
    // * Reading the entries form the event tree (extract nTracksTPC) and filling
    //   it in the multiplicity histograms (hNTPC,hNTPCvsCent)
    // * Distinction between PbPb and pp as for PbPb you need to fill them with
    //   restriction in centrality
    // * nEntriesPerCent will give you the normalization value for the different
    //	  centralities
    //*********************************************************************************
    
    ULong_t nEntriesPerCent= 0;
    ULong64_t nbytes2 = 0;
    for (ULong_t i=0; i<nEntriesEvent;i++) {
        nbytes2 += Event->GetEvent(i);
        if (eventCentrality > startCentrality && eventCentrality < endCentrality){
            hNTPC->Fill(eventMult);
            hNTPCvsCent->Fill(eventMult,eventCentrality);
            nEntriesPerCent++;
        }
        // give an output for every 10Mio events processed to see that is working
        if ( i%10000000 == 0 ) {
            cout << i/10000000 << " * 10^7 events have been processed" << endl;
        }
    }
    
    
    //*********************************************************************************
    // * Reading the entries form the track tree (extract pt values) and filling it
    //   in the pt-histogram
    // * Distinction between PbPb and pp as for PbPb you need to fill them with
    //   restriction in centrality
    //*********************************************************************************
    ULong64_t nbytes = 0;
    cout << nEntriesTrack/100000000 << " * 10^8 tracks have to be processed" << endl;
    for (ULong_t i=0; i<nEntriesTrack;i++) {
        nbytes += Track->GetEvent(i);
        /// To do: here you need to add something
        
        if (trackCentrality > startCentrality && trackCentrality < endCentrality){
            htrackPt->Fill(trackPt);
        }
        
        // give an output for every 10Mio events processed to see that is working
        if ( i%10000000 == 0 ) {
            cout << i/10000000 << " * 10^7 events have been processed" << endl;
        }
        
    }
    
    
    //*********************************************************************************
    //	Definition of canvas
    //*********************************************************************************
    TCanvas *c1 = new TCanvas("c1","Pt");
    TCanvas *c2 = new TCanvas("c2","nTracksTPC");
    TCanvas *c3 = new TCanvas("c3","nTracksTPCvsCent");
    //*********************************************************************************
    // * Axis labeling for multiplicity histogram and plotting it to an eps file with
    //   the name dependent on the collision system and centrality
    //*********************************************************************************
    hNTPC->Sumw2();
    hNTPC->GetYaxis()->SetTitleOffset(1.3);
    hNTPC->Scale(1./nEntriesPerCent);
    hNTPC->SetXTitle("number of TPC tracks");
    hNTPC->SetYTitle("Counts/N_{evt}");
    c2->cd();
    c2->SetLogy(1);
    HistoSetMarkerAndColor( hNTPC, 20, 1., kBlue, kBlue );
    hNTPC->Draw("e1,p");
    c2->SaveAs(Form("nTracksTPC_%i-%i.eps",startCentrality,endCentrality));
    
    
    hNTPCvsCent->SetXTitle("number of TPC tracks");
    hNTPCvsCent->SetYTitle("Centrality");
    hNTPCvsCent->GetYaxis()->SetTitleOffset(1.3);
    c3->cd();
    c3->SetLogy(0);
    c3->SetLogz(1);
    c3->SetRightMargin(0.1);
    hNTPCvsCent->Draw("colz");
    c3->SaveAs(Form("nTracksTPCvsCent_%i-%i.eps",startCentrality,endCentrality));
    
    
    
    //*********************************************************************************
    // * Normalisation of the pt-histogram to the number of entries in this
    //   centrality bin and to the bin with in pt by deviding through the created
    //   histogram for deltaPt (fDeltaPt)
    // * Labeling of the histograms and plotting it to an eps file with the name
    //   dependent on the collision system and the centralities
    //*********************************************************************************
    cout << "Entries total: " << nEntriesEvent << "\t Entries per Centrality: "  << nEntriesPerCent << endl;
    
    /// To do:The following commands need to be uncommented and you need to add the plotting of the
    ///        pt-histogram, don't forget to label your axis.
    
    htrackPt->Sumw2();
    htrackPt->Divide(fDeltaPt);
    htrackPt->Scale(1./nEntriesPerCent);
    htrackPt->GetYaxis()->SetTitleOffset(1.3);
    htrackPt->SetXTitle("p_{T} (GeV/c)");
    htrackPt->SetYTitle("dN/dp_{T} (c/GeV)");
    c1->cd();
    c1->SetLogy(1);
    htrackPt->Draw();
    c1->SaveAs(Form("hTracksPt_%i-%i.eps",startCentrality,endCentrality));
    
    //*********************************************************************************
    // Saving the histograms to a root-file for further processing for the RAA
    // * the option "UPDATE" in the end will allow to save different centrality classes
    //   in one file as it just creates the file vom scratch if it is non existent
    //   and just overwrites the histograms which are written if it exists
    //*********************************************************************************
    
    TFile* fileOutput = new TFile("RAABaseOutput.root","UPDATE");
    hNTPC->Write(Form("nTracksTPC_%s_%i-%i",optionCollSystem.Data(),startCentrality,endCentrality),TObject::kOverwrite);
    hNTPCvsCent->Write(Form("hNTPCvsCent_%s_%i-%i",optionCollSystem.Data(),startCentrality,endCentrality),TObject::kOverwrite);
    /// To do: uncomment this as soon as you are ready 
    htrackPt->Write(Form("trackPt_%s_%i-%i",optionCollSystem.Data(),startCentrality,endCentrality),TObject::kOverwrite);
    fileOutput->Write();
    fileOutput->Close();
    
}

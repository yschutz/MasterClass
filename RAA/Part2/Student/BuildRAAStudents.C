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
  gStyle->SetTitleOffset(1.7,"y");
  gStyle->SetTitleOffset(0.7,"x");		
  gStyle->SetCanvasColor(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetLineWidth(0.01);
  gStyle->SetPadTopMargin(0.06);
  gStyle->SetPadBottomMargin(0.09);
  gStyle->SetPadRightMargin(0.04);
  gStyle->SetPadLeftMargin(0.11	);	
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
//  * filename         - name of the input-file
//*********************************************************************************
void BuildRAAStudents(TString filename = "RAABaseOutput.root"){

  //________Reset ROOT_______________________________________________________________
  gROOT->Reset();
	
  //________Set basic style setting__________________________________________________
  StyleSettings();				
  cout << "Analysing file " <<  filename.Data() << endl;
  
  //*********************************************************************************
  //Definition of number of Collisions in the different centrality classes
  //*********************************************************************************   
  Double_t nColl_0_5 =1686.87;	
  Double_t nColl_0_10 = 1502.7;
  Double_t nColl_5_10 = 1319.89;
  Double_t nColl_10_20 = 923.26;
  Double_t nColl_20_30 = 558.68;
  Double_t nColl_30_40 = 321.20;
  Double_t nColl_40_50 = 171.67;
  Double_t nColl_50_60 = 85.13;
  Double_t nColl_60_70 = 38.51;
  Double_t nColl_70_80 = 15.78;
  Double_t nColl_80_90 = 6.32;
	
  //*********************************************************************************
  //Attaching & reading the input-file
  //*********************************************************************************   
  TFile fileInput(filename.Data());  
  //____ reading the number of TPC tracks for pp events & 0-5% PbPb events___________
  TH1D *hNTracksTPCPbPb_0_5 =			(TH1D*)fileInput.Get(Form("nTracksTPC_PbPb_%i-%i",0,5));
  TH1D *hNTracksTPCPbPb_70_80 =			(TH1D*)fileInput.Get(Form("nTracksTPC_PbPb_%i-%i",70,80));
  /// To do: Do the same for the other centralities 
  
  //_____reading the pt-spectrum for 0-5% & 70-80% PbPb events, scaling it___________
  //_____by corresponding number of Collisions (nColl)_______________________________
  TH1D *hTrackPtPbPb_0_5 =			(TH1D*)fileInput.Get(Form("trackPt_PbPb_%i-%i",0,5));
  hTrackPtPbPb_0_5->Scale(1./nColl_0_5);
  TH1D *hTrackPtPbPb_70_80 =			(TH1D*)fileInput.Get(Form("trackPt_PbPb_%i-%i",70,80));
  hTrackPtPbPb_70_80->Scale(1./nColl_70_80);
  /// To do: Do the same for the other centralities 
  
  //*********************************************************************************
  // Attaching and Reading pp-reference file
  //*********************************************************************************   
  TFile *fileInputPP   = new TFile("PP_2760GeV_BaseLine.root");
  TH1F  *hNTracksTPCpp = (TH1F*)fileInputPP->Get("nTracksTPC_pp");	
  TH1D  *hTrackPtpp    = (TH1D*)fileInputPP->Get("trackPt_pp");	
  
  //*********************************************************************************
  // Building the RCP
  //*********************************************************************************
  TH1D*	hRCP_0_5 = (TH1D*)hTrackPtPbPb_0_5->Clone("RCP_vs_Pt_0-5");
  hRCP_0_5->Sumw2();
  hRCP_0_5->Divide(hTrackPtPbPb_70_80);
  /// To do: Do the same for the other centralities 
  
  //*********************************************************************************
  // Building the RAA
  //*********************************************************************************   
  
  /// To Do: Here you need to add the histograms for RAA (PbPb-pt-spectrum in a certain centrality, scaled by NColl,  devided by the pp reference spectrum
	
	
	
	
  //*********************************************************************************
  //Definition of canvas
  //*********************************************************************************
  TCanvas *c1 = new TCanvas("c1","Pt");
  TCanvas *c2 = new TCanvas("c2","nTracksTPC");
  TCanvas *c3 = new TCanvas("c3","RCP");
  TCanvas *c4 = new TCanvas("c4","RAA");
  
  //*********************************************************************************
  // * Plotting the track pt for 0-5, 70-80 PbPb events and pp events
  //*********************************************************************************	
  c1->cd(); 
  c1->SetLogy(1);
  hTrackPtpp->GetYaxis()->SetTitleOffset(1.3);
  HistoSetMarkerAndColor( hTrackPtpp, 20,0.8, kRed, kRed); 
  hTrackPtpp->Draw("e1");
  
  HistoSetMarkerAndColor( hTrackPtPbPb_0_5, 20,0.8, kBlue+2, kBlue+2); 
  hTrackPtPbPb_0_5->Draw("same,e1");
  HistoSetMarkerAndColor( hTrackPtPbPb_70_80, 20,0.8, kMagenta+2, kMagenta+2); 
  hTrackPtPbPb_70_80->Draw("same,e1");
  /// To do: Add different centralities to this plot and the legend
  
  TLegend* legendTrackPt = new TLegend(0.55,0.70,0.95,0.9);
  legendTrackPt->SetTextSize(0.03);			
  legendTrackPt->SetFillColor(0);
  legendTrackPt->SetLineColor(0);
  legendTrackPt->SetNColumns(2);
  legendTrackPt->AddEntry(hTrackPtPbPb_0_5,"PbPb 0-5 %");
  legendTrackPt->AddEntry(hTrackPtPbPb_70_80,"PbPb 70-80 %");
  legendTrackPt->AddEntry(hTrackPtpp,"pp");
  legendTrackPt->Draw();

  c1->SaveAs("trackPt_compared.eps");	
	
   
  //*********************************************************************************
  // * Plotting of the multiplicity histogram for 0-5%, 70-80% PbPb & pp
  //*********************************************************************************
			
  c2->cd(); 
  c2->SetLogy(1);
  hNTracksTPCpp->GetYaxis()->SetRangeUser(5.e-6,1.);
  hNTracksTPCpp->GetYaxis()->SetTitleOffset(1.3);
  HistoSetMarkerAndColor( hNTracksTPCpp, 20,0.8, kRed, kRed); 
  hNTracksTPCpp->Draw("e1");
  HistoSetMarkerAndColor( hNTracksTPCPbPb_0_5, 20,0.8, kBlue+2, kBlue+2); 
  hNTracksTPCPbPb_0_5->Draw("same,e1");
  HistoSetMarkerAndColor( hNTracksTPCPbPb_70_80, 20,0.8, kMagenta+2, kMagenta+2); 
  hNTracksTPCPbPb_70_80->Draw("same,e1");	
  /// To do: Add different centralities to this plot and the legend
	
  legendTrackPt->Draw();
  c2->Update();
  c2->SaveAs("nTracksTPC_compared.eps");	
			
  //**********************************************************************************
  // Axis labeling & plotting of the RCP for 0-5% 
  //**********************************************************************************
  c3->cd(); 
  c3->SetLogy(0);
  hRCP_0_5->SetTitle("R_{CP} for different centralities");
  hRCP_0_5->SetXTitle("p_{t} (GeV/c)");
  hRCP_0_5->SetYTitle("R_{CP} = #frac{1/N^{AA,X}_{evt} 1/<N_{coll,X}> dN^{AA,X}/dp_{t}}{1/N^{PbPb,per}_{evt} 1/<N_{coll,per}> dN^{PbPb,per}/dp_{t}}");
  hRCP_0_5->GetYaxis()->SetTitleOffset(1.3);
  hRCP_0_5->GetYaxis()->SetRangeUser(0.,1.5);
	
  HistoSetMarkerAndColor( hRCP_0_5, 20,0.8, kBlue+2, kBlue+2); 
  hRCP_0_5->Draw("e1");
  /// To do: Add different centralities to this plot and the legend
	
  TLegend* legendRCP = new TLegend(0.15,0.78,0.55,0.9);
  legendRCP->SetTextSize(0.03);			
  legendRCP->SetFillColor(0);
  legendRCP->SetLineColor(0);
  legendRCP->SetNColumns(4);
  legendRCP->AddEntry(hRCP_0_5,"0-5 %");
  legendRCP->Draw();

  c3->Update();
  c3->SaveAs("RCP_compared.eps");	
	
   
  //**********************************************************************************
  // Axis labeling & plotting of the RAA in the different classes
  //**********************************************************************************
  /// To do: Plot RAA in the different centrality classes, use RCP plot as an example
	
	
}

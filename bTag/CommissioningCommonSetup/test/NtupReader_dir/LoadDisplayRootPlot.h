/////////////////////////////////////////////////////////////////
// Author : Caroline Collard (IPHC) - first commit : april 2011
////////////////////////////////////////////////////////////////
#include "Configuration.h"
#include <vector>
#include <string>
#include <TString.h>
#include <math.h>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <THStack.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TAxis.h>

using namespace std;

const TString format="pdf";

#define PLOT_COMMON_TITLE "CMS 2011 preliminary, #sqrt{s} = 8 TeV,  488 pb^{-1}"

#define MAXNFILES 10
////// IMPORTANT : CHANGE THE DEFINITION OF THE CONSTRUCTOR OF THE CLASS "Plots"  TO CORRECT THE FILE LOCATION
template<typename T> class Plots {
 public:
  Plots(Bool_t mu,Float_t ptval,UInt_t datayear);
  ~Plots() {
    for (UInt_t i=0; i<Nfiles&&i<MAXNFILES; i++)
      mc[i]->Close(); 
    data->Close();
    //data2->Close();
  }
  T * histo0_Data;
  T * histo0_Data1;
  T * histo0_Data2;
  T * histo0_Data3;

  //  T * histo0_2010;
  T * histo0_MC;
  T * histo0_MC_b;
  T * histo0_MC_c;
  T * histo0_MC_udsg;
  T * histo0_MC_gspl;

  T * histo0_ratio;
  T * histo0_ratio2;

  T * histo_MC[MAXNFILES];
  T * histo_MC_b[MAXNFILES];
  T * histo_MC_c[MAXNFILES];
  T * histo_MC_udsg[MAXNFILES];
  T * histo_MC_gspl[MAXNFILES];
  T * histo_2MC[MAXNFILES];
  T * histo0_2MC;
  TCanvas *canvas;
  TPad *canvas_1,*canvas_2;
 protected:
  Bool_t mu_;
  Float_t ptval_;
  UInt_t year_;
  char dir_[100];
  TFile * mc[MAXNFILES];
  TFile * data;//,*data2;
  UInt_t Nfiles;
};

template<typename T> Plots<T>::Plots( Bool_t mu, Float_t ptval, UInt_t datayear ) : mu_(mu),ptval_(ptval),year_(datayear) {
  char filename[300];
  vector<string> nameroot;
  if (mu==false)
    sprintf(dir_,"dir_jet%.0fpu/",ptval_);
  else
    sprintf(dir_,"dir_btag%.0fpu/",ptval_);
  printf("<<<<<<<< Opening directory %s >>>>>>>>\n",dir_);
  const UInt_t nSamples=sizeof(MC_Weights)/sizeof(Float_t);
  for (UInt_t iSample=0;iSample<nSamples;iSample++) 
    if (
	strstr(MC_SampleNames[iSample],"qcd")!=NULL&&
	strlen(MC_files[iSample])>0&&
	((strstr(MC_SampleNames[iSample],"qcdmu")!=NULL&&mu)||
	 (strstr(MC_SampleNames[iSample],"qcdmu")==NULL&&mu==false))
	) {
      printf("MC%d: histo_%s.root is added.\n",iSample,MC_SampleNames[iSample]);
      nameroot.push_back(string(dir_)+"histo_"+string(MC_SampleNames[iSample])+".root");
    }

  for (UInt_t i=0; i<nameroot.size()&&i<MAXNFILES; i++) {
    mc[i] = (TFile*) gROOT->GetListOfFiles()->FindObject(nameroot[i].c_str());
    if (!mc[i] || !mc[i]->IsOpen()) mc[i] = new TFile(nameroot[i].c_str());
  }
  Nfiles=nameroot.size();
  string dataroot2;
  if (datayear==2010) {
    sprintf(filename,"%shisto_minijet2010.root",dir_);
  }
  else if (datayear>=2011) {
    if (mu==false) sprintf(filename,"%shisto_minijet%d.root",dir_,datayear);
    else sprintf(filename,"%shisto_minibtag%d.root",dir_,datayear);
  }
  printf("DATA: %s.\n",filename);
  data   = new TFile(filename);
/*
  if (mu==false) {dataroot2=string(dir_)+"histo_minijet2010.root"; } // pas de 2010 pour l'instant
  else           {dataroot2=string(dir_)+"histo_minibtag2011.root"; }  // AS NO 2010 BTAG DATA YET, PUT 2011 TO AVOID CRASH...
  data2  = new TFile(dataroot2.c_str());
*/
}

void OverFlowBinFix(TH1F* );

class Plots1D:public Plots<TH1F>{
 public:
 Plots1D(Bool_t mu,Float_t ptval,UInt_t datayear) : Plots<TH1F>(mu,ptval,datayear){};
  void PlotStack(TString selection, TString label, bool down=false, bool logy=false, bool bOverflow=false, TString labely="jets");
  void TagRate(TString selection, TString label, bool down=false, bool logy=false,  TString labely="tag rate");
  void PlotData(TString selection, TString label, bool down=false, bool logy=false, bool bOverflow=false);
};

class Plots2D:public Plots<TH2F>{
 public:
 Plots2D(Bool_t mu,Float_t ptval,UInt_t datayear) : Plots<TH2F>(mu,ptval,datayear){};
 void PlotStack2D(TString selection, TString label, TString labely, bool logy, float ymin=0., float ymax=10.);
 void PlotProfile2D(TString selection, TString label, TString labely, bool logy, float ymin, float ymax, Bool_t isdata=true,string fitequ="");
};

void Plots1D::PlotStack(TString selection, TString label, bool down, bool logy, bool bOverflow, TString labely){
  //  TH1F * histo0_ratio2;
  
  //###################################################
  //###################################################

  try{
    histo0_Data = (TH1F*) data->Get(selection);
    //histo0_2010 = (TH1F*) data2->Get(selection);
  }
  catch(...) {
    cerr<<selection<<" is not found in data!"<<endl;
  }

  TString bsel = selection+"_b";
  TString csel = selection+"_c";
  TString lsel = selection+"_udsg"; 
  TString gsel = selection+"_gspl";
  Bool_t DoNotStack=false;
  if (selection=="npv" || selection=="pthat" || selection=="npv_no_scaled" || selection=="npu") DoNotStack=true;
  if (DoNotStack) {
    // IF selection=="npv" or selection=="pthat", no _b info
    // PUT DUMMY NAMES TO AVOID CRASH...
    bsel = "sv_jetpt_b";
    csel = "sv_jetpt_b";
    lsel = "sv_jetpt_b";
    gsel = "sv_jetpt_b";
  }

  for (UInt_t i=0; i<Nfiles; i++) {
    try{
      histo_MC[i]     =(TH1F*)mc[i]->Get(selection);
      histo_MC_b[i]   =(TH1F*)mc[i]->Get(bsel);
      histo_MC_c[i]   =(TH1F*)mc[i]->Get(csel);
      histo_MC_udsg[i]=(TH1F*)mc[i]->Get(lsel);
      histo_MC_gspl[i]=(TH1F*)mc[i]->Get(gsel);
      //   if (selection=="npv") cout <<  " somme MC :      i = " << i  << endl;
 
      if (i==0) {
	histo0_MC=      (TH1F*)mc[i]->Get(selection);
	histo0_MC_b=    (TH1F*)mc[i]->Get(bsel);
	histo0_MC_c=    (TH1F*)mc[i]->Get(csel);
	histo0_MC_udsg= (TH1F*)mc[i]->Get(lsel);
	histo0_MC_gspl= (TH1F*)mc[i]->Get(gsel);
	//    histo0_MC_b->Add(histo_MC_gspl[0]);
      }
      else {
	histo0_MC->Add(histo_MC[i]);
	histo0_MC_b->Add(histo_MC_b[i]);
	//    histo0_MC_b->Add(histo_MC_gspl[i]);
	histo0_MC_c->Add(histo_MC_c[i]);
	histo0_MC_udsg->Add(histo_MC_udsg[i]);
	histo0_MC_gspl->Add(histo_MC_gspl[i]);
      }
    }
    catch(...) {
      cerr<<"Either "<<bsel<<" or "<<csel<<" or "<<lsel<<" or "<<gsel<<" is not found!"<<endl;
    }
  }
  // fix overflow bins:
  if(bOverflow){
    OverFlowBinFix(histo0_MC);
    OverFlowBinFix(histo0_MC_b);
    OverFlowBinFix(histo0_MC_c);
    OverFlowBinFix(histo0_MC_udsg);
    OverFlowBinFix(histo0_MC_gspl);
    OverFlowBinFix(histo0_Data);
    //OverFlowBinFix(histo0_2010);
  }

  // equivalent for ratio plot around line 490

  //  Double_t titleoffsetx=0.13;
  Double_t titleoffsety=0.2;
  Double_t titlesizex=0.17;
  Double_t titlesizey=0.2;
  Double_t labelsizex=0.14;
  Double_t labelsizey=0.12;

  histo0_Data->GetYaxis()->SetLabelSize(labelsizey);
  histo0_Data->GetYaxis()->SetTitleSize(titlesizey);
  histo0_Data->GetYaxis()->SetTitleOffset(titleoffsety);
  histo0_MC->GetYaxis()->SetLabelSize(labelsizey);
  histo0_MC->GetYaxis()->SetTitleSize(titlesizey);
  histo0_MC->GetYaxis()->SetTitleOffset(titleoffsety);

 
  // SCALE MC TO DATA
  Float_t mctotal=histo0_MC->Integral();
  Float_t scaleparam=histo0_Data->Integral()/mctotal;
  if (!DoNotStack) {
  // cout<<"Check MC entries: "<<endl<<mctotal<<"(MCTotal)="<<histo0_MC_b->Integral()<<"(b)+"<<histo0_MC_c->Integral()<<"(c)+"<<histo0_MC_udsg->Integral()<<"(udsg)+"<<histo0_MC_gspl->Integral()<<"(bglsplitting)"<<endl;
    cout<<"histo0_MC: "<<mctotal<<"; Fractions Sum Over: "<<histo0_MC_b->Integral()+histo0_MC_c->Integral()+histo0_MC_udsg->Integral()+histo0_MC_gspl->Integral()<<endl;
    histo0_MC_b->Scale(scaleparam);
    histo0_MC_c->Scale(scaleparam);
    histo0_MC_udsg->Scale(scaleparam);
    histo0_MC_gspl->Scale(scaleparam);
    histo0_MC->Scale(scaleparam);
  }
  else {
    float scaleaa=1./histo0_MC->Integral();
    float scalebb=1./histo0_Data->Integral();
    histo0_MC_b->Scale(scaleaa);
    histo0_MC_c->Scale(scaleaa);
    histo0_MC_udsg->Scale(scaleaa);
    histo0_MC_gspl->Scale(scaleaa);
    histo0_MC->Scale(scaleaa);
    histo0_Data->Scale(scalebb);
    histo0_MC->SetMaximum(histo0_Data->GetMaximum()*1.1);

  }


  // SCALE 2010 TO 2011
  // float scaledata=histo0_Data->Integral()/histo0_2010->Integral();
  // histo0_2010->Scale(scaledata);


  // MAKE DATA/MC RATIO
  histo0_ratio = (TH1F*) histo0_Data->Clone();
  histo0_ratio->SetName("histo0_ratio");
  histo0_ratio->SetTitle("");


  // REBIN 

  if (selection=="muon_Pt"  ) {
    histo0_ratio->Rebin(2);
    histo0_Data->Rebin(2);
    //histo0_2010->Rebin(2);
    histo0_MC->Rebin(2);
    histo0_MC_b->Rebin(2);
    histo0_MC_c->Rebin(2);
    histo0_MC_udsg->Rebin(2);
    histo0_MC_gspl->Rebin(2);
  }



  histo0_ratio->Divide(histo0_MC);
  /*
  if (selection=="npv_no_scaled" ){
    for (int iii=1; iii<histo0_ratio->GetNbinsX(); iii++) {
      cout << iii-1 << " " << histo0_ratio->GetBinContent(iii) << endl;
    }
  }
  else if (selection=="npu"){
    for (int iii=1; iii<histo0_MC->GetNbinsX(); iii++) {
      cout << iii-1 << " " << histo0_MC->GetBinContent(iii) << endl;
    }
  }
  */
 
  // MAKE 2010/2011 RATIO 
  /*
  TH1F * histo0_rat2010 = (TH1F*) histo0_2010->Clone();
  histo0_rat2010->SetName("histo0_rat2010");
  histo0_rat2010->Divide(histo0_MC);

  TH1F * histo0_ratdata = (TH1F*) histo0_Data->Clone();
  histo0_ratdata->SetName("histo0_ratdata");
  histo0_ratdata->Divide(histo0_2010);
  */

  // SET COLORS
  histo0_Data->SetLineWidth(2);
  histo0_MC->SetLineColor(2);
  histo0_MC_b->SetFillColor(2);
  histo0_MC_c->SetFillColor(8);
  histo0_MC_gspl->SetFillColor(7);
  histo0_MC_udsg->SetFillColor(4);

  // DO STACK
  THStack* hs= new THStack(selection,label+";"+label+";entries");
  if (!DoNotStack) {
    if (down==false) {
      hs->Add(histo0_MC_udsg);
      hs->Add(histo0_MC_c);
      hs->Add(histo0_MC_gspl); 
      hs->Add(histo0_MC_b);
    }
    else {
      hs->Add(histo0_MC_b);
      hs->Add(histo0_MC_gspl);  
      hs->Add(histo0_MC_c);
      hs->Add(histo0_MC_udsg);
    }
  }
  else hs->Add(histo0_MC);
  
  // SET COSMETICS
  histo0_Data->SetMarkerStyle(20);
  histo0_Data->SetMarkerSize(0.75);
  // histo0_Data->GetXaxis()->SetTitle(histo0_MC->GetName());  
  histo0_Data->GetXaxis()->SetTitle(label);
  histo0_Data->GetYaxis()->SetTitle(labely);

  //  if (selection=="npv" || selection=="npv_no_scaled") histo0_Data->GetXaxis()->SetTitle(histo0_MC->GetTitle());  
  /*
  histo0_2010->SetMarkerStyle(21);
  histo0_2010->SetMarkerColor(7);
  histo0_2010->SetLineColor(7);
  histo0_2010->SetMarkerSize(0.5);
  */
  //  hs->SetTitle(histo0_Data->GetTitle());

  if (selection=="sv_vtxsum_phi" || selection=="sv_vtxdir_phi" || selection=="sv_vtxsum_eta" || selection=="sv_vtxdir_eta") {
    hs->SetMaximum(hs->GetMaximum()*1.5);
    hs->SetMinimum(0.);
    histo0_Data->SetMinimum(0.);
    histo0_Data->SetMaximum(histo0_Data->GetMaximum()*1.5);
  }
  if (selection=="trackNHits" || selection=="trackNHits_cut") {
    hs->SetMaximum(hs->GetMaximum()*1.25);
    histo0_Data->SetMaximum(histo0_Data->GetMaximum()*1.25);
  }
  if (selection=="trackIP3dsig" || selection=="trackIP3dsig_sel"
      || selection=="trackIP3dsig_cutsel" ) {
    hs->SetMaximum(hs->GetMaximum()*1.5);
    histo0_Data->SetMaximum(histo0_Data->GetMaximum()*1.5);
  }

  if (selection=="sv_flightsig3d_3tr") {
    hs->SetMaximum(hs->GetMaximum()*1.5);
    histo0_Data->SetMaximum(histo0_Data->GetMaximum()*1.5);
  }

  gStyle->SetOptTitle(0);

  // CREATE CANVAS
  canvas = new TCanvas("c1", "c1",10,32,782,552);
  canvas->cd();

 


  // FIRST MC & DATA
  canvas_1 = new TPad("canvas_1", "canvas_1",0,0.25,1.0,0.98);
  canvas_1->Draw();
  canvas_1->cd();


  if (histo0_Data->GetMaximum() > hs->GetMaximum() ) {
    hs->SetMaximum(histo0_Data->GetMaximum() );
  }
  if (selection=="sv_eratio") {
    hs->SetMaximum(hs->GetMaximum() *1.25 );
    if (ptval_==70. && mu_==true) hs->SetMaximum(hs->GetMaximum() *1.25 );
    if (hs->GetMaximum()<histo0_Data->GetMaximum()) hs->SetMaximum(histo0_Data->GetMaximum()*1.1 );
    if (ptval_==110. && mu_==true) hs->SetMaximum(hs->GetMaximum() *2. );
  }
  if (ptval_==110. && mu_==true && selection=="sv_vtx_pt" ) hs->SetMaximum(hs->GetMaximum() *1.5 );

  if (selection=="sv_flightsig3d") hs->SetMinimum(10);
  hs->Draw("hist");
  hs->GetHistogram()->SetTitleSize(0.08,"Y");
  hs->GetHistogram()->SetTitleOffset(0.45,"Y");
 
  if (selection!="pthat" && selection!="npu") histo0_Data->Draw("e same");

  // ADD LEGEND
  TLegend* qw = 0;
  if (mu_==false) {
    if (selection=="sv_eratio" || selection=="sv_eratio_3tr") {
      qw = new TLegend(0.54,0.63,0.88,0.9);
    }
    else if (selection=="IP3d1" || selection=="IP3d1sorted" || selection=="trackIP3d" || selection=="trackIP3d_cutsel") {
      qw = new TLegend(0.12,0.63,0.42,0.9);
    }
    else if (selection == "sv_normchi2"){
      qw = new TLegend(0.54,0.63,0.88,0.9);
    }
    else {
      qw = new TLegend(0.65,0.73,0.95,1.);
    }
  } 
  else {
    if (selection=="sv_eratio") {
      qw = new TLegend(0.12,0.63,0.42,0.9);
    }
    else if (selection=="IP3d1" || selection=="IP3d1sorted" || selection=="IP3d1sig" || selection=="IP3d1sigsorted" || selection=="sv_ipsigcharm" ||
	     selection=="IP3d2" || selection=="IP3d2sorted" || selection=="IP3d2sig" || selection=="IP3d2sigsorted" || 
	     selection=="IP3d3" || selection=="IP3d3sorted" || selection=="IP3d3sig" || selection=="IP3d3sigsorted"  
	     ) {
      qw = new TLegend(0.12,0.63,0.42,0.9);
    }
    else if (selection=="sv_mass_muenriched" || selection=="sv_normchi2"){
      qw = new TLegend(0.54,0.63,0.88,0.9);
    }
    else {
      qw = new TLegend(0.65,0.73,0.95,1.);
    }
  }
  /*
  if (selection!="pthat" && selection!="npu") {
    if (year_==2010) {
      //     qw->SetHeader("CMS Preliminary, 33 pb^{-1}");
      qw->AddEntry(histo0_Data,        "JET Run2010B ",           "p");
    }
    else if (year_==2011) {
      if (mu_==false) {
	if (ptval_==30) {
	  //       qw->SetHeader("2011 effective lumi : 0.0053  pb^{-1}"); // 30
	  qw->AddEntry(histo0_Data,        "data (HLT_JET30)",           "p"); 
	}
	else if (ptval_==60.) { 
	  //       qw->SetHeader("2011 effective lumi : 0.1  pb^{-1}"); // 60
	  qw->AddEntry(histo0_Data,        "data (HLT_JET60)",           "p"); 
	}
	else if (ptval_==80.) {
	  //       qw->SetHeader("2011 effective lumi : 0.52  pb^{-1}"); // 80
	  qw->AddEntry(histo0_Data,        "data( HLT_JET80)",           "p"); 
	}
	else if (ptval_==110.) {
	  //       qw->SetHeader("2011 effective lumi : 2.5  pb^{-1}"); // 110
	  qw->AddEntry(histo0_Data,        "data (HLT_JET110)",           "p"); 
	}

      }
      else  { 
	if (ptval_==20.) {
	  //       qw->SetHeader("2011 effective lumi : 1.9  pb^{-1}"); // 20
	  qw->AddEntry(histo0_Data,        "data (HLT_BTagMu)",           "p"); 
	}
	else if (ptval_==60.) { 
	  //       qw->SetHeader("2011 effective lumi : 24.3  pb^{-1}"); // 60
	  qw->AddEntry(histo0_Data,        "data (HLT_BTagMu_DiJet60_Mu7)",           "p"); 
	}
	else if (ptval_==40.) { 
	  //       qw->SetHeader("2011 effective lumi : 6.0  pb^{-1}"); // 40
	  qw->AddEntry(histo0_Data,        "data (HLT_BTagMu_DiJet40_Mu5)",           "p"); 
	}
	else if (ptval_==70.) { 
	  //       qw->SetHeader("2011 effective lumi : 34.3  pb^{-1}"); // 70
	  qw->AddEntry(histo0_Data,        "data (HLT_BTagMu_DiJet70_Mu5)",           "p"); 
	}
	else if (ptval_==110.) { 
	  //       qw->SetHeader("2011 effective lumi : 212 pb^{-1}"); // 110
	  qw->AddEntry(histo0_Data,        "data (HLT_BTagMu_DiJet110_Mu5)",           "p"); 
	}
      }

    }
  }
  */
  qw->AddEntry(histo0_Data,        "data",           "p");
  if (!DoNotStack) {
    qw->AddEntry(histo0_MC_b,        "b quark"           ,"f");
    qw->AddEntry(histo0_MC_gspl,     "b from gluon splitting"     ,"f");
    qw->AddEntry(histo0_MC_c,        "c quark"           ,"f");
    qw->AddEntry(histo0_MC_udsg,     "uds quark or gluon"     ,"f");
  }
  else {
    qw->AddEntry(histo0_MC,     "QCD "     ,"f");
  }
 
  qw->SetFillColor(0);
  qw->Draw();


  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42); //22

  latex->SetTextAlign(13);
  latex->DrawLatex(0.12, 0.96, PLOT_COMMON_TITLE);


  canvas->cd();


  // SECOND DATA/MC RATIO
  //canvas_2 = new TPad("canvas_2", "canvas_2",0,0.12,1.0,0.32);
  canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.32);
  canvas_2->Draw();
  canvas_2->cd();
  gPad->SetBottomMargin(0.375);
  gPad->SetGridy();
  //  gPad->SetGridx();
  histo0_ratio->SetMarkerStyle(20);
  histo0_ratio->SetMarkerSize(0.75);
  histo0_ratio->SetLineWidth(2);
  histo0_ratio->GetYaxis()->SetTitle("Data/MC");
  //  histo0_ratio->GetXaxis()->SetTitle(histo0_Data->GetName());  
  histo0_ratio->GetXaxis()->SetTitle(label);  

  // equivalent for main plot around line 205
  // histo0_ratio->GetYaxis()->SetTitleOffset( titleoffsety );
  //  histo0_ratio->GetYaxis()->SetTitleSize( titlesizey);
  //  histo0_ratio->GetYaxis()->SetLabelSize( labelsizey );
  histo0_ratio->GetYaxis()->SetNdivisions( 505 );
  histo0_ratio->GetXaxis()->SetLabelSize( labelsizex);
  histo0_ratio->GetXaxis()->SetTitleSize( titlesizex );

  histo0_ratio->SetMinimum(0.4);
  histo0_ratio->SetMaximum(1.6);
  histo0_ratio->Draw("E1X0");

  canvas->cd();


  /*
  // THIRD 2011/2010 RATIO, only for jet dataset
  canvas_3 = new TPad("canvas_3", "canvas_3",0,0.,1.0,0.20);
  if (mu_==false && selection!="npv") {
  canvas_3->Draw();
  canvas_3->cd();
  gPad->SetBottomMargin(0.375);
  gPad->SetGridy();
  gPad->SetGridx();
  histo0_ratdata->SetTitle();
  histo0_ratdata->SetMinimum(0.4);
  histo0_ratdata->SetMaximum(1.6);
  histo0_ratdata->GetYaxis()->SetTitle("2011/2010");
  histo0_ratdata->GetXaxis()->SetTitle(histo0_Data->GetName());  

  histo0_ratdata->GetYaxis()->SetTitleOffset( 0.4 );
  histo0_ratdata->GetYaxis()->SetTitleSize( 0.1 );
  histo0_ratdata->GetYaxis()->SetLabelSize(0.1);
  histo0_ratdata->GetYaxis()->SetNdivisions( 505 );
  histo0_ratdata->GetXaxis()->SetLabelSize(0.1);
  histo0_ratdata->GetXaxis()->SetTitleSize( 0.1 );

  histo0_ratdata->SetMarkerStyle(20);
  histo0_ratdata->SetMarkerColor(6);
  histo0_ratdata->SetLineWidth(2);
  histo0_ratdata->SetLineColor(6);
  histo0_ratdata->SetMarkerSize(0.75);
  histo0_ratdata->Draw("E1X0");
  }
  */

  canvas->cd();

  // SAVE PNG FILES
  TString name_can=string(dir_)+selection+"_Linear."+format;
  canvas->SaveAs(name_can.Data());

  if (logy==1) {
    canvas_1->SetLogy(logy);
    TString name_can2=string(dir_)+selection+"_Log."+format;
    canvas->SaveAs(name_can2.Data());
  }
}

void Plots1D::TagRate(TString selection, TString label, bool down, bool logy, TString labely){
  //###################################################
  
  histo0_Data = (TH1F*)data->Get(selection);
  TString selection2;
  if (selection=="discri_ssche") {
    selection2 = "discri_ssche0";
  } 
  else if (selection=="discri_sschp")  {
    selection2 = "discri_sschp0";
  } 
  else {
    // WE DON'T CARE THIS CASE!
    selection2 = "sv_nvertices";
  }
  histo0_Data2 = (TH1F*)data->Get(selection2);

  int nbinx=histo0_Data->GetNbinsX();
  float minx=histo0_Data->GetXaxis()->GetXmin();
  float maxx=histo0_Data->GetXaxis()->GetXmax();
  TH1F * TagRate_Data = new TH1F ("TagRate_Data",label,nbinx,minx,maxx);
  TH1F * TagRate_MC   = new TH1F ("TagRate_MC",label,nbinx,minx,maxx);
  TH1F * TagRate_MC_b = new TH1F ("TagRate_MC_b",label,nbinx,minx,maxx);
  TH1F * TagRate_MC_c = new TH1F ("TagRate_MC_c",label,nbinx,minx,maxx);
  TH1F * TagRate_MC_udsg = new TH1F ("TagRate_MC_udsg",label,nbinx,minx,maxx);
  TH1F * TagRate_MC_gspl = new TH1F ("TagRate_MC_gspl",label,nbinx,minx,maxx);

  TString bsel = selection+"_b";
  TString csel = selection+"_c";
  TString lsel = selection+"_udsg"; 
  TString gsel = selection+"_gspl";
  if (selection=="npv" || selection=="pthat" || selection=="npv_no_scaled" || selection=="npu") {
    // IF selection=="npv" or selection=="pthat", no _b info
    // PUT DUMMY NAMES TO AVOID CRASH...
    bsel = "sv_jetpt_b";
    csel = "sv_jetpt_b";
    lsel = "sv_jetpt_b";
    gsel = "sv_jetpt_b";
  }
  for (UInt_t i=0; i<Nfiles; i++){
    histo_MC[i]     =(TH1F*)mc[i]->Get(selection);
    histo_2MC[i]     =(TH1F*)mc[i]->Get(selection2);
    histo_MC_b[i]   =(TH1F*)mc[i]->Get(bsel);
    histo_MC_c[i]   =(TH1F*)mc[i]->Get(csel);
    histo_MC_udsg[i]=(TH1F*)mc[i]->Get(lsel);
    histo_MC_gspl[i]=(TH1F*)mc[i]->Get(gsel);
  
    //   if (selection=="npv") cout <<  " somme MC :      i = " << i  << endl;
 
    if (i==0) {
      histo0_MC=      (TH1F*)mc[i]->Get(selection);
      histo0_2MC=      (TH1F*)mc[i]->Get(selection2);
      histo0_MC_b=    (TH1F*)mc[i]->Get(bsel);
      histo0_MC_c=    (TH1F*)mc[i]->Get(csel);
      histo0_MC_udsg= (TH1F*)mc[i]->Get(lsel);
      histo0_MC_gspl= (TH1F*)mc[i]->Get(gsel);
    }
    else {
      histo0_MC->Add(histo_MC[i]);
      histo0_2MC->Add(histo_2MC[i]);
      histo0_MC_b->Add(histo_MC_b[i]);
      histo0_MC_c->Add(histo_MC_c[i]);
      histo0_MC_udsg->Add(histo_MC_udsg[i]);
      histo0_MC_gspl->Add(histo_MC_gspl[i]);
    }
  }


  // MAKE TAG RATE
  int nbin_max=histo0_Data->GetNbinsX();
  for (int ii=0; ii<nbin_max; ii++) {
    float totdata=histo0_Data->Integral(0,nbin_max+1);
    float totmc=histo0_MC->Integral(0,nbin_max+1);
    if (selection=="discri_ssche"||selection=="discri_sschp") {
      totdata=histo0_Data2->Integral(0,nbin_max+1);
      totmc=histo0_2MC->Integral(0,nbin_max+1);
    }
    float val = histo0_Data->Integral(ii+1,nbin_max+1) / totdata;
    float err = sqrt(totdata *val*(1-val))/ totdata;
      
    float valMC = histo0_MC->Integral(ii+1,nbin_max+1)/ totmc;
    float errMC = sqrt(totmc *valMC*(1-valMC))/ totmc;

    TagRate_Data->SetBinContent(ii+1, val);
    TagRate_Data->SetBinError(ii+1, err);
    TagRate_MC->SetBinContent(ii+1,   histo0_MC->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC->SetBinError(ii+1, errMC  );
    TagRate_MC_b->SetBinContent(ii+1, histo0_MC_b->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC_c->SetBinContent(ii+1, histo0_MC_c->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC_udsg->SetBinContent(ii+1, histo0_MC_udsg->Integral(ii+1,nbin_max+1) / totmc);
    TagRate_MC_gspl->SetBinContent(ii+1, histo0_MC_gspl->Integral(ii+1,nbin_max+1) / totmc);
  }

  // MAKE DATA/MC RATIO
  histo0_ratio = (TH1F*) TagRate_Data->Clone();
  histo0_ratio->SetName("histo0_ratio");
  histo0_ratio->SetTitle("");
  histo0_ratio->Divide(TagRate_MC);


  // SET COLORS
  TagRate_MC->SetLineColor(2);
  TagRate_MC_b->SetFillColor(2);
  TagRate_MC_c->SetFillColor(8);
  TagRate_MC_gspl->SetFillColor(7);
  TagRate_MC_udsg->SetFillColor(4);

  // DO STACK
  THStack* hs= new THStack();
  if (down==false) {
    hs->Add(TagRate_MC_udsg);
    hs->Add(TagRate_MC_c);
    hs->Add(TagRate_MC_gspl); 
    hs->Add(TagRate_MC_b);
  }
  else {
    hs->Add(TagRate_MC_b);
    hs->Add(TagRate_MC_gspl);  
    hs->Add(TagRate_MC_c);
    hs->Add(TagRate_MC_udsg);
  }
  

  // SET COSMETICS
  TagRate_Data->SetMarkerStyle(20);
  TagRate_Data->SetMarkerSize(0.75);
  TagRate_Data->GetXaxis()->SetTitle(label);  

  TString titlenam="Tag Rate for " + selection;

  gStyle->SetOptTitle(0);

  // CREATE CANVAS
  canvas = new TCanvas("c1", "c1",10,32,782,552);
  canvas->cd();

  // FIRST MC & DATA

  canvas_1 = new TPad("canvas_1", "canvas_1",0,0.25,1.0,0.98);
  canvas_1->Draw();
  canvas_1->cd();



  if (TagRate_Data->GetMaximum() > hs->GetMaximum() ) {
    hs->SetMaximum(TagRate_Data->GetMaximum() );
  }
  hs->Draw("hist");
  // can only be done after drawing...
  //  hs->GetYaxis()->SetTitleSize(titlesizey);
  //  hs->GetYaxis()->SetTitleOffset(titleoffsety);
  hs->GetHistogram()->GetYaxis()->SetTitle(labely);
  //  hs->GetHistogram()->GetYaxis()->SetTitleColor(kBlack);

  TagRate_Data->Draw("e same");

  // ADD LEGEND
  TLegend* qw = 0;
  if (mu_==false) {
    if (selection=="sv_eratio") {
      qw = new TLegend(0.5,0.73,0.8,1.);
    }
    else if (selection=="IP3d1" || selection=="IP3d1sorted" || selection=="trackIP3d" || selection=="trackIP3d_cutsel") {
      qw = new TLegend(0.12,0.63,0.42,0.9);
    }
    else {
      qw = new TLegend(0.6,0.73,0.95,1.);
    }
  } 
  else {
    if (selection=="sv_eratio") {
      qw = new TLegend(0.12,0.63,0.42,0.9);
    }
    else if (selection=="IP3d1" || selection=="IP3d1sorted" || selection=="IP3d1sig" || selection=="IP3d1sigsorted" || selection=="sv_ipsigcharm" ||
	     selection=="IP3d2" || selection=="IP3d2sorted" || selection=="IP3d2sig" || selection=="IP3d2sigsorted" || 
	     selection=="IP3d3" || selection=="IP3d3sorted" || selection=="IP3d3sig" || selection=="IP3d3sigsorted" 
	     ) {
      qw = new TLegend(0.12,0.63,0.42,0.9);
    }
    else {
      qw = new TLegend(0.6,0.73,0.95,1.);
    }
  }
  /*
  if (selection!="pthat" && selection!="npu") {
    if (year_==2010) {
      //     qw->SetHeader("CMS Preliminary, 33 pb^{-1}");
      qw->AddEntry(TagRate_Data,        "JET Run2010B ",           "p");
    }
    else if (year_==2011) {
      if (mu_==false) {
	if (ptval_==30) {
	  //       qw->SetHeader("2011 effective lumi : 0.0053  pb^{-1}"); // 30
	  qw->AddEntry(TagRate_Data,        "data (HLT_JET30)",           "p"); 
	}
	else if (ptval_==60.) { 
	  //       qw->SetHeader("2011 effective lumi : 0.11  pb^{-1}"); // 60
	  qw->AddEntry(TagRate_Data,        "data (HLT_JET60)",           "p"); 
	}
	else if (ptval_==80.) {
	  //       qw->SetHeader("2011 effective lumi : 0.52  pb^{-1}"); // 80
	  qw->AddEntry(TagRate_Data,        "data (HLT_JET80)",           "p"); 
	}
	else if (ptval_==110.) {
	  //       qw->SetHeader("2011 effective lumi : 2.5  pb^{-1}"); // 110
	  qw->AddEntry(TagRate_Data,        "data (HLT_JET110)",           "p"); 
	}

      }
      else  { 
	if (ptval_==20.) {
	  //       qw->SetHeader("2011 effective lumi : 1.9  pb^{-1}"); // 20
	  qw->AddEntry(TagRate_Data,        "data (HLT_BTagMu_DiJet20_Mu5)",           "p"); 
	}
	else if (ptval_==60.) { 
	  //       qw->SetHeader("2011 effective lumi : 24.3  pb^{-1}"); // 60
	  qw->AddEntry(TagRate_Data,        "data (HLT_BTagMu_DiJet60_Mu7)",           "p"); 
	}
	else if (ptval_==40.) { 
	  //       qw->SetHeader("2011 effective lumi : 6.0  pb^{-1}"); // 40
	  qw->AddEntry(TagRate_Data,        "data (HLT_BTagMu_DiJet40_Mu5)",           "p"); 
	}
	else if (ptval_==70.) { 
	  //       qw->SetHeader("2011 effective lumi : 34.3  pb^{-1}"); // 70
	  qw->AddEntry(TagRate_Data,        "data (HLT_BTagMu_DiJet70_Mu5)",           "p"); 
	}
	else if (ptval_==110.) { 
	  //       qw->SetHeader("2011 effective lumi : 212  pb^{-1}"); // 110
	  qw->AddEntry(TagRate_Data,        "data (HLT_BTagMu_DiJet110_Mu5)",           "p"); 
	}
      }

    }
  }
  */
  qw->AddEntry(TagRate_Data,        "data"                     ,"p");
  qw->AddEntry(TagRate_MC_b,        "b quark"                  ,"f");
  qw->AddEntry(TagRate_MC_gspl,     "b from gluon splitting"   ,"f");
  qw->AddEntry(TagRate_MC_c,        "c quark"                  ,"f");
  qw->AddEntry(TagRate_MC_udsg,     "uds quark or gluon"     ,"f");
 
  qw->SetFillColor(0);
  qw->Draw();

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42); //22

  latex->SetTextAlign(13);
  latex->DrawLatex(0.12, 0.96, PLOT_COMMON_TITLE);


  canvas->cd();



  // SECOND DATA/MC RATIO
  //canvas_2 = new TPad("canvas_2", "canvas_2",0,0.12,1.0,0.32);
  canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.32);
  canvas_2->Draw();
  canvas_2->cd();
  gPad->SetBottomMargin(0.375);
  gPad->SetGridy();
  gPad->SetGridx();
  histo0_ratio->SetMarkerStyle(20);
  histo0_ratio->SetMarkerSize(0.75);
  histo0_ratio->SetLineWidth(2);
  histo0_ratio->GetYaxis()->SetTitle("Data/MC");
  histo0_ratio->GetXaxis()->SetTitle(label);  

  histo0_ratio->GetYaxis()->SetTitleOffset( 0.4 );
  histo0_ratio->GetYaxis()->SetTitleSize( 0.1 );
  histo0_ratio->GetYaxis()->SetLabelSize(0.1);
  histo0_ratio->GetYaxis()->SetNdivisions( 505 );
  histo0_ratio->GetXaxis()->SetLabelSize(0.1);
  histo0_ratio->GetXaxis()->SetTitleSize( 0.15 );

  histo0_ratio->SetMinimum(0.4);
  histo0_ratio->SetMaximum(1.6);
  histo0_ratio->Draw("E1X0");

  canvas->cd();
  // SAVE PNG FILES
  TString name_can=string(dir_)+"tagRate_"+selection+"_Linear."+format;
  canvas->SaveAs(name_can.Data());

  if (logy==1) {
    canvas_1->SetLogy(logy);
    canvas->SetLogy(logy);
    TString name_can2=string(dir_)+"tagRate_"+selection+"_Log."+format;
    canvas->SaveAs(name_can2.Data());
  }
}
//#define ShowRatio//show ratio plots among scenarioes
void Plots1D::PlotData(TString selection, TString label, bool down, bool logy, bool bOverflow){
  TString npv1;
  TString npv2;
  TString npv3;
  npv1 = "npv1_"+selection;
  npv2 = "npv2_"+selection;
  npv3 = "npv3_"+selection;
  
  histo0_Data1 = (TH1F*)data->Get(npv1);
  histo0_Data2 = (TH1F*)data->Get(npv2);
  histo0_Data3 = (TH1F*)data->Get(npv3);
  // fix overflow bins:
 
  if(bOverflow){
    OverFlowBinFix( histo0_Data1);
    OverFlowBinFix( histo0_Data2);
    OverFlowBinFix( histo0_Data3);
  }


  //  Double_t titleoffsetx=0.13;
  //  Double_t titleoffsety=0.2;
  Double_t titlesizex=0.17;
  //  Double_t titlesizey=0.2;
  Double_t labelsizex=0.14;
  //  Double_t labelsizey=0.12;

  //   histo0_Data->GetYaxis()->SetLabelSize(labelsizey);
  //   histo0_Data->GetYaxis()->SetTitleSize(titlesizey);
  //   histo0_Data->GetYaxis()->SetTitleOffset(titleoffsety);
  //   histo0_MC->GetYaxis()->SetLabelSize(labelsizey);
  //   histo0_MC->GetYaxis()->SetTitleSize(titlesizey);
  //   histo0_MC->GetYaxis()->SetTitleOffset(titleoffsety);


  // SCALE MC TO DATA
  float scaleparam=1.;
  float scaleparam2=1.;
  /*
    if (selection=="pt_tracks" || selection=="pt_seltracks" || selection=="pt_cutseltracks" ) {
    int nbinx_data = histo0_Data2->GetNbinsX();
    scaleparam=histo0_Data2->Integral(2,nbinx_data)/histo0_Data1->Integral(2,nbinx_data);
    scaleparam2=histo0_Data2->Integral(2,nbinx_data)/histo0_Data3->Integral(2,nbinx_data);
    }
    else {  
    scaleparam=histo0_Data2->Integral()/histo0_Data1->Integral();
    scaleparam2=histo0_Data2->Integral()/histo0_Data3->Integral();
    }
  */
  scaleparam=histo0_Data2->Integral()/histo0_Data1->Integral();
  scaleparam2=histo0_Data2->Integral()/histo0_Data3->Integral();

  histo0_Data1->Scale(scaleparam);
  histo0_Data3->Scale(scaleparam2);


  // MAKE DATA/MC RATIO
  histo0_ratio = (TH1F*) histo0_Data2->Clone();
  histo0_ratio->SetName("histo0_ratio");
  histo0_ratio->SetTitle(label);
  histo0_ratio->Divide(histo0_Data1);

  histo0_ratio2 = (TH1F*) histo0_Data2->Clone();
  histo0_ratio2->SetName("histo0_ratio2");
  histo0_ratio2->SetTitle(label);
  histo0_ratio2->Divide(histo0_Data3);

  // SET COSMETICS
  histo0_Data1->SetMarkerStyle(21);
  histo0_Data1->SetLineColor(2);
  histo0_Data1->SetMarkerColor(2);
  histo0_Data1->SetMarkerSize(0.75);
  histo0_Data2->SetMarkerStyle(20);
  histo0_Data2->SetMarkerSize(0.75);
  histo0_Data3->SetMarkerStyle(22);
  histo0_Data3->SetLineColor(8);
  histo0_Data3->SetMarkerColor(8);
  histo0_Data3->SetMarkerSize(0.75);

  // CREATE CANVAS
  canvas = new TCanvas("c1", "c1",10,32,782,552);
  canvas->cd();
  
#ifdef ShowRatio
  // FIRST MC & DATA
  canvas_1 = new TPad("canvas_1", "canvas_1",0,0.25,1.0,0.98);
  canvas_1->Draw();
  canvas_1->cd();
#endif

  if (histo0_Data1->GetMaximum() > histo0_Data2->GetMaximum()) histo0_Data2->SetMaximum(histo0_Data1->GetMaximum() *1.1);
  if (histo0_Data3->GetMaximum() > histo0_Data2->GetMaximum()) histo0_Data2->SetMaximum(histo0_Data3->GetMaximum() *1.1);
  histo0_Data2->Draw("e");
  histo0_Data1->Draw("e same");
  histo0_Data3->Draw("e same");

  // ADD LEGEND
  TLegend* qw = 0;
  if (mu_==false) {
    if (selection=="eratio") {
      qw = new TLegend(0.5,0.60,0.85,1.);
    }
    else {
      qw = new TLegend(0.7,0.60,0.95,1.);
    }
  } 
  else {
    if (selection=="eratio") {
      qw = new TLegend(0.12,0.60,0.47,0.90);
    }
    else if (selection=="IP3d1" || selection=="IP3d1sorted") {
      qw = new TLegend(0.12,0.60,0.47,1.);
    }
    else {
      qw = new TLegend(0.65,0.60,0.95,1.);
    }
  }

  
  if (mu_==false) {
    if (ptval_==30) {
      //       qw->SetHeader("2011 JET30 L=0.0053 pb^{-1}"); // 30
    }
    else if (ptval_==60.) { 
      //       qw->SetHeader("2011 JET60 L=0.11 pb^{-1}"); // 60
    }
    else if (ptval_==80.) {
      //       qw->SetHeader("2011 JET80 L=0.52 pb^{-1}"); // 80
    }
    else if (ptval_==110.) {
      //       qw->SetHeader("2011 JET110 L=2.5 pb^{-1}"); // 110
    }
  }
  else  { 
    if (ptval_==20.) {
      //       qw->SetHeader("2011 BTagMu_DiJet20_Mu5 L=1.9 pb^{-1}"); // 20
    }
    else if (ptval_==60.) { 
      //       qw->SetHeader("2011 BTagMu_DiJet60_Mu7 L=24.3 pb^{-1}"); // 60
    }
    else if (ptval_==40.) { 
      //       qw->SetHeader("2011 BTagMu_DiJet40_Mu5 L=6.0 pb^{-1}"); // 60
    }
    else if (ptval_==70.) { 
      //       qw->SetHeader("2011 BTagMu_DiJet70_Mu5 L=34.3 pb^{-1}"); // 60
    }
    else if (ptval_==110.) { 
      //      qw->SetHeader("2011 BTagMu_DiJet110_Mu5 L=212 pb^{-1}"); // 60
    }
  }
  qw->AddEntry(histo0_Data1,        "data (#PV: 1-6)",           "p"); 
  qw->AddEntry(histo0_Data2,        "data (#PV: 7-10)",           "p"); 
  qw->AddEntry(histo0_Data3,        "data (#PV: >=11)",           "p"); 
  qw->SetFillColor(0);
  qw->Draw();
   
  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42); //22
  latex->SetTextAlign(13);
  latex->DrawLatex(0.1, 0.96, PLOT_COMMON_TITLE);

  canvas->cd();
#ifdef ShowRatio
  // SECOND DATA/DATA RATIO
  canvas_2 = new TPad("canvas_2", "canvas_2",0,0.,1.0,0.32);
  canvas_2->Draw();
  canvas_2->cd();
  gPad->SetBottomMargin(0.375);
  gPad->SetGridy();
  gPad->SetGridx();
  histo0_ratio->SetMarkerStyle(20);
  histo0_ratio->SetMarkerSize(0.75);
  histo0_ratio->SetLineWidth(2);
  histo0_ratio->GetYaxis()->SetTitle("#PV 7-10/#PV 1-6");
  histo0_ratio->GetXaxis()->SetTitle(label);  
  histo0_ratio->GetYaxis()->SetTitleOffset( 0.4 );
  histo0_ratio->GetYaxis()->SetTitleSize( 0.1 );
  histo0_ratio->GetYaxis()->SetLabelSize(0.1);
  histo0_ratio->GetYaxis()->SetNdivisions( 505 );
  //  histo0_ratio->GetXaxis()->SetLabelSize(0.1);
  //  histo0_ratio->GetXaxis()->SetTitleSize( 0.1 );
  histo0_ratio->GetXaxis()->SetLabelSize( labelsizex);
  histo0_ratio->GetXaxis()->SetTitleSize( titlesizex );
  histo0_ratio->SetMinimum(0.4);
  histo0_ratio->SetMaximum(1.6);
  histo0_ratio->Draw("E1X0");
  canvas->cd();
#endif
  // SAVE FILES
  TString name_can=string(dir_)+"npv_"+selection+"_Linear."+format;
  canvas->SaveAs(name_can.Data());

  if (logy==1) {
#ifdef ShowRatio
    canvas_1->SetLogy(logy);
#else
    canvas->SetLogy(logy);
#endif
    TString name_can2=string(dir_)+"npv_"+selection+"_Log."+format;
    canvas->SaveAs(name_can2.Data());
  }
}

void Plots2D::PlotProfile2D(TString selection, TString label, TString labely, bool logy, float ymin, float ymax, Bool_t isdata, string fitequ){
  if (isdata)
    histo0_MC=(TH2F*) data->Get(selection);
  else {
    for (UInt_t i=0; i<Nfiles; i++) 
      if (i==0) histo0_MC=(TH2F*)mc[i]->Get(selection);
      else histo0_MC->Add(histo_MC[i]);
  }
  TProfile * pro_mc = histo0_MC->ProfileX(selection+"_total");
  pro_mc->SetLineColor(1);
 Double_t titleoffsetx=0.8;
  Double_t titleoffsety=0.8;
  Double_t titlesizex=0.05;
  Double_t titlesizey=0.05;
  Double_t labelsizex=0.035;
  Double_t labelsizey=0.035;
  pro_mc->SetMaximum(ymax);
  pro_mc->SetMinimum(ymin);
  pro_mc->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc->GetXaxis()->SetTitle(label);  
  pro_mc->GetYaxis()->SetTitle(labely);  
  canvas = new TCanvas("c1", "c1",10,32,782,552);
  canvas->cd();
  if (isdata) pro_mc->Draw("E");
  else pro_mc->Draw("hist");
  if ( fitequ.size()>0 )
    pro_mc->Fit( fitequ.c_str() );
    // SAVE FILES
  canvas->SetGrid();
  TString name_can=string(dir_)+selection+"_Linear."+format;
  canvas->SaveAs(name_can.Data());
  if (logy==1) {
    canvas->SetLogy(logy);
    name_can=string(dir_)+selection+"_Log."+format;
    canvas->SaveAs(name_can.Data());
  }
}

void Plots2D::PlotStack2D(TString selection, TString label, TString labely, bool logy, float ymin, float ymax){
  histo0_Data = (TH2F*) data->Get(selection);
  TProfile * pro_data = histo0_Data->ProfileX(selection+"data",1,-1,"");
  //according to http://root.cern.ch/root/html/TProfile.html#TProfile:BuildOptions, the eerror calculation of root TProfile is buggy ( 0 error for spread 0 bin ), so we correct it manually
  //corrections reference: http://web-docs.gsi.de/~halo/docs/hydra/classDocumentation/doxy_dev/root52800b/html/classTProfileHelper.html#6c0a7cb6d5cbb2595cb35ac8f6f1250d
  for (UInt_t ibin=0;ibin<=pro_data->GetNbinsX();ibin++) {
    if ( pro_data->GetBinError(ibin)<1E-5 ) pro_data->SetBinError(ibin,sqrt(pro_data->GetBinContent(ibin)));
    //cerr<<pro_data->GetBinCenter(ibin)<<","<<pro_data->GetBinContent(ibin)<<"+/-"<<pro_data->GetBinError(ibin)<<"("<<pro_data->GetBinEntries(ibin)<<" ) ;  ";
  }
  TString bsel = selection+"_b";
  TString csel = selection+"_c";
  TString lsel = selection+"_udsg"; 
  TString gsel = selection+"_gspl";
  for (UInt_t i=0; i<Nfiles; i++){
    histo_MC[i]     =(TH2F*)mc[i]->Get(selection);
    histo_MC_b[i]   =(TH2F*)mc[i]->Get(bsel);
    histo_MC_c[i]   =(TH2F*)mc[i]->Get(csel);
    histo_MC_udsg[i]=(TH2F*)mc[i]->Get(lsel);
    histo_MC_gspl[i]=(TH2F*)mc[i]->Get(gsel);
  
 
    if (i==0) {
      histo0_MC=      (TH2F*)mc[i]->Get(selection);
      histo0_MC_b=    (TH2F*)mc[i]->Get(bsel);
      histo0_MC_c=    (TH2F*)mc[i]->Get(csel);
      histo0_MC_udsg= (TH2F*)mc[i]->Get(lsel);
      histo0_MC_gspl= (TH2F*)mc[i]->Get(gsel);
    }
    else {
      histo0_MC->Add(histo_MC[i]);
      histo0_MC_b->Add(histo_MC_b[i]);
      histo0_MC_c->Add(histo_MC_c[i]);
      histo0_MC_udsg->Add(histo_MC_udsg[i]);
      histo0_MC_gspl->Add(histo_MC_gspl[i]);
    }
  }

   /*  // SCALE MC TO DATA
  float scaleparam=histo0_Data->Integral()/histo0_MC->Integral();
  histo0_MC_b->Scale(scaleparam);
  histo0_MC_c->Scale(scaleparam);
  histo0_MC_udsg->Scale(scaleparam);
  histo0_MC_gspl->Scale(scaleparam);
  histo0_MC->Scale(scaleparam);
  */
    TProfile * pro_mc = histo0_MC->ProfileX(selection+"_qcdtotal");
    TProfile * pro_mc_b = histo0_MC_b->ProfileX(bsel);
    TProfile * pro_mc_c = histo0_MC_c->ProfileX(csel);
    TProfile * pro_mc_udsg = histo0_MC_udsg->ProfileX(lsel);
    TProfile * pro_mc_gspl = histo0_MC_gspl->ProfileX(gsel);
 
  // SET COLORS
  pro_mc->SetLineColor(1);
  pro_mc_b->SetLineColor(2);
  pro_mc_c->SetLineColor(8);
  pro_mc_udsg->SetLineColor(4);
  pro_mc_gspl->SetLineColor(7);

  // SET COSMETICS
  pro_data->SetMarkerStyle(20);
  pro_data->SetMarkerSize(0.75);
  pro_mc_gspl->GetXaxis()->SetTitle(label);  
  pro_mc_gspl->GetYaxis()->SetTitle(labely);  

  pro_mc_gspl->GetYaxis()->SetRangeUser(ymin, ymax);
  pro_mc->GetYaxis()->SetRangeUser(ymin, ymax);
  pro_mc_b->GetYaxis()->SetRangeUser(ymin, ymax);
  pro_data->GetYaxis()->SetRangeUser(ymin, ymax);
  pro_mc_c->GetYaxis()->SetRangeUser(ymin, ymax);
  pro_mc_udsg->GetYaxis()->SetRangeUser(ymin, ymax);

  gStyle->SetOptTitle(0);


  Double_t titleoffsetx=0.8;
  Double_t titleoffsety=0.8;
  Double_t titlesizex=0.05;
  Double_t titlesizey=0.05;
  Double_t labelsizex=0.035;
  Double_t labelsizey=0.035;

  pro_data->GetYaxis()->SetLabelSize(labelsizey);
  pro_data->GetYaxis()->SetTitleSize(titlesizey);
  pro_data->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc_b->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_b->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_b->GetYaxis()->SetTitleOffset(titleoffsety);
  pro_mc_c->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_c->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_c->GetYaxis()->SetTitleOffset(titleoffsety);

  pro_mc_gspl->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_gspl->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_gspl->GetYaxis()->SetTitleOffset(titleoffsety);

  pro_mc_udsg->GetYaxis()->SetLabelSize(labelsizey);
  pro_mc_udsg->GetYaxis()->SetTitleSize(titlesizey);
  pro_mc_udsg->GetYaxis()->SetTitleOffset(titleoffsety);



  pro_data->GetXaxis()->SetLabelSize(labelsizex);
  pro_data->GetXaxis()->SetTitleSize(titlesizex);
  pro_data->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc_b->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_b->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_b->GetXaxis()->SetTitleOffset(titleoffsetx);
  pro_mc_c->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_c->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_c->GetXaxis()->SetTitleOffset(titleoffsetx);

  pro_mc_gspl->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_gspl->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_gspl->GetXaxis()->SetTitleOffset(titleoffsetx);

  pro_mc_udsg->GetXaxis()->SetLabelSize(labelsizex);
  pro_mc_udsg->GetXaxis()->SetTitleSize(titlesizex);
  pro_mc_udsg->GetXaxis()->SetTitleOffset(titleoffsetx);


  // CREATE CANVAS
  canvas = new TCanvas("c1", "c1",10,32,782,552);
  canvas->cd();

  pro_mc_gspl->Draw("hist");
  pro_mc_b->Draw("hist,same");
  pro_mc_c->Draw("hist,same");
  pro_mc_udsg->Draw("hist,same");
  pro_mc->Draw("hist,same");
  pro_data->Draw("e,same");

  // ADD LEGEND
  TLegend* qw = 0;
  if (mu_==false) {
    qw = new TLegend(0.35,0.11,0.65,0.33);
  } 
  else {
    qw = new TLegend(0.35,0.11,0.65,0.33);
  }
  /*
  if (mu_==false) {
    if (ptval_==30) {
      //       qw->SetHeader("2011 effective lumi : 0.0053  pb^{-1}"); // 30
      qw->AddEntry(pro_data,        "data (HLT_JET30)",           "p"); 
    }
    else if (ptval_==60.) { 
      //       qw->SetHeader("2011 effective lumi : 0.11  pb^{-1}"); // 60
      qw->AddEntry(pro_data,        "data (HLT_JET60)",           "p"); 
    }
    else if (ptval_==80.) {
      //      qw->SetHeader("2011 effective lumi : 0.52  pb^{-1}"); // 80
      qw->AddEntry(pro_data,        "data (HLT_JET80)",           "p"); 
    }
    else if (ptval_==110.) {
      //      qw->SetHeader("2011 effective lumi : 2.5  pb^{-1}"); // 110
      qw->AddEntry(pro_data,        "data (HLT_JET110)",           "p"); 
    }
  }
  else  { 
    if (ptval_==20.) {
      //       qw->SetHeader("2011 effective lumi : 1.9  pb^{-1}"); // 20
      qw->AddEntry(pro_data,        "data (HLT_BTagMu)",           "p"); 
    }
    else if (ptval_==60.) { 
      //       qw->SetHeader("2011 effective lumi : 24.3  pb^{-1}"); // 60
      qw->AddEntry(pro_data,        "data (HLT_BTagMu_DiJet60_Mu7)",           "p"); 
    }
    else if (ptval_==40.) { 
      //      qw->SetHeader("2011 effective lumi : 6.0  pb^{-1}"); // 40
      qw->AddEntry(pro_data,        "data (HLT_BTagMu_DiJet40_Mu5)",           "p"); 
    }
    else if (ptval_==70.) { 
      //      qw->SetHeader("2011 effective lumi : 34.3  pb^{-1}"); // 70
      qw->AddEntry(pro_data,        "data (HLT_BTagMu_DiJet70_Mu5)",           "p"); 
    }
    else if (ptval_==110.) { 
      //      qw->SetHeader("2011 effective lumi : 212  pb^{-1}"); // 110
      qw->AddEntry(pro_data,        "data (HLT_BTagMu_DiJet110_Mu5)",           "p"); 
    }

  }
 */
  qw->AddEntry(pro_data,        "data"                   ,"p");
  qw->AddEntry(pro_mc,          "total "                 ,"l");
  qw->AddEntry(pro_mc_b,        "b quark"                ,"l");
  qw->AddEntry(pro_mc_gspl,     "b from gluon splitting" ,"l");
  qw->AddEntry(pro_mc_c,        "c quark"                ,"l");
  qw->AddEntry(pro_mc_udsg,     "uds quark or gluon"     ,"l");
 
  qw->SetFillColor(0);
  qw->Draw();

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.055);
  latex->SetTextFont(42); //22

  latex->SetTextAlign(13);
  latex->DrawLatex(0.12, 0.96, PLOT_COMMON_TITLE);

  canvas->cd();

  // SAVE FILES
  TString name_can=string(dir_)+selection+"_Linear."+format;
  canvas->SaveAs(name_can.Data());

  if (logy==1) {
    canvas->SetLogy(logy);
    name_can=string(dir_)+selection+"_Log."+format;
    canvas->SaveAs(name_can.Data());
  }
}

void OverFlowBinFix(TH1F* histo){

  Float_t val, errval;

  val=histo->GetBinContent(1)+histo->GetBinContent(0);
  errval=0;
  if(histo->GetBinContent(1)!=0.)
    errval+=pow(histo->GetBinError(1)/histo->GetBinContent(1),2);
  if(histo->GetBinContent(0)!=0.)
    errval+=pow(histo->GetBinError(0)/histo->GetBinContent(0),2);
  errval=sqrt(errval)*val;
  histo->SetBinContent(1,val);
  histo->SetBinError(1,errval);
  histo->SetBinContent(0,0);
  histo->SetBinError(0,0);

  Int_t highbin=histo->GetNbinsX();
  
  val=histo->GetBinContent(highbin)+histo->GetBinContent(highbin+1);
  errval=0;
  if(histo->GetBinContent(highbin)!=0.)
    errval+=pow(histo->GetBinError(highbin)/histo->GetBinContent(highbin),2);
  if(histo->GetBinContent(highbin+1)!=0.)
    errval+=pow(histo->GetBinError(highbin+1)/histo->GetBinContent(highbin+1),2);
  errval=sqrt(errval)*val;
  histo->SetBinContent(highbin,val);
  histo->SetBinError(highbin,errval);
  histo->SetBinContent(highbin+1,0);
  histo->SetBinError(highbin+1,0);
}

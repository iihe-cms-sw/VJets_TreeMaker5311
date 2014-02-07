#include <vector>
#include <string>
#include "TStyle.h"

using namespace std;

int colorList[] = {1,2,3,4,6,7,8,9,10,11}; 
//int markerStyleList[] = {21,21,23,23,22,22,23,23,21,21};  

void setTDRStyle() {
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();

  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadBorderMode(0);

  //tdrStyle->SetFrameColor(0);
  tdrStyle->SetPadColor(0);
  tdrStyle->SetCanvasColor(0);
  tdrStyle->SetStatColor(0);
  tdrStyle->SetFillColor(0);

  tdrStyle->SetPaperSize(20,26);
  //tdrStyle->SetPadTopMargin(0.08);
  //tdrStyle->SetPadBottomMargin(0.14);
  tdrStyle->SetPadRightMargin(0.04);
  tdrStyle->SetPadLeftMargin(0.16);
  //tdrStyle->SetCanvasDefH(800);
  //tdrStyle->SetCanvasDefW(800);
  //tdrStyle->SetPadGridX(1);
  //tdrStyle->SetPadGridY(1);
  tdrStyle->SetPadTickX(1);
  tdrStyle->SetPadTickY(1);

  tdrStyle->SetTextFont(42); //132
  tdrStyle->SetTextSize(0.09);
  tdrStyle->SetLabelFont(42,"xyz");
  tdrStyle->SetTitleFont(42,"xyz");
  tdrStyle->SetLabelSize(0.045,"xyz"); //0.035
  tdrStyle->SetTitleSize(0.045,"xyz");
  tdrStyle->SetTitleOffset(1.5,"y");
    
  tdrStyle->SetTitleX(0.16);
  tdrStyle->SetTitleY(0.93);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(0);
  tdrStyle->SetTitleBorderSize(1);
  tdrStyle->SetTitleFontSize(0.04);
  //tdrStyle->SetPadTopMargin(0.05);
  //tdrStyle->SetPadBottomMargin(0.13);
  //tdrStyle->SetPadLeftMargin(0.14);
  //tdrStyle->SetPadRightMargin(0.02);

  // use bold lines and markers
  tdrStyle->SetMarkerStyle(8);
  tdrStyle->SetHistLineWidth(3);
  tdrStyle->SetLineWidth(1);
  
  tdrStyle->SetOptTitle(kFALSE);
  tdrStyle->SetOptStat(0);

  tdrStyle->cd();
}

TObject *getHistogram(TFile *f, string algo,string histoName, string range = "GLOBAL")
{
  string prefix = "DQMData/Run 1/Btag/Run summary/JetTag";
  string d = prefix+"_"+algo+"_"+range;
  cout <<" DIR "<<d<<endl;
  TDirectory *dir  =(TDirectory *) f->Get(d.c_str());
  return dir->Get((histoName+"_"+algo+"_"+range).c_str());
}

TObject *getCutHistogram(TFile *f, string algo,string histoName, string range = "GLOBAL", string type ="B")
{
  string prefix = "DQMData/Run 1/Btag/Run summary/JetTag";
  string d = prefix+"_"+algo+"_"+range;
  cout <<" DIR "<<d<<endl;
  TDirectory *dir  =(TDirectory *) f->Get(d.c_str());
  cout << "Histo name: " << histoName+"_"+algo+"_"+range+type << endl;
  return dir->Get((histoName+"_"+algo+"_"+range+type).c_str());
}

void setStyle(int i, TH1F *obj)
{
  obj->SetTitle("");
  obj->SetMarkerColor(colorList[i]);
  obj->SetMarkerStyle(21);
  //  obj->SetMarkerStyle(markerStyleList[i]);
}


void drawAll()
{
  
  //style setting
  setTDRStyle();

  TFile *_file1 = TFile::Open("BTAG_DQM.root");
  
  vector<TFile *> files;
  vector<string> algos;
  vector<string> lname;

  algos.push_back("standardSimpleSecondaryVertexHighEffPFBJetTags");
  algos.push_back("standardSimpleSecondaryVertexHighPurPFBJetTags");
  algos.push_back("standardTrackCountingHighEffPFBJetTags");
  algos.push_back("standardTrackCountingHighPurPFBJetTags");
  algos.push_back("standardJetProbabilityPFBJetTags");
  algos.push_back("standardJetBProbabilityPFBJetTags");

  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);

  lname.push_back("SSVHE");
  lname.push_back("SSVHP");
  lname.push_back("TCHE");
  lname.push_back("TCHP");
  lname.push_back("JP");
  lname.push_back("JBP");

  //  TLegend *leg = new TLegend(0.12,0.6,0.52,0.9,NULL,"brNDC");
  //  TLegend *leg = new TLegend(0.52,0.15,0.72,0.45,NULL,"brNDC");
  TLegend *legend = new TLegend(0.74,0.15,0.92,0.45,NULL,"brNDC");

  legend->SetBorderSize(0);
  legend->SetFillColor(kWhite);
  legend->SetMargin(0.12);
  legend->SetTextSize(0.035);  

  //include the official CMS label
  TPaveText *pt = new TPaveText(0.1,0.945,0.95,0.995,"brNDC");   
  pt->SetBorderSize(0);   
  pt->SetFillStyle(0);  
  pt->SetTextAlign(13);   
  pt->SetTextFont(42);   
  pt->SetTextSize(0.04);   
  TText *text = pt->AddText("PYTHIA6: CMS Preliminary 2010           #sqrt{s} = 7 TeV");   

  TCanvas * c1 = new TCanvas("c1","non-b vs b eff",700,700);
  c1->SetGridy();  
  c1->SetGridx();  
  c1->SetLogy();  
  c1->cd();

  for(int i = 0 ; i < algos.size() ; i++)
   {
     cout << algos[i] << endl;

     //     FlavEffVsBEff_DUSG_discr_standardSimpleSecondaryVertexHighPurPFBJetTags_GLOBAL
     TH1F * nonb_vs_b = (TH1F *)getHistogram(files[i],algos[i],"FlavEffVsBEff_DUSG_discr","GLOBAL")->Clone("nonb_vs_b");
     cout << nonb_vs_b << endl;
     nonb_vs_b->SetMaximum(1.5);
     nonb_vs_b->SetMinimum(10E-6);
     setStyle(i,nonb_vs_b);
     if(i==0) nonb_vs_b->Draw(); else nonb_vs_b->Draw("same"); 
     legend->AddEntry(nonb_vs_b,lname[i].c_str(),"p");     

   }
  legend->Draw("same");
  pt->Draw();

  c1->Print("nonb_vs_beff.png");
  c1->Print("nonb_vs_beff.pdf");

  //Eff vs Discriminator cut

  //  TLegend *leg = new TLegend(0.6,0.25,0.8,0.45,NULL,"brNDC");
  TLegend *leg = new TLegend(0.8,0.7,0.95,0.85,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetFillColor(kWhite);
  leg->SetMargin(0.12);
  leg->SetTextSize(0.035);  

  c1->Clear();
  c1->cd();
  c1->SetLogy(0);

  for(int i = 0 ; i < 2 ; i++)
   {
     cout << algos[i] << endl;

     TH1F *effb = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","B");
     effb->GetYaxis()->SetTitle("b-efficiency");
     setStyle(i,effb);
     if(i==0) effb->Draw(); else effb->Draw("same"); 
     leg->AddEntry(effb,lname[i].c_str(),"p");         
   }
  leg->Draw("same");
  pt->Draw();

  c1->Print("SSVbeff.png");
  c1->Print("SSVbeff.pdf");

  c1->Clear();
  leg->Clear();
  c1->cd();
  c1->SetLogy(1);

  for(int i = 0 ; i < 2 ; i++)
   {
     cout << algos[i] << endl;
     TH1F *efflight = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","DUSG");
     efflight->GetYaxis()->SetTitle("non b-efficiency");
     setStyle(i,efflight); 
     if(i==0) efflight->Draw(); else efflight->Draw("same"); 
     leg->AddEntry(efflight,lname[i].c_str(),"p");         
   }
  leg->Draw("same");
  pt->Draw();

  c1->Print("SSVlighteff.png");
  c1->Print("SSVlighteff.pdf");

  //Track Counting
  c1->Clear();
  leg->Clear();
  c1->cd();
  c1->SetLogy(0);

  for(int i = 2 ; i < 4 ; i++)
   {
     cout << algos[i] << endl;

     TH1F *effb = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","B");
     effb->GetYaxis()->SetTitle("b-efficiency");
     setStyle(i,effb);
     effb->GetXaxis()->SetRangeUser(1.1,14.);
     if(i==2) effb->Draw(); else effb->Draw("same"); 
     leg->AddEntry(effb,lname[i].c_str(),"p");         
   }
  leg->Draw("same");
  pt->Draw();

  c1->Print("TCbeff.png");
  c1->Print("TCbeff.pdf");

  c1->Clear();
  leg->Clear();
  c1->cd();
  c1->SetLogy(1);

  for(int i = 2 ; i < 4 ; i++)
   {
     cout << algos[i] << endl;

     TH1F *efflight = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","DUSG");
     efflight->GetYaxis()->SetTitle("non b-efficiency");
     setStyle(i,efflight); 
     efflight->GetXaxis()->SetRangeUser(1.1,14.);
     if(i==2) efflight->Draw(); else efflight->Draw("same"); 
     leg->AddEntry(efflight,lname[i].c_str(),"p");         
   }
  leg->Draw("same");
  pt->Draw();

  c1->Print("TClighteff.png");
  c1->Print("TClighteff.pdf");

  //Jet Probability
  c1->Clear();
  leg->Clear();
  c1->cd();
  c1->SetLogy(0);

  for(int i = 4 ; i < 6 ; i++)
   {
     cout << algos[i] << endl;

     TH1F *effb = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","B");
     effb->GetYaxis()->SetTitle("b-efficiency");
     setStyle(i,effb);
     effb->GetXaxis()->SetRangeUser(0.1,2.2);
     if(i==4) effb->Draw(); else effb->Draw("same"); 
     leg->AddEntry(effb,lname[i].c_str(),"p");         
   }
  leg->Draw("same");
  pt->Draw();

  c1->Print("JPbeff.png");
  c1->Print("JPbeff.pdf");

  c1->Clear();
  leg->Clear();
  c1->cd();
  c1->SetLogy(1);

  for(int i = 4 ; i < 6 ; i++)
   {
     cout << algos[i] << endl;

     TH1F *efflight = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","DUSG");
     efflight->GetYaxis()->SetTitle("non b-efficiency");
     setStyle(i,efflight); 
     efflight->GetXaxis()->SetRangeUser(0.1,2.2);
     if(i==4) efflight->Draw(); else efflight->Draw("same"); 
     leg->AddEntry(efflight,lname[i].c_str(),"p");         
   }
  leg->Draw("same");
  pt->Draw();

  c1->Print("JPlighteff.png");
  c1->Print("JPlighteff.pdf");

}

// KEY: TH1F	effVsDiscrCut_discr_standardSimpleSecondaryVertexHighPurPFBJetTags_GLOBALB;1	Eff. vs Disc. Cut: Discriminator b-jets
// KEY: TH1F	effVsDiscrCut_discr_standardSimpleSecondaryVertexHighPurPFBJetTags_GLOBALDUSG;1	Eff. vs Disc. Cut: Discriminator dusg-jets






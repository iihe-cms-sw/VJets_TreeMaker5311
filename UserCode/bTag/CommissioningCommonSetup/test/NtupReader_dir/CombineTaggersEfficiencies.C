#include <vector>
#include <string>
#include <TFile.h>
#include <TLegend.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <iostream>
#include "setTDRStyle.h"
#define PUMIN 0
#define PUMAX 54
using namespace std;

const EColor ColorList[]={EColor(kOrange+7),kRed,kCyan,kGreen,kMagenta,kBlue,kBlack,kGray};
int markerStyleList[] = {21,21,23,23,22,22,23,23,24,24,24,24,25,25,25,25,26,26,26,26};  

template <typename T> 
void setStyle(int i, T *obj,int linesty)
{
obj->SetMarkerColor(ColorList[i]);
obj->SetLineColor(ColorList[i]);
obj->SetLineStyle(linesty);
obj->SetMarkerStyle(markerStyleList[i]);
obj->SetMarkerSize(.3);
}

//option=0 udsgvsb
//option=1 cvsb
void combine(vector<string>& algos,Byte_t option=1)
{
  setTDRStyle();
  TLegend * leg=NULL;
  if ( option==0 ) leg = new TLegend(0.2,0.65,0.4,0.9);
  if ( option==1 ) leg = new TLegend(0.6,0.25,0.8,0.5);
  TCanvas * c1 = new TCanvas();
  c1->SetLogy();  
  c1->SetGridy();  
  c1->SetGridx();
  if ( option==0 ) c1->DrawFrame(0,1E-4,1.0,1.0,"alltaggers;b jet efficiency;udsg jet efficiency");
  if ( option==1 ) c1->DrawFrame(0,1E-4,1.0,1.0,"alltaggers;b jet efficiency;c jet efficiency");
  TLatex latex;
  latex.SetTextSize(0.03);
  latex.SetTextAlign(31);
  latex.DrawLatex(1.,1.,"CMS 2011");
  for ( vector<string>::const_iterator algo=algos.begin();algo!=algos.end();algo++) {
    char filename[200],objname[200];
    sprintf(filename,"result_%s.root",algo->c_str());
    TFile *_file=TFile::Open(filename);
    string obj;
    if ( algo->compare("CSVPF")==0 ) obj.assign("CSV");
    else obj.assign(*algo);
    if ( option==0 ) sprintf(objname,"%s_efficiencies_UDSGvsB_PU%dto%d",obj.c_str(),PUMIN,PUMAX);
    if ( option==1 ) sprintf(objname,"%s_efficiencies_CvsB_PU%dto%d",obj.c_str(),PUMIN,PUMAX);
    printf("%s->%s\n",filename,objname);
    TGraphAsymmErrors *eff=(TGraphAsymmErrors *) _file->Get(objname);
    if (eff!=NULL) {
      setStyle(algo-algos.begin(),eff,1);
      eff->Draw("lpsame");
      leg->AddEntry(eff,obj.c_str(),"lp");
    }
    else printf("%s is not found in %s",objname,filename);
  }
  latex.Draw();
  leg->Draw("same");
  if ( option==0 ) c1->SaveAs("Combined_udsgvsb_Efficiencies.pdf");
  if ( option==1 ) c1->SaveAs("Combined_cvsb_Efficiencies.pdf");
}

int main( int argc, char* argv[] ) {
  vector<string> algos;
  for (int n = 1; n < argc; n++)
    algos.push_back(string(argv[n]));
  combine(algos,0);
  combine(algos,1);
}

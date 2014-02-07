///// Alexander.Schmidt@cern.ch
///// 
///// run this macro with: 
///// root -l macroComparison.C+'("filenameA", "filenameB", "filenameC")'
///// where the filenames are the three histogram files to be compared


#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include <iostream>

void macroComparison(string sfileA,string sfileB,string sfileC, string fname){

  TFile * fileA = new TFile(TString(sfileA), "READ");
  TFile * fileB = new TFile(TString(sfileB), "READ");
  TFile * fileC = new TFile(TString(sfileC), "READ");

  // get canvas name
  TList *listA = fileA->GetListOfKeys();  
  TString canvasnameA = listA->At(0)->GetName();
 
  TList *listB = fileB->GetListOfKeys();  
  TString canvasnameB = listB->At(0)->GetName();

  TList *listC = fileC->GetListOfKeys();  
  TString canvasnameC = listC->At(0)->GetName();

  // make histogram name out of canvas name
  TString histonameA = canvasnameA;
  Ssiz_t objIndexA = histonameA.Index("canvas");
  histonameA.Resize(objIndexA);
  histonameA+= "_data_hist";

  TString histonameB = canvasnameB;
  Ssiz_t objIndexB = histonameB.Index("canvas");
  histonameB.Resize(objIndexB);
  histonameB+= "_data_hist";

  TString histonameC = canvasnameC;
  Ssiz_t objIndexC = histonameC.Index("canvas");
  histonameC.Resize(objIndexC);
  histonameC+= "_data_hist";

  // get canvas from files
  // get histograms from canvases
  
  TCanvas *cvA = (TCanvas*) fileA->Get(canvasnameA);
  TH1D * histoA = (TH1D*) cvA->FindObject(histonameA)->Clone();
  // set name to different value
  cvA->SetName("dummy");
  
  TCanvas *cvB = (TCanvas*) fileB->Get(canvasnameB);
  TH1D * histoB = (TH1D*) cvB->FindObject(histonameB)->Clone();
  cvB->SetName("dummy2");

  TCanvas *cvC = (TCanvas*) fileC->Get(canvasnameC);
  TH1D * histoC = (TH1D*) cvC->FindObject(histonameC)->Clone();


  // ratio plots
  TH1D * histoAoverBRatio =  (TH1D*) histoA->Clone();
  TH1D * histoAoverCRatio =  (TH1D*) histoA->Clone();
  TH1D * histoBoverCRatio =  (TH1D*) histoB->Clone();

  histoAoverBRatio->Divide(histoB);
  histoAoverCRatio->Divide(histoC);
  histoBoverCRatio->Divide(histoC);

  // make new Canvas for plotting
  TCanvas *cv1 = new TCanvas(histonameA, histonameA, 1024,1024);
  cv1->cd();
  cv1->Clear();
  cv1->Divide(1,2,0.01,0.0);
  

  cv1->cd(1);
  cv1->SetFillColor(0); 
  gStyle->SetOptTitle(0);
  gPad->SetFillColor(0);
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  cv1->cd(2);
  cv1->SetFillColor(0);
  gStyle->SetOptTitle(0);
  gPad->SetFillColor(0);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();

  cv1->cd(1);

  histoA->SetMarkerStyle(20);
  histoA->SetMarkerSize(2);
  histoA->SetMarkerColor(1);
  histoA->Draw("E1X0");
  //  histoA->DrawNormalized("E1X0");

  histoB->SetMarkerStyle(21);
  histoB->SetMarkerSize(2);
  histoB->SetMarkerColor(2);
  histoB->Draw("E1X0SAME");
  //  histoB->DrawNormalized("E1X0SAME");

  histoC->SetMarkerStyle(22);
  histoC->SetMarkerSize(2);
  histoC->SetMarkerColor(3);
  histoC->Draw("E1X0SAME");
  //  histoC->DrawNormalized("E1X0SAME");

  TLegend *leg = new TLegend(0.685, 0.575, 0.8, 0.875); 
  leg->AddEntry(histoA,"A");
  leg->AddEntry(histoB,"B");
  leg->AddEntry(histoC,"C");

  leg->Draw("SAME");


  // set style for ratio plots

  histoAoverBRatio->SetMarkerStyle(20);
  histoAoverCRatio->SetMarkerStyle(21);
  histoBoverCRatio->SetMarkerStyle(22);
  histoAoverBRatio->SetMarkerSize(1);
  histoAoverCRatio->SetMarkerSize(1);
  histoBoverCRatio->SetMarkerSize(1);
  histoAoverBRatio->SetMarkerColor(1);
  histoAoverCRatio->SetMarkerColor(2);
  histoBoverCRatio->SetMarkerColor(3);
  histoAoverBRatio->SetLineColor(1);
  histoAoverCRatio->SetLineColor(2);
  histoBoverCRatio->SetLineColor(3);

  cv1->cd(2);
  histoAoverBRatio->GetYaxis()->SetRangeUser(0.9,1.1);
  histoAoverBRatio->GetYaxis()->CenterTitle(1);
  histoAoverBRatio->GetYaxis()->SetTitleSize( 0.16 );
  histoAoverBRatio->GetYaxis()->SetTitleOffset( 0.425 );
  histoAoverBRatio->GetYaxis()->SetLabelSize( 0.16 );
  histoAoverBRatio->GetYaxis()->SetNdivisions( 505 );

  histoAoverBRatio->GetXaxis()->SetTitleSize( 0.16 );
  histoAoverBRatio->GetXaxis()->SetLabelSize( 0.16 );
  histoAoverBRatio->GetXaxis()->SetTitleOffset( 1 );
  histoAoverBRatio->GetXaxis()->SetLabelOffset( 0.006 );
  histoAoverBRatio->GetXaxis()->SetNdivisions( 510 );
  histoAoverBRatio->GetXaxis()->SetTickLength( histoAoverBRatio->GetXaxis()->GetTickLength() * 3.0 );
  histoAoverBRatio->SetYTitle("Data/Data"); 

  histoAoverBRatio->Draw("E1X0");
  histoAoverCRatio->Draw("E1X0SAME");
  histoBoverCRatio->Draw("E1X0SAME");

  cv1->Update();
  cv1->Print(TString(fname)+TString("_Linear.png"));


  /// the same with log scale

 // make new Canvas for plotting
  TCanvas *cv2 = new TCanvas(histonameA+"log", histonameA+"log", 1024,1024);
  cv2->cd();
  cv2->Clear();
  cv2->Divide(1,2,0.01,0.0);
  

  cv2->cd(1);
  cv2->SetFillColor(0); 
  gStyle->SetOptTitle(0);
  gPad->SetLogy();
  gPad->SetFillColor(0);
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  cv2->cd(2);
  cv2->SetFillColor(0);
  gStyle->SetOptTitle(0);
  gPad->SetFillColor(0);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();

  cv2->cd(1);

 
  histoA->Draw("E1X0");
  histoB->Draw("E1X0SAME");
  histoC->Draw("E1X0SAME");

  TLegend *leg2 = new TLegend(0.685, 0.575, 0.8, 0.875); 
  leg2->AddEntry(histoA,"A");
  leg2->AddEntry(histoB,"B");
  leg2->AddEntry(histoC,"C");

  leg2->Draw("SAME");



  cv2->cd(2);
 
  histoAoverBRatio->Draw("E1X0");
  histoAoverCRatio->Draw("E1X0SAME");
  histoBoverCRatio->Draw("E1X0SAME");

  cv2->Update();
  cv2->Print(TString(fname)+TString("_Log.png"));


}

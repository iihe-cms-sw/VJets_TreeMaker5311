#include "bTag/CommissioningCommonSetup/interface/QualityHistogram.h"

#include <TH1D.h>
#include <TH2D.h>

QualityHistogram::QualityHistogram() : dataHist_undef(0), dataHist_loose(0), dataHist_tight(0), dataHist_high_purity(0),
                                       mcHist_undef(0), mcHist_loose(0), mcHist_tight(0), mcHist_high_purity(0) {}

QualityHistogram::QualityHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx,
                                 const double& xmin, const double& xmax)
{
  const char* titleCStr = title.c_str();

  dataHist_undef = new TH1D((name + "_data_hist_undef").c_str(), titleCStr, nbinsx, xmin, xmax);
  dataHist_loose = new TH1D((name + "_data_hist_loose").c_str(), titleCStr, nbinsx, xmin, xmax);
  dataHist_tight = new TH1D((name + "_data_hist_tight").c_str(), titleCStr, nbinsx, xmin, xmax);
  dataHist_high_purity = new TH1D((name + "_data_hist_high_purity").c_str(), titleCStr, nbinsx, xmin, xmax);

  mcHist_undef = new TH1D((name + "_mc_hist_undef").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHist_loose = new TH1D((name + "_mc_hist_loose").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHist_tight = new TH1D((name + "_mc_hist_tight").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHist_high_purity = new TH1D((name + "_mc_hist_high_purity").c_str(), titleCStr, nbinsx, xmin, xmax);
}

QualityHistogram::QualityHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx, 
                                 const double& xmin, const double& xmax, const unsigned int nbinsy, 
                                 const double& ymin, const double& ymax)
{
  const char* titleCStr = title.c_str();

  dataHist_undef = new TH2D((name + "_data_hist_undef").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  dataHist_loose = new TH2D((name + "_data_hist_loose").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  dataHist_tight = new TH2D((name + "_data_hist_tight").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  dataHist_high_purity = new TH2D((name + "_data_hist_high_purity").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);

  mcHist_undef = new TH2D((name + "_mc_hist_undef").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHist_loose = new TH2D((name + "_mc_hist_loose").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHist_tight = new TH2D((name + "_mc_hist_tight").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHist_high_purity = new TH2D((name + "_mc_hist_high_purity").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
}

QualityHistogram::~QualityHistogram() {}
  
void QualityHistogram::fill(const double& x, const double& weight, const bool isData, const int quality)
{
  if(isData)
  {
    switch(quality)
    {
      case 0:
        dataHist_loose->Fill(x, weight);
        break;
      case 1:
        dataHist_tight->Fill(x, weight);
        break;
      case 2:
        dataHist_high_purity->Fill(x, weight);
        break;
      default:
        dataHist_undef->Fill(x, weight);
    }
  }
  else
  {
    switch(quality)
    {
      case 0:
        mcHist_loose->Fill(x, weight);
        break;
      case 1:
        mcHist_tight->Fill(x, weight);
        break;
      case 2:
        mcHist_high_purity->Fill(x, weight);
        break;
      default:
        mcHist_undef->Fill(x, weight);
    }
  }
}

void QualityHistogram::fill(const double& x, const double& y, const double& weight, const bool isData, const int quality)
{
  if(isData)
  {
    switch(quality)
    {
      case 0:
        static_cast<TH2D*>(dataHist_loose)->Fill(x, weight);
        break;
      case 1:
        static_cast<TH2D*>(dataHist_tight)->Fill(x, weight);
        break;
      case 2:
        static_cast<TH2D*>(dataHist_high_purity)->Fill(x, weight);
        break;
      default:
        static_cast<TH2D*>(dataHist_undef)->Fill(x, weight);
    }
  }
  else
  {
    switch(quality)
    {
      case 0:
        static_cast<TH2D*>(mcHist_loose)->Fill(x, weight);
        break;
      case 1:
        static_cast<TH2D*>(mcHist_tight)->Fill(x, weight);
        break;
      case 2:
        static_cast<TH2D*>(mcHist_high_purity)->Fill(x, weight);
        break;
      default:
        static_cast<TH2D*>(mcHist_undef)->Fill(x, weight);
    }
  }
}

void QualityHistogram::setXTitle(const std::string& title)
{
  const char* titleCStr = title.c_str();

  dataHist_undef->GetXaxis()->SetTitle(titleCStr);
  dataHist_loose->GetXaxis()->SetTitle(titleCStr);
  dataHist_tight->GetXaxis()->SetTitle(titleCStr);
  dataHist_high_purity->GetXaxis()->SetTitle(titleCStr);
  mcHist_undef->GetXaxis()->SetTitle(titleCStr);
  mcHist_loose->GetXaxis()->SetTitle(titleCStr);
  mcHist_tight->GetXaxis()->SetTitle(titleCStr);
  mcHist_high_purity->GetXaxis()->SetTitle(titleCStr);
}

void QualityHistogram::setYTitle(const std::string& title)
{
  const char* titleCStr = title.c_str();

  dataHist_undef->GetYaxis()->SetTitle(titleCStr);
  dataHist_loose->GetYaxis()->SetTitle(titleCStr);
  dataHist_tight->GetYaxis()->SetTitle(titleCStr);
  dataHist_high_purity->GetYaxis()->SetTitle(titleCStr);
  mcHist_undef->GetYaxis()->SetTitle(titleCStr);
  mcHist_loose->GetYaxis()->SetTitle(titleCStr);
  mcHist_tight->GetYaxis()->SetTitle(titleCStr);
  mcHist_high_purity->GetYaxis()->SetTitle(titleCStr);
}

void QualityHistogram::sumW2()
{
  dataHist_undef->Sumw2();
  dataHist_loose->Sumw2();
  dataHist_tight->Sumw2();
  dataHist_high_purity->Sumw2();
  mcHist_undef->Sumw2();
  mcHist_loose->Sumw2();
  mcHist_tight->Sumw2();
  mcHist_high_purity->Sumw2();
}

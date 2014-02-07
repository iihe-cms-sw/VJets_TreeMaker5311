#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"

#include <TH1D.h>
#include <TH2D.h>

//#include <iostream>

FlavorHistogram::FlavorHistogram() : bScale(1.0), cScale(1.0), dataHist(0), mcHistAll(0), mcHistLight(0), mcHistNone(0),
  mcHistC(0), mcHistB(0), mcHistBGluSplit(0) {}

FlavorHistogram::FlavorHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx,
                                 const double& xmin, const double& xmax)
{
  const char* titleCStr = title.c_str();

  dataHist = new TH1D((name + "_data_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHistAll = new TH1D((name + "_mc_all_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHistLight = new TH1D((name + "_mc_light_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHistNone = new TH1D((name + "_mc_none_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHistC = new TH1D((name + "_mc_c_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHistB = new TH1D((name + "_mc_b_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
  mcHistBGluSplit = new TH1D((name + "_mc_bglusplit_hist").c_str(), titleCStr, nbinsx, xmin, xmax);
}

FlavorHistogram::FlavorHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx, 
                                 const double& xmin, const double& xmax, const unsigned int nbinsy, 
                                 const double& ymin, const double& ymax) : bScale(1.0), cScale(1.0)
{
  const char* titleCStr = title.c_str();

  dataHist = new TH2D((name + "_data_hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHistAll = new TH2D((name + "_mc_all hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHistLight = new TH2D((name + "_mc_light hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHistNone = new TH2D((name + "_mc_none hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHistC = new TH2D((name + "_mc_c_hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHistB = new TH2D((name + "_mc_b_hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
  mcHistBGluSplit = new TH2D((name + "_mc_bglusplit_hist").c_str(), titleCStr, nbinsx, xmin, xmax, nbinsy, ymin, ymax);
}

FlavorHistogram::~FlavorHistogram() {}
  
void FlavorHistogram::fill(const double& x, const double& weight, const bool isData, const int flavor, const bool bglusplit)
{
  /*std::cout << "x = " << x << "; "
            << "weight = " << weight << "; "
            << "isData = " << isData << "; "
            << "flavor = " << flavor << "; "
            << "bglusplit = " << bglusplit << "." << std::endl;*/

  if(isData)
  {
    dataHist->Fill(x, weight);
  }
  else
  {
    switch(flavor)
    {
      case 1:
      case 2:
      case 3:
      case 21:
        mcHistAll->Fill(x, weight);
        mcHistLight->Fill(x, weight);
        break;
      case 4:
        mcHistAll->Fill(x, weight * cScale);
        mcHistC->Fill(x, weight * cScale);
        break;
      case 5:
        mcHistAll->Fill(x, weight * bScale);
        bglusplit ? mcHistBGluSplit->Fill(x, weight * bScale) : mcHistB->Fill(x, weight * bScale);
        break;
      default:
        mcHistAll->Fill(x, weight);
        mcHistNone->Fill(x, weight);
    }
  }
}

void FlavorHistogram::fill(const double& x, const double& y, const double& weight, const bool isData, const int flavor, const bool bglusplit)
{
  if(isData)
  {
    static_cast<TH2D*>(dataHist)->Fill(x, y, weight);
  }
  else
  {
    switch(flavor)
    {
      case 1:
      case 2:
      case 3:
      case 21:
        static_cast<TH2D*>(mcHistAll)->Fill(x, y, weight);
        static_cast<TH2D*>(mcHistLight)->Fill(x, y, weight);
        break;
      case 4:
        static_cast<TH2D*>(mcHistAll)->Fill(x, y, weight * cScale);
        static_cast<TH2D*>(mcHistC)->Fill(x, y, weight * cScale);
        break;
      case 5:
        static_cast<TH2D*>(mcHistAll)->Fill(x, y, weight * bScale);
        bglusplit ? static_cast<TH2D*>(mcHistBGluSplit)->Fill(x, y, weight * bScale) :
                    static_cast<TH2D*>(mcHistB)->Fill(x, y, weight * bScale);
        break;
      default:
        static_cast<TH2D*>(mcHistAll)->Fill(x, y, weight);
        static_cast<TH2D*>(mcHistNone)->Fill(x, y, weight);
    }
  }
}

void FlavorHistogram::setXTitle(const std::string& title)
{
  const char* titleCStr = title.c_str();

  dataHist->GetXaxis()->SetTitle(titleCStr);
  mcHistAll->GetXaxis()->SetTitle(titleCStr);
  mcHistLight->GetXaxis()->SetTitle(titleCStr);
  mcHistNone->GetXaxis()->SetTitle(titleCStr);
  mcHistC->GetXaxis()->SetTitle(titleCStr);
  mcHistB->GetXaxis()->SetTitle(titleCStr);
  mcHistBGluSplit->GetXaxis()->SetTitle(titleCStr);
}

void FlavorHistogram::setYTitle(const std::string& title)
{
  const char* titleCStr = title.c_str();

  dataHist->GetYaxis()->SetTitle(titleCStr);
  mcHistAll->GetYaxis()->SetTitle(titleCStr);
  mcHistLight->GetYaxis()->SetTitle(titleCStr);
  mcHistNone->GetYaxis()->SetTitle(titleCStr);
  mcHistC->GetYaxis()->SetTitle(titleCStr);
  mcHistB->GetYaxis()->SetTitle(titleCStr);
  mcHistBGluSplit->GetYaxis()->SetTitle(titleCStr);
}

void FlavorHistogram::sumW2()
{
  dataHist->Sumw2();
  mcHistAll->Sumw2();
  mcHistLight->Sumw2();
  mcHistNone->Sumw2();
  mcHistC->Sumw2();
  mcHistB->Sumw2();
  mcHistBGluSplit->Sumw2();
}

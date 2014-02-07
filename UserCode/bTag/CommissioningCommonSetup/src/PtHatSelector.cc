#include "bTag/CommissioningCommonSetup/interface/PtHatSelector.h"
#include "bTag/CommissioningCommonSetup/interface/generateName.h"

#include <sstream>
#include <algorithm>
#include <functional>

PtHatSelector::PtHatSelector() : BaseSelector(), ptHat(0), b_ptHat(0), info(), fFormulaX(0), fSelection(0), 
  fSelectionPtHat(0), data_hist(0), pthat_hist(0), mc_hists() {}

PtHatSelector::PtHatSelector(const InformationPtHat& info_) : BaseSelector(), ptHat(0), b_ptHat(0), info(info_),
  fFormulaX(0), fSelection(0), fSelectionPtHat(0)
{
  for(std::vector<double>::const_iterator iPlot = info_.getPtHatBins().begin(); (iPlot+1)!= info_.getPtHatBins().end(); ++iPlot)
  {
    std::stringstream lowBin;
    lowBin << *iPlot;
    std::stringstream highBin;
    highBin << *(iPlot+1);
    TH1D* temp = new TH1D(((info_.getPlotName()+"_mc_hist_"+lowBin.str()+"_"+highBin.str()).c_str()),info_.getPlotTitle().c_str(),info_.getNBinsX(),info_.getXLow(),info_.getXUp());
    temp->GetXaxis()->SetTitle( info_.getXTitle().c_str() );
	temp->Sumw2();
    mc_hists.push_back(temp);    
  }

  data_hist = new TH1D((info_.getPlotName()+"_data_hist").c_str(),info_.getPlotTitle().c_str(),info_.getNBinsX(),info_.getXLow(),info_.getXUp());
  data_hist->GetXaxis()->SetTitle( info_.getXTitle().c_str() );
  data_hist->Sumw2();

  pthat_hist = new TH1D((info_.getPlotName()+"_pthat_hist").c_str(),info_.getPlotTitle().c_str(),info_.getNBinsPtHat(),info_.getPtHatLow(),info_.getPtHatUp());
  pthat_hist->GetXaxis()->SetTitle("p_{T} hat");
  pthat_hist->Sumw2();
}

void PtHatSelector::Init(TTree* tree)
{
  if(!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  treeWeight = fChain->GetWeight();

  ptHat = SetPtrToValue<Float_t>(b_ptHat, "ptHat");

  if(fFormulaX == 0)
  {
    fFormulaX = new TTreeFormula(generateName().c_str(), info.getAliasX().c_str(), fChain);
    fFormulaX->SetQuickLoad(true);
  }
  else
  {
    fFormulaX->SetTree(fChain);
  }

  if(fSelection == 0)
  {
    fSelection = new TTreeFormula(generateName().c_str(), info.getCut().c_str(), fChain);
    fSelection->SetQuickLoad(true);
  }
  else
  {
    fSelection->SetTree(fChain);
  }

  if(fSelectionPtHat == 0)
  {
    fSelectionPtHat = new TTreeFormula(generateName().c_str(), info.getPtHatCut().c_str(), fChain);
    fSelectionPtHat->SetQuickLoad(true);
  }
  else
  {
    fSelectionPtHat->SetTree(fChain);
  }
}

Bool_t PtHatSelector::Process(Long64_t entry)
{
  if(!fFormulaX || !fSelection || !fSelectionPtHat)
    return kTRUE;

  b_ptHat->GetEntry(entry);

  int nData = fSelection->GetNdata();
  fFormulaX->GetNdata();
  fSelectionPtHat->GetNdata();

  fFormulaX->EvalInstance(0);
  for(int i = 0; i != nData; ++i)
  {
    if(fSelection->EvalInstance(i))
    {
      if(isData)
      {
        data_hist->Fill(fFormulaX->EvalInstance(i), treeWeight);
      }
      else
      {
        int iter = std::count_if(info.getPtHatBins().begin(), info.getPtHatBins().end() - 1, std::bind2nd(std::less<double>(), *ptHat)) - 1;
        if(iter > -1)  mc_hists[iter]->Fill(fFormulaX->EvalInstance(i), treeWeight);
      }
    }

    if(!isData && fSelectionPtHat->EvalInstance())
    {
      pthat_hist->Fill(fFormulaX->EvalInstance(i), treeWeight);
    }
  }

  return kTRUE;
}

#include "bTag/CommissioningCommonSetup/interface/CutCompSelector.h"
#include "bTag/CommissioningCommonSetup/interface/generateName.h"

#include <sstream>

CutCompSelector::CutCompSelector() : BaseSelector(), MCTrueFlavor(0), isBGluonSplitting(0), info(), fFormulaX(0),
  fFormulaCutInfo(0), fSelection(0), dataCutCompInfo(0), mcCutCompInfo(0) {}

CutCompSelector::CutCompSelector(const InformationCutComp& info_) : BaseSelector(), MCTrueFlavor(0),
  isBGluonSplitting(0), info(info_), varCutInfo(""), fFormulaX(0), fFormulaCutInfo(0), fSelection(0)
{
  std::stringstream binaryBinningStream("");
  for(unsigned int i = 0; i != info_.getCutList().size(); ++i)
  {
    unsigned long binaryBinner = (0x1 << i);
    binaryBinningStream << binaryBinner << "*("<< info_.getCutItem(i) <<")";
    if(i < info_.getLabelList().size() - 1) binaryBinningStream <<"+";
  }
  varCutInfo = binaryBinningStream.str(); 

  fFormulaCut = std::vector<TTreeFormula*>(info_.getCutList().size(), 0);

  unsigned long pow2 = 0x1 << info_.getCutList().size();
  dataCutCompInfo = new TH1D((info_.getPlotName()+"_CutCompInfo_data").c_str(),info_.getPlotTitle().c_str(), pow2 - 1, 0.5, pow2 - 0.5);
  dataCutCompInfo->GetXaxis()->SetTitle(info_.getXTitle().c_str());
  dataCutCompInfo->GetYaxis()->SetTitle(info_.getYTitle().c_str());
  dataCutCompInfo->Sumw2();
  mcCutCompInfo = new TH1D((info_.getPlotName()+"_CutCompInfo_mc").c_str(),info_.getPlotTitle().c_str(), pow2 - 1, 0.5, pow2 - 0.5);
  mcCutCompInfo->GetXaxis()->SetTitle(info_.getXTitle().c_str());
  mcCutCompInfo->GetYaxis()->SetTitle(info_.getYTitle().c_str());
  mcCutCompInfo->Sumw2();
  if(!info_.getAliasX().empty())
  {
    for(unsigned int i = 0; i != info_.getLabelList().size(); ++i)
    {
      std::stringstream nPlot("");
	  nPlot << i;
	  histograms.push_back(FlavorHistogram(info_.getPlotName() + nPlot.str(), info_.getPlotTitle(), info_.getNBinsX(), info_.getXLow(), info_.getXUp(), 2, 0, 1));
    }
  }
}

CutCompSelector::~CutCompSelector()
{
  if(fFormulaX) delete fFormulaX;
  if(fFormulaCutInfo) delete fFormulaCutInfo;
  if(fSelection) delete fSelection;

  for(unsigned int i = 0; i != fFormulaCut.size(); ++i)
    if(fFormulaCut[i]) delete fFormulaCut[i];
}

void CutCompSelector::Init(TTree* tree)
{
  if(!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  treeWeight = fChain->GetWeight();
  
  MCTrueFlavor = SetPtrToValue<Int_t>(b_MCTrueFlavor, "MCTrueFlavor");
  isBGluonSplitting = SetPtrToValue<Bool_t>(b_isBGluonSplitting, "isBGluonSplitting");

  if(fFormulaCutInfo == 0)
  {
    fFormulaCutInfo = new TTreeFormula(generateName().c_str(), varCutInfo.c_str(), fChain);
    fFormulaCutInfo->SetQuickLoad(true);
  }
  else
  {
    fFormulaCutInfo->SetTree(fChain);
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

  if(!info.getAliasX().empty())
  {
    if(fFormulaX == 0)
    {
      fFormulaX = new TTreeFormula(generateName().c_str(), info.getAliasX().c_str(), fChain);
      fFormulaX->SetQuickLoad(true);
    }
    else
    {
      fFormulaX->SetTree(fChain);
    }
    for(unsigned int i = 0; i != info.getCutList().size(); ++i)
    {
      TTreeFormula* temp = fFormulaCut[i];
      if(temp == 0)
      {
        temp = new TTreeFormula(generateName().c_str(), info.getCutItem(i).c_str(), fChain);
        temp->SetQuickLoad(true);
      }
      else
      {
        temp->SetTree(fChain);
      }
    }
  }
}

Bool_t CutCompSelector::Notify()
{
  if(fFormulaX) fFormulaX->Notify();
  if(fFormulaCutInfo) fFormulaCutInfo->Notify();
  if(fSelection) fSelection->Notify();
  
  for(unsigned int i = 0; i != fFormulaCut.size(); ++i)
    if(fFormulaCut[i]) fFormulaCut[i]->Notify();

  return kTRUE;
}

Bool_t CutCompSelector::Process(Long64_t entry)
{
  b_MCTrueFlavor->GetEntry(entry);
  b_isBGluonSplitting->GetEntry(entry);

  int nData = fSelection->GetNdata();

  fFormulaCutInfo->GetNdata();
  fFormulaCutInfo->EvalInstance(0);

  if(fFormulaX)
  {
    fFormulaX->GetNdata();
    fFormulaX->EvalInstance(0);
  }

  for(unsigned int i = 0; i != fFormulaCut.size(); ++i)
  {
    if(fFormulaCut[i])
    {
      fFormulaCut[i]->GetNdata();
      fFormulaCut[i]->EvalInstance(0);
    }
  }

  for(int i = 0; i != nData; ++i)
  {
    if(fSelection->EvalInstance(i))
    {
      if(isData)
        dataCutCompInfo->Fill(fFormulaCutInfo->EvalInstance(i), treeWeight);
      else
        mcCutCompInfo->Fill(fFormulaCutInfo->EvalInstance(i), treeWeight);

      for(unsigned int j = 0; j != fFormulaCut.size(); ++j)
      {
        if(fFormulaX && fFormulaCut[j])
          histograms[j].fill(fFormulaX->EvalInstance(i), fFormulaCut[j]->EvalInstance(i), treeWeight, isData, MCTrueFlavor[i], *isBGluonSplitting);
      }
    }
  }

  return kTRUE;
}

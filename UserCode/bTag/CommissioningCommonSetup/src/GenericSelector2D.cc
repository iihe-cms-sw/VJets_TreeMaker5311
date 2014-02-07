#include "bTag/CommissioningCommonSetup/interface/GenericSelector2D.h"
#include "bTag/CommissioningCommonSetup/interface/generateName.h"

#include <cstdlib>

GenericSelector2D::GenericSelector2D() : BaseSelector() {} 

GenericSelector2D::GenericSelector2D(const Information2D& info_) : BaseSelector(),
  MCTrueFlavor(0), isBGluonSplitting(0), info(info_), fFormulaX(0), fFormulaY(0), fSelection(0),
  histograms(info_.getPlotName(), info_.getPlotTitle(), info_.getNBinsX(), info_.getXLow(), info_.getXUp(),
             info_.getNBinsY(), info_.getYLow(), info_.getYUp())
{
  histograms.setXTitle(info_.getXTitle());
  histograms.setYTitle(info_.getYTitle());
  histograms.setBScale(info_.getBScale());
  histograms.setCScale(info_.getCScale());
  histograms.sumW2();
}

void GenericSelector2D::Init(TTree* tree)
{
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  treeWeight = fChain->GetWeight();

  MCTrueFlavor = SetPtrToValue<Int_t>(b_MCTrueFlavor, "MCTrueFlavor");
  isBGluonSplitting = SetPtrToValue<Bool_t>(b_isBGluonSplitting, "isBGluonSplitting");

  if(fFormulaX == 0)
  {
    fFormulaX = new TTreeFormula(generateName().c_str(), info.getAliasX().c_str(), fChain);
    fFormulaX->SetQuickLoad(true);
  }
  else
  {
    fFormulaX->SetTree(fChain);
  }

  if(fFormulaY == 0)
  {
    fFormulaY = new TTreeFormula(generateName().c_str(), info.getAliasY().c_str(), fChain);
    fFormulaY->SetQuickLoad(true);
  }
  else
  {
    fFormulaY->SetTree(fChain);
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
}

Bool_t GenericSelector2D::Process(Long64_t entry)
{
  if(!fFormulaX || !fFormulaY || !fSelection)
    return kTRUE;

  b_MCTrueFlavor->GetEntry(entry);
  b_isBGluonSplitting->GetEntry(entry);
  
  int nData = fSelection->GetNdata();
  fFormulaX->GetNdata();
  fFormulaY->GetNdata();

  //This is needed to ensure the needed
  //branches are loaded.
  fFormulaX->EvalInstance(0);
  fFormulaY->EvalInstance(0);
  for(int i = 0; i != nData; ++i)
    if(fSelection->EvalInstance(i))
      histograms.fill(fFormulaX->EvalInstance(i), fFormulaY->EvalInstance(i), isData, MCTrueFlavor[i], *isBGluonSplitting);

  return kTRUE;
}

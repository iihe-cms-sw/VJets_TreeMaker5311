#include "bTag/CommissioningCommonSetup/interface/GenericSelector.h"
#include "bTag/CommissioningCommonSetup/interface/generateName.h"

#include <cstdlib>

GenericSelector::GenericSelector() : BaseSelector() {} 

GenericSelector::GenericSelector(const Information1D& info_): BaseSelector(),
  MCTrueFlavor(0), isBGluonSplitting(0), info(info_), fFormula(0), fSelection(0),
  histograms(info_.getPlotName(), info_.getPlotTitle(), info_.getNBinsX(), info_.getXLow(), info_.getXUp())
{
  histograms.setXTitle(info_.getXTitle());
  histograms.setYTitle(info_.getYTitle());
  histograms.setBScale(info_.getBScale());
  histograms.setCScale(info_.getCScale());
  histograms.sumW2();
}

void GenericSelector::Init(TTree* tree)
{
  if (!tree) return;
  fChain = tree;
  //fChain->SetMakeClass(1);
  treeWeight = fChain->GetWeight();

  MCTrueFlavor = SetPtrToValue<Int_t>(b_MCTrueFlavor, "MCTrueFlavor");
  isBGluonSplitting = SetPtrToValue<Bool_t>(b_isBGluonSplitting, "isBGluonSplitting");

  if(fFormula == 0)
  {
    fFormula = new TTreeFormula(generateName().c_str(), info.getAliasX().c_str(), fChain);
    fFormula->SetQuickLoad(true);
  }
  else
  {
    fFormula->SetTree(fChain);
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

Bool_t GenericSelector::Process(Long64_t entry)
{
  if(!fFormula || !fSelection)
    return kTRUE;

  b_MCTrueFlavor->GetEntry(entry);
  b_isBGluonSplitting->GetEntry(entry);
  
  int nData = fSelection->GetNdata();
  fFormula->GetNdata();

  //This is needed to ensure the needed
  //branches are loaded.
  fFormula->EvalInstance(0);
  for(int i = 0; i != nData; ++i)
  {
    if(fSelection->EvalInstance(i))
    {
      histograms.fill(fFormula->EvalInstance(i), treeWeight, isData, MCTrueFlavor[i], *isBGluonSplitting);
    }
  }

  return kTRUE;
}

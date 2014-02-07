#include "bTag/CommissioningCommonSetup/interface/GenericTrackQualitySelector.h"
#include "bTag/CommissioningCommonSetup/interface/generateName.h"

#include <cstdlib>

GenericTrackQualitySelector::GenericTrackQualitySelector() : BaseSelector() {} 

GenericTrackQualitySelector::GenericTrackQualitySelector(const InformationQuality& info_) :
  BaseSelector(), trackQuality(0), info(info_), fFormula(0), fSelection(0),
  histograms(info_.getPlotName(), info_.getPlotTitle(), info_.getNBinsX(), info_.getXLow(), info_.getXUp())
{
  histograms.setXTitle(info_.getXTitle());
  histograms.setYTitle(info_.getYTitle());
  histograms.sumW2();
}

void GenericTrackQualitySelector::Init(TTree* tree)
{
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  treeWeight = fChain->GetWeight();

  trackQuality = SetPtrToValue<Int_t>(b_trackQuality, "trackQuality");

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

Bool_t GenericTrackQualitySelector::Process(Long64_t entry)
{
  if(!fFormula || !fSelection)
    return kTRUE;

  b_trackQuality->GetEntry(entry);
  
  int nData = fSelection->GetNdata();
  fFormula->GetNdata();

  //This is needed to ensure the needed
  //branches are loaded.
  fFormula->EvalInstance(0);
  for(int i = 0; i != nData; ++i)
    if(fSelection->EvalInstance(i))
      histograms.fill(fFormula->EvalInstance(i), treeWeight, isData, trackQuality[i]);

  return kTRUE;
}

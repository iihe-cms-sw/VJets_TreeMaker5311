#ifndef bTag_CommissioningCommonSetup_CutCompSelector_h
#define bTag_CommissioningCommonSetup_CutCompSelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/InformationCutComp.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"

#include <vector>

#include <TChain.h>
#include <TBranch.h>
#include <TTreeFormula.h>

class CutCompSelector : public BaseSelector
{
  public:
    CutCompSelector();
    CutCompSelector(const InformationCutComp& info_);
    virtual ~CutCompSelector();
    virtual void    Init(TTree *tree);
    inline virtual Bool_t Notify();
    virtual Bool_t  Process(Long64_t entry);
    inline virtual void setHistograms(const std::vector<FlavorHistogram>& histos) {histograms = histos;}
    inline virtual TH1D* const getDataCutCompInfoHist() const {return dataCutCompInfo;}
    inline virtual TH1D* const getMCCutCompInfoHist() const {return mcCutCompInfo;}
    inline virtual std::vector<FlavorHistogram> GetCutHistograms() {return histograms;}
    inline InformationCutComp const getInfo() const {return info;}

  protected:
    Int_t const* MCTrueFlavor;
    Bool_t const* isBGluonSplitting;

    TBranch* b_MCTrueFlavor;
    TBranch* b_isBGluonSplitting;

    InformationCutComp info;

    std::string varCutInfo;

    TTreeFormula* fFormulaX;
    TTreeFormula* fFormulaCutInfo;
    std::vector<TTreeFormula*> fFormulaCut;
    TTreeFormula* fSelection;

    TH1D* dataCutCompInfo;
    TH1D* mcCutCompInfo;
    std::vector<FlavorHistogram> histograms;
};

#endif

#ifndef bTag_CommissioningCommonSetup_GenericSelector_h
#define bTag_CommissioningCommonSetup_GenericSelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

#include <string>

#include <TChain.h>
#include <TTreeFormula.h>

class GenericSelector : public BaseSelector
{
  public:
    GenericSelector();
    GenericSelector(const Information1D& info_);
    virtual ~GenericSelector() { if(fFormula) delete fFormula; if(fSelection) delete fSelection;}
    virtual void    Init(TTree *tree);
    inline virtual Bool_t Notify() {fFormula->Notify(); fSelection->Notify(); return kTRUE;}
    virtual Bool_t  Process(Long64_t entry);
    inline virtual void setHistograms(const FlavorHistogram& histos) {histograms = histos;}
    inline virtual FlavorHistogram getHistograms() const {return histograms;}
    inline Information1D const getInfo() const {return info;}

  protected:
    Int_t const* MCTrueFlavor;
    Bool_t const* isBGluonSplitting;

    TBranch* b_MCTrueFlavor;
    TBranch* b_isBGluonSplitting;

    Information1D info;

    TTreeFormula* fFormula;
    TTreeFormula* fSelection;
  
    FlavorHistogram histograms;
};

#endif

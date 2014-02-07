#ifndef bTag_CommissioningCommonSetup_PtHatSelector_h
#define bTag_CommissioningCommonSetup_PtHatSelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/InformationPtHat.h"

#include <vector>

#include <TTree.h>
#include <TBranch.h>
#include <TTreeFormula.h>
#include <TH1D.h>

class PtHatSelector : public BaseSelector
{
  public:
    PtHatSelector();
    PtHatSelector(const InformationPtHat& info_);
    virtual ~PtHatSelector() {if(fFormulaX) delete fFormulaX; if(fSelection) delete fSelection; if(fSelectionPtHat) delete fSelectionPtHat;}
    virtual void Init(TTree* tree);
    virtual inline Bool_t Notify() {fFormulaX->Notify(); fSelection->Notify(); fSelectionPtHat->Notify(); return kTRUE;}
    virtual Bool_t Process(Long64_t entry);
    inline InformationPtHat const getInfo() const {return info;}

    inline TH1D* const getDataHist() const {return data_hist;}
    inline TH1D* const getPtHatHist() const {return pthat_hist;}
    inline std::vector<TH1D*> const getMCHists() const {return mc_hists;}


  protected:
    Float_t* ptHat;

    TBranch* b_ptHat;

    InformationPtHat info;

    TTreeFormula* fFormulaX;
    TTreeFormula* fSelection;
    TTreeFormula* fSelectionPtHat;

    TH1D* data_hist;
    TH1D* pthat_hist;
    std::vector<TH1D*> mc_hists;
};

#endif

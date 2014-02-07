#ifndef bTag_CommissioningCommonSetup_GenericSelector2D_h
#define bTag_CommissioningCommonSetup_GenericSelector2D_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/Information2D.h"

#include <string>

#include <TTreeFormula.h>
#include <TH2D.h>

class GenericSelector2D : public BaseSelector
{
  public:
    GenericSelector2D();
    GenericSelector2D(const Information2D& info_);
    virtual ~GenericSelector2D() { if(fFormulaX) delete fFormulaX; if(fFormulaY) delete fFormulaY; if(fSelection) delete fSelection;}
    virtual void    Init(TTree *tree);
    inline virtual Bool_t Notify() {fFormulaX->Notify(); fFormulaY->Notify(); fSelection->Notify(); return kTRUE;}
    virtual Bool_t  Process(Long64_t entry);
    inline FlavorHistogram getHistograms() const {return histograms;}
    inline Information2D const getInfo() const {return info;}

  protected:
    Int_t* MCTrueFlavor;
    Bool_t* isBGluonSplitting;

    TBranch* b_MCTrueFlavor;
    TBranch* b_isBGluonSplitting;

    Information2D const info;

    TTreeFormula* fFormulaX;
    TTreeFormula* fFormulaY;
    TTreeFormula* fSelection;
  
    /*TH2D* dataHist;
    TH2D* mcHistb;
    TH2D* mcHistbglusplit;
    TH2D* mcHistc; 
    TH2D* mcHistl;
    TH2D* mcHistn;*/
    FlavorHistogram histograms;
};

#endif

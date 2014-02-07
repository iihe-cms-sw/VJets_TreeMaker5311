#ifndef bTag_CommissioningCommonSetup_GenericJetTrackQualitySelector_h
#define bTag_CommissioningCommonSetup_GenericJetTrackQualitySelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/QualityHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/InformationQuality.h"

#include <string>

#include <TChain.h>
#include <TTreeFormula.h>
#include <TH1D.h>

class GenericJetTrackQualitySelector : public BaseSelector
{
  public:
    GenericJetTrackQualitySelector();
    GenericJetTrackQualitySelector(const InformationQuality& info_);
    virtual ~GenericJetTrackQualitySelector() { if(fFormula) delete fFormula; if(fSelection) delete fSelection;}
    virtual void    Init(TTree *tree);
    inline virtual Bool_t Notify() {fFormula->Notify(); fSelection->Notify(); return kTRUE;}
    virtual Bool_t  Process(Long64_t entry);
    inline QualityHistogram getHistograms() const {return histograms;}
    inline InformationQuality const getInfo() const {return info;}

  protected:
    Int_t const* jetTrackQuality;

    TBranch* b_jetTrackQuality;

    InformationQuality info;

    TTreeFormula* fFormula;
    TTreeFormula* fSelection;
  
    QualityHistogram histograms;
};

#endif

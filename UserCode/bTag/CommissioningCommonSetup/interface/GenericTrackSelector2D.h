#ifndef bTag_CommissioningCommonSetup_GenericTrackSelector2D_h
#define bTag_CommissioningCommonSetup_GenericTrackSelector2D_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/Information2D.h"

#include <string>

#include <TSelector.h>
#include <TChain.h>
#include <TTreeFormula.h>
#include <TH2D.h>

class GenericTrackSelector2D : public BaseSelector
{
  public:
    GenericTrackSelector2D();
    GenericTrackSelector2D(const Information2D& info_);
    virtual ~GenericTrackSelector2D() { if(fFormulaX) delete fFormulaX; if(fFormulaY) delete fFormulaY; if(fSelectionJet) delete fSelectionJet; if(fSelectionTrack) delete fSelectionTrack;}
    virtual void    Init(TTree *tree);
    inline virtual Bool_t Notify(); 
    virtual Bool_t  Process(Long64_t entry);
    inline Information2D const getInfo() const {return info;}

    inline FlavorHistogram getHistograms() const {return histograms;}

  protected:
    Int_t const* nJets;
    Int_t const* nTracks;
    Int_t const* MCTrueFlavor;
    Int_t const* trackJetIndex;
    Bool_t const* isBGluonSplitting;

    TBranch* b_nJets;
    TBranch* b_nTracks;
    TBranch* b_MCTrueFlavor;
    TBranch* b_trackJetIndex;
    TBranch* b_isBGluonSplitting;

    Information2D info;

    TTreeFormula* fFormulaX;
    TTreeFormula* fFormulaY;
    TTreeFormula* fSelectionJet;
    TTreeFormula* fSelectionTrack;
  
    FlavorHistogram histograms;
};

#endif

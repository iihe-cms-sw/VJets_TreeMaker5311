#ifndef bTag_CommissioningCommonSetup_GenericTrackSelector_h
#define bTag_CommissioningCommonSetup_GenericTrackSelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

#include <string>

#include <TTreeFormula.h>
#include <TH1D.h>
#include <TBranch.h>

class GenericTrackSelector : public BaseSelector
{
  public:
    GenericTrackSelector();
    GenericTrackSelector(const Information1D& info_);
    virtual ~GenericTrackSelector() { if(fFormula) delete fFormula; if(fSelectionJet) delete fSelectionJet; if(fSelectionTrack) delete fSelectionTrack;}
    virtual void    Init(TTree *tree);
    virtual Bool_t Notify();
    virtual Bool_t  Process(Long64_t entry);
    inline FlavorHistogram getHistograms() const {return histograms;}
    inline Information1D const getInfo() const {return info;} 

  protected:
    Int_t const* nJets;
    Int_t const* nTracks;
    Int_t const* MCTrueFlavor;
    Int_t const* trackJetIndex;
    Bool_t const* isBGluonSplitting;

    TBranch* b_trigger;
    TBranch* b_nJets;
    TBranch* b_nTracks;
    TBranch* b_jetPt;
    TBranch* b_jetEta;
    TBranch* b_MCTrueFlavor;
    TBranch* b_trackJetIndex;
    TBranch* b_isBGluonSplitting;

    Information1D info;

    TTreeFormula* fFormula;
    TTreeFormula* fSelectionJet;
    TTreeFormula* fSelectionTrack;
  
    FlavorHistogram histograms;
};

#endif

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 12 12:20:57 2010 by ROOT version 5.22/00d
// from TTree t/t
// found on file: /storage/5/jyothsna/btag_18Sep_QCDPt80/standardPFNtuple_1_2_pIs.root
//////////////////////////////////////////////////////////

#ifndef bTag_CommissioningCommonSetup_MuonSelector_h
#define bTag_CommissioningCommonSetup_MuonSelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/InformationMuonCuts.h"

#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include "TH1D.h"

class MuonSelector : public BaseSelector {
  public :
    MuonSelector();
    MuonSelector(const InformationMuonCuts& info_);
    virtual ~MuonSelector() { }
    virtual void    Init(TTree *tree);
    virtual Bool_t  Process(Long64_t entry);
    inline FlavorHistogram getHistograms() const {return histograms;}
    inline InformationMuonCuts const getInfo() const {return info;}

  protected:
   // some objects
    InformationMuonCuts info;
    FlavorHistogram histograms;


   // Declaration of leaf types
    Bool_t*          triggerHLTJet30U;
    Bool_t*          isBGluonSplitting;
    Int_t*           nJets;
    Float_t*         jetPt;   //[nJets]
    Float_t*         jetEta;   //[nJets]
    Int_t*           MCTrueFlavor;   //[nJets]
    Bool_t*          muon1IsGlobal;   //[nJets]
    Bool_t*          muon1IsTracker;   //[nJets]
    Bool_t*          muon1IsStandalone;   //[nJets]
    Float_t*         muon1Pt;   //[nJets]
    Float_t*         muon1Eta;   //[nJets]
    Float_t*         muon1Phi;   //[nJets]
    Int_t*           muon1NumberOfMatches;   //[nJets]
    Int_t*           muon1GlobalMuonHits;   //[nJets]
    Int_t*           muon1InnerValidHits;   //[nJets]
    Int_t*           muon1NExpectedOuterHits;   //[nJets]
    Int_t*           muon1NPixelHits;   //[nJets]
    Float_t*         muon1InnerNChi2;   //[nJets]
    Float_t*         muon1GlobalNChi2;   //[nJets]
    Float_t*         muon1VzPVDist;   //[nJets]
    Float_t*         muon1PtRel;   //[nJets]
    Float_t*         muon1Sip2d;   //[nJets]
    Float_t*         muon1Ip2d;   //[nJets]
    Float_t*         muon1Ipe2d;   //[nJets]
    Float_t*         muon1Sip3d;   //[nJets]
    Float_t*         muon1Ip3d;   //[nJets]
    Float_t*         muon1Ipe3d;   //[nJets]
    Float_t*         muon1P0Par;   //[nJets]
    Float_t*         muon1DeltaR;   //[nJets]
    Float_t*         muon1EtaRel;   //[nJets]
    Float_t*         muon1Ratio;   //[nJets]
    Int_t*           muon1TrackQuality;   //[nJets]
    Float_t*         muon1RatioRel;   //[nJets]
    Bool_t*          muon2IsGlobal;   //[nJets]
    Bool_t*          muon2IsTracker;   //[nJets]
    Bool_t*          muon2IsStandalone;   //[nJets]
    Float_t*         muon2Pt;   //[nJets]
    Float_t*         muon2Eta;   //[nJets]
    Float_t*         muon2Phi;   //[nJets]
    Int_t*           muon2NumberOfMatches;   //[nJets]
    Int_t*           muon2GlobalMuonHits;   //[nJets]
    Int_t*           muon2InnerValidHits;   //[nJets]
    Int_t*           muon2NExpectedOuterHits;   //[nJets]
    Int_t*           muon2NPixelHits;   //[nJets]
    Float_t*         muon2InnerNChi2;   //[nJets]
    Float_t*         muon2GlobalNChi2;   //[nJets]
    Float_t*         muon2VzPVDist;   //[nJets]
    Float_t*         muon2PtRel;   //[nJets]
    Float_t*         muon2Sip2d;   //[nJets]
    Float_t*         muon2Ip2d;   //[nJets]
    Float_t*         muon2Ipe2d;   //[nJets]
    Float_t*         muon2Sip3d;   //[nJets]
    Float_t*         muon2Ip3d;   //[nJets]
    Float_t*         muon2Ipe3d;   //[nJets]
    Float_t*         muon2P0Par;   //[nJets]
    Float_t*         muon2DeltaR;   //[nJets]
    Float_t*         muon2EtaRel;   //[nJets]
    Float_t*         muon2Ratio;   //[nJets]
    Int_t*           muon2TrackQuality;   //[nJets]
    Float_t*         muon2RatioRel;   //[nJets]
    Bool_t*          muon3IsGlobal;   //[nJets]
    Bool_t*          muon3IsTracker;   //[nJets]
    Bool_t*          muon3IsStandalone;   //[nJets]
    Float_t*         muon3Pt;   //[nJets]
    Float_t*         muon3Eta;   //[nJets]
    Float_t*         muon3Phi;   //[nJets]
    Int_t*           muon3NumberOfMatches;   //[nJets]
    Int_t*           muon3GlobalMuonHits;   //[nJets]
    Int_t*           muon3InnerValidHits;   //[nJets]
    Int_t*           muon3NExpectedOuterHits;   //[nJets]
    Int_t*           muon3NPixelHits;   //[nJets]
    Float_t*         muon3InnerNChi2;   //[nJets]
    Float_t*         muon3GlobalNChi2;   //[nJets]
    Float_t*         muon3VzPVDist;   //[nJets]
    Float_t*         muon3PtRel;   //[nJets]
    Float_t*         muon3Sip2d;   //[nJets]
    Float_t*         muon3Ip2d;   //[nJets]
    Float_t*         muon3Ipe2d;   //[nJets]
    Float_t*         muon3Sip3d;   //[nJets]
    Float_t*         muon3Ip3d;   //[nJets]
    Float_t*         muon3Ipe3d;   //[nJets]
    Float_t*         muon3P0Par;   //[nJets]
    Float_t*         muon3DeltaR;   //[nJets]
    Float_t*         muon3EtaRel;   //[nJets]
    Float_t*         muon3Ratio;   //[nJets]
    Int_t*           muon3TrackQuality;   //[nJets]
    Float_t*         muon3RatioRel;   //[nJets]
    Bool_t*          muon4IsGlobal;   //[nJets]
    Bool_t*          muon4IsTracker;   //[nJets]
    Bool_t*          muon4IsStandalone;   //[nJets]
    Float_t*         muon4Pt;   //[nJets]
    Float_t*         muon4Eta;   //[nJets]
    Float_t*         muon4Phi;   //[nJets]
    Int_t*           muon4NumberOfMatches;   //[nJets]
    Int_t*           muon4GlobalMuonHits;   //[nJets]
    Int_t*           muon4InnerValidHits;   //[nJets]
    Int_t*           muon4NExpectedOuterHits;   //[nJets]
    Int_t*           muon4NPixelHits;   //[nJets]
    Float_t*         muon4InnerNChi2;   //[nJets]
    Float_t*         muon4GlobalNChi2;   //[nJets]
    Float_t*         muon4VzPVDist;   //[nJets]
    Float_t*         muon4PtRel;   //[nJets]
    Float_t*         muon4Sip2d;   //[nJets]
    Float_t*         muon4Ip2d;   //[nJets]
    Float_t*         muon4Ipe2d;   //[nJets]
    Float_t*         muon4Sip3d;   //[nJets]
    Float_t*         muon4Ip3d;   //[nJets]
    Float_t*         muon4Ipe3d;   //[nJets]
    Float_t*         muon4P0Par;   //[nJets]
    Float_t*         muon4DeltaR;   //[nJets]
    Float_t*         muon4EtaRel;   //[nJets]
    Float_t*         muon4Ratio;   //[nJets]
    Int_t*           muon4TrackQuality;   //[nJets]
    Float_t*         muon4RatioRel;   //[nJets]

    // List of branches
    TBranch        *b_triggerHLTJet30U;   //!
    TBranch        *b_isBGluonSplitting;   //!
    TBranch        *b_nJets;   //!
    TBranch        *b_jetPt;   //!
    TBranch        *b_jetEta;   //!
    TBranch        *b_MCTrueFlavor;   //!
    TBranch        *b_muon1IsGlobal;   //!
    TBranch        *b_muon1IsTracker;   //!
    TBranch        *b_muon1IsStandalone;   //!
    TBranch        *b_muon1Pt;   //!
    TBranch        *b_muon1Eta;   //!
    TBranch        *b_muon1Phi;   //!
    TBranch        *b_muon1NumberOfMatches;   //!
    TBranch        *b_muon1GlobalMuonHits;   //!
    TBranch        *b_muon1InnerValidHits;   //!
    TBranch        *b_muon1NExpectedOuterHits;   //!
    TBranch        *b_muon1NPixelHits;   //!
    TBranch        *b_muon1InnerNChi2;   //!
    TBranch        *b_muon1GlobalNChi2;   //!
    TBranch        *b_muon1VzPVDist;   //!
    TBranch        *b_muon1PtRel;   //!
    TBranch        *b_muon1Sip2d;   //!
    TBranch        *b_muon1Ip2d;   //!
    TBranch        *b_muon1Ipe2d;   //!
    TBranch        *b_muon1Sip3d;   //!
    TBranch        *b_muon1Ip3d;   //!
    TBranch        *b_muon1Ipe3d;   //!
    TBranch        *b_muon1P0Par;   //!
    TBranch        *b_muon1DeltaR;   //!
    TBranch        *b_muon1EtaRel;   //!
    TBranch        *b_muon1Ratio;   //!
    TBranch        *b_muon1TrackQuality;   //!
    TBranch        *b_muon1RatioRel;   //!
    TBranch        *b_muon2IsGlobal;   //!
    TBranch        *b_muon2IsTracker;   //!
    TBranch        *b_muon2IsStandalone;   //!
    TBranch        *b_muon2Pt;   //!
    TBranch        *b_muon2Eta;   //!
    TBranch        *b_muon2Phi;   //!
    TBranch        *b_muon2NumberOfMatches;   //!
    TBranch        *b_muon2GlobalMuonHits;   //!
    TBranch        *b_muon2InnerValidHits;   //!
    TBranch        *b_muon2NExpectedOuterHits;   //!
    TBranch        *b_muon2NPixelHits;   //!
    TBranch        *b_muon2InnerNChi2;   //!
    TBranch        *b_muon2GlobalNChi2;   //!
    TBranch        *b_muon2VzPVDist;   //!
    TBranch        *b_muon2PtRel;   //!
    TBranch        *b_muon2Sip2d;   //!
    TBranch        *b_muon2Ip2d;   //!
    TBranch        *b_muon2Ipe2d;   //!
    TBranch        *b_muon2Sip3d;   //!
    TBranch        *b_muon2Ip3d;   //!
    TBranch        *b_muon2Ipe3d;   //!
    TBranch        *b_muon2P0Par;   //!
    TBranch        *b_muon2DeltaR;   //!
    TBranch        *b_muon2EtaRel;   //!
    TBranch        *b_muon2Ratio;   //!
    TBranch        *b_muon2TrackQuality;   //!
    TBranch        *b_muon2RatioRel;   //!
    TBranch        *b_muon3IsGlobal;   //!
    TBranch        *b_muon3IsTracker;   //!
    TBranch        *b_muon3IsStandalone;   //!
    TBranch        *b_muon3Pt;   //!
    TBranch        *b_muon3Eta;   //!
    TBranch        *b_muon3Phi;   //!
    TBranch        *b_muon3NumberOfMatches;   //!
    TBranch        *b_muon3GlobalMuonHits;   //!
    TBranch        *b_muon3InnerValidHits;   //!
    TBranch        *b_muon3NExpectedOuterHits;   //!
    TBranch        *b_muon3NPixelHits;   //!
    TBranch        *b_muon3InnerNChi2;   //!
    TBranch        *b_muon3GlobalNChi2;   //!
    TBranch        *b_muon3VzPVDist;   //!
    TBranch        *b_muon3PtRel;   //!
    TBranch        *b_muon3Sip2d;   //!
    TBranch        *b_muon3Ip2d;   //!
    TBranch        *b_muon3Ipe2d;   //!
    TBranch        *b_muon3Sip3d;   //!
    TBranch        *b_muon3Ip3d;   //!
    TBranch        *b_muon3Ipe3d;   //!
    TBranch        *b_muon3P0Par;   //!
    TBranch        *b_muon3DeltaR;   //!
    TBranch        *b_muon3EtaRel;   //!
    TBranch        *b_muon3Ratio;   //!
    TBranch        *b_muon3TrackQuality;   //!
    TBranch        *b_muon3RatioRel;   //!
    TBranch        *b_muon4IsGlobal;   //!
    TBranch        *b_muon4IsTracker;   //!
    TBranch        *b_muon4IsStandalone;   //!
    TBranch        *b_muon4Pt;   //!
    TBranch        *b_muon4Eta;   //!
    TBranch        *b_muon4Phi;   //!
    TBranch        *b_muon4NumberOfMatches;   //!
    TBranch        *b_muon4GlobalMuonHits;   //!
    TBranch        *b_muon4InnerValidHits;   //!
    TBranch        *b_muon4NExpectedOuterHits;   //!
    TBranch        *b_muon4NPixelHits;   //!
    TBranch        *b_muon4InnerNChi2;   //!
    TBranch        *b_muon4GlobalNChi2;   //!
    TBranch        *b_muon4VzPVDist;   //!
    TBranch        *b_muon4PtRel;   //!
    TBranch        *b_muon4Sip2d;   //!
    TBranch        *b_muon4Ip2d;   //!
    TBranch        *b_muon4Ipe2d;   //!
    TBranch        *b_muon4Sip3d;   //!
    TBranch        *b_muon4Ip3d;   //!
    TBranch        *b_muon4Ipe3d;   //!
    TBranch        *b_muon4P0Par;   //!
    TBranch        *b_muon4DeltaR;   //!
    TBranch        *b_muon4EtaRel;   //!
    TBranch        *b_muon4Ratio;   //!
    TBranch        *b_muon4TrackQuality;   //!
    TBranch        *b_muon4RatioRel;   //!
};

#endif

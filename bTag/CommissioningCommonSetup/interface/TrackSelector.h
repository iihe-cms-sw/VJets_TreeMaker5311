//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 12 12:20:57 2010 by ROOT version 5.22/00d
// from TTree t/t
// found on file: /storage/5/jyothsna/btag_18Sep_QCDPt80/standardPFNtuple_1_2_pIs.root
//////////////////////////////////////////////////////////

#ifndef bTag_CommissioningCommonSetup_TrackSelector_h
#define bTag_CommissioningCommonSetup_TrackSelector_h

#include "bTag/CommissioningCommonSetup/interface/BaseSelector.h"
#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/InformationTrackCuts.h"

#include <TROOT.h>
#include <TFile.h>
#include <iostream>
#include "TH1D.h"

class TrackSelector : public BaseSelector {
  public :
   TrackSelector();
   TrackSelector(const InformationTrackCuts& info_);
   virtual ~TrackSelector() { }
   virtual void    Init(TTree *tree);
   virtual Bool_t  Process(Long64_t entry);
   inline FlavorHistogram getHistograms() const {return histograms;}
   inline InformationTrackCuts const getInfo() const {return info;}

  protected:
  // some objects
   InformationTrackCuts info;
   FlavorHistogram histograms;

   // Declaration of leaf types
   Bool_t*          triggerHLTJet15U;
   Bool_t*          triggerHLTJet30U;
   Bool_t*          isBGluonSplitting;
   Int_t*           nJets;
   Float_t*         jetPt;   //[nJets]
   Float_t*         jetEta;   //[nJets]
   Int_t*           nTracks;
   Int_t*           trackJetIndex;   //[nTracks]
   Bool_t*          trackSelected;   //[nTracks]
   Float_t*         trackTransverseMomentum;   //[nTracks]
   Float_t*         trackEta;   //[nTracks]
   Float_t*         trackPhi;   //[nTracks]
   Float_t*         trackMomentum;   //[nTracks]
   Int_t*           trackNHits;   //[nTracks]
   Int_t*           trackNPixelHits;   //[nTracks]
   Float_t*         trackChi2;   //[nTracks]
   Float_t*         trackNormChi2;   //[nTracks]
   Int_t*           trackQuality;   //[nTracks]
   Float_t*         trackLongitudinalImpactParameter;   //[nTracks]
   Float_t*         trackIP;   //[nTracks]
   Float_t*         trackDecayLength;   //[nTracks]
   Float_t*         trackDistJetAxis;   //[nTracks]
   Float_t*         trackDeltaR;   //[nTracks]
   Float_t*         trackIP3d;   //[nTracks]
   Float_t*         trackIP2d;   //[nTracks]
   Float_t*         trackIP3dError;   //[nTracks]
   Float_t*         trackIP2dError;   //[nTracks]
   Int_t*           trackHasSharedPix1;   //[nTracks]
   Int_t*           trackHasSharedPix2;   //[nTracks]
   Int_t*           trackHasSharedPix3;   //[nTracks]
   Int_t*           trackHasSharedPixAll;   //[nTracks]
   Int_t*           MCTrueFlavor;   //[nJets]

   // List of branches
   TBranch        *b_triggerHLTJet15U;   //!
   TBranch        *b_triggerHLTJet30U;   //!
   TBranch        *b_isBGluonSplitting;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_nTracks;   //!
   TBranch        *b_trackJetIndex;   //!
   TBranch        *b_trackSelected;   //!
   TBranch        *b_trackTransverseMomentum;   //!
   TBranch        *b_trackEta;   //!
   TBranch        *b_trackPhi;   //!
   TBranch        *b_trackMomentum;   //!
   TBranch        *b_trackNHits;   //!
   TBranch        *b_trackNPixelHits;   //!
   TBranch        *b_trackChi2;   //!
   TBranch        *b_trackNormChi2;   //!
   TBranch        *b_trackQuality;   //!
   TBranch        *b_trackLongitudinalImpactParameter;   //!
   TBranch        *b_trackIP;   //!
   TBranch        *b_trackDecayLength;   //!
   TBranch        *b_trackDistJetAxis;   //!
   TBranch        *b_trackDeltaR;   //!
   TBranch        *b_trackIP3d;   //!
   TBranch        *b_trackIP2d;   //!
   TBranch        *b_trackIP3dError;   //!
   TBranch        *b_trackIP2dError;   //!
   TBranch        *b_trackHasSharedPix1;   //!
   TBranch        *b_trackHasSharedPix2;   //!
   TBranch        *b_trackHasSharedPix3;   //!
   TBranch        *b_trackHasSharedPixAll;   //!
   TBranch        *b_MCTrueFlavor;   //!
};

#endif

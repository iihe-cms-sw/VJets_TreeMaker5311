// The class definition in TrackSelector.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("TrackSelector.C")
// Root > T->Process("TrackSelector.C","some options")
// Root > T->Process("TrackSelector.C+")
//

#include "bTag/CommissioningCommonSetup/interface/TrackSelector.h"
#include <TH2.h>
#include <TStyle.h>
#include <TH1D.h>
#include <TLeaf.h>
#include <cstdlib>
#include <cmath>

TrackSelector::TrackSelector() : BaseSelector() {}

TrackSelector::TrackSelector(const InformationTrackCuts& info_) : BaseSelector(), info(info_),
  histograms(info_.getPlotName(), info_.getPlotTitle(), info_.getNBinsX(), info_.getXLow(), info_.getXUp())
{
  histograms.setXTitle(info_.getXTitle());
  histograms.setYTitle(info_.getYTitle());
  histograms.setBScale(info_.getBScale());
  histograms.setCScale(info_.getCScale());
  histograms.sumW2();
}

void TrackSelector::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fChain->SetMakeClass(1);

   treeWeight = fChain->GetWeight();

   triggerHLTJet15U = SetPtrToValue<Bool_t>(b_triggerHLTJet15U, "triggerHLTJet15U");
   triggerHLTJet30U = SetPtrToValue<Bool_t>(b_triggerHLTJet30U, "triggerHLTJet30U");
   isBGluonSplitting = SetPtrToValue<Bool_t>(b_isBGluonSplitting, "isBGluonSplitting");
   nJets = SetPtrToValue<Int_t>(b_nJets, "nJets");
   jetPt = SetPtrToValue<Float_t>(b_jetPt, "jetPt");
   jetEta = SetPtrToValue<Float_t>(b_jetEta, "jetEta");
   nTracks = SetPtrToValue<Int_t>(b_nTracks, "nTracks");
   trackJetIndex = SetPtrToValue<Int_t>(b_trackJetIndex, "trackJetIndex");
   trackSelected = SetPtrToValue<Bool_t>(b_trackSelected, "trackSelected");
   trackTransverseMomentum = SetPtrToValue<Float_t>(b_trackTransverseMomentum, "trackTransverseMomentum");
   trackEta = SetPtrToValue<Float_t>(b_trackEta, "trackEta");
   trackPhi = SetPtrToValue<Float_t>(b_trackPhi, "trackPhi");
   trackMomentum = SetPtrToValue<Float_t>(b_trackMomentum, "trackMomentum");
   trackNHits = SetPtrToValue<Int_t>(b_trackNHits, "trackNHits");
   trackNPixelHits = SetPtrToValue<Int_t>(b_trackNPixelHits, "trackNPixelHits");
   trackChi2 = SetPtrToValue<Float_t>(b_trackChi2, "trackChi2");
   trackNormChi2 = SetPtrToValue<Float_t>(b_trackNormChi2, "trackNormChi2");
   trackQuality = SetPtrToValue<Int_t>(b_trackQuality, "trackQuality");
   trackLongitudinalImpactParameter = SetPtrToValue<Float_t>(b_trackLongitudinalImpactParameter, "trackLongitudinalImpactParameter");
   trackIP = SetPtrToValue<Float_t>(b_trackIP, "trackIP");
   trackDecayLength = SetPtrToValue<Float_t>(b_trackDecayLength, "trackDecayLength");
   trackDistJetAxis = SetPtrToValue<Float_t>(b_trackDistJetAxis, "trackDistJetAxis");
   trackDeltaR = SetPtrToValue<Float_t>(b_trackDeltaR, "trackDeltaR");
   trackIP3d = SetPtrToValue<Float_t>(b_trackIP3d, "trackIP3d");
   trackIP2d = SetPtrToValue<Float_t>(b_trackIP2d, "trackIP2d");
   trackIP3dError = SetPtrToValue<Float_t>(b_trackIP3dError, "trackIP3dError");
   trackIP2dError = SetPtrToValue<Float_t>(b_trackIP2dError, "trackIP2dError");
   trackHasSharedPix1 = SetPtrToValue<Int_t>(b_trackHasSharedPix1, "trackHasSharedPix1");
   trackHasSharedPix2 = SetPtrToValue<Int_t>(b_trackHasSharedPix2, "trackHasSharedPix2");
   trackHasSharedPix3 = SetPtrToValue<Int_t>(b_trackHasSharedPix3, "trackHasSharedPix3");
   trackHasSharedPixAll = SetPtrToValue<Int_t>(b_trackHasSharedPixAll, "trackHasSharedPixAll");
   MCTrueFlavor = SetPtrToValue<Int_t>(b_MCTrueFlavor, "MCTrueFlavor");
}

Bool_t TrackSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either TrackSelector::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

  b_isBGluonSplitting->GetEntry(entry);
  b_nJets -> GetEntry(entry);
  b_nTracks->GetEntry(entry);
  b_jetPt->GetEntry(entry);
  b_jetEta->GetEntry(entry);
  b_trackNHits->GetEntry(entry);
  b_triggerHLTJet30U->GetEntry(entry);  
  b_trackJetIndex->GetEntry(entry);
  b_trackNPixelHits->GetEntry(entry);
  b_trackNHits->GetEntry(entry);
  b_trackNormChi2->GetEntry(entry);
  b_trackTransverseMomentum->GetEntry(entry);
  b_trackDistJetAxis->GetEntry(entry);
  b_trackDecayLength->GetEntry(entry);
  b_trackIP2d->GetEntry(entry);
  b_trackLongitudinalImpactParameter->GetEntry(entry);

  b_MCTrueFlavor->GetEntry(entry);

  unsigned int sizeOfJets =    *nJets;
  unsigned int sizeOfTracks =  *nTracks;
 

  // only fill histograms if cuts are fulfilled
  
  if(*triggerHLTJet30U != info.getTriggerHLTJet30U()) return kTRUE;

  /// ITERATE OVER ALL JETS
  for(unsigned int i=0 ;i != sizeOfJets; ++i){
    if(jetPt[i] < info.getJetPtCut()) continue;
    if(fabs(jetEta[i]) > info.getJetEtaCut()) continue;

    unsigned int NtrackspassingCuts = 0;
    // now iterate over tracks of this jet and check cuts
    for(unsigned int j=0 ;j != sizeOfTracks; ++j){
      if(trackJetIndex[j]   != static_cast<int>(i))           continue;
      if(trackNPixelHits[j] <  static_cast<int>(info.getNPixHitsCut())) continue;
      if(trackNHits[j]      <  static_cast<int>(info.getNHitsCut()))    continue;
      if(trackNormChi2[j]     >  info.getChi2Cut())    continue;
      if(trackTransverseMomentum[j] < info.getTrackPtCut()) continue; 
      if(fabs(trackDistJetAxis[j]) > info.getDistJetAxisCut()) continue;
      if(trackDecayLength[j] > info.getDecayLengthCut()) continue; 
      if(fabs(trackIP2d[j]) > info.getIP2dCut()) continue;
      if(fabs(trackLongitudinalImpactParameter[j]) > info.getLongIPCut() ) continue;
      // passed all cuts, now count the track
      ++NtrackspassingCuts;
    }

    // fill number of tracks in histograms depending on jet flavour etc...
    histograms.fill(NtrackspassingCuts, treeWeight, isData, MCTrueFlavor[i], *isBGluonSplitting);

  } // END ITERATION OVER ALL JETS
  

   return kTRUE;
}

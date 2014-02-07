// The class definition in MuonSelector.h has been generated automatically
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
// Root > T->Process("MuonSelector.C")
// Root > T->Process("MuonSelector.C","some options")
// Root > T->Process("MuonSelector.C+")
//

#include <cmath>
#include <cstdlib>

#include "bTag/CommissioningCommonSetup/interface/MuonSelector.h"
#include <TH2.h>
#include <TStyle.h>

MuonSelector::MuonSelector() : BaseSelector() {}

MuonSelector::MuonSelector(const InformationMuonCuts& info_) : BaseSelector(), info(info_),
  histograms(info_.getPlotName(), info_.getPlotTitle(), info_.getNBinsX(), info_.getXLow(), info_.getXUp())
{
  histograms.setXTitle(info_.getXTitle());
  histograms.setYTitle(info_.getYTitle());
  histograms.setBScale(info_.getBScale());
  histograms.setCScale(info_.getCScale());
  histograms.sumW2();
}

void MuonSelector::Init(TTree *tree)
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

   triggerHLTJet30U = SetPtrToValue<Bool_t>(b_triggerHLTJet30U, "triggerHLTJet30U");
   isBGluonSplitting = SetPtrToValue<Bool_t>(b_isBGluonSplitting, "isBGluonSplitting");
   nJets = SetPtrToValue<Int_t>(b_nJets, "nJets");
   jetPt = SetPtrToValue<Float_t>(b_jetPt, "jetPt");
   jetEta = SetPtrToValue<Float_t>(b_jetEta, "jetEta");
   MCTrueFlavor = SetPtrToValue<Int_t>(b_MCTrueFlavor, "MCTrueFlavor");
   muon1IsGlobal = SetPtrToValue<Bool_t>(b_muon1IsGlobal, "muon1IsGlobal");
   muon1IsTracker = SetPtrToValue<Bool_t>(b_muon1IsTracker, "muon1IsTracker");
   muon1IsStandalone = SetPtrToValue<Bool_t>(b_muon1IsStandalone, "muon1IsStandalone");
   muon1Pt = SetPtrToValue<Float_t>(b_muon1Pt, "muon1Pt");
   muon1Eta = SetPtrToValue<Float_t>(b_muon1Eta, "muon1Eta");
   muon1Phi = SetPtrToValue<Float_t>(b_muon1Phi, "muon1Phi");
   muon1NumberOfMatches = SetPtrToValue<Int_t>(b_muon1NumberOfMatches, "muon1NumberOfMatches");
   muon1GlobalMuonHits = SetPtrToValue<Int_t>(b_muon1GlobalMuonHits, "muon1GlobalMuonHits");
   muon1InnerValidHits = SetPtrToValue<Int_t>(b_muon1InnerValidHits, "muon1InnerValidHits");
   muon1NExpectedOuterHits = SetPtrToValue<Int_t>(b_muon1NExpectedOuterHits, "muon1NExpectedOuterHits");
   muon1NPixelHits = SetPtrToValue<Int_t>(b_muon1NPixelHits, "muon1NPixelHits");
   muon1InnerNChi2 = SetPtrToValue<Float_t>(b_muon1InnerNChi2, "muon1InnerNChi2");
   muon1GlobalNChi2 = SetPtrToValue<Float_t>(b_muon1GlobalNChi2, "muon1GlobalNChi2");
   muon1VzPVDist = SetPtrToValue<Float_t>(b_muon1VzPVDist, "muon1VzPVDist");
   muon1PtRel = SetPtrToValue<Float_t>(b_muon1PtRel, "muon1PtRel");
   muon1Sip2d = SetPtrToValue<Float_t>(b_muon1Sip2d, "muon1Sip2d");
   muon1Ip2d = SetPtrToValue<Float_t>(b_muon1Ip2d, "muon1Ip2d");
   muon1Ipe2d = SetPtrToValue<Float_t>(b_muon1Ipe2d, "muon1Ipe2d");
   muon1Sip3d = SetPtrToValue<Float_t>(b_muon1Sip3d, "muon1Sip3d");
   muon1Ip3d = SetPtrToValue<Float_t>(b_muon1Ip3d, "muon1Ip3d");
   muon1Ipe3d = SetPtrToValue<Float_t>(b_muon1Ipe3d, "muon1Ipe3d");
   muon1P0Par = SetPtrToValue<Float_t>(b_muon1P0Par, "muon1P0Par");
   muon1DeltaR = SetPtrToValue<Float_t>(b_muon1DeltaR, "muon1DeltaR");
   muon1EtaRel = SetPtrToValue<Float_t>(b_muon1EtaRel, "muon1EtaRel");
   muon1Ratio = SetPtrToValue<Float_t>(b_muon1Ratio, "muon1Ratio");
   muon1TrackQuality = SetPtrToValue<Int_t>(b_muon1TrackQuality, "muon1TrackQuality");
   muon1RatioRel = SetPtrToValue<Float_t>(b_muon1RatioRel, "muon1RatioRel");
   muon2IsGlobal = SetPtrToValue<Bool_t>(b_muon2IsGlobal, "muon2IsGlobal");
   muon2IsTracker = SetPtrToValue<Bool_t>(b_muon2IsTracker, "muon2IsTracker");
   muon2IsStandalone = SetPtrToValue<Bool_t>(b_muon2IsStandalone, "muon2IsStandalone");
   muon2Pt = SetPtrToValue<Float_t>(b_muon2Pt, "muon2Pt");
   muon2Eta = SetPtrToValue<Float_t>(b_muon2Eta, "muon2Eta");
   muon2Phi = SetPtrToValue<Float_t>(b_muon2Phi, "muon2Phi");
   muon2NumberOfMatches = SetPtrToValue<Int_t>(b_muon2NumberOfMatches, "muon2NumberOfMatches");
   muon2GlobalMuonHits = SetPtrToValue<Int_t>(b_muon2GlobalMuonHits, "muon2GlobalMuonHits");
   muon2InnerValidHits = SetPtrToValue<Int_t>(b_muon2InnerValidHits, "muon2InnerValidHits");
   muon2NExpectedOuterHits = SetPtrToValue<Int_t>(b_muon2NExpectedOuterHits, "muon2NExpectedOuterHits");
   muon2NPixelHits = SetPtrToValue<Int_t>(b_muon2NPixelHits, "muon2NPixelHits");
   muon2InnerNChi2 = SetPtrToValue<Float_t>(b_muon2InnerNChi2, "muon2InnerNChi2");
   muon2GlobalNChi2 = SetPtrToValue<Float_t>(b_muon2GlobalNChi2, "muon2GlobalNChi2");
   muon2VzPVDist = SetPtrToValue<Float_t>(b_muon2VzPVDist, "muon2VzPVDist");
   muon2PtRel = SetPtrToValue<Float_t>(b_muon2PtRel, "muon2PtRel");
   muon2Sip2d = SetPtrToValue<Float_t>(b_muon2Sip2d, "muon2Sip2d");
   muon2Ip2d = SetPtrToValue<Float_t>(b_muon2Ip2d, "muon2Ip2d");
   muon2Ipe2d = SetPtrToValue<Float_t>(b_muon2Ipe2d, "muon2Ipe2d");
   muon2Sip3d = SetPtrToValue<Float_t>(b_muon2Sip3d, "muon2Sip3d");
   muon2Ip3d = SetPtrToValue<Float_t>(b_muon2Ip3d, "muon2Ip3d");
   muon2Ipe3d = SetPtrToValue<Float_t>(b_muon2Ipe3d, "muon2Ipe3d");
   muon2P0Par = SetPtrToValue<Float_t>(b_muon2P0Par, "muon2P0Par");
   muon2DeltaR = SetPtrToValue<Float_t>(b_muon2DeltaR, "muon2DeltaR");
   muon2EtaRel = SetPtrToValue<Float_t>(b_muon2EtaRel, "muon2EtaRel");
   muon2Ratio = SetPtrToValue<Float_t>(b_muon2Ratio, "muon2Ratio");
   muon2TrackQuality = SetPtrToValue<Int_t>(b_muon2TrackQuality, "muon2TrackQuality");
   muon2RatioRel = SetPtrToValue<Float_t>(b_muon2RatioRel, "muon2RatioRel");
   muon3IsGlobal = SetPtrToValue<Bool_t>(b_muon3IsGlobal, "muon3IsGlobal");
   muon3IsTracker = SetPtrToValue<Bool_t>(b_muon3IsTracker, "muon3IsTracker");
   muon3IsStandalone = SetPtrToValue<Bool_t>(b_muon3IsStandalone, "muon3IsStandalone");
   muon3Pt = SetPtrToValue<Float_t>(b_muon3Pt, "muon3Pt");
   muon3Eta = SetPtrToValue<Float_t>(b_muon3Eta, "muon3Eta");
   muon3Phi = SetPtrToValue<Float_t>(b_muon3Phi, "muon3Phi");
   muon3NumberOfMatches = SetPtrToValue<Int_t>(b_muon3NumberOfMatches, "muon3NumberOfMatches");
   muon3GlobalMuonHits = SetPtrToValue<Int_t>(b_muon3GlobalMuonHits, "muon3GlobalMuonHits");
   muon3InnerValidHits = SetPtrToValue<Int_t>(b_muon3InnerValidHits, "muon3InnerValidHits");
   muon3NExpectedOuterHits = SetPtrToValue<Int_t>(b_muon3NExpectedOuterHits, "muon3NExpectedOuterHits");
   muon3NPixelHits = SetPtrToValue<Int_t>(b_muon3NPixelHits, "muon3NPixelHits");
   muon3InnerNChi2 = SetPtrToValue<Float_t>(b_muon3InnerNChi2, "muon3InnerNChi2");
   muon3GlobalNChi2 = SetPtrToValue<Float_t>(b_muon3GlobalNChi2, "muon3GlobalNChi2");
   muon3VzPVDist = SetPtrToValue<Float_t>(b_muon3VzPVDist, "muon3VzPVDist");
   muon3PtRel = SetPtrToValue<Float_t>(b_muon3PtRel, "muon3PtRel");
   muon3Sip2d = SetPtrToValue<Float_t>(b_muon3Sip2d, "muon3Sip2d");
   muon3Ip2d = SetPtrToValue<Float_t>(b_muon3Ip2d, "muon3Ip2d");
   muon3Ipe2d = SetPtrToValue<Float_t>(b_muon3Ipe2d, "muon3Ipe2d");
   muon3Sip3d = SetPtrToValue<Float_t>(b_muon3Sip3d, "muon3Sip3d");
   muon3Ip3d = SetPtrToValue<Float_t>(b_muon3Ip3d, "muon3Ip3d");
   muon3Ipe3d = SetPtrToValue<Float_t>(b_muon3Ipe3d, "muon3Ipe3d");
   muon3P0Par = SetPtrToValue<Float_t>(b_muon3P0Par, "muon3P0Par");
   muon3DeltaR = SetPtrToValue<Float_t>(b_muon3DeltaR, "muon3DeltaR");
   muon3EtaRel = SetPtrToValue<Float_t>(b_muon3EtaRel, "muon3EtaRel");
   muon3Ratio = SetPtrToValue<Float_t>(b_muon3Ratio, "muon3Ratio");
   muon3TrackQuality = SetPtrToValue<Int_t>(b_muon3TrackQuality, "muon3TrackQuality");
   muon3RatioRel = SetPtrToValue<Float_t>(b_muon3RatioRel, "muon3RatioRel");
   muon4IsGlobal = SetPtrToValue<Bool_t>(b_muon4IsGlobal, "muon4IsGlobal");
   muon4IsTracker = SetPtrToValue<Bool_t>(b_muon4IsTracker, "muon4IsTracker");
   muon4IsStandalone = SetPtrToValue<Bool_t>(b_muon4IsStandalone, "muon4IsStandalone");
   muon4Pt = SetPtrToValue<Float_t>(b_muon4Pt, "muon4Pt");
   muon4Eta = SetPtrToValue<Float_t>(b_muon4Eta, "muon4Eta");
   muon4Phi = SetPtrToValue<Float_t>(b_muon4Phi, "muon4Phi");
   muon4NumberOfMatches = SetPtrToValue<Int_t>(b_muon4NumberOfMatches, "muon4NumberOfMatches");
   muon4GlobalMuonHits = SetPtrToValue<Int_t>(b_muon4GlobalMuonHits, "muon4GlobalMuonHits");
   muon4InnerValidHits = SetPtrToValue<Int_t>(b_muon4InnerValidHits, "muon4InnerValidHits");
   muon4NExpectedOuterHits = SetPtrToValue<Int_t>(b_muon4NExpectedOuterHits, "muon4NExpectedOuterHits");
   muon4NPixelHits = SetPtrToValue<Int_t>(b_muon4NPixelHits, "muon4NPixelHits");
   muon4InnerNChi2 = SetPtrToValue<Float_t>(b_muon4InnerNChi2, "muon4InnerNChi2");
   muon4GlobalNChi2 = SetPtrToValue<Float_t>(b_muon4GlobalNChi2, "muon4GlobalNChi2");
   muon4VzPVDist = SetPtrToValue<Float_t>(b_muon4VzPVDist, "muon4VzPVDist");
   muon4PtRel = SetPtrToValue<Float_t>(b_muon4PtRel, "muon4PtRel");
   muon4Sip2d = SetPtrToValue<Float_t>(b_muon4Sip2d, "muon4Sip2d");
   muon4Ip2d = SetPtrToValue<Float_t>(b_muon4Ip2d, "muon4Ip2d");
   muon4Ipe2d = SetPtrToValue<Float_t>(b_muon4Ipe2d, "muon4Ipe2d");
   muon4Sip3d = SetPtrToValue<Float_t>(b_muon4Sip3d, "muon4Sip3d");
   muon4Ip3d = SetPtrToValue<Float_t>(b_muon4Ip3d, "muon4Ip3d");
   muon4Ipe3d = SetPtrToValue<Float_t>(b_muon4Ipe3d, "muon4Ipe3d");
   muon4P0Par = SetPtrToValue<Float_t>(b_muon4P0Par, "muon4P0Par");
   muon4DeltaR = SetPtrToValue<Float_t>(b_muon4DeltaR, "muon4DeltaR");
   muon4EtaRel = SetPtrToValue<Float_t>(b_muon4EtaRel, "muon4EtaRel");
   muon4Ratio = SetPtrToValue<Float_t>(b_muon4Ratio, "muon4Ratio");
   muon4TrackQuality = SetPtrToValue<Int_t>(b_muon4TrackQuality, "muon4TrackQuality");
   muon4RatioRel = SetPtrToValue<Float_t>(b_muon4RatioRel, "muon4RatioRel");
}

Bool_t MuonSelector::Process(Long64_t entry)
{
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MuonSelector::GetEntry() or TBranch::GetEntry()
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

  b_triggerHLTJet30U->GetEntry(entry);
  b_isBGluonSplitting->GetEntry(entry);

  b_nJets -> GetEntry(entry);
  b_jetPt->GetEntry(entry);
  b_jetEta->GetEntry(entry);

  b_muon1Pt->GetEntry(entry);
  b_muon1Eta->GetEntry(entry);
  b_muon1IsGlobal->GetEntry(entry);
  b_muon1GlobalMuonHits->GetEntry(entry);
  b_muon1NumberOfMatches->GetEntry(entry);
  b_muon1InnerValidHits->GetEntry(entry);
  b_muon1NPixelHits->GetEntry(entry); 
  b_muon1NExpectedOuterHits->GetEntry(entry);
  b_muon1InnerNChi2->GetEntry(entry);
  b_muon1GlobalNChi2->GetEntry(entry); 
  b_muon1VzPVDist->GetEntry(entry);
  b_muon1DeltaR->GetEntry(entry);

  b_muon2Pt->GetEntry(entry);
  b_muon2Eta->GetEntry(entry);
  b_muon2IsGlobal->GetEntry(entry);
  b_muon2GlobalMuonHits->GetEntry(entry);
  b_muon2NumberOfMatches->GetEntry(entry);
  b_muon2InnerValidHits->GetEntry(entry);
  b_muon2NPixelHits->GetEntry(entry); 
  b_muon2NExpectedOuterHits->GetEntry(entry);
  b_muon2InnerNChi2->GetEntry(entry);
  b_muon2GlobalNChi2->GetEntry(entry); 
  b_muon2VzPVDist->GetEntry(entry);
  b_muon2DeltaR->GetEntry(entry);

  b_muon3Pt->GetEntry(entry);
  b_muon3Eta->GetEntry(entry);
  b_muon3IsGlobal->GetEntry(entry);
  b_muon3GlobalMuonHits->GetEntry(entry);
  b_muon3NumberOfMatches->GetEntry(entry);
  b_muon3InnerValidHits->GetEntry(entry);
  b_muon3NPixelHits->GetEntry(entry); 
  b_muon3NExpectedOuterHits->GetEntry(entry);
  b_muon3InnerNChi2->GetEntry(entry);
  b_muon3GlobalNChi2->GetEntry(entry); 
  b_muon3VzPVDist->GetEntry(entry);
  b_muon3DeltaR->GetEntry(entry);

  b_muon4Pt->GetEntry(entry);
  b_muon4Eta->GetEntry(entry);
  b_muon4IsGlobal->GetEntry(entry);
  b_muon4GlobalMuonHits->GetEntry(entry);
  b_muon4NumberOfMatches->GetEntry(entry);
  b_muon4InnerValidHits->GetEntry(entry);
  b_muon4NPixelHits->GetEntry(entry); 
  b_muon4NExpectedOuterHits->GetEntry(entry);
  b_muon4InnerNChi2->GetEntry(entry);
  b_muon4GlobalNChi2->GetEntry(entry); 
  b_muon4VzPVDist->GetEntry(entry);
  b_muon4DeltaR->GetEntry(entry);

  if(!isData) b_MCTrueFlavor->GetEntry(entry);

  unsigned int sizeOfJets = *nJets;

   // only fill histograms if cuts are fulfilled
  

  if(*triggerHLTJet30U != info.getTriggerHLTJet30U()) return kTRUE;
  

  ////////////////////////////////////////////////////////////
  /// ITERATE OVER ALL JETS
  for(unsigned int i=0 ;i != sizeOfJets; ++i){//loop over jets
    

    if(jetPt[i] < info.getJetPtCut()) continue;
    if(fabs(jetEta[i]) > info.getJetEtaCut()) continue;
    

    /*
      muon1Pt > 5.0 && TMath::Abs(muon1Eta) < 2.4 && muon1IsGlobal == 1 
      && muon1GlobalMuonHits > 0 && muon1NumberOfMatches > 1 
      && muon1InnerValidHits > 10 && muon1NPixelHits > 1 
      && muon1NExpectedOuterHits < 3 && muon1InnerNChi2 < 10 
      && muon1GlobalNChi2 < 10 && muon1VzPVDist < 2 && muon1DeltaR < 0.4
    */

    unsigned int Nmupassingcuts = 0;
    
    ///////////////////////////////////////
    //Count separately for the muons ordered in pT, muon1,2,3 and 4
    //muon1

    
    if (  (muon1Pt[i] > info.getMuonPtCut()) &&
    (fabs(muon1Eta[i]) < info.getMuonEtaCut()) &&
    (muon1IsGlobal[i] == info.getMuonIsGlobal()) &&
    (muon1GlobalMuonHits[i] > info.getMuonGlobalMuonHits()) &&
    (muon1NumberOfMatches[i] > info.getMuonNumberOfMatches()) &&
    (muon1InnerValidHits[i] > info.getMuonInnerValidHits()) &&
    (muon1NPixelHits[i] > info.getMuonNPixelHits()) &&
    (muon1NExpectedOuterHits[i] < info.getMuonNExpectedOuterHits()) &&
    (muon1InnerNChi2[i] < info.getMuonInnerNChi2()) &&
    (muon1GlobalNChi2[i] < info.getMuonGlobalNChi2()) &&
      (fabs(muon1VzPVDist[i]) < info.getMuonVzPVDist()) &&
	  (muon1DeltaR[i] < info.getMuonDeltaR()) ) ++Nmupassingcuts;
    
   if (  (muon2Pt[i] > info.getMuonPtCut()) &&
    (fabs(muon2Eta[i]) < info.getMuonEtaCut()) &&
    (muon2IsGlobal[i] == info.getMuonIsGlobal()) &&
    (muon2GlobalMuonHits[i] > info.getMuonGlobalMuonHits()) &&
    (muon2NumberOfMatches[i] > info.getMuonNumberOfMatches()) &&
    (muon2InnerValidHits[i] > info.getMuonInnerValidHits()) &&
    (muon2NPixelHits[i] > info.getMuonNPixelHits()) &&
    (muon2NExpectedOuterHits[i] < info.getMuonNExpectedOuterHits()) &&
    (muon2InnerNChi2[i] < info.getMuonInnerNChi2()) &&
    (muon2GlobalNChi2[i] < info.getMuonGlobalNChi2()) &&
      (fabs(muon2VzPVDist[i]) < info.getMuonVzPVDist()) &&
	  (muon2DeltaR[i] < info.getMuonDeltaR()) ) ++Nmupassingcuts; 

   if (  (muon3Pt[i] > info.getMuonPtCut()) &&
    (fabs(muon3Eta[i]) < info.getMuonEtaCut()) &&
    (muon3IsGlobal[i] == info.getMuonIsGlobal()) &&
    (muon3GlobalMuonHits[i] > info.getMuonGlobalMuonHits()) &&
    (muon3NumberOfMatches[i] > info.getMuonNumberOfMatches()) &&
    (muon3InnerValidHits[i] > info.getMuonInnerValidHits()) &&
    (muon3NPixelHits[i] > info.getMuonNPixelHits()) &&
    (muon3NExpectedOuterHits[i] < info.getMuonNExpectedOuterHits()) &&
    (muon3InnerNChi2[i] < info.getMuonInnerNChi2()) &&
    (muon3GlobalNChi2[i] < info.getMuonGlobalNChi2()) &&
      (fabs(muon3VzPVDist[i]) < info.getMuonVzPVDist()) &&
	  (muon3DeltaR[i] < info.getMuonDeltaR()) ) ++Nmupassingcuts;

   if (  (muon4Pt[i] > info.getMuonPtCut()) &&
    (fabs(muon4Eta[i]) < info.getMuonEtaCut()) &&
    (muon4IsGlobal[i] == info.getMuonIsGlobal()) &&
    (muon4GlobalMuonHits[i] > info.getMuonGlobalMuonHits()) &&
    (muon4NumberOfMatches[i] > info.getMuonNumberOfMatches()) &&
    (muon4InnerValidHits[i] > info.getMuonInnerValidHits()) &&
    (muon4NPixelHits[i] > info.getMuonNPixelHits()) &&
    (muon4NExpectedOuterHits[i] < info.getMuonNExpectedOuterHits()) &&
    (muon4InnerNChi2[i] < info.getMuonInnerNChi2()) &&
    (muon4GlobalNChi2[i] < info.getMuonGlobalNChi2()) &&
      (fabs(muon4VzPVDist[i]) < info.getMuonVzPVDist()) &&
	  (muon4DeltaR[i] < info.getMuonDeltaR()) ) ++Nmupassingcuts;

    ///////////////////////////////////////
  
  
    // fill number of muons in histograms depending on jet flavour etc...
    histograms.fill(Nmupassingcuts, treeWeight, isData, MCTrueFlavor[i], *isBGluonSplitting);

  }//loop over jets

   return kTRUE;
}

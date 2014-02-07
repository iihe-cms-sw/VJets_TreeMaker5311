//#define GetPUFromEarlyThan_4_1_2
//#define GetPUFromEarlyThan_4_4_0
// system include files
#include <memory>
#include <vector>
#include <deque>
#include <map>
#include <string>
#include <cmath>
#include <sstream>


// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/Common/interface/TriggerResults.h"
//#include "FWCore/Framework/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/BTauReco/interface/SoftLeptonTagInfo.h"
#include "DataFormats/BTauReco/interface/SecondaryVertexTagInfo.h"
#include "DataFormats/BTauReco/interface/TrackIPTagInfo.h"
#include "DataFormats/BTauReco/interface/JetTag.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetCollection.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/Measurement1D.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include <DataFormats/EgammaCandidates/interface/GsfElectron.h>
//#include "DataFormats/EgammaCandidates/interface/ElectronTrackLinks.h"
#include <DataFormats/EgammaCandidates/interface/GsfElectronFwd.h>

#include "DataFormats/Math/interface/deltaR.h"
#include "Math/GenVector/VectorUtil.h"
#include "DataFormats/GeometryVector/interface/VectorUtil.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertToFromReco.h"
//Generation Information Headers
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
//Simulation Information Headers
#include "SimDataFormats/Track/interface/SimTrack.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertex.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include <DataFormats/GsfTrackReco/interface/GsfTrack.h>
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/PatternTools/interface/TSCBLBuilderNoMaterial.h"

#include "RecoBTau/JetTagComputer/interface/GenericMVAJetTagComputer.h"
#include "RecoBTau/JetTagComputer/interface/GenericMVAJetTagComputerWrapper.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputerRecord.h"
#include "RecoBTag/SecondaryVertex/interface/CombinedSVComputer.h"
#include "RecoBTag/SecondaryVertex/interface/TrackKinematics.h"

#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
//added by Caroline
#include "DataFormats/GeometrySurface/interface/Line.h"
//end add

#include <TTree.h>
#include <TFile.h>
#include <utility>

using namespace edm;
using namespace std;
using namespace reco;
using namespace ROOT::Math::VectorUtil;

// maximum array size (the arrays are stored with variable size in the root tree)
const UInt_t MAXJETS = 100;
const UInt_t MAXTRACKS = 4000;


class TagNtupleProducer : public edm::EDAnalyzer {

public:
  explicit TagNtupleProducer(const edm::ParameterSet&);
  ~TagNtupleProducer();
  
  
  virtual void beginJob() ;
  virtual void beginRun(const edm::Run&, const edm::EventSetup&);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endRun(const edm::Run&, const edm::EventSetup&);
  virtual void endJob() ;
  std::pair<Bool_t,reco::TrackBase::ParameterVector> parametersAtClosestApproach(Basic3DVector<Double_t> vertex,
									       Basic3DVector<Double_t> momAtVtx,
									       Float_t charge,
									       const reco::BeamSpot& bs) const;
  void getSharedHitsInfo(UInt_t layer, const reco::TrackRefVector & tracks, Int_t &nSharedHits, Int_t &nTotalHits);
  Bool_t GenParticleParentTree( HepMC::GenParticle *genPar );
  Bool_t hasSharedHit(UInt_t layer, track_iterator, const reco::TrackRefVector & tracks);      
  // ----------member data ---------------------------
  
  //Input from python config
  Bool_t getMCTruth_;
  Bool_t getSimTruth_;
  Bool_t getMCPUInfo_;
  Bool_t getSharedHitInfo_;
  Bool_t firstEvent_;
  edm::InputTag HepMCTag_;
  edm::InputTag simTruthTag_;
  edm::InputTag BeamSpotTag_;
  edm::InputTag jet_src_;
  edm::InputTag SVComputer_;
  edm::InputTag triggerTag_;
  edm::InputTag jet_MC_src_;
  edm::InputTag GenJet_Matcher_;
  edm::InputTag jet_tracks_;
  edm::InputTag primaryVertexProducer_;
  edm::InputTag SV_tag_infos_;
  edm::InputTag IP_tag_infos_;  
  edm::InputTag electron_tag_infos_;
  edm::InputTag muon_tag_infos_;
  std::string label_;
  std::string filename_;
  std::vector< edm::ParameterSet > bTag_Config_;
  Double_t chi2cut_;
  
  // data formats
  TTree *EventInfo,*RunInfo;
  TFile *file;

  UInt_t eventNumber ;
  UInt_t runNumber ;
  UInt_t lumiBlockNumber ;

  //added by Caroline,Jinzhong for TriggerNames and prescales
  HLTConfigProvider  hltConfigProvider_;
  //Basic Trigger Information
  std::vector<std::string> TriggerNames_,*HLTNamesSet;
  std::vector<Int_t> HLTTableBitMatch_;
  std::string *HLTTableName;
  std::vector<UInt_t> *HLTPrescaleFactors;
  std::vector<Bool_t> *HLTriggerResults;
  
  UInt_t numberOfPUVertices; // the number of pileup interactions that have been added to the event from BX=0
  Float_t numberOfPUVerticesMixingTruth;// the "MixingTruth" mean number of pileup interactions for this event from which each bunch crossing has been sampled; same for all bunch crossings in an event (before possion smearing); in fact BX=-1, this value is zero, it's a bug I believe.
  UInt_t numberOfPUVerticesTot;  // all PX : BX=0 and out-of-time PU
  UInt_t numberOfPrimaryVertices ;
  UInt_t numberOfTracksAtPV;
  Float_t PVx;
  Float_t PVy;
  Float_t PVz;
  Float_t PVChi2;
  Float_t PVndof;
  Float_t PVNormalizedChi2;

  Float_t pthat;
  Float_t mcweight;
  Bool_t isBGluonSplitting;
  Bool_t isCGluonSplitting;

  //Basic Jet Information
  Int_t nJets;

  math::XYZTLorentzVector jetP4[MAXJETS];
  Float_t jetPt[MAXJETS];                                 
  Float_t jetEta[MAXJETS];                                
  Float_t jetPhi[MAXJETS];                                
  Float_t jetEMFraction[MAXJETS];        
  Float_t jetChargedEmEnergyFraction[MAXJETS];
  Float_t jetNeutralEmEnergyFraction[MAXJETS];
  Float_t jetChargedHadronEnergyFraction[MAXJETS];
  Float_t jetNeutralHadronEnergyFraction[MAXJETS];
  Float_t jetChargedMultiplicity[MAXJETS];
  Float_t jetMass[MAXJETS];                               
  Int_t jetnConstituents[MAXJETS];  
  Int_t jetnTracks[MAXJETS];                           
  //  Int_t jetnElectrons[MAXJETS];                              
  //  Int_t jetnMuons[MAXJETS];                                  
  math::XYZVector jetVertex[MAXJETS];                   
  Float_t jetVertexChi2[MAXJETS];               
  Float_t jetVertexChi2Ndof[MAXJETS];           
  Float_t jetVertexNormalizedChi2[MAXJETS];    

  //Track Information
  Int_t nTracks;

  Int_t trackJetIndex[MAXTRACKS];
  Bool_t trackSelected[MAXTRACKS];
  math::XYZVector track3Momentum[MAXTRACKS];
  Float_t trackTransverseMomentum[MAXTRACKS];
  Float_t trackEta[MAXTRACKS];
  Float_t trackPhi[MAXTRACKS];
  Float_t trackMomentum[MAXTRACKS];
  Int_t trackNHits[MAXTRACKS];
  Int_t trackNPixelHits[MAXTRACKS];
  Float_t trackChi2[MAXTRACKS];
  Float_t trackNormChi2[MAXTRACKS];
  Int_t trackQuality[MAXTRACKS];
  Float_t trackLongitudinalImpactParameter[MAXTRACKS];
  Float_t trackIP[MAXTRACKS];
  Float_t trackDecayLength[MAXTRACKS];
  Float_t trackDistJetAxis[MAXTRACKS];
  Float_t trackDeltaR[MAXTRACKS];
  Float_t trackIP3d[MAXTRACKS];
  Float_t trackIP3dError[MAXTRACKS];
  Float_t trackIP3dProbability[MAXTRACKS];
  Float_t trackIP2d[MAXTRACKS];
  Float_t trackIP2dError[MAXTRACKS];
  Float_t trackIP2dProbability[MAXTRACKS];
  Float_t trackPtRel[MAXTRACKS];
  Bool_t trackHasSharedPix1[MAXTRACKS];
  Bool_t trackHasSharedPix2[MAXTRACKS];
  Bool_t trackHasSharedPix3[MAXTRACKS];
  Bool_t trackHasSharedPixAll[MAXTRACKS];
  Bool_t trackIsVertexTrack[MAXTRACKS];
  Bool_t trackIsFromB[MAXTRACKS];
  Bool_t trackIsFromPU[MAXTRACKS];

  //MC Truth Information
  Int_t PartonFlavor[MAXJETS];//PartonFlavor==0 means that the jet matches no GEN level parton.It could be a piled-up jet or bad reconstructed jet.One see the algorithms in: http://cmslxr.fnal.gov/lxr/source/PhysicsTools/JetMCAlgos/plugins/JetFlavourIdentifier.cc and http://cmslxr.fnal.gov/lxr/source/PhysicsTools/JetMCAlgos/plugins/JetPartonMatcher.cc                
  Float_t PartonPt[MAXJETS];
  Float_t PartonEta[MAXJETS];
  Float_t PartonPhi[MAXJETS];
  Float_t GenJetPt[MAXJETS];
  Float_t GenJetEta[MAXJETS];
  Float_t GenJetPhi[MAXJETS];
 				       
  //secondaryVertexTagInfos:			       
  math::XYZVector SecondaryVertex[MAXJETS];             
  Float_t SV3dDistance[MAXJETS];                          
  Float_t SV3dDistanceError[MAXJETS];                     
  Float_t SV2dDistance[MAXJETS];                          
  Float_t SV2dDistanceError[MAXJETS];                     
  Float_t SVChi2[MAXJETS];                
  Float_t SVIPFirstAboveCharm[MAXJETS];                                
  Float_t SVDegreesOfFreedom[MAXJETS];                    
  Float_t SVNormChi2[MAXJETS];                            
  Int_t SVnSelectedTracks[MAXJETS];                               
  Float_t SVMass[MAXJETS];                                
  Float_t SVEnergyRatio[MAXJETS];                                
  Int_t SVnVertices[MAXJETS];                             
  Int_t   SVnVertexTracks[MAXJETS];         
  Int_t   SVnVertexTracksAll[MAXJETS];         
  Int_t   SVnFirstVertexTracks[MAXJETS];         
  Int_t   SVnFirstVertexTracksAll[MAXJETS];         
  Float_t SVjetDeltaR[MAXJETS];
  Float_t SVvtxSumVtxDirDeltaR[MAXJETS];
  Float_t SVvtxSumJetDeltaR[MAXJETS];
  Float_t SVvtxPt[MAXJETS];
  Float_t SVvtxSumDirEta[MAXJETS];
  Float_t SVvtxSumDirPhi[MAXJETS];
  Float_t SVvtxDirEta[MAXJETS];
  Float_t SVvtxDirPhi[MAXJETS];
  // added by Caroline
  Float_t SVvtxSumPTrel[MAXJETS];
  Float_t SVvtxDirPTrel[MAXJETS];
  Float_t SVvtxDistJetAxis[MAXJETS];
  Int_t   SVtotCharge[MAXJETS];
  // end add
               
  //impactParameterTagInfos
  Int_t IPnSelectedTracks[MAXJETS];                       
  Int_t IPnSelectedAndDecayLengthAndJetAsixTracks[MAXJETS];
  math::XYZVector IPghostTrackP3[MAXJETS];      
  Float_t IPghostTrackPt[MAXJETS];                        
  Float_t IPghostTrackPtRel[MAXJETS];                        
  Float_t IPghostTrackEta[MAXJETS];                       
  Float_t IPghostTrackPhi[MAXJETS];                       
  Float_t IPghostTrackDeltaR[MAXJETS];        
  Int_t   IPPix1SharedHits[MAXJETS];
  Int_t   IPPix1TotalHits[MAXJETS];
  Int_t   IPPix2SharedHits[MAXJETS];
  Int_t   IPPix2TotalHits[MAXJETS];
  Int_t   IPPix3SharedHits[MAXJETS];
  Int_t   IPPix3TotalHits[MAXJETS];
  Int_t   IPPixAllSharedHits[MAXJETS];
  Int_t   IPPixAllTotalHits[MAXJETS];

  //softElectronTagInfos
  Int_t nElectrons[MAXJETS]; 
                           
  Float_t electron1Pt[MAXJETS];   			       
  Float_t electron1Eta[MAXJETS];  			       
  Float_t electron1Phi[MAXJETS];  			       
  Int_t electron1NHits[MAXJETS];   		     
  Int_t electron1NExpectedOuterHits[MAXJETS];   		       
  Int_t electron1NPixelHits[MAXJETS];   		       
  Float_t electron1NChi2[MAXJETS];			       
  Float_t electron1PtRel[MAXJETS];   		       
  Float_t electron1Sip2d[MAXJETS];   		       
  Float_t electron1Ip2d[MAXJETS];   		       
  Float_t electron1Ipe2d[MAXJETS];   		       
  Float_t electron1Sip3d[MAXJETS];
  Float_t electron1Ip3d[MAXJETS];
  Float_t electron1Ipe3d[MAXJETS];   		          		          		       
  Float_t electron1P0Par[MAXJETS];   		       
  Float_t electron1DeltaR[MAXJETS];  		       
  Float_t electron1EtaRel[MAXJETS];  		       
  Float_t electron1Ratio[MAXJETS];   		     
  Int_t electron1TrackQuality[MAXJETS];   		       
  Float_t electron1RatioRel[MAXJETS];  
  Float_t electron1eSuperClusterOverP[MAXJETS];
  Float_t electron1eSeedClusterOverP[MAXJETS];
  Float_t electron1eSeedClusterOverPout[MAXJETS];
  Float_t electron1eEleClusterOverPout[MAXJETS];
  Float_t electron1deltaEtaSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron1deltaEtaSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron1deltaEtaEleClusterTrackAtCalo[MAXJETS];
  Float_t electron1deltaPhiSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron1deltaPhiSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron1deltaPhiEleClusterTrackAtCalo[MAXJETS];                  

  Float_t electron2Pt[MAXJETS];   			       
  Float_t electron2Eta[MAXJETS];  			       
  Float_t electron2Phi[MAXJETS];  			       
  Int_t electron2NHits[MAXJETS];   		     
  Int_t electron2NExpectedOuterHits[MAXJETS];   		       
  Int_t electron2NPixelHits[MAXJETS];   		       
  Float_t electron2NChi2[MAXJETS];			       
  Float_t electron2PtRel[MAXJETS];   		       
  Float_t electron2Sip2d[MAXJETS];   		       
  Float_t electron2Ip2d[MAXJETS];   		       
  Float_t electron2Ipe2d[MAXJETS];   		       
  Float_t electron2Sip3d[MAXJETS];
  Float_t electron2Ip3d[MAXJETS];
  Float_t electron2Ipe3d[MAXJETS];   		          		          		       
  Float_t electron2P0Par[MAXJETS];   		       
  Float_t electron2DeltaR[MAXJETS];  		       
  Float_t electron2EtaRel[MAXJETS];  		       
  Float_t electron2Ratio[MAXJETS];   		     
  Int_t electron2TrackQuality[MAXJETS];   		       
  Float_t electron2RatioRel[MAXJETS];
  Float_t electron2eSuperClusterOverP[MAXJETS];
  Float_t electron2eSeedClusterOverP[MAXJETS];
  Float_t electron2eSeedClusterOverPout[MAXJETS];
  Float_t electron2eEleClusterOverPout[MAXJETS];
  Float_t electron2deltaEtaSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron2deltaEtaSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron2deltaEtaEleClusterTrackAtCalo[MAXJETS];
  Float_t electron2deltaPhiSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron2deltaPhiSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron2deltaPhiEleClusterTrackAtCalo[MAXJETS];

  Float_t electron3Pt[MAXJETS];   			       
  Float_t electron3Eta[MAXJETS];  			       
  Float_t electron3Phi[MAXJETS];  			       
  Int_t electron3NHits[MAXJETS];   		     
  Int_t electron3NExpectedOuterHits[MAXJETS];   		       
  Int_t electron3NPixelHits[MAXJETS];   		       
  Float_t electron3NChi2[MAXJETS];			       
  Float_t electron3PtRel[MAXJETS];   		       
  Float_t electron3Sip2d[MAXJETS];   		       
  Float_t electron3Ip2d[MAXJETS];   		       
  Float_t electron3Ipe2d[MAXJETS];   		       
  Float_t electron3Sip3d[MAXJETS];
  Float_t electron3Ip3d[MAXJETS];
  Float_t electron3Ipe3d[MAXJETS];   		          		          		       
  Float_t electron3P0Par[MAXJETS];   		       
  Float_t electron3DeltaR[MAXJETS];  		       
  Float_t electron3EtaRel[MAXJETS];  		       
  Float_t electron3Ratio[MAXJETS];   		     
  Int_t electron3TrackQuality[MAXJETS];   		       
  Float_t electron3RatioRel[MAXJETS];
  Float_t electron3eSuperClusterOverP[MAXJETS];
  Float_t electron3eSeedClusterOverP[MAXJETS];
  Float_t electron3eSeedClusterOverPout[MAXJETS];
  Float_t electron3eEleClusterOverPout[MAXJETS];
  Float_t electron3deltaEtaSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron3deltaEtaSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron3deltaEtaEleClusterTrackAtCalo[MAXJETS];
  Float_t electron3deltaPhiSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron3deltaPhiSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron3deltaPhiEleClusterTrackAtCalo[MAXJETS];

  Float_t electron4Pt[MAXJETS];   			       
  Float_t electron4Eta[MAXJETS];  			       
  Float_t electron4Phi[MAXJETS];  			       
  Int_t electron4NHits[MAXJETS];   		     
  Int_t electron4NExpectedOuterHits[MAXJETS];   		       
  Int_t electron4NPixelHits[MAXJETS];   		       
  Float_t electron4NChi2[MAXJETS];			       
  Float_t electron4PtRel[MAXJETS];   		       
  Float_t electron4Sip2d[MAXJETS];   		       
  Float_t electron4Ip2d[MAXJETS];   		       
  Float_t electron4Ipe2d[MAXJETS];   		       
  Float_t electron4Sip3d[MAXJETS];
  Float_t electron4Ip3d[MAXJETS];
  Float_t electron4Ipe3d[MAXJETS];   		          		          		       
  Float_t electron4P0Par[MAXJETS];   		       
  Float_t electron4DeltaR[MAXJETS];  		       
  Float_t electron4EtaRel[MAXJETS];  		       
  Float_t electron4Ratio[MAXJETS];   		     
  Int_t electron4TrackQuality[MAXJETS];   		       
  Float_t electron4RatioRel[MAXJETS];
  Float_t electron4eSuperClusterOverP[MAXJETS];
  Float_t electron4eSeedClusterOverP[MAXJETS];
  Float_t electron4eSeedClusterOverPout[MAXJETS];
  Float_t electron4eEleClusterOverPout[MAXJETS];
  Float_t electron4deltaEtaSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron4deltaEtaSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron4deltaEtaEleClusterTrackAtCalo[MAXJETS];
  Float_t electron4deltaPhiSuperClusterTrackAtVtx[MAXJETS];
  Float_t electron4deltaPhiSeedClusterTrackAtCalo[MAXJETS];
  Float_t electron4deltaPhiEleClusterTrackAtCalo[MAXJETS];

  std::vector<Float_t*> electronPt;   			       
  std::vector<Float_t*> electronEta;  			       
  std::vector<Float_t*> electronPhi;  			       
  std::vector<Int_t*> electronNHits;   		     
  std::vector<Int_t*> electronNExpectedOuterHits;   		       
  std::vector<Int_t*> electronNPixelHits;   		       
  std::vector<Float_t*> electronNChi2;			       
  std::vector<Float_t*> electronPtRel;   		       
  std::vector<Float_t*> electronSip2d;   		       
  std::vector<Float_t*> electronIp2d;   		       
  std::vector<Float_t*> electronIpe2d;   		       
  std::vector<Float_t*> electronSip3d;
  std::vector<Float_t*> electronIp3d;
  std::vector<Float_t*> electronIpe3d;   		          		          		       
  std::vector<Float_t*> electronP0Par;   		       
  std::vector<Float_t*> electronDeltaR;  		       
  std::vector<Float_t*> electronEtaRel;  		       
  std::vector<Float_t*> electronRatio;   		     
  std::vector<Int_t*> electronTrackQuality;   		       
  std::vector<Float_t*> electronRatioRel;
  std::vector<Float_t*> electroneSuperClusterOverP;
  std::vector<Float_t*> electroneSeedClusterOverP;
  std::vector<Float_t*> electroneSeedClusterOverPout;
  std::vector<Float_t*> electroneEleClusterOverPout;
  std::vector<Float_t*> electrondeltaEtaSuperClusterTrackAtVtx;
  std::vector<Float_t*> electrondeltaEtaSeedClusterTrackAtCalo;
  std::vector<Float_t*> electrondeltaEtaEleClusterTrackAtCalo;
  std::vector<Float_t*> electrondeltaPhiSuperClusterTrackAtVtx;
  std::vector<Float_t*> electrondeltaPhiSeedClusterTrackAtCalo;
  std::vector<Float_t*> electrondeltaPhiEleClusterTrackAtCalo;
						       
  //softMuonTagInfos	
  Int_t nMuons[MAXJETS];                                

  Bool_t  muon1IsGlobal[MAXJETS];
  Bool_t  muon1IsTracker[MAXJETS];
  Bool_t  muon1IsStandalone[MAXJETS];
  Float_t muon1Pt[MAXJETS];   			       
  Float_t muon1Eta[MAXJETS];  			       
  Float_t muon1Phi[MAXJETS];  	
  Int_t   muon1NumberOfMatches[MAXJETS];
  Int_t   muon1GlobalMuonHits[MAXJETS];
  Int_t   muon1InnerValidHits[MAXJETS];
  Int_t   muon1NExpectedOuterHits[MAXJETS];   			       
  Int_t   muon1NPixelHits[MAXJETS];
  Float_t muon1InnerNChi2[MAXJETS];
  Float_t muon1GlobalNChi2[MAXJETS];
  Float_t muon1VzPVDist[MAXJETS];
  Float_t muon1PtRel[MAXJETS];   
  Float_t muon1Sip2d[MAXJETS];   		       
  Float_t muon1Ip2d[MAXJETS];   		       
  Float_t muon1Ipe2d[MAXJETS];   		       
  Float_t muon1Sip3d[MAXJETS];
  Float_t muon1Ip3d[MAXJETS];
  Float_t muon1Ipe3d[MAXJETS];   	
  Float_t muon1P0Par[MAXJETS];   			       
  Float_t muon1DeltaR[MAXJETS];  			       
  Float_t muon1EtaRel[MAXJETS];  			       
  Float_t muon1Ratio[MAXJETS];   			    
  Int_t   muon1TrackQuality[MAXJETS];   			       
  Float_t muon1RatioRel[MAXJETS];                          

  Bool_t  muon2IsGlobal[MAXJETS];
  Bool_t  muon2IsTracker[MAXJETS];
  Bool_t  muon2IsStandalone[MAXJETS];
  Float_t muon2Pt[MAXJETS];   			       
  Float_t muon2Eta[MAXJETS];  			       
  Float_t muon2Phi[MAXJETS];  	
  Int_t   muon2NumberOfMatches[MAXJETS];
  Int_t   muon2GlobalMuonHits[MAXJETS];
  Int_t   muon2InnerValidHits[MAXJETS];
  Int_t   muon2NExpectedOuterHits[MAXJETS];   			       
  Int_t   muon2NPixelHits[MAXJETS];
  Float_t muon2InnerNChi2[MAXJETS];
  Float_t muon2GlobalNChi2[MAXJETS];
  Float_t muon2VzPVDist[MAXJETS];
  Float_t muon2PtRel[MAXJETS];   
  Float_t muon2Sip2d[MAXJETS];   		       
  Float_t muon2Ip2d[MAXJETS];   		       
  Float_t muon2Ipe2d[MAXJETS];   		       
  Float_t muon2Sip3d[MAXJETS];
  Float_t muon2Ip3d[MAXJETS];
  Float_t muon2Ipe3d[MAXJETS];   	
  Float_t muon2P0Par[MAXJETS];   			       
  Float_t muon2DeltaR[MAXJETS];  			       
  Float_t muon2EtaRel[MAXJETS];  			       
  Float_t muon2Ratio[MAXJETS];   			    
  Int_t   muon2TrackQuality[MAXJETS];   			       
  Float_t muon2RatioRel[MAXJETS];                          

  Bool_t  muon3IsGlobal[MAXJETS];
  Bool_t  muon3IsTracker[MAXJETS];
  Bool_t  muon3IsStandalone[MAXJETS];
  Float_t muon3Pt[MAXJETS];   			       
  Float_t muon3Eta[MAXJETS];  			       
  Float_t muon3Phi[MAXJETS];  	
  Int_t   muon3NumberOfMatches[MAXJETS];
  Int_t   muon3GlobalMuonHits[MAXJETS];
  Int_t   muon3InnerValidHits[MAXJETS];
  Int_t   muon3NExpectedOuterHits[MAXJETS];   			       
  Int_t   muon3NPixelHits[MAXJETS];
  Float_t muon3InnerNChi2[MAXJETS];
  Float_t muon3GlobalNChi2[MAXJETS];
  Float_t muon3VzPVDist[MAXJETS];
  Float_t muon3PtRel[MAXJETS];   
  Float_t muon3Sip2d[MAXJETS];   		       
  Float_t muon3Ip2d[MAXJETS];   		       
  Float_t muon3Ipe2d[MAXJETS];   		       
  Float_t muon3Sip3d[MAXJETS];
  Float_t muon3Ip3d[MAXJETS];
  Float_t muon3Ipe3d[MAXJETS];   	
  Float_t muon3P0Par[MAXJETS];   			       
  Float_t muon3DeltaR[MAXJETS];  			       
  Float_t muon3EtaRel[MAXJETS];  			       
  Float_t muon3Ratio[MAXJETS];   			    
  Int_t   muon3TrackQuality[MAXJETS];   			       
  Float_t muon3RatioRel[MAXJETS];                          

  Bool_t  muon4IsGlobal[MAXJETS];
  Bool_t  muon4IsTracker[MAXJETS];
  Bool_t  muon4IsStandalone[MAXJETS];
  Float_t muon4Pt[MAXJETS];   			       
  Float_t muon4Eta[MAXJETS];  			       
  Float_t muon4Phi[MAXJETS];  	
  Int_t   muon4NumberOfMatches[MAXJETS];
  Int_t   muon4GlobalMuonHits[MAXJETS];
  Int_t   muon4InnerValidHits[MAXJETS];
  Int_t   muon4NExpectedOuterHits[MAXJETS];   			       
  Int_t   muon4NPixelHits[MAXJETS];
  Float_t muon4InnerNChi2[MAXJETS];
  Float_t muon4GlobalNChi2[MAXJETS];
  Float_t muon4VzPVDist[MAXJETS];
  Float_t muon4PtRel[MAXJETS];   
  Float_t muon4Sip2d[MAXJETS];   		       
  Float_t muon4Ip2d[MAXJETS];   		       
  Float_t muon4Ipe2d[MAXJETS];   		       
  Float_t muon4Sip3d[MAXJETS];
  Float_t muon4Ip3d[MAXJETS];
  Float_t muon4Ipe3d[MAXJETS];   	
  Float_t muon4P0Par[MAXJETS];   			       
  Float_t muon4DeltaR[MAXJETS];  			       
  Float_t muon4EtaRel[MAXJETS];  			       
  Float_t muon4Ratio[MAXJETS];   			    
  Int_t   muon4TrackQuality[MAXJETS];   			       
  Float_t muon4RatioRel[MAXJETS];                          



  std::vector< Bool_t* > muonIsGlobal;
  std::vector< Bool_t* >  muonIsTracker;
  std::vector<Bool_t*>  muonIsStandalone;
  std::vector<Float_t*> muonPt;   			       
  std::vector<Float_t*> muonEta;  			       
  std::vector<Float_t*> muonPhi;  	
  std::vector<Int_t*>   muonNumberOfMatches;
  std::vector<Int_t*>   muonGlobalMuonHits;
  std::vector<Int_t*>   muonInnerValidHits;
  std::vector<Int_t*>   muonNExpectedOuterHits;   			       
  std::vector<Int_t*>   muonNPixelHits;
  std::vector<Float_t*> muonInnerNChi2;
  std::vector<Float_t*> muonGlobalNChi2;
  std::vector<Float_t*> muonVzPVDist;
  std::vector<Float_t*> muonPtRel;   
  std::vector<Float_t*> muonSip2d;   		       
  std::vector<Float_t*> muonIp2d;   		       
  std::vector<Float_t*> muonIpe2d;   		       
  std::vector<Float_t*> muonSip3d;
  std::vector<Float_t*> muonIp3d;
  std::vector<Float_t*> muonIpe3d;   	
  std::vector<Float_t*> muonP0Par;   			       
  std::vector<Float_t*> muonDeltaR;  			       
  std::vector<Float_t*> muonEtaRel;  			       
  std::vector<Float_t*> muonRatio;   			    
  std::vector<Int_t*>   muonTrackQuality;   			       
  std::vector<Float_t*> muonRatioRel; 

  std::map< std::string, Float_t* > bTagArrays;

  edm::ESHandle<MagneticField> theMF;

  //end add

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

namespace{
  struct JetRefCompare : public binary_function<RefToBase<Jet>, RefToBase<Jet>, Bool_t> {
    inline Bool_t operator () (const RefToBase<Jet> &j1,
			     const RefToBase<Jet> &j2) const
    { return j1.id() < j2.id() || (j1.id() == j2.id() && j1.key() < j2.key()); }
  };
}
TagNtupleProducer::TagNtupleProducer(const edm::ParameterSet& iConfig):
  getSharedHitInfo_(iConfig.getParameter<Bool_t>( "getSharedHitInfo" )),
  HepMCTag_( iConfig.getUntrackedParameter<edm::InputTag>("HepMCTag",edm::InputTag("generator")) ),
  simTruthTag_( iConfig.getUntrackedParameter<edm::InputTag>("SimTruthTag",edm::InputTag("N/A")) ),//default is Not Appilcable
  BeamSpotTag_(iConfig.getParameter<edm::InputTag>( "BSTag" )),
  jet_src_(iConfig.getParameter<edm::InputTag>( "jetSrc" )),
  SVComputer_ (iConfig.getParameter<edm::InputTag>( "svComputer")),
  triggerTag_(iConfig.getParameter<edm::InputTag>("TriggerTag")),
  jet_MC_src_(iConfig.getParameter<edm::InputTag>( "jetMCSrc" )),
  GenJet_Matcher_(iConfig.getParameter<edm::InputTag>( "GenJetMatcher" )),
  jet_tracks_(iConfig.getParameter<edm::InputTag>( "jetTracks" )),
  primaryVertexProducer_  (iConfig.getParameter<InputTag>("primaryVertex")),
  SV_tag_infos_(iConfig.getParameter<edm::InputTag>( "SVTagInfos" )),
  IP_tag_infos_(iConfig.getParameter<edm::InputTag>( "IPTagInfos" )),
  electron_tag_infos_(iConfig.getParameter<edm::InputTag>( "ElectronTagInfos" )),
  muon_tag_infos_(iConfig.getParameter<edm::InputTag>( "MuonTagInfos" )),
  label_(iConfig.getParameter<string>( "Label" )),
  filename_(iConfig.getParameter<string>( "filename" )),
  bTag_Config_(iConfig.getParameter< vector<edm::ParameterSet> >("bTagConfig")),  //b tagger outputs configured in python file
  chi2cut_(iConfig.getParameter<Double_t>("TrackMCTruthMatchChi2Cut")),
  TriggerNames_(iConfig.getParameter< vector<string> >("TriggerSelection"))
{
  string alias;
  
  file = new TFile (filename_.c_str(), "RECREATE" );
  // create tree structure
  EventInfo = new TTree("t","t");
  RunInfo = new TTree("RunInfo","RunInfo");
  
  for (vector< ParameterSet >::iterator ibTag = bTag_Config_.begin(); ibTag != bTag_Config_.end(); ibTag++) 
    {
      bTagArrays[ibTag->getParameter<string>("alias")] = new Float_t[MAXJETS];
    }
  
  muonIsGlobal            .push_back(muon1IsGlobal           ); muonIsGlobal            .push_back(muon2IsGlobal);
  muonIsTracker           .push_back(muon1IsTracker          ); muonIsTracker           .push_back(muon2IsTracker          ); 
  muonIsStandalone        .push_back(muon1IsStandalone       ); muonIsStandalone        .push_back(muon2IsStandalone       ); 
  muonPt                  .push_back(muon1Pt                 ); muonPt                  .push_back(muon2Pt                 );       
  muonEta                 .push_back(muon1Eta                ); muonEta                 .push_back(muon2Eta                );       
  muonPhi                 .push_back(muon1Phi                ); muonPhi                 .push_back(muon2Phi                ); 
  muonNumberOfMatches     .push_back(muon1NumberOfMatches    ); muonNumberOfMatches     .push_back(muon2NumberOfMatches    ); 
  muonGlobalMuonHits      .push_back(muon1GlobalMuonHits     ); muonGlobalMuonHits      .push_back(muon2GlobalMuonHits     ); 
  muonInnerValidHits      .push_back(muon1InnerValidHits     ); muonInnerValidHits      .push_back(muon2InnerValidHits     ); 
  muonNExpectedOuterHits  .push_back(muon1NExpectedOuterHits ); muonNExpectedOuterHits  .push_back(muon2NExpectedOuterHits ); 
  muonNPixelHits          .push_back(muon1NPixelHits         ); muonNPixelHits          .push_back(muon2NPixelHits         ); 
  muonInnerNChi2          .push_back(muon1InnerNChi2         ); muonInnerNChi2          .push_back(muon2InnerNChi2         ); 
  muonGlobalNChi2         .push_back(muon1GlobalNChi2        ); muonGlobalNChi2         .push_back(muon2GlobalNChi2        ); 
  muonVzPVDist            .push_back(muon1VzPVDist           ); muonVzPVDist            .push_back(muon2VzPVDist           ); 
  muonPtRel               .push_back(muon1PtRel              ); muonPtRel               .push_back(muon2PtRel              ); 
  muonSip2d               .push_back(muon1Sip2d              ); muonSip2d               .push_back(muon2Sip2d              ); 
  muonIp2d                .push_back(muon1Ip2d               ); muonIp2d                .push_back(muon2Ip2d               ); 
  muonIpe2d               .push_back(muon1Ipe2d              ); muonIpe2d               .push_back(muon2Ipe2d              ); 
  muonSip3d               .push_back(muon1Sip3d              ); muonSip3d               .push_back(muon2Sip3d              ); 
  muonIp3d                .push_back(muon1Ip3d               ); muonIp3d                .push_back(muon2Ip3d               ); 
  muonIpe3d               .push_back(muon1Ipe3d              ); muonIpe3d               .push_back(muon2Ipe3d              ); 
  muonP0Par               .push_back(muon1P0Par              ); muonP0Par               .push_back(muon2P0Par              );       
  muonDeltaR              .push_back(muon1DeltaR             ); muonDeltaR              .push_back(muon2DeltaR             );       
  muonEtaRel              .push_back(muon1EtaRel             ); muonEtaRel              .push_back(muon2EtaRel             );       
  muonRatio               .push_back(muon1Ratio              ); muonRatio               .push_back(muon2Ratio              );       
  muonTrackQuality        .push_back(muon1TrackQuality       ); muonTrackQuality        .push_back(muon2TrackQuality       );       
  muonRatioRel            .push_back(muon1RatioRel           ); muonRatioRel            .push_back(muon2RatioRel           ); 
  
  muonIsGlobal            .push_back(muon3IsGlobal           ); muonIsGlobal            .push_back(muon4IsGlobal           ); 
  muonIsTracker           .push_back(muon3IsTracker          ); muonIsTracker           .push_back(muon4IsTracker          ); 
  muonIsStandalone        .push_back(muon3IsStandalone       ); muonIsStandalone        .push_back(muon4IsStandalone       ); 
  muonPt                  .push_back(muon3Pt                 ); muonPt                  .push_back(muon4Pt                 );       
  muonEta                 .push_back(muon3Eta                ); muonEta                 .push_back(muon4Eta                );       
  muonPhi                 .push_back(muon3Phi                ); muonPhi                 .push_back(muon4Phi                ); 
  muonNumberOfMatches     .push_back(muon3NumberOfMatches    ); muonNumberOfMatches     .push_back(muon4NumberOfMatches    ); 
  muonGlobalMuonHits      .push_back(muon3GlobalMuonHits     ); muonGlobalMuonHits      .push_back(muon4GlobalMuonHits     ); 
  muonInnerValidHits      .push_back(muon3InnerValidHits     ); muonInnerValidHits      .push_back(muon4InnerValidHits     ); 
  muonNExpectedOuterHits  .push_back(muon3NExpectedOuterHits ); muonNExpectedOuterHits  .push_back(muon4NExpectedOuterHits ); 
  muonNPixelHits          .push_back(muon3NPixelHits         ); muonNPixelHits          .push_back(muon4NPixelHits         ); 
  muonInnerNChi2          .push_back(muon3InnerNChi2         ); muonInnerNChi2          .push_back(muon4InnerNChi2         ); 
  muonGlobalNChi2         .push_back(muon3GlobalNChi2        ); muonGlobalNChi2         .push_back(muon4GlobalNChi2        ); 
  muonVzPVDist            .push_back(muon3VzPVDist           ); muonVzPVDist            .push_back(muon4VzPVDist           ); 
  muonPtRel               .push_back(muon3PtRel              ); muonPtRel               .push_back(muon4PtRel              ); 
  muonSip2d               .push_back(muon3Sip2d              ); muonSip2d               .push_back(muon4Sip2d              ); 
  muonIp2d                .push_back(muon3Ip2d               ); muonIp2d                .push_back(muon4Ip2d               ); 
  muonIpe2d               .push_back(muon3Ipe2d              ); muonIpe2d               .push_back(muon4Ipe2d              ); 
  muonSip3d               .push_back(muon3Sip3d              ); muonSip3d               .push_back(muon4Sip3d              ); 
  muonIp3d                .push_back(muon3Ip3d               ); muonIp3d                .push_back(muon4Ip3d               ); 
  muonIpe3d               .push_back(muon3Ipe3d              ); muonIpe3d               .push_back(muon4Ipe3d              ); 
  muonP0Par               .push_back(muon3P0Par              ); muonP0Par               .push_back(muon4P0Par              );       
  muonDeltaR              .push_back(muon3DeltaR             ); muonDeltaR              .push_back(muon4DeltaR             );       
  muonEtaRel              .push_back(muon3EtaRel             ); muonEtaRel              .push_back(muon4EtaRel             );       
  muonRatio               .push_back(muon3Ratio              ); muonRatio               .push_back(muon4Ratio              );       
  muonTrackQuality        .push_back(muon3TrackQuality       ); muonTrackQuality        .push_back(muon4TrackQuality       );       
  muonRatioRel            .push_back(muon3RatioRel           ); muonRatioRel            .push_back(muon4RatioRel           ); 


  electronPt                 .push_back(electron1Pt                   );  electronPt                 .push_back(electron2Pt                   );
  electronEta                .push_back(electron1Eta                  );  electronEta                .push_back(electron2Eta                  );			
  electronPhi                .push_back(electron1Phi                  );  electronPhi                .push_back(electron2Phi                  );			
  electronNHits              .push_back(electron1NHits                );  electronNHits              .push_back(electron2NHits                );		
  electronNExpectedOuterHits .push_back(electron1NExpectedOuterHits   );  electronNExpectedOuterHits .push_back(electron2NExpectedOuterHits   );		
  electronNPixelHits         .push_back(electron1NPixelHits           );  electronNPixelHits         .push_back(electron2NPixelHits           );		
  electronNChi2              .push_back(electron1NChi2                );  electronNChi2              .push_back(electron2NChi2                );			
  electronPtRel              .push_back(electron1PtRel                );  electronPtRel              .push_back(electron2PtRel                );		
  electronSip2d              .push_back(electron1Sip2d                );  electronSip2d              .push_back(electron2Sip2d                );		
  electronIp2d               .push_back(electron1Ip2d                 );  electronIp2d               .push_back(electron2Ip2d                 );		
  electronIpe2d              .push_back(electron1Ipe2d                );  electronIpe2d              .push_back(electron2Ipe2d                );		
  electronSip3d              .push_back(electron1Sip3d                );  electronSip3d              .push_back(electron2Sip3d                );
  electronIp3d               .push_back(electron1Ip3d                 );  electronIp3d               .push_back(electron2Ip3d                 );
  electronIpe3d              .push_back(electron1Ipe3d                );  electronIpe3d              .push_back(electron2Ipe3d                );
  electronP0Par              .push_back(electron1P0Par                );  electronP0Par              .push_back(electron2P0Par                );		
  electronDeltaR             .push_back(electron1DeltaR               );  electronDeltaR             .push_back(electron2DeltaR               );		
  electronEtaRel             .push_back(electron1EtaRel               );  electronEtaRel             .push_back(electron2EtaRel               );		
  electronRatio              .push_back(electron1Ratio                );  electronRatio              .push_back(electron2Ratio                );		
  electronTrackQuality       .push_back(electron1TrackQuality         );  electronTrackQuality       .push_back(electron2TrackQuality         );	    
  electronRatioRel           .push_back(electron1RatioRel             );  electronRatioRel           .push_back(electron2RatioRel             );

  electronPt                 .push_back(electron3Pt                   );  electronPt                 .push_back(electron4Pt                   );
  electronEta                .push_back(electron3Eta                  );  electronEta                .push_back(electron4Eta                  );			
  electronPhi                .push_back(electron3Phi                  );  electronPhi                .push_back(electron4Phi                  );			
  electronNHits              .push_back(electron3NHits                );  electronNHits              .push_back(electron4NHits                );		
  electronNExpectedOuterHits .push_back(electron3NExpectedOuterHits   );  electronNExpectedOuterHits .push_back(electron4NExpectedOuterHits   );		
  electronNPixelHits         .push_back(electron3NPixelHits           );  electronNPixelHits         .push_back(electron4NPixelHits           );		
  electronNChi2              .push_back(electron3NChi2                );  electronNChi2              .push_back(electron4NChi2                );			
  electronPtRel              .push_back(electron3PtRel                );  electronPtRel              .push_back(electron4PtRel                );		
  electronSip2d              .push_back(electron3Sip2d                );  electronSip2d              .push_back(electron4Sip2d                );		
  electronIp2d               .push_back(electron3Ip2d                 );  electronIp2d               .push_back(electron4Ip2d                 );		
  electronIpe2d              .push_back(electron3Ipe2d                );  electronIpe2d              .push_back(electron4Ipe2d                );		
  electronSip3d              .push_back(electron3Sip3d                );  electronSip3d              .push_back(electron4Sip3d                );
  electronIp3d               .push_back(electron3Ip3d                 );  electronIp3d               .push_back(electron4Ip3d                 );
  electronIpe3d              .push_back(electron3Ipe3d                );  electronIpe3d              .push_back(electron4Ipe3d                );
  electronP0Par              .push_back(electron3P0Par                );  electronP0Par              .push_back(electron4P0Par                );		
  electronDeltaR             .push_back(electron3DeltaR               );  electronDeltaR             .push_back(electron4DeltaR               );		
  electronEtaRel             .push_back(electron3EtaRel               );  electronEtaRel             .push_back(electron4EtaRel               );		
  electronRatio              .push_back(electron3Ratio                );  electronRatio              .push_back(electron4Ratio                );		
  electronTrackQuality       .push_back(electron3TrackQuality         );  electronTrackQuality       .push_back(electron4TrackQuality         );	    
  electronRatioRel           .push_back(electron3RatioRel             );  electronRatioRel           .push_back(electron4RatioRel             );

  //Some Electron Information on gsfTrack/ClusterMatching
  electroneSuperClusterOverP.            push_back(electron1eSuperClusterOverP);
  electroneSeedClusterOverP.             push_back(electron1eSeedClusterOverP);
  electroneSeedClusterOverPout.          push_back(electron1eSeedClusterOverPout);
  electroneEleClusterOverPout.           push_back(electron1eEleClusterOverPout);
  electrondeltaEtaSuperClusterTrackAtVtx.push_back(electron1deltaEtaSuperClusterTrackAtVtx);
  electrondeltaEtaSeedClusterTrackAtCalo.push_back(electron1deltaEtaSeedClusterTrackAtCalo);
  electrondeltaEtaEleClusterTrackAtCalo. push_back(electron1deltaEtaEleClusterTrackAtCalo);
  electrondeltaPhiSuperClusterTrackAtVtx.push_back(electron1deltaPhiSuperClusterTrackAtVtx);
  electrondeltaPhiSeedClusterTrackAtCalo.push_back(electron1deltaPhiSeedClusterTrackAtCalo);
  electrondeltaPhiEleClusterTrackAtCalo. push_back(electron1deltaPhiEleClusterTrackAtCalo);

  electroneSuperClusterOverP.            push_back(electron2eSuperClusterOverP);
  electroneSeedClusterOverP.             push_back(electron2eSeedClusterOverP);
  electroneSeedClusterOverPout.          push_back(electron2eSeedClusterOverPout);
  electroneEleClusterOverPout.           push_back(electron2eEleClusterOverPout);
  electrondeltaEtaSuperClusterTrackAtVtx.push_back(electron2deltaEtaSuperClusterTrackAtVtx);
  electrondeltaEtaSeedClusterTrackAtCalo.push_back(electron2deltaEtaSeedClusterTrackAtCalo);
  electrondeltaEtaEleClusterTrackAtCalo. push_back(electron2deltaEtaEleClusterTrackAtCalo);
  electrondeltaPhiSuperClusterTrackAtVtx.push_back(electron2deltaPhiSuperClusterTrackAtVtx);
  electrondeltaPhiSeedClusterTrackAtCalo.push_back(electron2deltaPhiSeedClusterTrackAtCalo);
  electrondeltaPhiEleClusterTrackAtCalo. push_back(electron2deltaPhiEleClusterTrackAtCalo);

  electroneSuperClusterOverP.            push_back(electron3eSuperClusterOverP);
  electroneSeedClusterOverP.             push_back(electron3eSeedClusterOverP);
  electroneSeedClusterOverPout.          push_back(electron3eSeedClusterOverPout);
  electroneEleClusterOverPout.           push_back(electron3eEleClusterOverPout);
  electrondeltaEtaSuperClusterTrackAtVtx.push_back(electron3deltaEtaSuperClusterTrackAtVtx);
  electrondeltaEtaSeedClusterTrackAtCalo.push_back(electron3deltaEtaSeedClusterTrackAtCalo);
  electrondeltaEtaEleClusterTrackAtCalo. push_back(electron3deltaEtaEleClusterTrackAtCalo);
  electrondeltaPhiSuperClusterTrackAtVtx.push_back(electron3deltaPhiSuperClusterTrackAtVtx);
  electrondeltaPhiSeedClusterTrackAtCalo.push_back(electron3deltaPhiSeedClusterTrackAtCalo);
  electrondeltaPhiEleClusterTrackAtCalo. push_back(electron3deltaPhiEleClusterTrackAtCalo);

  electroneSuperClusterOverP.            push_back(electron4eSuperClusterOverP);
  electroneSeedClusterOverP.             push_back(electron4eSeedClusterOverP);
  electroneSeedClusterOverPout.          push_back(electron4eSeedClusterOverPout);
  electroneEleClusterOverPout.           push_back(electron4eEleClusterOverPout);
  electrondeltaEtaSuperClusterTrackAtVtx.push_back(electron4deltaEtaSuperClusterTrackAtVtx);
  electrondeltaEtaSeedClusterTrackAtCalo.push_back(electron4deltaEtaSeedClusterTrackAtCalo);
  electrondeltaEtaEleClusterTrackAtCalo. push_back(electron4deltaEtaEleClusterTrackAtCalo);
  electrondeltaPhiSuperClusterTrackAtVtx.push_back(electron4deltaPhiSuperClusterTrackAtVtx);
  electrondeltaPhiSeedClusterTrackAtCalo.push_back(electron4deltaPhiSeedClusterTrackAtCalo);
  electrondeltaPhiEleClusterTrackAtCalo. push_back(electron4deltaPhiEleClusterTrackAtCalo);

  EventInfo->Branch(  "eventNumber"             , &eventNumber             , "eventNumber/i"            );
  EventInfo->Branch(  "runNumber"		    , &runNumber               , "runNumber/i"   	    );
  EventInfo->Branch(  "lumiBlockNumber" 	    , &lumiBlockNumber         , "lumiBlockNumber/i" 	    );
  HLTPrescaleFactors = new vector<UInt_t>();
  EventInfo->Branch(  "HLTPrescaleFactors",  &HLTPrescaleFactors);
  HLTriggerResults = new vector<Bool_t>();
  EventInfo->Branch(  "HLTriggerResults",  &HLTriggerResults);

  EventInfo->Branch(  "numberOfPUVertices",       &numberOfPUVertices, "numberOfPUVertices/i");
  EventInfo->Branch(  "numberOfPUVerticesMixingTruth",       &numberOfPUVerticesMixingTruth, "numberOfPUVerticesMixingTruth/F");
  EventInfo->Branch(  "numberOfPUVerticesTot",       &numberOfPUVerticesTot, "numberOfPUVerticesTot/i");
  EventInfo->Branch(  "numberOfPrimaryVertices" , &numberOfPrimaryVertices , "numberOfPrimaryVertices/i"); 
  EventInfo->Branch(  "numberOfTracksAtPV" , &numberOfTracksAtPV , "numberOfTracksAtPV/i"); 
  EventInfo->Branch(  "PVx" , &PVx , "PVx/F"); 
  EventInfo->Branch(  "PVy" , &PVy , "PVy/F"); 
  EventInfo->Branch(  "PVz" , &PVz , "PVz/F"); 
  EventInfo->Branch(  "PVChi2" , &PVChi2 , "PVChi2/F"); 
  EventInfo->Branch(  "PVndof" , &PVndof , "PVndof/F"); 
  EventInfo->Branch(  "PVNormalizedChi2" , &PVNormalizedChi2 , "PVNormalizedChi2/F"); 

  EventInfo->Branch(  "pthat" , &pthat, "pthat/F");
  EventInfo->Branch(  "mcweight" , &mcweight, "mcweight/F");
  EventInfo->Branch(  "isBGluonSplitting" , &isBGluonSplitting, "isBGluonSplitting/O");
  EventInfo->Branch(  "isCGluonSplitting" , &isCGluonSplitting, "isCGluonSplitting/O");


  //Basic Jet Information
  EventInfo->Branch(  "nJets", &nJets, "nJets/I");
           
  EventInfo->Branch(  "jetPt", jetPt , "jetPt[nJets]/F");                                 
  EventInfo->Branch(  "jetEta", jetEta, "jetEta[nJets]/F");                                
  EventInfo->Branch(  "jetPhi", jetPhi, "jetPhi[nJets]/F");                                
  EventInfo->Branch(  "jetEMFraction", jetEMFraction, "jetEMFraction[nJets]/F");        
  EventInfo->Branch(  "jetChargedEmEnergyFraction", jetChargedEmEnergyFraction, "jetChargedEmEnergyFraction[nJets]/F");
  EventInfo->Branch(  "jetNeutralEmEnergyFraction",  jetNeutralEmEnergyFraction, "jetNeutralEmEnergyFraction[nJets]/F");
  EventInfo->Branch(  "jetChargedHadronEnergyFraction", jetChargedHadronEnergyFraction, "jetChargedHadronEnergyFraction[nJets]/F");
  EventInfo->Branch(  "jetNeutralHadronEnergyFraction", jetNeutralHadronEnergyFraction, "jetNeutralHadronEnergyFraction[nJets]/F");
  EventInfo->Branch(  "jetChargedMultiplicity", jetChargedMultiplicity , "jetChargedMultiplicity[nJets]/F");
  EventInfo->Branch(  "jetMass", jetMass , "jetMass[nJets]/F");                               
  EventInfo->Branch(  "jetnConstituents", jetnConstituents ,"jetnConstituents[nJets]/I");  
  EventInfo->Branch(  "jetnTracks", jetnTracks, "jetnTracks[nJets]/I");                           
  //   EventInfo->Branch(  "jetnElectrons", jetnElectrons   [nJets];                              
  //   EventInfo->Branch(  "jetnMuons", jetnMuons   [nJets];                                  
  //   math::XYZVector "jetVertex", jetVertex , "jetVertex[nJets]/F");                   
  EventInfo->Branch(  "jetVertexChi2", jetVertexChi2 , "jetVertexChi2[nJets]/F");               
  EventInfo->Branch(  "jetVertexChi2Ndof", jetVertexChi2Ndof , "jetVertexChi2Ndof[nJets]/F");           
  EventInfo->Branch(  "jetVertexNormalizedChi2", jetVertexNormalizedChi2 , "jetVertexNormalizedChi2[nJets]/F");    

  //Track Information
  EventInfo->Branch(  "nTracks", &nTracks, "nTracks/I");

  EventInfo->Branch(  "trackJetIndex", trackJetIndex, "trackJetIndex[nTracks]/I");
  EventInfo->Branch( "trackSelected", trackSelected, "trackSelected[nTracks]/O") ;
  //				 math::XYZVector "track3Momentum", track3Momentum,   [nTracks];
  EventInfo->Branch(  "trackTransverseMomentum", trackTransverseMomentum,  "trackTransverseMomentum[nTracks]/F");
  EventInfo->Branch(  "trackEta", trackEta, "trackEta[nTracks]/F");
  EventInfo->Branch(  "trackPhi", trackPhi, "trackPhi[nTracks]/F");
  EventInfo->Branch(  "trackMomentum",  trackMomentum, "trackMomentum[nTracks]/F");
  EventInfo->Branch(  "trackNHits", trackNHits, "trackNHits[nTracks]/I");
  EventInfo->Branch(  "trackNPixelHits",  trackNPixelHits, "trackNPixelHits[nTracks]/I");
  EventInfo->Branch(  "trackChi2",  trackChi2, "trackChi2[nTracks]/F");
  EventInfo->Branch(  "trackNormChi2", trackNormChi2, "trackNormChi2[nTracks]/F");
  EventInfo->Branch(  "trackQuality",  trackQuality, "trackQuality[nTracks]/I");
  EventInfo->Branch(  "trackLongitudinalImpactParameter",  trackLongitudinalImpactParameter, "trackLongitudinalImpactParameter[nTracks]/F");
  EventInfo->Branch(  "trackIP", trackIP, "trackIP[nTracks]/F");
  EventInfo->Branch(  "trackDecayLength", trackDecayLength, "trackDecayLength[nTracks]/F");
  EventInfo->Branch(  "trackDistJetAxis",  trackDistJetAxis, "trackDistJetAxis[nTracks]/F");
  EventInfo->Branch(  "trackDeltaR", trackDeltaR, "trackDeltaR[nTracks]/F");
  EventInfo->Branch(  "trackIP3d", trackIP3d, "trackIP3d[nTracks]/F");
  EventInfo->Branch(  "trackIP3dError",  trackIP3dError, "trackIP3dError[nTracks]/F");
  EventInfo->Branch(  "trackIP3dProbability",  trackIP3dProbability, "trackIP3dProbability[nTracks]/F");
  EventInfo->Branch(  "trackIP2d",  trackIP2d, "trackIP2d[nTracks]/F");
  EventInfo->Branch(  "trackIP2dError",  trackIP2dError, "trackIP2dError[nTracks]/F");
  EventInfo->Branch(  "trackIP2dProbability",  trackIP2dProbability, "trackIP2dProbability[nTracks]/F");
  EventInfo->Branch(  "trackPtRel",  trackPtRel, "trackPtRel[nTracks]/F");
  EventInfo->Branch(  "trackHasSharedPix1",  trackHasSharedPix1, "trackHasSharedPix1[nTracks]/O");
  EventInfo->Branch(  "trackHasSharedPix2",  trackHasSharedPix2, "trackHasSharedPix2[nTracks]/O");
  EventInfo->Branch(  "trackHasSharedPix3",  trackHasSharedPix3, "trackHasSharedPix3[nTracks]/O");
  EventInfo->Branch(  "trackHasSharedPixAll",  trackHasSharedPixAll, "trackHasSharedPixAll[nTracks]/O");
  EventInfo->Branch( "trackIsVertexTrack", trackIsVertexTrack, "trackIsVertexTrack[nTracks]/O") ;
  EventInfo->Branch( "trackIsFromB", trackIsFromB, "trackIsFromB[nTracks]/O") ;
  EventInfo->Branch( "trackIsFromPU", trackIsFromPU, "trackIsFromPU[nTracks]/O") ;

  //MC Truth Information
  EventInfo->Branch(  "MCTrueFlavor", PartonFlavor, "MCTrueFlavor[nJets]/I"); 
  EventInfo->Branch(  "PartonPt",PartonPt, "PartonPt[nJets]/F"); 
  EventInfo->Branch(  "PartonEta", PartonEta, "PartonEta[nJets]/F"); 
  EventInfo->Branch(  "PartonPhi", PartonPhi, "PartonPhi[nJets]/F");                          
  EventInfo->Branch(  "GenJetPt",GenJetPt, "GenJetPt[nJets]/F"); 
  EventInfo->Branch(  "GenJetEta", GenJetEta, "GenJetEta[nJets]/F"); 
  EventInfo->Branch(  "GenJetPhi", GenJetPhi, "GenJetPhi[nJets]/F");                          
 				       
  //secondaryVertexTagInfos:			       
  //  math::XYZVector "SecondaryVertex",    [nJets];             
  EventInfo->Branch(  "SV3dDistance", SV3dDistance, "SV3dDistance[nJets]/F");                          
  EventInfo->Branch(  "SV3dDistanceError",  SV3dDistanceError, "SV3dDistanceError[nJets]/F");                     
  EventInfo->Branch(  "SV2dDistance", SV2dDistance, "SV2dDistance[nJets]/F");                          
  EventInfo->Branch(  "SV2dDistanceError",  SV2dDistanceError, "SV2dDistanceError[nJets]/F");                     
  EventInfo->Branch(  "SVChi2",  SVChi2, "SVChi2[nJets]/F");                
  EventInfo->Branch(  "SVIPFirstAboveCharm",  SVIPFirstAboveCharm, "SVIPFirstAboveCharm[nJets]/F");                                
  EventInfo->Branch(  "SVDegreesOfFreedom",  SVDegreesOfFreedom, "SVDegreesOfFreedom[nJets]/F");                    
  EventInfo->Branch(  "SVNormChi2", SVNormChi2, "SVNormChi2[nJets]/F");                            
  EventInfo->Branch(  "SVnSelectedTracks",  SVnSelectedTracks, "SVnSelectedTracks[nJets]/I");                               
  EventInfo->Branch(  "SVMass",  SVMass, "SVMass[nJets]/F");                                
  EventInfo->Branch(  "SVEnergyRatio",  SVEnergyRatio, "SVEnergyRatio[nJets]/F");                                
  EventInfo->Branch(  "SVnVertices",  SVnVertices, "SVnVertices[nJets]/I");                             
  EventInfo->Branch(  "SVnVertexTracks", SVnVertexTracks, "SVnVertexTracks[nJets]/I");         
  EventInfo->Branch(  "SVnVertexTracksAll",  SVnVertexTracksAll, "SVnVertexTracksAll[nJets]/I");         
  EventInfo->Branch(  "SVnFirstVertexTracks", SVnFirstVertexTracks, "SVnFirstVertexTracks[nJets]/I");         
  EventInfo->Branch(  "SVnFirstVertexTracksAll",  SVnFirstVertexTracksAll, "SVnFirstVertexTracksAll[nJets]/I");         
  EventInfo->Branch(  "SVjetDeltaR",  SVjetDeltaR, "SVjetDeltaR[nJets]/F");
  EventInfo->Branch(  "SVvtxSumVtxDirDeltaR", SVvtxSumVtxDirDeltaR, "SVvtxSumVtxDirDeltaR[nJets]/F");
  EventInfo->Branch(  "SVvtxSumJetDeltaR",  SVvtxSumJetDeltaR, "SVvtxSumJetDeltaR[nJets]/F");

  EventInfo->Branch(  "SVvtxPt",  SVvtxPt, "SVvtxPt[nJets]/F");
  EventInfo->Branch(  "SVvtxSumDirEta",  SVvtxSumDirEta, "SVvtxSumDirEta[nJets]/F");
  EventInfo->Branch(  "SVvtxSumDirPhi",  SVvtxSumDirPhi, "SVvtxSumDirPhi[nJets]/F");
  EventInfo->Branch(  "SVvtxDirEta",  SVvtxDirEta, "SVvtxDirEta[nJets]/F");
  EventInfo->Branch(  "SVvtxDirPhi",  SVvtxDirPhi, "SVvtxDirPhi[nJets]/F");

  // added by Caroline
   EventInfo->Branch(  "SVvtxSumPTrel",  SVvtxSumPTrel, "SVvtxSumPTrel[nJets]/F");
   EventInfo->Branch(  "SVvtxDirPTrel",  SVvtxDirPTrel, "SVvtxDirPTrel[nJets]/F");
   EventInfo->Branch(  "SVvtxDistJetAxis",  SVvtxDistJetAxis, "SVvtxDistJetAxis[nJets]/F");
   EventInfo->Branch(  "SVtotCharge",  SVtotCharge, "SVtotCharge[nJets]/I");
  // end add
               
  //impactParameterTagInfos
  EventInfo->Branch(  "IPnSelectedTracks",  IPnSelectedTracks, "IPnSelectedTracks[nJets]/I"); 
  EventInfo->Branch(  "IPnSelectedAndDecayLengthAndJetAsixTracks",  IPnSelectedAndDecayLengthAndJetAsixTracks, "IPnSelectedAndDecayLengthAndJetAsixTracks[nJets]/I"); 
 
  //  math::XYZVector "IPghostTrackP3",    [nJets];      
  EventInfo->Branch(  "IPghostTrackPt",  IPghostTrackPt, "IPghostTrackPt[nJets]/F");                        
  EventInfo->Branch(  "IPghostTrackPtRel", IPghostTrackPtRel, "IPghostTrackPtRel[nJets]/F");                        
  EventInfo->Branch(  "IPghostTrackEta",  IPghostTrackEta, "IPghostTrackEta[nJets]/F");                       
  EventInfo->Branch(  "IPghostTrackPhi",  IPghostTrackPhi, "IPghostTrackPhi[nJets]/F");                       
  EventInfo->Branch(  "IPghostTrackDeltaR", IPghostTrackDeltaR, "IPghostTrackDeltaR[nJets]/F");
  EventInfo->Branch(  "IPPix1SharedHits",  IPPix1SharedHits, "IPPix1SharedHits[nJets]/I");
  EventInfo->Branch(  "IPPix1TotalHits",  IPPix1TotalHits, "IPPix1TotalHits[nJets]/I");
  EventInfo->Branch(  "IPPix2SharedHits", IPPix2SharedHits, "IPPix2SharedHits[nJets]/I");
  EventInfo->Branch(  "IPPix2TotalHits", IPPix2TotalHits, "IPPix2TotalHits[nJets]/I");
  EventInfo->Branch(  "IPPix3SharedHits",  IPPix3SharedHits, "IPPix3SharedHits[nJets]/I");
  EventInfo->Branch(  "IPPix3TotalHits", IPPix3TotalHits, "IPPix3TotalHits[nJets]/I");
  EventInfo->Branch(  "IPPixAllSharedHits", IPPixAllSharedHits, "IPPixAllSharedHits[nJets]/I");
  EventInfo->Branch(  "IPPixAllTotalHits", IPPixAllTotalHits, "IPPixAllTotalHits[nJets]/I");

  //softElectronTagInfos
  EventInfo->Branch(  "nElectrons",  nElectrons, "nElectrons[nJets]/I");  
                          
  EventInfo->Branch(  "electron1Pt",                  electron1Pt,                 "electron1Pt[nJets]/F");   			       
  EventInfo->Branch(  "electron1Eta",                 electron1Eta,                "electron1Eta[nJets]/F");  			       
  EventInfo->Branch(  "electron1Phi",                 electron1Phi,                "electron1Phi[nJets]/F");  			       
  EventInfo->Branch(  "electron1NHits",               electron1NHits,              "electron1NHits[nJets]/I");   		     
  EventInfo->Branch(  "electron1NExpectedOuterHits",  electron1NExpectedOuterHits, "electron1NExpectedOuterHits[nJets]/I");   		       
  EventInfo->Branch(  "electron1NPixelHits",          electron1NPixelHits,         "electron1NPixelHits[nJets]/I");   		       
  EventInfo->Branch(  "electron1NChi2",               electron1NChi2,              "electron1NChi2[nJets]/F");			       
  EventInfo->Branch(  "electron1PtRel",               electron1PtRel,              "electron1PtRel[nJets]/F");   		       
  EventInfo->Branch(  "electron1Sip2d",               electron1Sip2d,              "electron1Sip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron1Ip2d",                electron1Ip2d,               "electron1Ip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron1Ipe2d",               electron1Ipe2d,              "electron1Ipe2d[nJets]/F");   		       
  EventInfo->Branch(  "electron1Sip3d",               electron1Sip3d,              "electron1Sip3d[nJets]/F");
  EventInfo->Branch(  "electron1Ip3d",                electron1Ip3d,               "electron1Ip3d[nJets]/F");
  EventInfo->Branch(  "electron1Ipe3d",               electron1Ipe3d,              "electron1Ipe3d[nJets]/F");   		          		          		       
  EventInfo->Branch(  "electron1P0Par",               electron1P0Par,              "electron1P0Par[nJets]/F");   		       
  EventInfo->Branch(  "electron1DeltaR",              electron1DeltaR,             "electron1DeltaR[nJets]/F");  		       
  EventInfo->Branch(  "electron1EtaRel",              electron1EtaRel,             "electron1EtaRel[nJets]/F");  		       
  EventInfo->Branch(  "electron1Ratio",               electron1Ratio,              "electron1Ratio[nJets]/F");   		     
  EventInfo->Branch(  "electron1TrackQuality",        electron1TrackQuality,       "electron1TrackQuality[nJets]/I");   		       
  EventInfo->Branch(  "electron1RatioRel",            electron1RatioRel,           "electron1RatioRel[nJets]/F");                      
  EventInfo->Branch(  "electron1eSuperClusterOverP",             electron1eSuperClusterOverP,             "electron1eSuperClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron1eSeedClusterOverP",              electron1eSeedClusterOverP,              "electron1eSeedClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron1eSeedClusterOverPout",           electron1eSeedClusterOverPout,           "electron1eSeedClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron1eEleClusterOverPout",            electron1eEleClusterOverPout,            "electron1eEleClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron1deltaEtaSuperClusterTrackAtVtx", electron1deltaEtaSuperClusterTrackAtVtx, "electron1deltaEtaSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron1deltaEtaSeedClusterTrackAtCalo", electron1deltaEtaSeedClusterTrackAtCalo, "electron1deltaEtaSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron1deltaEtaEleClusterTrackAtCalo",  electron1deltaEtaEleClusterTrackAtCalo,  "electron1deltaEtaEleClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron1deltaPhiSuperClusterTrackAtVtx", electron1deltaPhiSuperClusterTrackAtVtx, "electron1deltaPhiSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron1deltaPhiSeedClusterTrackAtCalo", electron1deltaPhiSeedClusterTrackAtCalo, "electron1deltaPhiSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron1deltaPhiEleClusterTrackAtCalo",  electron1deltaPhiEleClusterTrackAtCalo,  "electron1deltaPhiEleClusterTrackAtCalo[nJets]/F");
            
  EventInfo->Branch(  "electron2Pt",                  electron2Pt,                 "electron2Pt[nJets]/F");   			       
  EventInfo->Branch(  "electron2Eta",                 electron2Eta,                "electron2Eta[nJets]/F");  			       
  EventInfo->Branch(  "electron2Phi",                 electron2Phi,                "electron2Phi[nJets]/F");  			       
  EventInfo->Branch(  "electron2NHits",               electron2NHits,              "electron2NHits[nJets]/I");   		     
  EventInfo->Branch(  "electron2NExpectedOuterHits",  electron2NExpectedOuterHits, "electron2NExpectedOuterHits[nJets]/I");   		       
  EventInfo->Branch(  "electron2NPixelHits",          electron2NPixelHits,         "electron2NPixelHits[nJets]/I");   		       
  EventInfo->Branch(  "electron2NChi2",               electron2NChi2,              "electron2NChi2[nJets]/F");			       
  EventInfo->Branch(  "electron2PtRel",               electron2PtRel,              "electron2PtRel[nJets]/F");   		       
  EventInfo->Branch(  "electron2Sip2d",               electron2Sip2d,              "electron2Sip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron2Ip2d",                electron2Ip2d,               "electron2Ip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron2Ipe2d",               electron2Ipe2d,              "electron2Ipe2d[nJets]/F");   		       
  EventInfo->Branch(  "electron2Sip3d",               electron2Sip3d,              "electron2Sip3d[nJets]/F");
  EventInfo->Branch(  "electron2Ip3d",                electron2Ip3d,               "electron2Ip3d[nJets]/F");
  EventInfo->Branch(  "electron2Ipe3d",               electron2Ipe3d,              "electron2Ipe3d[nJets]/F");   		          		          		       
  EventInfo->Branch(  "electron2P0Par",               electron2P0Par,              "electron2P0Par[nJets]/F");   		       
  EventInfo->Branch(  "electron2DeltaR",              electron2DeltaR,             "electron2DeltaR[nJets]/F");  		       
  EventInfo->Branch(  "electron2EtaRel",              electron2EtaRel,             "electron2EtaRel[nJets]/F");  		       
  EventInfo->Branch(  "electron2Ratio",               electron2Ratio,              "electron2Ratio[nJets]/F");   		     
  EventInfo->Branch(  "electron2TrackQuality",        electron2TrackQuality,       "electron2TrackQuality[nJets]/I");   		       
  EventInfo->Branch(  "electron2RatioRel",            electron2RatioRel,           "electron2RatioRel[nJets]/F"); 
  EventInfo->Branch(  "electron2eSuperClusterOverP",             electron2eSuperClusterOverP,             "electron2eSuperClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron2eSeedClusterOverP",              electron2eSeedClusterOverP,              "electron2eSeedClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron2eSeedClusterOverPout",           electron2eSeedClusterOverPout,           "electron2eSeedClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron2eEleClusterOverPout",            electron2eEleClusterOverPout,            "electron2eEleClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron2deltaEtaSuperClusterTrackAtVtx", electron2deltaEtaSuperClusterTrackAtVtx, "electron2deltaEtaSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron2deltaEtaSeedClusterTrackAtCalo", electron2deltaEtaSeedClusterTrackAtCalo, "electron2deltaEtaSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron2deltaEtaEleClusterTrackAtCalo",  electron2deltaEtaEleClusterTrackAtCalo,  "electron2deltaEtaEleClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron2deltaPhiSuperClusterTrackAtVtx", electron2deltaPhiSuperClusterTrackAtVtx, "electron2deltaPhiSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron2deltaPhiSeedClusterTrackAtCalo", electron2deltaPhiSeedClusterTrackAtCalo, "electron2deltaPhiSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron2deltaPhiEleClusterTrackAtCalo",  electron2deltaPhiEleClusterTrackAtCalo,  "electron2deltaPhiEleClusterTrackAtCalo[nJets]/F");
                          
  EventInfo->Branch(  "electron3Pt",                  electron3Pt,                 "electron3Pt[nJets]/F");   			       
  EventInfo->Branch(  "electron3Eta",                 electron3Eta,                "electron3Eta[nJets]/F");  			       
  EventInfo->Branch(  "electron3Phi",                 electron3Phi,                "electron3Phi[nJets]/F");  			       
  EventInfo->Branch(  "electron3NHits",               electron3NHits,              "electron3NHits[nJets]/I");   		     
  EventInfo->Branch(  "electron3NExpectedOuterHits",  electron3NExpectedOuterHits, "electron3NExpectedOuterHits[nJets]/I");   		       
  EventInfo->Branch(  "electron3NPixelHits",          electron3NPixelHits,         "electron3NPixelHits[nJets]/I");   		       
  EventInfo->Branch(  "electron3NChi2",               electron3NChi2,              "electron3NChi2[nJets]/F");			       
  EventInfo->Branch(  "electron3PtRel",               electron3PtRel,              "electron3PtRel[nJets]/F");   		       
  EventInfo->Branch(  "electron3Sip2d",               electron3Sip2d,              "electron3Sip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron3Ip2d",                electron3Ip2d,               "electron3Ip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron3Ipe2d",               electron3Ipe2d,              "electron3Ipe2d[nJets]/F");   		       
  EventInfo->Branch(  "electron3Sip3d",               electron3Sip3d,              "electron3Sip3d[nJets]/F");
  EventInfo->Branch(  "electron3Ip3d",                electron3Ip3d,               "electron3Ip3d[nJets]/F");
  EventInfo->Branch(  "electron3Ipe3d",               electron3Ipe3d,              "electron3Ipe3d[nJets]/F");   		          		          		       
  EventInfo->Branch(  "electron3P0Par",               electron3P0Par,              "electron3P0Par[nJets]/F");   		       
  EventInfo->Branch(  "electron3DeltaR",              electron3DeltaR,             "electron3DeltaR[nJets]/F");  		       
  EventInfo->Branch(  "electron3EtaRel",              electron3EtaRel,             "electron3EtaRel[nJets]/F");  		       
  EventInfo->Branch(  "electron3Ratio",               electron3Ratio,              "electron3Ratio[nJets]/F");   		     
  EventInfo->Branch(  "electron3TrackQuality",        electron3TrackQuality,       "electron3TrackQuality[nJets]/I");   		       
  EventInfo->Branch(  "electron3RatioRel",            electron3RatioRel,           "electron3RatioRel[nJets]/F");  
  EventInfo->Branch(  "electron3eSuperClusterOverP",             electron3eSuperClusterOverP,             "electron3eSuperClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron3eSeedClusterOverP",              electron3eSeedClusterOverP,              "electron3eSeedClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron3eSeedClusterOverPout",           electron3eSeedClusterOverPout,           "electron3eSeedClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron3eEleClusterOverPout",            electron3eEleClusterOverPout,            "electron3eEleClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron3deltaEtaSuperClusterTrackAtVtx", electron3deltaEtaSuperClusterTrackAtVtx, "electron3deltaEtaSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron3deltaEtaSeedClusterTrackAtCalo", electron3deltaEtaSeedClusterTrackAtCalo, "electron3deltaEtaSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron3deltaEtaEleClusterTrackAtCalo",  electron3deltaEtaEleClusterTrackAtCalo,  "electron3deltaEtaEleClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron3deltaPhiSuperClusterTrackAtVtx", electron3deltaPhiSuperClusterTrackAtVtx, "electron3deltaPhiSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron3deltaPhiSeedClusterTrackAtCalo", electron3deltaPhiSeedClusterTrackAtCalo, "electron3deltaPhiSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron3deltaPhiEleClusterTrackAtCalo",  electron3deltaPhiEleClusterTrackAtCalo,  "electron3deltaPhiEleClusterTrackAtCalo[nJets]/F"); 
                          
  EventInfo->Branch(  "electron4Pt",                  electron4Pt,                 "electron4Pt[nJets]/F");   			       
  EventInfo->Branch(  "electron4Eta",                 electron4Eta,                "electron4Eta[nJets]/F");  			       
  EventInfo->Branch(  "electron4Phi",                 electron4Phi,                "electron4Phi[nJets]/F");  			       
  EventInfo->Branch(  "electron4NHits",               electron4NHits,              "electron4NHits[nJets]/I");   		     
  EventInfo->Branch(  "electron4NExpectedOuterHits",  electron4NExpectedOuterHits, "electron4NExpectedOuterHits[nJets]/I");   		       
  EventInfo->Branch(  "electron4NPixelHits",          electron4NPixelHits,         "electron4NPixelHits[nJets]/I");   		       
  EventInfo->Branch(  "electron4NChi2",               electron4NChi2,              "electron4NChi2[nJets]/F");			       
  EventInfo->Branch(  "electron4PtRel",               electron4PtRel,              "electron4PtRel[nJets]/F");   		       
  EventInfo->Branch(  "electron4Sip2d",               electron4Sip2d,              "electron4Sip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron4Ip2d",                electron4Ip2d,               "electron4Ip2d[nJets]/F");   		       
  EventInfo->Branch(  "electron4Ipe2d",               electron4Ipe2d,              "electron4Ipe2d[nJets]/F");   		       
  EventInfo->Branch(  "electron4Sip3d",               electron4Sip3d,              "electron4Sip3d[nJets]/F");
  EventInfo->Branch(  "electron4Ip3d",                electron4Ip3d,               "electron4Ip3d[nJets]/F");
  EventInfo->Branch(  "electron4Ipe3d",               electron4Ipe3d,              "electron4Ipe3d[nJets]/F");   		          		          		       
  EventInfo->Branch(  "electron4P0Par",               electron4P0Par,              "electron4P0Par[nJets]/F");   		       
  EventInfo->Branch(  "electron4DeltaR",              electron4DeltaR,             "electron4DeltaR[nJets]/F");  		       
  EventInfo->Branch(  "electron4EtaRel",              electron4EtaRel,             "electron4EtaRel[nJets]/F");  		       
  EventInfo->Branch(  "electron4Ratio",               electron4Ratio,              "electron4Ratio[nJets]/F");   		     
  EventInfo->Branch(  "electron4TrackQuality",        electron4TrackQuality,       "electron4TrackQuality[nJets]/I");   		       
  EventInfo->Branch(  "electron4RatioRel",            electron4RatioRel,           "electron4RatioRel[nJets]/F"); 
  EventInfo->Branch(  "electron4eSuperClusterOverP",             electron4eSuperClusterOverP,             "electron4eSuperClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron4eSeedClusterOverP",              electron4eSeedClusterOverP,              "electron4eSeedClusterOverP[nJets]/F");
  EventInfo->Branch(  "electron4eSeedClusterOverPout",           electron4eSeedClusterOverPout,           "electron4eSeedClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron4eEleClusterOverPout",            electron4eEleClusterOverPout,            "electron4eEleClusterOverPout[nJets]/F");
  EventInfo->Branch(  "electron4deltaEtaSuperClusterTrackAtVtx", electron4deltaEtaSuperClusterTrackAtVtx, "electron4deltaEtaSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron4deltaEtaSeedClusterTrackAtCalo", electron4deltaEtaSeedClusterTrackAtCalo, "electron4deltaEtaSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron4deltaEtaEleClusterTrackAtCalo",  electron4deltaEtaEleClusterTrackAtCalo,  "electron4deltaEtaEleClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron4deltaPhiSuperClusterTrackAtVtx", electron4deltaPhiSuperClusterTrackAtVtx, "electron4deltaPhiSuperClusterTrackAtVtx[nJets]/F");
  EventInfo->Branch(  "electron4deltaPhiSeedClusterTrackAtCalo", electron4deltaPhiSeedClusterTrackAtCalo, "electron4deltaPhiSeedClusterTrackAtCalo[nJets]/F");
  EventInfo->Branch(  "electron4deltaPhiEleClusterTrackAtCalo",  electron4deltaPhiEleClusterTrackAtCalo,  "electron4deltaPhiEleClusterTrackAtCalo[nJets]/F");

  //softMuonTagInfos	
  EventInfo->Branch(  "nMuons", nMuons, "nMuons[nJets]/I");                                
  
  EventInfo->Branch(  "muon1IsGlobal",          muon1IsGlobal,            "muon1IsGlobal[nJets]/O");            
  EventInfo->Branch(  "muon1IsTracker", 	   muon1IsTracker,           "muon1IsTracker[nJets]/O");           
  EventInfo->Branch(  "muon1IsStandalone", 	   muon1IsStandalone,        "muon1IsStandalone[nJets]/O");        
  EventInfo->Branch(  "muon1Pt",    		   muon1Pt,                  "muon1Pt[nJets]/F");                     	       
  EventInfo->Branch(  "muon1Eta",   		   muon1Eta,                 "muon1Eta[nJets]/F");                    	       
  EventInfo->Branch(  "muon1Phi",   		   muon1Phi,                 "muon1Phi[nJets]/F");                 
  EventInfo->Branch(  "muon1NumberOfMatches",   muon1NumberOfMatches,     "muon1NumberOfMatches[nJets]/I");     
  EventInfo->Branch(  "muon1GlobalMuonHits",    muon1GlobalMuonHits,      "muon1GlobalMuonHits[nJets]/I");      
  EventInfo->Branch(  "muon1InnerValidHits",    muon1InnerValidHits,      "muon1InnerValidHits[nJets]/I");      
  EventInfo->Branch(  "muon1NExpectedOuterHits",muon1NExpectedOuterHits,  "muon1NExpectedOuterHits[nJets]/I");    			       
  EventInfo->Branch(  "muon1NPixelHits", 	   muon1NPixelHits,          "muon1NPixelHits[nJets]/I");          
  EventInfo->Branch(  "muon1InnerNChi2", 	   muon1InnerNChi2,          "muon1InnerNChi2[nJets]/F");          
  EventInfo->Branch(  "muon1GlobalNChi2", 	   muon1GlobalNChi2,         "muon1GlobalNChi2[nJets]/F");         
  EventInfo->Branch(  "muon1VzPVDist", 	   muon1VzPVDist,            "muon1VzPVDist[nJets]/F");            
  EventInfo->Branch(  "muon1PtRel",    	   muon1PtRel,               "muon1PtRel[nJets]/F");               
  EventInfo->Branch(  "muon1Sip2d",    	   muon1Sip2d,               "muon1Sip2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon1Ip2d",    	   muon1Ip2d,                "muon1Ip2d[nJets]/F");                    
  EventInfo->Branch(  "muon1Ipe2d",    	   muon1Ipe2d,               "muon1Ipe2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon1Sip3d", 		   muon1Sip3d,               "muon1Sip3d[nJets]/F");               
  EventInfo->Branch(  "muon1Ip3d", 		   muon1Ip3d,                "muon1Ip3d[nJets]/F");                
  EventInfo->Branch(  "muon1Ipe3d",    	   muon1Ipe3d,               "muon1Ipe3d[nJets]/F");               
  EventInfo->Branch(  "muon1P0Par",    	   muon1P0Par,               "muon1P0Par[nJets]/F");                  		       
  EventInfo->Branch(  "muon1DeltaR",   	   muon1DeltaR,              "muon1DeltaR[nJets]/F");                 		       
  EventInfo->Branch(  "muon1EtaRel",   	   muon1EtaRel,              "muon1EtaRel[nJets]/F");                 		       
  EventInfo->Branch(  "muon1Ratio",    	   muon1Ratio,               "muon1Ratio[nJets]/F");                  		    
  EventInfo->Branch(  "muon1TrackQuality",      muon1TrackQuality,        "muon1TrackQuality[nJets]/I");           		       
  EventInfo->Branch(  "muon1RatioRel",          muon1RatioRel,            "muon1RatioRel[nJets]/F");                           


  EventInfo->Branch(  "muon2IsGlobal",          muon2IsGlobal,            "muon2IsGlobal[nJets]/O");            
  EventInfo->Branch(  "muon2IsTracker", 	   muon2IsTracker,           "muon2IsTracker[nJets]/O");           
  EventInfo->Branch(  "muon2IsStandalone", 	   muon2IsStandalone,        "muon2IsStandalone[nJets]/O");        
  EventInfo->Branch(  "muon2Pt",    		   muon2Pt,                  "muon2Pt[nJets]/F");                     	       
  EventInfo->Branch(  "muon2Eta",   		   muon2Eta,                 "muon2Eta[nJets]/F");                    	       
  EventInfo->Branch(  "muon2Phi",   		   muon2Phi,                 "muon2Phi[nJets]/F");                 
  EventInfo->Branch(  "muon2NumberOfMatches",   muon2NumberOfMatches,     "muon2NumberOfMatches[nJets]/I");     
  EventInfo->Branch(  "muon2GlobalMuonHits",    muon2GlobalMuonHits,      "muon2GlobalMuonHits[nJets]/I");      
  EventInfo->Branch(  "muon2InnerValidHits",    muon2InnerValidHits,      "muon2InnerValidHits[nJets]/I");      
  EventInfo->Branch(  "muon2NExpectedOuterHits",muon2NExpectedOuterHits,  "muon2NExpectedOuterHits[nJets]/I");    			       
  EventInfo->Branch(  "muon2NPixelHits", 	   muon2NPixelHits,          "muon2NPixelHits[nJets]/I");          
  EventInfo->Branch(  "muon2InnerNChi2", 	   muon2InnerNChi2,          "muon2InnerNChi2[nJets]/F");          
  EventInfo->Branch(  "muon2GlobalNChi2", 	   muon2GlobalNChi2,         "muon2GlobalNChi2[nJets]/F");         
  EventInfo->Branch(  "muon2VzPVDist", 	   muon2VzPVDist,            "muon2VzPVDist[nJets]/F");            
  EventInfo->Branch(  "muon2PtRel",    	   muon2PtRel,               "muon2PtRel[nJets]/F");               
  EventInfo->Branch(  "muon2Sip2d",    	   muon2Sip2d,               "muon2Sip2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon2Ip2d",    	   muon2Ip2d,                "muon2Ip2d[nJets]/F");                    
  EventInfo->Branch(  "muon2Ipe2d",    	   muon2Ipe2d,               "muon2Ipe2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon2Sip3d", 		   muon2Sip3d,               "muon2Sip3d[nJets]/F");               
  EventInfo->Branch(  "muon2Ip3d", 		   muon2Ip3d,                "muon2Ip3d[nJets]/F");                
  EventInfo->Branch(  "muon2Ipe3d",    	   muon2Ipe3d,               "muon2Ipe3d[nJets]/F");               
  EventInfo->Branch(  "muon2P0Par",    	   muon2P0Par,               "muon2P0Par[nJets]/F");                  		       
  EventInfo->Branch(  "muon2DeltaR",   	   muon2DeltaR,              "muon2DeltaR[nJets]/F");                 		       
  EventInfo->Branch(  "muon2EtaRel",   	   muon2EtaRel,              "muon2EtaRel[nJets]/F");                 		       
  EventInfo->Branch(  "muon2Ratio",    	   muon2Ratio,               "muon2Ratio[nJets]/F");                  		    
  EventInfo->Branch(  "muon2TrackQuality",      muon2TrackQuality,        "muon2TrackQuality[nJets]/I");           		       
  EventInfo->Branch(  "muon2RatioRel",          muon2RatioRel,            "muon2RatioRel[nJets]/F");                           


  EventInfo->Branch(  "muon3IsGlobal",          muon3IsGlobal,            "muon3IsGlobal[nJets]/O");            
  EventInfo->Branch(  "muon3IsTracker", 	   muon3IsTracker,           "muon3IsTracker[nJets]/O");           
  EventInfo->Branch(  "muon3IsStandalone", 	   muon3IsStandalone,        "muon3IsStandalone[nJets]/O");        
  EventInfo->Branch(  "muon3Pt",    		   muon3Pt,                  "muon3Pt[nJets]/F");                     	       
  EventInfo->Branch(  "muon3Eta",   		   muon3Eta,                 "muon3Eta[nJets]/F");                    	       
  EventInfo->Branch(  "muon3Phi",   		   muon3Phi,                 "muon3Phi[nJets]/F");                 
  EventInfo->Branch(  "muon3NumberOfMatches",   muon3NumberOfMatches,     "muon3NumberOfMatches[nJets]/I");     
  EventInfo->Branch(  "muon3GlobalMuonHits",    muon3GlobalMuonHits,      "muon3GlobalMuonHits[nJets]/I");      
  EventInfo->Branch(  "muon3InnerValidHits",    muon3InnerValidHits,      "muon3InnerValidHits[nJets]/I");      
  EventInfo->Branch(  "muon3NExpectedOuterHits",muon3NExpectedOuterHits,  "muon3NExpectedOuterHits[nJets]/I");    			       
  EventInfo->Branch(  "muon3NPixelHits", 	   muon3NPixelHits,          "muon3NPixelHits[nJets]/I");          
  EventInfo->Branch(  "muon3InnerNChi2", 	   muon3InnerNChi2,          "muon3InnerNChi2[nJets]/F");          
  EventInfo->Branch(  "muon3GlobalNChi2", 	   muon3GlobalNChi2,         "muon3GlobalNChi2[nJets]/F");         
  EventInfo->Branch(  "muon3VzPVDist", 	   muon3VzPVDist,            "muon3VzPVDist[nJets]/F");            
  EventInfo->Branch(  "muon3PtRel",    	   muon3PtRel,               "muon3PtRel[nJets]/F");               
  EventInfo->Branch(  "muon3Sip2d",    	   muon3Sip2d,               "muon3Sip2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon3Ip2d",    	   muon3Ip2d,                "muon3Ip2d[nJets]/F");                    
  EventInfo->Branch(  "muon3Ipe2d",    	   muon3Ipe2d,               "muon3Ipe2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon3Sip3d", 		   muon3Sip3d,               "muon3Sip3d[nJets]/F");               
  EventInfo->Branch(  "muon3Ip3d", 		   muon3Ip3d,                "muon3Ip3d[nJets]/F");                
  EventInfo->Branch(  "muon3Ipe3d",    	   muon3Ipe3d,               "muon3Ipe3d[nJets]/F");               
  EventInfo->Branch(  "muon3P0Par",    	   muon3P0Par,               "muon3P0Par[nJets]/F");                  		       
  EventInfo->Branch(  "muon3DeltaR",   	   muon3DeltaR,              "muon3DeltaR[nJets]/F");                 		       
  EventInfo->Branch(  "muon3EtaRel",   	   muon3EtaRel,              "muon3EtaRel[nJets]/F");                 		       
  EventInfo->Branch(  "muon3Ratio",    	   muon3Ratio,               "muon3Ratio[nJets]/F");                  		    
  EventInfo->Branch(  "muon3TrackQuality",      muon3TrackQuality,        "muon3TrackQuality[nJets]/I");           		       
  EventInfo->Branch(  "muon3RatioRel",          muon3RatioRel,            "muon3RatioRel[nJets]/F");                           


  EventInfo->Branch(  "muon4IsGlobal",          muon4IsGlobal,            "muon4IsGlobal[nJets]/O");            
  EventInfo->Branch(  "muon4IsTracker", 	   muon4IsTracker,           "muon4IsTracker[nJets]/O");           
  EventInfo->Branch(  "muon4IsStandalone", 	   muon4IsStandalone,        "muon4IsStandalone[nJets]/O");        
  EventInfo->Branch(  "muon4Pt",    		   muon4Pt,                  "muon4Pt[nJets]/F");                     	       
  EventInfo->Branch(  "muon4Eta",   		   muon4Eta,                 "muon4Eta[nJets]/F");                    	       
  EventInfo->Branch(  "muon4Phi",   		   muon4Phi,                 "muon4Phi[nJets]/F");                 
  EventInfo->Branch(  "muon4NumberOfMatches",   muon4NumberOfMatches,     "muon4NumberOfMatches[nJets]/I");     
  EventInfo->Branch(  "muon4GlobalMuonHits",    muon4GlobalMuonHits,      "muon4GlobalMuonHits[nJets]/I");      
  EventInfo->Branch(  "muon4InnerValidHits",    muon4InnerValidHits,      "muon4InnerValidHits[nJets]/I");      
  EventInfo->Branch(  "muon4NExpectedOuterHits",muon4NExpectedOuterHits,  "muon4NExpectedOuterHits[nJets]/I");    			       
  EventInfo->Branch(  "muon4NPixelHits", 	   muon4NPixelHits,          "muon4NPixelHits[nJets]/I");          
  EventInfo->Branch(  "muon4InnerNChi2", 	   muon4InnerNChi2,          "muon4InnerNChi2[nJets]/F");          
  EventInfo->Branch(  "muon4GlobalNChi2", 	   muon4GlobalNChi2,         "muon4GlobalNChi2[nJets]/F");         
  EventInfo->Branch(  "muon4VzPVDist", 	   muon4VzPVDist,            "muon4VzPVDist[nJets]/F");            
  EventInfo->Branch(  "muon4PtRel",    	   muon4PtRel,               "muon4PtRel[nJets]/F");               
  EventInfo->Branch(  "muon4Sip2d",    	   muon4Sip2d,               "muon4Sip2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon4Ip2d",    	   muon4Ip2d,                "muon4Ip2d[nJets]/F");                    
  EventInfo->Branch(  "muon4Ipe2d",    	   muon4Ipe2d,               "muon4Ipe2d[nJets]/F");                  	       
  EventInfo->Branch(  "muon4Sip3d", 		   muon4Sip3d,               "muon4Sip3d[nJets]/F");               
  EventInfo->Branch(  "muon4Ip3d", 		   muon4Ip3d,                "muon4Ip3d[nJets]/F");                
  EventInfo->Branch(  "muon4Ipe3d",    	   muon4Ipe3d,               "muon4Ipe3d[nJets]/F");               
  EventInfo->Branch(  "muon4P0Par",    	   muon4P0Par,               "muon4P0Par[nJets]/F");                  		       
  EventInfo->Branch(  "muon4DeltaR",   	   muon4DeltaR,              "muon4DeltaR[nJets]/F");                 		       
  EventInfo->Branch(  "muon4EtaRel",   	   muon4EtaRel,              "muon4EtaRel[nJets]/F");                 		       
  EventInfo->Branch(  "muon4Ratio",    	   muon4Ratio,               "muon4Ratio[nJets]/F");                  		    
  EventInfo->Branch(  "muon4TrackQuality",      muon4TrackQuality,        "muon4TrackQuality[nJets]/I");           		       
  EventInfo->Branch(  "muon4RatioRel",          muon4RatioRel,            "muon4RatioRel[nJets]/F");                           

  for(map< string, Float_t* >::iterator it = bTagArrays.begin(); it!= bTagArrays.end(); it++){
    TString name = it->first;
    TString nameExt = name;
    nameExt += "[nJets]/F";
    EventInfo->Branch(  name, it->second, nameExt ); 
  }
  //Build the run information TTree. It will be filled once per run.
  HLTTableName = new string();
  RunInfo->Branch("RunNumber",&runNumber,"RunNum/i");
  RunInfo->Branch("TableName",&HLTTableName);
  HLTNamesSet = new vector<string>();
  RunInfo->Branch("HLTNamesSet",&HLTNamesSet);
  firstEvent_=true;
}


TagNtupleProducer::~TagNtupleProducer()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
}

//
// member functions
//

// ------------ method called to produce the data  ------------
void TagNtupleProducer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace std;
  using namespace reco;

  if (firstEvent_) {
    if ( iEvent.isRealData() ) {
      getMCPUInfo_=false;
      getMCTruth_=false;
      getSimTruth_=false;
    }
    else {
      try{
	Handle<std::vector< PileupSummaryInfo > >  puInfo;
	getMCPUInfo_=iEvent.getByLabel( InputTag("addPileupInfo"), puInfo);
      }
      catch (cms::Exception) {
	LogWarning("DataFormat")<< "MC Pileup information is not found.(BadTag?)";
	getMCPUInfo_=false;
      }
      try{
	Handle<JetFlavourMatchingCollection> jetMC;
	Handle< GenEventInfoProduct   > genInfo;
      	getMCTruth_=iEvent.getByLabel(jet_MC_src_,jetMC)&&iEvent.getByLabel("generator",genInfo);
      }
      catch (cms::Exception) {
	LogWarning("DataFormat")<< "Generator level information is not found.(BadTag?)";
	getMCTruth_=false;
      }
      try{
	Handle<SimTrackContainer> STCollectionH;
	Handle<SimVertexContainer> SVCollectionH;
	Handle<HepMCProduct> HepMCH;
    	getSimTruth_=iEvent.getByLabel(simTruthTag_, STCollectionH)&&iEvent.getByLabel(simTruthTag_, SVCollectionH)&&iEvent.getByLabel(HepMCTag_, HepMCH);
      }
      catch (cms::Exception) {
	LogWarning("DataFormat")<< "Simulation level information is not found.(BadTag? or AOD?)";
	getSimTruth_=false;
      }
    }
    firstEvent_=false;
  }
  HLTriggerResults->clear(); HLTPrescaleFactors->clear();
  
  //Get The Various collections defined in the configuration file
  edm::Handle<edm::TriggerResults>  hltresults;
  iEvent.getByLabel(triggerTag_, hltresults);

  for ( vector<Int_t>::const_iterator iter=HLTTableBitMatch_.begin();iter!=HLTTableBitMatch_.end();iter++)
    HLTriggerResults->push_back( hltresults->accept(*iter) );
  //According to https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideHighLevelTrigger#HLT_Prescales
  //For MC, all prescale factors are ONE, so we just skip it to save the space. Do not try to access prescale factors in the MC ntuples.
  if ( iEvent.isRealData() )
    for ( vector<string>::const_iterator iter=HLTNamesSet->begin();iter!=HLTNamesSet->end();iter++)
      HLTPrescaleFactors->push_back( hltConfigProvider_.prescaleValue(iEvent, iSetup, *iter) );
  
  eventNumber = iEvent.eventAuxiliary().event();
  lumiBlockNumber = iEvent.eventAuxiliary().luminosityBlock();

  Handle< View<Jet> > jets;
  iEvent.getByLabel(jet_src_,jets);

  Handle<JetTracksAssociation::Container > jetTracks;
  iEvent.getByLabel(jet_tracks_,jetTracks);

  Handle<reco::VertexCollection> primaryVertex;
  iEvent.getByLabel(primaryVertexProducer_, primaryVertex);

  // offlinePrimaryVertices are already selected with
  // ndof > 4 && abs(z) <= 24 && position.Rho <= 2
  numberOfPrimaryVertices = primaryVertex->size();
  
  if (getMCPUInfo_) {
    edm::InputTag PileupSrc_("addPileupInfo");

#ifdef GetPUFromEarlyThan_4_1_2
    Handle<std::vector< PileupSummaryInfo > >  puInfo;
    if ( iEvent.getByLabel(PileupSrc_, puInfo) ) {
      numberOfPUVertices = (*puInfo)[0].getPU_NumInteractions();
      // std::cout<<" numberOfPUVertices = " << numberOfPUVertices << std::endl;
    }
    numberOfPUVerticesMixingTruth=numberOfPUVertices;
    numberOfPUVerticesTot=numberOfPUVertices;
#else
     Handle<std::vector< PileupSummaryInfo > >  PupInfo;
     iEvent.getByLabel(PileupSrc_, PupInfo);
     std::vector<PileupSummaryInfo>::const_iterator PVI;
     numberOfPUVertices=0; // the number of pileup interactions that have been added to the event from this bunch crossing
     numberOfPUVerticesMixingTruth=0;// the *true* mean number of pileup interactions for this event from which each bunch crossing has been sampled; same for all bunch crossings in an event
     numberOfPUVerticesTot=0;
     for(PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
       Int_t BX = PVI->getBunchCrossing();
       if(BX == 0) {
	 numberOfPUVertices = PVI->getPU_NumInteractions();
#ifdef GetPUFromEarlyThan_4_4_0
	 numberOfPUVerticesMixingTruth = -1.;//not available
#else
	 numberOfPUVerticesMixingTruth = PVI->getTrueNumInteractions();
#endif
       } 
       numberOfPUVerticesTot+=numberOfPUVertices;
     }
#endif
  }
  
  edm::Handle<reco::BeamSpot> recoBeamSpotHandle;
  iEvent.getByLabel(BeamSpotTag_,recoBeamSpotHandle);

  edm::ESHandle<TransientTrackBuilder> builder;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder", builder);

  //Get the TagInfo stuff and make is accesible with a map to a RefToBase<Jet>, since that's apparently what the JetTags use
  typedef RefToBase<Jet> JetRef;
  typedef map<JetRef, const SoftLeptonTagInfo*, JetRefCompare> slTagInfoMap;
  slTagInfoMap softElectronTagInfo;
  edm::Handle< View < reco::SoftLeptonTagInfo > > SoftElectronTagInfoVector;
 
  iEvent.getByLabel(electron_tag_infos_,SoftElectronTagInfoVector);
  for(View< reco::SoftLeptonTagInfo >::const_iterator iTagInfo = SoftElectronTagInfoVector->begin(); iTagInfo != SoftElectronTagInfoVector->end(); iTagInfo++) 
    {
      softElectronTagInfo[iTagInfo->jet()] = &*iTagInfo;
    }
    

  slTagInfoMap softMuonTagInfo;
  edm::Handle< View < reco::SoftLeptonTagInfo > > SoftMuonTagInfoVector;
 
  iEvent.getByLabel(muon_tag_infos_,SoftMuonTagInfoVector);
  for(View< reco::SoftLeptonTagInfo >::const_iterator iTagInfo = SoftMuonTagInfoVector->begin(); iTagInfo != SoftMuonTagInfoVector->end(); iTagInfo++) 
    {
      softMuonTagInfo[iTagInfo->jet()] = &*iTagInfo;
    }
    

  typedef map< JetRef, const SecondaryVertexTagInfo*, JetRefCompare > svTagInfoMap;
  svTagInfoMap svTagInfo;
  edm::Handle< View < reco::SecondaryVertexTagInfo > > svTagInfoVector;

  iEvent.getByLabel(SV_tag_infos_,svTagInfoVector);
  for(View< reco::SecondaryVertexTagInfo >::const_iterator iTagInfo = svTagInfoVector->begin(); iTagInfo != svTagInfoVector->end(); iTagInfo++) 
    {
      svTagInfo[iTagInfo->jet()] = &*iTagInfo;
    }
    
  typedef map<JetRef, const TrackIPTagInfo*, JetRefCompare> ipTagInfoMap;
  ipTagInfoMap ipTagInfo;
  edm::Handle< View < reco::TrackIPTagInfo > > ipTagInfoVector;

  iEvent.getByLabel(IP_tag_infos_,ipTagInfoVector);  
  for(View< reco::TrackIPTagInfo >::const_iterator iTagInfo = ipTagInfoVector->begin(); iTagInfo != ipTagInfoVector->end(); iTagInfo++) 
    {
      ipTagInfo[iTagInfo->jet()] = &*iTagInfo;
    }
    
  //Parton Container:Get the MC Truth Matching (if requested) and define a map from RefToBase<Jet> to the flavor; if MC Truth not requested, fill with -1
  typedef map<JetRef, const reco::JetFlavour *, JetRefCompare> TruthMap;
  edm::Handle<JetFlavourMatchingCollection> jetMC;
  TruthMap JetFlavor;
  //GenJet Container
  edm::Handle< edm::Association<reco::GenJetCollection> >GenJetMatch;

  if(getMCTruth_)
    {
      //Get GenParton
      iEvent.getByLabel(jet_MC_src_, jetMC);      
      for (JetFlavourMatchingCollection::const_iterator iFlav = jetMC->begin(); iFlav != jetMC->end(); iFlav++) 
	JetFlavor[iFlav->first] = &iFlav->second;
      // fill pt hat
      pthat = -1;
      mcweight=1.;
      Handle< GenEventInfoProduct   > genInfo;
      iEvent.getByLabel("generator", genInfo  );      
      if (genInfo.isValid()) {
	if( genInfo->hasBinningValues() )
	  if( genInfo->binningValues().size() > 0 ){
	    pthat = genInfo->binningValues()[0];  
	  }
        mcweight=genInfo->weight();
      }
      //Get GenJet
      iEvent.getByLabel( GenJet_Matcher_, GenJetMatch );
    } 
  else 
    {
      for (UInt_t iJet = 0; iJet < jets->size(); iJet++)
	JetFlavor[RefToBase<Jet>(jets, iJet)] = NULL;
      pthat = -2;
      mcweight=1.;
    }
  //SimTrack,SimVertex Container, we build a map from SimTrack to its TrackParameter Vector here
  //if sample is AODSIM, getSimTruth_ must be false
  map< vector<SimTrack>::iterator,reco::TrackBase::ParameterVector > SimTrackParameterVectorMap;
  SimTrackContainer STC;
  SimVertexContainer SVC;
  HepMC::GenEvent *HepGenEvent=NULL;
  if (getSimTruth_) {
    //SimTrack and SimVertex
    Handle<SimTrackContainer> STCollectionH;
    Handle<SimVertexContainer> SVCollectionH;
    Handle<edm::HepMCProduct> HepMCH;
    iEvent.getByLabel(simTruthTag_, STCollectionH);
    iEvent.getByLabel(simTruthTag_, SVCollectionH);
    STC = *STCollectionH.product();
    SVC = *SVCollectionH.product();
    iEvent.getByLabel(HepMCTag_, HepMCH);
    HepGenEvent=const_cast<HepMC::GenEvent *>( HepMCH->GetEvent() );
    for (vector<SimTrack>::iterator Trk_iter = STC.begin(); Trk_iter != STC.end(); ++Trk_iter ) {
      Basic3DVector<Double_t> momAtVtx( Trk_iter->momentum().x(),Trk_iter->momentum().y(), Trk_iter->momentum().z() );
      Basic3DVector<Double_t> vert;
      if ( !Trk_iter->noVertex() ) {
	SimVertex *thisVtx=&SVC[Trk_iter->vertIndex()];
	vert=Basic3DVector<Double_t>( thisVtx->position().x(),thisVtx->position().y(),thisVtx->position().z() );
      }
      else {
	edm::LogWarning("SimBug")<<"SimTrack has no SimVertex. Consider the SimTrack is from (0,0,0)"<<endl;
	vert=Basic3DVector<Double_t>( 0,0,0 );
      }
      std::pair<Bool_t,reco::TrackBase::ParameterVector> params = parametersAtClosestApproach(vert, momAtVtx, Trk_iter->charge(), *recoBeamSpotHandle);
      if (params.first) SimTrackParameterVectorMap[Trk_iter]=params.second;
    }
  }

  // determine gluon splitting
  isBGluonSplitting = 0;
  isCGluonSplitting = 0;
  if(getMCTruth_) {
    Bool_t bFoundS3BQuark = false;
    Bool_t bFoundS2BQuark = false;
    Bool_t bFoundS3CQuark = false;
    Bool_t bFoundS2CQuark = false;
    edm::Handle<GenParticleCollection> genParticles;
    iEvent.getByLabel("genParticles" , genParticles );
    for( size_t i = 0; i < genParticles->size(); ++ i ) {
      const GenParticle & genCand = (*genParticles)[ i ];
      Int_t MC_particleID=abs(genCand.pdgId());
      if(genCand.status() == 3 && MC_particleID ==5){
	bFoundS3BQuark = true;
      }
      if(genCand.status() == 2 && MC_particleID ==5){
	bFoundS2BQuark = true;
      }
      if(genCand.status() == 3 && MC_particleID ==4){
	bFoundS3CQuark = true;
      }
      if(genCand.status() == 2 && MC_particleID ==4){
	bFoundS2CQuark = true;
      }
    }
    // if no status 3 b quark but status 2
    if( (!bFoundS3BQuark) && bFoundS2BQuark  ) isBGluonSplitting  = 1;
    // if no status 3 c quark but status 2
    if( (!bFoundS3CQuark) && bFoundS2CQuark  ) isCGluonSplitting  = 1;
  }

  //Get the btaggers that are defined in the configuration file

  map< string, reco::JetTagCollection > bTags;

  for (vector< ParameterSet >::iterator ibTag = bTag_Config_.begin(); ibTag != bTag_Config_.end(); ibTag++) {
    edm::Handle<reco::JetTagCollection> bTagHandle;
    iEvent.getByLabel(ibTag->getParameter<InputTag>("label"), bTagHandle);
    bTags[ibTag->getParameter<string>("alias")] = *bTagHandle;
  }
  
  //instantiate a tagging variable computer for unification of some calculations like vertex mass corrections
  edm::ESHandle<JetTagComputer> computerHandle;;
  iSetup.get<JetTagComputerRecord>().get( SVComputer_.label(), computerHandle );
  const GenericMVAJetTagComputer *computer =
    dynamic_cast<const GenericMVAJetTagComputer*>( computerHandle.product() );
  if (!computer){
    edm::LogError("DataLost")<<"computer missing !!!"<<endl;
    exit(1);
  }

  computer->passEventSetup(iSetup);
  
  Int_t iTotalTracksCounter = 0;
  nJets = jets->size();
  if(nJets > (Int_t)MAXJETS) {
    edm::LogError("DataLost")<<"MAXJETS is not big enough. Current nJets="<<nJets<<endl;
    return;
  }

  //Begin Loop over Jets and record the various information
  for (Int_t iJet = 0; iJet<nJets; iJet++)
    {

      JetRef thisJetRef = RefToBase<Jet>(jets, iJet);
 
      //Basic Jet Information
      jetP4[iJet] = (thisJetRef->p4());               
      jetPt[iJet] = thisJetRef->p4().Pt();                              
      jetEta[iJet] = (thisJetRef->p4().Eta());
      jetPhi[iJet] = (thisJetRef->p4().Phi());
      //Store some Calo Jet Information:
      if( const CaloJet* caloJet = dynamic_cast<const CaloJet*>(&(*(thisJetRef.get()) )) )
	jetEMFraction[iJet] = (caloJet->emEnergyFraction());
      else jetEMFraction[iJet] = (-1.0);

      //Store some Particle Flow Jet Information
      if(const PFJet* pfJet = dynamic_cast<const PFJet*>(&(*(thisJetRef.get()) )))
	{
	  jetChargedEmEnergyFraction[iJet] = (pfJet->chargedEmEnergyFraction());
	  jetNeutralEmEnergyFraction[iJet] = (pfJet->neutralEmEnergyFraction());
	  jetChargedHadronEnergyFraction[iJet] = (pfJet->chargedHadronEnergyFraction());
	  jetNeutralHadronEnergyFraction[iJet] = (pfJet->neutralHadronEnergyFraction());
	  jetChargedMultiplicity[iJet] = (pfJet->chargedMultiplicity());
	}
      else
	{
	  jetChargedEmEnergyFraction[iJet] = (-1.0);
	  jetNeutralEmEnergyFraction[iJet] = (-1.0);
	  jetChargedHadronEnergyFraction[iJet] = (-1.0);
	  jetNeutralHadronEnergyFraction[iJet] = (-1.0);
	  jetChargedMultiplicity[iJet] = (-1.0);
	}
      jetMass[iJet] = (thisJetRef->p4().M());
      jetnConstituents[iJet] = (thisJetRef->nConstituents());
      jetnTracks[iJet] = ((*jetTracks)[thisJetRef].size());

      jetVertex[iJet] = (math::XYZVector(thisJetRef->vx(),thisJetRef->vy(),thisJetRef->vz()));
      jetVertexChi2[iJet] = (thisJetRef->vertexChi2());
      jetVertexChi2Ndof[iJet] = (thisJetRef->vertexNdof());
      jetVertexNormalizedChi2[iJet] = (thisJetRef->vertexNormalizedChi2());

      //Stolen from CMSSW/RecoBTag/ImpactParameter/plugins/TrackIPProducer.cc
      Vertex dummy;
      const Vertex *pv = &dummy;
      edm::Ref<VertexCollection> pvRef;
      if (primaryVertex->size() != 0) {
	pv = &*primaryVertex->begin();
	// we always use the first vertex (at the moment)
	pvRef = edm::Ref<VertexCollection>(primaryVertex, 0);
      } else { // create a dummy PV
	Vertex::Error e;
	e(0, 0) = 0.0015 * 0.0015;
	e(1, 1) = 0.0015 * 0.0015;
	e(2, 2) = 15. * 15.;
	Vertex::Point p(0, 0, 0);
	dummy = Vertex(p, e, 0, 0, 0);
      }
      //End Stealing
      
      // vertex properties
      numberOfTracksAtPV = pv->nTracks();
      PVx = pv->x();
      PVy = pv->y();
      PVz = pv->z();
      PVChi2 = pv->chi2();
      PVndof = pv->ndof();
      PVNormalizedChi2 = pv->normalizedChi2();
      //some counters
      size_t nSelectedTracks=0;
      size_t nSelectedAndDecayLengthAndJetAsixTracks=0;

      if ( iTotalTracksCounter+(*jetTracks)[thisJetRef].size() >= (Int_t)MAXTRACKS) {
   	     for (Int_t jJet = iJet; jJet<nJets; jJet++)
           iTotalTracksCounter+=(*jetTracks)[RefToBase<Jet>(jets, jJet)].size();
	     edm::LogError("DataLost")<<"MAXTRACKS is not big enough. Current nTotalTracks="<<iTotalTracksCounter<<endl;
	     return;
	  }
      //jet axis of the selected tracks jet
      const GlobalVector jetAxis = ipTagInfo[thisJetRef]->axis();
      const math::XYZVector axis( jetAxis.x(), jetAxis.y(), jetAxis.z());
      //jet direction
      GlobalVector direction(thisJetRef->momentum().x(), thisJetRef->momentum().y(), thisJetRef->momentum().z());

      typedef  vector< pair<track_iterator,Measurement1D> > trackSortedQueueVec;
      trackSortedQueueVec trackSortedQueue;
      for(track_iterator iTrack = (*jetTracks)[thisJetRef].begin(); iTrack != (*jetTracks)[thisJetRef].end(); iTrack++) {
	Measurement1D ip3d = IPTools::signedImpactParameter3D( builder->build(*iTrack), direction, *pv).second;
	trackSortedQueue.push_back( make_pair(iTrack,ip3d) );
      }
      
      if ( !trackSortedQueue.empty() )
	for(trackSortedQueueVec::iterator iTrack = trackSortedQueue.begin(); iTrack != trackSortedQueue.end()-1; iTrack++)
	  for (trackSortedQueueVec::iterator jTrack=iTrack+1; jTrack != trackSortedQueue.end(); jTrack++) 
	    if ( (*iTrack).second.value()<(*jTrack).second.value() ) swap(*iTrack,*jTrack);

      RefVector<TrackCollection> SelectedBTaggingTracks=ipTagInfo[thisJetRef]->selectedTracks();
      for(trackSortedQueueVec::const_iterator iTrack_iter = trackSortedQueue.begin(); iTrack_iter != trackSortedQueue.end(); iTrack_iter++)
	{ 
	  track_iterator iTrack=iTrack_iter->first;
	  trackJetIndex[iTotalTracksCounter] = iJet;
	  Bool_t isSelected = false;
	  trackIsFromPU[iTotalTracksCounter] = false;
	  trackIsFromB[iTotalTracksCounter] = false;
	  trackIP3dProbability[iTotalTracksCounter] = -9999.;
	  trackIP2dProbability[iTotalTracksCounter] = -9999.;
	  
	  for(Byte_t jTrack = 0; jTrack<SelectedBTaggingTracks.size(); jTrack ++ ) {
	    if( (*iTrack)==SelectedBTaggingTracks[jTrack] ) {
	      trackIP3dProbability[iTotalTracksCounter] = (ipTagInfo[thisJetRef]->probabilities(0)[jTrack]);
	      trackIP2dProbability[iTotalTracksCounter] = (ipTagInfo[thisJetRef]->probabilities(1)[jTrack]);
	      isSelected = true;
	      break;
	    }
	  }
	  
	  trackPtRel[iTotalTracksCounter] = ( Perp( math::XYZVector( (*iTrack)->px(),(*iTrack)->py(),(*iTrack)->pz() ), 
						    axis) );
	  
	  if (getSimTruth_) {//start investigating if the track is from PU
	    TrackBase::ParameterVector rParameters = (*iTrack)->parameters();
	    TrackBase::CovarianceMatrix recoTrackCovMatrix = (*iTrack)->covariance();
	    recoTrackCovMatrix.Invert();
	    vector<SimTrack>::iterator BestMatchSimTrk=STC.end();
	    Double_t BestChi2=1E30;
	    for (vector<SimTrack>::iterator Trk_iter = STC.begin(); Trk_iter != STC.end(); ++Trk_iter ) {
	      reco::TrackBase::ParameterVector sParameters=SimTrackParameterVectorMap[Trk_iter];
	      reco::TrackBase::ParameterVector diffParameters = rParameters - sParameters;
	      diffParameters[2] = reco::deltaPhi(diffParameters[2],0.f);
	      Double_t chi2 = ROOT::Math::Dot(diffParameters * recoTrackCovMatrix, diffParameters);
	      if (chi2<chi2cut_&&chi2<BestChi2) {
		BestChi2=chi2;
		BestMatchSimTrk=Trk_iter;
	      }
	    }
	    if ( BestMatchSimTrk!=STC.end() ) {
	      vector<SimTrack>::iterator thisTrk=BestMatchSimTrk;
	      SimVertex *thisVtx=NULL;//the very beginning vertex of this track
	      do {
		if (thisTrk->noVertex()) break;
		thisVtx=&SVC[thisTrk->vertIndex()];
		if (thisVtx->noParent()) break;
		Int_t TrkID =Int_t( thisVtx->parentIndex() );
		thisTrk = STC.begin();
		for (; thisTrk != STC.end(); ++thisTrk )
		  if ( (Int_t) thisTrk->trackId() == TrkID) break;
		if ( thisTrk==STC.end() ) {
		  edm::LogError("SimBug")<<"parentIndex/trackId Error, considered as no parent track"<<endl;
		  break;
		}
	      }while(true);
	      
	      if (thisVtx==NULL) trackIsFromPU[iTotalTracksCounter]=true;
	      if ( thisTrk==STC.end() ) trackIsFromPU[iTotalTracksCounter]=true;
	      else if ( thisTrk->noGenpart() ) trackIsFromPU[iTotalTracksCounter]=true;
	      else trackIsFromB[iTotalTracksCounter]=GenParticleParentTree( HepGenEvent->barcode_to_particle(thisTrk->genpartIndex()) );

	      if (trackIsFromPU[iTotalTracksCounter]&&numberOfPUVertices==0) {
		trackIsFromPU[iTotalTracksCounter]=false;
		edm::LogError("SimBug")<<"Found a PU track in PU0 event."<<endl;
	      }
	    }
	    else {
	      trackIsFromPU[iTotalTracksCounter]=false;
	      edm::LogInfo("MCMatching")<<"No matched SimTrack. Increase TrackMCTruthMatchChi2Cut??"<<endl;
	    }
	  }//end of getSimTruth

	  if(isSelected) nSelectedTracks++;
	  
	  // check if track is attached to a vertex
	  trackIsVertexTrack[iTotalTracksCounter] = false;
	  if( svTagInfo[thisJetRef]->nVertices() > 0 ) {
	    const Vertex &sv = svTagInfo[thisJetRef]->secondaryVertex(0);
	    for(Vertex::trackRef_iterator iter = sv.tracks_begin();
		iter != sv.tracks_end(); iter++) {
	      if (sv.trackWeight(*iter) < 0.5) continue;
	      
	      TrackRef vtxTrackRef =  (*iter).castTo<TrackRef>();
	      
	      if( vtxTrackRef == (*iTrack) ) trackIsVertexTrack[iTotalTracksCounter] = true;
	    }
	  }

	  trackSelected[iTotalTracksCounter] = (isSelected);
	  track3Momentum[iTotalTracksCounter] = (math::XYZVector((*iTrack)->px(),(*iTrack)->py(),(*iTrack)->pz()));
	  trackTransverseMomentum[iTotalTracksCounter] = ((*iTrack)->pt());
	  trackEta[iTotalTracksCounter] = ((*iTrack)->eta());
	  trackPhi[iTotalTracksCounter] = ((*iTrack)->phi());
	  trackMomentum[iTotalTracksCounter] = ((*iTrack)->p());
	  trackNHits[iTotalTracksCounter] = ((*iTrack)->hitPattern().numberOfValidHits());
	  trackNPixelHits[iTotalTracksCounter] = ((*iTrack)->hitPattern().numberOfValidPixelHits());
	  trackChi2[iTotalTracksCounter] = ((*iTrack)->chi2());
	  trackNormChi2[iTotalTracksCounter] = ((*iTrack)->normalizedChi2());
	  //trackQuality[iTotalTracksCounter] = ((*iTrack)->qualityMask());
	  //cout << (*iTrack)->qualityMask() << endl;
	  for(Int_t i = 2; i>-2; i--){
	    if((*iTrack)->quality(reco::TrackBase::TrackQuality(i))){
	      trackQuality[iTotalTracksCounter] = (i);
	      break;
	    }
	  }
	  trackLongitudinalImpactParameter[iTotalTracksCounter] = ((*iTrack)->dz(pv->position()));
  	  TransientTrack transientTrack = builder->build(*iTrack);
   	  Measurement1D ip2d = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second;
  	  trackIP3d[iTotalTracksCounter] = (iTrack_iter->second.value());
  	  trackIP2d[iTotalTracksCounter] = (ip2d.value());
  	  trackIP3dError[iTotalTracksCounter] = (iTrack_iter->second.error());
  	  trackIP2dError[iTotalTracksCounter] = (ip2d.error());
	  Double_t decayLength=-1;
	  TrajectoryStateOnSurface closest =
	    IPTools::closestApproachToJet(transientTrack.impactPointState(),
					  *pv, direction,
					  transientTrack.field());
	  if (closest.isValid()){
	    decayLength =  (closest.globalPosition()-   RecoVertex::convertPos(pv->position())).mag() ;
	  }
	  else{
	    decayLength = -1;
	  }
	  trackDecayLength[iTotalTracksCounter] = (decayLength);

	  Double_t distJetAxis =  IPTools::jetTrackDistance(transientTrack, direction, *pv).second.value();
	  trackDistJetAxis[iTotalTracksCounter] = ( distJetAxis );
	  trackDeltaR[iTotalTracksCounter] = ( ROOT::Math::VectorUtil::DeltaR(thisJetRef->momentum(),(*iTrack)->momentum()) );

	  if (isSelected && decayLength < 5 && fabs(distJetAxis) < 0.07) nSelectedAndDecayLengthAndJetAsixTracks++;

	  if(getSharedHitInfo_) {
	    Bool_t tsharedP1, tsharedP2, tsharedP3;
	    trackHasSharedPix1[iTotalTracksCounter] = ( tsharedP1 = hasSharedHit(1, iTrack, (*jetTracks)[thisJetRef]));
	    trackHasSharedPix2[iTotalTracksCounter] = ( tsharedP2 = hasSharedHit(2, iTrack, (*jetTracks)[thisJetRef]));
	    trackHasSharedPix3[iTotalTracksCounter] = ( tsharedP3 = hasSharedHit(3, iTrack, (*jetTracks)[thisJetRef]));
	    trackHasSharedPixAll[iTotalTracksCounter] = ( tsharedP1 || tsharedP2 || tsharedP3 );
	  }
	  else {
	    trackHasSharedPix1[iTotalTracksCounter] = false;
	    trackHasSharedPix2[iTotalTracksCounter] = false;
	    trackHasSharedPix3[iTotalTracksCounter] = false;
	    trackHasSharedPixAll[iTotalTracksCounter] = false;
	  }

	  iTotalTracksCounter++;
	}
      
      IPnSelectedTracks[iJet] = nSelectedTracks;                       
      IPnSelectedAndDecayLengthAndJetAsixTracks[iJet] = nSelectedAndDecayLengthAndJetAsixTracks;                       

      //MC Truth Information
      reco::GenJetRef GenJet;
      if (getMCTruth_) {
	PartonFlavor[iJet] = std::abs(JetFlavor[thisJetRef]->getFlavour());
	GenJet = (*GenJetMatch)[thisJetRef];
      }
      if (GenJet.isNonnull() && GenJet.isAvailable()) {
	GenJetPt[iJet] = GenJet->pt();
	GenJetEta[iJet] = GenJet->eta();
	GenJetPhi[iJet] = GenJet->phi();
	/* This part is only for studying GenJet. It can be deleted in the later versions.
	   A GenJet is not a jet generated by only one parton. It is formed by clustering all generated particles, like the reco jet. The difference is that the GenJet does not include particles from pileups since particles from pileups are not saved in the events.
	cout<<"##### "<<GenJetPt[iJet]<<","<<GenJetEta[iJet]<<","<<GenJetPhi[iJet]<<" #####"<<endl;
	std::vector <const GenParticle*> mcparts = GenJet->getGenConstituents();
	for ( std::vector <const GenParticle*>::const_iterator mcpart=mcparts.begin();mcpart!=mcparts.end();mcpart++) 
	  if ( (*mcpart)->numberOfMothers()>0 ) {
	    Candidate * mother=const_cast<Candidate *>( (*mcpart)->mother() );
	    while ( mother->numberOfMothers()>0 ) {
	      Int_t flavour=abs( mother->pdgId() );
	      if ( flavour == 1 || flavour == 2 || flavour == 3 || flavour == 4 || flavour == 5 || flavour == 21 ) break;//udscbg
	      mother=const_cast<Candidate *>( mother->mother() );
	    }
	    Float_t diffeta=mother->eta()-GenJetEta[iJet],diffphi=mother->phi()-GenJetPhi[iJet];
	    if ( mother->numberOfMothers()>0 && diffeta*diffeta+diffphi*diffphi<1.) cout<<mother->pdgId()<<":"<<mother->pt()<<","<<mother->eta()<<","<<mother->phi()<<endl;
	  }*/
      }
      else {
	GenJetPt[iJet] = -9999.;
	GenJetEta[iJet] = -9999.;
	GenJetPhi[iJet] =  -9999.;
      }
      //if flavor=0, that means no matched parton
      if (getMCTruth_&&JetFlavor[thisJetRef]->getFlavour()!=0) {
	math::XYZTLorentzVector partonMomentum = JetFlavor[thisJetRef]->getLorentzVector();
	PartonPt[iJet] = partonMomentum.Pt();
	PartonEta[iJet] = partonMomentum.Eta();
	PartonPhi[iJet] = partonMomentum.phi();
      }
      else {
	PartonFlavor[iJet] = -1;
	PartonPt[iJet] = -9999.;
	PartonEta[iJet] = -9999.;
	PartonPhi[iJet] = -9999.;
      }
      
      //secondaryVertexTagInfos:

      SVnSelectedTracks[iJet] = (svTagInfo[thisJetRef]->nSelectedTracks());  
      if( svTagInfo[thisJetRef]->nVertices() > 0 )
	{
	  // Compute tagging variables
	  std::vector<const reco::BaseTagInfo*>  baseTagInfos;
	  baseTagInfos.push_back( &(*ipTagInfo[thisJetRef]) ); 
	  baseTagInfos.push_back( &(*svTagInfo[thisJetRef]) );
	  JetTagComputer::TagInfoHelper helper(baseTagInfos);
	  TaggingVariableList vars = computer->taggingVariables(helper);
	  math::XYZVector thisVertex(svTagInfo[thisJetRef]->secondaryVertex(0).x(),svTagInfo[thisJetRef]->secondaryVertex(0).y(),svTagInfo[thisJetRef]->secondaryVertex(0).z());
	  
	  edm::RefToBase<Jet> jet = ipTagInfo[thisJetRef]->jet();
	  math::XYZVector jetDir = jet->momentum().Unit();
	  
	  SVnVertices[iJet] = (svTagInfo[thisJetRef]->nVertices());
	  SecondaryVertex[iJet] = (thisVertex);
	  SV3dDistance[iJet] = (svTagInfo[thisJetRef]->flightDistance(0).value());                          
	  SV3dDistanceError[iJet] = (svTagInfo[thisJetRef]->flightDistance(0).error());                     
	  SV2dDistance[iJet] = (svTagInfo[thisJetRef]->flightDistance(0, true).value());                          
	  SV2dDistanceError[iJet] = (svTagInfo[thisJetRef]->flightDistance(0, true).error());                     
	  SVChi2[iJet] = (svTagInfo[thisJetRef]->secondaryVertex(0).chi2());                                
	  SVDegreesOfFreedom[iJet] = (svTagInfo[thisJetRef]->secondaryVertex(0).ndof());                    
	  SVNormChi2[iJet] = (svTagInfo[thisJetRef]->secondaryVertex(0).normalizedChi2());
	  SVnFirstVertexTracksAll[iJet] = (svTagInfo[thisJetRef]->secondaryVertex(0).tracksSize());
	  SVnFirstVertexTracks[iJet] = ( svTagInfo[thisJetRef]->nVertexTracks(0)  );
	  SVnVertexTracks[iJet] = ( svTagInfo[thisJetRef]->nVertexTracks()  );
	  Int_t nVertexTrackSize = 0;
	  for(UInt_t i=0; i< svTagInfo[thisJetRef]->nVertices(); i++) nVertexTrackSize+= svTagInfo[thisJetRef]->secondaryVertex(i).tracksSize();
	  SVnVertexTracksAll[iJet] = ( nVertexTrackSize );
	  if(vars.checkTag(reco::btau::vertexMass)) SVMass[iJet] = ( vars.get( reco::btau::vertexMass));
	  else  SVMass[iJet] = ( -9999 );
	  if(vars.checkTag(reco::btau::vertexEnergyRatio)) SVEnergyRatio[iJet] = ( vars.get(reco::btau::vertexEnergyRatio) );
	  else SVEnergyRatio[iJet] = ( -9999 );
	  
	  if(vars.checkTag(reco::btau::vertexJetDeltaR)) SVjetDeltaR[iJet] = (  vars.get( reco::btau::vertexJetDeltaR) );
	  else SVjetDeltaR[iJet] = ( -9999 );

	  if(vars.checkTag(reco::btau::trackSip3dSigAboveCharm) ) SVIPFirstAboveCharm[iJet] = (  vars.get( reco::btau::trackSip3dSigAboveCharm ));
	  else SVIPFirstAboveCharm[iJet] = (  -9999 );
	  
//        added by Caroline
          Int_t totcharge=0;
//        end add

	  TrackKinematics vertexKinematics;
	  const Vertex &vertex = svTagInfo[thisJetRef]->secondaryVertex(0);
	  Bool_t hasRefittedTracks = vertex.hasRefittedTracks();
	  TrackRefVector tracks = svTagInfo[thisJetRef]->vertexTracks(0);
	  for(TrackRefVector::const_iterator track = tracks.begin();
	      track != tracks.end(); track++) {
	    Double_t w = svTagInfo[thisJetRef]->trackWeight(0, *track);
	    if (w < 0.5)
	      continue;
	    if (hasRefittedTracks) {
	      Track actualTrack = vertex.refittedTrack(*track);
	      vertexKinematics.add(actualTrack, w);

              // added Caroline
              totcharge+=actualTrack.charge();
              // end add
		  
	    } else {
	      vertexKinematics.add(**track, w);

              // added Caroline
              const reco::Track& mytrack = **track;
              totcharge+=mytrack.charge();
              // end add
	    }
	  }
	  Bool_t useTrackWeights = true;
	  math::XYZTLorentzVector vertexSum = useTrackWeights
	    ? vertexKinematics.weightedVectorSum()
	    : vertexKinematics.vectorSum();
	  
	  math::XYZTLorentzVector flightDir( svTagInfo[thisJetRef]->flightDirection(0).x(), svTagInfo[thisJetRef]->flightDirection(0).y(), svTagInfo[thisJetRef]->flightDirection(0).z(), 0  );
	  SVvtxSumJetDeltaR[iJet] = ( Geom::deltaR(vertexSum, jetDir) );
	  SVvtxSumVtxDirDeltaR[iJet] = ( Geom::deltaR( flightDir, vertexSum ) );
	  
	  SVvtxPt[iJet] = vertex.p4().pt();
	  SVvtxSumDirEta[iJet] = vertex.p4().eta();
	  SVvtxSumDirPhi[iJet] = vertex.p4().phi();
	  SVvtxDirEta[iJet] =  flightDir.eta();
	  SVvtxDirPhi[iJet] =  flightDir.phi();

          // new variables (added by Caroline)
          // PTrel info
          math::XYZVector flightDir2=math::XYZVector(flightDir.x(),flightDir.y(),flightDir.z());
          //math::XYZVector vertexSum2=math::XYZVector(vertex.p4().px(),vertex.p4().py(),vertex.p4().pz());
          math::XYZVector vertexSum2=math::XYZVector(vertexSum.x(),vertexSum.y(),vertexSum.z());
          SVvtxSumPTrel[iJet]=Perp(vertexSum2, jetDir);
          SVvtxDirPTrel[iJet]=Perp(flightDir2, jetDir);

          // Distance to Jet Axis (inspired from RecoBTag/BTagTools/src/SignedImpactParameter3D.cc (l 141 -> 154)

          //get the  secondary vertex line
          //Line::PositionType pos(GlobalPoint(vertexSum.x(),vertexSum.y(),vertexSum.z()));
          Line::PositionType pos(GlobalPoint(vertex.p4().x(),vertex.p4().y(),vertex.p4().z()));
          Line::DirectionType dir(GlobalVector(flightDir.px(),flightDir.py(),flightDir.pz()));
          Line trackline(pos,dir);
          // get the Jet  line 
          Line::PositionType pos2(GlobalPoint(jetVertex[iJet].x(),jetVertex[iJet].y(),jetVertex[iJet].z()));
          Line::DirectionType dir2(GlobalVector(jetDir.x(),jetDir.y(),jetDir.z()));
          Line jetline(pos2,dir2);
          // now compute the distance between the two lines
          SVvtxDistJetAxis[iJet] = (jetline.distance(trackline)).mag();


          // total charge at the secondary vertex
          SVtotCharge[iJet]=totcharge;

          //end add
	  
	}
      else
	{
	  SVnVertices[iJet] = (0);
	  SecondaryVertex[iJet] = (math::XYZVector(0,0,0));             
	  SV3dDistance[iJet] = (-1);                          
	  SV3dDistanceError[iJet] = (-1);                     
	  SV2dDistance[iJet] = (-1);                          
	  SV2dDistanceError[iJet] = (-1);                     
	  SVChi2[iJet] = (-1);                                
	  SVDegreesOfFreedom[iJet] = (-1);                    
	  SVNormChi2[iJet] = (-1);                                                           
	  SVnVertexTracks[iJet] = (-1);
	  SVnVertexTracksAll[iJet] = (-1);
	  SVnFirstVertexTracksAll[iJet] = (-1);
	  SVnFirstVertexTracks[iJet] = (-1);
	  SVMass[iJet] = (-1);
	  SVEnergyRatio[iJet] = (-1);
	  SVjetDeltaR[iJet] = (-1);
	  
	  SVIPFirstAboveCharm[iJet] = (  -9999 );
	  SVvtxSumVtxDirDeltaR[iJet] = ( -9999 );
	  SVvtxSumJetDeltaR[iJet] =  ( -9999 );

	  SVvtxPt[iJet] = (-1 );
	  SVvtxSumDirEta[iJet] = (-9999 );
	  SVvtxSumDirPhi[iJet] = (-9999 );
	  SVvtxDirEta[iJet] = (-9999 );
	  SVvtxDirPhi[iJet] = (-9999 );

          SVvtxSumPTrel[iJet] = (-9999 );
          SVvtxDirPTrel[iJet] = (-9999 );
          SVvtxDistJetAxis[iJet] =  (-9999 );
          SVtotCharge[iJet] = (-9999 );

	}
      
      //impactParameterTagInfos ghost track
   
      const math::XYZVector ghostTrack(ipTagInfo[thisJetRef]->ghostTrack()->px(),ipTagInfo[thisJetRef]->ghostTrack()->py(),ipTagInfo[thisJetRef]->ghostTrack()->pz());

      IPghostTrackP3[iJet] = (ghostTrack);
      IPghostTrackPt[iJet] = (ipTagInfo[thisJetRef]->ghostTrack()->pt());                        
      IPghostTrackPtRel[iJet] = (Perp(ghostTrack,axis));
      IPghostTrackEta[iJet] = (ipTagInfo[thisJetRef]->ghostTrack()->eta());                       
      IPghostTrackPhi[iJet] = (ipTagInfo[thisJetRef]->ghostTrack()->phi());                       
      IPghostTrackDeltaR[iJet] = (deltaR(thisJetRef->eta(), thisJetRef->phi(), ipTagInfo[thisJetRef]->ghostTrack()->eta(), ipTagInfo[thisJetRef]->ghostTrack()->phi()));                    

      //impactParameterTagInfos pixel hits
      if(getSharedHitInfo_) {
	Int_t nPix1Shared, nPix1Total;
	Int_t nPix2Shared, nPix2Total;
	Int_t nPix3Shared, nPix3Total;
	getSharedHitsInfo(1, ipTagInfo[thisJetRef]->selectedTracks(), nPix1Shared, nPix1Total);
	getSharedHitsInfo(2, ipTagInfo[thisJetRef]->selectedTracks(), nPix2Shared, nPix2Total);
	getSharedHitsInfo(3, ipTagInfo[thisJetRef]->selectedTracks(), nPix3Shared, nPix3Total);
	
	IPPix1TotalHits[iJet] = (nPix1Total); IPPix1SharedHits[iJet] = (nPix1Shared);
	IPPix2TotalHits[iJet] = (nPix2Total); IPPix2SharedHits[iJet] = (nPix2Shared);
	IPPix3TotalHits[iJet] = (nPix3Total); IPPix3SharedHits[iJet] = (nPix3Shared);
	IPPixAllTotalHits[iJet] = (nPix1Total + nPix2Total + nPix3Total); IPPixAllSharedHits[iJet] = (nPix1Shared + nPix2Shared + nPix3Shared);
      }
      
      //softElectronTagInfos

      for(UInt_t i=0; i<4; i++){
	nElectrons[iJet] = ( 0 );
	electronPt[i][iJet] = (-9999 );   			       
	electronEta[i][iJet] = (-9999);  			       
	electronPhi[i][iJet] = (-9999);  			       
	electronNHits[i][iJet] = (-9999 );   
	electronNExpectedOuterHits[i][iJet] = ( -9999 );
	electronNPixelHits[i][iJet] = (-9999 );   
	electronNChi2[i][iJet] = (-9999);			       
	electronPtRel[i][iJet] = (-9999);   
	electronTrackQuality[i][iJet] = (-9999 );
	electronSip2d[i][iJet] = (-9999);   
	electronIp2d[i][iJet] = (-9999 );   
	electronIpe2d[i][iJet] = (-9999);   		       
	electronSip3d[i][iJet] = (-9999);   
	electronIp3d[i][iJet] = (-9999 );   
	electronIpe3d[i][iJet] = ( -9999 );   
	electronP0Par[i][iJet] = (-9999);   		       
	electronDeltaR[i][iJet] = (-9999);  		       
	electronEtaRel[i][iJet] = (-9999);  		       
	electronRatio[i][iJet] = (-9999);   		       
	electronRatioRel[i][iJet] = (-9999);
	electroneSuperClusterOverP[i][iJet] = (-9999);
	electroneSeedClusterOverP[i][iJet] = (-9999);
	electroneSeedClusterOverPout[i][iJet] = (-9999);
	electroneEleClusterOverPout[i][iJet] = (-9999);
	electrondeltaEtaSuperClusterTrackAtVtx[i][iJet] = (-9999);
	electrondeltaEtaSeedClusterTrackAtCalo[i][iJet] = (-9999);
	electrondeltaEtaEleClusterTrackAtCalo[i][iJet] = (-9999);
	electrondeltaPhiSuperClusterTrackAtVtx[i][iJet] = (-9999);
	electrondeltaPhiSeedClusterTrackAtCalo[i][iJet] = (-9999);
	electrondeltaPhiEleClusterTrackAtCalo[i][iJet] = (-9999);
      } 

      if( softElectronTagInfo[thisJetRef]->leptons() > 0 )
	{
	  nElectrons[iJet] = (softElectronTagInfo[thisJetRef]->leptons());


	  edm::Handle<reco::GsfElectronCollection> allelectrons;
	  iEvent.getByLabel("gsfElectrons",allelectrons);
	  // loop over all electrons in tag info and match them to gsfelectron collection 
	  // create pt sorted map of electrons
	  std::map<Double_t, const GsfElectron*> elMap;
	  std::map<const GsfElectron*, UInt_t> tagInfoMap;

	  for(UInt_t iElectron = 0; iElectron < softElectronTagInfo[thisJetRef]->leptons(); iElectron++)
	    {
	      //loop on electron collection
	      //count electrons
	      UInt_t iElCounter = 0;
	      for(size_t i=0; i < allelectrons->size(); ++i){
		const GsfElectron & el = (*allelectrons)[i];
		GsfTrackRef gsftrack = el.gsfTrack();
 		if(gsftrack.isNull() && !gsftrack.isAvailable()){
 		  std::cout<<"track is null"<<std::endl;
 		  continue;
 		}
 		reco::GsfTrackRef softLepTrack =  (softElectronTagInfo[thisJetRef]->lepton(iElectron)).castTo<GsfTrackRef>();
		if( gsftrack == softLepTrack){
 		  iElCounter++;
		  elMap[gsftrack->pt()] = &el ;
		  tagInfoMap[&el] = iElectron;
		}
	      }
		
	      if(iElCounter != 1){
		std::cout<<"ERROR: iElCounter= "<<iElCounter<<"this should never happen" << std::endl;
		exit(1);
	      }	      
	    }

	
	  if( elMap.size() != softElectronTagInfo[thisJetRef]->leptons()){
	    std::cout<<"ERROR: softElectronTagInfo[thisJetRef]->leptons():  this should never happen"<< std::endl;
	    exit(1);
	  }

	  // now loop over el map and fill info
	  Int_t iElMapCounter = 0;
	  for(std::map<Double_t, const GsfElectron*>::reverse_iterator it= elMap.rbegin(); it != elMap.rend(); it++){

	    const GsfElectron * el =  it->second;

	    electronPt[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->pt());   			       
	    electronEta[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->eta());  			       
	    electronPhi[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->phi());  			       
	    electronNHits[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->hitPattern().numberOfValidHits() );   
	    electronNExpectedOuterHits[iElMapCounter][iJet] = ( softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->trackerExpectedHitsOuter().numberOfHits() );   
	    electronNPixelHits[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->hitPattern().numberOfValidPixelHits() );   
	    electronNChi2[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])->normalizedChi2());			       
	    electronPtRel[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).ptRel);   
	      
	    TransientTrack transientTrack = builder->build( *softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el]) );
	    GlobalVector direction(thisJetRef->momentum().x(), thisJetRef->momentum().y(), thisJetRef->momentum().z());
	    Measurement1D ip3d = IPTools::signedImpactParameter3D(transientTrack, direction, *pv).second;
	    Measurement1D ip2d = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second;

	    electronSip2d[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).sip2d);   
	    electronIp2d[iElMapCounter][iJet] = ( ip2d.value() );   
	    electronIpe2d[iElMapCounter][iJet] = ( ip2d.error() );   		       
	    electronSip3d[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).sip3d);   
	    electronIp3d[iElMapCounter][iJet] = ( ip3d.value() );   
	    electronIpe3d[iElMapCounter][iJet] = ( ip3d.error() );   
	    electronP0Par[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).p0Par);   		       
	    electronDeltaR[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).deltaR);  		       
	    electronEtaRel[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).etaRel);  		       
	    electronRatio[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).ratio);   		       
	    electronRatioRel[iElMapCounter][iJet] = (softElectronTagInfo[thisJetRef]->properties(tagInfoMap[el]).ratioRel);

	    electroneSuperClusterOverP[iElMapCounter][iJet] = el->eSuperClusterOverP();
	    electroneSeedClusterOverP[iElMapCounter][iJet] = el->eSeedClusterOverP();
	    electroneSeedClusterOverPout[iElMapCounter][iJet] = el->eSeedClusterOverPout();
	    electroneEleClusterOverPout[iElMapCounter][iJet] = el->eEleClusterOverPout();
	    electrondeltaEtaSuperClusterTrackAtVtx[iElMapCounter][iJet] = el->deltaEtaSuperClusterTrackAtVtx();
	    electrondeltaEtaSeedClusterTrackAtCalo[iElMapCounter][iJet] = el->deltaEtaSeedClusterTrackAtCalo();
	    electrondeltaEtaEleClusterTrackAtCalo[iElMapCounter][iJet] = el->deltaEtaEleClusterTrackAtCalo();
	    electrondeltaPhiSuperClusterTrackAtVtx[iElMapCounter][iJet] = el->deltaPhiSuperClusterTrackAtVtx();
	    electrondeltaPhiSeedClusterTrackAtCalo[iElMapCounter][iJet] = el->deltaPhiSeedClusterTrackAtCalo();
	    electrondeltaPhiEleClusterTrackAtCalo[iElMapCounter][iJet] = el->deltaPhiEleClusterTrackAtCalo();

	    for(Int_t i = 2; i>-2; i--){
	      if( (*softElectronTagInfo[thisJetRef]->lepton(tagInfoMap[el])).quality(reco::TrackBase::TrackQuality(i))){
		electronTrackQuality[iElMapCounter][iJet] = (i);
		break;
	      }
	    }
	  }
	}
	
      
      //softMuonTagInfos
      for(UInt_t i=0; i<4; i++){
	nMuons[iJet] = 0;
	muonIsGlobal[i][iJet] = 0;             
	muonIsTracker[i][iJet] = 0;            
	muonIsStandalone[i][iJet] = 0;         
	muonPt[i][iJet] = -999;                        
	muonEta[i][iJet] = -999;                       
	muonPhi[i][iJet] = -999;                  
	muonNumberOfMatches[i][iJet] = -999;      
	muonGlobalMuonHits[i][iJet] = -999;       
	muonInnerValidHits[i][iJet] = -999;       
	muonNExpectedOuterHits[i][iJet] = -999;   
	muonNPixelHits[i][iJet] = -999;           
	muonInnerNChi2[i][iJet] = -999;           
	muonGlobalNChi2[i][iJet] = -999;          
	muonVzPVDist[i][iJet] = -999;             
	muonPtRel[i][iJet] = -999;                
	muonSip2d[i][iJet] = -999;                
	muonIp2d[i][iJet] = -999;                 
	muonIpe2d[i][iJet] = -999;                
	muonSip3d[i][iJet] = -999;                
	muonIp3d[i][iJet] = -999;                 
	muonIpe3d[i][iJet] = -999;                
	muonP0Par[i][iJet] = -999;                     
	muonDeltaR[i][iJet] = -999;                    
	muonEtaRel[i][iJet] = -999;                    
	muonRatio[i][iJet] = -999;                     
	muonTrackQuality[i][iJet] = -999;              
	muonRatioRel[i][iJet] = -999;             

      }

      if ( softMuonTagInfo[thisJetRef]->leptons() > 0 )
	{
	  nMuons[iJet] = (softMuonTagInfo[thisJetRef]->leptons());
	      
	  edm::Handle<reco::MuonCollection> allmuons;
	  iEvent.getByLabel("muons",allmuons);
	  // loop over all muons in tag info and match them to muon collection 
	  // create pt sorted map of muons
	  std::map<Double_t, const Muon*> muMap;
	  std::map<const Muon*, UInt_t> tagInfoMap;

	  for(UInt_t iMuon = 0; iMuon < softMuonTagInfo[thisJetRef]->leptons(); iMuon++)
	    {
	      //loop on muon collection
	      //count muons
	      UInt_t iMuCounter = 0;
	      for(size_t i=0; i < allmuons->size(); ++i){
		const Muon & mu = (*allmuons)[i];
		TrackRef globTrack = mu.globalTrack();
		TrackRef softLepTrackRef =  (softMuonTagInfo[thisJetRef]->lepton(iMuon)).castTo<TrackRef>();
		if( globTrack == softLepTrackRef ){   // found a matched muon
		  iMuCounter++;
		  muMap[mu.globalTrack()->pt()] = &mu ;
		  tagInfoMap[&mu] = iMuon;
		}
		    
	      }
	      if(iMuCounter != 1){
		std::cout<<"ERROR: iMuCounter != 1 this should never happen" << std::endl;
		exit(1);
	      }
	    }

	  if( muMap.size() != softMuonTagInfo[thisJetRef]->leptons()){
	    std::cout<<"ERROR: softMuonTagInfo[thisJetRef]->leptons():  this should never happen"<< std::endl;
	    exit(1);
	  }

	  // now loop over mu map and fill info
	  Int_t iMuMapCounter = 0;
	  for(std::map<Double_t, const Muon*>::reverse_iterator it= muMap.rbegin(); it != muMap.rend(); it++){

	    const Muon * mu =  it->second;

	    muonIsGlobal[iMuMapCounter][iJet] =  mu->isGlobalMuon();                
	    muonIsTracker[iMuMapCounter][iJet] =  mu->isTrackerMuon();
	    muonIsStandalone[iMuMapCounter][iJet] =  mu->isStandAloneMuon();
	    muonPt[iMuMapCounter][iJet] = softMuonTagInfo[thisJetRef]->lepton(tagInfoMap[mu])->pt(); 
	    muonEta[iMuMapCounter][iJet] = softMuonTagInfo[thisJetRef]->lepton(tagInfoMap[mu])->eta(); 
	    muonPhi[iMuMapCounter][iJet] = softMuonTagInfo[thisJetRef]->lepton(tagInfoMap[mu])->phi(); 
	    muonNumberOfMatches[iMuMapCounter][iJet] =  mu->numberOfMatches();
	    muonGlobalMuonHits[iMuMapCounter][iJet] = mu->globalTrack()->hitPattern().numberOfValidMuonHits();
	    muonInnerValidHits[iMuMapCounter][iJet] = mu->innerTrack()->numberOfValidHits();        
	    muonNExpectedOuterHits[iMuMapCounter][iJet] = mu->innerTrack()->trackerExpectedHitsOuter().numberOfHits();
	    muonNPixelHits[iMuMapCounter][iJet] = mu->innerTrack()->hitPattern().numberOfValidPixelHits();
	    muonInnerNChi2[iMuMapCounter][iJet] = mu->innerTrack()->normalizedChi2();    
	    muonGlobalNChi2[iMuMapCounter][iJet] = mu->globalTrack()->normalizedChi2();
	    muonVzPVDist[iMuMapCounter][iJet] = mu->vz() - pv->z();
	    muonPtRel[iMuMapCounter][iJet] = softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).ptRel;

	    TransientTrack transientTrack = builder->build( *softMuonTagInfo[thisJetRef]->lepton(tagInfoMap[mu]) );
	    GlobalVector direction(thisJetRef->momentum().x(), thisJetRef->momentum().y(), thisJetRef->momentum().z());
	    Measurement1D ip3d = IPTools::signedImpactParameter3D(transientTrack, direction, *pv).second;
	    Measurement1D ip2d = IPTools::signedTransverseImpactParameter(transientTrack, direction, *pv).second;  	
		
		
	    muonSip2d[iMuMapCounter][iJet] = softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).sip2d;
	    muonIp2d[iMuMapCounter][iJet] = ip2d.value();
	    muonIpe2d[iMuMapCounter][iJet] = ip2d.error();   
	    muonSip3d[iMuMapCounter][iJet] = softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).sip3d;
	    muonIp3d[iMuMapCounter][iJet] = ip3d.value();
	    muonIpe3d[iMuMapCounter][iJet] = ip3d.error();   
	    muonP0Par[iMuMapCounter][iJet] =        (softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).p0Par);
	    muonDeltaR[iMuMapCounter][iJet] =       (softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).deltaR); 
	    muonEtaRel[iMuMapCounter][iJet] =       (softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).etaRel); 
	    muonRatio[iMuMapCounter][iJet] =        (softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).ratio);
	    muonRatioRel[iMuMapCounter][iJet] =  (softMuonTagInfo[thisJetRef]->properties(tagInfoMap[mu]).ratioRel);  
		
		
	    for(Int_t i = 2; i>-2; i--){
	      if( (*softMuonTagInfo[thisJetRef]->lepton(tagInfoMap[mu])).quality(reco::TrackBase::TrackQuality(i))){
		muonTrackQuality[iMuMapCounter][iJet] = (i);
		break;
	      }
	    }
	    iMuMapCounter++;
	    if(iMuMapCounter>=4) break;
	  }
	}

    for (vector< ParameterSet >::iterator ibTag = bTag_Config_.begin(); ibTag != bTag_Config_.end(); ibTag++) 
	{
	  bTagArrays[ibTag->getParameter<string>("alias")][iJet] = ((bTags[ibTag->getParameter<string>("alias")])[thisJetRef]);
	}
    } // end loop over jets
  
  nTracks = iTotalTracksCounter;

  // fill the tree
  EventInfo->Fill();  

}

// ------------ method called once each job just before starting event loop  ------------
void TagNtupleProducer::beginJob() {}

//copied and modified from http://www.codeproject.com/KB/string/wildcmp.aspx
//It does not belong to any class
Bool_t wildcmp(const char *wild, const char *string) {
  // Written by Jack Handy - jakkhandy@hotmail.com
  const char *cp = NULL, *mp = NULL;

  while ((*string) && (*wild != '*')) {
    if ((*wild != *string) && (*wild != '?')) {
      return false;
    }
    wild++;
    string++;
  }

  while (*string) {
    if (*wild == '*') {
      if (!*++wild) {
        return true;
      }
      mp = wild;
      cp = string+1;
    } else if ((*wild == *string) || (*wild == '?')) {
      wild++;
      string++;
    } else {
      wild = mp;
      string = cp++;
    }
  }

  while (*wild == '*') {
    wild++;
  }
  return !*wild;
}

// ------------ method called at the beginning of the run  ------------
void TagNtupleProducer::beginRun(const Run& r, const EventSetup& iSet)
{
  iSet.get<IdealMagneticFieldRecord>().get(theMF);
  runNumber = r.runAuxiliary().run();
  // passed as parameter to HLTConfigProvider::init(), not yet used
  Bool_t isConfigChanged = false;
  if ( hltConfigProvider_.init( r, iSet, triggerTag_.process() , isConfigChanged ) ) {
    // if init returns TRUE, initialisation has succeeded!
    HLTTableName->assign(hltConfigProvider_.tableName());
    HLTNamesSet->clear();
    HLTTableBitMatch_.clear();
    const vector<string> & HLTNamesSet_=hltConfigProvider_.triggerNames();
    for ( vector<string>::const_iterator itertable=HLTNamesSet_.begin();itertable!=HLTNamesSet_.end();itertable++ ) {
      Bool_t selected=false;
      cout<<itertable-HLTNamesSet_.begin()<<":"<<*itertable<<" -------- ";
      for ( vector<string>::const_iterator iter=TriggerNames_.begin();iter!=TriggerNames_.end();iter++ ) {
	cout<<*iter<<",";
	if ( wildcmp(iter->c_str(),itertable->c_str()) ) {
	  selected=true;
	  break;
	}
      }
      if (selected) {
	HLTTableBitMatch_.push_back(itertable-HLTNamesSet_.begin());
	HLTNamesSet->push_back(*itertable);
	cout<<"   (:SAVE=:)"<<endl;
      }
      else 
	cout<<"   ):DROP:("<<endl;
    }
    //    for ( vector<string>::const_iterator iter=HLTNamesSet->begin();iter!=HLTNamesSet->end();iter++ )
    //      cout<<*iter<<":"<<HLTTableBitMatch_.at(iter-HLTNamesSet->begin())<<endl;
    RunInfo->Fill();
  } else {
      // if init returns FALSE, initialisation has NOT succeeded, which indicates a problem
      // with the file and/or code and needs to be investigated!
    LogWarning("DataLost") << " HLT config extraction failure with process name " <<triggerTag_.process();
    // In this case, all access methods will return empty values!
  }
}


// ------------ method called at the end of the run  ------------
void TagNtupleProducer::endRun(const Run& r, const EventSetup& iSet)
{
}

// end add

// ------------ method called once each job just after ending the event loop  ------------
void TagNtupleProducer::endJob() {

  file->Write();
  file->Close();
}

//take it from
// http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/SimTracker/TrackAssociation/src/TrackAssociatorByChi2.cc?revision=1.41&view=markup
std::pair<Bool_t,reco::TrackBase::ParameterVector> 
TagNtupleProducer::parametersAtClosestApproach(Basic3DVector<Double_t> vertex,
					       Basic3DVector<Double_t> momAtVtx,
					       Float_t charge,
					       const reco::BeamSpot& bs) const{
  
  TrackBase::ParameterVector sParameters;
  try {
    FreeTrajectoryState ftsAtProduction(GlobalPoint(vertex.x(),vertex.y(),vertex.z()),
					GlobalVector(momAtVtx.x(),momAtVtx.y(),momAtVtx.z()),
					TrackCharge(charge),
					theMF.product());
    TSCBLBuilderNoMaterial tscblBuilder;
    TrajectoryStateClosestToBeamLine tsAtClosestApproach = tscblBuilder(ftsAtProduction,bs);//as in TrackProducerAlgorithm
    
    GlobalPoint v = tsAtClosestApproach.trackStateAtPCA().position();
    GlobalVector p = tsAtClosestApproach.trackStateAtPCA().momentum();
    sParameters[0] = tsAtClosestApproach.trackStateAtPCA().charge()/p.mag();
    sParameters[1] = Geom::halfPi() - p.theta();
    sParameters[2] = p.phi();
    sParameters[3] = (-v.x()*sin(p.phi())+v.y()*cos(p.phi()));
    sParameters[4] = v.z()*p.perp()/p.mag() - (v.x()*p.x()+v.y()*p.y())/p.perp() * p.z()/p.mag();
    
    return pair<Bool_t,TrackBase::ParameterVector>(true,sParameters);
  } catch ( ... ) {
    return pair<Bool_t,TrackBase::ParameterVector>(false,sParameters);
  }
}

void TagNtupleProducer::getSharedHitsInfo(UInt_t layer, const reco::TrackRefVector & tracks, Int_t &nSharedHits, Int_t &nTotalHits){

  nSharedHits=0;
  nTotalHits=0;

  //iterate over tracks
  for(track_iterator itTrack = tracks.begin(); itTrack!=tracks.end(); itTrack++){
    //iterate over hits of this track
    for(trackingRecHit_iterator hitIt = (*itTrack)->recHitsBegin(); hitIt != (*itTrack)->recHitsEnd(); hitIt++  ){
      if( !(*hitIt)->isValid() ) continue;
      DetId detid = (*hitIt)->geographicalId();
      UInt_t subdet = detid.subdetId();

      UInt_t thislayer = -1;
      if(subdet==PixelSubdetector::PixelBarrel){//PXB
	PXBDetId pxbdet=(detid);
	thislayer=pxbdet.layer();

      }
      else if(subdet==PixelSubdetector::PixelEndcap){
	PXFDetId pxfdet(detid);
	thislayer=pxfdet.disk();
      }

      // if they are on the desired layer
      if(layer == thislayer){
	nTotalHits++;
	//iterate over all other tracks avoiding Double_t counting
	track_iterator itTrack2 = itTrack;
	itTrack2++;
	if(itTrack2 == tracks.end()) continue;
	for(; itTrack2!=tracks.end(); itTrack2++){
	  // iterate again over hits
	  for(trackingRecHit_iterator hitIt2 = (*itTrack2)->recHitsBegin(); hitIt2 != (*itTrack2)->recHitsEnd(); hitIt2++  ){
	    if( !(*hitIt2)->isValid() ) continue;
	    DetId detid2 = (*hitIt2)->geographicalId();
	    if( detid.rawId() == detid2.rawId()  ){
	      // check identity via
	      if( (*hitIt)->sharesInput( &(**hitIt2), TrackingRecHit::some )) nSharedHits++; 
	    }
	  }
	}
      }
    }
  }
}

Bool_t TagNtupleProducer::hasSharedHit(UInt_t layer, track_iterator thisTrack, const reco::TrackRefVector & tracks) {
  //iterate over hits of this track 
  for(trackingRecHit_iterator hitIt = (*thisTrack)->recHitsBegin(); hitIt != (*thisTrack)->recHitsEnd(); hitIt++  ){
    if( !(*hitIt)->isValid() ) continue;
   
    DetId detid = (*hitIt)->geographicalId();
    UInt_t subdet = detid.subdetId();
    
    UInt_t thislayer = -1;
    if(subdet==PixelSubdetector::PixelBarrel){//PXB
      PXBDetId pxbdet=(detid);
      thislayer=pxbdet.layer();
      
    }
    else if(subdet==PixelSubdetector::PixelEndcap){
      PXFDetId pxfdet(detid);
      thislayer=pxfdet.disk();
    }
    
    // if they are on the desired layer
    if(layer == thislayer)  //iterate over all other tracks avoiding double counting
      for(track_iterator itTrack2 = tracks.begin(); itTrack2!=tracks.end(); itTrack2++){
	if (thisTrack==itTrack2) continue;//avoid double counting track 
	// iterate again over hits
	for(trackingRecHit_iterator hitIt2 = (*itTrack2)->recHitsBegin(); hitIt2 != (*itTrack2)->recHitsEnd(); hitIt2++  ){
	  if( !(*hitIt2)->isValid() ) continue;
	  DetId detid2 = (*hitIt2)->geographicalId();
	  if( detid.rawId() == detid2.rawId()  )// check identity via
	    if( (*hitIt)->sharesInput( &(**hitIt2), TrackingRecHit::some )) return true;
	}
      }
  }
  return false;
}

Bool_t TagNtupleProducer::GenParticleParentTree( HepMC::GenParticle *genPar ) {
  HepMC::GenVertex *thisVtx = genPar->production_vertex();
  if (thisVtx) {
      for (HepMC::GenVertex::particles_in_const_iterator pgenD = thisVtx->particles_in_const_begin(); pgenD != thisVtx->particles_in_const_end(); ++pgenD)
	if ( (*pgenD)->pdg_id()!=92 )  {//Pythia special code for string, we only care about the particles after hadronization
	  UInt_t PID=abs( (*pgenD)->pdg_id() );
	  if ( (PID>=5000&&PID<6000)||(PID%1000>=500&&PID%1000<600) ) return true;
	  if ( GenParticleParentTree( (*pgenD) ) )   return true;
	}
  }
  return false;
}

//define this as a plug-in
DEFINE_FWK_MODULE(TagNtupleProducer);

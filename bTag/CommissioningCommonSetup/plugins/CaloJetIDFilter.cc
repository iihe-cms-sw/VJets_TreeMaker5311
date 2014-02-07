// -*- C++ -*-
//
// Package:    CaloJetIDFilter
// Class:      CaloJetIDFilter
// 
/**\class CaloJetIDFilter CaloJetIDFilter.cc BTaggingDataValidation/CaloJetIDFilter/src/CaloJetIDFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jason Keller
//         Created:  Thu Apr 25 23:23:27 CDT 2020
// $Id: CaloJetIDFilter.cc,v 1.3 2010/08/31 20:25:03 kellerjd Exp $
//
//


// system include files
#include <memory>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

//
// class declaration
//

class CaloJetIDFilter : public edm::EDFilter {
   public:
      explicit CaloJetIDFilter(const edm::ParameterSet&);
      ~CaloJetIDFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      edm::InputTag jetIDTag_;
      edm::InputTag caloJetTag_;
      bool filter_;

      std::vector<double> pTCuts_;
      std::vector<double> etaCuts_;
      std::vector<double> fHPDCuts_;
      std::vector<double> fRBXCuts_;
      std::vector<int> n90HitsCuts_;
      std::vector<double> fSubDetector1Cuts_;
      std::vector<double> fSubDetector2Cuts_;
      std::vector<double> fSubDetector3Cuts_;
      std::vector<double> fSubDetector4Cuts_;
      std::vector<double> EMFCuts_;
      std::vector<double> restrictedEMFCuts_;
      std::vector<int> nHCALTowersCuts_;
      std::vector<int> nECALTowersCuts_;
      std::vector<double> approximatefHPDCuts_;
      std::vector<double> approximatefRBXCuts_;
      std::vector<int> hitsInN90Cuts_;
      std::vector<int> numberOfHits2RPCCuts_;
      std::vector<int> numberOfHits3RPCCuts_;
      std::vector<int> numberOfHitsRPCCuts_;
     
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
CaloJetIDFilter::CaloJetIDFilter(const edm::ParameterSet& iConfig) :
  jetIDTag_(iConfig.getParameter<edm::InputTag>("JetIDTag")),
  caloJetTag_(iConfig.getParameter<edm::InputTag>("CaloJetsTag")),
  filter_(iConfig.getParameter<bool>("filter")),
  pTCuts_(iConfig.getParameter<std::vector<double> >("PtCuts")),
  etaCuts_(iConfig.getParameter<std::vector<double> >("EtaCuts")),
  fHPDCuts_(iConfig.getParameter<std::vector<double> >("fHPDCuts")),
  fRBXCuts_(iConfig.getParameter<std::vector<double> >("fRBXCuts")),
  n90HitsCuts_(iConfig.getParameter<std::vector<int> >("n90HitsCuts")),
  fSubDetector1Cuts_(iConfig.getParameter<std::vector<double> >("fSubDetector1Cuts")),
  fSubDetector2Cuts_(iConfig.getParameter<std::vector<double> >("fSubDetector2Cuts")),
  fSubDetector3Cuts_(iConfig.getParameter<std::vector<double> >("fSubDetector3Cuts")),
  fSubDetector4Cuts_(iConfig.getParameter<std::vector<double> >("fSubDetector4Cuts")),
  EMFCuts_(iConfig.getParameter<std::vector<double> >("EMFCuts")),
  restrictedEMFCuts_(iConfig.getParameter<std::vector<double> >("RestrictedEMFCuts")),
  nHCALTowersCuts_(iConfig.getParameter<std::vector<int> >("nHCALTowersCuts")),
  nECALTowersCuts_(iConfig.getParameter<std::vector<int> >("nECALTowersCuts")),
  approximatefHPDCuts_(iConfig.getParameter<std::vector<double> >("ApproximatefHPDCuts")),
  approximatefRBXCuts_(iConfig.getParameter<std::vector<double> >("ApproximatefRBXCuts")),
  hitsInN90Cuts_(iConfig.getParameter<std::vector<int> >("hitsInN90Cuts")),
  numberOfHits2RPCCuts_(iConfig.getParameter<std::vector<int> >("numberOfHits2RPCCuts")),
  numberOfHits3RPCCuts_(iConfig.getParameter<std::vector<int> >("numberOfHits3RPCCuts")),
  numberOfHitsRPCCuts_(iConfig.getParameter<std::vector<int> >("numberOfHitsRPCCuts"))
{
   //now do what ever initialization is needed
  produces<reco::CaloJetCollection>();
}


CaloJetIDFilter::~CaloJetIDFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
CaloJetIDFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;
  using namespace reco;
  Handle<JetIDValueMap> jetIDs;
  iEvent.getByLabel(jetIDTag_, jetIDs);

  Handle<CaloJetCollection> caloJets;
  iEvent.getByLabel(caloJetTag_, caloJets);

  std::auto_ptr<CaloJetCollection> goodJets(new CaloJetCollection());
  for(unsigned int i = 0; i != caloJets->size(); ++i)
  {
    const CaloJetRef jetRef(caloJets, i);
    if(jetRef.isNull())
      continue;

    const JetID& jetID = (*jetIDs)[jetRef];
    if(jetRef->pt() <= pTCuts_.front() || jetRef->pt() >= pTCuts_.back()) continue;
    if(jetRef->eta() <= etaCuts_.front() || jetRef->eta() >= etaCuts_.back()) continue;
    if(jetID.fHPD <= fHPDCuts_.front() || jetID.fHPD >= fHPDCuts_.back()) continue;
    if(jetID.fRBX <= fRBXCuts_.front() || jetID.fRBX >= fRBXCuts_.back()) continue;
    if(jetID.n90Hits <= n90HitsCuts_.front() || jetID.n90Hits >= n90HitsCuts_.back()) continue;
    if(jetID.fSubDetector1 <= fSubDetector1Cuts_.front() || jetID.fSubDetector1 >= fSubDetector1Cuts_.back()) continue;
    if(jetID.fSubDetector2 <= fSubDetector2Cuts_.front() || jetID.fSubDetector2 >= fSubDetector2Cuts_.back()) continue;
    if(jetID.fSubDetector3 <= fSubDetector3Cuts_.front() || jetID.fSubDetector3 >= fSubDetector3Cuts_.back()) continue;
    if(jetID.fSubDetector4 <= fSubDetector4Cuts_.front() || jetID.fSubDetector4 >= fSubDetector4Cuts_.back()) continue;
    if(jetRef->emEnergyFraction() <= EMFCuts_.front() || jetRef->emEnergyFraction() >= EMFCuts_.back()) continue;
    if(jetID.restrictedEMF <= restrictedEMFCuts_.front() || jetID.restrictedEMF >= restrictedEMFCuts_.back()) continue;
    if(jetID.nHCALTowers <= nHCALTowersCuts_.front() || jetID.nHCALTowers >= nHCALTowersCuts_.back()) continue;
    if(jetID.nECALTowers <= nECALTowersCuts_.front() || jetID.nECALTowers >= nECALTowersCuts_.back()) continue;
    if(jetID.approximatefHPD <= approximatefHPDCuts_.front() || jetID.approximatefHPD >= approximatefHPDCuts_.back()) continue;
    if(jetID.approximatefRBX <= approximatefRBXCuts_.front() || jetID.approximatefRBX >= approximatefRBXCuts_.back()) continue;
    if(jetID.hitsInN90 <= hitsInN90Cuts_.front() || jetID.hitsInN90 >= hitsInN90Cuts_.back()) continue;
    if(jetID.numberOfHits2RPC <= numberOfHits2RPCCuts_.front() || jetID.numberOfHits2RPC >= numberOfHits2RPCCuts_.back()) continue;
    if(jetID.numberOfHits3RPC <= numberOfHits3RPCCuts_.front() || jetID.numberOfHits3RPC >= numberOfHits3RPCCuts_.back()) continue;
    if(jetID.numberOfHitsRPC <= numberOfHitsRPCCuts_.front() || jetID.numberOfHitsRPC >= numberOfHitsRPCCuts_.back()) continue;
    goodJets->push_back(*(jetRef.get()));
  }

  bool jetsEmpty = goodJets->empty();
  iEvent.put(goodJets);
  return !filter_ || !jetsEmpty;
}

// ------------ method called once each job just before starting event loop  ------------
void 
CaloJetIDFilter::beginJob() {}

// ------------ method called once each job just after ending the event loop  ------------
void 
CaloJetIDFilter::endJob() {}

//define this as a plug-in
DEFINE_FWK_MODULE(CaloJetIDFilter);

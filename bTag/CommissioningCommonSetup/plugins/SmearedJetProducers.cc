#include "bTag/CommissioningCommonSetup/interface/SmearedJetProducer.h"

#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/JPTJet.h"
#include "DataFormats/JetReco/interface/PFJet.h"

typedef SmearedJetProducer<reco::CaloJet> SmearedCaloJetProducer;
DEFINE_FWK_MODULE(SmearedCaloJetProducer);
typedef SmearedJetProducer<reco::JPTJet> SmearedJPTJetProducer;
DEFINE_FWK_MODULE(SmearedJPTJetProducer);
typedef SmearedJetProducer<reco::PFJet> SmearedPFJetProducer;
DEFINE_FWK_MODULE(SmearedPFJetProducer);

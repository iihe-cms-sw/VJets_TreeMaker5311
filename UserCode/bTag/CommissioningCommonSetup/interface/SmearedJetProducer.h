#ifndef bTag_CommissioningCommonSetup_SmearedJetProducer_h
#define bTag_CommissioningCommonSetup_SmearedJetProducer_h

#include <memory>
#include <vector>

// user include files
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "CLHEP/Random/RandGaussQ.h"
//
// class declaration
//

template<class T>
class SmearedJetProducer : public edm::EDProducer {
   public:
      typedef std::vector<T> JetCollection;
      explicit SmearedJetProducer(const edm::ParameterSet&);
      ~SmearedJetProducer();

   private:
      virtual void beginJob() ;
      virtual void produce(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      //InputTag for jet collection
      edm::InputTag jetTag_;
      //Mean of Eta Gaussian
      double meanEta_;
      //Std. Dev of Eta Gaussina
      double sigmaEta_;
      //Mean of Phi Gaussian
      double meanPhi_;
      //Std. Dev of Phi Gaussina
      double sigmaPhi_;
      //Gaussian random number generator
      CLHEP::RandGaussQ* gaussianEta_;
      CLHEP::RandGaussQ* gaussianPhi_;
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
template<class T>
SmearedJetProducer<T>::SmearedJetProducer(const edm::ParameterSet& iConfig) :
  jetTag_(iConfig.getParameter<edm::InputTag>("src")),
  meanEta_(iConfig.getParameter<double>("MeanEta")),
  sigmaEta_(iConfig.getParameter<double>("StdDevEta")),
  meanPhi_(iConfig.getParameter<double>("MeanPhi")),
  sigmaPhi_(iConfig.getParameter<double>("StdDevPhi")),
  gaussianEta_(0), gaussianPhi_(0)
{
  edm::Service<edm::RandomNumberGenerator> rng;
  if ( ! rng.isAvailable()) 
  {
    throw cms::Exception("Configuration")
      << "SmearedJetProducer requires the RandomNumberGeneratorService\n"
         "which is not present in the configuration file.  You must add the service\n"
         "in the configuration file or remove the modules that require it.";
  }

  CLHEP::HepRandomEngine& engine = rng->getEngine();

  // engine MUST be a reference here, if a pointer is used the
  // distribution will destroy the engine in its destructor, a major
  // problem because the service owns the engine and will destroy it 
  gaussianEta_ = new CLHEP::RandGaussQ(engine, meanEta_, sigmaEta_);
  gaussianPhi_ = new CLHEP::RandGaussQ(engine, meanPhi_, sigmaPhi_);

  produces<std::vector<T> >();
}

template<class T>
SmearedJetProducer<T>::~SmearedJetProducer()
{
  delete gaussianEta_; 
  delete gaussianPhi_; 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
template<class T>
void SmearedJetProducer<T>::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace edm;

  Handle<JetCollection> jets;
  iEvent.getByLabel(jetTag_, jets);

  std::auto_ptr<std::vector<T> > output(new std::vector<T>());

  typename JetCollection::const_iterator jet;
  for(jet = jets->begin(); jet != jets->end(); ++jet)
  {
   double deta = gaussianEta_->fire();
   double dphi = gaussianPhi_->fire();
   double jetEta = jet->eta() + deta;
   double jetPhi = deltaPhi( jet->phi() , -dphi );

   double theta = 2 * atan( exp(-1.0 * jetEta) );
   double jetPt = jet->p() * sin(theta);
   reco::Candidate::PolarLorentzVector newVec(jetPt, jetEta, jetPhi,
jet->mass());
   T smearedJet = *jet;
   smearedJet.setP4(newVec);
   output->push_back(smearedJet);
  }

  iEvent.put(output);
}

// ------------ method called once each job just before starting event loop  ------------
template<class T>
void SmearedJetProducer<T>::beginJob() {}

// ------------ method called once each job just after ending the event loop  ------------
template<class T>
void SmearedJetProducer<T>::endJob() {}

#endif

// -*- C++ -*-
//
// Package:    EventLumiFilter
// Class:      EventLumiFilter
// 
/**\class EventLumiFilter EventLumiFilter.cc EventLumi/EventLumiFilter/src/EventLumiFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jason Keller
//         Created:  Tue Apr 20 18:50:18 CDT 2010
// $Id: EventLumiFilter.cc,v 1.1 2010/04/23 04:30:55 kellerjd Exp $
//
//


// system include files
#include <memory>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

//
// class declaration
//

class EventLumiFilter : public edm::EDFilter {
   public:
      explicit EventLumiFilter(const edm::ParameterSet&);
      ~EventLumiFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
      std::vector<edm::LuminosityBlockRange> lumis_;
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
EventLumiFilter::EventLumiFilter(const edm::ParameterSet& iConfig) :
  lumis_(iConfig.getParameter<std::vector<edm::LuminosityBlockRange> >("lumisToProcess"))
{
   //now do what ever initialization is needed

}


EventLumiFilter::~EventLumiFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
EventLumiFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   const LuminosityBlockNumber_t& lumiBlock = iEvent.getLuminosityBlock().luminosityBlock();
   const RunNumber_t& runNum = iEvent.run();

   std::cout << "RunNum = " << runNum << "; LumiBlock = " << lumiBlock << "." << std::endl;

   for(std::vector<LuminosityBlockRange>::const_iterator lumi = lumis_.begin();
       lumi != lumis_.end(); ++lumi)
   {
     if((runNum >= lumi->startRun() && runNum <= lumi->endRun()) && 
        (lumiBlock >= lumi->startLumi() && lumiBlock <= lumi->endLumi()))
     {
       std::cout << "It passed!" << std::endl;
       return true;
     }
   }
   return false;
}

// ------------ method called once each job just before starting event loop  ------------
void 
EventLumiFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
EventLumiFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(EventLumiFilter);

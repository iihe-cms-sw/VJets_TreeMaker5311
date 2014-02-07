/**_________________________________________________________________
   class:   BTagBaseTrackEvent.cc
   package: RecoBTag/PerformanceMeasurements


 author: Francisco Yumiceva, Fermilab (yumiceva@fnal.gov)
         Victor E. Bazterra, UIC (baites@fnal.gov)

 version $Id: BTagBaseTrackEvent.cc,v 1.4 2008/09/12 16:41:30 bazterra Exp $

________________________________________________________________**/

#ifdef NOSCRAMV
#include "BTagBaseTrackEvent.h"
#else
#include "RecoBTag/PerformanceMeasurements/interface/BTagBaseTrackEvent.h"
#endif

ClassImp(BTagBaseTrackEvent)

//_______________________________________________________________
void BTagBaseTrackEvent::Reset()
{
    pt.clear();
    eta.clear();
    phi.clear();
    charge.clear();
    trkchi2.clear();
    trkndof.clear();
    trkrechits.clear();
    d0.clear();
    d0sigma.clear();
    jet_deltaR.clear();
    jet_ptrel.clear();
}

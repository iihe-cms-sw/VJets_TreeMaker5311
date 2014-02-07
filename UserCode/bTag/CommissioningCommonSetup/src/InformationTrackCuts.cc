#include "bTag/CommissioningCommonSetup/interface/InformationTrackCuts.h"

InformationTrackCuts::InformationTrackCuts() : Information1D(), triggerHLTJet15U(false), triggerHLTJet30U(false),
  jetPtCut(0.0), jetEtaCut(0.0), nHitsCut(0), nPixHitsCut(0.0), Chi2Cut(0.0), trackPtCut(0.0), distJetAxisCut(0.0),
  decayLengthCut(0.0), IP2dCut(0.0), longIPCut(0.0) {}

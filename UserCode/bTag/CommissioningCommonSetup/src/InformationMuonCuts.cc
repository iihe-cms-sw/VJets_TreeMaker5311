#include "bTag/CommissioningCommonSetup/interface/InformationMuonCuts.h"

InformationMuonCuts::InformationMuonCuts() : Information1D(), triggerHLTJet30U(false), jetPtCut(0.0), jetEtaCut(0.0),
                        muonPtCut(0.0), muonEtaCut(0.0), muonIsGlobal(false), muonGlobalMuonHits(0),
                        muonNumberOfMatches(0), muonInnerValidHits(0), muonNPixelHits(0), muonNExpectedOuterHits(0),
                        muonInnerNChi2(0.0), muonGlobalNChi2(0.0), muonVzPVDist(0.0), muonDeltaR(0.0) {}


#ifndef bTag_CommissioningCommonSetup_InformationMuonCuts_h
#define bTag_CommissioningCommonSetup_InformationMuonCuts_h

#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

class InformationMuonCuts : public Information1D
{
  public:
    // Constructors and destructors
    InformationMuonCuts();
    virtual ~InformationMuonCuts() {}

    // Setters
    inline void setTriggerHLTJet30U(const bool trigger) {triggerHLTJet30U = trigger;}
    inline void setJetPtCut(const float& cut) {jetPtCut = cut;}
    inline void setJetEtaCut(const float& cut) {jetEtaCut = cut;}
    inline void setMuonPtCut(const float& cut) {muonPtCut = cut;}
    inline void setMuonEtaCut(const float& cut) {muonEtaCut = cut;}
    inline void setMuonIsGlobal(const bool isGlobal) {muonIsGlobal = isGlobal;}
    inline void setMuonGlobalMuonHits(const int nhits) {muonGlobalMuonHits = nhits;}
    inline void setMuonNumberOfMatches(const int nmatches) {muonNumberOfMatches = nmatches;}
    inline void setMuonInnerValidHits(const int nvalidhits) {muonInnerValidHits = nvalidhits;}
    inline void setMuonNPixelHits(const int npixelhits) {muonNPixelHits = npixelhits;}
    inline void setMuonNExpectedOuterHits(const int nexpectedhits) {muonNExpectedOuterHits = nexpectedhits;}
    inline void setMuonInnerNChi2(const float& nchi2) {muonInnerNChi2 = nchi2;}
    inline void setMuonGlobalNChi2(const float& nchi2) {muonGlobalNChi2 = nchi2;}
    inline void setMuonVzPVDist(const float& dist) {muonVzPVDist = dist;}
    inline void setMuonDeltaR(const float& deltar) {muonDeltaR = deltar;}

    // Getters
    inline bool const getTriggerHLTJet30U() const {return triggerHLTJet30U;}
    inline float const getJetPtCut() const {return jetPtCut;}
    inline float const getJetEtaCut() const {return jetEtaCut;}
    inline float const getMuonPtCut() const {return muonPtCut;}
    inline float const getMuonEtaCut() const {return muonEtaCut;}
    inline bool const getMuonIsGlobal() const {return muonIsGlobal;}
    inline int const getMuonGlobalMuonHits() const {return muonGlobalMuonHits;}
    inline int const getMuonNumberOfMatches() const {return muonNumberOfMatches;}
    inline int const getMuonInnerValidHits() const {return muonInnerValidHits;}
    inline int const getMuonNPixelHits() const {return muonNPixelHits;}
    inline int const getMuonNExpectedOuterHits() const {return muonNExpectedOuterHits;}
    inline float const getMuonInnerNChi2() const {return muonInnerNChi2;}
    inline float const getMuonGlobalNChi2() const {return muonGlobalNChi2;}
    inline float const getMuonVzPVDist() const {return muonVzPVDist;}
    inline float const getMuonDeltaR() const {return muonDeltaR;}

  private:
    bool triggerHLTJet30U;
    float jetPtCut;
    float jetEtaCut;
    float muonPtCut;
    float muonEtaCut;
    bool  muonIsGlobal;
    int muonGlobalMuonHits;
    int muonNumberOfMatches;
    int muonInnerValidHits;
    int muonNPixelHits;
    int muonNExpectedOuterHits;   			       
    float muonInnerNChi2;
    float muonGlobalNChi2;
    float muonVzPVDist;
    float muonDeltaR;
};


#endif

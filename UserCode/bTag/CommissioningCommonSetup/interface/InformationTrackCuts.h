#ifndef bTag_CommissioningCommonSetup_InformationTrackCuts_h
#define bTag_CommissioningCommonSetup_InformationTrackCuts_h

#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

class InformationTrackCuts : public Information1D
{
  public:
    // Constructors and destructors
    InformationTrackCuts();
    virtual ~InformationTrackCuts() {};

    inline void setTriggerHLTJet15U(const bool trigger) {triggerHLTJet15U = trigger;}
    inline void setTriggerHLTJet30U(const bool trigger) {triggerHLTJet30U = trigger;}
    inline void setJetPtCut(const float& cut) {jetPtCut = cut;}
    inline void setJetEtaCut(const float& cut) {jetEtaCut = cut;}
    inline void setNHitsCut(const unsigned int cut) {nHitsCut = cut;}
    inline void setNPixHitsCut(const unsigned int cut) {nPixHitsCut = cut;}
    inline void setChi2Cut(const float& cut) {Chi2Cut = cut;}
    inline void setTrackPtCut(const float& cut) {trackPtCut = cut;}
    inline void setDistJetAxisCut(const float& cut) {distJetAxisCut = cut;}
    inline void setDecayLengthCut(const float& cut) {decayLengthCut = cut;}
    inline void setIP2dCut(const float& cut) {IP2dCut = cut;}
    inline void setLongIPCut(const float& cut) {longIPCut = cut;}

    // Getters
    inline bool const getTriggerHLTJet15U() const {return triggerHLTJet15U;}
    inline bool const getTriggerHLTJet30U() const {return triggerHLTJet30U;}
    inline float const getJetPtCut() const {return jetPtCut;}
    inline float const getJetEtaCut() const {return jetEtaCut;}
    inline unsigned int const getNHitsCut() const {return nHitsCut;}
    inline unsigned int const getNPixHitsCut() const {return nPixHitsCut;}
    inline float const getChi2Cut() const {return Chi2Cut;}
    inline float const getTrackPtCut() const {return trackPtCut;}
    inline float const getDistJetAxisCut() const {return distJetAxisCut;}
    inline float const getDecayLengthCut() const {return decayLengthCut;}
    inline float const getIP2dCut() const {return IP2dCut;}
    inline float const getLongIPCut() const {return longIPCut;}

  private:
    bool triggerHLTJet15U;
    bool triggerHLTJet30U;
    float jetPtCut;
    float jetEtaCut;
    unsigned int nHitsCut;
    unsigned int nPixHitsCut;
    float Chi2Cut;
    float trackPtCut;
    float distJetAxisCut;
    float decayLengthCut;
    float IP2dCut;
    float longIPCut;
};


#endif

#ifndef bTag_CommissioningCommonSetup_Information2D_h
#define bTag_CommissioningCommonSetup_Information2D_h

#include "bTag/CommissioningCommonSetup/interface/BaseInformation.h"

#include <string>

class Information2D : public BaseInformation
{
  public:
    // Constructor and destructor
    Information2D();
    virtual ~Information2D() {};

    // Setters
    inline void setAliasY(const std::string& alias) {aliasy = alias;}
    inline void setYTitle(const std::string& title) {yTitle = title;}
    inline void setJetCut(const std::string& cut) {jetCut = cut;}
    inline void setTrackCut(const std::string& cut) {trackCut = cut;}
    inline void setYLow(const double& low) {yLow = low;}
    inline void setYUp(const double& up) {yUp = up;}
    inline void setNBinsY(const unsigned int bins) {nBinsY = bins;}
    inline void setBScale(const double& scale) {bScale = scale;}
    inline void setCScale(const double& scale) {cScale = scale;}
    inline void setDisplayNoInfo(const bool display) {displayNoInfo = display;}
    inline void setLegendPosition(const bool position) {legendPosition = position;}

    // Getters
    inline std::string const getAliasY() const {return aliasy;}
    inline std::string const getYTitle() const {return yTitle;}
    inline std::string const getJetCut() const {return jetCut;}
    inline std::string const getTrackCut() const {return trackCut;}
    inline double const getYLow() const {return yLow;}
    inline double const getYUp() const {return yUp;}
    inline unsigned int const getNBinsY() const {return nBinsY;}
    inline double const getBScale() const {return bScale;}
    inline double const getCScale() const {return cScale;}
    inline bool const getDisplayNoInfo() const {return displayNoInfo;}
    inline bool const getLegendPosition() const {return legendPosition;}

  private:
    std::string aliasy;
    std::string yTitle;
    std::string jetCut;
    std::string trackCut;
    double yLow;
    double yUp;
    unsigned int nBinsY;
    double bScale;
    double cScale;
    bool displayNoInfo;
    bool legendPosition;
};

#endif

#ifndef bTag_CommissioningCommonSetup_Information1D_h
#define bTag_CommissioningCommonSetup_Information1D_h

// This class is used to contain basic information for making 1D plots.

#include "bTag/CommissioningCommonSetup/interface/BaseInformation.h"

class Information1D : public BaseInformation
{
  public:
    // Constructors and destructors
    Information1D();
    virtual ~Information1D() {};
    
    // Setters
    inline void setYTitle(const std::string& title) {yTitle = title;}
    inline void setJetCut(const std::string& cut) {jetCut = cut;}
    inline void setTrackCut(const std::string& cut) {trackCut = cut;}
    inline void setYMin(const double& min) {yMin = min;}
    inline void setRatioMin(const double& min) {ratiomin = min;}
    inline void setRatioMax(const double& max) {ratiomax = max;}
    inline void setBScale(const double& scale) {bscale = scale;}
    inline void setCScale(const double& scale) {cscale = scale;}
    inline void setRatioRebin(const int rebin) {ratiorebin = rebin;}
    inline void setDisplayOverUnderflowBin(const bool display) {displayOverUnderflowBin = display;}
    inline void setDisplayNoInfo(const bool display) {displayNoInfo = display;}
    inline void setLegendPosition(const bool legendPos) {legendPosition = legendPos;}

    // Getters
    inline std::string const getYTitle() const {return yTitle;}
    inline std::string const getJetCut() const {return jetCut;}
    inline std::string const getTrackCut() const {return trackCut;}
    inline double const getYMin() const {return yMin;}
    inline double const getRatioMin() const {return ratiomin;}
    inline double const getRatioMax() const {return ratiomax;}
    inline double const getBScale() const {return bscale;}
    inline double const getCScale() const {return cscale;}
    inline int const getRatioRebin() const {return ratiorebin;}
    inline bool const getDisplayOverUnderflowBin() const {return displayOverUnderflowBin;}
    inline bool const getDisplayNoInfo() const {return displayNoInfo;}
    inline bool const getLegendPosition() const {return legendPosition;}

  private:
    std::string yTitle; // The y axis title
    std::string jetCut;
    std::string trackCut;
    double yMin;        // The min value for the y axis
    double ratiomin;    // The min value for the ratio pad
    double ratiomax;    // The max value for the ratio pad
    double bscale;      // How much to scale the b content by
    double cscale;      // How much to scale the c content by
    int ratiorebin;     // Rebinning for the ratio plot
    bool displayOverUnderflowBin; // Fill the first/last bin with under/overflow entries?
    bool displayNoInfo; // Display jets with no flavor?
    bool legendPosition; // Position the legend on the bottom right (true) or top right (false)?
};

#endif

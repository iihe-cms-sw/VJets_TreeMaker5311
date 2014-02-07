#ifndef bTag_CommissioningCommonSetup_InformationPtHat_h
#define bTag_CommissioningCommonSetup_InformationPtHat_h

#include "bTag/CommissioningCommonSetup/interface/BaseInformation.h"

#include <vector>
#include <string>

class InformationPtHat : public BaseInformation
{
  public:
    InformationPtHat();
    virtual ~InformationPtHat() {};

    inline void setPtHatCut(const std::string& cut) {ptHatCut = cut;}
    inline void setPtHatLow(const double& low) {ptHatLow = low;}
    inline void setPtHatUp(const double& up) {ptHatUp = up;}
    inline void setNBinsPtHat(const unsigned int nbins) {nBinsPtHat = nbins;}
    inline void setPtHatBins(const std::vector<double>& bins) {ptHatBins = bins;}

    inline void addPtHatBin(const double& bin) {ptHatBins.push_back(bin);}

    inline std::string const getPtHatCut() const {return ptHatCut;}
    inline double const getPtHatLow() const {return ptHatLow;}
    inline double const getPtHatUp() const {return ptHatUp;}
    inline double const getNBinsPtHat() const {return nBinsPtHat;}
    inline std::vector<double> const getPtHatBins() const {return ptHatBins;}
    inline double const getPtHatBin(const unsigned int iter) const {return ptHatBins[iter];}
    

  private:
    std::string ptHatCut;
    double ptHatLow;
    double ptHatUp;
    unsigned int nBinsPtHat;
    std::vector<double> ptHatBins;
};

#endif

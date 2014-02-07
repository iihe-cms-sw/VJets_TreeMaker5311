#ifndef bTag_CommissioningCommonSetup_InformationQuality_h
#define bTag_CommissioningCommonSetup_InformationQuality_h

#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

#include <string>

class InformationQuality : public Information1D
{
  public:
    // Constructors and destructor
    InformationQuality();
    virtual ~InformationQuality() {};

    // Setter
    inline void setQualityVar(const std::string var) {qualityVar = var;}

    // Getter
    inline std::string const getQualityVar() const {return qualityVar;}

  private:
    std::string qualityVar;
};

#endif

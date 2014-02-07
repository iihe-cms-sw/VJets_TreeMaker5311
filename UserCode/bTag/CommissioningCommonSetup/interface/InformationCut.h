#ifndef bTag_CommissioningCommonSetup_InformationCut_h
#define bTag_CommissioningCommonSetup_InformationCut_h

#include "bTag/CommissioningCommonSetup/interface/Information2D.h"

#include <string>

class InformationCut : public Information2D
{
  public:
    InformationCut();
    virtual ~InformationCut() {};

    inline void setDirection(const std::string& dir) {direction = dir;}

    inline std::string const getDirection() const {return direction;}

  private:
    std::string direction;
};

#endif

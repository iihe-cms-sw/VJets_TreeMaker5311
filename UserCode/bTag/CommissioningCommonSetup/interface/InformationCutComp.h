#ifndef bTag_CommissioningCommonSetup_InformationCutComp_h
#define bTag_CommissioningCommonSetup_InformationCutComp_h

#include "bTag/CommissioningCommonSetup/interface/Information1D.h"

#include <vector>
#include <string>

class InformationCutComp : public Information1D
{
  public:
    // Constructors and destructors
    InformationCutComp();
    virtual ~InformationCutComp() {};

    // Setters
    inline void setCutList(const std::vector<std::string>& list) {cutList = list;}
    inline void setLabelList(const std::vector<std::string>& list) {labelList = list;}

    inline void addCutItem(const std::string& cut) {cutList.push_back(cut);}
    inline void addLabelItem(const std::string& label) {labelList.push_back(label);}

    inline std::vector<std::string> const getCutList() const {return cutList;}
    inline std::vector<std::string> const getLabelList() const {return labelList;}

    inline std::string const getCutItem(const unsigned int iter) const {return cutList.at(iter);}
    inline std::string const getLabelItem(const unsigned int iter) const {return labelList.at(iter);}

  private:
    std::vector<std::string> cutList;
    std::vector<std::string> labelList;
};

#endif

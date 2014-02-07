#include "bTag/CommissioningCommonSetup/interface/BaseInformation.h"

BaseInformation::BaseInformation() : plotType(""), plotName(""), plotTitle(""), label(""), aliasx(""), xTitle(""),
                                     cut(""), xlow(0.0), xup(0.0), nbinsx(0) {}

BaseInformation::BaseInformation(const std::string& type, const std::string& name, const std::string& title,
                                 const std::string& Label, const std::string& aliasX, const std::string& xtitle,
                                 const std::string& selection, double xmin, double xmax, unsigned int nBinsX) :
  plotType(type), plotName(name), plotTitle(title), label(Label), aliasx(aliasX), xTitle(xtitle), cut(selection),
  xlow(xmin), xup(xmax), nbinsx(nBinsX) {}

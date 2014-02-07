#ifndef bTag_CommissioningCommonSetup_BaseInformation_h
#define bTag_CommissioningCommonSetup_BaseInformation_h

// This class is used to hold standard information used
// to make bTag commissioning plots.  This should be used
// as a base class only to build other information classes.

#include <string>

class BaseInformation
{
  public:
    // Constructors and destructors
    BaseInformation();
    BaseInformation(const std::string& type, const std::string& name, const std::string& title,
                    const std::string& Label, const std::string& aliasX, const std::string& xtitle,
                    const std::string& selection, double xmin, double xmax, unsigned int nBinsX);
    virtual ~BaseInformation() {};
    
    // Setters
    inline void setPlotType(const std::string& type) {plotType = type;}
    inline void setPlotName(const std::string& name) {plotName = name;}
    inline void setPlotTitle(const std::string& title) {plotTitle = title;}
    inline void setLabel(const std::string& newLabel) {label = newLabel;}
    inline void setAliasX(const std::string& alias) {aliasx = alias;}
    inline void setXTitle(const std::string& title) {xTitle = title;}
    inline void setCut(const std::string& selection) {cut = selection;}
    inline void setXLow(const double& xMin) {xlow = xMin;}
    inline void setXUp(const double& xMax) {xup = xMax;}
    inline void setNBinsX(const unsigned int nbins) {nbinsx = nbins;}

    // Getters
    inline std::string const getPlotType() const {return plotType;}
    inline std::string const getPlotName() const {return plotName;}
    inline std::string const getPlotTitle() const {return plotTitle;}
    inline std::string const getLabel() const {return label;}
    inline std::string const getAliasX() const {return aliasx;}
    inline std::string const getXTitle() const {return xTitle;}
    inline std::string const getCut() const {return cut;}
    inline double const getXLow() const {return xlow;}
    inline double const getXUp() const {return xup;}
    inline unsigned int const getNBinsX() const {return nbinsx;}

  private:
    std::string plotType;  // the type of plot being made
    std::string plotName;  // the name of the plot
    std::string plotTitle; // the plot title
    std::string label;     // any additional label added to tree branches
    std::string aliasx;    // the name of the variable for plotting
    std::string xTitle;    // the x axis label
    std::string cut;       // the selection cut
    double xlow;           // the minimum x axis value
    double xup;            // the maximum x axis value
    unsigned int nbinsx;   // the number of x axis bins
};

#endif

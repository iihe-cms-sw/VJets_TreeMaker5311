#ifndef bTag_CommissioningCommonSetup_QualityHistogram_h
#define bTag_CommissioningCommonSetup_QualityHistogram_h

#include <string>

#include <TH1.h>

class QualityHistogram
{
  public:
    // Constructors and destructor
    QualityHistogram();
    QualityHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx, const double& xmin, const double& xmax);
    QualityHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx, const double& xmin, const double& xmax, const unsigned int nbinsy, const double& ymin, const double& ymax);
    ~QualityHistogram();

    void fill(const double& x, const double& weight, const bool isData, const int quality);
    void fill(const double& x, const double& y, const double& weight, const bool isData, const int quality);

    // Setters
    inline void setDataHistUndef(TH1* hist) {dataHist_undef = hist;}
    inline void setDataHistLoose(TH1* hist) {dataHist_loose = hist;}
    inline void setDataHistTight(TH1* hist) {dataHist_tight = hist;}
    inline void setDataHistHighPurity(TH1* hist) {dataHist_high_purity = hist;}
    inline void setMCHistUndef(TH1* hist) {mcHist_undef = hist;}
    inline void setMCHistLoose(TH1* hist) {mcHist_loose = hist;}
    inline void setMCHistTight(TH1* hist) {mcHist_tight = hist;}
    inline void setMCHistHighPurity(TH1* hist) {mcHist_high_purity = hist;}

    void setXTitle(const std::string& title);
    void setYTitle(const std::string& title);
    
    void sumW2();

    // Getters
    inline TH1* const getDataHistUndef() const {return dataHist_undef;}
    inline TH1* const getDataHistLoose() const {return dataHist_loose;}
    inline TH1* const getDataHistTight() const {return dataHist_tight;}
    inline TH1* const getDataHistHighPurity() const {return dataHist_high_purity;}
    inline TH1* const getMCHistUndef() const {return mcHist_undef;}
    inline TH1* const getMCHistLoose() const {return mcHist_loose;}
    inline TH1* const getMCHistTight() const {return mcHist_tight;}
    inline TH1* const getMCHistHighPurity() const {return mcHist_high_purity;}

  private:
    TH1* dataHist_undef;
    TH1* dataHist_loose;
    TH1* dataHist_tight;
    TH1* dataHist_high_purity;
    TH1* mcHist_undef;
    TH1* mcHist_loose;
    TH1* mcHist_tight;
    TH1* mcHist_high_purity;
};

#endif

#ifndef bTag_CommissioningCommonSetup_FlavorHistogram_h
#define bTag_CommissioningCommonSetup_FlavorHistogram_h

#include <string>

#include <TH1.h>

class FlavorHistogram
{
  public:
    // Constructors and destructor
    FlavorHistogram();
    FlavorHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx, const double& xmin, const double& xmax);
    FlavorHistogram(const std::string& name, const std::string& title, const unsigned int nbinsx, const double& xmin, const double& xmax, const unsigned int nbinsy, const double& ymin, const double& ymax);
    ~FlavorHistogram();

    void fill(const double& x, const double& weight, const bool isData, const int flavor = 0, const bool bglusplit = false);
    void fill(const double& x, const double& y, const double& weight, const bool isData, const int flavor = 0, const bool bglusplit = false);

    // Setters
    inline void setBScale(const double& scale) {bScale = scale;}
    inline void setCScale(const double& scale) {cScale = scale;}
    inline void setDataHist(TH1* hist) {dataHist = hist;}
    inline void setMCHistAll(TH1* hist) {mcHistAll = hist;}
    inline void setMCHistLight(TH1* hist) {mcHistLight = hist;}
    inline void setMCHistNone(TH1* hist) {mcHistNone = hist;}
    inline void setMCHistC(TH1* hist) {mcHistC = hist;}
    inline void setMCHistB(TH1* hist) {mcHistB = hist;}
    inline void setMCHistBGluSplit(TH1* hist) {mcHistBGluSplit = hist;}

    void setXTitle(const std::string& title);
    void setYTitle(const std::string& title);

    void sumW2();

    // Getters
    inline double const getBScale() const {return bScale;}
    inline double const getCScale() const {return cScale;}
    inline TH1* const getDataHist() const {return dataHist;}
    inline TH1* const getMCHistAll() const {return mcHistAll;}
    inline TH1* const getMCHistLight() const {return mcHistLight;}
    inline TH1* const getMCHistNone() const {return mcHistNone;}
    inline TH1* const getMCHistC() const {return mcHistC;}
    inline TH1* const getMCHistB() const {return mcHistB;}
    inline TH1* const getMCHistBGluSplit() const {return mcHistBGluSplit;}

  private:
    double bScale;
    double cScale;
    TH1* dataHist;
    TH1* mcHistAll;
    TH1* mcHistLight;
    TH1* mcHistNone; 
    TH1* mcHistC;
    TH1* mcHistB;
    TH1* mcHistBGluSplit;
};

#endif

#define btagNtupReader_cxx
//#define GENJET 
#include "Configuration.h"
//////Keywords List for argv[1]:
//(1)efficiencies under different discriminators: SSVHE SSVHP TCHE TCHP JetP JetBP CSVPF CSVMVA
//(2)average jet P_T in each pt bin: "MeanPt b u d ". " b u d "is the interested flavors. Spaces are needed on both left and right sides of the flavor name.
//(3)Reco Jet Pt vs Gen/PartonP_T plots: "MCPt b u d uds "
//(4)ratio of PU tracks in jets in different PUs: "NPUTracks b u d uds "
//#define DEBUG//turn on all information
//----------------------(A)General Cut and Parameters-----------------------
//#define CALJETMEANZ//if do not use the mean Track Longitudinal IP per jet, comment it out to speed up
//#define PARTON //partonkinematics (before hardonization)
//#define GENJET //Generated Jets (after hardonization)
//#define CLEANMCTRUTHFLAVORBYPUTRACK //MC with sim information only
//#define CLEANMCTRUTHFLAVORBYDecayLengthAndJetAsixPUTRACK // MC with sim information only 
#define JetCut(iJet) fabs(WhichEta[iJet])<2.4//Do not apply pt cut here. It will fill all into ptbins. For udsgvsb or cvsb plots, it will use (PTVAL,max) bins of the 2D histos.
#if bTagNtupleVersion>4
#define EventCut numberOfPUVerticesMixingTruth<MAXPU//&&HLTAcceptance("HLT_PFJet40")// comment it out to drop the Event cut
#else
#define EventCut numberOfPUVertices<MAXPU//&&HLTAcceptance("HLT_PFJet40")// comment it out to drop the Event cut
#endif
//----------------------(B)Pileup Variables--------------------------------
//------------the PU bins-------------------------------------
//the largest PU should be MAXPU but not MAXPU-1
const Byte_t PU_Bin_LowEdge[]={10,25,35};
const Byte_t PU_Bin_HighEdge[]={12,30,40};
const Byte_t PU_nBins=sizeof(PU_Bin_LowEdge)/sizeof(Byte_t);
//MC PU distribution
//----------------------(C)Options particularly for pt/eta plots---------------
//#define FITOPTION "pol4"//set only "polN", non-predefined function in root will lead to crash; comment it out to use direct linear fit between two nearest points
#define Required_BEffAccuracy 0.00001//the maximum tolerable discrepancy between the B eff and the required fixed B eff. It should be very small to reduce the bias. It can be zero but will be worse than small values.
#define Required_UDSGEffAccuracy 0.00005//the maximum tolerable discrepancy between the log10(UDSG eff) and the required fixed log10(UDSG eff). It should be very small to reduce the bias. It can be zero but will be worse than small values.
//-----------------------(D)Plotting Preference---------------------
#ifdef DEBUG
  #define SHOWDISCRIMINATORVALUESONPLOT //show discriminator values on non b eff vs b eff plots
#endif
#define RESULT_FILENAME "result"//the file name of the result, it will be named as result_filename+"_"+option+".root"
#define PLOTTITLE "CMS 2011 simulation preliminary, #sqrt{s} = 7 TeV"
const EColor ColorList[]={kBlack,kRed,kGreen,kMagenta,kBlue,kCyan,kGray,EColor(kOrange+7)}; const unsigned char numColor=sizeof(ColorList)/sizeof(EColor);
#define NDISCRIMINATORBINS 5000
#define NEFFPOINTS 25//appoximate number of points on non B eff vs B eff plots
#define MAKEJetVarPlot
//#define SHOWSECONDERRORBAR
const Float_t EtaBins[]={0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0,2.2,2.4}; const UInt_t nEtaBins=sizeof(EtaBins)/sizeof(Float_t)-1;
const Float_t PtBins[]={20,40,60,80,100,130,160,190,220,300,400,500}; const UInt_t nPtBins=sizeof(PtBins)/sizeof(Float_t)-1;
#define SavePDFFormat
#ifdef PARTON
  #define WhichPt PartonPt
  #define WhichEta PartonEta
#else
  #ifdef GENJET
    #define WhichPt GenJetPt
    #define WhichEta GenJetEta
  #else
    #define WhichPt jetPt
    #define WhichEta jetEta
  #endif
#endif

//################################################################
//#####   End of Settings for Efficiency Calculation codes   #####
//################################################################
#include "btagNtupReader.h"
#include "setTDRStyle.h"
#include <TH2.h>
#include <TF1.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLegendEntry.h>
#include <TText.h>
#include <THStack.h>
#include <TCanvas.h>
#include <math.h>
#include <string.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TEfficiency.h>
#include <TApplication.h>
#include <TLatex.h>
using namespace std;

Float_t puweight[MAXPU+1];

#define NFLAVORS 12
const char *Flavor_Names[]={"b","bgluonsplit","c","cgluonsplit","u","d","s","g","NoFlavor","uds","udsg","udsgwtNoFlavor"};
enum Flavor {B=0,Bgluonsplit=1,C=2,Cgluonsplit=3,U=4,D=5,S=6,G=7,NoFlavor=8,UDS=9,UDSG=10,UDSGwtNoFlavor=11};
template <typename T> class DistributionPlots {
public:
  DistributionPlots() {}
  DistributionPlots(DistributionPlots *PlotsToCopy) {
    for (int i=0;i<NFLAVORS; i++)
      fplots[i]=(T*) PlotsToCopy->fplots[i]->Clone();
  }
  virtual void Add(DistributionPlots *PlotsToAdd, Double_t weight=1.) {
    for (int i=0;i<NFLAVORS; i++) {
      fplots[i]->Add(PlotsToAdd->fplots[i],weight);      
    }
  }
  virtual void Divide(DistributionPlots *denominator) {
    for (int i=0;i<NFLAVORS; i++)
      fplots[i]->Divide(denominator->fplots[i]);
  }
  virtual void Divide(DistributionPlots *numerator,DistributionPlots *denominator) {
    for (int i=0;i<NFLAVORS; i++) {
      if ( fplots[i] ==NULL ) fplots[i] = (T*) numerator->fplots[i]->Clone();
      fplots[i]->Divide(numerator->fplots[i],denominator->fplots[i]);      
    }
  }
  virtual void Scale(Double_t weight=1.) {
    for (int i=0;i<NFLAVORS; i++)
      fplots[i]->Scale(weight);
  }
  virtual void Reset(Option_t* option = "") {
    for (int i=0;i<NFLAVORS; i++)
      fplots[i]->Reset(option);
  }
  virtual void Write(const char *FlavorSelection=" b udsg ") {
    for (int i=0;i<NFLAVORS; i++) {
      char tempstr[50];
      sprintf(tempstr," %s ",Flavor_Names[i]);
      if ( strstr(FlavorSelection,tempstr)!=NULL )
	fplots[i]->Write();
    }
  }
  virtual T* Total() {
    T* total=(T*) fplots[0]->Clone();
    for (int i=1;i<NFLAVORS; i++)
      total->Add(fplots[i]);
    return total;
  }
  inline void NameParsing (const char *namestring, char *newname, char *newtitle, const char *flavorname) {
    const char *column=strchr(namestring,';');
    if (column!=NULL) {
      char titlename[150];
      strncpy(titlename,namestring,column-namestring);
      titlename[column-namestring]='\0';
      sprintf(newname,"%s_%s",titlename,flavorname);
      sprintf(newtitle,"%s_%s%s",titlename,flavorname,column);
    }
    else {
      sprintf(newname,"%s_%s",namestring,flavorname);
      sprintf(newtitle,"%s_%s",namestring,flavorname);
    }
  }
  virtual T* GetHisto(Byte_t whichone) {return fplots[whichone];}
  virtual void DumpInfo(const char *FlavorSelection=" b udsg ") {
    for (int i=0;i<NFLAVORS; i++) {
      char tempstr[50];
      sprintf(tempstr," %s ",Flavor_Names[i]);
      if ( strstr(FlavorSelection,tempstr)!=NULL ) {
	printf("%.0f %s jets included.\n",fplots[i]->GetEntries(),Flavor_Names[i]);
      }
    }
  }
  virtual void SetAxisTitles(const char* Xtitle, const char* Ytitle="", const char* Ztitle="") {
    for (int i=0;i<NFLAVORS; i++) {
      fplots[i]->SetXTitle(Xtitle);
      fplots[i]->SetYTitle(Ytitle);
      fplots[i]->SetZTitle(Ztitle);
    }
  }
  virtual ~DistributionPlots() { delete[] fplots;}
protected:
  T *fplots[NFLAVORS];
};

class DistributionPlots2D: public DistributionPlots<TH2D> {
 public:
  DistributionPlots2D():DistributionPlots<TH2D>() {}
  DistributionPlots2D(DistributionPlots2D *PlotsToCopy): DistributionPlots<TH2D>(PlotsToCopy) {}
  DistributionPlots2D(const char *namestring, const Int_t Discriminator_nBins, const Float_t * Discriminator_Bins,  const Int_t JetVar_nBins, const Float_t * JetVar_Bins) {
    for (int i=0;i<NFLAVORS; i++) {
      char newname[200],newtitle[200];
      NameParsing(namestring,newname,newtitle,Flavor_Names[i]);
      fplots[i]=new TH2D( newname,newtitle,Discriminator_nBins,Discriminator_Bins,JetVar_nBins,JetVar_Bins);
      fplots[i]->Sumw2();
    }
  }
  virtual Double_t GetEff(Float_t *Eff, Float_t *EffupError, Float_t *EffdownError, Flavor WhichFlavor, Int_t firstJetVarBin=0,Int_t lastJetVarBin=-1);
  virtual void GetJetVarPlot(TGraphAsymmErrors **, TGraphAsymmErrors **, const Byte_t nPlotFlavors, Flavor *PlotFlavor, Flavor RequiredEff_Flavor, Float_t RequiredEff, Float_t RequiredEffAccuracy, Bool_t *IsLogY, Bool_t IsLogX=false);
  inline void CorrectResult(Float_t *result,Float_t *result_upError,Float_t *result_downError) {
    if (*result<1E-5) {
      *result=1E-5;
      *result_downError=1E-5;
    }
    if (*result>=1.) {
      *result=1.;
      *result_upError=0.;
    }
    if (*result+*result_upError>1.) *result_upError=1.-*result;
    if (*result_downError<0)  *result_downError=1E-10;
    if (*result_downError>*result) *result_downError=*result;
  }
  virtual void Fill(Flavor WhichFlavor,Float_t x,Float_t y,Float_t weight=1.) {
    fplots[Int_t(WhichFlavor)]->Fill(x,y,weight);
  }
  virtual void DrawAndWrite(const char *FlavorSelection=" b udsg ") {
    for (Byte_t i=0;i<NFLAVORS; i++) {
      char tempstr[50];
      sprintf(tempstr," %s ",Flavor_Names[i]);
      if ( strstr(FlavorSelection,tempstr)==NULL ) continue;
      TCanvas *canvas=new TCanvas(fplots[i]->GetName(),fplots[i]->GetTitle(),500,500);
      fplots[i]->SetMarkerSize(0.6);
      fplots[i]->Draw("COLZ");
      fplots[i]->Draw("same,text");
      gPad->SetLogz();
      canvas->Write();
    }
  }
};

class DistributionPlots1D: public DistributionPlots<TH1D> {
public:
  DistributionPlots1D():DistributionPlots<TH1D>() {}
  DistributionPlots1D(DistributionPlots1D *PlotsToCopy): DistributionPlots<TH1D>(PlotsToCopy) {}
  DistributionPlots1D(char *namestring, const Int_t nBins, const Float_t * Bins) {
    for (int i=0;i<NFLAVORS; i++) {
      char newname[200],newtitle[200];
      NameParsing(namestring,newname,newtitle,Flavor_Names[i]);
      fplots[i]=new TH1D( newname,newtitle,nBins,Bins);
      fplots[i]->Sumw2();
    }
  }
  virtual void Fill(Flavor WhichFlavor,Float_t x,Float_t weight=1.) {
    fplots[Int_t(WhichFlavor)]->Fill(x,weight);
  }
  virtual void FillVariance(Flavor WhichFlavor,Float_t x,DistributionPlots1D *Average) {
    TH1D *AveragePlot=Average->fplots[Int_t(WhichFlavor)];
    Int_t ibin=AveragePlot->FindBin(x);
    fplots[Int_t(WhichFlavor)]->AddBinContent( ibin,pow(x-AveragePlot->GetBinContent(ibin),2) );
  }
  virtual void SetBinError(DistributionPlots1D *Variance) {
    Int_t nbins=fplots[0]->GetNbinsX();
    for (int i=0;i<NFLAVORS; i++) 
      for (Int_t ibin=0;ibin<=nbins;ibin++)
	fplots[i]->SetBinError( ibin, sqrt(Variance->fplots[i]->GetBinContent(ibin)) );
  }
};

class Plotting : public btagNtupReader{
public :
  Plotting(string filename):btagNtupReader(filename){
  //Speed up by opening only useful branches
    fChain->SetBranchStatus("*",0);
    fChain->SetBranchStatus("HLTriggerResults",1);
    fChain->SetBranchStatus("pthat*",1);
    fChain->SetBranchStatus("nJets*",1);
    fChain->SetBranchStatus("nTracks*",1);
    fChain->SetBranchStatus("track*",1);
    fChain->SetBranchStatus("jet*",1);
    fChain->SetBranchStatus("isBGluonSplitting*",1);
    fChain->SetBranchStatus("isCGluonSplitting*",1);
    fChain->SetBranchStatus("numberOfPUVertices*",1);
    fChain->SetBranchStatus("MCTrueFlavor*",1);
    fChain->SetBranchStatus("standard*",1);
#if bTagNtupleVersion>=4
    fChain->SetBranchStatus("Parton*",1);
    fChain->SetBranchStatus("Gen*",1);  
    fChain->SetBranchStatus("IPnSelected*",1);  
#endif
  }
  enum DiscriminatorOption {TCHE2D=1,TCHP2D=3,ReCalTCHE=2,ReCalTCHP=4,JetProb=5,JetBProb=6,CSVMVA=7,CSVPF=8,SSVHE=9,SSVHP=10,TCHE=11,TCHP=12,GTPF=13,JetDistStudyHighEff=22,JetDistStudyHighPur=23};
  inline  void Jet_MeanTrackLongitudinalIP(Float_t *result) {
    Byte_t NTracksInJet[nJets];
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
      result[iJet]=0.;NTracksInJet[iJet]=0;
     }
    for ( Int_t iTrack=0; iTrack<nTracks; iTrack++ ) {
      result[trackJetIndex[iTrack]]+=trackLongitudinalImpactParameter[iTrack];
      NTracksInJet[trackJetIndex[iTrack]]++;
    }
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
      result[iJet]=result[iJet]/NTracksInJet[iJet];
      //cerr<<Int_t(iJet)<<":"<<result[iJet]<<"\t";
    }
  }
  inline void WhichFlavors(Byte_t iJet,vector<Flavor> &theflavors);
  virtual void FlavourHist(DiscriminatorOption option, const Float_t weight, DistributionPlots2D **VsJetPt, DistributionPlots2D **VsJetEta, DistributionPlots2D **VsJetPhi=NULL, DistributionPlots2D **VsEventPtHat=NULL);
  virtual void MeanPt(DistributionPlots1D **TotalPt, DistributionPlots1D **TotalEntries);
  virtual void UncertaintyOfMeanPt(DistributionPlots1D *Average, DistributionPlots1D **Variance,Byte_t PU_LowEdge,Byte_t PU_HighEdge);
#if bTagNtupleVersion==4
  virtual void NPUTracks(DistributionPlots2D *,DistributionPlots2D *,DistributionPlots2D *,DistributionPlots2D *);
#endif
#if bTagNtupleVersion>=4
  virtual void MCPtvsRecoPt(DistributionPlots2D **GenPtvsJetPt, DistributionPlots2D **PartonvsJetPt, DistributionPlots1D **, DistributionPlots1D **);
#endif
  virtual void PileUpDistribution(TH1D* dis) {
     if (fChain == 0) return;
     Long64_t nentries = fChain->GetEntries();//GetEntriesFast() will give you something wrong in the first running
     for (Long64_t jentry=0; jentry<nentries;jentry++) {
       if ( LoadTree(jentry) < 0 ) break;
       fChain->GetEntry(jentry);
       dis->Fill(Double_t(numberOfPUVertices));
     }
  }
  virtual Long64_t NEntries() {if (fChain) return fChain->GetEntries();else return 0;}
  //  static Float_t TrackCuts[7];

private:
  Float_t * SetVar(DiscriminatorOption option) {
    switch (option) {
    case JetProb:
      return standardJetProbabilityPFBJetTags;
    case JetBProb:
      return standardJetBProbabilityPFBJetTags;
    case CSVMVA:
      return standardCombinedSecondaryVertexMVAPFBJetTags;
    case CSVPF:
      return standardCombinedSecondaryVertexPFBJetTags;
    case SSVHE:
      return standardSimpleSecondaryVertexHighEffPFBJetTags;
    case SSVHP:
      return standardSimpleSecondaryVertexHighPurPFBJetTags;
    case TCHE:
      return standardTrackCountingHighEffPFBJetTags;
    case TCHP:
      return standardTrackCountingHighPurPFBJetTags;
    case GTPF:
      return standardGhostTrackPFBJetTags;
    default:
      printf("Option \"%d\" is meaningless.",Int_t(option));
      return NULL;
    }
  }
};

UInt_t SelectPoints(UInt_t NPoints,Float_t *Ref_eff_X,Float_t *Ref_eff_XdownError,Float_t *Ref_eff_XupError,Float_t *eff_X,Float_t *eff_Y,Float_t *eff_XdownError,Float_t *eff_XupError,Float_t *eff_YdownError,Float_t *eff_YupError,Float_t Increment_X,Float_t Increment_Y, Bool_t IsLogY) {
  UInt_t NReturnPoints=0;
  eff_X[0]=Ref_eff_X[0];
  eff_XdownError[0]=Ref_eff_XdownError[0];
  eff_XupError[0]=Ref_eff_XupError[0];
  for (UInt_t i=1;i<NPoints;i++) {
    if (IsLogY) {
      if (eff_X[NReturnPoints]-Ref_eff_X[i]<Increment_X&&log10(eff_Y[NReturnPoints]/eff_Y[i])<Increment_Y)  continue;
    }
    else {
      if (eff_X[NReturnPoints]-Ref_eff_X[i]<Increment_X&&eff_Y[NReturnPoints]-eff_Y[i]<Increment_Y)  continue;
    }
    NReturnPoints++;
    eff_X[NReturnPoints]=Ref_eff_X[i];
    eff_Y[NReturnPoints]=eff_Y[i];
    eff_XdownError[NReturnPoints]=Ref_eff_XdownError[i];
    eff_XupError[NReturnPoints]=Ref_eff_XupError[i];
    eff_YdownError[NReturnPoints]=eff_YdownError[i];
    eff_YupError[NReturnPoints]=eff_YupError[i];
  }
  return NReturnPoints;
}

template <typename T>
void SetPlotStyle(T *plot, char * name, EColor color, Int_t LineStyle=1, Int_t MarkerStyle=20) {
    plot->SetName(name);
    plot->SetLineColor(color);
    plot->SetLineStyle(LineStyle);
    plot->SetMarkerColor(color);
    plot->SetLineWidth(2);
    plot->SetMarkerStyle(MarkerStyle);
    plot->SetMarkerSize(0.4);
}

Float_t MINUDSG_JetVar=8E-4,MAXUDSG_JetVar=0.3,MINBC_JetVar=0.15,MAXBC_JetVar=0.9,MINB=0,MAXB=1.,MINUDSG=1E-4,MAXUDSG=1.,MINC=1E-4,MAXC=1.;
inline void SetAxisesRanges(Float_t fMINUDSG_JetVar=8E-4,Float_t fMAXUDSG_JetVar=0.3,Float_t fMINBC_JetVar=0.15,Float_t fMAXBC_JetVar=0.9,Float_t fMINB=0,Float_t fMAXB=1.,Float_t fMINUDSG=1E-4,Float_t fMAXUDSG=1.,Float_t fMINC=1E-4,Float_t fMAXC=1.) {
  MINUDSG_JetVar=fMINUDSG_JetVar;  MAXUDSG_JetVar=fMAXUDSG_JetVar;
  MINBC_JetVar=fMINBC_JetVar;  MAXBC_JetVar=fMAXBC_JetVar;
  MINB=fMINB;  MAXB=fMAXB;
  MINUDSG=fMINUDSG;  MAXUDSG=fMAXUDSG;
  MINC=fMINC;  MAXC=fMAXC;
}

void DrawDiscirminatorValue(UInt_t NPoints,Float_t *eff_X,Float_t *eff_Y,const Float_t *Discriminator_Values) {
  Float_t LastPt_X=-1.,LastPt_Y=-1;
  Bool_t printtwobits=Discriminator_Values[0]-Discriminator_Values[NPoints]>1.1?true:false;
  for ( UInt_t iCut=0; iCut<=NPoints; iCut++ )
    if (fabs(eff_X[iCut]-LastPt_X)>(MAXB-MINB)/10.||fabs(eff_Y[iCut]-LastPt_Y)>(MAXUDSG-MINUDSG)/10.) {
      char tmptext[20];
      if (printtwobits) sprintf(tmptext,"%0.2f",Discriminator_Values[iCut]);
      else sprintf(tmptext,"%0.1f",Discriminator_Values[iCut]);
      LastPt_X=eff_X[iCut];LastPt_Y=eff_Y[iCut];
      TText *t = new TText(LastPt_X,LastPt_Y,tmptext);
      t->SetTextAlign(12);
      t->SetTextColor(kGray+2);
      t->SetTextSize(0.035);
      t->Draw();
    }
}

inline void SetLegendStyle(TLegend *Legends) {
  Legends->SetBorderSize(0);
  Legends->SetTextSize(0.03);
  //Legends->SetFillStyle(0); //transparent background
}

#if bTagNtupleVersion>=4
void MCPtvsRecoPt (Plotting **MC, const char *options) {
  char namestring[100],legendtext[100];
  TLegend *Legends = new TLegend(0.16,0.72,0.5,0.95);
  SetLegendStyle(Legends);
  const UShort_t NRelPtBins=100;
  const Float_t MinRelPt=-2.,MaxRelPt=5.;
  Float_t RelPtBins[NRelPtBins+1];
  for (UShort_t iRelPtBin=0; iRelPtBin<=NRelPtBins; iRelPtBin++) 
    RelPtBins[iRelPtBin]=MinRelPt+(MaxRelPt-MinRelPt)*iRelPtBin/NRelPtBins;
  DistributionPlots2D *GenPtvsJetPt[nSamples][MAXPU+1],*PartonvsJetPt[nSamples][MAXPU+1];
  DistributionPlots1D *BiasFromGenPt[nSamples][MAXPU+1],*BiasFromPartonPt[nSamples][MAXPU+1];
  //-------------Fill TH2D from samples----------------------
  for (UInt_t iSample=0;iSample<nSamples;iSample++) {
    for (Byte_t iPU=0; iPU<MAXPU+1; iPU++) {
      sprintf(namestring,"GenPtvsJetPt_Sample%d_%d",iSample,iPU);
      GenPtvsJetPt[iSample][iPU] = new DistributionPlots2D(namestring,nPtBins,PtBins,nPtBins,PtBins);
      sprintf(namestring,"PartonvsJetPt_Sample%d_%d",iSample,iPU);
      PartonvsJetPt[iSample][iPU] = new DistributionPlots2D(namestring,nPtBins,PtBins,nPtBins,PtBins);
      sprintf(namestring,"BiasFromGenPt_Sample%d_%d",iSample,iPU);
      BiasFromGenPt[iSample][iPU] = new DistributionPlots1D(namestring,NRelPtBins,RelPtBins);
      sprintf(namestring,"BiasFromPartonPt_Sample%d_%d",iSample,iPU);
      BiasFromPartonPt[iSample][iPU] = new DistributionPlots1D(namestring,NRelPtBins,RelPtBins);
    }
    MC[iSample]->MCPtvsRecoPt(GenPtvsJetPt[iSample],PartonvsJetPt[iSample],BiasFromGenPt[iSample],BiasFromPartonPt[iSample]);
  }
  TCanvas *canvas_Gen=new TCanvas("RelGenPt","RelGenPt",500,500);
  canvas_Gen->DrawFrame(MinRelPt,0,MaxRelPt,50,"BiasFromGenPt;#Delta Pt/GenJetPt;N Jets");
  TCanvas *canvas_Parton=new TCanvas("RelPartonPt","RelPartonPt",500,500);
  canvas_Parton->DrawFrame(MinRelPt,0,MaxRelPt,50,"BiasFromPartonPt;#Delta Pt/PartonPt;N Jets");
  for (Byte_t PU_iBin=0; PU_iBin<PU_nBins; PU_iBin++) {
    printf("####################### PU = %d to %d ##########################\n",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    sprintf(namestring,"PU%dto%d:GenPtvsJetPt;JetPt (GeV/c);GenJetPt (GeV/c)",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots2D *GenPtvsJetPt_iPUBin = new DistributionPlots2D(namestring,nPtBins,PtBins,nPtBins,PtBins);
    sprintf(namestring,"PU%dto%d:PartonvsJetPt;JetPt (GeV/c);PartonPt (GeV/c)",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots2D *PartonvsJetPt_iPUBin = new DistributionPlots2D(namestring,nPtBins,PtBins,nPtBins,PtBins);
    sprintf(namestring,"PU%dto%d:BiasFromGenPt;#Delta Pt/GenJetPt;N Jets",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots1D *BiasFromGenPt_iPUBin = new DistributionPlots1D(namestring,NRelPtBins,RelPtBins); 
    sprintf(namestring,"PU%dto%d:BiasFromPartonPt;#Delta Pt/PartonPt;N Jets",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots1D *BiasFromPartonPt_iPUBin = new DistributionPlots1D(namestring,NRelPtBins,RelPtBins); 

    for (UInt_t iSample=0;iSample<nSamples;iSample++) 
      for (Byte_t PU=PU_Bin_LowEdge[PU_iBin]; PU<=PU_Bin_HighEdge[PU_iBin]; PU++) {
	GenPtvsJetPt_iPUBin ->Add(GenPtvsJetPt[iSample][PU],puweight[PU]*MC_Weights[iSample]);
	PartonvsJetPt_iPUBin->Add(PartonvsJetPt[iSample][PU],puweight[PU]*MC_Weights[iSample]);
	BiasFromGenPt_iPUBin->Add(BiasFromGenPt[iSample][PU],puweight[PU]*MC_Weights[iSample]);
	BiasFromPartonPt_iPUBin->Add(BiasFromPartonPt[iSample][PU],puweight[PU]*MC_Weights[iSample]);
      }
    GenPtvsJetPt_iPUBin -> DrawAndWrite(options);
    PartonvsJetPt_iPUBin -> DrawAndWrite(options);
    Byte_t nSelectedFlavors=0;
    for (Byte_t iFlavor=0;iFlavor<NFLAVORS; iFlavor++) {
      char tempstr[50];
      sprintf(tempstr," %s ",Flavor_Names[iFlavor]);
      if ( strstr(options,tempstr)==NULL ) continue;
      nSelectedFlavors++;
      EColor color=ColorList[(PU_iBin+nSelectedFlavors*PU_nBins+1)%numColor];
      TH1D *Histo=BiasFromGenPt_iPUBin->GetHisto(iFlavor);
      sprintf(namestring,"PU%dto%d_BiasFromGenPt_%s",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Flavor_Names[iFlavor]);
      SetPlotStyle(Histo,namestring,color);
      Histo -> Scale( 100./Histo->Integral(0,NRelPtBins+2) );
      canvas_Gen->cd();
      Histo -> Draw("E1,same");
      canvas_Parton->cd();
      Histo=BiasFromPartonPt_iPUBin->GetHisto(iFlavor);
      sprintf(namestring,"PU%dto%d_BiasFromPartonPt_%s",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Flavor_Names[iFlavor]);
      SetPlotStyle(Histo,namestring,color);
      Histo -> Scale( 100/Histo->Integral(0,NRelPtBins+2) );
      Histo -> Draw("E1,same");
      if (PU_Bin_LowEdge[PU_iBin]!=PU_Bin_HighEdge[PU_iBin]) sprintf(legendtext,"PU = %dto%d:%s",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Flavor_Names[iFlavor]);
      else sprintf(legendtext,"PU = %d:%s",PU_Bin_LowEdge[PU_iBin],Flavor_Names[iFlavor]);
      Legends->AddEntry(Histo,legendtext,"lep");
    }
  }
  canvas_Gen->cd();
  Legends->Draw();
  canvas_Gen->Write();
  canvas_Gen->SaveAs("JetPtRelGenPt.pdf");
  canvas_Parton->cd();
  Legends->Draw();
  canvas_Parton->Write();
  canvas_Parton->SaveAs("JetPtRelPartonPt.pdf");
}
#endif

#define DRAWUNCERTAINTY
void MeanPtPlot (Plotting **MC, const char *options) {
  char namestring[100],legendtext[100]; 
  TLegend *Legends = new TLegend(0.16,0.72,0.5,0.95);
  SetLegendStyle(Legends);
  DistributionPlots1D *total[nSamples][MAXPU+1],*nentries[nSamples][MAXPU+1];
  for (UInt_t iSample=0;iSample<nSamples;iSample++) {
    for (Byte_t iPU=0; iPU<MAXPU+1; iPU++) {
      sprintf(namestring,"TotalPt_Sample%d_%d",iSample,iPU);
      total[iSample][iPU] = new DistributionPlots1D(namestring,nPtBins,PtBins);
      sprintf(namestring,"NEntries_Sample%d_%d",iSample,iPU);
      nentries[iSample][iPU] = new DistributionPlots1D(namestring,nPtBins,PtBins);
    }
    MC[iSample]->MeanPt(total[iSample],nentries[iSample]);
  }
#ifdef DRAWUNCERTAINTY
  TCanvas *Canvas_MeanPt = new TCanvas("JetMeanPt","JetMeanPt",500,500);
  Canvas_MeanPt->DrawFrame(PtBins[0],PtBins[0],PtBins[nPtBins],PtBins[nPtBins],"JetMeanPt;p_{T} (GeV/c);p_{T} (GeV/c)");
#endif
  TCanvas *Canvas_MeanPt_BarPlot = new TCanvas("JetMeanPt_BarPlot","JetMeanPt_BarPlot",500,500);
  Canvas_MeanPt_BarPlot->DrawFrame(PtBins[0],PtBins[0],PtBins[nPtBins],PtBins[nPtBins],"JetMeanPt;p_{T} (GeV/c);p_{T} (GeV/c)");
  for (Byte_t PU_iBin=0; PU_iBin<PU_nBins; PU_iBin++) {
    printf("####################### PU = %d to %d ##########################\n",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    EColor color=ColorList[PU_iBin%numColor];
    sprintf(namestring,"PU%dto%d:MeanPt",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots1D *MeanPt_iPUBin = new DistributionPlots1D(namestring,nPtBins,PtBins);
    sprintf(namestring,"PU%dto%d:NEntries",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots1D *NEntries_iPUBin = new DistributionPlots1D(namestring,nPtBins,PtBins);
    for (UInt_t iSample=0;iSample<nSamples;iSample++) 
      for (Byte_t PU=PU_Bin_LowEdge[PU_iBin]; PU<=PU_Bin_HighEdge[PU_iBin]; PU++) {
	MeanPt_iPUBin ->Add(total[iSample][PU],puweight[PU]*MC_Weights[iSample]);
#ifdef DEBUG
	printf("Adding %dth sample with %d PU, the weight is %.5f*%.5f=%.5f\n", iSample, PU,MC_Weights[iSample],puweight[PU],puweight[PU]*MC_Weights[iSample]);
	total[iSample][PU]->DumpInfo();
#endif
	NEntries_iPUBin->Add(nentries[iSample][PU],puweight[PU]*MC_Weights[iSample]);
      }
    MeanPt_iPUBin->Divide(NEntries_iPUBin);
    MeanPt_iPUBin->Write(options);
    NEntries_iPUBin->Write(options);
    TH1D *TotalJets=NEntries_iPUBin->Total();
    TotalJets->Write(namestring);
#ifdef DRAWUNCERTAINTY
    //Calculate Uncertainty
     DistributionPlots1D *Variance[nSamples][MAXPU+1];
    for (UInt_t iSample=0;iSample<nSamples;iSample++) {
      for (Byte_t iPU=0; iPU<MAXPU+1; iPU++) {
	sprintf(namestring,"VariancePt_PU%dto%d:Sample%d_%d",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],iSample,iPU);
	Variance[iSample][iPU] = new DistributionPlots1D(namestring,nPtBins,PtBins);
      }
      MC[iSample]->UncertaintyOfMeanPt(MeanPt_iPUBin,Variance[iSample],PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    }
    sprintf(namestring,"PU%dto%d:Variance_EtavsDis",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    DistributionPlots1D *Variance_iPUBin = new DistributionPlots1D(namestring,nPtBins,PtBins);
    for (UInt_t iSample=0;iSample<nSamples;iSample++) 
      for (Byte_t PU=PU_Bin_LowEdge[PU_iBin]; PU<=PU_Bin_HighEdge[PU_iBin]; PU++)
	Variance_iPUBin->Add(Variance[iSample][PU],puweight[PU]*MC_Weights[iSample]);
    Variance_iPUBin->Divide(NEntries_iPUBin);
    MeanPt_iPUBin->SetBinError(Variance_iPUBin);
#endif
    Byte_t nSelectedFlavors=0;
    for (Byte_t iFlavor=0;iFlavor<NFLAVORS; iFlavor++) {
      char tempstr[50];
      sprintf(tempstr," %s ",Flavor_Names[iFlavor]);
      if ( strstr(options,tempstr)!=NULL ) nSelectedFlavors++;
    }
    Float_t binwidth=0.9/nSelectedFlavors/PU_nBins;
    nSelectedFlavors=0;
    for (Byte_t iFlavor=0;iFlavor<NFLAVORS; iFlavor++) {
      char tempstr[50];
      sprintf(tempstr," %s ",Flavor_Names[iFlavor]);
      if ( strstr(options,tempstr)==NULL ) continue;
      nSelectedFlavors++;
      color=ColorList[(PU_iBin+nSelectedFlavors*PU_nBins)%numColor];
      TH1D *Histo=(TH1D*) MeanPt_iPUBin->GetHisto(iFlavor)->Clone();
      sprintf(namestring,"PU%dto%d_MeanPt_barplot_%s",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Flavor_Names[iFlavor]);
      SetPlotStyle(Histo,namestring,color);
      Float_t offset=(PU_iBin+nSelectedFlavors*PU_nBins)*binwidth+0.05;
      Canvas_MeanPt_BarPlot->cd();
      Histo->SetBarOffset(offset);
      Histo->SetBarWidth(binwidth);
      Histo->SetFillColor(color);
      Histo->Draw("BAR same");
#ifdef DRAWUNCERTAINTY
      Canvas_MeanPt->cd();
      Float_t newBinning[nPtBins+1];
      newBinning[0]=PtBins[0];
      for (Byte_t iptbin=0;iptbin<nPtBins;iptbin++) {
	Float_t values_X=PtBins[iptbin]+(offset+binwidth/2.)*(PtBins[iptbin+1]-PtBins[iptbin]);
	newBinning[iptbin+1]=2*values_X-newBinning[iptbin];
      }
     sprintf(namestring,"PU%dto%d_MeanPtwtUncertainty_%s",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Flavor_Names[iFlavor]);
       TH1D *HistowtError=new TH1D(namestring,namestring,nPtBins,newBinning);
      for (Byte_t iptbin=1;iptbin<=nPtBins;iptbin++) {
	HistowtError->SetBinContent(iptbin,Histo->GetBinContent(iptbin));
	HistowtError->SetBinError(iptbin,Histo->GetBinError(iptbin));
      }
      SetPlotStyle(HistowtError,namestring,color);
      HistowtError->Draw("E same");
#endif
      if (PU_Bin_LowEdge[PU_iBin]!=PU_Bin_HighEdge[PU_iBin]) sprintf(legendtext,"PU = %dto%d:%s",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Flavor_Names[iFlavor]);
      else sprintf(legendtext,"PU = %d:%s",PU_Bin_LowEdge[PU_iBin],Flavor_Names[iFlavor]);
      Legends->AddEntry(Histo,legendtext,"F");
    }
  }//end of loop PU_iBin
#ifdef DRAWUNCERTAINTY
  Canvas_MeanPt->cd();
  gPad->SetGrid();
  Legends->Draw();
  Canvas_MeanPt->Write();
#ifdef SavePDFFormat
  Canvas_MeanPt->SaveAs("MeanPt.pdf");
#endif
#endif
  Canvas_MeanPt_BarPlot->cd();
  gPad->SetGrid();
  Legends->Draw();
  Canvas_MeanPt_BarPlot->Write();
#ifdef SavePDFFormat
  Canvas_MeanPt_BarPlot->SaveAs("MeanPt_BarPlot.pdf");
#endif
  // cin.get();
  return;
}

void EfficienciesPlotting (Plotting **MC, const char *Title, const Float_t *Discriminator_Bins, const UInt_t Discriminator_nBins, Plotting::DiscriminatorOption discriminator_option,Float_t RequiredBEff, Float_t RequiredUDSGEff_ETA, Float_t RequiredUDSGEff_PT) {
  char result_filename[100],namestring[100],legendtext[100]; 
  TLegend *Legends_Efficiencies = new TLegend(0.23,0.67,0.46,0.9);
  SetLegendStyle(Legends_Efficiencies);
  Float_t Ref_eff_X[Discriminator_nBins+1],Ref_eff_XupError[Discriminator_nBins+1],Ref_eff_XdownError[Discriminator_nBins+1],
    eff_X[Discriminator_nBins+1],eff_XupError[Discriminator_nBins+1],eff_XdownError[Discriminator_nBins+1],
    eff_Y[Discriminator_nBins+1],eff_YupError[Discriminator_nBins+1],eff_YdownError[Discriminator_nBins+1];
  DistributionPlots2D *VsJetPt_plots[MAXPU+1],*VsJetEta_plots[MAXPU+1];
  //-------------Fill TH2D from samples----------------------
  for (Byte_t iPU=0; iPU<MAXPU+1; iPU++) {
    sprintf(namestring,"%s_Pt_PU%d",Title,iPU);
    VsJetPt_plots[iPU] = new DistributionPlots2D(namestring,Discriminator_nBins,Discriminator_Bins,nPtBins,PtBins);
    sprintf(namestring,"%s_Eta_PU%d",Title,iPU);
    VsJetEta_plots[iPU] = new DistributionPlots2D(namestring,Discriminator_nBins,Discriminator_Bins,nEtaBins,EtaBins);
  }
  //for udsgvsb,cvsb efficiency plots, we cut the pt at PTVAL
  Byte_t PtBinCut=1;
  for (;PtBinCut<nPtBins;PtBinCut++)
    if  ( PtBins[PtBinCut-1]>=(PTVAL-1E-4) ) break;
  for (UInt_t iSample=0;iSample<nSamples;iSample++) 
    MC[iSample]->FlavourHist(discriminator_option,MC_Weights[iSample],VsJetPt_plots,VsJetEta_plots);
  //-------------make plots-----------------------------
  TLatex latex;
//11=horizontal left vertical bottom
//31=horizontal right vertical bottom
#define DrawTitle(MINX,MAXX,MAXY) latex.SetTextAlign(31);	\
  latex.SetTextSize(0.05);					\
  latex.DrawLatex(MAXX,MAXY,Title);			\
  latex.SetTextSize(0.03);				\
  latex.SetTextAlign(11);			\
  latex.DrawLatex(MINX,MAXY,PLOTTITLE)

  sprintf(namestring,"eff_BvsUDSGwt%s",Title);
  TCanvas *Canvas_eff_BvsUDSG = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;b jets efficiency;udsg jets efficiency",PLOTTITLE);
  //TH1F * tempBvsUDSG=
  Canvas_eff_BvsUDSG->DrawFrame(MINB,MINUDSG,MAXB,MAXUDSG,namestring);
  //tempBvsUDSG->GetXaxis()->SetNdivisions(10);
  //tempBvsUDSG->GetXaxis()->SetLabelSize(0.05);
  //tempBvsUDSG->GetYaxis()->SetTickLength(1.);
  DrawTitle(MINB,MAXB,MAXUDSG*1.1);

  sprintf(namestring,"eff_BvsCwt%s",Title);
  TCanvas *Canvas_eff_BvsC = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;b jets efficiency;c jets efficiency",PLOTTITLE);
  //TH1F * tempBvsC=
  Canvas_eff_BvsC->DrawFrame(MINB,MINC,MAXB,MAXC,namestring);
  //tempBvsC->GetXaxis()->SetNdivisions(10);
  //tempBvsC->GetXaxis()->SetLabelSize(0.05);
  //tempBvsC->GetYaxis()->SetNdivisions(10);
  //tempBvsC->GetYaxis()->SetLabelSize(0.05);
  DrawTitle(MINB,MAXB,MAXC*1.01);

  sprintf(namestring,"eff_BvsUDSGwtPUwt%s",Title);
  TCanvas *Canvas_eff_BvsUDSGwtPU = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;b jets efficiency;udsg with PU jets efficiency",PLOTTITLE);
  Canvas_eff_BvsUDSGwtPU->DrawFrame(MINB,MINUDSG,MAXB,MAXUDSG,namestring);
  DrawTitle(MINB,MAXB,MAXUDSG*1.1);
#ifdef MAKEJetVarPlot
  TLegend *Legends_C_Efficiencies = new TLegend(0.2,0.5,0.43,0.68,"c jets");
  SetLegendStyle(Legends_C_Efficiencies);
  ( (TLegendEntry*) Legends_C_Efficiencies->GetListOfPrimitives()->First() )->SetTextAlign(22);//centering the legend header
  sprintf(namestring,"UDSGC_Eff_Etawt%s",Title);
  TCanvas *Canvas_UDSGC_Eff_Eta = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;#eta;non-b jets efficiency",PLOTTITLE);
  Canvas_UDSGC_Eff_Eta->DrawFrame(EtaBins[0],MINUDSG_JetVar,EtaBins[nEtaBins],MAXUDSG_JetVar,namestring);
  DrawTitle(EtaBins[0],EtaBins[nEtaBins],MAXUDSG_JetVar*1.1);

  sprintf(namestring,"UDSGC_Eff_Ptwt%s",Title);
  TCanvas *Canvas_UDSGC_Eff_Pt = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;p_{T} (GeV/c);non-b jets efficiency",PLOTTITLE);
  Canvas_UDSGC_Eff_Pt->DrawFrame(PtBins[0],MINUDSG_JetVar,PtBins[nPtBins],MAXUDSG_JetVar,namestring);
  DrawTitle(PtBins[0],PtBins[nPtBins],MAXUDSG_JetVar*1.1);
  sprintf(namestring,"BC_Eff_Etawt%s",Title);
  TCanvas *Canvas_BC_Eff_Eta = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;#eta;bc jets efficiency",PLOTTITLE);
  Canvas_BC_Eff_Eta->DrawFrame(EtaBins[0],MINBC_JetVar,EtaBins[nEtaBins],MAXBC_JetVar,namestring);
  DrawTitle(EtaBins[0],EtaBins[nEtaBins],MAXBC_JetVar*1.01);
  sprintf(namestring,"BC_Eff_Ptwt%s",Title);
  TCanvas *Canvas_BC_Eff_Pt = new TCanvas(namestring,namestring,500,500);
  sprintf(namestring,"%s;p_{T} (GeV/c);bc jets efficiency",PLOTTITLE);
  Canvas_BC_Eff_Pt->DrawFrame(PtBins[0],MINBC_JetVar,PtBins[nPtBins],MAXBC_JetVar,namestring);
  DrawTitle(PtBins[0],PtBins[nPtBins],MAXBC_JetVar*1.01);
#endif

  for (Byte_t PU_iBin=0; PU_iBin<PU_nBins; PU_iBin++) {
    printf("####################### PU = %d to %d ##########################\n",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    EColor color=ColorList[PU_iBin%numColor];
    sprintf(namestring,"PU%dto%d:%s_PtvsDis",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Title);
    DistributionPlots2D *VsJetPt_plots_iPUBin = new DistributionPlots2D(namestring,Discriminator_nBins,Discriminator_Bins,nPtBins,PtBins);
    VsJetPt_plots_iPUBin->SetAxisTitles(Title,"p_{T} (GeV/c)","#");
    sprintf(namestring,"PU%dto%d:%s_EtavsDis",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],Title);
    DistributionPlots2D *VsJetEta_plots_iPUBin = new DistributionPlots2D(namestring,Discriminator_nBins,Discriminator_Bins,nEtaBins,EtaBins);
    VsJetEta_plots_iPUBin->SetAxisTitles(Title,"#eta","#");
    for (Byte_t PU=PU_Bin_LowEdge[PU_iBin]; PU<=PU_Bin_HighEdge[PU_iBin]; PU++) {
	VsJetPt_plots_iPUBin->Add(VsJetPt_plots[PU],puweight[PU]);
#ifdef DEBUG
	printf("Adding %d PU, the weight is %.5f\n", PU,puweight[PU]);
	VsJetPt_plots[PU]->DumpInfo();
#endif
	VsJetEta_plots_iPUBin->Add(VsJetEta_plots[PU],puweight[PU]);
      }
    VsJetPt_plots_iPUBin->Write();
    VsJetEta_plots_iPUBin->Write();
    UInt_t NReturnPoints;
    printf("Calculating B eff:\n");
    VsJetPt_plots_iPUBin->GetEff(Ref_eff_X,Ref_eff_XdownError,Ref_eff_XupError,B,PtBinCut);
    printf("Calculating C eff:\n");
    Canvas_eff_BvsC->cd();
    VsJetPt_plots_iPUBin->GetEff(eff_Y,eff_YupError,eff_YdownError,C,PtBinCut);
#ifdef SHOWDISCRIMINATORVALUESONPLOT
    if (PU_iBin==PU_nBins-1) DrawDiscirminatorValue(Discriminator_nBins,Ref_eff_X,eff_Y,Discriminator_Bins);
#endif
    NReturnPoints=SelectPoints(Discriminator_nBins,Ref_eff_X,Ref_eff_XdownError,Ref_eff_XupError,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError,(MAXB-MINB)/NEFFPOINTS,(MAXC-MINC)/NEFFPOINTS,false);
    //printf("Kept %d points on the plot.\n",NReturnPoints);
    TGraphAsymmErrors *PU_efficiencies_BvsC=new TGraphAsymmErrors(NReturnPoints,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError);
    sprintf(namestring,"%s_efficiencies_CvsB_PU%dto%d",Title,PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    SetPlotStyle(PU_efficiencies_BvsC,namestring,color);
    PU_efficiencies_BvsC->Draw("same,CP");
    PU_efficiencies_BvsC->Write();
    printf("Calculating UDSG eff:\n");
    Canvas_eff_BvsUDSG->cd();
    VsJetPt_plots_iPUBin->GetEff(eff_Y,eff_YupError,eff_YdownError,UDSG,PtBinCut);
#ifdef SHOWDISCRIMINATORVALUESONPLOT
    if (PU_iBin==PU_nBins-1) DrawDiscirminatorValue(Discriminator_nBins,Ref_eff_X,eff_Y,Discriminator_Bins);
#endif
    NReturnPoints=SelectPoints(Discriminator_nBins,Ref_eff_X,Ref_eff_XdownError,Ref_eff_XupError,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError,(MAXB-MINB)/NEFFPOINTS,log10(MAXUDSG/MINUDSG)/NEFFPOINTS,true);
    printf("Kept %d points on the plot.\n",NReturnPoints);
    TGraphAsymmErrors *PU_efficiencies_BvsUDSG=new TGraphAsymmErrors(NReturnPoints,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError);
    sprintf(namestring,"%s_efficiencies_UDSGvsB_PU%dto%d",Title,PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    SetPlotStyle(PU_efficiencies_BvsUDSG,namestring,color);
    PU_efficiencies_BvsUDSG->Draw("same,CP");
    PU_efficiencies_BvsUDSG->Write();
    printf("Calculating UDSGwtNoFlavor eff:\n");
    Canvas_eff_BvsUDSGwtPU->cd();
    VsJetPt_plots_iPUBin->GetEff(eff_Y,eff_YupError,eff_YdownError,UDSGwtNoFlavor,PtBinCut);
#ifdef SHOWDISCRIMINATORVALUESONPLOT
    if (PU_iBin==PU_nBins-1) DrawDiscirminatorValue(Discriminator_nBins,Ref_eff_X,eff_Y,Discriminator_Bins);
#endif
    NReturnPoints=SelectPoints(Discriminator_nBins,Ref_eff_X,Ref_eff_XdownError,Ref_eff_XupError,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError,(MAXB-MINB)/NEFFPOINTS,log10(MAXUDSG/MINUDSG)/NEFFPOINTS,true);
    printf("Kept %d points on the plot.\n",NReturnPoints);
    TGraphAsymmErrors *PU_efficiencies_BvsUDSGwtPU=new TGraphAsymmErrors(NReturnPoints,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError);
    sprintf(namestring,"%s_efficiencies_UDSGwtPUvsB_PU%dto%d",Title,PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    SetPlotStyle(PU_efficiencies_BvsUDSGwtPU,namestring,color);
    PU_efficiencies_BvsUDSGwtPU->Draw("same,CP");
    PU_efficiencies_BvsUDSGwtPU->Write();
    if ( PU_Bin_LowEdge[PU_iBin]!=PU_Bin_HighEdge[PU_iBin] )
      sprintf(legendtext,"PU = %d to %d",PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin]);
    else sprintf(legendtext,"PU = %d",PU_Bin_LowEdge[PU_iBin]);
    Legends_Efficiencies->AddEntry(PU_efficiencies_BvsUDSG,legendtext,"LEP");
#ifdef MAKEJetVarPlot
    const char *JetVarNames[]={"Eta","Pt"};
    DistributionPlots2D* VsJetVar_plots_iPUBin[]={VsJetEta_plots_iPUBin,VsJetPt_plots_iPUBin};
    const Byte_t nJetVars=sizeof(VsJetVar_plots_iPUBin)/sizeof(DistributionPlots2D*);
   //----------------------------UDSGC vs eta/pt at fixed B---------------------------------------------------
    TCanvas *Canvases[]={Canvas_UDSGC_Eff_Eta,Canvas_UDSGC_Eff_Pt};
    for (Byte_t iJetVar=0;iJetVar<nJetVars;iJetVar++) {
      printf("Calculating UDSGC eff vs %s at fixed %.3f-/+%.3f Beff:\n",JetVarNames[iJetVar],RequiredBEff,Required_BEffAccuracy);
      Flavor PlottingFlavors[]={UDSG,C};
      Bool_t IsLogY[]={true,true};
      EColor colors[]={color,ColorList[(PU_iBin+PU_nBins)%numColor]};
      const Byte_t nFlavors=sizeof(PlottingFlavors)/sizeof(Flavor);
      TGraphAsymmErrors *UDSGC_Eff[nFlavors],*UDSGC_Eff_fittingError[nFlavors];
      VsJetVar_plots_iPUBin[iJetVar]->GetJetVarPlot(UDSGC_Eff,UDSGC_Eff_fittingError,nFlavors,PlottingFlavors,B,RequiredBEff,Required_BEffAccuracy,IsLogY,false);
      Canvases[iJetVar]->cd();
      for (Byte_t iFlavor=0;iFlavor<nFlavors;iFlavor++) {
	sprintf(namestring,"%s_%s_efficiencies_%s_PU%dto%d_RequireBEff=%.2f",Title,Flavor_Names[Int_t(PlottingFlavors[iFlavor])],JetVarNames[iJetVar],PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],RequiredBEff);
	SetPlotStyle(UDSGC_Eff_fittingError[iFlavor],namestring,colors[iFlavor]);
	UDSGC_Eff_fittingError[iFlavor]->Draw("same,P");
#ifdef SHOWSECONDERRORBAR
	sprintf(namestring,"%s_NoProprogatedError",namestring);
	SetPlotStyle(UDSGC_Eff[iFlavor],namestring,EColor(colors[iFlavor]-1));
	UDSGC_Eff[iFlavor]->Draw("same,P");
	UDSGC_Eff[iFlavor]->Write();
#endif
	UDSGC_Eff_fittingError[iFlavor]->Write();
      }
    }
    //----------------------------BC vs eta/pt at fixed UDSG---------------------------------------------------
    Canvases[0]=Canvas_BC_Eff_Eta;Canvases[1]=Canvas_BC_Eff_Pt;
    for (Byte_t iJetVar=0;iJetVar<nJetVars;iJetVar++) {
      printf("Calculating BC eff vs %s at fixed %.3f-%.3f/+%.3f UDSGeff:\n",JetVarNames[iJetVar],RequiredUDSGEff_ETA,RequiredUDSGEff_ETA-exp10(log10(RequiredUDSGEff_ETA)-Required_UDSGEffAccuracy),exp10(log10(RequiredUDSGEff_ETA)+Required_UDSGEffAccuracy)-RequiredUDSGEff_ETA);
      Flavor PlottingFlavors[]={B,C};
      Bool_t IsLogY[]={false,false};
      EColor colors[]={color,ColorList[(PU_iBin+PU_nBins)%numColor]};
      Double_t RequiredUDSGEff[]={RequiredUDSGEff_ETA,RequiredUDSGEff_PT};
      const Byte_t nFlavors=sizeof(PlottingFlavors)/sizeof(Flavor);
      TGraphAsymmErrors *BC_Eff[nFlavors],*BC_Eff_fittingError[nFlavors];
      VsJetVar_plots_iPUBin[iJetVar]->GetJetVarPlot(BC_Eff,BC_Eff_fittingError,nFlavors,PlottingFlavors,UDSG,RequiredUDSGEff[iJetVar],Required_UDSGEffAccuracy,IsLogY,true);
      Canvases[iJetVar]->cd();
      for (Byte_t iFlavor=0;iFlavor<nFlavors;iFlavor++) {
	sprintf(namestring,"%s_%s_efficiencies_%s_PU%dto%d_Required_UDSGEff_ETA=%.2f",Title,Flavor_Names[Int_t(PlottingFlavors[iFlavor])],JetVarNames[iJetVar],PU_Bin_LowEdge[PU_iBin],PU_Bin_HighEdge[PU_iBin],RequiredUDSGEff_ETA);
	SetPlotStyle(BC_Eff_fittingError[iFlavor],namestring,colors[iFlavor]);
	BC_Eff_fittingError[iFlavor]->Draw("same,P");
#ifdef SHOWSECONDERRORBAR
	sprintf(namestring,"%s_NoProprogatedError",namestring);
	SetPlotStyle(BC_Eff[iFlavor],namestring,EColor(colors[iFlavor]-1));
	BC_Eff[iFlavor]->Draw("same,P");
	BC_Eff[iFlavor]->Write();
#endif
	BC_Eff_fittingError[iFlavor]->Write();
      }
      if (!iJetVar) Legends_C_Efficiencies->AddEntry(BC_Eff_fittingError[1],legendtext,"LEP");
    }
#endif//end of #ifdef MAKEJetVarPlot
  }

  //------------------------draw the total eff/mistag line---------------------
  sprintf(result_filename,"%s_UDSGeffvsBeff.pdf",Title);
  Canvas_eff_BvsUDSG->cd();
  Legends_Efficiencies->Draw();
  gPad->SetFixedAspectRatio();
  gPad->SetGrid();
  gPad->SetLogy();
#ifdef SavePDFFormat
  Canvas_eff_BvsUDSG->SaveAs(result_filename);
#endif
  Canvas_eff_BvsUDSG->Write();
  Canvas_eff_BvsC->cd();
  gPad->SetGrid();
  Legends_Efficiencies->Draw();
#ifdef SavePDFFormat
  sprintf(result_filename,"%s_CeffvsBeff.pdf",Title);
  Canvas_eff_BvsC->SaveAs(result_filename);
  Canvas_eff_BvsC->Write();
#endif
  Canvas_eff_BvsUDSGwtPU->cd();
  Legends_Efficiencies->Draw();
  gPad->SetFixedAspectRatio();
  gPad->SetGrid();
  gPad->SetLogy();
#ifdef SavePDFFormat
  sprintf(result_filename,"%s_UDSGwtNoFlavoreffvsBeff.pdf",Title);
  Canvas_eff_BvsUDSGwtPU->SaveAs(result_filename);
#endif
  Canvas_eff_BvsUDSGwtPU->Write();

#ifdef MAKEJetVarPlot
  Canvas_UDSGC_Eff_Eta->cd();
  Legends_Efficiencies->SetHeader("udsg jets");
  ( (TLegendEntry*) Legends_Efficiencies->GetListOfPrimitives()->First() )->SetTextAlign(22);//centering the legend header
  Legends_Efficiencies->SetX1NDC(0.65);
  Legends_Efficiencies->SetX2NDC(0.88);
  Legends_Efficiencies->SetY1NDC(0.2);
  Legends_Efficiencies->SetY2NDC(0.43);
  Legends_Efficiencies->Draw();
  Legends_C_Efficiencies->Draw();
  gPad->SetFixedAspectRatio();
  gPad->SetGrid();
  gPad->SetLogy();
#ifdef SavePDFFormat
  sprintf(result_filename,"%s_UDSGCeffvsJetEta_FixBeff%dpercent.pdf",Title,Int_t(RequiredBEff*100));
  Canvas_UDSGC_Eff_Eta->SaveAs(result_filename);
#endif
  Canvas_UDSGC_Eff_Eta->Write();
  
  Canvas_UDSGC_Eff_Pt->cd();
  Legends_Efficiencies->Draw();
  Legends_C_Efficiencies->Draw();
  gPad->SetFixedAspectRatio();
  gPad->SetGrid();
  gPad->SetLogy();
#ifdef SavePDFFormat
  sprintf(result_filename,"%s_UDSGCeffvsJetPt_FixBeff%dpercent.pdf",Title,Int_t(RequiredBEff*100));
  Canvas_UDSGC_Eff_Pt->SaveAs(result_filename);
#endif
  Canvas_UDSGC_Eff_Pt->Write();

  Canvas_BC_Eff_Eta->cd();
  Legends_Efficiencies->SetHeader("b jets");
  Legends_Efficiencies->SetY1NDC(0.34);
  Legends_Efficiencies->SetY2NDC(0.57);
  Legends_Efficiencies->Draw();
  Legends_C_Efficiencies->SetY1NDC(0.5);
  Legends_C_Efficiencies->SetY2NDC(0.73);
  Legends_C_Efficiencies->Draw();
  gPad->SetFixedAspectRatio();
  gPad->SetGrid();
#ifdef SavePDFFormat
  if (RequiredUDSGEff_ETA<0.00999) {
    char temp[100];
    sprintf(temp,"%.2f",RequiredUDSGEff_ETA*100);
    sprintf(result_filename,"%s_BCeffvsJetEta_FixUDSG0point%spercent.pdf",Title,temp+2);
  }
  else sprintf(result_filename,"%s_BCeffvsJetEta_FixUDSG%dpercent.pdf",Title,Int_t(0.5+RequiredUDSGEff_ETA*100));
  Canvas_BC_Eff_Eta->SaveAs(result_filename);
#endif
  Canvas_BC_Eff_Eta->Write();

  Canvas_BC_Eff_Pt->cd();
  Legends_Efficiencies->Draw();
  Legends_C_Efficiencies->Draw();
  gPad->SetFixedAspectRatio();
  gPad->SetGrid();
#ifdef SavePDFFormat
  if (RequiredUDSGEff_PT<0.00999) {
    char temp[100];
    sprintf(temp,"%.2f",RequiredUDSGEff_PT*100);
    sprintf(result_filename,"%s_BCeffvsJetPt_FixUDSG0point%spercent.pdf",Title,temp+2);
  }
  else sprintf(result_filename,"%s_BCeffvsJetPt_FixUDSG%dpercent.pdf",Title,Int_t(0.5+RequiredUDSGEff_PT*100));
  Canvas_BC_Eff_Pt->SaveAs(result_filename);
#endif
  Canvas_BC_Eff_Pt->Write();
#endif//end of #ifdef MAKEJetVarPlot
  return;
}

void Load(const char options[]="TCHP",Float_t RequiredBEff=-1., Float_t RequiredUDSGEff_ETA=0.01, Float_t RequiredUDSGEff_PT=0.05) {
  setTDRStyle();
//------------Prepare files---------------------
  Bool_t UseDefaultRequiredEff=false;
  if (RequiredBEff<0) {RequiredBEff=0.5;UseDefaultRequiredEff=true;}
  Plotting *MC[nSamples];
#if !defined(npu_probs)
  TH1D* npu_mc=new TH1D("pu_distribution","pu_distribution;# of PU; entries",MAXPU+1,-0.5,MAXPU+0.5);
#endif
  //Long64_t Tot_Evt=0;  Double_t Tot_Weight=0.;
  for (UInt_t iSample=0;iSample<nSamples;iSample++) {
    MC[iSample]=new Plotting(MC_files[iSample]);
#if !defined(npu_probs)
    MC[iSample]->PileUpDistribution(npu_mc);
#endif
  }
#if !defined(npu_probs)
  npu_mc->Scale(1/npu_mc->Integral());//normalize to ONE
#endif
//#########################Setup MC reweighting coefficient######################
  //Get PU distribution in MC
#ifndef npu_probs
  for(Byte_t npu=0; npu<MAXPU+1; ++npu) {
    Double_t numberofpu_mc=npu_mc->GetBinContent(npu_mc->FindBin(npu));
    if ( numberofpu_mc>0 ) puweight[npu]=numberofpu_mc;
    else puweight[npu]=0; //printf("%d:%f/%f=%f;\n",npu,npu_data->GetBinContent(npu_data->FindBin(npu)),numberofpu_mc,puweight[npu]);
  }
#else
  for(Byte_t npu=0; npu<MAXPU+1; ++npu)
    puweight[npu]=npu_probs[npu]>0?npu_probs[npu]:0.;
#endif
#ifdef pudistribution_data_filename //Get PU distribution in data
  TFile *filepuest = new TFile(pudistribution_data_filename,"READ");
  TH1D* npu_data= (TH1D*) filepuest->Get("pileup");
  npu_data->Scale(1/npu_data->Integral());//normalize to ONE
  npu_data->SetTitle("pu_distribution;# of PU; entries");
  for(Byte_t npu=0; npu<MAXPU+1; ++npu)
    puweight[npu]=npu_data->GetBinContent(npu_data->FindBin(npu))/puweight[npu];
  TCanvas *Canvas_npu=new TCanvas("npu","npu",500,500);
  Canvas_npu->cd();
  //npu_mc->Draw();
  //Canvas_npu->SaveAs("pudist_MC.pdf");
  npu_data->Draw();
  char pudis_data_filename[]=pudistribution_data_filename;
  strncpy (strstr(pudis_data_filename,".root"),".pdf\0",5);
  Canvas_npu->SaveAs(pudis_data_filename);
  Canvas_npu->Destructor();
  filepuest->Close();
#else//reweight to flat distribution, for PU<=10 cases, there should be no difference between this reweighting and the raw distribution. It becomes a pure MC calulation.
  for(Short_t npu=MAXPU; npu>=0; npu--)
    puweight[npu]=puweight[0]/puweight[npu];
#endif
  for(Byte_t npu=0; npu<=MAXPU; npu++)
    cerr<<Short_t(npu)<<"PUweight: "<<puweight[npu]<<"\t";
  cerr<<endl;
  char result_filename[200];
  sprintf(result_filename,"%s_%s.root",RESULT_FILENAME,options);
  TFile * result = new TFile(result_filename,"RECREATE");
  result->cd();
  //------------draw plots---------------------
  if ( strstr(options,"TC2DHE")!=NULL ) {
    const Float_t TrackCounting2d_SBins[]={0,0.3,0.5,0.7,1.1,1.5,2.,2.5,3,4,5,6,7,8,9,10,12,15,20,25};
    EfficienciesPlotting(MC,"TrackCounting2dHighEfficiency",TrackCounting2d_SBins,sizeof(TrackCounting2d_SBins)/sizeof(Float_t)-1,Plotting::TCHE2D,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TC3DHE")!=NULL ) {
    const Float_t TrackCounting3d_SBins[]={0,0.6,1.1,1.5,2.,2.5,3,4,5,6,7,8,9,10,12,15,20,30};
    EfficienciesPlotting(MC,"TrackCounting3dHighEfficiency",TrackCounting3d_SBins,sizeof(TrackCounting3d_SBins)/sizeof(Float_t)-1,Plotting::ReCalTCHE,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TC2DHP")!=NULL ) {
    const Float_t TrackCounting2d_SBins[]={0,0.3,0.5,0.7,1.1,1.5,2.,2.5,3,4,5,6,7,8,9,10,12,15,20,25};
    EfficienciesPlotting(MC,"TrackCounting2dHighPurity",TrackCounting2d_SBins,sizeof(TrackCounting2d_SBins)/sizeof(Float_t)-1,Plotting::TCHP2D,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TC3DHP")!=NULL ) {
    const Float_t TrackCounting3d_SBins[]={0,0.6,1.1,1.5,2.,2.5,3,4,5,6,7,8,9,10,12,15,20,30};
    EfficienciesPlotting(MC,"TrackCounting3dHighPurity",TrackCounting3d_SBins,sizeof(TrackCounting3d_SBins)/sizeof(Float_t)-1,Plotting::ReCalTCHE,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"JetP")!=NULL ) {
    const Float_t start=0,end=4.,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t JetProbability_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      JetProbability_Bins[i]=start+i*binwidth;
    SetAxisesRanges();
    EfficienciesPlotting(MC,"JetP",JetProbability_Bins,NDISCRIMINATORBINS,Plotting::JetProb,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"JetBP")!=NULL ) {
    const Float_t start=0,end=7.,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t JetBProbability_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      JetBProbability_Bins[i]=start+i*binwidth;
    SetAxisesRanges();
    EfficienciesPlotting(MC,"JetBP",JetBProbability_Bins,NDISCRIMINATORBINS,Plotting::JetBProb,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"CSVMVA")!=NULL ) {
    const Float_t start=0,end=1,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t CombinedSecondaryVertex_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      CombinedSecondaryVertex_Bins[i]=start+i*binwidth;
    SetAxisesRanges();
    EfficienciesPlotting(MC,"CSVMVA",CombinedSecondaryVertex_Bins,NDISCRIMINATORBINS,Plotting::CSVMVA,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"CSVPF")!=NULL ) {
    const Float_t start=0,end=1,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t CombinedSecondaryVertex_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      CombinedSecondaryVertex_Bins[i]=start+i*binwidth;
    SetAxisesRanges(1E-4,0.2,0.15,0.9,0.25,1);
    EfficienciesPlotting(MC,"CSV",CombinedSecondaryVertex_Bins,NDISCRIMINATORBINS,Plotting::CSVPF,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TCHE")!=NULL ) {
    const Float_t start=-10.,end=30,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t TrackCounting_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      TrackCounting_Bins[i]=start+i*binwidth;
    SetAxisesRanges();
    EfficienciesPlotting(MC,"TCHE",TrackCounting_Bins,NDISCRIMINATORBINS,Plotting::TCHE,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TCHP")!=NULL ) {
    const Float_t start=-10.,end=30,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t TrackCounting_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      TrackCounting_Bins[i]=start+i*binwidth;
    SetAxisesRanges(8E-4,0.5,0.1,0.8,0,1,1E-4);
    EfficienciesPlotting(MC,"TCHP",TrackCounting_Bins,NDISCRIMINATORBINS,Plotting::TCHP,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"SSVHE")!=NULL ) {
    const Float_t start=1.,end=5,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t SimpleSecondaryVertex_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      SimpleSecondaryVertex_Bins[i]=start+i*binwidth;
    if (UseDefaultRequiredEff) {RequiredBEff=0.5;RequiredUDSGEff_ETA=0.002;RequiredUDSGEff_PT=0.01;}//it has different default values
    SetAxisesRanges(1E-3,0.3,0,0.7,0,0.7,1E-4,3E-2,1E-4,0.2);
    EfficienciesPlotting(MC,"SSVHE",SimpleSecondaryVertex_Bins,NDISCRIMINATORBINS,Plotting::SSVHE,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"SSVHP")!=NULL ) {
    const Float_t start=1.,end=5,binwidth=(end-start)/Float_t(NDISCRIMINATORBINS);
    Float_t SimpleSecondaryVertex_Bins[NDISCRIMINATORBINS+1];
    for (UInt_t i=0; i<=NDISCRIMINATORBINS; i++)
      SimpleSecondaryVertex_Bins[i]=start+i*binwidth;
    if (UseDefaultRequiredEff) {RequiredBEff=0.3;RequiredUDSGEff_ETA=0.0002;RequiredUDSGEff_PT=0.001;}//it has different default values since the purity is too high
    SetAxisesRanges(5E-5,0.1,0,0.6,0.25,0.6,1E-4,5E-3,1E-4,0.2);
    EfficienciesPlotting(MC,"SSVHP",SimpleSecondaryVertex_Bins,NDISCRIMINATORBINS,Plotting::SSVHP,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TracktoJetDistHEStudy")!=NULL ) {
    const Float_t TracktoJetDist_Bins[]={0.02,0.03,0.04,0.05,0.06,0.07,0.08,0.11,0.15,0.19,0.23,0.3,0.9};
    EfficienciesPlotting(MC,"TracktoJetDistHighEfficiency",TracktoJetDist_Bins,sizeof(TracktoJetDist_Bins)/sizeof(Float_t)-1,Plotting::JetDistStudyHighEff,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"TracktoJetDistHPStudy")!=NULL ) {
    const Float_t TracktoJetDist_Bins[]={0.02,0.03,0.04,0.05,0.07,0.09,0.15,0.3,0.9};
    EfficienciesPlotting(MC,"TracktoJetDistHighPurity",TracktoJetDist_Bins,sizeof(TracktoJetDist_Bins)/sizeof(Float_t)-1,Plotting::JetDistStudyHighPur,RequiredBEff,RequiredUDSGEff_ETA,RequiredUDSGEff_PT);
  }
  if ( strstr(options,"MeanPt")!=NULL ) {
    MeanPtPlot(MC,options);
  }
#if bTagNtupleVersion>=4
  if ( strstr(options,"MCPt")!=NULL ) {
    MCPtvsRecoPt(MC,options);
  }
#endif
#if bTagNtupleVersion==4
  if ( strstr(options,"NPUTracks")!=NULL ) {
    const Byte_t MAXPUTrack=10, NPercents=25;
    Float_t NTracks[MAXPUTrack+2],PUs[MAXPU+2],Percents[NPercents+1];
    for (Byte_t i=0;i<=MAXPUTrack+1;i++)
      NTracks[i]=-0.5+i;
    for (Byte_t i=0;i<=MAXPU+1;i++)
      PUs[i]=-0.5+i;
    for (Byte_t i=0;i<=NPercents;i++)
      Percents[i]=i/Float_t(NPercents);
    char namestring[150];
    DistributionPlots2D *NJetPlots = new DistributionPlots2D("NJet_PUTracks;N PUTracks in Jet;N of PU",MAXPUTrack,NTracks,MAXPU,PUs),
      *RatioPlots = new DistributionPlots2D("NJet_PUTracksRatio;PUTracks Ratio in Jet;N of PU",NPercents,Percents,MAXPU,PUs),
      *NJetPlotswtDecayLengthAndJetAxisCut = new DistributionPlots2D("NJet_PUTrackswtDecayLengthAndJetAxisCut;N DecayLengthAndJetAxisPUTracks in Jet;N of PU",MAXPUTrack,NTracks,MAXPU,PUs),
      *RatioPlotswtDecayLengthAndJetAxisCut = new DistributionPlots2D("NJet_PUTracksRatiowtDecayLengthAndJetAxisCut;DecayLengthAndJetAxisPUTracks Ratio in Jet;N of PU",NPercents,Percents,MAXPU,PUs);
    //-------------Fill TH2D from samples----------------------
    for (UInt_t iSample=0;iSample<nSamples;iSample++) {
      sprintf(namestring,"NJetPlots_Sample%d",iSample);
      DistributionPlots2D *thisNJetPlot = new DistributionPlots2D(namestring,MAXPUTrack,NTracks,MAXPU,PUs);
      sprintf(namestring,"RatioPlots_Sample%d",iSample);
      DistributionPlots2D *thisRatioPlot = new DistributionPlots2D(namestring,NPercents,Percents,MAXPU,PUs);
      sprintf(namestring,"NJetPlotswtDecayLengthAndJetAxisCut_Sample%d",iSample);
      DistributionPlots2D *thisNJetPlotwtDecayLengthAndJetAxisCut = new DistributionPlots2D(namestring,MAXPUTrack,NTracks,MAXPU,PUs);
      sprintf(namestring,"RatioPlotswtDecayLengthAndJetAxisCut_Sample%d",iSample);
      DistributionPlots2D *thisRatioPlotwtDecayLengthAndJetAxisCut = new DistributionPlots2D(namestring,NPercents,Percents,MAXPU,PUs);
      MC[iSample]->NPUTracks(thisNJetPlot,thisRatioPlot,NJetPlotswtDecayLengthAndJetAxisCut,RatioPlotswtDecayLengthAndJetAxisCut);
      NJetPlots->Add(thisNJetPlot,MC_Weights[iSample]);
      RatioPlots->Add(thisRatioPlot,MC_Weights[iSample]);
      NJetPlotswtDecayLengthAndJetAxisCut->Add(thisNJetPlotwtDecayLengthAndJetAxisCut,MC_Weights[iSample]);
      RatioPlotswtDecayLengthAndJetAxisCut->Add(thisRatioPlotwtDecayLengthAndJetAxisCut,MC_Weights[iSample]);
    }
    NJetPlots -> DrawAndWrite(options);
    RatioPlots-> DrawAndWrite(options);
    NJetPlotswtDecayLengthAndJetAxisCut-> DrawAndWrite(options);
    RatioPlotswtDecayLengthAndJetAxisCut->DrawAndWrite(options);
  }
#endif
  result->Close();
  return;
}
//##############################################
//## Distirbution to Efficiency Core Function ##
//##############################################
Double_t DistributionPlots2D::GetEff(Float_t *Eff, Float_t *EffupError, Float_t *EffdownError, Flavor WhichFlavor, Int_t firstJetVarBin,Int_t lastJetVarBin) {
  Int_t whichone=Int_t(WhichFlavor);
  if (fplots[whichone]==NULL) return 0;
  char namestring[20];
  sprintf(namestring,"Bin%dto%d",firstJetVarBin,lastJetVarBin);
  TH1D *ProjX=fplots[whichone]->ProjectionX(namestring,firstJetVarBin,lastJetVarBin);//including all the overflow bins
  Int_t nbins=ProjX->GetNbinsX();
  //an easy option is to use TH1D::GetIntegral(); To speed up, I programmed it by myself
  Double_t tot=0,passed=0,tot_variance=0.;
  for (Int_t ibin=0;ibin<=nbins+1;ibin++) {
    tot+=ProjX->GetBinContent(ibin);
    tot_variance+=ProjX->GetBinError(ibin)*ProjX->GetBinError(ibin);
  }
  if (tot<=0.) {
    for (Int_t ibin=0;ibin<=nbins;ibin++) {
      Eff[ibin]=1E-5;
      EffupError[ibin]=0.;
      EffdownError[ibin]=1E-5;
    }
    return -1.;
  }
  Double_t tot_eq=tot*tot/tot_variance;//Total equivalent number of events is to calulate the error bars. It is just an approximation since efficiency with weighted histograms is an un-solved problem in statistic.
  if (lastJetVarBin==-1) cerr<<"Total equivalent number of events="<<tot<<"^2/"<<tot_variance<<"="<<Long64_t(tot_eq)<<endl;
  passed=tot;
  for (Int_t ibin=0;ibin<=nbins;ibin++) {
    passed-=ProjX->GetBinContent(ibin);//throw away the previous bin so the rest is the number of passed jets
    if (passed<0.) passed=0.;//it is possible less than 0 since the double calculate is not 100% accurate
    Eff[ibin]=passed/tot;
    Double_t passed_eq=Eff[ibin]*tot_eq;
    //cerr<<"eff:"<<passed<<"/"<<tot<<"="<<Eff[ibin]<<"; for error bar: "<<passed_eq<<"/"<<tot_eq<<"="<<passed_eq/tot_eq<<endl;
    if (tot_eq<2147483647.&&tot_eq>1.) {//less than the MAX of Int_t
      EffupError[ibin]=TEfficiency::ClopperPearson(Int_t(tot_eq),Int_t(passed_eq),0.683,true)-Eff[ibin];
      EffdownError[ibin]=Eff[ibin]-TEfficiency::ClopperPearson(Int_t(tot_eq),Int_t(passed_eq),0.683,false);
    }
    else {//using binomial, it's an approximation but under such large number it make no difference. Strictly saying, binomial is safe if np>5 and n(1-p)>5. See http://en.wikipedia.org/wiki/Binomial_proportion_confidence_interval
      if (tot_eq<=1.) {//if less than one effective entry, set the error bar to largest
	EffupError[ibin]=1.;
	EffdownError[ibin]=1.;
      }
      else {
	EffupError[ibin]=sqrt(Eff[ibin]*(1-Eff[ibin])/tot_eq);
	EffdownError[ibin]=EffupError[ibin];
      }
    }
    CorrectResult(&Eff[ibin],&EffupError[ibin],&EffdownError[ibin]);
  }
  return tot;
}

//############################################
//## Efficiency vs JetVarPlot Core Function ##
//############################################
void DistributionPlots2D::GetJetVarPlot(TGraphAsymmErrors *FlavorEffVsJetVarPlots[], TGraphAsymmErrors *FlavorEffVsJetVarPlotswtFittingBias[], const Byte_t nPlotFlavors, Flavor *PlotFlavor, Flavor RequiredEff_Flavor, Float_t RequiredEff, Float_t RequiredEffAccuracy, Bool_t *IsLogY, Bool_t IsLogX) {
  const Int_t Discriminator_nBins=fplots[0]->GetNbinsX(),nJetVarBins=fplots[0]->GetNbinsY();
  if (IsLogX) RequiredEff=log10(RequiredEff);
  Float_t eff_X[Discriminator_nBins+1],eff_XupError[Discriminator_nBins+1],eff_XdownError[Discriminator_nBins+1],
    eff_Y[Discriminator_nBins+1],eff_YupError[Discriminator_nBins+1],eff_YdownError[Discriminator_nBins+1];
  Float_t result[nPlotFlavors][nJetVarBins],result_upError[nPlotFlavors][nJetVarBins],result_downError[nPlotFlavors][nJetVarBins],JetVar[nPlotFlavors][nJetVarBins],xError[nPlotFlavors][nJetVarBins],fitting_upError[nPlotFlavors][nJetVarBins],fitting_downError[nPlotFlavors][nJetVarBins];
  Int_t iNonemptyJetVarBins[nPlotFlavors];//it counts the total non-empty bins of each Y lines
  for (Int_t iPlotFlavor=0;iPlotFlavor<nPlotFlavors;iPlotFlavor++)
    iNonemptyJetVarBins[iPlotFlavor]=0;
  for (Int_t iJetVarBin=1; iJetVarBin<=nJetVarBins; iJetVarBin++ ) {
#ifdef DEBUG
    printf("JetVar(%.2f), Require %s eff=%.5f+/-%.5f",fplots[0]->GetYaxis()->GetBinCenter(iJetVarBin),Flavor_Names[Int_t(RequiredEff_Flavor)],RequiredEff,RequiredEffAccuracy);
#endif
    Double_t totX=GetEff(eff_X,eff_XupError,eff_XdownError,RequiredEff_Flavor,iJetVarBin,iJetVarBin);
    if (totX>0) {
      if (IsLogX) 
	for (Int_t iCut=0; iCut<=Discriminator_nBins; iCut++) {
	  eff_XupError[iCut]=log10(1+eff_XupError[iCut]/eff_X[iCut]);
	  if (eff_XdownError[iCut]<eff_X[iCut]) eff_XdownError[iCut]=-log10(1-eff_XdownError[iCut]/eff_X[iCut]);
	  else eff_XdownError[iCut]=log10(eff_X[iCut]);
	  eff_X[iCut]=log10(eff_X[iCut]);
	}
      //similar with DQMOffline/RecoB/src/BTagDifferentialPlot.cc, instead of filling a TH1, find it directly on the TGraph
      if (eff_X[0]<RequiredEff||eff_X[Discriminator_nBins]>RequiredEff) {
	printf("\n-- the required %s eff (%.3f) is away from the plot (%.3f~%.3f). Fitting does not make sense. Skip this JetVar bin (%.2f).\n",Flavor_Names[Int_t(RequiredEff_Flavor)],RequiredEff,eff_X[Discriminator_nBins],eff_X[0],fplots[0]->GetYaxis()->GetBinCenter(iJetVarBin));
	char namestring[255];
	sprintf(namestring,"JetVar(%.2f): !!require %s Eff=%.5f+/-%.5f in %.3f~%.3f!! This JetVar bin is skipped.",fplots[0]->GetYaxis()->GetBinCenter(iJetVarBin),Flavor_Names[Int_t(RequiredEff_Flavor)],RequiredEff,RequiredEffAccuracy,eff_X[Discriminator_nBins],eff_X[0]);
	TNamed *t=new TNamed(namestring,namestring);
	t->Write();
	continue;
      }
      for (Int_t Discriminator_iCut=1; Discriminator_iCut<=Discriminator_nBins; Discriminator_iCut++)
	if (eff_X[Discriminator_iCut]<RequiredEff) {
	  UInt_t firstpt=Discriminator_iCut,lastpt=firstpt-1;
	  Float_t discrepancy=fabs(eff_X[lastpt]-RequiredEff);
	  if (fabs(RequiredEff-eff_X[firstpt])<discrepancy) discrepancy=fabs(RequiredEff-eff_X[firstpt]);
	  //Discriminator_iCut is the closest point to required eff
	  else Discriminator_iCut=lastpt;
#ifdef DEBUG
	  printf(" in %.5f~%.5f: \n",eff_X[firstpt],eff_X[lastpt]);
#endif
	  for (Byte_t iPlotFlavor=0;iPlotFlavor<nPlotFlavors;iPlotFlavor++) {
	    Double_t totY=GetEff(eff_Y,eff_YupError,eff_YdownError,PlotFlavor[iPlotFlavor],iJetVarBin,iJetVarBin);
	    if (totY>0) {
 	      //printf("-- Plotting %s eff:\n",Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])]);
	      Int_t iNonemptyJetVarBin=iNonemptyJetVarBins[iPlotFlavor];
	      JetVar[iPlotFlavor][iNonemptyJetVarBin]=fplots[0]->GetYaxis()->GetBinCenter(iJetVarBin);
	      xError[iPlotFlavor][iNonemptyJetVarBin]=fplots[0]->GetYaxis()->GetBinWidth(iJetVarBin)/2.;
	      result[iPlotFlavor][iNonemptyJetVarBin]=eff_Y[Discriminator_iCut];
	      result_upError[iPlotFlavor][iNonemptyJetVarBin]=eff_YupError[Discriminator_iCut];
	      result_downError[iPlotFlavor][iNonemptyJetVarBin]=eff_YdownError[Discriminator_iCut];
#ifdef DEBUG
	      printf("---- first : %s eff=%.5f+%.5f-%.5f\n",Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])],eff_Y[Discriminator_iCut],eff_YupError[Discriminator_iCut],eff_YdownError[Discriminator_iCut]);
#endif
	      char namestring[255];
	      sprintf(namestring,"JetVar(%.2f), require %s Eff=%.5f+/-%.5f in %.3f~%.3f, find %s Eff in %.3f~%.3f",JetVar[iPlotFlavor][iNonemptyJetVarBin],Flavor_Names[Int_t(RequiredEff_Flavor)],RequiredEff,RequiredEffAccuracy,eff_X[firstpt],eff_X[lastpt],Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])],eff_Y[firstpt],eff_Y[lastpt]);
#ifdef FITOPTION
	      TCanvas *temp=new TCanvas(namestring,"fitting",500,500);
#endif
	      Short_t fitStatus=5;TF1 *myfunc = NULL;Bool_t IsYLogged=false;
	      if (discrepancy>RequiredEffAccuracy&&eff_X[lastpt]!=eff_X[firstpt]) {
		if (IsLogY[iPlotFlavor]) {
		  for (Int_t iCut=0; iCut<=Discriminator_nBins; iCut++) {
		    eff_YupError[iCut]=log10(1+eff_YupError[iCut]/eff_Y[iCut]);
		    if (eff_YdownError[iCut]<eff_Y[iCut]) eff_YdownError[iCut]=-log10(1-eff_YdownError[iCut]/eff_Y[iCut]);
		    else eff_YdownError[iCut]=-log10(eff_Y[iCut]);
		    eff_Y[iCut]=log10(eff_Y[iCut]);
		  }
		  IsYLogged=true;
		}
#ifdef FITOPTION
		/*//fit part of the data
		  const Float_t TotalXRange=eff_X[0]-eff_X[Discriminator_nBins];
		  while (eff_X[lastpt]-eff_X[firstpt]<0.6*TotalXRange) {
		  if (lastpt>0) lastpt--;
		  if (eff_X[lastpt]-eff_X[firstpt]<0.6*TotalXRange&&firstpt<Discriminator_nBins) firstpt++;
		  }*/
		//TGraphAsymmErrors *PlotFlavorvsRequiredEffFlavor=new TGraphAsymmErrors(firstpt-lastpt+1,eff_X+lastpt,eff_Y+lastpt,eff_XdownError+lastpt,eff_XupError+lastpt,eff_YdownError+lastpt,eff_YupError+lastpt);//fit part of the data
		TGraphAsymmErrors *PlotFlavorvsRequiredEffFlavor=new TGraphAsymmErrors(Discriminator_nBins+1,eff_X,eff_Y,eff_XdownError,eff_XupError,eff_YdownError,eff_YupError);
		try {
		  fitStatus = PlotFlavorvsRequiredEffFlavor->Fit(FITOPTION,"q");
		  //fitStatus = PlotFlavorvsRequiredEffFlavor->Fit(FITOPTION,"q","same,P",eff_X[firstpt],eff_Y[lastpt]);//fit part of the data
		}catch (...){
		  cerr<<"---- Fit CRASHES! ";
		  fitStatus=5;
		}
		if (fitStatus == 0) {
		  gPad->SetGrid();
		  myfunc = PlotFlavorvsRequiredEffFlavor->GetFunction(FITOPTION);  //myfunc->Print();
		  result[iPlotFlavor][iNonemptyJetVarBin]=myfunc->Eval(RequiredEff);
		  if (IsYLogged) result[iPlotFlavor][iNonemptyJetVarBin]=exp10(result[iPlotFlavor][iNonemptyJetVarBin]);
		}
		else printf("---- Failed to fit hisogram %s,\nperhaps because of too few entries.(tot%s=%.3f,tot%s=%.3f). Going to use linear approximation.\n",temp->GetTitle(),Flavor_Names[Int_t(RequiredEff_Flavor)],totX,Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])],totY);
		PlotFlavorvsRequiredEffFlavor->SetName(namestring);
		sprintf(namestring,"%s;%s eff;%s eff",namestring,Flavor_Names[Int_t(RequiredEff_Flavor)],Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])]);
		PlotFlavorvsRequiredEffFlavor->SetTitle(namestring);
		temp->cd();
		PlotFlavorvsRequiredEffFlavor->Draw("AP");
#endif
		if (fitStatus!=0 ) {
		  result[iPlotFlavor][iNonemptyJetVarBin]=(eff_Y[lastpt]-eff_Y[firstpt])/(eff_X[lastpt]-eff_X[firstpt])*(RequiredEff-eff_X[firstpt])+eff_Y[firstpt];
		  if (IsYLogged) result[iPlotFlavor][iNonemptyJetVarBin]=exp10(result[iPlotFlavor][iNonemptyJetVarBin]);
		}
		//correct error bars into range 0~1
		CorrectResult(&result[iPlotFlavor][iNonemptyJetVarBin],&result_upError[iPlotFlavor][iNonemptyJetVarBin],&result_downError[iPlotFlavor][iNonemptyJetVarBin]);
	      } //#end of if (discrepancy>RequiredEffAccuracy)
	      //the propagated error from X axis
	      Float_t RequiredEffEdges[2]={RequiredEff+eff_XupError[Discriminator_iCut],RequiredEff-eff_XdownError[Discriminator_iCut]};
	      Float_t YLimit[2],YLimit_Error[2];
	      for (Int_t UpOrDown=0;UpOrDown<2;UpOrDown++) {
		Int_t Edge=firstpt;
		for (;Edge>=0&&Edge<=Discriminator_nBins; UpOrDown==0?Edge--:Edge++) {
		  if (Edge==0||Edge==Discriminator_nBins) {
		    YLimit[UpOrDown]=eff_Y[Edge];
		    YLimit_Error[UpOrDown]=UpOrDown==0?eff_YupError[Edge]:eff_YdownError[Edge];
		    break;
		  }
		  if ( (UpOrDown==0&&eff_X[Edge-1]>RequiredEffEdges[0])||
		       (UpOrDown==1&&eff_X[Edge]<RequiredEffEdges[1]) )
		    {
		      UInt_t edge_firstpt=Edge,edge_lastpt=edge_firstpt-1;
		      Float_t edge_discrepancy=fabs(RequiredEffEdges[UpOrDown]-eff_X[edge_lastpt]);
		      if (fabs(eff_X[edge_firstpt]-RequiredEffEdges[UpOrDown])<edge_discrepancy) edge_discrepancy=fabs(eff_X[edge_firstpt]-RequiredEffEdges[UpOrDown]);//Edge is the closest point to required eff
		      else Edge=edge_lastpt;
		      YLimit[UpOrDown]=eff_Y[Edge];
		      YLimit_Error[UpOrDown]=UpOrDown==0?eff_YupError[Edge]:eff_YdownError[Edge];
		      if (edge_discrepancy>RequiredEffAccuracy&&eff_X[edge_lastpt]!=eff_X[edge_firstpt]) {
			if (fitStatus == 0&&myfunc!=NULL) YLimit[UpOrDown]=myfunc->Eval(RequiredEffEdges[UpOrDown]);
			else YLimit[UpOrDown]=(eff_Y[edge_lastpt]-eff_Y[edge_firstpt])/(eff_X[edge_lastpt]-eff_X[edge_firstpt])*(RequiredEffEdges[UpOrDown]-eff_X[edge_firstpt])+eff_Y[edge_firstpt];
		      }
		      break;
		    }//end of  if required eff searched
		}//end of Edge loop
		if (IsYLogged) {
		  YLimit[UpOrDown]=exp10(YLimit[UpOrDown]);
		  YLimit_Error[UpOrDown]=UpOrDown==0?exp10(eff_YupError[Edge]+eff_Y[Edge])-YLimit[UpOrDown]:YLimit[UpOrDown]-exp10(eff_Y[Edge]-eff_YdownError[Edge]);
		}
	      }//end of UpOrDown loop
	      fitting_upError[iPlotFlavor][iNonemptyJetVarBin]=YLimit_Error[0]+YLimit[0]-result[iPlotFlavor][iNonemptyJetVarBin];
	      fitting_downError[iPlotFlavor][iNonemptyJetVarBin]=YLimit_Error[1]+result[iPlotFlavor][iNonemptyJetVarBin]-YLimit[1];
	      CorrectResult(&result[iPlotFlavor][iNonemptyJetVarBin],&fitting_upError[iPlotFlavor][iNonemptyJetVarBin],&fitting_downError[iPlotFlavor][iNonemptyJetVarBin]);
	      sprintf(namestring,"%s -- result: %s eff=%.5f+%.5f-%.5f",namestring,Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])],result[iPlotFlavor][iNonemptyJetVarBin],fitting_upError[iPlotFlavor][iNonemptyJetVarBin],fitting_downError[iPlotFlavor][iNonemptyJetVarBins[iPlotFlavor]]);
#ifdef FITOPTION
	      if (discrepancy>RequiredEffAccuracy) {
		temp->Write(namestring);
		temp->Destructor();
	      }
	      else {
		temp->Destructor();
		TObject temp2;
		temp2.Write(namestring);
		temp2.~TObject();
	      }
#else
	      TObject temp;
	      temp.Write(namestring);
	      temp.~TObject();
#endif
#ifdef DEBUG
	      printf("---- result: %s eff=%.5f+%.5f-%.5f\n",Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])],result[iPlotFlavor][iNonemptyJetVarBin],fitting_upError[iPlotFlavor][iNonemptyJetVarBin],fitting_downError[iPlotFlavor][iNonemptyJetVarBin]);
#endif
	      iNonemptyJetVarBins[iPlotFlavor]++;
	    }//end of if totY>0
	    else printf("-- No %s jet is in this JetVar bin (%.2f)\n",Flavor_Names[Int_t(PlotFlavor[iPlotFlavor])],fplots[0]->GetYaxis()->GetBinCenter(iJetVarBin));
	  }//end of Flavor plots
	  break;
	}//end of if require eff searched
    }//end of if totX>0
    else printf("-- No %s jet is in this JetVar bin (%.2f)\n",Flavor_Names[Int_t(RequiredEff_Flavor)],fplots[0]->GetYaxis()->GetBinCenter(iJetVarBin));
  }//end of JetVarBins
  for (Int_t iPlotFlavor=0;iPlotFlavor<nPlotFlavors;iPlotFlavor++) {
    FlavorEffVsJetVarPlots[iPlotFlavor]=new TGraphAsymmErrors(iNonemptyJetVarBins[iPlotFlavor],JetVar[iPlotFlavor],result[iPlotFlavor],xError[iPlotFlavor],xError[iPlotFlavor],result_downError[iPlotFlavor],result_upError[iPlotFlavor]);
    FlavorEffVsJetVarPlotswtFittingBias[iPlotFlavor]=new TGraphAsymmErrors(iNonemptyJetVarBins[iPlotFlavor],JetVar[iPlotFlavor],result[iPlotFlavor],xError[iPlotFlavor],xError[iPlotFlavor],fitting_downError[iPlotFlavor],fitting_upError[iPlotFlavor]);
 }
}

//#################################################
//## Definiations of Functions in Class Plotting ##
//#################################################
void Plotting::WhichFlavors(Byte_t iJet,vector<Flavor> &theflavors) {
  Int_t pdgid=MCTrueFlavor[iJet];
#ifdef CLEANMCTRUTHFLAVORBYPUTRACK
  if (IPnSelectedTracks[iJet]==0) pdgid=-1;
  else if (IPnSelectedPUTracks[iJet]>0 ) pdgid=-1;
#endif
#ifdef CLEANMCTRUTHFLAVORBYDecayLengthAndJetAsixPUTRACK
  if (IPnSelectedAndDecayLengthAndJetAsixTracks[iJet]==0) pdgid=-1;
  else if (IPnSelectedAndDecayLengthAndJetAsixPUTracks[iJet]>0 ) pdgid=-1;
#endif
  switch ( pdgid ) {
  case 5://B
    theflavors.push_back(B);
    if ( isBGluonSplitting ) theflavors.push_back(Bgluonsplit);
    break;
  case 4://C
    theflavors.push_back(C);
    if ( isCGluonSplitting ) theflavors.push_back(Cgluonsplit);
    break;
  case 1://D
    theflavors.push_back(D);
    theflavors.push_back(UDS);
    theflavors.push_back(UDSG);
    theflavors.push_back(UDSGwtNoFlavor);
    break;
  case 2://U
    theflavors.push_back(U);
    theflavors.push_back(UDS);
    theflavors.push_back(UDSG);
    theflavors.push_back(UDSGwtNoFlavor);
    break;
  case 3://S
    theflavors.push_back(S);
    theflavors.push_back(UDS);
    theflavors.push_back(UDSG);
    theflavors.push_back(UDSGwtNoFlavor);
    break;
  case 21://G
    theflavors.push_back(G);
    theflavors.push_back(UDSG);
    theflavors.push_back(UDSGwtNoFlavor);
    break;
  default://BvsUDSGwtPU
    theflavors.push_back(NoFlavor);
    theflavors.push_back(UDSGwtNoFlavor);
  }
}

void Plotting::FlavourHist(DiscriminatorOption option, const Float_t weight, DistributionPlots2D **VsJetPt, DistributionPlots2D **VsJetEta, DistributionPlots2D **VsJetPhi, DistributionPlots2D **VsEventPtHat) {
  VsJetPhi=NULL;VsEventPtHat=NULL;//Not plotting there now
  if (fChain == 0) return;
  Float_t *Var=SetVar(option);
  if ( Var==NULL ) return;
  Long64_t Nevents=0;
  Long64_t nentries = fChain->GetEntries();
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if ( LoadTree(jentry) < 0 ) break;
    fChain->GetEntry(jentry);
#ifdef EventCut
    if (!(EventCut)) continue;
#endif
#ifdef CALJETMEANZ
    Float_t MeanZ[30];
    Jet_MeanTrackLongitudinalIP(MeanZ);
#endif
#if bTagNtupleVersion>4
    numberOfPUVertices=UInt_t(numberOfPUVerticesMixingTruth);
#endif
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
      if (!(JetCut(iJet))) continue;
      Float_t this_variable=Var[iJet];
      Float_t pt=WhichPt[iJet],eta=WhichEta[iJet];
      vector<Flavor> theflavors;
      WhichFlavors(iJet,theflavors);
      for (vector<Flavor>::const_iterator iflavor=theflavors.begin();iflavor!=theflavors.end();iflavor++) {
	VsJetPt[numberOfPUVertices]->Fill(*iflavor,this_variable,pt,weight);
	VsJetEta[numberOfPUVertices]->Fill(*iflavor,this_variable,fabs(eta),weight);
      }
    }
    Nevents++;
  }
//Jets which are heavily biased by PU tracks are considered as no flavor (PU)
  cout<<Nevents<<" events were considered."<<endl;
}
#if bTagNtupleVersion==4
void Plotting::NPUTracks(DistributionPlots2D *NJetPlots,DistributionPlots2D *RatioPlots,DistributionPlots2D *NJetPlotswtDecayLengthAndJetAxisCut,DistributionPlots2D *RatioPlotswtDecayLengthAndJetAxisCut) {
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntries();
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if ( LoadTree(jentry) < 0 ) break;
    fChain->GetEntry(jentry);
#ifdef EventCut
    if (!(EventCut)) continue;
#endif
#ifdef CALJETMEANZ
    Float_t MeanZ[30];
    Jet_MeanTrackLongitudinalIP(MeanZ);
#endif
#if bTagNtupleVersion>4
    numberOfPUVertices=UInt_t(numberOfPUVerticesMixingTruth);
#endif
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
      if (!(JetCut(iJet))) continue;
      vector<Flavor> theflavors;
      WhichFlavors(iJet,theflavors);
      for (vector<Flavor>::const_iterator iflavor=theflavors.begin();iflavor!=theflavors.end();iflavor++) {
	NJetPlots->Fill(*iflavor,IPnSelectedPUTracks[iJet],numberOfPUVertices,puweight[numberOfPUVertices]);
	if ( IPnSelectedTracks[iJet] ==0 ) RatioPlots->Fill(*iflavor,-1,numberOfPUVertices,puweight[numberOfPUVertices]);
	else RatioPlots->Fill(*iflavor,IPnSelectedPUTracks[iJet]/Float_t(IPnSelectedTracks[iJet]),numberOfPUVertices,puweight[numberOfPUVertices]);
	NJetPlotswtDecayLengthAndJetAxisCut->Fill(*iflavor,IPnSelectedAndDecayLengthAndJetAsixPUTracks[iJet],numberOfPUVertices,puweight[numberOfPUVertices]);
	if ( IPnSelectedAndDecayLengthAndJetAsixTracks[iJet]==0 ) RatioPlotswtDecayLengthAndJetAxisCut->Fill(*iflavor,-1,numberOfPUVertices,puweight[numberOfPUVertices]);
	else RatioPlotswtDecayLengthAndJetAxisCut->Fill(*iflavor,IPnSelectedAndDecayLengthAndJetAsixPUTracks[iJet]/Float_t(IPnSelectedAndDecayLengthAndJetAsixTracks[iJet]),numberOfPUVertices,puweight[numberOfPUVertices]);
      }
    }
  }
}
#endif

#if bTagNtupleVersion>=4
void Plotting::MCPtvsRecoPt(DistributionPlots2D **GenPtvsJetPt, DistributionPlots2D **PartonvsJetPt, DistributionPlots1D **BiasFromGenPt, DistributionPlots1D **BiasFromPartonPt) {
  if (fChain == 0) return;
    Long64_t Nevents=0;
  Long64_t nentries = fChain->GetEntries();
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if ( LoadTree(jentry) < 0 ) break;
    fChain->GetEntry(jentry);
#ifdef EventCut
    if (!(EventCut)) continue;
#endif
#ifdef CALJETMEANZ
    Float_t MeanZ[30];
    Jet_MeanTrackLongitudinalIP(MeanZ);
#endif
#if bTagNtupleVersion>4
    numberOfPUVertices=UInt_t(numberOfPUVerticesMixingTruth);
#endif
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
#ifdef JetCut
      if (!(JetCut(iJet))) continue;
#endif
      vector<Flavor> theflavors;
      WhichFlavors(iJet,theflavors);
      for (vector<Flavor>::const_iterator iflavor=theflavors.begin();iflavor!=theflavors.end();iflavor++) {
	if ( GenJetPt[iJet]>0 ) {
	  GenPtvsJetPt[numberOfPUVertices]->Fill(*iflavor,jetPt[iJet],GenJetPt[iJet]);
	  BiasFromGenPt[numberOfPUVertices]->Fill(*iflavor,(jetPt[iJet]-GenJetPt[iJet])/GenJetPt[iJet]);
	}
	if ( PartonPt[iJet]>0 ) {
	  PartonvsJetPt[numberOfPUVertices]->Fill(*iflavor,jetPt[iJet],PartonPt[iJet]);
	  BiasFromPartonPt[numberOfPUVertices]->Fill(*iflavor,(jetPt[iJet]-PartonPt[iJet])/PartonPt[iJet]);
	}
      }
    }
    Nevents++;
  }
  cout<<Nevents<<" events were considered while looking for MCPtvsRecoPt."<<endl;
}
#endif

void Plotting::MeanPt(DistributionPlots1D **TotalPt, DistributionPlots1D **TotalEntries) {
  if (fChain == 0) return;
    Long64_t Nevents=0;
  Long64_t nentries = fChain->GetEntries();
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if ( LoadTree(jentry) < 0 ) break;
    fChain->GetEntry(jentry);
#ifdef EventCut
    if (!(EventCut)) continue;
#endif
#ifdef CALJETMEANZ
    Float_t MeanZ[30];
    Jet_MeanTrackLongitudinalIP(MeanZ);
#endif
#if bTagNtupleVersion>4
    numberOfPUVertices=UInt_t(numberOfPUVerticesMixingTruth);
#endif
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
#ifdef JetCut
      if (!(JetCut(iJet))) continue;
#endif
      vector<Flavor> theflavors;
      Float_t this_variable=WhichPt[iJet];
      WhichFlavors(iJet,theflavors);
      for (vector<Flavor>::const_iterator iflavor=theflavors.begin();iflavor!=theflavors.end();iflavor++) {
	TotalPt[numberOfPUVertices]->Fill(*iflavor,this_variable,this_variable);
	TotalEntries[numberOfPUVertices]->Fill(*iflavor,this_variable,1);
      }
    }
    Nevents++;
  }
  cout<<Nevents<<" events were considered while checking the mean pT."<<endl;
}

void Plotting::UncertaintyOfMeanPt(DistributionPlots1D *Average, DistributionPlots1D **Variance,Byte_t PU_LowEdge,Byte_t PU_HighEdge) {
  if (fChain == 0) return;
  Long64_t Nevents=0;
  Long64_t nentries = fChain->GetEntries();
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if ( LoadTree(jentry) < 0 ) break;
    fChain->GetEntry(jentry);
#ifdef EventCut
    if (!(EventCut)) continue;
#endif
#if bTagNtupleVersion>4
    numberOfPUVertices=UInt_t(numberOfPUVerticesMixingTruth);
#endif
    if (numberOfPUVertices<PU_LowEdge||numberOfPUVertices>PU_HighEdge) continue;
#ifdef CALJETMEANZ
    Float_t MeanZ[30];
    Jet_MeanTrackLongitudinalIP(MeanZ);
#endif
    for ( Byte_t iJet=0; iJet<nJets; iJet++ ) {
#ifdef JetCut
      if (!(JetCut(iJet))) continue;
#endif
      Float_t this_variable=WhichPt[iJet];
      vector<Flavor> theflavors;
      WhichFlavors(iJet,theflavors);
      for (vector<Flavor>::const_iterator iflavor=theflavors.begin();iflavor!=theflavors.end();iflavor++) 
	Variance[numberOfPUVertices]->FillVariance(*iflavor,this_variable,Average);
    }
    Nevents++;
  }
  cout<<Nevents<<" events were considered while calculating the uncertainty of pT."<<endl;
}

int main( int argc, char* argv[] ) {
  Load(argv[1]);
  return 0;
}
//Something might-be-used

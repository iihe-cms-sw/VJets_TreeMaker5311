#include "TROOT.h"
#include "TStyle.h"
#include "TColor.h"
#include "TObject.h"
#include "TFile.h"
#include "TProfile.h"
#include "THStack.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TPaveText.h"
#include "TStyle.h"

#include <boost/program_options.hpp>
#include <math.h>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <utility>

#include "bTag/CommissioningCommonSetup/interface/TSelectorMultiDraw.h"
#include "bTag/CommissioningCommonSetup/interface/GenericSelector.h"
#include "bTag/CommissioningCommonSetup/interface/GenericSelector2D.h"
#include "bTag/CommissioningCommonSetup/interface/GenericTrackSelector.h"
#include "bTag/CommissioningCommonSetup/interface/GenericTrackSelector2D.h"
#include "bTag/CommissioningCommonSetup/interface/GenericTrackQualitySelector.h"
#include "bTag/CommissioningCommonSetup/interface/GenericJetTrackQualitySelector.h"
#include "bTag/CommissioningCommonSetup/interface/MuonSelector.h"
#include "bTag/CommissioningCommonSetup/interface/TrackSelector.h"
#include "bTag/CommissioningCommonSetup/interface/PtHatSelector.h"
#include "bTag/CommissioningCommonSetup/interface/CutCompSelector.h"

#include "bTag/CommissioningCommonSetup/interface/Information1D.h"
#include "bTag/CommissioningCommonSetup/interface/Information2D.h"
#include "bTag/CommissioningCommonSetup/interface/InformationCut.h"
#include "bTag/CommissioningCommonSetup/interface/InformationCutComp.h"
#include "bTag/CommissioningCommonSetup/interface/InformationTrackCuts.h"
#include "bTag/CommissioningCommonSetup/interface/InformationMuonCuts.h"
#include "bTag/CommissioningCommonSetup/interface/InformationQuality.h"
#include "bTag/CommissioningCommonSetup/interface/InformationPtHat.h"

#include "bTag/CommissioningCommonSetup/interface/FlavorHistogram.h"
#include "bTag/CommissioningCommonSetup/interface/QualityHistogram.h"

namespace po = boost::program_options;

enum PlotType{jetPlots1D, trackPlots_flavorStack, trackPlots_qualityStack, jetTrackQualityPlots, effVsCutPlot,
              trackCutPlot, muonCutPlot, cutCompPlots, ptHatPlots, jetPlots2D, reweightedPlot, reweightedTrackPlot,
              cutPlots, PlotTypeSize};

std::string const PlotNames[] = {"jetPlots1D", "trackPlots_flavorStack", "trackPlots_qualityStack",
                                 "jetTrackQualityPlots", "effVsCutPlot", "trackCutPlot", "muonCutPlot", "cutCompPlots",
                                 "ptHatPlots", "jetPlots2D", "reweightedPlot", "reweightedTrackPlot", "cutPlots"};

PlotType getPlotTypeByName(const std::string& name)
{
  unsigned int index = std::find(PlotNames, PlotNames + PlotTypeSize, name) - PlotNames;
  if(index == PlotTypeSize)
  {
    std::cout << "Invalid plot type of " << name << "!  Aborting." << std::endl;
    abort();
  }

  return PlotType(index);
}

std::pair<std::string, std::string> getEntryAndValue(const std::string& line)
{
  const size_t position = line.find("=");
  std::string entry(line.substr(0, position));
  size_t first = entry.find_first_not_of(" ");
  size_t last = entry.find_last_not_of(" ");
  entry = entry.substr(first, last - first + 1);
  std::string value(line.substr(position + 1));
  first = value.find_first_not_of(" ");
  last = value.find_last_not_of(" ");
  value = value.substr(first, last - first + 1);

  return std::pair<std::string, std::string>(entry, value);
}

double IntegralAndError(const TH1D* h, const int i, const int j, double &err){
  double integral = h->Integral( i, j);

  double error = 0;
  for(int k = i; k != j; ++k){
    error += h->GetBinError(k) * h->GetBinError(k);
  }

  err = sqrt(error);
  return integral;
}

//Header on canvas: CMS preliminary/cm energy and lumi
// for plots with ratio at the bottom
void cmsPrel(const double& intLumi) {

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.045);
  latex->SetTextFont(42); //22

  latex->SetTextAlign(13);
  latex->DrawLatex(0.22, 0.96, Form("CMS Preliminary 2010,     #sqrt{s} = 7 TeV,  L = %.2g pb^{   -1}",intLumi));

}

// For profile and noRatio plots
void cmsProfPrel(const double& intLumi) {

  TLatex *latex = new TLatex();
  latex->SetNDC();
  latex->SetTextSize(0.035);
  latex->SetTextFont(42); //22

  latex->SetTextAlign(13);
  latex->DrawLatex(0.22, 0.95, Form("CMS Preliminary 2010,   #sqrt{s} = 7 TeV,  L = %.2g pb^{-1}",intLumi));

}

void setTDRStyle() {

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-DR");

  gROOT->SetStyle("Plain");
  gROOT->ForceStyle();

  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetPadBorderMode(0);
  tdrStyle->SetPadBorderMode(0);

  //	tdrStyle->SetFrameColor(0);
  tdrStyle->SetPadColor(0);
  tdrStyle->SetCanvasColor(0);
  tdrStyle->SetStatColor(0);
  tdrStyle->SetFillColor(0);

  tdrStyle->SetPaperSize(20,26);
  //	tdrStyle->SetPadTopMargin(0.08);
  //	tdrStyle->SetPadBottomMargin(0.14);
  tdrStyle->SetPadRightMargin(0.04);
  tdrStyle->SetPadLeftMargin(0.16);
  //	tdrStyle->SetCanvasDefH(800);
  //	tdrStyle->SetCanvasDefW(800);
  //	tdrStyle->SetPadGridX(1);
  //	tdrStyle->SetPadGridY(1);
  tdrStyle->SetPadTickX(1);
  tdrStyle->SetPadTickY(1);

  tdrStyle->SetTextFont(42); //132
  tdrStyle->SetTextSize(0.09);
  tdrStyle->SetLabelFont(42,"xyz");
  tdrStyle->SetTitleFont(42,"xyz");
  tdrStyle->SetLabelSize(0.045,"xyz"); //0.035
  tdrStyle->SetTitleSize(0.045,"xyz");
  tdrStyle->SetTitleOffset(1.5,"y");
    
  tdrStyle->SetTitleX(0.16);
  tdrStyle->SetTitleY(0.93);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(0);
  tdrStyle->SetTitleBorderSize(1);
  tdrStyle->SetTitleFontSize(0.04);
  //	tdrStyle->SetPadTopMargin(0.05);
  //	tdrStyle->SetPadBottomMargin(0.13);
  //	tdrStyle->SetPadLeftMargin(0.14);
  //	tdrStyle->SetPadRightMargin(0.02);

  // use bold lines and markers
  tdrStyle->SetMarkerStyle(8);
  tdrStyle->SetHistLineWidth(3);
  tdrStyle->SetLineWidth(1);
  
  tdrStyle->SetOptTitle(kFALSE);
  tdrStyle->SetOptStat(0);

  tdrStyle->cd();
}

using namespace std;

Information1D param1d(ifstream& plotFile)
{
  Information1D thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "jetCut"){
      thisPlot.setJetCut(value);
    }
    else if(entry == "trackCut"){
      thisPlot.setTrackCut(value);
    }
    else if(entry == "bScale"){
      thisPlot.setBScale(atof(value.c_str()));
    }
    else if(entry == "cScale"){
      thisPlot.setCScale(atof(value.c_str()));
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "yTitle"){
      thisPlot.setYTitle(value);
    }
    else if(entry == "yMin"){
      thisPlot.setYMin(atof(value.c_str()));
    }
    else if(entry == "ratioMin"){
      thisPlot.setRatioMin(atof(value.c_str()));
    }
    else if(entry == "ratioMax"){
      thisPlot.setRatioMax(atof(value.c_str()));
    }
    else if(entry == "ratioRebin"){
      thisPlot.setRatioRebin(atof(value.c_str()));
    }
    else if(entry == "displayOverUnderflowBin"){
      thisPlot.setDisplayOverUnderflowBin((bool)atoi(value.c_str()));
    }
    else if(entry == "displayNoInfo"){
      thisPlot.setDisplayNoInfo((bool)(atoi(value.c_str())));
    }
    else if(entry == "legendPosition"){
      thisPlot.setLegendPosition((bool)(atoi(value.c_str())));
    }
  } 
  return thisPlot;
}


InformationTrackCuts paramTrackCuts(ifstream& plotFile)
{
  InformationTrackCuts thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "bScale"){
      thisPlot.setBScale(atof(value.c_str()));
    }
    else if(entry == "cScale"){
      thisPlot.setCScale(atof(value.c_str()));
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "yTitle"){
      thisPlot.setYTitle(value);
    }
    else if(entry == "yMin"){
      thisPlot.setYMin(atof(value.c_str()));
    }
    else if(entry == "ratioMin"){
      thisPlot.setRatioMin(atof(value.c_str()));
    }
    else if(entry == "ratioMax"){
      thisPlot.setRatioMax(atof(value.c_str()));
    }
    else if(entry == "ratioRebin"){
      thisPlot.setRatioRebin(atof(value.c_str()));
    }
    else if(entry == "displayOverUnderflowBin"){
      thisPlot.setDisplayOverUnderflowBin((bool)atoi(value.c_str()));
    }
    else if(entry == "displayNoInfo"){
      thisPlot.setDisplayNoInfo((bool)(atoi(value.c_str())));
    }
    else if(entry == "legendPosition"){
      thisPlot.setLegendPosition((bool)(atoi(value.c_str())));
    }
    else if(entry == "jetPtCut"){
      thisPlot.setJetPtCut(atof(value.c_str()));
    }
    else if(entry == "jetEtaCut"){
      thisPlot.setJetEtaCut(atof(value.c_str()));
    }
    else if(entry == "nHitsCut"){
      thisPlot.setNHitsCut(atoi(value.c_str()));
    }
    else if(entry == "nPixHitsCut"){
      thisPlot.setNPixHitsCut(atoi(value.c_str()));
    }
    else if(entry == "Chi2Cut"){
      thisPlot.setChi2Cut(atof(value.c_str()));
    }
    else if(entry == "trackPtCut"){
      thisPlot.setTrackPtCut(atof(value.c_str()));
    }
    else if(entry == "distJetAxisCut"){
      thisPlot.setDistJetAxisCut(atof(value.c_str()));
    }
    else if(entry == "decayLengthCut"){
      thisPlot.setDecayLengthCut(atof(value.c_str()));
    }
    else if(entry == "IP2dCut"){
      thisPlot.setIP2dCut(atof(value.c_str()));
    }
    else if(entry == "longIPCut"){
      thisPlot.setLongIPCut(atof(value.c_str()));
    }
    else if(entry == "triggerHLTJet30U"){
      thisPlot.setTriggerHLTJet30U((bool)(atoi(value.c_str())));
    }
  }
  return thisPlot;
}

InformationMuonCuts paramMuonCuts(ifstream& plotFile)
{
  InformationMuonCuts thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "bScale"){
      thisPlot.setBScale(atof(value.c_str()));
    }
    else if(entry == "cScale"){
      thisPlot.setBScale(atof(value.c_str()));
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "yTitle"){
      thisPlot.setYTitle(value);
    }
    else if(entry == "yMin"){
      thisPlot.setYMin(atof(value.c_str()));
    }
    else if(entry == "ratioMin"){
      thisPlot.setRatioMin(atof(value.c_str()));
    }
    else if(entry == "ratioMax"){
      thisPlot.setRatioMax(atof(value.c_str()));
    }
    else if(entry == "ratioRebin"){
      thisPlot.setRatioRebin(atof(value.c_str()));
    }
    else if(entry == "displayOverUnderflowBin"){
      thisPlot.setDisplayOverUnderflowBin((bool)atoi(value.c_str()));
    }
    else if(entry == "displayNoInfo"){
      thisPlot.setDisplayNoInfo((bool)(atoi(value.c_str())));
    }
    else if(entry == "legendPosition"){
      thisPlot.setLegendPosition((bool)(atoi(value.c_str())));
    }
    else if(entry == "triggerHLTJet30U"){
      thisPlot.setTriggerHLTJet30U((bool)(atoi(value.c_str())));
    }
    else if(entry == "jetPtCut"){
      thisPlot.setJetPtCut(atof(value.c_str()));
    }
    else if(entry == "jetEtaCut"){
      thisPlot.setJetEtaCut(atof(value.c_str()));
    }
    else if(entry == "muonPtCut"){
      thisPlot.setMuonPtCut(atof(value.c_str()));
    }
    else if(entry == "muonEtaCut"){
      thisPlot.setMuonEtaCut(atof(value.c_str()));
    }
    else if(entry == "muonIsGlobal"){
      thisPlot.setMuonIsGlobal((bool)(atoi(value.c_str())));
    }
    else if(entry == "muonGlobalMuonHits"){
      thisPlot.setMuonGlobalMuonHits(atoi(value.c_str()));
    }
    else if(entry == "muonNumberOfMatches"){
      thisPlot.setMuonNumberOfMatches(atoi(value.c_str()));
    }
    else if(entry == "muonInnerValidHits"){
      thisPlot.setMuonInnerValidHits(atoi(value.c_str()));
    }
    else if(entry == "muonNPixelHits"){
      thisPlot.setMuonNPixelHits(atoi(value.c_str()));
    }
    else if(entry == "muonNExpectedOuterHits"){
      thisPlot.setMuonNExpectedOuterHits(atoi(value.c_str()));
    }
    else if(entry == "muonInnerNChi2"){
      thisPlot.setMuonInnerNChi2(atof(value.c_str()));
    }
    else if(entry == "muonGlobalNChi2"){
      thisPlot.setMuonGlobalNChi2(atof(value.c_str()));
    }
    else if(entry == "muonVzPVDist"){
      thisPlot.setMuonVzPVDist(atof(value.c_str()));
    }
    else if(entry == "muonDeltaR"){
      thisPlot.setMuonDeltaR(atof(value.c_str()));
    }
  }
  return thisPlot;
}

InformationCutComp paramCutComp(ifstream& plotFile)
{
  InformationCutComp thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "cutList"){
      std::string cutListString = value;
      char* allCuts = new char [cutListString.size()+1];
      strcpy(allCuts, cutListString.c_str());
      char * tokens = strtok(allCuts,",");
      while(tokens!=NULL){
	    thisPlot.addCutItem(std::string(tokens));
	    tokens = strtok(NULL,",");
      }
      delete[] allCuts;
    }
    else if(entry == "labelList"){
      std::string labelListString = value;
      char* allLabels = new char [labelListString.size()+1];
      strcpy (allLabels, labelListString.c_str());
      char * tokens = strtok(allLabels,",");
      while(tokens!=NULL){
	    thisPlot.addLabelItem(std::string(tokens));
	    tokens = strtok(NULL,",");
      }
      delete[] allLabels;
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "ratioMin"){
      thisPlot.setRatioMin(atof(value.c_str()));
    }
    else if(entry == "ratioMax"){
      thisPlot.setRatioMax(atof(value.c_str()));
    }
    else if(entry == "ratioRebin"){
      thisPlot.setRatioRebin(atof(value.c_str()));
    }
    else if(entry == "displayNoInfo"){
      thisPlot.setDisplayNoInfo((bool)(atoi(value.c_str())));
    }
    else if(entry == "legendPosition"){
      thisPlot.setLegendPosition((bool)(atoi(value.c_str())));
    }
  }
  return thisPlot;
}

InformationQuality paramQuality(ifstream& plotFile)
{
  InformationQuality thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "qualityVar"){
      thisPlot.setQualityVar(value);
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
  }
  return thisPlot;
}

InformationPtHat paramPtHat(ifstream& plotFile)
{
  InformationPtHat thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "pthatCut"){
      thisPlot.setPtHatCut(value);
    }
    else if(entry == "ptHatBins"){
      std::string binString = value;
      vector<double> bins;
      char* allBins;
      allBins = new char [binString.size()+1];
      strcpy (allBins, binString.c_str());
      char * tokens = strtok(allBins,",");
      while(tokens!=NULL){
	    thisPlot.addPtHatBin(atof(tokens));
	    tokens = strtok(NULL,",");
      }
      delete[] allBins;
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "pthatlow"){
      thisPlot.setPtHatLow(atof(value.c_str()));
    }
    else if(entry == "pthatup"){
      thisPlot.setPtHatUp(atof(value.c_str()));
    }
    else if(entry == "nbinspthat"){
      thisPlot.setNBinsPtHat(atoi(value.c_str()));
    }
  }
  return thisPlot;
}

Information2D param2d(ifstream& plotFile)
{
  Information2D thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "aliasy"){
      thisPlot.setAliasY(value);
    }
    else if(entry == "yTitle"){
      thisPlot.setYTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "jetCut"){
      thisPlot.setJetCut(value);
    }
    else if(entry == "trackCut"){
      thisPlot.setTrackCut(value);
    }
    else if(entry == "bScale"){
      thisPlot.setBScale(atof(value.c_str()));
    }
    else if(entry == "cScale"){
      thisPlot.setCScale(atof(value.c_str()));
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "ylow"){
      thisPlot.setYLow(atof(value.c_str()));
    }
    else if(entry == "yup"){
      thisPlot.setYUp(atof(value.c_str()));
    }
    else if(entry == "nbinsy"){
      thisPlot.setNBinsY(atoi(value.c_str()));
    }
    else if(entry == "displayNoInfo"){
      thisPlot.setDisplayNoInfo((bool)(atoi(value.c_str())));
    }
    else if(entry == "legendPosition"){
      thisPlot.setLegendPosition((bool)(atoi(value.c_str())));
    }
  }
  return thisPlot;
}

InformationCut paramCut(ifstream& plotFile)
{
  InformationCut thisPlot;
  std::string line;
  while(!getline(plotFile,line).eof() && line != "end_plot") {
    if (line[0] == '#') continue;
    std::pair<std::string, std::string> parsedLine = getEntryAndValue(line);
    const std::string& entry = parsedLine.first;
    const std::string& value = parsedLine.second;
    if(entry == "plotName"){
      thisPlot.setPlotName(value);
    }
    else if(entry == "plotTitle"){
      thisPlot.setPlotTitle(value);
    }
    else if(entry == "label"){
      thisPlot.setLabel(value);
    }
    else if(entry == "aliasx"){
      thisPlot.setAliasX(value);
    }
    else if(entry == "xTitle"){
      thisPlot.setXTitle(value);
    }
    else if(entry == "aliasy"){
      thisPlot.setAliasY(value);
    }
    else if(entry == "yTitle"){
      thisPlot.setYTitle(value);
    }
    else if(entry == "cut"){
      thisPlot.setCut(value);
    }
    else if(entry == "bScale"){
      thisPlot.setBScale(atof(value.c_str()));
    }
    else if(entry == "cScale"){
      thisPlot.setCScale(atof(value.c_str()));
    }
    else if(entry == "direction"){
      thisPlot.setDirection(value);
    }
    else if(entry == "xlow"){
      thisPlot.setXLow(atof(value.c_str()));
    }
    else if(entry == "xup"){
      thisPlot.setXUp(atof(value.c_str()));
    }
    else if(entry == "nbinsx"){
      thisPlot.setNBinsX(atoi(value.c_str()));
    }
    else if(entry == "ylow"){
      thisPlot.setYLow(atof(value.c_str()));
    }
    else if(entry == "yup"){
      thisPlot.setYUp(atof(value.c_str()));
    }
    else if(entry == "nbinsy"){
      thisPlot.setNBinsY(atoi(value.c_str()));
    }
    else if(entry == "displayNoInfo"){
      thisPlot.setDisplayNoInfo((bool)(atoi(value.c_str())));
    }
    else if(entry == "legendPosition"){
      thisPlot.setLegendPosition((bool)(atoi(value.c_str())));
    }
  }
  return thisPlot;
}

double binarySummer(int dimension,vector<int>& whichCut, int cutPoint, int start, vector<double>& binaryHist)
{
  //recursive summation for cut correlation calculations
  //cout << "in the summer" << endl;
  double sum = 0;
  for(unsigned int k = start; k != whichCut.size(); ++k)
    {
      bool kMatch = false;
      for(int i = 0; i != dimension; ++i)
	{
	  if(static_cast<int>(k) == whichCut[i]){
	    kMatch = true;
	    continue;
	  }
	}
      if(kMatch) continue;
      whichCut[cutPoint] = k;
      unsigned long iBin = 0;
      for(int i = 0; i < cutPoint+1; ++i)
	    {
	      iBin += 0x1 << whichCut[i];
	    }
      sum += binaryHist[iBin];
      if(cutPoint < static_cast<int>(whichCut.size()))  sum += binarySummer(dimension,whichCut,cutPoint+1,k+1,binaryHist);
    }
  //cout << "leaving summer" << endl;
  return sum;
}

void MakeACutCompPlot(const InformationCutComp& info, const TH1D* CutCompInfo_data, const TH1D* CutCompInfo_mc)
{
  //Sort Out Triggers into Correlation Plot
  TH2D* dataCorrelations = new TH2D((info.getPlotName()+"_dataCorrelations").c_str(),info.getPlotName().c_str(),info.getCutList().size(),0,info.getCutList().size(),info.getCutList().size(),0,info.getCutList().size());
  dataCorrelations->Sumw2();
  TH2D* mcCorrelations = new TH2D((info.getPlotName()+"_mcCorrelations").c_str(),info.getPlotName().c_str(),info.getCutList().size(),0,info.getCutList().size(),info.getCutList().size(),0,info.getCutList().size());
  mcCorrelations->Sumw2();
  vector<int> whichCut(info.getCutList().size(),0);
  vector<double> transcribe_data;
  vector<double> transcribe_mc;
  vector<double> transcribe_data_error2;
  vector<double> transcribe_mc_error2;
  double integral_mc=0;
  double integral_data=0;
  double integral_mc_error=0;
  double integral_data_error=0;
  for(int i = 0; i <= CutCompInfo_data->GetNbinsX(); ++i)
    {
      double placeHolder=CutCompInfo_mc->GetBinContent(i);
      transcribe_mc.push_back(placeHolder);
      integral_mc+=placeHolder;
      placeHolder=CutCompInfo_data->GetBinContent(i);
      transcribe_data.push_back(placeHolder);
      integral_data+=placeHolder;
      placeHolder = pow(CutCompInfo_mc->GetBinError(i),2);
      transcribe_mc_error2.push_back(placeHolder);
      integral_mc_error+=placeHolder;
      placeHolder = pow(CutCompInfo_data->GetBinError(i),2);
      transcribe_data_error2.push_back(placeHolder);
      integral_data_error+=placeHolder;
    }
  integral_mc_error=sqrt(integral_mc_error);
  integral_data_error=sqrt(integral_data_error);
  for(unsigned int i = 0; i != whichCut.size(); ++i)
    {
      whichCut[0]=i;
      unsigned long index_i = 0x1 << i;
      double dataBinContent = transcribe_data[index_i];
      double dataBinError = transcribe_data_error2[index_i];
      dataBinContent += binarySummer(1,whichCut,1,0,transcribe_data);
      dataBinError += binarySummer(1,whichCut,1,0,transcribe_data_error2);
      double mcBinContent = transcribe_mc[index_i];
      double mcBinError = transcribe_mc_error2[index_i];
      mcBinContent += binarySummer(1,whichCut,1,0,transcribe_mc);
      mcBinError += binarySummer(1,whichCut,1,0,transcribe_mc_error2);
      dataCorrelations->SetBinContent(i+1,i+1,dataBinContent);
      dataCorrelations->SetBinError(i+1,i+1,sqrt(dataBinError));
      mcCorrelations->SetBinContent(i+1,i+1,mcBinContent);
      mcCorrelations->SetBinError(i+1,i+1,sqrt(mcBinError));
      for(unsigned int j = i+1; j < whichCut.size(); j++)
	{
	  whichCut[1]=j;
      unsigned long index_j = 0x1 << j;
	  dataBinContent = transcribe_data[index_i + index_j];
	  dataBinError = transcribe_data_error2[index_i + index_j];
	  dataBinContent += binarySummer(2,whichCut,2,0,transcribe_data);
	  dataBinError += binarySummer(2,whichCut,2,0,transcribe_data_error2);
	  mcBinContent = transcribe_mc[index_i + index_j];
	  mcBinError = transcribe_mc_error2[index_i + index_j];
	  mcBinContent += binarySummer(2,whichCut,2,0,transcribe_mc);
	  mcBinError += binarySummer(2,whichCut,2,0,transcribe_mc_error2);
	  dataCorrelations->SetBinContent(i+1,j+1,dataBinContent);
	  dataCorrelations->SetBinError(i+1,j+1,sqrt(dataBinError));
	  mcCorrelations->SetBinContent(i+1,j+1,mcBinContent);
	  mcCorrelations->SetBinError(i+1,j+1,sqrt(mcBinError));
	  dataCorrelations->SetBinContent(j+1,i+1,dataBinContent);
	  dataCorrelations->SetBinError(j+1,i+1,sqrt(dataBinError));
	  mcCorrelations->SetBinContent(j+1,i+1,mcBinContent);
	  mcCorrelations->SetBinError(j+1,i+1,sqrt(mcBinError));
	}
    }

  //include the official CMS label
  TPaveText *pt = new TPaveText(0.4,0.92,0.9,0.97,"brNDC");   
  pt->SetBorderSize(0);   
  pt->SetFillStyle(0);  
  pt->SetTextAlign(13);   
  pt->SetTextFont(42);   
  pt->SetTextSize(0.035);   

  //Set up 2D drawing with labels on plot
  TCanvas canvas_data_correlations((info.getPlotName()+"canvas_data_correlations").c_str(),info.getPlotTitle().c_str(),1024,1024);
  //Following code snippet stolen from http://www.slac.stanford.edu/BFROOT/www/Computing/Offline/ROOT/info/binLabels.C
  canvas_data_correlations.SetGrid();
  canvas_data_correlations.SetLeftMargin(0.20);
  canvas_data_correlations.SetBottomMargin(0.20);
  dataCorrelations->GetXaxis()->SetLabelOffset(99);
  dataCorrelations->GetYaxis()->SetLabelOffset(99);
  dataCorrelations->Draw("text");
  // draw labels along X
  TLatex t;
  double x,y;
  y = gPad->GetUymin() - 0.1*mcCorrelations->GetYaxis()->GetBinWidth(1);
  t.SetTextAngle(60);
  t.SetTextSize(0.02);
  t.SetTextAlign(33);
  for (unsigned int i=0;i<info.getLabelList().size();++i) {
    x = mcCorrelations->GetXaxis()->GetBinCenter(i+1);
    t.DrawLatex(x,y,info.getLabelItem(i).c_str());
  }
  // draw labels along y
  x = gPad->GetUxmin() - 0.1*dataCorrelations->GetXaxis()->GetBinWidth(1);
  t.SetTextAlign(32);
  t.SetTextAngle(0);
  for (unsigned int i=0;i<info.getLabelList().size();++i) {
    y = dataCorrelations->GetYaxis()->GetBinCenter(i+1);
    t.DrawLatex(x,y,info.getLabelItem(i).c_str());
  }
  pt->Draw();
  canvas_data_correlations.SaveAs((info.getPlotName()+"_data_correlations.pdf").c_str());
  canvas_data_correlations.SaveAs((info.getPlotName()+"_data_correlations.png").c_str());
  canvas_data_correlations.SaveAs((info.getPlotName()+"_data_correlations.root").c_str());

  TCanvas canvas_mc_correlations((info.getPlotName()+"canvas_mc_correlations").c_str(),info.getPlotTitle().c_str(),1024,1024);
  //Following code snippet stolen from http://www.slac.stanford.edu/BFROOT/www/Computing/Offline/ROOT/info/binLabels.C
  canvas_mc_correlations.SetGrid();
  canvas_mc_correlations.SetLeftMargin(0.15);
  canvas_mc_correlations.SetBottomMargin(0.15);
  mcCorrelations->GetXaxis()->SetLabelOffset(99);
  mcCorrelations->GetYaxis()->SetLabelOffset(99);
  mcCorrelations->Draw("text");
  // draw labels along X
  y = gPad->GetUymin() - 0.1*mcCorrelations->GetYaxis()->GetBinWidth(1);
  t.SetTextAngle(60);
  t.SetTextSize(0.02);
  t.SetTextAlign(33);
  for (unsigned int i=0;i<info.getLabelList().size();++i) {
    x = mcCorrelations->GetXaxis()->GetBinCenter(i+1);
    t.DrawLatex(x,y,info.getLabelItem(i).c_str());
  }
  // draw labels along y
  x = gPad->GetUxmin() - 0.1*mcCorrelations->GetXaxis()->GetBinWidth(1);
  t.SetTextAlign(32);
  t.SetTextAngle(0);
  for (unsigned int i=0;i<info.getLabelList().size();++i) {
    y = mcCorrelations->GetYaxis()->GetBinCenter(i+1);
    t.DrawLatex(x,y,info.getLabelItem(i).c_str());
  }
  pt->Draw();
  canvas_mc_correlations.SaveAs((info.getPlotName()+"_mc_correlations.pdf").c_str());
  canvas_mc_correlations.SaveAs((info.getPlotName()+"_mc_correlations.png").c_str());
  canvas_mc_correlations.SaveAs((info.getPlotName()+"_mc_correlations.root").c_str());

  //Set up 1D Efficiency Plots -- Diagonal Entries/CutCompInfo->Integral(0,-1)

  //TH1D* dataPassed = new TH1D((info.plotName+"_dataPassed").c_str(),info.plotName.c_str(),info.cutList.size(),0,info.cutList.size());
  //dataPassed->Sumw2();
  //TH1D* mcPassed = new TH1D((info.plotName+"_mcPassed").c_str(),info.plotName.c_str(),info.cutList.size(),0,info.cutList.size());
  //mcPassed->Sumw2();
  //TH1D* dataTotal = new TH1D((info.plotName+"_dataTotal").c_str(),info.plotName.c_str(),info.cutList.size(),0,info.cutList.size());
  //dataTotal->Sumw2();
  //TH1D* mcTotal = new TH1D((info.plotName+"_mcTotal").c_str(),info.plotName.c_str(),info.cutList.size(),0,info.cutList.size());
  //mcTotal->Sumw2();

  TH1D* dataEfficiencies = new TH1D((info.getPlotName()+"_dataEfficiencies").c_str(),info.getPlotName().c_str(),info.getCutList().size(),0,info.getCutList().size());
  dataEfficiencies->Sumw2();
  TH1D* mcEfficiencies = new TH1D((info.getPlotName()+"_mcEfficiencies").c_str(),info.getPlotName().c_str(),info.getCutList().size(),0,info.getCutList().size());
  mcEfficiencies->Sumw2();


  for (unsigned int i = 0; i != info.getLabelList().size(); ++i) {
    //dataPassed->SetBinContent(i+1,dataCorrelations->GetBinContent(i+1,i+1));
    //dataPassed->SetBinError(i+1,dataCorrelations->GetBinError(i+1,i+1));
    //mcPassed->SetBinContent(i+1,mcCorrelations->GetBinContent(i+1,i+1));
    //mcPassed->SetBinError(i+1,mcCorrelations->GetBinError(i+1,i+1));
    //dataTotal->SetBinContent(i+1,integral_data);
    //dataTotal->SetBinError(i+1,integral_data_error);
    //mcTotal->SetBinContent(i+1,integral_mc);
    //mcTotal->SetBinError(i+1,integral_mc_error);
    double data_content = dataCorrelations->GetBinContent(i+1,i+1);
    double data_error = dataCorrelations->GetBinError(i+1,i+1);
    double data_calc_ratio = data_content/integral_data;
    double data_calc_error = 0;
    if(data_content!=integral_data) data_calc_error = sqrt(fabs(((1.-2.*data_calc_ratio)*data_error*data_error+data_calc_ratio*data_calc_ratio*integral_data_error*integral_data_error)/(integral_data*integral_data)));
    dataEfficiencies->SetBinContent(i+1,data_calc_ratio);
    dataEfficiencies->SetBinError(i+1, data_calc_error);
    
    double mc_content = mcCorrelations->GetBinContent(i+1,i+1);
    double mc_calc_ratio = mc_content/integral_mc;
    double mc_calc_error = 0;
    if(mc_content!=integral_mc) mc_calc_error = sqrt(fabs(((1.-2.*data_calc_ratio)*data_error*data_error+data_calc_ratio*data_calc_ratio*integral_data_error*integral_data_error)/(integral_data*integral_data)));
    mcEfficiencies->SetBinContent(i+1,mc_calc_ratio);
    mcEfficiencies->SetBinError(i+1,mc_calc_error);
  }


  //dataEfficiencies->Divide(dataPassed,dataTotal,1,1,"B");
  //mcEfficiencies->Divide(mcPassed,mcTotal,1,1,"B");

  dataEfficiencies->SetMaximum(1);
  mcEfficiencies->SetMaximum(1);
  mcEfficiencies->GetYaxis()->SetTitle("Cut Efficiency");

  TH1D* drawHelper = (TH1D*)dataEfficiencies->Clone((info.getPlotName()+"draw_helper").c_str());
  drawHelper->SetMarkerStyle(20);

  TLegend legend(0.725,0.6, 0.925, 0.85);
  legend.AddEntry(drawHelper,"Data","LPE");
  legend.AddEntry(mcEfficiencies,"MC","F");

  TCanvas canvas_efficiencies((info.getPlotName()+"efficiencies").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_efficiencies.SetBottomMargin(0.20);
  mcEfficiencies->GetXaxis()->SetLabelOffset(99);
  dataEfficiencies->GetXaxis()->SetLabelOffset(99);
  drawHelper->GetXaxis()->SetLabelOffset(99);
  mcEfficiencies->Draw("HIST");
  dataEfficiencies->SetLineColor(kBlack);
  dataEfficiencies->Draw("E1SAME");
  drawHelper->Draw("PSAME");
  // draw labels along X
  y = gPad->GetUymin() - 0.01*mcCorrelations->GetYaxis()->GetBinWidth(1);
  t.SetTextAngle(60);
  t.SetTextSize(0.02);
  t.SetTextAlign(33);
  for (unsigned int i = 0; i != info.getLabelList().size(); ++i) {
    x = mcCorrelations->GetXaxis()->GetBinCenter(i+1);
    t.DrawLatex(x,y,info.getLabelItem(i).c_str());
  }
  legend.Draw();
  pt->Draw();
  
  canvas_efficiencies.SaveAs((info.getPlotName()+"_efficiencies.pdf").c_str());
  canvas_efficiencies.SaveAs((info.getPlotName()+"_efficiencies.png").c_str());
  canvas_efficiencies.SaveAs((info.getPlotName()+"_efficiencies.root").c_str());

  //Add MC Histograms and Create a Scale Factor from untriggered hists (good for any of the hists)
  if(info.getAliasX() != "")
    {
      
    }
}

void MakeAProfilePlot(const Information1D& info, const FlavorHistogram& hists, const double& intLumi, const int dataVsmc)
{
  TH1D* data_hist = static_cast<TH1D*>(hists.getDataHist());
  TH1D* mc_all_hist = static_cast<TH1D*>(hists.getMCHistAll());
  TH1D* mc_light_hist = static_cast<TH1D*>(hists.getMCHistLight());
  TH1D* mc_none_hist = static_cast<TH1D*>(hists.getMCHistNone());
  TH1D* mc_c_hist = static_cast<TH1D*>(hists.getMCHistC());
  TH1D* mc_b_hist = static_cast<TH1D*>(hists.getMCHistB());
  TH1D* mc_bglusplit_hist = static_cast<TH1D*>(hists.getMCHistBGluSplit());


  THStack mc_stack((info.getPlotName() + "mc_stack").c_str(), info.getPlotTitle().c_str());
  if(dataVsmc) {
    mc_none_hist->SetLineColor(kMagenta);
    mc_none_hist->SetLineWidth(3);
    mc_none_hist->SetFillStyle(0);
    mc_light_hist->SetLineColor(kBlue);
    mc_light_hist->SetLineWidth(3);
    mc_light_hist->SetFillStyle(0);
    mc_c_hist->SetLineColor(kGreen);
    mc_c_hist->SetLineWidth(3);
    mc_c_hist->SetFillStyle(0);
    mc_b_hist->SetLineColor(kRed);
    mc_b_hist->SetLineWidth(3);
    mc_b_hist->SetFillStyle(0);
    mc_bglusplit_hist->SetLineColor(kOrange);
    mc_bglusplit_hist->SetLineWidth(3);
    mc_bglusplit_hist->SetFillStyle(0);
    mc_all_hist->SetLineColor(kBlack);
  }
  else  {
    mc_all_hist->SetLineColor(kRed);
    mc_all_hist->SetMarkerColor(kRed);
  }
  mc_all_hist->SetLineWidth(3);
  mc_all_hist->SetFillStyle(0);

  if(dataVsmc) {
    if(info.getDisplayNoInfo()) mc_stack.Add(mc_none_hist);
    mc_stack.Add(mc_light_hist);
    mc_stack.Add(mc_c_hist);
    mc_stack.Add(mc_b_hist);
    mc_stack.Add(mc_bglusplit_hist);
    mc_stack.Add(mc_all_hist);
  }
  else     mc_stack.Add(mc_all_hist);

  data_hist->SetLineColor(kBlack);
  data_hist->SetFillStyle(0);

  TH1D* drawHelper = (TH1D*)data_hist->Clone((info.getPlotName() + "draw_helper").c_str());
  drawHelper->SetMarkerStyle(20);

  //add ratio
  TH1D* ratio = (TH1D*)data_hist->Clone((info.getPlotName() + "ratio").c_str());
  TH1D* ratioMC = (TH1D*)mc_all_hist->Clone((info.getPlotName() + "ratioMC").c_str());
  if(dataVsmc)   ratio->SetTitle((info.getPlotTitle() + ": Data to Monte Carlo Simulation Ratio").c_str());  
  else           ratio->SetTitle((info.getPlotTitle() + ": Data to Data Ratio").c_str());  
  
  if(info.getRatioRebin() > 1){
    ratio->Rebin(info.getRatioRebin());
    ratioMC->Rebin(info.getRatioRebin());
  }

  ratio->Divide(ratioMC);
  ratio->SetMarkerStyle(20);

  TCanvas canvas_ratio((info.getPlotName() + "canvas_ratio").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_ratio.cd();
  if(dataVsmc)     ratio->SetYTitle("Data/Sim ratio");
  else             ratio->SetYTitle("Data/Data ratio");
  ratio->Draw("E1X0");
  cmsPrel(intLumi);
  canvas_ratio.SaveAs((info.getPlotName() + "_ratio.pdf").c_str());
  canvas_ratio.SaveAs((info.getPlotName() + "_ratio.png").c_str());
  canvas_ratio.SaveAs((info.getPlotName() + "_ratio.root").c_str());

  /////////////
  TLegend *legend;
  if(info.getLegendPosition())
    legend = new TLegend(0.725,0.15, 0.925, 0.35); //"bottom right"
  else
    legend = new TLegend(0.725,0.6, 0.925, 0.85); //By default legend is on the "top right" side of the canvas
  legend->AddEntry(drawHelper,"Data","LPE");
  if(dataVsmc) {
    legend->AddEntry(mc_all_hist,"Sim.(total)","L");
    legend->AddEntry(mc_light_hist,"Sim.(light)","L");
    legend->AddEntry(mc_c_hist,"Sim.(charm)","L");
    legend->AddEntry(mc_b_hist,"Sim.(bottom)","L");
    legend->AddEntry(mc_bglusplit_hist,"Sim.(b glusplit)","L");
    if(info.getDisplayNoInfo()) legend->AddEntry(mc_none_hist,"Sim.(no info)","L");
  }
  else      legend->AddEntry(mc_all_hist,"Data","LPE");
  
  legend->SetBorderSize(0);
  legend->SetFillColor(kWhite);
  legend->SetMargin(0.12);
  legend->SetTextSize(0.035);

  data_hist->GetYaxis()->CenterTitle(1);
  data_hist->GetYaxis()->SetTitleSize( 0.055 );
  data_hist->GetYaxis()->SetTitleOffset( 1.3 );

  //set the Y-axis range for the ratio
  ratio->GetYaxis()->SetRangeUser(info.getRatioMin(), info.getRatioMax());
  ratio->GetYaxis()->CenterTitle(1);
  ratio->GetYaxis()->SetTitleSize( 0.16 );
  ratio->GetYaxis()->SetTitleOffset( 0.425 );
  ratio->GetYaxis()->SetLabelSize( 0.16 );
  ratio->GetYaxis()->SetNdivisions( 505 );

  ratio->GetXaxis()->SetTitleSize( 0.16 );
  ratio->GetXaxis()->SetLabelSize( 0.16 );
  ratio->GetXaxis()->SetTitleOffset( 1 );
  ratio->GetXaxis()->SetLabelOffset( 0.006 );
  ratio->GetXaxis()->SetNdivisions( 510 );
  ratio->GetXaxis()->SetTickLength( ratio->GetXaxis()->GetTickLength() * 3.0 );
  if(dataVsmc) ratio->SetYTitle("Data/Sim");
  else         ratio->SetYTitle("Data/Data");

  TCanvas canvas((info.getPlotName() + "canvas").c_str(), info.getPlotTitle().c_str(),1024,1024);
  canvas.cd();
  canvas.Divide(1,2,0.01,0.0);
  
  canvas.cd(1);
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  canvas.cd(2);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();
 
  canvas.cd(1);

  if(dataVsmc)   mc_stack.Draw("HISTNOSTACK");
  else           mc_stack.Draw("E1X0");
  drawHelper->Draw("PSAME");
  data_hist->Draw("E1X0SAME");
  legend->Draw();

  cmsProfPrel(intLumi);

  gPad->Update();
  canvas.cd(2);
  ratio->Draw("E1X0");
  gPad->Update();

  canvas.SaveAs((info.getPlotName() + "_Linear.pdf").c_str());
  canvas.SaveAs((info.getPlotName() +"_Linear.png").c_str());
  canvas.SaveAs((info.getPlotName() +"_Linear.root").c_str());

  canvas.Clear();
  canvas.Divide(1,2,0.01,0.0);
  
  canvas.cd(1);
  gPad->SetLogy();
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  canvas.cd(2);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();

  canvas.cd(2);
  ratio->Draw("E1X0");

  canvas.cd(1);

  //  canvas.SetLogy();
  //  data_hist->SetMaximum(max(mc_stack_bDown.GetMaximum(),ymaxdef) * 6.0);
  //  data_hist->SetMinimum(info.getYMin());
  if(dataVsmc)   mc_stack.Draw("HISTNOSTACK");
  else           mc_stack.Draw("E1X0");
  drawHelper->Draw("PSAME");
  data_hist->Draw("E1X0SAME");
  legend->Draw();
  cmsProfPrel(intLumi);
  canvas.SaveAs((info.getPlotName() + "_Log.pdf").c_str());
  canvas.SaveAs((info.getPlotName() + "_Log.png").c_str());
  canvas.SaveAs((info.getPlotName() + "_Log.root").c_str());
}

void MakeAEffVsCutPlot(const Information1D& info, const FlavorHistogram& hists, const double& scale, const double& intLumi, const int dataVsmc)
{
  TH1D* data_hist = static_cast<TH1D*>(hists.getDataHist());
  TH1D* mc_light_hist = static_cast<TH1D*>(hists.getMCHistLight());
  TH1D* mc_all_hist = static_cast<TH1D*>(hists.getMCHistAll());
  TH1D* mc_none_hist = static_cast<TH1D*>(hists.getMCHistNone());
  TH1D* mc_c_hist = static_cast<TH1D*>(hists.getMCHistC());
  TH1D* mc_b_hist = static_cast<TH1D*>(hists.getMCHistB());
  TH1D* mc_bglusplit_hist = static_cast<TH1D*>(hists.getMCHistBGluSplit());


  double dataIntegralError = 0.0;
  double mc_allIntegralError = 0.0;
  
  double dataIntegral = IntegralAndError(data_hist, 0, info.getNBinsX() + 1, dataIntegralError);
  double mc_allIntegral = IntegralAndError(mc_all_hist, 0, info.getNBinsX() + 1, mc_allIntegralError);

  TH1D* dataEff = static_cast<TH1D*>(data_hist->Clone((info.getPlotName()+"dataEff").c_str()));
  TH1D* mc_allEff = static_cast<TH1D*>(mc_all_hist->Clone((info.getPlotName()+"mc_allEff").c_str()));
  
  for(unsigned int i=1; i<=info.getNBinsX() ; ++i){
    
    double dataIntEffError = 0.0;
    double mc_allIntEffError = 0.0;
    
    double dataIntEff = IntegralAndError(dataEff, i, info.getNBinsX()+1, dataIntEffError);
    double mc_allIntEff= IntegralAndError(mc_allEff, i, info.getNBinsX()+1, mc_allIntEffError);
    
    double dataEffError = sqrt( (dataIntEffError/dataIntegral)*(dataIntEffError/dataIntegral) 
				+ (dataIntEff*dataIntegralError/(dataIntegral*dataIntegral))*(dataIntEff*dataIntegralError/(dataIntegral*dataIntegral))  );
    double mc_allEffError = sqrt( (mc_allIntEffError/mc_allIntegral)*(mc_allIntEffError/mc_allIntegral) 
				  + (mc_allIntEff*mc_allIntegralError/(mc_allIntegral*mc_allIntegral))*(mc_allIntEff*mc_allIntegralError/(mc_allIntegral*mc_allIntegral))  );
    
    data_hist->SetBinContent    (i, dataIntEff/dataIntegral);
    data_hist->SetBinError      (i, dataEffError);
    mc_all_hist->SetBinContent  (i, mc_allIntEff/mc_allIntegral);
    mc_all_hist->SetBinError    (i, mc_allEffError);
    
  }

  if(dataVsmc) {//dataVsmc
    
    double mc_bIntegralError    ;
    double mc_bglusplitIntegralError    ;
    double mc_cIntegralError    ;
    double mc_lightIntegralError;
    double mc_noneIntegralError ;
    
    double mc_bIntegral     = IntegralAndError(mc_b_hist    , 0,info.getNBinsX()+1, mc_bIntegralError);
    double mc_bglusplitIntegral     = IntegralAndError(mc_bglusplit_hist    , 0,info.getNBinsX()+1, mc_bglusplitIntegralError);
    double mc_cIntegral     = IntegralAndError(mc_c_hist    , 0,info.getNBinsX()+1, mc_cIntegralError);
    double mc_lightIntegral = IntegralAndError(mc_light_hist, 0,info.getNBinsX()+1, mc_lightIntegralError);
    double mc_noneIntegral  = IntegralAndError(mc_none_hist , 0,info.getNBinsX()+1,  mc_noneIntegralError);
    
    TH1D *mc_bEff = static_cast<TH1D*>(mc_b_hist->Clone((info.getPlotName()+"mc_bEff").c_str()));
    TH1D *mc_bglusplitEff = static_cast<TH1D*>(mc_bglusplit_hist->Clone((info.getPlotName()+"mc_bglusplitEff").c_str()));
    TH1D *mc_cEff = static_cast<TH1D*>(mc_c_hist->Clone((info.getPlotName()+"mc_cEff").c_str()));
    TH1D *mc_lightEff = static_cast<TH1D*>(mc_light_hist->Clone((info.getPlotName()+"mc_lightEff").c_str()));
    TH1D *mc_noneEff  = static_cast<TH1D*>(mc_none_hist->Clone((info.getPlotName()+"mc_noneEff").c_str()));
    
    for(unsigned int i=1; i<=info.getNBinsX() ; ++i){
    
      double mc_bIntEffError = 0.0;
      double mc_bglusplitIntEffError = 0.0;
      double mc_cIntEffError = 0.0;
      double mc_lightIntEffError = 0.0;
      double mc_noneIntEffError = 0.0;
      
      double mc_bIntEff = IntegralAndError(mc_bEff    ,i,info.getNBinsX()+1, mc_bIntEffError);
      double mc_bglusplitIntEff = IntegralAndError(mc_bglusplitEff    ,i,info.getNBinsX()+1, mc_bglusplitIntEffError);
      double mc_cIntEff = IntegralAndError(mc_cEff    ,i,info.getNBinsX()+1, mc_cIntEffError);
      double mc_lightIntEff = IntegralAndError(mc_lightEff,i,info.getNBinsX()+1, mc_lightIntEffError);
      double mc_noneIntEff = IntegralAndError(mc_noneEff ,i,info.getNBinsX()+1,  mc_noneIntEffError);
      
      double mc_bEffError = sqrt( (mc_bIntEffError/mc_bIntegral)*(mc_bIntEffError/mc_bIntegral) 
				  + (mc_bIntEff*mc_bIntegralError/(mc_bIntegral*mc_bIntegral))*(mc_bIntEff*mc_bIntegralError/(mc_bIntegral*mc_bIntegral))  );
      double mc_bglusplitEffError = sqrt( (mc_bglusplitIntEffError/mc_bglusplitIntegral)*(mc_bglusplitIntEffError/mc_bglusplitIntegral) 
				  + (mc_bglusplitIntEff*mc_bglusplitIntegralError/(mc_bglusplitIntegral*mc_bglusplitIntegral))*(mc_bglusplitIntEff*mc_bglusplitIntegralError/(mc_bglusplitIntegral*mc_bglusplitIntegral))  );
      
      double mc_cEffError = sqrt( (mc_cIntEffError/mc_cIntegral)*(mc_cIntEffError/mc_cIntegral) 
				  + (mc_cIntEff*mc_cIntegralError/(mc_cIntegral*mc_cIntegral))*(mc_cIntEff*mc_cIntegralError/(mc_cIntegral*mc_cIntegral))  );
      
      double mc_lightEffError = sqrt( (mc_lightIntEffError/mc_lightIntegral)*(mc_lightIntEffError/mc_lightIntegral) 
				      + (mc_lightIntEff*mc_lightIntegralError/(mc_lightIntegral*mc_lightIntegral))*(mc_lightIntEff*mc_lightIntegralError/(mc_lightIntegral*mc_lightIntegral))  );
      
      double mc_noneEffError = sqrt( (mc_noneIntEffError/mc_noneIntegral)*(mc_noneIntEffError/mc_noneIntegral) 
				     + (mc_noneIntEff*mc_noneIntegralError/(mc_noneIntegral*mc_noneIntegral))*(mc_noneIntEff*mc_noneIntegralError/(mc_noneIntegral*mc_noneIntegral))  );
      
      mc_b_hist->SetBinContent    (i, mc_bIntEff/mc_bIntegral);
      mc_b_hist->SetBinError      (i, mc_bEffError);
      mc_bglusplit_hist->SetBinContent    (i, mc_bglusplitIntEff/mc_bglusplitIntegral);
      mc_bglusplit_hist->SetBinError      (i, mc_bglusplitEffError);
      mc_c_hist->SetBinContent    (i, mc_cIntEff/mc_cIntegral);
      mc_c_hist->SetBinError      (i, mc_cEffError);
      mc_light_hist->SetBinContent    (i, mc_lightIntEff/mc_lightIntegral);
      mc_light_hist->SetBinError      (i, mc_lightEffError);
      mc_none_hist->SetBinContent    (i, mc_noneIntEff/mc_noneIntegral);
      mc_none_hist->SetBinError      (i, mc_noneEffError);
    }
  }//dataVsmc
      
  data_hist    ->GetYaxis()->SetTitle("Efficiency");
  mc_all_hist  ->GetYaxis()->SetTitle("Efficiency");
  if(dataVsmc) {
     mc_b_hist    ->GetYaxis()->SetTitle("Efficiency");
     mc_bglusplit_hist    ->GetYaxis()->SetTitle("Efficiency");
     mc_c_hist    ->GetYaxis()->SetTitle("Efficiency");
     mc_light_hist->GetYaxis()->SetTitle("Efficiency");
     mc_none_hist ->GetYaxis()->SetTitle("Efficiency");
  }

  FlavorHistogram newHists;
  newHists.setDataHist(data_hist);
  newHists.setMCHistLight(mc_light_hist);
  newHists.setMCHistAll(mc_all_hist);
  newHists.setMCHistNone(mc_none_hist);
  newHists.setMCHistC(mc_c_hist);
  newHists.setMCHistB(mc_b_hist);
  newHists.setMCHistBGluSplit(mc_bglusplit_hist);

  MakeAProfilePlot(info, newHists, intLumi, dataVsmc);

}


void MakeAFlavorPlot(const Information1D& info, const FlavorHistogram& hists, double& scale, const double& intLumi, const int dataVsmc)
{
  TH1D* data_hist = static_cast<TH1D*>(hists.getDataHist());
  TH1D* mc_all_hist = static_cast<TH1D*>(hists.getMCHistAll());
  TH1D* mc_light_hist = static_cast<TH1D*>(hists.getMCHistLight());
  TH1D* mc_none_hist = static_cast<TH1D*>(hists.getMCHistNone());
  TH1D* mc_c_hist = static_cast<TH1D*>(hists.getMCHistC());
  TH1D* mc_b_hist = static_cast<TH1D*>(hists.getMCHistB());
  TH1D* mc_bglusplit_hist = static_cast<TH1D*>(hists.getMCHistBGluSplit());


  if(scale==0)
    {
      //assume normalization to data
      scale = data_hist->Integral()/mc_all_hist->Integral();
    }    
  if(scale==-1)
    {
      //assume normalization to data
      scale = data_hist->Integral(0,info.getNBinsX()+1)/mc_all_hist->Integral(0,info.getNBinsX()+1);
    }      
  mc_all_hist->Scale(scale);
  if(dataVsmc) {//only for data vs mc comparison
    mc_b_hist->Scale(scale);
    mc_bglusplit_hist->Scale(scale);
    mc_c_hist->Scale(scale);
    mc_light_hist->Scale(scale);
    mc_none_hist->Scale(scale);

    mc_none_hist->SetFillColor(kMagenta);
    mc_light_hist->SetFillColor(kBlue);
    mc_c_hist->SetFillColor(kGreen);
    mc_b_hist->SetFillColor(kRed);
    mc_bglusplit_hist->SetFillColor(kOrange);
  }
  mc_all_hist->SetLineColor(kRed);
  mc_all_hist->SetFillColor(kRed);
  mc_all_hist->SetMarkerColor(kRed);
  
  // calculate over and underflow bin if requested
  if(info.getDisplayOverUnderflowBin()){
    double nBinX=mc_all_hist->GetNbinsX();
    mc_all_hist->SetBinContent(nBinX, mc_all_hist->GetBinContent(nBinX) + mc_all_hist->GetBinContent(nBinX+1));
    mc_all_hist->SetBinContent(1, mc_all_hist->GetBinContent(1) + mc_all_hist->GetBinContent(0));
    if(dataVsmc) {//only for data vs mc comparison
      mc_b_hist->SetBinContent(nBinX, mc_b_hist->GetBinContent(nBinX) + mc_b_hist->GetBinContent(nBinX+1));
      mc_b_hist->SetBinContent(1, mc_b_hist->GetBinContent(1) + mc_b_hist->GetBinContent(0));
      mc_bglusplit_hist->SetBinContent(nBinX, mc_bglusplit_hist->GetBinContent(nBinX) + mc_bglusplit_hist->GetBinContent(nBinX+1));
      mc_bglusplit_hist->SetBinContent(1, mc_bglusplit_hist->GetBinContent(1) + mc_bglusplit_hist->GetBinContent(0));
      mc_c_hist->SetBinContent(nBinX, mc_c_hist->GetBinContent(nBinX) + mc_c_hist->GetBinContent(nBinX+1));
      mc_c_hist->SetBinContent(1, mc_c_hist->GetBinContent(1) + mc_c_hist->GetBinContent(0));
      mc_light_hist->SetBinContent(nBinX, mc_light_hist->GetBinContent(nBinX) + mc_light_hist->GetBinContent(nBinX+1));
      mc_light_hist->SetBinContent(1, mc_light_hist->GetBinContent(1) + mc_light_hist->GetBinContent(0));
      mc_none_hist->SetBinContent(nBinX, mc_none_hist->GetBinContent(nBinX) + mc_none_hist->GetBinContent(nBinX+1));
      mc_none_hist->SetBinContent(1, mc_none_hist->GetBinContent(1) + mc_none_hist->GetBinContent(0));
    }
    data_hist->SetBinContent(nBinX, data_hist->GetBinContent(nBinX) + data_hist->GetBinContent(nBinX+1));
    data_hist->SetBinContent(1, data_hist->GetBinContent(1) + data_hist->GetBinContent(0));
  }

  THStack mc_stack_bUp((info.getPlotName()+"_mc_stack_bUp").c_str(),info.getPlotTitle().c_str());
  if(dataVsmc) {//only for data vs mc comparison
    if(info.getDisplayNoInfo()) mc_stack_bUp.Add(mc_none_hist);
    mc_stack_bUp.Add(mc_light_hist);
    mc_stack_bUp.Add(mc_c_hist);
    mc_stack_bUp.Add(mc_bglusplit_hist);
    mc_stack_bUp.Add(mc_b_hist);
  }
  else     mc_stack_bUp.Add(mc_all_hist);

  THStack mc_stack_bDown((info.getPlotName()+"_mc_stack_bDown").c_str(),info.getPlotTitle().c_str());
  if(dataVsmc) {//only for data vs mc comparison
    mc_stack_bDown.Add(mc_b_hist);
    mc_stack_bDown.Add(mc_bglusplit_hist);
    mc_stack_bDown.Add(mc_c_hist);
    mc_stack_bDown.Add(mc_light_hist);
    if(info.getDisplayNoInfo()) mc_stack_bDown.Add(mc_none_hist);
  }
  else     mc_stack_bDown.Add(mc_all_hist);
  
  TH1D* drawHelper = (TH1D*)data_hist->Clone((info.getPlotName()+"draw_helper").c_str());
  drawHelper->SetMarkerStyle(20);
  
  TH1D* ratio = (TH1D*)data_hist->Clone((info.getPlotName()+"ratio").c_str());
  TH1D* ratioMC = (TH1D*)mc_all_hist->Clone((info.getPlotName()+"ratioMC").c_str());
  ratio->SetTitle((info.getPlotTitle()+": Data to Monte Carlo Simulation Ratio").c_str());  
  
  if(info.getRatioRebin() > 1){
    ratio->Rebin(info.getRatioRebin());
    ratioMC->Rebin(info.getRatioRebin());
  }

  ratio->Divide(ratioMC);
  ratio->SetMarkerStyle(20);

  TLegend *legend;
  if(info.getLegendPosition())
    legend = new TLegend(0.685, 0.575, 0.985, 0.875); // "top right"
  else
    legend = new TLegend(0.195, 0.575, 0.495, 0.875); //By default legend is on the "top left" side of the canvas
  legend->AddEntry(drawHelper,"Data","LPE");
  if(dataVsmc) {//only for data vs mc comparison
    legend->AddEntry(mc_light_hist,"Sim.(light)","F");
    legend->AddEntry(mc_c_hist,"Sim.(charm)","F");
    legend->AddEntry(mc_bglusplit_hist,"Sim.(b glusplit)","F");
    legend->AddEntry(mc_b_hist,"Sim.(bottom)","F");
    if(info.getDisplayNoInfo()) legend->AddEntry(mc_none_hist,"Sim.(no info)","F");
  }
  else     legend->AddEntry(mc_all_hist,"Data","LPE");

  legend->SetBorderSize(0);
  legend->SetFillColor(kWhite);
  legend->SetMargin(0.12);
  legend->SetTextSize(0.05);

  //store the default value of y-maximum
  double ymaxdef = data_hist->GetMaximum();
  data_hist->GetYaxis()->CenterTitle(1);

  //Make Canvas
  TCanvas canvas_bUp((info.getPlotName()+"canvas_bUp").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_bUp.cd();

  data_hist->SetMaximum(max(mc_stack_bUp.GetMaximum(),ymaxdef) * 1.4);
  data_hist->Draw("E1X0");
  if(dataVsmc) mc_stack_bUp.Draw("HISTSAME");
  else         mc_stack_bUp.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();

  //include the official CMS label
  cmsProfPrel(intLumi);

  canvas_bUp.SaveAs((info.getPlotName()+"_bUp_Linear_noRatio.pdf").c_str());
  canvas_bUp.SaveAs((info.getPlotName()+"_bUp_Linear_noRatio.png").c_str());
  canvas_bUp.SaveAs((info.getPlotName()+"_bUp_Linear_noRatio.root").c_str());
  canvas_bUp.Clear();
  canvas_bUp.SetLogy();

  data_hist->SetMaximum(max(mc_stack_bUp.GetMaximum(),ymaxdef) * 6.0);
  data_hist->SetMinimum(info.getYMin());
  data_hist->SetMarkerStyle(1);
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bUp.Draw("HISTSAME");
  else           mc_stack_bUp.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();
  cmsProfPrel(intLumi);
  canvas_bUp.SaveAs((info.getPlotName()+"_bUp_Log_noRatio.pdf").c_str());
  canvas_bUp.SaveAs((info.getPlotName()+"_bUp_Log_noRatio.png").c_str());
  canvas_bUp.SaveAs((info.getPlotName()+"_bUp_Log_noRatio.root").c_str());
  data_hist->SetMinimum(0);
  
  TCanvas canvas_bDown((info.getPlotName()+"canvas_bDown").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_bDown.cd();
  data_hist->SetMaximum(max(mc_stack_bDown.GetMaximum(),ymaxdef) * 1.4);
  data_hist->SetMarkerStyle(1);
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bDown.Draw("HISTSAME");
  else           mc_stack_bDown.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();

  //include the official CMS label
  cmsProfPrel(intLumi);

  canvas_bDown.SaveAs((info.getPlotName()+"_bDown_Linear_noRatio.pdf").c_str());
  canvas_bDown.SaveAs((info.getPlotName()+"_bDown_Linear_noRatio.png").c_str());
  canvas_bDown.SaveAs((info.getPlotName()+"_bDown_Linear_noRatio.root").c_str());
  canvas_bDown.Clear();
  canvas_bDown.SetLogy();
  data_hist->SetMaximum(max(mc_stack_bDown.GetMaximum(),ymaxdef) * 6.0);
  data_hist->SetMinimum(info.getYMin());
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bDown.Draw("HISTSAME");
  else           mc_stack_bDown.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();
  cmsProfPrel(intLumi);
  canvas_bDown.SaveAs((info.getPlotName()+"_bDown_Log_noRatio.pdf").c_str());
  canvas_bDown.SaveAs((info.getPlotName()+"_bDown_Log_noRatio.png").c_str());
  canvas_bDown.SaveAs((info.getPlotName()+"_bDown_Log_noRatio.root").c_str());

  TCanvas canvas_ratio((info.getPlotName()+"canvas_ratio").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_ratio.cd();
  if(dataVsmc) ratio->SetYTitle("Data/Sim");
  else         ratio->SetYTitle("Data/Data");
  ratio->Draw("E1X0");
  cmsPrel(intLumi);
  canvas_ratio.SaveAs((info.getPlotName()+"_ratio.pdf").c_str());
  canvas_ratio.SaveAs((info.getPlotName()+"_ratio.png").c_str());
  canvas_ratio.SaveAs((info.getPlotName()+"_ratio.root").c_str());
  
  ////////////////////////////////////////////////////
  // Set the titles, range etc here before drawing
  data_hist->GetYaxis()->CenterTitle(1);
  data_hist->GetYaxis()->SetTitleSize( 0.055 );
  data_hist->GetYaxis()->SetTitleOffset( 1.3 );

  //set the Y-axis range for the ratio
  ratio->GetYaxis()->SetRangeUser(info.getRatioMin(),info.getRatioMax());
  ratio->GetYaxis()->CenterTitle(1);
  ratio->GetYaxis()->SetTitleSize( 0.16 );
  ratio->GetYaxis()->SetTitleOffset( 0.425 );
  ratio->GetYaxis()->SetLabelSize( 0.16 );
  ratio->GetYaxis()->SetNdivisions( 505 );

  ratio->GetXaxis()->SetTitleSize( 0.16 );
  ratio->GetXaxis()->SetLabelSize( 0.16 );
  ratio->GetXaxis()->SetTitleOffset( 1 );
  ratio->GetXaxis()->SetLabelOffset( 0.006 );
  ratio->GetXaxis()->SetNdivisions( 510 );
  ratio->GetXaxis()->SetTickLength( ratio->GetXaxis()->GetTickLength() * 3.0 );
  if(dataVsmc) ratio->SetYTitle("Data/Sim");
  else         ratio->SetYTitle("Data/Data");

  //Draw the plot and ratio on the same canvas
  TCanvas canvas_bDownLin((info.getPlotName()+"canvas_bDownLin").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_bDownLin.cd();
  canvas_bDownLin.Clear();

  canvas_bDownLin.Divide(1,2,0.01,0.0);
  
  canvas_bDownLin.cd(1);
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  canvas_bDownLin.cd(2);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();
 
  canvas_bDownLin.cd(1);
  data_hist->SetMaximum(max(mc_stack_bDown.GetMaximum(),ymaxdef) * 1.4);
  data_hist->SetMinimum(0);
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bDown.Draw("HISTSAME");
  else           mc_stack_bDown.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();
  cmsPrel(intLumi);
  gPad->Update();

  canvas_bDownLin.cd(2);
  ratio->Draw("E1X0");
  gPad->Update();

  canvas_bDownLin.SaveAs((info.getPlotName()+"_bDown_Linear.pdf").c_str());
  canvas_bDownLin.SaveAs((info.getPlotName()+"_bDown_Linear.png").c_str());
  canvas_bDownLin.SaveAs((info.getPlotName()+"_bDown_Linear.root").c_str());
  //
  //
  TCanvas canvas_bDownLog((info.getPlotName()+"canvas_bDownLog").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_bDownLog.cd();

  canvas_bDownLog.Divide(1,2,0.01,0.0);
  
  canvas_bDownLog.cd(1);
  gPad->SetLogy();
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  canvas_bDownLog.cd(2);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();

  canvas_bDownLog.cd(2);
  ratio->Draw("E1X0");

  canvas_bDownLog.cd(1);
  data_hist->SetMaximum(max(mc_stack_bDown.GetMaximum(),ymaxdef) * 6.0);
  data_hist->SetMinimum(info.getYMin());
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bDown.Draw("HISTSAME");
  else           mc_stack_bDown.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();
  cmsPrel(intLumi);

  canvas_bDownLog.SaveAs((info.getPlotName()+"_bDown_Log.pdf").c_str());
  canvas_bDownLog.SaveAs((info.getPlotName()+"_bDown_Log.png").c_str());
  canvas_bDownLog.SaveAs((info.getPlotName()+"_bDown_Log.root").c_str());


  ////////////////////////////////////////
  //Draw the plot and ratio on the same canvas
  TCanvas canvas_bUpLin((info.getPlotName()+"canvas_bUpLin").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_bUpLin.cd();
  canvas_bUpLin.Clear();

  canvas_bUpLin.Divide(1,2,0.01,0.0);
  
  canvas_bUpLin.cd(1);
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  canvas_bUpLin.cd(2);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();
 
  canvas_bUpLin.cd(1);
  data_hist->SetMaximum(max(mc_stack_bUp.GetMaximum(),ymaxdef) * 1.4);
  data_hist->SetMinimum(0);
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bUp.Draw("HISTSAME");
  else           mc_stack_bUp.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();
  cmsPrel(intLumi);
  gPad->Update();

  canvas_bUpLin.cd(2);
  ratio->Draw("E1X0");
  gPad->Update();

  canvas_bUpLin.SaveAs((info.getPlotName()+"_bUp_Linear.pdf").c_str());
  canvas_bUpLin.SaveAs((info.getPlotName()+"_bUp_Linear.png").c_str());
  canvas_bUpLin.SaveAs((info.getPlotName()+"_bUp_Linear.root").c_str());
  //
  //
  TCanvas canvas_bUpLog((info.getPlotName()+"canvas_bUpLog").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_bUpLog.cd();

  canvas_bUpLog.Divide(1,2,0.01,0.0);
  
  canvas_bUpLog.cd(1);
  gPad->SetLogy();
  gPad->SetPad( 0.0, 0.25, 1.0, 1.0 );
  gPad->SetTopMargin(0.1);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);

  canvas_bUpLog.cd(2);
  gPad->SetPad( 0.0, 0.0,  1.0, 0.25 );
  gPad->SetBottomMargin(0.375);
  gPad->SetLeftMargin(0.16);
  gPad->SetRightMargin(0.04);
  gPad->SetGridy();

  canvas_bUpLog.cd(2);
  ratio->Draw("E1X0");

  canvas_bUpLog.cd(1);

  data_hist->SetMaximum(max(mc_stack_bUp.GetMaximum(),ymaxdef) * 6.0);
  data_hist->SetMinimum(info.getYMin());
  data_hist->Draw("E1X0");
  if(dataVsmc)   mc_stack_bUp.Draw("HISTSAME");
  else           mc_stack_bUp.Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend->Draw();
  cmsPrel(intLumi);

  canvas_bUpLog.SaveAs((info.getPlotName()+"_bUp_Log.pdf").c_str());
  canvas_bUpLog.SaveAs((info.getPlotName()+"_bUp_Log.png").c_str());
  canvas_bUpLog.SaveAs((info.getPlotName()+"_bUp_Log.root").c_str());

  return;
}

void MakeAPtHatPlot(const InformationPtHat& info, vector<TH1D*> mc_hists, TH1D* data_hist, TH1D* pthat_hist, double& scale)
{
  if(scale==0)
    {
      //assume normalization to data
      double mc_int = 0;
      for(vector<TH1D*>::iterator iPlot = mc_hists.begin();iPlot!=mc_hists.end(); ++iPlot)
	  {
	    mc_int+=(*iPlot)->Integral();
	  }
      scale = data_hist->Integral()/mc_int;
    }   
  if(scale==-1)
    {
      double mc_int = 0;
      for(vector<TH1D*>::iterator iPlot = mc_hists.begin();iPlot!=mc_hists.end(); ++iPlot)
	  {
	    mc_int+=(*iPlot)->Integral(0,info.getNBinsX()+1);
	  }
      scale = data_hist->Integral(0,info.getNBinsX()+1)/mc_int;
    }      

  THStack mc_stack_highUp((info.getPlotName()+"_mc_stack_highUp").c_str(),info.getPlotTitle().c_str());
  THStack mc_stack_highDown((info.getPlotName()+"_mc_stack_highDown").c_str(),info.getPlotTitle().c_str());

  TH1D* drawHelper = (TH1D*)data_hist->Clone((info.getPlotName()+"draw_helper").c_str());
  drawHelper->SetMarkerStyle(20);

  TLegend legend(0.725,0.65, 0.925, 0.85);
  legend.AddEntry(drawHelper,"Data","LPE");

  int iColor = 2;
  vector<double>::const_iterator iPtHat;
  vector<TH1D*>::iterator iPlot;
  for(iPtHat = info.getPtHatBins().begin() , iPlot = mc_hists.begin(); (iPtHat+1)!=info.getPtHatBins().end()||iPlot!=mc_hists.end(); ++iPtHat,++iPlot)
	  {
	    std::stringstream lowBin;
	    lowBin << *iPtHat;
	    std::stringstream highBin;
	    highBin << *(iPtHat+1);
	    (*iPlot)->Scale(scale);
	    (*iPlot)->SetFillColor(iColor);
	    iColor++;
	    mc_stack_highUp.Add(*iPlot);	    
	    if(*(iPtHat+1)!=-1) legend.AddEntry(*iPlot,(lowBin.str()+" < pthat < "+highBin.str()).c_str());
	    else legend.AddEntry(*iPlot,(lowBin.str()+" < pthat").c_str());
	  }

  for(vector<TH1D*>::reverse_iterator irPlot = mc_hists.rbegin(); irPlot!=mc_hists.rend(); irPlot++)
	  {
	    mc_stack_highDown.Add(*irPlot);
	  }

  mc_stack_highUp.SetMaximum(max(mc_stack_highUp.GetMaximum(),data_hist->GetMaximum()) * 1.2);
  mc_stack_highDown.SetMaximum(max(mc_stack_highDown.GetMaximum(),data_hist->GetMaximum()) * 1.2);

  legend.SetBorderSize(1);
  legend.SetFillColor(kWhite);
  //legend.SetFillStyle(1);

  TCanvas canvas_highUp((info.getPlotName()+"canvas_highUp").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_highUp.cd();
  mc_stack_highUp.Draw("HIST");
  data_hist->Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend.Draw();
  canvas_highUp.SaveAs((info.getPlotName()+"_highUp_Linear.pdf").c_str());
  canvas_highUp.SaveAs((info.getPlotName()+"_highUp_Linear.png").c_str());
  canvas_highUp.SaveAs((info.getPlotName()+"_highUp_Linear.root").c_str());
  canvas_highUp.Clear();
  canvas_highUp.SetLogy();
  mc_stack_highUp.SetMinimum(0.1);
  data_hist->SetMinimum(0.1);
  data_hist->SetMarkerStyle(1);
  mc_stack_highUp.Draw("HIST");
  data_hist->SetMarkerStyle(1);
  data_hist->Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend.Draw();
  canvas_highUp.SaveAs((info.getPlotName()+"_highUp_Log.pdf").c_str());
  canvas_highUp.SaveAs((info.getPlotName()+"_highUp_Log.png").c_str());
  canvas_highUp.SaveAs((info.getPlotName()+"_highUp_Log.root").c_str());
  data_hist->SetMinimum(0);
  
  TCanvas canvas_highDown((info.getPlotName()+"canvas_highDown").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_highDown.cd();
  mc_stack_highDown.Draw("HIST");
  data_hist->SetMarkerStyle(1);
  data_hist->Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend.Draw();
  canvas_highDown.SaveAs((info.getPlotName()+"_highDown_Linear.pdf").c_str());
  canvas_highDown.SaveAs((info.getPlotName()+"_highDown_Linear.png").c_str());
  canvas_highDown.SaveAs((info.getPlotName()+"_highDown_Linear.root").c_str());
  canvas_highDown.Clear();
  canvas_highDown.SetLogy();
  mc_stack_highDown.SetMinimum(0.1);
  data_hist->SetMinimum(0.1);
  mc_stack_highDown.Draw("HIST");
  data_hist->Draw("E1X0SAME");
  drawHelper->Draw("PSAME");
  legend.Draw();
  canvas_highDown.SaveAs((info.getPlotName()+"_highDown_Log.pdf").c_str());
  canvas_highDown.SaveAs((info.getPlotName()+"_highDown_Log.png").c_str());
  canvas_highDown.SaveAs((info.getPlotName()+"_highDown_Log.root").c_str());

  TCanvas canvas_pthat((info.getPlotName()+"canvas_pthat").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_pthat.cd();
  pthat_hist->SetMarkerStyle(21);
  pthat_hist->Draw();
  canvas_pthat.SaveAs((info.getPlotName()+"_pthat_Linear.pdf").c_str());
  canvas_pthat.SaveAs((info.getPlotName()+"_pthat_Linear.png").c_str());
  canvas_pthat.SaveAs((info.getPlotName()+"_pthat_Linear.root").c_str());
  pthat_hist->SetMinimum(0.1);
  canvas_pthat.Clear();
  canvas_pthat.SetLogy();
  pthat_hist->Draw();
  canvas_pthat.SaveAs((info.getPlotName()+"_pthat_Log.pdf").c_str());
  canvas_pthat.SaveAs((info.getPlotName()+"_pthat_Log.png").c_str());
  canvas_pthat.SaveAs((info.getPlotName()+"_pthat_Log.root").c_str());
}



void MakeAReweightedPlot(const Information2D& info, const FlavorHistogram& hists, double& scale, const double& intLumi, const int dataVsmc)
{
  TH2D* data_hist = static_cast<TH2D*>(hists.getDataHist());
  TH2D* mc_light_hist = static_cast<TH2D*>(hists.getMCHistLight());
  TH2D* mc_all_hist = static_cast<TH2D*>(hists.getMCHistAll());
  TH2D* mc_none_hist = static_cast<TH2D*>(hists.getMCHistNone());
  TH2D* mc_c_hist = static_cast<TH2D*>(hists.getMCHistC());
  TH2D* mc_b_hist = static_cast<TH2D*>(hists.getMCHistB());
  TH2D* mc_bglusplit_hist = static_cast<TH2D*>(hists.getMCHistBGluSplit());


  double finalScale = scale;
  if(scale==0)
    {
      //assume normalization to data
      scale = data_hist->Integral()/mc_all_hist->Integral();
    }   
  if(scale==-1)
    {
      //assume normalization to data
      scale = data_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1)/mc_all_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1);
    }   
  
  mc_all_hist->Scale(scale);
  if(dataVsmc) {
    mc_b_hist->Scale(scale);
    mc_bglusplit_hist->Scale(scale);
    mc_c_hist->Scale(scale);
    mc_light_hist->Scale(scale);
    mc_none_hist->Scale(scale);
  }

  TH1D* reweighted_data_hist = data_hist->ProjectionX();
  TH1D* reweighted_mc_all_hist = mc_all_hist->ProjectionX();
  TH1D* reweighted_mc_b_hist = mc_b_hist->ProjectionX();
  TH1D* reweighted_mc_bglusplit_hist = mc_bglusplit_hist->ProjectionX();
  TH1D* reweighted_mc_c_hist = mc_c_hist->ProjectionX();
  TH1D* reweighted_mc_light_hist = mc_light_hist->ProjectionX();
  TH1D* reweighted_mc_none_hist = mc_none_hist->ProjectionX();

  reweighted_data_hist->Reset();
  reweighted_mc_all_hist->Reset();
  reweighted_mc_b_hist->Reset();
  reweighted_mc_bglusplit_hist->Reset();
  reweighted_mc_c_hist->Reset();
  reweighted_mc_light_hist->Reset();
  reweighted_mc_none_hist->Reset();  
  
  TH1D* data_temp = 0;
  TH1D* mc_all_temp = 0;
  TH1D* mc_b_temp = 0;
  TH1D* mc_bglusplit_temp = 0;
  TH1D* mc_c_temp = 0;
  TH1D* mc_light_temp = 0;
  TH1D* mc_none_temp = 0;

  for(unsigned int iYbin = 0; iYbin != info.getNBinsY()+2; ++iYbin){
    double tempScale;
    data_temp = data_hist->ProjectionX("data_temp",iYbin,iYbin);
    mc_all_temp = mc_all_hist->ProjectionX("mc_all_temp",iYbin,iYbin);
    if(mc_all_temp->Integral(0,info.getNBinsY()+1) !=0 ) tempScale = data_temp->Integral(0,info.getNBinsY()+1)/mc_all_temp->Integral(0,info.getNBinsY()+1);
    else tempScale = 1;
    mc_all_temp->Scale(tempScale);
    reweighted_data_hist->Add(data_temp);
    reweighted_mc_all_hist->Add(mc_all_temp);

    if(dataVsmc) {
      mc_b_temp = mc_b_hist->ProjectionX("mc_b_temp",iYbin,iYbin);
      mc_bglusplit_temp = mc_bglusplit_hist->ProjectionX("mc_bglusplit_temp",iYbin,iYbin);
      mc_c_temp = mc_c_hist->ProjectionX("mc_c_temp",iYbin,iYbin);
      mc_light_temp = mc_light_hist->ProjectionX("mc_light_temp",iYbin,iYbin);
      mc_none_temp = mc_none_hist->ProjectionX("mc_none_temp",iYbin,iYbin);

      mc_b_temp->Scale(tempScale);
      mc_bglusplit_temp->Scale(tempScale);
      mc_c_temp->Scale(tempScale);
      mc_light_temp->Scale(tempScale);
      mc_none_temp->Scale(tempScale);
      reweighted_mc_b_hist->Add(mc_b_temp);
      reweighted_mc_bglusplit_hist->Add(mc_bglusplit_temp);
      reweighted_mc_c_hist->Add(mc_c_temp);
      reweighted_mc_light_hist->Add(mc_light_temp);
      reweighted_mc_none_hist->Add(mc_none_temp);
    }
    delete data_temp;
    delete mc_all_temp;
    if(mc_b_temp) delete mc_b_temp;
    if(mc_b_temp) delete mc_bglusplit_temp;
    if(mc_b_temp) delete mc_c_temp;
    if(mc_b_temp) delete mc_light_temp;
    if(mc_b_temp) delete mc_none_temp;
  }

  FlavorHistogram reweightedHists;
  reweightedHists.setDataHist(reweighted_data_hist);
  reweightedHists.setMCHistB(reweighted_mc_b_hist);
  reweightedHists.setMCHistBGluSplit(reweighted_mc_bglusplit_hist);
  reweightedHists.setMCHistC(reweighted_mc_c_hist);
  reweightedHists.setMCHistLight(reweighted_mc_light_hist);
  reweightedHists.setMCHistNone(reweighted_mc_none_hist);

  Information1D reweightedInfo;
  reweightedInfo.setDisplayNoInfo(info.getDisplayNoInfo());
  reweightedInfo.setPlotName(info.getPlotName());
  reweightedInfo.setPlotTitle(info.getPlotTitle());

  MakeAFlavorPlot(reweightedInfo,reweightedHists,finalScale, intLumi, dataVsmc);
}

void MakeA2DPlot(const Information2D& info, const FlavorHistogram& hists, double& scale, const double& intLumi, const int dataVsmc)
{
  TH2D* data_hist = static_cast<TH2D*>(hists.getDataHist());
  TH2D* mc_light_hist = static_cast<TH2D*>(hists.getMCHistLight());
  TH2D* mc_all_hist = static_cast<TH2D*>(hists.getMCHistAll());
  TH2D* mc_none_hist = static_cast<TH2D*>(hists.getMCHistNone());
  TH2D* mc_c_hist = static_cast<TH2D*>(hists.getMCHistC());
  TH2D* mc_b_hist = static_cast<TH2D*>(hists.getMCHistB());
  TH2D* mc_bglusplit_hist = static_cast<TH2D*>(hists.getMCHistBGluSplit());

  if(scale==0)
    {
      //assume normalization to data
      scale = data_hist->Integral()/mc_all_hist->Integral();
    }   
  if(scale==-1)
    {
      //assume normalization to data
      scale = data_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1)/mc_all_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1);
    }    

  mc_all_hist->Scale(scale);
  if(dataVsmc) {
    mc_b_hist->Scale(scale);
    mc_bglusplit_hist->Scale(scale);
    mc_c_hist->Scale(scale);
    mc_light_hist->Scale(scale);
    mc_none_hist->Scale(scale);
  }

  FlavorHistogram profiles;
  Information1D profInfo;
  profInfo.setDisplayNoInfo(info.getDisplayNoInfo());
  profInfo.setLegendPosition(info.getLegendPosition());

  TProfile* data_prof_x = data_hist->ProfileX();
  TProfile* mc_all_prof_x = mc_all_hist->ProfileX();
  data_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
  mc_all_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
  data_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
  mc_all_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
  profiles.setDataHist(data_prof_x);
  profiles.setMCHistAll(mc_all_prof_x);

  if(dataVsmc) {
    TProfile* mc_b_prof_x = mc_b_hist->ProfileX();
    TProfile* mc_bglusplit_prof_x = mc_bglusplit_hist->ProfileX();
    TProfile* mc_c_prof_x = mc_c_hist->ProfileX();
    TProfile* mc_light_prof_x = mc_light_hist->ProfileX();
    TProfile* mc_none_prof_x = mc_none_hist->ProfileX();
    mc_bglusplit_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_b_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_c_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_light_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_none_prof_x->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_b_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_bglusplit_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_c_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_light_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_none_prof_x->GetYaxis()->SetTitle(info.getYTitle().c_str());
    profiles.setMCHistB(mc_b_prof_x);
    profiles.setMCHistBGluSplit(mc_bglusplit_prof_x);
    profiles.setMCHistC(mc_c_prof_x);
    profiles.setMCHistLight(mc_light_prof_x);
    profiles.setMCHistNone(mc_none_prof_x);
  }
  
  profInfo.setPlotName(info.getPlotName()+"_x_profile");
  profInfo.setPlotTitle(info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile");
 
  MakeAProfilePlot(profInfo, profiles, intLumi, dataVsmc);

  TProfile* data_prof_y = data_hist->ProfileY();
  TProfile* mc_all_prof_y = mc_all_hist->ProfileY();
  data_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
  mc_all_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
  data_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
  mc_all_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
  profiles.setDataHist(data_prof_y);
  profiles.setMCHistAll(mc_all_prof_y);

  if(dataVsmc) {
    TProfile* mc_b_prof_y = mc_b_hist->ProfileY();
    TProfile* mc_bglusplit_prof_y = mc_bglusplit_hist->ProfileY();
    TProfile* mc_c_prof_y = mc_c_hist->ProfileY();
    TProfile* mc_light_prof_y = mc_light_hist->ProfileY();
    TProfile* mc_none_prof_y = mc_none_hist->ProfileY();
    mc_b_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
    mc_bglusplit_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
    mc_c_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
    mc_light_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
    mc_none_prof_y->SetTitle((info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile").c_str());
    mc_b_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
    mc_bglusplit_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
    mc_c_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
    mc_light_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
    mc_none_prof_y->GetYaxis()->SetTitle(info.getXTitle().c_str());
    profiles.setMCHistB(mc_b_prof_y);
    profiles.setMCHistBGluSplit(mc_bglusplit_prof_y);
    profiles.setMCHistC(mc_c_prof_y);
    profiles.setMCHistLight(mc_light_prof_y);
    profiles.setMCHistNone(mc_none_prof_y);
  }
  
  profInfo.setPlotName(info.getPlotName()+"_y_profile");
  profInfo.setPlotTitle(info.getPlotTitle()+" : "+" "+info.getYTitle()+" Profile");

  MakeAProfilePlot(profInfo, profiles, intLumi, dataVsmc);

  //Make Canvases

  if(dataVsmc) {
    TCanvas mc_comp((info.getPlotName()+"mc_comp").c_str(),info.getPlotTitle().c_str(),2048,2048);
    mc_comp.Divide(2,2);
    mc_comp.cd(1);
    mc_b_hist->SetTitle((info.getPlotTitle()+" : True b-jet").c_str());
    mc_b_hist->Draw("colz");
    mc_comp.cd(2);
    mc_c_hist->SetTitle((info.getPlotTitle()+" : True c-jet").c_str());
    mc_c_hist->Draw("colz");
    mc_comp.cd(3);
    mc_light_hist->SetTitle((info.getPlotTitle()+" : True light-jet").c_str());
    mc_light_hist->Draw("colz");
    mc_comp.cd(4);
    mc_none_hist->SetTitle((info.getPlotTitle()+" : No Truth Match").c_str());
    mc_none_hist->Draw("colz");
    std::string mc_name = info.getPlotName()+"_flavorComp.pdf";
    mc_comp.SaveAs(mc_name.c_str());
    mc_name = info.getPlotName()+"_flavorComp.png";
    mc_comp.SaveAs(mc_name.c_str());
    mc_name = info.getPlotName()+"_flavorComp.root";
    mc_comp.SaveAs(mc_name.c_str());
  }

  TCanvas data_comp((info.getPlotName()+"data_comp").c_str(),info.getPlotTitle().c_str(),2048,1024);
  data_comp.Divide(2,1);
  data_comp.cd(1);
  if(dataVsmc)   mc_all_hist->SetTitle((info.getPlotTitle()+" : Monte Carlo Simulation").c_str());
  else           mc_all_hist->SetTitle((info.getPlotTitle()+" : Data").c_str());
  mc_all_hist->Draw("colz");
  data_comp.cd(2);
  data_hist->SetTitle((info.getPlotTitle()+" : Data").c_str());
  data_hist->Draw("colz");
  std::string data_name = info.getPlotName()+"_dataComp.pdf";
  data_comp.SaveAs(data_name.c_str());
  data_name = info.getPlotName()+"_dataComp.png";
  data_comp.SaveAs(data_name.c_str());
  data_name = info.getPlotName()+"_dataComp.root";
  data_comp.SaveAs(data_name.c_str());
  return;
}

void MakeATrackQualityPlot(const InformationQuality& info, const QualityHistogram& hists, double& scale)
{
  TH1D* data_hist_undef = static_cast<TH1D*>(hists.getDataHistUndef());
  TH1D* data_hist_loose = static_cast<TH1D*>(hists.getDataHistLoose());
  TH1D* data_hist_tight = static_cast<TH1D*>(hists.getDataHistTight());
  TH1D* data_hist_high_purity = static_cast<TH1D*>(hists.getDataHistHighPurity());

  TH1D* mc_hist_undef = static_cast<TH1D*>(hists.getMCHistUndef());
  TH1D* mc_hist_loose = static_cast<TH1D*>(hists.getMCHistLoose());
  TH1D* mc_hist_tight = static_cast<TH1D*>(hists.getMCHistTight());
  TH1D* mc_hist_high_purity = static_cast<TH1D*>(hists.getMCHistHighPurity());

  if(scale==0)
    {
      //assume normalization to data
      scale = (data_hist_undef->Integral()+data_hist_loose->Integral()+data_hist_tight->Integral()+data_hist_high_purity->Integral())/(mc_hist_undef->Integral()+mc_hist_loose->Integral()+mc_hist_tight->Integral()+mc_hist_high_purity->Integral());
    }   
  if(scale==-1)
    {
      //assume normalization to data
      scale = (data_hist_undef->Integral(0,info.getNBinsX()+1)+data_hist_loose->Integral(0,info.getNBinsX()+1)+data_hist_tight->Integral(0,info.getNBinsX()+1)+data_hist_high_purity->Integral(0,info.getNBinsX()+1))/(mc_hist_undef->Integral(0,info.getNBinsX()+1)+mc_hist_loose->Integral(0,info.getNBinsX()+1)+mc_hist_tight->Integral(0,info.getNBinsX()+1)+mc_hist_high_purity->Integral(0,info.getNBinsX()+1));
    }   
  mc_hist_undef->Scale(scale);
  mc_hist_loose->Scale(scale);
  mc_hist_tight->Scale(scale);
  mc_hist_high_purity->Scale(scale);
  mc_hist_undef->SetFillColor(kCyan);
  mc_hist_loose->SetFillColor(kMagenta);
  mc_hist_tight->SetFillColor(kBlue);
  mc_hist_high_purity->SetFillColor(kRed);

  data_hist_undef->SetMarkerStyle(23);
  data_hist_loose->SetMarkerStyle(22);
  data_hist_tight->SetMarkerStyle(21);
  data_hist_high_purity->SetMarkerStyle(20);

  THStack mc_stack_hpUp((info.getPlotName()+"_mc_stack_hpUp").c_str(),info.getPlotTitle().c_str());
  mc_stack_hpUp.Add(mc_hist_undef);
  mc_stack_hpUp.Add(mc_hist_loose);
  mc_stack_hpUp.Add(mc_hist_tight);
  mc_stack_hpUp.Add(mc_hist_high_purity);

  THStack mc_stack_hpDown((info.getPlotName()+"_mc_stack_hpDown").c_str(),info.getPlotTitle().c_str());
  mc_stack_hpDown.Add(mc_hist_high_purity);
  mc_stack_hpDown.Add(mc_hist_tight);
  mc_stack_hpDown.Add(mc_hist_loose);
  mc_stack_hpDown.Add(mc_hist_undef);

  THStack data_stack_hpUp((info.getPlotName()+"_data_stack_hpUp").c_str(),info.getPlotTitle().c_str());
  data_stack_hpUp.Add(data_hist_undef);
  data_stack_hpUp.Add(data_hist_loose);
  data_stack_hpUp.Add(data_hist_tight);
  data_stack_hpUp.Add(data_hist_high_purity);

  THStack data_stack_hpDown((info.getPlotName()+"_data_stack_hpDown").c_str(),info.getPlotTitle().c_str());
  data_stack_hpDown.Add(data_hist_high_purity);
  data_stack_hpDown.Add(data_hist_tight);
  data_stack_hpDown.Add(data_hist_loose);
  data_stack_hpDown.Add(data_hist_undef);

  data_stack_hpUp.SetMaximum(max(data_stack_hpUp.GetMaximum(),mc_stack_hpUp.GetMaximum()) * 1.2);
  data_stack_hpDown.SetMaximum(max(data_stack_hpDown.GetMaximum(),mc_stack_hpDown.GetMaximum()) * 1.2);
  mc_stack_hpDown.SetMaximum(max(data_stack_hpDown.GetMaximum(),mc_stack_hpDown.GetMaximum()) * 1.2);
  mc_stack_hpUp.SetMaximum(max(data_stack_hpUp.GetMaximum(),mc_stack_hpUp.GetMaximum()) * 1.2);
  

  //Make Canvas

  TLegend MClegend(0.7,0.7,0.95,0.95);
  MClegend.SetHeader("Monte Carlo Simulation");
  MClegend.AddEntry(mc_hist_high_purity,"High Purity Tracks","F");
  MClegend.AddEntry(mc_hist_tight,"Tight Tracks","F");
  MClegend.AddEntry(mc_hist_loose,"Loose Tracks","F");
  MClegend.AddEntry(mc_hist_undef,"Undefined Tracks","F");

  MClegend.SetBorderSize(1);
  MClegend.SetFillColor(kWhite);

  TLegend datalegend(0.05,0.7,0.3,0.95);
  datalegend.SetHeader("Data");
  datalegend.AddEntry(data_hist_high_purity,"High Purity Tracks","PE");
  datalegend.AddEntry(data_hist_tight,"Tight Tracks","PE");
  datalegend.AddEntry(data_hist_loose,"Loose Tracks","PE");
  datalegend.AddEntry(data_hist_undef,"Undefined Tracks","PE");

  datalegend.SetBorderSize(1);
  datalegend.SetFillColor(kWhite);

  datalegend.SetTextSize(MClegend.GetTextSize());

  TCanvas canvas_hpUp((info.getPlotName()+"canvas_hpUp").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_hpUp.cd();
  mc_stack_hpUp.Draw("HIST");
  mc_stack_hpUp.GetXaxis()->SetTitle(info.getXTitle().c_str());
  data_stack_hpUp.Draw("E1X0SAME");
  MClegend.Draw();
  datalegend.Draw();
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpUp_overlay_Linear.pdf").c_str());
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpUp_overlay_Linear.png").c_str());
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpUp_overlay_Linear.root").c_str());
  canvas_hpUp.Clear();
  canvas_hpUp.SetLogy();
  mc_stack_hpUp.SetMinimum(0.1);
  data_stack_hpUp.SetMinimum(0.1);
  mc_stack_hpUp.Draw("HIST");
  mc_stack_hpUp.GetXaxis()->SetTitle(info.getXTitle().c_str());
  data_stack_hpUp.Draw("E1X0SAME");
  MClegend.Draw();
  datalegend.Draw();
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpUp_overlay_Log.pdf").c_str());
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpUp_overlay_Log.png").c_str());
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpUp_overlay_Log.root").c_str());
  
  TCanvas canvas_hpDown((info.getPlotName()+"canvas_hpDown").c_str(),info.getPlotTitle().c_str(),1024,1024);
  canvas_hpDown.cd();
  mc_stack_hpDown.Draw("HIST");
  mc_stack_hpDown.GetXaxis()->SetTitle(info.getXTitle().c_str());
  data_stack_hpDown.Draw("E1X0SAME");
  MClegend.Draw();
  datalegend.Draw();
  canvas_hpDown.SaveAs((info.getPlotName()+"_hpDown_overlay_Linear.pdf").c_str());
  canvas_hpDown.SaveAs((info.getPlotName()+"_hpDown_overlay_Linear.png").c_str());
  canvas_hpDown.SaveAs((info.getPlotName()+"_hpDown_overlay_Linear.root").c_str());
  canvas_hpDown.Clear();
  canvas_hpDown.SetLogy();
  mc_stack_hpDown.SetMinimum(0.1);
  data_stack_hpDown.SetMinimum(0.1);
  mc_stack_hpUp.Draw("HIST");
  data_stack_hpDown.Draw("E1X0SAME");
  MClegend.Draw();
  datalegend.Draw();
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpDown_overlay_Log.pdf").c_str());
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpDown_overlay_Log.png").c_str());
  canvas_hpUp.SaveAs((info.getPlotName()+"_hpDown_overlay_Log.root").c_str());

  mc_stack_hpUp.SetMinimum(0);
  data_stack_hpUp.SetMinimum(0);
  mc_stack_hpDown.SetMinimum(0);
  data_stack_hpDown.SetMinimum(0);

  data_hist_undef->SetFillColor(kCyan);
  data_hist_loose->SetFillColor(kMagenta);
  data_hist_tight->SetFillColor(kBlue);
  data_hist_high_purity->SetFillColor(kRed);

  THStack data_stack2_hpUp((info.getPlotName()+"_data_stack2_hpUp").c_str(),info.getPlotTitle().c_str());
  data_stack2_hpUp.Add(data_hist_undef);
  data_stack2_hpUp.Add(data_hist_loose);
  data_stack2_hpUp.Add(data_hist_tight);
  data_stack2_hpUp.Add(data_hist_high_purity);
  data_stack2_hpUp.SetMaximum(max(data_stack2_hpUp.GetMaximum(),mc_stack_hpUp.GetMaximum()) * 1.2);

  THStack data_stack2_hpDown((info.getPlotName()+"_data_stack2_hpDown").c_str(),info.getPlotTitle().c_str());
  data_stack2_hpDown.Add(data_hist_high_purity);
  data_stack2_hpDown.Add(data_hist_tight);
  data_stack2_hpDown.Add(data_hist_loose);
  data_stack2_hpDown.Add(data_hist_undef);
  data_stack2_hpDown.SetMaximum(max(data_stack2_hpDown.GetMaximum(),mc_stack_hpDown.GetMaximum()) * 1.2);

  TLegend legend2(0.7,0.7,0.95,0.95);
  legend2.AddEntry(mc_hist_high_purity,"High Purity Tracks","F");
  legend2.AddEntry(mc_hist_tight,"Tight Tracks","F");
  legend2.AddEntry(mc_hist_loose,"Loose Tracks","F");
  legend2.AddEntry(mc_hist_undef,"Undefined Tracks","F");

  legend2.SetBorderSize(1);
  legend2.SetFillColor(kWhite);

  mc_stack_hpDown.SetTitle((info.getPlotTitle()+" : Monte Carlo Simulation").c_str());
  data_stack2_hpDown.SetTitle((info.getPlotTitle()+" : Data").c_str());
  mc_stack_hpUp.SetTitle((info.getPlotTitle()+" : Monte Carlo Simulation").c_str());
  data_stack2_hpUp.SetTitle((info.getPlotTitle()+" : Data").c_str());

  TCanvas canvasTwo_hpUp((info.getPlotName()+"canvasTwo_hpUp").c_str(),info.getPlotTitle().c_str(),2048,1024);
  canvasTwo_hpUp.Divide(2,1);
  canvasTwo_hpUp.cd(1);
  data_stack2_hpUp.Draw("HIST");
  data_stack2_hpUp.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpUp.cd(2);
  mc_stack_hpUp.Draw("HIST");
  mc_stack_hpUp.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpUp.SaveAs((info.getPlotName()+"_hpUp_Linear.pdf").c_str());
  canvasTwo_hpUp.SaveAs((info.getPlotName()+"_hpUp_Linear.png").c_str());
  canvasTwo_hpUp.SaveAs((info.getPlotName()+"_hpUp_Linear.root").c_str());
  canvasTwo_hpUp.Clear();
  mc_stack_hpUp.SetMinimum(0.1);
  data_stack2_hpUp.SetMinimum(0.1);
  canvasTwo_hpUp.Divide(2,1);
  canvasTwo_hpUp.GetPad(1)->SetLogy();
  canvasTwo_hpUp.GetPad(2)->SetLogy();
  canvasTwo_hpUp.cd(1);
  data_stack2_hpUp.Draw("HIST");
  data_stack2_hpUp.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpUp.cd(2);
  mc_stack_hpUp.Draw("HIST");
  mc_stack_hpUp.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpUp.SaveAs((info.getPlotName()+"_hpUp_Log.pdf").c_str());
  canvasTwo_hpUp.SaveAs((info.getPlotName()+"_hpUp_Log.png").c_str());
  canvasTwo_hpUp.SaveAs((info.getPlotName()+"_hpUp_Log.root").c_str());

  TCanvas canvasTwo_hpDown((info.getPlotName()+"canvasTwo_hpDown").c_str(),info.getPlotTitle().c_str(),2048,1024);
  canvasTwo_hpDown.Divide(2,1);
  canvasTwo_hpDown.cd(1);
  data_stack2_hpDown.Draw("HIST");
  data_stack2_hpDown.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpDown.cd(2);
  mc_stack_hpDown.Draw("HIST");
  mc_stack_hpDown.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpDown.SaveAs((info.getPlotName()+"_hpDown_Linear.pdf").c_str());
  canvasTwo_hpDown.SaveAs((info.getPlotName()+"_hpDown_Linear.png").c_str());
  canvasTwo_hpDown.SaveAs((info.getPlotName()+"_hpDown_Linear.root").c_str());
  canvasTwo_hpDown.Clear();
  mc_stack_hpDown.SetMinimum(0.1);
  data_stack2_hpDown.SetMinimum(0.1);
  canvasTwo_hpDown.Divide(2,1);
  canvasTwo_hpDown.GetPad(1)->SetLogy();
  canvasTwo_hpDown.GetPad(2)->SetLogy();
  canvasTwo_hpDown.cd(1);
  data_stack2_hpDown.Draw("HIST");
  data_stack2_hpDown.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpDown.cd(2);
  mc_stack_hpDown.Draw("HIST");
  mc_stack_hpDown.GetXaxis()->SetTitle(info.getXTitle().c_str());
  legend2.Draw();
  canvasTwo_hpDown.SaveAs((info.getPlotName()+"_hpDown_Log.pdf").c_str());
  canvasTwo_hpDown.SaveAs((info.getPlotName()+"_hpDown_Log.png").c_str());
  canvasTwo_hpDown.SaveAs((info.getPlotName()+"_hpDown_Log.root").c_str());
  
  return;
}

void MakeACutPlot(const InformationCut& info, const FlavorHistogram& hists, double& scale, const double& intLumi, const int dataVsmc)
{
  //Turn 2D Plot into Integral Plot:
  TH2D* data_hist = static_cast<TH2D*>(hists.getDataHist());
  TH2D* mc_light_hist = static_cast<TH2D*>(hists.getMCHistLight());
  TH2D* mc_all_hist = static_cast<TH2D*>(hists.getMCHistAll());
  TH2D* mc_none_hist = static_cast<TH2D*>(hists.getMCHistNone());
  TH2D* mc_c_hist = static_cast<TH2D*>(hists.getMCHistC());
  TH2D* mc_b_hist = static_cast<TH2D*>(hists.getMCHistB());
  TH2D* mc_bglusplit_hist = static_cast<TH2D*>(hists.getMCHistBGluSplit());


  TH2D* data_int = (TH2D*)data_hist->Clone((info.getPlotName()+"_data_int").c_str());
  TH2D* mc_all_int = (TH2D*)mc_all_hist->Clone((info.getPlotName()+"_mc_all_int").c_str());

  vector<double> error2_data(info.getNBinsY()+2,0);
  vector<double> error2_mc_all(info.getNBinsY()+2,0);

  if(info.getDirection() == "greaterThan")
    {
      for(unsigned int iXbin = 0; iXbin<info.getNBinsX()+2; ++iXbin){
	for(unsigned int iYbin = 0; iYbin<info.getNBinsY()+2; ++iYbin){
	  data_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,data_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	  error2_data[iYbin]+=pow(data_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	  data_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));

	  mc_all_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,mc_all_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	  error2_data[iYbin]+=pow(mc_all_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	  mc_all_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));
	}
      }
    }

  if(info.getDirection() == "lessThan")
    {
      for(unsigned int iXbin = 0; iXbin<info.getNBinsX()+2; ++iXbin){
	for(unsigned int iYbin = 0; iYbin<info.getNBinsY()+2; ++iYbin){
	  data_int->SetBinContent(iXbin,iYbin,data_hist->Integral(0,iXbin,iYbin,iYbin));
	  error2_data[iYbin]+=pow(data_hist->GetBinError(iXbin,iYbin),2);
	  data_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));

	  mc_all_int->SetBinContent(iXbin,iYbin,mc_all_hist->Integral(0,iXbin,iYbin,iYbin));
	  error2_data[iYbin]+=pow(mc_all_hist->GetBinError(iXbin,iYbin),2);
	  mc_all_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));

	}
      }
    }

  if(scale==0)
    {
      //assume normalization to data
      scale = data_hist->Integral()/mc_all_hist->Integral();
    }   
  if(scale==-1)
    {
      //assume normalization to data
      scale = data_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1)/mc_all_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1);
    }    

  mc_all_int->Scale(scale);
  
  FlavorHistogram profiles;
  Information1D profInfo;
  profInfo.setDisplayNoInfo(info.getDisplayNoInfo());
  profInfo.setLegendPosition(info.getLegendPosition());

  TProfile* data_prof = data_int->ProfileX();
  TProfile* mc_all_prof = mc_all_int->ProfileX();
  data_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
  mc_all_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
  data_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
  mc_all_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
  profiles.setDataHist(data_prof);

  if(dataVsmc) {//dataVsmc comparison
    TH2D* mc_b_int = (TH2D*)mc_all_hist->Clone((info.getPlotName()+"_mc_b_int").c_str());
    TH2D* mc_bglusplit_int = (TH2D*)mc_all_hist->Clone((info.getPlotName()+"_mc_bglusplit_int").c_str());
    TH2D* mc_c_int = (TH2D*)mc_all_hist->Clone((info.getPlotName()+"_mc_c_int").c_str());
    TH2D* mc_light_int = (TH2D*)mc_all_hist->Clone((info.getPlotName()+"_mc_light_int").c_str());
    TH2D* mc_none_int = (TH2D*)mc_all_hist->Clone((info.getPlotName()+"_mc_none_int").c_str());
    
    vector<double> error2_mc_b(info.getNBinsY()+2,0);
    vector<double> error2_mc_bglusplit(info.getNBinsY()+2,0);
    vector<double> error2_mc_c(info.getNBinsY()+2,0);
    vector<double> error2_mc_light(info.getNBinsY()+2,0);
    vector<double> error2_mc_none(info.getNBinsY()+2,0);

    if(info.getDirection() == "greaterThan")
      {
	for(unsigned int iXbin = 0; iXbin<info.getNBinsX()+2; ++iXbin){
	  for(unsigned int iYbin = 0; iYbin<info.getNBinsY()+2; ++iYbin){
	    mc_b_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,mc_b_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_b_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	    mc_b_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));

	    mc_bglusplit_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,mc_bglusplit_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_bglusplit_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	    mc_bglusplit_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));
	    
	    mc_c_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,mc_c_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_c_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	    mc_c_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));
	    
	    mc_light_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,mc_light_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_light_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	    mc_light_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));
	    
	    mc_none_int->SetBinContent(info.getNBinsX()+1-iXbin,iYbin,mc_none_hist->Integral(info.getNBinsX()+1-iXbin,info.getNBinsX()+1,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_none_hist->GetBinError(info.getNBinsX()+1-iXbin,iYbin),2);
	    mc_none_int->SetBinError(info.getNBinsX()+1-iXbin,iYbin,sqrt(error2_data[iYbin]));
	  }
	}
      }
    
    if(info.getDirection() == "lessThan")
      {
	for(unsigned int iXbin = 0; iXbin<info.getNBinsX()+2; ++iXbin){
	  for(unsigned int iYbin = 0; iYbin<info.getNBinsY()+2; ++iYbin){
	    
	    mc_b_int->SetBinContent(iXbin,iYbin,mc_b_hist->Integral(0,iXbin,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_b_hist->GetBinError(iXbin,iYbin),2);
	    mc_b_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));

	    mc_bglusplit_int->SetBinContent(iXbin,iYbin,mc_bglusplit_hist->Integral(0,iXbin,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_bglusplit_hist->GetBinError(iXbin,iYbin),2);
	    mc_bglusplit_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));
	    
	    mc_c_int->SetBinContent(iXbin,iYbin,mc_c_hist->Integral(0,iXbin,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_c_hist->GetBinError(iXbin,iYbin),2);
	    mc_c_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));
	    
	    mc_light_int->SetBinContent(iXbin,iYbin,mc_light_hist->Integral(0,iXbin,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_light_hist->GetBinError(iXbin,iYbin),2);
	    mc_light_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));
	    
	    mc_none_int->SetBinContent(iXbin,iYbin,mc_none_hist->Integral(0,iXbin,iYbin,iYbin));
	    error2_data[iYbin]+=pow(mc_none_hist->GetBinError(iXbin,iYbin),2);
	    mc_none_int->SetBinError(iXbin,iYbin,sqrt(error2_data[iYbin]));
	  }
	}
      }
    
    mc_all_int->Add(mc_b_int);
    mc_all_int->Add(mc_bglusplit_int);
    mc_all_int->Add(mc_c_int);
    mc_all_int->Add(mc_light_int);
    mc_all_int->Add(mc_none_int);
  
    mc_all_hist->Add(mc_b_hist);
    mc_all_hist->Add(mc_bglusplit_hist);
    mc_all_hist->Add(mc_c_hist);
    mc_all_hist->Add(mc_light_hist);
    mc_all_hist->Add(mc_none_hist);

    //assume normalization to data
    if(scale==0) scale = data_hist->Integral()/mc_all_hist->Integral();
    //assume normalization to data
    if(scale==-1)  scale = data_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1)/mc_all_hist->Integral(0,info.getNBinsX()+1,0,info.getNBinsY()+1);

    mc_b_int->Scale(scale);
    mc_bglusplit_int->Scale(scale);
    mc_c_int->Scale(scale);
    mc_light_int->Scale(scale);
    mc_none_int->Scale(scale);


    TProfile* mc_b_prof = mc_b_int->ProfileX();
    TProfile* mc_bglusplit_prof = mc_bglusplit_int->ProfileX();
    TProfile* mc_c_prof = mc_c_int->ProfileX();
    TProfile* mc_light_prof = mc_light_int->ProfileX();
    TProfile* mc_none_prof = mc_none_int->ProfileX();
    mc_b_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_bglusplit_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_c_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_light_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_none_prof->SetTitle((info.getPlotTitle()+" : "+" "+info.getXTitle()+" Profile").c_str());
    mc_b_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_bglusplit_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_c_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_light_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
    mc_none_prof->GetYaxis()->SetTitle(info.getYTitle().c_str());
    profiles.setMCHistB(mc_b_prof);
    profiles.setMCHistBGluSplit(mc_bglusplit_prof);
    profiles.setMCHistC(mc_c_prof);
    profiles.setMCHistLight(mc_light_prof);
    profiles.setMCHistNone(mc_none_prof);
  
    //Make Canvases
    TCanvas mc_comp((info.getPlotName()+"mc_comp").c_str(),info.getPlotTitle().c_str(),2048,2048);
    mc_comp.Divide(2,2);
    mc_comp.cd(1);
    mc_b_int->SetTitle((info.getPlotTitle()+" : True b-jet").c_str());
    mc_b_int->Draw("colz");
    mc_comp.cd(2);
    mc_c_int->SetTitle((info.getPlotTitle()+" : True c-jet").c_str());
    mc_c_int->Draw("colz");
    mc_comp.cd(3);
    mc_light_int->SetTitle((info.getPlotTitle()+" : True light-jet").c_str());
    mc_light_int->Draw("colz");
    mc_comp.cd(4);
    mc_none_int->SetTitle((info.getPlotTitle()+" : No Truth Match").c_str());
    mc_none_int->Draw("colz");
    std::string mc_name = info.getPlotName()+"_flavorComp.pdf";
    mc_comp.SaveAs(mc_name.c_str());
    mc_name = info.getPlotName()+"_flavorComp.png";
    mc_comp.SaveAs(mc_name.c_str());
    mc_name = info.getPlotName()+"_flavorComp.root";
    mc_comp.SaveAs(mc_name.c_str());
  }

  profInfo.setPlotName(info.getPlotName()+"_profile");
  profInfo.setPlotTitle(info.getPlotTitle()+" : "+info.getXTitle()+" Profile");

  MakeAProfilePlot(profInfo, profiles, intLumi, dataVsmc);
  
  TCanvas data_comp((info.getPlotName()+"data_comp").c_str(),info.getPlotTitle().c_str(),2048,1024);
  data_comp.cd(1);
  data_comp.Divide(2,1);
  if(dataVsmc)  mc_all_int->SetTitle((info.getPlotTitle()+" : Monte Carlo Simulation").c_str());
  else          mc_all_int->SetTitle((info.getPlotTitle()+" : Data").c_str());
  mc_all_int->Draw("colz");
  data_comp.cd(2);
  data_int->SetTitle((info.getPlotTitle()+" : Data").c_str());
  data_int->Draw("colz");
  std::string data_name = info.getPlotName()+"_dataComp.pdf";
  data_comp.SaveAs(data_name.c_str());
  data_name = info.getPlotName()+"_dataComp.png";
  data_comp.SaveAs(data_name.c_str());
  data_name = info.getPlotName()+"_dataComp.root";
  data_comp.SaveAs(data_name.c_str());
}

void createOptions(po::options_description& desc)
{
  desc.add_options()
  ("help", "Produce help message and exit.")
  ("inputMCFiles", po::value<std::string>(), "Name of input file listing MC files with weights.")
  ("inputDataFiles", po::value<std::string>(), "Name of input file listing Data files with weights.")
  ("plotsFile", po::value<std::string>(), "Name of input file listing plots to be made.")
  ("finalNorm", po::value<double>(), "Normalization of plots (lumi norm. = 1, area norm. w/o overflow = 0, area norm w/ overflow = -1).")
  ("outputFile", po::value<std::string>(), "Name of output ROOT file [default = plots.root].")
  ("integratedLumi", po::value<double>(), "The integrated luminosity in the input data files.")
  ("dataVsMC", po::value<int>(), "1 = compare data to MC, 0 = compare data to data [default = 1].")
  ("plotName", po::value<std::string>(), "Name of plot to produce [leave unspecified to produce all plots]")
  ("firstPlot", po::value<int>(), "(w/ lastPlot) Denotes the range of plots in plotsFile to be produced [leave unspecified to start w/ first plot]")
  ("lastPlot", po::value<int>(), "(w/ firstPlot) Denotes the range of plots in plotsFile to be produced [leave unspecified to end w/ last plot]")
  ;
}

int main(int argc, char* argv[])
{
  std::cout << "Getting options...";
  po::options_description desc("Allowed options");
  createOptions(desc);

  po::variables_map vm;
  po::store(boost::program_options::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if(vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 1;
  }

  std::string mcFilesName;
  if(vm.count("inputMCFiles"))
  {
    mcFilesName = vm["inputMCFiles"].as<std::string>();
  }
  else
  {
    std::cout << "inputMCFiles not specified!  Exiting" << std::endl;
    return 1;
  }

  std::string dataFilesName;
  if(vm.count("inputDataFiles"))
  {
    dataFilesName = vm["inputDataFiles"].as<std::string>();
  }
  else
  {
    std::cout << "inputDataFiles not specified!  Exiting." << std::endl;
    return 1;
  }

  std::string plotsFileName;
  if(vm.count("plotsFile"))
  {
    plotsFileName = vm["plotsFile"].as<std::string>();
  }
  else
  {
    std::cout << "plotsFile not specified!  Exiting." << std::endl;
    return 1;
  }

  double intLumi = 0.0;
  if(vm.count("integratedLumi"))
  {
    intLumi = vm["integratedLumi"].as<double>();
  }
  else
  {
    std::cout << "integratedLumi not specified!  Exiting." << std::endl;
    return 1;
  }

  double finalNorm = vm.count("finalNorm") ? vm["finalNorm"].as<double>() : -1;
  std::string rootFile(vm.count("outputFile") ? vm["outputFile"].as<std::string>() : "plots.root");
  int dataVsmc = vm.count("dataVsMC") ? vm["dataVsMC"].as<int>() : 1;
  std::string doPlot( vm.count("plotName") ? vm["plotName"].as<std::string>() : "");
  int firstPlot = vm.count("firstPlot") ? vm["firstPlot"].as<int>() : 0;
  int lastPlot = vm.count("lastPlot") ? vm["lastPlot"].as<int>() : 0;

  std::cout << "done." << std::endl;

  TFile* theFile = TFile::Open(rootFile.c_str(),"RECREATE");

  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);

  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(kWhite);
  gStyle->SetPadColor(kWhite);
  gStyle->SetStatBorderSize(1);
  gStyle->SetStatColor(kWhite);
  gStyle->SetTitleFillColor(0);
  gStyle->SetFrameFillColor(10);
  gStyle->SetCanvasColor(kWhite); 
  //  gStyle->SetMarkerStyle(6);
  //  gStyle->SetMarkerSize(1);
  gStyle->SetMarkerColor(1);
  gStyle->SetOptStat(000000000);
  gStyle->SetOptTitle(kFALSE);

  setTDRStyle();
  ifstream mcFiles(mcFilesName.c_str());
  ifstream dataFiles(dataFilesName.c_str());
  ifstream plotFiles(plotsFileName.c_str());

  std::cout << "Creating selector...";
  TSelectorMultiDraw* selector = new TSelectorMultiDraw();
  std::cout << "done." << std::endl;

  //  TString normalizationText = "";
  //  if(finalNorm<=0) normalizationText = "MC normalized to Data";
  //  else normalizationText = "normalized to Luminosity";

  std::cout << "Looping over plot files...";
  int plotCounter=0;
  bool foundPlot = false;
  while (! plotFiles.eof()) {

    string line;
    getline (plotFiles,line);
    if (line[0] == '#') continue;
    if(line.find("plot_type")==string::npos) continue;
    
    // found plot type increase plot counter
    plotCounter++;
    // check if plot is in the range of plots to be plotted
    if(firstPlot <=0 && lastPlot <=0) {}// no plot selection continue as usual
    else if( plotCounter < firstPlot) continue; // skip plot if it is outside of range
    else if(plotCounter > lastPlot ) break; // leave loop once you have found the last plot 

    switch(getPlotTypeByName(getEntryAndValue(line).second)) {
      case jetPlots1D:
      {
        Information1D thisPlot = param1d(plotFiles);
        thisPlot.setPlotType("jetPlots1D");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
          selector->AddGenericSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case trackPlots_flavorStack:
      {
        Information1D thisPlot = param1d(plotFiles);
        thisPlot.setPlotType("trackPlots_flavorStack");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddGenericTrackSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case trackPlots_qualityStack:
      {
        InformationQuality thisPlot = paramQuality(plotFiles);
        thisPlot.setPlotType("trackPlots_qualityStack");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
          selector->AddGenericTrackQualitySelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case jetTrackQualityPlots:
      {
        InformationQuality thisPlot = paramQuality(plotFiles);
        thisPlot.setPlotType("jetTrackQualityPlots");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
          selector->AddGenericJetTrackQualitySelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case effVsCutPlot:
      {
        Information1D thisPlot = param1d(plotFiles);
        thisPlot.setPlotType("effVsCutPlot");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddGenericSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case trackCutPlot:
      {
        InformationTrackCuts thisPlot = paramTrackCuts(plotFiles);
        thisPlot.setPlotType("trackCutPlot");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddTrackSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case muonCutPlot:
      {
        InformationMuonCuts thisPlot = paramMuonCuts(plotFiles);
        thisPlot.setPlotType("muonCutPlot");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddMuonSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case cutCompPlots:
      {
        InformationCutComp thisPlot = paramCutComp(plotFiles);
        thisPlot.setPlotType("cutCompPlots");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
          selector->AddCutCompSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
	    }
        break;
      }
      case ptHatPlots:
      {
        InformationPtHat thisPlot = paramPtHat(plotFiles);
        thisPlot.setPlotType("ptHatPlots");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
          selector->AddPtHatSelector(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case jetPlots2D:
      {
        Information2D thisPlot = param2d(plotFiles);
        thisPlot.setPlotType("jetPlots2D");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddGenericSelector2D(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case reweightedPlot:
      {
        Information2D thisPlot = param2d(plotFiles);
        thisPlot.setPlotType("reweightedPlot");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddGenericSelector2D(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case reweightedTrackPlot:
      {
        Information2D thisPlot = param2d(plotFiles);
        thisPlot.setPlotType("reweightedTrackPlot");
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddGenericTrackSelector2D(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      case cutPlots:
      {
        InformationCut thisPlot = paramCut(plotFiles);
        if(doPlot.empty() || doPlot == thisPlot.getPlotName()){
	      selector->AddGenericSelector2D(thisPlot);
          if(!doPlot.empty()) foundPlot = true;
        }
        break;
      }
      default:
        std::cout << "Invalid option!  Aborting." << std::endl;
        abort();
    }
    if(foundPlot) break;
  }
  plotFiles.close();
  std::cout << "done." << std::endl;

  selector->SetIsData(false);
  std::cout << "Looping over input files..." << std::endl;
  while (! mcFiles.eof()) {
    std::string line;
    getline (mcFiles,line);
    if (line[0] == '#') continue;
    size_t position = line.find(",");
    if(position == std::string::npos) continue;
    std::string thisFileName = line.substr(0,position);
    double thisWeight = atof(line.substr(position+1).c_str());
    std::cout<<"opening file " << thisFileName << std::endl;
    TFile* thisFile = TFile::Open(thisFileName.c_str());
    TTree* thisTree = (TTree*)thisFile->Get("t");
    thisTree->SetWeight(thisWeight);
    thisTree->Process(selector, "", thisTree->GetEntries(), 0);
    thisFile->Close("r");
    delete thisFile;
  }
  mcFiles.close();

  selector->SetIsData(true);
  while (! dataFiles.eof()) {
    std::string line;
    getline (dataFiles,line);
    if (line[0] == '#') continue;
    size_t position = line.find(",");
    if(position == std::string::npos) continue;
    std::string thisFileName = line.substr(0,position);
    double thisWeight = atof(line.substr(position+1).c_str());
    std::cout<<"opening file " << thisFileName << std::endl;
    TFile* thisFile = TFile::Open(thisFileName.c_str());
    TTree* thisTree = (TTree*)thisFile->Get("t");
    thisTree->SetWeight(thisWeight);
    thisTree->Process(selector, "", thisTree->GetEntries(), 0);
    thisFile->Close("r");
    delete thisFile;
  }
  dataFiles.close();
  std::cout << "done." << std::endl;

  theFile->cd();

  const std::vector<BaseSelector*>& selectorVector = selector->getSelectors();
  for(std::vector<BaseSelector*>::const_iterator iter = selectorVector.begin();
      iter != selectorVector.end(); ++iter)
  {
    if(const GenericSelector* generic = dynamic_cast<const GenericSelector*>(*iter))
    {
      const std::string& plotType = generic->getInfo().getPlotType();
      if(plotType == "jetPlots1D")
        MakeAFlavorPlot(generic->getInfo(), generic->getHistograms(), finalNorm, intLumi, dataVsmc);
      else if(plotType == "effVsCutPlot")
        MakeAEffVsCutPlot(generic->getInfo(), generic->getHistograms(), finalNorm, intLumi, dataVsmc);
    }
    else if(const GenericTrackSelector* genTrack = dynamic_cast<const GenericTrackSelector*>(*iter))
    {
      MakeAFlavorPlot(genTrack->getInfo(), genTrack->getHistograms(), finalNorm, intLumi, dataVsmc);
    }
    else if(const GenericTrackQualitySelector* genTrackQual = dynamic_cast<const GenericTrackQualitySelector*>(*iter))
    {
      MakeATrackQualityPlot(genTrackQual->getInfo(), genTrackQual->getHistograms(), finalNorm);
    }
    else if(const GenericJetTrackQualitySelector* genJetTrack = dynamic_cast<const GenericJetTrackQualitySelector*>(*iter))
    {
      MakeATrackQualityPlot(genJetTrack->getInfo(), genJetTrack->getHistograms(), finalNorm);
    }
    else if(const GenericSelector2D* generic2D = dynamic_cast<const GenericSelector2D*>(*iter))
    {
      const std::string& plotType = generic2D->getInfo().getPlotType();
      if(plotType == "jetPlots2D")
        MakeA2DPlot(generic2D->getInfo(), generic2D->getHistograms(), finalNorm, intLumi, dataVsmc);
      else if(plotType == "reweightedPlot")
        MakeAReweightedPlot(generic2D->getInfo(), generic2D->getHistograms(), finalNorm, intLumi, dataVsmc);
      //else if(plotType == "cutPlots")
      //  MakeACutPlot(generic2D->getInfo(), generic2D->getHistograms(), finalNorm, intLumi, dataVsmc);
    }
    else if(const GenericTrackSelector2D* genTrack2D = dynamic_cast<const GenericTrackSelector2D*>(*iter))
    {
      MakeAReweightedPlot(genTrack2D->getInfo(), genTrack2D->getHistograms(), finalNorm, intLumi, dataVsmc);
    }
    else if(const TrackSelector* track = dynamic_cast<const TrackSelector*>(*iter))
    {
      MakeAFlavorPlot(track->getInfo(), track->getHistograms(), finalNorm, intLumi, dataVsmc);
    }
    else if(const MuonSelector* muon = dynamic_cast<const MuonSelector*>(*iter))
    {
      MakeAFlavorPlot(muon->getInfo(), muon->getHistograms(), finalNorm, intLumi, dataVsmc);
    }
    else if(const CutCompSelector* cutComp = dynamic_cast<const CutCompSelector*>(*iter))
    {
      MakeACutCompPlot(cutComp->getInfo(), cutComp->getDataCutCompInfoHist(), cutComp->getMCCutCompInfoHist());
    }
    else if(const PtHatSelector* ptHat = dynamic_cast<const PtHatSelector*>(*iter))
    {
      const std::vector<TH1D*>& mcHists = ptHat->getMCHists();
      MakeAPtHatPlot(ptHat->getInfo(), mcHists, ptHat->getDataHist(), ptHat->getPtHatHist(), finalNorm);
    }
  }

  theFile->Write();
  theFile->Close();
  delete theFile;

  return 0;
}

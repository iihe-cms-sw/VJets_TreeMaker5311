#include "drawAll.C"
#include <fstream>
#include <sstream>

/*
Taggername      WPname WPDiscrcut
SimpleSVHighEff SSVM 1.74
SimpleSVHighEff SSVT 3.05
  
SimpleSVHighPur SSVM 2.0
SimpleSVHighPur SSVT 2.0
  
TrackCountingHighEff TCHEM 3.3
TrackCountingHighEff TCHET 10.2
  
TrackCountingHighPur TCHPM 1.93
TrackCountingHighPur TCHPT 3.41
  
JetProbability JPM 0.459
JetProbability JPT 0.669
  
JetBProbability JBM 1.83
JetBProbability JBT 1.95
*/

float Medium[] = {1.74, 2.0, 3.3, 1.93, 0.459, 1.83};
float Tight[] = {3.05, 2.0, 10.2, 3.41, 0.669, 1.95};

void getEff_Mistag()
{
  TFile *_file1 = TFile::Open("BTAG_DQM.root");
  
  vector<string> algos;
  algos.push_back("standardSimpleSecondaryVertexHighEffPFBJetTags");
  algos.push_back("standardSimpleSecondaryVertexHighPurPFBJetTags");
  algos.push_back("standardTrackCountingHighEffPFBJetTags");
  algos.push_back("standardTrackCountingHighPurPFBJetTags");
  algos.push_back("standardJetProbabilityPFBJetTags");
  algos.push_back("standardJetBProbabilityPFBJetTags");

  vector<TFile *> files;
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);
  files.push_back(_file1);

  vector<string> lname;
  lname.push_back("SSVHE");
  lname.push_back("SSVHP");
  lname.push_back("TCHE");
  lname.push_back("TCHP");
  lname.push_back("JP");
  lname.push_back("JBP");

  // ... summary file ..
  ofstream fout("eff_mistag_summary.txt");

  for(int i = 0 ; i < algos.size() ; i++)
    {
      cout << algos[i] << endl;

      ///////////////////////////Eff b
      TH1F *effb = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","B");
      effb->GetYaxis()->SetTitle("b-efficiency");

      ///////////////////////////Mistag rate
      TH1F *efflight = (TH1F *)getCutHistogram(files[i],algos[i],"effVsDiscrCut_discr","GLOBAL","DUSG");
      efflight->GetYaxis()->SetTitle("non b-efficiency");

      cout << lname[i] << " Medium WP " << Medium[i] << " Effb: " << effb->GetBinContent(effb->FindBin(Medium[i])) << endl;
      cout << lname[i] << " Medium WP " << Medium[i] << " Mistag: " << efflight->GetBinContent(efflight->FindBin(Medium[i])) << endl;

      fout << lname[i] << " Medium WP " << Medium[i] 
	   << " Effb: " << effb->GetBinContent(effb->FindBin(Medium[i]))
	   << " Mistag: " << efflight->GetBinContent(efflight->FindBin(Medium[i])) << endl;
      //
      //
      cout << lname[i] << " Tight WP " << Tight[i] << "  Effb: " << effb->GetBinContent(effb->FindBin(Tight[i])) << endl;
      cout << lname[i] << " Tight WP " << Tight[i] << " Mistag: " << efflight->GetBinContent(efflight->FindBin(Tight[i])) << endl;

      fout << lname[i] << " Tight WP " << Tight[i] 
	   << "  Effb: " << effb->GetBinContent(effb->FindBin(Tight[i]))
	   << " Mistag: " << efflight->GetBinContent(efflight->FindBin(Tight[i])) << endl;

    }

}

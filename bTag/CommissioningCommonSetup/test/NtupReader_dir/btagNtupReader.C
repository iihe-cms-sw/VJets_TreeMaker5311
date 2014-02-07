/////////////////////////////////////////////////////////////////
// Author : Caroline Collard (IPHC) - first commit : april 2011
//          Jinzhong Zhang (NEU,USA) - change for Run2011B and readability, and so on : Nov 2011
////////////////////////////////////////////////////////////////
#include "Configuration.h"
//the primary vertex divisions will be (0,FIRST_NPV-1) (FIRST_NPV,SECOND_NPV) (SECOND_NPV+1,MAXPU)
#define FIRST_NPV 7
#define SECOND_NPV 10
//########################################################################
#include "btagNtupReader.h"
#include <TH1.h>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <math.h>
#include <cstring>

Float_t bEnrichCutSal=1.0;

// TO BE DEFINED FIRST !

#if PTVAL == 30 && defined(RUN_ON_JET)
TString my_trigger_path="HLT_PFJet40";Float_t JetPtthresholdsfornmu=-1; Float_t cutJetPt=30.;
#endif

#if PTVAL == 60 && defined(RUN_ON_JET)
TString my_trigger_path="HLT_PFJet80";Float_t JetPtthresholdsfornmu=-1; Float_t cutJetPt=60.;
#endif

#if PTVAL == 100 && defined(RUN_ON_JET)
TString my_trigger_path="HLT_PFJet80";Float_t JetPtthresholdsfornmu=-1; Float_t cutJetPt=100.;
#endif

#if PTVAL == 120 && defined(RUN_ON_JET)
TString my_trigger_path="HLT_PFJet80";Float_t JetPtthresholdsfornmu=-1; Float_t cutJetPt=120.;
#endif

#if PTVAL == 140 && defined(RUN_ON_JET)
TString my_trigger_path="HLT_PFJet140";Float_t JetPtthresholdsfornmu=-1; Float_t cutJetPt=140.;
#define MaxJetPt 1000
#endif

#if PTVAL == 20 && defined(RUN_ON_BTAG)
TString my_trigger_path="HLT_BTagMu_DiJet20_Mu5";Float_t JetPtthresholdsfornmu=30.; Float_t cutJetPt=30.; Float_t cutMuonPt=5.;
#endif

#if PTVAL == 40 && defined(RUN_ON_BTAG)
TString my_trigger_path="HLT_BTagMu_DiJet40_Mu5";Float_t JetPtthresholdsfornmu=60.; Float_t cutJetPt=65.; Float_t cutMuonPt=7.;
#endif


#if PTVAL == 70 && defined(RUN_ON_BTAG)
TString my_trigger_path="HLT_BTagMu_DiJet70_Mu5";Float_t JetPtthresholdsfornmu=100; Float_t cutJetPt=105.; Float_t cutMuonPt=7.;
#endif

#if PTVAL == 110 && defined(RUN_ON_BTAG)
TString my_trigger_path="HLT_BTagMu_DiJet110_Mu5";Float_t JetPtthresholdsfornmu=120;/*130?*/ Float_t cutJetPt=120.; Float_t cutMuonPt=5.;
#endif


class Plotter : public btagNtupReader{
public:
  Plotter(string filename):btagNtupReader(filename){};
  virtual void     Loop(Bool_t isRealData, Float_t weight, const string resultfilename);
  virtual void     AddHisto(vector<TH1F*> &HistoBtag, string name,  const int& j, string title,  const int& nbins, const Float_t& min, const Float_t& max);
  virtual void     AddHisto2D(vector<TH2F*> &Histo2DB, string name,  const int& j, string title, const int& nbins, const Float_t& min, const Float_t& max, const int& nbinsy, const Float_t& miny, const Float_t& maxy);
  virtual void PileUpDistribution(TH1D* dis) {
     if (fChain == 0) return;
     Long64_t nentries = fChain->GetEntries();//GetEntriesFast() will give you something wrong in the first running
     for (Long64_t jentry=0; jentry<nentries;jentry++) {
       if ( LoadTree(jentry) < 0 ) break;
       fChain->GetEntry(jentry);
       
       dis->Fill(numberOfPUVerticesMixingTruth);
       //dis->Fill(numberOfPUVertices);
     }
  }
};

Float_t puweight[MAXPU+1];
TString final_dir;

#if !defined(npu_probs) || !defined(pudistribution_data_filename)
void predefinedpuweights();
#endif

void Plotter::Loop(const Bool_t isRealData, const Float_t weightsave, const string resultfilename)
{
  if (fChain == 0) return;
  vector<TH1F*> HistoBtag;
  vector<TH2F*> Histo2DB;
  double pi=acos(-1);

  int jmax=1;
  int nhisto_to_clone = 0;
  int nhisto_to_clone2D = 0;
  if (!isRealData) jmax=5;

  // HISTOGRAM DEFINITIONS 
  // FOR DATA : only 1 histo
  // FOR MC : all, b, c, udsg, splitting
  for (int j=0; j<jmax; j++) {

    //   TEMPLATE :
    //   AddHisto(string name, int j, string title, string xaxis, const int& nbins, const Float_t& min, const Float_t& max)

    // general information on the jets
    AddHisto(HistoBtag, "all_jetpt",       j,"PT of all jets",80,0.,400.);   //0

    // discri
    AddHisto(HistoBtag, "discri_tche",   j,"TCHE Discriminator",50,0.,30.);  // 1
    AddHisto(HistoBtag, "discri_tchp",   j,"TCHP Discriminator",50,0.,30.);  // 2

    AddHisto(HistoBtag, "discri_ssche0",   j,"SSVHE Discriminator",80,-1.,7.); // 3
    AddHisto(HistoBtag, "discri_ssche",   j,"SSVHE Discriminator",50,0.,7.);   // 4
    AddHisto(HistoBtag, "discri_sschp0",   j,"SSVHP Discriminator",80,-1.,7.); // 5
    AddHisto(HistoBtag, "discri_sschp",   j,"SSVHP Discriminator",50,0.,7.);   

    AddHisto(HistoBtag, "discri_jetprob",   j,"JetProb Discriminator",50,0.,2.5);
    AddHisto(HistoBtag, "discri_jetbprob",   j,"JetBProb Discriminator",50,0.,8.);

    // track info : 
    AddHisto(HistoBtag, "n_tracks_jet",   j,"# of tracks in jet",40,-0.5,39.5);
    AddHisto(HistoBtag, "pt_tracks",   j,"Pt of tracks",80,0.,200.);    // 10
    AddHisto(HistoBtag, "n_seltracks_jet",   j,"# of sel tracks in jet",40,-0.5,39.5);
    AddHisto(HistoBtag, "pt_seltracks",   j,"Pt of  sel tracks",80,0.,200.);

    AddHisto(HistoBtag, "trackNHits",  j,"Number of hits",35,-0.5, 34.5);
    AddHisto(HistoBtag, "trackNPixelHits",  j,"number of pixel hits",10,-0.5, 9.5);
    AddHisto(HistoBtag, "trackNormChi2",  j,"Normalized #chi^{2}",100,0.,30.); // 15
    AddHisto(HistoBtag, "trackTransverseMomentum",  j,"Track p_{T}",150,0.,15.);
    AddHisto(HistoBtag, "trackLongitudinalIP",  j,"Longitudinal IP",100,-20.,20.);
    AddHisto(HistoBtag, "trackIP2d",  j,"Transverse IP",200,-1.,1.);
    AddHisto(HistoBtag, "trackDistJetAxis",  j,"Track jet axis distance",100,0.,0.3);
    AddHisto(HistoBtag, "trackDecayLength",  j,"Track decay length",100,0,25.); //20

    AddHisto(HistoBtag, "trackNHits_cut",  j,"Number of hits",35,-0.5, 34.5);
    AddHisto(HistoBtag, "trackNPixelHits_cut",  j,"number of pixel hits",10,-0.5, 9.5);
    AddHisto(HistoBtag, "trackNormChi2_cut",  j,"Normalized #chi^{2}",100,0.,30.);
    AddHisto(HistoBtag, "trackTransverseMomentum_cut",  j,"Track p_{T}",150,0.,15.);
    AddHisto(HistoBtag, "trackLongitudinalIP_cut",  j,"Longitudinal IP",100,-20.,20.);// 25
    AddHisto(HistoBtag, "trackIP2d_cut",  j,"Transverse IP",200,-1.,1.);
    AddHisto(HistoBtag, "trackDistJetAxis_cut",  j,"Track jet axis distance",100,0.,0.3);
    AddHisto(HistoBtag, "trackDecayLength_cut",  j,"Track decay length",100,0,25.);

    AddHisto(HistoBtag, "trackIP3d",  j,"Track 3D IP value",100,-0.1,0.1);
    AddHisto(HistoBtag, "trackIP3dError",  j,"Track 3D IP Error",100,0.,0.1); //30
    AddHisto(HistoBtag, "trackIP3dsig",  j,"Track 3D IP significance",100,-35.,35.);
    AddHisto(HistoBtag, "trackIP3d_sel",  j,"Track 3D IP value",100,-0.1,0.1);
    AddHisto(HistoBtag, "trackIP3dError_sel",  j,"Track 3D IP Error",100,0,0.1);
    AddHisto(HistoBtag, "trackIP3dsig_sel",  j,"Track 3D IP significance",100,-35.,35.);
    AddHisto(HistoBtag, "trackIP3d_cutsel",  j,"Track 3D IP value",100,-0.1,0.1); //35
    AddHisto(HistoBtag, "trackIP3dError_cutsel",  j,"Track 3D IP Error",100,0.,0.1);
    AddHisto(HistoBtag, "trackIP3dsig_cutsel",  j,"Track 3D IP significance",100,-35.,35.);

    // IP  : first 3 tracks
    AddHisto(HistoBtag, "IP3d1",j,"1st track 3D IP value",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d1sig",j,"1st track 3D IP significance",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d1sigz",j,"1st track 3D IP significance",100,-2.,2.); //40
    AddHisto(HistoBtag, "IP3d2",j,"2nd track 3D IP value",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d2sig",j,"2nd track 3D IP significance",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigz",j,"2nd track 3D IP significance",100,-2.,2.); 
    AddHisto(HistoBtag, "IP3d3",j,"3rd track 3D IP value",100,-0.1,0.1); 
    AddHisto(HistoBtag, "IP3d3sig",j,"3rd track 3D IP significance",100,-35.,35.); //45
    AddHisto(HistoBtag, "IP3d3sigz",j,"3rd track 3D IP significance",100,-2.,2.);
 
    AddHisto(HistoBtag, "IP3d1sorted",j,"1st track 3D IP value (sorted)",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d1sigsorted",j,"1st track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d1sigzsorted",j,"1st track 3D IP significance (sorted)",100,-2.,2.);
    AddHisto(HistoBtag, "IP3d2sorted",j,"2nd track 3D IP value (sorted)",100,-0.1,0.1); //50
    AddHisto(HistoBtag, "IP3d2sigsorted",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigzsorted",j,"2nd track 3D IP significance (sorted)",100,-2.,2.);
    AddHisto(HistoBtag, "IP3d3sorted",j,"3rd track 3D IP value (sorted)",100,-0.1,0.1); 
    AddHisto(HistoBtag, "IP3d3sigsorted",j,"3rd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d3sigzsorted",j,"3rd track 3D IP significance (sorted)",100,-2.,2.);//55

    // SV : 

    AddHisto(HistoBtag, "sv_nvertices0", j,"# of secondary vertices",6,-0.5,5.5);
    AddHisto(HistoBtag, "sv_nvertices", j,"# of secondary vertices",6,-0.5,5.5);
    AddHisto(HistoBtag, "sv_jetpt",       j,"PT of jet",80,0.,400.);
    AddHisto(HistoBtag, "sv_normchi2",    j,"Norm Chi2",50,0.,10.);

    AddHisto(HistoBtag, "sv_mass",    j,"SV mass",50,0.,8.); // 60
    AddHisto(HistoBtag, "sv_mass_3tr", j,"SV mass for >=3tracks",50,0.,8.);

    AddHisto(HistoBtag, "sv_trackmul",  j,"Track multiplicity : SVnVertexTracks",24,0.25,12.25);
    AddHisto(HistoBtag, "sv_trackmul1",  j,"Track multiplicity : SVnFirstVertexTracks",10,0,10);
    AddHisto(HistoBtag, "sv_tracksel",  j,"Track multiplicity : SVnSelectedTracks",20,0,20);

    AddHisto(HistoBtag, "sv_ipsigcharm",  j,"IP significance 2D charm",50,-35.,35.); //65
    AddHisto(HistoBtag, "sv_eratio",    j,"Fractional energy",50,0.,1.);

    AddHisto(HistoBtag, "sv_flightsig2d", j,"Flight significance 2D",50,0.,80.);
    AddHisto(HistoBtag, "sv_flightsig3d", j,"Flight significance 3D",50,0.,80.);
    AddHisto(HistoBtag, "sv_flightdis2d", j,"Flight distance 2D",50,0.,2.5);
    AddHisto(HistoBtag, "sv_flightdis3d", j,"Flight distance 3D",50,0.,15.); //70
    AddHisto(HistoBtag, "sv_flightsig3d_3tr", j,"Flight significance 3D for >=3tracks",50,0.,80.);

    AddHisto(HistoBtag, "sv_deltar_jet",    j,"SVjetDeltaR",50,0.,0.5);
    AddHisto(HistoBtag, "sv_deltar_sum_jet",    j,"SVvtxSumJetDeltaR",50,0.,0.5);
    AddHisto(HistoBtag, "sv_deltar_sum_dir",    j,"SVvtxSumVtxDirDeltaR",50,0.,0.5);

    AddHisto(HistoBtag, "sv_vtxsum_eta",    j,"SVvtxSuDATA",52,-2.6,2.6); //75
    AddHisto(HistoBtag, "sv_vtxsum_phi",    j,"SVvtxSumPhi",50,-1.*pi,pi);
    AddHisto(HistoBtag, "sv_vtxdir_eta",    j,"SVvtxDirEta",52,-2.6,2.6);
    AddHisto(HistoBtag, "sv_vtxdir_phi",    j,"SVvtxDirPhi",50,-1.*pi,pi);

    AddHisto(HistoBtag, "sv_vtxsum_ptrel",  j,"SVvtxSumPTrel",100,0.,10.);
    AddHisto(HistoBtag, "sv_vtxdir_ptrel",  j,"SVvtxDirPTrel",100,0.,10.); //80
    AddHisto(HistoBtag, "sv_totcharge",     j,"Total charge",21,-10.5,10.5);
    AddHisto(HistoBtag, "sv_vtx_pt",        j,"PT of vtx",50,0.,100.);
    

    // muon in jets : for all jets
    AddHisto(HistoBtag, "muons_multiplicity", j,"# of muons in jet",7,-0.5,6.5);
    AddHisto(HistoBtag, "muon_Pt", j, "Muon p_{T}",200,0,100);
    AddHisto(HistoBtag, "muon_ptrel", j , "Muon p_{T}^{rel}",50,0,5); //85
    AddHisto(HistoBtag, "muon_Ip3d", j, "Muon 3D IP",50,-0.1,0.1);
    AddHisto(HistoBtag, "muon_Ip2d", j, "Muon 2D IP",50,-0.1,0.1);
    AddHisto(HistoBtag, "muon_Sip3d", j, "Muon 3D IP significance",50,-35,35);
    AddHisto(HistoBtag, "muon_Sip2d", j, "Muon 2D IP significance",50,-35,35);
    AddHisto(HistoBtag, "muon_DeltaR", j,"Muon1 deltaR",50,0,0.5); //90
    AddHisto(HistoBtag, "muon_Ratio", j,"Muon1 energy ratio",50,0.,1.4);

    // additional plots : with #PV selection
    // #PV: 1-6
    AddHisto(HistoBtag, "npv1_IP3d2sig0",        j,"2nd sorted track 3D IP significance (1-6 PVs)",100,-35.,35.);
    AddHisto(HistoBtag, "npv1_IP3d3sig0",        j,"3rd sorted track 3D IP significance (1-6 PVs)",100,-35.,35.);
    AddHisto(HistoBtag, "npv1_ntracks_jet",      j,"# of tracks in jet (1-6 PVs)",                 45,-0.5,44.5);
    AddHisto(HistoBtag, "npv1_nseltracks_jet",   j,"# of sel tracks in jet (1-6 PVs)",              45,-0.5,44.5); //95
    AddHisto(HistoBtag, "npv1_ncutseltracks_jet",j,"# of cut sel tracks in jet (1-6 PVs)",          45,-0.5,44.5);
    AddHisto(HistoBtag, "npv1_pt_tracks",        j,"Pt of tracks (1-6 PVs)",                        80,0.,200.);
    AddHisto(HistoBtag, "npv1_pt_seltracks",     j,"Pt of  sel tracks (1-6 PVs)",                   80,0.,200.);
    AddHisto(HistoBtag, "npv1_pt_cutseltracks",  j,"Pt of  cut sel tracks (1-6 PVs)",               80,0.,200.);

    AddHisto(HistoBtag, "npv1_nsv0",              j,"# of secondary vertices (1-6 PVs)",             6 ,-0.5,5.5); //100
    AddHisto(HistoBtag, "npv1_nsv",              j,"# of secondary vertices (1-6 PVs)",              6,-0.5,5.5);
    AddHisto(HistoBtag, "npv1_flightsig3d",      j,"Flight Significance 3D (1-6 PVs)",                  50,0.,100.);
    AddHisto(HistoBtag, "npv1_flightsig3d_3tr",  j,"Flight Significance 3D for >=3tracks (1-6 PVs)",    50,0.,100.);
    AddHisto(HistoBtag, "npv1_svmass",           j,"SV mass (1-6 PVs)",                             50,0.,8.);
    AddHisto(HistoBtag, "npv1_svmass_3tr",       j,"SV mass for >=3tracks (1-6 PVs)",               50,0.,8.); //105

    // #PV: 7-10
    AddHisto(HistoBtag, "npv2_IP3d2sig0",        j,"2nd sorted track 3D IP significance (7-10 PVs)",100,-35.,35.);
    AddHisto(HistoBtag, "npv2_IP3d3sig0",        j,"3rd sorted track 3D IP significance (7-10 PVs)",100,-35.,35.);
    AddHisto(HistoBtag, "npv2_ntracks_jet",      j,"# of tracks in jet (7-10 PVs)",                 45,-0.5,44.5);
    AddHisto(HistoBtag, "npv2_nseltracks_jet",   j,"# of sel tracks in jet (7-10 PVs)",              45,-0.5,44.5); 
    AddHisto(HistoBtag, "npv2_ncutseltracks_jet",j,"# of cut sel tracks in jet (7-10 PVs)",          45,-0.5,44.5);//110
    AddHisto(HistoBtag, "npv2_pt_tracks",        j,"Pt of tracks (7-10 PVs)",                        80,0.,200.);
    AddHisto(HistoBtag, "npv2_pt_seltracks",     j,"Pt of  sel tracks (7-10 PVs)",                   80,0.,200.);
    AddHisto(HistoBtag, "npv2_pt_cutseltracks",  j,"Pt of  cut sel tracks (7-10 PVs)",               80,0.,200.);

    AddHisto(HistoBtag, "npv2_nsv0",              j,"# of secondary vertices (7-10 PVs)",              6,-0.5,5.5);
    AddHisto(HistoBtag, "npv2_nsv",              j,"# of secondary vertices (7-10 PVs)",              6,-0.5,5.5); //115
    AddHisto(HistoBtag, "npv2_flightsig3d",      j,"Flight Significance 3D (7-10 PVs)",                  50,0.,100.);
    AddHisto(HistoBtag, "npv2_flightsig3d_3tr",  j,"Flight Significance 3D for >=3tracks (7-10 PVs)",    50,0.,100.);
    AddHisto(HistoBtag, "npv2_svmass",           j,"SV mass (7-10 PVs)",                             50,0.,8.);
    AddHisto(HistoBtag, "npv2_svmass_3tr",       j,"SV mass for >=3tracks (7-10 PVs)",               50,0.,8.);

    // #PV: >=11
    AddHisto(HistoBtag, "npv3_IP3d2sig0",        j,"2nd sorted track 3D IP significance (#PV#geq11)",100,-35.,35.); //120
    AddHisto(HistoBtag, "npv3_IP3d3sig0",        j,"3rd sorted track 3D IP significance (#PV#geq11)",100,-35.,35.);
    AddHisto(HistoBtag, "npv3_ntracks_jet",      j,"# of tracks in jet (#PV#geq11)",                 45,-0.5,44.5);
    AddHisto(HistoBtag, "npv3_nseltracks_jet",   j,"# of sel tracks in jet (#PV#geq11)",              45,-0.5,44.5);
    AddHisto(HistoBtag, "npv3_ncutseltracks_jet",j,"# of cut sel tracks in jet (#PV#geq11)",          45,-0.5,44.5); 
    AddHisto(HistoBtag, "npv3_pt_tracks",        j,"Pt of tracks (#PV#geq11)",                        80,0.,200.); //125
    AddHisto(HistoBtag, "npv3_pt_seltracks",     j,"Pt of  sel tracks (#PV#geq11)",                   80,0.,200.);
    AddHisto(HistoBtag, "npv3_pt_cutseltracks",  j,"Pt of  cut sel tracks (#PV#geq11)",               80,0.,200.);

    AddHisto(HistoBtag, "npv3_nsv0",              j,"# of secondary vertices (#PV#geq11)",              6,-0.5,5.5); 
    AddHisto(HistoBtag, "npv3_nsv",              j,"# of secondary vertices (#PV#geq11)",              6,-0.5,5.5);
    AddHisto(HistoBtag, "npv3_flightsig3d",      j,"Flight Significance 3D (#PV#geq11)",                  50,0.,100.); //130
    AddHisto(HistoBtag, "npv3_flightsig3d_3tr",  j,"Flight Significance 3D for >=3tracks (#PV#geq11)",    50,0.,100.);
    AddHisto(HistoBtag, "npv3_svmass",           j,"SV mass (#PV#geq11)",                             50,0.,8.);
    AddHisto(HistoBtag, "npv3_svmass_3tr",       j,"SV mass for >=3tracks (#PV#geq11)",               50,0.,8.);




    // additional plots for AN-11-180 
    AddHisto(HistoBtag, "n_cutseltracks_jet",   j,"# of sel tracks in jet",40,-0.5,39.5);
    AddHisto(HistoBtag, "pt_cutseltracks",   j,"Pt of  sel tracks",80,0.,200.);  //135
    AddHisto(HistoBtag, "IP3d1Errorsorted",  j,"1st track 3D IP Error",100,0.,0.1); 
    AddHisto(HistoBtag, "IP3d2Errorsorted",  j,"2nd track 3D IP Error",100,0.,0.1); 
    AddHisto(HistoBtag, "IP3d3Errorsorted",  j,"3rd track 3D IP Error",100,0.,0.1); 
    AddHisto(HistoBtag, "trackIP3d_bin1_cutsel",  j,"Track 3D IP value (1<Pt<=2 GeV/c)",100,-0.1,0.1); 
    AddHisto(HistoBtag, "trackIP3d_bin2_cutsel",  j,"Track 3D IP value (2<Pt<=5 GeV/c)",100,-0.1,0.1); //140
    AddHisto(HistoBtag, "trackIP3d_bin3_cutsel",  j,"Track 3D IP value (5<Pt<=8 GeV/c)",100,-0.1,0.1); 
    AddHisto(HistoBtag, "trackIP3d_bin4_cutsel",  j,"Track 3D IP value (8<Pt<=12 GeV/c)",100,-0.1,0.1); 
    AddHisto(HistoBtag, "trackIP3d_bin5_cutsel",  j,"Track 3D IP value (12<Pt<=20 GeV/c)",100,-0.1,0.1); 
    AddHisto(HistoBtag, "trackIP3d_bin6_cutsel",  j,"Track 3D IP value (20<Pt<=50 GeV/c)",100,-0.1,0.1); 
    AddHisto(HistoBtag, "npv1_nsv_3tr",              j,"# of secondary vertices (>=3tr) (1-6 PVs)",              5,0,5); //145
    AddHisto(HistoBtag, "npv2_nsv_3tr",              j,"# of secondary vertices (>=3tr) (7-10 PVs)",              5,0,5); 
    AddHisto(HistoBtag, "npv3_nsv_3tr",              j,"# of secondary vertices (>=3tr) (#PV#geq11)",              5,0,5);
    AddHisto(HistoBtag, "npv1_nsv0_3tr",              j,"# of secondary vertices (>=3tr) (1-6 PVs)",              5,0,5);
    AddHisto(HistoBtag, "npv2_nsv0_3tr",              j,"# of secondary vertices (>=3tr) (7-10 PVs)",              5,0,5); 
    AddHisto(HistoBtag, "npv3_nsv0_3tr",              j,"# of secondary vertices (>=3tr) (#PV#geq11)",              5,0,5); //150

    //IP significance sorted in bins of track pt
    AddHisto(HistoBtag, "IP3d1sigsorted_bin1",j,"1st track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d1sigsorted_bin2",j,"1st track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d1sigsorted_bin3",j,"1st track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d1sigsorted_bin4",j,"1st track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d1sigsorted_bin5",j,"1st track 3D IP significance (sorted)",100,-35.,35.);//155
    AddHisto(HistoBtag, "IP3d1sigsorted_bin6",j,"1st track 3D IP significance (sorted)",100,-35.,35.);
 
    AddHisto(HistoBtag, "IP3d2sigsorted_bin1",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigsorted_bin2",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigsorted_bin3",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigsorted_bin4",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);//160
    AddHisto(HistoBtag, "IP3d2sigsorted_bin5",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigsorted_bin6",j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
 
    // muon-enriched (ptrel>0) standard track-based properties:

    AddHisto(HistoBtag, "sv_mass_muenriched",              j,"SV mass",50,0.,8.); //163, similar to 60
    AddHisto(HistoBtag, "IP3d2sigsorted_muenriched",       j,"2nd track 3D IP significance (sorted)",100,-35.,35.); //164, similar to 51
    AddHisto(HistoBtag, "IP3d2sigzsorted_muenriched",       j,"2nd track 3D IP significance (sorted)",100,-2.,2.); //165, similar to 52
    AddHisto(HistoBtag, "sv_flightsig2d_muenriched",        j,"Flight significance 2D",50,0.,80.); //166, similar to 67
    AddHisto(HistoBtag, "sv_flightsig3d_muenriched",        j,"Flight significance 3D",50,0.,80.); //167, similar to 68
    // the following are filled just after the 'normal' discriminants (#4-10)
    AddHisto(HistoBtag, "discri_tche_muenriched",           j,"TCHE Discriminator",50,0.,30.);  // 168 
    AddHisto(HistoBtag, "discri_tchp_muenriched",           j,"TCHP Discriminator",50,0.,30.);  // 169
    AddHisto(HistoBtag, "discri_ssche_muenriched",          j,"SSVHE Discriminator",50,0.,7.);   // 170
    AddHisto(HistoBtag, "discri_sschp_muenriched",          j,"SSVHP Discriminator",50,0.,7.);  //171
    AddHisto(HistoBtag, "muenriched_discrim",               j,"muon pTrel cut on for b-enrichment",50,-5,10);// 172, for reference 
    AddHisto(HistoBtag, "discri_jetprob_muenriched",        j,"JetProb Discriminator",50,0.,2.5); //173
    AddHisto(HistoBtag, "discri_jetbprob_muenriched",       j,"JetBProb Discriminator",50,0.,8.); //174

    // Key variables with coarse jet pt bins (50-80 80-120 120+)

    // IP variables (sorted)
    AddHisto(HistoBtag, "trackIP3d_jetptbin1",          j,"Track 3D IP value (sorted)",100,-0.1,0.1);  //175
    AddHisto(HistoBtag, "trackIP3d_jetptbin2",          j,"Track 3D IP value (sorted)",100,-0.1,0.1);
    AddHisto(HistoBtag, "trackIP3d_jetptbin3",          j,"Track 3D IP value (sorted)",100,-0.1,0.1);
						         
    AddHisto(HistoBtag, "IP3d1_jetptbin1",              j,"1st track 3D IP value (sorted)",100,-0.1,0.1);  
    AddHisto(HistoBtag, "IP3d1_jetptbin2",              j,"1st track 3D IP value (sorted)",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d1_jetptbin3",              j,"1st track 3D IP value (sorted)",100,-0.1,0.1);  //180
						         
    AddHisto(HistoBtag, "IP3d2_jetptbin1",              j,"2nd track 3D IP value (sorted)",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d2_jetptbin2",              j,"2nd track 3D IP value (sorted)",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d2_jetptbin3",              j,"2nd track 3D IP value (sorted)",100,-0.1,0.1);  
						         
    AddHisto(HistoBtag, "IP3d3_jetptbin1",              j,"3rd track 3D IP value (sorted)",100,-0.1,0.1);
    AddHisto(HistoBtag, "IP3d3_jetptbin2",              j,"3rd track 3D IP value (sorted)",100,-0.1,0.1);  //185
    AddHisto(HistoBtag, "IP3d3_jetptbin3",              j,"3rd track 3D IP value (sorted)",100,-0.1,0.1);
     						         
    AddHisto(HistoBtag, "IP3dsigsorted_jetptbin1",      j,"Track 3D IP significance (sorted)",200,-35.,35.); 
    AddHisto(HistoBtag, "IP3dsigsorted_jetptbin2",      j,"Track 3D IP significance (sorted)",200,-35.,35.);  
    AddHisto(HistoBtag, "IP3dsigsorted_jetptbin3",      j,"Track 3D IP significance (sorted)",200,-35.,35.);
						         
    AddHisto(HistoBtag, "IP3d1sigsorted_jetptbin1",     j,"1st track 3D IP significance (sorted)",100,-35.,35.);  //190  
    AddHisto(HistoBtag, "IP3d1sigsorted_jetptbin2",     j,"1st track 3D IP significance (sorted)",100,-35.,35.); 
    AddHisto(HistoBtag, "IP3d1sigsorted_jetptbin3",     j,"1st track 3D IP significance (sorted)",100,-35.,35.);
						         
    AddHisto(HistoBtag, "IP3d2sigsorted_jetptbin1",     j,"2nd track 3D IP significance (sorted)",100,-35.,35.);  
    AddHisto(HistoBtag, "IP3d2sigsorted_jetptbin2",     j,"2nd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d2sigsorted_jetptbin3",     j,"2nd track 3D IP significance (sorted)",100,-35.,35.);  //195
						         
    AddHisto(HistoBtag, "IP3d3sigsorted_jetptbin1",     j,"3rd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d3sigsorted_jetptbin2",     j,"3rd track 3D IP significance (sorted)",100,-35.,35.);
    AddHisto(HistoBtag, "IP3d3sigsorted_jetptbin3",     j,"3rd track 3D IP significance (sorted)",100,-35.,35.);  
						        
    AddHisto(HistoBtag, "IP3dErrorsorted_jetptbin1",    j,"Track 3D IP Error (sorted)",100,0.,0.1);
    AddHisto(HistoBtag, "IP3dErrorsorted_jetptbin2",    j,"Track 3D IP Error (sorted)",100,0.,0.1);  //200
    AddHisto(HistoBtag, "IP3dErrorsorted_jetptbin3",    j,"Track 3D IP Error (sorted)",100,0.,0.1);

    AddHisto(HistoBtag, "IP3d1Errorsorted_jetptbin1",    j,"1st track 3D IP Error (sorted)",100,0.,0.1);
    AddHisto(HistoBtag, "IP3d1Errorsorted_jetptbin2",    j,"1st track 3D IP Error (sorted)",100,0.,0.1);  
    AddHisto(HistoBtag, "IP3d1Errorsorted_jetptbin3",    j,"1st track 3D IP Error (sorted)",100,0.,0.1); 
						         
    AddHisto(HistoBtag, "IP3d2Errorsorted_jetptbin1",    j,"2nd track 3D IP Error (sorted)",100,0.,0.1);  //205
    AddHisto(HistoBtag, "IP3d2Errorsorted_jetptbin2",    j,"2nd track 3D IP Error (sorted)",100,0.,0.1); 
    AddHisto(HistoBtag, "IP3d2Errorsorted_jetptbin3",    j,"2nd track 3D IP Error (sorted)",100,0.,0.1); 
     						         
    AddHisto(HistoBtag, "IP3d3Errorsorted_jetptbin1",    j,"3rd track 3D IP Error (sorted)",100,0.,0.1);  
    AddHisto(HistoBtag, "IP3d3Errorsorted_jetptbin2",    j,"3rd track 3D IP Error (sorted)",100,0.,0.1); 
    AddHisto(HistoBtag, "IP3d3Errorsorted_jetptbin3",    j,"3rd track 3D IP Error (sorted)",100,0.,0.1);  //210

    // Vertex variables (sorted)
    AddHisto(HistoBtag, "sv_mass_jetptbin1",            j,"SV mass (sorted)",50,0.,8.);
    AddHisto(HistoBtag, "sv_mass_jetptbin2",            j,"SV mass (sorted)",50,0.,8.);
    AddHisto(HistoBtag, "sv_mass_jetptbin3",            j,"SV mass (sorted)",50,0.,8.);  

    AddHisto(HistoBtag, "sv_mass_3tr_jetptbin1",        j,"SV mass for >=3tracks (sorted)",50,0.,8.);
    AddHisto(HistoBtag, "sv_mass_3tr_jetptbin2",        j,"SV mass for >=3tracks (sorted)",50,0.,8.);  //215
    AddHisto(HistoBtag, "sv_mass_3tr_jetptbin3",        j,"SV mass for >=3tracks (sorted)",50,0.,8.);

    AddHisto(HistoBtag, "sv_ipsigcharm_jetptbin1",      j,"IP significance 2D charm (sorted)",50,-35.,35.);
    AddHisto(HistoBtag, "sv_ipsigcharm_jetptbin2",      j,"IP significance 2D charm (sorted)",50,-35.,35.);  
    AddHisto(HistoBtag, "sv_ipsigcharm_jetptbin3",      j,"IP significance 2D charm (sorted)",50,-35.,35.);

    AddHisto(HistoBtag, "sv_eratio_jetptbin1",          j,"Fractional energy (sorted)",50,0.,1.);  //220
    AddHisto(HistoBtag, "sv_eratio_jetptbin2",          j,"Fractional energy (sorted)",50,0.,1.);
    AddHisto(HistoBtag, "sv_eratio_jetptbin3",          j,"Fractional energy (sorted)",50,0.,1.);

    AddHisto(HistoBtag, "sv_flightsig2d_jetptbin1",      j,"Flight significance 2D (sorted)",50,0.,80.);  
    AddHisto(HistoBtag, "sv_flightsig2d_jetptbin2",      j,"Flight significance 2D (sorted)",50,0.,80.);
    AddHisto(HistoBtag, "sv_flightsig2d_jetptbin3",      j,"Flight significance 2D (sorted)",50,0.,80.);  //225  
						          
    AddHisto(HistoBtag, "sv_flightsig3d_jetptbin1",      j,"Flight significance 3D (sorted)",50,0.,80.);
    AddHisto(HistoBtag, "sv_flightsig3d_jetptbin2",      j,"Flight significance 3D (sorted)",50,0.,80.);
    AddHisto(HistoBtag, "sv_flightsig3d_jetptbin3",      j,"Flight significance 3D (sorted)",50,0.,80.);  
						          
    AddHisto(HistoBtag, "sv_flightdis2d_jetptbin1",      j,"Flight distance 2D (sorted)",50,0.,2.5);
    AddHisto(HistoBtag, "sv_flightdis2d_jetptbin2",      j,"Flight distance 2D (sorted)",50,0.,2.5);  //230
    AddHisto(HistoBtag, "sv_flightdis2d_jetptbin3",      j,"Flight distance 2D (sorted)",50,0.,2.5);
						          
    AddHisto(HistoBtag, "sv_flightdis3d_jetptbin1",      j,"Flight distance 3D (sorted)",50,0.,15.);
    AddHisto(HistoBtag, "sv_flightdis3d_jetptbin2",      j,"Flight distance 3D (sorted)",50,0.,15.);  
    AddHisto(HistoBtag, "sv_flightdis3d_jetptbin3",      j,"Flight distance 3D (sorted)",50,0.,15.);

    AddHisto(HistoBtag, "sv_flightsig3d_3tr_jetptbin1",  j,"Flight significance 3D for >=3tracks (sorted)",50,0.,80.);  //235
    AddHisto(HistoBtag, "sv_flightsig3d_3tr_jetptbin2",  j,"Flight significance 3D for >=3tracks (sorted)",50,0.,80.);
    AddHisto(HistoBtag, "sv_flightsig3d_3tr_jetptbin3",  j,"Flight significance 3D for >=3tracks (sorted)",50,0.,80.);

    AddHisto(HistoBtag, "sv_deltar_jet_jetptbin1",        j,"SVjetDeltaR (sorted)",50,0.,0.5);  
    AddHisto(HistoBtag, "sv_deltar_jet_jetptbin2",        j,"SVjetDeltaR (sorted)",50,0.,0.5);
    AddHisto(HistoBtag, "sv_deltar_jet_jetptbin3",        j,"SVjetDeltaR (sorted)",50,0.,0.5);  //240

    AddHisto(HistoBtag, "sv_deltar_sum_jet_jetptbin1",    j,"SVvtxSumJetDeltaR (sorted)",50,0.,0.5);
    AddHisto(HistoBtag, "sv_deltar_sum_jet_jetptbin2",    j,"SVvtxSumJetDeltaR (sorted)",50,0.,0.5);
    AddHisto(HistoBtag, "sv_deltar_sum_jet_jetptbin3",    j,"SVvtxSumJetDeltaR (sorted)",50,0.,0.5);  

    AddHisto(HistoBtag, "sv_deltar_sum_dir_jetptbin1",    j,"SVvtxSumVtxDirDeltaR (sorted)",50,0.,0.5);
    AddHisto(HistoBtag, "sv_deltar_sum_dir_jetptbin2",    j,"SVvtxSumVtxDirDeltaR (sorted)",50,0.,0.5);  //245
    AddHisto(HistoBtag, "sv_deltar_sum_dir_jetptbin3",    j,"SVvtxSumVtxDirDeltaR (sorted)",50,0.,0.5);

    // Discriminators (sorted)
    AddHisto(HistoBtag, "discri_tche_jetptbin1",          j,"TCHE Discriminator (sorted)",50,0.,30.);
    AddHisto(HistoBtag, "discri_tche_jetptbin2",          j,"TCHE Discriminator (sorted)",50,0.,30.);  
    AddHisto(HistoBtag, "discri_tche_jetptbin3",          j,"TCHE Discriminator (sorted)",50,0.,30.);

    AddHisto(HistoBtag, "discri_tchp_jetptbin1",          j,"TCHP Discriminator (sorted)",50,0.,30.);  //250
    AddHisto(HistoBtag, "discri_tchp_jetptbin2",          j,"TCHP Discriminator (sorted)",50,0.,30.);
    AddHisto(HistoBtag, "discri_tchp_jetptbin3",          j,"TCHP Discriminator (sorted)",50,0.,30.);
     
    AddHisto(HistoBtag, "discri_ssche0_jetptbin1",        j,"SSVHE Discriminator (sorted)",80,-1.,7.);  
    AddHisto(HistoBtag, "discri_ssche0_jetptbin2",        j,"SSVHE Discriminator (sorted)",80,-1.,7.);
    AddHisto(HistoBtag, "discri_ssche0_jetptbin3",        j,"SSVHE Discriminator (sorted)",80,-1.,7.);  //255

    AddHisto(HistoBtag, "discri_ssche_jetptbin1",         j,"SSVHE Discriminator (sorted)",50,0.,7.);
    AddHisto(HistoBtag, "discri_ssche_jetptbin2",         j,"SSVHE Discriminator (sorted)",50,0.,7.);
    AddHisto(HistoBtag, "discri_ssche_jetptbin3",         j,"SSVHE Discriminator (sorted)",50,0.,7.);  

    AddHisto(HistoBtag, "discri_sschp0_jetptbin1",        j,"SSVHP Discriminator (sorted)",80,-1.,7.);
    AddHisto(HistoBtag, "discri_sschp0_jetptbin2",        j,"SSVHP Discriminator (sorted)",80,-1.,7.);  //260
    AddHisto(HistoBtag, "discri_sschp0_jetptbin3",        j,"SSVHP Discriminator (sorted)",80,-1.,7.);

    AddHisto(HistoBtag, "discri_sschp_jetptbin1",         j,"SSVHP Discriminator (sorted)",50,0.,7.);   
    AddHisto(HistoBtag, "discri_sschp_jetptbin2",         j,"SSVHP Discriminator (sorted)",50,0.,7.);  
    AddHisto(HistoBtag, "discri_sschp_jetptbin3",         j,"SSVHP Discriminator (sorted)",50,0.,7.);
     
    AddHisto(HistoBtag, "discri_jetprob_jetptbin1",       j,"JetProb Discriminator (sorted)",50,0.,2.5);  //265
    AddHisto(HistoBtag, "discri_jetprob_jetptbin2",       j,"JetProb Discriminator (sorted)",50,0.,2.5);
    AddHisto(HistoBtag, "discri_jetprob_jetptbin3",       j,"JetProb Discriminator (sorted)",50,0.,2.5);

    AddHisto(HistoBtag, "discri_jetbprob_jetptbin1",      j,"JetBProb Discriminator (sorted)",50,0.,8.);  
    AddHisto(HistoBtag, "discri_jetbprob_jetptbin2",      j,"JetBProb Discriminator (sorted)",50,0.,8.);
    AddHisto(HistoBtag, "discri_jetbprob_jetptbin3",      j,"JetBProb Discriminator (sorted)",50,0.,8.);  //270

    // Vertex variables for 3 track vertices
    AddHisto(HistoBtag, "sv_ipsigcharm_3tr",              j,"IP significance 2D charm >=3tracks",50,-35.,35.);
    AddHisto(HistoBtag, "sv_eratio_3tr",                  j,"Fractional energy >=3tracks",50,0.,1.);
    AddHisto(HistoBtag, "sv_vtx_pt_3tr",                  j,"PT of vtx >=3tracks",50,0.,100.);

    // Center integer binning for multiplicity plots
    AddHisto(HistoBtag, "sv_trackmul_centered",           j,"Track multiplicity : SVnVertexTracks (centered)",13,-0.5,12.5);
    AddHisto(HistoBtag, "sv_trackmul1_centered",          j,"Track multiplicity : SVnFirstVertexTracks (centered)",11,-0.5,10.5);  //275
    AddHisto(HistoBtag, "sv_tracksel_centered",           j,"Track multiplicity : SVnSelectedTracks (centered)",21,-0.5,20.5);
    AddHisto(HistoBtag, "muons_multiplicity_centered",    j,"# of muons in jet (centered)",11,-0.5,10.5);


    AddHisto(HistoBtag, "npv1_sveratio",        j,"SV eratio (1-6 PVs)",                       50,0.,1.);          //278
    AddHisto(HistoBtag, "npv1_deltar_jet",      j,"SV deltaR jet (1-6 PVs)",                 50,0.,0.5);
    AddHisto(HistoBtag, "npv1_deltar_sum_jet",      j,"SV deltaR sum jet (1-6 PVs)",                 50,0.,0.5);   //280
    AddHisto(HistoBtag, "npv1_deltar_sum_dir",      j,"SV deltaR sum dir (1-6 PVs)",                 50,0.,0.5);   
    AddHisto(HistoBtag, "npv1_vtxpt",      j,"SV vtx pt (1-6 PVs)",                 50,0.,100);   
    AddHisto(HistoBtag, "npv1_discri_tche",      j,"TCHE discriminator (1-6 PVs)",                 50,0.,30);   
    AddHisto(HistoBtag, "npv1_discri_tchp",      j,"TCHP discriminator (1-6 PVs)",                 50,0.,30);   
    AddHisto(HistoBtag, "npv1_discri_jetprob",      j,"JP discriminator (1-6 PVs)",                 50,0.,2.5);   //285
    AddHisto(HistoBtag, "npv1_discri_jetbprob",      j,"JBP discriminator (1-6 PVs)",                 50,0.,8);   

    AddHisto(HistoBtag, "npv1_discri_ssche0",       j,"SSVHE Discriminator (1-6 PVs)",80,-1.,7.); 
    AddHisto(HistoBtag, "npv1_discri_ssche",   j,"SSVHE Discriminator (1-6 PVs)",50,0.,7.);   
    AddHisto(HistoBtag, "npv1_discri_sschp0",   j,"SSVHP Discriminator (1-6 PVs)",80,-1.,7.); 
    AddHisto(HistoBtag, "npv1_discri_sschp",   j,"SSVHP Discriminator (1-6 PVs)",50,0.,7.);                     //290
    AddHisto(HistoBtag, "npv1_muon_Pt", j,       "Muon p_{T} (1-6 PVs)",200,0,100);
    AddHisto(HistoBtag, "npv1_muon_ptrel", j ,   "Muon p_{T}^{rel} (1-6 PVs)",50,0,5); 
    AddHisto(HistoBtag, "npv1_muon_DeltaR", j,        "Muon1 deltaR (1-6 PVs)",50,0,0.5); 

    AddHisto(HistoBtag, "npv2_sveratio",        j,"SV eratio (7-10 PVs)",                       50,0.,1.);
    AddHisto(HistoBtag, "npv2_deltar_jet",      j,"SV deltaR jet (7-10 PVs)",                 50,0.,0.5);           //295
    AddHisto(HistoBtag, "npv2_deltar_sum_jet",      j,"SV deltaR sum jet (7-10 PVs)",                 50,0.,0.5);   
    AddHisto(HistoBtag, "npv2_deltar_sum_dir",      j,"SV deltaR sum dir (7-10 PVs)",                 50,0.,0.5);   
    AddHisto(HistoBtag, "npv2_vtxpt",      j,"SV vtx pt (7-10 PVs)",                 50,0., 100);   
    AddHisto(HistoBtag, "npv2_discri_tche",      j,"TCHE discriminator (7-10 PVs)",                 50,0.,30);   
    AddHisto(HistoBtag, "npv2_discri_tchp",      j,"TCHP discriminator (7-10 PVs)",                 50,0.,30);   //300
    AddHisto(HistoBtag, "npv2_discri_jetprob",      j,"JP discriminator (7-10 PVs)",                 50,0.,2.5);   
    AddHisto(HistoBtag, "npv2_discri_jetbprob",      j,"JBP discriminator (7-10 PVs)",                 50,0.,8);   

    AddHisto(HistoBtag, "npv2_discri_ssche0",       j,"SSVHE Discriminator (7-10 PVs)",80,-1.,7.); 
    AddHisto(HistoBtag, "npv2_discri_ssche",   j,"SSVHE Discriminator (7-10 PVs)",50,0.,7.);   
    AddHisto(HistoBtag, "npv2_discri_sschp0",   j,"SSVHP Discriminator (7-10 PVs)",80,-1.,7.); //305
    AddHisto(HistoBtag, "npv2_discri_sschp",   j,"SSVHP Discriminator (7-10 PVs)",50,0.,7.);   
    AddHisto(HistoBtag, "npv2_muon_Pt", j,       "Muon p_{T} (7-10 PVs)",200,0,100);
    AddHisto(HistoBtag, "npv2_muon_ptrel", j ,   "Muon p_{T}^{rel} (7-10 PVs)",50,0,5); 
    AddHisto(HistoBtag, "npv2_muon_DeltaR", j,        "Muon1 deltaR (7-10 PVs)",50,0,0.5); 

     
    AddHisto(HistoBtag, "npv3_sveratio",        j,"SV eratio (#PV#geq11)",                       50,0.,1.);     //310
    AddHisto(HistoBtag, "npv3_deltar_jet",      j,"SV deltaR jet (#PV#geq11)",                 50,0.,0.5);
    AddHisto(HistoBtag, "npv3_deltar_sum_jet",      j,"SV deltaR sum jet (#PV#geq11)",                 50,0.,0.5);   
    AddHisto(HistoBtag, "npv3_deltar_sum_dir",      j,"SV deltaR sum dir (#PV#geq11)",                 50,0.,0.5);   
    AddHisto(HistoBtag, "npv3_vtxpt",      j,"SV vtx pt (#PV#geq11)",                 50,0., 100);   
    AddHisto(HistoBtag, "npv3_discri_tche",      j,"TCHE discriminator (#PV#geq11)",                 50,0.,30);      //315
    AddHisto(HistoBtag, "npv3_discri_tchp",      j,"TCHP discriminator (#PV#geq11)",                 50,0.,30);   
    AddHisto(HistoBtag, "npv3_discri_jetprob",      j,"JP discriminator (#PV#geq11)",                 50,0.,2.5);   
    AddHisto(HistoBtag, "npv3_discri_jetbprob",      j,"JBP discriminator (#PV#geq11)",                 50,0.,8);   

    AddHisto(HistoBtag, "npv3_discri_ssche0",       j,"SSVHE Discriminator (#PV#geq11)",80,-1.,7.); 
    AddHisto(HistoBtag, "npv3_discri_ssche",   j,"SSVHE Discriminator (#PV#geq11)",50,0.,7.);      //320
    AddHisto(HistoBtag, "npv3_discri_sschp0",   j,"SSVHP Discriminator (#PV#geq11)",80,-1.,7.); 
    AddHisto(HistoBtag, "npv3_discri_sschp",   j,"SSVHP Discriminator (#PV#geq11)",50,0.,7.);   
    AddHisto(HistoBtag, "npv3_muon_Pt", j,       "Muon p_{T} (#PV#geq11)",200,0,100);
    AddHisto(HistoBtag, "npv3_muon_ptrel", j ,   "Muon p_{T}^{rel} (#PV#geq11)",50,0,5); 
    AddHisto(HistoBtag, "npv3_muon_DeltaR", j,        "Muon1 deltaR (#PV#geq11)",50,0,0.5);       //325


    AddHisto(HistoBtag, "npv1_jetpt",               j,"PT of all jets",80,50,400.);   //326
    AddHisto(HistoBtag, "npv1_jetpt_220_300",       j,"PT of all jets",80,200,400.);   //327
    AddHisto(HistoBtag, "npv1_jetpt_300_400",       j,"PT of all jets",80,200,400.);   //328

    AddHisto(HistoBtag, "npv2_jetpt",               j,"PT of all jets",80,50,400.);   //329
    AddHisto(HistoBtag, "npv2_jetpt_220_300",       j,"PT of all jets",80,200,400.);   //330
    AddHisto(HistoBtag, "npv2_jetpt_300_400",       j,"PT of all jets",80,200,400.);   //331
     
    AddHisto(HistoBtag, "npv3_jetpt",               j,"PT of all jets",80,50,400.);   //332
    AddHisto(HistoBtag, "npv3_jetpt_220_300",       j,"PT of all jets",80,200,400.);   //333
    AddHisto(HistoBtag, "npv3_jetpt_300_400",       j,"PT of all jets",80,200,400.);   //334
    AddHisto(HistoBtag, "discri_csv",   j,"CSV Discriminator",50,0.,1.);//335
    AddHisto(HistoBtag, "discri_csvmva",   j,"CSVMVA Discriminator",50,0.,1.);//336

    // ====> ADD HERE NEW HISTO AT 1D FOR WHICH QUARK CONTENT INFO NEEDED
    // example :
    // AddHisto(HistoBtag, "new_histo", j,"Test Caro",50,0.,1.);

    // 2D plots SV
#define MAXJETPT 1000
    AddHisto2D(Histo2DB, "sv_track_vs_jetpt", j,"SV track multiplicity vs jet pt",30,60,MAXJETPT, 10,-0.5,9.5);   //0
    AddHisto2D(Histo2DB, "sv_mass_vs_flightDistSignCut", j, " SVMass vs Cut on SV sign ", 100,0, 80,100,0,6);
    AddHisto2D(Histo2DB, "sv_mass_vs_flightDistCut", j, " SVMass vs Cut on SV distance ",100,0, 10,100,0,6);

    AddHisto2D(Histo2DB, "track_vs_jetpt", j,"track multiplicity vs jet pt",30,60,MAXJETPT, 100,-0.5,99.5);
    AddHisto2D(Histo2DB, "seltrack_vs_jetpt", j,"sel track multiplicity vs jet pt",30,60,MAXJETPT, 100,-0.5,99.5);
    AddHisto2D(Histo2DB, "cutseltrack_vs_jetpt", j,"sel track multiplicity vs jet pt",30,60,MAXJETPT, 100,-0.5,99.5);  //5

    // Trend plot average SVmass vs jetpt
     
    AddHisto2D(Histo2DB, "avg_sv_mass_vs_jetpt", j,"Avg SVMass vs jet pt",30,60,MAXJETPT, 100,0,6); 

    // 2D plots for veretx deltaR and muon ptrel,deltaR vs jet pt
     
    AddHisto2D(Histo2DB, "sv_deltar_jet_vs_jetpt", j,"SVJetDeltaR vs jet pt",25,60,300, 50,0.,0.5);  
    AddHisto2D(Histo2DB, "sv_deltar_sum_jet_vs_jetpt", j,"SVvtxSumJetDeltaR vs jet pt",25,60,300, 50,0.,0.5);
    AddHisto2D(Histo2DB, "sv_deltar_sum_dir_vs_jetpt", j,"SVvtxSumVtxDirDeltaR vs jet pt",25,60,300, 50,0.,0.5);

    AddHisto2D(Histo2DB, "muon_ptrel_vs_jetpt", j,"Muon_p{T}^{rel} vs jet pt",30,60,MAXJETPT, 50,0,5);  //10
    AddHisto2D(Histo2DB, "muon_DeltaR_vs_jetpt", j,"Muon1 DeltaR vs jet pt",30,60,MAXJETPT, 50,0,0.5);
     
    AddHisto2D(Histo2DB, "sv_eratio_vs_jetpt", j,"SV eratio vs jet pt",30,60,MAXJETPT, 50,0,1);
    AddHisto2D(Histo2DB, "sv_vtx_pt_vs_jetpt", j,"SV vtx pt vs jet pt",30,60,MAXJETPT, 50,0,100); //13
    // ====> ADD HERE NEW HISTO AT 2D FOR WHICH QUARK CONTENT INFO IS NEEDED

    if (j==0) nhisto_to_clone=HistoBtag.size();
    if (j==0) nhisto_to_clone2D=Histo2DB.size();
  }
  AddHisto(HistoBtag, "pthat",       0,"PThat",80,0.,400.);//0
  AddHisto(HistoBtag, "prescale_30",      0,"prescale HLT_JET30",400,0.,400.);
  AddHisto(HistoBtag, "prescale_60",      0,"prescale HLT_JET60",400,0.,400.);
  AddHisto(HistoBtag, "prescale_80",      0,"prescale HLT_JET80",400,0.,400.);
  AddHisto(HistoBtag, "prescale_110",     0,"prescale HLT_JET110",400,0.,400.);

#if bTagNtupleVersion >= 4
  AddHisto(HistoBtag, "prescale_190",     0,"prescale HLT_JET190",400,0.,400.);
#endif
  AddHisto(HistoBtag, "weight_prescale",  0,"weight from prescale HLTJET50to100",400,0.,400.);
  AddHisto(HistoBtag, "npv",  0,"numberOfPrimaryVertices",50,-0.5,49.5);
  AddHisto(HistoBtag, "npv_no_scaled",  0,"numberOfPrimaryVertices",50,-0.5,49.5);

  AddHisto(HistoBtag, "npu",  0,"numberOfPUVertices",50,-0.5,49.5);
  AddHisto(HistoBtag, "npu_no_scaled",  0,"numberOfPUVertices_no_scaled",50,-0.5,49.5);//10

  // ====> ADD HERE NEW HISTO AT 1D OR 2D FOR WHICH QUARK CONTENT INFO IS NOT NEEDED
  // in this case, do not put "j" but 0!
  // example : 
  // AddHisto(HistoBtag, "another_new_histo", 0,"Test Caro 2",50,0.,1.);
  AddHisto2D(Histo2DB, "pu_vs_pv", 0,"pu vs pv",MAXPU,0,MAXPU, MAXPU,0,MAXPU); //2D-0
  AddHisto2D(Histo2DB, "ip3dvsphi",0,"ip3d vs phi",60,-3.14,3.14,20000,-35.,35.); //2D-1
  AddHisto2D(Histo2DB, "ip2dvsphi",0,"ip2d vs phi",60,-3.14,3.14,20000,-1.,1.); //2D-2
  AddHisto2D(Histo2DB, "ip3dsigvsphi",0,"ip3dsig vs phi",60,-3.14,3.14,2000,-35.,35.); //2D-3
  AddHisto2D(Histo2DB, "ip2dsigvsphi",0,"ip2dsig vs phi",60,-3.14,3.14,2000,-35.,35.); //2D-4

  cout << " # histos created : " << HistoBtag.size() << " (nhisto_to_clone : " << nhisto_to_clone  << ")" << endl;
  /*
    for (int i=0; i<HistoBtag.size(); i++) {
    cout << " HistoBtag[" <<i << "] " << HistoBtag[i]->GetName() << endl;
    }

  */



  cout<< " weight " << weightsave << endl;

  Long64_t nentries = fChain->GetEntries();

  cout << " nentries " << nentries << endl;
  Long64_t nbytes = 0, nb = 0;

  // LOOP ON EVENTS IN THE NTUPLES 

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);  nbytes += nb;
    Float_t weight=weightsave;
    if ( (jentry+1) % (nentries/100) == 0) cout<<"\r"<<"Calculating "<<jentry+1<<"/"<<nentries<<"("<<(jentry+1)/Float_t(nentries)*100<<"%)";
    
    if (!isRealData) {
      int npvval=int(numberOfPUVerticesMixingTruth);

      //int npvval=numberOfPUVertices;

      if (npvval>=MAXPU) npvval=MAXPU;
      //      cerr<<npvval<<":"<<numberOfPUVertices<<":"<<puweight[npvval]<<endl;
      weight*=mcweight;
      weight*=puweight[npvval];
      HistoBtag[jmax*nhisto_to_clone]->Fill(pthat,weight);
      HistoBtag[jmax*nhisto_to_clone+9]->Fill(numberOfPUVertices,weight);
      if (!isRealData) {
	HistoBtag[jmax*nhisto_to_clone+10]->Fill(numberOfPUVertices,weightsave*mcweight);
	Histo2DB[jmax*nhisto_to_clone2D]->Fill(numberOfPrimaryVertices,numberOfPUVertices, weight);
      }
       else HistoBtag[jmax*nhisto_to_clone+10]->Fill(numberOfPUVertices,weightsave);

    }
    else {
      if (runNumber==191090 ||  runNumber==191367|| runNumber==191411 ||runNumber==193112 ||runNumber==193116) continue;


      HistoBtag[jmax*nhisto_to_clone+1]->Fill(HLTPrescale("HLT_Jet30"));
      HistoBtag[jmax*nhisto_to_clone+2]->Fill(HLTPrescale("HLT_Jet60"));
      HistoBtag[jmax*nhisto_to_clone+3]->Fill(HLTPrescale("HLT_Jet80"));
      HistoBtag[jmax*nhisto_to_clone+4]->Fill(HLTPrescale("HLT_Jet110"));
      HistoBtag[jmax*nhisto_to_clone+5]->Fill(HLTPrescale("HLT_Jet190"));
    }

    //// IMPORTANT : TRIGGER 
    bool cut_trigger= false;

    UInt_t prescale=1;
    if (JetPtthresholdsfornmu>0) {//"<0" means that we are not studying BTagMu trigger.
      UInt_t nmu=0;
      for (int i=0; i<nJets; i++){
        
	if (-2.4<jetEta[i] && jetEta[i]<2.4 && jetPt[i]>JetPtthresholdsfornmu) nmu++;
	}
      if (isRealData) {
	if ( HLTAcceptance(my_trigger_path.Data()) && nmu>1 ) {
	  cut_trigger=true;
	  prescale = HLTPrescale(my_trigger_path.Data());
	}
      }
      else if (nmu>1) cut_trigger=true;
    }
    else {
      if ( HLTAcceptance(my_trigger_path.Data()) ) {//if study 2010 DATA by ntuple(>3), one needs to change the trigger name. It is extremely low probablity that one should do that.
	cut_trigger=true;
	prescale = HLTPrescale(my_trigger_path.Data());
      }
    }

    if (cut_trigger) {
      if (isRealData) {
	//         weight=prescale;  // if we want to apply prescale weight in data
	HistoBtag[jmax*nhisto_to_clone+6]->Fill(prescale);
      }
      HistoBtag[jmax*nhisto_to_clone+7]->Fill(numberOfPrimaryVertices,weight);
      if (!isRealData)HistoBtag[jmax*nhisto_to_clone+8]->Fill(numberOfPrimaryVertices,weightsave*mcweight);
      else HistoBtag[jmax*nhisto_to_clone+8]->Fill(numberOfPrimaryVertices,weightsave);


      // FILL HERE YOUR NEW HISTO outside "j" loop
      // example :
      // Float_t caro_test_2=0.2;
      // HistoBtag[jmax*nhisto_to_clone+11]->Fill(caro_test_2, weight);   // increase the "10" of "npu" histo into a "11"

      for (int i=0; i<nJets; i++) {
	// cut on Muon in jets from plot_muJet.cfg
#ifdef RUN_ON_BTAG
	Bool_t cut_mu_pass=false;
	if (muon1Pt[i]> cutMuonPt     && TMath::Abs(muon1Eta[i]) < 2.4 && muon1IsGlobal[i] == 1     && 
	    muon1GlobalMuonHits[i]> 0 && muon1NumberOfMatches[i]>1     && muon1InnerValidHits[i]>10 &&
	    muon1NPixelHits[i]> 1     && muon1NExpectedOuterHits[i]<3  && muon1InnerNChi2[i]< 10    &&
	    muon1GlobalNChi2[i]< 10   && muon1VzPVDist[i]< 2           && muon1DeltaR[i]< 0.4) {
	  cut_mu_pass=true;
	}
#else
	Bool_t cut_mu_pass=true;// BYPASS THIS CUT IF YOU DON'T CARE ABOUT MUON IN JET
#endif

#ifdef MAXJETPT
	if ( jetPt[i]<MAXJETPT )
#endif
	  if (jetPt[i]>cutJetPt && -2.4<jetEta[i] && jetEta[i]<2.4 && cut_mu_pass) {
	  for (int j=0; j<jmax; j++) {
	    int ok=0;
	    if (j==0) ok=1;  // everyone 
	    else if (j==1 && MCTrueFlavor[i]==5 && isBGluonSplitting==0) ok=1;   // b only
	    else if (j==2 && MCTrueFlavor[i]==4) ok=1;  // c only
	    else if (j==3 && (MCTrueFlavor[i]<=0 || MCTrueFlavor[i]==1 || MCTrueFlavor[i]==2 || MCTrueFlavor[i]==3 || MCTrueFlavor[i]==21 )) ok=1;  // light 
	    else if (j==4 && MCTrueFlavor[i]==5 && isBGluonSplitting==1) ok=1; // gluon splitting in b
	    if (ok==1) {
	      // HERE WE FILL THE HISTOGRAMS

	      HistoBtag[0+j*nhisto_to_clone]->Fill(jetPt[i], weight);      // alljetpt
	      if (numberOfPrimaryVertices<FIRST_NPV) {          
		HistoBtag[326+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		if( (220<jetPt[i]) && (jetPt[i] < 300) )   HistoBtag[327+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		if( (300<jetPt[i]) && (jetPt[i]< 400) )   HistoBtag[328+j*nhisto_to_clone]->Fill(jetPt[i], weight);
	      }
	      else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		HistoBtag[329+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		if( (220<jetPt[i]) && (jetPt[i] < 300) )   HistoBtag[330+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		if( (300<jetPt[i]) && (jetPt[i]< 400) )   HistoBtag[331+j*nhisto_to_clone]->Fill(jetPt[i], weight);
	      }
	      else {
		HistoBtag[332+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		if( (220<jetPt[i]) && (jetPt[i] < 300) )   HistoBtag[333+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		if( (300<jetPt[i]) && (jetPt[i]< 400) )    HistoBtag[334+j*nhisto_to_clone]->Fill(jetPt[i], weight);
	      }
	      //discri
	      HistoBtag[1+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
	      HistoBtag[2+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);

	      HistoBtag[3+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
	      HistoBtag[4+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
	      HistoBtag[5+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
	      HistoBtag[6+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);

	      HistoBtag[7+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
	      HistoBtag[8+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      HistoBtag[335+j*nhisto_to_clone]->Fill(standardCombinedSecondaryVertexPFBJetTags[i], weight);
	      HistoBtag[336+j*nhisto_to_clone]->Fill(standardCombinedSecondaryVertexMVAPFBJetTags[i], weight);

	      if (numberOfPrimaryVertices<FIRST_NPV) {          
		HistoBtag[283+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[284+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
			 
		HistoBtag[287+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[288+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[289+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[290+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
			 
		HistoBtag[285+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[286+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }
	      else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		HistoBtag[299+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[300+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
			 
		HistoBtag[303+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[304+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[305+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[306+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
			 
		HistoBtag[301+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[302+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }
	      else {
		HistoBtag[315+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[316+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
			 
		HistoBtag[319+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[320+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[321+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[322+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
			 
		HistoBtag[317+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[318+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }

	      // Sort by jetPt (50-80 80-120 120+)
	      if (50<jetPt[i] && jetPt[i]<80){
		HistoBtag[247+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[250+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
		HistoBtag[253+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[256+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[259+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[262+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[265+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[268+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }
	      if (80<jetPt[i] && jetPt[i]<120){
		HistoBtag[248+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[251+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
		HistoBtag[254+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[257+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[260+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[263+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[266+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[269+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }
	      if (120<jetPt[i]){
		HistoBtag[249+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[252+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
		HistoBtag[255+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[258+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[261+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[264+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);

		HistoBtag[267+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[270+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }
		       
	      // and same but when requiring a pTrel cut, to enrich the jets in b's
	      HistoBtag[172+j*nhisto_to_clone]->Fill(muon1PtRel[i],weight);
	      if(muon1PtRel[i]>bEnrichCutSal){		       
		HistoBtag[168+j*nhisto_to_clone]->Fill(standardTrackCountingHighEffPFBJetTags[i], weight);
		HistoBtag[169+j*nhisto_to_clone]->Fill(standardTrackCountingHighPurPFBJetTags[i], weight);
		HistoBtag[170+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighEffPFBJetTags[i], weight);
		HistoBtag[171+j*nhisto_to_clone]->Fill(standardSimpleSecondaryVertexHighPurPFBJetTags[i], weight);
		HistoBtag[173+j*nhisto_to_clone]->Fill(standardJetProbabilityPFBJetTags[i], weight);
		HistoBtag[174+j*nhisto_to_clone]->Fill(standardJetBProbabilityPFBJetTags[i], weight);
	      }

	      // info on tracks
	      int ntrack_jet=0;
              int nselectedtrack_jet=0;
	      int nseltrack_jet=0;
	      int ncutseltrack_jet=0;
	      int n1_ip=-1;
	      int n2_ip=-1;
	      int n3_ip=-1;
	      float sig1_ip=-9999;
	      float sig2_ip=-9999;
	      float sig3_ip=-9999;

	      for (int k=0; k<nTracks; k++) {  // LOOP ON THE TRACKS
		if (trackJetIndex[k]==i)  {    // TRACK ASSOCIATED TO THE JET
		  ntrack_jet++;
		  HistoBtag[10+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight);  //pt_tracks
		  // 9,11,12 : see below 
		  //on all tracks
		  HistoBtag[13+j*nhisto_to_clone]->Fill(trackNHits[k], weight);
		  HistoBtag[14+j*nhisto_to_clone]->Fill(trackNPixelHits[k], weight);
		  HistoBtag[15+j*nhisto_to_clone]->Fill(trackNormChi2[k], weight);
		  HistoBtag[16+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight);
		  HistoBtag[17+j*nhisto_to_clone]->Fill(trackLongitudinalImpactParameter[k], weight);
		  HistoBtag[18+j*nhisto_to_clone]->Fill(trackIP2d[k], weight);
		  HistoBtag[19+j*nhisto_to_clone]->Fill(TMath::Abs(trackDistJetAxis[k]), weight);
		  HistoBtag[20+j*nhisto_to_clone]->Fill(trackDecayLength[k], weight);
		  // cuts N
		  // trackNormChi2 < 5 && trackNHits >= 8 && trackNPixelHits >=2 && 
		  // TMath::Abs(trackLongitudinalImpactParameter) < 17  &&
		  // trackTransverseMomentum > 1 && TMath::Abs(trackIP2d) < 0.2 && 
		  // TMath::Abs(trackDistJetAxis) < 0.07 && trackDecayLength < 5
		  // cuts N-1
		  bool cut1=false;
		  bool cut2=false;
		  bool cut3=false;
		  bool cut4=false;
		  if (trackNormChi2[k] < 5 && trackNHits[k]>= 8 && trackNPixelHits[k] >=2) cut1=true;
		  if (TMath::Abs(trackLongitudinalImpactParameter[k]) < 17) cut2=true;
		  if (trackTransverseMomentum[k]> 1 && TMath::Abs(trackIP2d[k]) < 0.2) cut3=true;
		  if (TMath::Abs(trackDistJetAxis[k])< 0.07 && trackDecayLength[k]< 5) cut4=true;
		  if (trackSelected[k] && trackTransverseMomentum[k]>4 && fabs(trackEta[k])<1.0 ) {
		    Histo2DB[1+jmax*nhisto_to_clone2D]->Fill(trackPhi[k],trackIP3d[k], weight);
		    Histo2DB[2+jmax*nhisto_to_clone2D]->Fill(trackPhi[k],trackIP2d[k], weight);
		    Histo2DB[3+jmax*nhisto_to_clone2D]->Fill(trackPhi[k],trackIP3d[k]/trackIP3dError[k], weight);
		    Histo2DB[4+jmax*nhisto_to_clone2D]->Fill(trackPhi[k],trackIP2d[k]/trackIP2dError[k], weight);
		  }
		  if (trackNormChi2[k]< 5 && trackNPixelHits[k] >=2 && cut2 && cut3 && cut4) HistoBtag[21+j*nhisto_to_clone]->Fill(trackNHits[k], weight);
		  if (trackNormChi2[k]< 5 && trackNHits[k]>= 8      && cut2 && cut3 && cut4) HistoBtag[22+j*nhisto_to_clone]->Fill(trackNPixelHits[k], weight);
		  if (trackNHits[k]>= 8   && trackNPixelHits[k] >=2 && cut2 && cut3 && cut4) HistoBtag[23+j*nhisto_to_clone]->Fill(trackNormChi2[k], weight);
		  if (cut1 && cut2 && TMath::Abs(trackIP2d[k]) < 0.2 && cut4)                HistoBtag[24+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight);
		  if (cut1 && cut3 && cut4)                                                  HistoBtag[25+j*nhisto_to_clone]->Fill(trackLongitudinalImpactParameter[k], weight);
		  if (cut1 && cut2 && trackTransverseMomentum[k]> 1 && cut4)                 HistoBtag[26+j*nhisto_to_clone]->Fill(trackIP2d[k], weight);
		  if (cut1 && cut2 && cut3 && trackDecayLength[k]< 5)                        HistoBtag[27+j*nhisto_to_clone]->Fill(TMath::Abs(trackDistJetAxis[k]), weight);
		  if (cut1 && cut2 && cut3 && TMath::Abs(trackDistJetAxis[k])< 0.07)         HistoBtag[28+j*nhisto_to_clone]->Fill(trackDecayLength[k], weight);
		  HistoBtag[29+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		  HistoBtag[30+j*nhisto_to_clone]->Fill(trackIP3dError[k], weight);
		  HistoBtag[31+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
		  // 32 -34 : IP seltracks
		  // 35 -37 : IP cutseltracks

		  //depending on #PV
		  if (numberOfPrimaryVertices<FIRST_NPV) {          
		    HistoBtag[97+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight);  //pt_tracks
		  }
		  else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		    HistoBtag[111+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight);  //pt_tracks
		  }
		  else {
		    HistoBtag[125+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight);  //pt_tracks
		  }
		  if (trackSelected[k]==true) { // selected tracks
		    nseltrack_jet++;
		    HistoBtag[12+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    HistoBtag[32+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    HistoBtag[33+j*nhisto_to_clone]->Fill(trackIP3dError[k], weight);
		    HistoBtag[34+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);

		    //depending on #PV
		    if (numberOfPrimaryVertices<FIRST_NPV) {          
		      HistoBtag[98+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    }
		    else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		      HistoBtag[112+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    }
		    else {
		      HistoBtag[126+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    }
		  }

		  // apply cut and sort IPsig
		  if (trackSelected[k] && TMath::Abs(trackDistJetAxis[k]) < 0.07 && trackDecayLength[k] < 5   && trackDecayLength[k]>-1 && trackIP3dError[k]!=0.) {
		    ncutseltrack_jet++;
		    HistoBtag[35+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    HistoBtag[36+j*nhisto_to_clone]->Fill(trackIP3dError[k], weight);
		    HistoBtag[37+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
		    HistoBtag[135+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks

		    if (1<trackTransverseMomentum[k] && trackTransverseMomentum[k]<=2) HistoBtag[139+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    if (2<trackTransverseMomentum[k] && trackTransverseMomentum[k]<=5) HistoBtag[140+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    if (5<trackTransverseMomentum[k] && trackTransverseMomentum[k]<=8) HistoBtag[141+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    if (8<trackTransverseMomentum[k] && trackTransverseMomentum[k]<=12) HistoBtag[142+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    if (12<trackTransverseMomentum[k] && trackTransverseMomentum[k]<=20) HistoBtag[143+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		    if (20<trackTransverseMomentum[k] && trackTransverseMomentum[k]<=50) HistoBtag[144+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);

		    // Sort by jetPt
		    if (50<jetPt[i] && jetPt[i]<80) {
		      HistoBtag[175+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		      HistoBtag[187+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
		      HistoBtag[199+j*nhisto_to_clone]->Fill(trackIP3dError[k], weight);
		    }
		    if (80<jetPt[i] && jetPt[i]<120) {
		      HistoBtag[176+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		      HistoBtag[188+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
		      HistoBtag[200+j*nhisto_to_clone]->Fill(trackIP3dError[k], weight);
		    }
		    if (120<jetPt[i]) {
		      HistoBtag[177+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
		      HistoBtag[189+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
		      HistoBtag[201+j*nhisto_to_clone]->Fill(trackIP3dError[k], weight);
		    }
		    //depending on #PV
		    if (numberOfPrimaryVertices<FIRST_NPV) {          
		      HistoBtag[99+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    }
		    else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		      HistoBtag[113+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    }
		    else {
		      HistoBtag[127+j*nhisto_to_clone]->Fill(trackTransverseMomentum[k], weight); //pt_seltracks
		    }

		    Float_t sig=trackIP3d[k]/trackIP3dError[k];
		    if (sig>sig1_ip) {
		      sig3_ip=sig2_ip;
		      sig2_ip=sig1_ip;
		      sig1_ip=sig;
		      n3_ip=n2_ip;
		      n2_ip=n1_ip;
		      n1_ip=k;
		    }
		    else if (sig>sig2_ip) {
		      sig3_ip=sig2_ip;
		      sig2_ip=sig;
		      n3_ip=n2_ip;
		      n2_ip=k;
		    }
		    else if (sig>sig3_ip) {
		      sig3_ip=sig;
		      n3_ip=k;
		    }
		    if ( nselectedtrack_jet<3 && trackSelected[k] ) {
		      if (TMath::Abs(trackDistJetAxis[k]) < 0.07 && trackDecayLength[k] < 5 && trackIP3d[k]>-99 && trackIP3dError[k]>-0.5) {  
			HistoBtag[38+nselectedtrack_jet*3+j*nhisto_to_clone]->Fill(trackIP3d[k], weight);
			HistoBtag[39+nselectedtrack_jet*3+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
			HistoBtag[40+nselectedtrack_jet*3+j*nhisto_to_clone]->Fill(trackIP3d[k]/trackIP3dError[k], weight);
		      }
		      nselectedtrack_jet++;
		    }
		  }  
		} // end trackJetIndex
	      } // end nTracks loop
	      HistoBtag[9+j*nhisto_to_clone]->Fill(ntrack_jet, weight);  // ntracks_jet
	      HistoBtag[11+j*nhisto_to_clone]->Fill(nseltrack_jet, weight); // nseltracks_jet


	      // tracks sorted by ip
	      if (n1_ip>-1)  {
		HistoBtag[47+j*nhisto_to_clone]->Fill(trackIP3d[n1_ip], weight);
		HistoBtag[48+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		HistoBtag[49+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		HistoBtag[136+j*nhisto_to_clone]->Fill(trackIP3dError[n1_ip], weight);

		if (1<trackTransverseMomentum[n1_ip] && trackTransverseMomentum[n1_ip]<=2)HistoBtag[151+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		if (2<trackTransverseMomentum[n1_ip] && trackTransverseMomentum[n1_ip]<=5)HistoBtag[152+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		if (5<trackTransverseMomentum[n1_ip] && trackTransverseMomentum[n1_ip]<=8)HistoBtag[153+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		if (8<trackTransverseMomentum[n1_ip] && trackTransverseMomentum[n1_ip]<=12)HistoBtag[154+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		if (12<trackTransverseMomentum[n1_ip] && trackTransverseMomentum[n1_ip]<=20)HistoBtag[155+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		if (20<trackTransverseMomentum[n1_ip] && trackTransverseMomentum[n1_ip]<=50)HistoBtag[156+j*nhisto_to_clone]->Fill(sig1_ip, weight);

		// Sort by jetPt
		if (50<jetPt[i] && jetPt[i]<80) {
		  HistoBtag[178+j*nhisto_to_clone]->Fill(trackIP3d[n1_ip], weight);
		  HistoBtag[190+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		  HistoBtag[202+j*nhisto_to_clone]->Fill(trackIP3dError[n1_ip], weight);
		}
		if (80<jetPt[i] && jetPt[i]<120) {
		  HistoBtag[179+j*nhisto_to_clone]->Fill(trackIP3d[n1_ip], weight);
		  HistoBtag[191+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		  HistoBtag[203+j*nhisto_to_clone]->Fill(trackIP3dError[n1_ip], weight);
		}
		if (120<jetPt[i]) {
		  HistoBtag[180+j*nhisto_to_clone]->Fill(trackIP3d[n1_ip], weight);
		  HistoBtag[192+j*nhisto_to_clone]->Fill(sig1_ip, weight);
		  HistoBtag[204+j*nhisto_to_clone]->Fill(trackIP3dError[n1_ip], weight);
		}
			  
		// add sorted
		//  std::cout<<"trackIP3d[n1_ip]/trackIP3dError[n1_ip] - sig1_ip = " << trackIP3d[n1_ip]/trackIP3dError[n1_ip] - sig1_ip << std::endl;
	      }
                     

	      if (n2_ip>-1) {
		HistoBtag[50+j*nhisto_to_clone]->Fill(trackIP3d[n2_ip], weight);
		HistoBtag[51+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		HistoBtag[52+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		HistoBtag[137+j*nhisto_to_clone]->Fill(trackIP3dError[n2_ip], weight);

		if (1<trackTransverseMomentum[n2_ip] && trackTransverseMomentum[n2_ip]<=2)HistoBtag[157+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if (2<trackTransverseMomentum[n2_ip] && trackTransverseMomentum[n2_ip]<=5)HistoBtag[158+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if (5<trackTransverseMomentum[n2_ip] && trackTransverseMomentum[n2_ip]<=8)HistoBtag[159+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if (8<trackTransverseMomentum[n2_ip] && trackTransverseMomentum[n2_ip]<=12)HistoBtag[160+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if (12<trackTransverseMomentum[n2_ip] && trackTransverseMomentum[n2_ip]<=20)HistoBtag[161+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if (20<trackTransverseMomentum[n2_ip] && trackTransverseMomentum[n2_ip]<=50)HistoBtag[162+j*nhisto_to_clone]->Fill(sig2_ip, weight);

		// Sort by jetPt
		if (50<jetPt[i] && jetPt[i]<80) {
		  HistoBtag[181+j*nhisto_to_clone]->Fill(trackIP3d[n2_ip], weight);
		  HistoBtag[193+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		  HistoBtag[205+j*nhisto_to_clone]->Fill(trackIP3dError[n2_ip], weight);
		}
		if (80<jetPt[i] && jetPt[i]<120) {
		  HistoBtag[182+j*nhisto_to_clone]->Fill(trackIP3d[n2_ip], weight);
		  HistoBtag[194+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		  HistoBtag[206+j*nhisto_to_clone]->Fill(trackIP3dError[n2_ip], weight);
		}
		if (120<jetPt[i]) {
		  HistoBtag[183+j*nhisto_to_clone]->Fill(trackIP3d[n2_ip], weight);
		  HistoBtag[195+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		  HistoBtag[207+j*nhisto_to_clone]->Fill(trackIP3dError[n2_ip], weight);
		}
			  
		if(muon1PtRel[i]>bEnrichCutSal && n2_ip>-1){
		  HistoBtag[164+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		  HistoBtag[165+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		}
	      }
                
		  

	      if (n3_ip>-1) {
		HistoBtag[53+j*nhisto_to_clone]->Fill(trackIP3d[n3_ip], weight);
		HistoBtag[54+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		HistoBtag[55+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		HistoBtag[138+j*nhisto_to_clone]->Fill(trackIP3dError[n3_ip], weight);

		// Sort by jetPt
		if (50<jetPt[i] && jetPt[i]<80) {
		  HistoBtag[184+j*nhisto_to_clone]->Fill(trackIP3d[n3_ip], weight);
		  HistoBtag[196+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		  HistoBtag[208+j*nhisto_to_clone]->Fill(trackIP3dError[n3_ip], weight);
		}
		if (80<jetPt[i] && jetPt[i]<120) {
		  HistoBtag[185+j*nhisto_to_clone]->Fill(trackIP3d[n3_ip], weight);
		  HistoBtag[197+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		  HistoBtag[209+j*nhisto_to_clone]->Fill(trackIP3dError[n3_ip], weight);
		}
		if (120<jetPt[i]) {
		  HistoBtag[186+j*nhisto_to_clone]->Fill(trackIP3d[n3_ip], weight);
		  HistoBtag[198+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		  HistoBtag[210+j*nhisto_to_clone]->Fill(trackIP3dError[n3_ip], weight);
		}
			  
		if(muon1PtRel[i]>bEnrichCutSal){
		  HistoBtag[166+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		  HistoBtag[167+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		}
	      }
                    
	      Histo2DB[3+j*nhisto_to_clone2D]->Fill(jetPt[i],ntrack_jet, weight);
	      Histo2DB[4+j*nhisto_to_clone2D]->Fill(jetPt[i],nseltrack_jet, weight);
	      Histo2DB[5+j*nhisto_to_clone2D]->Fill(jetPt[i],ncutseltrack_jet, weight);

	      // SV
	      Histo2DB[0+j*nhisto_to_clone2D]->Fill(jetPt[i],SVnVertexTracks[i], weight);
	      HistoBtag[56+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
	      if(SVnVertices[i]>0) {
		HistoBtag[57+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		HistoBtag[58+j*nhisto_to_clone]->Fill(jetPt[i], weight);
		HistoBtag[59+j*nhisto_to_clone]->Fill(SVNormChi2[i], weight);

		HistoBtag[60+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		// 61 see below

		HistoBtag[62+j*nhisto_to_clone]->Fill(SVnVertexTracks[i], weight);
		HistoBtag[63+j*nhisto_to_clone]->Fill(SVnFirstVertexTracks[i], weight);
		HistoBtag[64+j*nhisto_to_clone]->Fill(SVnSelectedTracks[i], weight);

		// Centered integer binned for multiplicity plots
		HistoBtag[274+j*nhisto_to_clone]->Fill(SVnVertexTracks[i], weight);
		HistoBtag[275+j*nhisto_to_clone]->Fill(SVnFirstVertexTracks[i], weight);
		HistoBtag[276+j*nhisto_to_clone]->Fill(SVnSelectedTracks[i], weight);

		HistoBtag[65+j*nhisto_to_clone]->Fill(SVIPFirstAboveCharm[i], weight);
		HistoBtag[66+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);

		HistoBtag[67+j*nhisto_to_clone]->Fill(SV2dDistance[i]/SV2dDistanceError[i], weight);
		HistoBtag[68+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		if(muon1PtRel[i]>bEnrichCutSal){		 
		  HistoBtag[163+j*nhisto_to_clone]->Fill(SVMass[i], weight);      
		  HistoBtag[166+j*nhisto_to_clone]->Fill(SV2dDistance[i]/SV2dDistanceError[i], weight);
		  HistoBtag[167+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV2dDistanceError[i], weight);

		}

		HistoBtag[69+j*nhisto_to_clone]->Fill(SV2dDistance[i], weight);
		HistoBtag[70+j*nhisto_to_clone]->Fill(SV3dDistance[i], weight);

		if (numberOfPrimaryVertices<FIRST_NPV) {          
		  HistoBtag[278+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[279+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight);
		  HistoBtag[280+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		  HistoBtag[281+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);
		  HistoBtag[282+j*nhisto_to_clone]->Fill(SVvtxPt[i], weight);
		}
		else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		  HistoBtag[294+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[295+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight);
		  HistoBtag[296+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		  HistoBtag[297+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);
		  HistoBtag[298+j*nhisto_to_clone]->Fill(SVvtxPt[i], weight);
		}
		else {
		  HistoBtag[310+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[311+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight); 
		  HistoBtag[312+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		  HistoBtag[313+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);
		  HistoBtag[314+j*nhisto_to_clone]->Fill(SVvtxPt[i], weight);
		}


		if (SVnVertexTracks[i]>=3) {

		  // 3-track vertex variables
		  HistoBtag[61+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  HistoBtag[71+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[271+j*nhisto_to_clone]->Fill(SVIPFirstAboveCharm[i], weight);
		  HistoBtag[272+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[273+j*nhisto_to_clone]->Fill(SVvtxPt[i], weight);

		  // Sory by jetPt
		  if (50<jetPt[i] && jetPt[i]<80){
		    HistoBtag[214+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		    HistoBtag[235+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  }
		  if (80<jetPt[i] && jetPt[i]<120){
		    HistoBtag[215+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		    HistoBtag[236+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  }
		  if (120<jetPt[i]){
		    HistoBtag[216+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		    HistoBtag[237+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  }
			     
		}

		HistoBtag[72+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight);
		HistoBtag[73+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		HistoBtag[74+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);

		// vertex deltaRs vs jet pt (do ->ProfileX once created)
		Histo2DB[7+j*nhisto_to_clone2D]->Fill(jetPt[i], SVjetDeltaR[i], weight);
		Histo2DB[8+j*nhisto_to_clone2D]->Fill(jetPt[i], SVvtxSumJetDeltaR[i], weight);
		Histo2DB[9+j*nhisto_to_clone2D]->Fill(jetPt[i], SVvtxSumVtxDirDeltaR[i], weight);
			
		HistoBtag[75+j*nhisto_to_clone]->Fill(SVvtxSumDirEta[i], weight);
		HistoBtag[76+j*nhisto_to_clone]->Fill(SVvtxSumDirPhi[i], weight);
		HistoBtag[77+j*nhisto_to_clone]->Fill(SVvtxDirEta[i], weight);
		HistoBtag[78+j*nhisto_to_clone]->Fill(SVvtxDirPhi[i], weight);

		HistoBtag[79+j*nhisto_to_clone]->Fill(SVvtxSumPTrel[i], weight);
		HistoBtag[80+j*nhisto_to_clone]->Fill(SVvtxDirPTrel[i], weight);
		HistoBtag[81+j*nhisto_to_clone]->Fill(SVtotCharge[i], weight);
		HistoBtag[82+j*nhisto_to_clone]->Fill(SVvtxPt[i], weight);

		Histo2DB[1+j*nhisto_to_clone2D]->Fill(SV3dDistance[i]/SV3dDistanceError[i], SVMass[i], weight);
		Histo2DB[2+j*nhisto_to_clone2D]->Fill(SV3dDistance[i], SVMass[i], weight);

		Histo2DB[12+j*nhisto_to_clone2D]->Fill(jetPt[i], SVEnergyRatio[i] , weight);
		Histo2DB[13+j*nhisto_to_clone2D]->Fill(jetPt[i], SVvtxPt[i] , weight);

		// Sort by jetPt (50-80 80-120 120+)
		if (50<jetPt[i] && jetPt[i]<80){
		  HistoBtag[211+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  HistoBtag[217+j*nhisto_to_clone]->Fill(SVIPFirstAboveCharm[i], weight);
		  HistoBtag[220+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[223+j*nhisto_to_clone]->Fill(SV2dDistance[i]/SV2dDistanceError[i], weight);
		  HistoBtag[226+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[229+j*nhisto_to_clone]->Fill(SV2dDistance[i], weight);
		  HistoBtag[232+j*nhisto_to_clone]->Fill(SV3dDistance[i], weight);
		  HistoBtag[238+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight);
		  HistoBtag[241+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		  HistoBtag[244+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);
		}
		if (80<jetPt[i] && jetPt[i]<120){
		  HistoBtag[212+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  HistoBtag[218+j*nhisto_to_clone]->Fill(SVIPFirstAboveCharm[i], weight);
		  HistoBtag[221+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[224+j*nhisto_to_clone]->Fill(SV2dDistance[i]/SV2dDistanceError[i], weight);
		  HistoBtag[227+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[230+j*nhisto_to_clone]->Fill(SV2dDistance[i], weight);
		  HistoBtag[233+j*nhisto_to_clone]->Fill(SV3dDistance[i], weight);
		  HistoBtag[239+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight);
		  HistoBtag[242+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		  HistoBtag[245+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);
		}
		if(120<jetPt[i]){
		  HistoBtag[213+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  HistoBtag[219+j*nhisto_to_clone]->Fill(SVIPFirstAboveCharm[i], weight);
		  HistoBtag[221+j*nhisto_to_clone]->Fill(SVEnergyRatio[i], weight);
		  HistoBtag[225+j*nhisto_to_clone]->Fill(SV2dDistance[i]/SV2dDistanceError[i], weight);
		  HistoBtag[228+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[231+j*nhisto_to_clone]->Fill(SV2dDistance[i], weight);
		  HistoBtag[234+j*nhisto_to_clone]->Fill(SV3dDistance[i], weight);
		  HistoBtag[240+j*nhisto_to_clone]->Fill(SVjetDeltaR[i], weight);
		  HistoBtag[243+j*nhisto_to_clone]->Fill(SVvtxSumJetDeltaR[i], weight);
		  HistoBtag[246+j*nhisto_to_clone]->Fill(SVvtxSumVtxDirDeltaR[i], weight);
		}

		//Avg SVMass vs jetpt (do ->ProfileX once created)
		Histo2DB[6+j*nhisto_to_clone2D]->Fill(jetPt[i], SVMass[i], weight);
			
	      } // end cut SVnVertices
                // muon info
	      HistoBtag[83+j*nhisto_to_clone]->Fill(nMuons[i], weight);

	      // Centered at integer for muon multiplicity
	      HistoBtag[277+j*nhisto_to_clone]->Fill(nMuons[i], weight);

	      HistoBtag[84+j*nhisto_to_clone]->Fill(muon1Pt[i], weight);
	      HistoBtag[85+j*nhisto_to_clone]->Fill(muon1PtRel[i], weight);
	      HistoBtag[86+j*nhisto_to_clone]->Fill(muon1Ip3d[i], weight);
	      HistoBtag[87+j*nhisto_to_clone]->Fill(muon1Ip2d[i], weight);
	      HistoBtag[88+j*nhisto_to_clone]->Fill(muon1Sip3d[i], weight);
	      HistoBtag[89+j*nhisto_to_clone]->Fill(muon1Sip2d[i], weight);
	      HistoBtag[90+j*nhisto_to_clone]->Fill(muon1DeltaR[i], weight);
	      HistoBtag[91+j*nhisto_to_clone]->Fill(muon1Ratio[i], weight);


	      if (numberOfPrimaryVertices<FIRST_NPV) {          
		HistoBtag[291+j*nhisto_to_clone]->Fill(muon1Pt[i], weight);
		HistoBtag[292+j*nhisto_to_clone]->Fill(muon1PtRel[i], weight);
		HistoBtag[293+j*nhisto_to_clone]->Fill(muon1DeltaR[i], weight);
	      }
	      else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		HistoBtag[307+j*nhisto_to_clone]->Fill(muon1Pt[i], weight);
		HistoBtag[308+j*nhisto_to_clone]->Fill(muon1PtRel[i], weight);
		HistoBtag[309+j*nhisto_to_clone]->Fill(muon1DeltaR[i], weight);
	      }
	      else {
		HistoBtag[323+j*nhisto_to_clone]->Fill(muon1Pt[i], weight);
		HistoBtag[324+j*nhisto_to_clone]->Fill(muon1PtRel[i], weight);
		HistoBtag[325+j*nhisto_to_clone]->Fill(muon1DeltaR[i], weight);
	      }

	      //muon deltaR,ptrel vs jet pt (do -ProfileX once created)
	      Histo2DB[10+j*nhisto_to_clone2D]->Fill(jetPt[i], muon1PtRel[i], weight);
	      Histo2DB[11+j*nhisto_to_clone2D]->Fill(jetPt[i], muon1DeltaR[i], weight);
		
	      HistoBtag[134+j*nhisto_to_clone]->Fill(ncutseltrack_jet, weight); // nseltracks_jet
	      // depending on #PV
	      if (numberOfPrimaryVertices<FIRST_NPV) {          
		if(sig2_ip> -9000) HistoBtag[92+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if(sig3_ip> -9000) HistoBtag[93+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		HistoBtag[94+j*nhisto_to_clone]->Fill(ntrack_jet, weight);  // ntracks_jet
		HistoBtag[95+j*nhisto_to_clone]->Fill(nseltrack_jet, weight); // nseltracks_jet
		HistoBtag[96+j*nhisto_to_clone]->Fill(ncutseltrack_jet, weight); // nseltracks_jet
		// 97 : see above in track loop
		// 98 : see above in track loop
		// 99 : see above in track loop
		HistoBtag[100+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		int nnsv3tr=0;
		if (SVnVertexTracks[i]>=3) nnsv3tr=SVnVertices[i];
		HistoBtag[148+j*nhisto_to_clone]->Fill(nnsv3tr, weight);
		if(SVnVertices[i]>0) {
		  HistoBtag[101+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		  HistoBtag[102+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[104+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  if (SVnVertexTracks[i]>=3) {
		    HistoBtag[103+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		    HistoBtag[105+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		    HistoBtag[145+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		  }
		}
	      }
	      else if (numberOfPrimaryVertices>=FIRST_NPV && numberOfPrimaryVertices<=SECOND_NPV) {
		if(sig2_ip> -9000) HistoBtag[106+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if(sig3_ip> -9000) HistoBtag[107+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		HistoBtag[108+j*nhisto_to_clone]->Fill(ntrack_jet, weight);  // ntracks_jet
		HistoBtag[109+j*nhisto_to_clone]->Fill(nseltrack_jet, weight); // nseltracks_jet
		HistoBtag[110+j*nhisto_to_clone]->Fill(ncutseltrack_jet, weight); // nseltracks_jet
		// 111 : see above in track loop
		// 112 : see above in track loop
		// 113 : see above in track loop
		HistoBtag[114+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		int nnsv3tr=0;
		if (SVnVertexTracks[i]>=3) nnsv3tr=SVnVertices[i];
		HistoBtag[149+j*nhisto_to_clone]->Fill(nnsv3tr, weight);
		if(SVnVertices[i]>0) {
		  HistoBtag[115+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		  HistoBtag[116+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[118+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  if (SVnVertexTracks[i]>=3) {
		    HistoBtag[117+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		    HistoBtag[119+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		    HistoBtag[146+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		  }
		}
	      }
	      else {
		if(sig2_ip> -9000) HistoBtag[120+j*nhisto_to_clone]->Fill(sig2_ip, weight);
		if(sig3_ip> -9000) HistoBtag[121+j*nhisto_to_clone]->Fill(sig3_ip, weight);
		HistoBtag[122+j*nhisto_to_clone]->Fill(ntrack_jet, weight);  // ntracks_jet
		HistoBtag[123+j*nhisto_to_clone]->Fill(nseltrack_jet, weight); // nseltracks_jet
		HistoBtag[124+j*nhisto_to_clone]->Fill(ncutseltrack_jet, weight); // nseltracks_jet
		// 125 : see above in track loop
		// 126 : see above in track loop
		// 127 : see above in track loop
		HistoBtag[128+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		int nnsv3tr=0;
		if (SVnVertexTracks[i]>=3) nnsv3tr=SVnVertices[i];
		HistoBtag[150+j*nhisto_to_clone]->Fill(nnsv3tr, weight);
		if(SVnVertices[i]>0) {
		  HistoBtag[129+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		  HistoBtag[130+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		  HistoBtag[132+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		  if (SVnVertexTracks[i]>=3) {
		    HistoBtag[131+j*nhisto_to_clone]->Fill(SV3dDistance[i]/SV3dDistanceError[i], weight);
		    HistoBtag[133+j*nhisto_to_clone]->Fill(SVMass[i], weight);
		    HistoBtag[147+j*nhisto_to_clone]->Fill(SVnVertices[i], weight);
		  }
		}
	      }
	      // FILL HERE YOUR NEW HISTO in "j" loop
	      // example :
	      // float caro_new_info=0.6;
	      // HistoBtag[151+j*nhisto_to_clone]->Fill(caro_new_info, weight); // the "150" of "nnsv3tr" becomes here a "150" for "new_histo"
	    } // end cut ok

	  } // end loop j : quark composition
	} //jet cut (Pt, Eta)
      } // end loop i : Jet
    } // cut trigger
 } // loop event

  clog<<"\r"<<"Calculating "<<nentries<<"/"<<nentries<<"(100/100%)"<<endl;
  TString rootfiletofill = final_dir+"/"+resultfilename+".root";
  TFile* fout  = new TFile(rootfiletofill,"RECREATE");
  for (unsigned int i=0; i<HistoBtag.size(); i++) {
    HistoBtag[i]->Write();
  }
  for (unsigned int i=0; i<Histo2DB.size(); i++) {
    Histo2DB[i]->Write();
  }
  fout->Close();

  for (unsigned int i=0; i<HistoBtag.size(); i++) {
    HistoBtag[i]=0;
  }
  HistoBtag.clear();
  for (unsigned int i=0; i<Histo2DB.size(); i++) {
    Histo2DB[i]=0;
  }
  Histo2DB.clear();
}


void Plotter::AddHisto(vector<TH1F*> &HistoBtag, string name, const int& j, string title, const int& nbins, const Float_t& min, const Float_t& max)  {
        
  char namehisto[35];
  if (j==0) sprintf(namehisto,"%s",name.c_str());
  else if (j==1) sprintf(namehisto,"%s_b",name.c_str());
  else if (j==2) sprintf(namehisto,"%s_c",name.c_str());
  else if (j==3) sprintf(namehisto,"%s_udsg",name.c_str());
  else if (j==4) sprintf(namehisto,"%s_gspl",name.c_str());
  TH1F* h = new TH1F(namehisto,title.c_str(),nbins,min,max);
  h->Sumw2();
  HistoBtag.push_back(h);
}
void Plotter::AddHisto2D(vector<TH2F*> &Histo2DB, string name, const int& j, string title, const int& nbins, const Float_t& min, const Float_t& max, const int& nbinsy, const Float_t& miny, const Float_t& maxy)  {
        
  char namehisto[35];
  if (j==0) sprintf(namehisto,"%s",name.c_str());
  else if (j==1) sprintf(namehisto,"%s_b",name.c_str());
  else if (j==2) sprintf(namehisto,"%s_c",name.c_str());
  else if (j==3) sprintf(namehisto,"%s_udsg",name.c_str());
  else if (j==4) sprintf(namehisto,"%s_gspl",name.c_str());
  TH2F* h1 = new TH2F(namehisto,title.c_str(),nbins,min,max,nbinsy,miny,maxy);
  h1->Sumw2();
  Histo2DB.push_back(h1);
}

#ifdef EarlyAnalysis
void predefinedpuweights() {
  // 499 pb-1
#ifdef RUN_ON_JET

    puweight[0]=0.156498;
    puweight[1]=0.348279;
    puweight[2]=0.931435;
    puweight[3]=1.38161;
    puweight[4]=1.91335;
    puweight[5]=1.8012;
    puweight[6]=1.77467;
    puweight[7]=1.70689;
    puweight[8]=1.25496;
    puweight[9]=0.849071;
    puweight[10]=0.560916;
    puweight[11]=0.410454;
    puweight[12]=0.292295;
    puweight[13]=0.236444;
    puweight[14]=0.186692;
    puweight[15]=0.151324;
    puweight[16]=0.12243;
    puweight[17]=0.0901566;
    puweight[18]=0.0918909;
    puweight[19]=0.12913;
    puweight[20]=0.200706;
    puweight[21]=0.181146;
    puweight[22]=0.341892;
    puweight[23]=0.0849563;
    puweight[24]=0.0416172;
    puweight[25]=0.;
#endif
}
#endif

int main( int argc, char* argv[] ) {
  char tmpstr[300];
#ifdef RUN_ON_JET
  sprintf(tmpstr,"dir_jet%ipu",PTVAL);
  final_dir=TString(tmpstr);
#else
  sprintf(tmpstr,"dir_btag%ipu",PTVAL);
  final_dir=TString(tmpstr);
#endif

  TString action = "mkdir "+final_dir;
  system(action);

  for (int n = 1; n < argc; n++)
    if ( strstr(argv[n],"data")!=NULL ) {
#if DATAYEAR >= 2011
      cout << " ----> DATA" <<DATAYEAR<< endl;
#ifdef RUN_ON_JET
      if (strlen(JetDATA)>0) { 
	Plotter yt(JetDATA);
	sprintf(tmpstr,"histo_minijet%d",DATAYEAR);
	yt.Loop( true, 1.,string(tmpstr) ); //weight=1
      }
#else
      if (strlen(MetDATA)>0){ 
	Plotter yt(MetDATA);    
	sprintf(tmpstr,"histo_minibtag%d",DATAYEAR);
	yt.Loop(true, 1.,string(tmpstr) ); //weight=1
	system(action);
      }
#endif
#endif

    }


#if defined(npu_probs) && defined(pudistribution_data_filename)
  for(Byte_t npu=0; npu<MAXPU+1; ++npu)
    puweight[npu]=npu_probs[npu]>0?npu_probs[npu]:0.;
  TFile *filepuest = new TFile(pudistribution_data_filename,"READ");
  TH1D* npu_data= (TH1D*) filepuest->Get("pileup");
  npu_data->Scale(1/npu_data->Integral());//normalize to ONE
  for(Byte_t npu=0; npu<MAXPU+1; ++npu)
   if defined(npu_probs) && defined(pudistribution_data_filename) puweight[npu]=npu_data->GetBinContent(npu_data->FindBin(npu))/puweight[npu];
#endif  
#if !defined(npu_probs) && defined(pudistribution_data_filename)

  TH1D* npu_mc=new TH1D("pu_distribution","pu_distribution;# of PU; entries",MAXPU+1,-0.5,MAXPU+0.5);


  const UInt_t nSamples=sizeof(MC_Weights)/sizeof(Float_t);
  for (UInt_t iSample=0;iSample<nSamples;iSample++) 
    for (int n = 1; n < argc; n++)
      if ( strcmp(argv[n],MC_SampleNames[iSample])==0 &&
	   strlen(MC_files[iSample])>0
	   ) {
	printf(" ----> MC : %s (weight=%f)\n",MC_SampleNames[iSample],MC_Weights[iSample]);
	Plotter mc(MC_files[iSample]);

	mc.PileUpDistribution(npu_mc);
	npu_mc->Scale(1/npu_mc->Integral());    
	
	
        TFile *filepuest = new TFile(pudistribution_data_filename,"READ");
        TH1D* npu_data= (TH1D*) filepuest->Get("pileup");
        npu_data->Scale(1/npu_data->Integral());//normalize to ONE
	
	for(Byte_t npu=0; npu<MAXPU+1; ++npu) {
	  
	  Double_t numberofpu_mc=npu_mc->GetBinContent(npu_mc->FindBin(npu));
	  //if ( numberofpu_mc>0 ) puweight[npu]=numberofpu_mc;
	  //else puweight[npu]=0; //printf("%d:%f/%f=%f;\n",npu,npu_data->GetBinContent(npu_data->FindBin(npu)),numberofpu_mc,puweight[npu]);
	  if (numberofpu_mc!=0)puweight[npu]=npu_data->GetBinContent(npu_data->FindBin(npu))/numberofpu_mc;
	  else numberofpu_mc=0;
	}
	
	clog<<"Pileup weights:"<<endl;
	for(Byte_t npu=0; npu<MAXPU+1; ++npu)
	  clog<<Int_t(npu)<<" : "<<puweight[npu]<<endl;
	mc.Loop(false, MC_Weights[iSample],string("histo_")+string(MC_SampleNames[iSample]));
      }
      
#endif
  return 0;
}

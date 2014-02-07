#include "LoadDisplayRootPlot.h"
#include <TROOT.h>
void run(){
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
#ifdef RUN_ON_JET
  //  Plots1D P1D(false,PTVAL,DATAYEAR);
  Plots2D P2D(false,PTVAL,DATAYEAR);
#else
  Plots1D P1D(true,PTVAL,DATAYEAR);
  Plots2D P2D(true,PTVAL,DATAYEAR);
#endif
  // void PlotStack(TString selection, TString label, bool down=false, bool logy=false, bool bOverflow=false, TString labely="jets");
  //  for Jet dataset
#ifdef RUN_ON_JET
    P1D.PlotStack("npv","number of PV", 1, 0,1);
    P1D.PlotStack("npv_no_scaled","number of PV", 1, 0,1);
    P1D.PlotStack("all_jetpt","jet p_{T}",1, 1, 1);

    P1D.PlotStack("discri_tche","TCHE discriminator",1, 1, 1);
    P1D.PlotStack("discri_tchp","TCHP discriminator",1, 1, 1);
    P1D.PlotStack("discri_ssche0","SSVHE discriminator", 1, 1, 1);
    P1D.PlotStack("discri_ssche", "SSVHE discriminator", 1, 0, 0);
    P1D.PlotStack("discri_sschp0","SSVHP discriminator", 1, 1, 1);
    P1D.PlotStack("discri_sschp", "SSVHP discriminator", 1, 0, 0);
    P1D.PlotStack("discri_jetprob", "JP discriminator", 1, 1, 1);
    P1D.PlotStack("discri_jetbprob", "JBP discriminator",1, 1, 1);
    P1D.PlotStack("discri_csv", "CSV discriminator",1, 1, 1);
    P1D.PlotStack("discri_csvmva", "CSVMVA discriminator",1, 1, 1);

    P1D.PlotStack("n_tracks_jet", "nr. of tracks in jet",  1, 0, 1);
    P1D.PlotStack("n_seltracks_jet","nr. of selected tracks",  1, 0, 1);
    P1D.PlotStack("pt_tracks", "track p_{T}", 1, 1, 1);
    P1D.PlotStack("pt_seltracks", "track p_{T}", 1, 1, 1);

    // IP
    P1D.PlotStack("IP3d1",  "1st track 3D IP", 1, 1, 1);
    P1D.PlotStack("IP3d2",  "2nd track 3D IP",1, 1, 1);
    P1D.PlotStack("IP3d3",  "3rd track 3D IP",1, 1, 1);
    P1D.PlotStack("IP3d1sig", "1st track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d2sig", "2nd track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d3sig", "3rd track 3D IP significance", 1, 1, 1);

    P1D.PlotStack("IP3d1sorted","1st track 3D IP",  1, 1, 1);
    P1D.PlotStack("IP3d2sorted","2nd track 3D IP",  1, 1, 1);
    P1D.PlotStack("IP3d3sorted","3rd track 3D IP",  1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted", "1st track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted", "2nd track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d3sigsorted", "3rd track 3D IP significance", 1, 1, 1);

    P1D.PlotStack("trackNHits", "nr. of hits", 1, 1, 1);
    P1D.PlotStack("trackNPixelHits", "nr. of pixel hits", 1, 1, 1);
    P1D.PlotStack("trackNormChi2", "norm. #chi^{2}", 1, 1, 1);
    P1D.PlotStack("trackTransverseMomentum", "p_{T}", 1, 1, 1);
    P1D.PlotStack("trackLongitudinalIP", "longitudinal IP", 1, 1, 1);
    P1D.PlotStack("trackIP2d", "2D IP", 1, 1, 1);
    P1D.PlotStack("trackDistJetAxis", "distance to jet axis", 1, 1, 1);
    P1D.PlotStack("trackDecayLength", "decay length", 1, 1, 1);

    P1D.PlotStack("trackNHits_cut","nr. of hits",1, 1, 1);
    P1D.PlotStack("trackNPixelHits_cut","nr. of pixel hits", 1, 1, 1);
    P1D.PlotStack("trackNormChi2_cut", "norm. #chi^{2}",1, 1, 1);
    P1D.PlotStack("trackTransverseMomentum_cut","track p_{T}",1, 1, 1);
    P1D.PlotStack("trackLongitudinalIP_cut", "longitudinal IP", 1, 1, 1);
    P1D.PlotStack("trackIP2d_cut", "2D IP", 1, 1, 1);
    P1D.PlotStack("trackDistJetAxis_cut", "distance to jet axis", 1, 1, 1);
    P1D.PlotStack("trackDecayLength_cut", "decay length", 1, 1, 1);

    P1D.PlotStack("trackIP3d", "3D IP", 1, 1, 1);
    P1D.PlotStack("trackIP3dError", "3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3dsig", "3D IP significance", 1, 1, 1);
    P1D.PlotStack("trackIP3d_sel", "3D IP", 1, 1, 1);
    P1D.PlotStack("trackIP3dError_sel", "3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3dsig_sel", "3D IP significance", 1, 1, 1);
    P1D.PlotStack("trackIP3d_cutsel", "3D IP",  1, 1, 1);
    P1D.PlotStack("trackIP3dError_cutsel", "3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3dsig_cutsel", "3D IP significance", 1, 1, 1);

    // SV
    P1D.PlotStack("sv_nvertices0", "nr. of SV", 1, 1, 1);
    P1D.PlotStack("sv_nvertices",  "nr. of SV", 1, 1, 1);
    P1D.PlotStack("sv_jetpt", "jet p_{T}", 1, 1, 1);
    P1D.PlotStack("sv_tracksel", "SV track multiplicity", 1, 0, 1);
    P1D.PlotStack("sv_trackmul", "SV track multiplicity", 1, 0, 1);

    P1D.PlotStack("sv_flightsig2d", "2D flight significance", 1, 1, 1);
    P1D.PlotStack("sv_flightsig3d", "3D flight significance", 1, 1, 1);
    P1D.PlotStack("sv_flightsig3d_3tr", "3-track 3D flight significance", 1, 1, 1);
    P1D.PlotStack("sv_flightdis2d", "2D flight distance" ,1, 1, 1);
    P1D.PlotStack("sv_flightdis3d", "2D flight distance", 1, 1, 1);

    P1D.PlotStack("sv_normchi2","norm. #chi^{2}", 1, 1, 1);
    P1D.PlotStack("sv_mass", "SV mass", 1, 0, 1);
    P1D.PlotStack("sv_mass_3tr", "3-track SV mass", 1, 0, 1);
    P1D.PlotStack("sv_eratio", "SV energy ratio", 1, 0, 0);
    P1D.PlotStack("sv_ipsigcharm", "2D IP significance", 1, 1, 1);
   
    P1D.PlotStack("sv_vtx_pt", "vertex p_{T}", 1, 1, 1);
    P1D.PlotStack("sv_vtxdir_eta", "vertex #eta", 1, 0, 1);
    P1D.PlotStack("sv_vtxdir_phi", "vertex #phi", 1, 0, 1);
 
    P1D.PlotStack("sv_deltar_jet",    "#Delta R(vertex dir., jet)"            , 1, 0, 1);
    P1D.PlotStack("sv_deltar_sum_jet","#Delta R(vertex {#vec p}, jet)"          ,1, 0, 1);
    P1D.PlotStack("sv_deltar_sum_dir","#Delta R(vertex {#vec p}, vertex dir.)"  ,1, 0, 1);

    P1D.PlotStack("n_cutseltracks_jet", "nr. of tracks", 1, 0, 1);
    P1D.PlotStack("pt_cutseltracks", "track p_{T}", 1, 1, 1);

    // IP
    P1D.PlotStack("IP3d1Errorsorted","1st track 3D IP error", 1, 1, 1);
    P1D.PlotStack("IP3d2Errorsorted","2nd track 3D IP error", 1, 1, 1);
    P1D.PlotStack("IP3d3Errorsorted","3rd track 3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3d_bin1_cutsel", "3D IP", 1, 1, 1);
    P1D.PlotStack("trackIP3d_bin2_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin3_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin4_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin5_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin6_cutsel","3D IP",1, 1, 1);


    P1D.TagRate("discri_tche", "TCHE discr. cut", 1,1);
    P1D.TagRate("discri_tchp", "TCHP discr. cut", 1,1);
    P1D.TagRate("discri_ssche","SSVHE discr. cut", 1,0);
    P1D.TagRate("discri_sschp","SSVHP discr. cut", 1,0);
    P1D.TagRate("discri_jetprob", "JP discr. cut", 1,1);
    P1D.TagRate("discri_jetbprob", "JBP discr. cut", 1,1);


    P1D.PlotData("IP3d2sig0", "2nd track 3D IP significance", 1, 1, 1);
    P1D.PlotData("IP3d3sig0","3nd track 3D IP significance",1, 1, 1);
    P1D.PlotData("ntracks_jet","nr. of tracks in jet", 1, 1, 1);
    P1D.PlotData("nseltracks_jet","nr. of selected tracks", 1, 1, 1);
    P1D.PlotData("ncutseltracks_jet","nr. of selected tracks",1, 1, 1);

    P1D.PlotData("pt_tracks", "track p_{T}", 1, 1, 1);
    P1D.PlotData("pt_seltracks", "track p_{T}", 1, 1, 1);
    P1D.PlotData("pt_cutseltracks", "track p_{T}", 1, 1, 1);

    P1D.PlotData("nsv0", "nr. of SV", 1, 1, 1);
    P1D.PlotData("nsv", "nr. of SV", 1, 1, 1);
    P1D.PlotData("flightsig3d", "3D flight significance",1, 1, 1);
    P1D.PlotData("flightsig3d_3tr","3D flight significance",1, 1, 1);
    P1D.PlotData("svmass", "SV mass",  1, 0, 1);
    P1D.PlotData("svmass_3tr", "3-track SV mass", 1, 0, 1);

    P1D.PlotData("nsv_3tr", "nr. of 3-track SV", 1, 1, 1);
    P1D.PlotData("nsv0_3tr", "nr. of 3-track SV", 1, 1, 1);
    P1D.PlotData("sveratio",          "SV energy ratio",               1, 0, 0);
    P1D.PlotData("deltar_jet",       "SV deltaR jet",        1, 0, 1);
    P1D.PlotData("deltar_sum_jet",     "SV deltaR sum jet",  1, 0, 1);
    P1D.PlotData("deltar_sum_dir",     "SV deltaR sum dir",  1, 0, 1);
    P1D.PlotData("vtxpt",              "SV vtx pt",          1, 1, 1);
    P1D.PlotData("discri_tche",       "TCHE discriminator",  1, 1, 1);
    P1D.PlotData("discri_tchp",       "TCHP discriminator",  1, 1, 1);
    P1D.PlotData("discri_jetprob",    "JP discriminator",    1, 1, 1);
    P1D.PlotData("discri_jetbprob",   "JBP discriminator",   1, 1, 1);
    P1D.PlotData("discri_ssche0",    "SSVHE Discriminator",  1, 1, 1);
    P1D.PlotData("discri_ssche",     "SSVHE Discriminator",  1, 0, 0);
    P1D.PlotData("discri_sschp0",   "SSVHP Discriminator",   1, 1, 1);
    P1D.PlotData("discri_sschp",    "SSVHP Discriminator",   1, 0, 0);
    P1D.PlotData("muon_Pt",         "Muon p_{T}",            1, 1, 1);
    P1D.PlotData("muon_ptrel",      "Muon p_{T}^{rel}",      1, 0, 1);
    P1D.PlotData("muon_DeltaR",     "Muon deltaR",           1, 0, 1);



    P1D.PlotStack("IP3d1sigsorted_bin1", "1st track 3D IP significance bin1", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin2", "1st track 3D IP significance bin2", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin3", "1st track 3D IP significance bin3", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin4", "1st track 3D IP significance bin4", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin5", "1st track 3D IP significance bin5", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin6", "1st track 3D IP significance bin6", 1, 1, 1);

    P1D.PlotStack("IP3d2sigsorted_bin1", "2nd track 3D IP significance bin1", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin2", "2nd track 3D IP significance bin2", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin3", "2nd track 3D IP significance bin3", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin4", "2nd track 3D IP significance bin4", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin5", "2nd track 3D IP significance bin5", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin6", "2nd track 3D IP significance bin6", 1, 1, 1);

    P1D.PlotStack("trackIP3d_jetptbin1",   "3D IP (jet bin1)",    1, 1, 1);    
    P1D.PlotStack("trackIP3d_jetptbin2",   "3D IP (jet bin2)",    1, 1, 1);        
    P1D.PlotStack("trackIP3d_jetptbin3",   "3D IP (jet bin3)",    1, 1, 1);          
                             
    P1D.PlotStack("IP3d1_jetptbin1",   "1st track 3D IP (jet bin1)",    1, 1, 1);              
    P1D.PlotStack("IP3d1_jetptbin2",   "1st track 3D IP (jet bin2)",    1, 1, 1);              
    P1D.PlotStack("IP3d1_jetptbin3",   "1st track 3D IP (jet bin3)",    1, 1, 1);              
                         
    P1D.PlotStack("IP3d2_jetptbin1",   "2nd track 3D IP (jet bin1)",    1, 1, 1);              
    P1D.PlotStack("IP3d2_jetptbin2",   "2nd track 3D IP (jet bin2)",    1, 1, 1);              
    P1D.PlotStack("IP3d2_jetptbin3",   "2nd track 3D IP (jet bin3)",    1, 1, 1);              
                        
    P1D.PlotStack("IP3d3_jetptbin1",   "3rd track 3D IP (jet bin1)",    1, 1, 1);              
    P1D.PlotStack("IP3d3_jetptbin2",   "3rd track 3D IP (jet bin2)",    1, 1, 1);              
    P1D.PlotStack("IP3d3_jetptbin3",   "3rd track 3D IP (jet bin3)",    1, 1, 1);              
                               
    // P1D.PlotStack("IP3dsigsorted_jetptbin1",    "",    1, 1, 1);      
    //  P1D.PlotStack("IP3dsigsorted_jetptbin2",    "",    1, 1, 1);      
    //  P1D.PlotStack("IP3dsigsorted_jetptbin3",    "",    1, 1, 1);      
                             
    P1D.PlotStack("IP3d1sigsorted_jetptbin1",   "1st track 3D IP sig. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d1sigsorted_jetptbin2",   "1st track 3D IP sig. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d1sigsorted_jetptbin3",   "1st track 3D IP sig. (jet bin3)",    1, 1, 1);     
                             		                            
    P1D.PlotStack("IP3d2sigsorted_jetptbin1",   "2nd track 3D IP sig. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d2sigsorted_jetptbin2",   "2nd track 3D IP sig. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d2sigsorted_jetptbin3",   "2nd track 3D IP sig. (jet bin3)",    1, 1, 1);     
                             		                            
    P1D.PlotStack("IP3d3sigsorted_jetptbin1",   "3rd track 3D IP sig. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d3sigsorted_jetptbin2",   "3rd track 3D IP sig. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d3sigsorted_jetptbin3",   "3rd track 3D IP sig. (jet bin3)",    1, 1, 1);     
                            
    //  P1D.PlotStack("IP3dErrorsorted_jetptbin1",   "",    1, 1, 1);    
    //  P1D.PlotStack("IP3dErrorsorted_jetptbin2",   "",    1, 1, 1);    
    //  P1D.PlotStack("IP3dErrorsorted_jetptbin3",   "",    1, 1, 1);    
                              
    P1D.PlotStack("IP3d1Errorsorted_jetptbin1",   "1st track 3D IP err. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d1Errorsorted_jetptbin2",   "1st track 3D IP err. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d1Errorsorted_jetptbin3",   "1st track 3D IP err. (jet bin3)",    1, 1, 1);     
                                 	                                   
    P1D.PlotStack("IP3d2Errorsorted_jetptbin1",   "2nd track 3D IP err. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d2Errorsorted_jetptbin2",   "2nd track 3D IP err. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d2Errorsorted_jetptbin3",   "2nd track 3D IP err. (jet bin3)",    1, 1, 1);     
                              		                                   
    P1D.PlotStack("IP3d3Errorsorted_jetptbin1",   "3rd track 3D IP err. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d3Errorsorted_jetptbin2",   "3rd track 3D IP err. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d3Errorsorted_jetptbin3",   "3rd track 3D IP err. (jet bin3)",    1, 1, 1);     
          
    P1D.PlotStack("sv_mass_jetptbin1",   "2-track SV mass (jet bin1)",    1, 0, 1);            
    P1D.PlotStack("sv_mass_jetptbin2",   "2-track SV mass (jet bin2)",    1, 0, 1);            
    P1D.PlotStack("sv_mass_jetptbin3",   "2-track SV mass (jet bin3)",    1, 0, 1);            
                            
    P1D.PlotStack("sv_mass_3tr_jetptbin1",   "3-track SV mass (jet bin1)",    1, 0, 1);        
    P1D.PlotStack("sv_mass_3tr_jetptbin2",   "3-track SV mass (jet bin2)",    1, 0, 1);        
    P1D.PlotStack("sv_mass_3tr_jetptbin3",   "3-track SV mass (jet bin3)",    1, 0, 1);        
                             
    //  P1D.PlotStack("sv_ipsigcharm_jetptbin1",   "",    1, 1, 1);      
    //  P1D.PlotStack("sv_ipsigcharm_jetptbin2",   "",    1, 1, 1);      
    //  P1D.PlotStack("sv_ipsigcharm_jetptbin3",   "",    1, 1, 1);      
                           
    P1D.PlotStack("sv_eratio_jetptbin1",   "SV energy ratio (jet bin1)",    1, 0, 0);          
    P1D.PlotStack("sv_eratio_jetptbin2",   "SV energy ratio (jet bin2)",    1, 0, 0);          
    P1D.PlotStack("sv_eratio_jetptbin3",   "SV energy ratio (jet bin3)",    1, 0, 0);          
                                
    P1D.PlotStack("sv_flightsig2d_jetptbin1",   "2D flight sig. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig2d_jetptbin2",   "2D flight sig. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig2d_jetptbin3",   "2D flight sig. (jet bin3)",    1, 1, 1);       
                             
    P1D.PlotStack("sv_flightsig3d_jetptbin1",   "3D flight sig. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig3d_jetptbin2",   "3D flight sig. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig3d_jetptbin3",   "3D flight sig. (jet bin3)",    1, 1, 1);       
                               
    P1D.PlotStack("sv_flightdis2d_jetptbin1",   "2D flight dist. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis2d_jetptbin2",   "2D flight dist. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis2d_jetptbin3",   "2D flight dist. (jet bin3)",    1, 1, 1);       
                               
    P1D.PlotStack("sv_flightdis3d_jetptbin1",   "3D flight dist. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis3d_jetptbin2",   "3D flight dist. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis3d_jetptbin3",   "3D flight dist. (jet bin3)",    1, 1, 1);       
                                 
    P1D.PlotStack("sv_flightsig3d_3tr_jetptbin1",   "3-track 3D flight dist. (jet bin1)",    1, 1, 1);   
    P1D.PlotStack("sv_flightsig3d_3tr_jetptbin2",   "3-track 3D flight dist. (jet bin2)",    1, 1, 1);   
    P1D.PlotStack("sv_flightsig3d_3tr_jetptbin3",   "3-track 3D flight dist. (jet bin3)",    1, 1, 1);   
                               


    P1D.PlotStack("sv_deltar_jet_jetptbin1",   "#Delta R(vertex dir., jet) (jet bin1)",    1, 0, 1);        
    P1D.PlotStack("sv_deltar_jet_jetptbin2",   "#Delta R(vertex dir., jet) (jet bin2)",    1, 0, 1);        
    P1D.PlotStack("sv_deltar_jet_jetptbin3",   "#Delta R(vertex dir., jet) (jet bin3)",    1, 0, 1);        
                                
    P1D.PlotStack("sv_deltar_sum_jet_jetptbin1",   "#Delta R(vertex {#vec p}, jet) (jet bin1)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_jet_jetptbin2",   "#Delta R(vertex {#vec p}, jet) (jet bin2)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_jet_jetptbin3",   "#Delta R(vertex {#vec p}, jet) (jet bin3)",    1, 0, 1);    
                               
    P1D.PlotStack("sv_deltar_sum_dir_jetptbin1",   "#Delta R(vertex {#vec p}, vertex dir.) (jet bin1)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_dir_jetptbin2",   "#Delta R(vertex {#vec p}, vertex dir.) (jet bin2)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_dir_jetptbin3",   "#Delta R(vertex {#vec p}, vertex dir.) (jet bin3)",    1, 0, 1);    
            
    P1D.PlotStack("discri_tche_jetptbin1",   "TCHE discriminator (jet bin1)",    1, 1, 1);          
    P1D.PlotStack("discri_tche_jetptbin2",   "TCHE discriminator (jet bin2)",    1, 1, 1);          
    P1D.PlotStack("discri_tche_jetptbin3",   "TCHE discriminator (jet bin3)",    1, 1, 1);          
                              
    P1D.PlotStack("discri_tchp_jetptbin1",   "TCHP discriminator (jet bin1)",    1, 1, 1);          
    P1D.PlotStack("discri_tchp_jetptbin2",   "TCHP discriminator (jet bin2)",    1, 1, 1);          
    P1D.PlotStack("discri_tchp_jetptbin3",   "TCHP discriminator (jet bin3)",    1, 1, 1);          
                            
    P1D.PlotStack("discri_ssche0_jetptbin1",   "SSVHE discriminator (jet bin1)",    1, 1, 1);        
    P1D.PlotStack("discri_ssche0_jetptbin2",   "SSVHE discriminator (jet bin2)",    1, 1, 1);        
    P1D.PlotStack("discri_ssche0_jetptbin3",   "SSVHE discriminator (jet bin3)",    1, 1, 1);        
                           
    P1D.PlotStack("discri_ssche_jetptbin1",   "SSVHE discriminator (jet bin1)",    1, 0, 0);         
    P1D.PlotStack("discri_ssche_jetptbin2",   "SSVHE discriminator (jet bin2)",    1, 0, 0);         
    P1D.PlotStack("discri_ssche_jetptbin3",   "SSVHE discriminator (jet bin3)",    1, 0, 0);         
                         
    P1D.PlotStack("discri_sschp0_jetptbin1",   "SSVHP discriminator (jet bin1)",    1, 1, 1);        
    P1D.PlotStack("discri_sschp0_jetptbin2",   "SSVHP discriminator (jet bin2)",    1, 1, 1);        
    P1D.PlotStack("discri_sschp0_jetptbin3",   "SSVHP discriminator (jet bin3)",    1, 1, 1);        
                                
    P1D.PlotStack("discri_sschp_jetptbin1",   "SSVHP discriminator (jet bin1)",    1, 0, 0);         
    P1D.PlotStack("discri_sschp_jetptbin2",   "SSVHP discriminator (jet bin2)",    1, 0, 0);         
    P1D.PlotStack("discri_sschp_jetptbin3",   "SSVHP discriminator (jet bin3)",    1, 0, 0);         
                            
    P1D.PlotStack("discri_jetprob_jetptbin1",   "JP discriminator (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("discri_jetprob_jetptbin2",   "JP discriminator (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("discri_jetprob_jetptbin3",   "JP discriminator (jet bin3)",    1, 1, 1);       
                               
    P1D.PlotStack("discri_jetbprob_jetptbin1",   "JBP discriminator (jet bin1)",    1, 1, 1);      
    P1D.PlotStack("discri_jetbprob_jetptbin2",   "JBP discriminator (jet bin2)",    1, 1, 1);      
    P1D.PlotStack("discri_jetbprob_jetptbin3",   "JBP discriminator (jet bin3)",    1, 1, 1);      

    P1D.PlotStack("sv_ipsigcharm_3tr",   "",    1, 1, 1);              
    P1D.PlotStack("sv_eratio_3tr",   "3-track SV energy ratio",    1, 0, 0);                  
    P1D.PlotStack("sv_vtx_pt_3tr",   "3-track SV vtx p_{T}",    1, 1, 1);                  

    P1D.PlotStack("sv_trackmul_centered",   "SV track multiplicity",    1, 1, 1);           
    P1D.PlotStack("sv_trackmul1_centered",   "SV track multiplicity",    1, 1, 1);          
    P1D.PlotStack("sv_tracksel_centered",   "SV track multiplicity",    1, 1, 1);           


    P1D.PlotStack("npv1_IP3d2sig0",        "2nd sorted track 3D IP significance (1-6 PVs)",   1, 1, 1);  
    P1D.PlotStack(  "npv1_IP3d3sig0",        "3rd sorted track 3D IP significance (1-6 PVs)", 1, 1, 1);  
    // P1D.PlotStack( "npv1_ntracks_jet",      "nr. of tracks in jet (1-6 PVs)",                   1, 1, 1);  
    //  P1D.PlotStack( "npv1_nseltracks_jet",   "nr. of sel tracks in jet (1-6 PVs)",               1, 1, 1);  
    P1D.PlotStack( "npv1_ncutseltracks_jet","nr. of tracks in jet (1-6 PVs)",           1, 1, 1);  
    P1D.PlotStack( "npv1_pt_tracks",        "Pt of tracks (1-6 PVs)",                         1, 1, 1);  
    P1D.PlotStack( "npv1_pt_seltracks",     "Pt of  sel tracks (1-6 PVs)",                    1, 1, 1);  
    P1D.PlotStack( "npv1_pt_cutseltracks",  "Pt of  cut sel tracks (1-6 PVs)",                1, 1, 1);  
    P1D.PlotStack( "npv1_nsv0",             "# of secondary vertices (1-6 PVs)",              1, 1, 1);  
    P1D.PlotStack( "npv1_nsv",              "# of secondary vertices (1-6 PVs)",              1, 1, 1);  
    P1D.PlotStack( "npv1_flightsig3d",      "Flight Significance 3D (1-6 PVs)",               1, 1, 1);  
    P1D.PlotStack( "npv1_flightsig3d_3tr",  "Flight Significance 3D for >=3tracks (1-6 PVs)", 1, 1, 1);  
    P1D.PlotStack( "npv1_svmass",           "SV mass (1-6 PVs)",                              1, 0, 1);  
    P1D.PlotStack( "npv1_svmass_3tr",       "SV mass for >=3tracks (1-6 PVs)",                1, 0, 1);  
    P1D.PlotStack( "npv1_nsv0_3tr",         "# of secondary vertices (>=3tr) (1-6 PVs)",      1, 1, 1);
    P1D.PlotStack("npv1_sveratio",        "SV eratio (1-6 PVs)",                  1, 0, 0);
    P1D.PlotStack("npv1_deltar_jet",      "SV deltaR jet (1-6 PVs)",              1, 0, 1);
    P1D.PlotStack("npv1_deltar_sum_jet",      "SV deltaR sum jet (1-6 PVs)",      1, 0, 1);
    P1D.PlotStack("npv1_deltar_sum_dir",      "SV deltaR sum dir (1-6 PVs)",      1, 0, 1);
    P1D.PlotStack("npv1_vtxpt",      "SV vtx pt (1-6 PVs)",                       1, 1, 1);
    P1D.PlotStack("npv1_discri_tche",      "TCHE discriminator (1-6 PVs)",        1, 1, 1);
    P1D.PlotStack("npv1_discri_tchp",      "TCHP discriminator (1-6 PVs)",        1, 1, 1);
    P1D.PlotStack("npv1_discri_jetprob",      "JP discriminator (1-6 PVs)",       1, 1, 1);
    P1D.PlotStack("npv1_discri_jetbprob",      "JBP discriminator (1-6 PVs)",     1, 1, 1);
    P1D.PlotStack("npv1_discri_ssche0",       "SSVHE Discriminator (1-6 PVs)",    1, 1, 1);
    P1D.PlotStack("npv1_discri_ssche",   "SSVHE Discriminator (1-6 PVs)",         1, 0, 0);
    P1D.PlotStack("npv1_discri_sschp0",   "SSVHP Discriminator (1-6 PVs)",        1, 1, 1);
    P1D.PlotStack("npv1_discri_sschp",   "SSVHP Discriminator (1-6 PVs)",         1, 0, 0);
    P1D.PlotStack("npv1_muon_Pt",        "Muon p_{T} (1-6 PVs)",                  1, 1, 1);
    P1D.PlotStack("npv1_muon_ptrel",    "Muon p_{T}^{rel} (1-6 PVs)",             1, 0, 1);
    P1D.PlotStack("npv1_muon_DeltaR",         "Muon1 deltaR (1-6 PVs)",           1, 0, 1);


    P1D.PlotStack("npv2_IP3d2sig0",        "2nd sorted track 3D IP significance (#PV:7-10)",   1, 1, 1);  
    P1D.PlotStack(  "npv2_IP3d3sig0",        "3rd sorted track 3D IP significance (#PV:7-10)", 1, 1, 1);  
    // P1D.PlotStack( "npv2_ntracks_jet",      "nr. of tracks in jet (#PV:7-10)",                   1, 1, 1);  
    //  P1D.PlotStack( "npv2_nseltracks_jet",   "nr. of sel tracks in jet (#PV:7-10)",               1, 1, 1);  
    P1D.PlotStack( "npv2_ncutseltracks_jet","nr. of tracks in jet (#PV:7-10)",           1, 1, 1);  
    P1D.PlotStack( "npv2_pt_tracks",        "Pt of tracks (#PV:7-10)",                         1, 1, 1);  
    P1D.PlotStack( "npv2_pt_seltracks",     "Pt of  sel tracks (#PV:7-10)",                    1, 1, 1);  
    P1D.PlotStack( "npv2_pt_cutseltracks",  "Pt of  cut sel tracks (#PV:7-10)",                1, 1, 1);  
    P1D.PlotStack( "npv2_nsv0",             "# of secondary vertices (#PV:7-10)",              1, 1, 1);  
    P1D.PlotStack( "npv2_nsv",              "# of secondary vertices (#PV:7-10)",              1, 1, 1);  
    P1D.PlotStack( "npv2_flightsig3d",      "Flight Significance 3D (#PV:7-10)",               1, 1, 1);  
    P1D.PlotStack( "npv2_flightsig3d_3tr",  "Flight Significance 3D for >=3tracks (#PV:7-10)", 1, 1, 1);  
    P1D.PlotStack( "npv2_svmass",           "SV mass (#PV:7-10)",                              1, 0, 1);  
    P1D.PlotStack( "npv2_svmass_3tr",       "SV mass for >=3tracks (#PV:7-10)",                1, 0, 1);  
    P1D.PlotStack( "npv2_nsv0_3tr",         "# of secondary vertices (>=3tr) (#PV:7-10)",      1, 1, 1);
 
    P1D.PlotStack("npv2_sveratio",        "SV eratio (#PV:7-10)",                  1, 0, 0);
    P1D.PlotStack("npv2_deltar_jet",      "SV deltaR jet (#PV:7-10)",              1, 0, 1);
    P1D.PlotStack("npv2_deltar_sum_jet",      "SV deltaR sum jet (#PV:7-10)",      1, 0, 1);
    P1D.PlotStack("npv2_deltar_sum_dir",      "SV deltaR sum dir (#PV:7-10)",      1, 0, 1);
    P1D.PlotStack("npv2_vtxpt",      "SV vtx pt (#PV:7-10)",                       1, 1, 1);
    P1D.PlotStack("npv2_discri_tche",      "TCHE discriminator (#PV:7-10)",        1, 1, 1);
    P1D.PlotStack("npv2_discri_tchp",      "TCHP discriminator (#PV:7-10)",        1, 1, 1);
    P1D.PlotStack("npv2_discri_jetprob",      "JP discriminator (#PV:7-10)",       1, 1, 1);
    P1D.PlotStack("npv2_discri_jetbprob",      "JBP discriminator (#PV:7-10)",     1, 1, 1);
    P1D.PlotStack("npv2_discri_ssche0",       "SSVHE Discriminator (#PV:7-10)",    1, 1, 1);
    P1D.PlotStack("npv2_discri_ssche",   "SSVHE Discriminator (#PV:7-10)",         1, 0, 0);
    P1D.PlotStack("npv2_discri_sschp0",   "SSVHP Discriminator (#PV:7-10)",        1, 1, 1);
    P1D.PlotStack("npv2_discri_sschp",   "SSVHP Discriminator (#PV:7-10)",         1, 0, 0);
    P1D.PlotStack("npv2_muon_Pt",        "Muon p_{T} (#PV:7-10)",                  1, 1, 1);
    P1D.PlotStack("npv2_muon_ptrel",    "Muon p_{T}^{rel} (#PV:7-10)",             1, 0, 1);
    P1D.PlotStack("npv2_muon_DeltaR",         "Muon1 deltaR (#PV:7-10)",           1, 0, 1);


    P1D.PlotStack("npv3_IP3d2sig0",        "2nd sorted track 3D IP significance (#PV#geq11)",   1, 1, 1);  
    P1D.PlotStack(  "npv3_IP3d3sig0",        "3rd sorted track 3D IP significance (#PV#geq11)", 1, 1, 1);  
    // P1D.PlotStack( "npv3_ntracks_jet",      "nr. of tracks in jet (#PV#geq11)",                   1, 1, 1);  
    //  P1D.PlotStack( "npv3_nseltracks_jet",   "nr. of sel tracks in jet (#PV#geq11)",               1, 1, 1);  
    P1D.PlotStack( "npv3_ncutseltracks_jet","nr. of tracks in jet (#PV#geq11)",           1, 1, 1);  
    P1D.PlotStack( "npv3_pt_tracks",        "Pt of tracks (#PV#geq11)",                         1, 1, 1);  
    P1D.PlotStack( "npv3_pt_seltracks",     "Pt of  sel tracks (#PV#geq11)",                    1, 1, 1);  
    P1D.PlotStack( "npv3_pt_cutseltracks",  "Pt of  cut sel tracks (#PV#geq11)",                1, 1, 1);  
    P1D.PlotStack( "npv3_nsv0",             "# of secondary vertices (#PV#geq11)",              1, 1, 1);  
    P1D.PlotStack( "npv3_nsv",              "# of secondary vertices (#PV#geq11)",              1, 1, 1);  
    P1D.PlotStack( "npv3_flightsig3d",      "Flight Significance 3D (#PV#geq11)",               1, 1, 1);  
    P1D.PlotStack( "npv3_flightsig3d_3tr",  "Flight Significance 3D for >=3tracks (#PV#geq11)", 1, 1, 1);  
    P1D.PlotStack( "npv3_svmass",           "SV mass (#PV#geq11)",                              1, 0, 1);  
    P1D.PlotStack( "npv3_svmass_3tr",       "SV mass for >=3tracks (#PV#geq11)",                1, 0, 1);  
    P1D.PlotStack( "npv3_nsv0_3tr",         "# of secondary vertices (>=3tr) (#PV#geq11)",      1, 1, 1);
 
    P1D.PlotStack("npv3_sveratio",        "SV eratio (#PV#geq11)",                  1, 0, 0);
    P1D.PlotStack("npv3_deltar_jet",      "SV deltaR jet (#PV#geq11)",              1, 0, 1);
    P1D.PlotStack("npv3_deltar_sum_jet",      "SV deltaR sum jet (#PV#geq11)",      1, 0, 1);
    P1D.PlotStack("npv3_deltar_sum_dir",      "SV deltaR sum dir (#PV#geq11)",      1, 0, 1);
    P1D.PlotStack("npv3_vtxpt",      "SV vtx pt (#PV#geq11)",                       1, 1, 1);
    P1D.PlotStack("npv3_discri_tche",      "TCHE discriminator (#PV#geq11)",        1, 1, 1);
    P1D.PlotStack("npv3_discri_tchp",      "TCHP discriminator (#PV#geq11)",        1, 1, 1);
    P1D.PlotStack("npv3_discri_jetprob",      "JP discriminator (#PV#geq11)",       1, 1, 1);
    P1D.PlotStack("npv3_discri_jetbprob",      "JBP discriminator (#PV#geq11)",     1, 1, 1);
    P1D.PlotStack("npv3_discri_ssche0",       "SSVHE Discriminator (#PV#geq11)",    1, 1, 1);
    P1D.PlotStack("npv3_discri_ssche",   "SSVHE Discriminator (#PV#geq11)",         1, 0, 0);
    P1D.PlotStack("npv3_discri_sschp0",   "SSVHP Discriminator (#PV#geq11)",        1, 1, 1);
    P1D.PlotStack("npv3_discri_sschp",   "SSVHP Discriminator (#PV#geq11)",         1, 0, 0);
    P1D.PlotStack("npv3_muon_Pt",        "Muon p_{T} (#PV#geq11)",                  1, 1, 1);
    P1D.PlotStack("npv3_muon_ptrel",    "Muon p_{T}^{rel} (#PV#geq11)",             1, 0, 1);
    P1D.PlotStack("npv3_muon_DeltaR",         "Muon1 deltaR (#PV#geq11)",           1, 0, 1);
    P2D.PlotStack2D("track_vs_jetpt", "jet p_{T}", "track multiplicity",  0,10,35.);
    P2D.PlotStack2D("cutseltrack_vs_jetpt", "jet p_{T}", "track multiplicity", 0,4,17.);
    P2D.PlotStack2D("sv_track_vs_jetpt",    "jet p_{T}", "SV track multiplicity",  0,1.,5.5);

    P2D.PlotStack2D("sv_mass_vs_flightDistSignCut", "flight dist sign.", "SV mass",   0, 0, 6.5);
    P2D.PlotStack2D("sv_mass_vs_flightDistCut","flight dist.", "SV mass",  0,0,4.5);

    P2D.PlotStack2D("avg_sv_mass_vs_jetpt", "jet p_{T}", "SV mass", 0,0,5.5);

    P2D.PlotStack2D("sv_deltar_jet_vs_jetpt", "jet p_{T}", "#Delta R(vertex dir., jet)", 0,0,0.13);
    P2D.PlotStack2D("sv_deltar_sum_dir_vs_jetpt","jet p_{T}", "#Delta R(vertex {#vec p}, vertex dir.)",  0,0,0.09);
    P2D.PlotStack2D("sv_deltar_sum_jet_vs_jetpt","jet p_{T}", "#Delta R(vertex {#vec p}, jet)",  0,0,0.09);
    P2D.PlotStack2D("muon_ptrel_vs_jetpt", "jet p_{T}","muon p_{T}^{rel}",  0,0,1);
    P2D.PlotStack2D("muon_DeltaR_vs_jetpt","jet p_{T}","muon #Delta R",   0,0,1.);

  
    P2D.PlotStack2D("sv_eratio_vs_jetpt","jet p_{T}","SV energy ratio",  0,0,0.75);
    P2D.PlotStack2D("sv_vtx_pt_vs_jetpt","jet p_{T}","SV p_{T}",  0,0,55);
    P2D.PlotProfile2D("pu_vs_pv","# of Reco PV","# of PU",  0,0,60,false,"pol1");
    P2D.PlotProfile2D("ip3dvsphi","track #phi","ip3d",  0,-0.02,0.02);
    P2D.PlotProfile2D("ip2dvsphi","track #phi","ip2d",  0,0.0002,0.0012);
    P2D.PlotProfile2D("ip3dsigvsphi","track #phi","ip3dsig",  0,0.15,0.5);
    P2D.PlotProfile2D("ip2dsigvsphi","track #phi","ip2dsig",  0,0.1,0.35);
#else
    // // for btagmet dataset 


    P1D.PlotStack("muons_multiplicity", "muon multiplicity" ,1, 1, 1);
    P1D.PlotData("muons_multiplicity_centered",   "muon multiplicity",    1, 1, 1);    
    P1D.PlotStack("muon_Ip3d", "muon 3D IP", 1, 1, 1);
    P1D.PlotStack("muon_Ip2d", "muon 2D IP", 1, 1, 1);
    P1D.PlotStack("muon_Sip3d", "muon 3D IP sig.", 1, 1, 1);
    P1D.PlotStack("muon_Sip2d", "muon 2D IP sig.", 1, 1, 1);
    P1D.PlotStack("muon_ptrel","muon ptrel", 1, 0, 1);
    P1D.PlotStack("muon_Pt", "muon pt", 1, 1, 1);
    P1D.PlotStack("muon_DeltaR", "muon delta R", 1, 0, 1);
    // P1D.PlotStack("muon_Ratio", 0, 0, 1);

    P1D.PlotStack("sv_mass_muenriched",            "sv_mass_muenriched",               1, 0, 1, 1);      
    P1D.PlotStack("IP3d2sigsorted_muenriched",     "IP3d2sigsorted_muenriched",        1, 1, 1);      
    P1D.PlotStack("IP3d2sigzsorted_muenriched",    "IP3d2sigzsorted_muenriched",       1, 1, 1);      
    P1D.PlotStack("sv_flightsig2d_muenriched",     "sv_flightsig2d_muenriched",        1, 1, 1);      
    P1D.PlotStack("sv_flightsig3d_muenriched",     "sv_flightsig3d_muenriched",        1, 1, 1);      
    P1D.PlotStack("discri_tche_muenriched",        "discri_tche_muenriched",           1, 1, 1);      
    P1D.PlotStack("discri_tchp_muenriched",        "discri_tchp_muenriched",           1, 1, 1);      
    P1D.PlotStack("discri_ssche_muenriched",       "discri_ssche_muenriched",          1, 0,0, 1);      
    P1D.PlotStack("discri_sschp_muenriched",       "discri_sschp_muenriched",          1, 0,0, 1);      
    P1D.PlotStack("muenriched_discrim",            "muenriched_discrim",               1, 1, 1);      
    P1D.PlotStack("discri_jetprob_muenriched",     "discri_jetprob_muenriched",        1, 1, 1);      
    P1D.PlotStack("discri_jetbprob_muenriched",    "discri_jetbprob_muenriched",       1, 1, 1);      



    P1D.PlotStack("discri_tche","TCHE discriminator",1, 1, 1);
    P1D.PlotStack("discri_tchp","TCHP discriminator",1, 1, 1);
    P1D.PlotStack("discri_ssche0","SSVHE discriminator", 1, 1, 1);
    P1D.PlotStack("discri_ssche", "SSVHE discriminator", 1, 0, 0);
    P1D.PlotStack("discri_sschp0","SSVHP discriminator", 1, 1, 1);
    P1D.PlotStack("discri_sschp", "SSVHP discriminator", 1, 0, 0);
    P1D.PlotStack("discri_jetprob", "JP discriminator", 1, 1, 1);
    P1D.PlotStack("discri_jetbprob", "JBP discriminator",1, 1, 1);

    P1D.PlotStack("n_tracks_jet", "nr. of tracks in jet",  1, 0, 1);
    P1D.PlotStack("n_seltracks_jet","nr. of selected tracks",  1, 0, 1);
    P1D.PlotStack("pt_tracks", "track p_{T}", 1, 1, 1);
    P1D.PlotStack("pt_seltracks", "track p_{T}", 1, 1, 1);

    // IP
    P1D.PlotStack("IP3d1",  "1st track 3D IP", 1, 1, 1);
    P1D.PlotStack("IP3d2",  "2nd track 3D IP",1, 1, 1);
    P1D.PlotStack("IP3d3",  "3rd track 3D IP",1, 1, 1);
    P1D.PlotStack("IP3d1sig", "1st track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d2sig", "2nd track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d3sig", "3rd track 3D IP significance", 1, 1, 1);

    P1D.PlotStack("IP3d1sorted","1st track 3D IP",  1, 1, 1);
    P1D.PlotStack("IP3d2sorted","2nd track 3D IP",  1, 1, 1);
    P1D.PlotStack("IP3d3sorted","3rd track 3D IP",  1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted", "1st track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted", "2nd track 3D IP significance", 1, 1, 1);
    P1D.PlotStack("IP3d3sigsorted", "3rd track 3D IP significance", 1, 1, 1);

    P1D.PlotStack("trackNHits", "nr. of hits", 1, 1, 1);
    P1D.PlotStack("trackNPixelHits", "nr. of pixel hits", 1, 1, 1);
    P1D.PlotStack("trackNormChi2", "norm. #chi^{2}", 1, 1, 1);
    P1D.PlotStack("trackTransverseMomentum", "p_{T}", 1, 1, 1);
    P1D.PlotStack("trackLongitudinalIP", "longitudinal IP", 1, 1, 1);
    P1D.PlotStack("trackIP2d", "2D IP", 1, 1, 1);
    P1D.PlotStack("trackDistJetAxis", "distance to jet axis", 1, 1, 1);
    P1D.PlotStack("trackDecayLength", "decay length", 1, 1, 1);

    P1D.PlotStack("trackNHits_cut","nr. of hits",1, 1, 1);
    P1D.PlotStack("trackNPixelHits_cut","nr. of pixel hits", 1, 1, 1);
    P1D.PlotStack("trackNormChi2_cut", "norm. #chi^{2}",1, 1, 1);
    P1D.PlotStack("trackTransverseMomentum_cut","track p_{T}",1, 1, 1);
    P1D.PlotStack("trackLongitudinalIP_cut", "longitudinal IP", 1, 1, 1);
    P1D.PlotStack("trackIP2d_cut", "2D IP", 1, 1, 1);
    P1D.PlotStack("trackDistJetAxis_cut", "distance to jet axis", 1, 1, 1);
    P1D.PlotStack("trackDecayLength_cut", "decay length", 1, 1, 1);

    P1D.PlotStack("trackIP3d", "3D IP", 1, 1, 1);
    P1D.PlotStack("trackIP3dError", "3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3dsig", "3D IP significance", 1, 1, 1);
    P1D.PlotStack("trackIP3d_sel", "3D IP", 1, 1, 1);
    P1D.PlotStack("trackIP3dError_sel", "3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3dsig_sel", "3D IP significance", 1, 1, 1);
    P1D.PlotStack("trackIP3d_cutsel", "3D IP",  1, 1, 1);
    P1D.PlotStack("trackIP3dError_cutsel", "3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3dsig_cutsel", "3D IP significance", 1, 1, 1);

    // SV
    P1D.PlotStack("sv_nvertices0", "nr. of SV", 1, 1, 1);
    P1D.PlotStack("sv_nvertices",  "nr. of SV", 1, 1, 1);
    P1D.PlotStack("sv_jetpt", "jet p_{T}", 1, 1, 1);
    P1D.PlotStack("sv_tracksel", "SV track multiplicity", 1, 0, 1);
    P1D.PlotStack("sv_trackmul", "SV track multiplicity", 1, 0, 1);

    P1D.PlotStack("sv_flightsig2d", "2D flight significance", 1, 1, 1);
    P1D.PlotStack("sv_flightsig3d", "3D flight significance", 1, 1, 1);
    P1D.PlotStack("sv_flightsig3d_3tr", "3-track 3D flight significance", 1, 1, 1);
    P1D.PlotStack("sv_flightdis2d", "2D flight distance" ,1, 1, 1);
    P1D.PlotStack("sv_flightdis3d", "2D flight distance", 1, 1, 1);

    P1D.PlotStack("sv_normchi2","norm. #chi^{2}", 1, 1, 1);
    P1D.PlotStack("sv_mass", "SV mass", 1, 0, 1);
    P1D.PlotStack("sv_mass_3tr", "3-track SV mass", 1, 0, 1);
    P1D.PlotStack("sv_eratio", "SV energy ratio", 1, 0, 0);
    P1D.PlotStack("sv_ipsigcharm", "2D IP significance", 1, 1, 1);
   
    P1D.PlotStack("sv_vtx_pt", "vertex p_{T}", 1, 1, 1);
    P1D.PlotStack("sv_vtxdir_eta", "vertex #eta", 1, 0, 1);
    P1D.PlotStack("sv_vtxdir_phi", "vertex #phi", 1, 0, 1);
 
    P1D.PlotStack("sv_deltar_jet",    "#Delta R(vertex dir., jet)"            , 1, 0, 1);
    P1D.PlotStack("sv_deltar_sum_jet","#Delta R(vertex {#vec p}, jet)"          ,1, 0, 1);
    P1D.PlotStack("sv_deltar_sum_dir","#Delta R(vertex {#vec p}, vertex dir.)"  ,1, 0, 1);

    P1D.PlotStack("n_cutseltracks_jet", "nr. of tracks", 1, 0, 1);
    P1D.PlotStack("pt_cutseltracks", "track p_{T}", 1, 1, 1);

    // IP
    P1D.PlotStack("IP3d1Errorsorted","1st track 3D IP error", 1, 1, 1);
    P1D.PlotStack("IP3d2Errorsorted","2nd track 3D IP error", 1, 1, 1);
    P1D.PlotStack("IP3d3Errorsorted","3rd track 3D IP error", 1, 1, 1);
    P1D.PlotStack("trackIP3d_bin1_cutsel", "3D IP", 1, 1, 1);
    P1D.PlotStack("trackIP3d_bin2_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin3_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin4_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin5_cutsel","3D IP",1, 1, 1);
    P1D.PlotStack("trackIP3d_bin6_cutsel","3D IP",1, 1, 1);


    P1D.TagRate("discri_tche", "TCHE discr. cut", 1,1);
    P1D.TagRate("discri_tchp", "TCHP discr. cut", 1,1);
    P1D.TagRate("discri_ssche","SSVHE discr. cut", 1,0);
    P1D.TagRate("discri_sschp","SSVHP discr. cut", 1,0);
    P1D.TagRate("discri_jetprob", "JP discr. cut", 1,1);
    P1D.TagRate("discri_jetbprob", "JBP discr. cut", 1,1);


    P1D.PlotData("IP3d2sig0", "2nd track 3D IP significance", 1, 1, 1);
    P1D.PlotData("IP3d3sig0","3nd track 3D IP significance",1, 1, 1);
    P1D.PlotData("ntracks_jet","nr. of tracks in jet", 1, 1, 1);
    P1D.PlotData("nseltracks_jet","nr. of selected tracks", 1, 1, 1);
    P1D.PlotData("ncutseltracks_jet","nr. of selected tracks",1, 1, 1);

    P1D.PlotData("pt_tracks", "track p_{T}", 1, 1, 1);
    P1D.PlotData("pt_seltracks", "track p_{T}", 1, 1, 1);
    P1D.PlotData("pt_cutseltracks", "track p_{T}", 1, 1, 1);

    P1D.PlotData("nsv0", "nr. of SV", 1, 1, 1);
    P1D.PlotData("nsv", "nr. of SV", 1, 1, 1);
    P1D.PlotData("flightsig3d", "3D flight significance",1, 1, 1);
    P1D.PlotData("flightsig3d_3tr","3D flight significance",1, 1, 1);
    P1D.PlotData("svmass", "SV mass",  1, 0, 1);
    P1D.PlotData("svmass_3tr", "3-track SV mass", 1, 0, 1);

    P1D.PlotData("nsv_3tr", "nr. of 3-track SV", 1, 1, 1);
    P1D.PlotData("nsv0_3tr", "nr. of 3-track SV", 1, 1, 1);

    P1D.PlotData("sveratio",          "SV energy ratio",               1, 0, 0);
    P1D.PlotData("deltar_jet",       "SV deltaR jet",        1, 0, 1);
    P1D.PlotData("deltar_sum_jet",     "SV deltaR sum jet",  1, 0, 1);
    P1D.PlotData("deltar_sum_dir",     "SV deltaR sum dir",  1, 0, 1);
    P1D.PlotData("vtxpt",              "SV vtx pt",          1, 1, 1);
    P1D.PlotData("discri_tche",       "TCHE discriminator",  1, 1, 1);
    P1D.PlotData("discri_tchp",       "TCHP discriminator",  1, 1, 1);
    P1D.PlotData("discri_jetprob",    "JP discriminator",    1, 1, 1);
    P1D.PlotData("discri_jetbprob",   "JBP discriminator",   1, 1, 1);
    P1D.PlotData("discri_ssche0",    "SSVHE Discriminator",  1, 1, 1);
    P1D.PlotData("discri_ssche",     "SSVHE Discriminator",  1, 0, 0);
    P1D.PlotData("discri_sschp0",   "SSVHP Discriminator",   1, 1, 1);
    P1D.PlotData("discri_sschp",    "SSVHP Discriminator",   1, 0, 0);
    P1D.PlotData("muon_Pt",         "Muon p_{T}",            1, 1, 1);
    P1D.PlotData("muon_ptrel",      "Muon p_{T}^{rel}",      1, 0, 1);
    P1D.PlotData("muon_DeltaR",     "Muon deltaR",           1, 0, 1);



    P1D.PlotStack("IP3d1sigsorted_bin1", "1st track 3D IP significance bin1", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin2", "1st track 3D IP significance bin2", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin3", "1st track 3D IP significance bin3", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin4", "1st track 3D IP significance bin4", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin5", "1st track 3D IP significance bin5", 1, 1, 1);
    P1D.PlotStack("IP3d1sigsorted_bin6", "1st track 3D IP significance bin6", 1, 1, 1);

    P1D.PlotStack("IP3d2sigsorted_bin1", "2nd track 3D IP significance bin1", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin2", "2nd track 3D IP significance bin2", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin3", "2nd track 3D IP significance bin3", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin4", "2nd track 3D IP significance bin4", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin5", "2nd track 3D IP significance bin5", 1, 1, 1);
    P1D.PlotStack("IP3d2sigsorted_bin6", "2nd track 3D IP significance bin6", 1, 1, 1);

    P1D.PlotStack("trackIP3d_jetptbin1",   "3D IP (jet bin1)",    1, 1, 1);    
    P1D.PlotStack("trackIP3d_jetptbin2",   "3D IP (jet bin2)",    1, 1, 1);        
    P1D.PlotStack("trackIP3d_jetptbin3",   "3D IP (jet bin3)",    1, 1, 1);          
                             
    P1D.PlotStack("IP3d1_jetptbin1",   "1st track 3D IP (jet bin1)",    1, 1, 1);              
    P1D.PlotStack("IP3d1_jetptbin2",   "1st track 3D IP (jet bin2)",    1, 1, 1);              
    P1D.PlotStack("IP3d1_jetptbin3",   "1st track 3D IP (jet bin3)",    1, 1, 1);              
                         
    P1D.PlotStack("IP3d2_jetptbin1",   "2nd track 3D IP (jet bin1)",    1, 1, 1);              
    P1D.PlotStack("IP3d2_jetptbin2",   "2nd track 3D IP (jet bin2)",    1, 1, 1);              
    P1D.PlotStack("IP3d2_jetptbin3",   "2nd track 3D IP (jet bin3)",    1, 1, 1);              
                        
    P1D.PlotStack("IP3d3_jetptbin1",   "3rd track 3D IP (jet bin1)",    1, 1, 1);              
    P1D.PlotStack("IP3d3_jetptbin2",   "3rd track 3D IP (jet bin2)",    1, 1, 1);              
    P1D.PlotStack("IP3d3_jetptbin3",   "3rd track 3D IP (jet bin3)",    1, 1, 1);              
                               
    // P1D.PlotStack("IP3dsigsorted_jetptbin1",    "",    1, 1, 1);      
    //  P1D.PlotStack("IP3dsigsorted_jetptbin2",    "",    1, 1, 1);      
    //  P1D.PlotStack("IP3dsigsorted_jetptbin3",    "",    1, 1, 1);      
                             
    P1D.PlotStack("IP3d1sigsorted_jetptbin1",   "1st track 3D IP sig. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d1sigsorted_jetptbin2",   "1st track 3D IP sig. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d1sigsorted_jetptbin3",   "1st track 3D IP sig. (jet bin3)",    1, 1, 1);     
                             		                            
    P1D.PlotStack("IP3d2sigsorted_jetptbin1",   "2nd track 3D IP sig. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d2sigsorted_jetptbin2",   "2nd track 3D IP sig. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d2sigsorted_jetptbin3",   "2nd track 3D IP sig. (jet bin3)",    1, 1, 1);     
                             		                            
    P1D.PlotStack("IP3d3sigsorted_jetptbin1",   "3rd track 3D IP sig. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d3sigsorted_jetptbin2",   "3rd track 3D IP sig. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d3sigsorted_jetptbin3",   "3rd track 3D IP sig. (jet bin3)",    1, 1, 1);     
                            
    //  P1D.PlotStack("IP3dErrorsorted_jetptbin1",   "",    1, 1, 1);    
    //  P1D.PlotStack("IP3dErrorsorted_jetptbin2",   "",    1, 1, 1);    
    //  P1D.PlotStack("IP3dErrorsorted_jetptbin3",   "",    1, 1, 1);    
                              
    P1D.PlotStack("IP3d1Errorsorted_jetptbin1",   "1st track 3D IP err. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d1Errorsorted_jetptbin2",   "1st track 3D IP err. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d1Errorsorted_jetptbin3",   "1st track 3D IP err. (jet bin3)",    1, 1, 1);     
                                 	                                   
    P1D.PlotStack("IP3d2Errorsorted_jetptbin1",   "2nd track 3D IP err. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d2Errorsorted_jetptbin2",   "2nd track 3D IP err. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d2Errorsorted_jetptbin3",   "2nd track 3D IP err. (jet bin3)",    1, 1, 1);     
                              		                                   
    P1D.PlotStack("IP3d3Errorsorted_jetptbin1",   "3rd track 3D IP err. (jet bin1)",    1, 1, 1);     
    P1D.PlotStack("IP3d3Errorsorted_jetptbin2",   "3rd track 3D IP err. (jet bin2)",    1, 1, 1);     
    P1D.PlotStack("IP3d3Errorsorted_jetptbin3",   "3rd track 3D IP err. (jet bin3)",    1, 1, 1);     
          
    P1D.PlotStack("sv_mass_jetptbin1",   "2-track SV mass (jet bin1)",    1, 0, 1);            
    P1D.PlotStack("sv_mass_jetptbin2",   "2-track SV mass (jet bin2)",    1, 0, 1);            
    P1D.PlotStack("sv_mass_jetptbin3",   "2-track SV mass (jet bin3)",    1, 0, 1);            
                            
    P1D.PlotStack("sv_mass_3tr_jetptbin1",   "3-track SV mass (jet bin1)",    1, 0, 1);        
    P1D.PlotStack("sv_mass_3tr_jetptbin2",   "3-track SV mass (jet bin2)",    1, 0, 1);        
    P1D.PlotStack("sv_mass_3tr_jetptbin3",   "3-track SV mass (jet bin3)",    1, 0, 1);        
                             
    //  P1D.PlotStack("sv_ipsigcharm_jetptbin1",   "",    1, 1, 1);      
    //  P1D.PlotStack("sv_ipsigcharm_jetptbin2",   "",    1, 1, 1);      
    //  P1D.PlotStack("sv_ipsigcharm_jetptbin3",   "",    1, 1, 1);      
                           
    P1D.PlotStack("sv_eratio_jetptbin1",   "SV energy ratio (jet bin1)",    1, 0, 0);          
    P1D.PlotStack("sv_eratio_jetptbin2",   "SV energy ratio (jet bin2)",    1, 0, 0);          
    P1D.PlotStack("sv_eratio_jetptbin3",   "SV energy ratio (jet bin3)",    1, 0, 0);          
                                
    P1D.PlotStack("sv_flightsig2d_jetptbin1",   "2D flight sig. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig2d_jetptbin2",   "2D flight sig. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig2d_jetptbin3",   "2D flight sig. (jet bin3)",    1, 1, 1);       
                             
    P1D.PlotStack("sv_flightsig3d_jetptbin1",   "3D flight sig. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig3d_jetptbin2",   "3D flight sig. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightsig3d_jetptbin3",   "3D flight sig. (jet bin3)",    1, 1, 1);       
                               
    P1D.PlotStack("sv_flightdis2d_jetptbin1",   "2D flight dist. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis2d_jetptbin2",   "2D flight dist. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis2d_jetptbin3",   "2D flight dist. (jet bin3)",    1, 1, 1);       
                               
    P1D.PlotStack("sv_flightdis3d_jetptbin1",   "3D flight dist. (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis3d_jetptbin2",   "3D flight dist. (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("sv_flightdis3d_jetptbin3",   "3D flight dist. (jet bin3)",    1, 1, 1);       
                                 
    P1D.PlotStack("sv_flightsig3d_3tr_jetptbin1",   "3-track 3D flight dist. (jet bin1)",    1, 1, 1);   
    P1D.PlotStack("sv_flightsig3d_3tr_jetptbin2",   "3-track 3D flight dist. (jet bin2)",    1, 1, 1);   
    P1D.PlotStack("sv_flightsig3d_3tr_jetptbin3",   "3-track 3D flight dist. (jet bin3)",    1, 1, 1);   
                               


    P1D.PlotStack("sv_deltar_jet_jetptbin1",   "#Delta R(vertex dir., jet) (jet bin1)",    1, 0, 1);        
    P1D.PlotStack("sv_deltar_jet_jetptbin2",   "#Delta R(vertex dir., jet) (jet bin2)",    1, 0, 1);        
    P1D.PlotStack("sv_deltar_jet_jetptbin3",   "#Delta R(vertex dir., jet) (jet bin3)",    1, 0, 1);        
                                
    P1D.PlotStack("sv_deltar_sum_jet_jetptbin1",   "#Delta R(vertex {#vec p}, jet) (jet bin1)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_jet_jetptbin2",   "#Delta R(vertex {#vec p}, jet) (jet bin2)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_jet_jetptbin3",   "#Delta R(vertex {#vec p}, jet) (jet bin3)",    1, 0, 1);    
                               
    P1D.PlotStack("sv_deltar_sum_dir_jetptbin1",   "#Delta R(vertex {#vec p}, vertex dir.) (jet bin1)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_dir_jetptbin2",   "#Delta R(vertex {#vec p}, vertex dir.) (jet bin2)",    1, 0, 1);    
    P1D.PlotStack("sv_deltar_sum_dir_jetptbin3",   "#Delta R(vertex {#vec p}, vertex dir.) (jet bin3)",    1, 0, 1);    
            
    P1D.PlotStack("discri_tche_jetptbin1",   "TCHE discriminator (jet bin1)",    1, 1, 1);          
    P1D.PlotStack("discri_tche_jetptbin2",   "TCHE discriminator (jet bin2)",    1, 1, 1);          
    P1D.PlotStack("discri_tche_jetptbin3",   "TCHE discriminator (jet bin3)",    1, 1, 1);          
                              
    P1D.PlotStack("discri_tchp_jetptbin1",   "TCHP discriminator (jet bin1)",    1, 1, 1);          
    P1D.PlotStack("discri_tchp_jetptbin2",   "TCHP discriminator (jet bin2)",    1, 1, 1);          
    P1D.PlotStack("discri_tchp_jetptbin3",   "TCHP discriminator (jet bin3)",    1, 1, 1);          
                            
    P1D.PlotStack("discri_ssche0_jetptbin1",   "SSVHE discriminator (jet bin1)",    1, 1, 1);        
    P1D.PlotStack("discri_ssche0_jetptbin2",   "SSVHE discriminator (jet bin2)",    1, 1, 1);        
    P1D.PlotStack("discri_ssche0_jetptbin3",   "SSVHE discriminator (jet bin3)",    1, 1, 1);        
                           
    P1D.PlotStack("discri_ssche_jetptbin1",   "SSVHE discriminator (jet bin1)",    1, 0, 0);         
    P1D.PlotStack("discri_ssche_jetptbin2",   "SSVHE discriminator (jet bin2)",    1, 0, 0);         
    P1D.PlotStack("discri_ssche_jetptbin3",   "SSVHE discriminator (jet bin3)",    1, 0, 0);         
                         
    P1D.PlotStack("discri_sschp0_jetptbin1",   "SSVHP discriminator (jet bin1)",    1, 1, 1);        
    P1D.PlotStack("discri_sschp0_jetptbin2",   "SSVHP discriminator (jet bin2)",    1, 1, 1);        
    P1D.PlotStack("discri_sschp0_jetptbin3",   "SSVHP discriminator (jet bin3)",    1, 1, 1);        
                                
    P1D.PlotStack("discri_sschp_jetptbin1",   "SSVHP discriminator (jet bin1)",    1, 0, 0);         
    P1D.PlotStack("discri_sschp_jetptbin2",   "SSVHP discriminator (jet bin2)",    1, 0, 0);         
    P1D.PlotStack("discri_sschp_jetptbin3",   "SSVHP discriminator (jet bin3)",    1, 0, 0);         
                            
    P1D.PlotStack("discri_jetprob_jetptbin1",   "JP discriminator (jet bin1)",    1, 1, 1);       
    P1D.PlotStack("discri_jetprob_jetptbin2",   "JP discriminator (jet bin2)",    1, 1, 1);       
    P1D.PlotStack("discri_jetprob_jetptbin3",   "JP discriminator (jet bin3)",    1, 1, 1);       
                               
    P1D.PlotStack("discri_jetbprob_jetptbin1",   "JBP discriminator (jet bin1)",    1, 1, 1);      
    P1D.PlotStack("discri_jetbprob_jetptbin2",   "JBP discriminator (jet bin2)",    1, 1, 1);      
    P1D.PlotStack("discri_jetbprob_jetptbin3",   "JBP discriminator (jet bin3)",    1, 1, 1);      

    P1D.PlotStack("sv_ipsigcharm_3tr",   "",    1, 1, 1);              
    P1D.PlotStack("sv_eratio_3tr",   "3-track SV energy ratio",    1, 0, 0);                  
    P1D.PlotStack("sv_vtx_pt_3tr",   "3-track SV vtx p_{T}",    1, 1, 1);                  

    P1D.PlotStack("sv_trackmul_centered",   "SV track multiplicity",    1, 1, 1);           
    P1D.PlotStack("sv_trackmul1_centered",   "SV track multiplicity",    1, 1, 1);          
    P1D.PlotStack("sv_tracksel_centered",   "SV track multiplicity",    1, 1, 1);           


    P1D.PlotStack("npv1_IP3d2sig0",        "2nd sorted track 3D IP significance",   1, 1, 1);  
    P1D.PlotStack(  "npv1_IP3d3sig0",        "3rd sorted track 3D IP significance", 1, 1, 1);  
    // P1D.PlotStack( "npv1_ntracks_jet",      "nr. of tracks in jet (1-6 PVs)",                   1, 1, 1);  
    //  P1D.PlotStack( "npv1_nseltracks_jet",   "nr. of sel tracks in jet (1-6 PVs)",               1, 1, 1);  
    P1D.PlotStack( "npv1_ncutseltracks_jet","nr. of tracks in jet (1-6 PVs)",           1, 1, 1);  
    P1D.PlotStack( "npv1_pt_tracks",        "Pt of tracks (1-6 PVs)",                         1, 1, 1);  
    P1D.PlotStack( "npv1_pt_seltracks",     "Pt of  sel tracks (1-6 PVs)",                    1, 1, 1);  
    P1D.PlotStack( "npv1_pt_cutseltracks",  "Pt of  cut sel tracks (1-6 PVs)",                1, 1, 1);  
    P1D.PlotStack( "npv1_nsv0",             "# of secondary vertices (1-6 PVs)",              1, 1, 1);  
    P1D.PlotStack( "npv1_nsv",              "# of secondary vertices (1-6 PVs)",              1, 1, 1);  
    P1D.PlotStack( "npv1_flightsig3d",      "Flight Significance 3D (1-6 PVs)",               1, 1, 1);  
    P1D.PlotStack( "npv1_flightsig3d_3tr",  "Flight Significance 3D for >=3tracks (1-6 PVs)", 1, 1, 1);  
    P1D.PlotStack( "npv1_svmass",           "SV mass (1-6 PVs)",                              1, 0, 1);  
    P1D.PlotStack( "npv1_svmass_3tr",       "SV mass for >=3tracks (1-6 PVs)",                1, 0, 1);  
    P1D.PlotStack( "npv1_nsv0_3tr",         "# of secondary vertices (>=3tr) (1-6 PVs)",      1, 1, 1);
 
    P1D.PlotStack("npv1_sveratio",        "SV eratio (1-6 PVs)",                  1, 0, 0);
    P1D.PlotStack("npv1_deltar_jet",      "SV deltaR jet (1-6 PVs)",              1, 0, 1);
    P1D.PlotStack("npv1_deltar_sum_jet",      "SV deltaR sum jet (1-6 PVs)",      1, 0, 1);
    P1D.PlotStack("npv1_deltar_sum_dir",      "SV deltaR sum dir (1-6 PVs)",      1, 0, 1);
    P1D.PlotStack("npv1_vtxpt",      "SV vtx pt (1-6 PVs)",                       1, 1, 1);
    P1D.PlotStack("npv1_discri_tche",      "TCHE discriminator (1-6 PVs)",        1, 1, 1);
    P1D.PlotStack("npv1_discri_tchp",      "TCHP discriminator (1-6 PVs)",        1, 1, 1);
    P1D.PlotStack("npv1_discri_jetprob",      "JP discriminator (1-6 PVs)",       1, 1, 1);
    P1D.PlotStack("npv1_discri_jetbprob",      "JBP discriminator (1-6 PVs)",     1, 1, 1);
    P1D.PlotStack("npv1_discri_ssche0",       "SSVHE Discriminator (1-6 PVs)",    1, 1, 1);
    P1D.PlotStack("npv1_discri_ssche",   "SSVHE Discriminator (1-6 PVs)",         1, 0, 0);
    P1D.PlotStack("npv1_discri_sschp0",   "SSVHP Discriminator (1-6 PVs)",        1, 1, 1);
    P1D.PlotStack("npv1_discri_sschp",   "SSVHP Discriminator (1-6 PVs)",         1, 0, 0);
    P1D.PlotStack("npv1_muon_Pt",        "Muon p_{T} (1-6 PVs)",                  1, 1, 1);
    P1D.PlotStack("npv1_muon_ptrel",    "Muon p_{T}^{rel} (1-6 PVs)",             1, 0, 1);
    P1D.PlotStack("npv1_muon_DeltaR",         "Muon1 deltaR (1-6 PVs)",           1, 0, 1);


    P1D.PlotStack("npv2_IP3d2sig0",        "2nd sorted track 3D IP significance (#PV:7-10)",   1, 1, 1);  
    P1D.PlotStack(  "npv2_IP3d3sig0",        "3rd sorted track 3D IP significance (#PV:7-10)", 1, 1, 1);  
    // P1D.PlotStack( "npv2_ntracks_jet",      "nr. of tracks in jet (#PV:7-10)",                   1, 1, 1);  
    //  P1D.PlotStack( "npv2_nseltracks_jet",   "nr. of sel tracks in jet (#PV:7-10)",               1, 1, 1);  
    P1D.PlotStack( "npv2_ncutseltracks_jet","nr. of tracks in jet (#PV:7-10)",           1, 1, 1);  
    P1D.PlotStack( "npv2_pt_tracks",        "Pt of tracks (#PV:7-10)",                         1, 1, 1);  
    P1D.PlotStack( "npv2_pt_seltracks",     "Pt of  sel tracks (#PV:7-10)",                    1, 1, 1);  
    P1D.PlotStack( "npv2_pt_cutseltracks",  "Pt of  cut sel tracks (#PV:7-10)",                1, 1, 1);  
    P1D.PlotStack( "npv2_nsv0",             "# of secondary vertices (#PV:7-10)",              1, 1, 1);  
    P1D.PlotStack( "npv2_nsv",              "# of secondary vertices (#PV:7-10)",              1, 1, 1);  
    P1D.PlotStack( "npv2_flightsig3d",      "Flight Significance 3D (#PV:7-10)",               1, 1, 1);  
    P1D.PlotStack( "npv2_flightsig3d_3tr",  "Flight Significance 3D for >=3tracks (#PV:7-10)", 1, 1, 1);  
    P1D.PlotStack( "npv2_svmass",           "SV mass (#PV:7-10)",                              1, 0, 1);  
    P1D.PlotStack( "npv2_svmass_3tr",       "SV mass for >=3tracks (#PV:7-10)",                1, 0, 1);  
    P1D.PlotStack( "npv2_nsv0_3tr",         "# of secondary vertices (>=3tr) (#PV:7-10)",      1, 1, 1);
 
    P1D.PlotStack("npv2_sveratio",        "SV eratio (#PV:7-10)",                  1, 0, 0);
    P1D.PlotStack("npv2_deltar_jet",      "SV deltaR jet (#PV:7-10)",              1, 0, 1);
    P1D.PlotStack("npv2_deltar_sum_jet",      "SV deltaR sum jet (#PV:7-10)",      1, 0, 1);
    P1D.PlotStack("npv2_deltar_sum_dir",      "SV deltaR sum dir (#PV:7-10)",      1, 0, 1);
    P1D.PlotStack("npv2_vtxpt",      "SV vtx pt (#PV:7-10)",                       1, 1, 1);
    P1D.PlotStack("npv2_discri_tche",      "TCHE discriminator (#PV:7-10)",        1, 1, 1);
    P1D.PlotStack("npv2_discri_tchp",      "TCHP discriminator (#PV:7-10)",        1, 1, 1);
    P1D.PlotStack("npv2_discri_jetprob",      "JP discriminator (#PV:7-10)",       1, 1, 1);
    P1D.PlotStack("npv2_discri_jetbprob",      "JBP discriminator (#PV:7-10)",     1, 1, 1);
    P1D.PlotStack("npv2_discri_ssche0",       "SSVHE Discriminator (#PV:7-10)",    1, 1, 1);
    P1D.PlotStack("npv2_discri_ssche",   "SSVHE Discriminator (#PV:7-10)",         1, 0, 0);
    P1D.PlotStack("npv2_discri_sschp0",   "SSVHP Discriminator (#PV:7-10)",        1, 1, 1);
    P1D.PlotStack("npv2_discri_sschp",   "SSVHP Discriminator (#PV:7-10)",         1, 0, 0);
    P1D.PlotStack("npv2_muon_Pt",        "Muon p_{T} (#PV:7-10)",                  1, 1, 1);
    P1D.PlotStack("npv2_muon_ptrel",    "Muon p_{T}^{rel} (#PV:7-10)",             1, 0, 1);
    P1D.PlotStack("npv2_muon_DeltaR",         "Muon1 deltaR (#PV:7-10)",           1, 0, 1);


    P1D.PlotStack("npv3_IP3d2sig0",        "2nd sorted track 3D IP significance (#PV#geq11)",   1, 1, 1);  
    P1D.PlotStack(  "npv3_IP3d3sig0",        "3rd sorted track 3D IP significance (#PV#geq11)", 1, 1, 1);  
    // P1D.PlotStack( "npv3_ntracks_jet",      "nr. of tracks in jet (#PV#geq11)",                   1, 1, 1);  
    //  P1D.PlotStack( "npv3_nseltracks_jet",   "nr. of sel tracks in jet (#PV#geq11)",               1, 1, 1);  
    P1D.PlotStack( "npv3_ncutseltracks_jet","nr. of tracks in jet (#PV#geq11)",           1, 1, 1);  
    P1D.PlotStack( "npv3_pt_tracks",        "Pt of tracks (#PV#geq11)",                         1, 1, 1);  
    P1D.PlotStack( "npv3_pt_seltracks",     "Pt of  sel tracks (#PV#geq11)",                    1, 1, 1);  
    P1D.PlotStack( "npv3_pt_cutseltracks",  "Pt of  cut sel tracks (#PV#geq11)",                1, 1, 1);  
    P1D.PlotStack( "npv3_nsv0",             "# of secondary vertices (#PV#geq11)",              1, 1, 1);  
    P1D.PlotStack( "npv3_nsv",              "# of secondary vertices (#PV#geq11)",              1, 1, 1);  
    P1D.PlotStack( "npv3_flightsig3d",      "Flight Significance 3D (#PV#geq11)",               1, 1, 1);  
    P1D.PlotStack( "npv3_flightsig3d_3tr",  "Flight Significance 3D for >=3tracks (#PV#geq11)", 1, 1, 1);  
    P1D.PlotStack( "npv3_svmass",           "SV mass (#PV#geq11)",                              1, 0, 1);  
    P1D.PlotStack( "npv3_svmass_3tr",       "SV mass for >=3tracks (#PV#geq11)",                1, 0, 1);  
    P1D.PlotStack( "npv3_nsv0_3tr",         "# of secondary vertices (>=3tr) (#PV#geq11)",      1, 1, 1);
 
    P1D.PlotStack("npv3_sveratio",        "SV eratio (#PV#geq11)",                  1, 0, 0);
    P1D.PlotStack("npv3_deltar_jet",      "SV deltaR jet (#PV#geq11)",              1, 0, 1);
    P1D.PlotStack("npv3_deltar_sum_jet",      "SV deltaR sum jet (#PV#geq11)",      1, 0, 1);
    P1D.PlotStack("npv3_deltar_sum_dir",      "SV deltaR sum dir (#PV#geq11)",      1, 0, 1);
    P1D.PlotStack("npv3_vtxpt",      "SV vtx pt (#PV#geq11)",                       1, 1, 1);
    P1D.PlotStack("npv3_discri_tche",      "TCHE discriminator (#PV#geq11)",        1, 1, 1);
    P1D.PlotStack("npv3_discri_tchp",      "TCHP discriminator (#PV#geq11)",        1, 1, 1);
    P1D.PlotStack("npv3_discri_jetprob",      "JP discriminator (#PV#geq11)",       1, 1, 1);
    P1D.PlotStack("npv3_discri_jetbprob",      "JBP discriminator (#PV#geq11)",     1, 1, 1);
    P1D.PlotStack("npv3_discri_ssche0",       "SSVHE Discriminator (#PV#geq11)",    1, 1, 1);
    P1D.PlotStack("npv3_discri_ssche",   "SSVHE Discriminator (#PV#geq11)",         1, 0, 0);
    P1D.PlotStack("npv3_discri_sschp0",   "SSVHP Discriminator (#PV#geq11)",        1, 1, 1);
    P1D.PlotStack("npv3_discri_sschp",   "SSVHP Discriminator (#PV#geq11)",         1, 0, 0);
    P1D.PlotStack("npv3_muon_Pt",        "Muon p_{T} (#PV#geq11)",                  1, 1, 1);
    P1D.PlotStack("npv3_muon_ptrel",    "Muon p_{T}^{rel} (#PV#geq11)",             1, 0, 1);
    P1D.PlotStack("npv3_muon_DeltaR",         "Muon1 deltaR (#PV#geq11)",           1, 0, 1);

    P2D.PlotStack2D("track_vs_jetpt", "jet p_{T}", "track multiplicity",  0,10,35);
    P2D.PlotStack2D("cutseltrack_vs_jetpt", "jet p_{T}", "track multiplicity", 0,4,17);
    P2D.PlotStack2D("sv_track_vs_jetpt",    "jet p_{T}", "SV track multiplicity",  0,1.,5.5);

    P2D.PlotStack2D("sv_mass_vs_flightDistSignCut", "flight dist sign.", "SV mass", 0, 0, 6.5);
    P2D.PlotStack2D("sv_mass_vs_flightDistCut","flight dist.", "SV mass",  0,0,4.5);

    P2D.PlotStack2D("avg_sv_mass_vs_jetpt", "jet p_{T}", "SV mass", 0,0,5.5);

    P2D.PlotStack2D("sv_deltar_jet_vs_jetpt", "jet p_{T}", "#Delta R(vertex dir., jet)", 0,0,0.13);
    P2D.PlotStack2D("sv_deltar_sum_jet_vs_jetpt","jet p_{T}", "#Delta R(vertex {#vec p}, jet)",  0,0,0.09);
    P2D.PlotStack2D("sv_deltar_sum_dir_vs_jetpt","jet p_{T}", "#Delta R(vertex {#vec p}, vertex dir.)",  0,0,0.09);
    P2D.PlotStack2D("muon_ptrel_vs_jetpt", "jet p_{T}","muon p_{T}^{rel}",  0);
    P2D.PlotStack2D("muon_DeltaR_vs_jetpt","jet p_{T}","muon #Delta R",   0);

    P2D.PlotStack2D("sv_eratio_vs_jetpt","jet p_{T}","SV energy ratio",  0,0,0.75);
    P2D.PlotStack2D("sv_vtx_pt_vs_jetpt","jet p_{T}","SV p_{T}",  0,0,55);
#endif
}

int main() {
  run();
  return 1;
}

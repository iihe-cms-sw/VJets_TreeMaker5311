#include "Rtypes.h"
//##################################################
//#####   General Settings for all codes   #########
//##################################################
//------------(I)Samples------------------------------
#define bTagNtupleVersion 5
//it stands for version of the commissioning ntuples
//Version3=14May
//Version4=27Dec
//The samples are defined in the end of this file, classfied by versions.
//Each version must contain at least five variables:
//(1)const char JetDATA[]="" : the Jet data
//(2)const char MetDATA[]="" : the MET data
//(3)const char *MC_files[]={"","",.....}: the files locations
//(4)const char *MC_SampleNames[]={"","",.....}: the sample names
//(5)const Float_t MC_Weights[]={,,....}: the sample weights 
//   weight = ( filter(QCDSamples=1) * sect) /n_ev;
//The program will skip empty strings. The arrays (3)(4)(5) have to have the same size and have to be corresponding.
//------------(II) the PU reweight-------------------------------------
//From https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupInformation
const Double_t Spring11[]={0.0698146584, 0.0698146584, 0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0698146584,0.0630151648,0.0526654164,0.0402754482,0.0292988928,0.0194384503,0.0122016783,0.007207042,0.004003637,0.0020278322,0.0010739954,0.0004595759,0.0002229748,0.0001028162,4.58337152809607E-05};
// In-time (Poisson-smeared) Distribution for Fall 2011
// From https://twiki.cern.ch/twiki/bin/view/CMS/PileupMCReweightingUtilities
// For MC ntuple, use in-time only num of PUs for reweighting (numberOfPUVertices)
const Double_t Fall2011[55]={0.014583699,0.025682975,0.038460562,0.049414536,0.056931087,0.061182816,0.062534625,0.061476918,0.058677499,0.055449877,0.051549051,0.047621024,0.043923799,0.040569076,0.037414654,0.034227033,0.031437714,0.028825596,0.026218978,0.023727061,0.021365645,0.01918743,0.016972815,0.014920601,0.013038989,0.011293777,0.009612465,0.008193556,0.006888047,0.005715239,0.004711232,0.003869926,0.003154521,0.002547417,0.002024714,0.001574411,0.001245808,0.000955206,0.000735305,0.000557304,0.000412503,0.000305502,0.000231002,0.000165701,0.000121201,9.30006E-05,6.40004E-05,4.22003E-05,2.85002E-05,1.96001E-05,1.59001E-05,1.01001E-05,8.50006E-06,6.60004E-06,2.70002E-06};
const Double_t Summer2012[60] = {2.344E-05,2.344E-05,2.344E-05,2.344E-05,4.687E-04,4.687E-04,7.032E-04,9.414E-04,1.234E-03,1.603E-03,2.464E-03,3.250E-03,5.021E-03,6.644E-03,8.502E-03,1.121E-02,1.518E-02,2.033E-02,2.608E-02,3.171E-02,3.667E-02,4.060E-02,4.338E-02,4.520E-02,    4.641E-02,    4.735E-02,    4.816E-02,    4.881E-02,    4.917E-02,    4.909E-02,    4.842E-02,    4.707E-02,    4.501E-02,    4.228E-02,    3.896E-02,    3.521E-02,    3.118E-02,    2.702E-02,    2.287E-02,    1.885E-02,    1.508E-02,    1.166E-02,    8.673E-03,    6.190E-03,    4.222E-03,    2.746E-03,    1.698E-03,    9.971E-04,    5.549E-04,    2.924E-04,    1.457E-04,    6.864E-05,    3.054E-05,    1.282E-05,    5.081E-06,    1.898E-06,    6.688E-07,2.221E-07,    6.947E-08,    2.047E-08   };  
//Link to a predefined PU scenario
#define npu_probs Summer2012//comment it out to let the program calculate the pudistribution from ntuples
//the file name of the PU distribution in data
//See https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupReweighting for details
//#define pudistribution_data_filename "2012Prompt_json_DCSONLY_190389-190688.root"//comment it out to reweight PU to flat distribution (PU0)

#ifdef npu_probs
const UInt_t MAXPU=sizeof(npu_probs)/sizeof(const Double_t)-1;
#else
const UInt_t MAXPU=60;
#endif
//##############################
//#######   Samples   ##########
//##############################
#if bTagNtupleVersion == 0 //39X
const char JetDATA[]="";
const char MetDATA[]="";
const char QCD15to30[]="";
const char QCD30to50[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_30to50_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCD50to80[]="";
const char QCD80to120[]="";
const char QCD120to170[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_120to170_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCD170to300[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_170to300_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCD300to470[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_300to470_TuneZ2_7TeV_pythia6_PU/*.root";
const char *MC_files[]={};
const char *MC_SampleNames[]={};
Float_t MC_Weights[]={};
#endif

#if bTagNtupleVersion == 1 //39X
const char JetDATA[]="";
const char MetDATA[]="";
const char QCD5to15[]="";
const char QCD15to30[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_15to30_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCD30to50[]="";
const char QCD50to80[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_50to80_TuneZ2_7TeV_pythia6_PU_v2/*.root";
const char QCD80to120[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Pt_80to120_TuneZ2_7TeV_pythia6_PU_prescale/*.root";
const char QCD120to170[]="";
const char QCD170to300[]="";
const char QCD300to470[]="";
const char QCDMu15to20[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT15to20_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCDMu20to30[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT20to30_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCDMu30to50[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT30to50_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCDMu50to80[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT50to80_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCDMu80to120[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT80to120_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCDMu120to150[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT120to150_TuneZ2_7TeV_pythia6_PU/*.root";
const char QCDMu150plus[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V1/QCD_Mu_PT150_TuneZ2_7TeV_pythia6_PU/*.root"; 
const char *MC_files[]={QCD5to15,QCD15to30,QCD30to50,QCD50to80,QCD80to120,QCD120to170,QCD170to300,QCD300to470,QCDMu15to20,QCDMu20to30,QCDMu30to50,QCDMu50to80,QCDMu80to120,QCDMu120to150,QCDMu150plus};
const char *MC_SampleNames[]={"qcd5","qcd15","qcd30","qcd50","qcd80","qcd120","qcd170","qcd300","qcdmu15","qcdmu20","qcdmu30","qcdmu50","qcdmu80","qcdmu120","qcdmu150"};
Float_t MC_Weights[]={36750000000.  / 1648096.,//weight05
		      815900000. / 5420080.,//weight15
		      53120000.  / 3244045.,//weight30
		      6359000.   / 2739226.,//weight50
		      784300.    / 3208299.,//weight80
		      115100.    / 3045200.,//weight120
		      24260.     / 3212880.,//weight170
		      1168.      / 3060200.,//weight300
		      (0.00254    *579200000.)/2884915.,//wmu15
		      (0.00518    *236300000.)/11417239.,//wmu20
		      (0.01090    *53070000. )/11431864.,//wmu30
		      (0.02274    *6351000.  )/10748755.,//wmu50
		      (0.03700    *785100.   )/3191979.,//wmu80
		      (0.04777   *92950.    )/998503.,//wmu120
		      (0.05964   *47580.)/1022541.//wmu150
};
#endif

#if bTagNtupleVersion == 2 //311X
const char JetDATA[]="";
const char MetDATA[]="";
const char QCD15to30[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD15to30/QCD15to30/*.root";
const char QCD30to50[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD30to50/*.root";
const char QCD50to80[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD50to80/*.root";
const char QCD80to120[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD80to120/*.root";
const char QCD120to170[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD120to170/*.root";
const char QCD170to300[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD170to300/*.root";
const char QCD300to470[]="/opt/sbg/cms/ui2_data2/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCD300to470/*.root";
const char QCDMu15to20[]="/opt/sbg/cms/ui2_data1/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu15to20/*.root";
const char QCDMu20to30[]="/opt/sbg/cms/ui2_data1/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu20to30/*.root";
const char QCDMu30to50[]="/opt/sbg/cms/ui2_data1/ccollard/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu30to50/*.root";
const char QCDMu50to80[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu50to80/QCDMu50to80/*.root";
const char QCDMu80to120[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu80to120/QCDMu80to120/*.root";
const char QCDMu120to150[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu120to150/QCDMu120to150/*.root";
const char QCDMu150plus[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V3/bTagCommissioning_30_03_11/QCDMu150/QCDMu150/*.root";
const char *MC_files[]={QCD15to30,QCD30to50,QCD50to80,QCD80to120,QCD120to170,QCD170to300,QCD300to470,QCDMu15to20,QCDMu20to30,QCDMu30to50,QCDMu50to80,QCDMu80to120,QCDMu120to150,QCDMu150plus};
const char *MC_SampleNames[]={"qcd15","qcd30","qcd50","qcd80","qcd120","qcd170","qcd300","qcdmu15","qcdmu20","qcdmu30","qcdmu50","qcdmu80","qcdmu120","qcdmu150"};
Float_t MC_Weights[]={815900000. / 8213600.,//weight15
		      53120000.  / 6529320.,//weight30
		      6359000.   / 4301392.,//weight50
		      784300.    / 6407732.,//weight80
		      115100.    / 6090400.,//weight120
		      24260.     / 5684160.,//weight170
		      1168.      / 6336960.,//weight300
		      (0.00254    *579200000.)/2884915.,//wmu15
		      (0.00518    *236300000.)/11352301.,//wmu20
		      (0.01090    *53070000. )/10909951.,//wmu30
		      (0.02274    *6351000.  )/10686315.,//wmu50
		      (0.03700    *785100.   )/3183540.,//wmu80
		      (0.04777   *92950.    )/991024.,//wmu120
		      (0.05964   *47580.    )/1015900.//wmu150
};
#endif

#if bTagNtupleVersion == 3 // 311X DA & SV fix
const char JetDATA[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/v42x_v1/Data/link_jet/*.root";
//"/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/Data/Jet2011/*.root";
const char MetDATA[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/v42x_v1/Data/link_met/*.root";
//"/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/Data/Met2011/*.root"
const char QCD15to30[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/14MayQCDPt_15to30/*.root";
const char QCD30to50[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_14MayQCDPt_30to50/*.root";
const char QCD50to80[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_14MayQCDPt_50to80/*.root";
const char QCD80to120[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_14MayQCDPt_80to120/*.root";
const char QCD120to170[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_14MayQCDPt_120to170/*.root";
const char QCD170to300[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_14MayQCDPt_170to300/*.root";
const char QCD300to470[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_14MayQCDPt_300to470/*.root";
const char QCDMu15to20[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/14MayQCDMuEnrichedPt_15to20/*.root";
const char QCDMu20to30[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/14MayQCDMuEnrichedPt_20to30/*.root";
const char QCDMu30to50[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/QCDMuEnriched_Pt-30to50/*.root";
const char QCDMu50to80[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/QCDMuEnriched_Pt-50to80/*.root";
const char QCDMu80to120[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/QCDMuEnriched_Pt-80to120/*.root";
const char QCDMu120to150[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/14MayQCDMuEnrichedPt_120to150/*.root";
const char QCDMu150plus[]="/opt/sbg/data/data1/cms/ccollard/files/btagCommNtuple/V4_DA_SVfix/MC/14MayQCDMuEnrichedPt_150/*.root";
const char *MC_files[]={QCD15to30,QCD30to50,QCD50to80,QCD80to120,QCD120to170,QCD170to300,QCD300to470,QCDMu15to20,QCDMu20to30,QCDMu30to50,QCDMu50to80,QCDMu80to120,QCDMu120to150,QCDMu150plus};
const char *MC_SampleNames[]={"qcd15","qcd30","qcd50","qcd80","qcd120","qcd170","qcd300","qcdmu15","qcdmu20","qcdmu30","qcdmu50","qcdmu80","qcdmu120","qcdmu150"};
Float_t MC_Weights[]={815900000. / 8213600.,//weight15
		      53120000.  / 6529320.,//weight30
		      6359000.   / 4301392.,//weight50
		      784300.    / 6407732.,//weight80
		      115100.    / 6090400.,//weight120
		      24260.     / 5684160.,//weight170
		      1168.      / 6336960.,//weight300
		      (0.00254    *579200000.)/2884915.,//wmu15
		      (0.00518    *236300000.)/11352301.,//wmu20
		      (0.01090    *53070000. )/10909951.,//wmu30
		      (0.02274    *6351000.  )/10686315.,//wmu50
		      (0.03700    *785100.   )/3183540.,//wmu80
		      (0.04777   *92950.    )/991024.,//wmu120
		      (0.05964   *47580.    )/1015900.//wmu150
};
#endif

#if bTagNtupleVersion == 4
const char JetDATA[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_15Apr2012_Jet_Run2012A-PromptReco-v1/*.root";
const char MetDATA[]="";
const char QCD15to3000[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_11Apr2012_QCD_Pt-15to3000/*.root";
const char QCD15to30[]="/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_27Dec2011_QCD_Pt-15to30/*.root";
const char QCD30to50[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-30to50/*.root";
const char QCD50to80[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-50to80/*.root";
const char QCD80to120[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-80to120/*.root";
const char QCD120to170[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-120to170/*.root";
const char QCD170to300[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-170to300/*.root";
const char QCD300to470[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-300to470/*.root";
const char QCD470to600[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-470to600/*.root";
const char QCD600to800[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-600to800/*.root";
const char QCD800to1000[]="/afs/cern.ch/user/z/zhangjin/MetaData/bTaggingNtuple_03Feb2012_QCD_Pt-800to1000/*.root";
const char *MC_files[]={QCD30to50,QCD50to80,QCD80to120,QCD120to170,QCD170to300,QCD300to470,QCD470to600,QCD600to800,QCD800to1000};
const char *MC_SampleNames[]={"qcd30","qcd50","qcd80","qcd120","qcd170","qcd300","qcd470","qcd600","qcd800"};
Float_t MC_Weights[]={53120000.  / 6583068.,//weight30
                      6359000.   / 6600000., //weight50
		      784300.    / 6581772.,//weight80
		      115100.    / 6127528.,//weight120
		      24260.     / 6220160.,//weight170
		      1168.      / 6432669.,//weight300
		      70.22      / 3990085.,//weight470
		      15.55      / 4245695.,//weight600
		      1.844      / 4053888. //weight800
};
#endif

#if bTagNtupleVersion == 5
const char JetDATA[]="/opt/sbg/data/data2/cms/cbeluffi/bTaggingNtuple_30april_Jet_Run2012A_PromptReco-v1/*.root";
const char MetDATA[]="";
const char QCD120to170[]="/opt/sbg/data/data2/cms/cbeluffi/Ntuples_MC_8Tev/bTaggingNtuple_4April2012_QCD_Pt-120to170_8TeV/*.root";
const char QCD170to300[]="/opt/sbg/data/data2/cms/cbeluffi/Ntuples_MC_8Tev/bTaggingNtuple_4April2012_QCD_Pt-170to300_8TeV/*.root";
const char *MC_files[]={QCD120to170,QCD170to300};
const char *MC_SampleNames[]={"qcd120","qcd170"};
Float_t MC_Weights[]={115100.    / 242400.,//weight120
		      24260.     / 50000.,//weight170
};
#endif

const UInt_t nSamples=sizeof(MC_Weights)/sizeof(Float_t);

//############### Configuration of histograms maker ##############
#define RUN_ON_JET
#define PTVAL 140
#define DATAYEAR 2012

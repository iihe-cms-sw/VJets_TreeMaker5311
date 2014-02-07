#!/bin/bash
###############################################################
#Author : Caroline Collard (IPHC) - first commit : april 2011 #
#         Jinzhong Zhang (NEU,USA) - version 4 coding         #
###############################################################
ver=`grep "#define bTagNtupleVersion " Configuration.h`
ver=${ver#"#define bTagNtupleVersion "}
ver=`expr match "$ver" '\([0-9]*\)'`
line=`grep -n "#if bTagNtupleVersion == $ver" Configuration.h`
line=${line%:*}",\$p"
if [ -n "$1" ]; then 
    jobs=$1
    TAGGERS=$1
else
    jobs=`sed -n "$line" Configuration.h | grep -m 1 "const char \*MC_SampleNames"`
    jobs=${jobs#*\{}
    jobs=${jobs%\}*}
    jobs=${jobs//,/ }
    jobs=${jobs//\"/}
    jobs="data $jobs"
    TAGGERS="TCHE TCHP SSVHE SSVHP JetP JetBP CSVPF"
fi
echo -e "
-----------------------------------------------------------------------------------------------------------------
\e[32;07m You have to specify parameters in the Configuration.h\e[00m
-----------------------------------------------------------------------------------------------------------------

   I. the bTagNtuple Version
   \e[00;33m#define bTagNtupleVersion ?\e[00m
   It stands for version of the commissioning ntuples:
   Version0=not using?
   Version1=not using?
   Version2=not using?
   Version3=14May2011
   Version4=29Nov2011 and 01Dec2011

   II. where the data/MC are
   The samples are defined in the end of this file, classfied by versions.
   Each version must contain at least five variables:
     (1)\e[00;33mconst char JetDATA[]="";\e[00m : the Jet data
     (2)\e[00;33mconst char MetDATA[]="";\e[00m : the MET data
     (3)\e[00;33mconst char *MC_files[]={"","",.....};\e[00m : the files locations
     (4)\e[00;33mconst char *MC_SampleNames[]={"","",.....};\e[00m : the sample names
     (5)\e[00;33mconst Float_t MC_Weights[]={,,....};\e[00m : the sample weights 
        weight = ( filter(QCDSamples=1) * sect) /n_ev;
     The filenames should be separated by \",\", CERN castor file name, wildcards \"?\" and \"*\" are supported. e.g. \e[00;33m\"/castor/cern.ch/user/z/zhangjin/bTaggingNtuple_27Dec2011_QCD_Pt-120to170/*.root,/home/xxx/additional_QCD_Pt-120to170*.root\"\e[00m
     The program will skip empty strings. The arrays (3)(4)(5) have to have the same size and have to be corresponding.
     For MC, \e[0;31mif the MC_SampleName starts with \"qcd\", it will be used in Run_on_Jet;
             if the MC_SampleName starts with \"qcdmu\", it will be used in Run_on_bTag;
             if the MC_SampleName starts with neither of them, it will not be used.\e[00m
     For MC, the output histogram name is \e[00;33m\"histo_\"+string(MC_SampleNames[iSample])+\".root\"\e[00m
     For data, the output histogram name is histo_minijet2011.root or histo_minibtag2011.root. If you want to change both the names in \e[0;31mrun()\e[00m in btagNtupReader.C and the names in the constructor \e[00;33mtemplate<typename T> Plots<T>::Plots( Bool_t mu, Float_t ptval, UInt_t year )\e[00m in LoadDisplayRootPlot.h, but it is not necessary.

\e[32;07mCurrent settings:\e[00m
bTagNtupleVersion = \e[00;33m$ver\e[00m (Configuration.h)
samples =  \e[00;33m$jobs\e[00m (Configuration.h)
efficiency plots taggers = \e[00;33m$TAGGERS\e[00m (RunMeForEverything.sh)

\e[32;07mNow choose a mission to run ( if choose A or B, the job will run on lxplus batch):\e[00m
(1/A) Make Commissionging Root Files    
   a. Specify \"run on Jet\" or on MetBtag:
      \e[00;33m#define Run_on_Jet\e[00m --> Run_on_Jet otherwise Run_on_Btag
      \e[00;33m#define ptval 80\e[00m --> the single jet pt cut or the di-jet pt cut
      \e[00;33m#define DATAYEAR 2011\e[00m --> the year of the data
      if you do not want to use the default parameters, you need to change the corresponding sections in btagNtupReader.C, like \e[00;33m#if ptval_jet == 30 && defined(Run_on_Jet) \e[00m, like trigger path, JetPtthresholdsfornmu and cutMuonPt, etc.
 
   b. All outputs will be in \e[00;33mdir_jet[jetptcut]pu/\e[00m or \e[00;33mdir_btag[jetptcut]pu/\e[00m, about tens MB. The directory will be created by the code.

(2) Make Commissionging Root Plots From Commissionging Root Files
    a. Run step (1)
    b. After all root files are generated, run this step and then all outputs will be in \e[00;33mdir_jet[jetptcut]pu/\e[00m or \e[00;33mdir_btag[jetptcut]pu/\e[00m in pdf format by default.

(3/B) Make Efficiency Plots: Configure \"EfficiencyPlotter.C\"
   a. Specify which target is studying:
      \e[00;33m#define Jet\e[00m --> Reco Jets
      \e[00;33m#define GENJET\e[00m --> Gen Level Particle Jets (clustered generation level particles, not the original parton jet)
      \e[00;33m#define PARTON\e[00m -->  Gen Level Partons
   b. Change the pileup schemes in parameter part (B)
   c. Run this step and have the plots in the current directory. Default discriminators are SSVHE SSVHP TCHE TCHP. One can add more to this sh file based on instructions in EfficiencyPlotter.C.

(4) Combine Efficiencies of Different Taggers into one plot
   a. Run step(3)
   b. Setup \e[00;33mPUMIN\e[00m and \e[00;33mPUMAX\e[00m in CombineTaggersEfficiencies.C. The (PUMIN,PUMAX) senario must exist in EfficiencyPlotter.C.
   c. Run this step and have the pdf plots for udsgvsb and cvsb efficiencies.

(N) Say 'N' for additional information
"
Choice='S'
until [[ "$Choice" == [1234NnAB] ]]; do
    echo -en $"\e[33;07m Which one are you ready to run?(1-4/A/B/N)\e[00m"
    read -n 1 Choice
    echo
done
case $Choice in
    [1A])
	g++ `root-config --cflags --glibs` -o btagNtupReader btagNtupReader.C
	for i in $jobs
	do
	    if [ "$Choice" == "A" ]; then
		bsub -q 2nd "cd $CMSSW_BASE/src/;eval \`scramv1 runtime -sh\`;cd `pwd`;./btagNtupReader $i | tee $i.OUTPUT" 
	    else
		./btagNtupReader $i | tee $i.OUTPUT &
	    fi
	    sleep 10
	done
	echo -e "\e[33;01mPlease run step 2 after all jobs finish.\e[00m";;
   2) 
        g++ `root-config --cflags --glibs` -o displayPlots displayPlots.C
	./displayPlots
	rm btagNtupReader
	rm displayPlots;;
   [3B])
	g++ `root-config --cflags --glibs` -o EfficiencyPlotter EfficiencyPlotter.C
	for i in $TAGGERS
	do
	    if [ "$Choice" == "B" ]; then
		bsub -q 2nd "cd $CMSSW_BASE/src/;eval \`scramv1 runtime -sh\`;cd `pwd`;./EfficiencyPlotter \"$i\" | tee $i.OUTPUT" 
	    else
		./EfficiencyPlotter "$i" | tee $i.OUTPUT &
	    fi
	    sleep 10
	done;;
    4)
                g++ `root-config --cflags --glibs` -o CombineTaggersEfficiencies CombineTaggersEfficiencies.C
	./CombineTaggersEfficiencies $TAGGERS
                rm EfficiencyPlotter
                rm CombineTaggersEfficiencies;;
    [Nn])	
	echo -e '
---------------------------------------------------
* additional information: How to add a new histo? *
---------------------------------------------------
- Go in the  "Loop" function of btagNtupReader.C

- Define your histo following the template :
AddHisto(string name_of_histo, int j, string title, string xaxis, const int& nbins, const float& min, const float& max)
	example: \e[00;33mAddHisto(HistoBtag, "all_jetpt",       j,"PT of all jets",80,0.,400.);\e[00m
	the "j" index is used for MC samples to produce global histogram, b contribution, c contribution, light 
	contribution and gluon splitting into b.  If you need to keep this quark content info, put your new line
	in the "j" loop. Otherwise, put it after the loop.
	-->> Search for the comment "ADD HERE NEW HISTO" in  btagNtupReader.C to help you.
	There is also a template for 2D histo.

- Fill your histo
	example : \e[00;33mHistoBtag[0+j*nhisto_to_clone]->Fill(jetPt[i], weight);\e[00m         inside the "j" loop for 1D histo
	example : \e[00;33mHistoBtag[jmax*nhisto_to_clone+7]->Fill(numberOfPrimaryVertices,weight);\e[00m   outside the "j" loop for 1D histo
	The tricky thing is to know which index to use!
	--->  THE RULE is to increment the index of histo defined just before yours.
        -->> Search for the comment "FILL HERE YOUR NEW HISTO" in  btagNtupReader.C to help you.


example of code : inside "j" loop (loop through the colors)
...
   // last histo in the definition part
        \e[00;33m AddHisto(HistoBtag, "npv3_nsv0_3tr",              j,"# of secondary vertices (>=3tr) (#PV:>7)",              5,0,5); //150\e[00m 
   // my new histo 
	\e[00;33m AddHisto(HistoBtag, "new_histo", j,"Test Caro",50,0.,1.);\e[00m 
....
   // filling part for last histo :
        \e[00;33mHistoBtag[150+j*nhisto_to_clone]->Fill(nnsv3tr, weight);\e[00m 
   // my new histo
        \e[00;33mfloat caro_new_info=0.6;
        HistoBtag[151+j*nhisto_to_clone]->Fill(caro_new_info, weight); // the "150" of "nnsv3tr" becomes here a "150" for "new_histo"\e[00m 



other example of code :  outside "j" loop
...
   // last histo :
   \e[00;33mAddHisto(HistoBtag, "npu_no_scaled",  0,"numberOfPUVertices",50,-0.5,49.5);\e[00m
   // my new histo  
        \e[00;33mAddHisto(HistoBtag, "another_new_histo", 0,"Test Caro 2",50,0.,1.);\e[00m
....
   // last histo :
        \e[00;33mHistoBtag[jmax*nhisto_to_clone+10]->Fill(numberOfPUVertices,weightsave);\e[00m
   // my new histo
       // FILL HERE YOUR NEW HISTO outside "j" loop
       // example :
        \e[00;33mfloat caro_test_2=0.2;
        HistoBtag[jmax*nhisto_to_clone+11]->Fill(caro_test_2, weight);   // increase the "10" of "npu" histo into a "11"
\e[00m
------------------------------------------------------
* additional information: How to display more histos *
------------------------------------------------------
modify displayPlots.C: 

This will load "LoadDisplayRootPlot.h" which provides the template 
for the canvas, and make the plots for all the variables put in displayPlots.C
with the command :
PlotStack(TString selection, bool down=false, bool logy=false)
where selection  is the name of the histogram you want to plot,
      down determines the way of the stack histo (b,c,udgs or udsg,c,b),
      logy =1 means Logy option,
--> example :  \e[00;33mPlotStack("npv",0, 0);\e[00m
----------------------------------------------------------------------
* additional information: How to use the ntuples to do other studies *
----------------------------------------------------------------------
Make an inherit class from btagNtupReader.h, like btagNtupReader.C:
e.g. "class Plotter : public btagNtupReader{......}"
Basical functions offerred by the base \"btagNtupReader\" class are:
\e[00;33m
   virtual UInt_t   HLTPrescale(string HLTName);
   virtual Bool_t   HLTAcceptance(string HLTName);
   virtual UInt_t   HLTAcceptanceBit(string HLTName);//HLTriggerResults->at(HLTAcceptanceBit(HLTName)) is the HLT acceptance. One can use it to speed up the codes
   virtual void     PrintHLTTable();
\e[00m
';;
esac

## Installation

export SCRAM_ARCH=slc5_amd64_gcc462

scramv1 project CMSSW CMSSW_5_3_11
cd CMSSW_5_3_11/src/
## all the needed packages are listed in this file.... check if really all are downloaded
#wget -q -O - --no-check-certificate https://raw.github.com/iihe-cms-sw/VJetsTreeMaker/master/TAGS.txt | sh

## Ntuples
# local testing iin thi files you need to setup your flags...
cmsRun runDataAnalyzer_data_cfg.py PATH_TO_INPUT_ROOT_FILE   # name says it will eun on data file
cmsRun runDataAnalyzer_unfold_cfg.py PATH_TO_INPUT_ROOT_FILE # it will run on MC keeping both events if they pass OR of reco and gen cuts 
cmsRun runDataAnalyzer_mc_cfg.py PATH_TO_INPUT_ROOT_FILE     # it will run on MC file keeping if they pass RECO selections ( for background )

#### create jobs with multicrab
cd UserCode/llvv_fwk/test/
## you need to setup your grid certificates .. in the crab.cfg
multicrab -create -cfg multicrab_VJetsALL.cfg ### will create all Z/W jets josb (data, signal , abckgorund ) in directory : multicrab_data

## submit -- you can do it in batch of 500 like
multicrab -submit 1-500 -c multicrab_data  ## create a loop to run up to ...

## jobs will end up on your T2 ( the size wil be in TB)
## resubmit all fallen jobs in multicrab 
# create a script that runs the command below every hour or two ....
python resubmitCRABJObs.py multicrab_data

###  resubmit all fallen jobs in multicrab in sub job of the multicrab : Data8TeV_SingleMu2012A
python resubmitCRABJObs.py multicrab_data/Data8TeV_SingleMu2012A

# get jobs and create a repot
python runGetJobs.py multicrab_data/




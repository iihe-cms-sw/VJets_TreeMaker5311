#!/bin/bash
LumiCal=True #False
if [ -n "$1" ]; then
    JSON=`readlink -m $1`
else
#    JSON=/afs/cern.ch/user/z/zhangjin/Scripts/CMSRun2011AB_Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt
#    JSON="/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Reprocessing/Cert_160404-180252_7TeV_ReRecoNov08_Collisions11_JSON.txt"
     JSON=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/DCSOnly/json_DCSONLY.txt
fi
if [ -n "$2" ]; then
    HLTPath="--hltpath $2"
else
    HLTPath= #"HLT_Jet60_*"
fi
echo "input JSON: $JSON; $HLTPath"
JobName=${JSON#${JSON%/*}/}
JobName=${JobName%.txt}
CSVFile=/tmp/zhangjin/$JobName.csv
#LumiJSON=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/PileUp/pileup_2011_JSON_pixelLumi.txt
LumiJSON=/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/PileUp/pileup_JSON_DCSONLY_190389-190688.txt 
#LumiJSON=/tmp/zhangjin/pileup_$JobName.txt
OutputFile=`pwd`/$JobName.root
SCRAM_ARCH=slc5_amd64_gcc462
cd /afs/cern.ch/user/z/zhangjin/CMSSW_5_2_3_patch4/src/
eval `scramv1 runtime -sh`
cvs co RecoLuminosity/LumiDB/
cd RecoLuminosity/LumiDB/
scramv1 b
cd scripts/
if [ "$LumiCal" == "True" ]; then
    echo ">>>>>>>>Calculating Lumi<<<<<<"
#    ./pixelLumiCalc.py recorded -i $JSON $HLTPath
    ./lumiCalc2.py -i $JSON --norm pp7TeV $HLTPath recorded #| tee /afs/cern.ch/user/z/zhangjin/Run2011_HLT_Jet60_Lumi.txt#without pixel lumicorrection
else
    echo ">>>>>>>>Estimating pileup<<<<<<"
    if [ -n "$HLTPath" ]; then
	./pixelLumiCalc.py lumibyls -i $JSON -o $CSVFile $HLTPath #WITH Pixel Luminosity Corrections
	./pileupReCalc_HLTpaths.py -i $CSVFile --inputLumiJSON $LumiJSON -o $OutputFile
	rm $CSVFile
    else
#Note: 7TeV: minBiasXsec=73500; 8TeV minBiasXsec=72700(pythia6) 72850(pythia8)
#        ./lumiCalc2.py lumibylsXing --xingMinLum 0.1 -b stable -i $JSON -o $CSVFile #Without Pixel Luminosity Corrections
#        ./estimatePileup_makeJSON.py --csvInput $CSVFile $LumiJSON
#        ./estimatePileup2.py --minBiasXsec=73500 --maxPileupBin=50 --csvInput $CSVFile $OutputFile #old way
	./pileupCalc.py -i $JSON --inputLumiJSON $LumiJSON --calcMode true --minBiasXsec 72700 --maxPileupBin 60 --numPileupBins 60 $OutputFile
    fi
fi

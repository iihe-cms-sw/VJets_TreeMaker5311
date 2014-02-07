#!/bin/sh

### Loop over normalizations (area and lumi)
for norm in `echo "area" "lumi"`
do

### Loop over categories
#for category in `echo "loosePF" "standardPF" "looseCalo" "standardCalo"`
for category in `echo "standardPF" "standardCalo"`
do

### Loop over bins
#for bin in `echo "GLOBAL" "PT_10-20" "PT_20-40" "PT_40-99999" "ETA_0-1v5_PT_10-20" "ETA_1v5-2v5_PT_10-20" "ETA_0-1v5_PT_20-40" "ETA_1v5-2v5_PT_20-40" "ETA_0-1v5_PT_40-99999" "ETA_1v5-2v5_PT_40-99999"`
for bin in `echo "GLOBAL"`
do

### 3D plots
./TrackQualplot.py -r ipip_3D ${category}  ${bin} '3D IP value' ${norm}
./TrackQualplot.py -r ipipe_3D ${category}  ${bin} '3D IP error' ${norm}
./TrackQualplot.py -r ipips_3D ${category}  ${bin} '3D IP significance' ${norm} 
./TrackQualplot.py -r ipprob_3D ${category}  ${bin} '3D IP probability' ${norm}

./TrackQualplot.py -r ipip1_3D ${category}  ${bin} '1^{st} track 3D IP value' ${norm}
./TrackQualplot.py -r ipipe1_3D ${category}  ${bin} '1^{st} track 3D IP error' ${norm}
./TrackQualplot.py -r ipips1_3D ${category}  ${bin} '1^{st} track 3D IP significance' ${norm}
./TrackQualplot.py -r ipprob1_3D ${category}  ${bin} '1^{st} track 3D IP probability' ${norm}

./TrackQualplot.py -r ipip2_3D ${category}  ${bin} '2^{nd} track 3D IP value' ${norm}
./TrackQualplot.py -r ipipe2_3D ${category}  ${bin} '2^{nd} track 3D IP error' ${norm}
./TrackQualplot.py -r ipips2_3D ${category}  ${bin} '2^{nd} track 3D IP significance' ${norm}
./TrackQualplot.py -r ipprob2_3D ${category}  ${bin} '2^{nd} track 3D IP probability' ${norm}

### Track pT
./TrackQualplot.py -r iptkPt_3D  ${category}  ${bin} 'Track p_{T} 3D [GeV/c]'        ${norm}
./TrackQualplot.py -r iptkPt1_3D ${category}  ${bin} '1^{st} Track p_{T} 3D [GeV/c]' ${norm}
./TrackQualplot.py -r iptkPt2_3D ${category}  ${bin} '2^{nd} Track p_{T} 3D [GeV/c]' ${norm}
./TrackQualplot.py -r iptkPt3_3D ${category}  ${bin} '3^{rd} Track p_{T} 3D [GeV/c]' ${norm}
./TrackQualplot.py -r iptkPt4_3D ${category}  ${bin} '4^{th} Track p_{T} 3D [GeV/c]' ${norm}

### done with Loop over bins
done

### done with Loop over categories
done

### done with Loop over normalizations
done
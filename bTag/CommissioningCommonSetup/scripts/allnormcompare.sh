#!/bin/sh

### Loop over normalizations (area and lumi)
for norm in `echo "area" "lumi"`
do

### Loop over categories
#for category in `echo "loosePF" "standardPF" "looseCalo" "standardCalo"`
for category in `echo "standardPF" "standardCalo"`
do

### Loop over bins
for bin in `echo "GLOBAL" "PT_10-20" "PT_20-40" "PT_40-99999" "ETA_0-1v5_PT_10-20" "ETA_1v5-2v5_PT_10-20" "ETA_0-1v5_PT_20-40" "ETA_1v5-2v5_PT_20-40" "ETA_0-1v5_PT_40-99999" "ETA_1v5-2v5_PT_40-99999"`
do

###############################################################
################# Jet pT, eta, phi plots
#### from DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexMVAPFBJetTags
###############################################################
./NormoverlayMCData.py -r ajetPt  ${category}  ${bin} 'Jet p_{T} [GeV/c]' ${norm} 0 200
./NormoverlayMCData.py -r ajetEta ${category}  ${bin} 'Jet #eta' ${norm}
./NormoverlayMCData.py -r ajetPhi ${category}  ${bin} 'Jet #phi' ${norm}

###############################################################
################# Secondary Vertex TagInfo
###############################################################
./NormoverlayMCData.py -r svjetNSecondaryVertices ${category}  ${bin} 'No. of secondary vertices per jet' ${norm} 0.5 4.5

./NormoverlayMCData.py -r  svtrackMomentum ${category}  ${bin} 'track p [GeV/c]' ${norm} 0 100
./NormoverlayMCData.py -r  svtrackDeltaR ${category}  ${bin} '#DeltaR(track, jet axis)' ${norm} -0.1 0.7

./NormoverlayMCData.py -r svtrackPtRel ${category}  ${bin} 'track p_{T}^{rel} [GeV/c]' ${norm} 0 10
./NormoverlayMCData.py -r svtrackEtaRel ${category}  ${bin} 'track #eta^{rel}' ${norm} 0 10

./NormoverlayMCData.py -r svtrackPPar ${category}  ${bin} 'track parallel momentum [GeV/c]' ${norm} 0 250
./NormoverlayMCData.py -r -l svtrackPParRatio ${category}  ${bin} 'track parallel momentum ratio' ${norm} 0 10
./NormoverlayMCData.py -r svtrackJetDist ${category}  ${bin} 'minimum track approach distance to jet axis all jets' ${norm} -10 0
./NormoverlayMCData.py -r svtrackSumJetDeltaR ${category}  ${bin} '#DeltaR(track sum, jet axis)' ${norm} -0.1 0.5

./NormoverlayMCData.py -r svtrackSip2dSig ${category}  ${bin} 'signed transverse IP significance' ${norm} -50 50
./NormoverlayMCData.py -r svtrackSip2dVal ${category}  ${bin} 'signed transverse IP [cm]' ${norm} -0.3 0.3

./NormoverlayMCData.py -r svtrackSip3dSig ${category}  ${bin} 'signed 3D IP significance' ${norm} -80 80
./NormoverlayMCData.py -r svtrackSip3dVal ${category}  ${bin} 'signed 3D IP [cm]' ${norm} -3.1 3.1

./NormoverlayMCData.py -r -l svtrackSip3dSigAboveCharm ${category}  ${bin} 'first track 3D signed IP above charm' ${norm} -50 50 

./NormoverlayMCData.py -r svflightDistance2dVal_CAT1 ${category}  ${bin} 'transverse flight distance [cm]' ${norm} 0 2.5
./NormoverlayMCData.py -r svflightDistance2dSig_CAT1 ${category}  ${bin} 'transverse flight distance significance' ${norm} 0 80

./NormoverlayMCData.py -r svflightDistance3dVal_CAT1 ${category}  ${bin} '3D flight distance [cm]' ${norm} 0 15
./NormoverlayMCData.py -r svflightDistance3dSig_CAT1 ${category}  ${bin} '3D flight distance significance' ${norm} 0 80

./NormoverlayMCData.py -r svvertexNTracks_CAT1 ${category}  ${bin} 'no. of tracks at SV' ${norm} 0.5 11.5
./NormoverlayMCData.py -r svvertexMass_CAT1 ${category}  ${bin} 'vertex mass [GeV/c^{2}]' ${norm} 0 10.0
./NormoverlayMCData.py -r -l svvertexEnergyRatio_CAT1 ${category}  ${bin} 'fraction of charged jet energy at SV' ${norm} 0 1
./NormoverlayMCData.py -r svvertexJetDeltaR_CAT1 ${category}  ${bin} '#DeltaR(sv, jet axis)' ${norm} -0.1 0.7

##################################################################
################# Secondary Vertex TagInfo: V0 filter turned off
##################################################################
./NormoverlayMCData.py -r sv0jetNSecondaryVertices ${category}  ${bin} 'No. of secondary vertices per jet' ${norm} 0.5 4.5

./NormoverlayMCData.py -r  sv0trackMomentum ${category}  ${bin} 'track p [GeV/c]' ${norm} 0 100
./NormoverlayMCData.py -r  sv0trackDeltaR ${category}  ${bin} '#DeltaR(track, jet axis)' ${norm} -0.1 0.7

./NormoverlayMCData.py -r sv0trackPtRel ${category}  ${bin} 'track p_{T}^{rel} [GeV/c]' ${norm} 0 10
./NormoverlayMCData.py -r sv0trackEtaRel ${category}  ${bin} 'track #eta^{rel}' ${norm} 0 10

./NormoverlayMCData.py -r sv0trackPPar ${category}  ${bin} 'track parallel momentum [GeV/c]' ${norm} 0 250
./NormoverlayMCData.py -r -l sv0trackPParRatio ${category}  ${bin} 'track parallel momentum ratio' ${norm} 0 10
./NormoverlayMCData.py -r sv0trackJetDist ${category}  ${bin} 'minimum track approach distance to jet axis all jets' ${norm} -10 0
./NormoverlayMCData.py -r sv0trackSumJetDeltaR ${category}  ${bin} '#DeltaR(track sum, jet axis)' ${norm} -0.1 0.5

./NormoverlayMCData.py -r sv0trackSip2dSig ${category}  ${bin} 'signed transverse IP significance' ${norm} -50 50
./NormoverlayMCData.py -r sv0trackSip2dVal ${category}  ${bin} 'signed transverse IP [cm]' ${norm} -0.3 0.3

./NormoverlayMCData.py -r sv0trackSip3dSig ${category}  ${bin} 'signed 3D IP significance' ${norm} -80 80
./NormoverlayMCData.py -r sv0trackSip3dVal ${category}  ${bin} 'signed 3D IP [cm]' ${norm} -3.1 3.1

./NormoverlayMCData.py -r -l sv0trackSip3dSigAboveCharm ${category}  ${bin} 'first track 3D signed IP above charm' ${norm} -50 50 

./NormoverlayMCData.py -r sv0flightDistance2dVal_CAT1 ${category}  ${bin} 'transverse flight distance [cm]' ${norm} 0 2.5
./NormoverlayMCData.py -r sv0flightDistance2dSig_CAT1 ${category}  ${bin} 'transverse flight distance significance' ${norm} 0 80

./NormoverlayMCData.py -r sv0flightDistance3dVal_CAT1 ${category}  ${bin} '3D flight distance [cm]' ${norm} 0 15
./NormoverlayMCData.py -r sv0flightDistance3dSig_CAT1 ${category}  ${bin} '3D flight distance significance' ${norm} 0 80

./NormoverlayMCData.py -r sv0vertexNTracks_CAT1 ${category}  ${bin} 'no. of tracks at SV' ${norm} 0.5 11.5
./NormoverlayMCData.py -r sv0vertexMass_CAT1 ${category}  ${bin} 'vertex mass [GeV/c^{2}]' ${norm} 0 10.0
./NormoverlayMCData.py -r -l sv0vertexEnergyRatio_CAT1 ${category}  ${bin} 'fraction of charged jet energy at SV' ${norm} 0 1
./NormoverlayMCData.py -r sv0vertexJetDeltaR_CAT1 ${category}  ${bin} '#DeltaR(sv, jet axis)' ${norm} -0.1 0.7

###############################################################
################# Impact Parameter TagInfo
###############################################################
### 2dimensional plots
./NormoverlayMCData.py -r iptrackMultVsJetPt ${category}  ${bin} 'Track Multiplicity vs Jet Pt for all Tracks' ${norm}
./NormoverlayMCData.py -r ipselectedTrackMultVsJetPt ${category}  ${bin} 'Track Multiplicity vs Jet Pt for Selected Tracks' ${norm}

### Track quality plots
./NormoverlayMCData.py -r iptrackQual         ${category}  ${bin} 'Track Quality for all Tracks' ${norm}
./NormoverlayMCData.py -r ipselectedTrackQual ${category}  ${bin} 'Track Quality for Selected Tracks' ${norm}

### 2D plots
./NormoverlayMCData.py -r ipdecLen_2D ${category}  ${bin} '2D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen1_2D ${category}  ${bin} '1^{st} track 2D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen2_2D ${category}  ${bin} '2^{nd} track 2D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen3_2D ${category}  ${bin} '3^{rd} track 2D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen4_2D ${category}  ${bin} '4^{th} track 2D Decay length' ${norm} -0.5 5

./NormoverlayMCData.py -r -l ipjetDist_2D ${category}  ${bin} '2D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist1_2D ${category}  ${bin} ' 1^{st} track 2D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist2_2D ${category}  ${bin} ' 2^{nd} track 2D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist3_2D ${category}  ${bin} ' 3^{rd} track 2D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist4_2D ${category}  ${bin} ' 4^{th} track 2D Jet distance' ${norm} -0.15 0.01

./NormoverlayMCData.py -r ipselTrksNbr_2D ${category}  ${bin} 'no. of selected tracks for 2D in jet' ${norm} -0.5 19.5

### 3D plots
./NormoverlayMCData.py -r ipdecLen_3D ${category}  ${bin} '3D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen1_3D ${category}  ${bin} '1^{st} track 3D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen2_3D ${category}  ${bin} '2^{nd} track 3D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen3_3D ${category}  ${bin} '3^{rd} track 3D Decay length' ${norm} -0.5 5
./NormoverlayMCData.py -r ipdecLen4_3D ${category}  ${bin} '4^{th} track 3D Decay length' ${norm} -0.5 5

./NormoverlayMCData.py -r -l ipjetDist_3D  ${category}  ${bin} '3D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist1_3D ${category}  ${bin} ' 1^{st} track 3D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist2_3D ${category}  ${bin} ' 2^{nd} track 3D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist3_3D ${category}  ${bin} ' 3^{rd} track 3D Jet distance' ${norm} -0.15 0.01
./NormoverlayMCData.py -r -l ipjetDist4_3D ${category}  ${bin} ' 4^{th} track 3D Jet distance' ${norm} -0.15 0.01

./NormoverlayMCData.py -r ipselTrksNbr_3D ${category}  ${bin} 'no. of selected tracks for 3D in jet' ${norm} -0.5 19.5

### 2D plots
./NormoverlayMCData.py -r ipip_2D ${category}  ${bin} '2D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe_2D ${category}  ${bin} '2D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips_2D ${category}  ${bin} '2D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob_2D ${category}  ${bin} '2D IP probability' ${norm} -1.1 1.1

./NormoverlayMCData.py -r ipip1_2D ${category}  ${bin} '1^{st} track 2D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe1_2D ${category}  ${bin} '1^{st} track 2D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips1_2D ${category}  ${bin} '1^{st} track 2D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob1_2D ${category}  ${bin} '1^{st} track 2D IP probability' ${norm} -1.1 1.1

./NormoverlayMCData.py -r ipip2_2D ${category}  ${bin} '2^{nd} track 2D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe2_2D ${category}  ${bin} '2^{nd} track 2D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips2_2D ${category}  ${bin} '2^{nd} track 2D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob2_2D ${category}  ${bin} '2^{nd} track 2D IP probability' ${norm} -1.1 1.1

./NormoverlayMCData.py -r ipip3_2D ${category}  ${bin} '3^{rd} track 2D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe3_2D ${category}  ${bin} '3^{rd} track 2D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips3_2D ${category}  ${bin} '3^{rd} track 2D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob3_2D ${category}  ${bin} '3^{rd} track 2D IP probability' ${norm} -1.1 1.1

### 3D plots
./NormoverlayMCData.py -r ipip_3D ${category}  ${bin} '3D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe_3D ${category}  ${bin} '3D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips_3D ${category}  ${bin} '3D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob_3D ${category}  ${bin} '3D IP probability' ${norm} -1.1 1.1

./NormoverlayMCData.py -r ipip1_3D ${category}  ${bin} '1^{st} track 3D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe1_3D ${category}  ${bin} '1^{st} track 3D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips1_3D ${category}  ${bin} '1^{st} track 3D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob1_3D ${category}  ${bin} '1^{st} track 3D IP probability' ${norm} -1.1 1.1

./NormoverlayMCData.py -r ipip2_3D ${category}  ${bin} '2^{nd} track 3D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe2_3D ${category}  ${bin} '2^{nd} track 3D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips2_3D ${category}  ${bin} '2^{nd} track 3D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob2_3D ${category}  ${bin} '2^{nd} track 3D IP probability' ${norm} -1.1 1.1

./NormoverlayMCData.py -r ipip3_3D ${category}  ${bin} '3^{rd} track 3D IP value' ${norm} -0.2 0.2
./NormoverlayMCData.py -r ipipe3_3D ${category}  ${bin} '3^{rd} track 3D IP error' ${norm} 0 0.05
./NormoverlayMCData.py -r ipips3_3D ${category}  ${bin} '3^{rd} track 3D IP significance' ${norm} -40 40
./NormoverlayMCData.py -r ipprob3_3D ${category}  ${bin} '3^{rd} track 3D IP probability' ${norm} -1.1 1.1

### Track pT
./NormoverlayMCData.py -r iptkPt_2D ${category}  ${bin}  'Track p_{T} 2D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt1_2D ${category}  ${bin} '1^{st} Track p_{T} 2D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt2_2D ${category}  ${bin} '2^{nd} Track p_{T} 2D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt3_2D ${category}  ${bin} '3^{rd} Track p_{T} 2D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt4_2D ${category}  ${bin} '4^{th} Track p_{T} 2D [GeV/c]' ${norm}

./NormoverlayMCData.py -r iptkPt_3D ${category}  ${bin}  'Track p_{T} 3D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt1_3D ${category}  ${bin} '1^{st} Track p_{T} 3D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt2_3D ${category}  ${bin} '2^{nd} Track p_{T} 3D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt3_3D ${category}  ${bin} '3^{rd} Track p_{T} 3D [GeV/c]' ${norm}
./NormoverlayMCData.py -r iptkPt4_3D ${category}  ${bin} '4^{th} Track p_{T} 3D [GeV/c]' ${norm}

### Track N hits
./NormoverlayMCData.py -r iptkNHits_2D ${category}  ${bin}   'Track NHits 2D' ${norm}
./NormoverlayMCData.py -r iptkNHits1_2D ${category}  ${bin}  '1^{st} Track NHits 2D ' ${norm}
./NormoverlayMCData.py -r iptkNHits2_2D ${category}  ${bin}  '2^{nd} Track NHits 2D' ${norm}
./NormoverlayMCData.py -r iptkNHits3_2D ${category}  ${bin}  '3^{rd} Track NHits 2D' ${norm}
./NormoverlayMCData.py -r iptkNHits4_2D ${category}  ${bin}  '4^{th} Track NHits 2D' ${norm}

./NormoverlayMCData.py -r iptkNHits_3D ${category}  ${bin}   'Track NHits 3D' ${norm}
./NormoverlayMCData.py -r iptkNHits1_3D ${category}  ${bin}  '1^{st} Track NHits 3D' ${norm}
./NormoverlayMCData.py -r iptkNHits2_3D ${category}  ${bin}  '2^{nd} Track NHits 3D' ${norm}
./NormoverlayMCData.py -r iptkNHits3_3D ${category}  ${bin}  '3^{rd} Track NHits 3D' ${norm}
./NormoverlayMCData.py -r iptkNHits4_3D ${category}  ${bin}  '4^{th} Track NHits 3D' ${norm}

### Track Nomalized chi^2
./NormoverlayMCData.py -r iptkNChiSqr_2D ${category}  ${bin}   'Track Normalized #chi^2 2D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr1_2D ${category}  ${bin}  '1^{st} Track Normalized #chi^2 2D ' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr2_2D ${category}  ${bin}  '2^{nd} Track Normalized #chi^2 2D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr3_2D ${category}  ${bin}  '3^{rd} Track Normalized #chi^2 2D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr4_2D ${category}  ${bin}  '4^{th} Track Normalized #chi^2 2D' ${norm}

./NormoverlayMCData.py -r iptkNChiSqr_3D ${category}  ${bin}   'Track Normalized #chi^2 3D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr1_3D ${category}  ${bin}  '1^{st} Track Normalized #chi^2 3D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr2_3D ${category}  ${bin}  '2^{nd} Track Normalized #chi^2 3D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr3_3D ${category}  ${bin}  '3^{rd} Track Normalized #chi^2 3D' ${norm}
./NormoverlayMCData.py -r iptkNChiSqr4_3D ${category}  ${bin}  '4^{th} Track Normalized #chi^2 3D' ${norm}

###############################################################
################# Electron TagInfo
###############################################################
 ./NormoverlayMCData.py -r 'el1st lepton delta R' ${category}  ${bin} '#DeltaR(electron, jet axis)' ${norm} 0 0.5
 ./NormoverlayMCData.py -r 'el1st lepton energy ratio' ${category}  ${bin}   'Electron p_{T}/Energy^{jet}' ${norm} 0 1.4
 ./NormoverlayMCData.py -r 'el1st lepton eta rel'    ${category}  ${bin}   'Electron #eta^{rel}' ${norm} -1 10
 ./NormoverlayMCData.py -r 'el1st lepton p0 par' ${category}  ${bin} 'Electron p_{T}^{rel} in the B rest frame' ${norm} 0 3
 ./NormoverlayMCData.py -r 'el1st lepton pT rel' ${category}  ${bin} 'Electron p_{T}^{rel} [GeV/C]' ${norm}  0 3
 ./NormoverlayMCData.py -r 'el1st lepton pT'    ${category}  ${bin} 'Electron p_{T} [GeV/C]'  ${norm} 0 20
 ./NormoverlayMCData.py -r 'el1st lepton parallel energy ratio'   ${category}  ${bin} 'Electron Ppar/Energy^{jet}' ${norm} 0 1.4
 ./NormoverlayMCData.py -r 'el1st lepton sip2d'	${category}  ${bin} 'Electron signed 2D impact parameter significance' ${norm} -20 30
 ./NormoverlayMCData.py -r 'el1st lepton sip3d'	${category}  ${bin} 'Electron signed 3D impact parameter significance' ${norm} -20 30

###############################################################
################# Muon TagInfo
###############################################################
 ./NormoverlayMCData.py -r 'mu1st lepton delta R'	${category}  ${bin} '#DeltaR(muon, jet axis)' ${norm} 0 0.5
 ./NormoverlayMCData.py -r 'mu1st lepton energy ratio'	${category}  ${bin} 'Muon p_{T}/Energy^{jet}'  ${norm} 0 1.2
 ./NormoverlayMCData.py -r 'mu1st lepton eta rel'	${category}  ${bin} 'Muon #eta^{rel}'   ${norm} -1 10
 ./NormoverlayMCData.py -r 'mu1st lepton p0 par'	${category}  ${bin} 'Muon p_{T}^{rel} in the B rest frame'  ${norm} 0 3
 ./NormoverlayMCData.py -r 'mu1st lepton pT rel'	${category}  ${bin} 'Muon p_{T}^{rel} [GeV/C]'  ${norm} 0 3
 ./NormoverlayMCData.py -r 'mu1st lepton pT'	${category}  ${bin} 'Muon p_{T} [GeV/C]'  ${norm} 0 20
 ./NormoverlayMCData.py -r 'mu1st lepton parallel energy ratio'   ${category}  ${bin} 'Muon Ppar/Energy^{jet}'  ${norm} 0 1.4 
 ./NormoverlayMCData.py -r 'mu1st lepton sip2d'	${category}  ${bin} 'Muon signed 2D impact parameter significance'  ${norm} -20 30
 ./NormoverlayMCData.py -r 'mu1st lepton sip3d'	${category}  ${bin} 'Muon signed 3D impact parameter significance'  ${norm} -20 30

### done with Loop over bins
done

### done with Loop over categories
done

### done with Loop over normalizations
done

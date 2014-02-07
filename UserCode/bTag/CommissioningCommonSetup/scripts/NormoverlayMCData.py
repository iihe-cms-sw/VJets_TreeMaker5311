#!/usr/bin/env python
import sys, os, math, array, ROOT

import CommonSetup

def main(args, left, blind):

	mc0 = ROOT.TFile.Open("MC_MinBias_TuneD6T.root")
	mc1 = ROOT.TFile.Open("MC_QCD_Pt_0_15.root")
	mc2 = ROOT.TFile.Open("MC_QCD_Pt_15_20.root")
	mc3 = ROOT.TFile.Open("MC_QCD_Pt_20_30.root")
	mc4 = ROOT.TFile.Open("MC_QCD_Pt_30.root")

	data = ROOT.TFile.Open("data.root")

	histo = args[0]
        ##### IP Tag Infos
	if histo[0] == 'i' and histo[1] == 'p':
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseImpactParameterPFTagInfos_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_looseImpactParameterPFTagInfos_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardImpactParameterPFTagInfos_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_standardImpactParameterPFTagInfos_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseImpactParameterCaloTagInfos_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_looseImpactParameterCaloTagInfos_"+args[2]
		else:
			histo = histo[2:] + "_standardImpactParameterCaloTagInfos_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_standardImpactParameterCaloTagInfos_"+args[2]
        ##### SV Tag Infos...with V0 filter turned off
	elif histo[0] == 's' and histo[1] == 'v' and histo[2] == '0':
		if args[1] == 'loosePF':
			histo = histo[3:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_looseCombinedSecondaryVertexV0PF_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[3:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_standardCombinedSecondaryVertexV0PF_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[3:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_looseCombinedSecondaryVertexV0Calo_"+args[2]
		else:
			histo = histo[3:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_standardCombinedSecondaryVertexV0Calo_"+args[2]
        ##### SV Tag Infos
	elif histo[0] == 's' and histo[1] == 'v':
		if args[1] == 'loosePF':
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_looseCombinedSecondaryVertexPF_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_standardCombinedSecondaryVertexPF_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_looseCombinedSecondaryVertexCalo_"+args[2]
		else:
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/TaggingVariable_standardCombinedSecondaryVertexCalo_"+args[2]
        ##### Soft Muon Tag Infos
	elif histo[0] == 'm' and histo[1] == 'u':
		if args[1] == 'loosePF':
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_looseSoftMuonPFTagInfos_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_standardSoftMuonPFTagInfos_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_looseSoftMuonCaloTagInfos_"+args[2]
		else:
			histo = histo[2:]
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_standardSoftMuonCaloTagInfos_"+args[2]
        ##### Soft Electron Tag Infos
	elif histo[0] == 'e' and histo[1] == 'l':
		if args[1] == 'loosePF':
			histo = histo[2:] 
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_looseSoftElectronPFTagInfos_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] 
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_standardSoftElectronPFTagInfos_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] 
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_looseSoftElectronCaloTagInfos_"+args[2]
		else:
			histo = histo[2:] 
			pfx = "DQMData/Run 1/Btag/Run summary/SoftLepton_standardSoftElectronCaloTagInfos_"+args[2]
	###################### 	jet momentum, pT, eta, phi plots
	elif histo[0] == 'a' :			
		if args[1] == 'loosePF':
			histo = histo[1:] + "_looseCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[1:] + "_standardCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[1:] + "_looseCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
		else:
			histo = histo[1:] + "_standardCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
	else:
		pfx = "!!Nothing to draw!!"
		print "Nothing to draw"

        #####################			
	print pfx, histo

        #MinBias
	mc0 = [
		mc0.Get("%s/%sNI" % (pfx, histo)),
		mc0.Get("%s/%sDUSG" % (pfx, histo)),
		mc0.Get("%s/%sC" % (pfx, histo)),
		mc0.Get("%s/%sB" % (pfx, histo)),
		mc0.Get("%s/%sALL" % (pfx, histo))
		]
        #pT 0-15
	mc1 = [
		mc1.Get("%s/%sNI" % (pfx, histo)),
		mc1.Get("%s/%sDUSG" % (pfx, histo)),
		mc1.Get("%s/%sC" % (pfx, histo)),
		mc1.Get("%s/%sB" % (pfx, histo)),
		mc1.Get("%s/%sALL" % (pfx, histo))
		]
	#pT 15-20
	mc2 = [
		mc2.Get("%s/%sNI" % (pfx, histo)),
		mc2.Get("%s/%sDUSG" % (pfx, histo)),
		mc2.Get("%s/%sC" % (pfx, histo)),
		mc2.Get("%s/%sB" % (pfx, histo)),
		mc2.Get("%s/%sALL" % (pfx, histo))
		]

	#pT 20-30
	mc3 = [
		mc3.Get("%s/%sNI" % (pfx, histo)),
		mc3.Get("%s/%sDUSG" % (pfx, histo)),
		mc3.Get("%s/%sC" % (pfx, histo)),
		mc3.Get("%s/%sB" % (pfx, histo)),
		mc3.Get("%s/%sALL" % (pfx, histo))
		]

	#pT 30
	mc4 = [
		mc4.Get("%s/%sNI" % (pfx, histo)),
		mc4.Get("%s/%sDUSG" % (pfx, histo)),
		mc4.Get("%s/%sC" % (pfx, histo)),
		mc4.Get("%s/%sB" % (pfx, histo)),
		mc4.Get("%s/%sALL" % (pfx, histo))
		]	

###################################################
#### Scale MC to data lumi ...Data/MC ratio
####	
#Data total luminosity 12.44 nb-1

#/MinBias_TuneD6T_7TeV-pythia6/Spring10-START3X_V26B-v2/GEN-SIM-RECO       0.773 nb-1
#/QCDDiJet_Pt0to15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO                0.0412 nb-1
#                                              So we have 0.0412 + 0.773 = 0.814 nb-1
#The scale factor for pt0to15 and MinBias would then be                    12.44/0.814 = 15.28

#/QCDDiJet_Pt15to20/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO               3.45 nb-1
#/QCDDiJet_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO               4.23 nb-1
#/QCD_Pt30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO                        50.3 nb-1


# MC weights = 15.28, 15.28, 3.61, 2.94, 0.25

	scale = [ 15.28, 15.28, 3.61, 2.94, 0.25 ]
		
##################

## Adding the histos:
## scale[0]*mc0 + scale[1]*mc1 + scale[2]*mc2 + scale[3]*mc3 + scale[4]*mc4 
	for i in range(5):
		mc3[i].Add( mc3[i], mc4[i], float(scale[3]), float(scale[4]) )		
		mc2[i].Add( mc2[i], mc3[i], float(scale[2]), 1.0 )
		mc1[i].Add( mc1[i], mc2[i], float(scale[1]), 1.0 )
		mc0[i].Add( mc0[i], mc1[i], float(scale[0]), 1.0 )

	mc = mc0

	mc.append(data.Get("%s/%sALL" % (pfx, histo)))

##################
#### Now scale MC to the data area
#### The MC histograms are normalized to the area of the data histograms
	if args[4] == 'area':
		print 'Normalizing by Area!!!'
		for i in range(5):
			mc[i].Scale(mc[-1].Integral()/mc[4].Integral())
	elif args[4] == 'lumi':
		print 'Normalizing by Luminosity!!!'
	else:
		print 'No normalization specified!!!'
##################
	## Set Y-axis title
	if args[4] == 'area':
		yTitle = 'MC Normalized to Data'
	elif args[4] == 'lumi':
		yTitle = 'Normalized to luminosity'
	else:
		yTitle = 'Arbitrary units'

	if len(args) > 5:
		mc[4].GetXaxis().SetRangeUser(float(args[5]), float(args[6]))
		mc[-1].GetXaxis().SetRangeUser(float(args[5]), float(args[6]))

	for i, j in enumerate(mc):
		j.SetTitle("")
	
	newTitle = args[0]+"_"+args[1]+"_"+args[2]

	csetup = CommonSetup.CommonSetup()
	csetup.bDowndraw(mc[:-1], mc[-1], args[3], yTitle, newTitle, args[2], left, blind, args[4])
        ###### bDowndraw(mc,       data,  xTit,    yTit,   title,    bintype, left, blind, norm)

if __name__ == '__main__':
	app = ROOT.gApplication
	sh = ROOT.TSignalHandler(ROOT.kSigInterrupt, False)
	sh.Connect("Notified()", "TApplication", app, "Terminate()")
	ROOT.gSystem.AddSignalHandler(sh)
	csetup = CommonSetup.CommonSetup()
	csetup.style()
	s = 1
	run = len(sys.argv) > s and sys.argv[s] == '-r'
	if run: s += 1
	left = len(sys.argv) > s and sys.argv[s] == '-l'
	if left: s += 1
	blind = len(sys.argv) > s and sys.argv[s] == '-t'
	if blind: s += 1
	data = main(sys.argv[s:], left, blind)
	if not run:
		app.Run()

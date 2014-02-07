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
	###################### TCHP
	if histo[0] == 'T' and histo[1] == 'P' :
		if args[1] == 'loosePF':		
			histo = histo[2:] + "_looseTrackCountingHighPurPFBJetTags_" + args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseTrackCountingHighPurPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':		
			histo = histo[2:] + "_standardTrackCountingHighPurPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardTrackCountingHighPurPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':		
			histo = histo[2:] + "_looseTrackCountingHighPurCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseTrackCountingHighPurCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardTrackCountingHighPurCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardTrackCountingHighPurCaloBJetTags_"+args[2]

	######################  TCHE
	elif histo[0] == 'T' and histo[1] == 'E' :		
		if args[1] == 'loosePF':		
			histo = histo[2:] + "_looseTrackCountingHighEffPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseTrackCountingHighEffPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':		
			histo = histo[2:] + "_standardTrackCountingHighEffPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardTrackCountingHighEffPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':		
			histo = histo[2:] + "_looseTrackCountingHighEffCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseTrackCountingHighEffCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardTrackCountingHighEffCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardTrackCountingHighEffCaloBJetTags_"+args[2]

	###################### 	Jet prob
	elif histo[0] == 'J' and histo[1] == 'P' :
		if args[1] == 'loosePF':		
			histo = histo[2:] + "_looseJetProbabilityPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseJetProbabilityPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':		
			histo = histo[2:] + "_standardJetProbabilityPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardJetProbabilityPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':		
			histo = histo[2:] + "_looseJetProbabilityCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseJetProbabilityCaloBJetTags_"+args[2]
		else :
			histo = histo[2:] + "_standardJetProbabilityCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardJetProbabilityCaloBJetTags_"+args[2]		       

	###################### 	Jet B prob
	elif histo[0] == 'J' and histo[1] == 'B' :
		if args[1] == 'loosePF':		
			histo = histo[2:] + "_looseJetBProbabilityPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseJetBProbabilityPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':		
			histo = histo[2:] + "_standardJetBProbabilityPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardJetBProbabilityPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':		
			histo = histo[2:] + "_looseJetBProbabilityCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseJetBProbabilityCaloBJetTags_"+args[2]
		else :
			histo = histo[2:] + "_standardJetBProbabilityCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardJetBProbabilityCaloBJetTags_"+args[2]	       

	###################### 	SSV High Eff (>=2 tracks)
	elif histo[0] == 'S' and histo[1] == 'E' :		
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSimpleSecondaryVertexHighEffPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSimpleSecondaryVertexHighEffPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSimpleSecondaryVertexHighEffPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSimpleSecondaryVertexHighEffPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSimpleSecondaryVertexHighEffCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSimpleSecondaryVertexHighEffCaloBJetTags_"+args[2]
		else :
			histo = histo[2:] + "_standardSimpleSecondaryVertexHighEffCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSimpleSecondaryVertexHighEffCaloBJetTags_"+args[2]

	###################### 	SSV High Purity (>=3 tracks)
	elif histo[0] == 'S' and histo[1] == 'P' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSimpleSecondaryVertexHighPurPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSimpleSecondaryVertexHighPurPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSimpleSecondaryVertexHighPurPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSimpleSecondaryVertexHighPurPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSimpleSecondaryVertexHighPurCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSimpleSecondaryVertexHighPurCaloBJetTags_"+args[2]
		else :
			histo = histo[2:] + "_standardSimpleSecondaryVertexHighPurCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSimpleSecondaryVertexHighPurCaloBJetTags_"+args[2]

	###################### 	CSV
	elif histo[0] == 'C' and histo[1] == 'S' :
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseCombinedSecondaryVertexPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardCombinedSecondaryVertexPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardCombinedSecondaryVertexPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseCombinedSecondaryVertexCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardCombinedSecondaryVertexCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardCombinedSecondaryVertexCaloBJetTags_"+args[2]

	###################### 	CSV MVA
	elif histo[0] == 'C' and histo[1] == 'M' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardCombinedSecondaryVertexMVAPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardCombinedSecondaryVertexMVACaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardCombinedSecondaryVertexMVACaloBJetTags_"+args[2]

	###################### 	SoftMuonByIP3d
	elif histo[0] == 'M' and histo[1] == 'I' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSoftMuonByIP3dPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftMuonByIP3dPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSoftMuonByIP3dPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftMuonByIP3dPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSoftMuonByIP3dCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftMuonByIP3dCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardSoftMuonByIP3dCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftMuonByIP3dCaloBJetTags_"+args[2]

	###################### 	SoftMuonByPt
	elif histo[0] == 'M' and histo[1] == 'P' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSoftMuonByPtPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftMuonByPtPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSoftMuonByPtPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftMuonByPtPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSoftMuonByPtCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftMuonByPtCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardSoftMuonByPtCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftMuonByPtCaloBJetTags_"+args[2]

	###################### 	SoftMuon
	elif histo[0] == 'M' and histo[1] == 'U' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSoftMuonPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftMuonPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSoftMuonPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftMuonPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSoftMuonCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftMuonCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardSoftMuonCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftMuonCaloBJetTags_"+args[2]

	###################### 	SoftElectronByIP3d
	elif histo[0] == 'E' and histo[1] == 'I' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSoftElectronByIP3dPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftElectronByIP3dPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSoftElectronByIP3dPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftElectronByIP3dPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSoftElectronByIP3dCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftElectronByIP3dCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardSoftElectronByIP3dCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftElectronByIP3dCaloBJetTags_"+args[2]

	###################### 	SoftElectronByPt
	elif histo[0] == 'E' and histo[1] == 'P' :			
		if args[1] == 'loosePF':
			histo = histo[2:] + "_looseSoftElectronByPtPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftElectronByPtPFBJetTags_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[2:] + "_standardSoftElectronByPtPFBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftElectronByPtPFBJetTags_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[2:] + "_looseSoftElectronByPtCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_looseSoftElectronByPtCaloBJetTags_"+args[2]
		else:
			histo = histo[2:] + "_standardSoftElectronByPtCaloBJetTags_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/JetTag_standardSoftElectronByPtCaloBJetTags_"+args[2]

	###################### 
	else :
		pfx = "!!Nothing to draw!!"
		print "Nothing to draw"
		

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
	csetup.bUpdraw(mc[:-1], mc[-1], args[3], yTitle, newTitle, args[2], left, blind, args[4])
        ###### bUpdraw(mc,       data,  xTit,    yTit,   title,    bintype, left, blind, norm)


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

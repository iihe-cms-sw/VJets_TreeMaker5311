#!/usr/bin/env python
import sys, os, math, array, ROOT

import CommonSetup


def main(args, left):

	mc0 = ROOT.TFile.Open("MC_QCD_Pt_0_15.root")
	mc1 = ROOT.TFile.Open("MC_QCD_Pt_15_20.root")
	mc2 = ROOT.TFile.Open("MC_QCD_Pt_20_30.root")
	mc3 = ROOT.TFile.Open("MC_QCD_Pt_30.root")

	data = ROOT.TFile.Open("data.root")

	histo = args[0]
        ##### profile IP Tag Infos
	if histo[0] == 'p' and histo[1] == 'r' and histo[2] == 'o':
		if args[1] == 'loosePF':
			histo = histo[3:] + "_looseImpactParameterPFTagInfos_" + args[2] + "_Profile_"
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_looseImpactParameterPFTagInfos_"+args[2]
		elif args[1] == 'standardPF':
			histo = histo[3:] + "_standardImpactParameterPFTagInfos_"+args[2] + "_Profile_"
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_standardImpactParameterPFTagInfos_"+args[2]
		elif args[1] == 'looseCalo':
			histo = histo[3:] + "_looseImpactParameterCaloTagInfos_"+args[2] + "_Profile_"
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_looseImpactParameterCaloTagInfos_"+args[2]
		else:
			histo = histo[3:] + "_standardImpactParameterCaloTagInfos_"+args[2] + "_Profile_"
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_standardImpactParameterCaloTagInfos_"+args[2]

	else:
		pfx = "!!Nothing to draw!!"
		print "Nothing to draw"

        #####################			
	print pfx, histo

        #pT 0-15
	mc0 = [
		mc0.Get("%s/%sNI" % (pfx, histo)),
		mc0.Get("%s/%sDUSG" % (pfx, histo)),
		mc0.Get("%s/%sC" % (pfx, histo)),
		mc0.Get("%s/%sB" % (pfx, histo)),
		mc0.Get("%s/%sALL" % (pfx, histo))
		]
        #pT 15-20
	mc1 = [
		mc1.Get("%s/%sNI" % (pfx, histo)),
		mc1.Get("%s/%sDUSG" % (pfx, histo)),
		mc1.Get("%s/%sC" % (pfx, histo)),
		mc1.Get("%s/%sB" % (pfx, histo)),
		mc1.Get("%s/%sALL" % (pfx, histo))
		]
	#pT 20-30
	mc2 = [
		mc2.Get("%s/%sNI" % (pfx, histo)),
		mc2.Get("%s/%sDUSG" % (pfx, histo)),
		mc2.Get("%s/%sC" % (pfx, histo)),
		mc2.Get("%s/%sB" % (pfx, histo)),
		mc2.Get("%s/%sALL" % (pfx, histo))
		]

	#pT 30
	mc3 = [
		mc3.Get("%s/%sNI" % (pfx, histo)),
		mc3.Get("%s/%sDUSG" % (pfx, histo)),
		mc3.Get("%s/%sC" % (pfx, histo)),
		mc3.Get("%s/%sB" % (pfx, histo)),
		mc3.Get("%s/%sALL" % (pfx, histo))
		]

#### Scale MC to data lumi ...Data/MC ratio
#Data total luminosity 4.79 nb-1
#/QCDDiJet_Pt0to15/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO 0.0403 nb-1
#/QCDDiJet_Pt15to20/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO 3.21 nb-1
#/QCDDiJet_Pt20to30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO 3.77 nb-1
#/QCD_Pt30/Spring10-START3X_V26_S09-v1/GEN-SIM-RECO 21.26 nb-1

# MC weights = 118.86, 1.49, 1.27, 0.23

	scale = [ 118.86, 1.49, 1.27, 0.23 ]
    
##################

## Adding the histos:
## scale[0]*mc0 + scale[1]*mc1 + scale[2]*mc2 + scale[3]*mc3 + scale[4]*mc4 
	for i in range(5):
		mc2[i].Add( mc2[i], mc3[i], float(scale[2]), float(scale[3]) )
		mc1[i].Add( mc1[i], mc2[i], float(scale[1]), 1.0 )
		mc0[i].Add( mc0[i], mc1[i], float(scale[0]), 1.0 )

	mc = mc0

	mc.append(data.Get("%s/%sALL" % (pfx, histo)))

##################
#### Now scale MC to the data area
#### The MC histograms are normalized to the area of the data histograms
	if args[5] == 'area':
		print 'Normalizing by Area!!!'
		for i in range(5):
			mc[i].Scale(mc[-1].Integral()/mc[4].Integral())
	elif args[5] == 'lumi':
		print 'Normalizing by Luminosity!!!'
	else:
		print 'No normalization specified!!!'
##################

	if len(args) > 6:
		mc[4].GetXaxis().SetRangeUser(float(args[6]), float(args[7]))
		mc[-1].GetXaxis().SetRangeUser(float(args[6]), float(args[7]))

	for i, j in enumerate(mc):
		j.SetTitle("")
#		format(j, i)
	
	newTitle = args[0]+"_"+args[1]+"_"+args[2]

	csetup = CommonSetup.CommonSetup()
	csetup.drawProfile(mc[:-1], mc[-1], args[3], args[4], newTitle, args[2], left, args[5])
	###### drawProfile(mc,      data,   xTit,    yTit,    title,    bintype, left, norm)


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
	data = main(sys.argv[s:], left)
	if not run:
		app.Run()

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
        ##### IP Tag Infos
	if histo[0] == 'i' and histo[1] == 'p':
		if args[1] == 'standardPF':
			histo = histo[2:] + "_standardImpactParameterPFTagInfos_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_standardImpactParameterPFTagInfos_"+args[2]
		else:
			histo = histo[2:] + "_standardImpactParameterCaloTagInfos_"+args[2]
			pfx = "DQMData/Run 1/Btag/Run summary/TrackIPPlots_standardImpactParameterCaloTagInfos_"+args[2]
	else:
		pfx = "!!Nothing to draw!!"
		print "Nothing to draw"

        #####################			
	print pfx, histo


        #pT 0-15
	mc0 = [
		mc0.Get("%s/%sQualHighPur" % (pfx, histo)),
		mc0.Get("%s/%sQualTight" % (pfx, histo)),
		mc0.Get("%s/%sQualLoose" % (pfx, histo))
		]
        #pT 15-20
	mc1 = [
		mc1.Get("%s/%sQualHighPur" % (pfx, histo)),
		mc1.Get("%s/%sQualTight" % (pfx, histo)),
		mc1.Get("%s/%sQualLoose" % (pfx, histo))
		]
	#pT 20-30
	mc2 = [
		mc2.Get("%s/%sQualHighPur" % (pfx, histo)),
		mc2.Get("%s/%sQualTight" % (pfx, histo)),
		mc2.Get("%s/%sQualLoose" % (pfx, histo))
		]

	#pT 30
	mc3 = [
		mc3.Get("%s/%sQualHighPur" % (pfx, histo)),
		mc3.Get("%s/%sQualTight" % (pfx, histo)),
		mc3.Get("%s/%sQualLoose" % (pfx, histo))
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
	for i in range(3):
		mc2[i].Add( mc2[i], mc3[i], float(scale[2]), float(scale[3]) )
		mc1[i].Add( mc1[i], mc2[i], float(scale[1]), 1.0 )
		mc0[i].Add( mc0[i], mc1[i], float(scale[0]), 1.0 )

	mc = mc0

	data = [
		data.Get("%s/%sQualHighPur" % (pfx, histo)),
		data.Get("%s/%sQualTight" % (pfx, histo)),
		data.Get("%s/%sQualLoose" % (pfx, histo))
		]

##################
#### Now scale MC to the data area
#### The MC histograms are normalized to the area of the data histograms
	if args[4] == 'area':
		print 'Normalizing by Area!!!'
		for i in range(3):
			mc[i].Scale(data[i].Integral()/mc[i].Integral())
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

##################

	if len(args) > 5:
		mc[4].GetXaxis().SetRangeUser(float(args[5]), float(args[6]))
		mc[-1].GetXaxis().SetRangeUser(float(args[5]), float(args[6]))

	for i, j in enumerate(mc):
		j.SetTitle("")
	
	newTitle = args[0]+"_"+args[1]+"_"+args[2]
	
	csetup = CommonSetup.CommonSetup()
	csetup.drawQual(mc, data, args[3], yTitle, newTitle, args[2], left, args[4])
	###### drawQual(mc, data, xTit,    yTit,   title,    bintype, left, norm)


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

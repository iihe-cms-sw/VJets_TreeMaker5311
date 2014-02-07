#!/usr/bin/env python
import sys, os, math, array, ROOT

#For batch mode
ROOT.gROOT.SetBatch(True)

import Canvas

keep = []

class CommonSetup:

        def style(self):
                global tdrStyle

                ROOT.gROOT.SetStyle("Plain")
                ROOT.gROOT.ForceStyle()

                tdrStyle = ROOT.TStyle()
                tdrStyle.SetFrameBorderMode(0)
                tdrStyle.SetCanvasBorderMode(0)
                tdrStyle.SetPadBorderMode(0)
                tdrStyle.SetPadBorderMode(0)

        #	tdrStyle.SetFrameColor(0)
                tdrStyle.SetPadColor(0)
                tdrStyle.SetCanvasColor(0)
                tdrStyle.SetStatColor(0)
                tdrStyle.SetFillColor(0)

                tdrStyle.SetPaperSize(20,26)
        #	tdrStyle.SetPadTopMargin(0.08)
        #	tdrStyle.SetPadBottomMargin(0.14)
                tdrStyle.SetPadRightMargin(0.04)
                tdrStyle.SetPadLeftMargin(0.16)
        #	tdrStyle.SetCanvasDefH(800)
        #	tdrStyle.SetCanvasDefW(800)
        #	tdrStyle.SetPadGridX(1)
        #	tdrStyle.SetPadGridY(1)
                tdrStyle.SetPadTickX(1)
                tdrStyle.SetPadTickY(1)

                tdrStyle.SetTextFont(42) #132
                tdrStyle.SetTextSize(0.09)
                tdrStyle.SetLabelFont(42,"xyz")
                tdrStyle.SetTitleFont(42,"xyz")
                tdrStyle.SetLabelSize(0.045,"xyz") #0.035
                tdrStyle.SetTitleSize(0.045,"xyz")
                tdrStyle.SetTitleOffset(1.6,"y")

                tdrStyle.SetTitleX(0.16)
                tdrStyle.SetTitleY(0.93)
                tdrStyle.SetTitleColor(1)
                tdrStyle.SetTitleTextColor(1)
                tdrStyle.SetTitleFillColor(0)
                tdrStyle.SetTitleBorderSize(1)
                tdrStyle.SetTitleFontSize(0.04)
        #	tdrStyle.SetPadTopMargin(0.05)
        #	tdrStyle.SetPadBottomMargin(0.13)
        #	tdrStyle.SetPadLeftMargin(0.14)
        #	tdrStyle.SetPadRightMargin(0.02)

                # use bold lines and markers
                tdrStyle.SetMarkerStyle(8)
                tdrStyle.SetHistLineWidth(3)
                tdrStyle.SetLineWidth(1)

                tdrStyle.SetOptTitle(1)
                tdrStyle.SetOptStat(0)

                tdrStyle.cd()

        colors = [1, 2, 3, 4, 6, 7, 8, 9, 11]
        markers = [20, 21, 22, 23, 24, 25, 26, 27, 28]
        styles = [1, 2, 3, 4, 5, 6, 7, 8, 9]

        def format(self, h, i, fac = 1.):
                global colors, markers, styles

                h.UseCurrentStyle()
                h.SetFillColor(0)
                h.GetXaxis().SetTitleOffset(0.9)
                h.GetYaxis().SetTitleOffset(1.22)
        	h.SetLineColor(colors[i])
        	h.SetMarkerColor(colors[i])
        	h.SetLineStyle(styles[i])
        	h.SetMarkerStyle(markers[i])
                h.SetStats(0)


        def rebin(self, list, tmpl):
                global keep
                for i in range(len(list)):
                        old = list[i]
                        new = tmpl.Clone(old.GetName() + "_new")
                        keep.append(new)
                        for j in range(old.GetNbinsX()):
                                new.Fill(old.GetBinCenter(j), old.GetBinContent(j))
                        list[i] = new

        def prelim(self, header):
                t = ROOT.TPaveText(0.4, 0.92, 0.9, 0.97, "brNDC")
                title = "CMS Preliminary 2010"

#                if header == "GLOBAL":
#                        title = "p_{T} > 10 GeV and | #eta | < 2.5"
#                elif header == "PT_10-20":
#                        title = "10 < p_{T} < 20 GeV and | #eta | < 2.5"
#                elif header == "PT_20-40":
#                        title = "20 < p_{T} < 40 GeV and | #eta | < 2.5"
#                elif header == "ETA_0-1v5_PT_40-99999" :
#                        title = "p_{T} > 40 GeV and 0 < | #eta | < 1.5"
#                elif header == "ETA_1v5-2v5_PT_40-99999" :
#                        title = "p_{T} > 40 GeV and 1.5 < | #eta | < 2.5"
#                else:
#                        title = "CMS Preliminary 2010"

                t.AddText(title)
                t.SetTextSize(0.035)
                t.SetFillStyle(0)
                t.SetBorderSize(0)
                t.SetTextAlign(13)
                t.SetMargin(0.0)
                t.SetFillColor(0)
                t.Draw("SAME")
                return t

        ##### bDown stacking order: b first, then charm, then light
        def bDowndraw(self, mc, data, xTit, yTit, title, bintype, left, blind, norm):
                global keep

                c = Canvas.Canvas()
                keep.append(c)

                mc[4].SetFillColor(ROOT.kYellow) #total
                mc[3].SetFillColor(ROOT.kRed) #bottom
                mc[2].SetFillColor(ROOT.kGreen) #charm
                mc[1].SetFillColor(ROOT.kBlue) # light
                mc[0].SetFillColor(ROOT.kGray) #no info

                data.Sumw2()
                data.SetMarkerSize(1.2)
                data.SetMarkerColor(1)

                ################################# LINEAR PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(False)

                f1 = 1.4
                stack = ROOT.THStack( "bstack", title )

                ##### bDown stacking order
                #Stacking order b first, then charm, then light
                stack.Add(mc[3])
                stack.Add(mc[2])
                stack.Add(mc[1])
                stack.Add(mc[0])

                data.SetMaximum( max(data.GetMaximum(), mc[4].GetMaximum()) * f1)

                stack.SetTitle("")

                data.SetTitle("")
                data.SetXTitle(xTit)
                data.SetYTitle(yTit)

                data.Draw("E1")
                if not blind:
                        stack.Draw("histsame")
                else:
                        mc[4].Draw("histsame")
                data.Draw("sameE1")

                if left:
                        l = ROOT.TLegend(0.22, 0.73, 0.37, 0.88)
                else:
                        l = ROOT.TLegend(0.73, 0.73, 0.95, 0.88)
                l.SetFillColor(ROOT.kWhite)
                l.SetMargin(0.12)
                l.SetTextSize(0.035)
                l.SetBorderSize(0)

                keep.append(l)
                l.AddEntry(data, "DATA")
                if not blind:
                        l.AddEntry(mc[1], "MC (light)", "f")
                        l.AddEntry(mc[2], "MC (charm)", "f")
                        l.AddEntry(mc[3], "MC (bottom)", "f")
                        l.AddEntry(mc[0], "MC (no info)", "f")
                else :
                        l.AddEntry(mc[4], "MC")
                l.Draw()

                keep.append(self.prelim(bintype))

                if norm == 'area':
                    c.Print('PlotsArea/'+title)
                else: 
                    c.Print('PlotsLumi/'+title)

                ################################# LOG PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                f2 = 3.0
                data.SetMaximum( max(data.GetMaximum(), mc[4].GetMaximum()) * f2)
                data.SetMinimum( 0.2 )

                data.Draw("E1")
                if not blind:
                        stack.Draw("histsame")
                else:
                        mc[4].Draw("histsame")
                data.Draw("sameE1")

                l.Draw()
                keep.append(self.prelim(bintype))

                newtitle = 'log_'+title
                if norm == 'area':
                    c.Print('PlotsArea/'+newtitle)
                else: 
                    c.Print('PlotsLumi/'+newtitle)

                ################################# RATIO PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                hratio = data.Clone()
                hratio.Clear()
                hratio.Divide(data, mc[4], 1., 1.,"B")
                hratio.SetXTitle(xTit)
                if norm == 'area':
                    hratio.SetYTitle("Area Norm: data/MC ratio")
                else:
                    hratio.SetYTitle("Lumi Norm: data/MC ratio")

                hratio.SetMarkerSize(1.2)
                hratio.SetMarkerColor(1)
                hratio.SetLineWidth(3)

                hratio.Draw("E1")
                keep.append(self.prelim(bintype))

                ratio = 'ratio_'+title
                if norm == 'area':
                    c.Print('PlotsArea/'+ratio)
                else: 
                    c.Print('PlotsLumi/'+ratio)

		### Draw the plot and the ratio on the same canvas
                c.cd()
		c.Clear()
		c.Divide(1,2,0.01,0.0)

		c.Cd(1)
                ROOT.gPad.SetLogy(True)
		ROOT.gPad.SetPad( 0.0, 0.25, 1.0, 1.0 )
		ROOT.gPad.SetTopMargin(0.1)
		ROOT.gPad.SetLeftMargin(0.16) #0.13
		ROOT.gPad.SetRightMargin(0.04) #0.05

		c.Cd(2)
                ROOT.gPad.SetLogy(True)
		ROOT.gPad.SetPad( 0.0, 0.0,  1.0, 0.25 )
		ROOT.gPad.SetBottomMargin(0.375)
		ROOT.gPad.SetLeftMargin(0.16) #0.13
		ROOT.gPad.SetRightMargin(0.04) #0.05

		###
		c.Cd(1)
		data.GetYaxis().CenterTitle(1)
		data.GetYaxis().SetTitleSize( 0.055 )
		data.GetYaxis().SetTitleOffset( 1.3 )
		data.GetYaxis().SetLabelSize( 0.055 )
                data.Draw("E1")
                if not blind:
                        stack.Draw("histsame")
                else:
                        mc[4].Draw("histsame")
                data.Draw("sameE1")
                l.Draw()
                keep.append(self.prelim(bintype))
		

		###
		c.Cd(2)

		hratio.GetYaxis().CenterTitle(1)
		hratio.GetYaxis().SetTitleSize( 0.165 ) #0.11
		hratio.GetYaxis().SetTitleOffset( 0.4 ) #0.55
		hratio.GetYaxis().SetLabelSize( 0.16 )
		hratio.GetYaxis().SetNdivisions( 505 )

		hratio.GetXaxis().SetTitleSize( 0.16 )
		hratio.GetXaxis().SetLabelSize( 0.16 )
		hratio.GetXaxis().SetTitleOffset( 1 )
		hratio.GetXaxis().SetLabelOffset( 0.006 )
		hratio.GetXaxis().SetNdivisions( 510 )
		hratio.GetXaxis().SetTickLength( hratio.GetXaxis().GetTickLength() * 3.0 )

		hratio.SetYTitle("Data/MC")
                hratio.Draw("E1")

                ntitle = 'LOG_'+title
                if norm == 'area':
                    c.Print('PlotsArea/'+ntitle)
                else: 
                    c.Print('PlotsLumi/'+ntitle)


        #################################

        ##### bUp stacking order : light first, then charm, then bottom
        def bUpdraw(self, mc, data, xTit, yTit, title, bintype, left, blind, norm):
                global keep

                c = Canvas.Canvas()
                keep.append(c)

                mc[4].SetFillColor(ROOT.kYellow) #total
                mc[3].SetFillColor(ROOT.kRed) #bottom
                mc[2].SetFillColor(ROOT.kGreen) #charm
                mc[1].SetFillColor(ROOT.kBlue) # light
                mc[0].SetFillColor(ROOT.kGray) #no info

                data.Sumw2()
                data.SetMarkerSize(1.2)
                data.SetMarkerColor(1)

                ################################# LINEAR PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(False)

                f1 = 1.2
                stack = ROOT.THStack( "bstack", title )

                ##### bUp stacking order
                #Stacking order light first, then charm, then bottom
                stack.Add(mc[1]) #light
                stack.Add(mc[0]) #no info
                stack.Add(mc[2]) #charm
                stack.Add(mc[3]) #bottom

                data.SetMaximum( max(data.GetMaximum(), mc[4].GetMaximum()) * f1)

                stack.SetTitle("")

                data.SetTitle("")
                data.SetXTitle(xTit)
                data.SetYTitle(yTit)

                data.Draw("E1")
                if not blind:
                        stack.Draw("histsame")
                else:
                        mc[4].Draw("histsame")
                data.Draw("sameE1")

                if left:
                        l = ROOT.TLegend(0.22, 0.73, 0.37, 0.88)
                else:
                        l = ROOT.TLegend(0.73, 0.73, 0.95, 0.88)
                l.SetFillColor(ROOT.kWhite)
                l.SetMargin(0.12)
                l.SetTextSize(0.035)
                l.SetBorderSize(0)

                keep.append(l)
                l.AddEntry(data, "DATA")
                if not blind:
                        l.AddEntry(mc[1], "MC (light)", "f")
                        l.AddEntry(mc[2], "MC (charm)", "f")
                        l.AddEntry(mc[3], "MC (bottom)", "f")
                        l.AddEntry(mc[0], "MC (no info)", "f")
                else :
                        l.AddEntry(mc[4], "MC")
                l.Draw()
                keep.append(self.prelim(bintype))

                if norm == 'area':
                    c.Print('PlotsArea/'+title)
                else: 
                    c.Print('PlotsLumi/'+title)

                ################################# LOG PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                f2 = 3.0
                data.SetMaximum( max(data.GetMaximum(), mc[4].GetMaximum()) * f2)
                data.SetMinimum( 0.2 )

                data.Draw("E1")
                if not blind:
                        stack.Draw("histsame")
                else:
                        mc[4].Draw("histsame")
                data.Draw("sameE1")

                l.Draw()
                keep.append(self.prelim(bintype))

                newtitle = 'log_'+title
                if norm == 'area':
                    c.Print('PlotsArea/'+newtitle)
                else: 
                    c.Print('PlotsLumi/'+newtitle)

                ################################# RATIO PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                hratio = data.Clone()
                hratio.Clear()
                hratio.Divide(data, mc[4], 1., 1.,"B")
                hratio.SetXTitle(xTit)
                if norm == 'area':
                    hratio.SetYTitle("Area Norm: data/MC ratio")
                else:
                    hratio.SetYTitle("Lumi Norm: data/MC ratio")

                hratio.SetMarkerSize(1.2)
                hratio.SetMarkerColor(1)
                hratio.SetLineWidth(3)

                hratio.Draw("E1")
                keep.append(self.prelim(bintype))

                ratio = 'ratio_'+title
                if norm == 'area':
                    c.Print('PlotsArea/'+ratio)
                else: 
                    c.Print('PlotsLumi/'+ratio)

        #################################

        def drawQual(self, mc, data, xTit, yTit, title, bintype, left, norm):
                global keep

                c = Canvas.Canvas()
                keep.append(c)

                mc[2].SetFillColor(ROOT.kMagenta) #Loose
                mc[1].SetFillColor(ROOT.kBlue) #Tight
                mc[0].SetFillColor(ROOT.kRed) #HighPurity

                for i in range(3):
                        data[i].Sumw2()
                        data[i].SetMarkerSize(1.2)
                        data[i].SetMarkerColor(1)
                        data[i].SetMarkerStyle(20+i)

                ################################# LINEAR PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(False)

                for i in range(3):

                        f1 = 1.4
                        data[i].SetMaximum( max(data[i].GetMaximum(), mc[i].GetMaximum()) * f1)

                        data[i].SetTitle("")
                        data[i].GetXaxis().SetTitle(xTit)
                        data[i].GetYaxis().SetTitle(yTit)

                        data[i].Draw("E1")
                        mc[i].Draw("histsame")
                        data[i].Draw("sameE1")

                        if left:
                                leg = ROOT.TLegend(0.22, 0.63, 0.37, 0.88)
                        else:
                                leg = ROOT.TLegend(0.73, 0.73, 0.95, 0.88)
                                leg.SetFillColor(ROOT.kWhite)
                                leg.SetMargin(0.12)
                                leg.SetTextSize(0.035)
                                leg.SetBorderSize(0)

                        #keep.append(leg)
                        if i == 0 :
                                leg.SetHeader("HighPurity")
                        if i == 1 :
                                leg.SetHeader("Tight")
                        if i == 2 :
                                leg.SetHeader("Loose")

                        leg.AddEntry(data[i], "DATA")
                        leg.AddEntry(mc[i], "MC", "f")
                        leg.Draw()

                        keep.append(self.prelim(bintype))

                        if i == 0: qual = 'highpur'
                        if i == 1: qual = 'tight'
                        if i == 2: qual = 'loose'
                        ntitle = title+qual
                        if norm == 'area':
                                c.Print('PlotsArea/'+ntitle)
                        else: 
                                c.Print('PlotsLumi/'+ntitle)

                ################################# LOG PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                for i in range(3):
                        f2 = 3.0
                        data[i].SetMaximum( max(data[i].GetMaximum(), mc[i].GetMaximum()) * f2)
                        data[i].SetMinimum( 0.2 )

                        data[i].Draw("E1")
                        mc[i].Draw("histsame")
                        data[i].Draw("sameE1")

                        if left:
                                leg = ROOT.TLegend(0.22, 0.63, 0.37, 0.88)
                        else:
                                leg = ROOT.TLegend(0.73, 0.73, 0.95, 0.88)
                                leg.SetFillColor(ROOT.kWhite)
                                leg.SetMargin(0.12)
                                leg.SetTextSize(0.035)
                                leg.SetBorderSize(0)

                        #keep.append(leg)
                        if i == 0 :
                                leg.SetHeader("HighPurity")
                        if i == 1 :
                                leg.SetHeader("Tight")
                        if i == 2 :
                                leg.SetHeader("Loose")

                        leg.AddEntry(data[i], "DATA")
                        leg.AddEntry(mc[i], "MC", "f")
                        leg.Draw()

                        keep.append(self.prelim(bintype))

                        if i == 0: qual = 'highpur'
                        if i == 1: qual = 'tight'
                        if i == 2: qual = 'loose'
                        newtitle = 'log_'+title+qual
                        if norm == 'area':
                                c.Print('PlotsArea/'+newtitle)
                        else: 
                                c.Print('PlotsLumi/'+newtitle)

                ################################# STACKED LINEAR PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(False)
                #MC stack
                mcstack = ROOT.THStack( "mcstack", title )

                #Stacking order loose, tight, highpur
                mcstack.Add(mc[2])
                mcstack.Add(mc[1])
                mcstack.Add(mc[0])

                #Data stack
                datastack = ROOT.THStack( "datastack", title )

                #Stacking order loose, tight, highpur
                datastack.Add(data[2])
                datastack.Add(data[1])
                datastack.Add(data[0])

                f1 = 1.2
		datastack.SetMaximum( max(datastack.GetMaximum(), mcstack.GetMaximum()) * f1)

                datastack.SetTitle("")

                datastack.Draw("E1")
                mcstack.Draw("histsame")
                datastack.Draw("sameE1")
        	datastack.GetXaxis().SetTitle(xTit)
        	datastack.GetYaxis().SetTitle(yTit)
		c.Modified()

                if left:
                        l = ROOT.TLegend(0.22, 0.63, 0.37, 0.88)
                else:
                        l = ROOT.TLegend(0.63, 0.63, 0.95, 0.88)
                l.SetFillColor(ROOT.kWhite)
                l.SetMargin(0.12)
                l.SetTextSize(0.035)
                l.SetBorderSize(0)

                keep.append(l)
                l.AddEntry(data[0], "DATA: HighPurity")
                l.AddEntry(data[1], "DATA: Tight")
                l.AddEntry(data[2], "DATA: Loose")
                l.AddEntry(mc[0], "MC: HighPurity", "f")
                l.AddEntry(mc[1], "MC: Tight", "f")
                l.AddEntry(mc[2], "MC: Loose", "f")
                l.Draw()
                keep.append(self.prelim(bintype))

		c.Update()

                if norm == 'area':
                        c.Print('PlotsArea/'+title)
                else: 
                        c.Print('PlotsLumi/'+title)

                ################################# STACKED LOG PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                f2 = 4.0
		datastack.SetMaximum( max(datastack.GetMaximum(), mcstack.GetMaximum()) * f2)
                datastack.SetMinimum( 0.2 )

                datastack.Draw("E1")
                mcstack.Draw("histsame")
                datastack.Draw("sameE1")

                l.Draw()
                keep.append(self.prelim(bintype))

		c.Update()

                newtitle = 'log_'+title
                if norm == 'area':
                        c.Print('PlotsArea/'+newtitle)
                else: 
                        c.Print('PlotsLumi/'+newtitle)

        #################################
  
        #### Draw method for Profile Plots
        def drawProfile(self, mc, data, xTit, yTit, title, bintype, left, norm):
                global keep

                c = Canvas.Canvas()
                keep.append(c)

                mc[4].SetLineColor(1) #total
                mc[3].SetLineColor(ROOT.kRed) #bottom
                mc[2].SetLineColor(ROOT.kGreen) #charm
                mc[1].SetLineColor(ROOT.kBlue) # light
                mc[0].SetLineColor(ROOT.kGray) #no info

                mc[4].SetLineWidth(3)
                mc[3].SetLineWidth(3)
                mc[2].SetLineWidth(3)
                mc[1].SetLineWidth(3)
                mc[0].SetLineWidth(3)

                mc[4].SetErrorOption("i")
                mc[3].SetErrorOption("i")
                mc[2].SetErrorOption("i")
                mc[1].SetErrorOption("i")
                mc[0].SetErrorOption("i")

                data.SetMarkerSize(1.2)
                data.SetMarkerColor(1)
                data.SetErrorOption("i")

                ################################# LINEAR PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(False)

                f1 = 1.4
                data.SetMaximum( max(data.GetMaximum(), mc[4].GetMaximum()) * f1)

                data.SetTitle("")
                data.SetXTitle(xTit)
                data.SetYTitle(yTit)

                data.Draw("E1")
        	#mc[0].Draw("histsame")
                mc[1].Draw("histsame")
                mc[2].Draw("histsame")
                mc[3].Draw("histsame")
                mc[4].Draw("histsame")
                data.Draw("sameE1")

                if left:
                        l = ROOT.TLegend(0.22, 0.73, 0.37, 0.88)
                else:
                        l = ROOT.TLegend(0.73, 0.73, 0.95, 0.88)
                l.SetFillColor(ROOT.kWhite)
                l.SetMargin(0.12)
                l.SetTextSize(0.035)
                l.SetBorderSize(0)

                keep.append(l)
                l.AddEntry(data, "DATA")
                l.AddEntry(mc[4], "MC Total", "l")
                l.AddEntry(mc[1], "MC (light)", "l")
                l.AddEntry(mc[2], "MC (charm)", "l")
                l.AddEntry(mc[3], "MC (bottom)", "l")
                #l.AddEntry(mc[0], "MC (no info)", "l")
                l.Draw()
                keep.append(self.prelim(bintype))

                if norm == 'area':
                        c.Print('PlotsArea/'+title)
                else: 
                        c.Print('PlotsLumi/'+title)

                ################################# LOG PLOT ################################# 
                c.cd()
                ROOT.gPad.SetLogy(True)

                f2 = 3.0
                data.SetMaximum( max(data.GetMaximum(), mc[4].GetMaximum()) * f2)
                data.SetMinimum( 0.2 )

                data.Draw("E1")
                #mc[0].Draw("histsame")
                mc[1].Draw("histsame")
                mc[2].Draw("histsame")
                mc[3].Draw("histsame")
                mc[4].Draw("histsame")
                data.Draw("sameE1")

                l.Draw()
                keep.append(self.prelim(bintype))

                newtitle = 'log_'+title
                if norm == 'area':
                        c.Print('PlotsArea/'+newtitle)
                else: 
                        c.Print('PlotsLumi/'+newtitle)

        #################################

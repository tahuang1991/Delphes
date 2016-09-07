import random
import ROOT
import os
from Helper import *
import numpy as np
ROOT.gROOT.SetBatch(1)
#gStyle from TStyle
ROOT.gStyle.SetStatW(0.17)
ROOT.gStyle.SetStatH(0.15)

ROOT.gStyle.SetOptStat(111110)

ROOT.gStyle.SetTitleStyle(0)
ROOT.gStyle.SetTitleAlign(13) ## coord in top left
ROOT.gStyle.SetTitleX(0.)
ROOT.gStyle.SetTitleY(1.)
ROOT.gStyle.SetTitleW(1)
#ROOT.gStyle.SetTitleTextColor(4)
ROOT.gStyle.SetTitleXSize(0.05)
ROOT.gStyle.SetTitleYSize(0.05)
ROOT.gStyle.SetTitleH(0.058)
ROOT.gStyle.SetTitleBorderSize(0)

ROOT.gStyle.SetPadLeftMargin(0.126)
ROOT.gStyle.SetPadRightMargin(0.14)
ROOT.gStyle.SetPadTopMargin(0.06)
ROOT.gStyle.SetPadBottomMargin(0.13)


ybinLow = [0.1,.5,1.0,2.0,4.0,6.0,8.0,10.0,20,40,60.0,80,100,200,400,600,1000.0,2000,4000,6000, 10000, 20000, 40000, 60000,100000.0]
contentbins = np.asarray(ybinLow)
ybinLow2 = [1.0,2.0,4.0,6.0,10.0,20,40,60.0,100,200,400,600,1000.0,2000,4000,6000, 10000, 20000, 40000, 60000,100000.0,200000,400000,800000]
integralbins = np.asarray(ybinLow2)

def hist1D(tree, todraw, xbins, cut, B):

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    b1 = ROOT.TH1F("%s"%B,"%s"%B,xBins,xminBin,xmaxBin)
    tree.Draw("%s>>%s"%(todraw,B),cut)
    b1.Print()
    return b1

#hist0: normalized sum, 1:sum, 2: maxbincenter with maxbincontent, 3: maxbincenter(most probable mass)
def fillHistogram(rootfile, nfile, histss):


    	f = ROOT.TFile(rootfile)
	lists = f.GetListOfKeys()
	for x in range(len(lists)):
		subkey = lists.At(x)
		obj = subkey.ReadObj()
		if obj.GetName()=="evtree":
			continue
		#print " title ",obj.GetTitle()," Name ",obj.GetName()
		maxbin = obj.GetMaximumBin()
		maxbincenter = obj.GetBinCenter(maxbin)
		maxbincontent = obj.GetBinContent(maxbin)
		
		histss[2][nfile].Fill(maxbincenter, maxbincontent)
		histss[3][nfile].Fill(maxbincenter)
		histss[1][nfile].Add(obj.Rebin(20))
		obj.Scale(1.0/obj.Integral())
		#hists_1[nfile].Fill(obj.GetXaxis().GetBinCenter(maxbin))
		histss[0][nfile].Add(obj)


#___________________________________________
def draw1D_v2(filelist,x_bins,x_title,cut,benchmarks, Events, Lumi, NormalizedToUnity, pic_name):
    
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow WWWW"+" "*24 + "14TeV")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.GetYaxis().SetRangeUser(0.0,10000)
    b1.SetStats(0)
     

    color = [ROOT.kRed, ROOT.kBlue, ROOT.kMagenta+2, ROOT.kGreen+2, ROOT.kCyan]
    marker = [20,21,22,23,34]
    legend = ROOT.TLegend(0.75,0.5,0.86,0.85)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    hs1 = ROOT.THStack("hs1","%s distribution"%x_title)
    hs2 = ROOT.THStack("hs2","%s distribution"%x_title)
    hs3 = ROOT.THStack("hs3","%s distribution"%x_title)
    hs4 = ROOT.THStack("hs4","%s distribution"%x_title)
    hists_1 = []
    hists_2 = []
    hists_3 = []
    hists_4 = []
    for nfile in range(len(filelist)):
	hists_1.append(ROOT.TH1F("hist1_%d"%nfile,"hist1_%d"%nfile, 190, 200, 4000))
	ROOT.SetOwnership(hists_1[nfile],False)
	hists_2.append(ROOT.TH1F("hist2_%d"%nfile,"hist2_%d"%nfile, 190, 200, 4000))
	ROOT.SetOwnership(hists_2[nfile],False)
	hists_3.append(ROOT.TH1F("hist3_%d"%nfile,"hist3_%d"%nfile,xBins, xminBin, xmaxBin))
	ROOT.SetOwnership(hists_3[nfile],False)
	hists_4.append(ROOT.TH1F("hist4_%d"%nfile,"hist4_%d"%nfile,xBins, xminBin, xmaxBin))
	ROOT.SetOwnership(hists_4[nfile],False)
    #Events = [100000, 1000000, 1000000, 4000000]	
    for nfile in range(len(filelist)):
	
	filedir = filelist[nfile]
	B = benchmarks[nfile]
	weight = 0.0
    	if B == "TTbar":
		b = 'tt'
		print "CS for TTbar ", getCrossSection(b)," weight ",GetWeight(b, 300,Events[nfile])	
		weight = GetWeight(b, 300,Events[nfile])
    	else:
		print "CS for  ",B, getCrossSection(B)," weight ",GetWeight(B, 300,Events[nfile])	
		weight = GetWeight(B, 300,Events[nfile])
	print "filedir: ",filedir
    	if os.path.isdir(filedir):
          ls = os.listdir(filedir)
          for rootfile in ls:
                rootfile = filedir[:]+rootfile
		#print "rootfile ",rootfile
                if os.path.isdir(rootfile):
                        continue
		fillHistogram(rootfile,nfile,[hists_1, hists_2, hists_3, hists_4])
    	elif os.path.isfile(filedir):
		#print "it is a file"
		fillHistogram(filedir,nfile,[hists_1, hists_2, hists_3, hists_4])
    	else:
          print " it is not a file or dir ", filedir


	hists_1[nfile].SetLineColor(color[nfile])
	hists_1[nfile].SetMarkerColor(color[nfile])
	hists_1[nfile].SetMarkerStyle(marker[nfile])
	
	hists_2[nfile].SetLineColor(color[nfile])
	hists_2[nfile].SetMarkerColor(color[nfile])
	hists_2[nfile].SetMarkerStyle(marker[nfile])

	hists_3[nfile].SetLineColor(color[nfile])
	hists_3[nfile].SetMarkerColor(color[nfile])
	hists_3[nfile].SetMarkerStyle(marker[nfile])

	hists_4[nfile].SetLineColor(color[nfile])
	hists_4[nfile].SetMarkerColor(color[nfile])
	hists_4[nfile].SetMarkerStyle(marker[nfile])
	
	if (NormalizedToUnity):
		hists_1[nfile].Scale(1.0/hists_1[nfile].Integral())
        	hists_2[nfile].Scale(1.0/hists_2[nfile].Integral())
		hists_3[nfile].Scale(1.0/hists_3[nfile].Integral())
		hists_4[nfile].Scale(1.0/hists_4[nfile].Integral())
	else:
		hists_1[nfile].Scale(weight)
		hists_2[nfile].Scale(weight)
		hists_3[nfile].Scale(weight)
		hists_4[nfile].Scale(weight)
		

	hs1.Add(hists_1[nfile])
	hs2.Add(hists_2[nfile])
	hs3.Add(hists_3[nfile])
	hs4.Add(hists_4[nfile])
	legend.AddEntry(hists_1[nfile], "%s"%B, "pl")
    c1 = ROOT.TCanvas("c1","c1")
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()
  
    #c1.Divide(2,2)
    c1.cd()
    #b1.Draw()
    hs1.Draw("nostack+p")
    hs1.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    #hs1.GetHistogram().GetXaxis().SetRangeUser(xminBin, xmaxBin)
    c1.SetLogy()
    legend.Draw("same")
    tex1 = ROOT.TLatex(0.4,.9,"Integrated Luminosity = %d fb^{-1}"%Lumi)
    tex1.SetTextSize(0.05)
    tex1.SetTextFont(42)
    tex1.SetNDC()
    tex1.Draw("same")
    c1.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_addnormalizedall_combined.png"%pic_name)
    c1.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_addnormalizedall_combined.pdf"%pic_name)
    c1.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_addnormalizedall_combined.C"%pic_name)



    c2 = ROOT.TCanvas("c2","c2")
    c2.SetGridx()
    c2.SetGridy()
    c2.SetTickx()
    c2.SetTicky()
  
    c2.cd()
    #b1.Draw()
    hs2.Draw("nostack+p")
    c2.SetLogy()
    hs2.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    #hs2.GetHistogram().GetXaxis().SetRangeUser(xminBin, xmaxBin)
    legend.Draw("same")
    tex2 = ROOT.TLatex(0.25,.50,"add all survival solution(normalized in each event)")
    tex2.SetTextSize(0.05)
    tex2.SetTextFont(62)
    tex2.SetNDC()
    #tex2.Draw("same")
    tex1.Draw("same")
    c2.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_addall_combined.png"%pic_name)
    c2.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_addall_combined.pdf"%pic_name)
    c2.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_addall_combined.C"%pic_name)

    c3 = ROOT.TCanvas("c3","c3")
    c3.SetGridx()
    c3.SetGridy()
    c3.SetTickx()
    c3.SetTicky()
  
    hs3.Draw("nostack+p")
    c3.SetLogy()
    hs3.SetTitle("bincontent of maximum bin")
    hs3.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend.Draw("same")
    tex3 = ROOT.TLatex(0.25,.50,"maximum  bin content from MMC")
    tex3.SetTextSize(0.05)
    tex3.SetTextFont(62)
    tex3.SetNDC()
    #tex3.Draw("same")
    tex1.Draw("same")
    c3.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_maxbincontent_combined.png"%pic_name)
    c3.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_maxbincontent_combined.pdf"%pic_name)
    c3.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_maxbincontent_combined.C"%pic_name)

    #c1.cd(4)
    c4 = ROOT.TCanvas("c4","c4")
    c4.SetGridx()
    c4.SetGridy()
    c4.SetTickx()
    c4.SetTicky()
  
    hs4.SetTitle("most probable mass")
    c4.SetLogy()
    hs4.Draw("nostack+p")
    hs4.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend.Draw("same")
    tex4 = ROOT.TLatex(0.25,.50,"total number of survival solutions")
    tex4.SetTextSize(0.05)
    tex4.SetTextFont(62)
    tex4.SetNDC()
    #tex4.Draw("same")
    tex1.Draw("same")
    c4.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_mostprobable_combined.png"%pic_name)
    c4.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_mostprobable_combined.pdf"%pic_name)
    c4.SaveAs("DOEDiHiggs/Hhh_PDFvalidation_%s_mostprobable_combined.C"%pic_name)

    #c1.cd()

    #c1.SaveAs("Hhh_PDFvalidation_%s_combined.png"%pic_name)
	


		
#___________________________________________
def draw1D(filelist,dir,todraw,x_bins,x_title, y_title, cut,benchmarks, pic_name):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow WWWW"+" "*24 + "14TeV")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    b1.GetYaxis().SetRangeUser(0.0,10000)
    b1.SetStats(0)
    #b1.Draw()
     
    color = [ROOT.kRed, ROOT.kBlue, ROOT.kMagenta+2, ROOT.kGreen+2, ROOT.kCyan]
    marker = [20,21,22,23,34]
    legend = ROOT.TLegend(0.65,0.56,0.86,0.92)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    hs = ROOT.THStack("hs","M_{H} from MMC reconstruction")
    hists = []
    for nfile in range(len(filelist)):
	
	B = benchmarks[nfile]
        chain = ROOT.TChain(dir)	
	filedir = filelist[nfile]
    	if os.path.isdir(filedir):
          ls = os.listdir(filedir)
          for x in ls:
                x = filedir[:]+x
                if os.path.isdir(x):
                        continue
                chain.Add(x)
    	elif os.path.isfile(filedir):
          chain.Add(filedir)
    	else:
          print " it is not file or dir ", filedir

	#chain.Draw("%s>>%s(%d,%f,%f)"%(todraw,B, xBins, xminBin, xmaxBin ), cut)
	hist = hist1D(chain, todraw, x_bins, cut, B)
	#hist = hist1D(t, todraw, xBins, cut, B)
	#hist = ROOT.TH1F(ROOT.gDirectory.Get(B)).Clone()
	#hs.Add(ROOT.TH1F(ROOT.gDirectory.Get(B)))
        #legend.AddEntry(ROOT.TH1F(ROOT.gDirectory.Get(B)),"%s"%B,"pl") 
	#hists.append(ROOT.TH1F(ROOT.gDirectory.Get(B)).Clone(B))
	hist.SetLineColor(color[nfile])
	hist.SetLineWidth(3)
	hist.SetLineStyle(3)
	hist.SetMarkerColor(color[nfile])
	hist.SetMarkerStyle(marker[nfile])
	#hist.SetMarkerSize(2)
	hist.Scale(1.0/hist.GetEntries())
        hs.Add(hist)
        
	legend.AddEntry(hist, "%s"%B, "p")

	htmp = hist1D(t, todraw, x_bins, cut, B)
	#htmp.Print("ALL")
	hists.append(hist)
	print "nfile ",nfile," B ",B
    print "hists ",hists	
 
    hs.Draw("nostack,p")
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    hs.GetHistogram().GetYaxis().SetTitle("%s"%y_title)
    legend.Draw("same")
    c1.SaveAs("%s_Hhh_Evtreevalidation.pdf"%pic_name)
    c1.SaveAs("%s_Hhh_Evtreevalidation.png"%pic_name)
    c1.SaveAs("%s_Hhh_Evtreevalidation.C"%pic_name)


def fill2DHist(rootfile, nfile, histss):


    	f = ROOT.TFile(rootfile)
	lists = f.GetListOfKeys()
	for x in range(len(lists)):
		subkey = lists.At(x)
		obj = subkey.ReadObj()
		if obj.GetName()=="evtree":
			continue
		
		maxbin = obj.GetMaximumBin()
		maxbincenter = obj.GetBinCenter(maxbin)
		maxbincontent = obj.GetBinContent(maxbin)
		#if maxbincontent <1.0:
		#	print "rootfile",rootfile, " title ",obj.GetTitle()," Name ",obj.GetName()," mass ",maxbincenter," maxbincontent ",maxbincontent
 		histss[0].Fill(maxbincenter, maxbincontent)
 		histss[1].Fill(maxbincenter, obj.Integral())
		
#___________________________________________
def draw2D(filelist,x_bins, y_bins, x_title, y_title, cut,benchmarks, Events, Lumi, NormalizedToUnity, pic_name):
    
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow WWWW"+" "*24 + "14TeV")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.GetYaxis().SetRangeUser(0.0,10000)
    b1.SetStats(0)
     

    color = [ROOT.kRed, ROOT.kBlue, ROOT.kMagenta+2, ROOT.kGreen+2, ROOT.kCyan]
    marker = [20,21,22,23,34]
     
    hists1 = []
    hists2 = [] 
    hProfiles1 = []
    hProfiles2 = []
    for nfile in range(len(filelist)):
	B = benchmarks[nfile]
	hist1 = ROOT.TH2F("hist1_%d"%nfile,"%s"%B, xBins, xminBin, xmaxBin, len(contentbins)-1, contentbins)
	hist2 = ROOT.TH2F("hist2_%d"%nfile,"%s"%B, xBins, xminBin, xmaxBin, len(contentbins)-1, contentbins)
        hist1.GetXaxis().SetTitle("%s"%x_title)
        hist1.GetYaxis().SetTitle("%s"%y_title)
        hist2.GetXaxis().SetTitle("%s"%x_title)
        hist2.GetYaxis().SetTitle("Integral")
	ROOT.SetOwnership(hist1,False)
	ROOT.SetOwnership(hist2,False)
	filedir = filelist[nfile]
	weight = 0.0
        hists1.append(hist1)
        hists2.append(hist2)
    	if B == "TTbar":
		b = 'tt'
		print "CS for TTbar ", getCrossSection(b)," weight ",GetWeight(b, 300,Events[nfile])	
		weight = GetWeight(b, 300,Events[nfile])
    	else:
		print "CS for  ",B, getCrossSection(B)," weight ",GetWeight(B, 300,Events[nfile])	
		weight = GetWeight(B, 300,Events[nfile])
	print "filedir: ",filedir
    	if os.path.isdir(filedir):
          ls = os.listdir(filedir)
          for rootfile in ls:
                rootfile = filedir[:]+rootfile
		#print "rootfile ",rootfile
                if os.path.isdir(rootfile):
                        continue
		fill2DHist(rootfile, nfile, [hist1, hist2])
    	elif os.path.isfile(filedir):
		#print "it is a file"
		fill2DHist(filedir, nfile, [hist1, hist2])
    	else:
          print " it is not a file or dir ", filedir
	hProfiles1.append(hist1.ProfileX())
	hProfiles2.append(hist2.ProfileX())
    c1 = ROOT.TCanvas("c1","c1",1200,900)    
    c1.Divide(2,2)
    #pad11 = c1.GetListOfPrimitives().At(0)
    #pad12 = c1.GetListOfPrimitives().At(1)
    #pad13 = c1.GetListOfPrimitives().At(2)
    #pad11 = ROOT.TPad(c1.cd(1))
    c1.cd(1)
    hists1[0].Draw("colz")
    hProfiles1[0].Draw("same")
    ROOT.gPad.SetLogy()
    c1.cd(2)
    hists1[1].Draw("colz")
    hProfiles1[1].Draw("same")
    ROOT.gPad.SetLogy()
    c1.cd(3)
    hists1[2].Draw("colz")
    hProfiles1[2].Draw("same")
    ROOT.gPad.SetLogy()
    c1.cd(4)
    hists1[3].Draw("colz")
    hProfiles1[3].Draw("same")
    ROOT.gPad.SetLogy()
    c1.cd()
    c1.SaveAs("%s_mostprobablevsbincontent_logy_combined.png"%pic_name)
    c1.SaveAs("%s_mostprobablevsbincontent_logy_combined.pdf"%pic_name)
    c1.SaveAs("%s_mostprobablevsbincontent_logy_combined.C"%pic_name)
    """
    c2 = ROOT.TCanvas("c2","c2",1200,900)    
    c2.Divide(2,2)
    c2.cd(1)
    hists1[0].Scale(1.0/hists1[0].Integral())
    hists1[0].Draw("colz")
    ROOT.gPad.SetLogy()
    c2.cd(2)
    hists1[1].Scale(1.0/hists1[1].Integral())
    hists1[1].Draw("colz")
    ROOT.gPad.SetLogy()
    c2.cd(3)
    hists1[2].Scale(1.0/hists1[2].Integral())
    hists1[2].Draw("colz")
    ROOT.gPad.SetLogy()
    c2.cd(4)
    hists1[3].Scale(1.0/hists1[3].Integral())
    hists1[3].Draw("colz")
    ROOT.gPad.SetLogy()
    c2.cd()
    c2.SaveAs("%s_mostprobablevsbincontent_Unity_logy_combined.png"%pic_name)
    c2.SaveAs("%s_mostprobablevsbincontent_Unity_logy_combined.pdf"%pic_name)
    c2.SaveAs("%s_mostprobablevsbincontent_Unity_logy_combined.C"%pic_name)
    """    
    
    c3 = ROOT.TCanvas("c3","c3",1200,900)    
    c3.Divide(2,2)
    #pad11 = c1.GetListOfPrimitives().At(0)
    #pad12 = c1.GetListOfPrimitives().At(1)
    #pad13 = c1.GetListOfPrimitives().At(2)
    #pad11 = ROOT.TPad(c1.cd(1))
    c3.cd(1)
    hists2[0].Draw("colz")
    hProfiles2[0].Draw("same")
    ROOT.gPad.SetLogy()
    c3.cd(2)
    hists2[1].Draw("colz")
    hProfiles2[1].Draw("same")
    ROOT.gPad.SetLogy()
    c3.cd(3)
    hists2[2].Draw("colz")
    hProfiles2[2].Draw("same")
    ROOT.gPad.SetLogy()
    c3.cd(4)
    hists2[3].Draw("colz")
    hProfiles2[3].Draw("same")
    ROOT.gPad.SetLogy()
    c3.cd()
    c3.SaveAs("%s_mostprobablevsintegral_logy_combined.png"%pic_name)
    c3.SaveAs("%s_mostprobablevsintegral_logy_combined.pdf"%pic_name)
    c3.SaveAs("%s_mostprobablevsintegral_logy_combined.C"%pic_name)

    legend = ROOT.TLegend(0.75,0.5,0.86,0.85)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    for n in range(len(filelist)):
	hProfiles1[n].SetMarkerColor(color[n])
	hProfiles1[n].SetMarkerStyle(marker[n])

	hProfiles2[n].SetMarkerColor(color[n])
	hProfiles2[n].SetMarkerStyle(marker[n])
        legend.AddEntry(hProfiles1[n],"%s"%benchmarks[n],"p")


    c4 = ROOT.TCanvas("c4","c4",800,600)    
    c4.Draw()
    hProfiles1[0].SetStats(0)
    hProfiles1[0].Draw() 
    hProfiles1[1].Draw("same") 
    hProfiles1[2].Draw("same") 
    hProfiles1[3].Draw("same") 
    legend.Draw("same")
    c4.SetLogy()
    c4.SaveAs("%s_mostprobablevsbincontent_Profile_combined.png"%pic_name)
    c4.SaveAs("%s_mostprobablevsbincontent_Profile_combined.pdf"%pic_name)
    c4.SaveAs("%s_mostprobablevsbincontent_Profile_combined.C"%pic_name)
    
    c5 = ROOT.TCanvas("c5","c5",800,600)    
    c5.Draw()
    hProfiles2[0].SetStats(0)
    hProfiles2[0].Draw() 
    hProfiles2[1].Draw("same") 
    hProfiles2[2].Draw("same") 
    hProfiles2[3].Draw("same") 
    legend.Draw("same")
    c5.SetLogy()
    c5.SaveAs("%s_mostprobablevsintegral_Profile_combined.png"%pic_name)
    c5.SaveAs("%s_mostprobablevsintegral_Profile_combined.pdf"%pic_name)
    c5.SaveAs("%s_mostprobablevsintegral_Profile_combined.C"%pic_name)
    

filedir = "/fdata/hepx/store/user/lpernie/Hhh/"
filelist = [filedir+"delphes_B3_1M_PU40ALL_13May.root",filedir+"delphes_B6_1M_PU40ALL_13May.root",filedir+"delphes_B9_1M_PU40ALL_13May.root",filedir+"delphes_B12_1M_PU40ALL_13May.root"]
#filelist = ["DiHiggs_WWbb_1M_NewB3_allReco_25_MMC1M_isomu_MVA_PU40_0415_combined.root","DiHiggs_WWbb_1M_NewB6_allReco_25_MMC1M_isomu_MVA_PU40_0509_combined.root","DiHiggs_WWbb_1M_NewB9_allReco_25_MMC1M_isomu_MVA_PU40_0509_combined.root"]
filedir = "/fdata/hepx/store/user/taohuang/Hhh/combined_samples/"
filelist = [filedir+"DiHiggs_WWbb_1M_NewB3_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root",filedir+"DiHiggs_WWbb_1M_NewB6_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root",filedir+"DiHiggs_WWbb_1M_NewB9_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root",filedir+"DiHiggs_WWbb_1M_NewB12_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root",filedir+"TTbar_Wtomu_4M_allReco_Updatebtag_25_MVA_PU40_0725_combined.root"]
filelist = [filedir+"DiHiggs_WWbb_1M_NewB3_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root",filedir+"DiHiggs_WWbb_1M_NewB6_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root",filedir+"DiHiggs_WWbb_1M_NewB9_allReco_25_MMC1M_isomu_MVA_PU40_0725_combined.root","/fdata/hepx/store/user/taohuang/Hhh/Delphes_ttbar_4M_Wtomu_allReco_Updatebtag_25_MVA_PU40_0725/"]
filedir = "/fdata/hepx/store/user/taohuang/Hhh/"
filelist = [filedir+"Delphes_Hhh_NewB3_allReco_25_MMC1M_isomu_Clearcut_PU40_0901/",filedir+"Delphes_Hhh_NewB6_allReco_25_MMC1M_isomu_Clearcut_PU40_0901/",filedir+"Delphes_Hhh_NewB9_allReco_25_MMC1M_isomu_Clearcut_PU40_0901/",filedir+"Delphes_ttbar_4M_Wtomu_allReco_Updatebtag_25_Clearcut_PU40_0901/"]
benchmarks = ["B3","B6","B9","TTbar"]	
Events = [1000000, 1000000, 1000000, 4000000]
cut = "MMC_h2massweight1_prob>200 && hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons && (((b1jet_btag&2)>0 && (b2jet_btag&3)>0) || ((b1jet_btag&3)>0 && (b 2jet_btag&2)>0)) && dR_l1l2<3.3 && dR_l1l2>0.07 && dR_b1b2<5. && mass_l1l2<100 && mass_l1l2>5. && mass_b1b2>22 && dR_bl<5 && dR_l1l2b1b2<6 && MINdR_bl<3.2 && MINdR_bl>0.4 && mass_b1b2<700 && mass_trans<250 && MT2<400 && pt_b1b2<300"

pic_name = "testplots/MMC_h2mass_normalizedto300lumi"	
x_title = "MMC Mass [GeV]"
x_bins = "(40,200,1000)"
#draw1D_v2(filelist,x_bins,x_title,cut,benchmarks,Events, 300, False, pic_name)	
y_bins = "(100,0,10000.0)"
y_title = "Maxbincontent" 
draw2D(filelist,x_bins, y_bins, x_title, y_title, cut,benchmarks, Events, 300, False, pic_name)
pic_name = "testplots/MMC_h2mass_normalizedto3000lumi"	
#draw1D_v2(filelist,x_bins,x_title,cut,benchmarks,Events, 3000, False, pic_name)	




pic_name = "paper/MMC_h2massweight1_prob"	
todraw = "MMC_h2massweight1_prob"
x_title = "M_{H} [GeV]"
y_title = "arbitray unit"
x_bins = "(80,200.0,1000.0)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,y_title, cut,benchmarks, pic_name)	
pic_name = "paper/MMC_h2mass_prob"	
todraw = "MMC_h2mass_prob"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,y_title, cut,benchmarks, pic_name)	


pic_name = "PhionshellW"	
todraw = "w1_phi*(w1_mass>w2_mass)+w2_phi*(w1_mass<w2_mass)"
x_title = "#Phi of onshell W"
todraw = "w1_eta*(w1_mass>w2_mass)+w2_eta*(w1_mass<w2_mass)"
x_bins = "(100,-3.1415,3.1415)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PtonshellW"	
todraw = "w1_pt*(w1_mass>w2_mass)+w2_pt*(w1_mass<w2_mass)"
x_title = "p_{T} of onshell W"
x_bins = "(100,0,400)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	


pic_name = "EtanuonshellW"	
todraw = "nu1_eta*(w1_mass>w2_mass)+nu2_eta*(w1_mass<w2_mass)"
x_title = "#eta of neutrinos from onshell W"
x_bins = "(100,-4,4)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PhinuonshellW"	
todraw = "nu1_phi*(w1_mass>w2_mass)+nu2_phi*(w1_mass<w2_mass)"
x_title = "#phi of neutrinos from onshell W"
x_bins = "(100,-3.1415,3.1415)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PtnuonshellW"	
todraw = "nu1_pt*(w1_mass>w2_mass)+nu2_pt*(w1_mass<w2_mass)"
x_title = "p_{T} of neutrinos from onshell W"
x_bins = "(100,0,400)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "EtaoffshellW"	
todraw = "w1_eta*(w1_mass<w2_mass)+w2_eta*(w1_mass>w2_mass)"
x_title = "#eta of offshell W"
x_bins = "(100,-4,4)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PhioffshellW"	
todraw = "w1_phi*(w1_mass<w2_mass)+w2_phi*(w1_mass>w2_mass)"
x_title = "#Phi of offshell W"
x_bins = "(100,-3.1415,3.1415)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PtoffshellW"	
todraw = "w1_pt*(w1_mass<w2_mass)+w2_pt*(w1_mass>w2_mass)"
x_title = "p_{T} of offshell W"
x_bins = "(100,0,400)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "EtanuoffshellW"	
todraw = "nu1_eta*(w1_mass<w2_mass)+nu2_eta*(w1_mass>w2_mass)"
x_title = "#eta of neutrinos from offshell W"
x_bins = "(100,-4,4)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PhinuoffshellW"	
todraw = "nu1_phi*(w1_mass<w2_mass)+nu2_phi*(w1_mass>w2_mass)"
x_title = "#phi of neutrinos from offshell W"
x_bins = "(100,-3.1415,3.1415)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

pic_name = "PtnuoffshellW"	
todraw = "nu1_pt*(w1_mass<w2_mass)+nu2_pt*(w1_mass>w2_mass)"
x_title = "p_{T} of neutrinos from offshell W"
x_bins = "(100,0,400)"
#draw1D(filelist,"evtree",todraw,x_bins,x_title,cut,benchmarks, pic_name)	

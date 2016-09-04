import random
import ROOT
import os
from Helper import *
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
    maker = [20,21,22,23,34]
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
	hists_1[nfile].SetMarkerStyle(maker[nfile])
	
	hists_2[nfile].SetLineColor(color[nfile])
	hists_2[nfile].SetMarkerColor(color[nfile])
	hists_2[nfile].SetMarkerStyle(maker[nfile])

	hists_3[nfile].SetLineColor(color[nfile])
	hists_3[nfile].SetMarkerColor(color[nfile])
	hists_3[nfile].SetMarkerStyle(maker[nfile])

	hists_4[nfile].SetLineColor(color[nfile])
	hists_4[nfile].SetMarkerColor(color[nfile])
	hists_4[nfile].SetMarkerStyle(maker[nfile])
	
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

pic_name = "MMC_h2mass_normalizedto300lumi"	
x_title = "M_{H} [GeV]"
x_bins = "(80,200,1000)"
draw1D_v2(filelist,x_bins,x_title,cut,benchmarks,Events, 300, False, pic_name)	
pic_name = "MMC_h2mass_normalizedto3000lumi"	
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

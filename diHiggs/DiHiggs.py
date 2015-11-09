import random
import ROOT
import os
from array import *

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


#
def validateHhh(file,dir,pic_name):
    c1 = ROOT.TCanvas()
    f = ROOT.TFile(file)
    t = f.Get(dir)

    Hhhbaselinecut = "h2tohh && hasb1jet && hasb2jet &&hasMET && hasdRljet && hastwomuons"
    h1 = ROOT.TH1F("h1","h1",80,300,380)
    t.Draw("h2tohh_mass>>h1",Hhhbaselinecut) 
    h1.GetXaxis().SetTitle("M_{H}")
    h1.SetLineColor(ROOT.kBlue)
    
    h2 = ROOT.TH1F("h2","h2",40,120,130)
    t.Draw("htoWW_mass>>h2",Hhhbaselinecut) 
    h2.GetXaxis().SetTitle("M_{h#rightarrow WW}")
    h2.SetLineColor(ROOT.kBlue)

    h3 = ROOT.TH1F("h3","h3",40,120,130)
    t.Draw("htobb_mass>>h3",Hhhbaselinecut) 
    h3.GetXaxis().SetTitle("M_{h#rightarrow bb}")
    h3.SetLineColor(ROOT.kBlue)
    
    h12 = ROOT.TH2F("h12","h12",50,50,100,50,10,60)
    t.Draw("(w1_mass*(w1_mass>w2_mass)+w2_mass*(w1_mass<w2_mass)):(w1_mass*(w1_mass<w2_mass)+w2_mass*(w1_mass>w2_mass))>>h12",Hhhbaselinecut)
    h12.GetXaxis().SetTitle("M_{W}^{onshell}")
    h12.GetYaxis().SetTitle("M_{W}^{offshell}")
    
   
    h41 = ROOT.TH1F("h41","h41",60,0,120)
    #t.Draw("(mu1_pt*(w1_mass>w2_mass)+mu2_pt*(w1_mass<w2_mass))>>h41",Hhhbaselinecut) 
    t.Draw("mu1_pt>>h41",Hhhbaselinecut) 
    h42 = ROOT.TH1F("h42","h42",60,0,120)
    t.Draw("Muon1_pt>>h42",Hhhbaselinecut) 
    h41.GetXaxis().SetTitle("p_{T} of #mu^{-}")
    h42.GetXaxis().SetTitle("p_{T} of #mu^{-}")
    h41.SetLineColor(ROOT.kBlue)
    h42.SetLineColor(ROOT.kRed)
    
    h51 = ROOT.TH1F("h51","h51",60,0,120)
    #t.Draw("(mu1_pt*(w1_mass<w2_mass)+mu2_pt*(w1_mass>w2_mass))>>h51",Hhhbaselinecut) 
    t.Draw("mu2_pt>>h51",Hhhbaselinecut) 
    h52 = ROOT.TH1F("h52","h52",60,0,120)
    #t.Draw("(Muon1_pt*(w1_mass<w2_mass)+Muon2_pt*(w1_mass>w2_mass))>>h52",Hhhbaselinecut) 
    t.Draw("Muon2_pt>>h52",Hhhbaselinecut) 
    #h51.GetXaxis().SetTitle("p_{T} of #mu from offshellW")
    #h52.GetXaxis().SetTitle("p_{T} of #mu from offshellW")
    h51.GetXaxis().SetTitle("p_{T} of #mu^{+}")
    h52.GetXaxis().SetTitle("p_{T} of #mu^{+}")
    h51.SetLineColor(ROOT.kBlue)
    h52.SetLineColor(ROOT.kRed)
    
    h6 = ROOT.TH1F("h6","h6",60,0,120)
    t.Draw("(nu1_pt*(w1_mass>w2_mass)+nu2_pt*(w1_mass<w2_mass))>>h6",Hhhbaselinecut) 
    h6.GetXaxis().SetTitle("p_{T} of #nu from onshellW")
    h6.SetLineColor(ROOT.kBlue)

    h7 = ROOT.TH1F("h7","h7",60,0,120)
    t.Draw("(nu1_pt*(w1_mass<w2_mass)+nu2_pt*(w1_mass>w2_mass))>>h7",Hhhbaselinecut) 
    h7.GetXaxis().SetTitle("p_{T} of #nu from offshellW")
    h7.SetLineColor(ROOT.kBlue)
   
    h81 = ROOT.TH1F("h81","h81",80,0,200)
    t.Draw("(b1_pt*(b1jet_pt>b2jet_pt)+b2_pt*(b1jet_pt<b2jet_pt))>>h81",Hhhbaselinecut) 
    h82 = ROOT.TH1F("h42","h42",80,0,200)
    t.Draw("(b1jet_pt*(b1jet_pt>b2jet_pt)+b2jet_pt*(b1jet_pt<b2jet_pt))>>h82",Hhhbaselinecut) 
    h81.GetXaxis().SetTitle("p_{T}^{b} with larger p_{T}")
    h82.GetXaxis().SetTitle("p_{T}^{b} with larger p_{T}")
    h81.SetLineColor(ROOT.kBlue)
    h82.SetLineColor(ROOT.kRed)
    
    h91 = ROOT.TH1F("h91","h91",80,0,200)
    t.Draw("(b1_pt*(b1jet_pt<b2jet_pt)+b2_pt*(b1jet_pt>b2jet_pt))>>h91",Hhhbaselinecut) 
    h92 = ROOT.TH1F("h92","h92",80,0,200)
    t.Draw("(b1jet_pt*(b1jet_pt<b2jet_pt)+b2jet_pt*(b1jet_pt>b2jet_pt))>>h92",Hhhbaselinecut) 
    h91.GetXaxis().SetTitle("p_{T}^{b} with smaller p_{T}")
    h92.GetXaxis().SetTitle("p_{T}^{b} with smaller p_{T}")
    h91.SetLineColor(ROOT.kBlue)
    h92.SetLineColor(ROOT.kRed)
   
    
    h101 = ROOT.TH1F("h101","h101",80,0,200)
    t.Draw("(b1_energy*(b1jet_pt>b2jet_pt)+b2_energy*(b1jet_pt<b2jet_pt))>>h101",Hhhbaselinecut) 
    h102 = ROOT.TH1F("h42","h42",80,0,200)
    t.Draw("(b1jet_energy*(b1jet_pt>b2jet_pt)+b2jet_energy*(b1jet_pt<b2jet_pt))>>h102",Hhhbaselinecut) 
    h101.GetXaxis().SetTitle("E^{b} with larger p_{T}")
    h102.GetXaxis().SetTitle("E^{b} with larger p_{T}")
    h101.SetLineColor(ROOT.kBlue)
    h102.SetLineColor(ROOT.kRed)

    h111 = ROOT.TH1F("h101","h101",80,0,200)
    t.Draw("(b1_energy*(b1jet_pt<b2jet_pt)+b2_energy*(b1jet_pt>b2jet_pt))>>h111",Hhhbaselinecut) 
    h112 = ROOT.TH1F("h42","h42",80,0,200)
    t.Draw("(b1jet_energy*(b1jet_pt<b2jet_pt)+b2jet_energy*(b1jet_pt>b2jet_pt))>>h112",Hhhbaselinecut) 
    h111.GetXaxis().SetTitle("E^{b} with smaller p_{T}")
    h112.GetXaxis().SetTitle("E^{b} with smaller p_{T}")
    h111.SetLineColor(ROOT.kBlue)
    h112.SetLineColor(ROOT.kRed)

    c1.Clear()
    c1.Divide(3,4)
    c1.cd(1)
    h1.Draw()
   
    c1.cd(2)
    h2.Draw()
   
    c1.cd(3)
    h3.Draw()
   
    c1.cd(4)
    h12.Draw("colz")
   
    c1.cd(5)
    h41.Draw()
    h42.Draw("same")
    
    c1.cd(6)
    h51.Draw()
    h52.Draw("same")
   
    c1.cd(7)
    h6.Draw()
   
    c1.cd(8)
    h7.Draw()
   
    c1.cd(9)
    h81.Draw()
    h82.Draw("same")
    
    c1.cd(10)
    h91.Draw()
    h92.Draw("same")

    c1.cd(11)
    h101.Draw()
    h102.Draw("same")

    c1.cd(12)
    h111.Draw()
    h112.Draw("same")

    c1.cd() 
    #c1.SaveAs("Hhh_validation_%s_3x4.pdf"%pic_name)
    c1.SaveAs("Hhh_validation_%s_3x4.pdf"%pic_name)


#
def validatettbar(file,dir,pic_name):
    c1 = ROOT.TCanvas()
    f = ROOT.TFile(file)
    t = f.Get(dir)

    ttbaselinecut = "hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons && sqrt((mu1_eta-mu2_eta)**2+TVector2::Phi_mpi_pi(mu1_phi-mu2_phi)**2)<2.5"
    h1 = ROOT.TH1F("h1","h1",50,150,200)
    t.Draw("t1_mass>>h1",ttbaselinecut) 
    h1.GetXaxis().SetTitle("M_{t}")
    h1.SetLineColor(ROOT.kBlue)
    
    h2 = ROOT.TH1F("h2","h2",50,150,200)
    t.Draw("t2_mass>>h2",ttbaselinecut) 
    h2.GetXaxis().SetTitle("M_{#bar{t}}")
    h2.SetLineColor(ROOT.kBlue)

    h3 = ROOT.TH1F("h3","h3",40,60,100)
    t.Draw("w2_mass>>h3",ttbaselinecut) 
    h3.GetXaxis().SetTitle("M_{W^{-}}")
    h3.SetLineColor(ROOT.kBlue)
    
    h12 = ROOT.TH1F("h12","h12",40,60,100)
    t.Draw("w1_mass>>h12",ttbaselinecut) 
    h12.GetXaxis().SetTitle("M_{W^{+}}")
    h12.SetLineColor(ROOT.kBlue)
    
   
    h41 = ROOT.TH1F("h41","h41",60,0,120)
    t.Draw("mu1_pt>>h41",ttbaselinecut) 
    h42 = ROOT.TH1F("h42","h42",60,0,120)
    t.Draw("Muon1_pt>>h42",ttbaselinecut) 
    h41.GetXaxis().SetTitle("p_{T} of #mu^{-}")
    h42.GetXaxis().SetTitle("p_{T} of #mu^{-}")
    h41.SetLineColor(ROOT.kBlue)
    h42.SetLineColor(ROOT.kRed)
    
    h51 = ROOT.TH1F("h51","h51",60,0,120)
    t.Draw("mu2_pt>>h51",ttbaselinecut) 
    h52 = ROOT.TH1F("h52","h52",60,0,120)
    t.Draw("Muon2_pt>>h52",ttbaselinecut) 
    h51.GetXaxis().SetTitle("p_{T} of #mu^{+}")
    h52.GetXaxis().SetTitle("p_{T} of #mu^{+}")
    h51.SetLineColor(ROOT.kBlue)
    h52.SetLineColor(ROOT.kRed)
    
    h6 = ROOT.TH1F("h6","h6",60,0,120)
    t.Draw("(nu1_pt*(w1_mass>w2_mass)+nu2_pt*(w1_mass<w2_mass))>>h6",ttbaselinecut) 
    h6.GetXaxis().SetTitle("p_{T} of #nu from onshellW")
    h6.SetLineColor(ROOT.kBlue)

    h7 = ROOT.TH1F("h7","h7",60,0,120)
    t.Draw("(nu1_pt*(w1_mass<w2_mass)+nu2_pt*(w1_mass>w2_mass))>>h7",ttbaselinecut) 
    h7.GetXaxis().SetTitle("p_{T} of #nu from offshellW")
    h7.SetLineColor(ROOT.kBlue)
   
    h81 = ROOT.TH1F("h81","h81",80,0,200)
    t.Draw("(b1_pt*(b1jet_pt>b2jet_pt)+b2_pt*(b1jet_pt<b2jet_pt))>>h81",ttbaselinecut) 
    h82 = ROOT.TH1F("h42","h42",80,0,200)
    t.Draw("(b1jet_pt*(b1jet_pt>b2jet_pt)+b2jet_pt*(b1jet_pt<b2jet_pt))>>h82",ttbaselinecut) 
    h81.GetXaxis().SetTitle("p_{T}^{b} with larger p_{T}")
    h82.GetXaxis().SetTitle("p_{T}^{b} with larger p_{T}")
    h81.SetLineColor(ROOT.kBlue)
    h82.SetLineColor(ROOT.kRed)
    
    h91 = ROOT.TH1F("h91","h91",80,0,200)
    t.Draw("(b1_pt*(b1jet_pt<b2jet_pt)+b2_pt*(b1jet_pt>b2jet_pt))>>h91",ttbaselinecut) 
    h92 = ROOT.TH1F("h92","h92",80,0,200)
    t.Draw("(b1jet_pt*(b1jet_pt<b2jet_pt)+b2jet_pt*(b1jet_pt>b2jet_pt))>>h92",ttbaselinecut) 
    h91.GetXaxis().SetTitle("p_{T}^{b} with smaller p_{T}")
    h92.GetXaxis().SetTitle("p_{T}^{b} with smaller p_{T}")
    h91.SetLineColor(ROOT.kBlue)
    h92.SetLineColor(ROOT.kRed)
   
    
    h101 = ROOT.TH1F("h101","h101",80,0,200)
    t.Draw("(b1_energy*(b1jet_pt>b2jet_pt)+b2_energy*(b1jet_pt<b2jet_pt))>>h101",ttbaselinecut) 
    h102 = ROOT.TH1F("h42","h42",80,0,200)
    t.Draw("(b1jet_energy*(b1jet_pt>b2jet_pt)+b2jet_energy*(b1jet_pt<b2jet_pt))>>h102",ttbaselinecut) 
    h101.GetXaxis().SetTitle("E^{b} with larger p_{T}")
    h102.GetXaxis().SetTitle("E^{b} with larger p_{T}")
    h101.SetLineColor(ROOT.kBlue)
    h102.SetLineColor(ROOT.kRed)

    h111 = ROOT.TH1F("h101","h101",80,0,200)
    t.Draw("(b1_energy*(b1jet_pt<b2jet_pt)+b2_energy*(b1jet_pt>b2jet_pt))>>h111",ttbaselinecut) 
    h112 = ROOT.TH1F("h42","h42",80,0,200)
    t.Draw("(b1jet_energy*(b1jet_pt<b2jet_pt)+b2jet_energy*(b1jet_pt>b2jet_pt))>>h112",ttbaselinecut) 
    h111.GetXaxis().SetTitle("E^{b} with smaller p_{T}")
    h112.GetXaxis().SetTitle("E^{b} with smaller p_{T}")
    h111.SetLineColor(ROOT.kBlue)
    h112.SetLineColor(ROOT.kRed)
   
    c1.Clear()
    c1.Divide(3,4)
    c1.cd(1)
    h1.Draw()
   
    c1.cd(2)
    h2.Draw()
   
    c1.cd(3)
    h3.Draw()
   
    c1.cd(4)
    h12.Draw()

    c1.cd(5)
    h41.Draw()
    h42.Draw("same")
    
    c1.cd(6)
    h51.Draw()
    h52.Draw("same")
   
    c1.cd(7)
    h6.Draw()
   
    c1.cd(8)
    h7.Draw()
   
    c1.cd(9)
    h81.Draw()
    h82.Draw("same")
    
    c1.cd(10)
    h91.Draw()
    h92.Draw("same")


    c1.cd(11)
    h101.Draw()
    h102.Draw("same")

    c1.cd(12)
    h111.Draw()
    h112.Draw("same")
      
    c1.cd() 
    c1.SaveAs("ttbar_validation_%s_3x4.pdf"%pic_name)

#___________________________________________
def draw1D(file,dir,todraw,x_bins,x_title,cut,pic_name, rand):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    
    
    b = ROOT.TH1F("b","b",xBins,xminBin,xmaxBin)
    b.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b.GetYaxis().SetTitle("Events")
    b.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)

    b.Sumw2() 
    t.Draw(todraw+">>b",cut)
    #b1.Draw()
    print " b entries in draw1D ",b.GetEntries()
    #c1.SaveAs("Dihiggs_%s_%d"%(pic_name, rand)+"_B3.pdf")
    #c1.SaveAs("Dihiggs_%s_%d"%(pic_name, rand)+"_B3.png")
    return b	


#____________________________________________________________________
def draw2D(file,dir,num,xaxis,yaxis,x_bins,y_bins):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH2F("b1","b1",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b1.GetYaxis().SetTitle("%s"%yaxis)
    b1.GetXaxis().SetTitle("%s"%xaxis)
    b1.SetTitle("h1#rightarrow BB or WW, B3"+" "*12 + "CMS Simulation Preliminary")
   # b1.SetStats(1)
    todraw = "(%s)"%yaxis+":"+"(%s)>>b1"%xaxis
    t.Draw(todraw,num,"colz")
#    b1.SetMaximum(150)
    b1.Draw("colz")
    ROOT.gPad.SetLogz() 
    legend = ROOT.TLegend(0.15,0.56,0.40,0.64)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetHeader("PU140")
#legend.AddEntry(e1,"","l")
# legend.Draw("same")
    Num = b1.Integral(1,xBins,1,yBins)
    print "number of event ", Num

    tex = ROOT.TLatex(0.15,0.30,"#splitline{p_{T}^{sim}>20GeV,#frac{(pt-trackpt)}{pt}<-0.5}{stubs in TF matcehd to simtracks, Entries %s}"%Num)
#    tex = ROOT.TLatex(0.15,0.30,"p_{T}^{sim}>20GeV, #frac{abs(pt-trackpt)}{pt}<0.5, Entries %s"%Num)
#    tex = ROOT.TLatex(0.20,0.30,"#frac{(pt-trackpt)}{pt}>0.5, Entries %s"%Num)
#    tex = ROOT.TLatex(.2,0.3,"all stubs in TF matched to simtrack ")
    tex.SetTextSize(0.05)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
	
    c1.SaveAs("Dihiggs_%s"%xaxis+"_VS_%s.pdf"%yaxis)
    c1.SaveAs("Dihiggs_%s"%xaxis+"_VS_%s.png"%yaxis)



#___________________________________________
def draw1D_combined(file1,file2,dir1,dir2,todraw,x_bins,x_title,cut1,cut2,leg1,leg2, pic_name):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f1 = ROOT.TFile(file1)#ttbar
    t1 = f1.Get(dir1)
    f2 = ROOT.TFile(file2)#H->hh
    t2 = f2.Get(dir2)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    
    hs = ROOT.THStack("hs","%s distribution for Hhh and t#bar{t},NOT normalized "%x_title)
    b2 = ROOT.TH1F("b2","b2",xBins,xminBin,xmaxBin)
    b2.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b2.GetYaxis().SetTitle("Events")
    b2.GetXaxis().SetTitle("%s"%x_title)
    t2.Draw(todraw+">>b2",cut2)
    #b1.SetStats(0)
    #wmassout = ROOT.TFile("onshellwmassout.root","recreate")
    #wmassout.cd()
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)


    #ROOT.gStyle.SetOptFit(1)
    t1.Draw(todraw+">>b1",cut1)
    """
    i = 0
    while i<10:
	r = random.uniform(50,90)
	bin = b1.FindBin(r)
	print "r ",r ," bin ",bin," bincenter1 ",b1.GetBinCenter(bin)," center2 ",b1.GetBinCenter(bin+1)
	i = i+1
    legend = ROOT.TLegend(0.15,0.46,0.45,0.64)
    legend.SetFillColor(ROOT.kWhite)
    #b1.Draw()
    """
    b1.SetLineColor(ROOT.kBlack)
    b1.SetLineStyle(1)
    b1.SetLineWidth(2)
    b2.SetLineColor(ROOT.kRed)
    b2.SetLineStyle(2)
    b2.SetLineWidth(2)
    b1.Scale(1/4.0)
    hs.Add(b1)
    hs.Add(b2)
    #b2.Draw("same")
    hs.Draw("nostack")
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend = ROOT.TLegend(0.65,0.80,0.8,0.94)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    legend.AddEntry(b1,"%s"%leg1,"pl") 
    legentry = legend.AddEntry(b2,"%s"%(leg2),"pl") 
    legentry.SetTextColor(ROOT.kRed)
    legend.Draw("same")
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    #wmassout.Write()
    #wmassout.Close()
    
    c1.SaveAs("Dihiggs_cleaningcuts_%s"%pic_name+"_combined_ttdrll_B3.pdf")
    c1.SaveAs("Dihiggs_cleaningcuts_%s"%pic_name+"_combined_ttdrll_B3.png")
#    c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.ROOT")


#___________________________________________
def draw2D_combined(file,dir,todraw1,todraw2,x_bins,y_bins,x_title,y_title,cut1,cut2, pic_name):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f = ROOT.TFile(file)
    t = f.Get(dir)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    
    b2 = ROOT.TH2F("b2","b2",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b2.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b2.GetYaxis().SetTitle("Events")
    b2.GetXaxis().SetTitle("%s"%x_title)
    t.Draw(todraw2+">>b2",cut2)
    #b1.SetStats(0)
#    wmassout = ROOT.TFile("onshellwmassout.root","recreate")
#    wmassout.cd()
    b1 = ROOT.TH2F("b1","b1",xBins,xminBin,xmaxBin,yBins,yminBin,ymaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("%s"%y_title)
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)


    #ROOT.gStyle.SetOptFit(1)
    t.Draw(todraw1+">>b1",cut1)
    mean = 80.1
    signma = 1.5
    b1.Add(b2)
    #b1.Sumw2()
   # b1.Fit("myfun")
    #st =ROOT.TPaveStats(b1.FindObject("stats"))
    #st.SetX1NDC(0.1)
    #st.SetX2NCD(0.4)
    """
    i = 0
    while i<10:
	r = random.uniform(50,90)
	bin = b1.FindBin(r)
	print "r ",r ," bin ",bin," bincenter1 ",b1.GetBinCenter(bin)," center2 ",b1.GetBinCenter(bin+1)
	i = i+1
    legend = ROOT.TLegend(0.15,0.46,0.45,0.64)
    legend.SetFillColor(ROOT.kWhite)
    #b1.Draw()
    """
    #b2.Draw("same")
    b1.Draw("colz")
 #   wmassout.Write()
 #   wmassout.Close()
    
    #c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.pdf")
    #c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.png")
    #c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.C")
#    c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.ROOT")






#_____________________________________________________________________________
def drawAll_1D(dir,  treename, todraw,x_bins,x_title,cut,pic_name, text):
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ",dir
    ls = os.listdir(dir)
    tot = len(ls)
    rootfile = dir[:]+ls[0]
    tfile0 = ROOT.TFile(rootfile)
    t = tfile0.Get(treename)
    m = 0
    chain = ROOT.TChain(treename)
    e = ROOT.TH1F("e","e",xBins,xminBin,xmaxBin)
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    for x in ls:
	x = dir[:]+x
	chain.Add(x)
    chain.Draw(todraw+">>b1", cut)
   # c1.SetLogy()
    #b1.GetXaxis().SetTitleOffset(0.5)
    b1.GetXaxis().SetTitleSize(0.04)
    b1.Draw() 
    print "chain ",chain, " b1 entries ",b1.GetEntries()
    #print "GetMaximumbin() ", b1.GetMaximumBin()," bincenter ",b1.GetBinCenter(b1.GetMaximumBin())
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")

    c1.SaveAs("Dihiggs_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_All_B3.png")

#_____________________________________________________________________________
def drawAll_combined1D(dir, treename, todraw, truetodraw, x_bins,x_title,cut,pic_name, text):
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ", dir
    ls = os.listdir(dir)
    tot = len(ls)
    rootfile = dir[:]+ls[0]
#    ROOT.gStyle.SetOptStat()
    tfile0 = ROOT.TFile(rootfile)
    t = tfile0.Get(treename)
    m = 0
    acceptcut = "h2tohh && dR_bjet<0.1 && dR_bbarjet<0.1 && hasMET && hasbjet && hasbbarjet && hastwomuons"
    chain = ROOT.TChain(treename)
    e1 = ROOT.TH1F("e1","e1",xBins,xminBin,xmaxBin)
    e2 = ROOT.TH1F("e2","e2",xBins,xminBin,xmaxBin)
    e = ROOT.TH1F("e","e",xBins,xminBin,xmaxBin)
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)
    for x in ls:
	x = dir[:]+x
	chain.Add(x)
    chain.Draw(todraw+">>b1", cut)
    chain.Draw(truetodraw+">>e", cut)
    chain.Draw(truetodraw+">>e1", acceptcut)

    totinput = e1.GetEntries()+e1.GetBinContent(0)+e1.GetBinContent(xBins+1)
    num_MMC = b1.GetEntries() 
    rms = b1.GetRMS()
    value = e.GetXaxis().GetBinCenter(e.GetMaximumBin()) 
    xlow = b1.FindBin(value-10)
    xhigh = b1.FindBin(value+10)
    num_middle = b1.Integral(xlow,xhigh)
    truenum_middle = e.Integral(xlow,xhigh)
    x = xlow
    while x<xhigh:
	e2.SetBinContent(x, e.GetMaximum()+10)
	x = x+1
    e2.SetFillColorAlpha(46,0.3)
    e2.SetLineColorAlpha(46,0.3)
#    e2.SetFillStyle(3001)
    

    print " totinput ", totinput," e1 entries ", e1.GetEntries()," num_MMC ",num_MMC," rms ",rms," e1 peak ", value
    print " xlow ",xlow, " xhigh ", xhigh," num_middle ", num_middle," e2 entries ",e2.GetEntries()
   # c1.SetLogy()
    
    #b1.GetXaxis().SetTitleOffset(0.6)
    #b1GetXaxis().SetTitleSize(0.08)
    b1.Draw() 
    e2.Draw("histsame")
    e.Draw("same")
    e.SetLineColor(ROOT.kRed)
    e.SetLineWidth(2)
    b1.SetLineWidth(4)
    b1.GetYaxis().SetRangeUser(0,e.GetMaximum()+10)
    #legend = ROOT.TLegend(0.25,0.7,0.45,0.82)
    legend = ROOT.TLegend(0.15,0.82,0.65,0.94)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(42)
    legend.AddEntry(b1,"Reconstructed ","pl") 
    legentry = legend.AddEntry(e,"True,entries=%d peak=%d"%(totinput, value),"pl") 
    legentry.SetTextColor(ROOT.kRed)
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    #legend.Draw("same")
    #print "chain ",chain, " b1 entries ",b1.GetEntries()
    #print "GetMaximumbin() ", b1.GetMaximumBin()," bincenter ",b1.GetBinCenter(b1.GetMaximumBin())
    tex = ROOT.TLatex(0.15,0.7,text)
    tex.SetTextSize(0.05)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
    legend.Draw("same")
    c1.Update()
    #print "stats obj ", b1.GetListOfFunctions().FindObject("stats")
    #statbox = b1.FindObject("stats")
	
    #print "statbox ",statbox
    statbox = ROOT.TPaveText()
    #statbox.SetName("MMC summary");
     
    statbox.SetX1NDC(0.65)
    statbox.SetX2NDC(0.85)
    statbox.SetY1NDC(0.3)
    statbox.SetY2NDC(0.80)
    #statbox.Draw("same")
    #statlist = statbox.GetListOfLines();
    #print " statlist ", statlist
    #pstxt0 = ROOT.TLatex(0,0,"MMC results")
    #pstxt1 = ROOT.TLatex(0,0,"peak+[-10,10] = %d"%num_middle)
    #pstxt1.SetTextSize(0.03)
    #pstxt2 = ROOT.TLatex(0,0,"true peak = %d"%value)
    #pstxt2.SetTextSize(0.03)
    statbox.AddText("MMC Results")
    statbox.AddText("Entries  %d"%b1.GetEntries())
    statbox.AddText("Mean  %d"%b1.GetMean())
    statbox.AddText("RMS  %d"%b1.GetRMS())
    statbox.AddText("underflow %d"%b1.GetBinContent(0))
    statbox.AddText("overflow  %d"%b1.GetBinContent(xBins+1))
    peakwindow0 =statbox.AddText("entries in window")
    peakwindow1 = statbox.AddText("MMC  %d"%num_middle)
    peakwindow2 = statbox.AddText("True  %d"%truenum_middle)
    peakwindow0.SetTextColor(46)
    peakwindow1.SetTextColor(46)
    peakwindow2.SetTextColor(46)

    #tconst = statbox.GetLineWith("RMS");
    b1.SetStats(0)
    statbox.SetTextSize(0.04)
    peakwindow0.SetTextSize(0.035)
    statbox.SetFillColor(ROOT.kWhite)
    statbox.SetFillStyle(0)
    statbox.Draw("same")
    c1.Modified()

    c1.SaveAs("Dihiggs_combined_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("Dihiggs_combined_%s"%pic_name+"_All_B3.png")




#_____________________________________________________________________________
def drawAll_2D(dir, treename, todraw,x_bins,y_bins, x_title, y_title,cut,pic_name, text):

    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    
    b1 = ROOT.TH2F("b1","b1",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    ROOT.SetOwnership(b1, False)
    b1.SetTitle("h2#rightarrow bbWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("%s"%y_title)
    b1.GetXaxis().SetTitle("%s"%x_title)
    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ", dir
    ls = os.listdir(dir)
    tot = len(ls)
    
    chain = ROOT.TChain(treename)
    for x in ls:
	x = dir[:]+x
	chain.Add(x)
    chain.Draw(todraw+">>b1",cut,"colz")
    b1.Draw("colz")
    #ROOT.gPad.SetLogz()

    
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
     
    c1.SaveAs("Dihiggs_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_All_B3.png")
    return b1 

#_____________________________________________________________________________
def drawAllEff_2D(dir, treename, todraw,x_bins,y_bins, x_title, y_title,dencut, numcut,pic_name, text):


    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])
    
    h1 = ROOT.TH2F("h1","h1",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    h2 = ROOT.TH2F("h2","h2",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    h3 = ROOT.TH2F("h3","h3",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    h4 = ROOT.TH2F("h4","h4",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)

    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ", dir
    ls = os.listdir(dir)
    tot = len(ls)
    
    chain = ROOT.TChain(treename)
    for x in ls:
	x = dir[:]+x
	chain.Add(x)
    chain.Draw("b1jet_pt:abs(b1jet_eta)>>h1","h2tohh &&hasb1jet && b1jet_btag>0")
    chain.Draw("b1jet_pt:abs(b1jet_eta)>>h2","h2tohh &&hasb1jet")
    chain.Draw("b2jet_pt:abs(b2jet_eta)>>h3","h2tohh &&hasb2jet && b2jet_btag>0")
    chain.Draw("b2jet_pt:abs(b2jet_eta)>>h4","h2tohh &&hasb2jet")
    
    h1.Add(h3)
    h2.Add(h4) 


    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()
    Teff1 = ROOT.TEfficiency(h1, h2)
    bg = ROOT.TH2F("bg","bg",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    bg.SetTitle("h2#rightarrow bbWW, B3"+" "*26 + " 14TeV, PU0")
    bg.GetYaxis().SetTitle("%s"%y_title)
    bg.GetXaxis().SetTitle("%s"%x_title)
    bg.GetZaxis().SetRangeUser(0,1.0)
    bg.SetMaximum(1)
    bg.SetMinimum(0)
    bg.Draw()

    Teff1.Draw("samecolz") 
    ROOT.gStyle.SetOptStat(0)
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
    
    c1.SaveAs("Dihiggs_Eff_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("Dihiggs_Eff_%s"%pic_name+"_All_B3.png")


#_____________________________________________________________________________
def drawAllEffTTbar_2D(dir, treename, todraw,x_bins,y_bins, x_title, y_title,dencut, numcut,pic_name, text):


    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    yBins = int(y_bins[1:-1].split(',')[0])
    yminBin = float(y_bins[1:-1].split(',')[1])
    ymaxBin = float(y_bins[1:-1].split(',')[2])

    
    h1 = ROOT.TH2F("h1","h1",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    h2 = ROOT.TH2F("h2","h2",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    h3 = ROOT.TH2F("h3","h3",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    h4 = ROOT.TH2F("h4","h4",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)

    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ", dir
    ls = os.listdir(dir)
    tot = len(ls)
    
    chain = ROOT.TChain(treename)
    for x in ls:
	x = dir[:]+x
	chain.Add(x)
    chain.Draw("b1jet_pt:abs(b1jet_eta)>>h1","ttbar &&hasb1jet && b1jet_btag")
    chain.Draw("b1jet_pt:abs(b1jet_eta)>>h2","ttbar &&hasb1jet")
    chain.Draw("b2jet_pt:abs(b2jet_eta)>>h3","ttbar &&hasb2jet && b2jet_btag")
    chain.Draw("b2jet_pt:abs(b2jet_eta)>>h4","ttbar &&hasb2jet")
    
    tfile = ROOT.TFile("TTbar_testv2.root","RECREATE")
    h1.Add(h3)
    h2.Add(h4) 
    
    effbins = array("d",[0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0])
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()
    h1.Divide(h2)
    Teff1 = ROOT.TEfficiency(h1, h2)
   
    bg = ROOT.TH2F("bg","bg",xBins,xminBin,xmaxBin, yBins,yminBin,ymaxBin)
    bg.SetTitle("tt#rightarrow bbWW"+" "*28 + " 14TeV, PU0")
    bg.GetYaxis().SetTitle("%s"%y_title)
    bg.GetXaxis().SetTitle("%s"%x_title)
    #bg.GetZaxis().SetRangeUser(0,1.0)
    #bg.SetMaximum(1)
    #bg.SetMinimum(0)
    #Teff1.SetContour(len(effbins), effbins)
    bg.Draw()

    #h1.SetMaximum(1)
    #h1.SetMinimum(0)
    h1.Draw("samecolz") 
    #Teff1.Draw("samecolz") 
    ROOT.gStyle.SetOptStat(0)
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
    bg.Write()
    Teff1.Write()
    h1.Write()
    
    tfile.Close()
    
    c1.SaveAs("TTbar_Eff_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("TTbar_Eff_%s"%pic_name+"_All_B3.png")
    

#______________________________________________________________________________
def buildTChain(dir,treename, rootfilename):

    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ", dir
    ls = os.listdir(dir)
    tot = len(ls)
    
    chain = ROOT.TChain(treename)
   
    for x in ls:
	x = dir[:]+x
	chain.Add(x)
    file = ROOT.TFile(rootfilename,"recreate") 
    chain.Write()
    file.Write()
    file.Close()


#_______________________________________________________________________________
if __name__ == "__main__":
     
    #file = "/fdata/hepx/store/user/taohuang/Hhh/DiHiggs-1M-B3-1071409.root"
    #file = "/fdata/hepx/store/user/taohuang/Hhh/DiHiggs_100k_correctnu_0324_B3.root"
    #file1 = "./workroot/tt2WWbbAna_all_notau_0807_B3.root"
    #file2 = "./workroot/htoWWAna_1M_notau_0807_B3.root"
    #file1 = "./workroot/tt2WWbbAna_all_includetau_0807_B3.root"
    #file2 = "./workroot/htoWWAna_1M_Wtotau_0807_B3.root"
    file1 = "TTbar_WWbb_1M_PU0_leptonW_7570700_0928.root"
    file2 = "DiHiggs_WWbb_1M_PU0_leptonW_6969448_B3_0928.root"
    dir1 = "evtree"
    dir2 = "evtree"
    filedir = "/fdata/hepx/store/user/taohuang/Hhh/Delphes_split_mediumbtagEffv3/"
    
    
    htoWW_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz)**2)"
    
    
    htoWW_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz)**2)"
    hmass_bins = "(50,100,300)"
    hmass_bins1 = "(100,80,150)" 
    hmass_bins2 = "(50,0,200)" 
    htoWW_cut = "h2tohh" 
    hasjets_cut = "hasbjet && hasbbarjet"
    
    bjet_pt = "sqrt(bjet_px**2+bjet_py**2)" 
    bbarjet_pt = "sqrt(bbarjet_px**2+bbarjet_py**2)" 
    htoBB_mass = "sqrt((b1_energy+b2_energy)**2-(b1_px+b2_px)**2-(b1_py+b2_py)**2-(b1_pz+b2_pz)**2)"
    htoBBJets_mass = "sqrt((bjet_energy+bbarjet_energy)**2-(bjet_px+bbarjet_px)**2-(bjet_py+bbarjet_py)**2-(bjet_pz+bbarjet_pz)**2)"
    htoBBJets_correction_mass = "sqrt(((bjet_energy+bbarjet_energy)*rescalefactor)**2-((bjet_px+bbarjet_px)*rescalefactor)**2-((bjet_py+bbarjet_py)*rescalefactor)**2-((bjet_pz+bbarjet_pz)*rescalefactor)**2)"
    htoBBJetstot_mass = "sqrt((bjet_energy_tot+bbarjet_energy_tot)**2-(bjet_px_tot+bbarjet_px_tot)**2-(bjet_py_tot+bbarjet_py_tot)**2-(bjet_pz_tot+bbarjet_pz_tot)**2)"
    htoBB_cut = "h2tohh"

    nu12_px = "(nu1_px+nu2_px)"
    nu12_py = "(nu1_py+nu2_py)"
    bjets_nu_px = "(bjet_nu_px+bbarjet_nu_px)"
    bjets_nu_py = "(bjet_nu_py+bbarjet_nu_py)"
    bjets_nu_pz = "(bjet_nu_pz+bbarjet_nu_pz)"
    bjets_rescale_px = "(bjet_px+bbarjet_px)*(rescalefactor-1)"
    bjets_rescale_py = "(bjet_py+bbarjet_py)*(rescalefactor-1)"
    bjets_rescale_pz = "(bjet_pz+bbarjet_pz)*(rescalefactor-1)"
    nu12_bjetsnu_px = "(nu1_px+nu2_px+bjet_nu_px+bbarjet_nu_px)"
    nu12_bjetsnu_py = "(nu1_py+nu2_py+bjet_nu_py+bbarjet_nu_py)"
    nu12_pt = "sqrt((nu1_px+nu2_px)**2+(nu1_py+nu2_py)**2)"
    sum1_px = "(nu1_px+nu2_px+mu1_px+mu2_px+(bjet_px+bbarjet_px)*rescalefactor)"
    sum1_py = "(nu1_py+nu2_py+mu1_py+mu2_py+(bjet_py+bbarjet_py)*rescalefactor)"
    sum2_px = "(nu1_px+nu2_px+mu1_px+mu2_px+totjets_px)"
    sum2_py = "(nu1_py+nu2_py+mu1_py+mu2_py+totjets_py)"
    sum3_px = "(nu1_px+nu2_px-met_px)"
    sum3_py = "(nu1_py+nu2_py-met_py)"
    sum4_px = "(nu1_px+nu2_px-met_correction_px)"
    sum4_py = "(nu1_py+nu2_py-met_correction_py)"
    sum5_px = "(nu1_px+nu2_px+bjet_nu_px+bbarjet_nu_px-met_px)"
    sum5_py = "(nu1_py+nu2_py+bjet_nu_py+bbarjet_nu_py-met_py)"
    
    MissET_projectionX = "(met_px*(bjet_px+bbarjet_px)+met_py*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    MissET_projectionY = "(met_py*(bjet_px+bbarjet_px)-met_px*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    MissET_projectionX_correction = "(met_correction_px*(bjet_px+bbarjet_px)+met_correction_py*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    MissET_projectionY_correction = "(met_correction_py*(bjet_px+bbarjet_px)-met_correction_px*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    nu12_projectionX = "((nu1_px+nu2_px)*(bjet_px+bbarjet_px)+(nu1_py+nu2_py)*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    nu12_projectionY = "((nu1_py+nu2_py)*(bjet_px+bbarjet_px)-(nu1_px+nu2_px)*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    
    jets_mass_bins = "(200, 100,400)"
    h2_mass_jets = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy+jets_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px+jets_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py+jets_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz+jets_pz)**2)"
    #draw1D(file,dir,h2_mass_jets,h2mass_bins_3,"invariant mass of all decendants from b+#bar{b}", htoWW_cut,"h2_mass_jets_1M_mediateStates_0325")
    met_bins = "(150,0,150)"
    met = "met"
    #draw1D(file,dir,met,met_bins,"Simulated #slash{E}_{T}", "1","MET_1M_mediateStates_0325")
    
### as a reference for monitoring plots in MMC
    wmass_offshell_bins = "(60,0.0,60.0)" 
    wmass_onshell_bins = "(50,40.0,90.0)" 
    eta_bins = "(30,-6,6)"
    nu1_eta = "nu1_eta"
    nu2_eta = "nu2_eta"

    #offshell_nupt_bins = "(25,0,100)"
    offshell_nupt_bins = "(25,0,100)"
    onshell_nupt_bins = "(25,0,125)"
    nu1_pt = "sqrt(nu1_px**2+nu1_py**2)"
    nu2_pt = "sqrt(nu2_px**2+nu2_py**2)"
    onshellW_1_cut = "mu1_mother_mass>mu2_mother_mass"
    offshellW_1_cut = "mu2_mother_mass>mu1_mother_mass"
    ttbaselinecut = "hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons && sqrt((mu1_eta-mu2_eta)**2+TVector2::Phi_mpi_pi(mu1_phi-mu2_phi)**2)<2.5"
    Hhhbaselinecut = "h2tohh && hasb1jet && hasb2jet &&hasMET && hasdRljet && hastwomuons"
    #draw1D_combined(file1,file2, dir1,dir2,"mass_l1l2","(60,0,300)","M_{ll}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_Mll_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"dR_l1l2","(45,0,4.5)","#DeltaR_{ll}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_dRll_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"mass_b1b2","(80,0,400)","M_{bjets}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_Mbjets_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"dR_b1b2","(45,0,4.5)","#DeltaR_{bjets}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_dRbjets_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"met","(80,0,200)","#slash{E}_{T}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_met_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(dphi_llbb)","(70,0,3.5)","#Delta#phi_{(ll,jj)}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_dphillbb_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(dphi_llmet)","(70,0,3.5)","#Delta#phi_{(ll,#slash{E}_{T})}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_dphillmet_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(dR_minbl)","(45,0,4.5)","min#DeltaR_{bjetl}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_dRblmin_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(pt_b1b2)","(50,0,200)","p_{T}^{ll}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_ptll_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(pt_l1l2)","(60,0,300)","p_{T}^{bjets}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_ptbjets_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(mass_trans)","(60,0,300)","Transverse Mass",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","Hhh","delphes_masstrans_reco")
    
    #validateHhh(file2,dir2,"delphes_1005")
    #validatettbar(file1,dir1,"delphes_1005")
    #drawAll_2D(filedir,"evtree", "b1jet_pt:abs(b1jet_eta)","(30,0,2.5)","(30,30.0,210.0)","#eta","p_{T}","h2tohh && hasb1jet","mediumbtagEff","") 
    dencut = "h2tohh && hasb1jet"
    numcut = dencut+"&&b1jet_btag>0"
    drawAllEff_2D(filedir,"evtree", "b1jet_pt:abs(b1jet_eta)","(25,0,2.5)","(20,30.0,160.0)","#eta","p_{T}",dencut, numcut,"mediumbtagEff","") 
    #drawAll_1D(filedir,"evtree","(dR_b1l1*(Muon1_pt>Muon2_pt && b1jet_pt>b2jet_pt)+dR_b1l2*(Muon2_pt>Muon1_pt && b1jet_pt>b2jet_pt)+dR_b2l1*(Muon1_pt>Muon2_pt && b2jet_pt>b1jet_pt) + dR_b2l2*(Muon2_pt>Muon1_pt && b2jet_pt>b1jet_pt))","(100,0,5)","#Delta R_{l,bjet}","hasdRljet && hasMET && h2tohh","dRljet_energetic_1030", "")
    filedir_ttbar = "/fdata/hepx/store/user/taohuang/Hhh/Delphes_split_mediumbtagEff_ttbarv2/"
    #drawAll_1D(filedir_ttbar,"evtree","(dR_b1l1*(Muon1_pt>Muon2_pt && b1jet_pt>b2jet_pt)+dR_b1l2*(Muon2_pt>Muon1_pt && b1jet_pt>b2jet_pt)+dR_b2l1*(Muon1_pt>Muon2_pt && b2jet_pt>b1jet_pt) + dR_b2l2*(Muon2_pt>Muon1_pt && b2jet_pt>b1jet_pt))","(100,0,5)","#Delta R_{l,bjet}","hasdRljet && hasMET && ttbar","TTbar_dRljet_energetic_1030", "")
    drawAllEffTTbar_2D(filedir_ttbar,"evtree", "b1jet_pt:abs(b1jet_eta)","(25,0,2.5)","(20,30.0,160.0)","#eta","p_{T}",dencut, numcut,"mediumbtagEff","") 
    #draw1D_combined(file,dir,nu1_pt,nu2_pt, onshell_nupt_bins,"Simulated p_{T#nu}^{onshellW}",onshellW_1_cut,offshellW_1_cut,"onshell_nupt_1M_mediateStates_0325")
    delta_phi = "(25,-3.1415,3.1415)"
    delta_eta = "(50,-5.0,5.0)"
    #deltaR1(file,dir,delta_eta,delta_phi,h2toh1h1_cut,"h2toh1h1_0223")
    #deltaR2(file,dir,delta_eta,delta_phi,h2toh1h1_cut,"h2toh1h1_0223")
   
     
    #draw2D_combined(file,dir, onoffshellWmass2, onoffshellWmass1, wmass_onshell_bins,wmass_offshell_bins,"Simulated M_{W}^{onshell}","Simulated M_{W}^{offshell}",onshellW_1_cut,offshellW_1_cut,"onshellVsoffshell_Wmass_1M_mediateStates_0325")


import random
import ROOT
import os

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
def draw1DEff(file,dir,todraw,x_bins,x_title,cut1,cut2,pic_name):
    
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
    b.GetYaxis().SetTitle("Efficiency")
    b.GetXaxis().SetTitle("%s"%x_title)
    b.GetYaxis().SetRangeUser(0,1.0)
    b.SetStats(0)

    e1 = ROOT.TH1F("e1","e1",xBins,xminBin,xmaxBin)
    e2 = ROOT.TH1F("e2","e2",xBins,xminBin,xmaxBin)
   

    t.Draw(todraw+">>e1",cut1)#den
    t.Draw(todraw+">>e2",cut2)
    eff = ROOT.TEfficiency(e2,e1)

    b.Draw()
    eff.Draw("same")
    c1.SaveAs("Dihiggs_Eff_%s"%(pic_name)+"_B3_delphes.pdf")
    c1.SaveAs("Dihiggs_Eff_%s"%(pic_name)+"_B3_delphes.png")

#___________________________________________
def draw1DEff_Btag(file,dir,todraw1,todraw2,x_bins,x_title,cut11,cut12,cut21,cut22,pic_name):
    
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
    b.GetYaxis().SetTitle("Efficiency")
    b.GetXaxis().SetTitle("%s"%x_title)
    b.GetYaxis().SetRangeUser(0,1.0)
    b.SetStats(0)

    e11 = ROOT.TH1F("e11","e11",xBins,xminBin,xmaxBin)
    e21 = ROOT.TH1F("e21","e21",xBins,xminBin,xmaxBin)
    e12 = ROOT.TH1F("e12","e12",xBins,xminBin,xmaxBin)
    e22 = ROOT.TH1F("e22","e22",xBins,xminBin,xmaxBin)
   

    t.Draw(todraw+">>e11",cut11)#den
    t.Draw(todraw+">>e12",cut12)
    t.Draw(todraw+">>e21",cut21)#den
    t.Draw(todraw+">>e22",cut22)
    eff1 = ROOT.TEfficiency(e12,e11)
    eff2 = ROOT.TEfficiency(e22,e21)
    eff1.Add(eff2)

    b.Draw()
    eff1.Draw("same")
    return eff1 
    c1.SaveAs("Dihiggs_Eff_%s"%(pic_name)+"_B3_delphes.pdf")
    c1.SaveAs("Dihiggs_Eff_%s"%(pic_name)+"_B3_delphes.png")

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
    
    hs = ROOT.THStack("hs","%s distribution, PU40"%x_title)
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
    #normalized to same area
    b1.Scale(1.0/b1.Integral())
    b2.Scale(1.0/b2.Integral())
    hs.Add(b1)
    hs.Add(b2)
    #b2.Draw("same")
    hs.Draw("nostack")
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend = ROOT.TLegend(0.4,0.80,0.95,0.94)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    legend.AddEntry(b1,"%s, entries %.f"%(leg1, b1.GetEntries()),"pl") 
    legentry = legend.AddEntry(b2,"%s, entries %.f"%(leg2, b2.GetEntries()),"pl") 
    legentry.SetTextColor(ROOT.kRed)
    legend.Draw("same")
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    tex = ROOT.TLatex(.6,0.7,"HH: Louvain Sample ")
    tex.SetTextSize(0.05)
    tex.SetTextFont(42)
    tex.SetTextColor(ROOT.kRed)
    tex.SetNDC()
    #tex.Draw("same")
	
    
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined.png")


#___________________________________________
def draw1D_combined3(file1,file2, file3,dir1,dir2, dir3,todraw,x_bins,x_title,cut1,cut2,cut3, leg1,leg2, leg3,pic_name):
    
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    f1 = ROOT.TFile(file1)#ttbar
    t1 = f1.Get(dir1)
    f2 = ROOT.TFile(file2)#H->hh
    t2 = f2.Get(dir2)
    f3 = ROOT.TFile(file3)#H->hh
    t3 = f3.Get(dir3)
    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    
    hs = ROOT.THStack("hs","%s, normalized to same area"%x_title)

    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    t1.Draw(todraw+">>b1",cut1)

    b3 = ROOT.TH1F("b3","b3",xBins,xminBin,xmaxBin)
    t3.Draw(todraw+">>b3",cut3)

    b2 = ROOT.TH1F("b2","b2",xBins,xminBin,xmaxBin)
    t2.Draw(todraw+">>b2",cut2)

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
    b1.SetLineWidth(3)
    b2.SetLineColor(ROOT.kRed)
    b2.SetLineStyle(2)
    b2.SetLineWidth(3)
    b3.SetLineColor(ROOT.kBlue)
    b3.SetLineStyle(3)
    b3.SetLineWidth(3)
    b1.Scale(1.0/b1.Integral())
    b2.Scale(1.0/b2.Integral())
    b3.Scale(1.0/b3.Integral())
    hs.Add(b1)
    hs.Add(b2)
    hs.Add(b3)
    #b2.Draw("same")
    hs.Draw("nostack")
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend = ROOT.TLegend(0.65,0.60,0.9,0.94)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    legend.AddEntry(b1,"%s"%leg1,"pl") 
    legentry2 = legend.AddEntry(b2,"%s"%(leg2),"pl") 
    legentry2.SetTextColor(ROOT.kRed)
    legentry3 = legend.AddEntry(b3,"%s"%(leg3),"pl") 
    legentry3.SetTextColor(ROOT.kBlue)
    legend.Draw("same")
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    tex = ROOT.TLatex(.6,0.7,"HH: UCL Sample ")
    tex.SetTextSize(0.05)
    tex.SetTextFont(42)
    tex.SetTextColor(ROOT.kRed)
    tex.SetNDC()
    #tex.Draw("same")
	
    
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined3.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined3.png")
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

    chain = ROOT.TChain(treename)
    if os.path.isdir(dir):
    	  ls = os.listdir(dir)
    	  for x in ls:
		x = dir[:]+x
		chain.Add(x)
    elif os.path.isfile(dir):
	  chain.Add(dir)
    else:
	  print " it is not file or dir ", dir
#    ROOT.gStyle.SetOptStat()
    #acceptcut = "h2tohh && preselections && hasgenb1jet && hasgenb2jet && hasGenMET"
    #acceptcut = "preselections && hasgenb1jet && hasgenb2jet"
    #acceptcut = "preselections_gen && h2tohh && hasgenb1jet && hasgenb2jet && b1_pt>35 && b2_pt>35 && abs(b1_eta)<2.4 && abs(b2_eta)<2.4 && hasMET && dR_genb1jet<6 && dR_genb2jet<6"#tighter cut on parton 
    acceptcut = "hasRECOjet1 && hasRECOjet2 && hasMET && hastwomuons && hasdRljet"
    #acceptcut = "h2tohh && hasgenb1jet && hasgenb2jet && dR_genb1jet<6 && dR_genb2jet<6 && hasMET && b1_pt>30 && b2_pt>30 &&abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons "
    #acceptcut = "h2tohh && hasRECOjet1 && hasRECOjet2 && hasMET && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons && dR_b1jet<6 && dR_b2jet<6 && hasRecoMuon1 && hasRecoMuon2 && hasRecob1jetCorr && hasRecob2jetCorr && hasdRljet"
    #acceptcut = "preselections_gen && hasgenb1jet && hasgenb2jet && dR_genb1jet<0.1 && dR_genb2jet<0.1 && hasGenMET"
    #acceptcut = "dR_genb1jet<0.4 && dR_genb2jet<0.4 && hasgenb1jet && hasgenb2jet && hasMET && hastwomuons && b1_pt>30 && b2_pt>30"
    e1 = ROOT.TH1F("e1","e1",xBins,xminBin,xmaxBin)
    e2 = ROOT.TH1F("e2","e2",xBins,xminBin,xmaxBin)
    e = ROOT.TH1F("e","e",xBins,xminBin,xmaxBin)
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)
    chain.Draw(todraw+">>b1", cut)
    chain.Draw(truetodraw+">>e", cut)
    chain.Draw(truetodraw+">>e1", acceptcut)
    #entries depends on how many times it is filled, integral also depends on the weight 
    totinput = e1.GetEntries()
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
    
    print " MMC underflow  ",b1.GetBinContent(0)," overflow ",b1.GetBinContent(xBins+1)," integral(0,xbins+1) ",b1.Integral(0,xBins+1)," Integral (1,xBins) ",b1.Integral(1,xBins)
    print " totinput ", totinput," e1 entries ", e1.GetEntries()," num_MMC ",num_MMC," rms ",rms," e1 peak ", value
    print " xlow ",xlow, " xhigh ", xhigh," num_middle ", num_middle," e2 entries ",e2.GetEntries()
   # c1.SetLogy()
    
    #b1.GetXaxis().SetTitleOffset(0.6)
    #b1GetXaxis().SetTitleSize(0.08)
    yaxis_max = 0
    if b1.GetMaximum()>e2.GetMaximum():
	yaxis_max = b1.GetMaximum()
    else:
	yaxis_max = e2.GetMaximum()
    b1.Draw() 
    e2.Draw("histsame")
    e.Draw("same")
    e.SetLineColor(ROOT.kRed)
    e.SetLineWidth(2)
    e.SetLineStyle(2)
    b1.SetLineWidth(2)
    b1.GetYaxis().SetRangeUser(0, yaxis_max*1.05)
    b1.SetLineColor(ROOT.kBlack)
    legend = ROOT.TLegend(0.15,0.82,0.65,0.94)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(42)
    legend.AddEntry(b1,"Reconstructed ","pl") 
    #legentry = legend.AddEntry(e,"True,entries=%d peak=%d"%(totinput, value),"pl") 
    legentry = legend.AddEntry(e,"True ","pl") 
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
    txt="#splitline{p_{T}^{jet}>30,|#eta^{jet}|<2.5}{p_{T}^{#mu}>10,|#eta^{#mu}|<2.4; #slash{E}_{T}>20}"
    tex2 = ROOT.TLatex(0.15,0.5,txt)
    tex2.SetTextSize(0.05)
    tex2.SetTextFont(42)
    tex2.SetTextColorAlpha(ROOT.kBlack,0.6)
    tex2.SetNDC()
    #tex2.Draw("same")
    legend.Draw("same")
    c1.Update()
    #print "stats obj ", b1.GetListOfFunctions().FindObject("stats")
    #statbox = b1.FindObject("stats")
	
    #print "statbox ",statbox
    statbox = ROOT.TPaveText()
    #statbox.SetName("MMC summary");
     
    statbox.SetX1NDC(0.6)
    statbox.SetX2NDC(0.85)
    statbox.SetY1NDC(0.3)
    statbox.SetY2NDC(0.80)
    #statbox.Draw("same")
    #statlist = statbox.GetListOfLines();
    #print " statlist ", statlist
    pstxt0 = ROOT.TLatex(0,0,"MMC results")
    pstxt1 = ROOT.TLatex(0,0,"peak+[-10,10] = %d"%num_middle)
    pstxt1.SetTextSize(0.03)
    pstxt2 = ROOT.TLatex(0,0,"true peak = %d"%value)
    pstxt2.SetTextSize(0.03)
    trueinput_txt = statbox.AddText("True Inputs")
    totinput_txt = statbox.AddText("Entries %d"%totinput)
    peak_txt = statbox.AddText("peak %.1f GeV/c^{2}"%value)
    trueinput_txt.SetTextColor(ROOT.kRed)
    totinput_txt.SetTextColor(ROOT.kRed)
    peak_txt.SetTextColor(ROOT.kRed)
    statbox.AddText("MMC Results")
    statbox.AddText("Entries  %d(%.1f%%)"%(b1.GetEntries(),100*b1.GetEntries()/totinput))
    statbox.AddText("Mean %.1f GeV/c^{2}"%b1.GetMean())
    statbox.AddText("RMS %.1f GeV/c^{2}"%b1.GetRMS())
    statbox.AddText("underflow %d"%b1.GetBinContent(0))
    statbox.AddText("overflow  %d"%b1.GetBinContent(xBins+1))
    #peakwindow0 = statbox.AddText("in [%.1f,%.1f]GeV/c^{2}"%(value-10,value+10))
    #peakwindow00 = statbox.AddText("(/MMC Entries, inputs)")
    #peakwindow1 = statbox.AddText("MMC  %d(%.1f%%)"%(num_middle,100*num_middle/b1.GetEntries()))
    #peakwindow2 = statbox.AddText("True  %d(%.1f%%)"%(truenum_middle,100*truenum_middle/b1.GetEntries()))

    #peakwindow0.SetTextColor(46)
    #peakwindow00.SetTextColor(46)
    #peakwindow1.SetTextColor(46)
    #peakwindow2.SetTextColor(46)

    #tconst = statbox.GetLineWith("RMS");
    b1.SetStats(0)
    statbox.SetTextSize(0.04)
    #peakwindow0.SetTextSize(0.032)
    #peakwindow00.SetTextSize(0.030)
    statbox.SetFillColor(ROOT.kWhite)
    #statbox.SetFillStyle(0)
    statbox.Draw("same")
    c1.Modified()


    c1.SaveAs("Dihiggs_combined_%s"%pic_name+"_delphes.pdf")
    c1.SaveAs("Dihiggs_combined_%s"%pic_name+"_delphes.png")


def drawAll_combined1D_stacks(dir, treename, todraw, truetodraw, x_bins,x_title,cut1,cut2,cut3,pic_name, text):
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])

    chain = ROOT.TChain(treename)
    if os.path.isdir(dir):
    	  ls = os.listdir(dir)
    	  for x in ls:
		x = dir[:]+x
		chain.Add(x)
    elif os.path.isfile(dir):
	  chain.Add(dir)
    else:
	  print " it is not file or dir ", dir
#    ROOT.gStyle.SetOptStat()
    #acceptcut = "h2tohh && preselections && hasgenb1jet && hasgenb2jet && hasGenMET"
    #acceptcut = "preselections && hasgenb1jet && hasgenb2jet"
    #acceptcut = "preselections_gen && h2tohh && hasgenb1jet && hasgenb2jet && b1_pt>35 && b2_pt>35 && abs(b1_eta)<2.4 && abs(b2_eta)<2.4 && hasMET && dR_genb1jet<6 && dR_genb2jet<6"#tighter cut on parton 
    acceptcut = "preselections_gen && h2tohh && hasgenb1jet && hasgenb2jet && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hasMET && dR_genb1jet<0.1 && dR_genb2jet<0.1 && dR_mu1<0.1 && dR_mu2<0.1"
    #acceptcut = "preselections_gen && h2tohh && hasMET && dR_genb1jet<0.1 && dR_genb2jet<0.1"
    #acceptcut = "preselections_gen && hasgenb1jet && hasgenb2jet && dR_genb1jet<0.1 && dR_genb2jet<0.1 && hasGenMET"
    #acceptcut = "dR_genb1jet<0.4 && dR_genb2jet<0.4 && hasgenb1jet && hasgenb2jet && hasMET && hastwomuons && b1_pt>30 && b2_pt>30"
    hs = ROOT.THStack("hs","Reconstructed heavy higgs mass, with different #DeltaR_{(parton, jet)}")
    e1 = ROOT.TH1F("e1","e1",xBins,xminBin,xmaxBin)
    e2 = ROOT.TH1F("e2","e2",xBins,xminBin,xmaxBin)
    e = ROOT.TH1F("e","e",xBins,xminBin,xmaxBin)
    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    b2 = ROOT.TH1F("b2","b2",xBins,xminBin,xmaxBin)
    b3 = ROOT.TH1F("b3","b3",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)
    chain.Draw(todraw+">>b1", cut1)
    chain.Draw(todraw+">>b2", cut2)
    chain.Draw(todraw+">>b3", cut3)
    print " MMC underflow  ",b1.GetBinContent(0)," overflow ",b1.GetBinContent(xBins+1)," integral(0,xbins+1) ",b1.Integral(0,xBins+1)," Integral (1,xBins) ",b1.Integral(1,xBins)," cut1 ",cut1
    print "b2 MMC underflow  ",b2.GetBinContent(0)," overflow ",b2.GetBinContent(xBins+1)," integral(0,xbins+1) ",b2.Integral(0,xBins+1)," Integral (1,xBins) ",b2.Integral(1,xBins)," cut2 ",cut2
    print "b3 MMC underflow  ",b3.GetBinContent(0)," overflow ",b3.GetBinContent(xBins+1)," integral(0,xbins+1) ",b3.Integral(0,xBins+1)," Integral (1,xBins) ",b3.Integral(1,xBins)," cut3 ",cut3
    #entries depends on how many times it is filled, integral also depends on the weight 
    b1.SetFillColor(ROOT.kMagenta+2)
    b2.SetFillColor(ROOT.kBlue)
    b3.SetFillColor(ROOT.kGreen+2)
    hs.Add(b1)
    hs.Add(b2)
    hs.Add(b3)

    legend = ROOT.TLegend(0.4,0.6,0.9,0.94)
    #legend.SetFillColor(ROOT.kWhite)
    legend.SetBorderSize(0)
    legend.SetFillStyle(0)
    legend.SetTextSize(0.030)
    legend.SetTextFont(62)
    #legtxt1 = "both #DeltaR<0.1, entries: %d"%(b1.Integral(0,xBins+1)
    #legtxt2 = "#splitline{both #DeltaR<0.4}{at least one #DeltaR>=0.1, entries: %d}"%(b2.Integral(0,xBins+1))
    #legtxt3 = "at least one 0.4=<#DeltaR, entries: %d"%(b3.Integral(0,xBins+1))
    legtxt1 = "both two jet with btag, entries: %d"%(b1.Integral(0,xBins+1))
    legtxt2 = "ONLY one jet with btag, entries: %d"%(b2.Integral(0,xBins+1))
    legtxt3 = "NO jet with btag, entries: %d"%(b3.Integral(0,xBins+1))

    leg1=legend.AddEntry(b1, legtxt1,"f") 
    leg1.SetTextColor(ROOT.kMagenta+2)
    leg2=legend.AddEntry(b2, legtxt2,"f") 
    leg2.SetTextColor(ROOT.kBlue)
    leg3=legend.AddEntry(b3, legtxt3,"f") 
    leg3.SetTextColor(ROOT.kGreen+2)
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    hs.Draw()
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend.Draw("same")


    c1.SaveAs("Dihiggs_combineddR_%s"%pic_name+"_delphes.pdf")
    c1.SaveAs("Dihiggs_combineddR_%s"%pic_name+"_delphes.png")

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
    ROOT.gPad.SetLogz()

    
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")
    
    c1.SaveAs("Dihiggs_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_All_B3.png")


#___________________________________________
def draw1D_delphes_SB(file1,dir1,file2,dir2,todraw1,todraw2,cut, leg1, leg2,x_bins,x_title, pic_name):
    
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
    
    #hs = ROOT.THStack("hs","%s distribution for Signal and Background "%x_title)
    hs = ROOT.THStack("hs","%s distribution "%x_title)

    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    t1.Draw(todraw1+">>b1",cut)

    b2 = ROOT.TH1F("b2","b2",xBins,xminBin,xmaxBin)
    t2.Draw(todraw2+">>b2",cut)

    b2.SetLineColor(ROOT.kRed)
    #b2.SetLineStyle(3)
    b2.SetLineWidth(2)
    b1.SetLineColor(ROOT.kBlue)
    #b1.SetLineStyle(1)
    b1.SetLineWidth(2)
    #factor1 = 0.0850820764899
    #factor2 = 3.23058128357
    factor1 = 1
    factor2 = 1
    b1.Scale(factor1)
    b2.Scale(factor2)
    hs.Add(b2)
    hs.Add(b1)
    #b2.Draw("same")
    #hs.Draw()
    hs.Draw("nostack")
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    ROOT.gPad.SetLogy()
    legend = ROOT.TLegend(0.4,0.70,0.95,0.94)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.045)
    legend.SetTextFont(62)
    legentry1 = legend.AddEntry(b1,"%s"%(leg1),"pl") 
    legentry1.SetTextColor(ROOT.kBlue)
    legentry2 = legend.AddEntry(b2,"%s"%(leg2),"pl") 
    legentry2.SetTextColor(ROOT.kRed)
    legend.Draw("same")
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    #tex = ROOT.TLatex(.6,0.7,"HH: UCL Sample ")
    #tex.SetTextSize(0.05)
    #tex.SetTextFont(42)
    #tex.SetTextColor(ROOT.kRed)
    #tex.SetNDC()
    #tex.Draw("same")
	
    
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined_delphes.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined_delphes.png")

#___________________________________________
def draw1D_delphes_cmssw(file1,dir1, file2, dir2,todraw1,todraw2,cut1,cut2,leg1, leg2,x_bins,x_title, pic_name):
    
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
    
    hs = ROOT.THStack("hs","%s distribution for Hhh from delphes and CMSSW, normalized to same area "%x_title)

    b1 = ROOT.TH1F("b1","b1",xBins,xminBin,xmaxBin)
    t1.Draw(todraw1+">>b1",cut1)

    b2 = ROOT.TH1F("b2","b2",xBins,xminBin,xmaxBin)
    t2.Draw(todraw2+">>b2",cut2)

    b2.SetLineColor(ROOT.kRed)
    #b2.SetLineStyle(3)
    b2.SetLineWidth(2)
    b1.SetLineColor(ROOT.kBlue)
    #b1.SetLineStyle(1)
    b1.SetLineWidth(2)
    b1.Scale(1.0/b1.Integral())
    b2.Scale(1.0/b2.Integral())
    hs.Add(b1)
    hs.Add(b2)
    #b2.Draw("same")
    hs.Draw("nostack")
    hs.GetHistogram().GetXaxis().SetTitle("%s"%x_title)
    legend = ROOT.TLegend(0.65,0.80,0.85,0.94)
    legend.SetFillColor(ROOT.kWhite)
#    legend.SetFillStyle(0)
    legend.SetTextSize(0.05)
    legend.SetTextFont(62)
    legentry2 = legend.AddEntry(b2,"%s"%(leg2),"pl") 
    legentry2.SetTextColor(ROOT.kRed)
    legentry1 = legend.AddEntry(b1,"%s"%(leg1),"pl") 
    legentry1.SetTextColor(ROOT.kBlue)
    legend.Draw("same")
    #legend.AddEntry(b1,"Jet at particle level ","l") 
    #legend.AddEntry(e,"parton level","l") 
    #tex = ROOT.TLatex(.6,0.7,"HH: UCL Sample ")
    #tex.SetTextSize(0.05)
    #tex.SetTextFont(42)
    #tex.SetTextColor(ROOT.kRed)
    #tex.SetNDC()
    #tex.Draw("same")
	
    
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined2_delphes_cmssw.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_combined2_delphes_cmssw.png")

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
    #file1 = "TTbar_WWbb_1M_PU0_leptonW_7570700_1019_validatecuts.root"
    #file1 = "TTbar_WWbb_1M_PU0_leptonW_7570700_1010_validatecuts.root"
    #file2 = "DiHiggs_Louvain_combined.root"
    #file3 = "DiHiggs_WWbb_1M_PU0_leptonW_6969448_B3_1010_validatecuts.root"
    
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
    file1 = "DiHiggs_NewB3_1M_Reco_20160229_PU40.root" 
    file2 = "DiHiggs_NewB3_1M_Puppi_20160229_PU40.root" 
    file3 = "DiHiggs_1M_finalb_20160214.root"
    dir1 = "evtree"
    dir2 = "evtree"
    """
    draw1D_combined3(file1,file2, file3, dir1,dir2, dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && dR_genb1jet<0.4 && dR_b1jet<0.1","h2tohh && hasRECOjet1 && dR_genb1jet<0.4 && dR_b1jet<0.1","h2tohh && hasRECOjet1 && dR_genb1jet<0.4 && dR_b1jet<0.1","PU40 Reco","PU40 Puppi", "PU0 Reco","delphes_PU0PU40Puppi_reco_bjetpt")
    draw1D_combined3(file1,file2, file3, dir1,dir2, dir2,"met-genmet","(100,-100,100)","#slash{E}_{T}^{Gen}-#slash{E}_{T}","h2tohh && hasMET","h2tohh && hasMET","h2tohh && hasMET","PU40 Reco","PU40 Puppi", "PU0 Reco","delphes_PU0PU40Puppi_reco_met")
    draw1D_combined(file1,file2, dir1,dir2,"dR_b1jet","(100,0,1)","#Delta R_{(genjet,jet)}","h2tohh && dR_b1jet<10","h2tohh && dR_b1jet<10","Reco Jet","Puppi Jet","delphes_recoVsPuppi_dRb1jet")
    draw1D_combined(file1,file2, dir1,dir2,"dR_b2jet","(100,0,1)","#Delta R_{(genjet,jet)}","h2tohh && dR_b2jet<10","h2tohh && dR_b2jet<10","Reco Jet","Puppi Jet","delphes_recoVsPuppi_dRb2jet")
    draw1D_combined(file1,file2, dir1,dir2,"met-genmet","(100,-100,100)"," #slash{E}_{T}^{Gen}-#slash{E}_{T}","h2tohh && hasMET","h2tohh && hasMET","Reco MET","Puppi MET","delphes_recoVsPuppi_MET")
    draw1D_combined(file1,file2, dir1,dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1","h2tohh && hasRECOjet1","Reco Jet","Puppi Jet","delphes_recoVsPuppi_b1jetpt")
    draw1D_combined(file1,file2, dir1,dir2,"genb2jet_pt-b2jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet2","h2tohh && hasRECOjet2","Reco Jet","Puppi Jet","delphes_recoVsPuppi_b2jetpt")
    draw1D_combined(file1,file2, dir1,dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && hasb1jet","h2tohh && hasRECOjet1 && hasb1jet","Reco Jet","Puppi Jet","delphes_recoVsPuppi_btag_b1jetpt")
    draw1D_combined(file1,file2, dir1,dir2,"genb2jet_pt-b2jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet2 && hasb2jet","h2tohh && hasRECOjet2 && hasb2jet","Reco Jet","Puppi Jet","delphes_recoVsPuppi_btag_b2jetpt")
    draw1D_combined(file1,file2, dir1,dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && hasRecob1jetCorr","h2tohh && hasRECOjet1 && hasRecob1jetCorr","Reco Jet","Puppi Jet","delphes_recoVsPuppi_foundbjet_b1jetpt")
    draw1D_combined(file1,file2, dir1,dir2,"genb2jet_pt-b2jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet2 && hasRecob2jetCorr","h2tohh && hasRECOjet2 && hasRecob2jetCorr","Reco Jet","Puppi Jet","delphes_recoVsPuppi_foundbjet_b2jetpt")
    draw1D_combined(file3,file1, dir1,dir2,"dR_b1jet","(100,0,1)","#Delta R_{(genjet,jet)}","h2tohh && dR_b1jet<10","h2tohh && dR_b1jet<10","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_dRb1jet")
    draw1D_combined(file3,file1, dir1,dir2,"dR_b2jet","(100,0,1)","#Delta R_{(genjet,jet)}","h2tohh && dR_b2jet<10","h2tohh && dR_b2jet<10","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_dRb2jet")
    draw1D_combined(file3,file1, dir1,dir2,"met-genmet","(100,-100,100)"," #slash{E}_{T}^{Gen}-#slash{E}_{T}","h2tohh && hasMET","h2tohh && hasMET","PU0 Reco MET","PU40 Reco MET","delphes_reco_PU0VsPU40_MET")
    draw1D_combined(file3,file1, dir1,dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1","h2tohh && hasRECOjet1","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_b1jetpt")
    draw1D_combined(file3,file1, dir1,dir2,"genb2jet_pt-b2jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1","h2tohh && hasRECOjet2","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_b2jetpt")
    draw1D_combined(file3,file1, dir1,dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && hasb1jet","h2tohh && hasRECOjet1 && hasb1jet","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_btag_b1jetpt")
    draw1D_combined(file3,file1, dir1,dir2,"genb2jet_pt-b2jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && hasb2jet","h2tohh && hasRECOjet2 && hasb2jet","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_btag_b2jetpt")
    draw1D_combined(file3,file1, dir1,dir2,"genb1jet_pt-b1jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && hasRecob1jetCorr","h2tohh && hasRECOjet1 && hasRecob1jetCorr","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_foundbjet_b1jetpt")
    draw1D_combined(file3,file1, dir1,dir2,"genb2jet_pt-b2jet_pt","(100,-100,100)","p_{T}^{genjet}-p_{T}^{reco}","h2tohh && hasRECOjet1 && hasRecob2jetCorr","h2tohh && hasRECOjet2 && hasRecob2jetCorr","PU0 Reco Jet","PU40 Reco Jet","delphes_reco_PU0VsPU40_foundbjet_b2jetpt")
    """
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
    ttbaselinecut = "ttbar && hasMET && hasdRljet && dR_genl1l2<2.5 && dR_genl1l2>-1 && mu1_pt>15 && mu2_pt>15 && genb1jet_pt>15 && genb2jet_pt>15 && mu1_eta<2.5 && mu2_eta<2.5"
    Hhhbaselinecut = "hasb1jet && hasb2jet &&hasMET && hasdRljet "
    #draw1D_combined(file1,file2, dir1,dir2,"mass_l1l2","(70,0,300)","M_{ll}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_Mll_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"dR_l1l2","(45,0,4.5)","#DeltaR_{ll}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_dRll_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"mass_b1b2","(80,0,400)","M_{bjets}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_Mbjets_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"dR_b1b2","(45,0,4.5)","#DeltaR_{bjets}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_dRbjets_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"met","(50,0,300)","#slash{E}_{T}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_met_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(dphi_llbb)","(70,0,3.5)","#Delta#phi_{(ll,jj)}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_dphillbb_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(dphi_llmet)","(70,0,3.5)","#Delta#phi_{(ll,#slash{E}_{T})}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_dphillmet_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(dR_minbl)","(45,0,4.5)","min#DeltaR_{bjetl}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_dRblmin_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(pt_l1l2)","(50,0,200)","p_{T}^{ll}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_ptll_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(pt_b1b2)","(60,0,300)","p_{T}^{bjets}",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_ptbjets_reco")
    #draw1D_combined(file1,file2, dir1,dir2,"abs(mass_trans)","(60,0,300)","Transverse Mass",ttbaselinecut,Hhhbaselinecut,"t#bar{t}","HH","delphes_masstrans_reco")
    
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"mass_l1l2","(70,0,300)","M_{ll}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_Mll_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"dR_l1l2","(45,0,4.5)","#DeltaR_{ll}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_dRll_recoi)
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"mass_b1b2","(80,0,400)","M_{bjets}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_Mbjets_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"dR_b1b2","(45,0,4.5)","#DeltaR_{bjets}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_dRbjets_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"met","(50,0,300)","#slash{E}_{T}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_met_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"abs(dphi_llbb)","(70,0,3.5)","#Delta#phi_{(ll,jj)}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_dphillbb_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"abs(dphi_llmet)","(70,0,3.5)","#Delta#phi_{(ll,#slash{E}_{T})}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_dphillmet_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"pt_l1l2","(50,0,200)","p_{T}^{ll}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_ptll_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"pt_b1b2","(60,0,300)","p_{T}^{bjets}",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_ptbjets_reco")
    #draw1D_combined3(file1,file2, file3, dir1,dir2, dir3,"mass_trans","(60,0,300)","Transverse Mass",ttbaselinecut,Hhhbaselinecut, Hhhbaselinecut,"t#bar{t}","HH: UCL Sample", "Hhh","delphes_masstrans_reco")
    #validateHhh(file2,dir2,"delphes_1005")
    #validatettbar(file1,dir1,"delphes_1005")
    #runMMCok = "runMMCok && h2tohh && preselections_gen && hasgenb1jet && hasgenb2jet && b1_pt>35 && b2_pt>35 && abs(b1_eta)<2.4 && abs(b2_eta)<2.4 && hasMET && dR_genb1jet<0.1 && dR_genb2jet<0.1"
    #runMMCok = "runMMCok && h2tohh && hasgenb1jet && hasgenb2jet && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hasMET && dR_genb1jet<6 && dR_genb2jet<6 && hastwomuons"
    runMMCok = "runMMCok && hasRECOjet1 && hasRECOjet2 && hasMET && hastwomuons && hasdRljet"
    #runMMCok = "runMMCok && h2tohh && preselections_gen && hasMET && dR_genb1jet<6 && dR_genb2jet<6"
    #runMMCok = "runMMCok && h2tohh && hasgenb1jet && hasgenb2jet && dR_genb1jet<6 && dR_genb2jet<6 && hasMET && hastwomuons && b1_pt>30 && b2_pt>30 &&abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwogenmuons && dR_mu1<0.1 && dR_mu2<0.1"

    #file100_22_dR10_MuNomatch = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_dR10_btag_MuNomatch_combined.root"
    #drawAll_combined1D(file100_22_dR10_MuNomatch,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_100_22_noweight_1207_dR04_btag_MuNomatch_bpt30","")
    #drawAll_combined1D(file100_22_dR10_MuNomatch,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_100_22_weight1_1207_dR04_btag_MuNomatch_bpt30","")
    #drawAll_combined1D(file100_22_dR10_MuNomatch,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_100_22_weight4_1207_dR04_btag_MuNomatch_bpt30","")
    file000_22_dR10_0119 = "DiHiggs_WWbb_1M_B3_000_22_MMC1M_dR10_0119_combined.root"
    file100_22_dR10 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_MuBeforeIso_dR10_btag_0114_combined.root"
    file100_22_dR10_0119 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_MuBeforeIso_dR10_MudR10_0119_combined.root"
    file100_22_dR10_0125 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_MuafterIso_dR10_MudR10_0125_MuNomatch_combined.root"
    file110_22_dR10 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_Nobtag_0111_combined.root"
    file110_22_dR10_0119 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_Nobtag_0119_combined.root"
    file110_22_Nomatch_dR10 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_btag_0118_Nomatch_combined.root"
    file100_22_MuNomatch_dR10 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_Nobtag_0114_MuNoSimulation_combined.root"
    file100_22_MuNomatch_0120 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_MubeforeIso_dR10_MudR10_0120_MuNomatch_combined.root"
    file110_22_Nomatch_0120 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_btag_0120_Nomatch_combined.root"
    file110_22_Nomatch_0201_Ptjet = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_btag_Ptjet_0201_Nomatch_combined.root"
    file110_22_Nomatch_0201_btaginvariantmass = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_btaginvariantmass_0201_Nomatch_combined.root"
    file111_22_0120 = "DiHiggs_WWbb_1M_B3_111_22_MMC1M_dR10_0120_combined.root"
    file111_25_0120 = "DiHiggs_WWbb_1M_B3_111_25_MMC1M_dR10_0120_combined.root"
    file_allreco_22_0120 = "DiHiggs_WWbb_1M_B3_111_22_MMC1M_dR10_Nomatch_0120_combined.root"
    file_allreco_25_0120 = "DiHiggs_WWbb_1M_B3_allReco_25_MMC1M_0125_combined.root"
    file_allreco_25_0201_Ptjet = "DiHiggs_WWbb_1M_B3_allReco_25_MMC1M_Ptjet_0201_combined.root"
    file_allreco_25_0201_btaginvariantmass = "DiHiggs_WWbb_1M_B3_allReco_25_MMC1M_btaginvariantmass_0201_combined.root"
    file_allreco_25_B3_btaginvariantmass = "DiHiggs_WWbb_1M_B3_allReco_25_MMC1M_btaginvariantmass_0207_combined.root"
    file_allreco_25_B6_btaginvariantmass = "DiHiggs_WWbb_1M_B6_allReco_25_MMC1M_btaginvariantmass_0207_combined.root"
    file_allreco_25_ttbar_btaginvariantmass = "TTbar_Wtobmu_allReco_25_20160207_combined.root"
    file_allreco_25_newB3 = "DiHiggs_WWbb_1M_NewB3_allReco_25_MMC1M_btaginvariantmass_0222_combined.root"
    file_allreco_25_newB6 = "DiHiggs_WWbb_1M_NewB6_allReco_25_MMC1M_btaginvariantmass_0222_combined.root"
    txt000="#DeltaR_{parton,genjet}<0.4"
    ttbar_mass = "sqrt((t1_energy+t2_energy)**2-(t1_px+t2_px)**2-(t1_py+t2_py)**2-(t1_pz+t2_pz)**2)"
    #file111_25_PU40 = "DiHiggs_WWbb_1M_B3_111_25_MMC1M_btaginvariantmass_PU40_0307_combined.root"
    file111_25_PU40 = "/fdata/hepx/store/user/taohuang/Hhh/combined_samples/DiHiggs_WWbb_1M_B3_111_25_MMC1M_btaginvariantmass_MuonBeforeIso_PU40_0308_combined.root"

    drawAll_combined1D(file111_25_PU40,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_25_newB3_noweight_0308_dR60_MuBeforeIso_hasdRljet_PU40","PU40")
    drawAll_combined1D(file111_25_PU40,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_25_newB3_weight1_0308_dR60_MuBeforeIso_hasdRljet_PU40","PU40")
    #drawAll_combined1D(file110_22_Nomatch_0201_Ptjet,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_110_22_noweight_0201_Ptjet_dR60_nomatch_hasdRljet_Nocutonparton","")
    #drawAll_combined1D(file110_22_Nomatch_0201_Ptjet,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_110_22_weight1_0201_Ptjet_dR60_nomatch_hasdRljet_Nocutonparton","")
    #drawAll_combined1D(file_allreco_25_newB3,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_25_noweight_newB3_dR60_NOmatch_btaginvariantmass_MubeforeIso_hasdRljet_Nocutonparton","")
    #drawAll_combined1D(file_allreco_25_newB3,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_25_weight1_newB3_dR60_Nomatch_btaginvariantmass_MubeforeIso_hasdRljet_Nocutonparton","")
    #drawAll_combined1D(file_allreco_25_newB6,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,650)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_25_noweight_newB6_dR60_NOmatch_btaginvariantmass_MubeforeIso_hasdRljet_Nocutonparton","")
    #drawAll_combined1D(file_allreco_25_newB6,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,650)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_25_weight1_newB6_dR60_Nomatch_btaginvariantmass_MubeforeIso_hasdRljet_Nocutonparton","")
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_100_22_weight4_1207_dR04_btag","")
    file000_22_dR10 = "DiHiggs_WWbb_1M_B3_000_22_MMC1M_dR10_btag_gencut_1207_combined.root"
    dir = "evtree"
    btagcut11 ="h2tohh && dR_genb1jet<0.1  "
    #draw1DEff_Btag(file,dir,todraw1,todraw2,x_bins,x_title,cut11,cut12,cut21,cut22,"btageff_pt")

    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu1_pt","(50,0,200)","muon p_{T}^{sim}","h2tohh && mu1_pt>10 && abs(mu1_eta)<2.4","h2tohh && mu1_pt>10 && abs(mu1_eta)<2.4 && hasMuon1_beforeIso","Recomuon1_RECOeff_beforeIso_pt_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu2_pt","(50,0,200)","muon p_{T}^{sim}","h2tohh && mu2_pt>10 && abs(mu2_eta)<2.4","h2tohh && mu2_pt>10 && abs(mu2_eta)<2.4 && hasMuon2_beforeIso","Recomuon2_RECOeff_beforeIso_pt_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu1_eta","(50,-2.5,2.50)","muon #eta^{sim}","h2tohh && mu1_pt>10 && abs(mu1_eta)<2.4","h2tohh && mu1_pt>10 && abs(mu1_eta)<2.4 && hasMuon1_beforeIso","Recomuon1_RECOeff_beforeIso_eta_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu2_eta","(50,-2.5,2.50)","muon #eta^{sim}","h2tohh && Muon2_beforeIso_pt>10 && abs(Muon2_beforeIso_eta)<2.4","h2tohh && mu2_pt>10 && abs(mu2_eta)<2.4 && hasMuon2_beforeIso","Recomuon2_RECOeff_beforeIso_eta_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu1_pt","(50,0,200)","muon p_{T}^{sim}","h2tohh && Muon1_beforeIso_pt>10 && abs(Muon1_beforeIso_eta)<2.4 && hasMuon1_beforeIso","h2tohh && Muon1_beforeIso_pt>10&& abs(Muon1_beforeIso_eta)<2.4&& hasMuon1_beforeIso && hasMuon1","Recomuon1_Isoeff_pt_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu2_pt","(50,0,200)","muon p_{T}^{sim}","h2tohh && Muon2_beforeIso_pt>10 && abs(Muon2_beforeIso_eta)<2.4 && hasMuon2_beforeIso","h2tohh && Muon2_beforeIso_pt>10&& abs(Muon2_beforeIso_eta)<2.4 && hasMuon2_beforeIso && hasMuon2","Recomuon2_Isoeff_pt_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu1_eta","(50,-2.5,2.50)","muon #eta^{sim}","h2tohh && Muon1_beforeIso_pt>10 && abs(Muon1_beforeIso_eta)<2.4 && hasMuon1_beforeIso","h2tohh && Muon1_beforeIso_pt>10&& abs(Muon1_beforeIso_eta)<2.4 && hasMuon1_beforeIso && hasMuon1","Recomuon1_Isoeff_eta_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu2_eta","(50,-2.5,2.50)","muon #eta^{sim}","h2tohh && mu2_pt>10 && abs(mu2_eta)<2.4 && hasMuon2_beforeIso","h2tohh && Muon2_beforeIso_pt>10&& abs(Muon2_beforeIso_eta)<2.4&& hasMuon2_beforeIso && hasMuon2","Recomuon2_Isoeff_eta_cuts")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu1_pt","(50,0,200)","muon p_{T}^{sim}","h2tohh && Muon1_beforeIso_pt>10 && abs(Muon1_beforeIso_eta)<2.4 && hasMuon1_beforeIso","h2tohh && Muon1_beforeIso_pt>10&& abs(Muon1_beforeIso_eta)<2.4&& hasMuon1_beforeIso && abs(Muon1_beforeIso_IsoVar)<0.15","Recomuon1_Isoeff_pt_cutsv2")
    #draw1DEff("DiHiggs_NewB3_1M_Reco_20160229_PU40.root",dir,"mu1_eta","(50,-2.5,2.50)","muon #eta^{sim}","h2tohh && Muon1_beforeIso_pt>10 && abs(Muon1_beforeIso_eta)<2.4 && hasMuon1_beforeIso","h2tohh && Muon1_beforeIso_pt>10&& abs(Muon1_beforeIso_eta)<2.4 && hasMuon1_beforeIso && abs(Muon1_beforeIso_IsoVar)<0.15","Recomuon1_Isoeff_eta_cutsv2")
    #drawAll_combined1D(file000_22_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_000_22_noweight_1207_dR60_hasgenbjets_gencut_hasMET_tightercutonparton2","")
    #drawAll_combined1D(file000_22_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_000_22_weight1_1207_dR60_hasgenbjets_gencut_hasMET_tightercutonparton2","")
    #runMMCok01 = "runMMCok && h2tohh && hasgenb1jet && hasgenb2jet && hasMET && dR_genb1jet<0.1 && dR_genb2jet<0.1 && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons"
    #runMMCok02 = "runMMCok && h2tohh && hasgenb1jet && hasgenb2jet && hasMET && dR_genb1jet<0.4 && dR_genb2jet<0.4 && (dR_genb1jet>=0.1 || dR_genb2jet>=0.1) && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons"
    #runMMCok03 = "runMMCok && h2tohh && hasgenb1jet && hasgenb2jet && hasMET && dR_genb1jet<6 && dR_genb2jet<6 && (dR_genb1jet>=0.4 || dR_genb2jet>=0.4) && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons"
    #drawAll_combined1D_stacks(file100_22_MuNomatch_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok01+"&&MMC_h2mass_prob>=250",runMMCok02+"&&MMC_h2mass_prob>=250",runMMCok03+"&&MMC_h2mass_prob>=250","MMC_100_22_noweight_0114_munomatch_gencut_hasMET_cutonparton_cutongenmu","")
   # drawAll_combined1D_stacks(file100_22_MuNomatch_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok01+"&&MMC_h2massweight1_prob>=250",runMMCok02+"&&MMC_h2massweight1_prob>=250",runMMCok03+"&&MMC_h2massweight1_prob>=250","MMC_100_22_weight1_0114_munomatch_gencut_hasMET_cutonparton_cutongenmu","")
    #runMMCok01_reco = "runMMCok && h2tohh && hasRECOjet1 && hasRECOjet2 && hasMET && dR_b1jet<0.1 && dR_b2jet<0.1 && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons && hasRecoMuon1 && hasRecoMuon2 && hasRecob1jetCorr && hasRecob2jetCorr"
    #runMMCok02_reco = "runMMCok && h2tohh && hasRECOjet1 && hasRECOjet2 && hasMET && dR_b1jet<0.4 && dR_b2jet<0.4 && (dR_b1jet>=0.1 || dR_b2jet>=0.1) && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons && hasRecoMuon1 && hasRecoMuon2 && hasRecob1jetCorr && hasRecob2jetCorr"
    #runMMCok03_reco = "runMMCok && h2tohh && hasRECOjet1 && hasRECOjet2 && hasMET && dR_b1jet<6 && dR_b2jet<6 && (dR_b1jet>=0.4 || dR_b2jet>=0.4) && b1_pt>30 && b2_pt>30 && abs(b1_eta)<2.5 && abs(b2_eta)<2.5 && hastwomuons && hasRecoMuon1 && hasRecoMuon2 && hasRecob1jetCorr && hasRecob2jetCorr"
    runMMCok01_reco = "runMMCok && hasRECOjet1 && hasRECOjet2 && hasMET && (b1jet_btag&2)>0 && (b2jet_btag&2)>0 && hastwomuons && hasdRljet"
    runMMCok02_reco = "runMMCok && hasRECOjet1 && hasRECOjet2 && hasMET && ((b1jet_btag&2)>0 || (b2jet_btag&2)>0)&&((b1jet_btag&2)==0 || (b2jet_btag&2)==0) && hastwomuons && hasdRljet"
    runMMCok03_reco = "runMMCok && hasRECOjet1 && hasRECOjet2 && hasMET && ((b1jet_btag&2)==0 && (b2jet_btag&2)==0) && hastwomuons && hasdRljet"
    #drawAll_combined1D_stacks(file_allreco_25_newB3,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok01_reco+"&&MMC_h2mass_prob>=250",runMMCok02_reco+"&&MMC_h2mass_prob>=250",runMMCok03_reco+"&&MMC_h2mass_prob>=250","MMC_111_25_noweight_newB3_Nomatch_btaginvariantmass_hasMET_Nocutonparton","")
    #drawAll_combined1D_stacks(file_allreco_25_newB3,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok01_reco+"&&MMC_h2massweight1_prob>=250",runMMCok02_reco+"&&MMC_h2massweight1_prob>=250",runMMCok03_reco+"&&MMC_h2massweight1_prob>=250","MMC_111_25_weight1_newB3_Nomatch_btaginvariantmass_hasMET_Nocutonparton","")
    #drawAll_combined1D_stacks(file_allreco_25_newB6,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,650)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok01_reco+"&&MMC_h2mass_prob>=250",runMMCok02_reco+"&&MMC_h2mass_prob>=250",runMMCok03_reco+"&&MMC_h2mass_prob>=250","MMC_111_25_noweight_newB6_Nomatch_btaginvariantmass_hasMET_Nocutonparton","")
    #drawAll_combined1D_stacks(file_allreco_25_newB6,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,650)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok01_reco+"&&MMC_h2massweight1_prob>=250",runMMCok02_reco+"&&MMC_h2massweight1_prob>=250",runMMCok03_reco+"&&MMC_h2massweight1_prob>=250","MMC_111_25_weight1_newB6_Nomatch_btaginvariantmass_hasMET_Nocutonparton","")
    #draw1D_delphes_SB(file_allreco_25_B3_btaginvariantmass,"evtree", file_allreco_25_ttbar_btaginvariantmass,"evtree","MMC_h2massweight1_prob","MMC_h2massweight1_prob",runMMCok,"B3 Signal", "TTbar","(200,250,650)","Reconstructed M_{H}", "MMC_111_25_weight1_dR60_Nomatch_btaginvariantmass_2btag_MubeforeIso_hasdRljet_Nocutonparton") 
    file_finalb = "DiHiggs_1M_finalb_20160214.root"
    #draw1D_delphes_SB(file_finalb,"evtree", file_finalb,"evtree","dR_genb1jet","dR_genb1jet_finalb","h2tohh && b1_pt>40 && abs(b1_eta)<2.2","#DeltaR with b before gluon emssion", "#DeltaR with b after gluon emssion","(100,0,.40)","#Delta R_{(genjet, parton)}", "dR1_finalb_tightcutonparton_v2") 
    #draw1D_delphes_SB(file_finalb,"evtree", file_finalb,"evtree","dR_genb2jet","dR_genb2jet_finalb","h2tohh && b2_pt>40 && abs(b2_eta)<2.2","#DeltaR with b before gluon emssion", "#DeltaR with b after gluon emssion","(100,0,.40)","#Delta R_{(genjet, parton)}", "dR2_finalb_tightcutonparton_v2") 
   
    file100_22_dR10 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_dR10_btag_1202_combined.root"
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_100_22_noweight_1202_dR10_btag","")
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_100_22_weight1_1202_dR10_btag","")
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_100_22_weight4_1202_dR10_btag","")
    #drawAll_combined1D(file110_22_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_110_22_noweight_1202_dR10_btag","")
    #drawAll_combined1D(file110_22_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_110_22_weight1_1202_dR10_btag","")
    #drawAll_combined1D(file110_22_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_110_22_weight4_1202_dR10_btag","")
    #drawAll_combined1D(file110_22_Nomatch_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_110_22_noweight_1202_Nomatch_dR10_btag","")
    #drawAll_combined1D(file110_22_Nomatch_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_110_22_weight1_1202_Nomatch_dR10_btag","")
    #drawAll_combined1D(file110_22_Nomatch_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_110_22_weight4_1202_Nomatch_dR10_btag","")
    #file110_22_dR10 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_dR10_btag_combined.root"
    #drawAll_combined1D(file110_22_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_110_22_noweight_1123_dR10_btag","")
    #drawAll_combined1D(file110_22_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_110_22_weight1_1123_dR10_btag","")
    #drawAll_combined1D(file110_22_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_110_22_weight4_1123_dR10_btag","")
    #drawAll_combined1D(file111_22_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_22_noweight_1123_dR10_btag","")
    #drawAll_combined1D(file111_22_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_22_weight1_1123_dR10_btag","")
    #drawAll_combined1D(file111_22_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_111_22_weight4_1123_dR10_btag","")
    file111_allreco = "DiHiggs_WWbb_1M_B3_allReco_22_combined_v2.root"
    #drawAll_combined1D(file111_allreco,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_allreco_22_noweight_1123","")
    #drawAll_combined1D(file111_allreco,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_allreco_22_weight1_1123","")
    #drawAll_combined1D(file111_allreco,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_allreco_22_weight4_1123","")
    #drawAll_combined1D(file111_allreco,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_allreco_22_noweight_h2tohh_1123","H#rightarrow WWbb#rightarrow llbb in Gen")
    #drawAll_combined1D(file111_allreco,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_allreco_22_weight1_h2tohh_1123","H#rightarrow WWbb#rightarrow llbb in Gen")
    #drawAll_combined1D(file111_allreco,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_allreco_22_weight4_h2tohh_1123","H#rightarrow WWbb#rightarrow llbb in Gen")
    #file100_22_dR10 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_dR10_btag_combined_v2.root"
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_100_22_noweight_dR10_btag_1123","")
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_100_22_weight1_dR10_btag_1123","")
    #drawAll_combined1D(file100_22_dR10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_100_22_weight4_dR10_btag_1123","")
    """
    file_allreco_25 = "DiHiggs_WWbb_1M_B3_allReco_25_combined.root"
    drawAll_combined1D(file_allreco_25,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2mass_prob>200","MMC_allreco_25_noweight_Nosimulation_recoc1pdf_1123","")
    drawAll_combined1D(file_allreco_25,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight1_prob>200","MMC_allreco_25_weight1_Nosimulation_recoc1pdf_1123","")
    drawAll_combined1D(file_allreco_25,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight4_prob>200","MMC_allreco_25_weight4_Nosimulation_recoc1pdf_1123","")
    file110_22_Nosimulation_recoc1pdf = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_Notsimulation_recoc1pdf_combined.root"
    drawAll_combined1D(file110_22_Nosimulation_recoc1pdf,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2mass_prob>200","MMC_110_22_noweight_Nosimulation_recoc1pdf_1123","")
    drawAll_combined1D(file110_22_Nosimulation_recoc1pdf,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight1_prob>200","MMC_110_22_weight1_Nosimulation_recoc1pdf_1123","")
    drawAll_combined1D(file110_22_Nosimulation_recoc1pdf,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight4_prob>200","MMC_110_22_weight4_Nosimulation_recoc1pdf_1123","")
    file110_22_Nosimulation_energeticbjets_recoc1pdf = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_Notsimulation_energeticbjets_recoc1pdf_combined.root"
    drawAll_combined1D(file110_22_Nosimulation_energeticbjets_recoc1pdf,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2mass_prob>200","MMC_110_22_noweight_Nosimulation_energeticbjets_recoc1pdf_1123","")
    drawAll_combined1D(file110_22_Nosimulation_energeticbjets_recoc1pdf,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight1_prob>200","MMC_110_22_weight1_Nosimulation_energeticbjets_recoc1pdf_1123","")
    drawAll_combined1D(file110_22_Nosimulation_energeticbjets_recoc1pdf,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight4_prob>200","MMC_110_22_weight4_Nosimulation_energeticbjets_recoc1pdf_1123","")
    """
    file110_22_Nosimulation = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_Notsimulation_combined.root"
    #drawAll_combined1D(file110_22_Nosimulation,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2mass_prob>200","MMC_110_22_noweight_Nosimulation_1123","")
    #drawAll_combined1D(file110_22_Nosimulation,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight1_prob>200","MMC_110_22_weight1_Nosimulation_1123","")
    #drawAll_combined1D(file110_22_Nosimulation,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight4_prob>200","MMC_110_22_weight4_Nosimulation_1123","")
    file110_22_Nosimulation_energeticbjets = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_Notsimulation_energeticbjets_combined.root"
    #drawAll_combined1D(file110_22_Nosimulation_energeticbjets,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2mass_prob>200","MMC_110_22_noweight_Nosimulation_energeticbjets_2bjets_1123","")
    #drawAll_combined1D(file110_22_Nosimulation_energeticbjets,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight1_prob>200","MMC_110_22_weight1_Nosimulation_energeticbjets_2bjets_1123","")
    #drawAll_combined1D(file110_22_Nosimulation_energeticbjets,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight4_prob>200","MMC_110_22_weight4_Nosimulation_energeticbjets_2bjets_1123","")
    file110_22 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_combined.root"
    #drawAll_combined1D(file110_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2mass_prob>200","MMC_110_22_noweight_dR1_1123","")
    #drawAll_combined1D(file110_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight1_prob>200","MMC_110_22_weight1_dR1_1123","")
    #drawAll_combined1D(file110_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&& MMC_h2massweight4_prob>200","MMC_110_22_weight4_dR1_1123","")
    file111_energeticbjets = "DiHiggs_WWbb_1M_B3_allReco_22_energeticbjets_combined.root"
    #drawAll_combined1D(file111_energeticbjets,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_allreco_22_noweight_1118_energeticbjets_2bjets","")
    #drawAll_combined1D(file111_energeticbjets,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_allreco_22_weight1_1118_energeticbjets_2bjets","")
    #drawAll_combined1D(file111_energeticbjets,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_allreco_22_weight4_1118_energeticbjets_2bjets","")
    file111_22 = "DiHiggs_WWbb_1M_B3_111_22_MMC1M_combined_v2.root"
    #drawAll_combined1D(file111_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_22_noweight_1118_v2_dR1","")
    #drawAll_combined1D(file111_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_22_weight1_1118_v2_dR1","")
    #drawAll_combined1D(file111_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_111_22_weight4_1118_v2_dR1","")
    file000_n1n1 = "DiHiggs_WWbb_1M_B3_000_n1n1_MMC1M_1214_combined.root"
    #drawAll_combined1D(file000_n1n1,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_000_n1n1_noweight_1214_selectionsgencut_dR04_hasMET","")
    #drawAll_combined1D(file000_n1n1,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_000_n1n1_weight1_1214_selectiongencut_dR04_hasMET","")
    """
    fileantik4_000_n13 = "DiHiggs_WWbb_1M_B3_antik4_JetNoNu_000_n13_MMC1M_combined.root"
    drawAll_combined1D(fileantik4_000_n13,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_000_n13_noweight_1110","")
    drawAll_combined1D(fileantik4_000_n13,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_000_n13_weight1_1110","")
    drawAll_combined1D(fileantik4_000_n13,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_000_n13_weight4_1110","")
    fileantik4_000_03 = "DiHiggs_WWbb_1M_B3_JetNoNu_antik4_MuPt10_000_03_MMC1M_1110_combined.root"
    drawAll_combined1D(fileantik4_000_03,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_000_03_noweight_1110","")
    drawAll_combined1D(fileantik4_000_03,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_000_03_weight1_1110","")
    drawAll_combined1D(fileantik4_000_03,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_000_03_weight4_1110","")
    fileantik4_000_13 = "DiHiggs_WWbb_1M_B3_JetNoNu_antik4_MuPt10_000_13_MMC1M_1110_combined.root"
    drawAll_combined1D(fileantik4_000_03,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_000_13_noweight_1110","")
    drawAll_combined1D(fileantik4_000_03,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_000_13_weight1_1110","")
    drawAll_combined1D(fileantik4_000_03,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_000_13_weight4_1110","")
    fileantik4_000_23 = "DiHiggs_WWbb_1M_B3_JetNoNu_antik4_MuPt10_000_23_MMC1M_1110_combined.root"
    drawAll_combined1D(fileantik4_000_23,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_000_23_noweight_1110","")
    drawAll_combined1D(fileantik4_000_23,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_000_23_weight1_1110","")
    drawAll_combined1D(fileantik4_000_23,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_000_23_weight4_1110","")
    fileantik4_000_11 = "DiHiggs_WWbb_1M_B3_JetNoNu_antik4_MuPt10_000_11_MMC1M_1110_combined.root"
    drawAll_combined1D(fileantik4_000_11,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_000_11_noweight_1110","")
    drawAll_combined1D(fileantik4_000_11,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_000_11_weight1_1110","")
    drawAll_combined1D(fileantik4_000_11,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_000_11_weight4_1110","")
    fileantik4_000_22 = "DiHiggs_WWbb_1M_B3_JetNoNu_antik4_MuPt10_000_22_MMC1M_1110_combined.root"
    drawAll_combined1D(fileantik4_000_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_antik4_JetNoNu_000_22_noweight_1110","")
    drawAll_combined1D(fileantik4_000_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_antik4_JetNoNu_000_22_weight1_1110","")
    drawAll_combined1D(fileantik4_000_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_antik4_JetNoNu_000_22_weight4_1110","")
    file000_n10 = "DiHiggs_WWbb_1M_B3_antik4_JetNoNu_000_n10_MMC1M_bugfixed_combined.root"
    drawAll_combined1D(file000_n10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_n10_noweight_1110","")
    drawAll_combined1D(file000_n10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_n10_weight1_bugfixed_1110","")
    drawAll_combined1D(file000_n10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_n10_weight4_bugfixed_1110","")
    file000_n11 = "DiHiggs_WWbb_1M_B3_antik4_JetNoNu_000_n11_MMC1M_bugfixed_combined.root"
    drawAll_combined1D(file000_n11,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_n11_noweight_bugfixed_1102","")
    drawAll_combined1D(file000_n11,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_n11_weight1_bugfixed_1110","")
    drawAll_combined1D(file000_n11,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_n11_weight4_bugfixed_1110","")
    file000_n12 = "DiHiggs_WWbb_1M_B3_antik4_JetNoNu_000_n12_MMC1M_bugfixed_combined.root"
    drawAll_combined1D(file000_n12,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_n12_noweight_bugfixed_1102","")
    drawAll_combined1D(file000_n12,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_n12_weight1_bugfixed_1110","")
    drawAll_combined1D(file000_n12,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_n12_weight4_bugfixed_1110","")
    runMMCok = "runMMCok && preselections && preselections_gen"
    file100_22 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_combined.root"
    drawAll_combined1D(file100_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_100_22_noweight_1116","")
    drawAll_combined1D(file100_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_100_22_weight1_1116","")
    drawAll_combined1D(file100_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_100_22_weight4_1116","")

    file010_22 = "DiHiggs_WWbb_1M_B3_010_22_MMC1M_combined.root"
    drawAll_combined1D(file010_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_010_22_noweight_1116","")
    drawAll_combined1D(file010_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_010_22_weight1_1116","")
    drawAll_combined1D(file010_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_010_22_weight4_1116","")

    file001_22 = "DiHiggs_WWbb_1M_B3_001_22_MMC1M_combined.root"
    drawAll_combined1D(file001_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_001_22_noweight_1116","")
    drawAll_combined1D(file001_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_001_22_weight1_1116","")
    drawAll_combined1D(file001_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_001_22_weight4_1116","")

    file110_22 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_combined.root"
    drawAll_combined1D(file110_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2mass_prob>200","MMC_110_22_noweight_1116","")
    drawAll_combined1D(file110_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight1_prob>200","MMC_110_22_weight1_1116","")
    drawAll_combined1D(file110_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight4_prob>200","MMC_110_22_weight4_1116","")

    file101_22 = "DiHiggs_WWbb_1M_B3_101_22_MMC1M_combined.root"
    drawAll_combined1D(file101_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2mass_prob>200","MMC_101_22_noweight_1116","")
    drawAll_combined1D(file101_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight1_prob>200","MMC_101_22_weight1_1116","")
    drawAll_combined1D(file101_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight4_prob>200","MMC_101_22_weight4_1116","")

    file011_22 = "DiHiggs_WWbb_1M_B3_011_22_MMC1M_combined.root"
    drawAll_combined1D(file011_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2mass_prob>200","MMC_011_22_noweight_1116","")
    drawAll_combined1D(file011_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight1_prob>200","MMC_011_22_weight1_1116","")
    drawAll_combined1D(file011_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight4_prob>200","MMC_011_22_weight4_1116","")

    file111_22 = "DiHiggs_WWbb_1M_B3_111_22_MMC1M_combined.root"
    drawAll_combined1D(file111_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_22_noweight_1116","")
    drawAll_combined1D(file111_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_22_weight1_1116","")
    drawAll_combined1D(file111_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_111_22_weight4_1116","")
    #draw1D_combined(file,dir,nu1_pt,nu2_pt, onshell_nupt_bins,"Simulated p_{T#nu}^{onshellW}",onshellW_1_cut,offshellW_1_cut,"onshell_nupt_1M_mediateStates_0325")
   
    runMMCok = "h2tohh && preselections && hasgenb1jet && hasgenb2jet && hasGenMET"
    file000_n1n1 = "DiHiggs_WWbb_1M_B3_000_n1n1_MMC1M_1105_combined.root"
    drawAll_combined1D(file000_n1n1,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_000_n1n1_noweight_1102","")
    drawAll_combined1D(file000_n1n1,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_000_n1n1_weight1_1102","")
    file000_0n1 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_0n1_MMC1M_combined.root"
    drawAll_combined1D(file000_0n1,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_0n1_noweight_1102","")
    drawAll_combined1D(file000_0n1,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_0n1_weight1_1102","")
    drawAll_combined1D(file000_0n1,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_0n1_weight4_1102","")
    file000_1n1 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_1n1_MMC1M_combined.root"
    drawAll_combined1D(file000_1n1,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_1n1_noweight_1102","")
    drawAll_combined1D(file000_1n1,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_1n1_weight1_1102","")
    drawAll_combined1D(file000_1n1,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_1n1_weight4_1102","")
    file000_2n1 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_2n1_MMC1M_combined.root"
    drawAll_combined1D(file000_2n1,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_2n1_noweight_1102","")
    drawAll_combined1D(file000_2n1,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_2n1_weight1_1102","")
    drawAll_combined1D(file000_2n1,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_2n1_weight4_1102","")
    file000_n10 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_n10_MMC1M_combined.root"
    drawAll_combined1D(file000_n10,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_n10_noweight_1102","")
    drawAll_combined1D(file000_n10,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_n10_weight1_1102","")
    drawAll_combined1D(file000_n10,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_n10_weight4_1102","")
    file000_n11 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_n11_MMC1M_combined.root"
    drawAll_combined1D(file000_n11,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_n11_noweight_1102","")
    drawAll_combined1D(file000_n11,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_n11_weight1_1102","")
    drawAll_combined1D(file000_n11,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_n11_weight4_1102","")
    file000_n12 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_n12_MMC1M_combined.root"
    drawAll_combined1D(file000_n12,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_n12_noweight_1102","")
    drawAll_combined1D(file000_n12,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_n12_weight1_1102","")
    drawAll_combined1D(file000_n12,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_n12_weight4_1102","")
    fileJetNoNu000_22 = "DiHiggs_WWbb_1M_B3_JetNoNu_000_22_MMC1M_combined.root"
    drawAll_combined1D(fileJetNoNu000_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_JetNoNu_000_22_noweight_1102","")
    drawAll_combined1D(fileJetNoNu000_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(20,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_JetNoNu_000_22_weight1_1102","")
    drawAll_combined1D(fileJetNoNu000_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_JetNoNu_000_22_weight4_1102","")
    
    file000_22 = "DiHiggs_WWbb_1M_B3_000_22_MMC1M_combined.root"
    drawAll_combined1D(file000_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_000_22_noweight_1102","")
    drawAll_combined1D(file000_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_000_22_weight1_1102","")
    drawAll_combined1D(file000_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_000_22_weight4_1102","")
    file100_22 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_combined.root"
    drawAll_combined1D(file100_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_100_22_noweight_1102","")
    drawAll_combined1D(file100_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_100_22_weight1_1102","")
    drawAll_combined1D(file100_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_100_22_weight4_1102","")

    file010_22 = "DiHiggs_WWbb_1M_B3_010_22_MMC1M_combined.root"
    drawAll_combined1D(file010_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_010_22_noweight_1102","")
    drawAll_combined1D(file010_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_010_22_weight1_1102","")
    drawAll_combined1D(file010_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_010_22_weight4_1102","")

    file001_22 = "DiHiggs_WWbb_1M_B3_001_22_MMC1M_combined.root"
    drawAll_combined1D(file001_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_001_22_noweight_1102","")
    drawAll_combined1D(file001_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_001_22_weight1_1102","")
    drawAll_combined1D(file001_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_001_22_weight4_1102","")

    file110_22 = "DiHiggs_WWbb_1M_B3_110_22_MMC1M_combined.root"
    drawAll_combined1D(file110_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2mass_prob>200","MMC_110_22_noweight_1102","")
    drawAll_combined1D(file110_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight1_prob>200","MMC_110_22_weight1_1102","")
    drawAll_combined1D(file110_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]","h2tohh && preselections && MMC_h2massweight4_prob>200","MMC_110_22_weight4_1102","")

    file111_22 = "DiHiggs_WWbb_1M_B3_111_22_MMC1M_combined.root"
    drawAll_combined1D(file111_22,"evtree", "MMC_h2mass_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2mass_prob>=250","MMC_111_22_noweight_1102","")
    drawAll_combined1D(file111_22,"evtree", "MMC_h2massweight1_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight1_prob>=250","MMC_111_22_weight1_1102","")
    drawAll_combined1D(file111_22,"evtree", "MMC_h2massweight4_prob","h2tohh_mass","(200,250,450)","MMC reconstruction m_{H}[GeV/c^{2}]",runMMCok+"&&MMC_h2massweight4_prob>=250","MMC_111_22_weight4_1102","")
    #draw1D_combined(file,dir,nu1_pt,nu2_pt, onshell_nupt_bins,"Simulated p_{T#nu}^{onshellW}",onshellW_1_cut,offshellW_1_cut,"onshell_nupt_1M_mediateStates_0325")
    delta_phi = "(25,-3.1415,3.1415)"
    delta_eta = "(50,-5.0,5.0)"
    #deltaR1(file,dir,delta_eta,delta_phi,h2toh1h1_cut,"h2toh1h1_0223")
    #deltaR2(file,dir,delta_eta,delta_phi,h2toh1h1_cut,"h2toh1h1_0223")
    #file1 = "DiHiggs_WWbb_1M_B3_100_22_MMC1M_combined.root"
    file1 = "DiHiggs_WWbb_1M_B3_btagtest_1020_JetNoNu_combined.root"
    file2 = "DiHiggs_run2_PU0_cuts_1M_filter_B3_1105_fordelphes.root"
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genmet","met","h2tohh","h2tohh","delphes", "CMSSW","(50,0,150)","perfect detector #slash{E}_{T}", "genmet_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genmet","met","h2tohh && hasMET","h2tohh && hasMET","delphes", "CMSSW","(50,0,150)","perfect detector #slash{E}_{T}", "genmet_hasMET_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb1jet_pt","b1jet_pt","hasgenb1jet && h2tohh && dR_b1jet<0.4","hasb1jet && dR_b1jet<=0.4","delphes", "CMSSW","(60,0,240)","b1jet p_{T}", "b1jetpt_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb2jet_pt","b2jet_pt","hasgenb2jet && h2tohh&& dR_b2jet<0.4","hasb2jet&& dR_b2jet<0.4","delphes", "CMSSW","(60,0,240)","b2jet p_{T}", "b2jetpt_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb1jet_eta","b1jet_eta","hasgenb1jet && h2tohh && dR_b1jet<=0.4","hasb1jet && dR_b1jet<=0.4","delphes", "CMSSW","(50,-2.5,2.5)","b1jet #eta", "b1eta_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb2jet_eta","b2jet_eta","hasgenb2jet && h2tohh && dR_b2jet<=0.4","hasb2jet && dR_b2jet<=0.4","delphes", "CMSSW","(50,-2.5,2.5)","b2jet #eta", "b2eta_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb1jet_pt*(genb1jet_pt>genb2jet_pt)+genb2jet_pt*(genb2jet_pt>genb1jet_pt)","b1jet_pt*(b1jet_pt>b2jet_pt)+b2jet_pt*(b2jet_pt>b1jet_pt)","h2tohh && hasgenb1jet && hasgenb2jet && dR_b1jet<=0.4 && dR_b2jet<=0.4","hasb1jet && hasb2jet && dR_b1jet<=0.4 && dR_b2jet<=0.4","delphes", "CMSSW","(60,0,240)","leading bjet p_{T}", "leadingbjetpt_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb2jet_pt*(genb1jet_pt>genb2jet_pt)+genb1jet_pt*(genb2jet_pt>genb1jet_pt)","b2jet_pt*(b1jet_pt>b2jet_pt)+b1jet_pt*(b2jet_pt>b1jet_pt)","h2tohh && hasgenb1jet && hasgenb2jet && dR_b1jet<=0.4 && dR_b2jet<=0.4","hasb1jet && hasb2jet && dR_b1jet<=0.4 && dR_b2jet<=0.4","delphes", "CMSSW","(60,0,240)","subleading bjet p_{T}", "subbjetpt_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb1jet_eta*(genb1jet_pt>genb2jet_pt)+genb2jet_eta*(genb2jet_pt>genb1jet_pt)","b1jet_eta*(b1jet_pt>b2jet_pt)+b2jet_eta*(b2jet_pt>b1jet_pt)","h2tohh && hasgenb1jet && hasgenb2jet && dR_b1jet<=0.4 && dR_b2jet<=0.4","hasb1jet && hasb2jet && dR_b1jet<=0.4 && dR_b2jet<=0.4","delphes", "CMSSW","(50,-2.5,2.5)","leading bjet #eta", "leadingbjeteta_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","genb2jet_eta*(genb1jet_pt>genb2jet_pt)+genb1jet_eta*(genb2jet_pt>genb1jet_pt)","b2jet_eta*(b1jet_pt>b2jet_pt)+b1jet_eta*(b2jet_pt>b1jet_pt)","h2tohh && hasgenb1jet && hasgenb2jet&& dR_b1jet<=0.4 && dR_b2jet<=0.4","hasb1jet && hasb2jet&& dR_b1jet<=0.4 && dR_b2jet<=0.4","delphes", "CMSSW","(50,-2.5,2.5)","subleading bjet #eta", "subbjeteta_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","b1_pt","b1_pt","h2tohh","h2tohh","delphes", "CMSSW","(60,0,240)","b quark p_{T}", "genb1pt_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","b2_pt","b2_pt","h2tohh","h2tohh","delphes", "CMSSW","(60,0,240)","#bar{b} quark p_{T}", "genb2pt_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","b1_eta","b1_eta","h2tohh","h2tohh","delphes", "CMSSW","(50,-2.5,2.5)","b quark #eta", "genb1eta_1105_JetNoNu") 
    draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","b2_eta","b2_eta","h2tohh","h2tohh","delphes", "CMSSW","(50,-2.5,2.5)","#bar{b} quark #eta", "genb2eta_1105_JetNoNu") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu1_pt","l1_pt","h2tohh","h2tohh","delphes", "CMSSW","(60,0,180)","muon1 p_{T}", "genmu1pt_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu2_pt","l2_pt","h2tohh","h2tohh","delphes", "CMSSW","(60,0,180)","muon2 p_{T}", "genmu2pt_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu1_eta","l1_eta","h2tohh&& abs(mu1_eta)<=2.4 && abs(mu2_eta)<=2.4","h2tohh","delphes", "CMSSW","(50,-2.5,2.50)","muon1 #eta", "genmu1eta_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu2_eta","l2_eta","h2tohh&& abs(mu1_eta)<=2.4 && abs(mu2_eta)<=2.4","h2tohh","delphes", "CMSSW","(50,-2.5,2.50)","muon2 #eta", "genmu2eta_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu1_pt*(mu1_pt>mu2_pt)+mu2_pt*(mu2_pt>mu1_pt)","l1_pt*(l1_pt>l2_pt)+l2_pt*(l1_pt<l2_pt)","h2tohh","h2tohh","delphes", "CMSSW","(60,0,180)","leading muon p_{T}", "leadinggenmupt_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu2_pt*(mu1_pt>mu2_pt)+mu1_pt*(mu2_pt>mu1_pt)","l2_pt*(l1_pt>l2_pt)+l1_pt*(l1_pt<l2_pt)","h2tohh","h2tohh","delphes", "CMSSW","(60,0,180)","subleading muon p_{T}", "subgenmupt_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu1_eta*(mu1_pt>mu2_pt)+mu2_eta*(mu2_pt>mu1_pt)","l1_eta*(l1_pt>l2_pt)+l2_eta*(l1_pt<l2_pt)","h2tohh && abs(mu1_eta)<=2.4 && abs(mu2_eta)<=2.4","h2tohh && abs(l1_eta)<=2.4 && abs(l2_eta)<=2.4","delphes", "CMSSW","(50,-2.5,2.50)","leading muon #eta", "leadinggenmueta_1105") 
    #draw1D_delphes_cmssw(file1,"evtree", file2,"DiHiggsWWAna/evtree","mu2_eta*(mu1_pt>mu2_pt)+mu1_eta*(mu2_pt>mu1_pt)","l2_eta*(l1_pt>l2_pt)+l1_eta*(l1_pt<l2_pt)","h2tohh&& abs(mu1_eta)<=2.4 && abs(mu2_eta)<=2.4","h2tohh && abs(l1_eta)<=2.4 && abs(l2_eta)<=2.4 ","delphes", "CMSSW","(50,-2.5,2.50)","subleading muon #eta", "subgenmueta_1105") 
    #draw2D_combined(file,dir, onoffshellWmass2, onoffshellWmass1, wmass_onshell_bins,wmass_offshell_bins,"Simulated M_{W}^{onshell}","Simulated M_{W}^{offshell}",onshellW_1_cut,offshellW_1_cut,"onshellVsoffshell_Wmass_1M_mediateStates_0325")   
   """

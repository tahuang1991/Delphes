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
def draw1D_combined(file,dir,todraw1,todraw2,x_bins,x_title,cut1,cut2, pic_name):
    
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
    
    b2 = ROOT.TH1F("b2","b2",xBins,xminBin,xmaxBin)
    b2.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b2.GetYaxis().SetTitle("Events")
    b2.GetXaxis().SetTitle("%s"%x_title)
    t.Draw(todraw2+">>b2",cut2)
    #b1.SetStats(0)
    #wmassout = ROOT.TFile("onshellwmassout.root","recreate")
    #wmassout.cd()
    b1 = ROOT.TH1F("onshellWmasspdf","b1",xBins,xminBin,xmaxBin)
    b1.SetTitle("h2#rightarrow hh#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("Events")
    b1.GetXaxis().SetTitle("%s"%x_title)
    #b1.SetStats(0)


    #ROOT.gStyle.SetOptFit(1)
    t.Draw(todraw1+">>onshellWmasspdf",cut1)
    mean = 80.1
    signma = 1.5
    myfun = ROOT.TF1("myfun","exp(x*[0]+[1])+[2]*exp(-0.5*((x-80.1)/2.00)**2)",xminBin,xmaxBin)
    b1.Add(b2)
    b1.Sumw2()
    integral = b1.Integral("width")
    
    print "b1 integral ", integral," max",b1.GetBinContent(b1.GetMaximumBin())
    b1.Scale(1/integral)
    b1.Fit("pol6")
    #st =ROOT.TPaveStats(b1.FindObject("stats"))
    #st.SetX1NDC(0.1)
    #st.SetX2NCD(0.4)
    #print " -1 bin: ",b1.FindBin(-1)," 1000, bin: ",b1.FindBin(1000)," GetnBin",b1.GetNbinsX()
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
    b1.Draw()
    #wmassout.Write()
    #wmassout.Close()
    
    c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.png")
    c1.SaveAs("Dihiggs_%s"%pic_name+"combined_B3.C")
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




#deltaeta, deltaphi distribution
#___________________________________________
def deltaR1(file,dir,x_bins,y_bins,cut,pic_name):
    
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
    b1.SetTitle("h2#rightarrow h1h1#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("#Delta#phi(#mu,#nu)")
    b1.GetXaxis().SetTitle("#Delta#eta(#mu,#nu)")
    b1.SetStats(0)
    
    todraw = "TVector2::Phi_mpi_pi(mu1_phi-nu1_phi):(mu1_eta-nu1_eta)>>b1"
    t.Draw(todraw,cut)
    b1.Draw("colz")
#    b2 = ROOT.TF2("b2","x^2+y^2",xminBin,xmaxBin,yminBin,ymaxBin)
 #   contour = [1,2,3,4]
    #list = [None]*3
    #list.append(1)
    #list.append(2)
    #list.append(3)
    #list = list[-3:]    
    #b2.SetContour(4, contour)
   # b2.SetContourLevel(0,1)
    #b2.SetContourLevel(1,2)
    #b2.SetContourLevel(2,3)
    #b2.SetContourLevel(3,4)
    #b2.Draw("CONT3 same")
    legend = ROOT.TLegend(0.15,0.46,0.45,0.64)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
#    legend.SetHeader("PU140,simTrack Pt(%s"%pt_min+",%s)"%pt_max)
#legend.AddEntry(e1,"","l")
#    legend.Draw("same")
 #   line1 = "PU140,simTrack Pt(%s"%pt_min+",%s)"%pt_max
  #  line2 = "98 dphicut %f"%dphi_cut
   # tex = ROOT.TLatex(0.15,0.45,line1)
    #tex.SetTextSize(0.05)
    #tex.SetNDC()
    #tex.Draw("same")
    #tex2 = ROOT.TLatex(0.15,0.35,line2)
    #tex2.SetTextSize(0.05)
    #tex2.SetNDC()
    #tex2.Draw("same")
	
    c1.SaveAs("Dihiggs_deltaR1_%s"%pic_name+"_B3.pdf")
    c1.SaveAs("Dihiggs_deltaR1_%s"%pic_name+"_B3.png")


#___________________________________________
def deltaR2(file,dir,x_bins,y_bins,cut,pic_name):
    
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
    b1.SetTitle("h2#rightarrow h1h1#rightarrow BBWW, B3"+" "*12 + "CMS Simulation Preliminary")
    b1.GetYaxis().SetTitle("#Delta#phi(#mu,#nu)")
    b1.GetXaxis().SetTitle("#Delta#eta(#mu,#nu)")
    b1.SetStats(0)
    
    todraw = "TVector2::Phi_mpi_pi(mu2_phi-nu2_phi):(mu2_eta-nu2_eta)>>b1"
    t.Draw(todraw,cut)
    b1.Draw("colz")
#    b2 = ROOT.TF2("b2","x^2+y^2",xminBin,xmaxBin,yminBin,ymaxBin)
 #   contour = [1,2,3,4]
    #list = [None]*3
    #list.append(1)
    #list.append(2)
    #list.append(3)
    #list = list[-3:]    
    #b2.SetContour(4, contour)
   # b2.SetContourLevel(0,1)
    #b2.SetContourLevel(1,2)
    #b2.SetContourLevel(2,3)
    #b2.SetContourLevel(3,4)
    #b2.Draw("CONT3 same")
    legend = ROOT.TLegend(0.15,0.46,0.45,0.64)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
#    legend.SetHeader("PU140,simTrack Pt(%s"%pt_min+",%s)"%pt_max)
#legend.AddEntry(e1,"","l")
#    legend.Draw("same")
 #   line1 = "PU140,simTrack Pt(%s"%pt_min+",%s)"%pt_max
  #  line2 = "98 dphicut %f"%dphi_cut
   # tex = ROOT.TLatex(0.15,0.45,line1)
    #tex.SetTextSize(0.05)
    #tex.SetNDC()
    #tex.Draw("same")
    #tex2 = ROOT.TLatex(0.15,0.35,line2)
    #tex2.SetTextSize(0.05)
    #tex2.SetNDC()
    #tex2.Draw("same")
	
    c1.SaveAs("Dihiggs_deltaR2_%s"%pic_name+"_B3.pdf")
    c1.SaveAs("Dihiggs_deltaR2_%s"%pic_name+"_B3.png")

#_____________________________________________________________________________
def drawAll_1D(dir, treename, todraw,x_bins,x_title,cut,pic_name, text):
    c1 = ROOT.TCanvas()
    c1.SetGridx()
    c1.SetGridy()
    c1.SetTickx()
    c1.SetTicky()

    xBins = int(x_bins[1:-1].split(',')[0])
    xminBin = float(x_bins[1:-1].split(',')[1])
    xmaxBin = float(x_bins[1:-1].split(',')[2])
    if not os.path.isdir(dir):
          print "ERROR: This is not a valid directory: ", inputDir
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
    b1.Draw() 
    print "chain ",chain, " b1 entries ",b1.GetEntries()
    #print "GetMaximumbin() ", b1.GetMaximumBin()," bincenter ",b1.GetBinCenter(b1.GetMaximumBin())
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")

    c1.SaveAs("Dihiggs_%s"%pic_name+"_logy_All_B3.pdf")
    c1.SaveAs("Dihiggs_%s"%pic_name+"_logy_All_B3.png")

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
          print "ERROR: This is not a valid directory: ", inputDir
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
    chain.Draw(truetodraw+">>e", cut)
   # c1.SetLogy()
    
    b1.Draw() 
    e.Draw("same")
    e.SetLineColor(ROOT.kRed)
    b1.GetYaxis().SetRangeUser(0,e.GetMaximum()+10)
    legend = ROOT.TLegend(0.25,0.7,0.45,0.82)
    legend.SetFillColor(ROOT.kWhite)
    legend.SetFillStyle(0)
    legend.AddEntry(b1,"Reconstructed ","l") 
    legend.AddEntry(e,"True ","l") 
    legend.Draw("same")
    #print "chain ",chain, " b1 entries ",b1.GetEntries()
    #print "GetMaximumbin() ", b1.GetMaximumBin()," bincenter ",b1.GetBinCenter(b1.GetMaximumBin())
    tex = ROOT.TLatex(0.15,0.45,text)
    tex.SetTextSize(0.04)
    tex.SetTextFont(42)
    tex.SetNDC()
    tex.Draw("same")

    c1.SaveAs("Dihiggs_combined_%s"%pic_name+"_All_B3.pdf")
    c1.SaveAs("Dihiggs_combined_%s"%pic_name+"_All_B3.pdf")




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
    b1.SetTitle("h2#rightarrow bbbarWW, B3"+" "*12 + "CMS Simulation Preliminary")
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
     
    filedir = "/fdata/hepx/store/user/taohuang/Hhh/Delphes_ana/"
    #file = "/fdata/hepx/store/user/taohuang/Hhh/DiHiggs_100k_correctnu_0324_B3.root"
    #filedir = "/fdata/hepx/store/user/taohuang/DiHiggs_run2_PU0_htobbana_cuts_750k_B3_V2/"
    #filedir = "/fdata/hepx/store/user/taohuang/DiHiggs_run2_PU0_htobbana_cuts_50k_B3_JetNoNu_V2/"
    #filedir = "/fdata/hepx/store/user/taohuang/DiHiggs_run2_PU0_cuts_1M_filter_B3_MMC_useMET_METcorrection_V2substract_hepx/"
    #filedir = "/home/taohuang/Herwig++/Delphes-3.2.0/diHiggs/testroot/"
    dir = "evtree"
    
    #buildTChain(filedir,dir,"/fdata/hepx/store/user/taohuang/Dihiggs_TChain.root") 
    htoWW_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz)**2)"
    
    
    htoWW_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz)**2)"
    hmass_bins = "(50,100,300)"
    hmass_bins1 = "(100,80,150)" 
    hmass_bins2 = "(50,0,200)" 
    htoWW_cut = "h2tohh" 
    hasjets_cut = "hasbjet && hasbbarjet"
    
    #drawAll_1D(filedir,dir,htoWW_mass,hmass_bins1,"reconstructed mass of h#rightarrow WW, reconstruction from(#mu#mu#nu#nu)", htoWW_cut,"htoWW_mass_1M_JetNoNu_0605","p_T>10, |#eta|<2.4")
    #drawAll_combined1D(filedir,dir,htoWW_mass,"htoWW_mass",hmass_bins1,"reconstructed mass of h#rightarrow WW, reconstruction from(#mu#mu#nu#nu)", htoWW_cut,"htoWW_mass_1M_JetNoNu_0625","p_T>10, |#eta|<2.4")
    bjet_pt = "sqrt(bjet_px**2+bjet_py**2)" 
    bbarjet_pt = "sqrt(bbarjet_px**2+bbarjet_py**2)" 
    htoBB_mass = "sqrt((b1_energy+b2_energy)**2-(b1_px+b2_px)**2-(b1_py+b2_py)**2-(b1_pz+b2_pz)**2)"
    htoBBJets_mass = "sqrt((bjet_energy+bbarjet_energy)**2-(bjet_px+bbarjet_px)**2-(bjet_py+bbarjet_py)**2-(bjet_pz+bbarjet_pz)**2)"
    htoBBJets_correction_mass = "sqrt(((bjet_energy+bbarjet_energy)*rescalefactor)**2-((bjet_px+bbarjet_px)*rescalefactor)**2-((bjet_py+bbarjet_py)*rescalefactor)**2-((bjet_pz+bbarjet_pz)*rescalefactor)**2)"
    htoBBJetstot_mass = "sqrt((bjet_energy_tot+bbarjet_energy_tot)**2-(bjet_px_tot+bbarjet_px_tot)**2-(bjet_py_tot+bbarjet_py_tot)**2-(bjet_pz_tot+bbarjet_pz_tot)**2)"
    htoBB_cut = "h2tohh"
    #drawAll_1D(filedir,dir,"dR_bjet","(50,0,2)","deltaR(bjet, b genParticle)", "1","dR_bjet_cuts_50k_0621_JetNoNu_V2","p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,"dR_bbarjet","(50,0,2)","deltaR(bbarjet, bbar genParticle)", "1","dR_bbarjet_cuts_50k_0621_JetNoNu_V2","p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,bjet_pt, "(100,0,200)","p#_T(bjet)","1","bjetPt_cuts_1M_0605_V3","p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,bbarjet_pt, "(100,0,200)","p#_T(bbarjet)","1","bbarjetPt_cuts_1M_0605_V3","p_{T}>30, |#eta|<2.5")
    #draw1D(file,dir,htoBB_mass,hmass_bins1,"reconstructed mass of h#rightarrow BB", htoBB_cut,"htoBB_mass_1M_mediateStates_0325")
    #drawAll_1D(filedir,dir,htoBBJets_mass,hmass_bins2,"reconstructed mass of h#rightarrow BB, from GenJet (closest)", "1","htoBBjets_cuts_mass_1M_0606_V4"," p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,htoBBJets_mass,hmass_bins2,"reconstructed mass of h#rightarrow BB, from GenJet (closest)", "1","htoBBjets_cuts_mass_1M_0606_V2"," p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,htoBBJets_mass,hmass_bins2,"reconstructed mass of h#rightarrow BB, from GenJet (closest)", "dR_bjet<0.1 && dR_bbarjet<0.1","htoBBjets_cuts_dR1_mass_50k_0625_JetNoNu","#splitline{jet reconstruction algo: ak4GenJetsNoNu}{#DeltaR(bjet)<0.1, #DeltaR(bbarjet)<0.1, p_{T}>30, |#eta|<2.5}")
    #drawAll_1D(filedir,dir,htoBBJets_correction_mass,hmass_bins2,"reconstructed mass of h#rightarrow BB, from GenJet (closest)", "dR_bjet<0.1 && dR_bbarjet<0.1 &&"+hasjets_cut,"htoBBjets_cuts_dR1_mass_50k_0625_correction_JetNoNu","#splitline{jet reconstruction: ak4GenJetsNoNu, after rescaling}{#DeltaR(bjet)<0.1, #DeltaR(bbarjet)<0.1, p_{T}>30, |#eta|<2.5}")
    drawAll_combined1D(filedir,dir,htoBBJets_mass,"htobb_mass",hmass_bins2,"reconstructed mass of h#rightarrow BB, from Jets (closest)", "dR_bjet<0.1 && dR_bbarjet<0.1","htoBBjets_cuts_dR1_delphestest_0702","#splitline{jet reconstruction by antik}{#DeltaR(bjet)<0.1, #DeltaR(bbarjet)<0.1, p_{T}>30, |#eta|<2.5}")
    #drawAll_combined1D(filedir,dir,htoBBJets_correction_mass,"htobb_mass",hmass_bins2,"reconstructed mass of h#rightarrow BB, from GenJet (closest)", "dR_bjet<0.1 && dR_bbarjet<0.1 &&"+hasjets_cut,"htoBBjets_cuts_dR1_mass_50k_0625_correction_JetNoNu","#splitline{jet reconstruction: ak4GenJetsNoNu, after rescaling}{#DeltaR(bjet)<0.1, #DeltaR(bbarjet)<0.1, p_{T}>30, |#eta|<2.5}")
    #drawAll_1D(filedir,dir,htoBBJetstot_mass,hmass_bins1,"reconstructed mass of h#rightarrow BB, from GenJet (sum)", htoBB_cut,"htoBBjetstot_mass_1M_0606")
   
    #drawAll_1D(filedir,dir,"bjet_decendant_energy/bjet_energy","(101,-0.005,1.005)","#frac{E_{b decendants}}{E_{bjet}}", "dR_bjet<0.1","decendantenergyratio_dR1_0605_V3", "#DeltaR<0.1,p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,"bjet_decendant_energy/bjet_energy","(101,-0.005,1.005)","#frac{E_{b decendants}}{E_{bjet}}", "dR_bjet>0.4","decendantenergyratio_dR2_0605_V3", "#DeltaR>0.4,p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,"bbarjet_decendant_energy/bbarjet_energy","(101,-0.005,1.005)","#frac{E_{b decendants}}{E_{bbarjet}}", "dR_bbarjet<0.1","decendantenergyratio_bjetdR1_0605_V3", "#DeltaR<0.1,p_{T}>30, |#eta|<2.5")
    #drawAll_1D(filedir,dir,"bbarjet_decendant_energy/bbarjet_energy","(101,-0.005,1.005)","#frac{E_{b decendant}}{E_{bbarjet}}", "dR_bbarjet>0.4","decendantenergyratio_bbarjetdR2_0605_V3", "#DeltaR>0.4,p_{T}>30, |#eta|<2.5")

    #drawAll_2D(filedir,dir,"(bjet_decendant_energy/bjet_energy):dR_bjet","(50,0,2)","(55,0,1.1)", "dR(b genParticle, b GenJet)", "#frac{E_{b decendants}}{E_{bjet}}","1","energyratioVsdR_bjet_50k_0621_JetNoNu_V2","p_{T}>30, |#eta|<2.5")

    #drawAll_2D(filedir,dir,"(sqrt(bjet_decendant_px**2+bjet_decendant_py**2)/sqrt(bjet_px**2+bjet_py**2)):dR_bjet","(50,0,2)","(51,-0.01,1.01)", "dR(b genParticle, b GenJet)", "#frac{p_T(b decendants)}{p_T(bjet)}","1","ptratioVsdR_bjet_0605_V3","p_{T}>30, |#eta|<2.5")

    #drawAll_2D(filedir,dir,"(sqrt(bbarjet_decendant_px**2+bbarjet_decendant_py**2)/sqrt(bbarjet_px**2+bbarjet_py**2)):dR_bjet","(50,0,2)","(51,-0.01,1.01)", "dR(bbar genParticle, bbar GenJet)", "#frac{p_T(bbar decendants)}{p_T(bbarjet)}","1","ptratioVsdR_bbarjet_0605_V3","p_{T}>30, |#eta|<2.5")


    h2toh1h1_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy+b1_energy+b2_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px+b1_px+b2_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py+b1_py+b2_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz+b1_pz+b2_pz)**2)"
    h2toh1h1Jets_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy+bjet_energy+bbarjet_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px+bjet_px+bbarjet_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py+bjet_py+bbarjet_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz+bjet_pz+bbarjet_pz)**2)"
    h2toh1h1Jets_correction_mass = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy+(bjet_energy+bbarjet_energy)*rescalefactor)**2-(mu1_px+mu2_px+nu1_px+nu2_px+(bjet_px+bbarjet_px)*rescalefactor)**2-(mu1_py+mu2_py+nu1_py+nu2_py+(bjet_py+bbarjet_py)*rescalefactor)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz+(bjet_pz+bbarjet_pz)*rescalefactor)**2)"
    h2toh1h1_cut = "h2tohh && dR_bjet<0.1 && dR_bbarjet<0.1 && hasMET"
    h2mass_bins_6 = "(200,400,600)"
    h2mass_bins_3 = "(100,250,450)"
    #drawAll_1D(filedir,dir,h2toh1h1_mass,h2mass_bins_3,"reconstructed mass of h2#rightarrow BB#mu#nu#mu#nu", "h2tohh","h2toh1h1_mass_1M_genp_0605_V4"," ")
    #drawAll_1D(filedir,dir,h2toh1h1Jets_mass,h2mass_bins_3,"reconstructed mass of h2#rightarrow BB#mu#nu#mu#nu", h2toh1h1_cut,"h2toh1h1_mass_1M_jets_0625_JetNoNu","#splitline{jets: #DeltaR<0.1, p_{T}>30, |#eta|<2.5}{muons: p_{T}>10, |#eta|<2.4}") 
    #drawAll_1D(filedir,dir,h2toh1h1Jets_correction_mass,h2mass_bins_3,"reconstructed mass of h2#rightarrow BB#mu#nu#mu#nu", h2toh1h1_cut+"&&"+hasjets_cut,"h2toh1h1_mass_1M_jets_0625_correction_JetNoNu","#splitline{jets: #DeltaR<0.1, p_{T}>30, |#eta|<2.5, after rescaling}{muons: p_{T}>10, |#eta|<2.4}") 

    #drawAll_combined1D(filedir,dir,h2toh1h1Jets_mass,"h2tohh_mass",h2mass_bins_3,"reconstructed mass of h2#rightarrow BB#mu#nu#mu#nu", h2toh1h1_cut,"h2toh1h1_mass_1M_jets_metcut_0625_JetNoNu","#splitline{jets:ak4GenjetsNoNu #DeltaR<0.1, p_{T}>30, |#eta|<2.5}{muons: p_{T}>10, |#eta|<2.4 and #slash{E}_{T}>20}") 
    #drawAll_combined1D(filedir,dir,h2toh1h1Jets_correction_mass,"h2tohh_mass",h2mass_bins_3,"reconstructed mass of h2#rightarrow BB#mu#nu#mu#nu", h2toh1h1_cut,"h2toh1h1_mass_1M_jets_metcut_0625_correction_JetNoNu","#splitline{jets:ak4GenJetsNoNu #DeltaR<0.1, p_{T}>30, |#eta|<2.5, after rescaling}{muons: p_{T}>10, |#eta|<2.4 and #slash{E}_{T}>20}") 
    #drawAll_combined1D(filedir,dir,h2toh1h1Jets_correction_mass,"h2tohh_mass",h2mass_bins_3,"reconstructed mass of h2#rightarrow BB#mu#nu#mu#nu", h2toh1h1_cut+"&&"+hasjets_cut,"h2toh1h1_mass_1M_jets_0625_correction_JetNoNu","#splitline{jets: #DeltaR<0.1, p_{T}>30, |#eta|<2.5, after rescaling}{muons: p_{T}>10, |#eta|<2.4}") 
    htoWW_mass1 = "sqrt((mu1_mother_energy+mu2_mother_energy)**2-(mu1_mother_px+mu2_mother_px)**2-(mu1_mother_py+mu2_mother_py)**2-(mu1_mother_pz+mu2_mother_pz)**2)"
    #draw1D(file,dir,htoWW_mass1,hmass_bins1," reconstructed mass of h#rightarrow WW, reconstruction from (WW)", htoWW_cut,"htoWW_median_mass_1M_mediateStates_0325")
    #draw1D(file,dir,"htoWW_mass",hmass_bins1,"true h mass from  h candidates in generation", htoWW_cut,"htoWW_mass_100k_Gen_0324")
    #draw1D(file,dir,"h2tohh_mass",h2mass_bins_3,"true h2 mass from h2 candidates in generation", htoWW_cut,"h2_mass_100k_Gen_0324")

    nu12_px = "(nu1_px+nu2_px)"
    nu12_py = "(nu1_py+nu2_py)"
    nu12_pt = "sqrt((nu1_px+nu2_px)**2+(nu1_py+nu2_py)**2)"
    MissET_projectionX = "(met_px*(bjet_px+bbarjet_px)+met_py*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    MissET_projectionY = "(met_py*(bjet_px+bbarjet_px)-met_px*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    MissET_projectionX_correction = "(met_correction_px*(bjet_px+bbarjet_px)+met_correction_py*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    MissET_projectionY_correction = "(met_correction_py*(bjet_px+bbarjet_px)-met_correction_px*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    nu12_projectionX = "((nu1_px+nu2_px)*(bjet_px+bbarjet_px)+(nu1_py+nu2_py)*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"
    nu12_projectionY = "((nu1_py+nu2_py)*(bjet_px+bbarjet_px)-(nu1_px+nu2_px)*(bjet_py+bbarjet_py))/sqrt((bjet_px+bbarjet_px)**2+(bjet_py+bbarjet_py)**2)"

    #drawAll_2D(filedir,dir,nu12_px+":met_px","(50,-100,100)","(50,-100,100)","#slash{E}_{T,x}", "nu1_px+nu2_px","h2tohh && hasMET","metpx_0629_1M_afterfilter_metcut","#slash{E}_{T}>20")
    #drawAll_2D(filedir,dir,nu12_py+":met_py","(50,-100,100)","(50,-100,100)","#slash{E}_{T,y}","nu1_py+nu2_py","h2tohh && hasMET","metpy_0629_1M_afterfilter_metcut","#slash{E}_{T}>20 ")
    #drawAll_2D(filedir,dir,nu12_pt+":met","(50,0,200)","(50,0,200)","#slash{E}_{T}","#sqrt{(nu1_px+nu2_px)**2+(nu1_py+nu2_py)**2}","h2tohh && hasMET","met_0629_1M_afterfilter_metcut","#slash{E}_{T}>20")
   #METcorrection 
    #drawAll_2D(filedir,dir,nu12_px+":met_correction_px","(50,-100,100)","(50,-100,100)","#slash{E}_{T,x}", "nu1_px+nu2_px","h2tohh && hasMET","metpx_0629_1M_afterfilter_correction_metcut","#slash{E}_{T}>20, correction after bjets rescaling")
    #drawAll_2D(filedir,dir,nu12_py+":met_correction_py","(50,-100,100)","(50,-100,100)","#slash{E}_{T,y}","nu1_py+nu2_py","h2tohh && hasMET","metpy_0629_1M_afterfilter_correction_metcut","#slash{E}_{T}>20,correction after bjets rescaling ")
    #drawAll_2D(filedir,dir,nu12_pt+":met_correction","(50,0,200)","(50,0,200)","#slash{E}_{T}","#sqrt{(nu1_px+nu2_px)**2+(nu1_py+nu2_py)**2}","h2tohh && hasMET","met_0629_1M_afterfilter_correction_metcut","#slash{E}_{T}>20, correction after bjets rescaling")
    #drawAll_2D(filedir,dir,MissET_projectionY+":"+MissET_projectionX,"(50,-100,100)","(50,-100,100)","#slash{E} projection X", "#slash{E} projection Y","h2tohh && hasMET","projection_0629_1M_afterfilter_metcut","#slash{E}_{T}>20")
    #drawAll_2D(filedir,dir,MissET_projectionX_correction+":"+MissET_projectionX,"(50,-100,100)","(50,-100,100)","before correction", "after correction","h2tohh && hasMET","projectionX_0629_1M_afterfilter_metcut","#slash{E}_{T}>20")
    #drawAll_2D(filedir,dir,MissET_projectionY_correction+":"+MissET_projectionY,"(50,-100,100)","(50,-100,100)","before correction", "after correction","h2tohh && hasMET","projectionY_0629_1M_afterfilter_metcut","#slash{E}_{T}>20")
    #drawAll_2D(filedir,dir,MissET_projectionX_correction+":"+nu12_projectionX,"(50,-100,100)","(50,-100,100)","#vec{nu1+nu2} projection @ #hat{bjets} in X-Y plane","#slash{E}_{T} projection @ #hat{bjets} in X-Y plane","h2tohh && hasMET","nu12MET_projectionX_0629_1M_correction_V2_afterfilter_metcut","#slash{E}_{T}>20, after correction due to bjets rescaling ")
    #drawAll_2D(filedir,dir,MissET_projectionY_correction+":"+nu12_projectionY,"(50,-100,100)","(50,-100,100)","#vec{nu1+nu2} projection @ #hat{k}(#perp #hat{bjets}) in X-Y plane","#slash{E}_{T} projection @ #hat{k}(#perp #hat{bjets}) in X-Y plane","h2tohh && hasMET","nu12MET_projectionY_0629_1M_correction_V2_afterfilter_metcut","#slash{E}_{T}>20, after correction due to bjets rescaling")
#draw jets mass and draw h2 reconstruction mass from jets and muons neutrinos
    jets_mass_bins = "(200, 100,400)"
    #draw1D(file,dir,"jets_mass",jets_mass_bins,"invariant mass of all decendants from b+#bar{b}", htoWW_cut,"jets_mass_1M_mediateStates_0325")
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
#    draw1D_combined(file,dir,"mu1_mother_mass","mu2_mother_mass", wmass_onshell_bins,"Simulated M_{W}^{onshell}",onshellW_1_cut,offshellW_1_cut,"onshellW_mass_1M_mediateStates_0325")
    
    #draw1D_combined(file,dir,nu1_pt,nu2_pt, onshell_nupt_bins,"Simulated p_{T#nu}^{onshellW}",onshellW_1_cut,offshellW_1_cut,"onshell_nupt_1M_mediateStates_0325")
    delta_phi = "(25,-3.1415,3.1415)"
    delta_eta = "(50,-5.0,5.0)"
    #deltaR1(file,dir,delta_eta,delta_phi,h2toh1h1_cut,"h2toh1h1_0223")
    #deltaR2(file,dir,delta_eta,delta_phi,h2toh1h1_cut,"h2toh1h1_0223")
   
    onoffshellWmass1 = "mu1_mother_mass:mu2_mother_mass"
    onoffshellWmass2 = "mu2_mother_mass:mu1_mother_mass"
     
    #draw2D_combined(file,dir, onoffshellWmass2, onoffshellWmass1, wmass_onshell_bins,wmass_offshell_bins,"Simulated M_{W}^{onshell}","Simulated M_{W}^{offshell}",onshellW_1_cut,offshellW_1_cut,"onshellVsoffshell_Wmass_1M_mediateStates_0325")


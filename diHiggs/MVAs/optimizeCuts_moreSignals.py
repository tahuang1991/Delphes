#!/usr/bin/python
import os, ROOT
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut, TH1F
from optparse import OptionParser
print "===> Optimizing cuts for selecting heavy Higgs vs tt."

Signal = "B3"

#Parameters
signalFile           = ['../Output/delphes_B3_1M_PU0_Btag_Sim_noMMCnoMVA.root','../Output/delphes_B6_1M_PU0_Btag_Sim_noMMCnoMVA.root']
backgroundFile       = ['../Output/delphes_tt_1M_PU0_Wtobmu_Sim_noMMCnoMVA.root','../Output/delphes_tt_1M_PU0_Wtobmu_Sim_noMMCnoMVA.root']
signalFriendFile     = ""
backgroundFriendFile = ""
#presel        = '&& mass_b1b2<300 && dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons'
presel        = 'hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons && dR_l1l2<2.49'
cuts        = ['','']
outputs     = ['TMVA_B3_DRl1l2_Sim.root','TMVA_B6_DRl1l2_Sim.root']
weightDir = ['weights_B3_DRl1l2_Sim','weights_B6_DRl1l2_Sim']

# Loop on the category to be optimized
#for i in range(len(signalFile)):
for i in range(1,2):
    print "DOING ITERATION NUMBER: " + str(i)
    if len(signalFile) != len(outputs):
        raise RuntimeError('Cut set does not correspond to output set!')
    fullcut = presel + cuts[i]
    print "Now running optimization with Cut = " + fullcut + ". Output will be put in " + outputs[i] + "  and xml file in " + str(weightDir[i])
    #Loading files and entries before MVA
    inputSig = TFile.Open( signalFile[i] )
    inputBkg = TFile.Open( backgroundFile[i] )
    treeS    = inputSig.Get("evtree")
    treeB    = inputBkg.Get("evtree")
    NEntries_S = treeS.Draw("dR_l1l2>>hist",fullcut,"goff");
    NEntries_B = treeB.Draw("dR_l1l2>>hist",fullcut,"goff");
    #Get the corresponding weights
    hW = TH1F("hW","", 1000, 0., 10.) 
    treeS.Draw("weight>>hW",fullcut,"goff")
    signalWeight =  hW.GetMean()
    treeB.Draw("weight>>hW",fullcut,"goff")
    backgroundWeight = hW.GetMean()
    print "---> Signal: " + str(NEntries_S*signalWeight) + " the weight used is: " + str(signalWeight)
    print "---> backgr: " + str(NEntries_B*backgroundWeight) + " the weight used is: " + str(backgroundWeight)
    #Now the MVA
    print "Now starting seriously..."
    os.system('python CutsOptimization.py -a "' + fullcut + '" -o ' + outputs[i] + ' -i ' + signalFile[i] + ' -j ' + backgroundFile[i] + ' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('mv weights ' + weightDir[i])
        
print "DONE!!\n \n"

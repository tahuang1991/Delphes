#!/usr/bin/python
import os, ROOT
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut, TH1F
from optparse import OptionParser
print "===> Optimizing cuts for selecting Heavy Higgs vs tt."

Signal = "B3"

#Parameters
signalFile           = ['DiHiggs_WWbb_1M_B3_allReco_25_MMC1M_btaginvariantmass_0201_combined.root','../Output/delphes_B6_1M_PU0_Btag_Sim_noMMCnoMVA.root']
backgroundFile       = ['../Output/delphes_tt_1M_PU0_Wtobmu_Sim_noMMCnoMVA.root','../Output/delphes_tt_1M_PU0_Wtobmu_Sim_noMMCnoMVA.root']
signalFriendFile     = ""
backgroundFriendFile = ""
#presel        = '&& mass_b1b2<300 && dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons'
presel        = 'hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons && dR_l1l2<2.49'
cuts        = ['','']
outputs     = ['TMVA_B3_NoSim.root','TMVA_B6_DRl1l2_Sim.root']
weightDir = ['weights_B3_Nosim','weights_B6_DRl1l2_Sim']

# Loop on the category to be optimized
#for i in range(len(signalFile)):
for i in range(0,1):
    print "DOING ITERATION NUMBER: " + str(i)
    if len(signalFile) != len(outputs) or len(signalFile) != len(cuts) or len(signalFile) != len(weightDir) :
        raise RuntimeError('Problem in the lenght of your array!')
    fullcut = presel + cuts[i]
    print "Preselection is: " + fullcut
    print "Output will be in: " + outputs[i] + ", and xml file in " + str(weightDir[i])
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
    print "---> Backgr: " + str(NEntries_B*backgroundWeight) + " the weight used is: " + str(backgroundWeight)
    #Now the MVA
    print "Now starting serious things..."
    os.system('python CutsOptimization.py -a "' + fullcut + '" -o ' + outputs[i] + ' -i ' + signalFile[i] + ' -j ' + backgroundFile[i] + ' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('mv weights ' + weightDir[i])      
print "DONE!!\n \n"

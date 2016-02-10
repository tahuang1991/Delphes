#!/usr/bin/python
import os, ROOT
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut, TH1F
from optparse import OptionParser
print "===> Optimizing cuts for selecting Heavy Higgs vs tt."

Signal = "B3"

#Parameters
signalFile           = ['DiHiggs_WWbb_1M_B3_allReco_25_MMC1M_btaginvariantmass_0207_combined.root','DiHiggs_WWbb_1M_B6_allReco_25_MMC1M_btaginvariantmass_0207_combined.root']
backgroundFile       = ['TTbar_Wtobmu_allReco_25_20160207_combined.root','TTbar_Wtobmu_allReco_25_20160207_combined.root']
signalFriendFile     = ""
backgroundFriendFile = ""
#presel        = '&& mass_b1b2<300 && dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons'
#presel        = 'hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons'
presel        = '(hasb1jet || hasb2jet) && hasMET && hasdRljet && hastwomuons'
cuts        = ['&& event_n%2==0','&& event_n%2==0']
outputs     = ['TMVA_B3_NoSim_1btag_tmp2.root','TMVA_B6_NoSim_1btag_tmp2.root']
weightDir = ['weights_B3_Nosim_1btag_tmp2','weights_B6_Nosim_1btag_tmp2']

# Loop on the category to be optimized
for i in range(len(signalFile)):
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
    NEntries_S = treeS.Draw("dR_l1l2>>hist",presel,"goff");
    NEntries_B = treeB.Draw("dR_l1l2>>hist",presel,"goff");
    #Get the corresponding weights
    hW = TH1F("hW","", 1000, 0., 10.) 
    treeS.Draw("weight>>hW",fullcut,"goff")
    signalWeight =  hW.GetMean()
    treeB.Draw("weight>>hW",fullcut,"goff")
    backgroundWeight = hW.GetMean()
    if( i==1 ) :
       signalWeight = 0.00158119490325
    print "---> Signal: " + str(NEntries_S*signalWeight) + " the weight used is: " + str(signalWeight)
    print "---> Backgr: " + str(NEntries_B*backgroundWeight) + " the weight used is: " + str(backgroundWeight)
    #Now the MVA
    print "Now starting serious things..."
    os.system('python CutsOptimization.py -a "' + fullcut + '" -o ' + outputs[i] + ' -i ' + signalFile[i] + ' -j ' + backgroundFile[i] + ' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('mv weights ' + weightDir[i])      
print "DONE!!\n \n"

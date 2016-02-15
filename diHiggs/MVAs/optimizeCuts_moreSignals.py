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
#presel    = 'hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons'
presel    = '(hasb1jet || hasb2jet) && hasMET && hasdRljet && hastwomuons'
cuts      = ['','']
outputs   = ['TMVA_B3_NoSim_1btag','TMVA_B6_NoSim_1btag']
weightDir = ['weights_B3_Nosim_1btag','weights_B6_Nosim_1btag']
MVAS      = ['Likelihood','LikelihoodMIX','LikelihoodD','BDT','MLP','RuleFit','HMatrix']

# Loop on the category to be optimized
for i in range(len(signalFile)):
    print "DOING ITERATION NUMBER: " + str(i)
    if len(signalFile) != len(outputs) or len(signalFile) != len(cuts) or len(signalFile) != len(weightDir) :
        raise RuntimeError('Problem in the lenght of your array!')
    for iM in range(len(MVAS)):
        fullcut = presel + cuts[i]
        print "Preselection is: " + fullcut
        OutPut = outputs[i] + "_" + MVAS[iM] + '.root'
        Weight = weightDir[i] + "_" + MVAS[iM]
        print "Output will be in: " + OutPut + ", and xml file in " + str(Weight)
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
        if( MVAS[iM]=='MLP' or MVAS=='RuleFit' ):
          fullcut += ' && event_n%3!=0'
        os.system('python CutsOptimization.py -a "' + fullcut + '" -o ' + OutPut + ' -i ' + signalFile[i] + ' -j ' + backgroundFile[i] + ' -f ' + signalFriendFile+' -g ' + backgroundFriendFile+' -m ' + MVAS[iM] )
        os.system('rm -rf ' + Weight)
        os.system('mv weights ' + Weight)
print "DONE!!\n \n"

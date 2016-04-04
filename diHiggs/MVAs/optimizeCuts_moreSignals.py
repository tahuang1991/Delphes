#!/usr/bin/python
import os, ROOT
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut, TH1F
from optparse import OptionParser
print "===> Optimizing cuts for selecting Heavy Higgs vs tt."

signalFile           = ['../Output/delphes_B3_1M_PU40_ALL.root','../Output/delphes_B6_1M_PU40_ALL.root']
backgroundFile       = ['../Output/delphes_ttbar_1M_PU40_Wtobmu_ALL.root','../Output/delphes_ttbar_1M_PU40_Wtobmu_ALL.root']
signalFriendFile     = ""
backgroundFriendFile = ""

#presel    = 'preselections && (hasb1jet || hasb2jet) && dR_l1l2<3.3 && dR_b1b2<3.3&& TMath::Abs(dphi_l1l2b1b2)>1.&& mass_l1l2<75 && mass_b1b2<200 && mass_b1b2>50'
presel    = 'preselections && (hasb1jet || hasb2jet) && dR_l1l2<3.3 && dR_b1b2<3.3&& TMath::Abs(dphi_l1l2b1b2)>1.&& mass_l1l2<75 && mass_b1b2<200 && mass_b1b2>50 && mass_l1l2>5. && dR_l1l2>0.07'
cuts      = ['','']
outputs   = ['TMVA_B3_RECO_1btag_40PU','TMVA_B6_RECO_1btag_40PU']
weightDir = ['weights_B3_RECO_1btag_40PU','weights_B6_RECO_1btag_40PU']
#MVAS      = ['Likelihood','LikelihoodMIX','LikelihoodD','BDT','MLP','RuleFit','HMatrix']
MVAS      = ['MLPBFGS','MLPBNN']

# Loop on the category to be optimized
#for i in range(len(signalFile)):
for i in range(1):
    print "DOING ITERATION NUMBER: " + str(i)
    if len(signalFile) != len(outputs) or len(signalFile) != len(cuts) or len(signalFile) != len(weightDir) :
        raise RuntimeError('ERROR::Problem in the lenght of your array!')
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
        hW = TH1F("hW","", 1000, 0., 50.) 
        treeS.Draw("weight>>hW",fullcut,"goff")
        signalWeight =  hW.GetMean()
        treeB.Draw("weight>>hW",fullcut,"goff")
        backgroundWeight = hW.GetMean()
        print "---> Signal: " + str(NEntries_S*signalWeight) + " the weight used is: " + str(signalWeight)
        print "---> Backgr: " + str(NEntries_B*backgroundWeight) + " the weight used is: " + str(backgroundWeight)
        #Now the MVA
        print "Now starting serious things..."
        os.system('python CutsOptimization.py -a "' + fullcut + '" -o ' + OutPut + ' -i ' + signalFile[i] + ' -j ' + backgroundFile[i] + ' -f ' + signalFriendFile+' -g ' + backgroundFriendFile+' -m ' + MVAS[iM] )
        os.system('rm -rf ' + Weight)
        os.system('mv weights ' + Weight)
print "DONE!!\n \n"

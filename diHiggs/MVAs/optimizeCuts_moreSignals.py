#!/usr/bin/python
import os, ROOT
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut
from optparse import OptionParser
print "===> Optimizing cuts for selecting heavy Higgs vs tt."

Signal = "B3"

#Parameters
signalFile           = ['../Output/delphes_B3_1M_PU0_Btag_noMMCnoMVA.root','../Output/delphes_B6_1M_PU0_Btag_noMMCnoMVA.root']
signalWeight         = [0.085082,0.0033698]
backgroundFile       = ['../Output/delphes_tt_1M_PU0_Wtobmu_noMMCnoMVA.root','../Output/delphes_tt_1M_PU0_Wtobmu_noMMCnoMVA.root']
backgroundWeight      = [3.230581,3.230581]
#S/sqrt(S+B) is done for same number of S and B. You have to put manually in the Gui the number expected before cutting in the MVA.
# so you need the weight (here) and the number of event expected after the preselection.
signalFriendFile     = ""
backgroundFriendFile = ""
#presel        = '&& mass_b1b2<300 && dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons' #Place here an addicional cut you want to apply
presel        = 'hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons && dR_l1l2<2.49' #Place here an addicional cut you want to apply
# You can have a list of categories to optimize.
cuts        = ['','']
outputs     = ['TMVA_B3_DRl1l2_noMT2.root','TMVA_B6_DRl1l2_noMT2.root']
weightDir = ['weights_B3_DRl1l2_noMT2','weights_B6_DRl1l2_noMT2']

# Loop on the category to be optimized
for i in range(len(signalFile)):
#for i in range(1,2):
    print "DOING ITERATION NUMBER: " + str(i)
    if len(signalFile) != len(outputs):
        raise RuntimeError('Cut set does not correspond to output set!')
    fullcut = presel + cuts[i]
    print "Now running optimization with Cut = " + fullcut + ". Output will be put in " + outputs[i] + "  and xml file in " + str(weightDir[i])
    inputSig = TFile.Open( signalFile[i] )
    inputBkg = TFile.Open( backgroundFile[i] )
    treeS    = inputSig.Get("evtree")
    treeB    = inputBkg.Get("evtree")
    print "The Number of event to put in the mvaeffs*.png plot is: N_event_After_preselection*weight:"
    NEntries_S = treeS.Draw("dR_l1l2>>hist",fullcut,"goff");
    NEntries_B = treeB.Draw("dR_l1l2>>hist",fullcut,"goff");
    print "---> Signal: " + str(NEntries_S*signalWeight[i])
    print "---> backgr: " + str(NEntries_B*backgroundWeight[i])
    print "Now starting seriously........."
    os.system('python CutsOptimization.py -a "' + fullcut + '" -o ' + outputs[i] + ' -i ' + signalFile[i] + ' -j ' + backgroundFile[i] + ' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('mv weights ' + weightDir[i])
        
print "DONE!"

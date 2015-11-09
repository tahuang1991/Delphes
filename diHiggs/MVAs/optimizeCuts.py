#!/usr/bin/python
import os, ROOT
from ROOT import gSystem, gROOT, gApplication, TFile, TTree, TCut
from optparse import OptionParser
print "===> Optimizing cuts for selecting heavy Higgs vs tt."

#Parameters
signalFile           = "../delphes_B3_1M_PU0_Btag.root"
backgroundFile       = "../delphes_ttbar_1M_PU0.root"
#S/sqrt(S+B) is done for same number of S and B. You have to put manually in the Gui the number expected before cutting in the MVA.
# so you need the weight (here) and the number of event expected after the preselection.
signalWeight         = 0.085082 
backgroundWeight     = 3.230581
signalFriendFile     = ""
backgroundFriendFile = ""
additionalCut        = '&& mass_b1b2<300 && dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasGenMET && hasdRljet && hastwomuons' #Place here an addicional cut you want to apply
# You can have a list of categories to optimize.
cuts        = ['nu1and2_diBaxis_t>-900 && met_diBaxis_t>-900']
outputs     = ['TMVA.root']
weightfiles = ['OptimizedCuts.xml']

#Open file to compure evnts after preselection
inputSig = TFile.Open( signalFile )
inputBkg = TFile.Open( backgroundFile )
treeS    = inputSig.Get("evtree")
treeB    = inputBkg.Get("evtree")

# Loop on the category to be optimized
for i in range(len(cuts)):
    if len(cuts) != len(outputs):
        raise RuntimeError('Cut set does not correspond to output set!')
    fullcut=cuts[i]+additionalCut
    fullweightfile='weights/'+weightfiles[i]
    print "Now running optimization with Cut = " + fullcut + ". Output will be put in " + outputs[i] + "  and xml file in " + fullweightfile
    print "The Number of event to put in the mvaeffs*.png plot is: N_event_After_preselection*weight:"
    NEntries_S = treeS.Draw("dR_l1l2>>hist",cuts[i]+additionalCut,"goff");
    NEntries_B = treeB.Draw("dR_l1l2>>hist",cuts[i]+additionalCut,"goff");
    print "---> Signal: " + str(NEntries_S*signalWeight)
    print "---> backgr: " + str(NEntries_B*backgroundWeight)
    print "Now starting seriously........."
    os.system('python CutsOptimization.py -a "'+fullcut+'" -o ' + outputs[i] + ' -i ' + signalFile + ' -j ' + backgroundFile+' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('mv weights/TMVAClassification_Cuts.weights.xml ' + fullweightfile)
        
print "DONE!"

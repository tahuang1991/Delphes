#!/usr/bin/python
import os
from optparse import OptionParser

print "===> Optimizing cuts for selecting heavy Higgs vs tt."
signalFile           = "../delphes_B3_1M_PU0_Btag.root"
backgroundFile       = "../delphes_ttbar_1M_PU0.root"
signalFriendFile     = ""
backgroundFriendFile = ""
additionalCut        = '&& mass_b1b2<300 $$ dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasGenMET && hasdRljet && hastwomuons' #Place here an addicional cut you want to apply
# You can have a list of categories to be optmized.
#cuts        = ['nu1and2_diBaxis_t>-900 && met_diBaxis_t>-900']
cuts        = ['met_diBaxis_t>-900']
outputs     = ['TMVA.root']
weightfiles = ['OptimizedCuts.xml']

# Loop on the category to be optimized
for i in range(len(cuts)):
    if len(cuts) != len(outputs):
        raise RuntimeError('Cut set does not correspond to output set!')
    fullcut=cuts[i]+additionalCut
    fullweightfile='weights/'+weightfiles[i]
    print "Now running optimization with Cut = " + fullcut + ". Output will be put in " + outputs[i] + "  and xml file in " + fullweightfile
    print "Now starting seriously........."
    #os.system('python CutsOptimization.py -m Cuts -a "'+fullcut+'" -o ' + outputs[i] + ' -i ' + signalFile + ' -j ' + backgroundFile+' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('python CutsOptimization.py -a "'+fullcut+'" -o ' + outputs[i] + ' -i ' + signalFile + ' -j ' + backgroundFile+' -f ' + signalFriendFile+' -g ' + backgroundFriendFile)
    os.system('mv weights/TMVAClassification_Cuts.weights.xml ' + fullweightfile)
        
print "DONE!"

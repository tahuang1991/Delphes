
from math import *

BR_h_bb   = 0.577
BR_h_WW   = 0.215
BR_W_lnu  = 0.3257
BR_W_munu = 0.1057
BR_W_munu_taunu = 0.2182
BR_t_Wb   = 1.
def getCrossSection(Benchmark):
	
  Xsec = {}
  Xsec['tt']  = 953
  #Optimistic
  Xsec['B1']  = 1.1948649279633416   * 0.5035043253778491
  #Xsec['B2']  = 0.5900264559172156   * 0.7401476862292035
  Xsec['B2']  = 23.9 * 0.74
  #Xsec['B3']  = 0.4431392872406074   * 0.7550948295752251
  Xsec['B3']  = 19.0*.76
  #Xsec['B4']  = 0.3629024495635394   * 0.3271838865481416
  Xsec['B4']  = 20.1*0.33
  #Xsec['B5']  = 0.26198989157860825  * 0.30714428242103997
  Xsec['B5']  = 16.3*0.31
  #Xsec['B6']  = 0.1505457803080438   * 0.23554145150771
  Xsec['B6']  = 10.8*0.24
  #Xsec['B7']  = 0.08778859336272271  * 0.23284593958553115
  Xsec['B7']  = 6.96*0.23
  #Xsec['B8']  = 0.044958592682492124 * 0.30234102603499013
  Xsec['B8']  = 4.01*0.30
  #Xsec['B9']  = 0.02330230741512451  * 0.3283405583212012
  Xsec['B9']  = 2.23*0.33
  #Xsec['B10'] = 0.016878791231549145 * 0.19895423048929178
  Xsec['B10'] = 1.73*0.20
  #Xsec['B11'] = 0.008205926642337763 * 0.22349301758113208
  Xsec['B11'] = 0.92*0.22
  #Xsec['B12'] = 0.006805317561386914 * 0.07869395182066141
  Xsec['B12'] = 0.80*.079
  return Xsec[Benchmark]


def GetWeight(Benchmark, Lumi, InitEv):
   
  Lumi=1000*Lumi #convert inot pb-1
  BR = 1.0
  if( Benchmark != 'tt' ):
	BR = pow(BR_h_bb,2) + pow(BR_h_WW,2)*pow(BR_W_lnu,4) + 2*BR_h_bb*BR_h_WW*pow(BR_W_lnu,2)
  #else if ( proc==tt) BR = BR_t_Wb * BR_t_Wb * BR_W_munu_taunu * BR_W_munu_taunu
  elif ( Benchmark =='tt'):
      	BR = BR_t_Wb * BR_t_Wb * BR_W_munu * BR_W_munu
  else:
	print "WARNING::CrossSections_and_BR -> No process specified!"
  xsecbr = getCrossSection(Benchmark) * BR
  print "Benchmark ",Benchmark," CS ",getCrossSection(Benchmark)," BR ",BR," xsecbr ",xsecbr, " weight ",xsecbr*Lumi/InitEv
  return xsecbr*Lumi/InitEv



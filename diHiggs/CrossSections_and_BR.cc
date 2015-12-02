#include <iostream>
#include <utility>
#include <vector>
#include <cmath>

#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TRefArray.h"
#include "TObject.h"
#include "TString.h"
#include "CrossSections_and_BR.h"

using namespace std;

CrossSections_and_BR::CrossSections_and_BR(){

  Xsec_tt = 953;
  Xsec_B3 = 1.15 * 0.685;
  Xsec_B6 = 0.26 * 0.12;
  BR_h_bb = 0.577;
  BR_h_WW = 0.215;
  BR_W_lnu = 0.3272;
  BR_W_munu = 0.1063;
  BR_t_Wb = 1.;

}

CrossSections_and_BR::~CrossSections_and_BR(){
}

float CrossSections_and_BR::GetWeight( float Lumi, float InitEv, int proc ){ 
  
  float Xsec = -999.;
  if(proc==B3)      Xsec = Xsec_B3;
  else if(proc==B6) Xsec = Xsec_B6;
  else if(proc==tt) Xsec = Xsec_tt;
  else                cout<<"WARNING::CrossSections_and_BR -> No process specified!"<<endl;
  Lumi*=1000; //convert to pb-1
  float BR = 1.;
  if( proc==B3 || proc==B6 ) BR = pow(BR_h_bb,2) + pow(BR_h_WW,2)*pow(BR_W_lnu,4) + 2*BR_h_bb*BR_h_WW*pow(BR_W_lnu,2);
  else if ( proc==tt)          BR = BR_t_Wb * BR_t_Wb * BR_W_munu * BR_W_munu;
  else                           cout<<"WARNING::CrossSections_and_BR -> No process specified!"<<endl;
  float xsecbr = Xsec * BR;
  return xsecbr*Lumi/InitEv;
}

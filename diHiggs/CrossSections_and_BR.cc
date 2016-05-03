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

  Xsec_tt  = 953;
  //Optimistic
  Xsec_B1  = 1.1948649279633416   * 0.5035043253778491;
  Xsec_B2  = 0.5900264559172156   * 0.7401476862292035;
  Xsec_B3  = 0.4431392872406074   * 0.7550948295752251;
  Xsec_B4  = 0.3629024495635394   * 0.3271838865481416;
  Xsec_B5  = 0.26198989157860825  * 0.30714428242103997;
  Xsec_B6  = 0.1505457803080438   * 0.23554145150771982;
  Xsec_B7  = 0.08778859336272271  * 0.23284593958553115;
  Xsec_B8  = 0.044958592682492124 * 0.30234102603499013;
  Xsec_B9  = 0.02330230741512451  * 0.3283405583212012;
  Xsec_B10 = 0.016878791231549145 * 0.19895423048929178;
  Xsec_B11 = 0.008205926642337763 * 0.22349301758113208;
  Xsec_B12 = 0.006805317561386914 * 0.07869395182066141;
  //Pressimistic
  Xsec_B1p  = 0.03151854499349528  * 0.6007561554214891;
  Xsec_B2p  = 0.010504184010519923 * 0.7240605319081829;
  Xsec_B3p  = 0.6677582019732244   * 0.013879127007392215;
  Xsec_B4p  = 0.4201643037711516   * 0.017629411764918;
  Xsec_B5p  = 0.005252669763174569 * 0.6287516002157237;
  Xsec_B6p  = 0.005257365731772056   * 0.6203415872908459;
  Xsec_B7p  = 0.08661323785463505  * 0.048982258714930844;
  Xsec_B8p  = 0.08590745324712548  * 0.03020052200555651;
  Xsec_B9p  = 0.03822543928771288  * 0.010455631389548673;
  Xsec_B10p = 0.022251282360094736 * 0.04522116478851026;
  Xsec_B11p = 0.008634923814275509 * 0.1209097235855364;
  Xsec_B12p = 0.006798741039330509 * 0.06158560826791796;
  //BR
  BR_h_bb   = 0.577;
  BR_h_WW   = 0.215;
  BR_W_lnu  = 0.3257;
  BR_W_munu = 0.1057;
  BR_W_munu_taunu = 0.2182;
  BR_t_Wb   = 1.;

}

CrossSections_and_BR::~CrossSections_and_BR(){
}

float CrossSections_and_BR::GetWeight( float Lumi, float InitEv, int proc, bool Optimistic ){ 
  
  float Xsec = -999.;
  if(proc==B1)       Xsec = Optimistic ? Xsec_B1 : Xsec_B1p;
  else if(proc==B2)  Xsec = Optimistic ? Xsec_B2 : Xsec_B2p;
  else if(proc==B3)  Xsec = Optimistic ? Xsec_B3 : Xsec_B3p;
  else if(proc==B4)  Xsec = Optimistic ? Xsec_B4 : Xsec_B4p;
  else if(proc==B5)  Xsec = Optimistic ? Xsec_B5 : Xsec_B5p;
  else if(proc==B6)  Xsec = Optimistic ? Xsec_B6 : Xsec_B6p;
  else if(proc==B7)  Xsec = Optimistic ? Xsec_B7 : Xsec_B7p;
  else if(proc==B8)  Xsec = Optimistic ? Xsec_B8 : Xsec_B8p;
  else if(proc==B9)  Xsec = Optimistic ? Xsec_B9 : Xsec_B9p;
  else if(proc==B10) Xsec = Optimistic ? Xsec_B10 : Xsec_B10p;
  else if(proc==B11) Xsec = Optimistic ? Xsec_B11 : Xsec_B11p;
  else if(proc==B12) Xsec = Optimistic ? Xsec_B12 : Xsec_B12p;
  else if(proc==tt)  Xsec = Xsec_tt;
  else               cout<<"WARNING::CrossSections_and_BR -> No process specified!"<<endl;
  Lumi*=1000; //convert to pb-1
  float BR = 1.;
  if( proc!=tt )      BR = pow(BR_h_bb,2) + pow(BR_h_WW,2)*pow(BR_W_lnu,4) + 2*BR_h_bb*BR_h_WW*pow(BR_W_lnu,2);
  else if ( proc==tt) BR = BR_t_Wb * BR_t_Wb * BR_W_munu_taunu * BR_W_munu_taunu;
  else                cout<<"WARNING::CrossSections_and_BR -> No process specified!"<<endl;
  float xsecbr = Xsec * BR;
  if( Optimistic )  cout<<"Optimistic CROSS = "<<Xsec<<" * "<<BR<<" * "<<Lumi<<" / "<<InitEv<<" = "<<xsecbr*Lumi/InitEv<<endl;
  if( !Optimistic ) cout<<"Pessimistic CROSS = "<<Xsec<<" * "<<BR<<" * "<<Lumi<<" / "<<InitEv<<" = "<<xsecbr*Lumi/InitEv<<endl;
  return xsecbr*Lumi/InitEv;
}

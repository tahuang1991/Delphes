#include <TH1F.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TRandom3.h>
#include <TFormula.h>
#include <TPad.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TMarker.h>
#include <TChain.h>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "TTree.h"
#include "TLatex.h"
#include "TMath.h"
#include "TBranch.h"
#include "TFile.h"
#include "TStyle.h"
#include "TString.h"
#include "TEventList.h"
#include "TCut.h"
#include <iostream>
#include <sstream>  
#include <fstream>

using namespace std;

void EventFlow( TString Sig_name = "../Output/delphes_B3_1M_PU40_ALL.root", TString Bac_name = "../Output/delphes_ttbar_1M_PU40_Wtobl_ALL.root", TString Out="plots" ){

  //Opening Inputs
  cout<<"Hello, I'm creating computing the EventFlow."<<endl;
  TString Comm = "mkdir -p " + Out;
  system( Comm.Data() );
  //Opening files
  TFile* f_Sig = TFile::Open(Sig_name.Data());
  if( !f_Sig ) cout<<"Waring! File: "<<Sig_name<<" not present!"<<endl;
  TFile* f_Bac = TFile::Open(Bac_name.Data());
  if( !f_Bac ) cout<<"Waring! File: "<<Bac_name<<" not present!"<<endl;
  TTree *T_Sig = (TTree*) f_Sig->Get("evtree");
  if( !T_Sig ) cout<<"Waring! TTree evtree in "<<Sig_name<<" not present!"<<endl;
  TTree *T_Bac = (TTree*) f_Bac->Get("evtree");
  if( !T_Bac ) cout<<"Waring! TTree evtree in "<<Bac_name<<" not present!"<<endl;
  //Get Weight
  TH1F *hW = new TH1F("hW","", 1000, 0., 50.);
  float NEntries_S = T_Sig->Draw("weight>>hW","weight>0","goff");
  float weight_S = hW->GetMean();
  float NEntries_B = T_Bac->Draw("weight>>hW","weight>0","goff");
  float weight_B = hW->GetMean();
  cout<<"Signal: "<<Sig_name<<" has: "<<weight_S<<" weight."<<endl;
  cout<<"Background: "<<Bac_name<<" has: "<<weight_B<<" weight."<<endl;
  //Event Flow.
  TCut selection = "";
  int nCuts = 7;
  TString Cuts[]={"preselections","(hasb1jet || hasb2jet)","dR_l1l2<3.3 && dR_l1l2>0.07","dR_b1b2<3.3","TMath::Abs(dphi_l1l2b1b2)>1.","mass_l1l2<75 && mass_l1l2>5.","mass_b1b2<200 && mass_b1b2>50"};
  for( int i=0; i<nCuts; i++){
     if( i==0 ) selection = Cuts[i];
     else selection += Cuts[i];
     cout<<"Selection: "<<selection<<endl;
     NEntries_S = T_Sig->Draw("weight>>hist",selection,"goff");
     cout<<"S) "<<NEntries_S*weight_S<<endl;
     NEntries_B = T_Bac->Draw("weight>>hist",selection,"goff");
     cout<<"B) "<<NEntries_B*weight_B<<endl;
  }
}

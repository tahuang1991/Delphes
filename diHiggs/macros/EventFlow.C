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

void EventFlow( TString Sig_name = "ALL", TString Bac_name = "../Output/delphes_tt_4M_PU40_WtobtaumuALL_3May.root", TString Out="plots" ){

  //Opening Inputs
  cout<<"Hello, I'm creating computing the EventFlow."<<endl;  
  std::vector<TString> samples; samples.clear();
  if(Sig_name=="ALL"){
    samples.push_back("../Output/delphes_B2_1M_PU40ALL_3May.root");
    samples.push_back("../Output/delphes_B3_1M_PU40ALL_3May.root");
    samples.push_back("../Output/delphes_B6_1M_PU40ALL_3May.root");
    samples.push_back("../Output/delphes_B12_1M_PU40ALL_3May.root");
  }
  else samples.push_back(Sig_name);

  //Loop on the samples
  for(unsigned int j=0; j<samples.size(); j++ ){
    TString Comm = "mkdir -p " + Out;
    system( Comm.Data() );
    //Opening files
    cout<<"------------------- OPENING "<<samples[j]<<"-------------------------"<<endl;
    TFile* f_Sig = TFile::Open(samples[j].Data());
    if( !f_Sig ) cout<<"Waring! File: "<<samples[j]<<" not present!"<<endl;
    TFile* f_Bac = TFile::Open(Bac_name.Data());
    if( !f_Bac ) cout<<"Waring! File: "<<Bac_name<<" not present!"<<endl;
    TTree *T_Sig = (TTree*) f_Sig->Get("evtree");
    if( !T_Sig ) cout<<"Waring! TTree evtree in "<<samples[j]<<" not present!"<<endl;
    TTree *T_Bac = (TTree*) f_Bac->Get("evtree");
    if( !T_Bac ) cout<<"Waring! TTree evtree in "<<Bac_name<<" not present!"<<endl;
    //Get Weight
    TH1F *hW = new TH1F("hW","", 1000, 0., 50.);
    float NEntries_S = T_Sig->Draw("weight>>hW","weight>0","goff");
    float weight_S = hW->GetMean();
    float NEntries_B = T_Bac->Draw("weight>>hW","weight>0","goff");
    float weight_B = hW->GetMean();
    cout<<"Signal: "<<samples[j]<<" has: "<<NEntries_S<<" * "<<weight_S<<" events = "<<NEntries_S*weight_S<<endl;
    cout<<"Background: "<<Bac_name<<" has: "<<NEntries_B<<" * "<<weight_B<<" events = "<<NEntries_B*weight_B<<endl;
    //Event Flow. (preselections = "hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons && hasdRljet)
    TCut selection = "";
    int nCuts = 6;
    TString B_tag = "(hasb1jet || hasb2jet)";
    //TString B_tag = "(((b1jet_btag&2)>0 && (b2jet_btag&3)>0) || ((b1jet_btag&3)>0 && (b 2jet_btag&2)>0))";
    TString Cuts[]={"hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons",B_tag,"dR_l1l2<3.3 && dR_l1l2>0.07","dR_b1b2<5.","mass_l1l2<100 && mass_l1l2>5.","mass_b1b2>22"};
    TString Cuts_S[]={"Preselection","B-tag","$\\Delta$R(l1l2)","$\\Delta$R(b1b2)","m(l1l2)","m(b1b2)"};
    float nEv_S[nCuts], nEv_B[nCuts];
    for( int i=0; i<nCuts; i++){
	if( i==0 ){
	  selection = Cuts[i];
	  cout<<"\\begin{table}[h]"<<endl;
	  cout<<"\\centering"<<endl;
	  cout<<" \\begin{tabular}{c|c|c|c|c|c|c}"<<endl;
	  cout<<"   \\hline"<<endl;
	  cout<<"   Cut & N$_S$ & Abs$_{Eff}$ & Rel$_{Eff}$ & N$_B$ & Abs$_{Eff}$ & Rel$_{Eff}$\\\\"<<endl;
	  cout<<"   \\hline"<<endl;
	}
	else selection += Cuts[i];
	nEv_S[i] = T_Sig->Draw("weight>>hist",selection,"goff");
	nEv_B[i] = T_Bac->Draw("weight>>hist",selection,"goff");
	if(i==1) cout<<Cuts_S[i]<<" & "<<nEv_S[i]*weight_S<<" & "<<"1"<<" & "<<"1"<<" & "<<nEv_B[i]*weight_B<<" & "<<"1"<<" & "<<"1"<<"\\\\"<<endl;
	if(i>1)  cout<<Cuts_S[i]<<" & "<<nEv_S[i]*weight_S<<" & "<<nEv_S[i]/nEv_S[1]<<" & "<<nEv_S[i]/nEv_S[i-1]<<" & "<<nEv_B[i]*weight_B<<" & "<<nEv_B[i]/nEv_B[1]<<" & "<<nEv_B[i]/nEv_B[i-1]<<"\\\\"<<endl;
	if(i==nCuts-1){
	  cout<<"   \\hline"<<endl;
	  cout<<" \\end{tabular}"<<endl;
	  cout<<"\\end{table}"<<endl;
	}
    }
  }
}

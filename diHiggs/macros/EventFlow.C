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

void EventFlow( TString Sig_name = "ALL", TString Bac_name = "/fdata/hepx/store/user/lpernie/Hhh/delphes_tt_4M_PU40_WtobtaumuALL_13May.root", TString Out="plots" ){

  TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
  gStyle->SetOptStat(0);
  if(false){
  //Opening Inputs
  cout<<"Hello, I'm creating computing the EventFlow."<<endl;
  std::vector<TString> samples; samples.clear();
  if(Sig_name=="ALL"){
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B1_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B2_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B3_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B4_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B5_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B6_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B7_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B8_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B9_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B10_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B11_1M_PU40ALL_13May.root");
    samples.push_back("/fdata/hepx/store/user/lpernie/Hhh/delphes_B12_1M_PU40ALL_13May.root");
  }
  else samples.push_back(Sig_name);

  //Loop on the samples
  vector<float> AbsEff_s, N_s;
  AbsEff_s.clear(); N_s.clear();
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
    float weight_B = 3.40301;//hW->GetMean();
    cout<<"Signal: "<<samples[j]<<" has: "<<NEntries_S<<" * "<<weight_S<<" events = "<<NEntries_S*weight_S<<endl;
    cout<<"Background: "<<Bac_name<<" has: "<<NEntries_B<<" * "<<weight_B<<" events = "<<NEntries_B*weight_B<<endl;
    //Event Flow. (preselections = "hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons && hasdRljet)
    TString selection = "";
    int nCuts = 6;
    //TString B_tag = "(hasb1jet || hasb2jet)";
    TString B_tag = "(((b1jet_btag&2)>0 && (b2jet_btag&3)>0) || ((b1jet_btag&3)>0 && (b 2jet_btag&2)>0))";
    TString Cuts[]={"hasRECOjet1 && hasRECOjet2 && hasMET && hastwomuons",B_tag,"dR_l1l2<3.3 && dR_l1l2>0.07","dR_b1b2<5.","mass_l1l2<100 && mass_l1l2>5.","mass_b1b2>22"};
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
	else selection += ( " && " + Cuts[i]);
	TH1F *hs = new TH1F("hs","",100,0.,100);
	TH1F *hb = new TH1F("hb","",100,0.,100);
	T_Sig->Draw( "weight>>hs", "reweighting * (" + selection + ")", "goff" );
	T_Bac->Draw( "weight>>hb", "reweighting * (" + selection + ")", "goff" );
	nEv_S[i] = hs->Integral();
	nEv_B[i] = hb->Integral();
	delete hs;
	delete hb;
	if(i==1) cout<<Cuts_S[i]<<" & "<<nEv_S[i]*weight_S<<" & "<<"1"<<" & "<<"1"<<" & "<<nEv_B[i]*weight_B<<" & "<<"1"<<" & "<<"1"<<"\\\\"<<endl;
	if(i>1)  cout<<Cuts_S[i]<<" & "<<nEv_S[i]*weight_S<<" & "<<nEv_S[i]/nEv_S[1]<<" & "<<nEv_S[i]/nEv_S[i-1]<<" & "<<nEv_B[i]*weight_B<<" & "<<nEv_B[i]/nEv_B[1]<<" & "<<nEv_B[i]/nEv_B[i-1]<<"\\\\"<<endl;
	if(i==nCuts-1){
	  cout<<"   \\hline"<<endl;
	  cout<<" \\end{tabular}"<<endl;
	  cout<<"\\end{table}"<<endl;
	  AbsEff_s.push_back(nEv_S[i]/nEv_S[1]);
	  N_s.push_back(nEv_S[i]*weight_S);
	}
    }
  }// for samples

  TH1F *h_eff_abs = new TH1F( "h_eff_abs", "", int(samples.size()), 0, int(samples.size()) );
  TH1F *h_n_s = new TH1F( "h_n_s", "", int(samples.size()), 0, int(samples.size()) );
  for(unsigned int i=0; i<samples.size(); i++){
    h_eff_abs->SetBinContent( i+1, AbsEff_s[i] );
    h_n_s->SetBinContent( i+1, N_s[i] );
  }
  h_eff_abs->SetMinimum(0.9); h_eff_abs->GetXaxis()->SetTitle("Signal Sample"); h_eff_abs->GetYaxis()->SetTitle("Efficiency");
  h_eff_abs->Draw();
  myc1->SaveAs( Out + "/eff.pdf" );
  h_n_s->GetXaxis()->SetTitle("Signal Sample"); h_n_s->GetYaxis()->SetTitle("Events expected in 300 fb-1");
  h_n_s->Draw();
  myc1->SaveAs( Out + "/Nsig.pdf" );
  }
  //Sensitivity
  float Masses[26] = {250, 257,259, 340,342, 351,353, 414,416, 454,456, 510,512, 562,564, 603,604, 660,662, 713,715, 766,778, 839,841, 900};
  Int_t   Binnum = sizeof(Masses)/sizeof(Float_t) - 1;
  TH1F *h_sens = new TH1F( "h_sens", "", Binnum, Masses );
  h_sens->SetMarkerStyle(34);
  h_sens->SetMarkerColor(kBlue);
  h_sens->SetBinContent( 2, 2.31635);
  h_sens->SetBinContent( 4, 1.10901);
  h_sens->SetBinContent( 6, 0.934016);
  h_sens->SetBinContent( 8, 0.839262);
  h_sens->SetBinContent( 10, 1.17605);
  h_sens->SetBinContent( 12, 0.961658);
  h_sens->SetBinContent( 14, 0.733404);
  h_sens->SetBinContent( 16, 0.782335);
  h_sens->SetBinContent( 18, 0.918227);
  h_sens->SetBinContent( 20, 0.315506);
  h_sens->SetBinContent( 22, 0.0711883);
  h_sens->SetBinContent( 24, 0.018855);
  h_sens->SetMinimum(0.); h_sens->GetXaxis()->SetTitle("Signal Mass [GeV]"); h_sens->GetYaxis()->SetTitle("Sensitivity");
  h_sens->Draw("P");
  TH1F *h_sensCut = new TH1F( "h_sensCut", "", Binnum, Masses );
  h_sensCut->SetMarkerStyle(34);
  h_sensCut->SetMarkerColor(kRed);
  h_sensCut->SetBinContent( 2, 4.42847);
  h_sensCut->SetBinContent( 4, 2.16941);
  h_sensCut->SetBinContent( 6, 1.86745);
  h_sensCut->SetBinContent( 8, 1.80996);
  h_sensCut->SetBinContent( 10, 2.57802);
  h_sensCut->SetBinContent( 12, 2.47399);
  h_sensCut->SetBinContent( 14, 1.04581);
  h_sensCut->SetBinContent( 16, 1.55697);
  h_sensCut->SetBinContent( 18, 2.21026);
  h_sensCut->SetBinContent( 20, 0.225977);
  h_sensCut->SetBinContent( 22, 0.258508);
  h_sensCut->SetBinContent( 24, 0.0567165);
  h_sensCut->SetMinimum(0.); h_sensCut->GetXaxis()->SetTitle("Signal Mass [GeV]"); h_sensCut->GetYaxis()->SetTitle("Sensitivity");
  //h_sensCut->Draw("sameP");

  myc1->SaveAs( Out + "/Sens.pdf" );
}

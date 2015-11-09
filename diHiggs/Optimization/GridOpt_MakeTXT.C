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
#include <iostream>
#include <sstream>  
#include <fstream>

using namespace std;
//enum BR{ h_bb = 0.577, h_WW = 0.215, W_lnu = 0.3272, W_munu = 0.1063, t_WB =1. };
struct BRs {
  float h_bb; float h_WW; float W_lnu; float W_munu; float t_WB;
} MyBR;

//void GridOpt_MakeTXT( TString folder = "OUT_GridOpt_1/", TString Sig_name = "/fdata/hepx/store/user/taohuang/Hhh/Delphes_ana/DiHiggs_WWbb_1M_PU0_leptonW_6969448_B3_1010_validatecuts.root", TString Bac_name = "/fdata/hepx/store/user/taohuang/Hhh/Delphes_ana/TTbar_WWbb_1M_PU0_leptonW_7570700_1010_validatecuts.root" )
void GridOpt_MakeTXT( TString folder = "OUT_GridOpt/", TString Sig_name = "../delphes_B3_1M_PU0_Btag.root", TString Bac_name = "../delphes_ttbar_1M_PU0.root" ){

  TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
  //Cross sections and BR
  MyBR.h_bb =  0.577; MyBR.h_WW = 0.215; MyBR.W_lnu = 0.3272; MyBR.W_munu = 0.1063; MyBR.t_WB = 1.;
  float LumiInt     = 300 * 1000;   //L_int = 300 fb-1
  float Ntot_Hhh    = 1000000; 
  float Xsec_Hhh    = 1.15 * 0.685; //sigma(pp->H->hh) [pb]
  float Br_Hhh      = ( MyBR.h_bb + MyBR.h_WW * MyBR.W_lnu * MyBR.W_lnu ) * ( MyBR.h_bb + MyBR.h_WW * MyBR.W_lnu * MyBR.W_lnu ); //H->hh ; hh->bbbb  hh->WWbb, hh->WWWW ;  W -> lnu;
  float Ntot_tt     = 1000000;
  float Xsec_tt     = 953;          //sigma(pp->tt) [pb]
  //float Br_tt       = MyBR.t_WB * MyBR.t_WB * MyBR.W_lnu * MyBR.W_lnu;
  float Br_tt       = MyBR.t_WB * MyBR.t_WB * MyBR.W_munu * MyBR.W_munu;
  float Weight_Hhh  = (Xsec_Hhh * Br_Hhh * LumiInt) / Ntot_Hhh;
  float Weight_tt   = (Xsec_tt * Br_tt * LumiInt) / Ntot_tt;
  //Opening Inputs
  cout<<"Hello, I'm creating the '"<<folder<<"' folder, and opening the input files:"<<endl;
  cout<<"Signal: "<<Sig_name<<endl;
  cout<<"Background: "<<Bac_name<<endl;
  cout<<"Signal: has a Cross section of: "<<Xsec_Hhh<<" and a BR of "<<Br_Hhh<<". The #Events at 300pb are: "<<(Xsec_Hhh * Br_Hhh * LumiInt)<<". The Weights is: "<<Weight_Hhh<<endl;
  cout<<"Backgr: has a Cross section of: "<<Xsec_tt<<" and a BR of "<<Br_tt<<". The #Events at 300pb are: "<<(Xsec_tt * Br_tt * LumiInt)<<". The Weights is: "<<Weight_tt<<endl;
  TString Comm = "mkdir -p " + folder;
  system( Comm.Data() );
  TFile* f_Sig = TFile::Open(Sig_name.Data());
  if( !f_Sig ) cout<<"Waring! File: "<<Sig_name<<" not present!"<<endl;
  TFile* f_Bac = TFile::Open(Bac_name.Data());
  if( !f_Bac ) cout<<"Waring! File: "<<Bac_name<<" not present!"<<endl;
  TTree *T_Sig = (TTree*) f_Sig->Get("evtree");
  if( !T_Sig ) cout<<"Waring! TTree evtree in "<<Sig_name<<" not present!"<<endl;
  TTree *T_Bac = (TTree*) f_Bac->Get("evtree");
  if( !T_Bac ) cout<<"Waring! TTree evtree in "<<Bac_name<<" not present!"<<endl;
  float NEntries_S = T_Sig->Draw("dR_l1l2>>hist","dR_l1l2>0","goff");
  float NEntries_B = T_Bac->Draw("dR_l1l2>>hist","dR_l1l2>0","goff");
  cout<<"You have "<<NEntries_S<<" entries for Signal and "<<NEntries_B<<" for background."<<endl;
  float Norm1_S = 1./NEntries_S, Norm1_B = 1./NEntries_B;
  float NormCro_S = Weight_Hhh, NormCro_B = Weight_tt;

  //Txt File with S and B
  FILE *file_txt;
  TString name_txt = "/S_B.txt";
  file_txt=fopen( (folder + name_txt).Data(),"w");
  cout<<"Now I created the "<<(folder + name_txt)<<" file."<<endl;

  cout<<"Now I'm defining the GRID of cuts!"<<endl;
  float Niters = 1;
  //Grid: dR_l1l2
  vector <Float_t> cut_dR_l1l2; cut_dR_l1l2.clear();
  float cut_dR_l1l2_init = 1.1, cut_dR_l1l2_end = 2.3, cut_dR_l1l2_step = 0.3;
  for(Double_t j=cut_dR_l1l2_init; j<=cut_dR_l1l2_end; j+=cut_dR_l1l2_step) cut_dR_l1l2.push_back(j);
  TH1F *h_dR_l1l2_s = new TH1F("h_dR_l1l2_s","#Delta R l1-l2", 50, 0., 4.5);
  TH1F *h_dR_l1l2_b = new TH1F("h_dR_l1l2_b","#Delta R l1-l2", 50, 0., 4.5);
  T_Sig->Draw("dR_l1l2>>h_dR_l1l2_s","dR_l1l2>0","goff");
  T_Bac->Draw("dR_l1l2>>h_dR_l1l2_b","dR_l1l2>0","goff"); 
  h_dR_l1l2_s->Scale(Norm1_S); h_dR_l1l2_b->Scale(Norm1_B); h_dR_l1l2_s->SetLineColor(2);
  h_dR_l1l2_s->Draw(); h_dR_l1l2_b->Draw("same");
  myc1->SaveAs( (folder + "/dR_l1l2.png") );
  h_dR_l1l2_s->Scale(NEntries_S * NormCro_S); h_dR_l1l2_b->Scale(NEntries_B * NormCro_B);
  h_dR_l1l2_b->SetMinimum(1); h_dR_l1l2_b->Draw();  h_dR_l1l2_s->Draw("same");
  myc1->SaveAs( (folder + "/dR_l1l2_W.png") );
  myc1->SetLogy(1); myc1->SaveAs( (folder + "/dR_l1l2_W_log.png") ); myc1->SetLogy(0);
  Niters *= (cut_dR_l1l2_end-cut_dR_l1l2_init)/cut_dR_l1l2_step;
  //Grid: dR_b1b2
  vector <Float_t> cut_dR_b1b2; cut_dR_b1b2.clear();
  float cut_dR_b1b2_init = 2.2, cut_dR_b1b2_end = 3., cut_dR_b1b2_step = 0.2;
  for(Double_t j=cut_dR_b1b2_init; j<=cut_dR_b1b2_end; j+=cut_dR_b1b2_step) cut_dR_b1b2.push_back(j);
  TH1F *h_dR_b1b2_s = new TH1F("h_dR_b1b2_s","#Delta R b1-b2", 50, 0., 4.5);
  TH1F *h_dR_b1b2_b = new TH1F("h_dR_b1b2_b","#Delta R b1-b2", 50, 0., 4.5);
  T_Sig->Draw("dR_b1b2>>h_dR_b1b2_s","dR_b1b2>0","goff");
  T_Bac->Draw("dR_b1b2>>h_dR_b1b2_b","dR_b1b2>0","goff");
  h_dR_b1b2_s->Scale(Norm1_S); h_dR_b1b2_b->Scale(Norm1_B); h_dR_b1b2_s->SetLineColor(2);
  h_dR_b1b2_s->Draw(); h_dR_b1b2_b->Draw("same");
  myc1->SaveAs( (folder + "/dR_b1b2.png") );
  h_dR_b1b2_s->Scale(NEntries_S * NormCro_S); h_dR_b1b2_b->Scale(NEntries_B * NormCro_B);
  h_dR_b1b2_b->SetMinimum(1); h_dR_b1b2_b->Draw(); h_dR_b1b2_s->Draw("same");
  myc1->SaveAs( (folder + "/dR_b1b2_W.png") );
  myc1->SetLogy(1); myc1->SaveAs( (folder + "/dR_b1b2_W_log.png") ); myc1->SetLogy(0);
  Niters *= (cut_dR_b1b2_end-cut_dR_b1b2_init)/cut_dR_b1b2_step;
  //Grid: dR_b1b2_min
  vector <Float_t> cut_dR_b1b2_min; cut_dR_b1b2_min.clear();
  float cut_dR_b1b2_min_init = 0.7, cut_dR_b1b2_min_end = 1.5, cut_dR_b1b2_min_step = 0.2;
  for(Double_t j=cut_dR_b1b2_min_init; j<=cut_dR_b1b2_min_end; j+=cut_dR_b1b2_min_step) cut_dR_b1b2_min.push_back(j);
  Niters *= (cut_dR_b1b2_min_end-cut_dR_b1b2_min_init)/cut_dR_b1b2_min_step;
  //Grid: mass_l1l2
  vector <Float_t> cut_mass_l1l2; cut_mass_l1l2.clear();
  float cut_mass_l1l2_init = 50., cut_mass_l1l2_end = 70., cut_mass_l1l2_step = 5.;
  for(Double_t j=cut_mass_l1l2_init; j<=cut_mass_l1l2_end; j+=cut_mass_l1l2_step) cut_mass_l1l2.push_back(j);
  TH1F *h_mass_l1l2_s = new TH1F("h_mass_l1l2_s","Mass l1-l2", 50, 0., 300.);
  TH1F *h_mass_l1l2_b = new TH1F("h_mass_l1l2_b","Mass l1-l2", 50, 0., 300.);
  T_Sig->Draw("mass_l1l2>>h_mass_l1l2_s","mass_l1l2>0","goff");
  T_Bac->Draw("mass_l1l2>>h_mass_l1l2_b","mass_l1l2>0","goff");
  h_mass_l1l2_s->Scale(Norm1_S); h_mass_l1l2_b->Scale(Norm1_B); h_mass_l1l2_s->SetLineColor(2);
  h_mass_l1l2_s->Draw(); h_mass_l1l2_b->Draw("same");
  myc1->SaveAs( (folder + "/mass_l1l2.png") );
  h_mass_l1l2_s->Scale(NEntries_S * NormCro_S); h_mass_l1l2_b->Scale(NEntries_B * NormCro_B);
  h_mass_l1l2_b->SetMinimum(1); h_mass_l1l2_b->Draw(); h_mass_l1l2_s->Draw("same");
  myc1->SaveAs( (folder + "/mass_l1l2_W.png") );
  myc1->SetLogy(1); myc1->SaveAs( (folder + "/mass_l1l2_W_log.png") ); myc1->SetLogy(0);
  Niters *= (cut_mass_l1l2_end-cut_mass_l1l2_init)/cut_mass_l1l2_step;
  //Grid: mass_b1b2 
  vector <Float_t> cut_mass_b1b2; cut_mass_b1b2.clear();
  float cut_mass_b1b2_init = 140., cut_mass_b1b2_end = 170., cut_mass_b1b2_step = 5.;
  for(Double_t j=cut_mass_b1b2_init; j<=cut_mass_b1b2_end; j+=cut_mass_b1b2_step) cut_mass_b1b2.push_back(j);
  TH1F *h_mass_b1b2_s = new TH1F("h_mass_b1b2_s","Mass b1-b2", 50, 0., 300.);
  TH1F *h_mass_b1b2_b = new TH1F("h_mass_b1b2_b","Mass b1-b2", 50, 0., 300.);
  T_Sig->Draw("mass_b1b2>>h_mass_b1b2_s","mass_b1b2>0","goff");
  T_Bac->Draw("mass_b1b2>>h_mass_b1b2_b","mass_b1b2>0","goff");
  h_mass_b1b2_s->Scale(Norm1_S); h_mass_b1b2_b->Scale(Norm1_B); h_mass_b1b2_s->SetLineColor(2);
  h_mass_b1b2_s->Draw(); h_mass_b1b2_b->Draw("same");
  myc1->SaveAs( (folder + "/mass_b1b2.png") );
  h_mass_b1b2_s->Scale(NEntries_S * NormCro_S); h_mass_b1b2_b->Scale(NEntries_B * NormCro_B);
  h_mass_b1b2_b->SetMinimum(1); h_mass_b1b2_b->Draw(); h_mass_b1b2_s->Draw("same");
  myc1->SaveAs( (folder + "/mass_b1b2_W.png") );
  myc1->SetLogy(1); myc1->SaveAs( (folder + "/mass_b1b2_W_log.png") ); myc1->SetLogy(0);
  Niters *= (cut_mass_b1b2_end-cut_mass_b1b2_init)/cut_mass_b1b2_step;
  //Grid: mass_b1b2_min
  vector <Float_t> cut_mass_b1b2_min; cut_mass_b1b2_min.clear();
  float cut_mass_b1b2_min_init = 70., cut_mass_b1b2_min_end = 100., cut_mass_b1b2_min_step = 5.;
  for(Double_t j=cut_mass_b1b2_min_init; j<=cut_mass_b1b2_min_end; j+=cut_mass_b1b2_min_step) cut_mass_b1b2_min.push_back(j);
  Niters *= (cut_mass_b1b2_min_end-cut_mass_b1b2_min_init)/cut_mass_b1b2_min_step;
  //Grid: dr_bl
  vector <Float_t> cut_dR_bl; cut_dR_bl.clear();
  float cut_dR_bl_init = 1.25, cut_dR_bl_end = 2.0, cut_dR_bl_step = 0.25;
  for(Double_t j=cut_dR_bl_init; j<=cut_dR_bl_end; j+=cut_dR_bl_step) cut_dR_bl.push_back(j);
  TH1F *h_dR_bl_s = new TH1F("h_dR_bl_s","DR b-l", 50, 0., 5.);
  TH1F *h_dR_bl_b = new TH1F("h_dR_bl_b","DR b-l", 50, 0., 5.);
  T_Sig->Draw("dR_bl>>h_dR_bl_s","dR_bl>0","goff");
  T_Bac->Draw("dR_bl>>h_dR_bl_b","dR_bl>0","goff");
  h_dR_bl_s->Scale(Norm1_S); h_dR_bl_b->Scale(Norm1_B); h_dR_bl_s->SetLineColor(2);
  h_dR_bl_s->Draw(); h_dR_bl_b->Draw("same");
  myc1->SaveAs( (folder + "/dR_bl.png") );
  h_dR_bl_s->Scale(NEntries_S * NormCro_S); h_dR_bl_b->Scale(NEntries_B * NormCro_B);
  h_dR_bl_b->SetMinimum(1); h_dR_bl_b->Draw(); h_dR_bl_s->Draw("same");
  myc1->SaveAs( (folder + "/dR_bl_W.png") );
  myc1->SetLogy(1); myc1->SaveAs( (folder + "/dR_bl_W_log.png") ); myc1->SetLogy(0);
  Niters *= (cut_dR_bl_end-cut_dR_bl_init)/cut_dR_bl_step;
  fprintf(file_txt,"NEntries_S %.6f NormCro_S %.6f NEntries_B %.6f NormCro_B %.6f \n", NEntries_S, NormCro_S, NEntries_B, NormCro_B);
  fprintf(file_txt,"---------- \n");

  //Fill the TXT
  cout<<"Now I'm starting the LOOP on the cuts! You are optimizing on "<<Niters<<" bins."<<endl;
  int nBin(0);
  unsigned int i1=0, i2=0, i2b=0, i3=0, i4=0, i5=0, i6=0;
  for( i1=0; i1<cut_dR_l1l2.size(); i1++ ){
    for( i2=0; i2<cut_dR_b1b2.size(); i2++ ){
	for( i2b=0; i2b<cut_dR_b1b2_min.size(); i2b++ ){
	  for( i3=0; i3<cut_mass_l1l2.size(); i3++ ){
	    for( i4=0; i4<cut_mass_b1b2.size(); i4++ ){
		for( i5=0; i5<cut_mass_b1b2_min.size(); i5++ ){
		  for( i6=0; i6<cut_dR_bl.size(); i6++ ){
		    std::ostringstream ss_cut_dR_l1l2, ss_cut_dR_b1b2, ss_cut_dR_b1b2_min, ss_cut_mass_l1l2, ss_cut_mass_b1b2, ss_cut_mass_b1b2_min, ss_cut_dR_bl;
		    ss_cut_dR_l1l2 << cut_dR_l1l2[i1]; ss_cut_dR_b1b2 << cut_dR_b1b2[i2]; ss_cut_dR_b1b2_min << cut_dR_b1b2_min[i2b]; ss_cut_mass_l1l2 << cut_mass_l1l2[i3]; ss_cut_mass_b1b2 << cut_mass_b1b2[i4]; ss_cut_mass_b1b2_min << cut_mass_b1b2_min[i5]; ss_cut_dR_bl << cut_dR_bl[i6];
		    TString selection = "dR_l1l2 > 0 && dR_l1l2 < " + ss_cut_dR_l1l2.str() + " && dR_b1b2 < " + ss_cut_dR_b1b2.str() + " && dR_b1b2 > " + ss_cut_dR_b1b2_min.str() + " && mass_l1l2 < " + ss_cut_mass_l1l2.str() + " && mass_b1b2 < " + ss_cut_mass_b1b2.str() + " && mass_b1b2 > " + ss_cut_mass_b1b2_min.str() + " && dR_bl > " + ss_cut_dR_bl.str();
		    TString selection_Fixed  = " && dR_bl<3.5";
		    //cout<<selection<<endl;
		    if(nBin%1000==0) cout<<"You are at bin: "<<nBin<<endl;
		    float Signal   = T_Sig->Draw("dR_l1l2>>hist", selection + selection_Fixed, "goff") * NormCro_S;
		    float Backgr   = T_Bac->Draw("dR_l1l2>>hist", selection + selection_Fixed, "goff") * NormCro_B; 
		    float S_SqrtB  = (Backgr==0) ? 0. : Signal/sqrt(Backgr);
		    float S_B      = (Backgr==0) ? 0. : Signal/Backgr;
		    float S_SpB    = (Signal+Backgr==0) ? 0. : Signal/(Signal+Backgr);
		    float S_SqrtSB = (Signal+Backgr==0) ? 0. : Signal/sqrt(Signal+Backgr);
		    //cout<<"Signal "<<Signal<<" and Bkg: "<<Backgr<<" S/B "<<S_B<<" S/sqrt(B) "<<S_SqrtB<<" S/(S+B) "<<S_SpB<<" S/sqrt(S+B) "<<S_SqrtSB<<endl;
		    fprintf(file_txt,"BIN %i cut_dR_l1l2 %.2f cut_dR_b1b2 %.2f cut_dR_b1b2_min %.2f cut_mass_l1l2 %.2f cut_mass_b1b2 %.2f cut_mass_b1b2_min %.2f cut_dR_bl %.2f Signal %.6f Background %.6f S_B %.6f S_SqrtB %.6f S_SpB %.6f S_SqrtSB %.6f \n", nBin, cut_dR_l1l2[i1], cut_dR_b1b2[i2], cut_dR_b1b2_min[i2b],cut_mass_l1l2[i3], cut_mass_b1b2[i4], cut_mass_b1b2_min[i5], cut_dR_bl[i6], Signal, Backgr, S_B, S_SqrtB, S_SpB, S_SqrtSB);
		    nBin++;
		  }
		}
	    }
	  }
	}
    }
  }
  fclose (file_txt);
}

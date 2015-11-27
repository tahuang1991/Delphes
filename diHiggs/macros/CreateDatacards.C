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
struct BRs {
  float h_bb; float h_WW; float W_lnu; float W_munu; float t_WB;
} MyBR;

void CreateDatacards( TString Opt="CutCount", TString folder = "Outputs/", TString Sig_name = "/home/lpernie/HeavyHiggs/delphes/diHiggs/Output/delphes_B3_1M_PU0_Btag.root", TString Bac_name = "/home/lpernie/HeavyHiggs/delphes/diHiggs/Output/delphes_ttbar_1M_PU0_Wtobmu.root" ){

  //Cross sections and BR
  MyBR.h_bb =  0.577; MyBR.h_WW = 0.215; MyBR.W_lnu = 0.3272; MyBR.W_munu = 0.1063; MyBR.t_WB = 1.;
  float LumiInt     = 300 * 1000;   //L_int = 300 fb-1
  float Ntot_Hhh    = 1000000; 
  float Xsec_Hhh    = 1.15 * 0.685; //sigma(pp->H->hh) [pb]
  //float Br_Hhh      = ( MyBR.h_bb + MyBR.h_WW * MyBR.W_lnu * MyBR.W_lnu ) * ( MyBR.h_bb + MyBR.h_WW * MyBR.W_lnu * MyBR.W_lnu ); //H->hh ; hh->bbbb  hh->WWbb, hh->WWWW ;  W -> lnu;
  float Br_Hhh      = (MyBR.h_bb*MyBR.h_bb) + (MyBR.h_WW*MyBR.h_WW*MyBR.W_lnu*MyBR.W_lnu*MyBR.W_lnu*MyBR.W_lnu) + (2*MyBR.h_bb*MyBR.h_WW*MyBR.W_lnu*MyBR.W_lnu);
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
  cout<<"Signal: has a Cross section of: "<<Xsec_Hhh<<" and a BR of "<<Br_Hhh<<". The #Events at 300 fb-1 are: "<<(Xsec_Hhh * Br_Hhh * LumiInt)<<". The Weights is: "<<Weight_Hhh<<endl;
  cout<<"Backgr: has a Cross section of: "<<Xsec_tt<<" and a BR of "<<Br_tt<<". The #Events at 300 fb-1 are: "<<(Xsec_tt * Br_tt * LumiInt)<<". The Weights is: "<<Weight_tt<<endl;
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
  cout<<"You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
  float NormCro_S = Weight_Hhh, NormCro_B = Weight_tt;

  //Get Nevets
  TString selection = "mass_b1b2<300 && dR_l1l2>0 && dR_l1l2<2.5 && dR_b1b2>1 && dR_bl>1. && mass_l1l2<90. && mass_b1b2>50. && hasb1jet && hasb2jet && hasMET && hasGenMET && hasdRljet && hastwomuons && nu1and2_diBaxis_t>-900 && met_diBaxis_t>-900";
  selection += "&& MVA_bdt>0.1760";
  float Mysignal = T_Sig->Draw("dR_l1l2>>hist",selection,"goff") * NormCro_S; 
  float Mybackground = T_Bac->Draw("dR_l1l2>>hist",selection,"goff") * NormCro_B;;
  float Mydata = round(Mysignal + Mybackground);

  //Datacard
  TString name_txt = "/datacard";
  TString Thisname_txt = name_txt + "_CutCount.txt";
  FILE *file_txt=fopen( (folder + "/" + Thisname_txt).Data(),"w");
  cout<<"Now I created the "<<(folder + "/" + Thisname_txt)<<" file."<<endl;
  fprintf(file_txt,"# Simple counting experiment, with one signal and a few background processes \n");
  fprintf(file_txt,"imax 1  number of channels #You are defining N channels, where N=1,...,imax\n");
  fprintf(file_txt,"jmax 1  number of backgrounds \n");
  fprintf(file_txt,"kmax 2  number of nuisance parameters (sources of systematical uncertainties) \n"); //NSyst
  fprintf(file_txt,"------------ \n");
  fprintf(file_txt,"# we have just one channel, in which we observe 0 events \n");
  fprintf(file_txt,"bin 1 \n");
  fprintf(file_txt,"observation %.0f \n", Mydata); //Data? For now S+B
  fprintf(file_txt,"------------ \n");
  fprintf(file_txt,"# now we list the expected events for signal and all backgrounds in that bin \n");
  fprintf(file_txt,"# the second 'process' line must have a positive number for backgrounds, and 0 for signal \n");
  fprintf(file_txt,"# then we list the independent sources of uncertainties, and give their effect (syst. error) \n");
  fprintf(file_txt,"# on each process and bin \n");
  fprintf(file_txt,"bin              1     1 \n");
  fprintf(file_txt,"process         Hhh   tt \n");
  fprintf(file_txt,"process          0     1 \n");
  fprintf(file_txt,"rate            %.3f %.3f \n", Mysignal, Mybackground);
  fprintf(file_txt,"------------ \n");
  fprintf(file_txt,"lumi      lnN    1.11 1.11   lumi affects both signal and bkg. lnN = lognormal \n");
  fprintf(file_txt,"bg_others lnN    -    1.01   syst. on the background \n");
  fclose (file_txt);

  //Create root file for shape analysis
  TString rootFilePath = folder + "/Syst_file.root";
  TFile* file = new TFile(rootFilePath.Data(),"RECREATE");
  file->cd();
  TH1F *Hhh  = new TH1F( "Hhh", "", 200, 200., 600.);
  TH1F *tt  = new TH1F( "tt", "", 200, 200., 600.);
  TH1F *data_obs   = new TH1F( "data_obs", "", 200, 200., 600.);

  T_Sig->Draw("MMC_h2massweight1_prob>>Hhh",selection,"goff"); Hhh->Scale(Mysignal/Hhh->Integral());  
  T_Bac->Draw("MMC_h2massweight1_prob>>tt",selection,"goff"); tt->Scale(Mybackground/tt->Integral());
  for(int i=0; i<=data_obs->GetNbinsX(); i++){ //underflow, overflow included
    // It cannot be that you have data and not bkg:
    if(tt->GetBinContent(i)>0) data_obs->SetBinContent(i, Hhh->GetBinContent(i)+tt->GetBinContent(i));
  }

  //Shape analysis datacard
  Thisname_txt = name_txt + "_Shape.txt";
  FILE * file_shape_txt=fopen( (folder + "/" + Thisname_txt).Data(),"w");
  cout<<"Now I created the "<<(folder + "/" + Thisname_txt)<<" file."<<endl;
  fprintf(file_shape_txt,"imax 1 #You are defining N channels, where N=1,...,imax\n");
  fprintf(file_shape_txt,"jmax 1 #Number of backgrounds\n");
  fprintf(file_shape_txt,"kmax * #Number of nuisance parameters, here you just have to add them below\n");
  fprintf(file_shape_txt,"--------------- \n");
  fprintf(file_shape_txt,"shapes * * %s $PROCESS $PROCESS_$SYSTEMATIC \n",rootFilePath.Data());
  fprintf(file_shape_txt,"--------------- \n");
  fprintf(file_shape_txt,"bin 1 \n");
  fprintf(file_shape_txt,"observation %.0f \n", round(data_obs->Integral()));
  fprintf(file_shape_txt,"------------------------------ \n");
  fprintf(file_shape_txt,"bin             1          1 \n");
  fprintf(file_shape_txt,"process         Hhh       tt \n");
  fprintf(file_shape_txt,"process         0          1 \n");
  fprintf(file_shape_txt,"rate            %.3f      %.3f \n", Hhh->Integral(), tt->Integral());
  fprintf(file_shape_txt,"-------------------------------- \n");
  fprintf(file_shape_txt,"lumi     lnN    1.1       1.1 \n");
  //fprintf(file_shape_txt,"bgnorm   lnN    1.00       1.3 \n");
  //fprintf(file_shape_txt,"alpha  shapeN2    -           1   uncertainty on background shape and normalization \n");
  //fprintf(file_shape_txt,"sigma  shapeN2    0.5         -   uncertainty on signal resolution. Assume the histogram is a 2 sigma shift, \n");
  //fprintf(file_shape_txt,"#                                so divide the unit gaussian by 2 before doing the interpolation \n");
  fclose (file_shape_txt);

  Hhh->Write();
  tt->Write();
  data_obs->Write();
  file->Close();
  delete file;
}

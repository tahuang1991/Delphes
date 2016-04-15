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

void CreateDatacards( TString folder = "Outputs/", TString Sig_name = "DiHiggs_WWbb_1M_NewB3_allReco_25_MMC1M_dRminbl03_MVA_PU40_0404_combined.root", TString Bac_name = "TTbar_Wtomutau_allReco_Updatebtag_25_MVA_PU40_0411_combined.root" ){

  //Selections
  //TString selection = "hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons &&(hasb1jet || hasb2jet) && dR_l1l2<3.3 && dR_l1l2>0.07 && dR_b1b2<5. && mass_l1l2<100 && mass_l1l2>5. && mass_b1b2>22 && dR_bl<5 && dR_l1l2b1b2<6 && MINdR_bl<3.2 && MINdR_bl>0.4 && mass_b1b2<700 && mass_trans<250 && MT2<400 && pt_b1b2<300";
  TString selection = "preselections && (hasb1jet || hasb2jet) && dR_l1l2<3.3 && dR_b1b2<3.3&& TMath::Abs(dphi_l1l2b1b2)>1.&& mass_l1l2<75 && mass_b1b2<200 && mass_b1b2>50 && mass_l1l2>5. && dR_l1l2>0.07";
  TString selection_MVA = selection + " && MVA_value>0.5003";

  //Opening Inputs
  cout<<"Hello, I'm creating the '"<<folder<<"' folder, and opening the input files:"<<endl;
  cout<<"Signal: "<<Sig_name<<endl;
  cout<<"Background: "<<Bac_name<<endl;
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
  // Get the weigth
  TH1F *hW = new TH1F("hW","", 1000, 0., 50.);
  T_Sig->Draw("weight>>hW","","goff");
  float Weight_Hhh = hW->GetMean();
  T_Bac->Draw("weight>>hW","","goff");
  float Weight_tt  = 3.403;//hW->GetMean();
  cout<<"Optimisitc weights: W_s = "<<Weight_Hhh<<" and W_b = "<<Weight_tt<<endl;
  T_Sig->Draw("weight_pess>>hW","","goff");
  float Weight_Hhh_pess = Weight_Hhh * (0.360485189609*0.03464537035685626) / (1.1948649279633416*0.5035043253778491);//hW->GetMean();
  cout<<"Pessimistic weights: W_s = "<<Weight_Hhh_pess<<" and W_b = "<<Weight_tt<<endl;
  // Get #Entries
  float NEntries_S = T_Sig->Draw("dR_l1l2>>hist",selection,"goff");
  float NEntries_B = T_Bac->Draw("dR_l1l2>>hist",selection,"goff");
  cout<<"After cleaning selection:"<<endl;
  cout<<" Opt:  You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
  cout<<" Pess: You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh_pess<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
  float NEntries_MVA_S = T_Sig->Draw("dR_l1l2>>hist",selection_MVA,"goff");
  float NEntries_MVA_B = T_Bac->Draw("dR_l1l2>>hist",selection_MVA,"goff");
  cout<<"After cleaning selection + MVA:"<<endl;
  cout<<" Opt:  You have "<<NEntries_MVA_S<<"("<<NEntries_MVA_S*Weight_Hhh<<" weighted) entries for Signal and "<<NEntries_MVA_B<<"("<<NEntries_MVA_B*Weight_tt<<" weighted) for background."<<endl;
  cout<<" Pess: You have "<<NEntries_MVA_S<<"("<<NEntries_MVA_S*Weight_Hhh_pess<<" weighted) entries for Signal and "<<NEntries_MVA_B<<"("<<NEntries_MVA_B*Weight_tt<<" weighted) for background."<<endl;
  //Get #Events
  float Mysignal          = NEntries_MVA_S * Weight_Hhh; 
  float Mybackground      = NEntries_MVA_B * Weight_tt;
  float Mysignal_pess     = NEntries_MVA_S * Weight_Hhh_pess; 
  float Mydata(-999.);
  //Save Shape
  TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
  gStyle->SetOptStat(0);
  TLegend *leg = new TLegend(0.7,0.6,0.89,0.69);
  leg->SetFillColor(0); leg->SetLineColor(0);
  TH1F *mmc_S = new TH1F("mmc_S","", 50, 100., 700.);
  T_Sig->Draw("MMC_h2massweight1_prob>>mmc_S",selection_MVA,"goff");
  TH1F *mmc_B = new TH1F("mmc_B","", 50, 100., 700.);
  T_Bac->Draw("MMC_h2massweight1_prob>>mmc_B",selection_MVA,"goff");
  mmc_S->Scale(Mysignal/mmc_S->Integral());
  mmc_B->Scale(Mybackground/mmc_B->Integral());
  mmc_B->SetLineColor(kBlue); mmc_S->SetLineColor(kRed);
  mmc_B->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_B->GetYaxis()->SetTitle("Entries");
  mmc_B->Draw(); mmc_S->Draw("same");
  leg->AddEntry(mmc_S,"Hhh","l");
  leg->AddEntry(mmc_B,"tt","l");
  leg->Draw("same");
  myc1->SetLogy(1);
  myc1->SaveAs(folder + "/MMC_log.pdf");
  mmc_S->Scale(Mysignal_pess/mmc_S->Integral());
  mmc_B->Draw(); mmc_S->Draw("same");
  leg->Draw("same");
  myc1->SaveAs(folder + "/MMC_log_pess.pdf");
  delete myc1;

  //Datacard
  TString name_txt = "/datacard";
  //Optimistic and pessimistic
  for(int iV=0; iV<2; iV++){
    float Nsig = Mysignal;
    if( iV==1 ) Nsig = Mysignal_pess;
    //Cut and count (Significance and limits)
    for(int i=0; i<2; i++){
      TString Thisname_txt;
      if( i==0 && iV==0 ){ Thisname_txt = name_txt + "_CutCount_Significance.txt";      Mydata = Nsig + Mybackground; }
      if( i==0 && iV==0 ){ Thisname_txt = name_txt + "_CutCount_Significance_pess.txt"; Mydata = Nsig + Mybackground; }
      if( i==1 && iV==0 ){ Thisname_txt = name_txt + "_CutCount_Limits.txt";      Mydata = Mybackground; }
      if( i==1 && iV==1 ){ Thisname_txt = name_txt + "_CutCount_Limits_pess.txt"; Mydata = Mybackground; }
      FILE *file_txt=fopen( (folder + "/" + Thisname_txt).Data(),"w");
      cout<<"Now I created the "<<(folder + "/" + Thisname_txt)<<" file."<<endl;
      fprintf(file_txt,"# Simple counting experiment, with one signal and a few background processes \n");
      fprintf(file_txt,"imax 1  number of channels #You are defining N channels, where N=1,...,imax\n");
      fprintf(file_txt,"jmax 1  number of backgrounds \n");
      fprintf(file_txt,"kmax 2  number of nuisane parameters (sources of systematical uncertainties) \n"); //NSyst
      fprintf(file_txt,"------------ \n");
      fprintf(file_txt,"# we have just one channel, in which we observe 0 events \n");
      fprintf(file_txt,"bin 1 \n");
      fprintf(file_txt,"observation %.0f \n", Mydata);
      fprintf(file_txt,"------------ \n");
      fprintf(file_txt,"# now we list the expected events for signal and all backgrounds in that bin \n");
      fprintf(file_txt,"# the second 'process' line must have a positive number for backgrounds, and 0 for signal \n");
      fprintf(file_txt,"# then we list the independent sources of uncertainties, and give their effect (syst. error) \n");
      fprintf(file_txt,"# on each process and bin \n");
      fprintf(file_txt,"bin              1     1 \n");
      fprintf(file_txt,"process         Hhh   tt \n");
      fprintf(file_txt,"process          0     1 \n");
      fprintf(file_txt,"rate            %.3f %.3f \n", Nsig, Mybackground);
      fprintf(file_txt,"------------ \n");
      fprintf(file_txt,"lumi      lnN    1.11 1.11   lumi affects both signal and bkg. lnN = lognormal \n");
      fprintf(file_txt,"bg_others lnN    -    1.01   syst. on the background \n");
      fclose (file_txt);
    }
  
    //Create root file for shape analysis
    TString rootFilePath = folder + "/Syst_file.root";
    if( iV==1) rootFilePath = folder + "/Syst_file_pess.root";
    TFile* file = new TFile(rootFilePath.Data(),"RECREATE");
    file->cd();
    //Float_t BinRange[]={200.,300.,400.,500.,600.,700.,800.,900.};
    //Float_t BinRange[]={200.,250.,300.,350,400.,450.,500.,600.,700.,800.,900.};
    Float_t BinRange[]={200.,225,250.,275,300.,325,350,375,400.,425,450.,500.,550.,600.,650,700.,800.,900.};
    Int_t   Binnum = sizeof(BinRange)/sizeof(Float_t) - 1;
    TH1F *Hhh      = new TH1F( "Hhh", "", Binnum, BinRange);
    TH1F *tt       = new TH1F( "tt", "", Binnum, BinRange);
    TH1F *data_obs = new TH1F( "data_obs", "", Binnum, BinRange);
    T_Sig->Draw("MMC_h2massweight1_prob>>Hhh",selection_MVA,"goff"); Hhh->Scale(Nsig/Hhh->Integral());  
    T_Bac->Draw("MMC_h2massweight1_prob>>tt",selection_MVA,"goff"); tt->Scale(Mybackground/tt->Integral());
  
    //Shape analysis datacard
    for(int i=0; i<2; i++){
      TString Thisname_txt = name_txt + "_Shape";
      if( i==0 && iV==0 ){ Thisname_txt = name_txt + "_Shape_Significance.txt"; }
      if( i==0 && iV==1 ){ Thisname_txt = name_txt + "_Shape_Significance_pess.txt"; }
      if( i==1 && iV==0 ){ Thisname_txt = name_txt + "_Shape_Limits.txt"; }
      if( i==1 && iV==1 ){ Thisname_txt = name_txt + "_Shape_Limits_pess.txt"; }
      for(int j=0; j<=data_obs->GetNbinsX(); j++){ //underflow, overflow included
        data_obs->SetBinContent(j, Hhh->GetBinContent(j)+tt->GetBinContent(j));
        // It cannot be that you have data and not bkg:
        //if(i==0 && tt->GetBinContent(j)>0) data_obs->SetBinContent(j, Hhh->GetBinContent(j)+tt->GetBinContent(j));
        //if(i==1 && tt->GetBinContent(j)>0) data_obs->SetBinContent(j, tt->GetBinContent(j));
      }
      data_obs->Scale((Hhh->Integral()+tt->Integral())/data_obs->Integral());
      FILE * file_shape_txt=fopen( (folder + "/" + Thisname_txt).Data(),"w");
      cout<<"Now I created the "<<(folder + "/" + Thisname_txt)<<" file."<<endl;
      fprintf(file_shape_txt,"imax 1 #You are defining N channels, where N=1,...,imax\n");
      fprintf(file_shape_txt,"jmax 1 #Number of backgrounds\n");
      fprintf(file_shape_txt,"kmax * #Number of nuisance parameters, here you just have to add them below\n");
      fprintf(file_shape_txt,"--------------- \n");
      fprintf(file_shape_txt,"shapes * * %s $PROCESS $PROCESS_$SYSTEMATIC \n",rootFilePath.Data());
      fprintf(file_shape_txt,"--------------- \n");
      fprintf(file_shape_txt,"bin 1 \n");
      fprintf(file_shape_txt,"observation %.0f \n", data_obs->Integral());
      fprintf(file_shape_txt,"------------------------------ \n");
      fprintf(file_shape_txt,"bin             1          1 \n");
      fprintf(file_shape_txt,"process         Hhh       tt \n");
      fprintf(file_shape_txt,"process         0          1 \n");
      fprintf(file_shape_txt,"rate            %.3f      %.3f \n", Hhh->Integral(), tt->Integral());
      fprintf(file_shape_txt,"-------------------------------- \n");
      fprintf(file_shape_txt,"lumi     lnN    1.0000001       1.0000001 \n");
      //fprintf(file_shape_txt,"bgnorm   lnN    1.00       1.3 \n");
      //fprintf(file_shape_txt,"alpha  shapeN2    -           1   uncertainty on background shape and normalization \n");
      //fprintf(file_shape_txt,"sigma  shapeN2    0.5         -   uncertainty on signal resolution. Assume the histogram is a 2 sigma shift, \n");
      //fprintf(file_shape_txt,"#                                so divide the unit gaussian by 2 before doing the interpolation \n");
      fclose (file_shape_txt);
    }
    Hhh->Write();
    tt->Write();
    data_obs->Write();
    file->Close();
  delete file;
  }
}

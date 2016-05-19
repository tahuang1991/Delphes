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
#define N_Signals 12

using namespace std;
struct BRs {
  float h_bb; float h_WW; float W_lnu; float W_munu; float t_WB;
} MyBR;

void CreateDatacards( TString folder = "Outputs/" ){

  //Signals 
  TString name_base1 = "/fdata/hepx/store/user/lpernie/Hhh/delphes_";
  TString name_base2 = "_1M_PU40ALL_13May_MVAMMC.root";
  TString Sig_name[N_Signals] = {name_base1+"B1"+name_base2, name_base1+"B2"+name_base2, name_base1+"B3"+name_base2, name_base1+"B4"+name_base2, name_base1+"B5"+name_base2, name_base1+"B6"+name_base2, name_base1+"B7"+name_base2, name_base1+"B8"+name_base2, name_base1+"B9"+name_base2, name_base1+"B10"+name_base2, name_base1+"B11"+name_base2, name_base1+"B12"+name_base2};
  TString Bac_name = "/fdata/hepx/store/user/lpernie/Hhh/delphes_tt_4M_PU40_WtobtaumuALL_13May_MVAMMC.root";

  //Selections
  //TString selection = "hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons &&(hasb1jet || hasb2jet) && dR_l1l2<3.3 && dR_l1l2>0.07 && dR_b1b2<5. && mass_l1l2<100 && mass_l1l2>5. && mass_b1b2>22 && dR_bl<5 && dR_l1l2b1b2<6 && MINdR_bl<3.2 && MINdR_bl>0.4 && mass_b1b2<700 && mass_trans<250 && MT2<400 && pt_b1b2<300";
  TString selection = "(((b1jet_btag&2)>0 && (b2jet_btag&3)>0) || ((b1jet_btag&3)>0 && (b 2jet_btag&2)>0)) && hasRECOjet1 && hasRECOjet1 && hasMET && hastwomuons && dR_l1l2<3.3 && dR_l1l2>0.07 && dR_b1b2<5. && mass_l1l2<100 && mass_l1l2>5. && mass_b1b2>22";
  TString selection_MVA[N_Signals]   = {" && MVA_value>0.0704", " && MVA_value>0.4664", " && MVA_value>0.5803", " && MVA_value>0.1453", " && MVA_value>1.0042", " && MVA_value>0.2080", " && MVA_value>0.9642", " && MVA_value>0.2234", " && MVA_value>0.1464", " && MVA_value>0.9468", " && MVA_value>0.9501", " && MVA_value>0.9501" };
  TString selection_MVAtt[N_Signals] = {" && MVA_value_B1fortt>0.0704", " && MVA_value_B2fortt>0.4664", " && MVA_value_B3fortt>0.5803", " && MVA_value_B4fortt>0.1453", " && MVA_value_B5fortt>1.0042", " && MVA_value_B6fortt>0.2080", " && MVA_value_B7fortt>0.9642", " && MVA_value_B8fortt>0.2234", " && MVA_value_B9fortt>0.1464", " && MVA_value_B10fortt>0.9468", " && MVA_value_B11fortt>0.9501", " && MVA_value_B12fortt>0.9501" };

  //Opening Inputs
  TString Comm = "mkdir -p " + folder;
  system( Comm.Data() );

  for(int nS=0; nS<N_Signals; nS++){
    cout<<"Hello, I'm creating the '"<<folder<<"' folder, and opening the input files:"<<endl;
    cout<<"Signal: "<<Sig_name[nS]<<endl;
    cout<<"Background: "<<Bac_name<<endl;
    TFile* f_Sig = TFile::Open(Sig_name[nS].Data());
    if( !f_Sig ) cout<<"Waring! File: "<<Sig_name[nS]<<" not present!"<<endl;
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
    float Weight_tt  = hW->GetMean();
    cout<<"Optimisitc weights: W_s = "<<Weight_Hhh<<" and W_b = "<<Weight_tt<<endl;
    T_Sig->Draw("weight_pess>>hW","","goff");
    float Weight_Hhh_pess = hW->GetMean();
    cout<<"Pessimistic weights: W_s = "<<Weight_Hhh_pess<<" and W_b = "<<Weight_tt<<endl;
    // Get #Entries
    float NEntries_S = T_Sig->Draw("dR_l1l2>>hist","reweighting * (" + selection + ")","goff");
    float NEntries_B = T_Bac->Draw("dR_l1l2>>hist","reweighting * (" + selection + ")","goff");
    cout<<"After cleaning selection:"<<endl;
    cout<<" Opt:  You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
    cout<<" Pess: You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh_pess<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
    float NEntries_MVA_S = T_Sig->Draw("dR_l1l2>>hist","reweighting * (" + selection + selection_MVA[nS] + ")","goff");
    float NEntries_MVA_B = T_Bac->Draw("dR_l1l2>>hist","reweighting * (" + selection + selection_MVAtt[nS] + ")","goff");
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
    T_Sig->Draw("MMC_h2massweight1_prob>>mmc_S","reweighting * (" + selection + selection_MVA[nS] + ")","goff");
    TH1F *mmc_B = new TH1F("mmc_B","", 50, 100., 700.);
    T_Bac->Draw("MMC_h2massweight1_prob>>mmc_B","reweighting * (" + selection + selection_MVAtt[nS] + ")","goff");
    mmc_S->Scale(Mysignal/mmc_S->Integral());
    mmc_B->Scale(Mybackground/mmc_B->Integral());
    mmc_B->SetLineColor(kBlue); mmc_S->SetLineColor(kRed);
    mmc_B->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_B->GetYaxis()->SetTitle("Entries");
    mmc_B->Draw(); mmc_S->Draw("same");
    leg->AddEntry(mmc_S,"Hhh","l");
    leg->AddEntry(mmc_B,"tt","l");
    leg->Draw("same");
    myc1->SetLogy(1);
    myc1->SaveAs(folder + "/MMC_log_" + std::to_string(nS) + ".pdf");
    mmc_S->Scale(Mysignal_pess/mmc_S->Integral());
    mmc_B->Draw(); mmc_S->Draw("same");
    leg->Draw("same");
    myc1->SaveAs(folder + "/MMC_log_pess_" + std::to_string(nS) + ".pdf");
    delete myc1;

    //Datacard
    TString name_txt = "/datacard";
    //Optimistic and pessimistic
    for(int iV=0; iV<2; iV++){
	float Nsig = Mysignal;
	if( iV==1 ) Nsig = Mysignal_pess;
	//Cut and count (Significance and limits)
	//for(int i=0; i<2; i++){
	TString Thisname_txt;
	if( /*i==0 &&*/ iV==0 ){ Thisname_txt = name_txt + "_CutCount_" + std::to_string(nS) + ".txt";      Mydata = Nsig + Mybackground; }
	if( /*i==0 &&*/ iV==1 ){ Thisname_txt = name_txt + "_CutCount_pess_" + std::to_string(nS) + ".txt"; Mydata = Nsig + Mybackground; }
	//if( i==1 && iV==0 ){ Thisname_txt = name_txt + "_CutCount_Limits.txt";      Mydata = Mybackground; }
	//if( i==1 && iV==1 ){ Thisname_txt = name_txt + "_CutCount_Limits_pess.txt"; Mydata = Mybackground; }
	float syst_B = 0.000001;//sqrt(Mybackground)/Mybackground;
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
	fprintf(file_txt,"S lnN    1.000000001  -   lumi affects both signal and bkg. lnN = lognormal \n");
	fprintf(file_txt,"B lnN     -  %.2f syst. on the background \n", syst_B);
	fclose (file_txt);
	//}
	cout<<"Now Shape analysis!"<<endl;
	//Create root file for shape analysis
	TString rootFilePath = folder + "/Syst_file_" + std::to_string(nS) + ".root";
	if( iV==1 ) rootFilePath = folder + "/Syst_file_pess_" + std::to_string(nS) + ".root";
	TFile* file = new TFile(rootFilePath.Data(),"RECREATE");
	file->cd();
	//Float_t BinRange[]={200.,300.,400.,500.,600.,700.,800.,900.};
	//Float_t BinRange[]={200.,250.,300.,350,400.,450.,500.,600.,700.,800.,900.};
	Float_t BinRange[]={200.,225,250.,275,300.,325,350,375,400.,425,450.,500.,550.,600.,650,700.,800.,900.};
	Int_t   Binnum = sizeof(BinRange)/sizeof(Float_t) - 1;
	TH1F *Hhh      = new TH1F( "Hhh", "", Binnum, BinRange);
	TH1F *tt       = new TH1F( "tt", "", Binnum, BinRange);
	TH1F *data_obs = new TH1F( "data_obs", "", Binnum, BinRange);
	T_Sig->Draw("MMC_h2massweight1_prob>>Hhh","reweighting * (" + selection + selection_MVA[nS] + ")","goff"); Hhh->Scale(Nsig/Hhh->Integral());  
	T_Bac->Draw("MMC_h2massweight1_prob>>tt","reweighting * (" + selection + selection_MVAtt[nS] + ")","goff"); tt->Scale(Mybackground/tt->Integral());

	//Shape analysis datacard
	//for(int i=0; i<2; i++){
	TString Thisname_txt2;
	if( /*i==0 &&*/ iV==0 ){ Thisname_txt2 = name_txt + "_Shape_" + std::to_string(nS) + ".txt"; }
	if( /*i==0 &&*/ iV==1 ){ Thisname_txt2 = name_txt + "_Shape_pess_" + std::to_string(nS) + ".txt"; }
	//if( i==1 && iV==0 ){ Thisname_txt2 = name_txt + "_Shape_Limits.txt"; }
	//if( i==1 && iV==1 ){ Thisname_txt2 = name_txt + "_Shape_Limits_pess.txt"; }
	for(int j=0; j<=data_obs->GetNbinsX(); j++){ //underflow, overflow included
	  ////data_obs->SetBinContent(j, Hhh->GetBinContent(j)+tt->GetBinContent(j));
	  data_obs->SetBinContent(j, tt->GetBinContent(j));
	  // It cannot be that you have data and not bkg:
	  //if(i==0 && tt->GetBinContent(j)>0) data_obs->SetBinContent(j, Hhh->GetBinContent(j)+tt->GetBinContent(j));
	  //if(i==1 && tt->GetBinContent(j)>0) data_obs->SetBinContent(j, tt->GetBinContent(j));
	}
	////data_obs->Scale((Hhh->Integral()+tt->Integral())/data_obs->Integral());
	float syst_BB = 0.000001;//sqrt(tt->Integral())/tt->Integral();
	FILE *file_shape_txt=fopen( (folder + "/" + Thisname_txt2).Data(),"w");
	cout<<"Now I created the "<<(folder + "/" + Thisname_txt2)<<" file."<<endl;
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
	fprintf(file_shape_txt,"S lnN    1.01  -   lumi affects both signal and bkg. lnN = lognormal \n");
	fprintf(file_shape_txt,"B lnN     -  %.2f syst. on the background \n", syst_BB);
	//fprintf(file_shape_txt,"bgnorm   lnN    1.00       1.3 \n");
	//fprintf(file_shape_txt,"alpha  shapeN2    -           1   uncertainty on background shape and normalization \n");
	//fprintf(file_shape_txt,"sigma  shapeN2    0.5         -   uncertainty on signal resolution. Assume the histogram is a 2 sigma shift, \n");
	//fprintf(file_shape_txt,"#                                so divide the unit gaussian by 2 before doing the interpolation \n");
	fclose (file_shape_txt);
	//}
	Hhh->Write();
	tt->Write();
	data_obs->Write();
	file->Close();
	delete file;
    }
  }
}

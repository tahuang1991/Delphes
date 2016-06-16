#include <TH1F.h>
#include <TFile.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TH2F.h>
#include <TH1D.h>
#include <THStack.h>
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
  float BR = 0.35977;

  //Selections
  TString selection = "(((b1jet_btag&2)>0 && (b2jet_btag&3)>0) || ((b1jet_btag&3)>0 && (b 2jet_btag&2)>0)) && hasRECOjet1 && hasRECOjet2 && hasMET && hastwomuons && dR_l1l2<3.3 && dR_l1l2>0.07 && dR_b1b2<5. && mass_l1l2<100 && mass_l1l2>5. && mass_b1b2>22 && MMC_h2massweight1_prob>0";
  TString selection_MVA[N_Signals]   = {" && MVA_value>0.0704", " && MVA_value>0.4664", " && MVA_value>0.5803", " && MVA_value>0.1453", " && MVA_value>1.0042", " && MVA_value>0.2080", " && MVA_value>0.9642", " && MVA_value>0.2234", " && MVA_value>0.1464", " && MVA_value>0.9468", " && MVA_value>0.9501", " && MVA_value>0.9501" };
  TString selection_MVAtt[N_Signals] = {" && MVA_value_B1fortt>0.0704", " && MVA_value_B2fortt>0.4664", " && MVA_value_B3fortt>0.5803", " && MVA_value_B4fortt>0.1453", " && MVA_value_B5fortt>1.0042", " && MVA_value_B6fortt>0.2080", " && MVA_value_B7fortt>0.9642", " && MVA_value_B8fortt>0.2234", " && MVA_value_B9fortt>0.1464", " && MVA_value_B10fortt>0.9468", " && MVA_value_B11fortt>0.9501", " && MVA_value_B12fortt>0.9501" };
  //TString selection_MVA[N_Signals] = {"","","","","","","","","","","",""};
  //TString selection_MVAtt[N_Signals] = {"","","","","","","","","","","",""};
  Float_t BinRange[]={200.,225,250.,275,300.,325,350,375,400.,425,450.,500.,550.,600.,650,700.,800.,1000.};
  Int_t   Binnum = sizeof(BinRange)/sizeof(Float_t) - 1;

  //Opening Inputs
  TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
  TString Comm = "mkdir -p " + folder + "/Plots";
  system( Comm.Data() );

  for(int nS=0; nS<N_Signals; nS++){
    cout<<"-------  Signal: "<<Sig_name[nS]<<"  --------"<<endl;
    cout<<"-------  Background: "<<Bac_name<<"  --------"<<endl;
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
    //cout<<"Pessimistic weights: W_s = "<<Weight_Hhh_pess<<" and W_b = "<<Weight_tt<<endl;
    // Get #Entries
    TH1F *histSinit = new TH1F("histSinit","", 100, 0., 50.);
    TH1F *histS = new TH1F("histS","", 100, 0., 50.);
    TH1F *histB = new TH1F("histB","", 100, 0., 50.);
    T_Sig->Draw("weight>>histS","reweighting * (" + selection + ")","goff");
    T_Bac->Draw("weight>>histB","reweighting * (" + selection + ")","goff");
    float NEntries_S = histS->Integral();
    float NEntries_B = histB->Integral();
    cout<<" Selection)  You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
    //cout<<" Pess: You have "<<NEntries_S<<"("<<NEntries_S*Weight_Hhh_pess<<" weighted) entries for Signal and "<<NEntries_B<<"("<<NEntries_B*Weight_tt<<" weighted) for background."<<endl;
    T_Sig->Draw("weight>>histS","reweighting * (" + selection + selection_MVA[nS] + ")","goff");
    T_Bac->Draw("weight>>histB","reweighting * (" + selection + selection_MVAtt[nS] + ")","goff");
    float NEntries_MVA_S = histS->Integral(); 
    float NEntries_MVA_B = histB->Integral();
    cout<<" Selection_MVA)  You have "<<NEntries_MVA_S<<"("<<NEntries_MVA_S*Weight_Hhh<<" weighted) entries for Signal and "<<NEntries_MVA_B<<"("<<NEntries_MVA_B*Weight_tt<<" weighted) for background."<<endl;
    //cout<<" Pess: You have "<<NEntries_MVA_S<<"("<<NEntries_MVA_S*Weight_Hhh_pess<<" weighted) entries for Signal and "<<NEntries_MVA_B<<"("<<NEntries_MVA_B*Weight_tt<<" weighted) for background."<<endl;
    //Get #Events
    float Mysignal_init       = 1000000 * Weight_Hhh; 
    float Mysignal            = NEntries_MVA_S * Weight_Hhh; 
    float Mybackground        = NEntries_MVA_B * Weight_tt;
    float Mysignal_pess       = NEntries_MVA_S * Weight_Hhh_pess; 
    float Mydata(-999.);
    //tt after all MVAs cuts
    if(nS==0){
	TH1F *mmc_tt0 = new TH1F("mmc_tt0","", Binnum, BinRange); mmc_tt0->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt0->SetLineColor(kBlack);
	TH1F *mmc_tt1 = new TH1F("mmc_tt1","", Binnum, BinRange); mmc_tt1->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt1->SetLineColor(kOrange);
	TH1F *mmc_tt2 = new TH1F("mmc_tt2","", Binnum, BinRange); mmc_tt2->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt2->SetLineColor(kGreen); 
	TH1F *mmc_tt3 = new TH1F("mmc_tt3","", Binnum, BinRange); mmc_tt3->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt3->SetLineColor(kBlue);
	TH1F *mmc_tt4 = new TH1F("mmc_tt4","", Binnum, BinRange); mmc_tt4->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt4->SetLineColor(kAzure-4);
	TH1F *mmc_tt5 = new TH1F("mmc_tt5","", Binnum, BinRange); mmc_tt5->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt5->SetLineColor(kAzure);
	TH1F *mmc_tt6 = new TH1F("mmc_tt6","", Binnum, BinRange); mmc_tt6->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt6->SetLineColor(kAzure+4);
	TH1F *mmc_tt7 = new TH1F("mmc_tt7","", Binnum, BinRange); mmc_tt7->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt7->SetLineColor(kAzure+6);
	TH1F *mmc_tt8 = new TH1F("mmc_tt8","", Binnum, BinRange); mmc_tt8->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt8->SetLineColor(kAzure+8);
	TH1F *mmc_tt9 = new TH1F("mmc_tt9","", Binnum, BinRange); mmc_tt9->GetXaxis()->SetTitle("MMC mass [GeV]"); mmc_tt9->SetLineColor(kAzure+10);
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt0","weight * reweighting * (" + selection + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt1","weight * reweighting * (" + selection + selection_MVAtt[0] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt2","weight * reweighting * (" + selection + selection_MVAtt[1] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt3","weight * reweighting * (" + selection + selection_MVAtt[2] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt4","weight * reweighting * (" + selection + selection_MVAtt[3] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt5","weight * reweighting * (" + selection + selection_MVAtt[4] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt6","weight * reweighting * (" + selection + selection_MVAtt[5] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt7","weight * reweighting * (" + selection + selection_MVAtt[6] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt8","weight * reweighting * (" + selection + selection_MVAtt[7] + ")","goff");
	T_Bac->Draw("MMC_h2massweight1_prob>>mmc_tt9","weight * reweighting * (" + selection + selection_MVAtt[8] + ")","goff");
	mmc_tt0->Draw("H");
	mmc_tt1->Draw("Hsame");
	mmc_tt2->Draw("Hsame");
	mmc_tt3->Draw("Hsame");
	mmc_tt4->Draw("Hsame");
	mmc_tt5->Draw("Hsame");
	mmc_tt6->Draw("Hsame");
	mmc_tt7->Draw("Hsame");
	mmc_tt8->Draw("Hsame");
	mmc_tt9->Draw("Hsame");
	myc1->SaveAs(folder + "/Plots/MMC_afterMvas.pdf");
	myc1->SetLogy(1);
	myc1->SaveAs(folder + "/Plots/MMC_afterMvas_log.pdf");
    }
    //Save Shape
    for(int Nplot=0; Nplot<2; Nplot++){//0=no MVA cuts
	myc1->SetLogy(1);
	gStyle->SetOptStat(0);
	TLegend *leg = new TLegend(0.7,0.7,0.89,0.79);
	leg->SetFillColor(0); leg->SetLineColor(0);
	THStack *hs = new THStack("hs","");
	TH1F *mmc_S = new TH1F("mmc_S","", Binnum, BinRange); 
	if(Nplot==0) T_Sig->Draw("MMC_h2massweight1_prob>>mmc_S","weight * reweighting * (" + selection + selection_MVA[nS] + ")","goff");
	if(Nplot==1) T_Sig->Draw("MMC_h2massweight1_prob>>mmc_S","weight * reweighting * (" + selection + ")","goff");
	TH1F *mmc_B = new TH1F("mmc_B","", Binnum, BinRange); mmc_B->GetXaxis()->SetTitle("MMC mass [GeV]");
	if(Nplot==0) T_Bac->Draw("MMC_h2massweight1_prob>>mmc_B","weight * reweighting * (" + selection + selection_MVAtt[nS] + ")","goff");
	if(Nplot==1) T_Bac->Draw("MMC_h2massweight1_prob>>mmc_B","weight * reweighting * (" + selection + ")","goff");
	TH1F *h1 = new TH1F("h1","", Binnum, BinRange); h1->GetXaxis()->SetTitle("MMC Mass [GeV]");
	for(int nbins=0; nbins<mmc_S->GetNbinsX(); nbins++) h1->SetBinContent(nbins+1, mmc_S->GetBinContent(nbins+1) );
	h1->SetFillColor(kRed); hs->Add(h1);
	TH1F *h2 = new TH1F("h2","", Binnum, BinRange); h2->GetXaxis()->SetTitle("MMC Mass [GeV]");
	for(int nbins=0; nbins<mmc_B->GetNbinsX(); nbins++) h2->SetBinContent(nbins+1, mmc_B->GetBinContent(nbins+1));
	h2->SetFillColor(kBlue); hs->Add(h2);
	hs->Draw("");
	leg->AddEntry(h1,"Hhh","f");
	leg->AddEntry(h2,"tt","f");
	leg->Draw("same");
	if(Nplot==0) myc1->SaveAs(folder + "/Plots/MMC_log_" + std::to_string(nS+1) + ".pdf");
	if(Nplot==1) myc1->SaveAs(folder + "/Plots/MMC_log_noMVA_" + std::to_string(nS+1) + ".pdf");
	myc1->SetLogy(0);
	hs->Draw(""); leg->Draw("same");
	if(Nplot==0) myc1->SaveAs(folder + "/Plots/MMC_" + std::to_string(nS+1) + ".pdf");
	if(Nplot==1) myc1->SaveAs(folder + "/Plots/MMC_noMVA_" + std::to_string(nS+1) + ".pdf");
	delete h1; delete h2; delete hs; delete mmc_S; delete mmc_B; delete leg;
    }
    //Datacard
    TString name_txt = "/datacard";
    //Optimistic and pessimistic
    double Eff_gobal(-1.);
    for(int iV=0; iV<2; iV++){
	float Nsig = Mysignal;
	if( iV==1 ) Nsig = Mysignal_pess;
	Eff_gobal = Nsig/Mysignal_init;
	//Cut and count (Significance and limits)
	TString Thisname_txt;
	if( iV==0 ){ Thisname_txt = name_txt + "_CutCount_" + std::to_string(nS+1) + ".txt";      Mydata = Mybackground; }
	if( iV==1 ){ Thisname_txt = name_txt + "_CutCount_pess_" + std::to_string(nS+1) + ".txt"; Mydata = Mybackground; }
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
	fprintf(file_txt,"observation %.3f \n", Mydata);
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
	fprintf(file_txt,"# (S * %.3f)/(300*1000*%.5f*%.5f) \n", Nsig, Eff_gobal, BR);
	fclose (file_txt);
	cout<<"Now Shape analysis!"<<endl;
	//Create root file for shape analysis
	TString rootFilePath = folder + "/Syst_file_" + std::to_string(nS+1) + ".root";
	if( iV==1 ) rootFilePath = folder + "/Syst_file_pess_" + std::to_string(nS+1) + ".root";
	TFile* file = new TFile(rootFilePath.Data(),"RECREATE");
	file->cd();
	TH1F *Hhh      = new TH1F( "Hhh", "", Binnum, BinRange);
	TH1F *tt       = new TH1F( "tt", "", Binnum, BinRange);
	TH1F *data_obs = new TH1F( "data_obs", "", Binnum, BinRange);
	T_Sig->Draw("MMC_h2massweight1_prob>>Hhh","weight * reweighting * (" + selection + selection_MVA[nS] + ")","goff"); 
	T_Bac->Draw("MMC_h2massweight1_prob>>tt","weight * reweighting * (" + selection + selection_MVAtt[nS] + ")","goff");
	for(int j=0; j<=data_obs->GetNbinsX(); j++) data_obs->SetBinContent(j, tt->GetBinContent(j));
	//Shape analysis datacard
	TString Thisname_txt2;
	if( iV==0 ){ Thisname_txt2 = name_txt + "_Shape_" + std::to_string(nS+1) + ".txt"; }
	if( iV==1 ){ Thisname_txt2 = name_txt + "_Shape_pess_" + std::to_string(nS+1) + ".txt"; }

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
	fprintf(file_shape_txt,"observation %.3f \n", data_obs->Integral());
	fprintf(file_shape_txt,"------------------------------ \n");
	fprintf(file_shape_txt,"bin             1          1 \n");
	fprintf(file_shape_txt,"process         Hhh       tt \n");
	fprintf(file_shape_txt,"process         0          1 \n");
	fprintf(file_shape_txt,"rate            %.3f      %.3f \n", Hhh->Integral(), tt->Integral());
	fprintf(file_shape_txt,"-------------------------------- \n");
	fprintf(file_shape_txt,"S lnN    1.0000001  -   lumi affects both signal and bkg. lnN = lognormal \n");
	fprintf(file_shape_txt,"B lnN     -  %.2f syst. on the background \n", syst_BB);
	//fprintf(file_shape_txt,"bgnorm   lnN    1.00       1.3 \n");
	//fprintf(file_shape_txt,"alpha  shapeN2    -           1   uncertainty on background shape and normalization \n");
	//fprintf(file_shape_txt,"sigma  shapeN2    0.5         -   uncertainty on signal resolution. Assume the histogram is a 2 sigma shift, \n");
	//fprintf(file_shape_txt,"#                                so divide the unit gaussian by 2 before doing the interpolation \n");
	fprintf(file_shape_txt,"# (S * %.3f)/(300*1000*%.5f*%.5f) \n", Nsig, Eff_gobal, BR);
	fclose (file_shape_txt);
	Hhh->Write();
	tt->Write();
	data_obs->Write();
	file->Close();
	delete file;
    }
  }
}

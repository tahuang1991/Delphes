#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <set>
#include <cmath>
#include <cstdlib>
#include "TFile.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TH1.h"
#include "TF1.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"

using namespace std;

enum { S_and_B = 0, Only_S = 1, Only_B = 2};
enum { n_Sel = 0, n_Tag = 1, n_nBin = 2, n_nBinMin = 3, n_nBinMax = 4, n_Norm1 = 5, n_Topo = 6, n_gSty = 7 };

vector<TString> InitVec( TString Var, TString Sel, TString nBin, TString nBinMin, TString nBinMax, TString Norm1, TString Topo, TString gSty );
void Make2Plot(TTree * TS, TTree * TB, TString Var, TString Sel, TString Tag, TString Dir, float nBin, float binMin, float binMax, bool Norm1, int Topology, int gSty);
float GetMax(TH1F *h);
int GetHigherH(TH1F *h1, TH1F *h2);

void Plotter_DiHiggs( TString folder="Plots_FileOptim_B6", TString File_S="Output/delphes_B6_1M_PU0_Btag_noMMCnoMVA.root", TString File_B="Output/delphes_ttbar_1M_PU0_Wtobmu_noMMCnoMVA.root" ){
  gROOT->Reset(); 
  // Topology
  int Topology =- 1;
  if( File_S != "" && File_B != "" ) Topology = S_and_B;
  if( File_S != "" && File_B == "" ) Topology = Only_S;
  if( File_S == "" && File_B != "" ) Topology = Only_B;
  // Open Input files
  TFile* fS = NULL, *fB = NULL;
  TTree* TS = NULL, *TB = NULL;
  if( Topology == S_and_B || Topology == Only_S ){
    fS = TFile::Open( File_S.Data() );
    if(!fS){
	cout << "Invalid file: " << File_S << " .. try again" << endl;
	return;
    }
    TS = (TTree*) fS->Get("evtree");
    if(!TS){
	cout << "Invalid TTree: evtree .. try again" << endl;
	return;
    }
  }
  if( Topology == S_and_B || Topology == Only_B ){
    fB = TFile::Open( File_B.Data() );
    if(!fB){
	cout << "Invalid file: " << File_B << " .. try again" << endl;
	return;
    }
    TB = (TTree*) fB->Get("evtree");
    if(!TB){
	cout << "Invalid TTree: evtree .. try again" << endl;
	return;
    }
  }
  // Create output folder
  TString Comm = "mkdir -p " + folder;
  system( Comm.Data() );
  Comm = "rm -rf " + folder + "/*";
  system( Comm.Data() );
  // Variables
  TString Min_Muon_PtReco           = "(Muon1_pt*(Muon1_pt<Muon2_pt) + Muon2_pt*(Muon1_pt>Muon2_pt))";
  TString Max_Muon_PtReco           = "(Muon1_pt*(Muon1_pt>Muon2_pt) + Muon2_pt*(Muon1_pt<Muon2_pt))";
  TString DR_l1b1                   = "(dR_b1l1*(Muon1_pt>Muon2_pt && b1jet_pt>b2jet_pt) + dR_b1l2*(Muon2_pt>Muon1_pt && b1jet_pt>b2jet_pt) + dR_b2l1*(Muon1_pt>Muon2_pt && b2jet_pt>b1jet_pt) + dR_b2l2*(Muon2_pt>Muon1_pt && b2jet_pt>b1jet_pt))";
  // Selections
  TString Preselection              = "hasb1jet && hasb2jet && hasMET && hasdRljet && hastwomuons";
  TString Met                       = "hasMET && hasGenMET";
  TString BjetAndMu                 = "hasb1jet && hasb2jet && hastwomuons";
  TString baselinecut               = "sqrt((mu1_eta-mu2_eta)**2+TVector2::Phi_mpi_pi(mu1_phi-mu2_phi)**2)<2.5";
  TString htoWW_mass                = "sqrt((mu1_energy+mu2_energy+nu1_energy+nu2_energy)**2-(mu1_px+mu2_px+nu1_px+nu2_px)**2-(mu1_py+mu2_py+nu1_py+nu2_py)**2-(mu1_pz+mu2_pz+nu1_pz+nu2_pz)**2)";
  TString hmass_bins                = "(50,100,300)";
  TString hmass_bins1               = "(100,80,150)";
  TString hmass_bins2               = "(50,0,200)";
  TString htoWW_cut                 = "h2tohh";
  TString hasjets_cut               = "hasbjet && hasbbarjet";
  TString bjet_pt                   = "sqrt(bjet_px**2+bjet_py**2)";
  TString bbarjet_pt                = "sqrt(bbarjet_px**2+bbarjet_py**2)";
  TString htoBB_mass                = "sqrt((b1_energy+b2_energy)**2-(b1_px+b2_px)**2-(b1_py+b2_py)**2-(b1_pz+b2_pz)**2)";
  TString htoBBJets_mass            = "sqrt((bjet_energy+bbarjet_energy)**2-(bjet_px+bbarjet_px)**2-(bjet_py+bbarjet_py)**2-(bjet_pz+bbarjet_pz)**2)";
  TString htoBBJets_correction_mass = "sqrt(((bjet_energy+bbarjet_energy)*rescalefactor)**2-((bjet_px+bbarjet_px)*rescalefactor)**2-((bjet_py+bbarjet_py)*rescalefactor)**2-((bjet_pz+bbarjet_pz)*rescalefactor)**2)";
  TString htoBBJetstot_mass         = "sqrt((bjet_energy_tot+bbarjet_energy_tot)**2-(bjet_px_tot+bbarjet_px_tot)**2-(bjet_py_tot+bbarjet_py_tot)**2-(bjet_pz_tot+bbarjet_pz_tot)**2)";
  TString htoBB_cut                 = "h2tohh";
  // Specify whay do you want to plot, and after what selection
  map< TString,vector< vector<TString> > > ListPlots; ListPlots.clear(); // map[Variable_name] = vector[ vector[selection1,TagName1,...], vector[selection2,TagName2,...], ...] -> will plot Variable_name after all the selections specified
  vector<vector<TString> > selectionsTags; selectionsTags.clear();
  vector<TString> selectionTag; selectionTag.clear();
  // Here you can add plots
  // Order: n_Sel = 0, n_Tag = 1, n_nBin = 2, n_nBinMin = 3, n_nBinMax = 4, n_Norm1 = 5, n_Topo = 6, n_gSty = 7
  // Mi-Max Pt
  selectionTag = InitVec(Min_Muon_PtReco + ">0","Muon_PtReco_Min","50.","0.","100.","true","S_and_B","0"); selectionsTags.push_back(selectionTag);
  ListPlots[Min_Muon_PtReco] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
  selectionTag = InitVec(Max_Muon_PtReco + ">0","Muon_PtReco_Max","50.","0.","100.","true","S_and_B","0"); selectionsTags.push_back(selectionTag);
  ListPlots[Max_Muon_PtReco] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  // Met Reso
//  selectionTag = InitVec( "genmet_px>-900 && met_px>-900", "MetX_Reso", "100.", "-60.", "60.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["genmet_px-met_px"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "genmet_py>-900 && met_py>-900", "MetY_Reso", "100.", "-60.", "60.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["genmet_py-met_py"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "genmet_diBaxis_p>-900 && met_diBaxis_p>-900", "MetDiBp_Reso", "100.", "-60.", "60.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["genmet_diBaxis_p-met_diBaxis_p"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "genmet_diBaxis_t>-900 && met_diBaxis_t>-900", "MetDiBt_Reso", "100.", "-60.", "60.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["genmet_diBaxis_t-met_diBaxis_t"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//    //compared to v form W
//  selectionTag = InitVec( "nu1and2_px>-900 && met_px>-900", "MetX_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_px-met_px"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "nu1and2_py>-900 && met_py>-900", "MetY_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_py-met_py"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "nu1and2_diBaxis_p>-900 && met_diBaxis_p>-900", "MetDiBp_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_diBaxis_p-met_diBaxis_p"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "nu1and2_diBaxis_t>-900 && met_diBaxis_t>-900", "MetDiBt_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_diBaxis_t-met_diBaxis_t"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//    //compared to v form W (RECO met with rescale)
//  selectionTag = InitVec( "nu1and2_px>-900 && met_c1_px>-900", "MetX_c1_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_px-met_c1_px"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "nu1and2_py>-900 && met_c1_py>-900", "MetY_c1_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_py-met_c1_py"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "nu1and2_diBaxis_p>-900 && met_diBaxis_c1_p>-900", "MetDiBp_c1_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_diBaxis_p-met_diBaxis_c1_p"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
//  selectionTag = InitVec( "nu1and2_diBaxis_t>-900 && met_diBaxis_c1_t>-900", "MetDiBt_c1_Reso_nuFromW", "100.", "-150.", "150.", "true", "Only_S", "111111" ); selectionsTags.push_back(selectionTag);
//  ListPlots["nu1and2_diBaxis_t-met_diBaxis_c1_t"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
  //DR(l1,b1)
  selectionTag = InitVec( Preselection, "DR_MuB", "100.", "0.", "5.", "true", "S_and_B", "0" ); selectionsTags.push_back(selectionTag);
  ListPlots["(dR_bl)"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();
  //MT2
  selectionTag = InitVec( Preselection, "MT2", "100.", "0.", "400.", "true", "S_and_B", "0" ); selectionsTags.push_back(selectionTag);
  ListPlots["MT2"] = selectionsTags; selectionsTags.clear(); selectionTag.clear();

  // Loop over the Map
  typedef map< TString,vector< vector<TString> > >::iterator it_map;
  for(it_map iterator = ListPlots.begin(); iterator != ListPlots.end(); iterator++) {
    // Loop over the number of selections for the given Variable_name
    for( unsigned int nS = 0; nS<(iterator->second).size(); nS++ ){
	int topology = S_and_B;
	if((iterator->second)[nS][n_Topo] == "Only_S")  topology=Only_S;
	if((iterator->second)[nS][n_Topo] == "Only_B")  topology=Only_B;
	Make2Plot( TS, TB, iterator->first, (iterator->second)[nS][n_Sel], (iterator->second)[nS][n_Tag] , folder, atof((iterator->second)[nS][n_nBin]), atof((iterator->second)[nS][n_nBinMin]), atof((iterator->second)[nS][n_nBinMax]), (iterator->second)[nS][n_Norm1]=="true", topology, atoi((iterator->second)[nS][n_gSty]) );
    }
  }
}

void Make2Plot(TTree * TS, TTree * TB, TString Var, TString Sel, TString Tag, TString Dir, float nBin, float binMin, float binMax, bool Norm1, int Topology, int gSty){

  TString name = Dir + "/" + Tag;
  cout<<"Plotting: "<<name<<".png"<<endl;
  TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
  gStyle->SetOptStat(gSty);
  TH1F *hS=NULL, *hB=NULL;
  if( Topology == S_and_B || Topology == Only_S ){
    hS = new TH1F("hS", "", nBin, binMin, binMax);
    TS->Draw(Var+">>hS", Sel);
  }
  if( Topology == S_and_B || Topology == Only_B ){
    hB = new TH1F("hB", "", nBin, binMin, binMax);
    TB->Draw(Var+">>hB", Sel);
  }
  TLegend *leg = new TLegend(0.7,0.6,0.89,0.69);
  leg->SetFillColor(0); leg->SetLineColor(0);
  if( Topology == S_and_B ){
    if(Norm1){ hS->Scale(1./hS->Integral()); hB->Scale(1./hB->Integral()); }
    hS->SetLineColor(kRed);
    int order = GetHigherH( hS, hB );
    if(order==1){ hS->Draw(); hB->Draw("same"); }
    if(order==2){ hB->Draw(); hS->Draw("same"); }
    leg->AddEntry(hS,"Hhh","l");
    leg->AddEntry(hB,"tt","l");
    leg->Draw("same");
  }
  if( Topology == Only_S ){
    if(Norm1){ hS->Scale(1./hS->Integral()); }
    hS->Draw();
    leg->AddEntry(hS,"Hhh","l"); leg->Draw("same");
  }
  if( Topology == Only_B ){
    if(Norm1){ hB->Scale(1./hB->Integral()); }
    hB->Draw();
    leg->AddEntry(hB,"tt","l"); leg->Draw("same");
  }
  myc1->SaveAs( name + ".png" );
  delete myc1;
  delete leg;
  if( Topology == S_and_B || Topology == Only_S ) delete hS;
  if( Topology == S_and_B || Topology == Only_B ) delete hB;
}

float GetMax(TH1F *h){
  int binmax = h->GetMaximumBin();
  return h->GetBinContent(binmax);
}

int GetHigherH(TH1F *h1, TH1F *h2){
  float height1 = GetMax(h1);
  float height2 = GetMax(h2);
  if( height1 >= height2 ) return 1;
  else                     return 2;
}

vector<TString> InitVec( TString Var, TString Sel, TString nBin, TString nBinMin, TString nBinMax, TString Norm1, TString Topo, TString gSty ){
  vector<TString> selectionTag; selectionTag.clear();
  selectionTag.push_back(Var); selectionTag.push_back(Sel); selectionTag.push_back(nBin); selectionTag.push_back(nBinMin); selectionTag.push_back(nBinMax); selectionTag.push_back(Norm1); selectionTag.push_back(Topo); selectionTag.push_back(gSty);
  return selectionTag;
}

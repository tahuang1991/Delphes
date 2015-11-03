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

void GridOpt_OptFromTXT( TString folder = "OUT_GridOptALLCUT/", TString txt_name = "S_B.txt" ){

  TCanvas* myc1 = new TCanvas("myc1", "CMS", 600, 600);
  //Opening Inputs
  cout<<"Hello, I'm opening: "<<folder<<txt_name<<endl;

  //Now plot S/sqrt(B) vs cut number
  std::map<float,TString> VLines; VLines.clear();
  std::map<float, float> mapID_SB, mapID_SsqrtB, mapID_SpB, mapID_SsqrtSpB, mapID_S, mapID_B;
  std::ifstream infile((folder + txt_name).Data());
  std::string line1, line;
  float MaxSb = -1., MaxSb_SsqB = -1., Max_SpB = -1., Max_SqrtSpB = -1.;
  int MaxSbIndex_SB = -1, MaxSbIndex_SqSB = -1, MaxSbIndex_SpB = -1, MaxSbIndex_sqSpB = -1;
  int binN = 0, nTotLine = 0;
  float NEntries_S = -1., NormCro_S = -1., NEntries_B = -1., NormCro_B = -1.;
  //Count N lines
  std::ifstream infileN((folder + txt_name).Data());
  std::string Nline;
  while (std::getline(infileN, Nline))
         ++nTotLine;
  //Get NEntries_S and NormCro_S (and the same for B)
  while ( std::getline(infile, line1) ){
    //nTotLine++;
    if( binN==0 ){
      std::istringstream iss1(line1);
      //NEntries_S 8601.000000 NormCro_S 0.085082 NEntries_B 48944.000000 NormCro_B 30.608406
      TString s_NEntries_S, s_NEntries_Sn, s_NormCro_S, s_NormCro_Sn, s_NEntries_B, s_NEntries_Bn, s_NormCro_B, s_NormCro_Bn;
      if (!(iss1 >> s_NEntries_S >> s_NEntries_Sn >> s_NormCro_S >> s_NormCro_Sn >> s_NEntries_B >> s_NEntries_Bn  >> s_NormCro_B >> s_NormCro_Bn )) { break; } // error
      NEntries_S     = ::atof( s_NEntries_Sn);
      NormCro_S      = ::atof( s_NormCro_Sn);
      NEntries_B     = ::atof( s_NEntries_Bn);
      NormCro_B      = ::atof( s_NormCro_Bn);
    }
  }
  int nBin = nTotLine-2;
  TH1F *h_SB     = new TH1F("h_SB","", nBin, -0.5, float(nBin)-0.5);
  TH1F *h_SsqrtB = new TH1F("h_SsqrtB","", nBin, -0.5, float(nBin)-0.5);
  TH1F *h_SpB    = new TH1F("h_SpB","", nBin, -0.5, float(nBin)-0.5);
  TH1F *h_sqSpB  = new TH1F("h_sqSpB","", nBin, -0.5, float(nBin)-0.5);
  while (std::getline(infile, line))
  {
    if(binN==0 || binN==1){ binN++; continue; }
    std::istringstream iss(line);
    //BIN 18292 cut_dR_l1l2 1.40 cut_dR_b1b2 2.40 cut_dR_b1b2_min 0.00 cut_mass_l1l2 60.00 cut_mass_b1b2 165.00 cut_mass_b1b2_min 90.00 cut_dR_bl 2.25 Signal 248.524750 Background 21706.275391 S_B 0.011449 S_SqrtB 1.686852 S_SpB 0.011320 S_SqrtSB 1.677277 
    TString SBIN, SBIN_n, Scut_dR_l1l2, Scut_dR_l1l2_n, Scut_dR_b1b2, Scut_dR_b1b2_n, Scut_dR_b1b2_min, Scut_dR_b1b2_min_n, Scut_mass_l1l2, Scut_mass_l1l2_n, Scut_mass_b1b2, Scut_mass_b1b2_n, Scut_mass_b1b2_min, Scut_mass_b1b2_min_n, Scut_dR_bl, Scut_dR_bl_n, SS, SS_n, BB, BB_n, S_SqrtB, S_SqrtB_n, S_B, S_B_n, S_SpB, S_SpB_n, S_SqrtSB, S_SqrtSB_n;
    if (!(iss >> SBIN >> SBIN_n >> Scut_dR_l1l2 >> Scut_dR_l1l2_n >> Scut_dR_b1b2 >> Scut_dR_b1b2_n >> Scut_dR_b1b2_min >> Scut_dR_b1b2_min_n >> Scut_mass_l1l2 >> Scut_mass_l1l2_n >> Scut_mass_b1b2 >> Scut_mass_b1b2_n >> Scut_mass_b1b2_min >> Scut_mass_b1b2_min_n >> Scut_dR_bl >> Scut_dR_bl_n >> SS >> SS_n >> BB >> BB_n >> S_B >> S_B_n >> S_SqrtB >> S_SqrtB_n >> S_SpB >> S_SpB_n >> S_SqrtSB >> S_SqrtSB_n)) { break; } // error
    float f_ind     = ::atof(SBIN_n);
    float f_SB      = ::atof(S_B_n);
    float f_SsqrtB  = ::atof(S_SqrtB_n);
    float f_SpB     = ::atof(S_SpB_n);
    float f_SqrtSB  = ::atof(S_SqrtSB_n);
    float f_S       = ::atof(SS_n);
    float f_B       = ::atof(BB_n);
    vector<float> SB; SB.clear();             SB.push_back(f_ind); SB.push_back(f_SB);
    vector<float> SsqrtB; SsqrtB.clear();     SsqrtB.push_back(f_ind); SsqrtB.push_back(f_SsqrtB);
    vector<float> SpB; SpB.clear();           SpB.push_back(f_ind); SpB.push_back(f_SpB);
    vector<float> SsqrtSpB; SsqrtSpB.clear(); SsqrtSpB.push_back(f_ind); SsqrtSpB.push_back(f_SqrtSB);
    mapID_SB[f_ind] = f_SB; mapID_SsqrtB[f_ind] = f_SsqrtB; mapID_SpB[f_ind] = f_SpB; mapID_SsqrtSpB[f_ind] = f_SqrtSB;
    mapID_S[f_ind] = f_S; mapID_B[f_ind] = f_B;
    VLines[f_ind]   = line; 
    h_SB->SetBinContent( binN, f_SB);
    h_SsqrtB->SetBinContent( binN, f_SsqrtB);
    h_SpB->SetBinContent( binN, f_SpB);
    h_sqSpB->SetBinContent( binN, f_SqrtSB);
    if( f_SB > MaxSb ){
      MaxSbIndex_SB = f_ind;
      MaxSb = f_SB;
    }
    if( f_SsqrtB > MaxSb_SsqB ){
      MaxSbIndex_SqSB = f_ind;
      MaxSb_SsqB = f_SsqrtB;
    }
    if( f_SpB > Max_SpB ){
      MaxSbIndex_SpB = f_ind;
      Max_SpB = f_SpB;
    }
    if( f_SqrtSB > Max_SqrtSpB ){
      MaxSbIndex_sqSpB = f_ind;
      Max_SqrtSpB = f_SqrtSB;
    }
    binN++;
  }
  cout<<endl;
  gStyle->SetOptStat(0); myc1->cd();
  cout<<"Max S/B is "<<mapID_SB[MaxSbIndex_SB]<<" corresponding to index: "<<MaxSbIndex_SB<<endl;
  cout<<"This correspond to:---> "<<VLines[MaxSbIndex_SB]<<endl;
  cout<<"It has an efficienciy for S: "<<mapID_S[MaxSbIndex_SB]/(NEntries_S * NormCro_S)<<" and for B: "<<mapID_B[MaxSbIndex_SB]/(NEntries_B * NormCro_B)<<endl;
  h_SB->GetXaxis()->SetTitle("Cut Bin"); h_SB->GetYaxis()->SetTitle("S/B");
  h_SB->Draw("");
  myc1->SaveAs( (folder + "/SB.png") );
  cout<<endl;
  cout<<"Max S/sqrt(B) is "<<mapID_SsqrtB[MaxSbIndex_SqSB]<<" corresponding to index: "<<MaxSbIndex_SqSB<<endl;
  cout<<"This correspond to:---> "<<VLines[MaxSbIndex_SqSB]<<endl;
  cout<<"It has an efficienciy for S: "<<mapID_S[MaxSbIndex_SqSB]/(NEntries_S * NormCro_S)<<" and for B: "<<mapID_B[MaxSbIndex_SqSB]/(NEntries_B * NormCro_B)<<endl;
  h_SsqrtB->GetXaxis()->SetTitle("Cut Bin"); h_SsqrtB->GetYaxis()->SetTitle("S/sqrt(B)");
  h_SsqrtB->Draw("");
  myc1->SaveAs( (folder + "/SsqrtB.png") );
  cout<<endl;
  cout<<"Max S/(S+B) is "<<mapID_SpB[MaxSbIndex_SpB]<<" corresponding to index: "<<MaxSbIndex_SpB<<endl;
  cout<<"This correspond to:---> "<<VLines[MaxSbIndex_SpB]<<endl;
  cout<<"It has an efficienciy for S: "<<mapID_S[MaxSbIndex_SpB]/(NEntries_S * NormCro_S)<<" and for B: "<<mapID_B[MaxSbIndex_SpB]/(NEntries_B * NormCro_B)<<endl;
  h_SpB->GetXaxis()->SetTitle("Cut Bin"); h_SpB->GetYaxis()->SetTitle("S/(S+B)");
  h_SpB->Draw("");
  myc1->SaveAs( (folder + "/SSpB.png") );
  cout<<endl;
  cout<<"Max S/sqrt(S+B) is "<<mapID_SsqrtSpB[MaxSbIndex_sqSpB]<<" corresponding to index: "<<MaxSbIndex_sqSpB<<endl;
  cout<<"This correspond to:---> "<<VLines[MaxSbIndex_sqSpB]<<endl;
  cout<<"It has an efficienciy for S: "<<mapID_S[MaxSbIndex_sqSpB]/(NEntries_S * NormCro_S)<<" and for B: "<<mapID_B[MaxSbIndex_sqSpB]/(NEntries_B * NormCro_B)<<endl;
  h_sqSpB->GetXaxis()->SetTitle("Cut Bin"); h_sqSpB->GetYaxis()->SetTitle("S/sqrt(S+B)");
  h_sqSpB->Draw("");
  myc1->SaveAs( (folder + "/SsqrtSpB.png") );
}

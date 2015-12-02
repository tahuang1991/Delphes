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
#include "TMVA.h"
#include "TMVA_Factory.h"
#include "TMVA_reader.h"
#include <iostream>
#include <sstream>  
#include <fstream>

using TMVApp::TMVA_Reader;
using namespace std;

void TrainTheSamples( TString folder = "MVA_result/", TString Sig_name = "../delphes_B3_1M_PU0_Btag.root", TString Bac_name = "../delphes_ttbar_1M_PU0.root" ){

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
  //Input variables
  vector<TString>* inputVars = new vector<TString>;
  inputVars->push_back("dR_l1l2");
  inputVars->push_back("dR_lb");
  inputVars->push_back("dR_b1b2");
  inputVars->push_back("mass_l1l2");
  inputVars->push_back("mass_b1b2");
  //OutPut
  TFile* target = TFile::Open( "TMVA.root", "RECREATE" );
  TMVA_Factory *factory = new TMVA_Factory( "TMVAnalysis", target, "" );
  factory->SetInputTrees("evtree", "dR_l1l2>0", "dR_l1l2>0"); //(TTree* inputTree, TCut SigCut, TCut BgCut = "")
  factory->SetInputVariables( inputVars );
  factory->BookMethod("MethodFisher", "Fisher");//http://tmva.sourceforge.net/talks/voss19Jun06.pdf -> slide 37
  //Performance evaluation
  factory->EvaluateAllVariables(); // for each input variable used in MVAs
  factory->EvaluateAllMethods(); // for all MVAs
  //Close output file and cleanup
  target->Close();
  delete factory;
}

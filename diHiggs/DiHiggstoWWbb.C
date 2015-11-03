#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>

#include "TTree.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"
#include "TRefArray.h"
#include "TObject.h"
#include "TString.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TLorentzVector.h"
#include "classes/DelphesClasses.h"
#include "DiHiggstoWWbb.h"
#include "MMC.h"

using namespace std;

class ExRootTreeReader;
class ExRootResult;
class GenParticle;
class Jet;
class Track;
class Tower;

DiHiggstoWWbb::DiHiggstoWWbb(TString input_File, TString output_File, std::ifstream& config_File)
{
  inputFile = input_File;
  outputFile = output_File;
  readConfig(config_File); // Read parametersconfig.txt
  init();                  // Create branches
  branchParticle      = treeReader->UseBranch("Particle");
  branchMuon          = treeReader->UseBranch("Muon");
  branchMuonBeforeIso = treeReader->UseBranch("MuonBeforeIso");
  //TClonesArray *branchTrack = treeReader->UseBranch("Track");
  //TClonesArray *branchTower = treeReader->UseBranch("Tower");
  //TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  //TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  //TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  branchJet           = treeReader->UseBranch("Jet");
  branchGenJet        = treeReader->UseBranch("GenJet");
  branchMissingET     = treeReader->UseBranch("MissingET");
  branchGenMissingET  = treeReader->UseBranch("GenMissingET");
  allEntries = treeReader->GetEntries();
  cout << "** Chain contains " << allEntries << " events" << endl;
  cout <<" DiHiggstoWWbb gFile get options " << gFile->GetOption() << endl;
}

void DiHiggstoWWbb::readConfig(std::ifstream& ifile){
  std::vector<std::string> strs;
  strs.clear();
  std::string word;
  while (ifile >> word){
    strs.push_back(word);
  }
  if (!ifile.eof()) {
    std::cerr << "Hey there! Something went wrong when reading the list of words!\n";
    return;
  }
  getintpara(strs, "nEvents", nEvents_, -1);
  getintpara(strs, "nStarts", nStarts_, 0);
  getintpara(strs, "sample", sample_, Signal);
  getboolpara(strs, "simulation", simulation_, true);
  getdoublepara(strs,"jetsPt", jetsPt_, 20.0);
  getdoublepara(strs,"jetsEta", jetsEta_, 5.0);
  getdoublepara(strs,"bjetsPt", bjetsPt_, 30.0);
  getdoublepara(strs,"bjetsEta", bjetsEta_, 2.50);
  getdoublepara(strs,"jetleptonDeltaR", jetleptonDeltaR_, .30); //isolation bewteen jet and lepton
  getdoublepara(strs,"jetsDeltaR", jetsDeltaR_, .40);           //deltaR matching
  getdoublepara(strs,"leptonsDeltaR", leptonsDeltaR_, .40);
  getdoublepara(strs,"leptonIso", leptonIso_, 0.15);
  getdoublepara(strs,"muonPt1", muonPt1_, 20.0);
  getdoublepara(strs,"muonPt2", muonPt2_, 20.0);
  getdoublepara(strs,"muonEta", muonsEta_, 2.40);
  getdoublepara(strs,"metPt", metPt_, 20);
  getboolpara(strs, "runMMC", runMMC_, false);
  getboolpara(strs, "useRecoMET", useRecoMET_, false);//whether to use reco met or not as MMC input
  getboolpara(strs, "useRecoMuon", useRecoMuon_, false);
  getboolpara(strs, "useRecoBJet", useRecoBJet_, false);
  getintpara(strs, "iterations", iterations_, 1000000);
  getstringpara(strs,"RefPDFfile", RefPDFfile_, "MMCRefPDF.ROOT");
  getintpara(strs, "bjetrescaleAlgo", bjetrescaleAlgo_, 2);
  getboolpara(strs, "useMET", useMET_, true);
  getboolpara(strs, "weightfromonshellnupt_func", weightfromonshellnupt_func_, false);
  getboolpara(strs, "weightfromonshellnupt_hist", weightfromonshellnupt_hist_, true);
  getboolpara(strs, "weightfromonoffshellWmass_hist", weightfromonoffshellWmass_hist_, true);
  std::cout <<" jetspt "<< jetsPt_ <<" jetsEta "<< jetsEta_ <<" bjetspt " << bjetsPt_ <<" bjetsEta " << bjetsEta_ << std::endl; 
}

void DiHiggstoWWbb::getboolpara(std::vector<std::string>& strs, std::string paraname, bool &para, bool def){
  std::vector<std::string>::iterator it = strs.end();
  it=std::find(strs.begin(), strs.end(), paraname);
  if (it != strs.end()){
    it++;
    if ((*it) == "True" or (*it)== "true") para =true;
    else para =false;
  }
  else para = def;
}

void DiHiggstoWWbb::getstringpara(std::vector<std::string>& strs,std::string paraname, std::string &para, std::string def)
{
  std::vector<std::string>::iterator it = strs.end();
  it=std::find(strs.begin(), strs.end(), paraname);
  if (it != strs.end()){
    it++;
    para = *it;
  }
  else para = def;
}

void DiHiggstoWWbb::getintpara(std::vector<std::string>& strs, std::string paraname, int &para, int def)
{
  std::vector<std::string>::iterator it = strs.end();
  it=std::find(strs.begin(), strs.end(), paraname);
  if (it != strs.end()){
    it++;
    para = atoi(it->c_str());
  }
  else para = def;
}

void DiHiggstoWWbb::getdoublepara(std::vector<std::string>& strs, std::string paraname, double &para, double def)
{
  std::vector<std::string>::iterator it = strs.end();
  it=std::find(strs.begin(), strs.end(), paraname);
  if (it != strs.end()){
    it++;
    para = atof(it->c_str());
  }
  else para = def;

}

void DiHiggstoWWbb::calculateNormfactor(int bm){
  // Obtained from pdg
  float br_W2mu      = 0.1063;
  float br_W2e       = 0.1071;
  float br_W2tau     = 0.1138;
  float br_tt2bbll   = 0.105;
  float br_h2bb      = .577;
  float br_h2WW      = .215;
  //H->hh benchMark. keep updating 
  float mass_H[14]   = {262.5, 337, 365, 406, 489, 515, 555, 612, 677, 726, 777, 812, 868, 915};
  float cs_H[14]     = {.636, 1.27, 1.15, 0.94, 0.28, .26, .25, .125, .09, .04, .01, .02, .01, .007};//pb
  float br_H2hh[14]  = {.36, .71, .685, .12, .05, .12, .07, .13, .1, .12, .11, .14, .11, .11};
  //ttbar production. keep updating ttbar cross section at 14 TeV
  float cs_tt        = 953;
  float cs_H2hh      = cs_H[bm-1]*br_H2hh[bm-1];
  float cs_H2bbmumu  = cs_H2hh*2*br_h2WW*br_h2bb*br_W2mu*br_W2mu;
  float br_tt2bbmumu = br_tt2bbll*br_W2mu*br_W2mu/(br_W2mu+br_W2e+br_W2tau)/(br_W2mu+br_W2e+br_W2tau);
  float cs_tt2bbmumu = cs_tt*br_tt2bbmumu;
  cout <<" normalize signal and background(tt), cross section(cs) in pb " <<"  H->hh benchmark "<< bm <<" M_H " << mass_H[bm-1] << endl;
  cout <<" cs:H production "<< cs_H[bm-1] <<" br:H->hh "<< br_H2hh[bm-1] <<" cs:H->mumubb "<< cs_H2bbmumu << endl;
  cout <<" cs:ttbar production "<< cs_tt <<" cs:tt->mumubb "<< cs_tt2bbmumu << endl;
  float L = 3000000;
  cout <<"Integrate L(pb^{-1}) "<< L <<" total H->bbmumu "<<L*cs_H2bbmumu <<" total tt->bbmumu "<< L*cs_tt2bbmumu << endl;
}

void DiHiggstoWWbb::init(){ 
  chain = new TChain("Delphes");
  chain->Add(inputFile);

  treeReader = new ExRootTreeReader(chain);
  evtree = new TTree("evtree","event tree");
  evtree->Branch("b1_px",&b1_px, "b1_px/F");
  evtree->Branch("b1_py",&b1_py, "b1_py/F");
  evtree->Branch("b1_pz",&b1_pz, "b1_pz/F");
  evtree->Branch("b1_eta",&b1_eta, "b1_eta/F");
  evtree->Branch("b1_phi",&b1_phi, "b1_phi/F");
  evtree->Branch("b1_pt",&b1_pt, "b1_pt/F");
  evtree->Branch("b1_energy",&b1_energy, "b1_energy/F");
  evtree->Branch("b2_px",&b2_px, "b2_px/F");
  evtree->Branch("b2_py",&b2_py, "b2_py/F");
  evtree->Branch("b2_pz",&b2_pz, "b2_pz/F");
  evtree->Branch("b2_eta",&b2_eta, "b2_eta/F");
  evtree->Branch("b2_phi",&b2_phi, "b2_phi/F");
  evtree->Branch("b2_pt",&b2_pt, "b2_pt/F");
  evtree->Branch("b2_energy",&b2_energy, "b2_energy/F");
  evtree->Branch("htobb_px",&htobb_px, "htobb_px/F");
  evtree->Branch("htobb_py",&htobb_py, "htobb_py/F");
  evtree->Branch("htobb_pz",&htobb_pz, "htobb_pz/F");
  evtree->Branch("htobb_energy",&htobb_energy, "htobb_energy/F");
  evtree->Branch("htobb_mass",&htobb_mass, "htobb_mass/F");
  evtree->Branch("htobb",&htobb, "htobb/B");

  evtree->Branch("genb1jet_px",&genb1jet_px, "genb1jet_px/F");
  evtree->Branch("genb1jet_py",&genb1jet_py, "genb1jet_py/F");
  evtree->Branch("genb1jet_pz",&genb1jet_pz, "genb1jet_pz/F");
  evtree->Branch("genb1jet_eta",&genb1jet_eta, "genb1jet_eta/F");
  evtree->Branch("genb1jet_phi",&genb1jet_phi, "genb1jet_phi/F");
  evtree->Branch("genb1jet_pt",&genb1jet_pt, "genb1jet_pt/F");
  evtree->Branch("genb1jet_energy",&genb1jet_energy, "genb1jet_energy/F");
  evtree->Branch("genb2jet_px",&genb2jet_px, "genb2jet_px/F");
  evtree->Branch("genb2jet_py",&genb2jet_py, "genb2jet_py/F");
  evtree->Branch("genb2jet_pz",&genb2jet_pz, "genb2jet_pz/F");
  evtree->Branch("genb2jet_eta",&genb2jet_eta, "genb2jet_eta/F");
  evtree->Branch("genb2jet_phi",&genb2jet_phi, "genb2jet_phi/F");
  evtree->Branch("genb2jet_pt",&genb2jet_pt, "genb2jet_pt/F");
  evtree->Branch("genb2jet_energy",&genb2jet_energy, "genb2jet_energy/F");
  evtree->Branch("dR_genb1jet", &dR_genb1jet,"dR_genb1jet/F");  
  evtree->Branch("dR_genb2jet", &dR_genb2jet,"dR_genb2jet/F");  
  evtree->Branch("hasgenb1jet",&hasgenb1jet, "hasgenb1jet/B");
  evtree->Branch("hasgenb2jet",&hasgenb2jet, "hasgenb2jet/B");

  evtree->Branch("b1jet_px",&b1jet_px, "b1jet_px/F");
  evtree->Branch("b1jet_py",&b1jet_py, "b1jet_py/F");
  evtree->Branch("b1jet_pz",&b1jet_pz, "b1jet_pz/F");
  evtree->Branch("b1jet_eta",&b1jet_eta, "b1jet_eta/F");
  evtree->Branch("b1jet_phi",&b1jet_phi, "b1jet_phi/F");
  evtree->Branch("b1jet_pt",&b1jet_pt, "b1jet_pt/F");
  evtree->Branch("b1jet_energy",&b1jet_energy, "b1jet_energy/F");
  evtree->Branch("b1jet_btag",&b1jet_btag, "b1jet_btag/i");//unsigned int
  evtree->Branch("b2jet_px",&b2jet_px, "b2jet_px/F");
  evtree->Branch("b2jet_py",&b2jet_py, "b2jet_py/F");
  evtree->Branch("b2jet_pz",&b2jet_pz, "b2jet_pz/F");
  evtree->Branch("b2jet_eta",&b2jet_eta, "b2jet_eta/F");
  evtree->Branch("b2jet_phi",&b2jet_phi, "b2jet_phi/F");
  evtree->Branch("b2jet_pt",&b2jet_pt, "b2jet_pt/F");
  evtree->Branch("b2jet_energy",&b2jet_energy, "b2jet_energy/F");
  evtree->Branch("b2jet_btag",&b2jet_btag, "b2jet_btag/i");
  evtree->Branch("energeticbjets",&energeticbjets, "energeticbjets/I");
  evtree->Branch("dR_b1jet", &dR_b1jet,"dR_b1jet/F");  
  evtree->Branch("dR_b2jet", &dR_b2jet,"dR_b2jet/F");  

  evtree->Branch("hasb1jet",&hasb1jet, "hasb1jet/B");
  evtree->Branch("hasb2jet",&hasb2jet, "hasb2jet/B");
  evtree->Branch("hastwogenmuons",&hastwogenmuons, "hastwogenmuons/B");
  evtree->Branch("hastwomuons",&hastwomuons, "hastwomuons/B");

  evtree->Branch("mu1_px",&mu1_px, "mu1_px/F");
  evtree->Branch("mu1_py",&mu1_py, "mu1_py/F");
  evtree->Branch("mu1_pz",&mu1_pz, "mu1_pz/F");
  evtree->Branch("mu1_eta",&mu1_eta, "mu1_eta/F");
  evtree->Branch("mu1_phi",&mu1_phi, "mu1_phi/F");
  evtree->Branch("mu1_pt",&mu1_pt, "mu1_pt/F");
  evtree->Branch("mu1_energy",&mu1_energy, "mu1_energy/F");
  evtree->Branch("nu1_px",&nu1_px, "nu1_px/F");
  evtree->Branch("nu1_py",&nu1_py, "nu1_py/F");
  evtree->Branch("nu1_pz",&nu1_pz, "nu1_pz/F");
  evtree->Branch("nu1_eta",&nu1_eta, "nu1_eta/F");
  evtree->Branch("nu1_phi",&nu1_phi, "nu1_phi/F");
  evtree->Branch("nu1_pt",&nu1_pt, "nu1_pt/F");
  evtree->Branch("nu1_energy",&nu1_energy, "nu1_energy/F");
  evtree->Branch("mu2_px",&mu2_px, "mu2_px/F");
  evtree->Branch("mu2_py",&mu2_py, "mu2_py/F");
  evtree->Branch("mu2_pz",&mu2_pz, "mu2_pz/F");
  evtree->Branch("mu2_eta",&mu2_eta, "mu2_eta/F");
  evtree->Branch("mu2_phi",&mu2_phi, "mu2_phi/F");
  evtree->Branch("mu2_pt",&mu2_pt, "mu2_pt/F");
  evtree->Branch("mu2_energy",&mu2_energy, "mu2_energy/F");
  evtree->Branch("nu2_px",&nu2_px, "nu2_px/F");
  evtree->Branch("nu2_py",&nu2_py, "nu2_py/F");
  evtree->Branch("nu2_pz",&nu2_pz, "nu2_pz/F");
  evtree->Branch("nu2_eta",&nu2_eta, "nu2_eta/F");
  evtree->Branch("nu2_phi",&nu2_phi, "nu2_phi/F");
  evtree->Branch("nu2_pt",&nu2_pt, "nu2_pt/F");
  evtree->Branch("nu2_energy",&nu2_energy, "nu2_energy/F");

  evtree->Branch("w1_mass",&w1_mass, "w1_mass/F");
  evtree->Branch("w1_px",&w1_px, "w1_px/F");
  evtree->Branch("w1_py",&w1_py, "w1_py/F");
  evtree->Branch("w1_pz",&w1_pz, "w1_pz/F");
  evtree->Branch("w1_energy",&w1_energy, "w1_energy/F");
  evtree->Branch("w1_pt",&w1_pt, "w1_pt/F");
  evtree->Branch("w1_eta",&w1_eta, "w1_eta/F");
  evtree->Branch("w1_phi",&w1_phi, "w1_phi/F");
  evtree->Branch("w2_mass",&w2_mass, "w2_mass/F");
  evtree->Branch("w2_px",&w2_px, "w2_px/F");
  evtree->Branch("w2_py",&w2_py, "w2_py/F");
  evtree->Branch("w2_pz",&w2_pz, "w2_pz/F");
  evtree->Branch("w2_energy",&w2_energy, "w2_energy/F");
  evtree->Branch("w2_pt",&w2_pt, "w2_pt/F");
  evtree->Branch("w2_eta",&w2_eta, "w2_eta/F");
  evtree->Branch("w2_phi",&w2_phi, "w2_phi/F");

  evtree->Branch("htoWW_energy",&htoWW_energy);
  evtree->Branch("htoWW_px",&htoWW_px,"htoWW_px/F");
  evtree->Branch("htoWW_py",&htoWW_py,"htoWW_px/F");
  evtree->Branch("htoWW_pz",&htoWW_pz,"htoWW_pz/F");
  evtree->Branch("htoWW_mass",&htoWW_mass,"htoWW_mass/F");
  evtree->Branch("Wtomu1nu1",&Wtomu1nu1,"Wtomu1nu1/B");
  evtree->Branch("Wtomu2nu2",&Wtomu2nu2,"Wtomu2nu2/B");
  evtree->Branch("htoWW",&htoWW,"htoWW/B");


  evtree->Branch("t1_px",&t1_px,"t1_px/F");
  evtree->Branch("t1_py",&t1_py,"t1_py/F");
  evtree->Branch("t1_pz",&t1_pz,"t1_pz/F");
  evtree->Branch("t1_energy",&t1_energy,"t1_energy/F");
  evtree->Branch("t1_mass",&t1_mass,"t1_mass/F");
  evtree->Branch("t2_px",&t2_px,"t2_px/F");
  evtree->Branch("t2_py",&t2_py,"t2_py/F");
  evtree->Branch("t2_pz",&t2_pz,"t2_pz/F");
  evtree->Branch("t2_energy",&t2_energy,"t2_energy/F");
  evtree->Branch("t2_mass",&t2_mass,"t2_mass/F");
  evtree->Branch("ttoWb",&ttoWb,"ttoWb/B");
  evtree->Branch("tbartoWbbar",&tbartoWbbar,"tbartoWbbar/B");
  evtree->Branch("ttbar",&ttbar,"ttbar/B");
  /*
  evtree->Branch("Muon1_beforeIso_px",&Muon1_beforeIso_px, "Muon1_beforeIso_px/F");
  evtree->Branch("Muon1_beforeIso_py",&Muon1_beforeIso_py, "Muon1_beforeIso_py/F");
  evtree->Branch("Muon1_beforeIso_pz",&Muon1_beforeIso_pz, "Muon1_beforeIso_pz/F");
  evtree->Branch("Muon1_beforeIso_eta",&Muon1_beforeIso_eta, "Muon1_beforeIso_eta/F");
  evtree->Branch("Muon1_beforeIso_phi",&Muon1_beforeIso_phi, "Muon1_beforeIso_phi/F");
  evtree->Branch("Muon1_beforeIso_pt",&Muon1_beforeIso_pt, "Muon1_beforeIso_pt/F");
  evtree->Branch("Muon1_beforeIso_energy",&Muon1_beforeIso_energy, "Muon1_beforeIso_energy/F");
  evtree->Branch("Muon2_beforeIso_px",&Muon2_beforeIso_px, "Muon2_beforeIso_px/F");
  evtree->Branch("Muon2_beforeIso_py",&Muon2_beforeIso_py, "Muon2_beforeIso_py/F");
  evtree->Branch("Muon2_beforeIso_pz",&Muon2_beforeIso_pz, "Muon2_beforeIso_pz/F");
  evtree->Branch("Muon2_beforeIso_eta",&Muon2_beforeIso_eta, "Muon2_beforeIso_eta/F");
  evtree->Branch("Muon2_beforeIso_phi",&Muon2_beforeIso_phi, "Muon2_beforeIso_phi/F");
  evtree->Branch("Muon2_beforeIso_pt",&Muon2_beforeIso_pt, "Muon2_beforeIso_pt/F");
  evtree->Branch("Muon2_beforeIso_energy",&Muon2_beforeIso_energy, "Muon2_beforeIso_energy/F");
  evtree->Branch("dR_mu1_beforeIso",&dR_mu1_beforeIso, "dR_mu1_beforeIso/F");
  evtree->Branch("dR_mu2_beforeIso",&dR_mu2_beforeIso, "dR_mu2_beforeIso/F");
  evtree->Branch("hasMuon1_beforeIso",&hasMuon1_beforeIso, "hasMuon1_beforeIso/B");
  evtree->Branch("hasMuon2_beforeIso",&hasMuon2_beforeIso, "hasMuon2_beforeIso/B");
  evtree->Branch("Muon1_beforeIso_hasgenMu",&Muon1_beforeIso_hasgenMu, "Muon1_beforeIso_hasgenMu/B");
  evtree->Branch("Muon2_beforeIso_hasgenMu",&Muon2_beforeIso_hasgenMu, "Muon2_beforeIso_hasgenMu/B");
  evtree->Branch("Muon1_beforeIso_passIso",&Muon1_beforeIso_passIso, "Muon1_beforeIso_passIso/B");
  evtree->Branch("Muon2_beforeIso_passIso",&Muon2_beforeIso_passIso, "Muon2_beforeIso_passIso/B");
  */
  evtree->Branch("Muon1_px",&Muon1_px, "Muon1_px/F");
  evtree->Branch("Muon1_py",&Muon1_py, "Muon1_py/F");
  evtree->Branch("Muon1_pz",&Muon1_pz, "Muon1_pz/F");
  evtree->Branch("Muon1_eta",&Muon1_eta, "Muon1_eta/F");
  evtree->Branch("Muon1_phi",&Muon1_phi, "Muon1_phi/F");
  evtree->Branch("Muon1_pt",&Muon1_pt, "Muon1_pt/F");
  evtree->Branch("Muon1_energy",&Muon1_energy, "Muon1_energy/F");
  evtree->Branch("Muon2_px",&Muon2_px, "Muon2_px/F");
  evtree->Branch("Muon2_py",&Muon2_py, "Muon2_py/F");
  evtree->Branch("Muon2_pz",&Muon2_pz, "Muon2_pz/F");
  evtree->Branch("Muon2_eta",&Muon2_eta, "Muon2_eta/F");
  evtree->Branch("Muon2_phi",&Muon2_phi, "Muon2_phi/F");
  evtree->Branch("Muon2_pt",&Muon2_pt, "Muon2_pt/F");
  evtree->Branch("Muon2_energy",&Muon2_energy, "Muon2_energy/F");
  evtree->Branch("energeticMuon1",&energeticMuon1, "energeticMuon1/B");
  evtree->Branch("energeticMuon2",&energeticMuon2, "energeticMuon2/B");
  evtree->Branch("dR_mu1",&dR_mu1, "dR_mu1/F");
  evtree->Branch("dR_mu2",&dR_mu2, "dR_mu2/F");
  evtree->Branch("hasMuon1",&hasMuon1, "hasMuon1/B");
  evtree->Branch("hasMuon2",&hasMuon2, "hasMuon2/B");
  evtree->Branch("Muon1_hasgenMu",&Muon1_hasgenMu, "Muon1_hasgenMu/B");
  evtree->Branch("Muon2_hasgenMu",&Muon2_hasgenMu, "Muon2_hasgenMu/B");

  evtree->Branch("numbjets",&numbjets, "numbjets/I");
  evtree->Branch("numLeptons1",&numLeptons1, "numLeptons1/I");
  evtree->Branch("numLeptons2",&numLeptons2, "numLeptons2/I");
  evtree->Branch("dR_bl",&dR_bl, "dR_bl/F");
  evtree->Branch("dR_b1l1",&dR_b1l1, "dR_b1l1/F");
  evtree->Branch("dR_b1l2",&dR_b1l2, "dR_b1l2/F");
  evtree->Branch("dR_b2l1",&dR_b2l1, "dR_b2l1/F");
  evtree->Branch("dR_b2l2",&dR_b2l2, "dR_b2l2/F");
  evtree->Branch("dR_b1b2",&dR_b1b2, "dR_b1b2/F");
  evtree->Branch("dR_l1l2",&dR_l1l2, "dR_l1l2/F");
  evtree->Branch("dR_minbl",&dR_minbl, "dR_minbl/F");
  evtree->Branch("dR_genl1l2",&dR_genl1l2, "dR_genl1l2/F");
  evtree->Branch("mass_b1b2",&mass_b1b2, "mass_b1b2/F");
  evtree->Branch("energy_b1b2",&energy_b1b2, "energy_b1b2/F");
  evtree->Branch("pt_b1b2",&pt_b1b2, "pt_b1b2/F");
  evtree->Branch("phi_b1b2",&phi_b1b2, "phi_b1b2/F");
  evtree->Branch("eta_b1b2",&eta_b1b2, "eta_b1b2/F");
  evtree->Branch("mass_l1l2",&mass_l1l2, "mass_l1l2/F");
  evtree->Branch("energy_l1l2",&energy_l1l2, "energy_l1l2/F");
  evtree->Branch("pt_l1l2",&pt_l1l2, "pt_l1l2/F");
  evtree->Branch("phi_l1l2",&phi_l1l2, "phi_l1l2/F");
  evtree->Branch("eta_l1l2",&eta_l1l2, "eta_l1l2/F");
  evtree->Branch("dphi_llbb",&dphi_llbb, "dphi_llbb/F");
  evtree->Branch("dphi_llmet",&dphi_llmet, "dphi_llmet/F");
  evtree->Branch("mass_trans",&mass_trans, "mass_trans/F");

  evtree->Branch("genmet",&genmet,"genmet/F");
  evtree->Branch("genmet_phi",&genmet_phi,"genmet_phi/F");
  evtree->Branch("genmet_px",&genmet_px,"genmet_px/F");
  evtree->Branch("genmet_py",&genmet_py,"genmet_py/F");
  evtree->Branch("met",&met,"met/F");
  evtree->Branch("met_phi",&met_phi,"met_phi/F");
  evtree->Branch("met_px",&met_px,"met_px/F");
  evtree->Branch("met_py",&met_py,"met_py/F");
  evtree->Branch("genmet_diBaxis_p",&genmet_diBaxis_p,"genmet_diBaxis_p/F");
  evtree->Branch("genmet_diBaxis_t",&genmet_diBaxis_t,"genmet_diBaxis_t/F");
  evtree->Branch("met_diBaxis_p",&met_diBaxis_p,"met_diBaxis_p/F");
  evtree->Branch("met_diBaxis_t",&met_diBaxis_t,"met_diBaxis_t/F");

  evtree->Branch("hasGenMET",&hasGenMET, "hasGenMET/B");
  evtree->Branch("hasMET",&hasMET, "hasMET/B");
  evtree->Branch("hasdRljet",&hasdRljet, "hasdRljet/B");
  evtree->Branch("h2tohh_mass",&h2tohh_mass,"h2tohh_mass/F");
  evtree->Branch("h2tohh",&h2tohh,"h2tohh/B");
  evtree->Branch("preselections",&preselections, "preselections/B");


  evtree->Branch("MMC_h2mass_prob",&MMC_h2mass_prob,"MMC_h2mass_prob/F");
  evtree->Branch("MMC_h2massweight1_prob",&MMC_h2massweight1_prob,"MMC_h2massweight1_prob/F");
  evtree->Branch("MMC_h2massweight4_prob",&MMC_h2massweight4_prob,"MMC_h2massweight4_prob/F");
  evtree->Branch("MMC_h2mass_RMS",&MMC_h2mass_RMS,"MMC_h2mass_RMS/F");
  evtree->Branch("MMC_h2mass_Mean",&MMC_h2mass_Mean,"MMC_h2mass_Mean/F");
  evtree->Branch("MMC_h2mass_Entries",&MMC_h2mass_Entries,"MMC_h2mass_Entries/F");
  evtree->Branch("MMC_h2mass_overflow",&MMC_h2mass_overflow,"MMC_h2mass_overflow/F");
  evtree->Branch("MMC_h2mass_underflow",&MMC_h2mass_underflow,"MMC_h2mass_overflow/F");
}

void DiHiggstoWWbb::writeTree(){
  TFile* file = new TFile(outputFile,"recreate");
  evtree->Write();
  file->Close();
  calculateNormfactor(3);
}

DiHiggstoWWbb::~DiHiggstoWWbb(){
  cout << "Calling destructor." << endl;
  delete treeReader;
  delete chain;
  delete evtree;
}

// ------------ To find H->hh->WWBB->lvlvBB  ------------
void DiHiggstoWWbb::fetchHhhchain(TClonesArray *branchParticle){

  genh2 =  (GenParticle*) branchParticle->At(0);
  //printGenParticle(genP);
  if (genh2->PID != 99926 and genh2->PID != 99927) {
    cout <<" first genparticle is not 99926 nor 99927 " << endl; 
    printGenParticle(genh2);
    return;
  }
  h2tohh_mass = genh2->Mass;
  genhiggs1 =  (GenParticle*) branchParticle->At(genh2->D1); 
  genhiggs2 =  (GenParticle*) branchParticle->At(genh2->D2); 
  //printGenParticle(genhiggs2);
  while ((genhiggs1->D1>0 && ((GenParticle*)branchParticle->At(genhiggs1->D1))->PID == genhiggs1->PID) 
	|| (genhiggs1->D2>0 && ((GenParticle*)branchParticle->At(genhiggs1->D2))->PID == genhiggs1->PID)){
    if (genhiggs1->D1>0 && ((GenParticle*)branchParticle->At(genhiggs1->D1))->PID == genhiggs1->PID) 
	genhiggs1 = (GenParticle*)branchParticle->At(genhiggs1->D1);
    else   genhiggs1 = (GenParticle*)branchParticle->At(genhiggs1->D2);
  }
  while ((genhiggs2->D1>0 && ((GenParticle*)branchParticle->At(genhiggs2->D1))->PID == genhiggs2->PID) 
	|| (genhiggs2->D2>0 && ((GenParticle*)branchParticle->At(genhiggs2->D2))->PID == genhiggs2->PID)){
    if (genhiggs2->D1>0 && ((GenParticle*)branchParticle->At(genhiggs2->D1))->PID == genhiggs2->PID) 
	genhiggs2 = (GenParticle*)branchParticle->At(genhiggs2->D1);
    else   genhiggs2 = (GenParticle*)branchParticle->At(genhiggs2->D2);
  }
  if (abs(((GenParticle*)branchParticle->At(genhiggs1->D1))->PID) ==5 ) {
    //printGenParticle(genhiggs1);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs1->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs1->D2));
    htobb = true;
    genhtobb = genhiggs1;
  }
  else if (abs(((GenParticle*)branchParticle->At(genhiggs2->D1))->PID) ==5 ) {
    //printGenParticle(genhiggs2);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D2));
    htobb = true;
    genhtobb = genhiggs2;
  }
  if (abs(((GenParticle*)branchParticle->At(genhiggs2->D1))->PID) == 24){
    htoWW = true;
    genhtoWW = genhiggs2;
  }else if (abs(((GenParticle*)branchParticle->At(genhiggs1->D1))->PID) == 24){
    htoWW = true;
    genhtoWW = genhiggs1;
  }
  if (htobb){
    if (((GenParticle*)branchParticle->At(genhtobb->D1))->PID == 5){
	genb1 = (GenParticle*)branchParticle->At(genhtobb->D1);
	genb2 = (GenParticle*)branchParticle->At(genhtobb->D2);
    } 
    else {
	genb1 = (GenParticle*)branchParticle->At(genhtobb->D2);
	genb2 = (GenParticle*)branchParticle->At(genhtobb->D1);
    }
    // Move on to "final state" bquarks
    //printGenParticle(genhtobb);
    getFinalState(genb1, branchParticle);
    getFinalState(genb2, branchParticle);
    b1_p4 = genb1->P4();
    b1_px = genb1->Px; b1_py = genb1->Py; b1_pz = genb1->Pz; b1_energy = genb1->E;
    b1_eta = genb1->Eta; b1_phi = genb1->Phi; b1_pt = genb1->PT;
    b2_p4 = genb2->P4();
    b2_px = genb2->Px; b2_py = genb2->Py; b2_pz = genb2->Pz; b2_energy = genb2->E;
    b2_eta = genb2->Eta; b2_phi = genb2->Phi; b2_pt = genb2->PT;
    //TLorentzVector bbbar_lorentz = genb1->P4()+genb2->P4();
    //cout << " bbbar_lorentz Mass "<< bbbar_lorentz.M(); bbbar_lorentz.Print();
    htobb_px = genhtobb->Px; htobb_py= genhtobb->Py; htobb_pz = genhtobb->Pz; htobb_energy = genhtobb->E;
    htobb_mass = genhtobb->Mass;
  }

  if (htoWW){
    if (((GenParticle*)branchParticle->At(genhtoWW->D1))->PID == -24){ //W
	genW1 = (GenParticle*)branchParticle->At(genhtoWW->D1);  //to muon(13)
	genW2 = (GenParticle*)branchParticle->At(genhtoWW->D2);
    } 
    else {
	genW1 = (GenParticle*)branchParticle->At(genhtoWW->D2);
	genW2 = (GenParticle*)branchParticle->At(genhtoWW->D1);
    }

    getFinalState(genW1, branchParticle);
    //cout <<" htoWW genW1 "; printGenParticle(genW1);
    getFinalState(genW2, branchParticle);	
    //cout <<" htoWW genW2 "; printGenParticle(genW2);
    w1_mass = genW1->Mass; w1_px = genW1->Px; w1_py = genW1->Py; w1_pz = genW1->Pz; w1_energy = genW1->E;
    w1_eta = genW1->Eta; w1_phi = genW1->Phi;
    w2_mass = genW2->Mass; w2_px = genW2->Px; w2_py = genW2->Py; w2_pz = genW2->Pz; w2_energy = genW2->E;
    w2_eta = genW2->Eta; w2_phi = genW2->Phi;

    if (genW1->D1>0 && ((GenParticle*)branchParticle->At(genW1->D1))->PID == 13){
	genmu1 = (GenParticle*)branchParticle->At(genW1->D1);
	gennu1 = (GenParticle*)branchParticle->At(genW1->D2);
	Wtomu1nu1 = true;
    }else if (genW1->D2 >0 && ((GenParticle*)branchParticle->At(genW1->D2))->PID == 13){
	genmu1 = (GenParticle*)branchParticle->At(genW1->D2);
	gennu1 = (GenParticle*)branchParticle->At(genW1->D1);
	Wtomu1nu1 = true;
    }
    if (genW2->D1>0 && ((GenParticle*)branchParticle->At(genW2->D1))->PID == -13){
	genmu2 = (GenParticle*)branchParticle->At(genW2->D1);
	gennu2 = (GenParticle*)branchParticle->At(genW2->D2);
	Wtomu2nu2 = true;
    } else if (genW2->D1>0 && ((GenParticle*)branchParticle->At(genW2->D2))->PID == -13){
	genmu2 = (GenParticle*)branchParticle->At(genW2->D2);
	gennu2 = (GenParticle*)branchParticle->At(genW2->D1);
	Wtomu2nu2 = true;
    }
  }

  if (Wtomu1nu1){
    getFinalState(genmu1, branchParticle);
    getFinalState(gennu1, branchParticle);
    mu1_p4 = genmu1->P4();
    mu1_px = genmu1->Px; mu1_py = genmu1->Py; mu1_pz = genmu1->Pz; mu1_energy = genmu1->E;
    mu1_eta = genmu1->Eta; mu1_phi = genmu1->Phi; mu1_pt = genmu1->PT;
    nu1_p4 = gennu1->P4();
    nu1_px = gennu1->Px; nu1_py = gennu1->Py; nu1_pz = gennu1->Pz; nu1_energy = gennu1->E;
    nu1_eta = gennu1->Eta; nu1_phi = gennu1->Phi; nu1_pt = gennu1->PT;
    //cout << "mu1 from W "; printGenParticle(genmu1);
  }
  if (Wtomu2nu2){
    getFinalState(genmu2, branchParticle);	
    getFinalState(gennu2, branchParticle);	
    mu2_p4 = genmu2->P4();
    mu2_px = genmu2->Px; mu2_py = genmu2->Py; mu2_pz = genmu2->Pz; mu2_energy = genmu2->E;
    mu2_eta = genmu2->Eta; mu2_phi = genmu2->Phi; mu2_pt = genmu2->PT;
    nu2_p4 = gennu2->P4();
    nu2_px = gennu2->Px; nu2_py = gennu2->Py; nu2_pz = gennu2->Pz; nu2_energy = gennu2->E;
    nu2_eta = gennu2->Eta; nu2_phi = gennu2->Phi; nu2_pt = gennu2->PT;
    //cout << "mu2 from W "; printGenParticle(genmu2);
  }
} 

// ------------ To find tt->WBWB->lvBlvB  ------------
void DiHiggstoWWbb::fetchttbarchain(TClonesArray *branchParticle){

  GenParticle* particle = (GenParticle*) branchParticle->At(0); 
  if (particle->PID == -6) {
    gent1 = (GenParticle*) branchParticle->At(0);// t1->W1 b1-> l1 nu1, l1: 11 13.. W1:-24, t1:-6, b1:-5
    gent2 = (GenParticle*) branchParticle->At(1);// t2->W2 b2->l2, nu2, l2:-11 -13
  }
  else if (particle->PID == 6){
    gent1 = (GenParticle*) branchParticle->At(1);
    gent2 = (GenParticle*) branchParticle->At(0);
  }
  else {  
    cout <<" first GenParticle is not top quark " << std::endl;
    return;
  }

  /*for (i =0; i < branchParticle->GetEntries(); ++i){
    GenParticle* genP = (GenParticle*) branchParticle->At(i);
  //defualt M1 M2 D1 D2 is -1;
  cout << " genP Id " << genP->PID <<" Pt " << genP->PT << " M1 "<< genP->M1<<" M2 "<< genP->M2 << " D1 "<< genP->D1 <<" D2 "<<genP->D2<< endl;
  if ( genP->M1 >= 0 && genP->M1 <branchParticle->GetEntries()){
  GenParticle *M1P= (GenParticle*) branchParticle->At(genP->M1);
  cout <<" M1 Id " << M1P->PID <<" Pt " << M1P->PT << std::endl;
  }
  if ( genP->D1 >= 0 && genP->D1 <branchParticle->GetEntries()){
  GenParticle *D1P= (GenParticle*) branchParticle->At(genP->D1);
  cout <<" D1 Id " << D1P->PID <<" Pt " << D1P->PT << std::endl;
  }
  }*/
  getQuarkFinalState(gent1, branchParticle);
  getQuarkFinalState(gent2, branchParticle);

  if (((GenParticle*)branchParticle->At(gent1->D1))->PID == 5) {
    //printGenParticle(genhiggs1);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs1->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs1->D2));
    genb1 = (GenParticle*)branchParticle->At(gent1->D1);
    genW1 = (GenParticle*)branchParticle->At(gent1->D2);
    tbartoWbbar = true;

  }
  else if (((GenParticle*)branchParticle->At(gent1->D1))->PID == -24 ) {
    //printGenParticle(genhiggs2);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D2));
    genb1 = (GenParticle*)branchParticle->At(gent1->D2);
    genW1 = (GenParticle*)branchParticle->At(gent1->D1);
    tbartoWbbar = true;
  }

  if (((GenParticle*)branchParticle->At(gent2->D1))->PID == -5) {
    //printGenParticle(genhiggs1);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs1->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs1->D2));
    genb2 = (GenParticle*)branchParticle->At(gent2->D1);
    genW2 = (GenParticle*)branchParticle->At(gent2->D2);
    ttoWb = true;
  }
  else if (((GenParticle*)branchParticle->At(gent2->D1))->PID == 24 ) {
    //printGenParticle(genhiggs2);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D2));
    genb2 = (GenParticle*)branchParticle->At(gent2->D2);
    genW2 = (GenParticle*)branchParticle->At(gent2->D1);
    ttoWb = true;
  }
  /*if (ttoWb and tbartoWbbar) cout <<" find t and tbar and they decay into W b " << endl;
    else {
    cout <<" find t and tbar but they may not decay into W b" << endl;
    cout <<"tbar "; printGenParticle(gent1);
    cout <<"tbar's child "; printGenParticle((GenParticle*)branchParticle->At(gent1->D1));
    cout <<"tbar's child "; printGenParticle((GenParticle*)branchParticle->At(gent1->D2));
    printGenParticle(gent2);
    cout <<"t "; printGenParticle(gent2);
    cout <<"t's child "; printGenParticle((GenParticle*)branchParticle->At(gent2->D1));
    cout <<"t's child "; printGenParticle((GenParticle*)branchParticle->At(gent2->D2));
    }*/

  if (tbartoWbbar and ttoWb){
    //move on to "final state" bquarks
    t1_px = gent1->Px; t1_py = gent1->Py; t1_pz = gent1->Pz; t1_mass = gent1->Mass; t1_energy = gent1->E;
    t2_px = gent2->Px; t2_py = gent2->Py; t2_pz = gent2->Pz; t2_mass = gent2->Mass; t2_energy = gent2->E;
    getFinalState(genb1, branchParticle);
    getFinalState(genb2, branchParticle);
    b1_p4 = genb1->P4();
    b1_px = genb1->Px; b1_py = genb1->Py; b1_pz = genb1->Pz; b1_energy = genb1->E;
    b1_eta = genb1->Eta; b1_phi = genb1->Phi; b1_pt = genb1->PT;
    b2_p4 = genb2->P4();
    b2_px = genb2->Px; b2_py = genb2->Py; b2_pz = genb2->Pz; b2_energy = genb2->E;
    b2_eta = genb2->Eta; b2_phi = genb2->Phi; b2_pt = genb2->PT;
    //TLorentzVector bbbar_lorentz = genb1->P4()+genb2->P4();
    //cout << " bbbar_lorentz Mass "<< bbbar_lorentz.M(); bbbar_lorentz.Print();
    getFinalState(genW1, branchParticle);	
    getFinalState(genW2, branchParticle);	
    w1_mass = genW1->Mass; w1_px = genW1->Px; w1_py = genW1->Py; w1_pz = genW1->Pz; w1_energy = genW1->E;
    w1_eta = genW1->Eta; w1_phi = genW1->Phi;
    w2_mass = genW2->Mass; w2_px = genW2->Px; w2_py = genW2->Py; w2_pz = genW2->Pz; w2_energy = genW2->E;
    w2_eta = genW2->Eta; w2_phi = genW2->Phi;
    //cout <<" htoWW genW2 "; printGenParticle(genW2);

    if (genW1->D1>0 && ((GenParticle*)branchParticle->At(genW1->D1))->PID == 13){
	genmu1 = (GenParticle*)branchParticle->At(genW1->D1);
	gennu1 = (GenParticle*)branchParticle->At(genW1->D2);
	Wtomu1nu1 = true;
    }else if (genW1->D2 >0 && ((GenParticle*)branchParticle->At(genW1->D2))->PID == 13){
	genmu1 = (GenParticle*)branchParticle->At(genW1->D2);
	gennu1 = (GenParticle*)branchParticle->At(genW1->D1);
	Wtomu1nu1 = true;
    }
    if (genW2->D1>0 && ((GenParticle*)branchParticle->At(genW2->D1))->PID == -13){
	genmu2 = (GenParticle*)branchParticle->At(genW2->D1);
	gennu2 = (GenParticle*)branchParticle->At(genW2->D2);
	Wtomu2nu2 = true;
    } else if (genW2->D1>0 && ((GenParticle*)branchParticle->At(genW2->D2))->PID == -13){
	genmu2 = (GenParticle*)branchParticle->At(genW2->D2);
	gennu2 = (GenParticle*)branchParticle->At(genW2->D1);
	Wtomu2nu2 = true;
    }
  }

  if (Wtomu1nu1){
    getFinalState(genmu1, branchParticle);
    getFinalState(gennu1, branchParticle);
    mu1_p4 = genmu1->P4();
    mu1_px = genmu1->Px; mu1_py = genmu1->Py; mu1_pz = genmu1->Pz; mu1_energy = genmu1->E;
    mu1_eta = genmu1->Eta; mu1_phi = genmu1->Phi; mu1_pt = genmu1->PT;
    nu1_p4 = gennu1->P4();
    nu1_px = gennu1->Px; nu1_py = gennu1->Py; nu1_pz = gennu1->Pz; nu1_energy = gennu1->E;
    nu1_eta = gennu1->Eta; nu1_phi = gennu1->Phi; nu1_pt = gennu1->PT;
    //	cout << "mu1 from W "; printGenParticle(genmu1);
  }
  if (Wtomu2nu2){
    getFinalState(genmu2, branchParticle);
    getFinalState(gennu2, branchParticle);
    mu2_p4 = genmu2->P4();
    mu2_px = genmu2->Px; mu2_py = genmu2->Py; mu2_pz = genmu2->Pz; mu2_energy = genmu2->E;
    mu2_eta = genmu2->Eta; mu2_phi = genmu2->Phi; mu2_pt = genmu2->PT;
    nu2_p4 = gennu2->P4();
    nu2_px = gennu2->Px; nu2_py = gennu2->Py; nu2_pz = gennu2->Pz; nu2_energy = gennu2->E;
    nu2_eta = gennu2->Eta; nu2_phi = gennu2->Phi; nu2_pt = gennu2->PT;
    //   cout << "mu2 from W "; printGenParticle(genmu2);
  }
}

void DiHiggstoWWbb::getGenMET(TClonesArray *branchGenMET){
  // Analyse missing ET, generator level
  if(branchGenMET->GetEntriesFast() > 0){
    genMet = (MissingET*) branchGenMET->At(0);
    genmet = genMet->MET;
    genmet_phi = genMet->Phi;
    genmet_px = genMet->P4().Px();
    genmet_py = genMet->P4().Py();
    genmet_p4 = TLorentzVector();
    genmet_p4.SetXYZT(met_px, met_py, 0, met); 
    if (genmet > metPt_) hasGenMET = true;
  }
}

void DiHiggstoWWbb::matchMuon2Gen(TClonesArray *branchMuonBeforeIso, TClonesArray *branchMuon, GenParticle* genmu1, GenParticle* genmu2, float dR_){

  Muon *muon;
  GenParticle *particle;
  for(int i = 0; i < branchMuonBeforeIso->GetEntriesFast(); ++i){
    muon = (Muon*) branchMuonBeforeIso->At(i);
    TLorentzVector muon_p4 = muon->P4(); 
    particle = (GenParticle*) muon->Particle.GetObject();
    //printGenParticle(particle);
    if (genmu1 !=0 and muon->Charge<0 and muon_p4.DeltaR(genmu1->P4())<dR_mu1_beforeIso and muon_p4.DeltaR(genmu1->P4())<dR_) {
	dR_mu1_beforeIso = muon_p4.DeltaR(genmu1->P4());
	muon1_beforeIso = muon;
	hasMuon1_beforeIso = true;
	if (particle == genmu1) Muon1_beforeIso_hasgenMu = true;
    }
    else if(genmu2 !=0 and muon->Charge>0 and muon_p4.DeltaR(genmu2->P4())<dR_mu2_beforeIso and muon_p4.DeltaR(genmu2->P4())<dR_) {
	dR_mu2_beforeIso = muon_p4.DeltaR(genmu2->P4());
	muon2_beforeIso = muon;
	hasMuon2_beforeIso = true;
	if (particle == genmu2) Muon2_beforeIso_hasgenMu = true;
    }
    //if (hasMuon1_beforeIso) std::cout <<" has reco Muon1 before Iso  " << std::endl;
    //if (hasMuon2_beforeIso) std::cout <<" has reco Muon2 before Iso  " << std::endl;
  }
  if (hasMuon1_beforeIso){
    Muon1_beforeIso_px = muon1_beforeIso->P4().Px(); Muon1_beforeIso_py = muon1_beforeIso->P4().Py(); Muon1_beforeIso_pz = muon1_beforeIso->P4().Pz(); Muon1_beforeIso_energy = muon1_beforeIso->P4().E();
    Muon1_beforeIso_eta = muon1_beforeIso->P4().Eta(); Muon1_beforeIso_phi = muon1_beforeIso->P4().Phi();
    Muon1_beforeIso_pt = muon1_beforeIso->PT;
  }
  if (hasMuon2_beforeIso){
    Muon2_beforeIso_px = muon2_beforeIso->P4().Px(); Muon2_beforeIso_py = muon2_beforeIso->P4().Py(); Muon2_beforeIso_pz = muon2_beforeIso->P4().Pz(); Muon2_beforeIso_energy = muon2_beforeIso->P4().E();
    Muon2_beforeIso_eta = muon2_beforeIso->P4().Eta(); Muon2_beforeIso_phi = muon2_beforeIso->P4().Phi();
    Muon2_beforeIso_pt = muon2_beforeIso->PT;
  }
  // Loop over all Muon in event, reco muon
  for(int i = 0; i < branchMuon->GetEntriesFast(); ++i){
    muon = (Muon*) branchMuon->At(i);
    TLorentzVector muon_p4 = muon->P4(); 
    particle = (GenParticle*) muon->Particle.GetObject();
    // printGenParticle(particle);
    if (hasMuon1_beforeIso and  muon->Eta == muon1_beforeIso->Eta and muon->Phi == muon1_beforeIso->Phi) Muon1_beforeIso_passIso = true;
    if (hasMuon2_beforeIso and  muon->Eta == muon2_beforeIso->Eta and muon->Phi == muon2_beforeIso->Phi) Muon2_beforeIso_passIso = true;
    //check charge and deltaR, genmu1: charge<0, genmu2: charge>0
    if (genmu1 !=0 and muon->Charge<0 and muon_p4.DeltaR(genmu1->P4())<dR_mu1 and muon_p4.DeltaR(genmu1->P4())<dR_) {
	dR_mu1 = muon_p4.DeltaR(genmu1->P4());
	muon1 = muon;
	hasMuon1 = true;
    	Muon1_p4 = muon->P4();
	if (particle == genmu1) Muon1_hasgenMu = true;
    }
    else if (genmu2 !=0 and muon->Charge>0 and muon_p4.DeltaR(genmu2->P4())<dR_mu2 and muon_p4.DeltaR(genmu2->P4())<dR_) {
	dR_mu2 = muon_p4.DeltaR(genmu2->P4());
	muon2 = muon;
	hasMuon2 = true;
    	Muon2_p4 = muon->P4();
	if (particle == genmu2) Muon2_hasgenMu = true;
    }
    //if (hasMuon1) std::cout <<" has reco Muon1 " << std::endl;
    //if (hasMuon2) std::cout <<" has reco Muon2 " << std::endl;
    //cout <<" muon eta " << muon->Eta << " phi " << muon->Phi << " Pt "<< muon->PT << endl; 
  }
}

void DiHiggstoWWbb::matchBjets2Gen(TClonesArray *branchGenJet, TClonesArray *branchJet, GenParticle* genb1, GenParticle* genb2, float dR_){
  //loop all Gen jets 
  Jet *genjet, *jet;
  genjet =0; jet=0;
  for (int i =0;  i<  branchGenJet->GetEntries(); i++){
    genjet = (Jet*) branchGenJet->At(i);
    if (genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
    TLorentzVector genjet_p4 = genjet->P4();
    if (genb1 !=0  && genjet_p4.DeltaR(genb1->P4()) < dR_genb1jet && genjet_p4.DeltaR(genb1->P4())< dR_) {
	genb1jet_p4 = genjet_p4;
	dR_genb1jet = genjet_p4.DeltaR(genb1->P4());
	genb1jet_px = genjet_p4.Px(); genb1jet_py = genjet_p4.Py(); genb1jet_pz=genjet_p4.Pz(); genb1jet_energy = genjet_p4.Energy();
	genb1jet_eta = genjet_p4.Eta(); genb1jet_phi = genjet_p4.Phi();
	genb1jet_pt = genjet_p4.Pt();
	hasgenb1jet = true;
	genb1jet = genjet;
	//cout <<"genb1jet pt "<< genb1jet_pt << endl;
    }
    if (genb2 !=0  && genjet_p4.DeltaR(genb2->P4()) < dR_genb2jet && genjet_p4.DeltaR(genb2->P4()) < dR_){
	genb2jet_p4 = genjet_p4;
	dR_genb2jet = genjet_p4.DeltaR(genb2->P4());
	genb2jet_px = genjet_p4.Px(); genb2jet_py = genjet_p4.Py(); genb2jet_pz=genjet_p4.Pz(); genb2jet_energy = genjet_p4.Energy();
	genb2jet_eta = genjet_p4.Eta(); genb2jet_phi = genjet_p4.Phi();
	genb2jet_pt = genjet_p4.Pt();
	hasgenb2jet = true;
	genb2jet = genjet;
	//cout <<"genb2jet pt "<< genb2jet_pt << endl;
    }
  }

  //loop all reco jets 
  for (int i =0;  i<  branchJet->GetEntries(); i++)
  {
    jet = (Jet*) branchJet->At(i);
    if (jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
    TLorentzVector jet_p4 = jet->P4();
    if (genb1 !=0 && jet_p4.DeltaR(genb1->P4()) < dR_b1jet && jet_p4.DeltaR(genb1->P4()) < dR_) {
	b1jet = jet;
	dR_b1jet = jet_p4.DeltaR(genb1->P4());
	b1jet_p4 = jet_p4;
	//cout <<"b1jet pt "<< b1jet_pt << endl;
	hasb1jet = true;
    }
    if (genb2 !=0 && jet_p4.DeltaR(genb2->P4()) < dR_b2jet && jet_p4.DeltaR(genb2->P4()) < dR_){
	b2jet = jet;
	dR_b2jet = jet_p4.DeltaR(genb2->P4());
	b2jet_p4 = jet_p4;
	hasb2jet = true;
	//cout <<"b2jet pt "<< b2jet_pt << endl;
    }
  }
  // b1jet should be different from b2jet
  if (hasb1jet && hasb2jet && b1jet == b2jet){
    hasb1jet = false;
    hasb2jet = false;
  }
}

void DiHiggstoWWbb::printGenParticle(GenParticle *genP){
  cout << " genP Id " << genP->PID <<" Pt " << genP->PT << " M1 "<< genP->M1<<" M2 "<< genP->M2;
  cout << " D1 "<< genP->D1 <<" D2 "<<genP->D2 << " p4 "; genP->P4().Print();
}

void DiHiggstoWWbb::printJet(Jet *jet){

  GenParticle *particle;
  Muon *muon;
  Track *track;
  Tower *tower;
  TObject *object;
  TLorentzVector momentum;
  momentum.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
  TRefArray particlearray(jet->Particles);
  cout<<"Looping over jet constituents. Jet pt: "<<jet->PT<<", eta: "<<jet->Eta<<", phi: "<<jet->Phi<<endl;      

  // Loop over all jet's constituents
  for(int j = 0; j < jet->Constituents.GetEntriesFast(); ++j){
    object = jet->Constituents.At(j);
    // Check if the constituent is accessible
    if(object == 0) continue;
    if(object->IsA() == GenParticle::Class()){
	particle = (GenParticle*) object;
	cout << "    GenPart pt: " << particle->PT << ", eta: " << particle->Eta << ", phi: " << particle->Phi << endl;
	momentum += particle->P4();
    }
    else if(object->IsA() == Track::Class()){
	track = (Track*) object;
	cout << "    Track pt: " << track->PT << ", eta: " << track->Eta << ", phi: " << track->Phi << endl;
	momentum += track->P4();
    }
    else if(object->IsA() == Tower::Class()){
	tower = (Tower*) object;
	cout << "    Tower pt: " << tower->ET << ", eta: " << tower->Eta << ", phi: " << tower->Phi << endl;
	momentum += tower->P4();
    }
    else if(object->IsA() == Muon::Class()){
	muon = (Muon*) object;
	cout << "    Muon pt: " << muon->PT << ", eta: " << muon->Eta << ", phi: " << muon->Phi << endl;
	momentum += muon->P4();
    }
  }
  cout << " constituent sum pt:  " << momentum.Pt() <<" eta "<< momentum.Eta()  <<"  phi " << momentum.Phi() << std::endl;

  for (int j =0; j<jet->Particles.GetEntries();  j++){
    GenParticle *p_tmp = (GenParticle*) particlearray.At(j);
    printGenParticle(p_tmp);
  }
}

// Get the final state of a GenParticle
void DiHiggstoWWbb::getFinalState(GenParticle* &genp, TClonesArray *branchParticle)
{
  while ((genp->D1>0 && ((GenParticle*)branchParticle->At(genp->D1))->PID == genp->PID)
	&& (genp->D2 >0 && ((GenParticle*)branchParticle->At(genp->D2))->PID == genp->PID)){
    genp = (GenParticle*)branchParticle->At(genp->D1);
  }
}

// Get the final state of top quark, even if top emits a gluon before decay, still go to next top 
void DiHiggstoWWbb::getQuarkFinalState(GenParticle* &genp, TClonesArray *branchParticle)
{
  while ((genp->D1>0 && ((GenParticle*)branchParticle->At(genp->D1))->PID == genp->PID)
	or (genp->D2 >0 && ((GenParticle*)branchParticle->At(genp->D2))->PID == genp->PID)){
    if (((GenParticle*)branchParticle->At(genp->D1))->PID == genp->PID) genp = (GenParticle*)branchParticle->At(genp->D1);
    else genp = (GenParticle*)branchParticle->At(genp->D2);
  }
}

void DiHiggstoWWbb::printAllGenParticles(TClonesArray *branchParticle){
  for (int i =0; i < branchParticle->GetEntries(); ++i){
    GenParticle* genP = (GenParticle*) branchParticle->At(i);
    // Default M1 M2 D1 D2 is -1;
    cout << " genP Id " << genP->PID <<" Pt " << genP->PT << " M1 "<< genP->M1<<" M2 "<< genP->M2 << " D1 "<< genP->D1 <<" D2 "<<genP->D2<< endl;
    if ( genP->M1 >= 0 && genP->M1 <branchParticle->GetEntries()){
	GenParticle *M1P= (GenParticle*) branchParticle->At(genP->M1);
	cout <<" M1 Id " << M1P->PID <<" Pt " << M1P->PT << std::endl;
    }
    if ( genP->M2 >= 0 && genP->M2 <branchParticle->GetEntries()){
	GenParticle *M2P= (GenParticle*) branchParticle->At(genP->M2);
	cout <<" M2 Id " << M2P->PID <<" Pt " << M2P->PT << std::endl;
    }
    if ( genP->D1 >= 0 && genP->D1 <branchParticle->GetEntries()){
	GenParticle *D1P= (GenParticle*) branchParticle->At(genP->D1);
	cout <<" D1 Id " << D1P->PID <<" Pt " << D1P->PT << std::endl;
    }
    if ( genP->D2 >= 0 && genP->D2 <branchParticle->GetEntries()){
	GenParticle *D2P= (GenParticle*) branchParticle->At(genP->D2);
	cout <<" D2 Id " << D2P->PID <<" Pt " << D2P->PT << std::endl;
    }
  }
}

void DiHiggstoWWbb::initBranches(){
  allbjets.clear();
  allMuon1.clear();
  allMuon2.clear();
  muon1 =0; muon2=0;
  b1jet=0; b2jet=0; genb1jet =0; genb2jet=0;
  Met=0; genMet=0;
  genh2=0; genhiggs1 =0; genhiggs2 =0; 
  gent1 =0; gent2=0;
  genhtobb =0; genb1= 0; genb2=0;
  genhtoWW =0; genW1 =0; genW2=0; genmu1=0; genmu2=0; gennu1=0; gennu2=0;
  muon1_beforeIso =0; muon2_beforeIso =0;
  //Track *track;
  //Tower *tower;
  //create branches 
  b1_px =0;
  b1_py =0;
  b1_pz =0;
  b1_eta = 0;
  b1_phi = 0;
  b1_pt =0;
  b1_energy =0;
  b2_px =0;
  b2_py=0;
  b2_pz=0;
  b2_eta = 0;
  b2_phi = 0;
  b2_pt =0;
  b2_energy=0;
  htobb_px=0;
  htobb_py=0;
  htobb_pz=0;
  htobb_energy=0;
  htobb_mass =0;
  htobb=false; 

  genb1jet_px=0;
  genb1jet_py=0;
  genb1jet_pz=0;
  genb1jet_eta=0;
  genb1jet_phi=0;
  genb1jet_pt=0;
  genb1jet_energy=0;
  genb2jet_px=0;
  genb2jet_py=0;
  genb2jet_pz=0;
  genb2jet_eta=0;
  genb2jet_phi=0;
  genb2jet_pt=0;
  genb2jet_energy=0;
  dR_genb1jet=2.0;
  dR_genb2jet=2.0;
  hasgenb1jet=false;
  hasgenb2jet=false;

  dR_b1jet = 2.0;
  dR_b2jet = 2.0;
  b1jet_px=0;
  b1jet_py=0;
  b1jet_pz=0;
  b1jet_eta=0;
  b1jet_phi=0;
  b1jet_pt=0;
  b1jet_energy=0;
  b1jet_btag = 0;
  b2jet_px=0;
  b2jet_py=0;
  b2jet_pz=0;
  b2jet_eta=0;
  b2jet_phi=0;
  b2jet_pt=0;
  b2jet_energy=0;
  b2jet_btag = 0;
  energeticbjets = 0;
  hasb1jet=false;
  hasb2jet=false;

  mu1_px =0;
  mu1_py =0;
  mu1_pz =0;
  mu1_eta =0;
  mu1_phi =0;
  mu1_pt =0;
  mu1_energy =0;
  mu2_px =0;
  mu2_py =0;
  mu2_pz =0;
  mu2_eta =0;
  mu2_phi =0;
  mu2_pt =0;
  mu2_energy =0;
  nu1_px =0;
  nu1_py =0;
  nu1_pz =0;
  nu1_eta =0;
  nu1_phi =0;
  nu1_pt =0.0;
  nu1_energy =0;
  nu2_px =0;
  nu2_py =0;
  nu2_pz =0;
  nu2_eta =0;
  nu2_phi =0;
  nu2_pt = 0.0;
  nu2_energy =0;

  w1_mass =0.0;
  w1_px =0.0;
  w1_py =0.0;
  w1_pz =0.0;
  w1_energy =0.0;
  w1_eta = 0.0;
  w1_phi = 0.0;
  w1_pt = 0.0;
  w2_mass =0.0;
  w2_px =0.0;
  w2_py =0.0;
  w2_pz =0.0;
  w2_energy =0.0;
  w2_eta = 0.0;
  w2_phi = 0.0;
  w2_pt = 0.0;


  Wtomu2nu2=false;
  Wtomu1nu1=false;
  htoWW=false;

  t1_px = 0.0;
  t1_py = 0.0;
  t1_pz = 0.0;
  t1_energy = 0.0;
  t1_mass = 0.0;
  t2_px = 0.0;
  t2_py = 0.0;
  t2_pz = 0.0;
  t2_energy = 0.0;
  t2_mass = 0.0;
  ttoWb =false;
  tbartoWbbar = false;
  ttbar = false;

  Muon1_beforeIso_px = 0.0;
  Muon1_beforeIso_py = 0.0;
  Muon1_beforeIso_pz = 0.0;
  Muon1_beforeIso_eta = 0.0;
  Muon1_beforeIso_phi = 0.0;
  Muon1_beforeIso_pt = 0.0;
  Muon1_beforeIso_energy = 0.0;
  Muon2_beforeIso_px = 0.0;
  Muon2_beforeIso_py = 0.0;
  Muon2_beforeIso_pz = 0.0;
  Muon2_beforeIso_eta = 0.0;
  Muon2_beforeIso_phi = 0.0;
  Muon2_beforeIso_pt = 0.0;
  Muon2_beforeIso_energy = 0.0;
  dR_mu1_beforeIso = 2.0;
  dR_mu2_beforeIso = 2.0;

  Muon1_beforeIso_hasgenMu = false; 
  Muon2_beforeIso_hasgenMu = false; 
  Muon1_beforeIso_passIso = false;
  Muon2_beforeIso_passIso = false; 
  hasMuon1_beforeIso = false;
  hasMuon2_beforeIso = false;

  Muon1_px = 0;
  Muon1_py = 0;
  Muon1_pz = 0;
  Muon1_eta = 0;
  Muon1_phi = 0;
  Muon1_pt = 0;
  Muon1_energy = 0;
  Muon2_px = 0;
  Muon2_py = 0;
  Muon2_pz = 0;
  Muon2_eta = 0;
  Muon2_phi = 0;
  Muon2_pt = 0;
  Muon2_energy = 0;
  energeticMuon1 = false;
  energeticMuon2 = false;
  dR_mu1 = 2.0;
  dR_mu2 = 2.0;

  Muon1_hasgenMu = false; 
  Muon2_hasgenMu = false; 

  htoWW_px =0;
  htoWW_py =0;
  htoWW_pz =0;
  htoWW_energy =0;
  htoWW_mass = 0;

  numbjets =0;
  numLeptons1 =0;
  numLeptons2 =0;
  hasRecob1jet =false;
  hasRecob2jet =false;
  hasRecoMuon1 =false;
  hasRecoMuon1 =false;
  dR_bl=-1.0;
  dR_b1l1=-1.0;
  dR_b1l2=-1.0;
  dR_b2l1=-1.0;
  dR_b2l2=-1.0;
  dR_l1l2=-1.0;
  dR_b1b2=-1.0;
  dR_minbl = -1.0;
  dR_genl1l2 = -1;
  mass_l1l2 = -1.0;
  energy_l1l2 = 0.0;
  pt_l1l2 = 0.0;
  phi_l1l2 = 0.0;
  eta_l1l2 = 0.0;
  mass_b1b2 = -1.0;
  energy_b1b2 = 0.0;
  pt_b1b2 = 0.0;
  phi_b1b2 = 0.0;
  eta_b1b2 = 0.0;
  dphi_llbb = -10;
  dphi_llmet = -10;
  mass_trans = 0.0;

  genmet = 0;
  genmet_phi = 0;
  genmet_px = 0;
  genmet_py = 0;
  met = 0;
  met_phi = 0;
  met_px = 0;
  met_py = 0;
  genmet_diBaxis_p = -999.;
  genmet_diBaxis_t = -999.;
  met_diBaxis_p = -999.;
  met_diBaxis_t = -999;

  h2tohh_mass =0;
  //additional cuts
  hasMET = false;
  hasGenMET = false;
  hastwomuons = false;
  hastwogenmuons = false;
  hasMuon1 = false;
  hasMuon2 = false;
  hasdRljet = false;
  h2tohh =false;
  preselections=false;

  //MMC results
  MMC_h2mass_prob =0.0;
  MMC_h2massweight1_prob =0.0;
  MMC_h2massweight4_prob = 0.0;
  MMC_h2mass_Entries = 0.0;
  MMC_h2mass_RMS = 0.0;
  MMC_h2mass_Mean =0.0;
  MMC_h2mass_underflow =0.0;
  MMC_h2mass_overflow =0.0;
}

/*************************
 ****run method : process all subprocess
 *************************/  
void DiHiggstoWWbb::DiHiggstoWWbbrun()
{
  long entry;
  TLorentzVector momentum;

  int i =0;
  int totnumbjets=0;
  //TFile *MMCfile = new TFile("testMMC.root", "recreate"); 
  /****** Loop over all events *****/
  int totalevents=0;
  if (nEvents_ < 0) totalevents = allEntries;
  else totalevents = nStarts_+nEvents_;

  if (totalevents > allEntries){
	cout <<" set up is not correct:  nStarts_+nEvents_ > allEntries " << endl;
	totalevents = allEntries;
  }

  for(entry = nStarts_; entry < totalevents; ++entry){

    initBranches();
    // Load selected branches with data from specified event
    bool readsuccess = treeReader->ReadEntry(entry);
    if (not readsuccess) {
	cout <<"can not read Entry through treeReader" << endl;
	exit(0);
    }
    if (nEvents_ < 100) cout << "event id  "<< entry << endl;
    else if ((entry-nStarts_+1)%(nEvents_/100) == 0)   cout <<" event id " << entry <<" number of bjets pairs " << totnumbjets <<endl;

    // If simulation, take GEN info
    if (simulation_){
	if (sample_ == Signal) fetchHhhchain(branchParticle); 
	else fetchttbarchain(branchParticle);
	dR_genl1l2 = (Wtomu1nu1 and Wtomu2nu2)?mu1_p4.DeltaR(mu2_p4): -1; 
	if (sample_ == Background and dR_genl1l2 > 2.5) continue;
	matchBjets2Gen(branchGenJet, branchJet, genb1, genb2, jetsDeltaR_); 
	matchMuon2Gen(branchMuonBeforeIso, branchMuon,genmu1, genmu2, leptonsDeltaR_); 
	getGenMET(branchGenMissingET);
	//GENMET on Di-BJet Axis
	if( hasgenb1jet and hasgenb2jet ){
	  TVector3 genDiBjet_Transv( (genb1jet_p4 + genb2jet_p4).Px(), (genb1jet_p4 + genb2jet_p4).Py(), 0.);
	  TVector3 genMet_Transv( genmet_px, genmet_py, 0.);
	  float dphi_genmet_dibijet = TVector2::Phi_mpi_pi(genDiBjet_Transv.Phi()-genMet_Transv.Phi());
	  genmet_diBaxis_p = genMet_Transv.Mag()*cos(dphi_genmet_dibijet);
	  genmet_diBaxis_t = genMet_Transv.Mag()*sin(dphi_genmet_dibijet);
	}
    }

    //loop all reco jets 
    for (i =0;  i < branchJet->GetEntries(); i++){
	Jet* jet = (Jet*) branchJet->At(i);
	//Btag does not work here?????????
	if (jet->PT < jetsPt_ || abs(jet->Eta)> jetsEta_) continue;
	totjets_lorentz +=jet->P4();
	//bit1: loose btag, bit2: medium btag, bit3: tight. current sample: only medium btag implemented
	if ((jet->BTag<1) || jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
	allbjets.push_back(jet);
    }
    numbjets = allbjets.size();
    if (numbjets>=2) totnumbjets++;
    unsigned int b1at = 0;
    unsigned int b2at = 1;
    if (allbjets.size() > 2){
	// Find two bjets that have invariant mass close to 125
	float diff_higgsmass = 125.0;
	for (unsigned k =0; k<allbjets.size()-1; k++)
	  for (unsigned int j =i+1; j<allbjets.size(); j++){
	    TLorentzVector bjets_p4 = allbjets[k]->P4()+allbjets[j]->P4();
	    if (fabs(bjets_p4.M()-125)>diff_higgsmass) continue;
	    b1at = k;
	    b2at = j;
	    diff_higgsmass = fabs(bjets_p4.M()-125);
	  }
    }
    if (simulation_ and hasb1jet and hasb2jet and allbjets.size() >= 2){
	if (b1jet == allbjets.at(b1at) or b1jet==allbjets.at(b2at)) hasRecob1jet = true;
	if (b2jet == allbjets.at(b1at) or b2jet==allbjets.at(b2at)) hasRecob2jet = true;
    }
    else if ( !simulation_ and allbjets.size() >= 2){
	hasb1jet = true; hasRecob1jet = true;
	hasb2jet = true; hasRecob1jet = true;
	b1jet = allbjets.at(b1at);
	b2jet = allbjets.at(b2at);
	b1jet_p4 = b1jet->P4();
	b2jet_p4 = b2jet->P4();
    }

    // Analyse MET, reco level
    if(branchMissingET->GetEntriesFast() > 0){
	Met = (MissingET*) branchMissingET->At(0);
	met = Met->MET;
	met_phi = Met->Phi;
	met_px = Met->P4().Px();
	met_py = Met->P4().Py();
	Met_p4 = TLorentzVector();
	Met_p4.SetXYZT(met_px, met_py, 0, met); 
	if (met > metPt_) hasMET = true;
	//MET on Di-BJet Axis
	if( hasgenb1jet and hasgenb2jet ){
	  TVector3 genDiBjet_Transv( (genb1jet_p4 + genb2jet_p4).Px(), (genb1jet_p4 + genb2jet_p4).Py(), 0.);
	  TVector3 Met_Transv( met_px, met_py, 0.);
	  float dphi_met_dibijet = TVector2::Phi_mpi_pi(genDiBjet_Transv.Phi()-Met_Transv.Phi());
	  met_diBaxis_p = Met_Transv.Mag()*cos(dphi_met_dibijet);
	  met_diBaxis_t = Met_Transv.Mag()*sin(dphi_met_dibijet);
	}
    }

    // Loop over all Muon in the event, reco muon
    for(i = 0; i < branchMuon->GetEntriesFast(); ++i){
	Muon *muon = (Muon*) branchMuon->At(i);
	if (muon->Charge<0 and abs(muon->Eta)<muonsEta_ and muon->PT >= muonPt1_) 
	  allMuon1.push_back(muon);
	else if(muon->Charge>0 and abs(muon->Eta)<muonsEta_ and muon->PT >= muonPt1_) 
	  allMuon2.push_back(muon);
    }
    numLeptons1 = allMuon1.size();
    numLeptons2 = allMuon2.size();

if (simulation_ and hasMuon1 and hasMuon2 and allMuon1.size()>0 and allMuon2.size()>0){
if (muon1 == allMuon1.at(0)) hasRecoMuon1 = true;
	if (muon2 == allMuon2.at(0)) hasRecoMuon2 = true;
    }
    else if ( !simulation_ and allMuon1.size()>0 and allMuon2.size()>0){
	hasMuon1 = true; hasRecoMuon1 = true;
	hasMuon2 = true; hasRecoMuon2 = true;
	muon1 = allMuon1.at(0);
	muon2 = allMuon2.at(0);
	Muon1_p4 = muon1->P4();
	Muon2_p4 = muon2->P4();
    }

    //calculate additional variables for clearing up cuts and other studies
    if (hasb1jet and hasb2jet and hasMuon1 and hasMuon2){
	dR_bl   = (b1jet_p4.Pt()>b2jet_p4.Pt()) ? (b1jet_p4.DeltaR( (Muon1_p4.Pt()>Muon2_p4.Pt()) ? Muon1_p4 : Muon2_p4 )) : (b2jet_p4.DeltaR( (Muon1_p4.Pt()>Muon2_p4.Pt()) ? Muon1_p4 : Muon2_p4 ));
	dR_b1l1 = b1jet_p4.DeltaR(Muon1_p4);
	dR_b1l2 = b1jet_p4.DeltaR(Muon2_p4);
	dR_b2l1 = b2jet_p4.DeltaR(Muon1_p4);
	dR_b2l2 = b2jet_p4.DeltaR(Muon2_p4);
	dR_b1b2 = b1jet_p4.DeltaR(b2jet_p4);
	dR_l1l2 = Muon1_p4.DeltaR(Muon2_p4);
	TLorentzVector ll_p4 = Muon1_p4+Muon2_p4;
	TLorentzVector bjets_p4 = b1jet_p4+b2jet_p4;
	dR_minbl = min(min(dR_b1l1,dR_b1l2),min(dR_b2l1,dR_b2l2));
	dphi_llbb = TVector2::Phi_mpi_pi(ll_p4.Phi()-bjets_p4.Phi());
	dphi_llmet = TVector2::Phi_mpi_pi(ll_p4.Phi()-met_phi);
	mass_l1l2 = ll_p4.M(); energy_l1l2 = ll_p4.Energy(); pt_l1l2 = ll_p4.Pt(); eta_l1l2 = ll_p4.Eta(); phi_l1l2 = ll_p4.Phi();
	mass_b1b2 = bjets_p4.M(); energy_b1b2 = bjets_p4.Energy(); pt_b1b2 = bjets_p4.Pt(); eta_b1b2 = bjets_p4.Eta(); phi_b1b2 = bjets_p4.Phi();
	mass_trans = sqrt(2*ll_p4.Pt()*met*(1-cos(dphi_llmet)));
	if (dR_b1l1 > jetleptonDeltaR_ and dR_b1l2 > jetleptonDeltaR_ and dR_b2l1 > jetleptonDeltaR_ and dR_b2l2 > jetleptonDeltaR_) hasdRljet =true;
    }

    fillbranches();

    //-------- Here is possible to compute new variables --------
    if(true){ //Selection to be applied before the computation
	//Use GEN or RECO particles
    }

    //-------- MMC --------
    preselections = (hasb1jet and hasb2jet and hasMET and hastwomuons and hasdRljet);
    //only in simulation case, these two could be true
    h2tohh = (htobb and Wtomu1nu1 and Wtomu2nu2);
    ttbar  = (ttoWb and tbartoWbbar);
    if (runMMC_ and preselections and (not(simulation_) || (h2tohh and sample_==Signal) || (ttbar and sample_ ==Background))){
	cout <<" start to run MMC for this event " << entry <<endl;
	TLorentzVector bjet_pt1_lorentz, bjet_pt2_lorentz, bgenp_pt1_lorentz, bgenp_pt2_lorentz;
	if (useRecoBJet_ and b1jet_p4.Pt()>b2jet_p4.Pt()) {
	  cout <<" use Reco bjet " << endl;
	  bjet_pt1_lorentz = b1jet_p4; bjet_pt2_lorentz = b2jet_p4;
	} else if(useRecoBJet_){ 
	  cout <<" use Reco bjet " << endl;
		bjet_pt1_lorentz = b2jet_p4; bjet_pt2_lorentz = b1jet_p4;
	}

	if (simulation_ and not(useRecoBJet_) and genb1jet_p4.Pt()>genb2jet_p4.Pt()) {
	  cout <<" use Gen bjet " << endl;
	    bjet_pt1_lorentz = genb1jet_p4; bjet_pt2_lorentz = genb2jet_p4; }
	else if (simulation_ and not(useRecoBJet_) and genb1jet_p4.Pt()<genb2jet_p4.Pt()) {
	  cout <<" use Gen bjet " << endl;
		bjet_pt1_lorentz = genb2jet_p4; bjet_pt2_lorentz = genb1jet_p4;
	}

	if (b1_p4.Pt()>b2_p4.Pt()) {
	  bgenp_pt1_lorentz = b1_p4; bgenp_pt2_lorentz = b2_p4;
	} else { bgenp_pt1_lorentz = b2_p4; bgenp_pt2_lorentz = b1_p4; }


	TLorentzVector lepton1_lorentz, lepton2_lorentz;
	if (useRecoMuon_ ){
	  cout <<" use Reco Muon " << endl;
		lepton1_lorentz = Muon1_p4; lepton2_lorentz = Muon2_p4;
	}else if (not(useRecoMuon_) and simulation_){
	  cout <<" use Gen Muon " << endl;
		lepton1_lorentz = mu1_p4; lepton2_lorentz = mu2_p4;
	}

	TLorentzVector Met_lorentz;
	cout <<" use "<<(useRecoMET_?" Reco ":" Gen ") << "MET" << endl;
	if (useRecoMET_)	Met_lorentz = Met_p4;
	else if (not(useRecoMET_) and simulation_) Met_lorentz = genmet_p4;


	TLorentzVector h2tohh_genp_lorentz = TLorentzVector();
	//if (simulation_ and sample_==Signal) genh2->P4().Print();
	if (simulation_ and sample_==Signal) h2tohh_genp_lorentz = genh2->P4();
	else if (simulation_) h2tohh_genp_lorentz = gent1->P4()+gent2->P4();

	int onshellMarker_;
	if (genW1->Mass > genW2->Mass) onshellMarker_=1;
	else onshellMarker_=2;
	// Rescale bjets in MMC?????
	//MMC *thismmc = new MMC();
	MMC *thismmc = new MMC(&lepton1_lorentz, &lepton2_lorentz, &bjet_pt1_lorentz, &bjet_pt2_lorentz, &totjets_lorentz, &Met_lorentz, 
	    &nu1_p4, &nu2_p4, &bgenp_pt1_lorentz, &bgenp_pt2_lorentz, &h2tohh_genp_lorentz, 
	    onshellMarker_, simulation_, entry, weightfromonshellnupt_func_, weightfromonshellnupt_hist_, weightfromonoffshellWmass_hist_,
	    iterations_, RefPDFfile_, useMET_, bjetrescaleAlgo_);

	if (thismmc->runMMC()) {
	  //MMCtree =  (thismmc->getMMCTree())->CloneTree();
	  TH1F* MMC_h2mass =(TH1F*)(thismmc->getMMCh2()).Clone("MMC_h2mass");
	  TH1F* MMC_h2mass_weight1 =(TH1F*)(thismmc->getMMCh2weight1()).Clone("MMC_h2massweight1");
	  TH1F* MMC_h2mass_weight4 =(TH1F*)(thismmc->getMMCh2weight4()).Clone("MMC_h2massweight4");
	  std::cout <<" Mass_h2mass in Analyzer " << std::endl;
	  MMC_h2mass_prob = (MMC_h2mass->GetXaxis())->GetBinCenter(MMC_h2mass->GetMaximumBin());
	  MMC_h2massweight1_prob = (MMC_h2mass_weight1->GetXaxis())->GetBinCenter(MMC_h2mass_weight1->GetMaximumBin());
	  MMC_h2massweight4_prob = (MMC_h2mass_weight4->GetXaxis())->GetBinCenter(MMC_h2mass_weight4->GetMaximumBin());
	  MMC_h2mass_RMS = MMC_h2mass->GetRMS();
	  MMC_h2mass_Entries = MMC_h2mass->GetEntries();
	  MMC_h2mass_Mean = MMC_h2mass->GetMean();
	  int nbin=(MMC_h2mass->GetXaxis())->GetNbins();
	  MMC_h2mass_overflow = MMC_h2mass->GetBinContent(nbin+1);
	  MMC_h2mass_underflow = MMC_h2mass->GetBinContent(-1);
	  std::cout <<" most prob " << MMC_h2mass_prob <<" RMS "<< MMC_h2mass_RMS << " entries " << MMC_h2mass_Entries 
	    << " most prob weight1 "<< MMC_h2massweight1_prob <<" weight4 "<< MMC_h2massweight4_prob <<std::endl;
	}
	//MMCfile->WriteObject(mmctree,mmctree->GetTitle());
	delete thismmc;
    }
    //fill branches
    //if ((hasb1jet or hasb2jet) and h2tohh) evtree->Fill();
    if (h2tohh or ttbar or (hasb1jet and hasb2jet)) evtree->Fill();
  }
  //if (runMMC_ and hasdRljet and hasMET and ((h2tohh and is_signal) || (ttbar and !is_signal))) delete thismmc;
  //MMCfile->Close();
  //delete MMCfile;
  //evtree->Fill();
  //evtree->Print();
}

void DiHiggstoWWbb::fillbranches(){

  if (hasb1jet){
    b1jet_btag = b1jet->BTag;
    b1jet_px = b1jet_p4.Px(); b1jet_py = b1jet_p4.Py(); b1jet_pz= b1jet_p4.Pz(); b1jet_energy = b1jet_p4.Energy();
    b1jet_eta = b1jet_p4.Eta(); b1jet_phi = b1jet_p4.Phi(); b1jet_pt = b1jet_p4.Pt();
  }
  if (hasb2jet){
    b2jet_btag = b2jet->BTag;
    b2jet_px = b2jet_p4.Px(); b2jet_py = b2jet_p4.Py(); b2jet_pz= b2jet_p4.Pz(); b2jet_energy = b2jet_p4.Energy();
    b2jet_eta = b2jet_p4.Eta(); b2jet_phi = b2jet_p4.Phi(); b2jet_pt = b2jet_p4.Pt();
  }

  if (hasMuon1){
    Muon1_px = muon1->P4().Px(); Muon1_py = muon1->P4().Py(); Muon1_pz = muon1->P4().Pz(); Muon1_energy = muon1->P4().E();
    Muon1_eta = muon1->Eta; Muon1_phi = muon1->Phi; Muon1_pt = muon1->PT;
    //std::cout <<" Muon1 eta "<< muon1->Eta <<" phi "<< muon1->Phi <<" pt "<< muon1->PT << std::endl;
  }
  if (hasMuon2){
    Muon2_px = muon2->P4().Px(); Muon2_py = muon2->P4().Py(); Muon2_pz = muon2->P4().Pz(); Muon2_energy = muon2->P4().E();
    Muon2_eta = muon2->Eta; Muon2_phi = muon2->Phi; Muon2_pt = muon2->PT;
    //	std::cout <<" Muon2 eta "<< muon2->Eta <<" phi "<< muon2->Phi <<" pt "<< muon2->PT << std::endl;
  }

  if (hasMuon1 && hasMuon2){
    if (((muon1->PT > muonPt1_ && muon2->PT > muonPt2_) || (muon1->PT > muonPt2_ && muon2->PT > muonPt1_)) 
	  && fabs(muon1->Eta)<muonsEta_ && fabs(muon2->Eta)< muonsEta_) hastwomuons =true;
  }

}

/*
   bool DiHiggstoWWbb::checkEnergeticbjet(TClonesArray *branchJet){
   for (int i =0;  i<  branchJet->GetEntries(); i++){
   Jet *jet = (Jet*) branchJet->At(i);
   if (not(jet->BTag) or jet->PT < bjetsPt_ or abs(jet->Eta)> jetsEta_) continue;
   if (jet->PT>bjet_pt1 ){
   bjet_pt2= bjet_pt1;  bjet_pt1 = jet->PT; 
   }
   if (jet->PT<bjet_pt1 and jet->PT>bjet_pt2)
   bjet_pt2 = jet->PT;
   }
   if (bjet_pt2>b1jet_pt and bjet_pt2>b2jet_pt) energeticbjets = 0;
   if (bjet_pt2<=b1jet_pt and bjet_pt2<=b2jet_pt) energeticbjets = 2;
   if ((bjet_pt2<=b1jet_pt and bjet_pt2>b2jet_pt) or (bjet_pt2<=b2jet_pt and bjet_pt2>b1jet_pt))
   energeticbjets = 1;
   }
 */

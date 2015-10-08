

#ifndef ttbartoWWbb_h
#define ttbartoWWbb_h
#include <iostream>
#include <utility>
#include <vector>

#include "TString.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"

#include "classes/DelphesClasses.h"
//#include "DelphesClasses.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

class ttbartoWWbb {
   
   public:
	ttbartoWWbb();
	ttbartoWWbb(TString input_file, TString output_file, int numEvents);
	~ttbartoWWbb();

   public:
	TTree *evtree;
        TChain *chain;
	ExRootTreeReader *treeReader;

   public:
	void initBranches();//create tree and branches
	void init();//create tree and branches
	void writeTree();
	void ttbartoWWbbrun();

   private:

	void printGenParticle(GenParticle *genP);
	void printJet(Jet *jet);
	void getFinalState(GenParticle* &genp, TClonesArray *branchParticle);
	void getTopFinalState(GenParticle* &genp, TClonesArray *branchParticle);

   private:
	TString inputFile;
	TString outputFile;
	int nEvents;//events to analyse		

	
//config parameters
  	bool runMMC_;
  	double jetsPt_;
  	double jetsEta_;
  	double bjetsPt_;
  	double bjetsEta_;
  	double jetsDeltaR_;//deltaR matching
  	double jetleptonDeltaR_;
  	double leptonsDeltaR_;//deltaR matching
  //double leptonIso_ = 0.15;
  	double muonPt2_;
  	double muonPt1_;
  	double muonsEta_;
  	double metPt_;
     

   private:
	//tree branch
//create branches 
  float b1_px;
  float b1_py;
  float b1_pz;
  float b1_eta;
  float b1_phi;
  float b1_pt;
  float b1_energy;
  float b2_px;
  float b2_py;
  float b2_pz;
  float b2_eta;
  float b2_phi;
  float b2_pt;
  float b2_energy;
   
  float genb1jet_px;
  float genb1jet_py;
  float genb1jet_pz;
  float genb1jet_eta;
  float genb1jet_phi;
  float genb1jet_pt;
  float genb1jet_energy;
  float genb2jet_px;
  float genb2jet_py;
  float genb2jet_pz;
  float genb2jet_eta;
  float genb2jet_phi;
  float genb2jet_pt;
  float genb2jet_energy;
  float dR_genb1jet;
  float dR_genb2jet;
  bool hasgenb1jet;
  bool hasgenb2jet;
  
  float dR_b1jet;
  float dR_b2jet;
  float b1jet_px;
  float b1jet_py;
  float b1jet_pz;
  float b1jet_eta;
  float b1jet_phi;
  float b1jet_pt;
  float b1jet_energy;
  bool b1jet_btag;
  int energeticbjets;
  float b2jet_px;
  float b2jet_py;
  float b2jet_pz;
  float b2jet_eta;
  float b2jet_phi;
  float b2jet_pt;
  float b2jet_energy;
  bool b2jet_btag;
  bool hasb1jet;
  bool hasb2jet;

  float mu1_px;
  float mu1_py;
  float mu1_pz;
  float mu1_eta;
  float mu1_phi;
  float mu1_pt;
  float mu1_energy;
  float mu2_px;
  float mu2_py;
  float mu2_pz;
  float mu2_eta;
  float mu2_phi;
  float mu2_pt;
  float mu2_energy;
  float nu1_px;
  float nu1_py;
  float nu1_pz;
  float nu1_eta;
  float nu1_phi;
  float nu1_pt;
  float nu1_energy;
  float nu2_px;
  float nu2_py;
  float nu2_pz;
  float nu2_eta;
  float nu2_phi;
  float nu2_pt;
  float nu2_energy;
  
  bool Wtomu2nu2;
  bool Wtomu1nu1;
  float w1_px;
  float w1_py;
  float w1_pz;
  float w1_energy;
  float w1_mass;
  float w1_eta;
  float w1_phi;
  float w1_pt;
  float w2_px;
  float w2_py;
  float w2_pz;
  float w2_energy;
  float w2_mass;
  float w2_eta;
  float w2_phi;
  float w2_pt;
  
  float t1_px;
  float t1_py;
  float t1_pz;
  float t1_energy;
  float t1_mass;
  float t2_px;
  float t2_py;
  float t2_pz;
  float t2_energy;
  float t2_mass;
  bool tbartoWbbar;
  bool ttoWb;
  
  float Muon1_beforeIso_px;
  float Muon1_beforeIso_py;
  float Muon1_beforeIso_pz;
  float Muon1_beforeIso_eta;
  float Muon1_beforeIso_phi;
  float Muon1_beforeIso_pt;
  float Muon1_beforeIso_energy;
  float Muon2_beforeIso_px;
  float Muon2_beforeIso_py;
  float Muon2_beforeIso_pz;
  float Muon2_beforeIso_eta;
  float Muon2_beforeIso_phi;
  float Muon2_beforeIso_pt;
  float Muon2_beforeIso_energy;
  float dR_mu1_beforeIso;
  float dR_mu2_beforeIso;
 
  bool Muon1_beforeIso_hasgenMu; 
  bool Muon2_beforeIso_hasgenMu; 
  bool Muon1_beforeIso_passIso;
  bool Muon2_beforeIso_passIso; 
  bool hasMuon1_beforeIso;
  bool hasMuon2_beforeIso;
  
  float Muon1_px;
  float Muon1_py;
  float Muon1_pz;
  float Muon1_eta;
  float Muon1_phi;
  float Muon1_pt;
  float Muon1_energy;
  float Muon2_px;
  float Muon2_py;
  float Muon2_pz;
  float Muon2_eta;
  float Muon2_phi;
  float Muon2_pt;
  float Muon2_energy;
  bool energeticMuon1;
  bool energeticMuon2;
  float dR_mu1;
  float dR_mu2;

  bool Muon1_hasgenMu; 
  bool Muon2_hasgenMu; 


  float dR_b1l1;
  float dR_b1l2;
  float dR_b2l1;
  float dR_b2l2;
  float dR_l1l2;
  float dR_b1b2;
  float dR_minbl;
  float dR_genl1l2;
  float mass_l1l2;
  float energy_l1l2;
  float pt_l1l2;
  float phi_l1l2;
  float eta_l1l2;
  float mass_b1b2;
  float energy_b1b2;
  float pt_b1b2;
  float phi_b1b2;
  float eta_b1b2;
  float dphi_llbb;
  float dphi_llmet;
  float mass_trans;

  float genmet;
  float genmet_phi;
  float genmet_px;
  float genmet_py;
  float met;
  float met_phi;
  float met_px;
  float met_py;

//additional cuts
  bool hasMET;
  bool hasGenMET;
  bool hastwomuons;
  bool hastwogenmuons;
  bool hasMuon1;
  bool hasMuon2;
  bool hasdRljet;
  

};

#endif

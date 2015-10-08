

#ifndef DiHiggstollbb_h
#define DiHiggstollbb_h
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

class DiHiggstollbb {
   
   public:
	DiHiggstollbb();
	DiHiggstollbb(TString input_file, TString output_file, int numEvents);
	~DiHiggstollbb();

   public:
	TTree *evtree;
        TChain *chain;
	ExRootTreeReader *treeReader;

   public:
	void initBranches();//create tree and branches
	void init();//create tree and branches
	void writeTree();
	void DiHiggstollbbrun();

   private:

//	void printGenParticle(GenParticle *genP);
	template<class T> void printSortableObject(T *object);
	void insertInJetVector(std::vector<Jet*>& vec, Jet *object);
	void printJet(Jet *jet);
//	void getFinalState(GenParticle* &genp, TClonesArray *branchParticle);

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
 // 	double jetsDeltaR_;//deltaR matching
  	double jetleptonDeltaR_;
  //	double leptonsDeltaR_;//deltaR matching
  //double leptonIso_ = 0.15;
  	double muonPt2_;
  	double muonPt1_;
	double electronPt1_;
	double electronPt2_;
  	double muonsEta_;
	double electronsEta_;
  	double metPt_;
     

   private:
	//tree branch
//create branches 
  float b1jet_px;
  float b1jet_py;
  float b1jet_pz;
  float b1jet_eta;
  float b1jet_phi;
  float b1jet_pt;
  float b1jet_energy;
  float b2jet_px;
  float b2jet_py;
  float b2jet_pz;
  float b2jet_eta;
  float b2jet_phi;
  float b2jet_pt;
  float b2jet_energy;
  bool hasb1jet;
  bool hasb2jet;

  float lepton1_px;
  float lepton1_py;
  float lepton1_pz;
  float lepton1_eta;
  float lepton1_phi;
  float lepton1_pt;
  float lepton1_energy;
  float lepton2_px;
  float lepton2_py;
  float lepton2_pz;
  float lepton2_eta;
  float lepton2_phi;
  float lepton2_pt;
  float lepton2_energy;

  int numbjets;
  int numlepton1;
  int numlepton2;
 
  float dR_b1l1;
  float dR_b1l2;
  float dR_b2l1;
  float dR_b2l2;
  float dR_l1l2;
  float dR_b1b2;
  float mass_l1l2;
  float mass_b1b2;
  float dphi_llbb;

  float met;
  float met_phi;
  float met_px;
  float met_py;

 // float h2tohh_mass;
//additional cuts
  bool hasMET;
  //bool hasGenMET;
  bool hasmuon1;
  bool hasmuon2;
  bool haselectron1;
  bool haselectron2;
  bool hastwogenmuons;
  bool haslepton1;
  bool haslepton2;
  bool hasdRljet;
  

};

#endif

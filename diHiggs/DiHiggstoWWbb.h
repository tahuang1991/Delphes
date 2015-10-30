#ifndef DiHiggstoWWbb_h
#define DiHiggstoWWbb_h
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include "TString.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"

#include "classes/DelphesClasses.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

#include "MMC.h"

class DiHiggstoWWbb {

  public:
    DiHiggstoWWbb();
    DiHiggstoWWbb(TString input_file, TString output_file, std::ifstream& configfile);
    ~DiHiggstoWWbb();

    TTree *evtree;
    TChain *chain;
    ExRootTreeReader *treeReader;
    void writeTree();
    void DiHiggstoWWbbrun();
    void fillbranches();  //only for reco staff


  private:
    TString inputFile;
    TString outputFile;
    int nEvents_;      //Number of events to analyze
    int nStarts_;
    bool simulation_;  //True if you have also GEN info
    int sample_;       //Signal or Background (see next enum)
    enum {Signal = 1, Background = 2};

    //Parameters in config file
    double jetsPt_;
    double jetsEta_;
    double bjetsPt_;
    double bjetsEta_;
    double jetsDeltaR_;//deltaR matching
    double jetleptonDeltaR_;
    double leptonsDeltaR_;//deltaR matching
    double leptonIso_;
    double muonPt2_;
    double muonPt1_;
    double muonsEta_;
    double metPt_;
    //Parameters for MMC
    bool runMMC_;
    bool useRecoMET_;
    bool useRecoMuon_;
    bool useRecoBJet_;
    int iterations_;
    std::string RefPDFfile_;
    bool weightfromonshellnupt_func_;
    bool weightfromonshellnupt_hist_;
    bool weightfromonoffshellWmass_hist_; 
    bool useMET_;
    int bjetrescaleAlgo_; //0.no corrections; 1. simpel rescale, 2. elaborate rescale, -1. ideal case
    //private module to process different steps
    void readConfig(std::ifstream& configfile);
    void getboolpara(std::vector<std::string>& strs, std::string paraname, bool &para, bool def);
    void getstringpara(std::vector<std::string>& strs,std::string paraname, std::string &para, std::string def);
    void getintpara(std::vector<std::string>& strs, std::string paraname, int &para, int def);
    void getdoublepara(std::vector<std::string>& strs, std::string paraname, double &para, double def);
    void calculateNormfactor(int bm=3);  //print the normalization factor
    void initBranches();                 //create tree and branches
    void init();                         //create tree and branches
    void fetchHhhchain(TClonesArray *branchParticle);
    void fetchttbarchain(TClonesArray *branchParticle);
    void getGenMET(TClonesArray *branchGenMET);
    void matchMuon2Gen(TClonesArray *branchMuonBefore,TClonesArray *branchMuon, GenParticle *genmu1, GenParticle *genmu2, float dR);
    void matchBjets2Gen(TClonesArray *branchGenJet, TClonesArray *branchJet, GenParticle *genb1, GenParticle *genb2, float dR);

    //braches used
    TClonesArray *branchParticle;
    TClonesArray *branchMuon;
    TClonesArray *branchMuonBeforeIso;
    TClonesArray *branchJet;
    TClonesArray *branchGenJet;
    TClonesArray *branchMissingET;
    TClonesArray *branchGenMissingET;

    long allEntries;
    std::vector<Jet*> allbjets;  //most energetic one at beginning 
    std::vector<Muon*> allMuon1; //negative charge, most energetic one at beginning  
    std::vector<Muon*> allMuon2; 
    MissingET *Met;
    // With matching
    Jet *b1jet, *b2jet, *genb1jet, *genb2jet;
    Muon *muon1, *muon2;
    // Gen 
    GenParticle *genh2, *genhiggs1, *genhiggs2, *genhtobb, *genhtoWW;
    GenParticle *genb1, *genb2;
    GenParticle *genW1, *genW2, *genmu1, *genmu2, *gennu1, *gennu2;
    GenParticle *gent1, *gent2;
    MissingET *genMet;
    Muon *muon1_beforeIso, *muon2_beforeIso;
    //Lorentz Vector
    TLorentzVector Muon1_p4, Muon2_p4, b1jet_p4, b2jet_p4, Met_p4, totjets_lorentz;
    //Gen Lorentz vector
    TLorentzVector genb1jet_p4, genb2jet_p4; 
    TLorentzVector mu1_p4, mu2_p4, nu1_p4, nu2_p4, b1_p4, b2_p4;
    TLorentzVector genmet_p4;
     
    void printGenParticle(GenParticle *genP);
    void printJet(Jet *jet);
    void getFinalState(GenParticle* &genp, TClonesArray *branchParticle);
    void getQuarkFinalState(GenParticle* &genp, TClonesArray *branchParticle);
    void printAllGenParticles(TClonesArray *branchParticle);
    //tree branches
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
    float htobb_px;
    float htobb_py;
    float htobb_pz;
    float htobb_energy;
    float htobb_mass;
    bool htobb; 

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
    unsigned int b1jet_btag;
    float b2jet_px;
    float b2jet_py;
    float b2jet_pz;
    float b2jet_eta;
    float b2jet_phi;
    float b2jet_pt;
    float b2jet_energy;
    unsigned int b2jet_btag;
    int energeticbjets;
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

    float w1_mass;
    float w1_px;
    float w1_py;
    float w1_pz;
    float w1_energy;
    float w1_eta;
    float w1_phi;
    float w1_pt;
    float w2_mass;
    float w2_px;
    float w2_py;
    float w2_pz;
    float w2_energy;
    float w2_eta;
    float w2_phi;
    float w2_pt;

    bool Wtomu2nu2;
    bool Wtomu1nu1;
    bool htoWW;

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
    bool ttbar;

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

    float htoWW_px;
    float htoWW_py;
    float htoWW_pz;
    float htoWW_energy;
    float htoWW_mass;

    int numbjets;
    int numLeptons1;
    int numLeptons2;
    bool hasRecob1jet;
    bool hasRecob2jet;
    bool hasRecoMuon1;
    bool hasRecoMuon2;
    float dR_bl;
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
    float genmet_diBaxis_p;
    float genmet_diBaxis_t;
    float met_diBaxis_p;
    float met_diBaxis_t;

    float h2tohh_mass;
    //additional cuts
    bool hasMET;
    bool hasGenMET;
    bool hastwomuons;
    bool hastwogenmuons;
    bool hasMuon1;
    bool hasMuon2;
    bool hasdRljet;
    bool h2tohh;
    bool preselections;//check whether event pass preselection or not

    //MMC
    TTree* MMCtree;

    float MMC_h2mass_prob;
    float MMC_h2massweight1_prob;
    float MMC_h2massweight4_prob;
    float MMC_h2mass_Entries;
    float MMC_h2mass_RMS;
    float MMC_h2mass_Mean;
    float MMC_h2mass_underflow;
    float MMC_h2mass_overflow;
};

#endif

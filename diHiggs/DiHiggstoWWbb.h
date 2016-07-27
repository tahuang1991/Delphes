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
#include "TH1F.h"

#include "classes/DelphesClasses.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"
#include "TMVA/Reader.h"
#include "MMC.h"

class DiHiggstoWWbb {

  public:
    DiHiggstoWWbb();
    DiHiggstoWWbb(std::vector<TString> input_file, TString output_file, std::ifstream& configfile);
    ~DiHiggstoWWbb();

    TTree *evtree;
    TChain *chain;
    ExRootTreeReader *treeReader;
    void writeTree();
    void DiHiggstoWWbbrun();
    void fillbranches();  //only for reco staff


  private:
    std::vector<TString> inputFile;
    TString outputFile;
    int nEvents_;      //Number of events to analyze
    int nStarts_;
    bool simulation_;  //True if you have also GEN info
    int sample_;       //Signal or Background (see next enum)
    enum {tt = 0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12};

    //Parameters in config file
    std::string Particlename_;
    std::string Muonname_;
    std::string MuonBeforeIsoname_;
    std::string GenJetNoNuname_;
    std::string GenJetname_;
    std::string Jetname_;
    std::string GenMETname_;
    std::string METname_;
    double jetsPt_;
    double jetsEta_;
    double bjetsPt_;
    double bjetsEta_;
    bool PUSample_;
    bool energeticbjets_;
    double jetsDeltaR_;//deltaR matching
    double jetleptonDeltaR_;
    double leptonsDeltaR_;//deltaR matching
    double leptonIso_;
    double muonPt2_;
    double muonPt1_;
    double muonsEta_;
    double muonsIso_;
    double metPt_;
    float b1rescalefactor;
    std::vector<float> v_bjetrescalec1pdf;
    float b2rescalefactor;
    TH1F *bjetrescalec1pdf;
    //Parameters for MMC
    bool runMVA_;
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
    bool debug_;
    int bjetrescaleAlgo_; //0.no corrections; 1. simpel rescale, 2. elaborate rescale, -1. ideal case
    int metcorrection_; //0.no corrections; 1. simpel rescale, 2. elaborate rescale, -1. ideal case
    //private module to process different steps
    void readConfig(std::ifstream& configfile);
    void getboolpara(std::vector<std::string>& strs, std::string paraname, bool &para, bool def);
    void getstringpara(std::vector<std::string>& strs,std::string paraname, std::string &para, std::string def);
    void getintpara(std::vector<std::string>& strs, std::string paraname, int &para, int def);
    void getdoublepara(std::vector<std::string>& strs, std::string paraname, double &para, double def);
    void calculateNormfactor(int bm=3);  //print the normalization factor
    void initBranches();                 //create tree and branches
    void init();                         //create tree and branches
    void fetchHhhWWWWchain(TClonesArray *branchParticle);
    void fetchHhhchain(TClonesArray *branchParticle);
    void fetchttbarchain(TClonesArray *branchParticle);
    void getGenMET(TClonesArray *branchGenMET);
    void matchMuon2Gen(TClonesArray *branchMuonBefore,TClonesArray *branchMuon, GenParticle *genmu1, GenParticle *genmu2, float dR);
    void checkBjets2Gen(TClonesArray *branchGenJetNoNu, TClonesArray *branchJet, GenParticle *genb1, GenParticle *genb2, float dR);
    void matchBjets2Gen(TClonesArray *branchGenJetNoNu, TClonesArray *branchJet, GenParticle *genb1, GenParticle *genb2);
    void matchBjetswithNu2Gen(TClonesArray *branchGenJet,  GenParticle *genb1, GenParticle *genb2, float dR);
    
    void SlideRescale();
    void GetPDFc1();
    //braches used
    TClonesArray *branchParticle;
    TClonesArray *branchMuon;
    TClonesArray *branchMuonBeforeIso;
    TClonesArray *branchJet;
    TClonesArray *branchGenJet;
    TClonesArray *branchGenJet_withNu;
    TClonesArray *branchMissingET;
    TClonesArray *branchGenMissingET;
    TClonesArray *branchVertex;


  private:
    TFile *file;
    TH2F *MuonRecoEff_hist;
    void readoutMuonRecoEff();
    float GetMuonRecoWeight(TH2F *hist, float pt, float eta);
 
  private:
    long allEntries;
    std::vector<Jet*> alljets;  //most energetic one at beginning 
    std::vector<Muon*> allMuon1; //negative charge, most energetic one at beginning  
    std::vector<Muon*> allMuon2; 
    MissingET *Met;
    // With matching
    Jet *b1jet, *b2jet, *genb1jet, *genb2jet;
    Muon *muon1, *muon2;
    // Gen 
    GenParticle *genh2, *genhiggs1, *genhiggs2, *genhtobb, *genhtoWW;
    GenParticle *genb1, *genb2;
    GenParticle *genfinalb1, *genfinalb2;
    GenParticle *genW1, *genW2, *genmu1, *genmu2, *gennu1, *gennu2;
    GenParticle *gent1, *gent2;
    MissingET *genMet;
    Muon *muon1_beforeIso, *muon2_beforeIso;
    //Lorentz Vector
    TLorentzVector Muon1_p4, Muon2_p4, Muon1_beforeIso_p4, Muon2_beforeIso_p4, b1jet_p4, b2jet_p4, Met_p4, totjets_lorentz;
    //Gen Lorentz vector
    TLorentzVector genb1jet_p4, genb2jet_p4; 
    TLorentzVector mu1_p4, mu2_p4, nu1_p4, nu2_p4, b1_p4, b2_p4;
    TLorentzVector genmet_p4;
    //figure out misssing ET "contituent"
    TLorentzVector nufromb1_p4, nufromb2_p4;
    TLorentzVector genb1jet_withNu_p4, genb2jet_withNu_p4; 
    
    void printGenParticle(GenParticle *genP);
    void printDecendantsParticles(GenParticle* genp, TClonesArray *branchParticle);
    void printJet(Jet *jet);
    void getFinalState(GenParticle* &genp, TClonesArray *branchParticle);
    void getQuarkFinalState(GenParticle* &genp, TClonesArray *branchParticle);
    void printAllGenParticles(TClonesArray *branchParticle);
    TLorentzVector findNeutrinosfromJet(TRefArray particles);
    //tree branches
    int event_n;
    int vertices;
    float Thisweight;
    float Thisweight_pess;
    float weight;
    float weight_pess;
    float reweighting;
    TMVA::Reader *reader;
    TMVA::Reader *reader_ttB1;
    TMVA::Reader *reader_ttB2;
    TMVA::Reader *reader_ttB3;
    TMVA::Reader *reader_ttB4;
    TMVA::Reader *reader_ttB5;
    TMVA::Reader *reader_ttB6;
    TMVA::Reader *reader_ttB7;
    TMVA::Reader *reader_ttB8;
    TMVA::Reader *reader_ttB9;
    TMVA::Reader *reader_ttB10;
    TMVA::Reader *reader_ttB11;
    TMVA::Reader *reader_ttB12;
    TString Method;
    TString Method_ttB1;
    TString Method_ttB2;
    TString Method_ttB3;
    TString Method_ttB4;
    TString Method_ttB5;
    TString Method_ttB6;
    TString Method_ttB7;
    TString Method_ttB8;
    TString Method_ttB9;
    TString Method_ttB10;
    TString Method_ttB11;
    TString Method_ttB12;
    float MVA_dR_l1l2;
    float MVA_dR_b1b2;
    float MVA_dR_bl;
    float MVA_dR_l1l2b1b2;
    float MVA_MINdR_bl;
    float MVA_dphi_l1l2b1b2;
    float MVA_mass_l1l2;
    float MVA_mass_b1b2;
    float MVA_mass_trans;
    float MVA_MT2;
    float MVA_pt_b1b2;
    float MVA_MMC_h2massweight1_prob;
    float MVA_value;
    float MVA_value_B1fortt;
    float MVA_value_B2fortt;
    float MVA_value_B3fortt;
    float MVA_value_B4fortt;
    float MVA_value_B5fortt;
    float MVA_value_B6fortt;
    float MVA_value_B7fortt;
    float MVA_value_B8fortt;
    float MVA_value_B9fortt;
    float MVA_value_B10fortt;
    float MVA_value_B11fortt;
    float MVA_value_B12fortt;
    float MT2;
    float MT2_reco;
    float MT2_noMU;
    float MT2_noMUv2;
    float MT2_noMUv3;
    float MT2_noMU_reco;
    float MT2_noMU_recov2;
    float MT2_noMU_recov3;
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
    float finalb1_eta;
    float finalb1_phi;
    float finalb1_pt;
    float finalb1_energy;
    float finalb2_eta;
    float finalb2_phi;
    float finalb2_pt;
    float finalb2_energy;

    float genb1jet_px;
    float genb1jet_py;
    float genb1jet_pz;
    float genb1jet_eta;
    float genb1jet_phi;
    float genb1jet_pt;
    float genb1jet_energy;
    float genb1jet_mass;
    float genb2jet_px;
    float genb2jet_py;
    float genb2jet_pz;
    float genb2jet_eta;
    float genb2jet_phi;
    float genb2jet_pt;
    float genb2jet_energy;
    float genb2jet_mass;
    float dR_genb1jet;
    float dR_genb2jet;
    float dR_genb1jet_finalb;
    float dR_genb2jet_finalb;
    bool hasgenb1jet;
    bool hasgenb2jet;
    float DE_partonGneJet1;
    float DE_partonGneJet2;

    float genb1jet_withNu_px;
    float genb1jet_withNu_py;
    float genb1jet_withNu_pz;
    float genb1jet_withNu_eta;
    float genb1jet_withNu_phi;
    float genb1jet_withNu_pt;
    float genb1jet_withNu_energy;
    float genb1jet_withNu_mass;
    float genb2jet_withNu_px;
    float genb2jet_withNu_py;
    float genb2jet_withNu_pz;
    float genb2jet_withNu_eta;
    float genb2jet_withNu_phi;
    float genb2jet_withNu_pt;
    float genb2jet_withNu_energy;
    float genb2jet_withNu_mass;
    float dR_genb1jet_withNu;
    float dR_genb2jet_withNu;
    float dR_genb1jet_withNu_finalb;
    float dR_genb2jet_withNu_finalb;
    bool hasgenb1jet_withNu;
    bool hasgenb2jet_withNu;
   
    float nufromb1_px;
    float nufromb1_py;
    float nufromb1_pz;
    float nufromb1_energy;
    float nufromb2_px;
    float nufromb2_py;
    float nufromb2_pz;
    float nufromb2_energy;

    float dR_b1jet;
    float dR_b2jet;
    float b1jet_px;
    float b1jet_py;
    float b1jet_pz;
    float b1jet_eta;
    float b1jet_phi;
    float b1jet_pt;
    float b1jet_energy;
    float b1jet_mass;
    unsigned int b1jet_btag;
    float b2jet_px;
    float b2jet_py;
    float b2jet_pz;
    float b2jet_eta;
    float b2jet_phi;
    float b2jet_pt;
    float b2jet_energy;
    unsigned int b2jet_btag;
    float b2jet_mass;
    int energeticbjets;
    bool hasRECOjet1;
    bool hasRECOjet2;
    bool hasb1jet;
    bool hasb2jet;
    bool hasgenb1;
    bool hasgenb2;
    bool bpartonsOK;

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
    float nu1and2_pt;
    float nu1and2_px;
    float nu1and2_py;
    float nu1and2_diBaxis_p;
    float nu1and2_diBaxis_t;

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
    
    int w1_child_id;
    int w2_child_id;
    int tau1_child_id;
    int tau2_child_id; 
      
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
    float tt_gen_mass;

    float Muon1_beforeIso_px;
    float Muon1_beforeIso_py;
    float Muon1_beforeIso_pz;
    float Muon1_beforeIso_eta;
    float Muon1_beforeIso_phi;
    float Muon1_beforeIso_pt;
    float Muon1_beforeIso_energy;
    float Muon1_beforeIso_IsoVar;
    float Muon2_beforeIso_px;
    float Muon2_beforeIso_py;
    float Muon2_beforeIso_pz;
    float Muon2_beforeIso_eta;
    float Muon2_beforeIso_phi;
    float Muon2_beforeIso_pt;
    float Muon2_beforeIso_energy;
    float Muon2_beforeIso_IsoVar;
    float dR_mu1_beforeIso;
    float dR_mu2_beforeIso;

    bool Muon1_beforeIso_hasgenMu; 
    bool Muon2_beforeIso_hasgenMu; 
    bool Muon1_beforeIso_passIso;
    bool Muon2_beforeIso_passIso; 
    bool hasMuon1_beforeIso;
    bool hasMuon2_beforeIso;
    bool hastwomuons_beforeIso;

    float Muon1_px;
    float Muon1_py;
    float Muon1_pz;
    float Muon1_eta;
    float Muon1_phi;
    float Muon1_pt;
    float Muon1_energy;
    float Muon1_IsoVar;
    float Muon2_px;
    float Muon2_py;
    float Muon2_pz;
    float Muon2_eta;
    float Muon2_phi;
    float Muon2_pt;
    float Muon2_energy;
    float Muon2_IsoVar;
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
    int numjets;
    int numGenJet;
    int numLeptons1;
    int numLeptons2;
    bool hasRecob1jet;
    bool hasRecob2jet;
    bool hasRecob1jetCorr;
    bool hasRecob2jetCorr;
    bool hasRecob1jetPt;
    bool hasRecob2jetPt;
    bool hasRecob1jetPtCorr;
    bool hasRecob2jetPtCorr;
    bool hasRecob1b2jet_bTag;
    bool hasRecob1b2jetPt_bTag;
    float reconMassJet1_PtRes;
    float reconMassJet2_PtRes;
    float reconDRJet1_PtRes;
    float reconDRJet2_PtRes;
    bool hasRecoMuon1;
    bool hasRecoMuon2;


    float dR_genbl;
    float dR_genb1l1;
    float dR_genb1l2;
    float dR_genb2l1;
    float dR_genb2l2;
    float dR_genl1l2;
    float dR_genl1l2b1b2;
    float dphi_genl1l2b1b2;
    float dR_genb1b2;
    float dR_genminbl;
    float mass_genl1l2;
    float energy_genl1l2;
    float pt_genl1l2;
    float phi_genl1l2;
    float eta_genl1l2;
    float mass_genb1b2;
    float energy_genb1b2;
    float pt_genb1b2;
    float phi_genb1b2;
    float eta_genb1b2;
    float dphi_genllbb;
    float dphi_genllmet;
    float mass_gentrans;

    float minMass;
    float dR_bl;
    float dR_b1l1;
    float dR_b1l2;
    float dR_b2l1;
    float dR_b2l2;
    float MINdR_bl;
    float dR_l1l2;
    float dR_l1l2b1b2;
    float dphi_l1l2b1b2;
    float dR_b1b2;
    float dR_minbl;
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
    float met_c1_px;
    float met_c1_py;
    float met_diBaxis_c1_p;
    float met_diBaxis_c1_t;

    bool HhhtoWWWW;
    float allnu_met;

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
    bool preselections_gen;//check whether event pass preselection or not
    bool runMMCok;

    //MMC
    TTree* MMCtree;

    float MMC_h2mass_prob;
    float MMC_h2massweight1_prob;
    float MMC_h2massweight4_prob;
    float MMC_h2mass_Entries;
    float MMC_h2mass_RMS;
    float MMC_h2massweight1_RMS;
    float MMC_h2massweight4_RMS;
    float MMC_h2mass_Mean;
    float MMC_h2mass_underflow;
    float MMC_h2mass_overflow;
};

#endif

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>

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
#include "TMVA/MsgLogger.h"
#include "TMVA/MethodBase.h"
#include "TMVA/RootFinder.h"
#include "TMVA/PDF.h"
#include "TMVA/VariableIdentityTransform.h"
#include "TMVA/VariablePCATransform.h"
#include "TMVA/VariableGaussTransform.h"
#include "TMVA/VariableNormalizeTransform.h"
#include "TMVA/TSpline1.h"
#include "TMVA/Ranking.h"
#include "TMVA/ResultsClassification.h"
#include "TMVA/ResultsMulticlass.h"
#include "TMVA/Factory.h"
#include "TMVA/Reader.h"
#include "TMVA/Tools.h"
#include "TMVA/Config.h"
#include "TMVA/ClassifierFactory.h"
#include "TMVA/MethodCuts.h"
#include "TMVA/MethodCategory.h"
#include "classes/DelphesClasses.h"
#include "DiHiggstoWWbb.h"
#include "MMC.h"
#include "CrossSections_and_BR.h"
#include "mt2_bisect.h"

using namespace std;
using namespace TMVA;

class ExRootTreeReader;
class ExRootResult;
class GenParticle;
class Jet;
class Track;
class Tower;

DiHiggstoWWbb::DiHiggstoWWbb(std::vector<TString> input_File, TString output_File, std::ifstream& config_File)
{
  inputFile = input_File;
  outputFile = output_File;
  readConfig(config_File); // Read parametersconfig.txt
  init();                  // Create branches
  branchParticle      = treeReader->UseBranch(Particlename_.c_str());
  branchMuon          = treeReader->UseBranch(Muonname_.c_str());
  branchMuonBeforeIso = treeReader->UseBranch(MuonBeforeIsoname_.c_str());
  branchJet           = treeReader->UseBranch(Jetname_.c_str());
  branchGenJet        = treeReader->UseBranch(GenJetNoNuname_.c_str());
  branchGenJet_withNu       = treeReader->UseBranch(GenJetname_.c_str());
  branchMissingET     = treeReader->UseBranch(METname_.c_str());
  branchGenMissingET  = treeReader->UseBranch(GenMETname_.c_str());
  branchVertex  = treeReader->UseBranch("Vertex");
  //branchParticle      = treeReader->UseBranch("Particle");
  //branchMuon          = treeReader->UseBranch("Muon");
  //branchMuonBeforeIso = treeReader->UseBranch("MuonBeforeIso");
  //branchJet           = treeReader->UseBranch("Jet");
  //branchGenJet        = treeReader->UseBranch("GenJetNoNu");
  //branchGenJet_withNu       = treeReader->UseBranch("GenJet");
  //branchMissingET     = treeReader->UseBranch("MissingET");
  //TClonesArray *branchTrack = treeReader->UseBranch("Track");
  //TClonesArray *branchTower = treeReader->UseBranch("Tower");
  //TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  //TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
  //TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  allEntries = treeReader->GetEntries();
  cout << "** Chain contains " << allEntries << " events" << endl;
  cout <<" DiHiggstoWWbb gFile get options " << gFile->GetOption() << endl;
  //Compute weight
  CrossSections_and_BR *my_br = new CrossSections_and_BR();
  Thisweight = my_br->GetWeight(300,allEntries,sample_,true);
  Thisweight_pess = my_br->GetWeight(300,allEntries,sample_,false);
  delete my_br;
  TMVA::Tools::Instance();
  reader      = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB1 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB2 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB3 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB4 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB5 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB6 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB7 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB8 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB9 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB10 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB11 = new TMVA::Reader( "V:Color:Silent" );
  reader_ttB12 = new TMVA::Reader( "V:Color:Silent" );
  std::string methods_used[12] = {"BDT","BDTD","BDTD","KNN","BDTD","KNN","BDTD","BDT","BDT","BDT","KNN","KNN"};
  //std::string methods_used[12] = {"BDT","BDTD","BDT","BDT","BDT","BDT","BDTD","MLP","KNN","KNN","KNN","KNN"};
  if(runMVA_){
    TString nameWeight = "";
    TString folder_suffix = "_RECO_1btag_40PU_15Jul__ALL/TMVAClassification_";
    //TString folder_suffix = "_RECO_1btag_40PU_13MayTTMU_MMC__ALL/TMVAClassification_";
    if(sample_==B1)  { nameWeight = "MVAs/weights_B1" + folder_suffix + methods_used[0] + ".weights.xml";   Method = methods_used[0] + " method"; }
    if(sample_==B2)  { nameWeight = "MVAs/weights_B2" + folder_suffix + methods_used[1] + ".weights.xml";   Method = methods_used[1] + " method"; }
    if(sample_==B3)  { nameWeight = "MVAs/weights_B3" + folder_suffix + methods_used[2] + ".weights.xml";   Method = methods_used[2] + " method"; }
    if(sample_==B4)  { nameWeight = "MVAs/weights_B4" + folder_suffix + methods_used[3] + ".weights.xml";   Method = methods_used[3] + " method"; }
    if(sample_==B5)  { nameWeight = "MVAs/weights_B5" + folder_suffix + methods_used[4] + ".weights.xml";   Method = methods_used[4] + " method"; }
    if(sample_==B6)  { nameWeight = "MVAs/weights_B6" + folder_suffix + methods_used[5] + ".weights.xml";   Method = methods_used[5] + " method"; }
    if(sample_==B7)  { nameWeight = "MVAs/weights_B7" + folder_suffix + methods_used[6] + ".weights.xml";   Method = methods_used[6] + " method"; }
    if(sample_==B8)  { nameWeight = "MVAs/weights_B8" + folder_suffix + methods_used[7] + ".weights.xml";   Method = methods_used[7] + " method"; }
    if(sample_==B9)  { nameWeight = "MVAs/weights_B9" + folder_suffix + methods_used[8] + ".weights.xml";   Method = methods_used[8] + " method"; }
    if(sample_==B10) { nameWeight = "MVAs/weights_B10" + folder_suffix + methods_used[9] + ".weights.xml";  Method = methods_used[9] + " method"; }
    if(sample_==B11) { nameWeight = "MVAs/weights_B11" + folder_suffix + methods_used[10] + ".weights.xml"; Method = methods_used[10] + " method"; }
    if(sample_==B12) { nameWeight = "MVAs/weights_B12" + folder_suffix + methods_used[11] + ".weights.xml"; Method = methods_used[11] + " method"; }
    if(sample_!=tt){ reader->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader->AddVariable( "dR_bl",       &MVA_dR_bl); reader->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader->AddVariable( "mass_trans",  &MVA_mass_trans); reader->AddVariable( "MT2",         &MVA_MT2); reader->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);
	//reader->AddVariable( "MMC_h2massweight1_prob",     &MVA_MMC_h2massweight1_prob);
	reader->BookMVA( Method.Data(), nameWeight.Data() );
    }
    reader_ttB1->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB1->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB1->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB1->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB1->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB1->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB1->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB1->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB1->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB1->AddVariable( "MT2",         &MVA_MT2); reader_ttB1->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB1->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB2->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB2->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB2->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB2->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB2->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB2->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB2->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB2->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB2->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB2->AddVariable( "MT2",         &MVA_MT2); reader_ttB2->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB2->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB3->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB3->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB3->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB3->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB3->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB3->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB3->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB3->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB3->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB3->AddVariable( "MT2",         &MVA_MT2); reader_ttB3->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB3->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB4->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB4->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB4->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB4->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB4->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB4->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB4->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB4->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB4->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB4->AddVariable( "MT2",         &MVA_MT2); reader_ttB4->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB4->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB5->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB5->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB5->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB5->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB5->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB5->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB5->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB5->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB5->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB5->AddVariable( "MT2",         &MVA_MT2); reader_ttB5->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB5->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB6->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB6->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB6->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB6->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB6->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB6->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB6->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB6->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB6->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB6->AddVariable( "MT2",         &MVA_MT2); reader_ttB6->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB6->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB7->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB7->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB7->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB7->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB7->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB7->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB7->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB7->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB7->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB7->AddVariable( "MT2",         &MVA_MT2); reader_ttB7->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB7->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB8->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB8->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB8->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB8->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB8->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB8->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB8->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB8->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB8->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB8->AddVariable( "MT2",         &MVA_MT2); reader_ttB8->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB8->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB9->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB9->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB9->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB9->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB9->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB9->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB9->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB9->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB9->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB9->AddVariable( "MT2",         &MVA_MT2); reader_ttB9->AddVariable( "pt_b1b2",     &MVA_pt_b1b2);            //reader_ttB9->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB10->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB10->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB10->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB10->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB10->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB10->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB10->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB10->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB10->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB10->AddVariable( "MT2",         &MVA_MT2); reader_ttB10->AddVariable( "pt_b1b2",     &MVA_pt_b1b2); //reader_ttB10->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB11->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB11->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB11->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB11->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB11->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB11->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB11->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB11->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB11->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB11->AddVariable( "MT2",         &MVA_MT2); reader_ttB11->AddVariable( "pt_b1b2",     &MVA_pt_b1b2); //reader_ttB11->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);
    reader_ttB12->AddVariable( "dR_l1l2",     &MVA_dR_l1l2); reader_ttB12->AddVariable( "dR_b1b2",     &MVA_dR_b1b2); reader_ttB12->AddVariable( "dR_bl",       &MVA_dR_bl); reader_ttB12->AddVariable( "dR_l1l2b1b2", &MVA_dR_l1l2b1b2); reader_ttB12->AddVariable( "MINdR_bl", &MVA_MINdR_bl); reader_ttB12->AddVariable( "dphi_l1l2b1b2", &MVA_dphi_l1l2b1b2); reader_ttB12->AddVariable( "mass_l1l2",   &MVA_mass_l1l2); reader_ttB12->AddVariable( "mass_b1b2",   &MVA_mass_b1b2); reader_ttB12->AddVariable( "mass_trans",  &MVA_mass_trans); reader_ttB12->AddVariable( "MT2",         &MVA_MT2); reader_ttB12->AddVariable( "pt_b1b2",     &MVA_pt_b1b2); //reader_ttB12->AddVariable( "MMC_h2massweight1_prob", &MVA_MMC_h2massweight1_prob);

    if(sample_==tt){
	Method_ttB1  = methods_used[0] + " method"; reader_ttB1->BookMVA( Method_ttB1.Data(), "MVAs/weights_B1" + folder_suffix + methods_used[0] + ".weights.xml" );
	Method_ttB2  = methods_used[1] + " method"; reader_ttB2->BookMVA( Method_ttB2.Data(), "MVAs/weights_B2" + folder_suffix + methods_used[1] + ".weights.xml" );
	Method_ttB3  = methods_used[2] + " method"; reader_ttB3->BookMVA( Method_ttB3.Data(), "MVAs/weights_B3" + folder_suffix + methods_used[2] + ".weights.xml" );
	Method_ttB4  = methods_used[3] + " method"; reader_ttB4->BookMVA( Method_ttB4.Data(), "MVAs/weights_B4" + folder_suffix + methods_used[3] + ".weights.xml" );
	Method_ttB5  = methods_used[4] + " method"; reader_ttB5->BookMVA( Method_ttB5.Data(), "MVAs/weights_B5" + folder_suffix + methods_used[4] + ".weights.xml" );
	Method_ttB6  = methods_used[5] + " method"; reader_ttB6->BookMVA( Method_ttB6.Data(), "MVAs/weights_B6" + folder_suffix + methods_used[5] + ".weights.xml" );
	Method_ttB7  = methods_used[6] + " method"; reader_ttB7->BookMVA( Method_ttB7.Data(), "MVAs/weights_B7" + folder_suffix + methods_used[6] + ".weights.xml" );
	Method_ttB8  = methods_used[7] + " method"; reader_ttB8->BookMVA( Method_ttB8.Data(), "MVAs/weights_B8" + folder_suffix + methods_used[7] + ".weights.xml" );
	Method_ttB9  = methods_used[8] + " method"; reader_ttB9->BookMVA( Method_ttB9.Data(), "MVAs/weights_B9" + folder_suffix + methods_used[8] + ".weights.xml" );
	Method_ttB10 = methods_used[9] + " method"; reader_ttB10->BookMVA( Method_ttB10.Data(), "MVAs/weights_B10" + folder_suffix + methods_used[9] + ".weights.xml" );
	Method_ttB11 = methods_used[10] + " method"; reader_ttB11->BookMVA( Method_ttB11.Data(), "MVAs/weights_B11" + folder_suffix + methods_used[10] + ".weights.xml" );
	Method_ttB12 = methods_used[11] + " method"; reader_ttB12->BookMVA( Method_ttB12.Data(), "MVAs/weights_B12" + folder_suffix + methods_used[11] + ".weights.xml" );
    }
  }

  MuonRecoEff_hist = new TH2F(); 
  MuonRecoEff_hist->SetNameTitle("muonrecoeff","muonrecoeff");
  readoutMuonRecoEff(); 
  if(debug_){
  	std::cout <<" start to print Muon Reco eff "<< std::endl;
  	MuonRecoEff_hist->Print("ALL");
   }
  //TFile* file = new TFile(outputFile,"recreate");

   file = new TFile(outputFile,"recreate");
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
  getintpara(strs, "sample", sample_, B3);
  getboolpara(strs, "simulation", simulation_, true);
  getboolpara(strs, "PUSample", PUSample_, false);
  getstringpara(strs,"Particlename", Particlename_, "Particle");
  getstringpara(strs,"Muonname", Muonname_, "Muon");
  getstringpara(strs,"MuonBeforeIsoname", MuonBeforeIsoname_, "MuonBeforeIso");
  getstringpara(strs,"GenJetNoNuname", GenJetNoNuname_, "GenJetNoNu");
  getstringpara(strs,"GenJetname", GenJetname_, "GenJet");
  getstringpara(strs,"Jetname", Jetname_, "RecoJetPileUpID");//#
  getstringpara(strs,"GenMETname", GenMETname_, "GenMissingET");
  getstringpara(strs,"METname", METname_, "RecoMissingET");//#
  getdoublepara(strs,"jetsPt", jetsPt_, 20.0);
  getdoublepara(strs,"jetsEta", jetsEta_, 5.0);
  getdoublepara(strs,"bjetsPt", bjetsPt_, 30.0);
  getdoublepara(strs,"bjetsEta", bjetsEta_, 2.50);
  getboolpara(strs,"energeticbjets", energeticbjets_,false);
  getdoublepara(strs,"jetleptonDeltaR", jetleptonDeltaR_, .30); //isolation bewteen jet and lepton
  getdoublepara(strs,"jetsDeltaR", jetsDeltaR_, .40);           //deltaR matching
  getdoublepara(strs,"leptonsDeltaR", leptonsDeltaR_, .40);
  getdoublepara(strs,"leptonIso", leptonIso_, 0.15);
  getdoublepara(strs,"muonPt1", muonPt1_, 20.0);
  getdoublepara(strs,"muonPt2", muonPt2_, 20.0);
  getdoublepara(strs,"muonEta", muonsEta_, 2.40);
  getdoublepara(strs,"muonIso", muonsIso_, .30);
  getdoublepara(strs,"metPt", metPt_, 20);
  getboolpara(strs, "runMVA", runMVA_, false);
  getboolpara(strs, "runMMC", runMMC_, false);
  getboolpara(strs, "useRecoMET", useRecoMET_, false);//whether to use reco met or not as MMC input
  getboolpara(strs, "useRecoMuon", useRecoMuon_, false);
  getboolpara(strs, "useRecoBJet", useRecoBJet_, false);
  getintpara(strs, "iterations", iterations_, 1000000);
  getstringpara(strs,"MMCRef", RefPDFfile_, "MMCRefPDF.ROOT");
  getintpara(strs, "bjetrescaleAlgo", bjetrescaleAlgo_, 2);
  getintpara(strs, "metcorrection", metcorrection_, 2);
  getboolpara(strs, "useMET", useMET_, true);
  getboolpara(strs, "weightfromonshellnupt_func", weightfromonshellnupt_func_, false);
  getboolpara(strs, "weightfromonshellnupt_hist", weightfromonshellnupt_hist_, true);
  getboolpara(strs, "weightfromonoffshellWmass_hist", weightfromonoffshellWmass_hist_, true);
  getboolpara(strs, "debug", debug_, false);
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
  for (unsigned int i=0; i<inputFile.size(); i++){
    //std::cout <<"inputfile i "<< i <<" "<< inputFile.at(i) << std::endl;
    chain->Add(inputFile.at(i));
  }
  treeReader = new ExRootTreeReader(chain);
  evtree = new TTree("evtree","event tree");
  evtree->Branch("event_n",&event_n, "event_n/I");
  evtree->Branch("weight",&weight, "weight/F");
  evtree->Branch("weight_pess",&weight_pess, "weight_pess/F");
  evtree->Branch("reweighting",&reweighting, "reweighting/F");
  evtree->Branch("MVA_value",&MVA_value, "MVA_value/F");
  evtree->Branch("MVA_value_B1fortt",&MVA_value_B1fortt, "MVA_value_B1fortt/F");
  evtree->Branch("MVA_value_B2fortt",&MVA_value_B2fortt, "MVA_value_B2fortt/F");
  evtree->Branch("MVA_value_B3fortt",&MVA_value_B3fortt, "MVA_value_B3fortt/F");
  evtree->Branch("MVA_value_B4fortt",&MVA_value_B4fortt, "MVA_value_B4fortt/F");
  evtree->Branch("MVA_value_B5fortt",&MVA_value_B5fortt, "MVA_value_B5fortt/F");
  evtree->Branch("MVA_value_B6fortt",&MVA_value_B6fortt, "MVA_value_B6fortt/F");
  evtree->Branch("MVA_value_B7fortt",&MVA_value_B7fortt, "MVA_value_B7fortt/F");
  evtree->Branch("MVA_value_B8fortt",&MVA_value_B8fortt, "MVA_value_B8fortt/F");
  evtree->Branch("MVA_value_B9fortt",&MVA_value_B9fortt, "MVA_value_B9fortt/F");
  evtree->Branch("MVA_value_B10fortt",&MVA_value_B10fortt, "MVA_value_B10fortt/F");
  evtree->Branch("MVA_value_B11fortt",&MVA_value_B11fortt, "MVA_value_B11fortt/F");
  evtree->Branch("MVA_value_B12fortt",&MVA_value_B12fortt, "MVA_value_B12fortt/F");
  evtree->Branch("MT2",&MT2, "MT2/F");
  evtree->Branch("MT2_reco",&MT2_reco, "MT2_reco/F");
  evtree->Branch("MT2_noMU",&MT2_noMU, "MT2_noMU/F");
  evtree->Branch("MT2_noMUv2",&MT2_noMUv2, "MT2_noMUv2/F");
  evtree->Branch("MT2_noMUv3",&MT2_noMUv3, "MT2_noMUv3/F");
  evtree->Branch("MT2_noMU_reco",&MT2_noMU_reco, "MT2_noMU_reco/F");
  evtree->Branch("MT2_noMU_recov2",&MT2_noMU_recov2, "MT2_noMU_recov2/F");
  evtree->Branch("MT2_noMU_recov3",&MT2_noMU_recov3, "MT2_noMU_recov3/F");
  evtree->Branch("vertices",&vertices, "vertices/I");
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
  evtree->Branch("finalb1_eta",&finalb1_eta, "finalb1_eta/F");
  evtree->Branch("finalb1_phi",&finalb1_phi, "finalb1_phi/F");
  evtree->Branch("finalb1_pt",&finalb1_pt, "finalb1_pt/F");
  evtree->Branch("finalb1_energy",&finalb1_energy, "finalb1_energy/F");
  evtree->Branch("finalb2_eta",&finalb2_eta, "finalb2_eta/F");
  evtree->Branch("finalb2_phi",&finalb2_phi, "finalb2_phi/F");
  evtree->Branch("finalb2_pt",&finalb2_pt, "finalb2_pt/F");
  evtree->Branch("finalb2_energy",&finalb2_energy, "finalb2_energy/F");

  evtree->Branch("genb1jet_px",&genb1jet_px, "genb1jet_px/F");
  evtree->Branch("genb1jet_py",&genb1jet_py, "genb1jet_py/F");
  evtree->Branch("genb1jet_pz",&genb1jet_pz, "genb1jet_pz/F");
  evtree->Branch("genb1jet_eta",&genb1jet_eta, "genb1jet_eta/F");
  evtree->Branch("genb1jet_phi",&genb1jet_phi, "genb1jet_phi/F");
  evtree->Branch("genb1jet_pt",&genb1jet_pt, "genb1jet_pt/F");
  evtree->Branch("genb1jet_energy",&genb1jet_energy, "genb1jet_energy/F");
  evtree->Branch("genb1jet_mass",&genb1jet_mass, "genb1jet_mass/F");
  evtree->Branch("genb2jet_px",&genb2jet_px, "genb2jet_px/F");
  evtree->Branch("genb2jet_py",&genb2jet_py, "genb2jet_py/F");
  evtree->Branch("genb2jet_pz",&genb2jet_pz, "genb2jet_pz/F");
  evtree->Branch("genb2jet_eta",&genb2jet_eta, "genb2jet_eta/F");
  evtree->Branch("genb2jet_phi",&genb2jet_phi, "genb2jet_phi/F");
  evtree->Branch("genb2jet_pt",&genb2jet_pt, "genb2jet_pt/F");
  evtree->Branch("genb2jet_energy",&genb2jet_energy, "genb2jet_energy/F");
  evtree->Branch("genb2jet_mass",&genb2jet_mass, "genb2jet_mass/F");
  evtree->Branch("dR_genb1jet", &dR_genb1jet,"dR_genb1jet/F");  
  evtree->Branch("dR_genb2jet", &dR_genb2jet,"dR_genb2jet/F");  
  evtree->Branch("dR_genb1jet_finalb", &dR_genb1jet_finalb,"dR_genb1jet_finalb/F");  
  evtree->Branch("dR_genb2jet_finalb", &dR_genb2jet_finalb,"dR_genb2jet_finalb/F");  
  evtree->Branch("hasgenb1jet",&hasgenb1jet, "hasgenb1jet/B");
  evtree->Branch("hasgenb2jet",&hasgenb2jet, "hasgenb2jet/B");
  evtree->Branch("DE_partonGneJet1",&DE_partonGneJet1, "DE_partonGneJet1/F");
  evtree->Branch("DE_partonGneJet2",&DE_partonGneJet2, "DE_partonGneJet2/F");


  evtree->Branch("genb1jet_withNu_px",&genb1jet_withNu_px, "genb1jet_withNu_px/F");
  evtree->Branch("genb1jet_withNu_py",&genb1jet_withNu_py, "genb1jet_withNu_py/F");
  evtree->Branch("genb1jet_withNu_pz",&genb1jet_withNu_pz, "genb1jet_withNu_pz/F");
  evtree->Branch("genb1jet_withNu_eta",&genb1jet_withNu_eta, "genb1jet_withNu_eta/F");
  evtree->Branch("genb1jet_withNu_phi",&genb1jet_withNu_phi, "genb1jet_withNu_phi/F");
  evtree->Branch("genb1jet_withNu_pt",&genb1jet_withNu_pt, "genb1jet_withNu_pt/F");
  evtree->Branch("genb1jet_withNu_energy",&genb1jet_withNu_energy, "genb1jet_withNu_energy/F");
  evtree->Branch("genb2jet_withNu_px",&genb2jet_withNu_px, "genb2jet_withNu_px/F");
  evtree->Branch("genb2jet_withNu_py",&genb2jet_withNu_py, "genb2jet_withNu_py/F");
  evtree->Branch("genb2jet_withNu_pz",&genb2jet_withNu_pz, "genb2jet_withNu_pz/F");
  evtree->Branch("genb2jet_withNu_eta",&genb2jet_withNu_eta, "genb2jet_withNu_eta/F");
  evtree->Branch("genb2jet_withNu_phi",&genb2jet_withNu_phi, "genb2jet_withNu_phi/F");
  evtree->Branch("genb2jet_withNu_pt",&genb2jet_withNu_pt, "genb2jet_withNu_pt/F");
  evtree->Branch("genb2jet_withNu_energy",&genb2jet_withNu_energy, "genb2jet_withNu_energy/F");
  evtree->Branch("dR_genb1jet_withNu", &dR_genb1jet_withNu,"dR_genb1jet_withNu/F");  
  evtree->Branch("dR_genb2jet_withNu", &dR_genb2jet_withNu,"dR_genb2jet_withNu/F");  
  evtree->Branch("dR_genb1jet_withNu_finalb", &dR_genb1jet_withNu_finalb,"dR_genb1jet_withNu_finalb/F");  
  evtree->Branch("dR_genb2jet_withNu_finalb", &dR_genb2jet_withNu_finalb,"dR_genb2jet_withNu_finalb/F");  
  evtree->Branch("hasgenb1jet_withNu",&hasgenb1jet_withNu, "hasgenb1jet_withNu/B");
  evtree->Branch("hasgenb2jet_withNu",&hasgenb2jet_withNu, "hasgenb2jet_withNu/B");

  evtree->Branch("nufromb1_px",&nufromb1_px, "nufromb1_px/F");
  evtree->Branch("nufromb1_py",&nufromb1_py, "nufromb1_py/F");
  evtree->Branch("nufromb1_pz",&nufromb1_pz, "nufromb1_pz/F");
  evtree->Branch("nufromb1_energy",&nufromb1_energy, "nufromb1_energy/F");
  evtree->Branch("nufromb2_px",&nufromb2_px, "nufromb2_px/F");
  evtree->Branch("nufromb2_py",&nufromb2_py, "nufromb2_py/F");
  evtree->Branch("nufromb2_pz",&nufromb2_pz, "nufromb2_pz/F");
  evtree->Branch("nufromb2_energy",&nufromb2_energy, "nufromb2_energy/F");

  evtree->Branch("b1jet_px",&b1jet_px, "b1jet_px/F");
  evtree->Branch("b1jet_py",&b1jet_py, "b1jet_py/F");
  evtree->Branch("b1jet_pz",&b1jet_pz, "b1jet_pz/F");
  evtree->Branch("b1jet_eta",&b1jet_eta, "b1jet_eta/F");
  evtree->Branch("b1jet_phi",&b1jet_phi, "b1jet_phi/F");
  evtree->Branch("b1jet_pt",&b1jet_pt, "b1jet_pt/F");
  evtree->Branch("b1jet_energy",&b1jet_energy, "b1jet_energy/F");
  evtree->Branch("b1jet_mass",&b1jet_mass, "b1jet_mass/F");
  evtree->Branch("b1jet_btag",&b1jet_btag, "b1jet_btag/i");//unsigned int
  evtree->Branch("b2jet_px",&b2jet_px, "b2jet_px/F");
  evtree->Branch("b2jet_py",&b2jet_py, "b2jet_py/F");
  evtree->Branch("b2jet_pz",&b2jet_pz, "b2jet_pz/F");
  evtree->Branch("b2jet_eta",&b2jet_eta, "b2jet_eta/F");
  evtree->Branch("b2jet_phi",&b2jet_phi, "b2jet_phi/F");
  evtree->Branch("b2jet_pt",&b2jet_pt, "b2jet_pt/F");
  evtree->Branch("b2jet_energy",&b2jet_energy, "b2jet_energy/F");
  evtree->Branch("b2jet_mass",&b2jet_mass, "b2jet_mass/F");
  evtree->Branch("b2jet_btag",&b2jet_btag, "b2jet_btag/i");
  evtree->Branch("energeticbjets",&energeticbjets, "energeticbjets/I");
  evtree->Branch("dR_b1jet", &dR_b1jet,"dR_b1jet/F");  
  evtree->Branch("dR_b2jet", &dR_b2jet,"dR_b2jet/F");  

  evtree->Branch("hasb1jet",&hasb1jet, "hasb1jet/B");
  evtree->Branch("hasb2jet",&hasb2jet, "hasb2jet/B");
  evtree->Branch("hasgenb1",&hasgenb1, "hasgenb1/B");
  evtree->Branch("hasgenb2",&hasgenb2, "hasgenb2/B");
  evtree->Branch("hasRECOjet1",&hasRECOjet1, "hasRECOjet1/B");
  evtree->Branch("hasRECOjet2",&hasRECOjet2, "hasRECOjet2/B");
  evtree->Branch("bpartonsOK",&bpartonsOK, "bpartonsOK/B");
  evtree->Branch("hasRecob1jet",&hasRecob1jet, "hasRecob1jet/B");
  evtree->Branch("hasRecob2jet",&hasRecob2jet, "hasRecob2jet/B");
  evtree->Branch("hasRecob1jetCorr",&hasRecob1jetCorr, "hasRecob1jetCorr/B");
  evtree->Branch("hasRecob2jetCorr",&hasRecob2jetCorr, "hasRecob2jetCorr/B");
  evtree->Branch("hasRecob1jetPt",&hasRecob1jetPt, "hasRecob1jetPt/B");
  evtree->Branch("hasRecob2jetPt",&hasRecob2jetPt, "hasRecob2jetPt/B");
  evtree->Branch("hasRecob1jetPtCorr",&hasRecob1jetPtCorr, "hasRecob1jetPtCorr/B");
  evtree->Branch("hasRecob2jetPtCorr",&hasRecob2jetPtCorr, "hasRecob2jetPtCorr/B");
  evtree->Branch("hasRecob1b2jet_bTag",&hasRecob1b2jet_bTag, "hasRecob1b2jet_bTag/B");
  evtree->Branch("hasRecob1b2jetPt_bTag",&hasRecob1b2jetPt_bTag, "hasRecob1b2jetPt_bTag/B");
  evtree->Branch("reconMassJet1_PtRes",&reconMassJet1_PtRes, "reconMassJet1_PtRes/F");
  evtree->Branch("reconMassJet2_PtRes",&reconMassJet2_PtRes, "reconMassJet2_PtRes/F");
  evtree->Branch("reconDRJet1_PtRes",&reconDRJet1_PtRes, "reconDRJet1_PtRes/F");
  evtree->Branch("reconDRJet2_PtRes",&reconDRJet2_PtRes, "reconDRJet2_PtRes/F");
  evtree->Branch("hastwogenmuons",&hastwogenmuons, "hastwogenmuons/B");
  evtree->Branch("hastwomuons",&hastwomuons, "hastwomuons/B");
  evtree->Branch("minMass",&minMass, "minMass/F");

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
  evtree->Branch("nu1and2_pt",&nu1and2_pt, "nu1and2_pt/F");
  evtree->Branch("nu1and2_px",&nu1and2_px, "nu1and2_px/F");
  evtree->Branch("nu1and2_py",&nu1and2_py, "nu1and2_py/F");
  evtree->Branch("nu1and2_diBaxis_p",&nu1and2_diBaxis_p, "nu1and2_diBaxis_p/F");
  evtree->Branch("nu1and2_diBaxis_t",&nu1and2_diBaxis_t, "nu1and2_diBaxis_t/F");

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
  evtree->Branch("w1_child_id",&w1_child_id, "w1_child_id/I");
  evtree->Branch("w2_child_id",&w2_child_id, "w2_child_id/I");
  evtree->Branch("tau1_child_id",&tau1_child_id, "tau1_child_id/I");
  evtree->Branch("tau2_child_id",&tau2_child_id, "tau2_child_id/I");

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
  evtree->Branch("tt_gen_mass",&tt_gen_mass,"tt_gen_mass/F");

  evtree->Branch("Muon1_beforeIso_px",&Muon1_beforeIso_px, "Muon1_beforeIso_px/F");
  evtree->Branch("Muon1_beforeIso_py",&Muon1_beforeIso_py, "Muon1_beforeIso_py/F");
  evtree->Branch("Muon1_beforeIso_pz",&Muon1_beforeIso_pz, "Muon1_beforeIso_pz/F");
  evtree->Branch("Muon1_beforeIso_eta",&Muon1_beforeIso_eta, "Muon1_beforeIso_eta/F");
  evtree->Branch("Muon1_beforeIso_phi",&Muon1_beforeIso_phi, "Muon1_beforeIso_phi/F");
  evtree->Branch("Muon1_beforeIso_pt",&Muon1_beforeIso_pt, "Muon1_beforeIso_pt/F");
  evtree->Branch("Muon1_beforeIso_energy",&Muon1_beforeIso_energy, "Muon1_beforeIso_energy/F");
  evtree->Branch("Muon1_beforeIso_IsoVar",&Muon1_beforeIso_IsoVar, "Muon1_beforeIso_IsoVar/F");
  evtree->Branch("Muon2_beforeIso_px",&Muon2_beforeIso_px, "Muon2_beforeIso_px/F");
  evtree->Branch("Muon2_beforeIso_py",&Muon2_beforeIso_py, "Muon2_beforeIso_py/F");
  evtree->Branch("Muon2_beforeIso_pz",&Muon2_beforeIso_pz, "Muon2_beforeIso_pz/F");
  evtree->Branch("Muon2_beforeIso_eta",&Muon2_beforeIso_eta, "Muon2_beforeIso_eta/F");
  evtree->Branch("Muon2_beforeIso_phi",&Muon2_beforeIso_phi, "Muon2_beforeIso_phi/F");
  evtree->Branch("Muon2_beforeIso_pt",&Muon2_beforeIso_pt, "Muon2_beforeIso_pt/F");
  evtree->Branch("Muon2_beforeIso_energy",&Muon2_beforeIso_energy, "Muon2_beforeIso_energy/F");
  evtree->Branch("Muon2_beforeIso_IsoVar",&Muon2_beforeIso_IsoVar, "Muon2_beforeIso_IsoVar/F");
  evtree->Branch("dR_mu1_beforeIso",&dR_mu1_beforeIso, "dR_mu1_beforeIso/F");
  evtree->Branch("dR_mu2_beforeIso",&dR_mu2_beforeIso, "dR_mu2_beforeIso/F");
  evtree->Branch("hasMuon1_beforeIso",&hasMuon1_beforeIso, "hasMuon1_beforeIso/B");
  evtree->Branch("hasMuon2_beforeIso",&hasMuon2_beforeIso, "hasMuon2_beforeIso/B");
  evtree->Branch("Muon1_beforeIso_hasgenMu",&Muon1_beforeIso_hasgenMu, "Muon1_beforeIso_hasgenMu/B");
  evtree->Branch("Muon2_beforeIso_hasgenMu",&Muon2_beforeIso_hasgenMu, "Muon2_beforeIso_hasgenMu/B");
  evtree->Branch("Muon1_beforeIso_passIso",&Muon1_beforeIso_passIso, "Muon1_beforeIso_passIso/B");
  evtree->Branch("Muon2_beforeIso_passIso",&Muon2_beforeIso_passIso, "Muon2_beforeIso_passIso/B");
  evtree->Branch("hastwomuons_beforeIso",&hastwomuons_beforeIso, "hastwomuons_beforeIso/B");

  evtree->Branch("Muon1_px",&Muon1_px, "Muon1_px/F");
  evtree->Branch("Muon1_py",&Muon1_py, "Muon1_py/F");
  evtree->Branch("Muon1_pz",&Muon1_pz, "Muon1_pz/F");
  evtree->Branch("Muon1_eta",&Muon1_eta, "Muon1_eta/F");
  evtree->Branch("Muon1_phi",&Muon1_phi, "Muon1_phi/F");
  evtree->Branch("Muon1_pt",&Muon1_pt, "Muon1_pt/F");
  evtree->Branch("Muon1_energy",&Muon1_energy, "Muon1_energy/F");
  evtree->Branch("Muon1_IsoVar",&Muon1_IsoVar, "Muon1_IsoVar/F");
  evtree->Branch("Muon2_px",&Muon2_px, "Muon2_px/F");
  evtree->Branch("Muon2_py",&Muon2_py, "Muon2_py/F");
  evtree->Branch("Muon2_pz",&Muon2_pz, "Muon2_pz/F");
  evtree->Branch("Muon2_eta",&Muon2_eta, "Muon2_eta/F");
  evtree->Branch("Muon2_phi",&Muon2_phi, "Muon2_phi/F");
  evtree->Branch("Muon2_pt",&Muon2_pt, "Muon2_pt/F");
  evtree->Branch("Muon2_energy",&Muon2_energy, "Muon2_energy/F");
  evtree->Branch("Muon2_IsoVar",&Muon2_IsoVar, "Muon2_IsoVar/F");
  evtree->Branch("energeticMuon1",&energeticMuon1, "energeticMuon1/B");
  evtree->Branch("energeticMuon2",&energeticMuon2, "energeticMuon2/B");
  evtree->Branch("dR_mu1",&dR_mu1, "dR_mu1/F");
  evtree->Branch("dR_mu2",&dR_mu2, "dR_mu2/F");
  evtree->Branch("hasMuon1",&hasMuon1, "hasMuon1/B");
  evtree->Branch("hasMuon2",&hasMuon2, "hasMuon2/B");
  evtree->Branch("hasRecoMuon1",&hasRecoMuon1, "hasRecoMuon1/B");
  evtree->Branch("hasRecoMuon2",&hasRecoMuon2, "hasRecoMuon2/B");
  evtree->Branch("Muon1_hasgenMu",&Muon1_hasgenMu, "Muon1_hasgenMu/B");
  evtree->Branch("Muon2_hasgenMu",&Muon2_hasgenMu, "Muon2_hasgenMu/B");

  evtree->Branch("numbjets",&numbjets, "numbjets/I");
  evtree->Branch("numjets",&numjets, "numjets/I");
  evtree->Branch("numGenJet",&numGenJet, "numGenJet/I");
  evtree->Branch("numLeptons1",&numLeptons1, "numLeptons1/I");
  evtree->Branch("numLeptons2",&numLeptons2, "numLeptons2/I");

  evtree->Branch("dR_genbl",&dR_genbl, "dR_genbl/F");
  evtree->Branch("dR_genb1l1",&dR_genb1l1, "dR_genb1l1/F");
  evtree->Branch("dR_genb1l2",&dR_genb1l2, "dR_genb1l2/F");
  evtree->Branch("dR_genb2l1",&dR_genb2l1, "dR_genb2l1/F");
  evtree->Branch("dR_genb2l2",&dR_genb2l2, "dR_genb2l2/F");
  evtree->Branch("dR_genb1b2",&dR_genb1b2, "dR_genb1b2/F");
  evtree->Branch("dR_genl1l2",&dR_genl1l2, "dR_genl1l2/F");
  evtree->Branch("dR_genl1l2b1b2",&dR_genl1l2b1b2, "dR_genl1l2b1b2/F");
  evtree->Branch("dphi_genl1l2b1b2",&dphi_genl1l2b1b2, "dphi_genl1l2b1b2/F");
  evtree->Branch("dR_genminbl",&dR_genminbl, "dR_genminbl/F");
  evtree->Branch("mass_genb1b2",&mass_genb1b2, "mass_genb1b2/F");
  evtree->Branch("energy_genb1b2",&energy_genb1b2, "energy_genb1b2/F");
  evtree->Branch("pt_genb1b2",&pt_genb1b2, "pt_genb1b2/F");
  evtree->Branch("phi_genb1b2",&phi_genb1b2, "phi_genb1b2/F");
  evtree->Branch("eta_genb1b2",&eta_genb1b2, "eta_genb1b2/F");
  evtree->Branch("mass_genl1l2",&mass_genl1l2, "mass_genl1l2/F");
  evtree->Branch("energy_genl1l2",&energy_genl1l2, "energy_genl1l2/F");
  evtree->Branch("pt_genl1l2",&pt_genl1l2, "pt_genl1l2/F");
  evtree->Branch("phi_genl1l2",&phi_genl1l2, "phi_genl1l2/F");
  evtree->Branch("eta_genl1l2",&eta_genl1l2, "eta_genl1l2/F");
  evtree->Branch("dphi_genllbb",&dphi_genllbb, "dphi_genllbb/F");
  evtree->Branch("dphi_genllmet",&dphi_genllmet, "dphi_genllmet/F");
  evtree->Branch("mass_gentrans",&mass_gentrans, "mass_gentrans/F");

  evtree->Branch("dR_bl",&dR_bl, "dR_bl/F");
  evtree->Branch("dR_b1l1",&dR_b1l1, "dR_b1l1/F");
  evtree->Branch("dR_b1l2",&dR_b1l2, "dR_b1l2/F");
  evtree->Branch("dR_b2l1",&dR_b2l1, "dR_b2l1/F");
  evtree->Branch("dR_b2l2",&dR_b2l2, "dR_b2l2/F");
  evtree->Branch("dR_b1b2",&dR_b1b2, "dR_b1b2/F");
  evtree->Branch("MINdR_bl",&MINdR_bl, "MINdR_bl/F");
  evtree->Branch("dR_l1l2",&dR_l1l2, "dR_l1l2/F");
  evtree->Branch("dR_l1l2b1b2",&dR_l1l2b1b2, "dR_l1l2b1b2/F");
  evtree->Branch("dphi_l1l2b1b2",&dphi_l1l2b1b2, "dphi_l1l2b1b2/F");
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
  evtree->Branch("met_c1_px",&met_c1_px,"met_c1_px/F");
  evtree->Branch("met_c1_py",&met_c1_py,"met_c1_py/F");
  evtree->Branch("met_diBaxis_c1_p",&met_diBaxis_c1_p,"met_diBaxis_c1_p/F");
  evtree->Branch("met_diBaxis_c1_t",&met_diBaxis_c1_t,"met_diBaxis_c1_t/F");

  evtree->Branch("HhhtoWWWW",&HhhtoWWWW, "HhhtoWWWW/B");
  evtree->Branch("allnu_met",&allnu_met, "allnu_met/F");

  evtree->Branch("hasGenMET",&hasGenMET, "hasGenMET/B");
  evtree->Branch("hasMET",&hasMET, "hasMET/B");
  evtree->Branch("hasdRljet",&hasdRljet, "hasdRljet/B");
  evtree->Branch("h2tohh_mass",&h2tohh_mass,"h2tohh_mass/F");
  evtree->Branch("h2tohh",&h2tohh,"h2tohh/B");
  evtree->Branch("preselections",&preselections, "preselections/B");
  evtree->Branch("preselections_gen",&preselections_gen, "preselections_gen/B");
  evtree->Branch("runMMCok",&runMMCok, "runMMCok/B");

  evtree->Branch("MMC_h2mass_prob",&MMC_h2mass_prob,"MMC_h2mass_prob/F");
  evtree->Branch("MMC_h2massweight1_prob",&MMC_h2massweight1_prob,"MMC_h2massweight1_prob/F");
  evtree->Branch("MMC_h2massweight4_prob",&MMC_h2massweight4_prob,"MMC_h2massweight4_prob/F");
  evtree->Branch("MMC_h2mass_RMS",&MMC_h2mass_RMS,"MMC_h2mass_RMS/F");
  evtree->Branch("MMC_h2massweight1_RMS",&MMC_h2massweight1_RMS,"MMC_h2massweight1_RMS/F");
  evtree->Branch("MMC_h2massweight4_RMS",&MMC_h2massweight4_RMS,"MMC_h2massweight4_RMS/F");
  evtree->Branch("MMC_h2mass_Mean",&MMC_h2mass_Mean,"MMC_h2mass_Mean/F");
  evtree->Branch("MMC_h2mass_Entries",&MMC_h2mass_Entries,"MMC_h2mass_Entries/F");
  evtree->Branch("MMC_h2mass_overflow",&MMC_h2mass_overflow,"MMC_h2mass_overflow/F");
  evtree->Branch("MMC_h2mass_underflow",&MMC_h2mass_underflow,"MMC_h2mass_overflow/F");
  evtree->Branch("MMC_h2mass_MaxBin",&MMC_h2mass_MaxBin,"MMC_h2mass_MaxBin/F");
  evtree->Branch("MMC_h2mass_weight1_MaxBin",&MMC_h2mass_weight1_MaxBin,"MMC_h2mass_weight1_MaxBin/F");
  evtree->Branch("MMC_h2mass_weight4_MaxBin",&MMC_h2mass_weight4_MaxBin,"MMC_h2mass_weight4_MaxBin/F");
  GetPDFc1();
}

void DiHiggstoWWbb::writeTree(){
  //evtree->Write();
  file->WriteObject(evtree, evtree->GetName());
  file->Close();
  calculateNormfactor(3);
}

DiHiggstoWWbb::~DiHiggstoWWbb(){
  cout << "Calling destructor." << endl;
  delete treeReader;
  delete chain;
  delete evtree;
  delete reader;
  delete reader_ttB1;
  delete reader_ttB2;
  delete reader_ttB3;
  delete reader_ttB4;
  delete reader_ttB5;
  delete reader_ttB6;
  delete reader_ttB7;
  delete reader_ttB8;
  delete reader_ttB9;
  delete reader_ttB10;
  delete reader_ttB11;
  delete reader_ttB12;
}


// ------------ To find H->hh->WWWW->lvlvlvlv  ------------
void DiHiggstoWWbb::fetchHhhWWWWchain(TClonesArray *branchParticle){

  GenParticle *genh2,*genhiggs1,*genhiggs2;

  genh2 =  (GenParticle*) branchParticle->At(0);
  for (int i=0; i<branchParticle->GetEntriesFast(); i++){
    genh2 =  (GenParticle*) branchParticle->At(i);
    //std::cout <<" i "<< i << " genp pid "<< genh2->PID << std::endl;
    if (genh2->PID == 99926 or genh2->PID == 99927) 
	break;
  }
  //printGenParticle(genP);
  if (genh2->PID != 99926 and genh2->PID != 99927) {
    //cout <<"in HhhWWWW first genparticle is not 99926 nor 99927 " << endl; 
    //printGenParticle(genh2);
    return;
  }
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
  if (abs(((GenParticle*)branchParticle->At(genhiggs1->D1))->PID) ==24 && 
	abs(((GenParticle*)branchParticle->At(genhiggs1->D2))->PID) ==24 &&
	abs(((GenParticle*)branchParticle->At(genhiggs2->D1))->PID) ==24 &&
	abs(((GenParticle*)branchParticle->At(genhiggs2->D2))->PID) ==24) {
    //printGenParticle(genhiggs2);
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D1));
    //printGenParticle((GenParticle*)branchParticle->At(genhiggs2->D2));
    HhhtoWWWW = true;
  }
  if (HhhtoWWWW){
    GenParticle *genW1,*genW2,*genW3,*genW4,*gennu1,*gennu2,*gennu3,*gennu4;
    genW1=0; genW2=0; genW3=0; genW4=0; gennu1=0; gennu2=0; gennu3=0; gennu4=0;
    genW1 = (GenParticle*) branchParticle->At(genhiggs1->D1);
    genW2 = (GenParticle*) branchParticle->At(genhiggs1->D2);
    genW3 = (GenParticle*) branchParticle->At(genhiggs2->D1);
    genW4 = (GenParticle*) branchParticle->At(genhiggs2->D2);
    getFinalState(genW1, branchParticle);
    getFinalState(genW2, branchParticle);
    getFinalState(genW3, branchParticle);
    getFinalState(genW4, branchParticle);
    /*printGenParticle(genh2);
	printGenParticle(genhiggs1);
	printGenParticle(genhiggs2);
	printGenParticle(genW1);
	printGenParticle(genW2);
	printGenParticle(genW3);
	printGenParticle(genW4);
     */
    if (genW1->D1>0 && abs(((GenParticle*)branchParticle->At(genW1->D1))->PID) == 14)
	gennu1 = (GenParticle*)branchParticle->At(genW1->D1); 
    else if (genW1->D2>0 && abs(((GenParticle*)branchParticle->At(genW1->D2))->PID) == 14)
	gennu1 = (GenParticle*)branchParticle->At(genW1->D2);
    if (genW1->D1>0 && abs(((GenParticle*)branchParticle->At(genW1->D1))->PID) == 12)
	gennu1 = (GenParticle*)branchParticle->At(genW1->D1); 
    else if (genW1->D2>0 && abs(((GenParticle*)branchParticle->At(genW1->D2))->PID) == 12)
	gennu1 = (GenParticle*)branchParticle->At(genW1->D2);
    if (genW2->D1>0 && abs(((GenParticle*)branchParticle->At(genW2->D1))->PID) == 14)
	gennu2 = (GenParticle*)branchParticle->At(genW2->D1); 
    else if (genW2->D2>0 && abs(((GenParticle*)branchParticle->At(genW2->D2))->PID) == 14)
	gennu2 = (GenParticle*)branchParticle->At(genW2->D2);
    if (genW2->D1>0 && abs(((GenParticle*)branchParticle->At(genW2->D1))->PID) == 12)
	gennu2 = (GenParticle*)branchParticle->At(genW2->D1); 
    else if (genW2->D2>0 && abs(((GenParticle*)branchParticle->At(genW2->D2))->PID) == 12)
	gennu2 = (GenParticle*)branchParticle->At(genW2->D2);
    if (genW3->D1>0 && abs(((GenParticle*)branchParticle->At(genW3->D1))->PID) == 14)
	gennu3 = (GenParticle*)branchParticle->At(genW3->D1); 
    else if (genW3->D2>0 && abs(((GenParticle*)branchParticle->At(genW3->D2))->PID) == 14)
	gennu3 = (GenParticle*)branchParticle->At(genW3->D2);
    if (genW3->D1>0 && abs(((GenParticle*)branchParticle->At(genW3->D1))->PID) == 12)
	gennu3 = (GenParticle*)branchParticle->At(genW3->D1); 
    else if (genW3->D2>0 && abs(((GenParticle*)branchParticle->At(genW3->D2))->PID) == 12)
	gennu3 = (GenParticle*)branchParticle->At(genW3->D2);
    if (genW4->D1>0 && abs(((GenParticle*)branchParticle->At(genW4->D1))->PID) == 14)
	gennu4 = (GenParticle*)branchParticle->At(genW4->D1); 
    else if (genW4->D2>0 && abs(((GenParticle*)branchParticle->At(genW4->D2))->PID) == 14)
	gennu4 = (GenParticle*)branchParticle->At(genW4->D2);
    if (genW4->D1>0 && abs(((GenParticle*)branchParticle->At(genW4->D1))->PID) == 12)
	gennu4 = (GenParticle*)branchParticle->At(genW4->D1); 
    else if (genW4->D2>0 && abs(((GenParticle*)branchParticle->At(genW4->D2))->PID) == 12)
	gennu4 = (GenParticle*)branchParticle->At(genW4->D2);

    if (gennu1 != 0 and gennu2 != 0 and gennu3 != 0 and gennu4 != 0){
	getFinalState(gennu1, branchParticle);
	getFinalState(gennu2, branchParticle);
	getFinalState(gennu3, branchParticle);
	getFinalState(gennu4, branchParticle);
	TLorentzVector allnu_p4 = gennu1->P4()+gennu2->P4()+gennu3->P4()+gennu4->P4();
	allnu_met = allnu_p4.Pt();
    }
  }

}

// ------------ To find H->hh->WWBB->lvlvBB  ------------
void DiHiggstoWWbb::fetchHhhchain(TClonesArray *branchParticle){

  genh2 =  (GenParticle*) branchParticle->At(0);
  for (int i=0; i<branchParticle->GetEntriesFast(); i++){
    genh2 =  (GenParticle*) branchParticle->At(i);
    //std::cout <<"ith particle "<<i; printGenParticle(genh2);
    if (genh2->PID == 99926 or genh2->PID == 99927) 
	break;
  }
  //genh2 =  (GenParticle*) branchParticle->At(0);
  //std::cout  <<"h2tohh id "<< genh2->PID << " mass "<< genh2->Mass << std::endl;
  //printGenParticle(genP);
  if (genh2->PID != 99926 and genh2->PID != 99927) {
    cout <<" first genparticle is not 99926 nor 99927 " << endl; 
    printGenParticle(genh2);
    return;
  }
  if (debug_) cout <<" to fetch Hhh chain, h2 mass "<< genh2->Mass << endl;
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
    if (debug_) cout <<" find two b quark from higgs, htobb mass "<< genhtobb->Mass <<" b1_pt "<< b1_pt <<" b2_pt "<< b2_pt <<endl;
    htobb_px = genhtobb->Px; htobb_py= genhtobb->Py; htobb_pz = genhtobb->Pz; htobb_energy = genhtobb->E;
    htobb_mass = genhtobb->Mass;
  }

  GenParticle *w1_child,*w2_child; 
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
    w1_eta = genW1->Eta; w1_phi = genW1->Phi; w1_pt = genW1->PT;
    w2_mass = genW2->Mass; w2_px = genW2->Px; w2_py = genW2->Py; w2_pz = genW2->Pz; w2_energy = genW2->E;
    w2_eta = genW2->Eta; w2_phi = genW2->Phi; w2_pt = genW2->PT;
    htoWW_px = genhtoWW->Px; htoWW_py = genhtoWW->Py; htoWW_pz = genhtoWW->Pz; htoWW_energy = genhtoWW->E;
    htoWW_mass = genhtoWW->Mass;

    if (genW1->D1>0 && abs(((GenParticle*)branchParticle->At(genW1->D1))->PID)%2 == 1){
	w1_child_id = ((GenParticle*)branchParticle->At(genW1->D1))->PID;
	w1_child = (GenParticle*)branchParticle->At(genW1->D1);
    }else if (genW1->D2>0 && abs(((GenParticle*)branchParticle->At(genW1->D2))->PID)%2 == 1){
	w1_child_id = ((GenParticle*)branchParticle->At(genW1->D2))->PID;
	w1_child = (GenParticle*)branchParticle->At(genW1->D2);
    }
    if (genW2->D1>0 && abs(((GenParticle*)branchParticle->At(genW2->D1))->PID)%2 == 1){
	w2_child_id = ((GenParticle*)branchParticle->At(genW2->D1))->PID;
	w2_child = (GenParticle*)branchParticle->At(genW2->D1);
    }else if (genW2->D2>0 && abs(((GenParticle*)branchParticle->At(genW2->D2))->PID)%2 == 1){
	w2_child_id = ((GenParticle*)branchParticle->At(genW2->D2))->PID;
	w2_child = (GenParticle*)branchParticle->At(genW2->D2);
    }

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
    if (debug_) cout << "mu1 from W, W mass "<< genW1->Mass <<" mu1_pt "<< mu1_pt <<" eta "<< mu1_eta << endl;
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
    if (debug_) cout << "mu2 from W, W mass "<<genW2->Mass <<" mu2_pt "<< mu2_pt<<" eta"<< mu2_eta <<endl;
  }

  if (abs(w1_child_id)==15){
    getFinalState(w1_child, branchParticle);
    //cout <<" w1_child id "<< w1_child_id <<" and its children: "<< endl;
    for (int i= w1_child->D1; i<= w1_child->D2; i++){
	//cout <<"	tau1 child, i "<< i <<" pid "<<  ((GenParticle*)branchParticle->At(i))->PID << endl;
	if (abs(((GenParticle*)branchParticle->At(i))->PID)==11 or abs(((GenParticle*)branchParticle->At(i))->PID)==13)
	  tau1_child_id = ((GenParticle*)branchParticle->At(i))->PID;
    }
  }
  if (abs(w2_child_id)==15){
    getFinalState(w2_child, branchParticle);
    //cout <<" w2_child id "<< w2_child_id <<" and its children: "<< endl;
    for (int i= w2_child->D1; i<= w2_child->D2; i++){
	//cout <<"	tau2 child, i "<< i <<" pid "<<  ((GenParticle*)branchParticle->At(i))->PID << endl;
	if (abs(((GenParticle*)branchParticle->At(i))->PID)==11 or abs(((GenParticle*)branchParticle->At(i))->PID)==13)
	  tau2_child_id = ((GenParticle*)branchParticle->At(i))->PID;
    }
  }
  //cout <<" tau1_child_id "<< tau1_child_id <<" tau2_child_id "<< tau2_child_id << endl;
  if (Wtomu1nu1 and Wtomu2nu2){
    //cout <<" both two Ws decay into muons "<< endl;
    if (((genmu1->PT > muonPt1_ && genmu2->PT > muonPt2_) || (genmu1->PT > muonPt2_ && genmu2->PT > muonPt1_)) 
	  && fabs(genmu1->Eta)<muonsEta_ && fabs(genmu2->Eta)< muonsEta_) hastwogenmuons =true;
    if (debug_) cout <<" chech whether has two genmuons " <<(hastwogenmuons?" yes ":" no ") <<endl;

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
  tt_gen_mass = (gent1->P4()+gent2->P4()).M();
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

  GenParticle *w1_child,*w2_child; 
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

    if (genW1->D1>0 && abs(((GenParticle*)branchParticle->At(genW1->D1))->PID)%2 == 1){
	w1_child_id = ((GenParticle*)branchParticle->At(genW1->D1))->PID;
	w1_child = (GenParticle*)branchParticle->At(genW1->D1);
    }else if (genW1->D2>0 && abs(((GenParticle*)branchParticle->At(genW1->D2))->PID)%2 == 1){
	w1_child_id = ((GenParticle*)branchParticle->At(genW1->D2))->PID;
	w1_child = (GenParticle*)branchParticle->At(genW1->D2);
    }
    if (genW2->D1>0 && abs(((GenParticle*)branchParticle->At(genW2->D1))->PID)%2 == 1){
	w2_child_id = ((GenParticle*)branchParticle->At(genW2->D1))->PID;
	w2_child = (GenParticle*)branchParticle->At(genW2->D1);
    }else if (genW2->D2>0 && abs(((GenParticle*)branchParticle->At(genW2->D2))->PID)%2 == 1){
	w2_child_id = ((GenParticle*)branchParticle->At(genW2->D2))->PID;
	w2_child = (GenParticle*)branchParticle->At(genW2->D2);
    }
    if (debug_) cout <<"ttbar, w1_child id "<< w1_child_id <<" w2_child id "<< w2_child_id << endl;
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
    if (debug_) cout << "mu1 from W, W mass "<< genW1->Mass <<" mu1_pt "<< mu1_pt <<" eta "<< mu1_eta << endl;
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
    if (debug_) cout << "mu2 from W, W mass "<<genW2->Mass <<" mu2_pt "<< mu2_pt<<" eta"<< mu2_eta <<endl;
  }

  if (abs(w1_child_id)==15){
    getFinalState(w1_child, branchParticle);
    //cout <<" w1_child id "<< w1_child_id <<" and its children: "<< endl;
    for (int i= w1_child->D1; i<= w1_child->D2; i++){
	//cout <<"	tau1 child, i "<< i <<" pid "<<  ((GenParticle*)branchParticle->At(i))->PID << endl;
	if (abs(((GenParticle*)branchParticle->At(i))->PID)==11 or abs(((GenParticle*)branchParticle->At(i))->PID)==13)
	  tau1_child_id = ((GenParticle*)branchParticle->At(i))->PID;
    }
  }
  if (abs(w2_child_id)==15){
    getFinalState(w2_child, branchParticle);
    //cout <<" w2_child id "<< w2_child_id <<" and its children: "<< endl;
    for (int i= w2_child->D1; i<= w2_child->D2; i++){
	//cout <<"	tau2 child, i "<< i <<" pid "<<  ((GenParticle*)branchParticle->At(i))->PID << endl;
	if (abs(((GenParticle*)branchParticle->At(i))->PID)==11 or abs(((GenParticle*)branchParticle->At(i))->PID)==13)
	  tau2_child_id = ((GenParticle*)branchParticle->At(i))->PID;
    }
  }
  //cout <<" tau1_child_id "<< tau1_child_id <<" tau2_child_id "<< tau2_child_id << endl;

  if (Wtomu1nu1 and Wtomu2nu2){
    if (((genmu1->PT > muonPt1_ && genmu2->PT > muonPt2_) || (genmu1->PT > muonPt2_ && genmu2->PT > muonPt1_)) 
	  && fabs(genmu1->Eta)<muonsEta_ && fabs(genmu2->Eta)< muonsEta_) hastwogenmuons =true;
    //cout <<" chech whether has two genmuons " <<(hastwogenmuons?" yes ":" no ") <<endl;

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
    genmet_p4.SetXYZT(genmet_px, genmet_py, 0, genmet); 
    if (genmet > metPt_) hasGenMET = true;
    if (genmet_p4.Px()==-999 or genmet_p4.Py()==-999) cout <<" GenMET Px "<< genmet_p4.Px() <<" Py " <<genmet_p4.Py() << " genmet "<< genmet 
	<<" phi "<< genmet_phi << endl;

  }
}


//current use reco muon before Iso, rather after Iso
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
	Muon1_beforeIso_p4 = muon->P4();

	dR_mu1 = muon_p4.DeltaR(genmu1->P4());//trick to replace Isolated muon by muons before Iso 
	muon1 = muon;
	hasMuon1 = true;
	Muon1_p4 = muon->P4();

	if (particle == genmu1) Muon1_beforeIso_hasgenMu = true;
    }
    else if(genmu2 !=0 and muon->Charge>0 and muon_p4.DeltaR(genmu2->P4())<dR_mu2_beforeIso and muon_p4.DeltaR(genmu2->P4())<dR_) {
	dR_mu2_beforeIso = muon_p4.DeltaR(genmu2->P4());
	muon2_beforeIso = muon;
	hasMuon2_beforeIso = true;
	Muon2_beforeIso_p4 = muon->P4();

	dR_mu2 = muon_p4.DeltaR(genmu2->P4());
	muon2 = muon;
	hasMuon2 = true;
	Muon2_p4 = muon->P4();

	if (particle == genmu2) Muon2_beforeIso_hasgenMu = true;
    }
  }
  if (hasMuon1_beforeIso and dR_mu1_beforeIso<0.1 and debug_) std::cout <<" has reco Muon1 before Iso , dR " << dR_mu1_beforeIso <<" IsolationVar "<< muon1_beforeIso->IsolationVar << " pt "<< muon1_beforeIso->PT << " eta "<<muon1_beforeIso->Eta <<std::endl;
  if (hasMuon2_beforeIso and dR_mu2_beforeIso<0.1 and debug_) std::cout <<" has reco Muon2 before Iso , dR " << dR_mu2_beforeIso <<" IsolationVar "<< muon2_beforeIso->IsolationVar << " pt "<< muon2_beforeIso->PT << " eta "<<muon2_beforeIso->Eta <<std::endl;



  if (hasMuon1_beforeIso){
    Muon1_beforeIso_px = muon1_beforeIso->P4().Px(); Muon1_beforeIso_py = muon1_beforeIso->P4().Py(); Muon1_beforeIso_pz = muon1_beforeIso->P4().Pz(); Muon1_beforeIso_energy = muon1_beforeIso->P4().E();
    Muon1_beforeIso_eta = muon1_beforeIso->P4().Eta(); Muon1_beforeIso_phi = muon1_beforeIso->P4().Phi();
    Muon1_beforeIso_pt = muon1_beforeIso->PT;
    Muon1_beforeIso_IsoVar = muon1_beforeIso->IsolationVar;
  }
  if (hasMuon2_beforeIso){
    Muon2_beforeIso_px = muon2_beforeIso->P4().Px(); Muon2_beforeIso_py = muon2_beforeIso->P4().Py(); Muon2_beforeIso_pz = muon2_beforeIso->P4().Pz(); Muon2_beforeIso_energy = muon2_beforeIso->P4().E();
    Muon2_beforeIso_eta = muon2_beforeIso->P4().Eta(); Muon2_beforeIso_phi = muon2_beforeIso->P4().Phi();
    Muon2_beforeIso_pt = muon2_beforeIso->PT;
    Muon2_beforeIso_IsoVar = muon2_beforeIso->IsolationVar;
  }

  if (hasMuon1_beforeIso && hasMuon2_beforeIso){
    if (((muon1_beforeIso->PT > muonPt1_ && muon2_beforeIso->PT > muonPt2_) || (muon1_beforeIso->PT > muonPt2_ && muon2_beforeIso->PT > muonPt1_)) && fabs(muon1_beforeIso->Eta)<muonsEta_ && fabs(muon2_beforeIso->Eta)< muonsEta_) hastwomuons_beforeIso =true;
  }
  // Loop over all Muon in event, reco muon
  /*for(int i = 0; i < branchMuon->GetEntriesFast(); ++i){
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
  //cout <<" muon eta " << muon->Eta << " phi " << muon->Phi << " Pt "<< muon->PT << endl; 
  }*/
  //if (hasMuon1) std::cout <<" has reco Muon1 dR " << dR_mu1 <<" IsolationVar "<< muon1->IsolationVar <<" pt "<< muon1->PT <<std::endl;
  //if (hasMuon2) std::cout <<" has reco Muon2 dR " << dR_mu2 <<" IsolationVar "<< muon2->IsolationVar <<" pt "<< muon2->PT <<std::endl;
}

void DiHiggstoWWbb::matchBjets2Gen(TClonesArray *branchGenJet, TClonesArray *branchJet, GenParticle* genb1, GenParticle* genb2){

  Jet *genjet, *jet;
  genjet =0; jet=0;
  hasgenb1jet=false; hasgenb2jet=false;
  dR_genb1jet = 10; dR_genb2jet=10;
  hasRECOjet1=false; hasRECOjet2=false;
  dR_b1jet = 10; dR_b2jet=10;
  hasb1jet=false; hasb2jet=false;
  //loop all Gen jets 
  int NGenJet=0;
  for (int i =0;  i<branchGenJet->GetEntries(); i++){
    genjet = (Jet*) branchGenJet->At(i);
    if (genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;

    NGenJet++;
    TLorentzVector genjet_p4 = genjet->P4();
    if (genb1 !=0  && genjet_p4.DeltaR(genb1->P4()) < dR_genb1jet) {
	genb1jet_p4 = genjet_p4;
	dR_genb1jet = genjet_p4.DeltaR(genb1->P4());
	genb1jet_px = genjet_p4.Px(); genb1jet_py = genjet_p4.Py(); genb1jet_pz=genjet_p4.Pz(); genb1jet_energy = genjet_p4.Energy();
	genb1jet_eta = genjet_p4.Eta(); genb1jet_phi = genjet_p4.Phi();
	genb1jet_pt = genjet_p4.Pt();
	genb1jet_mass = genjet_p4.M();
	hasgenb1jet = true;
	genb1jet = genjet;
	//cout <<"genb1jet pt "<< genb1jet->PT << endl;
    }
    if (genb2 !=0  && genjet_p4.DeltaR(genb2->P4()) < dR_genb2jet){
	genb2jet_p4 = genjet_p4;
	dR_genb2jet = genjet_p4.DeltaR(genb2->P4());
	genb2jet_px = genjet_p4.Px(); genb2jet_py = genjet_p4.Py(); genb2jet_pz=genjet_p4.Pz(); genb2jet_energy = genjet_p4.Energy();
	genb2jet_eta = genjet_p4.Eta(); genb2jet_phi = genjet_p4.Phi();
	genb2jet_pt = genjet_p4.Pt();
	genb2jet_mass = genjet_p4.M();
	hasgenb2jet = true;
	genb2jet = genjet;
	//cout <<"genb2jet pt "<< genb2jet->pt << endl;
    }
  }
  numGenJet = NGenJet;
  // genb1jet should be different from genb2jet
  if (hasgenb1jet && hasgenb2jet && genb1jet == genb2jet){
    if (dR_genb1jet < dR_genb2jet){
	hasgenb2jet = false;
	dR_genb2jet = jetsDeltaR_;
	genb2jet =0;
	for (int i =0;  i<  branchGenJet->GetEntries(); i++){
	  genjet = (Jet*) branchGenJet->At(i);
	  if (genjet==genb1jet || genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
	  TLorentzVector genjet_p4 = genjet->P4();
	  if (genb2 !=0  && genjet_p4.DeltaR(genb2->P4()) < dR_genb2jet){
	    genb2jet_p4 = genjet_p4;
	    dR_genb2jet = genjet_p4.DeltaR(genb2->P4());
	    genb2jet_px = genjet_p4.Px(); genb2jet_py = genjet_p4.Py(); genb2jet_pz=genjet_p4.Pz(); genb2jet_energy = genjet_p4.Energy();
	    genb2jet_eta = genjet_p4.Eta(); genb2jet_phi = genjet_p4.Phi();
	    genb2jet_pt = genjet_p4.Pt();
	    genb2jet_mass = genjet_p4.M();
	    hasgenb2jet = true;
	    genb2jet = genjet;
	  }
	}
    }//if (dR_genb1jet < dR_genb2jet)
    else if (dR_genb1jet > dR_genb2jet){
	hasgenb1jet = false;
	dR_genb1jet = jetsDeltaR_;
	genb1jet = 0;
	for (int i =0;  i<  branchGenJet->GetEntries(); i++){
	  genjet = (Jet*) branchGenJet->At(i);
	  if (genjet==genb2jet || genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
	  TLorentzVector genjet_p4 = genjet->P4();
	  if (genb1 !=0  && genjet_p4.DeltaR(genb1->P4()) < dR_genb1jet){
	    genb1jet_p4 = genjet_p4;
	    dR_genb1jet = genjet_p4.DeltaR(genb1->P4());
	    genb1jet_px = genjet_p4.Px(); genb2jet_py = genjet_p4.Py(); genb2jet_pz=genjet_p4.Pz(); genb2jet_energy = genjet_p4.Energy();
	    genb1jet_eta = genjet_p4.Eta(); genb2jet_phi = genjet_p4.Phi();
	    genb1jet_pt = genjet_p4.Pt();
	    genb1jet_mass = genjet_p4.M();
	    hasgenb1jet = true;
	    genb1jet = genjet;
	  }
	}
    }//if (dR_genb1jet > dR_genb2jet)
  }


  if (hasgenb1jet && hasgenb2jet){
    findNeutrinosfromJet(genb1jet->Particles);
    findNeutrinosfromJet(genb2jet->Particles);
  }

  /*bool fidcut=(abs(genb1->Eta)<bjetsEta_ and abs(genb2->Eta)<bjetsEta_ and (genb1->PT)>bjetsPt_ and (genb2->PT)>bjetsPt_);
  //bool fidcut=true;
  if (fidcut and hasgenb1jet and hasgenb2jet and (dR_genb1jet>1 or dR_genb2jet>1)){
  cout <<"------------------- begin  dR debug jetwithout Nutrinos eventid "<<event_n <<"   ------------------------- "<< endl;
  cout <<" dR_genb1jet "<< dR_genb1jet <<" dR_genb2jet "<< dR_genb2jet << endl; 
  float b1pt= genb1->PT;
  float b2pt= genb2->PT;
  cout <<" genb1 pt "<<b1pt; genb1->P4().Print();
  cout <<" genb2 pt "<<b2pt; genb2->P4().Print();
  cout <<" nufromb1 pt "<<sqrt(nufromb1_px*nufromb1_px+nufromb1_py*nufromb1_py)<<" energy "<< nufromb1_energy 
  <<"  nufromb2 pt "<< sqrt(nufromb2_px*nufromb2_px+nufromb2_py*nufromb2_py) <<" energy "<<nufromb2_energy << endl;
  for (int i =0;  i < branchGenJet->GetEntries(); i++){
  genjet = (Jet*) branchGenJet->At(i);
  TLorentzVector genjet_p4 = genjet->P4();
  cout <<" dR(jet,b1) "<<genjet_p4.DeltaR(genb1->P4())<<" deltaE "<<(genb1->E-genjet_p4.Energy())<<" dR(jet,b2)"<<genjet_p4.DeltaR(genb2->P4())<<" deltaE "<<(genb2->E-genjet_p4.Energy())<< endl;
  printJet(genjet);
  }

  cout <<"------------------- end    dR debug jetwithout Nutrinos    ------------------------- "<< endl;
  }*/


  if (not (hasgenb1jet and hasgenb2jet)) return;
  if (debug_)
    cout <<" genb1jet pt "<< genb1jet->PT <<" eta "<< genb1jet->Eta<<" phi "<<genb1jet->Phi <<" dR "<<dR_genb1jet <<" genb2jet pt "<< genb2jet->PT <<" eta "<< genb2jet->Eta <<" phi "<< genb2jet->Phi <<" dR "<< dR_genb2jet << endl;
  DE_partonGneJet1 = genb1->P4().E()-genb1jet_energy; 
  DE_partonGneJet2 = genb2->P4().E()-genb2jet_energy; 
  //loop all reco jets 
  for (int i =0;  i<branchJet->GetEntries(); i++)
  {
    jet = (Jet*) branchJet->At(i);
    TLorentzVector jet_p4 = jet->P4();
    if (debug_)
	cout <<"debug in matchBjet2Gen jet i "<< i<<" btag "<<jet->BTag <<" pt "<< jet->PT<<" eta "<< jet->Eta <<" phi "<< jet->Phi <<" dR1 "<< jet_p4.DeltaR(genb1jet_p4) <<" dR2 "<<jet_p4.DeltaR(genb2jet_p4) << std::endl;
    //if (((jet->BTag)&2)<1 || jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
    if (jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
    if (hasgenb1jet && jet_p4.DeltaR(genb1jet_p4) < dR_b1jet) {
	b1jet = jet;
	dR_b1jet = jet_p4.DeltaR(genb1jet_p4);
	b1jet_p4 = jet_p4;
	hasRECOjet1=true;
    }
    if (hasgenb2jet && jet_p4.DeltaR(genb2jet_p4) < dR_b2jet){
	b2jet = jet;
	dR_b2jet = jet_p4.DeltaR(genb2jet_p4);
	b2jet_p4 = jet_p4;
	//cout <<"b2jet pt "<< b2jet->PT << endl;
	hasRECOjet2 = true;
    }
  }
  // b1jet should be different from b2jet
  if (hasRECOjet1 && hasRECOjet2 && b1jet == b2jet){
    if (dR_b1jet < dR_b2jet){ //to find another b1jet
	hasRECOjet2 = false;
	dR_b2jet = jetsDeltaR_;
	b2jet =0;
	for (int i =0;  i<branchJet->GetEntries(); i++)
	{
	  jet = (Jet*) branchJet->At(i);
	  if ( jet==b1jet || jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
	  TLorentzVector jet_p4 = jet->P4();
	  if (genb2jet !=0 && jet_p4.DeltaR(genb2jet_p4) < dR_b2jet) {
	    b2jet = jet;
	    dR_b2jet = jet_p4.DeltaR(genb2jet_p4);
	    b2jet_p4 = jet_p4;
	    hasRECOjet2 = true;
	  }

	}
    } //if (dR_b1jet < dR_b2jet)
    else if (dR_b1jet > dR_b2jet){
	hasRECOjet1 = false;
	dR_b1jet = jetsDeltaR_;
	b1jet =0;
	for (int i =0;  i<  branchJet->GetEntries(); i++)
	{
	  jet = (Jet*) branchJet->At(i);
	  if ( jet==b2jet || jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
	  TLorentzVector jet_p4 = jet->P4();
	  if (genb1jet !=0 && jet_p4.DeltaR(genb1jet_p4) < dR_b1jet) {
	    b1jet = jet;
	    dR_b1jet = jet_p4.DeltaR(genb1jet_p4);
	    b1jet_p4 = jet_p4;
	    hasRECOjet1 = true;
	  }
	}
    }
    else if (dR_b1jet == dR_b2jet) cerr<<" error dR_b1jet = dR_b2jet " << endl;
  }
  if (hasRECOjet1 and hasRECOjet2 and (dR_b1jet>0.15 or dR_b2jet >0.15 ) and dR_b1jet<5 and dR_b2jet<5 and bpartonsOK and debug_) {
    //std::cout <<" b1 eta  "<< genb1->Eta <<" pt "<< genb1->PT <<" b2 eta "<< genb2->Eta <<" pt "<< genb2->PT << std::endl;
    //std::cout <<" genb1jet pt "<< genb1jet->PT <<" dR_genb1jet "<< dR_genb1jet <<" genb2jet pt "<< genb2jet->PT <<" dR_genb2jet "<< dR_genb2jet << std::endl;
    std::cout <<"debug in matchBjet2Gen jet1 pt "<< b1jet->PT <<" eta "<< b1jet->Eta <<" dR_b1jet "<< dR_b1jet 
	<<" jet2 pt "<< b2jet->PT <<" eta "<< b2jet->Eta <<" dR_b2jet "<< dR_b2jet << std::endl;
  }
  //Now Btag
  if(hasRECOjet1 && !((b1jet->BTag)&2)<1 )  hasb1jet = true;
  if(hasRECOjet2 && !((b2jet->BTag)&2)<1 )  hasb2jet = true;
}


void DiHiggstoWWbb::matchBjetswithNu2Gen(TClonesArray *branchGenJet,  GenParticle *genb1, GenParticle *genb2, float dR_){

  Jet *genjet;
  Jet *genb1jet_withNu,*genb2jet_withNu;
  hasgenb1jet_withNu =false; hasgenb2jet_withNu=false;
  dR_genb1jet_withNu=10; dR_genb2jet_withNu =10;
  genjet =0;
  genb1jet_withNu=0;
  genb2jet_withNu=0;
  nufromb1_p4 = TLorentzVector();
  nufromb2_p4 = TLorentzVector();

  for (int i =0;  i < branchGenJet->GetEntries(); i++){
    genjet = (Jet*) branchGenJet->At(i);
    TLorentzVector genjet_p4 = genjet->P4();
    //cout <<" jet i "<< i <<" pt "<< genjet->PT <<" eta "<< genjet->Eta <<" dR(b1)" << genjet_p4.DeltaR(genb1->P4()) <<" dR(b2)" << genjet_p4.DeltaR(genb2->P4()) <<  endl; 
    if (genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
    if (genb1 !=0  && genjet_p4.DeltaR(genb1->P4()) < dR_genb1jet_withNu && genjet_p4.DeltaR(genb1->P4())< dR_) {
	genb1jet_withNu_p4 = genjet_p4;
	dR_genb1jet_withNu = genjet_p4.DeltaR(genb1->P4());
	hasgenb1jet_withNu = true;
	genb1jet_withNu = genjet;
	//cout <<"genb1jet pt "<< genb1jet_withNu->PT << " Eta "<< genb1jet_withNu->Eta<<endl;
    }
    if (genb2 !=0  && genjet_p4.DeltaR(genb2->P4()) < dR_genb2jet_withNu && genjet_p4.DeltaR(genb2->P4()) < dR_){
	genb2jet_withNu_p4 = genjet_p4;
	dR_genb2jet_withNu = genjet_p4.DeltaR(genb2->P4());
	hasgenb2jet_withNu = true;
	genb2jet_withNu = genjet;
	//cout <<"genb2jet pt "<< genb2jet_withNu->PT <<" Eta "<< genb2jet_withNu->Eta<< endl;
    }
  }
  // genb1jet should be different from genb2jet
  if (hasgenb1jet_withNu && hasgenb2jet_withNu && genb1jet_withNu == genb2jet_withNu){
    //cout <<"dRb1jet_withNu "<< dR_genb1jet_withNu<<"  dRb2jet_withNu "<< dR_genb2jet_withNu << endl;
    //cout <<" jet1 pt "<<genb1jet_withNu->PT <<" eta "<<genb1jet_withNu->Eta <<" jet2 pt "<<genb2jet_withNu->PT <<" Eta "<<genb2jet_withNu->Eta<<endl;
    if (dR_genb1jet_withNu < dR_genb2jet_withNu){ //to find another b1jet
	hasgenb2jet_withNu = false;
	dR_genb2jet_withNu = dR_;
	for (int i =0;  i < branchGenJet->GetEntries(); i++)
	{
	  genjet = (Jet*) branchGenJet->At(i);
	  if ( genjet == genb1jet_withNu || genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
	  TLorentzVector genjet_p4 = genjet->P4();
	  if (genb2 !=0  && genjet_p4.DeltaR(genb2->P4()) < dR_genb2jet_withNu && genjet_p4.DeltaR(genb2->P4()) < dR_){
	    genb2jet_withNu_p4 = genjet_p4;
	    dR_genb2jet_withNu = genjet_p4.DeltaR(genb2->P4());
	    hasgenb2jet_withNu = true;
	    genb2jet_withNu = genjet;
	  }

	}

    }
    else if (dR_genb1jet_withNu > dR_genb2jet_withNu){ //to find another b1jet
	hasgenb1jet_withNu = false;
	dR_genb1jet_withNu = dR_;
	for (int i =0;  i < branchGenJet->GetEntries(); i++)
	{
	  genjet = (Jet*) branchGenJet->At(i);
	  if ( genjet == genb2jet_withNu || genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
	  TLorentzVector genjet_p4 = genjet->P4();
	  if (genb1 !=0  && genjet_p4.DeltaR(genb1->P4()) < dR_genb1jet_withNu && genjet_p4.DeltaR(genb1->P4())< dR_) {
	    genb1jet_withNu_p4 = genjet_p4;
	    dR_genb1jet_withNu = genjet_p4.DeltaR(genb1->P4());
	    hasgenb1jet_withNu = true;
	    genb1jet_withNu = genjet;
	  }
	}
    }

  }

  if (hasgenb1jet_withNu && hasgenb2jet_withNu){
    genb1jet_withNu_px = genb1jet_withNu_p4.Px(); genb1jet_withNu_py = genb1jet_withNu_p4.Py(); genb1jet_withNu_pz=genb1jet_withNu_p4.Pz(); genb1jet_withNu_energy = genb1jet_withNu_p4.Energy();
    genb1jet_withNu_eta = genb1jet_withNu_p4.Eta(); genb1jet_withNu_phi = genb1jet_withNu_p4.Phi(); genb1jet_withNu_pt=genb1jet_withNu_p4.Pt(); 
    genb2jet_withNu_px = genb2jet_withNu_p4.Px(); genb2jet_withNu_py = genb2jet_withNu_p4.Py(); genb2jet_withNu_pz=genb2jet_withNu_p4.Pz(); genb2jet_withNu_energy = genb2jet_withNu_p4.Energy();
    genb2jet_withNu_eta = genb2jet_withNu_p4.Eta(); genb2jet_withNu_phi = genb2jet_withNu_p4.Phi(); genb2jet_withNu_pt=genb2jet_withNu_p4.Pt(); 
  }

  if (hasgenb1jet_withNu && hasgenb2jet_withNu){
    //TRefArray b1jet_pars = genb1jet_withNu.Particle;
    //cout <<" tried to find neutrino from bjets(withNu) " << endl;
    nufromb1_p4=findNeutrinosfromJet(genb1jet_withNu->Particles);
    nufromb2_p4=findNeutrinosfromJet(genb2jet_withNu->Particles);
    nufromb1_px = nufromb1_p4.Px(); nufromb1_py = nufromb1_p4.Py(); nufromb1_pz = nufromb1_p4.Pz(); nufromb1_energy = nufromb1_p4.Energy();
    nufromb2_px = nufromb2_p4.Px(); nufromb2_py = nufromb2_p4.Py(); nufromb2_pz = nufromb2_p4.Pz(); nufromb2_energy = nufromb2_p4.Energy();
  }
  /*bool fidcut=(abs(genb1->Eta)<bjetsEta_ and abs(genb2->Eta)<bjetsEta_ and (genb1->PT)>bjetsPt_ and (genb2->PT)>bjetsPt_);
  //bool fidcut=true;
  if (fidcut and hasgenb1jet_withNu and hasgenb2jet_withNu and (dR_genb1jet_withNu>1 or dR_genb2jet_withNu>1)){
  cout <<"-------------------   dR debug jetwithNutrinos  eventid "<<event_n <<"   ------------------------- "<< endl;
  cout <<" dR_genb1jet_withNu "<< dR_genb1jet_withNu<<" dR_genb2jet_withNu "<< dR_genb2jet_withNu << endl; 
  float b1pt= genb1->PT;
  float b2pt= genb2->PT;
  cout <<" genb1 pt "<<b1pt; genb1->P4().Print();
  cout <<" genb2 pt "<<b2pt; genb2->P4().Print();
  cout <<" nufromb1 pt "<<sqrt(nufromb1_px*nufromb1_px+nufromb1_py*nufromb1_py)<<" energy "<< nufromb1_energy 
  <<"  nufromb2 pt "<< sqrt(nufromb2_px*nufromb2_px+nufromb2_py*nufromb2_py) <<" energy "<<nufromb2_energy << endl;
  for (int i =0;  i < branchGenJet->GetEntries(); i++){
  genjet = (Jet*) branchGenJet->At(i);
  TLorentzVector genjet_p4 = genjet->P4();
  cout <<" dR(jet,b1) "<<genjet_p4.DeltaR(genb1->P4())<<" deltaE "<<(genb1->E-genjet_p4.Energy())<<" dR(jet,b2) "<<genjet_p4.DeltaR(genb2->P4())<<" deltaE "<<(genb2->E-genjet_p4.Energy())<< endl;
  printJet(genjet);
  }

  }*/

}


void DiHiggstoWWbb::checkBjets2Gen(TClonesArray *branchGenJet, TClonesArray *branchJet, GenParticle* genb1, GenParticle* genb2, float dR_){
  //loop all Gen jets 
  if (not (hasRECOjet1 and hasRECOjet2 and h2tohh)) return;

  Jet *jet;
  jet=0;
  float dR11=b1jet_p4.DeltaR(genb1->P4());
  float dR12=b1jet_p4.DeltaR(genb2->P4());
  float dR21=b2jet_p4.DeltaR(genb1->P4());
  float dR22=b2jet_p4.DeltaR(genb2->P4());
  float dR1=(dR11<dR12? dR11:dR12);
  float dR2=(dR21<dR22? dR21:dR22);
  if ((dR11<dR12 and dR21<dR22) or (dR11>dR12 and dR21>dR22)){
    if (dR1<dR2) dR2=(dR21>dR22? dR21:dR22);
    else dR1=(dR11>dR12? dR11:dR12);
  }
  bool b1jet2b1 =false;
  if (dR1==dR11) b1jet2b1=true;
  hasgenb1 = true;
  hasgenb2 = true;
  for (int i =0;  i<  branchJet->GetEntries(); i++){

    jet = (Jet*) branchJet->At(i);
    if (jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_ || jet==b1jet || jet==b2jet) continue;
    TLorentzVector jet_p4 = jet->P4();
    float tmpdR1 = (b1jet2b1 ? jet_p4.DeltaR(genb1->P4()) : jet_p4.DeltaR(genb2->P4()));
    float tmpdR2 = (b1jet2b1 ? jet_p4.DeltaR(genb2->P4()) : jet_p4.DeltaR(genb1->P4()));

    if (genb1 !=0  and genb2 !=0 and  tmpdR1<dR1) {
	hasgenb1 = false;
    }
    if (genb1 !=0  and genb2 !=0 and  tmpdR2<dR2) {
	hasgenb2 = false;
    }
  }
  if (htobb and not(simulation_)){
    dR_b1jet=dR1;
    dR_b2jet=dR2;
  }

}



TLorentzVector DiHiggstoWWbb::findNeutrinosfromJet(TRefArray particles){

  GenParticle *genp;
  unsigned int numneutrinos = 0;
  TLorentzVector nu_p4 = TLorentzVector();
  for(int i=0; i<particles.GetEntries(); i++){
    if (not (genp = (GenParticle*)particles.At(i))) continue;
    if (abs(genp->PID)==12 or abs(genp->PID)== 14 or abs(genp->PID)==16){
	//cout <<" find one neutrinos in jet: "; printGenParticle(genp);
	nu_p4 = nu_p4+genp->P4();
	numneutrinos++;
    }

  }

  return nu_p4;
}

void DiHiggstoWWbb::printGenParticle(GenParticle *genP){
  cout << " genP Id " << genP->PID <<" Pt " << genP->PT << " M1 "<< genP->M1<<" M2 "<< genP->M2;
  cout << " D1 "<< genP->D1 <<" D2 "<<genP->D2 << " p4 "; genP->P4().Print();
}

void DiHiggstoWWbb::printDecendantsParticles(GenParticle* genp, TClonesArray *branchParticle)
{
  std::cout <<" Decendants of particle id "<< genp->PID <<" mass "<< genp->Mass; genp->P4().Print();
  if (genp->D1>0){
    GenParticle* decendant1 = (GenParticle*)branchParticle->At(genp->D1);
    std::cout <<" decendant1 id "<< decendant1->PID <<" mass "<< decendant1->Mass; decendant1->P4().Print();
  }
  if (genp->D2>0){
    GenParticle* decendant2 = (GenParticle*)branchParticle->At(genp->D2);
    std::cout <<" decendant2 id "<< decendant2->PID <<" mass "<< decendant2->Mass; decendant2->P4().Print();
  }
  if (genp->D1>0) printDecendantsParticles((GenParticle*)branchParticle->At(genp->D1), branchParticle);
  if (genp->D2>0) printDecendantsParticles((GenParticle*)branchParticle->At(genp->D2), branchParticle);
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
	//cout << "    GenPart pt: " << particle->PT << ", eta: " << particle->Eta << ", phi: " << particle->Phi << endl;
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

  /*for (int j =0; j<jet->Particles.GetEntries();  j++){
    GenParticle *p_tmp = (GenParticle*) particlearray.At(j);
    printGenParticle(p_tmp);
    }*/
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
  alljets.clear();
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
  event_n = -999;
  weight = 0.;
  reweighting = 0;
  weight_pess = 0.;
  vertices = 0;
  MVA_value = -999.;
  MVA_value_B1fortt = -999.;
  MVA_value_B2fortt = -999.;
  MVA_value_B3fortt = -999.;
  MVA_value_B4fortt = -999.;
  MVA_value_B5fortt = -999.;
  MVA_value_B6fortt = -999.;
  MVA_value_B7fortt = -999.;
  MVA_value_B8fortt = -999.;
  MVA_value_B9fortt = -999.;
  MVA_value_B10fortt = -999.;
  MVA_value_B11fortt = -999.;
  MVA_value_B12fortt = -999.;
  MT2 = -999.;
  MT2_reco = -999.;
  MT2_noMU = -999.;
  MT2_noMUv2 = -999.;
  MT2_noMUv3 = -999.;
  MT2_noMU_reco = -999.;
  MT2_noMU_recov2 = -999.;
  MT2_noMU_recov3 = -999.;
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
  finalb1_eta = 0;
  finalb1_phi = 0;
  finalb1_pt =0;
  finalb1_energy =0;
  finalb2_eta = 0;
  finalb2_phi = 0;
  finalb2_pt =0;
  finalb2_energy =0;

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
  dR_genb1jet=jetsDeltaR_;
  dR_genb2jet=jetsDeltaR_;
  hasgenb1jet=false;
  hasgenb2jet=false;
  DE_partonGneJet1=0;
  DE_partonGneJet2=0;

  genb1jet_withNu_px=0;
  genb1jet_withNu_py=0;
  genb1jet_withNu_pz=0;
  genb1jet_withNu_eta=0;
  genb1jet_withNu_phi=0;
  genb1jet_withNu_pt=0;
  genb1jet_withNu_energy=0;
  genb2jet_withNu_px=0;
  genb2jet_withNu_py=0;
  genb2jet_withNu_pz=0;
  genb2jet_withNu_eta=0;
  genb2jet_withNu_phi=0;
  genb2jet_withNu_pt=0;
  genb2jet_withNu_energy=0;
  dR_genb1jet_withNu=jetsDeltaR_;
  dR_genb2jet_withNu=jetsDeltaR_;
  hasgenb1jet_withNu=false;
  hasgenb2jet_withNu=false;

  nufromb1_px =0.0;
  nufromb1_py =0.0;
  nufromb1_pz =0.0;
  nufromb1_energy =0.0;
  nufromb2_px =0.0;
  nufromb2_py =0.0;
  nufromb2_pz =0.0;
  nufromb2_energy =0.0;

  dR_b1jet = jetsDeltaR_;
  dR_b2jet = jetsDeltaR_;
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
  hasRECOjet1=false;
  hasRECOjet2=false;
  hasb1jet=false;
  hasb2jet=false;
  hasgenb1=false;
  hasgenb2=false;
  bpartonsOK=false;

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
  nu1and2_pt =-999;
  nu1and2_px =-999;
  nu1and2_py =-999;
  nu1and2_diBaxis_p =-999;
  nu1and2_diBaxis_t =-999;

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

  w1_child_id = -1;
  w2_child_id = -1;
  tau1_child_id = -1;
  tau2_child_id = -1;

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
  tt_gen_mass = 0;
  ttbar = false;

  Muon1_beforeIso_px = 0.0;
  Muon1_beforeIso_py = 0.0;
  Muon1_beforeIso_pz = 0.0;
  Muon1_beforeIso_eta = 0.0;
  Muon1_beforeIso_phi = 0.0;
  Muon1_beforeIso_pt = 0.0;
  Muon1_beforeIso_energy = 0.0;
  Muon1_beforeIso_IsoVar = 10.0;
  Muon2_beforeIso_px = 0.0;
  Muon2_beforeIso_py = 0.0;
  Muon2_beforeIso_pz = 0.0;
  Muon2_beforeIso_eta = 0.0;
  Muon2_beforeIso_phi = 0.0;
  Muon2_beforeIso_pt = 0.0;
  Muon2_beforeIso_energy = 0.0;
  Muon2_beforeIso_IsoVar = 10.0;
  dR_mu1_beforeIso = 2.0;
  dR_mu2_beforeIso = 2.0;

  Muon1_beforeIso_hasgenMu = false; 
  Muon2_beforeIso_hasgenMu = false; 
  Muon1_beforeIso_passIso = false;
  Muon2_beforeIso_passIso = false; 
  hastwomuons_beforeIso = false;
  hasMuon1_beforeIso = false;
  hasMuon2_beforeIso = false;

  Muon1_px = 0;
  Muon1_py = 0;
  Muon1_pz = 0;
  Muon1_eta = 0;
  Muon1_phi = 0;
  Muon1_pt = 0;
  Muon1_energy = 0;
  Muon1_IsoVar = 10.0;
  Muon2_px = 0;
  Muon2_py = 0;
  Muon2_pz = 0;
  Muon2_eta = 0;
  Muon2_phi = 0;
  Muon2_pt = 0;
  Muon2_energy = 0;
  Muon2_IsoVar = 10.0;
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
  numjets =0;
  numGenJet =0;
  numLeptons1 =0;
  numLeptons2 =0;
  hasRecob1jet =false;
  hasRecob2jet =false;
  hasRecob1jetCorr =false;
  hasRecob2jetCorr =false;
  hasRecob1jetPt =false;
  hasRecob2jetPt =false;
  hasRecob1jetPtCorr =false;
  hasRecob2jetPtCorr =false;
  hasRecob1b2jet_bTag =false;
  hasRecob1b2jetPt_bTag =false;
  reconMassJet1_PtRes=-999.;
  reconMassJet2_PtRes=-999.;
  reconDRJet1_PtRes=-999.;
  reconDRJet2_PtRes=-999.;
  hasRecoMuon1 =false;
  hasRecoMuon2 =false;

  dR_genbl=-1.0;
  dR_genb1l1=-1.0;
  dR_genb1l2=-1.0;
  dR_genb2l1=-1.0;
  dR_genb2l2=-1.0;
  dR_genl1l2=-1.0;
  dR_genl1l2b1b2=-1.0;
  dphi_genl1l2b1b2=-1.0;
  dR_genb1b2=-1.0;
  dR_genminbl = -1.0;
  mass_genl1l2 = -1.0;
  energy_genl1l2 = 0.0;
  pt_genl1l2 = 0.0;
  phi_genl1l2 = 0.0;
  eta_genl1l2 = 0.0;
  mass_genb1b2 = -1.0;
  energy_genb1b2 = 0.0;
  pt_genb1b2 = 0.0;
  phi_genb1b2 = 0.0;
  eta_genb1b2 = 0.0;
  dphi_genllbb = -10;
  dphi_genllmet = -10;
  mass_gentrans = 0.0;

  minMass=999.;
  dR_bl=-1.0;
  dR_b1l1=-1.0;
  dR_b1l2=-1.0;
  dR_b2l1=-1.0;
  dR_b2l2=-1.0;
  MINdR_bl=-1.0;
  dR_l1l2=-1.0;
  dR_l1l2b1b2=-1.0;
  dphi_l1l2b1b2=-1.0;
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

  genmet = -999.;
  genmet_phi = -999.;
  genmet_px = -999.;
  genmet_py = -999.;
  met = -999.;
  met_phi = -999.;
  met_px = -999.;
  met_py = -999.;
  genmet_diBaxis_p = -999.;
  genmet_diBaxis_t = -999.;
  met_diBaxis_p = -999.;
  met_diBaxis_t = -999;
  met_c1_px = -999;
  met_c1_py = -999;
  met_diBaxis_c1_p = -999;
  met_diBaxis_c1_t = -999;

  allnu_met = -999;
  HhhtoWWWW = false;

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
  preselections_gen=false;
  runMMCok=false;

  //MMC results
  MMC_h2mass_prob =0.0;
  MMC_h2massweight1_prob =0.0;
  MMC_h2massweight4_prob = 0.0;
  MMC_h2mass_Entries = 0.0;
  MMC_h2mass_RMS = 0.0;
  MMC_h2massweight1_RMS = 0.0;
  MMC_h2massweight4_RMS = 0.0;
  MMC_h2mass_Mean =0.0;
  MMC_h2mass_underflow =0.0;
  MMC_h2mass_overflow =0.0;
  MMC_h2mass_MaxBin =0.0;
  MMC_h2mass_weight1_MaxBin =0.0;
  MMC_h2mass_weight4_MaxBin =0.0;
}

/*************************
 ****run method : process all subprocess
 *************************/  
void DiHiggstoWWbb::DiHiggstoWWbbrun()
{
  if(debug_) cout<<"DEBUG::0"<<endl;
  long entry;
  TLorentzVector momentum;

  //TFile *MMCfile = new TFile("MMCfile.root","recreate");
  /****** Loop over all events *****/
  int totalevents=0;
  if (nEvents_ < 0) totalevents = allEntries;
  else totalevents = nStarts_+nEvents_;

  if (totalevents > allEntries){
    cout <<" set up is not correct:  nStarts_+nEvents_ > allEntries " << endl;
    totalevents = allEntries;
  }

  for(entry = nStarts_; entry < totalevents; ++entry){

    if(debug_) cout<<"DEBUG::1"<<endl;
    initBranches();
    weight = Thisweight;
    weight_pess = Thisweight_pess;
    // Load selected branches with data from specified event
    bool readsuccess = treeReader->ReadEntry(entry);
    if (not readsuccess) {
	cout <<"can not read Entry through treeReader" << endl;
	exit(0);
    }
    event_n = entry;
    if((entry-nStarts_)%1000==0) std::cerr<<"Analazyning event: "<<entry<<" / "<<totalevents<<std::endl;
    if (debug_)  
	std::cout<<"Analazyning event: "<<entry<<" / "<<totalevents<<std::endl;
    if (PUSample_) vertices = branchVertex->GetEntries(); 
    /*for (int i=0; i<branchVertex->GetEntries(); i++){
	Vertex *ver = (Vertex *)branchVertex->At(i);
	std::cout <<"vertex i "<<i<<" X "<< ver->X <<" Y "<< ver->Y <<" Z "<< ver->Z <<" T "<<ver->T <<std::endl;
	}*/

    //following information may not available in pure reco cases
    //else if (simulation_) fetchttbarchain(branchParticle);
    fetchHhhWWWWchain(branchParticle);
    if (sample_ != tt ) fetchHhhchain(branchParticle); 
    else if (sample_ == tt) fetchttbarchain(branchParticle);
    h2tohh = (htobb and Wtomu1nu1 and Wtomu2nu2);
    ttbar  = (ttoWb and tbartoWbbar and Wtomu1nu1 and Wtomu2nu2);
    getGenMET(branchGenMissingET);
    //if (not(h2tohh) and not(ttbar)) continue;
    if ((h2tohh or ttbar) and simulation_){
	//if (h2tohh or ttbar){
	//printDecendantsParticles(genb1, branchParticle);
	//printDecendantsParticles(genb2, branchParticle);
	GenParticle *genptmp1, *genptmp2;
	genptmp1 = genb1;
	while ((genptmp1->D1>0 && ((GenParticle*)branchParticle->At(genptmp1->D1))->PID == genb1->PID)
	    or (genptmp1->D2 >0 && ((GenParticle*)branchParticle->At(genptmp1->D2))->PID == genb1->PID)){
	  if (genptmp1->D1>0 && ((GenParticle*)branchParticle->At(genptmp1->D1))->PID == genb1->PID)
	    genptmp1 = (GenParticle*)branchParticle->At(genptmp1->D1);
	  if (genptmp1->D2>0 && ((GenParticle*)branchParticle->At(genptmp1->D2))->PID == genb1->PID)
	    genptmp1 = (GenParticle*)branchParticle->At(genptmp1->D2);
	}

	genptmp2 = genb2;
	while ((genptmp2->D1>0 && ((GenParticle*)branchParticle->At(genptmp2->D1))->PID == genb2->PID)
	    or (genptmp2->D2 >0 && ((GenParticle*)branchParticle->At(genptmp2->D2))->PID == genb2->PID)){
	  if (genptmp2->D1>0 && ((GenParticle*)branchParticle->At(genptmp2->D1))->PID == genb2->PID)
	    genptmp2 = (GenParticle*)branchParticle->At(genptmp2->D1);
	  if (genptmp2->D2>0 && ((GenParticle*)branchParticle->At(genptmp2->D2))->PID == genb2->PID)
	    genptmp2 = (GenParticle*)branchParticle->At(genptmp2->D2);
	}

	genfinalb1 = genptmp1;
	genfinalb2 = genptmp2;
	finalb1_eta = genfinalb1->Eta; finalb1_phi = genfinalb1->Phi; finalb1_pt = genfinalb1->PT; finalb1_energy = genfinalb1->E;
	finalb2_eta = genfinalb2->Eta; finalb2_phi = genfinalb2->Phi; finalb2_pt = genfinalb2->PT; finalb2_energy = genfinalb2->E;
	//match jet with b after gluon emssion
	matchBjetswithNu2Gen(branchGenJet_withNu, genfinalb1, genfinalb2, jetsDeltaR_); 
	matchBjets2Gen(branchGenJet, branchJet, genfinalb1, genfinalb2); 

	dR_genb1jet_withNu_finalb = dR_genb1jet_withNu;
	dR_genb2jet_withNu_finalb = dR_genb2jet_withNu;
	dR_genb1jet_finalb = dR_genb1jet;
	dR_genb2jet_finalb = dR_genb2jet;

	if( genb1->P4().Pt()>30. && fabs(genb1->P4().Eta())<2.4 && genb2->P4().Pt()>30. && fabs(genb2->P4().Eta())<2.4 ) bpartonsOK = true;	
	matchMuon2Gen(branchMuonBeforeIso, branchMuon,genmu1, genmu2, leptonsDeltaR_); 
	matchBjetswithNu2Gen(branchGenJet_withNu, genb1, genb2, jetsDeltaR_); 
	matchBjets2Gen(branchGenJet, branchJet, genb1, genb2); 
    }
    if (simulation_){
	//GENMET on Di-BJet Axis
	if( hasgenb1jet and hasgenb2jet ){
	  TVector3 genDiBjet_Transv( (genb1jet_p4 + genb2jet_p4).Px(), (genb1jet_p4 + genb2jet_p4).Py(), 0.);
	  TVector3 genMet_Transv( genmet_px, genmet_py, 0.);
	  float dphi_genmet_dibijet = TVector2::Phi_mpi_pi(genDiBjet_Transv.Phi()-genMet_Transv.Phi());
	  genmet_diBaxis_p = genMet_Transv.Mag()*cos(dphi_genmet_dibijet);
	  genmet_diBaxis_t = genMet_Transv.Mag()*sin(dphi_genmet_dibijet);
	  if (Wtomu1nu1 and Wtomu2nu2){
	    TVector3 nu1and2_V3( (gennu1->P4() + gennu2->P4()).Px(), (gennu1->P4() + gennu2->P4()).Py(), 0.);
	    nu1and2_pt = nu1and2_V3.Pt();
	    nu1and2_px = nu1and2_V3.Px();
	    nu1and2_py = nu1and2_V3.Py();
	    float dphi_nu1and2_dibijet = TVector2::Phi_mpi_pi(genDiBjet_Transv.Phi()-nu1and2_V3.Phi());
	    nu1and2_diBaxis_p  =  nu1and2_V3.Mag()*cos(dphi_nu1and2_dibijet);
	    nu1and2_diBaxis_t  =  nu1and2_V3.Mag()*sin(dphi_nu1and2_dibijet);
	  }
	}
    }
    if(debug_) cout<<"DEBUG::2"<<endl;

    // Loop over all Muon in the event, reco muon
    for(int i = 0; i < branchMuon->GetEntriesFast(); ++i){
	Muon *muon = (Muon*) branchMuon->At(i);
	//for(int i = 0; i < branchMuonBeforeIso->GetEntriesFast(); ++i){//now we use muon bfore Isolation
	//	Muon *muon = (Muon*) branchMuonBeforeIso->At(i);
	if (muon->Charge<0 and abs(muon->Eta)<muonsEta_ and muon->PT >= muonPt1_) 
	  allMuon1.push_back(muon);
	else if(muon->Charge>0 and abs(muon->Eta)<muonsEta_ and muon->PT >= muonPt1_) 
	  allMuon2.push_back(muon);
    }
    numLeptons1 = allMuon1.size();
    numLeptons2 = allMuon2.size();

    if (hasMuon1 and hasMuon2 and allMuon1.size()>0 and allMuon2.size()>0){
	//std::cout <<"two muons from selections muon1 pt "<< (allMuon1.at(0))->PT <<" muon2 pt "<< (allMuon2.at(0))->PT << std::endl;
	if (muon1 == allMuon1.at(0)) hasRecoMuon1 = true;
	if (muon2 == allMuon2.at(0)) hasRecoMuon2 = true;
    }

    //special case, choose two reco muons without matching
    if (not(simulation_) and allMuon1.size()>0){
	//cout<<"NOT simulation and take Reco Muon from Pt selection "<<endl;
	if (hasMuon1 and muon1==allMuon1.at(0)) hasRecoMuon1=true;
	else{

	  //cout <<"Not hasRecoMuon1 "<<(hasMuon1?" But hasMuon1 ":" Not hasMuon1 ")<<(muon1==allMuon1.at(0)?"muon1==allMuon1.at(0)":"muon1!=allMuon1.at(0)")<< endl;
	  hasMuon1=true;
	  hasRecoMuon1=false;//this muon is different from the one found by dR association
	  muon1=allMuon1.at(0);
	  if(hasMuon1_beforeIso and Muon1_beforeIso_IsoVar<0.15 and dR_mu1_beforeIso<0.1 and debug_){ 
	    cout <<" has reco Muon1 before Iso , dR " << dR_mu1_beforeIso <<" IsolationVar "<< muon1_beforeIso->IsolationVar << " pt "<< muon1_beforeIso->PT << " eta "<<muon1_beforeIso->Eta <<" charge "<<muon1_beforeIso->Charge <<endl;
	    cout <<"muon1 Pt "<< muon1->PT <<" eta "<< muon1->Eta <<" IsoVar "<< muon1->IsolationVar<<" charge "<<muon1->Charge << endl;
	  }
	}
	Muon1_p4 = muon1->P4();
    }else if (not(simulation_) and allMuon1.size()==0 ){
	hasMuon1 = false;
	hasRecoMuon1 = false;
    }

    if (not(simulation_) and allMuon2.size()>0){
	if (hasMuon2 and muon2==allMuon2.at(0)) hasRecoMuon2=true;
	else{
	  //cout <<"Not hasRecoMuon2 "<<(hasMuon2?" But hasMuon2 ":" Not hasMuon2 ")<<(muon2==allMuon2.at(0)?"muon2==allMuon2.at(0)":"muon2!=allMuon2.at(0)")<< endl;
	  hasMuon2=true;
	  hasRecoMuon2=false;
	  muon2=allMuon2.at(0);
	  if(hasMuon2_beforeIso and Muon2_beforeIso_IsoVar<0.15 and dR_mu2_beforeIso<0.1 and debug_){ 
	    cout <<" has reco Muon2 before Iso , dR " << dR_mu2_beforeIso <<" IsolationVar "<< muon2_beforeIso->IsolationVar << " pt "<< muon2_beforeIso->PT << " eta "<<muon2_beforeIso->Eta <<" charge "<< muon2_beforeIso->Charge << endl;
	    cout <<"muon2 Pt "<< muon2->PT <<" eta "<< muon2->Eta <<" IsoVar "<< muon2->IsolationVar<<" charge "<<muon2->Charge << endl;
	  }
	}

	Muon2_p4 = muon2->P4();
    }else if (not(simulation_) and allMuon2.size()==0 ){
	hasMuon2 = false;
	hasRecoMuon2 = false;
    }

    float muon1_weight =1.0;
    float muon2_weight =1.0;
    if (hasMuon1) muon1_weight =GetMuonRecoWeight(MuonRecoEff_hist, muon1->PT, fabs(muon1->Eta)); 
    if (hasMuon2) muon2_weight =GetMuonRecoWeight(MuonRecoEff_hist, muon2->PT, fabs(muon2->Eta)); 
    reweighting = muon1_weight * muon2_weight;
    //loop all reco jets 
    int NJet=0;
    //if (simulation_ and hasRECOjet1 and hasRECOjet2) cout<<"--------------"<<endl;
    for (int i =0;  i < branchJet->GetEntries(); i++){
	Jet* jet = (Jet*) branchJet->At(i);
	if (jet->PT < bjetsPt_ || abs(jet->Eta) > bjetsEta_) continue;
	NJet++;
	totjets_lorentz +=jet->P4();
	if (hasMuon1 and hasMuon2){
	  float mindR_bl = min(Muon1_p4.DeltaR(jet->P4()), Muon2_p4.DeltaR(jet->P4()));
	  //cout <<" reco jet pt "<< jet->PT <<" eta "<< jet->Eta <<" dR(jet,muon1) "<< Muon1_p4.DeltaR(jet->P4())<<" dR(jet,muon2) "<< Muon2_p4.DeltaR(jet->P4()) <<" mindR_bl "<< mindR_bl <<endl; 
	  if (mindR_bl<0.3) continue;
	}
	//bit1: loose btag, bit2: medium btag, bit3: tight. current sample: only medium btag implemented
	//if (((jet->BTag)&2)<1 || jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
	alljets.push_back(jet);
    }
    numjets = alljets.size();
    int b1at = -1, b2at = -1;
    std::vector< std::pair<int,int> > pairs;
    if (alljets.size() >= 2){
	for (int k=0; k<int(alljets.size())-1; k++){
	  for (int j=k+1; j<int(alljets.size()); j++){ 
	    TLorentzVector bjets_p4 = alljets[k]->P4()+alljets[j]->P4();
	    b1at = k;
	    b2at = j;
	    if (fabs(bjets_p4.M()-125)<minMass)
		minMass = fabs(bjets_p4.M()-125);
	    //########
	    if (fabs(bjets_p4.M()-125)<40 or (alljets[k]->BTag&2)>0 or (alljets[j]->BTag&2)>0){
		std::pair <int,int> foo;
		foo=std::make_pair(j,k);
		pairs.push_back(foo);
	    }
	    //########
	    //if (simulation_ and hasRECOjet1 and hasRECOjet2){
	    //float DR1k = TLorentzVector(genb1->P4()).DeltaR( TLorentzVector(alljets[k]->P4()) );
	    //float DR1j = TLorentzVector(genb1->P4()).DeltaR( TLorentzVector(alljets[j]->P4()) );
	    //float DR2k = TLorentzVector(genb2->P4()).DeltaR( TLorentzVector(alljets[k]->P4()) );
	    //float DR2j = TLorentzVector(genb2->P4()).DeltaR( TLorentzVector(alljets[j]->P4()) );
	    //float DR1 = DR1k < DR1j ? DR1k : DR1j;
	    //float DR2 = DR1==DR1k ? DR2j : DR2k;
	    //cout<<" -> "<<k<<" and "<<j<<" give "<<bjets_p4.M()<<" and DR is "<<DR1<<" "<<DR2<<endl;
	    //}
	  }
	}
    }
    if( int(pairs.size())>1 ){
	float ptmin=0.;
	int nbtagmin_medium =-1;
	int nbtagmin_loose = -1;
	for(int i=0; i<int(pairs.size());i++){
	  float PtSum = alljets[pairs[i].first]->P4().Pt()+alljets[pairs[i].second]->P4().Pt();
	  int nbtag_medium = ((alljets[pairs[i].first]->BTag&2)>0 ? 1:0)+((alljets[pairs[i].second]->BTag&2)>0 ? 1:0);
	  int nbtag_loose = ((alljets[pairs[i].first]->BTag&1)>0 ? 1:0)+((alljets[pairs[i].second]->BTag&1)>0 ? 1:0);
	  if( nbtag_medium>nbtagmin_medium or (nbtag_medium==nbtagmin_medium and nbtag_loose>nbtagmin_loose) or 
		(nbtag_medium==nbtagmin_medium and nbtag_loose==nbtagmin_loose and PtSum>ptmin)){ 
	    b2at = pairs[i].first; b1at = pairs[i].second; 
	    ptmin = PtSum; nbtagmin_medium=nbtag_medium; nbtagmin_medium=nbtag_loose;
	  }
	}
    }
    //Try also to choose the 2 with the greatest Pt
    int Ptb1at = -1, Ptb2at = -1;
    float MinPt = jetsPt_;
    if (alljets.size() >= 2){
	for (int k=0; k<int(alljets.size()); k++){
	  if ( alljets[k]->P4().Pt()>MinPt ){
	    MinPt = alljets[k]->P4().Pt();
	    Ptb1at = k;
	  }
	}
	MinPt = jetsPt_;
	for (int k=0; k<int(alljets.size()); k++){
	  if ( alljets[k]->P4().Pt()>MinPt && k!=Ptb1at ){
	    MinPt = alljets[k]->P4().Pt();
	    Ptb2at = k;
	  }
	}
    }
    //N B-Jet
    int NumBJet=0;
    for (unsigned k=0; k<alljets.size(); k++){ if( ((alljets[k]->BTag)&2)>=1 ) NumBJet++; }
    numbjets = NumBJet;
    if (hasRECOjet1 and hasRECOjet2 and b1at!=-1 and b2at!=-1 ){
	//cout<<"check jet from dR association and jets from invariant mass selectin "<<endl;
	//cout<<"Jets are: "<<alljets.size()<<endl;
	//cout<<"THIS: "<<alljets.at(b1at)->P4().Pt()<<" "<<alljets.at(b2at)->P4().Pt()<<endl;
	//cout<<"CORR: "<<b1jet->P4().Pt()<<" "<<b2jet->P4().Pt()<<endl;
	//if(( b1jet->P4().Pt()!=alljets.at(b1at)->P4().Pt() && b1jet->P4().Pt()!= alljets.at(b2at)->P4().Pt() ) ||
	//   ( b2jet->P4().Pt()!=alljets.at(b1at)->P4().Pt() && b2jet->P4().Pt()!= alljets.at(b2at)->P4().Pt() ) ) cout<<"WROONG"<<endl;
	hasRecob1jet = true;
	hasRecob2jet = true;
	if (b1jet == alljets.at(b1at) or b1jet==alljets.at(b2at) ) hasRecob1jetCorr = true;
	if (b2jet == alljets.at(b1at) or b2jet==alljets.at(b2at) ) hasRecob2jetCorr = true;
	if( ((alljets.at(b1at)->BTag)&2)<1 || ((alljets.at(b2at)->BTag)&2)<1 ) hasRecob1b2jet_bTag = false;
	else hasRecob1b2jet_bTag = true;
	//Resol this
	if( fabs(alljets.at(b1at)->P4().Pt()-genb1->P4().Pt()) < fabs(alljets.at(b2at)->P4().Pt()-genb1->P4().Pt()) ){
	  reconMassJet1_PtRes = alljets.at(b1at)->P4().Pt()-genb1->P4().Pt(); 
	  reconMassJet2_PtRes = alljets.at(b2at)->P4().Pt()-genb2->P4().Pt();
	}
	else{
	  reconMassJet1_PtRes = alljets.at(b2at)->P4().Pt()-genb1->P4().Pt();
	  reconMassJet2_PtRes = alljets.at(b1at)->P4().Pt()-genb2->P4().Pt();
	}
	//Resol DR
	if( fabs(b1jet->P4().Pt()-genb1->P4().Pt()) < fabs(b2jet->P4().Pt()-genb1->P4().Pt()) ){
	  reconDRJet1_PtRes = b1jet->P4().Pt()-genb1->P4().Pt();
	  reconDRJet2_PtRes = b2jet->P4().Pt()-genb2->P4().Pt();
	}
	else{
	  reconDRJet1_PtRes = b2jet->P4().Pt()-genb1->P4().Pt();
	  reconDRJet2_PtRes = b1jet->P4().Pt()-genb2->P4().Pt();
	}
    }
    if (hasRECOjet1 and hasRECOjet2 and Ptb1at!=-1 and Ptb2at!=-1 ){
	//cout<<"check jet from dR association and jets from pt selectin "<<endl;
	hasRecob1jetPt = true;
	hasRecob2jetPt = true;
	if (b1jet == alljets.at(Ptb1at) or b1jet==alljets.at(Ptb2at) ) hasRecob1jetPtCorr = true;
	if (b2jet == alljets.at(Ptb1at) or b2jet==alljets.at(Ptb2at) ) hasRecob2jetPtCorr = true;
	if( ((alljets.at(Ptb1at)->BTag)&2)<1 || ((alljets.at(Ptb2at)->BTag)&2)<1 ) hasRecob1b2jetPt_bTag = false;
	else hasRecob1b2jetPt_bTag = true;
    }
    // special case, bjet not associated to genjet
    if ( not(simulation_) and b1at!=-1 and b2at!=-1){
	//if ( !simulation_ and Ptb1at!=-1 and Ptb2at!=-1){

	//cout<<"NOT simulation and take jets from invariant mass selectin "<<endl;
	b1jet = alljets.at(b1at);
	b2jet = alljets.at(b2at);
	//b1jet = alljets.at(Ptb1at);
	//b2jet = alljets.at(Ptb2at);
	hasRECOjet1=true;
	hasRECOjet2=true; 
	b1jet_p4 = b1jet->P4();
	b2jet_p4 = b2jet->P4();
	if( ((b1jet->BTag)&2)<1 ) hasb1jet = false;
	else
	  hasb1jet = true;
	if( ((b2jet->BTag)&2)<1 ) hasb2jet = false;
	else
	  hasb2jet = true;

	/*if (b1jet_p4.M()<0) {
	  if (simulation_)
	  cerr <<" b1 parton mass "<< b1_p4.M(); //b1_p4.Print();
	  cerr <<" b1jet mass from p4 "<< b1jet_p4.M(); b1jet_p4.Print();
	  cerr <<" b1jet mass " << b1jet->Mass << endl;
	  }
	  if (b2jet_p4.M()<0) {
	  if (simulation_)
	  cerr <<" b2 parton mass "<< b2_p4.M(); //b2_p4.Print();
	  cerr <<" b2jet mass from p4 "<< b2jet_p4.M(); b2jet_p4.Print();
	  cerr <<" b2jet mass " << b2jet->Mass << endl;
	  }*/
    }else if (not(simulation_) and (b1at == -1 or b2at == -1)){
	hasRECOjet1 = false;
	hasRECOjet2 = false;
    }

    //check whether two jets are closest to b parton
    if (simulation_ and (h2tohh or ttbar))
	checkBjets2Gen(branchGenJet, branchJet, genb1, genb2, jetsDeltaR_); 
    if(debug_) cout<<"DEBUG::3"<<endl;
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
	if (Met_p4.Px() == -999 or Met_p4.Py()==-999) cout <<" wired Reco MET  in x "<<Met_p4.Px()<<" y "<<Met_p4.Py() << endl;
	//MET on Di-BJet Axis
	if( hasgenb1jet and hasgenb2jet and hasRECOjet1 and hasRECOjet2 ){
	  //if( hasgenb1jet and hasgenb2jet and hasb1jet and hasb2jet ){
	  TVector3 genDiBjet_Transv( (genb1jet_p4 + genb2jet_p4).Px(), (genb1jet_p4 + genb2jet_p4).Py(), 0.);
	  TVector3 genMet_Transv( genmet_px, genmet_py, 0.);
	  TVector3 Met_Transv( met_px, met_py, 0.);
	  float dphi_met_dibijet = TVector2::Phi_mpi_pi(genDiBjet_Transv.Phi()-genMet_Transv.Phi());
	  //float dphi_met_dibijet = TVector2::Phi_mpi_pi(genDiBjet_Transv.Phi()-Met_Transv.Phi());
	  met_diBaxis_p = Met_Transv.Mag()*cos(dphi_met_dibijet);
	  met_diBaxis_t = Met_Transv.Mag()*sin(dphi_met_dibijet);
	  //Slide rescale
	  b1rescalefactor = 1.;
	  b2rescalefactor = 1.;
	  SlideRescale();
	  TLorentzVector b1jet_p4_sorted = (b1jet_p4.Pt() > b2jet_p4.Pt()) ? b1jet_p4 : b2jet_p4;
	  TLorentzVector b2jet_p4_sorted = (b1jet_p4.Pt() > b2jet_p4.Pt()) ? b2jet_p4 : b1jet_p4;
	  float metpx_correction = Met_Transv.Px()-(b1rescalefactor-1)*b1jet_p4_sorted.Px()-(b2rescalefactor-1)*b2jet_p4_sorted.Px();
	  float metpy_correction = Met_Transv.Py()-(b1rescalefactor-1)*b1jet_p4_sorted.Py()-(b2rescalefactor-1)*b2jet_p4_sorted.Py();
	  TVector3 met_vec2(metpx_correction, metpy_correction, 0.);
	  met_c1_px = metpx_correction;
	  met_c1_py = metpy_correction;
	  met_diBaxis_c1_p = met_vec2.Mag()*cos(dphi_met_dibijet);
	  met_diBaxis_c1_t = met_vec2.Mag()*sin(dphi_met_dibijet);
	}
	}


	if(debug_) cout<<"DEBUG::4"<<endl;
	// clear-up cuts at gen level
	if (h2tohh or ttbar){
	  dR_genbl   = (b1_p4.Pt()>b2_p4.Pt()) ? (b1_p4.DeltaR( (mu1_p4.Pt()>mu2_p4.Pt()) ? mu1_p4 : mu2_p4 )) : (b2_p4.DeltaR( (mu1_p4.Pt()>mu2_p4.Pt()) ? mu1_p4 : mu2_p4 ));
	  dR_genb1l1 = b1_p4.DeltaR(mu1_p4);
	  dR_genb1l2 = b1_p4.DeltaR(mu2_p4);
	  dR_genb2l1 = b2_p4.DeltaR(mu1_p4);
	  dR_genb2l2 = b2_p4.DeltaR(mu2_p4);
	  dR_genb1b2 = b1_p4.DeltaR(b2_p4);
	  dR_genl1l2 = mu1_p4.DeltaR(mu2_p4);
	  dR_genl1l2b1b2 = (mu1_p4+mu2_p4).DeltaR(b1_p4+b2_p4);
	  dphi_genl1l2b1b2 = TVector2::Phi_mpi_pi( (mu1_p4+mu2_p4).Phi()-(b1_p4+b2_p4).Phi() );
	  TLorentzVector genll_p4 = mu1_p4+mu2_p4;
	  TLorentzVector genbb_p4 = b1_p4+b2_p4;
	  dR_genminbl = min(min(dR_genb1l1,dR_genb1l2),min(dR_genb2l1,dR_genb2l2));
	  dphi_genllbb = TVector2::Phi_mpi_pi(genll_p4.Phi()-genbb_p4.Phi());
	  dphi_genllmet = TVector2::Phi_mpi_pi(genll_p4.Phi()-genmet_phi);
	  mass_genl1l2 = genll_p4.M(); energy_genl1l2 = genll_p4.Energy(); pt_genl1l2 = genll_p4.Pt(); eta_genl1l2 = genll_p4.Eta(); phi_genl1l2 = genll_p4.Phi();
	  mass_genb1b2 = genbb_p4.M(); energy_genb1b2 = genbb_p4.Energy(); pt_genb1b2 = genbb_p4.Pt(); eta_genb1b2 = genbb_p4.Eta(); phi_genb1b2 = genbb_p4.Phi();
	  mass_gentrans = sqrt(2*genll_p4.Pt()*genmet*(1-cos(dphi_genllmet)));
	}

	//calculate additional variables for clearing up cuts and other studies
	if (hasRECOjet1 and hasRECOjet2 and hasMuon1 and hasMuon2){
	  dR_bl   = (b1jet_p4.Pt()>b2jet_p4.Pt()) ? (b1jet_p4.DeltaR( (Muon1_p4.Pt()>Muon2_p4.Pt()) ? Muon1_p4 : Muon2_p4 )) : (b2jet_p4.DeltaR( (Muon1_p4.Pt()>Muon2_p4.Pt()) ? Muon1_p4 : Muon2_p4 ));
	  dR_b1l1 = b1jet_p4.DeltaR(Muon1_p4);
	  dR_b1l2 = b1jet_p4.DeltaR(Muon2_p4);
	  dR_b2l1 = b2jet_p4.DeltaR(Muon1_p4);
	  dR_b2l2 = b2jet_p4.DeltaR(Muon2_p4);
	  dR_b1b2 = b1jet_p4.DeltaR(b2jet_p4);
	  dR_l1l2 = Muon1_p4.DeltaR(Muon2_p4);
	  dR_l1l2b1b2 = (Muon1_p4+Muon2_p4).DeltaR(b1jet_p4+b2jet_p4);
	  dphi_l1l2b1b2 = fabs(TVector2::Phi_mpi_pi( (Muon1_p4+Muon2_p4).Phi()-(b1jet_p4+b2jet_p4).Phi() ));
	  TLorentzVector ll_p4 = Muon1_p4+Muon2_p4;
	  TLorentzVector bjets_p4 = b1jet_p4+b2jet_p4;
	  dR_minbl = min(min(dR_b1l1,dR_b1l2),min(dR_b2l1,dR_b2l2));
	  dphi_llbb = TVector2::Phi_mpi_pi(ll_p4.Phi()-bjets_p4.Phi());
	  dphi_llmet = TVector2::Phi_mpi_pi(ll_p4.Phi()-met_phi);
	  mass_l1l2 = ll_p4.M(); energy_l1l2 = ll_p4.Energy(); pt_l1l2 = ll_p4.Pt(); eta_l1l2 = ll_p4.Eta(); phi_l1l2 = ll_p4.Phi();
	  mass_b1b2 = bjets_p4.M(); energy_b1b2 = bjets_p4.Energy(); pt_b1b2 = bjets_p4.Pt(); eta_b1b2 = bjets_p4.Eta(); phi_b1b2 = bjets_p4.Phi();
	  mass_trans = sqrt(2*ll_p4.Pt()*met*(1-cos(dphi_llmet)));
	  if (dR_b1l1 > jetleptonDeltaR_ and dR_b1l2 > jetleptonDeltaR_ and dR_b2l1 > jetleptonDeltaR_ and dR_b2l2 > jetleptonDeltaR_) hasdRljet =true;
	  MINdR_bl = dR_b1l1*(dR_b1l1<dR_b1l2 && dR_b1l1<dR_b2l1 && dR_b1l1<dR_b2l2) + dR_b2l1*(dR_b2l1<dR_b2l2 && dR_b2l1<dR_b1l1 && dR_b2l1<dR_b1l2) + dR_b1l2*(dR_b1l2<dR_b1l1 && dR_b1l2<dR_b2l1 && dR_b1l2<dR_b2l2) + dR_b2l2*(dR_b2l2<dR_b1l1 && dR_b2l2<dR_b1l2 && dR_b2l2<dR_b2l1);
	  //MT2: In order to construct MT2 for either the t tbar -> bW bW system or our signal H -> h h -> bb WW,
	  //we group each pair b_jet-lepton into an object: we then get "Particle A" and "Particle B" (each one given by a b_jet-lepton object),
	  //whose Kinematics is to be fed into the MT2 variable.
	  //There are two possible Lepton-Bquark pairings. We compute MT2 for both and pick the smallest value.
	  // Simulation=false: only 0 = RECO jets.
	  // Simulation=true: 0 = RECO jets, 1 = MATCHED jets.
	  for(int i=0; i<2; i++){
	    TLorentzVector B1_mt2,B2_mt2;
	    if(i==0){
		B1_mt2 = b1jet_p4;
		B2_mt2 = b2jet_p4;
	    }
	    if(i==1 and simulation_ and b1at!=-1 and b2at!=-1 and hasRecob1b2jet_bTag){
		B1_mt2 = alljets.at(b1at)->P4();
		B2_mt2 = alljets.at(b2at)->P4();
	    }
	    if( (i==0) || (i==1 && simulation_ && b1at!=-1 && b2at!=-1 and hasRecob1b2jet_bTag) ){
		//-Sorting leptons and b-jets
		TLorentzVector MU_1, MU_2, Bj_1, Bj_2;
		MU_1 = (Muon1_p4.Pt()>Muon2_p4.Pt()) ? Muon1_p4 : Muon2_p4;
		MU_2 = (Muon1_p4.Pt()>Muon2_p4.Pt()) ? Muon2_p4 : Muon1_p4;
		Bj_1 = (B1_mt2.Pt()>B2_mt2.Pt()) ? B1_mt2 : B2_mt2;
		Bj_2 = (B1_mt2.Pt()>B2_mt2.Pt()) ? B2_mt2 : B1_mt2;
		//-Invariant mass for "Particle A"
		float sumesBl1_1=(MU_1.E()+Bj_1.E())*(MU_1.E()+Bj_1.E());
		float sumpxsBl1_1=(MU_1.Px()+Bj_1.Px())*(MU_1.Px()+Bj_1.Px());
		float sumpysBl1_1=(MU_1.Py()+Bj_1.Py())*(MU_1.Py()+Bj_1.Py());
		float sumpzsBl1_1=(MU_1.Pz()+Bj_1.Pz())*(MU_1.Pz()+Bj_1.Pz());
		float M_Bl1_1=sqrt(sumesBl1_1-(sumpxsBl1_1+sumpysBl1_1+sumpzsBl1_1));
		float Pxa_1 = MU_1.Px()+Bj_1.Px();
		float Pya_1 = MU_1.Py()+Bj_1.Py();
		// Invariant mass for "Particle B"
		float sumesBl2_1=(MU_2.E()+Bj_2.E())*(MU_2.E()+Bj_2.E());
		float sumpxsBl2_1=(MU_2.Px()+Bj_2.Px())*(MU_2.Px()+Bj_2.Px());
		float sumpysBl2_1=(MU_2.Py()+Bj_2.Py())*(MU_2.Py()+Bj_2.Py());
		float sumpzsBl2_1=(MU_2.Pz()+Bj_2.Pz())*(MU_2.Pz()+Bj_2.Pz());
		float M_Bl2_1=sqrt(sumesBl2_1-(sumpxsBl2_1+sumpysBl2_1+sumpzsBl2_1));
		float Pxb_1 = MU_2.Px()+Bj_2.Px();
		float Pyb_1 = MU_2.Py()+Bj_2.Py();
		// computation
		double pa1[3] = { M_Bl1_1, Pxa_1, Pya_1 };
		double pb1[3] = { M_Bl2_1, Pxb_1, Pyb_1 };
		double pmiss[3] = { 0, Met_p4.Px(), Met_p4.Py() };
		mt2_bisect::mt2 mt2_event1;
		mt2_event1.set_momenta(pa1,pb1,pmiss);
		mt2_event1.set_mn(0.);
		float MT2_1 = mt2_event1.get_mt2();
		//-Invariant mass for "Particle A 2"
		float sumesBl1_2=(MU_1.E()+Bj_2.E())*(MU_1.E()+Bj_2.E());
		float sumpxsBl1_2=(MU_1.Px()+Bj_2.Px())*(MU_1.Px()+Bj_2.Px());
		float sumpysBl1_2=(MU_1.Py()+Bj_2.Py())*(MU_1.Py()+Bj_2.Py());
		float sumpzsBl1_2=(MU_1.Pz()+Bj_2.Pz())*(MU_1.Pz()+Bj_2.Pz());
		float M_Bl1_2=sqrt(sumesBl1_2-(sumpxsBl1_2+sumpysBl1_2+sumpzsBl1_2));
		float Pxa_2 = MU_1.Px()+Bj_2.Px();
		float Pya_2 = MU_1.Py()+Bj_2.Py();
		// Invariant mass for "Particle B 2"
		float sumesBl2_2=(MU_2.E()+Bj_1.E())*(MU_2.E()+Bj_1.E());
		float sumpxsBl2_2=(MU_2.Px()+Bj_1.Px())*(MU_2.Px()+Bj_1.Px());
		float sumpysBl2_2=(MU_2.Py()+Bj_1.Py())*(MU_2.Py()+Bj_1.Py());
		float sumpzsBl2_2=(MU_2.Pz()+Bj_1.Pz())*(MU_2.Pz()+Bj_1.Pz());
		float M_Bl2_2=sqrt(sumesBl2_2-(sumpxsBl2_2+sumpysBl2_2+sumpzsBl2_2));
		float Pxb_2 = MU_2.Px()+Bj_1.Px();
		float Pyb_2 = MU_2.Py()+Bj_1.Py();
		// computation
		double pa2[3] = { M_Bl1_2, Pxa_2, Pya_2 };
		double pb2[3] = { M_Bl2_2, Pxb_2, Pyb_2 };
		mt2_bisect::mt2 mt2_event2;
		mt2_event2.set_momenta(pa2,pb2,pmiss);
		mt2_event2.set_mn(0.);
		float MT2_2 = mt2_event2.get_mt2();
		if(i==0) MT2 = (MT2_1 < MT2_2) ? MT2_1 : MT2_2;
		if(i==1) MT2_reco = (MT2_1 < MT2_2) ? MT2_1 : MT2_2;
		//	    //NOW WITHOUT MUONS
		//	    //-Invariant mass for "Particle A"
		//	    //sumesBl1_1=(Bj_1.E())*(Bj_1.E());
		//	    //sumpxsBl1_1=(Bj_1.Px())*(Bj_1.Px());
		//	    //sumpysBl1_1=(Bj_1.Py())*(Bj_1.Py());
		//	    //sumpzsBl1_1=(Bj_1.Pz())*(Bj_1.Pz());
		//	    //M_Bl1_1=sqrt(sumesBl1_1-(sumpxsBl1_1+sumpysBl1_1+sumpzsBl1_1));
		//	    //Pxa_1 = Bj_1.Px();
		//	    //Pya_1 = Bj_1.Py();
		//	    //// Invariant mass for "Particle B"
		//	    //sumesBl2_1=(Bj_2.E())*(Bj_2.E());
		//	    //sumpxsBl2_1=(Bj_2.Px())*(Bj_2.Px());
		//	    //sumpysBl2_1=(Bj_2.Py())*(Bj_2.Py());
		//	    //sumpzsBl2_1=(Bj_2.Pz())*(Bj_2.Pz());
		//	    //M_Bl2_1=sqrt(sumesBl2_1-(sumpxsBl2_1+sumpysBl2_1+sumpzsBl2_1));
		//	    //Pxb_1 = Bj_2.Px();
		//	    //Pyb_1 = Bj_2.Py();
		//	    // computation
		//	    double pa1_nomu[3] = { 0., Bj_1.Px(), Bj_1.Py() };
		//	    double pb1_nomu[3] = { 0., Bj_2.Px(), Bj_2.Py() };
		//	    double pmiss_nomu[3]   = { 0,                      Met_p4.Px()+MU_1.Px()+MU_2.Px(), Met_p4.Py()+MU_1.Py()+MU_2.Py() };
		//	    double pmiss_nomuv2[3] = { (MU_1+MU_2).M(),        Met_p4.Px()+MU_1.Px()+MU_2.Px(), Met_p4.Py()+MU_1.Py()+MU_2.Py() };
		//	    double pmiss_nomuv3[3] = { (Met_p4+MU_1+MU_2).M(), Met_p4.Px()+MU_1.Px()+MU_2.Px(), Met_p4.Py()+MU_1.Py()+MU_2.Py() };
		//	    mt2_bisect::mt2 mt2_event1_nomu;
		//	    mt2_event1_nomu.set_momenta(pa1_nomu,pb1_nomu,pmiss_nomu);
		//	    mt2_event1_nomu.set_mn(0.);
		//	    float MT2_1_nomu = mt2_event1_nomu.get_mt2();
		//	    mt2_bisect::mt2 mt2_event1_nomuv2;
		//	    mt2_event1_nomuv2.set_momenta(pa1_nomu,pb1_nomu,pmiss_nomuv2);
		//	    mt2_event1_nomuv2.set_mn((Muon1_p4+Muon2_p4).M());
		//	    float MT2_1_nomuv2 = mt2_event1_nomuv2.get_mt2();
		//	    mt2_bisect::mt2 mt2_event1_nomuv3;
		//	    mt2_event1_nomuv3.set_momenta(pa1_nomu,pb1_nomu,pmiss_nomuv3);
		//	    mt2_event1_nomuv3.set_mn((Met_p4+Muon1_p4+Muon2_p4).M());
		//	    float MT2_1_nomuv3 = mt2_event1_nomuv3.get_mt2();
		//	    if(i==0) MT2_noMU = MT2_1_nomu;
		//	    if(i==0) MT2_noMUv2= MT2_1_nomuv2;
		//	    if(i==0) MT2_noMUv3 = MT2_1_nomuv3;
		//	    if(i==1) MT2_noMU_reco = MT2_1_nomu;
		//	    if(i==1) MT2_noMU_recov2 = MT2_1_nomuv2;
		//	    if(i==1) MT2_noMU_recov3 = MT2_1_nomuv3;
	    }
	  }
	}
	if(debug_) cout<<"DEBUG::5"<<endl;
	fillbranches();
	if ((h2tohh or ttbar) and debug_){
	  cout <<"parton informatin b1 pt "<< b1_pt <<" b1 eta "<< b1_eta <<" b2 pt "<< b2_pt <<" b2 eta "<< b2_eta << endl;
	  cout <<"genjet genb1jet pt "<< genb1jet_pt <<" eta "<< genb1jet_eta <<" dR "<< dR_genb1jet <<" genb2jet pt "<< genb2jet_pt <<" eta "<< genb2jet_eta <<" dR "<< dR_genb2jet << endl;
	  cout <<"jet b1jet pt "<< b1jet_pt <<" eta "<< b1jet_eta <<" dR "<< dR_b1jet <<" b2jet pt "<< b2jet_pt <<" eta "<< b2jet_eta <<" dR "<< dR_b2jet << endl;
	  cout <<"gen muon muon1 pt "<< mu1_pt <<" eta "<< mu1_eta <<" muon2 pt "<< mu2_pt <<" eta "<< mu2_eta << endl;
	  cout <<"reco muon muon1 pt" <<Muon1_pt <<" eta "<< Muon1_eta<<" phi "<< Muon1_phi <<" dR "<< dR_mu1 <<" muon2 pt "<< Muon2_pt <<" eta "<< Muon2_eta <<" phi "<< Muon2_phi <<" dR "<< dR_mu2 << endl;
	  if (dR_genminbl>0.3 and dR_minbl>0 and dR_minbl<0.1)
	    cout <<"warning!!!! dR_genminbl "<< dR_genminbl <<" dR_minbl "<< dR_minbl << endl;
	  cout <<"numGenjet "<< numGenJet <<" numjet "<< numjets << endl;
	}

	if(debug_) cout<<"DEBUG::6"<<endl;
	//-------- Here is possible to compute new variables --------
	if(true){ //Selection to be applied before the computation
	}

	/*if (hastwogenmuons and not(hastwomuons)){
	  cout <<" two gen muons " << endl;
	  cout <<" mu1 "; mu1_p4.Print();
	  cout <<" mu2 "; mu2_p4.Print();
	  for(i = 0; i < branchMuon->GetEntriesFast(); ++i){
	  Muon *muon = (Muon*) branchMuon->At(i);
	  cout <<" Reco Muon eta "<< muon->Eta <<" phi "<< muon->Phi <<" pt "<< muon->PT <<" charge sign "<< muon->Charge << endl;
	  }
	  }*/
	//-------- MMC --------
	preselections = (hasRECOjet1 and hasRECOjet1 and hasMET and hastwomuons and hasdRljet);
	//for simulation case
	preselections_gen = (hasgenb1jet and hasgenb2jet and hastwogenmuons);
	bool MMCready =  ((h2tohh and sample_!=tt) || (ttbar and sample_ ==tt));
	bool objectsready = (((useRecoMuon_ and hastwomuons) or (not(useRecoMuon_) and hastwogenmuons)) and 
	    ((useRecoBJet_ and hasRECOjet1 and hasRECOjet2) or (not(useRecoBJet_) and hasgenb1jet and hasgenb2jet)));
	if (runMMC_ and objectsready and (not(simulation_) || MMCready)){
	  cout <<" start to run MMC for this event " << entry <<endl;
	  TLorentzVector bjet_pt1_lorentz, bjet_pt2_lorentz, bgenp_pt1_lorentz, bgenp_pt2_lorentz;
	  if ((not(simulation_) or useRecoBJet_) and b1jet_p4.Pt()>b2jet_p4.Pt()) {
	    cout <<" use Reco bjet " <<" b1jetpt "<< b1jet_p4.Pt()<< " b2jetpt "<<b2jet_p4.Pt() <<endl;
	    bjet_pt1_lorentz = b1jet_p4; bjet_pt2_lorentz = b2jet_p4;
	  } else if((not(simulation_) or useRecoBJet_)){ 
	    cout <<" use Reco bjet " <<" b2jetpt "<<b2jet_p4.Pt() <<" b1jetpt "<<b1jet_p4.Pt() <<endl;
	    bjet_pt1_lorentz = b2jet_p4; bjet_pt2_lorentz = b1jet_p4;
	  }

	  if (simulation_ and not(useRecoBJet_) and genb1jet_p4.Pt()>genb2jet_p4.Pt()) {
	    cout <<" use Gen bjet " <<" b1genjet pt "<< genb1jet_p4.Pt() <<" b2genjet pt "<< genb2jet_p4.Pt() <<endl;
	    bjet_pt1_lorentz = genb1jet_p4; bjet_pt2_lorentz = genb2jet_p4; }
	  else if (simulation_ and not(useRecoBJet_)) {
	    cout <<" use Gen bjet " <<" b2genjet pt "<< genb2jet_p4.Pt() <<" b1genjet pt "<< genb1jet_p4.Pt()<< endl;
	    bjet_pt1_lorentz = genb2jet_p4; bjet_pt2_lorentz = genb1jet_p4;
	  }

	  if (bjet_pt1_lorentz.M2()<0.00000001 or bjet_pt2_lorentz.M2()<0.00000001){
	    cerr <<" error!! b1jet or b2jet M2() is less 0.0001, b1jet "<<bjet_pt1_lorentz.M2() <<" b2jet "<< bjet_pt2_lorentz.M2()<< endl;
	    //continue;
	  }
	  //always take gen as inputs 
	  if (simulation_ and b1_p4.Pt()>b2_p4.Pt()) {
	    bgenp_pt1_lorentz = b1_p4; bgenp_pt2_lorentz = b2_p4;
	  } else if (simulation_){ 
	    bgenp_pt1_lorentz = b2_p4; bgenp_pt2_lorentz = b1_p4; 
	  }
	  //else {
	  //  bgenp_pt1_lorentz = TLorentzVector(); bgenp_pt2_lorentz = TLorentzVector(); 
	  //}

	  if(debug_) cout<<"DEBUG::8"<<endl;
	  TLorentzVector lepton1_lorentz, lepton2_lorentz;
	  if ( not(simulation_) or useRecoMuon_ ){
	    cout <<" use Reco Muon " << endl;
	    lepton1_lorentz = Muon1_p4; lepton2_lorentz = Muon2_p4;
	    //lepton1_lorentz = Muon1_beforeIso_p4; lepton2_lorentz = Muon2_beforeIso_p4;
	  }else if (not(useRecoMuon_) and simulation_){
	    cout <<" use Gen Muon " << endl;
	    lepton1_lorentz = mu1_p4; lepton2_lorentz = mu2_p4;
	  }


	TLorentzVector Met_lorentz;
	cout <<" use "<<(useRecoMET_?" Reco ":" Gen ") << "MET" << endl;
	if (not(simulation_) or useRecoMET_)	Met_lorentz = Met_p4;
	else if (not(useRecoMET_) and simulation_) Met_lorentz = genmet_p4;

	TLorentzVector h2tohh_genp_lorentz = TLorentzVector();

	//always take the gen inputs for MMC
	if (sample_!=tt and h2tohh) h2tohh_genp_lorentz = genh2->P4();
	else if (sample_==tt and ttbar) h2tohh_genp_lorentz = gent1->P4()+gent2->P4();

	int onshellMarker_;
	if (simulation_ and genW1->Mass > genW2->Mass) onshellMarker_=1;
	else if (simulation_ and genW1->Mass < genW2->Mass) onshellMarker_=2;
	else onshellMarker_=-1;
	// Rescale bjets in MMC?????
	//MMC *thismmc = new MMC();
	if(debug_) cout<<"DEBUG::9"<<endl;
	MMC *thismmc = new MMC(&lepton1_lorentz, &lepton2_lorentz, &bjet_pt1_lorentz, &bjet_pt2_lorentz, &totjets_lorentz, &Met_lorentz, 
	    &nu1_p4, &nu2_p4, &bgenp_pt1_lorentz, &bgenp_pt2_lorentz, &h2tohh_genp_lorentz, 
	    onshellMarker_, simulation_,PUSample_,
	    entry, weightfromonshellnupt_func_, weightfromonshellnupt_hist_, weightfromonoffshellWmass_hist_,
	    iterations_, RefPDFfile_, useMET_, bjetrescaleAlgo_, metcorrection_);
	runMMCok = thismmc->runMMC();
	if (runMMCok) {
	  //MMCtree =  (thismmc->getMMCTree())->CloneTree();
	  stringstream ss;
	  ss <<"MMC_h2mass_event"<< entry;
   	  const std::string histname(ss.str());
	  stringstream ss1;
	  ss1 <<"MMC_h2massweight1_event"<< entry;
   	  const std::string histname1(ss1.str());
	  stringstream ss4;
	  ss4 <<"MMC_h2massweight4_event"<< entry;
   	  const std::string histname4(ss4.str());
	  TH1F* MMC_h2mass =(TH1F*)(thismmc->getMMCh2()).Clone(histname.c_str());
	  TH1F* MMC_h2mass_weight1 =(TH1F*)(thismmc->getMMCh2weight1()).Clone(histname1.c_str());
	  TH1F* MMC_h2mass_weight4 =(TH1F*)(thismmc->getMMCh2weight4()).Clone(histname4.c_str());
	  //std::cout <<" Mass_h2mass in Analyzer " << std::endl;
	  MMC_h2mass_prob = (MMC_h2mass->GetXaxis())->GetBinCenter(MMC_h2mass->GetMaximumBin());
	  MMC_h2massweight1_prob = (MMC_h2mass_weight1->GetXaxis())->GetBinCenter(MMC_h2mass_weight1->GetMaximumBin());
	  MMC_h2massweight4_prob = (MMC_h2mass_weight4->GetXaxis())->GetBinCenter(MMC_h2mass_weight4->GetMaximumBin());
	  MMC_h2mass_RMS = MMC_h2mass->GetRMS();
	  MMC_h2massweight1_RMS = MMC_h2mass_weight1->GetRMS();
	  MMC_h2massweight4_RMS = MMC_h2mass_weight4->GetRMS();
	  MMC_h2mass_Entries = MMC_h2mass->GetEntries();
	  MMC_h2mass_Mean = MMC_h2mass->GetMean();
	  int nbin=(MMC_h2mass->GetXaxis())->GetNbins();
	  MMC_h2mass_overflow = MMC_h2mass->GetBinContent(nbin+1);
	  MMC_h2mass_underflow = MMC_h2mass->GetBinContent(-1);
	  MMC_h2mass_MaxBin = MMC_h2mass->GetBinContent( MMC_h2mass->GetMaximumBin() );
	  MMC_h2mass_weight1_MaxBin = MMC_h2mass_weight1->GetBinContent( MMC_h2mass_weight1->GetMaximumBin() );
	  MMC_h2mass_weight4_MaxBin = MMC_h2mass_weight4->GetBinContent( MMC_h2mass_weight4->GetMaximumBin() );
	  std::cout <<" most prob " << MMC_h2mass_prob <<" RMS "<< MMC_h2mass_RMS << " entries " << MMC_h2mass_Entries 
	    << " most prob weight1 "<< MMC_h2massweight1_prob <<" weight4 "<< MMC_h2massweight4_prob <<std::endl;
	  if (MMC_h2mass_prob < 245 ) {
		std::cerr <<" error !!! MMC_h2mass_prob < 250, MMC_h2mass_prob: " <<MMC_h2mass_prob 
	    		<<" eventid "<<entry <<std::endl;
		MMC_h2mass->Print("ALL");
	  }
	  if (MMC_h2massweight1_prob < 245 ) {
		std::cerr <<" error !!! MMC_h2mass_prob < 250, MMC_h2massweight1_prob: " <<MMC_h2massweight1_prob 
	    		<<" eventid "<<entry <<std::endl;
		MMC_h2mass_weight1->Print("ALL");
	  }
	 //MMC_h2mass->Write();
	 //MMC_h2mass_weight1->Write();
	 //MMC_h2mass_weight4->Write();
	  //##!! NOT ALL HISTOS file->WriteObject(MMC_h2mass, histname.c_str());
	}
  	//std::cout <<"gFile get name "<< gFile->GetName() <<" gFile get options " << gFile->GetOption() << std::endl;
	delete thismmc;
    }
	//MVA
	if (hasRECOjet1 and hasRECOjet2 and hasMuon1 and hasMuon2 and MMC_h2massweight1_prob>0){
	  if(runMVA_){
	    MVA_dR_l1l2 = dR_l1l2; MVA_dR_b1b2=dR_b1b2; MVA_dR_bl=dR_bl; MVA_dR_l1l2b1b2 = dR_l1l2b1b2; MVA_MINdR_bl = MINdR_bl; MVA_dphi_l1l2b1b2 = dphi_l1l2b1b2; MVA_mass_l1l2=mass_l1l2; MVA_mass_b1b2=mass_b1b2; MVA_mass_trans = mass_trans;  MVA_MT2 = MT2; MVA_pt_b1b2 = pt_b1b2; MVA_MMC_h2massweight1_prob = MMC_h2massweight1_prob;
	    if(sample_!=tt) MVA_value = reader->EvaluateMVA( Method.Data() );
	    //Using also B6 MVA
	    if(sample_==tt){
		MVA_value_B1fortt  = reader_ttB1->EvaluateMVA( Method_ttB1.Data() );
		MVA_value_B2fortt  = reader_ttB2->EvaluateMVA( Method_ttB2.Data() );
		MVA_value_B3fortt  = reader_ttB3->EvaluateMVA( Method_ttB3.Data() );
		MVA_value_B4fortt  = reader_ttB4->EvaluateMVA( Method_ttB4.Data() );
		MVA_value_B5fortt  = reader_ttB5->EvaluateMVA( Method_ttB5.Data() );
		MVA_value_B6fortt  = reader_ttB6->EvaluateMVA( Method_ttB6.Data() );
		MVA_value_B7fortt  = reader_ttB7->EvaluateMVA( Method_ttB7.Data() );
		MVA_value_B8fortt  = reader_ttB8->EvaluateMVA( Method_ttB8.Data() );
		MVA_value_B9fortt  = reader_ttB9->EvaluateMVA( Method_ttB9.Data() );
		MVA_value_B10fortt = reader_ttB10->EvaluateMVA( Method_ttB10.Data() );
		MVA_value_B11fortt = reader_ttB11->EvaluateMVA( Method_ttB11.Data() );
		MVA_value_B12fortt = reader_ttB12->EvaluateMVA( Method_ttB12.Data() );
	    }
	  }
	}
	//if (runMMCok or preselections_gen or preselections) {
	if (h2tohh or ttbar or objectsready) {
	  //if (htobb) {
	  if(debug_) cout<<"DEBUG::10, fill the tree"<<endl;
	  evtree->Fill();
	}
	//if (not(runMMC_) and (h2tohh or ttbar)) evtree->Fill();
	//evtree->Fill();
	}//for(entry = nStarts_; entry < totalevents; ++entry)
}
    


void DiHiggstoWWbb::fillbranches(){

	if (hasRECOjet1){
	  b1jet_btag = b1jet->BTag;
	  b1jet_px = b1jet_p4.Px(); b1jet_py = b1jet_p4.Py(); b1jet_pz= b1jet_p4.Pz(); b1jet_energy = b1jet_p4.Energy();
	  b1jet_eta = b1jet_p4.Eta(); b1jet_phi = b1jet_p4.Phi(); b1jet_pt = b1jet_p4.Pt();
	  b1jet_mass = b1jet->Mass;
	  //if (b1jet->Mass <0 ) cerr <<" error, b1jet mass is less than 0, b1jet mass "<< b1jet->Mass << endl; 
	  //if (htobb and not(simulation_))  dR_b1jet = (b1jet_p4.DeltaR(b1_p4)>b1jet_p4.DeltaR(b2_p4))? b1jet_p4.DeltaR(b2_p4):b1jet_p4.DeltaR(b1_p4);
	}
	if (hasRECOjet2){
	  b2jet_btag = b2jet->BTag;
	  b2jet_px = b2jet_p4.Px(); b2jet_py = b2jet_p4.Py(); b2jet_pz= b2jet_p4.Pz(); b2jet_energy = b2jet_p4.Energy();
	  b2jet_eta = b2jet_p4.Eta(); b2jet_phi = b2jet_p4.Phi(); b2jet_pt = b2jet_p4.Pt();
	  b2jet_mass = b2jet->Mass;
	  //if (b2jet->Mass <0 ) cerr <<" error, b1jet mass is less than 0, b2jet mass "<<b2jet->Mass  << endl; 
	  //if (htobb and not(simulation_))  dR_b2jet = (b2jet_p4.DeltaR(b1_p4)>b2jet_p4.DeltaR(b2_p4))? b2jet_p4.DeltaR(b2_p4):b2jet_p4.DeltaR(b1_p4);
	}

	if (hasMuon1){
	  Muon1_px = muon1->P4().Px(); Muon1_py = muon1->P4().Py(); Muon1_pz = muon1->P4().Pz(); Muon1_energy = muon1->P4().E();
	  Muon1_eta = muon1->Eta; Muon1_phi = muon1->Phi; Muon1_pt = muon1->PT;
	  Muon1_IsoVar = muon1->IsolationVar;
	  if (Wtomu1nu1) dR_mu1 = Muon1_p4.DeltaR(genmu1->P4());
	  //std::cout <<" Muon1 eta "<< muon1->Eta <<" phi "<< muon1->Phi <<" pt "<< muon1->PT << std::endl;
	}
	if (hasMuon2){
	  Muon2_px = muon2->P4().Px(); Muon2_py = muon2->P4().Py(); Muon2_pz = muon2->P4().Pz(); Muon2_energy = muon2->P4().E();
	  Muon2_eta = muon2->Eta; Muon2_phi = muon2->Phi; Muon2_pt = muon2->PT;
	  Muon2_IsoVar = muon2->IsolationVar;
	  if (Wtomu2nu2) dR_mu2 = Muon2_p4.DeltaR(genmu2->P4());
	  //	std::cout <<" Muon2 eta "<< muon2->Eta <<" phi "<< muon2->Phi <<" pt "<< muon2->PT << std::endl;
	}

	if (hasMuon1 && hasMuon2){
	  if (((muon1->PT > muonPt1_ && muon2->PT > muonPt2_) || (muon1->PT > muonPt2_ && muon2->PT > muonPt1_)) 
		&& fabs(muon1->Eta)<muonsEta_ && fabs(muon2->Eta)< muonsEta_) hastwomuons =true;
	}

    }


    
void DiHiggstoWWbb::readoutMuonRecoEff(){

	TFile *f_tmp = TFile::Open(RefPDFfile_.c_str());
	if (f_tmp->IsZombie()) cout << "Error opening file" << endl;
	TH2F* muonrecoeff = (TH2F*)f_tmp->Get("muonrecoeffhist");
	int nx = muonrecoeff->GetNbinsX();
	int ny = muonrecoeff->GetNbinsY();
	float x0 = muonrecoeff->GetXaxis()->GetBinCenter(1)-muonrecoeff->GetXaxis()->GetBinWidth(1)/2.;
	float x1 = muonrecoeff->GetXaxis()->GetBinCenter(nx)+muonrecoeff->GetXaxis()->GetBinWidth(1)/2.;
	float y0 = muonrecoeff->GetYaxis()->GetBinCenter(1)-muonrecoeff->GetYaxis()->GetBinWidth(1)/2.;
	float y1 = muonrecoeff->GetYaxis()->GetBinCenter(ny)+muonrecoeff->GetYaxis()->GetBinWidth(1)/2.;
	//std::cout <<"nx "<< nx<<" x0 "<<x0 <<" x1 "<< x1 <<" ny "<< ny <<" y0 "<< y0 <<" y1 "<< y1 << std::endl;
	MuonRecoEff_hist->SetBins(nx,x0,x1,ny,y0,y1);
	for (int i=0; i<nx; i++){
	  for(int j=0; j<ny; j++){
	    float content = muonrecoeff->GetBinContent(i,j);
	    //std::cout <<" i "<< i <<" j "<<j <<" content "<< content << std::endl;
	    MuonRecoEff_hist->SetBinContent(i,j,content);
	  }
	} 
	f_tmp->Close();

    }

    
float DiHiggstoWWbb::GetMuonRecoWeight(TH2F *hist, float pt, float eta){


	float weight = 0.0;
	if (eta<0) eta = eta*(-1.0);
	int bin1 = hist->GetXaxis()->FindBin(eta);
	int bin2 = hist->GetYaxis()->FindBin(pt);
	//first make sure that x is within range
	if (bin1 == 0 || bin1 == hist->GetNbinsX()+1) return weight=0;
	if (bin2 == 0 || bin2 == hist->GetNbinsY()+1) return weight=0;

	float delphesEff = hist->GetBinContent(bin1, bin2);
	//float CMSEff = (pt>100?0.99:0.99*(1-exp(-pt/10.0)/4));
	float CMSEff = 0;
	if (fabs(eta)<0.9) {
	  if (pt>=10 && pt<25) CMSEff=0.92;
	  else if (pt>=25 && pt<30) CMSEff=0.95;
	  else if (pt>=30 && pt<35) CMSEff=0.97;
	  else if (pt>=35 && pt<40) CMSEff=0.98;
	  else if (pt>=40) CMSEff=0.99;
	}
	else if (fabs(eta)<2.1 && fabs(eta)>=0.9){
	  if (pt>=10 && pt<25) CMSEff=0.93;
	  else if (pt>=25 && pt<30) CMSEff=0.95;
	  else if (pt>=30 && pt<35) CMSEff=0.97;
	  else if (pt>=35) CMSEff=0.99;
	}
	else if (fabs(eta)>=2.1 && fabs(eta)<=2.4){
	  if (pt>=10 && pt<25) CMSEff=0.95;
	  else if (pt>=25 && pt<30) CMSEff=0.97;
	  else if (pt>=30 && pt<35) CMSEff=0.98;
	  else if (pt>=35) CMSEff=0.99;

	}//CMeff are found from CMS muon reco performance 
	else {cout <<"Error!! muon is not within fiducial "<< endl;}

	if(delphesEff == float(0)) weight =1.0;
	else 
	  weight = CMSEff/delphesEff; 
	//cout <<"Muon pt "<< pt <<" eta "<< eta <<" delphesEff "<< delphesEff <<" CMSEff "<< CMSEff <<" final weight "<<weight <<endl;
	return weight;
    }


    
void DiHiggstoWWbb::GetPDFc1(){
	v_bjetrescalec1pdf.clear();
	TFile *f_tmp = TFile::Open(RefPDFfile_.c_str());
	if (f_tmp->IsZombie()) cout << "Error opening file" << endl;
	TH1F *bjetrescalec1pdf = (TH1F*)f_tmp->Get("bjetrescalec1dR4pdf");
	for(int i=0;i<bjetrescalec1pdf->GetNbinsX(); i++){
	  v_bjetrescalec1pdf.push_back( bjetrescalec1pdf->GetBinContent(i+1) );
	}
	v_bjetrescalec1pdf.push_back( bjetrescalec1pdf->GetXaxis()->GetBinCenter(1)-bjetrescalec1pdf->GetBinWidth(1)/2. );
	v_bjetrescalec1pdf.push_back( bjetrescalec1pdf->GetXaxis()->GetBinCenter(bjetrescalec1pdf->GetNbinsX())+bjetrescalec1pdf->GetBinWidth(1)/2. );
	f_tmp->Close();
    }

    void DiHiggstoWWbb::SlideRescale(){
	TH1F *histo = new TH1F("histo", "", v_bjetrescalec1pdf.size(), v_bjetrescalec1pdf[v_bjetrescalec1pdf.size()-2], v_bjetrescalec1pdf[v_bjetrescalec1pdf.size()-1]);
	for(unsigned int i=0;i<v_bjetrescalec1pdf.size()-2; i++){
	  histo->SetBinContent(i+1,v_bjetrescalec1pdf[i]);
	}
	float rescalec1 = histo->GetRandom();
	TLorentzVector b1lorentz, b2lorentz;
	TLorentzVector b1jet_p4_sorted = (b1jet_p4.Pt() > b2jet_p4.Pt()) ? b1jet_p4 : b2jet_p4;
	TLorentzVector b2jet_p4_sorted = (b1jet_p4.Pt() > b2jet_p4.Pt()) ? b2jet_p4 : b1jet_p4;
	if (b1jet_p4_sorted.Pt() > b2jet_p4_sorted.Pt()){
	  b1lorentz = b1jet_p4_sorted;
	  b2lorentz = b2jet_p4_sorted;
	}
	else {
	  b1lorentz = b2jet_p4_sorted;
	  b2lorentz = b1jet_p4_sorted;
	}
	//x1*c2*c2+x2*c2+x3=0, slove for c2
	float x1 = b2lorentz.M2();
	float x2 = 2*rescalec1*(b1lorentz*b2lorentz);
	float x3 = rescalec1*rescalec1*b1lorentz.M2()-125*125;
	if (x2<0) std::cout <<"error bjets lorentzvector dot productor less than 0: " << x2 << std::endl;
	float rescalec2 = (-x2+std::sqrt(x2*x2-4*x1*x3))/(2*x1);

	if (b1jet_p4_sorted.Pt() > b2jet_p4_sorted.Pt()){
	  b1rescalefactor = rescalec1;
	  b2rescalefactor = rescalec2;
	}else{
	  std::cout <<"wired b1jet is not jet with larger pt "<<b1jet_p4_sorted.Pt()<<" "<<b2jet_p4_sorted.Pt()<< std::endl;
	  b2rescalefactor = rescalec1;
	  b1rescalefactor = rescalec2;
	}
	delete histo;
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

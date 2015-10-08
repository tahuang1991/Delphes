
#include <iostream>
#include <utility>
#include <vector>

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
#include "DiHiggstollbb.h"
#include "MMC.h"

using namespace std;


class ExRootTreeReader;
class ExRootResult;
class GenParticle;
class Jet;
class Track;
class Tower;
 
DiHiggstollbb::DiHiggstollbb(TString input_File, TString output_File, int numEvents)
//void DiHiggs_htobb()
{
  //gSystem->Load("libDelphes");
  inputFile = input_File;
  outputFile = output_File;
  nEvents = numEvents;
 
//config parameters
  runMMC_ = false;
  jetsPt_ =20;
  jetsEta_=5.0;
  bjetsPt_ =30;
  bjetsEta_ = 2.5;
 // jetsDeltaR_ = 0.4;//deltaR matching
  jetleptonDeltaR_ = 0.3;
 // leptonsDeltaR_ = 0.4;//deltaR matching
  //double leptonIso_ = 0.15;
  muonPt2_ = 20;
  muonPt1_ = 20;
  electronPt1_ = 25.0;
  electronPt2_ = 25.0;
  muonsEta_ = 2.4;
  electronsEta_ = 2.4;
  metPt_ = 20;
     

  init();
}


void DiHiggstollbb::init(){ 
  chain = new TChain("Delphes");
  //const char *inputFile("/home/taohuang/Herwig++/Delphes-3.2.0/delphes_output.root");
  //const char *inputFile("/fdata/hepx/store/user/taohuang/Hhh/delphes320_B3_100k.root");
  chain->Add(inputFile);

  treeReader = new ExRootTreeReader(chain);


  evtree = new TTree("evtree","event tree");
  evtree->Branch("b1jet_px",&b1jet_px, "b1jet_px/F");
  evtree->Branch("b1jet_py",&b1jet_py, "b1jet_py/F");
  evtree->Branch("b1jet_pz",&b1jet_pz, "b1jet_pz/F");
  evtree->Branch("b1jet_eta",&b1jet_eta, "b1jet_eta/F");
  evtree->Branch("b1jet_phi",&b1jet_phi, "b1jet_phi/F");
  evtree->Branch("b1jet_pt",&b1jet_pt, "b1jet_pt/F");
  evtree->Branch("b1jet_energy",&b1jet_energy, "b1jet_energy/F");
  evtree->Branch("b2jet_px",&b2jet_px, "b2jet_px/F");
  evtree->Branch("b2jet_py",&b2jet_py, "b2jet_py/F");
  evtree->Branch("b2jet_pz",&b2jet_pz, "b2jet_pz/F");
  evtree->Branch("b2jet_eta",&b2jet_eta, "b2jet_eta/F");
  evtree->Branch("b2jet_phi",&b2jet_phi, "b2jet_phi/F");
  evtree->Branch("b2jet_pt",&b2jet_pt, "b2jet_pt/F");
  evtree->Branch("b2jet_energy",&b2jet_energy, "b2jet_energy/F");

  evtree->Branch("hasb1jet",&hasb1jet, "hasb1jet/B");
  evtree->Branch("hasb2jet",&hasb2jet, "hasb2jet/B");

  evtree->Branch("lepton1_py",&lepton1_py, "lepton1_py/F");
  evtree->Branch("lepton1_pz",&lepton1_pz, "lepton1_pz/F");
  evtree->Branch("lepton1_eta",&lepton1_eta, "lepton1_eta/F");
  evtree->Branch("lepton1_phi",&lepton1_phi, "lepton1_phi/F");
  evtree->Branch("lepton1_pt",&lepton1_pt, "lepton1_pt/F");
  evtree->Branch("lepton1_energy",&lepton1_energy, "lepton1_energy/F");
  evtree->Branch("lepton2_px",&lepton2_px, "lepton2_px/F");
  evtree->Branch("lepton2_py",&lepton2_py, "lepton2_py/F");
  evtree->Branch("lepton2_pz",&lepton2_pz, "lepton2_pz/F");
  evtree->Branch("lepton2_eta",&lepton2_eta, "lepton2_eta/F");
  evtree->Branch("lepton2_phi",&lepton2_phi, "lepton2_phi/F");
  evtree->Branch("lepton2_pt",&lepton2_pt, "lepton2_pt/F");
  evtree->Branch("lepton2_energy",&lepton2_energy, "lepton2_energy/F");
  evtree->Branch("hasmuon1",&hasmuon1, "hasmuon1/B");
  evtree->Branch("hasmuon2",&hasmuon2, "hasmuon2/B");
  evtree->Branch("haselectron1",&haselectron1, "haselectron1/B");
  evtree->Branch("haselectron2",&haselectron2, "haselectron2/B");
  evtree->Branch("haslepton1",&haslepton1, "haslepton1/B");
  evtree->Branch("haslepton2",&haslepton2, "haslepton2/B");
  evtree->Branch("numbjets",&numbjets, "numbjets/F");
  evtree->Branch("numlepton1",&numlepton1,"numlepton1/I");
  evtree->Branch("numlepton2",&numlepton2,"numlepton2/I");
  evtree->Branch("dR_b1l1",&dR_b1l1, "dR_b1l1/F");
  evtree->Branch("dR_b1l2",&dR_b1l2, "dR_b1l2/F");
  evtree->Branch("dR_b2l1",&dR_b2l1, "dR_b2l1/F");
  evtree->Branch("dR_b2l2",&dR_b2l2, "dR_b2l2/F");
  evtree->Branch("dR_b1b2",&dR_b1b2, "dR_b1b2/F");
  evtree->Branch("dR_l1l2",&dR_l1l2, "dR_l1l2/F");
  evtree->Branch("mass_b1b2",&mass_b1b2, "mass_b1b2/F");
  evtree->Branch("mass_l1l2",&mass_l1l2, "mass_l1l2/F");
  evtree->Branch("dphi_llbb",&dphi_llbb, "dphi_llbb/F");

  evtree->Branch("met",&met,"met/F");
  evtree->Branch("met_phi",&met_phi,"met_phi/F");
  evtree->Branch("met_px",&met_px,"met_px/F");
  evtree->Branch("met_py",&met_py,"met_py/F");
	 
  evtree->Branch("hasMET",&hasMET, "hasMET/B");
  evtree->Branch("hasdRljet",&hasdRljet, "hasdRljet/B");
  
}


void DiHiggstollbb::writeTree(){

  //evtree->Print();
  TFile *file = new TFile(outputFile,"recreate");

  evtree->Write();
  file->Close();
//delete file;
}


DiHiggstollbb::~DiHiggstollbb(){


  cout << "** Exiting..." << endl;

  delete treeReader;
  delete chain;
  delete evtree;
}

template<class T>
void DiHiggstollbb::printSortableObject(T *object)
{
    cout <<" Pt " << object->PT << " Eta "<< object->Eta <<" Phi "<< object->Phi << endl;
}


void DiHiggstollbb::insertInJetVector(vector<Jet*>& vec, Jet *object)
{
    //auto it = vec.begin();
    for (vector<Jet*>::iterator it = vec.begin(); it != vec.end(); it++){
//	cout <<" it pt " << (*it)->PT <<" object Pt "<< object->PT << endl;
	if ((*it)->PT< object->PT) {
		vec.insert(it, object);
		return;
	}
     }
    vec.push_back(object);

}
//------------------------------------------------------------------------------


void DiHiggstollbb::initBranches(){
//create branches 
   b1jet_px=0;
   b1jet_py=0;
   b1jet_pz=0;
   b1jet_eta=0;
   b1jet_phi=0;
   b1jet_pt=0;
   b1jet_energy=0;
   b2jet_px=0;
   b2jet_py=0;
   b2jet_pz=0;
   b2jet_eta=0;
   b2jet_phi=0;
   b2jet_pt=0;
   b2jet_energy=0;
   hasb1jet=false;
   hasb2jet=false;
   lepton1_px = 0;
   lepton1_py = 0;
   lepton1_pz = 0;
   lepton1_eta = 0;
   lepton1_phi = 0;
   lepton1_pt = 0;
   lepton1_energy = 0;
   lepton2_px = 0;
   lepton2_py = 0;
   lepton2_pz = 0;
   lepton2_eta = 0;
   lepton2_phi = 0;
   lepton2_pt = 0;
   lepton2_energy = 0;

   numbjets = 0;
   numlepton1 = 0;
   numlepton2 = 0;
   dR_b1l1=-1.0;
   dR_b1l2=-1.0;
   dR_b2l1=-1.0;
   dR_b2l2=-1.0;
   dR_l1l2=-1.0;
   dR_b1b2=-1.0;
   mass_l1l2 = -1.0;
   mass_b1b2 = -1.0;
   dphi_llbb = -10;

   met = 0;
   met_phi = 0;
   met_px = 0;
   met_py = 0;

   //h2tohh_mass =0;
//additional cuts
   hasMET = false;
 //  hasGenMET = false;
   hasmuon1 = false;
   hasmuon2 = false;
   haselectron1 = false;
   haselectron2 = false;
   haslepton1 = false;
   haslepton2 = false;
   hasdRljet = false;
 //  h2tohh =false;
}


  
void DiHiggstollbb::DiHiggstollbbrun()
{

	//loop over events 1. find h->b bbar; 2 find two bjets after cuts; 3 fill Tree
 // TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  //TClonesArray *branchMuonBeforeIso = treeReader->UseBranch("MuonBeforeIso");

  //TClonesArray *branchTrack = treeReader->UseBranch("Track");
//  TClonesArray *branchTower = treeReader->UseBranch("Tower");

  //TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
 // TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
 // TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  //TClonesArray *branchGenJet = treeReader->UseBranch("GenJet");

  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  //TClonesArray *branchGenMissingET = treeReader->UseBranch("GenMissingET");

  long allEntries = treeReader->GetEntries();

  cout << "** Chain contains " << allEntries << " events" << endl;

  long entry;
  Jet *jet, *b1jet, *b2jet;
  std::vector<Jet*> allbjets;
  std::vector<Jet> testvec;
  std::vector<Muon*> allMuon1;
  std::vector<Muon*> allMuon2;
  b1jet=0; b2jet=0;
  MissingET *Met;
  Met=0;
  Electron *electron, *electron1, *electron2;
  electron1 =0; electron2 = 0;
  Photon *photon;
  Muon *muon, *muon1, *muon2;// *muon1_beforeIso, *muon2_beforeIso;
  muon1 =0; muon2=0; //muon1_beforeIso =0; muon2_beforeIso =0;
  //Track *track;
  //Tower *tower;
  
  TLorentzVector momentum;
  TLorentzVector lepton1_p4, lepton2_p4, b1jet_p4, b2jet_p4;
  TLorentzVector totjets_lorentz = TLorentzVector();
  //incase compilation error

  int i =0;
  // Loop over all events
  //TFile *MMCfile = new TFile("testMMC.root", "recreate"); 
  if (nEvents < 0 || nEvents>allEntries) nEvents = allEntries;
  for(entry = 0; entry < nEvents; ++entry)
  //for(entry = 0; entry < 1000; ++entry)
  //for(entry = 0; entry < 10000; ++entry)
  {
    //gen
    initBranches();
    allbjets.clear();

    // Load selected branches with data from specified event
    bool readsuccess = treeReader->ReadEntry(entry);
    if (not readsuccess) {
	cout <<"can not read Entry through treeReader" << endl;
	exit(0);
	}
    cout <<" event id " << entry << endl;

   //loop all reco jets 
    for (i =0;  i<  branchJet->GetEntries(); i++)
    {
      jet = (Jet*) branchJet->At(i);
	//Btag does not work here?????????
      //cout <<"Jet  eta "<< jet->Eta  <<" Pt "<< jet->PT <<" btag "<< jet->BTag << std::endl;
      if (jet->PT < jetsPt_ || abs(jet->Eta)> jetsEta_) continue;
      totjets_lorentz +=jet->P4();
      if ( jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
      insertInJetVector(allbjets, jet);
      //allbjets.push_back(jet);
       
    }
    numbjets = allbjets.size();
  //  for (vector<Jet *>::iterator it = allbjets.begin(); it != allbjets.end(); it++){
//		printSortableObject<Jet>(*it);
//	}
    cout <<" size of bjet vector  " << allbjets.size() << endl; 
    unsigned int b1at = 0;
    unsigned int b2at = 1;
    if (allbjets.size() < 2){
	hasb1jet = false;
	hasb2jet = false;
	}
    else if (allbjets.size() == 2){
	hasb1jet = true;
	hasb2jet = true;
	b1jet = allbjets.at(0);
	b2jet = allbjets.at(1);
	}
    else if (allbjets.size() > 2){
	cout <<" event has more than two bjets passing acceptance cut " << endl;
	//find two bjets that have invariant mass close to 125
	float diff_higgsmass = 125.0;
	for (unsigned k =0; k<allbjets.size()-1; k++)
		for (unsigned int j =i+1; j<allbjets.size(); j++){
			TLorentzVector bjets_p4 = allbjets[k]->P4()+allbjets[j]->P4();
			if (fabs(bjets_p4.M()-125)>diff_higgsmass) continue;
			b1at = k;
			b2at = j;
			diff_higgsmass = fabs(bjets_p4.M()-125);
		}
	hasb1jet = true;
	hasb2jet = true;
     }
	
      // b1jet should be different from b2jet
    if (hasb1jet && hasb2jet){
	b1jet = allbjets[b1at];
	b2jet = allbjets[b2at];
	b1jet_p4 = b1jet->P4();
        b1jet_px = b1jet_p4.Px(); b1jet_py = b1jet_p4.Py(); b1jet_pz= b1jet_p4.Pz(); b1jet_energy = b1jet_p4.Energy();
	b1jet_eta = b1jet_p4.Eta(); b1jet_phi = b1jet_p4.Phi();
	b1jet_pt = b1jet_p4.Pt();
	b2jet_p4 = b2jet->P4();
        b2jet_px = b2jet_p4.Px(); b2jet_py = b2jet_p4.Py(); b2jet_pz= b2jet_p4.Pz(); b2jet_energy = b2jet_p4.Energy();
	b2jet_eta = b2jet_p4.Eta(); b2jet_phi = b2jet_p4.Phi();
	b2jet_pt = b2jet_p4.Pt();
     }
    // TLorentzVector htobb_jets = b1jet->P4()+b2jet->P4();
     //cout <<" htobb_jet " << htobb_jets.M(); htobb_jets.Print();
    
    // Analyse missing ET, generator level

    //apply muon cuts on muons 
    // Analyse missing ET, reco level
    if(branchMissingET->GetEntriesFast() > 0)
    {
      Met = (MissingET*) branchMissingET->At(0);
      met = Met->MET;
      met_phi = Met->Phi;
      met_px = Met->P4().Px();
      met_py = Met->P4().Py();
      if (met > metPt_) hasMET = true;
    }

    //if (hasMET) cout <<" has MET " << met << endl;
    //apply muon cuts on muons 
    //Loop over all Muon(before Isolation) in event
    //how to check whether deltaR matching and matching genparticle has same performance??

    // Loop over all Muon in event, reco muon
    for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
    {
      muon = (Muon*) branchMuon->At(i);
//      printGenParticle(particle);
      //check charge and deltaR, genmu1: charge<0, genmu2: charge>0
      if (muon->Charge<0 and abs(muon->Eta)<muonsEta_ and muon->PT >= muonPt1_) 
	//insertInVector<Muon>(allMuon1, muon);
	allMuon1.push_back(muon);
      else if(muon->Charge>0 and abs(muon->Eta)<muonsEta_ and muon->PT >= muonPt1_) 
	//insertInVector<Muon>(allMuon2, muon);
	allMuon2.push_back(muon);
      //cout <<" muon eta " << muon->Eta << " phi " << muon->Phi << " Pt "<< muon->PT << endl; 
    }
    
    numlepton1 += allMuon1.size();
    numlepton2 += allMuon1.size();
    // Loop over all electrons in event
    // do similar thing for electron when electrons are also taken as final state
    for(i = 0; i < branchElectron->GetEntriesFast(); ++i)
    {
      electron = (Electron*) branchElectron->At(i);
      //particle = (GenParticle*) electron->Particle.GetObject();
      if (electron->Charge<0 and abs(electron->Eta)<electronsEta_ and electron->PT >= electronPt1_) {
 	if ((not haselectron1) or (haselectron1 and electron->PT>electron1->PT))	electron1 = electron;
	haselectron1 = true;
 	numlepton1++;
      }
      else if(electron->Charge>0 and abs(electron->Eta)<electronsEta_ and electron->PT >= electronPt1_) {
 	if ((not haselectron2) or (haselectron2 and electron->PT>electron2->PT))	electron2 = electron;
        haselectron2 = true;
 	numlepton2++;
      }
      //cout <<" electron "; printSortableObject<Electron>(electron);
    }

    if (hasmuon1){
	lepton1_p4 = muon1->P4();
	haslepton1 = true;
	}
    else if(haselectron1){
	lepton1_p4 = electron1->P4();
	haslepton1 = true;
	} 
    if (haslepton2){
	lepton2_p4 = muon2->P4();
	haslepton2 = true;
	}
    else if (haselectron2){
	lepton2_p4 = electron2->P4();
	haslepton2 = true;
	}
    if (haslepton1){
	lepton1_px = lepton1_p4.Px(); lepton1_py = lepton1_p4.Py(); lepton1_pz = lepton1_p4.Pz(); lepton1_energy = lepton1_p4.E();
	lepton1_eta = lepton1_p4.Eta(); lepton1_phi = lepton1_p4.Phi(); lepton1_pt = lepton1_p4.Pt();
   	}
    if (haslepton2){
	lepton2_px = lepton2_p4.Px(); lepton2_py = lepton2_p4.Py(); lepton2_pz = lepton2_p4.Pz(); lepton2_energy = lepton2_p4.E();
	lepton2_eta = lepton2_p4.Eta(); lepton2_phi = lepton2_p4.Phi(); lepton2_pt = lepton2_p4.Pt();
	}
    // Loop over all photons in event
    for(i = 0; i < branchPhoton->GetEntriesFast(); ++i)
    {
      photon = (Photon*) branchPhoton->At(i);

      // skip photons with references to multiple particles
      if(photon->Particles.GetEntriesFast() != 1) continue;

      //particle = (GenParticle*) photon->Particles.At(0);
      //cout <<" photon "; printSortableObject<Photon>(photon);
    }

   if (hasb1jet and hasb2jet and haslepton1 and haslepton2){
	dR_b1l1 = b1jet_p4.DeltaR(lepton1_p4);
	dR_b1l2 = b1jet_p4.DeltaR(lepton2_p4);
	dR_b2l1 = b2jet_p4.DeltaR(lepton1_p4);
	dR_b2l2 = b2jet_p4.DeltaR(lepton2_p4);
	dR_b1b2 = b1jet_p4.DeltaR(b2jet_p4);
	dR_l1l2 = lepton1_p4.DeltaR(lepton2_p4);
	TLorentzVector ll_p4 = lepton1_p4+lepton2_p4;
	TLorentzVector bjets_p4 = b1jet_p4+b2jet_p4;
	dphi_llbb = TVector2::Phi_mpi_pi(ll_p4.Phi()-bjets_p4.Phi());
	mass_l1l2 = ll_p4.M();
        mass_b1b2 = bjets_p4.M();
        if (dR_b1l1>jetleptonDeltaR_ and dR_b1l2>jetleptonDeltaR_ and dR_b2l1>jetleptonDeltaR_ and dR_b2l2>jetleptonDeltaR_) hasdRljet =true;
       
    }
     
    // Loop over all tracks in event
    /*
    for(i = 0; i < branchTrack->GetEntriesFast(); ++i)
    {
      track = (Track*) branchTrack->At(i);
      particle = (GenParticle*) track->Particle.GetObject();
      cout <<" Track "; printGenParticle(particle);

    }*/
   // calculate the DeltaR between bjet and lepton 

    if (runMMC_ and hasdRljet){
	 TLorentzVector bjets_lorentz=b1jet->P4()+b2jet->P4();
          
         cout <<" m_{bjets} " << bjets_lorentz.M(); bjets_lorentz.Print();
	 TLorentzVector met_lorentz = Met->P4();
	 bool weightfromonshellnupt_func = false;
         bool weightfromonshellnupt_hist = true;
	 bool weightfromonoffshellWmass_hist=true; 
	 int iterations = 100000;
	 std::string RefPDFfile("MMCRefPDF.ROOT");
	 bool useMET = true;
         bool onshellMarker_ = -1;
	 // rescale bjets in MMC?????
        //MMC *thismmc = new MMC();
        TLorentzVector null_lorentz = TLorentzVector(); 
         MMC *thismmc = new MMC(lepton1_p4, lepton2_p4, bjets_lorentz, totjets_lorentz, 
          met_lorentz, null_lorentz, null_lorentz, null_lorentz,
	   null_lorentz, onshellMarker_,// only for simulation 
          false, entry, weightfromonshellnupt_func, weightfromonshellnupt_hist, weightfromonoffshellWmass_hist,
          iterations, RefPDFfile, useMET);
	  thismmc->runMMC();
	  TTree *mmctree = (thismmc->getMMCTree())->CloneTree();
          std::cout <<"MMCTree entries " << (thismmc->getMMCTree())->GetEntries() << std::endl;
          std::cout <<"testtree entries " << mmctree->GetEntries()<<" title "<< mmctree->GetTitle() << std::endl;
	  //esttree->SetDirectory((TDirectory*)MMCfile);
	  //MMCfile->WriteObject(mmctree,mmctree->GetTitle());
	delete thismmc;
	}
//fill branches
    if (hasb1jet or hasb2jet or haslepton1 or haslepton2) evtree->Fill();
    }
 
}



//------------------------------------------------------------------------------


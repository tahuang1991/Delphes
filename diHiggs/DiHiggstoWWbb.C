
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
#include "DiHiggstoWWbb.h"
#include "MMC.h"

using namespace std;


class ExRootTreeReader;
class ExRootResult;
class GenParticle;
class Jet;
class Track;
class Tower;
 
DiHiggstoWWbb::DiHiggstoWWbb(TString input_File, TString output_File, int numEvents)
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
  jetsDeltaR_ = 0.4;//deltaR matching
  jetleptonDeltaR_ = 0.3;
  leptonsDeltaR_ = 0.4;//deltaR matching
  //double leptonIso_ = 0.15;
  muonPt2_ = 20;
  muonPt1_ = 20;
  muonsEta_ = 2.4;
  metPt_ = 20;
     

  init();
}


void DiHiggstoWWbb::init(){ 
  chain = new TChain("Delphes");
  //const char *inputFile("/home/taohuang/Herwig++/Delphes-3.2.0/delphes_output.root");
  //const char *inputFile("/fdata/hepx/store/user/taohuang/Hhh/delphes320_B3_100k.root");
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
  evtree->Branch("b1jet_btag",&b1jet_btag, "b1jet_btag/B");
  evtree->Branch("b2jet_px",&b2jet_px, "b2jet_px/F");
  evtree->Branch("b2jet_py",&b2jet_py, "b2jet_py/F");
  evtree->Branch("b2jet_pz",&b2jet_pz, "b2jet_pz/F");
  evtree->Branch("b2jet_eta",&b2jet_eta, "b2jet_eta/F");
  evtree->Branch("b2jet_phi",&b2jet_phi, "b2jet_phi/F");
  evtree->Branch("b2jet_pt",&b2jet_pt, "b2jet_pt/F");
  evtree->Branch("b2jet_energy",&b2jet_energy, "b2jet_energy/F");
  evtree->Branch("b2jet_btag",&b2jet_btag, "b2jet_btag/B");
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
  evtree->Branch("nu2_energy",&nu2_energy, "nu2_energy/F");
  evtree->Branch("htoWW_energy",&htoWW_energy);
  evtree->Branch("htoWW_px",&htoWW_px,"htoWW_px/F");
  evtree->Branch("htoWW_py",&htoWW_py,"htoWW_px/F");
  evtree->Branch("htoWW_pz",&htoWW_pz,"htoWW_pz/F");
  evtree->Branch("htoWW_mass",&htoWW_mass,"htoWW_mass/F");
  evtree->Branch("Wtomu1nu1",&Wtomu1nu1,"Wtomu1nu1/B");
  evtree->Branch("Wtomu2nu2",&Wtomu2nu2,"Wtomu2nu2/B");
  evtree->Branch("htoWW",&htoWW,"htoWW/B");

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

  evtree->Branch("dR_b1l1",&dR_b1l1, "dR_b1l1/F");
  evtree->Branch("dR_b1l2",&dR_b1l2, "dR_b1l2/F");
  evtree->Branch("dR_b2l1",&dR_b2l1, "dR_b2l1/F");
  evtree->Branch("dR_b2l2",&dR_b2l2, "dR_b2l2/F");
  evtree->Branch("dR_b1b2",&dR_b1b2, "dR_b1b2/F");
  evtree->Branch("dR_l1l2",&dR_l1l2, "dR_l1l2/F");
  evtree->Branch("mass_b1b2",&mass_b1b2, "mass_b1b2/F");
  evtree->Branch("mass_l1l2",&mass_l1l2, "mass_l1l2/F");
  evtree->Branch("dphi_llbb",&dphi_llbb, "dphi_llbb/F");

  evtree->Branch("genmet",&genmet,"genmet/F");
  evtree->Branch("genmet_phi",&genmet_phi,"genmet_phi/F");
  evtree->Branch("genmet_px",&genmet_px,"genmet_px/F");
  evtree->Branch("genmet_py",&genmet_py,"genmet_py/F");
  evtree->Branch("met",&met,"met/F");
  evtree->Branch("met_phi",&met_phi,"met_phi/F");
  evtree->Branch("met_px",&met_px,"met_px/F");
  evtree->Branch("met_py",&met_py,"met_py/F");
	 
  evtree->Branch("hasGenMET",&hasGenMET, "hasGenMET/B");
  evtree->Branch("hasMET",&hasMET, "hasMET/B");
  evtree->Branch("hasdRljet",&hasdRljet, "hasdRljet/B");
  evtree->Branch("h2tohh_mass",&h2tohh_mass,"h2tohh_mass/F");
  evtree->Branch("h2tohh",&h2tohh,"h2tohh/B");
  
 // AnalyseEvents(treeReader, evtree);
}


void DiHiggstoWWbb::writeTree(){

  //evtree->Print();
  TFile *file = new TFile(outputFile,"recreate");

  evtree->Write();
  file->Close();
//delete file;
}


DiHiggstoWWbb::~DiHiggstoWWbb(){


  cout << "** Exiting..." << endl;

  delete treeReader;
  delete chain;
  delete evtree;
}

void DiHiggstoWWbb::printGenParticle(GenParticle *genP)
{
    cout << " genP Id " << genP->PID <<" Pt " << genP->PT << " M1 "<< genP->M1<<" M2 "<< genP->M2;
    cout << " D1 "<< genP->D1 <<" D2 "<<genP->D2 << " p4 "; genP->P4().Print();
}

void DiHiggstoWWbb::printJet(Jet *jet)
{

  GenParticle *particle;
  Muon *muon;

  Track *track;
  Tower *tower;

  TObject *object;
  TLorentzVector momentum;
      momentum.SetPxPyPzE(0.0, 0.0, 0.0, 0.0);
      //TRefArray constituentarray(jet->Constituents);
      TRefArray particlearray(jet->Particles);
      cout<<"Looping over jet constituents. Jet pt: "<<jet->PT<<", eta: "<<jet->Eta<<", phi: "<<jet->Phi<<endl;      

      // Loop over all jet's constituents
      for(int j = 0; j < jet->Constituents.GetEntriesFast(); ++j)
      {
        object = jet->Constituents.At(j);
        // Check if the constituent is accessible
        if(object == 0) continue;

        if(object->IsA() == GenParticle::Class())
        {
          particle = (GenParticle*) object;
          cout << "    GenPart pt: " << particle->PT << ", eta: " << particle->Eta << ", phi: " << particle->Phi << endl;
          momentum += particle->P4();
        }
        else if(object->IsA() == Track::Class())
        {
          track = (Track*) object;
          cout << "    Track pt: " << track->PT << ", eta: " << track->Eta << ", phi: " << track->Phi << endl;
          momentum += track->P4();
        }
        else if(object->IsA() == Tower::Class())
        {
          tower = (Tower*) object;
          cout << "    Tower pt: " << tower->ET << ", eta: " << tower->Eta << ", phi: " << tower->Phi << endl;
          momentum += tower->P4();
        }
        else if(object->IsA() == Muon::Class())
        {
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
//get the finalsate of genp 
void DiHiggstoWWbb::getFinalState(GenParticle* &genp, TClonesArray *branchParticle)
{
  //     cout << "before getFinalState "; printGenParticle(genp);
       while ((genp->D1>0 && ((GenParticle*)branchParticle->At(genp->D1))->PID == genp->PID)
	     && (genp->D2 >0 && ((GenParticle*)branchParticle->At(genp->D2))->PID == genp->PID)){
 	//if ( genp->D1>0 && ((GenParticle*)branchParticle->At(genp->D1))->PID == genp->PID) 
 	genp = (GenParticle*)branchParticle->At(genp->D1);
        //else   genp = (GenParticle*)branchParticle->At(genp->D2);
        //cout << "during getFinalState "; printGenParticle(genp);
       }
//	cout <<" final State "; printGenParticle(genp);
}


void DiHiggstoWWbb::printAllGenParticles(TClonesArray *branchParticle)
{

    for (int i =0; i < branchParticle->GetEntries(); ++i){
	GenParticle* genP = (GenParticle*) branchParticle->At(i);
        //defualt M1 M2 D1 D2 is -1;
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
//------------------------------------------------------------------------------


void DiHiggstoWWbb::initBranches(){
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
   dR_genb1jet=0;
   dR_genb2jet=0;
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
   b1jet_btag = false;
   b2jet_px=0;
   b2jet_py=0;
   b2jet_pz=0;
   b2jet_eta=0;
   b2jet_phi=0;
   b2jet_pt=0;
   b2jet_energy=0;
   b2jet_btag = false;
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
   nu1_energy =0;
   nu2_px =0;
   nu2_py =0;
   nu2_pz =0;
   nu2_eta =0;
   nu2_phi =0;
   nu2_energy =0;
  
   Wtomu2nu2=false;
   Wtomu1nu1=false;
   htoWW=false;
  
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

   dR_b1l1=-1.0;
   dR_b1l2=-1.0;
   dR_b2l1=-1.0;
   dR_b2l2=-1.0;
   dR_l1l2=-1.0;
   dR_b1b2=-1.0;
   mass_l1l2 = -1.0;
   mass_b1b2 = -1.0;
   dphi_llbb = -10;

   genmet = 0;
   genmet_phi = 0;
   genmet_px = 0;
   genmet_py = 0;
   met = 0;
   met_phi = 0;
   met_px = 0;
   met_py = 0;

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
}


  
void DiHiggstoWWbb::DiHiggstoWWbbrun()
{

	//loop over events 1. find h->b bbar; 2 find two bjets after cuts; 3 fill Tree
  TClonesArray *branchParticle = treeReader->UseBranch("Particle");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  TClonesArray *branchPhoton = treeReader->UseBranch("Photon");
  TClonesArray *branchMuon = treeReader->UseBranch("Muon");
  TClonesArray *branchMuonBeforeIso = treeReader->UseBranch("MuonBeforeIso");

  //TClonesArray *branchTrack = treeReader->UseBranch("Track");
//  TClonesArray *branchTower = treeReader->UseBranch("Tower");

  //TClonesArray *branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
 // TClonesArray *branchEFlowPhoton = treeReader->UseBranch("EFlowPhoton");
 // TClonesArray *branchEFlowNeutralHadron = treeReader->UseBranch("EFlowNeutralHadron");
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchGenJet = treeReader->UseBranch("GenJet");

  TClonesArray *branchMissingET = treeReader->UseBranch("MissingET");
  TClonesArray *branchGenMissingET = treeReader->UseBranch("GenMissingET");

  long allEntries = treeReader->GetEntries();

  cout << "** Chain contains " << allEntries << " events" << endl;

  long entry;
  Jet *genjet, *jet, *b1jet, *b2jet, *genb1jet, *genb2jet;
  b1jet=0; b2jet=0; genb1jet =0; genb2jet=0;
  MissingET *Met, *genMet;
  Met=0;
  GenParticle *particle, *genh2, *genhiggs1, *genhiggs2, *genhtobb, *genb1, *genb2;
  genhtobb =0; genb1= 0; genb2=0;
  GenParticle *genhtoWW, *genW1, *genW2, *genmu1, *genmu2, *gennu1, *gennu2;
  genmu1=0; genmu2=0; gennu1=0; gennu2=0;
  Electron *electron;
  Photon *photon;
  Muon *muon, *muon1, *muon2, *muon1_beforeIso, *muon2_beforeIso;
  muon1 =0; muon2=0; muon1_beforeIso =0; muon2_beforeIso =0;
  //Track *track;
  //Tower *tower;
  
  TLorentzVector momentum;
  TLorentzVector Muon1_p4, Muon2_p4, b1jet_p4, b2jet_p4;
  TLorentzVector totjets_lorentz = TLorentzVector();
  //incase compilation error
  genhtoWW = 0;

  int i =0;
  // Loop over all events
  //TFile *MMCfile = new TFile("testMMC.root", "recreate"); 
  if (nEvents < 0) nEvents = allEntries;
  for(entry = 0; entry < nEvents; ++entry)
  //for(entry = 0; entry < 1000; ++entry)
  //for(entry = 0; entry < 10000; ++entry)
  {
    //gen
    initBranches();

    // Load selected branches with data from specified event
    bool readsuccess = treeReader->ReadEntry(entry);
    if (not readsuccess) {
	cout <<"can not read Entry through treeReader" << endl;
	exit(0);
	}
    cout <<" event id " << entry << endl;
  
     genh2 =  (GenParticle*) branchParticle->At(0); //printGenParticle(genP);
     if (genh2->PID != 99926 and genh2->PID != 99927) {
	cout <<" first genparticle is not 99926 nor 99927 " << endl; 
	printGenParticle(genh2);
	continue;
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
    //move on to "final state" bquarks
        //printGenParticle(genhtobb);
	getFinalState(genb1, branchParticle);
	getFinalState(genb2, branchParticle);
           b1_px = genb1->Px;
	   b1_py = genb1->Py;
	   b1_pz = genb1->Pz;
	   b1_eta = genb1->Eta;
	   b1_phi = genb1->Phi;
	   b1_pt = genb1->PT;
	   b1_energy = genb1->E;
	   b2_px = genb2->Px;
	   b2_py = genb2->Py;
	   b2_pz = genb2->Pz;
	   b2_eta = genb2->Eta;
	   b2_phi = genb2->Phi;
	   b2_pt = genb2->PT;
	   b2_energy = genb2->E;
           TLorentzVector bbbar_lorentz = genb1->P4()+genb2->P4();
           //cout << " bbbar_lorentz Mass "<< bbbar_lorentz.M(); bbbar_lorentz.Print();
           htobb_px = genhtobb->Px; htobb_py= genhtobb->Py; htobb_pz = genhtobb->Pz; htobb_energy = genhtobb->E;
	   htobb_mass = genhtobb->Mass;
    }

    if (htoWW){
		if (((GenParticle*)branchParticle->At(genhtoWW->D1))->PID == -24){//W-
 			genW1 = (GenParticle*)branchParticle->At(genhtoWW->D1);	//to muon(13)
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
	mu1_px = genmu1->Px; mu1_py = genmu1->Py; mu1_pz = genmu1->Pz; mu1_energy = genmu1->E;
	mu1_eta = genmu1->Eta; mu1_phi = genmu1->Phi; mu1_pt = genmu1->PT;
	nu1_px = gennu1->Px; nu1_py = gennu1->Py; nu1_pz = gennu1->Pz; nu1_energy = gennu1->E;
	nu1_eta = gennu1->Eta; nu1_phi = gennu1->Phi; nu1_pt = gennu1->PT;
   	//cout << "mu1 from W "; printGenParticle(genmu1);
    }
    if (Wtomu2nu2){
    	getFinalState(genmu2, branchParticle);	
    	getFinalState(gennu2, branchParticle);	
	mu2_px = genmu2->Px; mu2_py = genmu2->Py; mu2_pz = genmu2->Pz; mu2_energy = genmu2->E;
	mu2_eta = genmu2->Eta; mu2_phi = genmu2->Phi; mu2_pt = genmu2->PT;
	nu2_px = gennu2->Px; nu2_py = gennu2->Py; nu2_pz = gennu2->Pz; nu2_energy = gennu2->E;
	nu2_eta = gennu2->Eta; nu2_phi = gennu2->Phi; nu2_pt = gennu2->PT;
        //cout << "mu2 from W "; printGenParticle(genmu2);
    }
 
   //loop all Gen jets 
    for (i =0;  i<  branchGenJet->GetEntries(); i++)
    {
      genjet = (Jet*) branchGenJet->At(i);
      if (genjet->PT < bjetsPt_ || abs(genjet->Eta)> bjetsEta_) continue;
      TLorentzVector genjet_p4 = genjet->P4();
      if (htobb && genjet_p4.DeltaR(genb1->P4()) < dR_genb1jet && genjet_p4.DeltaR(genb1->P4())<jetsDeltaR_) {
	dR_genb1jet = genjet_p4.DeltaR(genb1->P4());
        genb1jet_px = genjet_p4.Px(); genb1jet_py = genjet_p4.Py(); genb1jet_pz=genjet_p4.Pz(); genb1jet_energy = genjet_p4.Energy();
        genb1jet_eta = genjet_p4.Eta(); genb1jet_phi = genjet_p4.Phi();
	genb1jet_pt = genjet_p4.Pt();
	hasgenb1jet = true;
	genb1jet = genjet;
	//cout <<"genb1jet pt "<< genb1jet_pt << endl;
        
       }
      if (htobb && genjet_p4.DeltaR(genb2->P4()) < dR_genb2jet && genjet_p4.DeltaR(genb2->P4()) < jetsDeltaR_){
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
    for (i =0;  i<  branchJet->GetEntries(); i++)
    {
      jet = (Jet*) branchJet->At(i);
      if (jet->PT < jetsPt_ || abs(jet->Eta)> jetsEta_) continue;
      totjets_lorentz +=jet->P4();
      if (jet->PT < bjetsPt_ || abs(jet->Eta)> bjetsEta_) continue;
      TLorentzVector jet_p4 = jet->P4();
      if (htobb && jet_p4.DeltaR(genb1->P4()) < dR_b1jet && jet_p4.DeltaR(genb1->P4()) < jetsDeltaR_) {
      	b1jet = jet;
	b1jet_btag = jet->BTag;
	dR_b1jet = jet_p4.DeltaR(genb1->P4());
	b1jet_p4 = jet_p4;
        b1jet_px = jet_p4.Px(); b1jet_py = jet_p4.Py(); b1jet_pz=jet_p4.Pz(); b1jet_energy = jet_p4.Energy();
	b1jet_eta = jet_p4.Eta(); b1jet_phi = jet_p4.Phi();
	b1jet_pt = jet_p4.Pt();
	//cout <<"b1jet pt "<< b1jet_pt << endl;
	hasb1jet = true;
        
       }
      if (htobb && jet_p4.DeltaR(genb2->P4()) < dR_b2jet && jet_p4.DeltaR(genb2->P4()) < jetsDeltaR_){
	b2jet = jet;
	b2jet_btag = jet->BTag;
        dR_b2jet = jet_p4.DeltaR(genb2->P4());
	b2jet_p4 = jet_p4;
        b2jet_px = jet_p4.Px(); b2jet_py = jet_p4.Py(); b2jet_pz=jet_p4.Pz(); b2jet_energy = jet_p4.Energy();
	b2jet_eta = jet_p4.Eta(); b2jet_phi = jet_p4.Phi();
	b2jet_pt = jet_p4.Pt();
	hasb2jet = true;
	//cout <<"b2jet pt "<< b2jet_pt << endl;
       }
    }
      // b1jet should be different from b2jet
    if (hasb1jet && hasb2jet && b1jet == b2jet){
	hasb1jet = false;
	hasb2jet = false;
     }
    else if (hasb1jet && hasb2jet){
     //cout <<" b1jet Pt " << b1jet->PT  <<"  b2jet Pt " << b2jet->PT << endl; 
    // TLorentzVector htobb_jets = b1jet->P4()+b2jet->P4();
     //cout <<" htobb_jet " << htobb_jets.M(); htobb_jets.Print();
	float bjet_pt1 = (b1jet_pt>b2jet_pt)?b1jet_pt:b2jet_pt;
	float bjet_pt2 = (b1jet_pt>b2jet_pt)?b2jet_pt:b1jet_pt;	
    	for (i =0;  i<  branchJet->GetEntries(); i++)
    	{
      		jet = (Jet*) branchJet->At(i);
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
    
    // Analyse missing ET, generator level
    if(branchGenMissingET->GetEntriesFast() > 0)
    {
      genMet = (MissingET*) branchGenMissingET->At(0);
      genmet = genMet->MET;
      genmet_phi = genMet->Phi;
      genmet_px = genMet->P4().Px();
      genmet_py = genMet->P4().Py();
      if (genmet > metPt_) hasGenMET = true;
      //cout <<" has genMET " << genmet << endl;
    }

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
      //cout <<" has MET " << met << endl;
    }

    //apply muon cuts on muons 
    if (Wtomu1nu1 && Wtomu2nu2){
    if (((genmu1->PT > muonPt1_ && genmu2->PT > muonPt2_) || (genmu1->PT > muonPt2_ && genmu2->PT > muonPt1_)) 
	&& fabs(genmu1->Eta)<muonsEta_ && fabs(genmu2->Eta)< muonsEta_) hastwogenmuons =true;
    }
    //Loop over all Muon(before Isolation) in event
    //how to check whether deltaR matching and matching genparticle has same performance??
    //
    for(i = 0; i < branchMuonBeforeIso->GetEntriesFast(); ++i)
    {
      muon = (Muon*) branchMuonBeforeIso->At(i);
      TLorentzVector muon_p4 = muon->P4(); 
      //
      particle = (GenParticle*) muon->Particle.GetObject();
//      printGenParticle(particle);
      if (Wtomu1nu1 and muon->Charge<0 and muon_p4.DeltaR(genmu1->P4())<dR_mu1_beforeIso and muon_p4.DeltaR(genmu1->P4())<leptonsDeltaR_) {
        dR_mu1_beforeIso = muon_p4.DeltaR(genmu1->P4());
	muon1_beforeIso = muon;
	hasMuon1_beforeIso = true;
	if (particle == genmu1) Muon1_beforeIso_hasgenMu = true;
      }
      else if(Wtomu2nu2 and muon->Charge>0 and muon_p4.DeltaR(genmu2->P4())<dR_mu2_beforeIso and muon_p4.DeltaR(genmu2->P4())<leptonsDeltaR_) {
	dR_mu2_beforeIso = muon_p4.DeltaR(genmu2->P4());
	muon2_beforeIso = muon;
        hasMuon2_beforeIso = true;
	if (particle == genmu2) Muon2_beforeIso_hasgenMu = true;
      }
      if (hasMuon1_beforeIso) std::cout <<" has reco Muon1 before Iso  " << std::endl;
      if (hasMuon2_beforeIso) std::cout <<" has reco Muon2 before Iso  " << std::endl;
    }
    if (hasMuon1_beforeIso){
	Muon1_beforeIso_px = muon1_beforeIso->P4().Px(); Muon1_beforeIso_py = muon1_beforeIso->P4().Py(); Muon1_beforeIso_pz = muon1_beforeIso->P4().Pz(); Muon1_beforeIso_energy = muon1_beforeIso->P4().E();
	Muon1_beforeIso_eta = muon1_beforeIso->P4().Eta(); Muon1_beforeIso_phi = muon1_beforeIso->P4().Phi();
	Muon1_pt = muon1_beforeIso->PT;
	}
    if (hasMuon2_beforeIso){
	Muon2_beforeIso_px = muon2_beforeIso->P4().Px(); Muon2_beforeIso_py = muon2_beforeIso->P4().Py(); Muon2_beforeIso_pz = muon2_beforeIso->P4().Pz(); Muon2_beforeIso_energy = muon2_beforeIso->P4().E();
	Muon2_beforeIso_eta = muon2_beforeIso->P4().Eta(); Muon2_beforeIso_phi = muon2_beforeIso->P4().Phi();
	Muon2_pt = muon2_beforeIso->PT;
	}

    // Loop over all Muon in event, reco muon
    for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
    {
      muon = (Muon*) branchMuon->At(i);
      TLorentzVector muon_p4 = muon->P4(); 
      particle = (GenParticle*) muon->Particle.GetObject();
//      printGenParticle(particle);
      if (hasMuon1_beforeIso and  muon->Eta == muon1_beforeIso->Eta and muon->Phi == muon1_beforeIso->Phi) Muon1_beforeIso_passIso = true;
      if (hasMuon2_beforeIso and  muon->Eta == muon2_beforeIso->Eta and muon->Phi == muon2_beforeIso->Phi) Muon2_beforeIso_passIso = true;
      //check charge and deltaR, genmu1: charge<0, genmu2: charge>0
      if (Wtomu1nu1 and muon->Charge<0 and muon_p4.DeltaR(genmu1->P4())<dR_mu1 and muon_p4.DeltaR(genmu1->P4())<leptonsDeltaR_) {
	dR_mu1 = muon_p4.DeltaR(genmu1->P4());
	muon1 = muon;
	hasMuon1 = true;
	if (particle == genmu1) Muon1_hasgenMu = true;
      }
      else if (Wtomu2nu2 and muon->Charge>0 and muon_p4.DeltaR(genmu2->P4())<dR_mu2 and muon_p4.DeltaR(genmu2->P4())<leptonsDeltaR_) {
	dR_mu2 = muon_p4.DeltaR(genmu2->P4());
	muon2 = muon;
        hasMuon2 = true;
	if (particle == genmu2) Muon2_hasgenMu = true;
      }
      if (hasMuon1) std::cout <<" has reco Muon1 " << std::endl;
      if (hasMuon2) std::cout <<" has reco Muon2 " << std::endl;
      //cout <<" muon eta " << muon->Eta << " phi " << muon->Phi << " Pt "<< muon->PT << endl; 
    }

    if (hasMuon1){
	Muon1_p4 = muon1->P4();
	Muon1_px = muon1->P4().Px(); Muon1_py = muon1->P4().Py(); Muon1_pz = muon1->P4().Pz(); Muon1_energy = muon1->P4().E();
	Muon1_eta = muon1->P4().Eta(); Muon1_phi = muon1->P4().Phi(); Muon1_pt = muon1->PT;
	}
    if (hasMuon2){
	Muon2_p4 = muon2->P4();
	Muon2_px = muon2->P4().Px(); Muon2_py = muon2->P4().Py(); Muon2_pz = muon2->P4().Pz(); Muon2_energy = muon2->P4().E();
	Muon2_eta = muon2->P4().Eta(); Muon2_phi = muon2->P4().Phi(); Muon2_pt = muon2->PT;
	}
   
    if (hasMuon1 && hasMuon2){
    if (((muon1->PT > muonPt1_ && muon2->PT > muonPt2_) || (muon1->PT > muonPt2_ && muon2->PT > muonPt1_)) 
	&& fabs(muon1->Eta)<muonsEta_ && fabs(muon2->Eta)< muonsEta_) hastwomuons =true;
    }

    if (hastwomuons){
	energeticMuon1 = true; energeticMuon2 = true;
    	for(i = 0; i < branchMuon->GetEntriesFast(); ++i)
    	{
   		muon = (Muon*) branchMuon->At(i);
		if (abs(muon->Eta)<muonsEta_ and muon->Charge<0 and muon->PT>Muon1_pt) energeticMuon1 = false; 
		if (abs(muon->Eta)<muonsEta_ and muon->Charge>0 and muon->PT>Muon2_pt) energeticMuon2 = false; 
        }

    }
    // Loop over all electrons in event
    for(i = 0; i < branchElectron->GetEntriesFast(); ++i)
    {
      electron = (Electron*) branchElectron->At(i);
      particle = (GenParticle*) electron->Particle.GetObject();
      //cout <<" electron "; printGenParticle(particle);
    }

    // Loop over all photons in event
    for(i = 0; i < branchPhoton->GetEntriesFast(); ++i)
    {
      photon = (Photon*) branchPhoton->At(i);

      // skip photons with references to multiple particles
      if(photon->Particles.GetEntriesFast() != 1) continue;

      particle = (GenParticle*) photon->Particles.At(0);
      //cout <<" photon "; printGenParticle(particle);
    }

   if (hasb1jet and hasb2jet and hasMuon1 and hasMuon2){
	dR_b1l1 = b1jet_p4.DeltaR(Muon1_p4);
	dR_b1l2 = b1jet_p4.DeltaR(Muon2_p4);
	dR_b2l1 = b2jet_p4.DeltaR(Muon1_p4);
	dR_b2l2 = b2jet_p4.DeltaR(Muon2_p4);
	dR_b1b2 = b1jet_p4.DeltaR(b2jet_p4);
	dR_l1l2 = Muon1_p4.DeltaR(Muon2_p4);
	TLorentzVector ll_p4 = Muon1_p4+Muon2_p4;
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

    h2tohh = (htobb and Wtomu1nu1 and Wtomu2nu2);
    if (runMMC_ && h2tohh && hasgenb1jet && hasgenb2jet){
	 TLorentzVector bjets_lorentz=genb1jet->P4()+genb2jet->P4();
          
         cout <<" m_{bjets} " << bjets_lorentz.M(); bjets_lorentz.Print();
	 TLorentzVector met_lorentz = Met->P4();
	 bool simulation_ = true;
	 bool weightfromonshellnupt_func = false;
         bool weightfromonshellnupt_hist = true;
	 bool weightfromonoffshellWmass_hist=true; 
	 int iterations = 100000;
	 std::string RefPDFfile("MMCRefPDF.ROOT");
	 bool useMET = true;
	 int onshellMarker_;
 	 if (genW1->Mass > genW2->Mass) onshellMarker_=1;
	 else onshellMarker_=2;
	 // rescale bjets in MMC?????
        //MMC *thismmc = new MMC();

         MMC *thismmc = new MMC(genmu1->P4(), genmu2->P4(), bjets_lorentz, totjets_lorentz, 
          met_lorentz, gennu1->P4(), gennu2->P4(), genb1->P4()+genb2->P4(),
	  genh2->P4(), onshellMarker_,// only for simulation 
          simulation_, entry, weightfromonshellnupt_func, weightfromonshellnupt_hist, weightfromonoffshellWmass_hist,
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
    if (htobb || (Wtomu1nu1 && Wtomu2nu2)) evtree->Fill();
    }
 
   // MMCfile->Close();
    //delete MMCfile;
    //evtree->Fill();
}



//------------------------------------------------------------------------------


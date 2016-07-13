// -*- C++ -*-
//

#ifndef MMC_cc
#define MMC_cc



//Simulation or not
#include <iostream>
#include "MMC.h"
#include <time.h>       /* time */

//constructor
MMC::MMC(TLorentzVector* mu1_lorentz, TLorentzVector* mu2_lorentz, TLorentzVector* b1jet_lorentz, TLorentzVector* b2jet_lorentz,
	TLorentzVector* totjets_lorentz,TLorentzVector* met_lorentz, TLorentzVector* nu1_lorentz, TLorentzVector* nu2_lorentz,
	TLorentzVector* b_genp_lorentz,TLorentzVector* bbar_genp_lorentz, TLorentzVector* h2_lorentz, int onshellMarker_,// simulation only
         bool simulation_, bool PUsample_, int ievent, bool weightfromonshellnupt_func, bool weightfromonshellnupt_hist, bool weightfromonoffshellWmass_hist,
        int iterations, std::string RefPDFfile, bool useMET, int bjetrescaleAlgo, int metcorrection, int verbose_
	)
{


   MMCdebug=false;
   if (MMCdebug) std::cout <<" MMC::Debug::1 "<< std::endl;
   mmc_mu1_lorentz = mu1_lorentz;
   mmc_mu2_lorentz = mu2_lorentz;
   mmc_b1jet_lorentz = b1jet_lorentz;
   mmc_b2jet_lorentz = b2jet_lorentz;
   mmc_totjets_lorentz = totjets_lorentz;
   mmc_bjets_lorentz = new TLorentzVector(*b1jet_lorentz+*b2jet_lorentz);
   mmcmet_vec2 = new TVector2(met_lorentz->Px(),met_lorentz->Py());

   //std::cout <<" mu1 lorentz "; mu1_lorentz->Print();
   //std::cout <<" b1 jet lorentz "; b1jet_lorentz->Print();
   //std::cout <<" b1 gen lorentz "; b_genp_lorentz->Print();
   simulation = simulation_;
   PUsample = PUsample_;
	
   if (MMCdebug)  std::cout <<(simulation_? "in MMC simulation is true ":" in MMC simulation is false ")<< std::endl;	
   onshellMarker = onshellMarker_;
   if (simulation){
	nu1_lorentz_true = nu1_lorentz;
   	nu2_lorentz_true = nu2_lorentz;
        
        
        if (onshellMarker == 1){
        	onshellW_lorentz_true = new TLorentzVector(*nu1_lorentz_true + *mmc_mu1_lorentz);
        	offshellW_lorentz_true = new TLorentzVector(*nu2_lorentz_true + *mmc_mu2_lorentz);
        }
        else if (onshellMarker ==2 ){
        	offshellW_lorentz_true = new TLorentzVector(*nu1_lorentz_true + *mmc_mu1_lorentz);
        	onshellW_lorentz_true = new TLorentzVector(*nu2_lorentz_true + *mmc_mu2_lorentz);
	} 
        else std::cout <<" onshellMarker input error"  << std::endl;

        htoWW_lorentz_true = new TLorentzVector(*offshellW_lorentz_true + *onshellW_lorentz_true);
        h2tohh_lorentz_true = new TLorentzVector(*h2_lorentz); 
	b1_lorentz = b_genp_lorentz;
        b2_lorentz = bbar_genp_lorentz;
   	htoBB_lorentz_true = new TLorentzVector(*b_genp_lorentz+*bbar_genp_lorentz);
	ideal_met_lorentz.SetXYZM(nu1_lorentz_true->Px()+nu2_lorentz_true->Px(),nu1_lorentz_true->Py()+nu2_lorentz_true->Py(),0,0);
   }
   

   verbose = verbose_; 
   iev = ievent;

   weightfromonshellnupt_func_ = weightfromonshellnupt_func;
   weightfromonshellnupt_hist_ = weightfromonshellnupt_hist;
   weightfromonoffshellWmass_hist_ = weightfromonoffshellWmass_hist;
   iterations_ = iterations;
   RefPDFfile_ = RefPDFfile;
   useMET_ = useMET;
   metcorrection_ = metcorrection;//0.no correction; 1. simpel rescale, 2.elaborate rescale, -1.ideal case
   bjetrescale_ = bjetrescaleAlgo;//0.no rescale; 1.simple rescale; 2.elaborate rescale, -1.ideal case
   writemmctree_ = false;

   //TFile filetest(RefPDFfile_.c_str(),"READ");
   //file_ref = filetest;
   
   //writemmctree_ = false;
   std::stringstream ss;
   ss << "mmctree_" << iev;
   const std::string name(ss.str());
   //if (writemmctree_) mmctree = fs->make<TTree>(name.c_str(), name.c_str());
   if (not writemmctree_) mmctree = new TTree(name.c_str(), name.c_str());

   std::stringstream histss;
   std::stringstream histweight1ss;
   std::stringstream histweight4ss;
   histss << "MMC_h2Mass_" << iev;
   histweight1ss << "MMC_h2Mass_weight1_" << iev;
   histweight4ss << "MMC_h2Mass_weight4_" << iev;
   const std::string histname(histss.str());
   const std::string histweight1name(histweight1ss.str());
   const std::string histweight4name(histweight4ss.str());
   MMC_h2Mass = TH1F(histname.c_str(),histname.c_str(), 3800, 200, 4000);
   MMC_h2Massweight1 = TH1F(histweight1name.c_str(),histweight1name.c_str(), 3800, 200, 4000);
   MMC_h2Massweight4 = TH1F(histweight4name.c_str(),histweight4name.c_str(), 3800, 200, 4000);
   initTree(mmctree);
   
   b1rescalefactor = 1;
   b2rescalefactor = 1;
   rescalec1 = 1;
   rescalec2 = 1;
   
   nu_onshellW_lorentz = new TLorentzVector();
   nu_offshellW_lorentz = new TLorentzVector();
   offshellW_lorentz = new TLorentzVector();
   onshellW_lorentz = new TLorentzVector();
   htoWW_lorentz = new TLorentzVector();
   htoBB_lorentz = new TLorentzVector(*mmc_bjets_lorentz);
   h2tohh_lorentz = new TLorentzVector();
   met_vec2 = new TVector2(mmcmet_vec2->Px(), mmcmet_vec2->Py());
   //printTrueLorentz(); 
   file = TFile::Open(RefPDFfile_.c_str(),"READ");
   if (MMCdebug) std::cout <<" MMC::Debug::2 "<< std::endl;
}

MMC::MMC(){

  //std::cout <<" empty constructor " << std::endl;


}


//deconstructor
MMC::~MMC(){

  //std::cout << " deconstructor 1 " << std::endl;

  delete mmcmet_vec2;
  delete mmc_bjets_lorentz;
  if (simulation){

    delete offshellW_lorentz_true;
    delete onshellW_lorentz_true;
    delete htoBB_lorentz_true;
    delete htoWW_lorentz_true;
    delete h2tohh_lorentz_true;
  }
  delete nu_onshellW_lorentz;
  delete nu_offshellW_lorentz;
  delete onshellW_lorentz;
  delete offshellW_lorentz;
  delete htoWW_lorentz;
  delete htoBB_lorentz;
  delete h2tohh_lorentz;
  delete met_vec2;
  //delete file_ref;
  //std::cout <<" gFile get options " << gFile->GetOption() << std::endl;
  file->Close();
  if (not writemmctree_) delete mmctree;

}




//================================================================================================================
//
// runMMC algo
//
//
//================================================================================================================
//----------- method called to run MMC method for each case -------------------
// control 0 : take muon from onshellW as muon from onshell W and nu_offshellW_eta = some_eta+deltaeta
// control 1 : take muon from onshellW  as muon from onshell W and nu_offshellW_eta = some_eta-deltaeta
// control 2 : take muon from offshellW as muon from onshell W and nu_offshellW_eta = some_eta+deltaeta
// control 3 : take muon from offshellW as muon from onshell W and nu_offshellW_eta = some_eta-deltaeta
bool 
MMC::runMMC(){//should not include any gen level information here in final version

  // genetated (eta,phi) pair
  eta_gen = 0;
  phi_gen = 0;
  //PU0:14.8,  PU40:25.2
  float met_sigma = (PUsample? 25.2:14.8);
  //std::cout <<(PUsample?" PUsample ":"Not PUsample ")<< " met_sigma "<< met_sigma << std::endl;
  //mmctree->SetDebug(100,0,9999999);
  //int count = 100000;
  bool validrun = false;
  eta_mean=0;
  eta_rms=1.403;
  //std::cout <<" time null " << time(NULL) << std::endl;
  seed_ = time(NULL);
  TRandom3 *generator = new TRandom3();
  generator->SetSeed(seed_+iev);
  //TF1* wmasspdf = new TF1("wmasspdf","exp(x*7.87e-3+1.69)+603.47*exp(-0.5*((x-80.1)/2.0)**2)",50,90);


   if (MMCdebug) std::cout <<" MMC::Debug::3  start runMMC() in MMC class "  << std::endl; 
   float nu_onshellW_pt =0;
   wmass_gen = 80.3;// initial value
   float step,random01;
   TH1F* wmasshist = readoutonshellWMassPDF(); 
   TH2F* onoffshellWmass_hist = readoutonoffshellWMassPDF(); 
   TH1F* onshellnupt_hist = readoutonshellnuptPDF(); 
   TH1F* bjetrescalec1_hist = readoutbjetrescalec1PDF(); 
   TH1F* bjetrescalec2_hist = readoutbjetrescalec2PDF(); 
   //std::cout <<" rescale priori distribution 1" << std::endl;
   //std::cout <<" onshellnupt max content " <<onshellnupt_hist->GetBinContent(onshellnupt_hist->GetMaximumBin()) << std::endl;
   onshellnupt_hist->Scale(1.0/onshellnupt_hist->GetBinContent(onshellnupt_hist->GetMaximumBin()));
   onoffshellWmass_hist->Scale(1.0/onoffshellWmass_hist->GetBinContent(onoffshellWmass_hist->GetMaximumBin()));
   bjetrescalec1_hist->Scale(1.0/bjetrescalec1_hist->GetBinContent(bjetrescalec1_hist->GetMaximumBin()));
   bjetrescalec2_hist->Scale(1.0/bjetrescalec2_hist->GetBinContent(bjetrescalec2_hist->GetMaximumBin()));
   //replace above by normalization
   //onshellnupt_hist->Scale(1.0/onshellnupt_hist->GetBinContent(onshellnupt_hist->GetMaximumBin()));
   //onoffshellWmass_hist->Scale(1.0/onoffshellWmass_hist->GetBinContent(onoffshellWmass_hist->GetMaximumBin()));
   //bjetrescalec2_hist->Scale(1.0/bjetrescalec2_hist->GetBinContent(bjetrescalec2_hist->GetMaximumBin()));
   //std::cout <<" rescale priori distribution 2" << std::endl;
   
  // printTrueLorentz();

  for (int i = 0; i < iterations_ ; i++){

    eta_gen = generator->Uniform(-6,6); 
    phi_gen = generator->Uniform(-3.1415926, 3.1415926);
    //wmass_gen = generator->Gaus(80.385,0.015);
    hmass_gen = generator->Gaus(125.03,0.3);
    if (metcorrection_>3){
    	metpx_gen = generator->Gaus(0.0,met_sigma);
   	metpy_gen = generator->Gaus(0.0,met_sigma);
    }else {
	metpx_gen = 0;
   	metpy_gen = 0;
	}
    TVector2 met_gen = TVector2(metpx_gen, metpy_gen);
    
    //generate onshell Wmass
    step = generator->Uniform(-4,4);
    //step = generator->Gaus(0,8);
    random01 = generator->Uniform(0,1);
    //wmass_gen = onshellWMassRandomWalk(wmass_gen, step, random01);
    wmass_gen = onshellWMassRandomWalk(wmass_gen, step, random01, wmasshist);
    if (bjetrescale_ ==1){
	b1rescalefactor = 125/mmc_bjets_lorentz->M();
	b2rescalefactor = 125/mmc_bjets_lorentz->M();
	rescalec1 = b1rescalefactor; rescalec2 = b2rescalefactor;
    }
    if (bjetrescale_ ==2){
	rescalec1 = bjetrescalec1_hist->GetRandom();
	//std::cout <<" rescale c1 " << rescalec1 << std::endl;
	bool hascorrection =  bjetsCorrection();
	if (not hascorrection) continue;
	//*htoBB_lorentz = b1rescalefactor*b1lorentz+b2rescalefactor*b2lorentz; 
    } 
    if (bjetrescale_>0){
	*htoBB_lorentz = b1rescalefactor*(*mmc_b1jet_lorentz)+b2rescalefactor*(*mmc_b2jet_lorentz);
	/*if (b2rescalefactor > 4 or b2rescalefactor < 0.1){
		//continue;
        	std::cerr <<" MMC bjetrescale b1 "<< b1rescalefactor << " b2 "<< b2rescalefactor << std::endl;
		//std::cerr <<" htobb after correction mass "<< htoBB_lorentz->M(); htoBB_lorentz->Print();
	 }*/
        if (fabs(htoBB_lorentz->M()-125)>1){
		continue;
		std::cerr <<" error the htobb mass is not close 125, htobb_mass "<< htoBB_lorentz->M() << std::endl;
        	std::cerr <<" MMC bjetrescale b1 "<< b1rescalefactor << " b2 "<< b2rescalefactor << std::endl;
	 }
		
	}

    if ((metcorrection_-3)>0 and  ((metcorrection_ -3)== bjetrescale_ or metcorrection_==bjetrescale_)) metCorrection();
    else if ((metcorrection_-3) ==1 or metcorrection_==1){
	b1rescalefactor = 125/mmc_bjets_lorentz->M();
	b2rescalefactor = 125/mmc_bjets_lorentz->M();
	metCorrection(); 
    }
    else if ((metcorrection_-3) ==2 or metcorrection_==2){
	rescalec1 = bjetrescalec1_hist->GetRandom();
	bool hascorrection = bjetsCorrection();//calculate b1rescalefactor b2rescalefactor
	if (not hascorrection) continue;
	metCorrection(); 
    }
    /*else if ((metcorrection_-3) ==3){
	rescalec1 = bjetrescalec1_hist->GetRandom();
	rescalec2 = bjetrescalec2_hist->GetRandom();
        b1rescalefactor = rescalec1; b2rescalefactor=rescalec2;
	//bjetsCorrection();
	metCorrection(); 
    }*/
    

    //std::cout <<" Met input px "<< mmcmet_vec2->Px() << " py "<< mmcmet_vec2->Py() <<" pt "<< mmcmet_vec2->Mod() <<std::endl;
    //std::cout <<" met before smearing metpx_gen " << met_vec2->Px() <<" metpy_gen " << met_vec2->Py() <<std::endl;
    //std::cout <<" metpx_gen " << metpx_gen <<" metpy_gen " << metpy_gen <<std::endl;
    if (metcorrection_>3 and useMET_) *met_vec2 = *met_vec2+met_gen;
    if (MMCdebug) std::cout <<" MMC::Debug::4 in MMC loop met for MMC px " << met_vec2->Px() <<" py " << met_vec2->Py() <<std::endl;
    if (bjetrescale_ == -1 and simulation)
	*htoBB_lorentz = *htoBB_lorentz_true;
    if (metcorrection_ ==-1 and simulation)
	*met_vec2 = TVector2(nu1_lorentz_true->Px()+nu2_lorentz_true->Px(),nu1_lorentz_true->Py()+nu2_lorentz_true->Py());
    /*
    std::cout <<" MMC metCorrection b1 "<< b1rescalefactor << " b2 "<< b2rescalefactor << std::endl;
    std::cout <<"b1jet Px "<<mmc_b1jet_lorentz->Px() <<" Py "<<mmc_b1jet_lorentz->Py() 
	<<" b2jet Px "<< mmc_b2jet_lorentz->Px() <<" Py "<<mmc_b2jet_lorentz->Py() << std::endl;
    std::cout <<"sum of two nu  px "<<nu1_lorentz_true->Px()+nu2_lorentz_true->Px() <<" py "<<nu1_lorentz_true->Py()+nu2_lorentz_true->Py()<<std::endl;
    std::cout <<" MMC input met  px "<< mmcmet_vec2->Px() << " py "<<mmcmet_vec2->Py() <<" pt "<< mmcmet_vec2->Mod() <<std::endl;
    std::cout <<" met after correction px "<< met_vec2->Px() << " py "<< met_vec2->Py() <<" pt "<< met_vec2->Mod() <<std::endl;
    // std::cout <<" bjets input M_h= "<< htoBB_lorentz->M(); htoBB_lorentz->Print();
    //wmass_gen = wmasspdf->GetRandom(50.0,90.0);
    //test
    //eta_gen = eta_nuonshellW_true;
    //phi_gen = phi_nuonshellW_true;
    //wmass_gen = mass_onshellW_true; 
    std::cout << "true eta phi of nuonshell ("<<eta_nuonshellW_true <<","<<phi_nuonshellW_true<<"), pt " <<pt_nuonshellW_true 
	<<" mass of onshellW " << mass_onshellW_true <<" wmass_gen "<< wmass_gen  << std::endl;
	std::cout << "true eta phi of nuoffshell ("<<eta_nuoffshellW_true <<","<<phi_nuoffshellW_true<<"), pt " <<pt_nuoffshellW_true 
	<<" mass of offshellW " << mass_offshellW_true <<  std::endl;
     */
    //std::cout <<" eta_gen "<< eta_gen << " phi_gen " << phi_gen << " wmass "<<wmass_gen << std::endl;
    int solutions = 0;//count num of soluble case
    bool solution[4]={false, false, false, false}; //record whether the case is soluble or not
    for (int j = 0; j < 4; j++){
	assignMuLorentzVec(j/2);
	nu_onshellW_pt = nu1pt_onshellW(std::make_pair(eta_gen, phi_gen), mu_onshellW_lorentz, wmass_gen); 
	nu_onshellW_lorentz->SetPtEtaPhiM(nu_onshellW_pt, eta_gen, phi_gen,0);
	//solution[j] = nulorentz_offshellW(jets_lorentz, mu_onshellW_lorentz,
	//std::cout << " calculate nu1_pt " << nu_onshellW_pt << " px " << nu_onshellW_lorentz->Px() <<" py "<< nu_onshellW_lorentz->Py() << std::endl;
	if (useMET_)
	  solution[j] = nulorentz_offshellW(met_vec2, mu_onshellW_lorentz,
		mu_offshellW_lorentz, nu_onshellW_lorentz,
		nu_offshellW_lorentz, j%2, hmass_gen);
	else
	  solution[j] = nulorentz_offshellW(mmc_totjets_lorentz, mu_onshellW_lorentz,
		mu_offshellW_lorentz, nu_onshellW_lorentz,
		nu_offshellW_lorentz, j%2, hmass_gen);



	//std::cout << j << " nu_offshellW_eta " << nu_offshellW_lorentz->Eta()<<" phi " << nu_offshellW_lorentz->Phi() << std::endl; 
	if (solution[j]) solutions++;
    }
    //       nu_offshellW_lorentz= NULL; 
    for (int j = 0; j < 4; j++){
	//  int fill = mmctree->Fill();
	if (!solution[j])  continue;
	// reassign muons LorentzVector
	if (simulation){
	  TLorentzVector tmpnu12;
	  tmpnu12.SetXYZM(met_vec2->Px(),met_vec2->Py(),nu1_lorentz_true->Pz()+nu2_lorentz_true->Pz(),0);
	  h2tohh_expect_lorentz = *mu_onshellW_lorentz+*mu_offshellW_lorentz+*htoBB_lorentz+tmpnu12;
	  mass_h2_expect = h2tohh_expect_lorentz.M();
	}

	control = j;
	assignMuLorentzVec(j/2);
	nu_onshellW_pt = nu1pt_onshellW(std::make_pair(eta_gen, phi_gen), mu_onshellW_lorentz, wmass_gen); 
	nu_onshellW_lorentz->SetPtEtaPhiM(nu_onshellW_pt, eta_gen, phi_gen,0);
	//nulorentz_offshellW(jets_lorentz, mu_onshellW_lorentz,
	if (useMET_)
	  nulorentz_offshellW(met_vec2, mu_onshellW_lorentz,
		mu_offshellW_lorentz, nu_onshellW_lorentz,
		nu_offshellW_lorentz, j%2, hmass_gen);
	else
	  nulorentz_offshellW(mmc_totjets_lorentz, mu_onshellW_lorentz,
		mu_offshellW_lorentz, nu_onshellW_lorentz,
		nu_offshellW_lorentz, j%2, hmass_gen);

	weight = 1.0/solutions;// change weight if we consider possibility factor  like matrix elements
	mu_onshellW_Eta = mu_onshellW_lorentz->Eta();
	mu_onshellW_Phi = mu_onshellW_lorentz->Phi();
	mu_onshellW_Pt = mu_onshellW_lorentz->Pt();
	mu_onshellW_E = mu_onshellW_lorentz->E();

	mu_offshellW_Eta = mu_offshellW_lorentz->Eta();
	mu_offshellW_Phi = mu_offshellW_lorentz->Phi();
	mu_offshellW_Pt = mu_offshellW_lorentz->Pt();
	mu_offshellW_E = mu_offshellW_lorentz->E();

	nu_onshellW_Eta = nu_onshellW_lorentz->Eta();
	nu_onshellW_Phi = nu_onshellW_lorentz->Phi();
	nu_onshellW_Pt = nu_onshellW_lorentz->Pt();
	nu_onshellW_E = nu_onshellW_lorentz->E();

	nu_offshellW_Eta = nu_offshellW_lorentz->Eta();
	nu_offshellW_Phi = nu_offshellW_lorentz->Phi();
	nu_offshellW_Pt = nu_offshellW_lorentz->Pt();
	nu_offshellW_E = nu_offshellW_lorentz->E();

	*onshellW_lorentz = *mu_onshellW_lorentz+*nu_onshellW_lorentz;
	*offshellW_lorentz = *mu_offshellW_lorentz+*nu_offshellW_lorentz;
	*htoWW_lorentz = *onshellW_lorentz+*offshellW_lorentz;
	*h2tohh_lorentz = *htoWW_lorentz+*htoBB_lorentz;
	if (h2tohh_lorentz->M()<245 or h2tohh_lorentz->M()>3800) {
			std::cerr <<" MMC h2 mass is too small, or too large,  M_h " <<h2tohh_lorentz->M() << std::endl;
			std::cerr <<" gen nu eta "<< eta_gen <<" nu phi "<< phi_gen << std::endl;
			std::cerr <<" from MMC mu_onshell (px,py,pz, E)= ("<< mu_onshellW_lorentz->Px()<<", "<<  mu_onshellW_lorentz->Py()<<", "<< mu_onshellW_lorentz->Pz()<<", "<< mu_onshellW_lorentz->E() <<")"<< std::endl;
			std::cerr <<" from MMC mu_offshell (px,py,pz, E)= ("<< mu_offshellW_lorentz->Px()<<", "<<  mu_offshellW_lorentz->Py()<<", "<< mu_offshellW_lorentz->Pz()<<", "<< mu_offshellW_lorentz->E() <<")"<< std::endl;
			std::cerr <<" from MMC nu_onshell (px,py,pz, E)= ("<< nu_onshellW_lorentz->Px()<<", "<<  nu_onshellW_lorentz->Py()<<", "<< nu_onshellW_lorentz->Pz()<<", "<< nu_onshellW_lorentz->E() <<")"<< std::endl;
			std::cerr <<" from MMC nu_offshell (px,py,pz, E)= ("<< nu_offshellW_lorentz->Px()<<", "<<  nu_offshellW_lorentz->Py()<<", "<< nu_offshellW_lorentz->Pz()<<", "<< nu_offshellW_lorentz->E() <<")"<< std::endl;
			std::cerr <<" from MMC htoBB, mass "<< htoBB_lorentz->M()<<"(px,py,pz, E)= ("<<htoBB_lorentz->Px()<<", "<< htoBB_lorentz->Py() <<", "<< htoBB_lorentz->Pz() <<", "<< htoBB_lorentz->E()<<")" <<std::endl;
		if (simulation){
    			std::cerr <<"following is pure gen level infromation " << std::endl;
    			std::cerr <<" nu1 px "<<nu1_lorentz_true->Px() << " py " <<nu1_lorentz_true->Py() << " pt "<< nu1_lorentz_true->Pt() 
			<< " eta "<<nu1_lorentz_true->Eta() << " phi "<< nu1_lorentz_true->Phi() << std::endl;
    			std::cerr <<" nu2 px "<<nu2_lorentz_true->Px() << " py " <<nu2_lorentz_true->Py() << " pt "<< nu2_lorentz_true->Pt() 
			<< " eta "<<nu2_lorentz_true->Eta() << " phi "<< nu2_lorentz_true->Phi() << std::endl;
    			std::cerr <<" onshellW mass "<< onshellW_lorentz_true->M(); onshellW_lorentz_true->Print();  
    			std::cerr <<"offshellW mass " <<offshellW_lorentz_true->M(); offshellW_lorentz_true->Print();  
    			std::cerr <<" htoWW mass "<< htoWW_lorentz_true->M(); htoWW_lorentz_true->Print();
    			std::cerr <<" htoBB mass "<< htoBB_lorentz_true->M(); htoBB_lorentz_true->Print();
    			std::cerr <<" h2tohh, pz " <<h2tohh_lorentz_true->Pz() << " Mass " << h2tohh_lorentz_true->M() << std::endl;
   		}	
		
		continue;
	}
	//*h2tohh_lorentz = *htoWW_lorentz+*htoBB_lorentz_true;

	//*met_vec2 = TVector2(nu_onshellW_lorentz->Px()+nu_offshellW_lorentz->Px(),
	//				nu_onshellW_lorentz->Py()+nu_offshellW_lorentz->Py());
	if (fabs(hmass_gen-htoWW_lorentz->M()) > 2) {
	  std::cout << "  hmass_gen " << hmass_gen << " Higgs mass from MMC " << htoWW_lorentz->M() <<std::endl;
	  verbose = 4;
	}

	if (verbose > 3){
	  std::cout << " onshell W mass "<< onshellW_lorentz->M();   onshellW_lorentz->Print();
	  std::cout << " offshell W mass "<< offshellW_lorentz->M(); offshellW_lorentz->Print();
	  std::cout << " htoWW mass "<< htoWW_lorentz->M(); htoWW_lorentz->Print();
	  //std::cout << " htoBB mass "<< htoBB_lorentz->M(); htoBB_lorentz->Print();
	  std::cout << " htoBB mass "<< htoBB_lorentz_true->M(); htoBB_lorentz_true->Print();
	  verbose = 0;
	}
	if (verbose > 3 && (h2tohh_lorentz->Pt()/h2tohh_lorentz->E())>0.0000001) {
	  std::cout << " h2tohh mass "<< h2tohh_lorentz->M() <<" pt " << h2tohh_lorentz->Pt();
	  h2tohh_lorentz->Print();
	}
	onshellW_Eta = onshellW_lorentz->Eta();
	onshellW_Phi = onshellW_lorentz->Phi();
	onshellW_Pt = onshellW_lorentz->Pt();
	onshellW_E = onshellW_lorentz->E();
	onshellW_Mass = onshellW_lorentz->M();
	offshellW_Eta = offshellW_lorentz->Eta();
	offshellW_Phi = offshellW_lorentz->Phi();
	offshellW_Pt = offshellW_lorentz->Pt();
	offshellW_E = offshellW_lorentz->E();
	offshellW_Mass = offshellW_lorentz->M();
	htoWW_Eta = htoWW_lorentz->Eta();
	htoWW_Phi = htoWW_lorentz->Phi();
	htoWW_Pt = htoWW_lorentz->Pt();
	htoWW_E = htoWW_lorentz->E();
	htoWW_Mass = htoWW_lorentz->M();
	htoBB_jets_Eta = htoBB_lorentz->Eta();
	htoBB_jets_Phi = htoBB_lorentz->Phi();
	htoBB_jets_Pt = htoBB_lorentz->Pt();
	htoBB_jets_E = htoBB_lorentz->E();
	htoBB_jets_Mass = htoBB_lorentz->M();
	h2tohh_Pt = h2tohh_lorentz->Pt();
	h2tohh_E = h2tohh_lorentz->E();
	h2tohh_Mass = h2tohh_lorentz->M();
	MMCmet_Px = met_vec2->Px();
	MMCmet_Py = met_vec2->Py();
	MMCmet_E = met_vec2->Mod();
	MMCmet_Phi = met_vec2->Phi();

	if (weightfromonshellnupt_func_) weight1 = weightfromonshellnupt(nu_onshellW_pt); 
	if (weightfromonshellnupt_hist_) weight1 = weightfromhist(onshellnupt_hist, nu_onshellW_pt); 
	if (weightfromonoffshellWmass_hist_) weight2 = weightfromhist(onoffshellWmass_hist, wmass_gen, offshellW_lorentz->M()); 
	if (weightfromonoffshellWmass_hist_) weight3 = weightfromhist(onoffshellWmass_hist, wmass_gen, offshellW_lorentz->M(), false);
	if (weightfrombjetrescalec1c2_hist_) weight4 = weightfromhist(bjetrescalec2_hist, rescalec2);
	weight1 = weight1*weight;
	weight2 = weight2*weight1;
	weight3 = weight1*weight3;
	weight4 = weight4*weight1;
	if ((h2tohh_lorentz->Pt()/h2tohh_lorentz->E())>0.0000001){
	  h2tohh_Eta = h2tohh_lorentz->Eta();
	  h2tohh_Phi = h2tohh_lorentz->Phi();
	}else {//pt =0, strange case here
	  h2tohh_Eta = 1000000;
	  h2tohh_Phi = 0;
	}

	//	printMMCresult();
	//       	mmctree->Fill();
	if (weight1<=0.0) continue;
	MMC_h2Mass.Fill(h2tohh_Mass, weight);
	MMC_h2Massweight1.Fill(h2tohh_Mass, weight1);
	MMC_h2Massweight4.Fill(h2tohh_Mass, weight4);
	if (MMCdebug)  std::cout <<" h2tohh mass "<< h2tohh_Mass <<" weight " << weight <<" weight1 "<< weight1 <<" nu_onshellW_pt "<< nu_onshellW_pt << std::endl;
        validrun =true;
    }//end controls loop,(0,1,2,3)
    //mmctree->Fill();
  }//end of iteration
  std::cout <<"initial MMC input met  px "<<mmcmet_vec2->Px() << " py "<<mmcmet_vec2->Py() <<" pt "<< mmcmet_vec2->Mod() <<std::endl;
  std::cout <<"last iteration MMC input met  px "<<met_vec2->Px() << " py "<<met_vec2->Py() <<" pt "<< met_vec2->Mod() <<std::endl;
  if (validrun) std::cout <<" true validrun "<< std::endl;
  else std::cout <<" false validrun "<< std::endl;
  //std::cout <<"last iteration bjets input M_h= "<< htoBB_lorentz->M(); htoBB_lorentz->Print();
  //std::cout <<"num of solutions " << MMC_h2Mass.GetEntries() << std::endl;
  delete generator;
  return validrun;
}

//------------ method called to initialize a tree for MMC for this event ------------
void
MMC::initTree(TTree* mmctree){

  //std::cout <<" init tree "<< mmctree->GetTitle() << std::endl; 
  //initial branch value if necessary
  //
  //
  weight1 = 1.0;
  weight2 = 1.0;
  weight3 = 1.0;
  weight4 = 1.0;

  if (simulation and onshellMarker == 1){
    eta_nuoffshellW_true = nu2_lorentz_true->Eta();
    phi_nuoffshellW_true = nu2_lorentz_true->Phi();
    pt_nuoffshellW_true = nu2_lorentz_true->Pt();
    px_nuoffshellW_true = nu2_lorentz_true->Px();
    py_nuoffshellW_true = nu2_lorentz_true->Py();
    eta_nuonshellW_true = nu1_lorentz_true->Eta();
    phi_nuonshellW_true = nu1_lorentz_true->Phi();
    pt_nuonshellW_true = nu1_lorentz_true->Pt();
    px_nuonshellW_true = nu1_lorentz_true->Px();
    py_nuonshellW_true = nu1_lorentz_true->Py();
  }
  else if (simulation and onshellMarker == 2){
    eta_nuoffshellW_true = nu1_lorentz_true->Eta();
    phi_nuoffshellW_true = nu1_lorentz_true->Phi();
    pt_nuoffshellW_true = nu1_lorentz_true->Pt();
    px_nuoffshellW_true = nu1_lorentz_true->Px();
    py_nuoffshellW_true = nu1_lorentz_true->Py();
    eta_nuonshellW_true = nu2_lorentz_true->Eta();
    phi_nuonshellW_true = nu2_lorentz_true->Phi();
    pt_nuonshellW_true = nu2_lorentz_true->Pt();
    px_nuonshellW_true = nu2_lorentz_true->Px();
    py_nuonshellW_true = nu2_lorentz_true->Py();
  }

  if (simulation){ 
    htoBB_Eta = htoBB_lorentz_true->Eta();
    htoBB_Phi = htoBB_lorentz_true->Phi();
    htoBB_Pt = htoBB_lorentz_true->Pt();
    htoBB_E = htoBB_lorentz_true->E();
    htoBB_Mass = htoBB_lorentz_true->M();
    b1_Eta = b1_lorentz->Eta();
    b1_Phi = b1_lorentz->Phi();
    b1_Pt = b1_lorentz->Pt();
    b1_Px = b1_lorentz->Px();
    b1_Py = b1_lorentz->Py();
    b1_E = b1_lorentz->E();
    b1_Mass = b1_lorentz->M();
    b2_Eta = b2_lorentz->Eta();
    b2_Phi = b2_lorentz->Phi();
    b2_Pt = b2_lorentz->Pt();
    b2_Px = b2_lorentz->Px();
    b2_Py = b2_lorentz->Py();
    b2_E = b2_lorentz->E();
    b2_Mass = b2_lorentz->M();

    mass_offshellW_true = offshellW_lorentz_true->M();
    mass_onshellW_true = onshellW_lorentz_true->M();
    mass_htoWW_true = htoWW_lorentz_true->M();
    pt_h2tohh_true = h2tohh_lorentz_true->Pt();
    mass_h2tohh_true = h2tohh_lorentz_true->M();
    ideal_met_Px = ideal_met_lorentz.Px();
    ideal_met_Py = ideal_met_lorentz.Py();
    ideal_met_E = ideal_met_lorentz.Energy();
  }
  else {
    eta_nuoffshellW_true = -1;
    phi_nuoffshellW_true = -1;
    pt_nuoffshellW_true = -1;
    eta_nuonshellW_true = -1;
    phi_nuonshellW_true = -1;
    pt_nuonshellW_true = -1;

    mass_offshellW_true = -1;
    mass_onshellW_true = -1;
    mass_htoWW_true = -1;
    pt_h2tohh_true = -1;
    mass_h2tohh_true = -1;



  }
  b1jet_Eta = mmc_b1jet_lorentz->Eta();
  b1jet_Phi = mmc_b1jet_lorentz->Phi();
  b1jet_Pt = mmc_b1jet_lorentz->Pt();
  b1jet_Px = mmc_b1jet_lorentz->Px();
  b1jet_Py = mmc_b1jet_lorentz->Py();
  b1jet_Energy = mmc_b1jet_lorentz->Energy();
  b1jet_Mass = mmc_b1jet_lorentz->M();
  b2jet_Eta = mmc_b2jet_lorentz->Eta();
  b2jet_Phi = mmc_b2jet_lorentz->Phi();
  b2jet_Pt = mmc_b2jet_lorentz->Pt();
  b2jet_Px = mmc_b2jet_lorentz->Px();
  b2jet_Py = mmc_b2jet_lorentz->Py();
  b2jet_Energy = mmc_b2jet_lorentz->Energy();
  b2jet_Mass = mmc_b2jet_lorentz->M();

  if (simulation){
    b1jet_dR = mmc_b1jet_lorentz->DeltaR(*b1_lorentz);
    b2jet_dR = mmc_b2jet_lorentz->DeltaR(*b2_lorentz);
    //b1jet_dR = deltaR(b1jet_Eta, b1jet_Phi, b1_lorentz->Eta(), b1_lorentz->Phi());
    //b2jet_dR = deltaR(b2jet_Eta, b2jet_Phi, b2_lorentz->Eta(), b2_lorentz->Phi());
    b1rescalefactor_true = b1_Pt/b1jet_Pt;
    b2rescalefactor_true = b2_Pt/b2jet_Pt;
    if (b1jet_Pt>b2jet_Pt){
	rescalec1_true = b1rescalefactor_true;
	rescalec2_true = b2rescalefactor_true;
    }
    else {
	rescalec1_true = b2rescalefactor_true;
	rescalec2_true = b1rescalefactor_true;
    }
  }
  met = mmcmet_vec2->Mod(); 
  met_px = mmcmet_vec2->Px();
  met_py = mmcmet_vec2->Py();
  met_phi = mmcmet_vec2->Phi();

  mmctree->Branch("ievent", &iev);
  mmctree->Branch("eta_mean", &eta_mean);
  mmctree->Branch("eta_rms", &eta_rms);
  mmctree->Branch("eta_gen",&eta_gen);
  mmctree->Branch("phi_gen",&phi_gen);
  mmctree->Branch("wmass_gen",&wmass_gen);
  mmctree->Branch("hmass_gen",&hmass_gen);

  mmctree->Branch("mu_onshellW_eta", &mu_onshellW_Eta);
  mmctree->Branch("mu_onshellW_phi", &mu_onshellW_Phi);
  mmctree->Branch("mu_onshellW_pt", &mu_onshellW_Pt);
  mmctree->Branch("mu_onshellW_E", &mu_onshellW_E);
  mmctree->Branch("mu_offshellW_eta", &mu_offshellW_Eta);
  mmctree->Branch("mu_offshellW_phi", &mu_offshellW_Phi);
  mmctree->Branch("mu_offshellW_pt", &mu_offshellW_Pt);
  mmctree->Branch("mu_offshellW_E", &mu_offshellW_E);
  mmctree->Branch("nu_onshellW_eta", &nu_onshellW_Eta);
  mmctree->Branch("nu_onshellW_phi", &nu_onshellW_Phi);
  mmctree->Branch("nu_onshellW_pt", &nu_onshellW_Pt);
  mmctree->Branch("nu_onshellW_E", &nu_onshellW_E);
  mmctree->Branch("nu_offshellW_eta", &nu_offshellW_Eta);
  mmctree->Branch("nu_offshellW_phi", &nu_offshellW_Phi);
  mmctree->Branch("nu_offshellW_pt", &nu_offshellW_Pt);
  mmctree->Branch("nu_offshellW_E", &nu_offshellW_E);
  mmctree->Branch("onshellW_eta", &onshellW_Eta);
  mmctree->Branch("onshellW_phi", &onshellW_Phi);
  mmctree->Branch("onshellW_pt", &onshellW_Pt);
  mmctree->Branch("onshellW_E", &onshellW_E);
  mmctree->Branch("onshellW_Mass", &onshellW_Mass);
  mmctree->Branch("offshellW_eta", &offshellW_Eta);
  mmctree->Branch("offshellW_phi", &offshellW_Phi);
  mmctree->Branch("offshellW_pt", &offshellW_Pt);
  mmctree->Branch("offshellW_E", &offshellW_E);
  mmctree->Branch("offshellW_Mass", &offshellW_Mass);
  mmctree->Branch("htoWW_Eta", &htoWW_Eta);
  mmctree->Branch("htoWW_Phi", &htoWW_Phi);
  mmctree->Branch("htoWW_Pt", &htoWW_Pt);
  mmctree->Branch("htoWW_E", &htoWW_E);
  mmctree->Branch("htoWW_Mass", &htoWW_Mass);

  mmctree->Branch("b1_Eta", &b1_Eta);
  mmctree->Branch("b1_Phi", &b1_Phi);
  mmctree->Branch("b1_Pt", &b1_Pt);
  mmctree->Branch("b1_Px", &b1_Px);
  mmctree->Branch("b1_Py", &b1_Py);
  mmctree->Branch("b1_E", &b1_E);
  mmctree->Branch("b1_Mass", &b1_Mass);
  mmctree->Branch("b2_Eta", &b2_Eta);
  mmctree->Branch("b2_Phi", &b2_Phi);
  mmctree->Branch("b2_Pt", &b2_Pt);
  mmctree->Branch("b2_Px", &b2_Px);
  mmctree->Branch("b2_Py", &b2_Py);
  mmctree->Branch("b2_E", &b2_E);
  mmctree->Branch("b2_Mass", &b2_Mass);

  mmctree->Branch("b1jet_Eta", &b1jet_Eta);
  mmctree->Branch("b1jet_Phi", &b1jet_Phi);
  mmctree->Branch("b1jet_Pt", &b1jet_Pt);
  mmctree->Branch("b1jet_Px", &b1jet_Px);
  mmctree->Branch("b1jet_Py", &b1jet_Py);
  mmctree->Branch("b1jet_E", &b1jet_Energy);
  mmctree->Branch("b1jet_Mass", &b1jet_Mass);
  mmctree->Branch("b2jet_Eta", &b2jet_Eta);
  mmctree->Branch("b2jet_Phi", &b2jet_Phi);
  mmctree->Branch("b2jet_Pt", &b2jet_Pt);
  mmctree->Branch("b2jet_Px", &b2jet_Px);
  mmctree->Branch("b2jet_Py", &b2jet_Py);
  mmctree->Branch("b2jet_E", &b2jet_Energy);
  mmctree->Branch("b2jet_Mass", &b2jet_Mass);
  mmctree->Branch("b2jet_dR", &b2jet_dR);
  mmctree->Branch("b1jet_dR", &b1jet_dR);

  mmctree->Branch("htoBB_Eta", &htoBB_Eta);
  mmctree->Branch("htoBB_Phi", &htoBB_Phi);
  mmctree->Branch("htoBB_Pt", &htoBB_Pt);
  mmctree->Branch("htoBB_E", &htoBB_E);
  mmctree->Branch("htoBB_Mass", &htoBB_Mass);
  mmctree->Branch("b1rescalefactor",&b1rescalefactor);
  mmctree->Branch("b2rescalefactor",&b2rescalefactor);
  mmctree->Branch("b1rescalefactor_true",&b1rescalefactor_true);
  mmctree->Branch("b2rescalefactor_true",&b2rescalefactor_true);
  mmctree->Branch("rescalec1",&rescalec1);
  mmctree->Branch("rescalec2",&rescalec2);
  mmctree->Branch("rescalec1_true",&rescalec1_true);
  mmctree->Branch("rescalec2_true",&rescalec2_true);

  mmctree->Branch("htoBB_jets_Eta", &htoBB_jets_Eta);
  mmctree->Branch("htoBB_jets_Phi", &htoBB_jets_Phi);
  mmctree->Branch("htoBB_jets_Pt", &htoBB_jets_Pt);
  mmctree->Branch("htoBB_jets_E", &htoBB_jets_E);
  mmctree->Branch("htoBB_jets_Mass", &htoBB_jets_Mass);

  mmctree->Branch("MMCmet_E",&MMCmet_E);
  mmctree->Branch("MMCmet_Phi",&MMCmet_Phi);
  mmctree->Branch("MMCmet_Px",&MMCmet_Px);
  mmctree->Branch("MMCmet_Py",&MMCmet_Py);
  mmctree->Branch("ideal_met_Px", &ideal_met_Px);
  mmctree->Branch("ideal_met_Py", &ideal_met_Py);
  mmctree->Branch("ideal_met_E", &ideal_met_E);


  mmctree->Branch("h2tohh_Eta", &h2tohh_Eta);
  mmctree->Branch("h2tohh_Phi", &h2tohh_Phi);
  mmctree->Branch("h2tohh_Pt", &h2tohh_Pt);
  mmctree->Branch("h2tohh_E", &h2tohh_E);
  mmctree->Branch("h2tohh_Mass", &h2tohh_Mass);


  mmctree->Branch("met_true",&met);
  mmctree->Branch("met_phi_true",&met_phi);
  mmctree->Branch("met_px_true",&met_px);
  mmctree->Branch("met_py_true",&met_py);

  mmctree->Branch("eta_nuoffshellW_true", &eta_nuoffshellW_true);
  mmctree->Branch("phi_nuoffshellW_true", &phi_nuoffshellW_true);
  mmctree->Branch("eta_nuonshellW_true", &eta_nuonshellW_true);
  mmctree->Branch("phi_nuonshellW_true", &phi_nuonshellW_true);
  mmctree->Branch("pt_nuoffshellW_true", &pt_nuoffshellW_true);
  mmctree->Branch("px_nuoffshellW_true", &px_nuoffshellW_true);
  mmctree->Branch("py_nuoffshellW_true", &py_nuoffshellW_true);
  mmctree->Branch("pt_nuonshellW_true", &pt_nuonshellW_true);
  mmctree->Branch("px_nuonshellW_true", &px_nuonshellW_true);
  mmctree->Branch("py_nuonshellW_true", &py_nuonshellW_true);
  mmctree->Branch("mass_offshellW_true", &mass_offshellW_true);
  mmctree->Branch("mass_onshellW_true", &mass_onshellW_true);
  mmctree->Branch("mass_h2_true", &mass_h2tohh_true);
  mmctree->Branch("pt_h2_true", &pt_h2tohh_true);
  mmctree->Branch("mass_htoWW_true", &mass_htoWW_true);
  mmctree->Branch("mass_h2_expect", &mass_h2_expect);

  mmctree->Branch("weight", &weight);
  mmctree->Branch("weight1", &weight1);
  mmctree->Branch("weight2", &weight2);
  mmctree->Branch("weight3", &weight3);
  mmctree->Branch("weight4", &weight4);
  mmctree->Branch("control", &control);

  //also init tree


}


TLorentzVector 
MMC::calculateMET(){

  TLorentzVector METlorentz = TLorentzVector();
  //TVector2 met_pxpy(nu1cand->px()+nu2cand->px(), nu1cand->py()+nu2cand->py());
  //METlorentz.SetPxPyPzE(nu1cand->px()+nu2cand->px(), nu1cand->py()+nu2cand->py(),0,met_pxpy.Mod());

  return METlorentz;
}


//------------ method called to assign muons lorenz vector --------------
void 
MMC::assignMuLorentzVec(int control){

  //  runMMC() control/2 == 0, namely control =0 here, we have correct muon lorentz Vector pair
  //
  //std::cout <<" beign muon assignment " << std::endl;
  if (simulation){
    if (onshellMarker == 1 && control == 0){
	mu_onshellW_lorentz = mmc_mu1_lorentz;
	mu_offshellW_lorentz = mmc_mu2_lorentz; }
    else if (onshellMarker == 1 && control == 1){
	mu_onshellW_lorentz = mmc_mu2_lorentz;
	mu_offshellW_lorentz = mmc_mu1_lorentz;}
    else if (onshellMarker == 2 && control == 0){
	mu_onshellW_lorentz = mmc_mu2_lorentz;
	mu_offshellW_lorentz = mmc_mu1_lorentz;}
    else if (onshellMarker == 2 && control == 1){
	mu_onshellW_lorentz = mmc_mu1_lorentz;
	mu_offshellW_lorentz = mmc_mu2_lorentz;}
  }//simulation case
  else {
    if (control == 0){
	mu_onshellW_lorentz = mmc_mu1_lorentz;
	mu_offshellW_lorentz = mmc_mu2_lorentz;}
    else if (control == 1){
	mu_onshellW_lorentz = mmc_mu2_lorentz;
	mu_offshellW_lorentz = mmc_mu1_lorentz;}

  }//real case, assign them randomly
  //std::cout <<" end muon assignment " << std::endl;


}

// ------------ method called to generate a pair (eta,phi) for nuetrino1  ------------
EtaPhi 
MMC::generatenu1_etaphi(){

  float eta=0.0;
  float phi=0.0;

  float mean=0;
  float rms=1.403;
  eta = genEtaGuass(mean, rms);
  phi = genPhiFlat();

  return std::make_pair(eta, phi);
}

// ------------ method called to generate eta from Gauss distribution  ------------
float 
MMC::genEtaGuass(float mean, float rms){

  TRandom3 *etaGenerator = new TRandom3();
  float eta = etaGenerator->Gaus(mean, rms);
  delete etaGenerator;

  return eta;

}

// ------------ method called to generate phi from Flat distribution  ------------
float 
MMC::genPhiFlat(){

  TRandom3 *phiGenerator = new TRandom3();
  float pi = 3.1415926;
  float phi = phiGenerator->Uniform(-pi, pi);
  delete phiGenerator;

  return phi;
}

//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
TH1F*
MMC::readoutonshellWMassPDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  //TFile* file = new TFile(RefPDFfile_.c_str(), "READ");
  // TFile* file = TFile::Open(RefPDFfile_.c_str(), "READ");
  TH1F* onshellWmasspdf = (TH1F*)file->Get("onshellWmasspdf");
  // std::cout <<" print onshellWMass PDF " ; onshellWmasspdf->Print();
  //delete file;
  //file->Close();
  return onshellWmasspdf;

}

//------------ method called to readout TH1F offshellWmasspdf from root file -----------------------------
//
TH1F*
MMC::readoutoffshellWMassPDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  // TFile* file = TFile::Open(RefPDFfile_.c_str(),"READ");
  TH1F* offshellWmasspdf = (TH1F*)file->Get("offshellWmasspdf");
  //delete file;
  //file->Close();
  return offshellWmasspdf;

}


//------------ method called to readout TH2F onoffshellWmasspdf from root file -----------------------------
//
TH2F*
MMC::readoutonoffshellWMassPDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  //TFile* file = TFile::Open(RefPDFfile_.c_str(),"READ");
  TH2F* onoffshellWmasspdf = (TH2F*)file->Get("onoffshellWmasspdf");
  //std::cout <<" print onoffshellWMass PDF " ; onoffshellWmasspdf->Print();
  //delete file;
  //file->Close();
  return onoffshellWmasspdf;

}


//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
TH1F*
MMC::readoutonshellnuptPDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  // TFile* file = TFile::Open(RefPDFfile_.c_str(),"READ");
  TH1F* onshellnuptpdf = (TH1F*)file->Get("onshellnuptpdf");
  //std::cout <<" print onshellnupt PDF " ; onshellnuptpdf->Print();
  //delete file;
  //file->Close();
  return onshellnuptpdf;

}

//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
TH1F*
MMC::readoutbjetrescalec1PDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  //   TFile* file = TFile::Open(RefPDFfile_.c_str(), "READ");
  TH1F* bjetrescalec1pdf;
  bjetrescalec1pdf = PUsample?(TH1F*)file->Get("recobjetrescalec1pdfPU40"):(TH1F*)file->Get("bjetrescalec1dR4pdf");
  //bjetrescalec1pdf = (TH1F*)file->Get("recobjetrescalec1pdf");
  //delete file;
  //file->Close();
  return bjetrescalec1pdf;

}

//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
TH1F*
MMC::readoutbjetrescalec2PDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  //TFile* file = new TFile(RefPDFfile_.c_str());
  TH1F* bjetrescalec2pdf;
  bjetrescalec2pdf = PUsample?(TH1F*)file->Get("recobjetrescalec2pdfPU40"):(TH1F*)file->Get("bjetrescalec2dR4pdf");
  // std::cout <<" print c2 PDF " ; bjetrescalec2pdf->Print();
  //delete file;
  //file->Close();
  return bjetrescalec2pdf;

}

//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
TH2F*
MMC::readoutbjetrescalec1c2PDF(){


  //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
  //TFile* file = new TFile(RefPDFfile_.c_str());
  TH2F* bjetrescalec1c2pdf = (TH2F*)file->Get("bjetrescalec1c2pdf");
  /// delete file;
  //file->Close();
  return bjetrescalec1c2pdf;

}


//------------ method to describe onshellW mass Probability density function ------------------------------
//
float 
MMC::onshellWMassPDF(float mass){

  // float sigma = 1.75;
  // float mean = 80.1;
  float p0 =7.87161e-03;
  float p1 =1.69085;
  float p2 =603.474 ;
  float p = 0;
  p = exp(mass*p0+p1)+p2*exp(-0.5*((mass-80.1)/2.00)*((mass-80.1)/2.00));
  return p;
}

//------------ use random walk to generate random onshellW mass accroding to wmass pdf --------------
//
float
MMC::onshellWMassRandomWalk(float x0, float step, float random){

  float xmin = 50;
  float xmax = 90;
  float x1 = x0+step;
  while (x1 > xmax || x1 < xmin){
    if (x1 > xmax) x1 = x1-xmax+xmin;
    if (x1 < xmin) x1 = xmax-(xmin-x1);
  }
  //transition probability
  float w = onshellWMassPDF(x1)/onshellWMassPDF(x0);
  //std::cout <<" initial " <<x0 <<" step " << step << " x1 "<< x1 << " transition probability " << w << " random " << random << std::endl;
  if (w >= 1.00) return x1;
  if (w < 1.00 && random < w) return x1;
  else return x0;

}  


//------------ use random walk to generate random onshellW mass accroding to wmass pdf --------------
//
float
MMC::onshellWMassRandomWalk(float x0, float step, float random, TH1F* hist){
  float xmin = 50;
  float xmax = 90;
  //periodic boundary codition
  while (x0 > xmax || x0 < xmin){
    if (x0 > xmax) x0 = x0-xmax+xmin;
    if (x0 < xmin) x0 = xmax-(xmin-x0);
  }

  float x1 = x0+step;
  while (x1 > xmax || x1 < xmin){
    if (x1 > xmax) x1 = x1-xmax+xmin;
    if (x1 < xmin) x1 = xmax-(xmin-x1);
  }
  //find
  int binx0_1,binx0_2;
  int binx1_1,binx1_2;
  double bincent0_1,bincont0_1;// center and content
  double bincent1_1,bincont1_1;

  binx0_1 = hist->FindBin(x0);
  binx1_1 = hist->FindBin(x1);

  if ((float)hist->GetBinCenter(binx0_1) < x0){
    binx0_2 = binx0_1+1;
  }
  else {
    binx0_2 = binx0_1;
    binx0_1 = binx0_1-1;
  }

  if ((float)hist->GetBinCenter(binx1_1) < x1){
    binx1_2 = binx1_1+1;
  }
  else {
    binx1_2 = binx1_1;
    binx1_1 = binx1_1-1;
  }
  bincent0_1 = hist->GetBinCenter(binx0_1);
  bincont0_1 = hist->GetBinContent(binx0_1);
  bincent1_1 = hist->GetBinCenter(binx1_1);
  bincont1_1 = hist->GetBinContent(binx1_1);
  double w0 = (x0-bincent0_1)*(bincont0_1-hist->GetBinContent(binx0_2))/(bincent0_1-hist->GetBinCenter(binx0_2))+bincont0_1;
  double w1 = (x1-bincent1_1)*(bincont1_1-hist->GetBinContent(binx1_2))/(bincent1_1-hist->GetBinCenter(binx1_2))+bincont1_1;
  //transition probability
  double w = w1/w0;
  //std::cout <<" initial " <<x0 <<" step " << step << " x1 "<< x1 << " transition probability " << w << " random " << random << std::endl;
  if (w >= 1.00) return x1;
  if (w < 1.00 && random < (float)w) return x1;
  else return x0;

}  


//---------- weight solution by a histogram --------------------------------------------------------
//
float
MMC::weightfromhist(TH1F* hist, float x){
  //hist should be scaled

  float weight = 0.0;
  int bin1 = hist->FindBin(x);
  //first make sure that x is within range
  if (bin1 == 0 || bin1 == hist->GetNbinsX()+1) return weight=0;

  weight = hist->Interpolate(x);
  return weight;
}

//---------- weight solution by a 2d histogram --------------------------------------------------------
//
float
MMC::weightfromhist(TH2F* hist, float x, float y, bool whole){
  //hist should be scaled

  float weight = 0.0;
  int bin1 = hist->GetXaxis()->FindBin(x);
  int bin2 = hist->GetYaxis()->FindBin(y);
  //first make sure that x is within range
  if (bin1 == 0 || bin1 == hist->GetNbinsX()+1) return weight=0;
  if (bin2 == 0 || bin2 == hist->GetNbinsY()+1) return weight=0;
  weight = hist->GetBinContent(bin1, bin2);
  if (whole){
    return weight;
  }else {
    if (hist->GetBinContent(bin1, bin2) < .1) return 0;
    float integral = hist->Integral(bin1,bin1, 0, hist->GetNbinsY()+1);
    return weight/integral;
  }

}


//---------- weight solution by nupt --------------------------------------------------------
//
float
MMC::weightfromonshellnupt(float nupt){

  float weight = 0.0;
  float max = 170;
  if (nupt<0 || nupt>125) return 0.0;

  weight = -16.925+12.4066*nupt-0.2884*std::pow(nupt,2)+0.00203*std::pow(nupt,3)+7.695e-7*std::pow(nupt,4)
    -7.2191e-8*std::pow(nupt,5)+2.499e-10*std::pow(nupt,6);
  if (weight < 0 && nupt<5) return 0.0;
  if (weight < 0) std::cout << " error! nupt " << nupt << " weight " << weight << std::endl;
  weight = weight/max;
  return weight;
}


//------------- method called to calculate pt of nuetrinos from on-shell W decay ------------
float 
MMC::nu1pt_onshellW(EtaPhi nu1_etaphi, TLorentzVector* mu1lorentz, float wMass){
   
  float nu1_pt=0.0;
  //   TVector2 *numu_phi = new TVector(nu_etaphi.first(),mu1lorentz->eta());
  float deltaeta = nu1_etaphi.first - mu1lorentz->Eta();
  float deltaphi = nu1_etaphi.second - mu1lorentz->Phi();
  nu1_pt = wMass*wMass/(2*mu1lorentz->Pt()*(cosh(deltaeta)-cos(deltaphi)));
  return nu1_pt;

}

//------------ method called to check whether the solution in this case exist or not -------------
// not use now, may be helpful later 
bool
MMC::checkSolution(TLorentzVector* jetslorentz,
    TLorentzVector* mu1lorentz,
    TLorentzVector* mu2lorentz,
    TLorentzVector* nu1lorentz, int control, float hMass){



  TLorentzVector* tmplorentz = new TLorentzVector(mu1lorentz->Px()+mu2lorentz->Px()+nu1lorentz->Px(),
	mu1lorentz->Py()+mu2lorentz->Py()+nu1lorentz->Py(),
	mu1lorentz->Pz()+mu2lorentz->Pz()+nu1lorentz->Pz(),
	mu1lorentz->Energy()+mu2lorentz->Energy()+nu1lorentz->Energy());

  float nu_tmp_px;
  float nu_tmp_py;
  float nu_tmp_pt;

  nu_tmp_px = -jetslorentz->Px()-mu1lorentz->Px()-mu2lorentz->Px()-nu1lorentz->Px();
  nu_tmp_py = -jetslorentz->Py()-mu1lorentz->Py()-mu2lorentz->Py()-nu1lorentz->Py();
  TVector2 nu_pxpy(nu_tmp_px, nu_tmp_py);

  nu_tmp_pt = nu_pxpy.Mod();

  float chdeltaeta;//cosh(nu2_eta-tmp2lorenz_eta)
  TLorentzVector* tmp2lorentz = new TLorentzVector(sqrt(pow(tmplorentz->Pt(),2)+pow(tmplorentz->M(),2)),0,tmplorentz->Pz(),tmplorentz->Energy());// construct massless lorentzvector with same pz and E as tmplorentzvector

  chdeltaeta = (pow(hMass,2)+pow(jetslorentz->Pt(),2)-pow(tmplorentz->M(),2)-pow(tmplorentz->Pt(),2)-pow(nu_tmp_pt,2))/(2*tmp2lorentz->Pt()*nu_tmp_pt);


  delete tmplorentz;
  delete tmp2lorentz;

  // place the cuts we may need 
  //
  //at present if (|chdeltaeta|>1) return true; 
  return (fabs(chdeltaeta)>1);
}



//------------- method called to calculate lorentzvector of second nuetrinos, which is from offshell W -----------
// return true if we can get nu_offshellW_lorentz
bool 
MMC::nulorentz_offshellW(TLorentzVector* jetslorentz, 
    TLorentzVector* mu1lorentz, 
    TLorentzVector* mu2lorentz, 
    TLorentzVector* nu1lorentz, 
    TLorentzVector* nu2lorentz, int control, float hMass){

  TLorentzVector* tmplorentz = new TLorentzVector(mu1lorentz->Px()+mu2lorentz->Px()+nu1lorentz->Px(),
	mu1lorentz->Py()+mu2lorentz->Py()+nu1lorentz->Py(),
	mu1lorentz->Pz()+mu2lorentz->Pz()+nu1lorentz->Pz(),
	mu1lorentz->Energy()+mu2lorentz->Energy()+nu1lorentz->Energy());
  float nu_tmp_px;
  float nu_tmp_py;
  float nu_tmp_pt;

  nu_tmp_px = -jetslorentz->Px()-mu1lorentz->Px()-mu2lorentz->Px()-nu1lorentz->Px();
  nu_tmp_py = -jetslorentz->Py()-mu1lorentz->Py()-mu2lorentz->Py()-nu1lorentz->Py();
  TVector2 nu_pxpy(nu_tmp_px, nu_tmp_py);

  nu_tmp_pt = nu_pxpy.Mod();

  float chdeltaeta;//cosh(nu_offshellW_eta-tmp2lorentz_eta)
  TLorentzVector* tmp2lorentz = new TLorentzVector(sqrt(pow(tmplorentz->Pt(),2)+pow(tmplorentz->M(),2)),0,tmplorentz->Pz(),tmplorentz->Energy());//fake one massless lorentzvector with same pz and E

  chdeltaeta = (pow(hMass,2)+pow(jetslorentz->Pt(),2)-pow(tmplorentz->M(),2)-pow(tmplorentz->Pt(),2)-pow(nu_tmp_pt,2))/(2*tmp2lorentz->Pt()*nu_tmp_pt);

  if (verbose >0 ){

    std::cout << "From jetLorentz nu2 px: " << nu_tmp_px << " py: "<< nu_tmp_py << " chdeltaeta: " << chdeltaeta << std::endl;
    float chdeltaeta_tmp = (pow(hMass,2)+2*(mmcmet_vec2->Px()*tmp2lorentz->Px()+mmcmet_vec2->Py()*tmp2lorentz->Py())-pow(nu_tmp_pt,2))/(2*tmp2lorentz->Pt()*nu_tmp_pt);
    std::cout << "From mmcmet nu2 px: "<< mmcmet_vec2->Px()-nu1lorentz->Px() <<" py: "<< mmcmet_vec2->Py()-nu1lorentz->Py()
	<<" chdeltaeta: " << chdeltaeta_tmp << std::endl; 
  }
  if (chdeltaeta < 1.0) {
    delete tmplorentz;
    delete tmp2lorentz;
    nu2lorentz->SetPtEtaPhiM(0, 0, 0, 0);
    return false;
  }
  float nu_tmp_phi = nu_pxpy.Phi_mpi_pi(nu_pxpy.Phi());
  float deltaeta = acosh(chdeltaeta);
  float nu_tmp_eta = (control == 1) ? (tmp2lorentz->Eta()-deltaeta) : (tmp2lorentz->Eta()+deltaeta);//control = j%2 
  // should check whether deltaeta > 1
  // std::cout <<"control "<< control <<" nu_tmp_px " << nu_tmp_px << "  nu_tmp_py " << nu_tmp_py << " nu_tmp_pt " << nu_tmp_pt 
  //         << " cosh(deltaeta2) " << chdeltaeta << " nu_tmp_eta " << nu_tmp_eta << " nu_tmp_phi " << nu_tmp_phi << std::endl; 
  if (fabs(nu_tmp_eta) > 7) {
    delete tmplorentz;
    delete tmp2lorentz;
    nu2lorentz->SetPtEtaPhiM(0, 0, 0, 0);
    return false;  //from simulation, |nu_offshellW_Eta|<6
  }
  nu2lorentz->SetPtEtaPhiM(nu_tmp_pt, nu_tmp_eta, nu_tmp_phi, 0);
  TLorentzVector* htoww_tmp = new TLorentzVector(*tmplorentz+*nu2lorentz);
  if (abs(htoww_tmp->M()-hMass) >2){
    std::cout <<" set Higgs Mass" << hMass << " MMC higgs mass" << htoww_tmp->M() << std::endl;
    htoww_tmp->Print();
    verbose = 1;
  }
  if (verbose > 0){
    std::cout << "tmplorentz mass " << tmplorentz->M(); tmplorentz->Print();
    std::cout << "tmp2lorentz mass " << tmp2lorentz->M(); tmp2lorentz->Print();
    std::cout << " jets lorentz"; jetslorentz->Print(); 
    std::cout << " mu1 lorentz "; mu1lorentz->Print();
    std::cout << " mu2 lorentz "; mu2lorentz->Print();
    std::cout << " nu1 lorentz "; nu1lorentz->Print();
    std::cout << " tmp lorentz "; tmplorentz->Print();
    std::cout << " nu2 lorentz "; nu2lorentz->Print();
  }
  // std::cout << " nu_offshellW lorentz "; nu2lorentz->Print();
  delete tmplorentz;
  delete tmp2lorentz;
  delete htoww_tmp;

  return true; 
}



//------------- method called to calculate lorentzvector of second nuetrinos, which is from offshell W -----------
// return true if we can get nu_offshellW_lorentz
bool 
MMC::nulorentz_offshellW(TVector2* met, 
    TLorentzVector* mu1lorentz, 
    TLorentzVector* mu2lorentz, 
    TLorentzVector* nu1lorentz, 
    TLorentzVector* nu2lorentz, int control, float hMass){

  TLorentzVector* tmplorentz = new TLorentzVector(mu1lorentz->Px()+mu2lorentz->Px()+nu1lorentz->Px(),
	mu1lorentz->Py()+mu2lorentz->Py()+nu1lorentz->Py(),
	mu1lorentz->Pz()+mu2lorentz->Pz()+nu1lorentz->Pz(),
	mu1lorentz->Energy()+mu2lorentz->Energy()+nu1lorentz->Energy());
  float nu_tmp_px;
  float nu_tmp_py;
  float nu_tmp_pt;

  nu_tmp_px = met->Px()-nu1lorentz->Px();
  nu_tmp_py = met->Py()-nu1lorentz->Py();
  TVector2 nu_pxpy(nu_tmp_px, nu_tmp_py);

  nu_tmp_pt = nu_pxpy.Mod();

  float chdeltaeta;//cosh(nu_offshellW_eta-tmp2lorentz_eta)
  TLorentzVector* tmp2lorentz = new TLorentzVector(sqrt(pow(tmplorentz->Pt(),2)+pow(tmplorentz->M(),2)),0,tmplorentz->Pz(),tmplorentz->Energy());//fake one massless lorentzvector with same pz and E

  chdeltaeta = (pow(hMass,2)+2*(nu_pxpy.Px()*tmplorentz->Px()+nu_pxpy.Py()*tmplorentz->Py())-pow(tmplorentz->M(),2))/(2*tmp2lorentz->Pt()*nu_tmp_pt);
  if (verbose >0 ){
    std::cout << "nu2 px: " << nu_tmp_px << " py: "<< nu_tmp_py << std::endl;
    std::cout << "chdeltaeta " << chdeltaeta << std::endl;
    std::cout << "tmp2lorentz "; tmp2lorentz->Print();
  }
  if (chdeltaeta < 1.0) {
    delete tmplorentz;
    delete tmp2lorentz;
    nu2lorentz->SetPtEtaPhiM(0, 0, 0, 0);
    return false;
  }
  float nu_tmp_phi = nu_pxpy.Phi_mpi_pi(nu_pxpy.Phi());
  float deltaeta = acosh(chdeltaeta);
  float nu_tmp_eta = (control == 1) ? (tmp2lorentz->Eta()-deltaeta) : (tmp2lorentz->Eta()+deltaeta);//control = j%2 
  // should check whether deltaeta > 1
  // std::cout <<"control "<< control <<" nu_tmp_px " << nu_tmp_px << "  nu_tmp_py " << nu_tmp_py << " nu_tmp_pt " << nu_tmp_pt 
  //         << " cosh(deltaeta2) " << chdeltaeta << " nu_tmp_eta " << nu_tmp_eta << " nu_tmp_phi " << nu_tmp_phi << std::endl; 
  if (fabs(nu_tmp_eta) > 7) {
    delete tmplorentz;
    delete tmp2lorentz;
    nu2lorentz->SetPtEtaPhiM(0, 0, 0, 0);
    return false;  //from simulation, |nu_offshellW_Eta|<6
  }
  nu2lorentz->SetPtEtaPhiM(nu_tmp_pt, nu_tmp_eta, nu_tmp_phi, 0);
  TLorentzVector* htoww_tmp = new TLorentzVector(*tmplorentz+*nu2lorentz);
  if (abs(htoww_tmp->M()-hMass) >2){
    std::cout <<" set Higgs Mass" << hMass << " MMC higgs mass" << htoww_tmp->M() << std::endl;
    htoww_tmp->Print();
    verbose = 1;
  }
  if (verbose > 0){
    std::cout << "tmplorentz mass " << tmplorentz->M(); tmplorentz->Print();
    std::cout << "tmp2lorentz mass " << tmp2lorentz->M(); tmp2lorentz->Print();
    std::cout << " met Tvector2 "; met->Print(); 
    std::cout << " mu1 lorentz "; mu1lorentz->Print();
    std::cout << " mu2 lorentz "; mu2lorentz->Print();
    std::cout << " nu1 lorentz "; nu1lorentz->Print();
    std::cout << " tmp lorentz "; tmplorentz->Print();
    std::cout << " nu2 lorentz "; nu2lorentz->Print();
  }
  // std::cout << " nu_offshellW lorentz "; nu2lorentz->Print();
  delete tmplorentz;
  delete tmp2lorentz;
  delete htoww_tmp;

  return true; 
}

//--------------------------- bjets correction, based on c1, calculate c2 here  ----------------------------------------------------------
//use rescalec1, rescalec2 to correct bjets
bool 
MMC::bjetsCorrection(){
  //c1rescale taken from pdf
  TLorentzVector b1lorentz;
  TLorentzVector b2lorentz;
  if (mmc_b1jet_lorentz->Pt()> mmc_b2jet_lorentz->Pt()){
    b1lorentz = *mmc_b1jet_lorentz;
    b2lorentz = *mmc_b2jet_lorentz;
  }
  else {
    std::cout <<"wired b1jet is not jet with larger pt "<< std::endl;
    b1lorentz = *mmc_b2jet_lorentz;
    b2lorentz = *mmc_b1jet_lorentz;
  }
  //x1*c2*c2+x2*c2+x3=0, slove for c2
  float x1 = b2lorentz.M2();
  float x2 = 2*rescalec1*(b1lorentz*b2lorentz);
  float x3 = rescalec1*rescalec1*b1lorentz.M2()-125*125;
  if (x2<0) std::cerr <<"error bjets lorentzvector dot productor less than 0 " << std::endl;
  /*std::cout <<" b2lorentz mass "<<  b2lorentz.M2();  b2lorentz.Print();
  std::cout <<" rescale1  "<< rescalec1 <<" x1 "<< x1 <<" x2 " << x2 << " x3 "<< x3 << std::endl;
  std::cout <<"c2 solution1 " << (-x2+std::sqrt(x2*x2-4*x1*x3))/(2*x1)<<" solution2 "<<(-x2-std::sqrt(x2*x2-4*x1*x3))/(2*x1)<<std::endl;
  std::cout <<"b1jet pt "<< mmc_b1jet_lorentz->Pt() <<" b2jet pt " << mmc_b2jet_lorentz->Pt() << std::endl;
   */
  if ((x2*x2-4*x1*x3) <0 or x1==0){
	return false;
	std::cout <<" error ! there is no soluations for bjetsCorrection "<< std::endl;
  }
  rescalec2 = (-x2+std::sqrt(x2*x2-4*x1*x3))/(2*x1);
  /*if (rescalec2<=0){
	std::cout <<" b1jet mass "<< b1lorentz.M(); b1lorentz.Print(); 
	std::cout <<" b2jet mass "<< b2lorentz.M(); b2lorentz.Print(); 
	std::cout <<" x1 "<< x1 <<" x2 "<< x2 <<" x3 "<< x3 << std::endl;
  	std::cout <<"c2 solution1 " << (-x2+std::sqrt(x2*x2-4*x1*x3))/(2*x1)<<" solution2 "<<(-x2-std::sqrt(x2*x2-4*x1*x3))/(2*x1)<<std::endl;
	}*/
  if (mmc_b1jet_lorentz->Pt()> mmc_b2jet_lorentz->Pt()){
    b1rescalefactor = rescalec1;
    b2rescalefactor = rescalec2;
  }else{
    std::cout <<"wired b1jet is not jet with larger pt "<< std::endl;
    b2rescalefactor = rescalec1;
    b1rescalefactor = rescalec2;
  }
  //TLorentzVector htobb_corr = b1rescalefactor*b1lorentz+ b2rescalefactor*b2lorentz;
  //finally b1rescalefactor->b1jet b2rescalefactor->b2jet;
  //std::cout <<" htoBB m_h after correction "<< htobb_corr.M(); htobb_corr.Print();
  //std::cout <<" htoBB mmc m_h"<< htoBB_lorentz->M(); htoBB_lorentz->Print();
  return true;

}

//--------------------------- MET correction  ----------------------------------------------------------
void 
MMC::metCorrection(){
  //std::cout <<" b1rescalefactor " << b1rescalefactor << " b2rescalefactor " << b2rescalefactor << std::endl;
  float metpx_correction = mmcmet_vec2->Px()-(b1rescalefactor-1)*mmc_b1jet_lorentz->Px()-(b2rescalefactor-1)*mmc_b2jet_lorentz->Px();
  float metpy_correction = mmcmet_vec2->Py()-(b1rescalefactor-1)*mmc_b1jet_lorentz->Py()-(b2rescalefactor-1)*mmc_b2jet_lorentz->Py();
  *met_vec2 = TVector2(metpx_correction, metpy_correction);

  //std::cout <<" metCorrection metpx_correction " <<  metpx_correction <<" metpy_correction " << metpy_correction << std::endl;
}

//--------------------------- retrun MMC result ----------------------------------------------------------
TH1F
MMC::getMMCh2(){

  //std::cout <<" RMS "<< MMC_h2Mass.GetRMS() << " entries " << MMC_h2Mass.GetEntries() << std::endl;
  return MMC_h2Mass;

}

//--------------------------- retrun MMC result ----------------------------------------------------------
TH1F
MMC::getMMCh2weight1(){

  //std::cout <<" RMS "<< MMC_h2Mass.GetRMS() << " entries " << MMC_h2Mass.GetEntries() << std::endl;
  return MMC_h2Massweight1;

}

//--------------------------- retrun MMC result ----------------------------------------------------------
TH1F
MMC::getMMCh2weight4(){

  //std::cout <<" RMS "<< MMC_h2Mass.GetRMS() << " entries " << MMC_h2Mass.GetEntries() << std::endl;
  return MMC_h2Massweight4;

}

//--------------------------- retrun MMC result ----------------------------------------------------------
TTree*
MMC::getMMCTree(){

  return mmctree;

}



//----------------------------- print lorentz vectors from analyzer --------------------------------------------
//
void 
MMC::printTrueLorentz(){

  std::cout <<"  print out lorentz vector pass to MMC " << std::endl;
  if (simulation) std::cout <<" onshell channel is " << onshellMarker << std::endl;
  std::cout <<" mu1 " ; mmc_mu1_lorentz->Print();
  std::cout <<" mu2 " ; mmc_mu2_lorentz->Print();
  std::cout <<"bjets,  M_h= " << htoBB_lorentz->M(); htoBB_lorentz->Print();
  std::cout <<"met px " << mmcmet_vec2->Px() <<" py" <<mmcmet_vec2->Py() << std::endl;
  if (simulation) {
    std::cout <<"following is pure gen level infromation " << std::endl;
    std::cout <<" nu1 px "<<nu1_lorentz_true->Px() << " py " <<nu1_lorentz_true->Py() << " pt "<< nu1_lorentz_true->Pt() 
	<< " eta "<<nu1_lorentz_true->Eta() << " phi "<< nu1_lorentz_true->Phi() << std::endl;
    std::cout <<" nu2 px "<<nu2_lorentz_true->Px() << " py " <<nu2_lorentz_true->Py() << " pt "<< nu2_lorentz_true->Pt() 
	<< " eta "<<nu2_lorentz_true->Eta() << " phi "<< nu2_lorentz_true->Phi() << std::endl;
    std::cout <<" onshellW mass "<< onshellW_lorentz_true->M(); onshellW_lorentz_true->Print();  
    std::cout <<"offshellW mass " <<offshellW_lorentz_true->M(); offshellW_lorentz_true->Print();  
    std::cout <<" htoWW mass "<< htoWW_lorentz_true->M(); htoWW_lorentz_true->Print();
    std::cout <<" htoBB mass "<< htoBB_lorentz_true->M(); htoBB_lorentz_true->Print();
    std::cout <<" h2tohh, pz " <<h2tohh_lorentz_true->Pz() << " Mass " << h2tohh_lorentz_true->M() << std::endl;
  }
}


//----------------------------- print lorentz vectors from MMC results --------------------------------------------
void 
MMC::printMMCresult(){

  std::cout <<" print out results from MMC, namely survival soltions " << std::endl;
  std::cout <<" onshell mu ";mu_onshellW_lorentz->Print();
  std::cout <<" onshell nu ";nu_onshellW_lorentz->Print();
  std::cout <<"offshell mu ";mu_offshellW_lorentz->Print();
  std::cout <<"offshell nu ";nu_offshellW_lorentz->Print();
  std::cout <<" onshell W  "; onshellW_lorentz->Print();
  std::cout <<"offshell W  "; offshellW_lorentz->Print();
  std::cout <<" htoBB      "; htoBB_lorentz->Print();
  std::cout <<" h2tohh  pz"<< h2tohh_lorentz->Pz() << " mass "<< h2tohh_lorentz->M() << std::endl; 

}

#endif

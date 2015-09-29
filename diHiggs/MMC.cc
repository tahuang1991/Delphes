// -*- C++ -*-
//




//Simulation or not
#include "MMC.h"

//constructor
MMC::MMC(TLorentzVector mu1_lorentz, TLorentzVector mu2_lorentz, TLorentzVector bjets_lorentz,TLorentzVector totjets_lorentz, 
         TLorentzVector met_lorentz, TLorentzVector nu1_lorentz, TLorentzVector nu2_lorentz, TLorentzVector bbbar_genp_lorentz,
	 TLorentzVector h2_lorentz, int onshellMarker_,// only for simulation 
         bool simulation_, int ievent, bool weightfromonshellnupt_func, bool weightfromonshellnupt_hist, bool weightfromonoffshellWmass_hist,
        int iterations, std::string RefPDFfile, bool useMET, int verbose_
	)
{

   mmc_mu1_lorentz = new TLorentzVector(mu1_lorentz);
   mmc_mu2_lorentz = new TLorentzVector(mu2_lorentz);
   mmc_bjets_lorentz = new TLorentzVector(bjets_lorentz);
   mmc_totjets_lorentz = new TLorentzVector(totjets_lorentz);
   mmcmet_vec2 = new TVector2(met_lorentz.Px(),met_lorentz.Py());

   simulation = simulation_;
   onshellMarker = onshellMarker_;
   if (simulation){
	
	nu1_lorentz_true = new TLorentzVector(nu1_lorentz);
   	nu2_lorentz_true = new TLorentzVector(nu2_lorentz);
        
        
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
        h2tohh_lorentz_true = new TLorentzVector(h2_lorentz); 
        htoBB_lorentz_true = new TLorentzVector(bbbar_genp_lorentz);
	//mmcmet_vec2->Set(nu1_lorentz_true->Px()+nu2_lorentz_true->Px(),nu1_lorentz_true->Py()+nu2_lorentz_true->Py());
   }
   
   verbose = verbose_; 
   iev = ievent;

   weightfromonshellnupt_func_ = weightfromonshellnupt_func;
   weightfromonshellnupt_hist_ = weightfromonshellnupt_hist;
   weightfromonoffshellWmass_hist_ = weightfromonoffshellWmass_hist;
   iterations_ = iterations;
   RefPDFfile_ = RefPDFfile;
   useMET_ = useMET;
   std::stringstream ss;
   ss << "mmctree_" << iev;
   const std::string name(ss.str());
   mmctree = new TTree(name.c_str(), name.c_str());
   std::stringstream histss;
   histss << "MMC_h2Mass_" << iev;
   const std::string histname(histss.str());
   MMC_h2Mass = TH1F(histname.c_str(),histname.c_str(), 900, 100, 1000);
   initTree(mmctree);
   
   
   nu_onshellW_lorentz = new TLorentzVector();
   nu_offshellW_lorentz = new TLorentzVector();
   offshellW_lorentz = new TLorentzVector();
   onshellW_lorentz = new TLorentzVector();
   htoWW_lorentz = new TLorentzVector();
   htoBB_lorentz = new TLorentzVector(*mmc_bjets_lorentz);
   h2tohh_lorentz = new TLorentzVector();
   met_vec2 = new TVector2();
  

}

MMC::MMC(){
 
  //std::cout <<" empty constructor " << std::endl;


}


//deconstructor
MMC::~MMC(){
    
//  std::cout << " deconstructor " << std::endl;
  delete mmc_mu1_lorentz; 
  delete mmc_mu2_lorentz; 
  delete mmc_bjets_lorentz; 
  delete mmc_totjets_lorentz; 
  delete mmcmet_vec2;

  if (simulation){
	delete nu1_lorentz_true;
	delete nu2_lorentz_true;
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
   delete mmctree;
   
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
MMC::runMMC(){

//   TTree *mmctree = new TTree(); 
   eta_mean = 0;
   eta_rms = 0;
// genetated (eta,phi) pair
   eta_gen = 0;
   phi_gen = 0;
   //mmctree->SetDebug(100,0,9999999);
   //int count = 100000;

   eta_mean=0;
   eta_rms=1.403;
   TRandom3 *generator = new TRandom3();
   generator->SetSeed(iev);
   //TF1* wmasspdf = new TF1("wmasspdf","exp(x*7.87e-3+1.69)+603.47*exp(-0.5*((x-80.1)/2.0)**2)",50,90);

  // later should take into consideration both possible cases
  // int onshell_control = 0;
   std::cout <<" start runMMC "  << std::endl; 
   float nu_onshellW_pt =0;
   wmass_gen = 80.3;// initial value
   float step,random01;
   TH1F* wmasshist = readoutonshellWMassPDF(); 
   TH2F* onoffshellWmass_hist = readoutonoffshellWMassPDF(); 
   TH1F* onshellnupt_hist= readoutonshellnuptPDF();
   std::cout <<" rescale priori distribution 1" << std::endl;
   std::cout <<" onshellnupt max content " <<onshellnupt_hist->GetBinContent(onshellnupt_hist->GetMaximumBin()) << std::endl;
   onshellnupt_hist->Scale(1.0/onshellnupt_hist->GetBinContent(onshellnupt_hist->GetMaximumBin()));
   onoffshellWmass_hist->Scale(1.0/onoffshellWmass_hist->GetBinContent(onoffshellWmass_hist->GetMaximumBin()));
  // printTrueLorentz();
   std::cout <<"MMC input met  px "<< mmcmet_vec2->Px() << " py "<<mmcmet_vec2->Py() <<" pt "<< mmcmet_vec2->Mod() <<std::endl;
   std::cout <<" bjets input M_h= "<< htoBB_lorentz->M(); htoBB_lorentz->Print();
    
   for (int i = 0; i < iterations_ ; i++){

	   eta_gen = generator->Uniform(-6,6); 
	   phi_gen = generator->Uniform(-3.1415926, 3.1415926);
           //wmass_gen = generator->Gaus(80.385,0.015);
           hmass_gen = generator->Gaus(125.03,0.3);
           //generate onshell Wmass
           step = generator->Uniform(-4,4);
           //step = generator->Gaus(0,8);
           random01 = generator->Uniform(0,1);
           //wmass_gen = onshellWMassRandomWalk(wmass_gen, step, random01);
           wmass_gen = onshellWMassRandomWalk(wmass_gen, step, random01, wmasshist);
	    
           //wmass_gen = wmasspdf->GetRandom(50.0,90.0);
           //test
           //eta_gen = eta_nuonshellW_true;
           //phi_gen = phi_nuonshellW_true;
	   //wmass_gen = mass_onshellW_true; 
           /*std::cout << "true eta phi of nuonshell ("<<eta_nuonshellW_true <<","<<phi_nuonshellW_true<<"), pt " <<pt_nuonshellW_true 
		<<" mass of onshellW " << mass_onshellW_true <<" wmass_gen "<< wmass_gen  << std::endl;
           std::cout << "true eta phi of nuoffshell ("<<eta_nuoffshellW_true <<","<<phi_nuoffshellW_true<<"), pt " <<pt_nuoffshellW_true 
		<<" mass of offshellW " << mass_offshellW_true <<  std::endl;
            */
           int solutions = 0;//count num of soluble case
           bool solution[4]={false, false, false, false}; //record whether the case is soluble or not
           for (int j = 0; j < 4; j++){
                 assignMuLorentzVec(j/2);
          	 nu_onshellW_pt = nu1pt_onshellW(std::make_pair(eta_gen, phi_gen), mu_onshellW_lorentz, wmass_gen); 
          	 nu_onshellW_lorentz->SetPtEtaPhiM(nu_onshellW_pt, eta_gen, phi_gen,0);
                 //solution[j] = nulorentz_offshellW(jets_lorentz, mu_onshellW_lorentz,
                 if (useMET_)
                 	solution[j] = nulorentz_offshellW(mmcmet_vec2, mu_onshellW_lorentz,
					            mu_offshellW_lorentz, nu_onshellW_lorentz,
						   nu_offshellW_lorentz, j%2, hmass_gen);
		else
                 	solution[j] = nulorentz_offshellW(mmc_totjets_lorentz, mu_onshellW_lorentz,
					            mu_offshellW_lorentz, nu_onshellW_lorentz,
						   nu_offshellW_lorentz, j%2, hmass_gen);



                 //std::cout << " calculate nu1_pt " << nu_onshellW_pt << " eta_gen "<< eta_gen << " phi_gen " << phi_gen << std::endl; 
                 //std::cout << j << " nu_offshellW_eta " << nu_offshellW_lorentz->Eta()<<" phi " << nu_offshellW_lorentz->Phi() << std::endl; 
                 if (solution[j]) solutions++;
           }
    //       nu_offshellW_lorentz= NULL; 
           for (int j = 0; j < 4; j++){
	 //  int fill = mmctree->Fill();
	        if (!solution[j])  continue;
                // reassign muons LorentzVector
                control = j;
                assignMuLorentzVec(j/2);
                nu_onshellW_pt = nu1pt_onshellW(std::make_pair(eta_gen, phi_gen), mu_onshellW_lorentz, wmass_gen); 
          	nu_onshellW_lorentz->SetPtEtaPhiM(nu_onshellW_pt, eta_gen, phi_gen,0);
                 //nulorentz_offshellW(jets_lorentz, mu_onshellW_lorentz,
                if (useMET_)
                	nulorentz_offshellW(mmcmet_vec2, mu_onshellW_lorentz,
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
                //*h2tohh_lorentz = *htoWW_lorentz+*htoBB_lorentz_true;
 		
                *met_vec2 = TVector2(nu_onshellW_lorentz->Px()+nu_offshellW_lorentz->Px(),
						nu_onshellW_lorentz->Py()+nu_offshellW_lorentz->Py());
		if (fabs(hmass_gen-htoWW_lorentz->M()) > 2) {
			std::cout << "  hmass_gen " << hmass_gen << " Higgs mass from MMC " << htoWW_lorentz->M() <<std::endl;
           		verbose = 2;
                 }

                if (verbose > 1){
  			std::cout << " onshell W mass "<< onshellW_lorentz->M();   onshellW_lorentz->Print();
  			std::cout << " offshell W mass "<< offshellW_lorentz->M(); offshellW_lorentz->Print();
  			std::cout << " htoWW mass "<< htoWW_lorentz->M(); htoWW_lorentz->Print();
  			//std::cout << " htoBB mass "<< htoBB_lorentz->M(); htoBB_lorentz->Print();
  			std::cout << " htoBB mass "<< htoBB_lorentz_true->M(); htoBB_lorentz_true->Print();
                        verbose = 0;
                }
  		if (verbose > 1 && (h2tohh_lorentz->Pt()/h2tohh_lorentz->E())>0.0000001) {
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
                weight1 = weight1*weight;
 		weight2 = weight2*weight1;
                weight3 = weight1*weight3;
                if ((h2tohh_lorentz->Pt()/h2tohh_lorentz->E())>0.0000001){
                	h2tohh_Eta = h2tohh_lorentz->Eta();
               		h2tohh_Phi = h2tohh_lorentz->Phi();
                }else {//pt =0, strange case here
                        h2tohh_Eta = 1000000;
                        h2tohh_Phi = 0;
                }

                //printMMCresult();
             	mmctree->Fill();
		MMC_h2Mass.Fill(h2tohh_Mass, weight);
           }//end controls loop,(0,1,2,3)
              	//mmctree->Fill();
   }//end of tries
    std::cout <<" mmctree entries " << mmctree->GetEntries() << std::endl;
   delete generator;
   if (mmctree->GetEntries()>0) return true;
   else return false;
}

//------------ method called to initialize a tree for MMC for this event ------------
void
MMC::initTree(TTree* mmctree){
 
   std::cout <<" init tree "<< mmctree->GetTitle() << std::endl; 
   //initial branch value if necessary
   //
   //
   weight1 = 1.0;
   weight2 = 1.0;
   weight3 = 1.0;

   if (simulation and onshellMarker == 1){
	eta_nuoffshellW_true = nu2_lorentz_true->Eta();
	phi_nuoffshellW_true = nu2_lorentz_true->Phi();
	pt_nuoffshellW_true = nu2_lorentz_true->Pt();
	eta_nuonshellW_true = nu1_lorentz_true->Eta();
	phi_nuonshellW_true = nu1_lorentz_true->Phi();
	pt_nuonshellW_true = nu1_lorentz_true->Pt();
    }
    else if (simulation and onshellMarker == 2){
	eta_nuoffshellW_true = nu1_lorentz_true->Eta();
	phi_nuoffshellW_true = nu1_lorentz_true->Phi();
	pt_nuoffshellW_true = nu1_lorentz_true->Pt();
	eta_nuonshellW_true = nu2_lorentz_true->Eta();
	phi_nuonshellW_true = nu2_lorentz_true->Phi();
	pt_nuonshellW_true = nu2_lorentz_true->Pt();
    }

   if (simulation){ 
   	htoBB_Eta = htoBB_lorentz_true->Eta();
   	htoBB_Phi = htoBB_lorentz_true->Phi();
   	htoBB_Pt = htoBB_lorentz_true->Pt();
   	htoBB_E = htoBB_lorentz_true->E();
   	htoBB_Mass = htoBB_lorentz_true->M();

        mass_offshellW_true = offshellW_lorentz_true->M();
        mass_onshellW_true = onshellW_lorentz_true->M();
   	mass_htoWW_true = htoWW_lorentz_true->M();
   	pt_h2tohh_true = h2tohh_lorentz_true->Pt();
   	mass_h2tohh_true = h2tohh_lorentz_true->M();
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
   mmctree->Branch("htoBB_Eta", &htoBB_Eta);
   mmctree->Branch("htoBB_Phi", &htoBB_Phi);
   mmctree->Branch("htoBB_Pt", &htoBB_Pt);
   mmctree->Branch("htoBB_E", &htoBB_E);
   mmctree->Branch("htoBB_Mass", &htoBB_Mass);
   mmctree->Branch("htoBB_jets_Eta", &htoBB_jets_Eta);
   mmctree->Branch("htoBB_jets_Phi", &htoBB_jets_Phi);
   mmctree->Branch("htoBB_jets_Pt", &htoBB_jets_Pt);
   mmctree->Branch("htoBB_jets_E", &htoBB_jets_E);
   mmctree->Branch("htoBB_jets_Mass", &htoBB_jets_Mass);

   mmctree->Branch("MMCmet_E",&MMCmet_E);
   mmctree->Branch("MMCmet_Phi",&MMCmet_Phi);
   mmctree->Branch("MMCmet_Px",&MMCmet_Px);
   mmctree->Branch("MMCmet_Py",&MMCmet_Py);

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
   mmctree->Branch("pt_nuonshellW_true", &pt_nuonshellW_true);
   mmctree->Branch("mass_offshellW_true", &mass_offshellW_true);
   mmctree->Branch("mass_onshellW_true", &mass_onshellW_true);
   mmctree->Branch("mass_h2_true", &mass_h2tohh_true);
   mmctree->Branch("pt_h2_true", &pt_h2tohh_true);
   mmctree->Branch("mass_htoWW_true", &mass_htoWW_true);

   mmctree->Branch("weight", &weight);
   mmctree->Branch("weight1", &weight1);
   mmctree->Branch("weight2", &weight2);
   mmctree->Branch("weight3", &weight3);
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
//MMC::readoutonshellWMassPDF(TFile *file){
MMC::readoutonshellWMassPDF(){

   //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
   TFile* file = new TFile(RefPDFfile_.c_str());
   TH1F* onshellWmasspdf = (TH1F*)file->Get("onshellWmasspdf");
   onshellWmasspdf->SetDirectory(0);
   TH1::AddDirectory(kFALSE);
   delete file;
   return onshellWmasspdf;

}

//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
TH1F*
//MMC::readoutoffshellWMassPDF(TFile *file){
MMC::readoutoffshellWMassPDF(){

	
   TFile* file = new TFile(RefPDFfile_.c_str());
   //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
   TH1F* offshellWmasspdf = (TH1F*)file->Get("offshellWmasspdf");
   offshellWmasspdf->SetDirectory(0);
   TH1::AddDirectory(kFALSE);
   delete file;
   return offshellWmasspdf;

}


//------------ method called to readout TH2F onoffshellWmasspdf from root file -----------------------------
//
TH2F*
//MMC::readoutonoffshellWMassPDF(TFile *file){
MMC::readoutonoffshellWMassPDF(){

	
   TFile* file = new TFile(RefPDFfile_.c_str());
   //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
   TH2F* onoffshellWmasspdf = (TH2F*)file->Get("onoffshellWmasspdf");
   onoffshellWmasspdf->SetDirectory(0);
   TH1::AddDirectory(kFALSE);
   delete file;
   return onoffshellWmasspdf;

}


//------------ method called to readout TH1F onshellWmasspdf from root file -----------------------------
//
//void 
//MMC::readoutonshellnuptPDF(TFile* file, TH1F* testth1){
TH1F*
//MMC::readoutonshellnuptPDF(TFile* file){
MMC::readoutonshellnuptPDF(){

	
   //TFile* file = new TFile("/home/taohuang/work/CMSSW_7_3_1/src/DiHiggsWW/MMC/plugins/MMCRefPDF.ROOT");
   TFile* file = new TFile(RefPDFfile_.c_str());
   TH1F* onshellnuptpdf = (TH1F*)file->Get("onshellnuptpdf");
   onshellnuptpdf->SetDirectory(0);
   TH1::AddDirectory(kFALSE);
   delete file;
   //testth1 = (TH1F*)file->Get("onshellnuptpdf");
   //testth1->Print();
   return onshellnuptpdf;

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

//--------------------------- retrun MMC result ----------------------------------------------------------
TH1F
MMC::getMMCh2(){

    std::cout <<" RMS "<< MMC_h2Mass.GetRMS() << " entries " << MMC_h2Mass.GetEntries() << std::endl;
    return MMC_h2Mass;

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
        std::cout <<" nu1 "; nu1_lorentz_true->Print();
	std::cout <<" nu2 "; nu2_lorentz_true->Print();
        std::cout <<" onshellW "; onshellW_lorentz_true->Print();  
        std::cout <<"offshellW "; offshellW_lorentz_true->Print();  
        std::cout <<" htoWW "; htoWW_lorentz_true->Print();
        std::cout <<" htoBB "; htoBB_lorentz_true->Print();
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


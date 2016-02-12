#include <iostream>
#include <vector>
#include "TPad.h"
#include "TCanvas.h"
#include "TColor.h"
#include "TSystem.h"
#include "TImage.h"
#include "TKey.h"
#include "TH1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TObjArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TString.h"

using namespace std;

void CopyDir( TFile * thisfile, TFile * outFile, TString CommonDir );

//.x MergeFiles.C+
void MergeFiles(){

  std::vector<TString> FilesToMerge;
  FilesToMerge.clear();
  FilesToMerge.push_back("TMVA_B3_NoSim_1btag_tmp1.root");
  FilesToMerge.push_back("TMVA_B3_NoSim_1btag_tmp2.root");
  TFile *outFile=TFile::Open("TMVA_B3_NoSim_1btag_tmpALL.root", "recreate");
  //Map File - Dir Name
  map<int,TString> NamesDir;
  NamesDir[0] = "Method_Likelihood";
  NamesDir[1] = "Method_BDT";

  //Copy common files
  TFile *f = TFile::Open( FilesToMerge[0].Data() );
  TH2F *h1 = (TH2F*) f->Get("CorrelationMatrixS");
  TH2F *h2 = (TH2F*) f->Get("CorrelationMatrixB");
  TTree *Test = (TTree*) f->Get("TestTree");
  TTree *Trai = (TTree*) f->Get("TestTree");
  outFile->cd();
  h1->Write();
  h2->Write();
  Test->Write();
  Trai->Write();
  f->Close();
  //Copy specific folder and take Braches
  for(unsigned int i=0; i<FilesToMerge.size(); i++){
    cout<<"OPENING: "<<FilesToMerge[i].Data()<<endl;
    TFile *thisfile = TFile::Open( FilesToMerge[i].Data() );
    if( i==0 ){
	std::vector<TString> CommonDir;
	CommonDir.clear();
	CommonDir.push_back("InputVariables_Id");
	CommonDir.push_back("InputVariables_Deco");
	CommonDir.push_back("InputVariables_PCA");
	CommonDir.push_back("InputVariables_Gauss_Deco");
	//Loop on Each common directory
	for(unsigned int j=0; j<CommonDir.size(); j++){
	  CopyDir( thisfile, outFile, CommonDir[j] );
	}//Common dir
    }//i==0
    //Now Specific folders
    for(unsigned int k=0; k<NamesDir.size(); k++){
	if( i==k ) CopyDir( thisfile, outFile, NamesDir[k] );
    }
  }
  outFile->Close();
}

void CopyDir( TFile * thisfile, TFile * outFile, TString CommonDir ){
	  outFile->cd();
	  TDirectory *plotDirectory = thisfile->GetDirectory(CommonDir);
	  TDirectory *dir1 = outFile->mkdir(CommonDir);  
	  dir1->cd();
	  TIter listPlots(plotDirectory->GetListOfKeys());
	  TKey *key;
	  cout<<"Loop on "<<CommonDir<<endl;
	  while((key = (TKey*)listPlots())) {
	    TString typeDirectory = key->GetClassName();
	    TString nameDirectory = key->GetTitle();
	    cout<<"->Type: "<<typeDirectory<<" and Name "<<nameDirectory<<endl;
	    if(typeDirectory == "TDirectoryFile") {
		cout<<"   Is a Directory!"<<endl;
		TDirectory *plotFinalDirectory = thisfile->GetDirectory(CommonDir+"/"+nameDirectory);
		dir1->cd();
		TDirectory *dir2 = dir1->mkdir(nameDirectory);
		dir2->cd();
		TIter listPlots2(plotFinalDirectory->GetListOfKeys());
		TKey *key2;
		cout<<"    Loop2 on "<<nameDirectory<<endl;
		while((key2 = (TKey*)listPlots2())) {
		  TString typeDirectory2 = key2->GetClassName();
		  TString nameDirectory2 = key2->GetTitle();
		  cout<<"->->Type: "<<typeDirectory2<<" and Name "<<nameDirectory2<<endl;
		  if( typeDirectory2 != "TDirectoryFile" ) {
		    dir2->cd();
		    TObject *obj2 = key2->ReadObj();
		    obj2->Write();
		    outFile->cd();
		  }
		  else{
		    TDirectory *plotFinalDirectory2 = thisfile->GetDirectory(CommonDir+"/"+nameDirectory+"/"+nameDirectory2);
		    dir2->cd();
		    TDirectory *dir3 = dir2->mkdir(nameDirectory2);
		    dir3->cd();
		    TIter listPlots3(plotFinalDirectory2->GetListOfKeys());
		    TKey *key3;
		    cout<<"    Loop3 on "<<nameDirectory2<<endl;
		    while((key3 = (TKey*)listPlots3())) {
			TString typeDirectory3 = key3->GetClassName();
			TString nameDirectory3 = key3->GetTitle();
			cout<<"->->->Type: "<<typeDirectory3<<" and Name "<<nameDirectory3<<endl;
			if( typeDirectory3 != "TDirectoryFile" ) {
			  dir3->cd();
			  TObject *obj3 = key3->ReadObj();
			  obj3->Write();
			  outFile->cd();
			}
			else{
			  cout<<"WARNING!!!! ANOTHER LAYER!!!"<<endl;
			}
		    }
		  }
		}
	    }
	    else{
		cout<<"   Not a directory!"<<endl; 
		outFile->cd(CommonDir);
		TObject *obj = key->ReadObj();
		obj->Write();
		outFile->cd("");
	    }
	  }
}

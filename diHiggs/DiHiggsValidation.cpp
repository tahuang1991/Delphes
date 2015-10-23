#include <iostream>
#include <utility>
#include <vector>
#include <fstream>

#include "TROOT.h"
#include "TSystem.h"
#include "TApplication.h"

#include "TString.h"

#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TPaveText.h"
#include "TClonesArray.h"
#include "TLorentzVector.h"

#include "classes/DelphesClasses.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

#include "DiHiggstoWWbb.h"

using namespace std;

int main(int argc, char *argv[]) {

  const char *appName = "DiHiggsValidation";
  if(argc < 7)
  {
    cout << " Usage: " << appName << " missing input_file" << endl;
    cout << " input_file - input file in ROOT format ('Delphes' tree)," << endl;
    cout <<" ./DiHiggsValidation.exe -i input_file -o outputfile -config parametersconfig.txt" << endl;
    return 1;
  }
  gROOT->SetBatch();
  TString inputFile;
  TString outputFile;
  std::ifstream configfile;
  std::ifstream infile;
  bool input_good =false;

  // Loading parameter (path of the program is stored in argv[0], this is why we start at 1)
  for (int i = 1; i < argc; i++) { 
    if (i + 1 != argc){ // Check that we haven't finished parsing already
	if (string(argv[i]) == "-i" or string(argv[i]) == "-inputfile") {
	  inputFile = TString(argv[i+1]);
	  infile.open(argv[i+1]);
	} else if (string(argv[i]) == "-o" or string(argv[i])=="-outputfile") {
	  outputFile = TString(argv[i+1]);
	} else if (string(argv[i]) == "-config" or string(argv[i])=="-Config"){
	  configfile.open(argv[i+1], std::ios_base::in);
	}  
    }
  }
  input_good = (infile.good() and configfile.good());
  if (input_good){
    cout <<" inputfile " << inputFile <<endl;
    cout <<" outputfile " << outputFile << endl;
    cout <<" configfile "<< configfile << endl;
  } else{
    cout <<" failed to find input file or outputfile " << endl;
    return 0;
  }
  infile.close();

  // Here you create the class of the analyzer
  DiHiggstoWWbb *diHiggsValidate = new DiHiggstoWWbb(inputFile, outputFile, configfile);
  // Here you call the main method of the Analyzer
  diHiggsValidate->DiHiggstoWWbbrun();
  diHiggsValidate->writeTree();
  delete diHiggsValidate;
}


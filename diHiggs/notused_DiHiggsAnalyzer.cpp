#include <iostream>
#include <utility>
#include <vector>

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
//#include "DelphesClasses.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

using namespace std;

//------------------------------------------------------------------------------

// Here you can put your analysis macro

#include "DiHiggs_h2tohh.h"

//void test(TString inputFile);
//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  char *appName = "Example";

  if(argc != 3)
  {
    cout << " Usage: " << appName << " input_file" << endl;
    cout << " input_file - input file in ROOT format ('Delphes' tree)," << endl;
    return 1;
  }

  gROOT->SetBatch();
  int appargc = 1;
  char *appargv[] = {appName};
  TApplication app(appName, &appargc, appargv);

  TString inputFile(argv[1]);
  TString outputFile(argv[2]);

//------------------------------------------------------------------------------

// Here you call your macro's main function 
  cout <<" inputfile " << inputFile << std::endl;
  cout <<" outputfile " << outputFile << std::endl;
  DiHiggs_h2tohh(inputFile, outputFile);

//------------------------------------------------------------------------------

}


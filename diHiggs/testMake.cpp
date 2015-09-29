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

#include "test.h"

//void test(TString inputFile);
//------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
  const char *appName = "Example";

  if(argc != 2)
  {
    cout << " Usage: " << appName << " input_file" << endl;
    cout << " input_file - input file in ROOT format ('Delphes' tree)," << endl;
    return 1;
  }

  gROOT->SetBatch();
  //int appargc = 1;
  //char *appargv[] = {appName};
  //TApplication app(appName, &appargc, appargv);

  TString inputFile(argv[1]);

//------------------------------------------------------------------------------

// Here you call your macro's main function 
  cout <<" inputfile " << inputFile << std::endl;
  test(inputFile);

//------------------------------------------------------------------------------

}

/* 
//------------------------------------------------------------------------------
class ExRootTreeReader;
//------------------------------------------------------------------------------

void test(TString inputFile)
{
  //gSystem->Load("libDelphes");

  // Create chain of root trees
  TChain chain("Delphes");
  chain.Add(inputFile);
  
  // Create object of class ExRootTreeReader
  ExRootTreeReader *treeReader = new ExRootTreeReader(&chain);
  unsigned int numberOfEntries = treeReader->GetEntries();
  
  // Get pointers to branches used in this analysis
  TClonesArray *branchJet = treeReader->UseBranch("Jet");
  TClonesArray *branchElectron = treeReader->UseBranch("Electron");
  
  // Book histograms
  TH1 *histJetPT = new TH1F("jet_pt", "jet P_{T}", 100, 0.0, 100.0);
  TH1 *histMass = new TH1F("mass", "M_{inv}(e_{1}, e_{2})", 100, 40.0, 140.0);

  // Loop over all events
  for(unsigned int entry = 0; entry < numberOfEntries; ++entry)
  {
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);
  
    // If event contains at least 1 jet
    if(branchJet->GetEntries() > 0)
    {
      // Take first jet
      Jet *jet = (Jet*) branchJet->At(0);
      
      // Plot jet transverse momentum
      histJetPT->Fill(jet->PT);
      
      // Print jet transverse momentum
      cout << jet->PT << endl;
    }

    Electron *elec1, *elec2;

    // If event contains at least 2 electrons
    if(branchElectron->GetEntries() > 1)
    {
      // Take first two electrons
      elec1 = (Electron *) branchElectron->At(0);
      elec2 = (Electron *) branchElectron->At(1);

      // Plot their invariant mass
      histMass->Fill(((elec1->P4()) + (elec2->P4())).M());
    }
  }

  // Show resulting histograms
  histJetPT->Draw();
  histMass->Draw();
}

*/

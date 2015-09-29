
#include "TString.h"
#include "TClonesArray.h"

#include "classes/DelphesClasses.h"
//#include "DelphesClasses.h"

#include "external/ExRootAnalysis/ExRootTreeReader.h"
#include "external/ExRootAnalysis/ExRootTreeWriter.h"
#include "external/ExRootAnalysis/ExRootTreeBranch.h"
#include "external/ExRootAnalysis/ExRootResult.h"
#include "external/ExRootAnalysis/ExRootUtilities.h"

void printGenParticle(GenParticle *genP);
void printJet(Jet *jet);
void getFinalState(GenParticle* &genp, TClonesArray *branchParticle);

void DiHiggs_h2tohh(TString inputFile, TString outputfile);

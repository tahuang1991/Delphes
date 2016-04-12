#ifndef CrossSections_and_BR_h
#define CrossSections_and_BR_h
#include <iostream>
#include <utility>
#include <vector>
#include "TString.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TChain.h"

class CrossSections_and_BR{

  public:
    CrossSections_and_BR();
    ~CrossSections_and_BR();
    float GetWeight(float Lumi, float InitEv, int proc); // Return Xsec * BR

  private:
    enum {tt = 0, B3 = 1, B6 = 2};
    Float_t Xsec_tt;
    Float_t Xsec_B1;
    Float_t Xsec_B2;
    Float_t Xsec_B3;
    Float_t Xsec_B4;
    Float_t Xsec_B5;
    Float_t Xsec_B6;
    Float_t Xsec_B7;
    Float_t Xsec_B8;
    Float_t Xsec_B9;
    Float_t Xsec_B10;
    Float_t Xsec_B11;
    Float_t Xsec_B12;
    Float_t BR_h_bb;
    Float_t BR_h_WW;
    Float_t BR_W_lnu;
    Float_t BR_W_munu;
    Float_t BR_W_munu_taunu;
    Float_t BR_t_Wb;

};

#endif

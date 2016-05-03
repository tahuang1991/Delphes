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
    float GetWeight(float Lumi, float InitEv, int proc, bool Optimistic); // Return Xsec * BR

  private:
    enum {tt = 0, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12};
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
    Float_t Xsec_B1p;
    Float_t Xsec_B2p;
    Float_t Xsec_B3p;
    Float_t Xsec_B4p;
    Float_t Xsec_B5p;
    Float_t Xsec_B6p;
    Float_t Xsec_B7p;
    Float_t Xsec_B8p;
    Float_t Xsec_B9p;
    Float_t Xsec_B10p;
    Float_t Xsec_B11p;
    Float_t Xsec_B12p;
    Float_t BR_h_bb;
    Float_t BR_h_WW;
    Float_t BR_W_lnu;
    Float_t BR_W_munu;
    Float_t BR_W_munu_taunu;
    Float_t BR_t_Wb;

};

#endif




#include "deltaR.h"
#include <cmath>
#include <iostream>

float deltaR(float eta1, float phi1, float eta2, float phi2){
       
      //std::cout <<" phi1 " << phi1 << " phi2 " << phi2 << std::endl;
      double dphi = TVector2::Phi_mpi_pi(phi1-phi2);
      double deta = eta1-eta2;
      double dR = std::sqrt(deta*deta + dphi*dphi);

      //std::cout <<" dphi " << dphi << " deta " << deta << "  dR " << dR << std::endl;
      return dR;   

};

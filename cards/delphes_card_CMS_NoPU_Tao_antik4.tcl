#######################################
# Order of execution of various modules
#######################################

#  PileUpMerger
#TrackPileUpSubtractor
set ExecutionPath {
  ParticlePropagator

  ChargedHadronTrackingEfficiency
  ElectronTrackingEfficiency
  MuonTrackingEfficiency

  ChargedHadronMomentumSmearing
  ElectronEnergySmearing
  MuonMomentumSmearing

  TrackMerger
  Calorimeter
  NeutralTowerMerger
  EFlowMerger

  GenJetFinder
  NeutrinoFilter
  GenJetNoNuFinder
  GenMissingET

  PhotonEfficiency
  PhotonIsolation

  ElectronEfficiency
  ElectronIsolation

  MuonEfficiency
  MuonIsolation

  MissingET

  FastJetFinder

  JetEnergyScale

  RecoJetFlavorAssociation

  BTagging
  BTaggingLoose
  BTaggingTight
  TauTagging

  UniqueObjectFinder

  ScalarHT

  TreeWriter
}
#  BTaggingMedium
  #PileUpJetID
  #JetPileUpSubtractor



###############
# PileUp Merger
###############

module PileUpMerger PileUpMerger {
  set InputArray Delphes/stableParticles

  set ParticleOutputArray stableParticles
  set VertexOutputArray vertices

  # pre-generated minbias input file
  set PileUpFile /fdata/hepx/store/user/taohuang/Delphes_PU_Sample/MinBias.pileup

  # average expected pile up
  set MeanPileUp 50

  # maximum spread in the beam direction in m
  set ZVertexSpread 0.10

  # maximum spread in time in s
  set TVertexSpread 1.5E-09

  # vertex smearing formula f(z,t) (z,t need to be respectively given in m,s)

  set VertexDistributionFormula {exp(-(t^2/(2*(0.05/2.99792458E8*exp(-(z^2/(2*(0.05)^2))))^2)))}

  #set VertexDistributionFormula { (abs(t) <= 1.0e-09) * (abs(z) <= 0.15) * (1.00) + \
  #                                (abs(t) >  1.0e-09) * (abs(z) <= 0.15) * (0.00) + \
  #				  (abs(t) <= 1.0e-09) * (abs(z) > 0.15)  * (0.00) + \
  #				  (abs(t) >  1.0e-09) * (abs(z) > 0.15)  * (0.00)}


}



#################################
# Propagate particles in cylinder
#################################

module ParticlePropagator ParticlePropagator {
  #set InputArray PileUpMerger/stableParticles
  set InputArray Delphes/stableParticles

  set OutputArray stableParticles
  set ChargedHadronOutputArray chargedHadrons
  set ElectronOutputArray electrons
  set MuonOutputArray muons

  # radius of the magnetic field coverage, in m
  set Radius 1.29
  # half-length of the magnetic field coverage, in m
  set HalfLength 3.00

  # magnetic field
  set Bz 3.8
}

####################################
# Charged hadron tracking efficiency
####################################

module Efficiency ChargedHadronTrackingEfficiency {
  set InputArray ParticlePropagator/chargedHadrons
  set OutputArray chargedHadrons

  # add EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for charged hadrons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) + \
                                           (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.70) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0)                  * (0.95) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.60) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0)                  * (0.85) + \
                         (abs(eta) > 2.5)                                                  * (0.00)}
}

##############################
# Electron tracking efficiency
##############################

module Efficiency ElectronTrackingEfficiency {
  set InputArray ParticlePropagator/electrons
  set OutputArray electrons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for electrons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) + \
                                           (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.73) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0   && pt <= 1.0e2) * (0.95) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0e2)                * (0.99) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.50) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0   && pt <= 1.0e2) * (0.83) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0e2)                * (0.90) + \
                         (abs(eta) > 2.5)                                                  * (0.00)}
}

##########################
# Muon tracking efficiency
##########################

module Efficiency MuonTrackingEfficiency {
  set InputArray ParticlePropagator/muons
  set OutputArray muons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # tracking efficiency formula for muons
  set EfficiencyFormula {                                                    (pt <= 0.1)   * (0.00) + \
                                           (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.75) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0)                  * (0.99) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.70) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0)                  * (0.98) + \
                         (abs(eta) > 2.5)                                                  * (0.00)}
}

########################################
# Momentum resolution for charged tracks
########################################

module MomentumSmearing ChargedHadronMomentumSmearing {
  set InputArray ChargedHadronTrackingEfficiency/chargedHadrons
  set OutputArray chargedHadrons

  # set ResolutionFormula {resolution formula as a function of eta and pt}

  # resolution formula for charged hadrons
  set ResolutionFormula {                  (abs(eta) <= 1.5) * (pt > 0.1   && pt <= 1.0)   * (0.02) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0   && pt <= 1.0e1) * (0.01) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0e1 && pt <= 2.0e2) * (0.03) + \
                                           (abs(eta) <= 1.5) * (pt > 2.0e2)                * (0.05) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 1.0)   * (0.03) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0   && pt <= 1.0e1) * (0.02) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0e1 && pt <= 2.0e2) * (0.04) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 2.0e2)                * (0.05)}
}

#################################
# Energy resolution for electrons
#################################

module EnergySmearing ElectronEnergySmearing {
  set InputArray ElectronTrackingEfficiency/electrons
  set OutputArray electrons

  # set ResolutionFormula {resolution formula as a function of eta and energy}

  # resolution formula for electrons
  set ResolutionFormula {                  (abs(eta) <= 2.5) * (energy > 0.1   && energy <= 2.0e1) * (energy*0.0225) + \
                                           (abs(eta) <= 2.5) * (energy > 2.0e1)                    * sqrt(energy^2*0.007^2 + energy*0.07^2 + 0.35^2) + \
                         (abs(eta) > 2.5 && abs(eta) <= 3.0)                                       * sqrt(energy^2*0.007^2 + energy*0.07^2 + 0.35^2) + \
                         (abs(eta) > 3.0 && abs(eta) <= 5.0)                                       * sqrt(energy^2*0.107^2 + energy*2.08^2)}

}

###############################
# Momentum resolution for muons
###############################

module MomentumSmearing MuonMomentumSmearing {
  set InputArray MuonTrackingEfficiency/muons
  set OutputArray muons

  # set ResolutionFormula {resolution formula as a function of eta and pt}

  # resolution formula for muons
  set ResolutionFormula {                  (abs(eta) <= 0.5) * (pt > 0.1   && pt <= 5.0)   * (0.02) + \
                                           (abs(eta) <= 0.5) * (pt > 5.0   && pt <= 1.0e2) * (0.015) + \
                                           (abs(eta) <= 0.5) * (pt > 1.0e2 && pt <= 2.0e2) * (0.03) + \
                                           (abs(eta) <= 0.5) * (pt > 2.0e2)                * (0.05 + pt*1.e-4) + \
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 0.1   && pt <= 5.0)   * (0.03) + \
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 5.0   && pt <= 1.0e2) * (0.02) + \
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 1.0e2 && pt <= 2.0e2) * (0.04) + \
                         (abs(eta) > 0.5 && abs(eta) <= 1.5) * (pt > 2.0e2)                * (0.05 + pt*1.e-4) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 0.1   && pt <= 5.0)   * (0.04) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 5.0   && pt <= 1.0e2) * (0.035) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 1.0e2 && pt <= 2.0e2) * (0.05) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 2.0e2)                * (0.05 + pt*1.e-4)}
}

##############
# Track merger
##############

module Merger TrackMerger {
# add InputArray InputArray
  add InputArray ChargedHadronMomentumSmearing/chargedHadrons
  add InputArray ElectronEnergySmearing/electrons
  add InputArray MuonMomentumSmearing/muons
  set OutputArray tracks
}

#############
# Calorimeter
#############

module Calorimeter Calorimeter {
  set ParticleInputArray ParticlePropagator/stableParticles
  set TrackInputArray TrackMerger/tracks

  set TowerOutputArray towers
  set PhotonOutputArray photons

  set EFlowTrackOutputArray eflowTracks
  set EFlowPhotonOutputArray eflowPhotons
  set EFlowNeutralHadronOutputArray eflowNeutralHadrons

  set pi [expr {acos(-1)}]

  # lists of the edges of each tower in eta and phi
  # each list starts with the lower edge of the first tower
  # the list ends with the higher edged of the last tower

  # 5 degrees towers
  set PhiBins {}
  for {set i -36} {$i <= 36} {incr i} {
    add PhiBins [expr {$i * $pi/36.0}]
  }
  foreach eta {-1.566 -1.479 -1.392 -1.305 -1.218 -1.131 -1.044 -0.957 -0.87 -0.783 -0.696 -0.609 -0.522 -0.435 -0.348 -0.261 -0.174 -0.087 0 0.087 0.174 0.261 0.348 0.435 0.522 0.609 0.696 0.783 0.87 0.957 1.044 1.131 1.218 1.305 1.392 1.479 1.566 1.653} {
    add EtaPhiBins $eta $PhiBins
  }

  # 10 degrees towers
  set PhiBins {}
  for {set i -18} {$i <= 18} {incr i} {
    add PhiBins [expr {$i * $pi/18.0}]
  }
  foreach eta {-4.35 -4.175 -4 -3.825 -3.65 -3.475 -3.3 -3.125 -2.95 -2.868 -2.65 -2.5 -2.322 -2.172 -2.043 -1.93 -1.83 -1.74 -1.653 1.74 1.83 1.93 2.043 2.172 2.322 2.5 2.65 2.868 2.95 3.125 3.3 3.475 3.65 3.825 4 4.175 4.35 4.525} {
    add EtaPhiBins $eta $PhiBins
  }

  # 20 degrees towers
  set PhiBins {}
  for {set i -9} {$i <= 9} {incr i} {
    add PhiBins [expr {$i * $pi/9.0}]
  }
  foreach eta {-5 -4.7 -4.525 4.7 5} {
    add EtaPhiBins $eta $PhiBins
  }

  # default energy fractions {abs(PDG code)} {Fecal Fhcal}
  add EnergyFraction {0} {0.0 1.0}
  # energy fractions for e, gamma and pi0
  add EnergyFraction {11} {1.0 0.0}
  add EnergyFraction {22} {1.0 0.0}
  add EnergyFraction {111} {1.0 0.0}
  # energy fractions for muon, neutrinos and neutralinos
  add EnergyFraction {12} {0.0 0.0}
  add EnergyFraction {13} {0.0 0.0}
  add EnergyFraction {14} {0.0 0.0}
  add EnergyFraction {16} {0.0 0.0}
  add EnergyFraction {1000022} {0.0 0.0}
  add EnergyFraction {1000023} {0.0 0.0}
  add EnergyFraction {1000025} {0.0 0.0}
  add EnergyFraction {1000035} {0.0 0.0}
  add EnergyFraction {1000045} {0.0 0.0}
  # energy fractions for K0short and Lambda
  add EnergyFraction {310} {0.3 0.7}
  add EnergyFraction {3122} {0.3 0.7}

  # set ECalResolutionFormula {resolution formula as a function of eta and energy}
  set ECalResolutionFormula {                  (abs(eta) <= 3.0) * sqrt(energy^2*0.007^2 + energy*0.07^2 + 0.35^2)  + \
                             (abs(eta) > 3.0 && abs(eta) <= 5.0) * sqrt(energy^2*0.107^2 + energy*2.08^2)}

  # set HCalResolutionFormula {resolution formula as a function of eta and energy}
  set HCalResolutionFormula {                  (abs(eta) <= 3.0) * sqrt(energy^2*0.050^2 + energy*1.50^2) + \
                             (abs(eta) > 3.0 && abs(eta) <= 5.0) * sqrt(energy^2*0.130^2 + energy*2.70^2)}
}

##########################
# Track pile-up subtractor
##########################
#not use this module for PU0
module TrackPileUpSubtractor TrackPileUpSubtractor {
# add InputArray InputArray OutputArray
  add InputArray Calorimeter/eflowTracks eflowTracks
  add InputArray ElectronEnergySmearing/electrons electrons
  add InputArray MuonMomentumSmearing/muons muons

  set VertexInputArray PileUpMerger/vertices
  # assume perfect pile-up subtraction for tracks with |z| > fZVertexResolution
  # Z vertex resolution in m
  set ZVertexResolution 0.0001
}


####################
# Neutral tower merger
####################

module Merger NeutralTowerMerger {
# add InputArray InputArray
  add InputArray Calorimeter/eflowPhotons
  add InputArray Calorimeter/eflowNeutralHadrons
  set OutputArray eflowTowers
}

####################
# Energy flow merger
####################

module Merger EFlowMerger {
# add InputArray InputArray
  #add InputArray TrackPileUpSubtractor/eflowTracks #only for PU case
  add InputArray Calorimeter/eflowTracks
  add InputArray Calorimeter/eflowPhotons
  add InputArray Calorimeter/eflowNeutralHadrons
  set OutputArray eflow
}

###################
# Photon efficiency
###################

module Efficiency PhotonEfficiency {
  set InputArray Calorimeter/eflowPhotons
  set OutputArray photons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # efficiency formula for photons
  set EfficiencyFormula {                                      (pt <= 10.0) * (0.00) + \
                                           (abs(eta) <= 1.5) * (pt > 10.0)  * (0.95) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 10.0)  * (0.85) + \
                         (abs(eta) > 2.5)                                   * (0.00)}
}

##################
# Photon isolation
##################

module Isolation PhotonIsolation {
  set CandidateInputArray PhotonEfficiency/photons
  set IsolationInputArray EFlowMerger/eflow

  set OutputArray photons

  set DeltaRMax 0.3

  set PTMin 1.0

  set PTRatioMax 0.15
}

#####################
# Electron efficiency
#####################

module Efficiency ElectronEfficiency {
  #set InputArray TrackPileUpSubtractor/electrons #only for PU case
  set InputArray ElectronEnergySmearing/electrons 
  set OutputArray electrons

  # set EfficiencyFormula {efficiency formula as a function of eta and pt}

  # efficiency formula for electrons
  set EfficiencyFormula {                                      (pt <= 10.0) * (0.00) + \
                                           (abs(eta) <= 1.5) * (pt > 10.0)  * (0.95) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.5) * (pt > 10.0)  * (0.85) + \
                         (abs(eta) > 2.5)                                   * (0.00)}
}

####################
# Electron isolation
####################

module Isolation ElectronIsolation {
  set CandidateInputArray ElectronEfficiency/electrons
  set IsolationInputArray EFlowMerger/eflow

  set OutputArray electrons

  set DeltaRMax 0.3

  set PTMin 1.0

  set PTRatioMax 0.15
}

#################
# Muon efficiency
#################

module Efficiency MuonEfficiency {
  #set InputArray TrackPileUpSubtractor/muons # only for PU case
  set InputArray MuonMomentumSmearing/muons
  set OutputArray muons

  # set EfficiencyFormula {efficiency as a function of eta and pt}

  # efficiency formula for muons
  set EfficiencyFormula {                                      (pt <= 10.0)               * (0.00) + \
                                           (abs(eta) <= 1.5) * (pt > 10.0 && pt <= 1.0e3) * (0.95) + \
                                           (abs(eta) <= 1.5) * (pt > 1.0e3)               * (0.95 * exp(0.5 - pt*5.0e-4)) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.4) * (pt > 10.0 && pt <= 1.0e3) * (0.95) + \
                         (abs(eta) > 1.5 && abs(eta) <= 2.4) * (pt > 1.0e3)               * (0.95 * exp(0.5 - pt*5.0e-4)) + \
                         (abs(eta) > 2.4)                                                 * (0.00)}
}

################
# Muon isolation
################

module Isolation MuonIsolation {
  set CandidateInputArray MuonEfficiency/muons
  set IsolationInputArray EFlowMerger/eflow

  set OutputArray muons

  set DeltaRMax 0.3

  set PTMin 1.0

  set PTRatioMax 0.15
}

###################
# Missing ET merger
###################

module Merger GenMissingET {
# add InputArray InputArray
  add InputArray NeutrinoFilter/filteredParticles
  set MomentumOutputArray momentum
}

module Merger MissingET {
# add InputArray InputArray
  add InputArray EFlowMerger/eflow
  set MomentumOutputArray momentum
}

##################
# Scalar HT merger
##################

module Merger ScalarHT {
# add InputArray InputArray
  add InputArray UniqueObjectFinder/jets
  add InputArray UniqueObjectFinder/electrons
  add InputArray UniqueObjectFinder/photons
  add InputArray UniqueObjectFinder/muons
  set EnergyOutputArray energy
}

#############
# Rho pile-up
#############
module FastJetGridMedianEstimator Rho {
  set InputArray EFlowMerger/eflow
  set RhoOutputArray rho
# add GridRange rapmin rapmax drap dphi
  # rapmin - the minimum rapidity extent of the grid
  # rapmax - the maximum rapidity extent of the grid
  # drap - the grid spacing in rapidity
  # dphi - the grid spacing in azimuth
	

  add GridRange -5.0 -2.5 1.0 1.0
  add GridRange -5.0 -2.5 1.0 1.0
  add GridRange -5.0 -2.5 1.0 1.0
}


#default Rho calculation for CMS
#module FastJetFinder Rho {
#  set InputArray Calorimeter/towers
#   set InputArray EFlowMerger/eflow

 # set ComputeRho true
  #set RhoOutputArray rho

  # area algorithm: 0 Do not compute area, 1 Active area explicit ghosts, 2 One ghost passive area, 3 Passive area, 4 Voronoi, 5 Active area
  #set AreaAlgorithm 5

  # jet algorithm: 1 CDFJetClu, 2 MidPoint, 3 SIScone, 4 kt, 5 Cambridge/Aachen, 6 antikt
  #set JetAlgorithm 4
  #set ParameterR 0.6
  #set GhostEtaMax 5.0

  #add RhoEtaRange 0.0 2.5
  #add RhoEtaRange 2.5 5.0

  #set JetPTMin 0.0
#}

#####################
# MC truth jet finder
#####################

module FastJetFinder GenJetFinder {
  set InputArray Delphes/stableParticles

  set OutputArray jets

  # algorithm: 1 CDFJetClu, 2 MidPoint, 3 SIScone, 4 kt, 5 Cambridge/Aachen, 6 antikt
  set JetAlgorithm 6
  set ParameterR 0.4

  set JetPTMin 20.0
}

#####################
# Neutrino Filter
#####################

module PdgCodeFilter NeutrinoFilter {

  set InputArray Delphes/stableParticles
  set OutputArray filteredParticles

  set PTMin 0.0

  add PdgCode {12}
  add PdgCode {14}
  add PdgCode {16}
  add PdgCode {-12}
  add PdgCode {-14}
  add PdgCode {-16}
}


#####################
# MC truth jetNoNu finder 
#####################

module FastJetFinder GenJetNoNuFinder {
  set InputArray NeutrinoFilter/filteredParticles

  set OutputArray jets

  # algorithm: 1 CDFJetClu, 2 MidPoint, 3 SIScone, 4 kt, 5 Cambridge/Aachen, 6 antikt
  set JetAlgorithm 6
  set ParameterR 0.4

  set JetPTMin 20.0
}



############
# Jet finder
############

module FastJetFinder FastJetFinder {
#  set InputArray Calorimeter/towers
  set InputArray EFlowMerger/eflow

  set OutputArray jets

  # algorithm: 1 CDFJetClu, 2 MidPoint, 3 SIScone, 4 kt, 5 Cambridge/Aachen, 6 antikt
  set JetAlgorithm 6
  set ParameterR 0.4

  set JetPTMin 20.0
}

###########################
# Jet Pile-Up ID
###########################
#Not use for PU0
module PileUpJetID PileUpJetID {
  set JetInputArray FastJetFinder/jets
  set TrackInputArray Calorimeter/eflowTracks
  set NeutralInputArray NeutralTowerMerger/eflowTowers

  set VertexInputArray PileUpMerger/vertices
  # assume perfect pile-up subtraction for tracks with |z| > fZVertexResolution
  # Z vertex resolution in m
  set ZVertexResolution 0.0001

  set OutputArray jets

  set UseConstituents 0
  set ParameterR 0.4

  set JetPTMin 20.0
}

###########################
# Jet Pile-Up Subtraction
###########################
#Not use for PU0
module JetPileUpSubtractor JetPileUpSubtractor {
  set JetInputArray PileUpJetID/jets
  set RhoInputArray Rho/rho

  set OutputArray jets

  set JetPTMin 20.0
}


##################
# Jet Energy Scale
##################

module EnergyScale JetEnergyScale {
  set InputArray FastJetFinder/jets
  #set InputArray JetPileUpSubtractor/jets #only for PU case
  set OutputArray jets

 # scale formula for jets
  set ScaleFormula {1.00}
}


########################
# Jet Flavor Association
########################

module JetFlavorAssociation RecoJetFlavorAssociation {
  
  set PartonInputArray Delphes/partons
  set ParticleInputArray Delphes/allParticles
  set ParticleLHEFInputArray Delphes/allParticlesLHEF
  set JetInputArray JetEnergyScale/jets
  
  set DeltaR 0.5
  set PartonPTMin 1.0
  set PartonEtaMax 2.5

}


###########
# b-tagging
###########

module BTagging BTaggingLoose {
#  set PartonInputArray Delphes/partons
  set JetInputArray JetEnergyScale/jets
  set BitNumber 0

  add EfficiencyFormula {0}      {(pt <= 20.0) * (0.000) + \
			          (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.096) + \
 			          (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.112) + \
				  (abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.0812) + \
				  (abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.0908) + \
				  (abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.0792) + \
				  (abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.0858) + \
				  (abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.0917) + \
				  (abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.084) + \
				  (abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.0906) + \
				  (abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.0989) + \
				  (abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.1022) + \
				  (abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.1035) + \
				  (abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.1095) + \
				  (abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.1201) + \
				  (abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.1348) + \
				  (abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.1482) + \
				  (abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.1629) + \
				  (abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.1775) + \
				  (abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.2002) + \
				  (abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.1977) + \
				  (abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.2084) + \
				  (abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.2195) + \
				  (abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.2424) + \
				  (abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.2909) + \
				  (abs(eta) <= 1.8) * (pt > 2000.0) * (0.3457) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.074765) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.100053) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.071492) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.084796) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.076927) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.08424) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.093118) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.084629) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.092977) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.10206) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.102344) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.098435) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.105507) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.112841) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.126329) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.140759) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.153193) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.107869) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.119527) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.08688) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.089324) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.097172) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.000) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.000) + \
				  (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.000) + \
                                  (abs(eta) > 2.4 && abs(eta) <= 4.0) * (0.02) +\
                                  (abs(eta) > 4.0) * (0.000) \
  }

  add EfficiencyFormula {4}          { (pt <= 20.0) * (0.000) + \
				       (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.387) + \
				       (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.448) + \
				       (abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.408) + \
				       (abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.427) + \
				       (abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.408) + \
				       (abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.425) + \
				       (abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.426) + \
				       (abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.4) + \
				       (abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.415) + \
				       (abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.416) + \
				       (abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.405) + \
				       (abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.387) + \
				       (abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.39) + \
				       (abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.389) + \
				       (abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.389) + \
				       (abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.381) + \
				       (abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.381) + \
				       (abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.367) + \
				       (abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.369) + \
				       (abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.326) + \
				       (abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.335) + \
				       (abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.326) + \
				       (abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.341) + \
				       (abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.403) + \
				       (abs(eta) <= 1.8) * (pt > 2000.0) * (0.47) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.2497) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.31891) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.273) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.28445) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.28036) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.28453) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.29495) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.27024) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.28912) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.29048) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.27507) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.23327) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.2493) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.2416) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.26652) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.24852) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.26927) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.19302) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.19433) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.17523) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.14981) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.16666) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
 				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.) + \
                                       (abs(eta) > 2.4 && abs(eta) <= 4.0) * (pt > 15.0) * (0.29*tanh(pt*0.00997 - 0.143)) + \
                                       (abs(eta) > 4.0) * (0.000)
  }

   # efficiency formula for b-jets
  add EfficiencyFormula {5}          { (pt <= 20.0) * (0.000) + \
				       (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.75) + \
				       (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.827) + \
				       (abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.837) + \
				       (abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.853) + \
				       (abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.855) + \
				       (abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.862) + \
				       (abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.868) + \
				       (abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.865) + \
				       (abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.863) + \
				       (abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.857) + \
				       (abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.851) + \
				       (abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.838) + \
				       (abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.831) + \
				       (abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.817) + \
				       (abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.796) + \
				       (abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.772) + \
				       (abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.76) + \
				       (abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.743) + \
				       (abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.703) + \
				       (abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.638) + \
				       (abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.605) + \
				       (abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.572) + \
				       (abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.541) + \
				       (abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.567) + \
				       (abs(eta) <= 1.8) * (pt > 2000.0) * (0.603) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.6063) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.7188) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.72) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.7365) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.7462) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.7454) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.7415) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.727) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.7112) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.7112) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.6754) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.6359) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.6348) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.6115) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.5585) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.5608) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.5208) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.456) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.4524) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.388) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.3928) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.3823) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.0) + \
                                       (abs(eta) > 2.4 && abs(eta) <= 4.0) * (pt > 15.0) * (0.584522*tanh(pt*0.0144387 + 0.397034)) + \
                                       (abs(eta) > 4.0) * (0.000)
  }
}


###########
# b-tagging
# medium
###########

module BTagging BTagging {
#  set PartonInputArray Delphes/partons
  set JetInputArray JetEnergyScale/jets

  set BitNumber 1

#  set DeltaR 0.5

#  set PartonPTMin 1.0

# set PartonEtaMax 2.5

  # add EfficiencyFormula {abs(PDG code)} {efficiency formula as a function of eta and pt}
  # PDG code = the highest PDG code of a quark or gluon inside DeltaR cone around jet axis
  # gluon's PDG code has the lowest priority

  # https://twiki.cern.ch/twiki/bin/view/CMSPublic/PhysicsResultsBTV
  # default efficiency formula (misidentification rate)
  #add EfficiencyFormula {0} {0.001}

  # efficiency formula for c-jets (misidentification rate)
  #add EfficiencyFormula {4} {                                      (pt <= 15.0) * (0.000) + \
                                                (abs(eta) <= 1.2) * (pt > 15.0) * (0.2*tanh(pt*0.03 - 0.4)) + \
                              (abs(eta) > 1.2 && abs(eta) <= 2.5) * (pt > 15.0) * (0.1*tanh(pt*0.03 - 0.4)) + \
                              (abs(eta) > 2.5)                                  * (0.000)}

  # efficiency formula for b-jets
  #add EfficiencyFormula {5} {                                      (pt <= 15.0) * (0.000) + \
                                                (abs(eta) <= 1.2) * (pt > 15.0) * (0.5*tanh(pt*0.03 - 0.4)) + \
                              (abs(eta) > 1.2 && abs(eta) <= 2.5) * (pt > 15.0) * (0.4*tanh(pt*0.03 - 0.4)) + \
                              (abs(eta) > 2.5)                                  * (0.000)}
  # based on arXiv:1211.4462
  
  # default efficiency formula (misidentification rate)
  add EfficiencyFormula {0} {0.01+0.00038*pt}

  # efficiency formula for c-jets (misidentification rate)
  add EfficiencyFormula {4} {0.25*tanh(0.018*pt)*(1/(1+ 0.0013*pt))}

  # efficiency formula for b-jets
  add EfficiencyFormula {5} {0.85*tanh(0.0025*pt)*(25.0/(1+0.063*pt))}

}


###########
# b-tagging
# medium
###########
module BTagging BTaggingMedium {


  set JetInputArray JetEnergyScale/jets
  set BitNumber 1
 
  add EfficiencyFormula {0}       { (pt <= 20.0) * (0.000) + \
				    (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.00654) + \
				    (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.00921) + \
					(abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.00573) + \
					(abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.00694) + \
					(abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.0062) + \
					(abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.00708) + \
					(abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.00779) + \
					(abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.00693) + \
					(abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.00777) + \
					(abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.00862) + \
					(abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.01038) + \
					(abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.01189) + \
					(abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.01279) + \
					(abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.01452) + \
					(abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.01696) + \
					(abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.01958) + \
					(abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.02253) + \
					(abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.01787) + \
					(abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.02154) + \
					(abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.01839) + \
					(abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.01987) + \
					(abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.02351) + \
					(abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.02937) + \
					(abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.04001) + \
					(abs(eta) <= 1.8) * (pt > 2000.0) * (0.0542) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.004236) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.006653) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.005512) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.00754) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.005813) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.006439) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.008063) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.00647) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.007583) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.008543) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.01034) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.011253) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.012945) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.014474) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.017361) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.020912) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.023139) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.010756) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.012569) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.006046) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.006428) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.00887) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.000) + \
 	   			        (abs(eta) > 2.4 && abs(eta) <= 4.0) * (0.001) + \
                                        (abs(eta) > 4.0) * (0.000) \
}

 # efficiency formula for c-jets (misidentification rate)
  add EfficiencyFormula {4}           { (pt <= 20.0) * (0.000) + \
					(abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.1102) + \
					(abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.1344) + \
					(abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.1025) + \
					(abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.1025) + \
					(abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.1063) + \
					(abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.1087) + \
					(abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.1124) + \
					(abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.109) + \
					(abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.111) + \
					(abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.1091) + \
					(abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.1087) + \
					(abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.1091) + \
					(abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.1107) + \
					(abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.1061) + \
					(abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.1017) + \
					(abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.1) + \
					(abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.0966) + \
					(abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.0697) + \
					(abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.0679) + \
					(abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.0503) + \
					(abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.0514) + \
					(abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.0481) + \
					(abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.0667) + \
					(abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.0861) + \
					(abs(eta) <= 1.8) * (pt > 2000.0) * (0.092) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.03331) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.04361) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.03863) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.04287) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.04431) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.04452) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.04339) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.0436) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.0456) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.05138) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.04794) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.04004) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.04713) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.04515) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.05314) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.05143) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.05936) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.02357) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.03222) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.01523) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.02621) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.01709) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.) + \
                                        (abs(eta) > 2.4 && abs(eta) <= 4.0) * (pt > 15.0) * (0.1898*tanh(pt*0.00997 - 0.143)) + \
                                        (abs(eta) > 4.0) * (0.000)
  }

 # efficiency formula for b-jets
  add EfficiencyFormula {5}           { (pt <= 20.0) * (0.000) + \
					(abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.536) + \
					(abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.6439) + \
					(abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.6504) + \
					(abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.6716) + \
					(abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.6841) + \
					(abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.6896) + \
					(abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.6916) + \
					(abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.6882) + \
					(abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.6838) + \
					(abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.6715) + \
					(abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.6554) + \
					(abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.6366) + \
					(abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.6192) + \
					(abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.595) + \
					(abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.5551) + \
					(abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.5138) + \
					(abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.4884) + \
					(abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.4009) + \
					(abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.3459) + \
					(abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.2523) + \
					(abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.2404) + \
					(abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.2198) + \
					(abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.2263) + \
					(abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.2614) + \
					(abs(eta) <= 1.8) * (pt > 2000.0) * (0.3194) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.3254) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.4339) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.4499) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.4716) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.4766) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.4788) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.4863) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.4891) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.462) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.4583) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.4247) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.3775) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.3734) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.3348) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.2939) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.285) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.2421) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.1565) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.1522) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.1231) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.1607) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.1323) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
					(abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.0) + \
				            (abs(eta) > 2.4 && abs(eta) <= 4.0) * (pt > 15.0) * (0.584522*tanh(pt*0.0144387 + 0.397034)) + \
                                        (abs(eta) > 4.0) * (0.000)
 }

}

###########
# b-tagging
###########

module BTagging BTaggingTight {
#  set PartonInputArray Delphes/partons
  set JetInputArray JetEnergyScale/jets

  set BitNumber 2

  add EfficiencyFormula {0}          { (pt <= 20.0) * (0.000) + \
				       (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.000164) + \
				       (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.000235) + \
				       (abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.000266) + \
				       (abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.000329) + \
				       (abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.000309) + \
				       (abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.000309) + \
				       (abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.000546) + \
				       (abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.000499) + \
				       (abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.000642) + \
				       (abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.000742) + \
				       (abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.000928) + \
				       (abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.001323) + \
				       (abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.001392) + \
				       (abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.00154) + \
				       (abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.002094) + \
				       (abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.002427) + \
				       (abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.002927) + \
				       (abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.001854) + \
				       (abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.002355) + \
				       (abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.002297) + \
				       (abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.002433) + \
				       (abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.002706) + \
				       (abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.003602) + \
				       (abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.004987) + \
				       (abs(eta) <= 1.8) * (pt > 2000.0) * (0.007414) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.000573) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.000574) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.000521) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.000786) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.000539) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.000673) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.000934) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.000781) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.000949) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.000977) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.001168) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.000879) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.000812) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.001215) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.001679) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.001893) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.002723) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.003555) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.003881) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.006046) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.005563) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.007611) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.000) + \
                                       (abs(eta) > 2.4 && abs(eta) <= 4.0) * (0.001) + \
                                       (abs(eta) > 4.0) * (0.000) \
   }

 # efficiency formula for c-jets (misidentification rate)
  add EfficiencyFormula {4}     { (pt <= 20.0) * (0.000) + \
				  (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.00531) + \
				       (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.00567) + \
				       (abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.0064) + \
				       (abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.00673) + \
				       (abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.00766) + \
				       (abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.00729) + \
				       (abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.00674) + \
				       (abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.00824) + \
				       (abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.00888) + \
				       (abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.00919) + \
				       (abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.01021) + \
				       (abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.01041) + \
				       (abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.01027) + \
				       (abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.00999) + \
				       (abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.01047) + \
				       (abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.01014) + \
				       (abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.01021) + \
				       (abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.00601) + \
				       (abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.0054) + \
				       (abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.00487) + \
				       (abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.00519) + \
				       (abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.00469) + \
				       (abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.00651) + \
				       (abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.01299) + \
				       (abs(eta) <= 1.8) * (pt > 2000.0) * (0.00897) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.01014) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.01288) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.01392) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.01533) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.01508) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.01579) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.01106) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.01346) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.01315) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.01156) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.0082) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.00439) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.00744) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.00685) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.00755) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.00706) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.00428) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.01031) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.00976) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.01523) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.00374) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.00854) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.0) + \
                                       (abs(eta) > 2.4 && abs(eta) <= 4.0) * (pt > 15.0) * (0.584522*tanh(pt*0.0144387 + 0.397034)) + \
				       (abs(eta) > 4.0) * (0.000)
}

  # efficiency formula for b-jets
  add EfficiencyFormula {5}          { (pt <= 20.0) * (0.000) + \
				       (abs(eta) <= 1.8) * (pt > 20.0 && pt <= 30) * (0.2426) + \
				       (abs(eta) <= 1.8) * (pt > 30.0 && pt <= 40) * (0.327) + \
				       (abs(eta) <= 1.8) * (pt > 40.0 && pt <= 50) * (0.3559) + \
				       (abs(eta) <= 1.8) * (pt > 50.0 && pt <= 60) * (0.3704) + \
				       (abs(eta) <= 1.8) * (pt > 60.0 && pt <= 70) * (0.3824) + \
				       (abs(eta) <= 1.8) * (pt > 70.0 && pt <= 80) * (0.3844) + \
				       (abs(eta) <= 1.8) * (pt > 80.0 && pt <= 90) * (0.3848) + \
				       (abs(eta) <= 1.8) * (pt > 90.0 && pt <= 100) * (0.3862) + \
				       (abs(eta) <= 1.8) * (pt > 100.0 && pt <= 120) * (0.3778) + \
				       (abs(eta) <= 1.8) * (pt > 120.0 && pt <= 140) * (0.3622) + \
				       (abs(eta) <= 1.8) * (pt > 140.0 && pt <= 160) * (0.3299) + \
				       (abs(eta) <= 1.8) * (pt > 160.0 && pt <= 180) * (0.2889) + \
				       (abs(eta) <= 1.8) * (pt > 180.0 && pt <= 200) * (0.2815) + \
				       (abs(eta) <= 1.8) * (pt > 200.0 && pt <= 250) * (0.253) + \
				       (abs(eta) <= 1.8) * (pt > 250.0 && pt <= 300) * (0.221) + \
				       (abs(eta) <= 1.8) * (pt > 300.0 && pt <= 350) * (0.1963) + \
				       (abs(eta) <= 1.8) * (pt > 350.0 && pt <= 400) * (0.1739) + \
				       (abs(eta) <= 1.8) * (pt > 400.0 && pt <= 500) * (0.0992) + \
				       (abs(eta) <= 1.8) * (pt > 500.0 && pt <= 600) * (0.0788) + \
				       (abs(eta) <= 1.8) * (pt > 600.0 && pt <= 700) * (0.0581) + \
				       (abs(eta) <= 1.8) * (pt > 700.0 && pt <= 800) * (0.0534) + \
				       (abs(eta) <= 1.8) * (pt > 800.0 && pt <= 1000) * (0.0521) + \
				       (abs(eta) <= 1.8) * (pt > 1000.0 && pt <= 1400) * (0.0626) + \
				       (abs(eta) <= 1.8) * (pt > 1400.0 && pt <= 2000) * (0.0826) + \
				       (abs(eta) <= 1.8) * (pt > 2000.0) * (0.1022) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt <= 20.0) * (0.000) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 20.0 && pt <= 30) * (0.1562) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 30.0 && pt <= 40) * (0.2499) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 40.0 && pt <= 50) * (0.2956) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 50.0 && pt <= 60) * (0.3128) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 60.0 && pt <= 70) * (0.3147) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 70.0 && pt <= 80) * (0.3222) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 80.0 && pt <= 90) * (0.304) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 90.0 && pt <= 100) * (0.3051) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 100.0 && pt <= 120) * (0.2657) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 120.0 && pt <= 140) * (0.2578) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 140.0 && pt <= 160) * (0.2087) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 160.0 && pt <= 180) * (0.1634) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 180.0 && pt <= 200) * (0.1651) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 200.0 && pt <= 250) * (0.1353) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 250.0 && pt <= 300) * (0.109) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 300.0 && pt <= 350) * (0.0799) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 350.0 && pt <= 400) * (0.0699) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 400.0 && pt <= 500) * (0.054) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 500.0 && pt <= 600) * (0.0718) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 600.0 && pt <= 700) * (0.0746) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 700.0 && pt <= 800) * (0.0803) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 800.0 && pt <= 1000) * (0.0882) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1000.0 && pt <= 1400) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 1400.0 && pt <= 2000) * (0.0) + \
				       (abs(eta) > 1.8 && abs(eta) <= 2.4) * (pt > 2000.0) * (0.0) + \
                                       (abs(eta) > 2.4 && abs(eta) <= 4.0) * (pt > 15.0) * (0.584522*tanh(pt*0.0144387 + 0.397034)) + \
                                       (abs(eta) > 4.0) * (0.000)
  }

}
module TauTagging TauTagging {
  set ParticleInputArray Delphes/allParticles
  set PartonInputArray Delphes/partons
  set JetInputArray JetEnergyScale/jets

  set DeltaR 0.5

  set TauPTMin 1.0

  set TauEtaMax 2.5

  # add EfficiencyFormula {abs(PDG code)} {efficiency formula as a function of eta and pt}

  # default efficiency formula (misidentification rate)
  add EfficiencyFormula {0} {0.001}
  # efficiency formula for tau-jets
  add EfficiencyFormula {15} {0.4}
}

#####################################################
# Find uniquely identified photons/electrons/tau/jets
#####################################################

module UniqueObjectFinder UniqueObjectFinder {
# earlier arrays take precedence over later ones
# add InputArray InputArray OutputArray
  add InputArray PhotonIsolation/photons photons
  add InputArray ElectronIsolation/electrons electrons
  add InputArray MuonIsolation/muons muons
  add InputArray JetEnergyScale/jets jets
}

##################
# ROOT tree writer
##################

# tracks, towers and eflow objects are not stored by default in the output.
# if needed (for jet constituent or other studies), uncomment the relevant
# "add Branch ..." lines.

module TreeWriter TreeWriter {
# add Branch InputArray BranchName BranchClass
  add Branch Delphes/allParticles Particle GenParticle

  add Branch TrackMerger/tracks Track Track
  add Branch Calorimeter/towers Tower Tower

  add Branch Calorimeter/eflowTracks EFlowTrack Track
  add Branch Calorimeter/eflowPhotons EFlowPhoton Tower
  add Branch Calorimeter/eflowNeutralHadrons EFlowNeutralHadron Tower

  add Branch GenJetFinder/jets GenJet Jet
  add Branch GenJetNoNuFinder/jets GenJetNoNu Jet

  add Branch ElectronEfficiency/electrons ElectronBeforeIso Electron
  add Branch PhotonEfficiency/photons PhotonBeforeIso Photon
  add Branch MuonEfficiency/muons MuonBeforeIso Muon

  add Branch UniqueObjectFinder/jets Jet Jet
  add Branch UniqueObjectFinder/electrons Electron Electron
  add Branch UniqueObjectFinder/photons Photon Photon
  add Branch UniqueObjectFinder/muons Muon Muon

  add Branch MissingET/momentum MissingET MissingET
  add Branch GenMissingET/momentum GenMissingET MissingET

  add Branch ScalarHT/energy ScalarHT ScalarHT
#  add Branch Rho/rho Rho Rho
  #add Branch PileUpMerger/vertices Vertex Vertex
}


// Class: ReadMLP
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : MLP::MLP
TMVA Release   : 4.1.2         [262402]
ROOT Release   : 5.32/04       [335876]
Creator        : lpernie
Date           : Fri Jul 15 14:32:57 2016
Host           : Linux login01.brazos.tamu.edu 2.6.32-431.23.3.el6.x86_64 #1 SMP Thu Jul 31 17:20:51 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /home/lpernie/Hhh/delphes/diHiggs/MVAs
Training events: 192991
Analysis type  : [Classification]


#OPT -*-*-*-*-*-*-*-*-*-*-*-*- options -*-*-*-*-*-*-*-*-*-*-*-*-

# Set by User:
NCycles: "600" [Number of training cycles]
HiddenLayers: "N+5" [Specification of hidden layer architecture]
NeuronType: "tanh" [Neuron activation function type]
V: "False" [Verbose output (short form of "VerbosityLevel" below - overrides the latter one)]
VarTransform: "N" [List of variable transformations performed before training, e.g., "D_Background,P_Signal,G,N_AllClasses" for: "Decorrelation, PCA-transformation, Gaussianisation, Normalisation, each for the given class of events ('AllClasses' denotes all events of all classes, if no class indication is given, 'All' is assumed)"]
H: "True" [Print method-specific help message]
TestRate: "5" [Test for overtraining performed at each #th epochs]
UseRegulator: "False" [Use regulator to avoid over-training]
# Default:
RandomSeed: "1" [Random seed for initial synapse weights (0 means unique seed for each run; default value '1')]
EstimatorType: "MSE" [MSE (Mean Square Estimator) for Gaussian Likelihood or CE(Cross-Entropy) for Bernoulli Likelihood]
NeuronInputType: "sum" [Neuron input function type]
VerbosityLevel: "Default" [Verbosity level]
CreateMVAPdfs: "False" [Create PDFs for classifier outputs (signal and background)]
IgnoreNegWeightsInTraining: "False" [Events with negative weights are ignored in the training (but are included for testing and performance evaluation)]
TrainingMethod: "BP" [Train with Back-Propagation (BP), BFGS Algorithm (BFGS), or Genetic Algorithm (GA - slower and worse)]
LearningRate: "2.000000e-02" [ANN learning rate parameter]
DecayRate: "1.000000e-02" [Decay rate for learning parameter]
EpochMonitoring: "False" [Provide epoch-wise monitoring plots according to TestRate (caution: causes big ROOT output file!)]
Sampling: "1.000000e+00" [Only 'Sampling' (randomly selected) events are trained each epoch]
SamplingEpoch: "1.000000e+00" [Sampling is used for the first 'SamplingEpoch' epochs, afterwards, all events are taken for training]
SamplingImportance: "1.000000e+00" [ The sampling weights of events in epochs which successful (worse estimator than before) are multiplied with SamplingImportance, else they are divided.]
SamplingTraining: "True" [The training sample is sampled]
SamplingTesting: "False" [The testing sample is sampled]
ResetStep: "50" [How often BFGS should reset history]
Tau: "3.000000e+00" [LineSearch "size step"]
BPMode: "sequential" [Back-propagation learning mode: sequential or batch]
BatchSize: "-1" [Batch size: number of events/batch, only set if in Batch Mode, -1 for BatchSize=number_of_events]
ConvergenceImprove: "1.000000e-30" [Minimum improvement which counts as improvement (<0 means automatic convergence check is turned off)]
ConvergenceTests: "-1" [Number of steps (without improvement) required for convergence (<0 means automatic convergence check is turned off)]
UpdateLimit: "10000" [Maximum times of regulator update]
CalculateErrors: "False" [Calculates inverse Hessian matrix at the end of the training to be able to calculate the uncertainties of an MVA value]
WeightRange: "1.000000e+00" [Take the events for the estimator calculations from small deviations from the desired value to large deviations only over the weight range]
##


#VAR -*-*-*-*-*-*-*-*-*-*-*-* variables *-*-*-*-*-*-*-*-*-*-*-*-

NVar 11
dR_l1l2                       dR_l1l2                       dR_l1l2                       dR_l1l2                                                         'F'    [0.0704614147544,3.29995322227]
dR_b1b2                       dR_b1b2                       dR_b1b2                       dR_b1b2                                                         'F'    [0.383513450623,4.99822092056]
dR_bl                         dR_bl                         dR_bl                         dR_bl                                                           'F'    [0.400089144707,4.99972915649]
dR_l1l2b1b2                   dR_l1l2b1b2                   dR_l1l2b1b2                   dR_l1l2b1b2                                                     'F'    [0.00575727969408,5.97906684875]
MINdR_bl                      MINdR_bl                      MINdR_bl                      MINdR_bl                                                        'F'    [0.400032073259,3.19927811623]
dphi_l1l2b1b2                 dphi_l1l2b1b2                 dphi_l1l2b1b2                 dphi_l1l2b1b2                                                   'F'    [1.38563655128e-05,3.14159226418]
mass_l1l2                     mass_l1l2                     mass_l1l2                     mass_l1l2                                                       'F'    [5.00042486191,99.9998016357]
mass_b1b2                     mass_b1b2                     mass_b1b2                     mass_b1b2                                                       'F'    [22.2296066284,699.396728516]
mass_trans                    mass_trans                    mass_trans                    mass_trans                                                      'F'    [0,249.994110107]
MT2                           MT2                           MT2                           MT2                                                             'F'    [24.6344547272,399.925476074]
pt_b1b2                       pt_b1b2                       pt_b1b2                       pt_b1b2                                                         'F'    [0.32002851367,299.99798584]
NSpec 0


============================================================================ */

#include <vector>
#include <cmath>
#include <string>
#include <iostream>

#ifndef IClassifierReader__def
#define IClassifierReader__def

class IClassifierReader {

 public:

   // constructor
   IClassifierReader() : fStatusIsClean( true ) {}
   virtual ~IClassifierReader() {}

   // return classifier response
   virtual double GetMvaValue( const std::vector<double>& inputValues ) const = 0;

   // returns classifier status
   bool IsStatusClean() const { return fStatusIsClean; }

 protected:

   bool fStatusIsClean;
};

#endif

class ReadMLP : public IClassifierReader {

 public:

   // constructor
   ReadMLP( std::vector<std::string>& theInputVars ) 
      : IClassifierReader(),
        fClassName( "ReadMLP" ),
        fNvars( 11 ),
        fIsNormalised( false )
   {      
      // the training input variables
      const char* inputVars[] = { "dR_l1l2", "dR_b1b2", "dR_bl", "dR_l1l2b1b2", "MINdR_bl", "dphi_l1l2b1b2", "mass_l1l2", "mass_b1b2", "mass_trans", "MT2", "pt_b1b2" };

      // sanity checks
      if (theInputVars.size() <= 0) {
         std::cout << "Problem in class \"" << fClassName << "\": empty input vector" << std::endl;
         fStatusIsClean = false;
      }

      if (theInputVars.size() != fNvars) {
         std::cout << "Problem in class \"" << fClassName << "\": mismatch in number of input values: "
                   << theInputVars.size() << " != " << fNvars << std::endl;
         fStatusIsClean = false;
      }

      // validate input variables
      for (size_t ivar = 0; ivar < theInputVars.size(); ivar++) {
         if (theInputVars[ivar] != inputVars[ivar]) {
            std::cout << "Problem in class \"" << fClassName << "\": mismatch in input variable names" << std::endl
                      << " for variable [" << ivar << "]: " << theInputVars[ivar].c_str() << " != " << inputVars[ivar] << std::endl;
            fStatusIsClean = false;
         }
      }

      // initialize min and max vectors (for normalisation)
      fVmin[0] = -1;
      fVmax[0] = 1;
      fVmin[1] = -1;
      fVmax[1] = 0.99999988079071;
      fVmin[2] = -1;
      fVmax[2] = 1;
      fVmin[3] = -1;
      fVmax[3] = 1;
      fVmin[4] = -1;
      fVmax[4] = 1;
      fVmin[5] = -1;
      fVmax[5] = 1;
      fVmin[6] = -1;
      fVmax[6] = 1;
      fVmin[7] = -1;
      fVmax[7] = 1;
      fVmin[8] = -1;
      fVmax[8] = 1;
      fVmin[9] = -1;
      fVmax[9] = 1;
      fVmin[10] = -1;
      fVmax[10] = 1;

      // initialize input variable types
      fType[0] = 'F';
      fType[1] = 'F';
      fType[2] = 'F';
      fType[3] = 'F';
      fType[4] = 'F';
      fType[5] = 'F';
      fType[6] = 'F';
      fType[7] = 'F';
      fType[8] = 'F';
      fType[9] = 'F';
      fType[10] = 'F';

      // initialize constants
      Initialize();

      // initialize transformation
      InitTransform();
   }

   // destructor
   virtual ~ReadMLP() {
      Clear(); // method-specific
   }

   // the classifier response
   // "inputValues" is a vector of input values in the same order as the 
   // variables given to the constructor
   double GetMvaValue( const std::vector<double>& inputValues ) const;

 private:

   // method-specific destructor
   void Clear();

   // input variable transformation

   double fMin_1[3][11];
   double fMax_1[3][11];
   void InitTransform_1();
   void Transform_1( std::vector<double> & iv, int sigOrBgd ) const;
   void InitTransform();
   void Transform( std::vector<double> & iv, int sigOrBgd ) const;

   // common member variables
   const char* fClassName;

   const size_t fNvars;
   size_t GetNvar()           const { return fNvars; }
   char   GetType( int ivar ) const { return fType[ivar]; }

   // normalisation of input variables
   const bool fIsNormalised;
   bool IsNormalised() const { return fIsNormalised; }
   double fVmin[11];
   double fVmax[11];
   double NormVariable( double x, double xmin, double xmax ) const {
      // normalise to output range: [-1, 1]
      return 2*(x - xmin)/(xmax - xmin) - 1.0;
   }

   // type of input variable: 'F' or 'I'
   char   fType[11];

   // initialize internal variables
   void Initialize();
   double GetMvaValue__( const std::vector<double>& inputValues ) const;

   // private members (method specific)

   double ActivationFnc(double x) const;
   double OutputActivationFnc(double x) const;

   int fLayers;
   int fLayerSize[3];
   double fWeightMatrix0to1[17][12];   // weight matrix from layer 0 to 1
   double fWeightMatrix1to2[1][17];   // weight matrix from layer 1 to 2

   double * fWeights[3];
};

inline void ReadMLP::Initialize()
{
   // build network structure
   fLayers = 3;
   fLayerSize[0] = 12; fWeights[0] = new double[12]; 
   fLayerSize[1] = 17; fWeights[1] = new double[17]; 
   fLayerSize[2] = 1; fWeights[2] = new double[1]; 
   // weight matrix from layer 0 to 1
   fWeightMatrix0to1[0][0] = -0.397508344591133;
   fWeightMatrix0to1[1][0] = -0.151401502462644;
   fWeightMatrix0to1[2][0] = -0.451909937580203;
   fWeightMatrix0to1[3][0] = 0.989712750957647;
   fWeightMatrix0to1[4][0] = -1.48227627456448;
   fWeightMatrix0to1[5][0] = -0.701460499926416;
   fWeightMatrix0to1[6][0] = 1.06412271684248;
   fWeightMatrix0to1[7][0] = 1.07637187879464;
   fWeightMatrix0to1[8][0] = -0.708272348802396;
   fWeightMatrix0to1[9][0] = -1.27687041656989;
   fWeightMatrix0to1[10][0] = -1.02931599227241;
   fWeightMatrix0to1[11][0] = -0.274096271295526;
   fWeightMatrix0to1[12][0] = -0.587144467724766;
   fWeightMatrix0to1[13][0] = -0.54300284136487;
   fWeightMatrix0to1[14][0] = -0.0907547271328023;
   fWeightMatrix0to1[15][0] = 0.449455535574358;
   fWeightMatrix0to1[0][1] = -0.561996484433813;
   fWeightMatrix0to1[1][1] = 1.32877715181391;
   fWeightMatrix0to1[2][1] = 0.66378508814454;
   fWeightMatrix0to1[3][1] = 1.39223349256277;
   fWeightMatrix0to1[4][1] = -0.406425421110333;
   fWeightMatrix0to1[5][1] = -0.859630474244961;
   fWeightMatrix0to1[6][1] = -0.266654321224947;
   fWeightMatrix0to1[7][1] = -0.0141590439110156;
   fWeightMatrix0to1[8][1] = -0.820854663430721;
   fWeightMatrix0to1[9][1] = -0.296884761942795;
   fWeightMatrix0to1[10][1] = 1.05897464474622;
   fWeightMatrix0to1[11][1] = -0.96131003225117;
   fWeightMatrix0to1[12][1] = -1.51631735880925;
   fWeightMatrix0to1[13][1] = 0.166296138424684;
   fWeightMatrix0to1[14][1] = 0.248197072655306;
   fWeightMatrix0to1[15][1] = 1.17291828302299;
   fWeightMatrix0to1[0][2] = -0.347568722519366;
   fWeightMatrix0to1[1][2] = 0.519219158780016;
   fWeightMatrix0to1[2][2] = 0.623275047268296;
   fWeightMatrix0to1[3][2] = 0.281758123942338;
   fWeightMatrix0to1[4][2] = -0.882084905666379;
   fWeightMatrix0to1[5][2] = 1.54338542344508;
   fWeightMatrix0to1[6][2] = -0.263240959479498;
   fWeightMatrix0to1[7][2] = 0.781654361135623;
   fWeightMatrix0to1[8][2] = 1.41782438756695;
   fWeightMatrix0to1[9][2] = 0.843469553676731;
   fWeightMatrix0to1[10][2] = 1.71050026298776;
   fWeightMatrix0to1[11][2] = 0.563443921825071;
   fWeightMatrix0to1[12][2] = 0.146941586397463;
   fWeightMatrix0to1[13][2] = -1.34837096461178;
   fWeightMatrix0to1[14][2] = -0.108445925151907;
   fWeightMatrix0to1[15][2] = -0.722957910328674;
   fWeightMatrix0to1[0][3] = 1.24110036992361;
   fWeightMatrix0to1[1][3] = 0.807770917366473;
   fWeightMatrix0to1[2][3] = 0.883051429792612;
   fWeightMatrix0to1[3][3] = 1.78943974394165;
   fWeightMatrix0to1[4][3] = -1.18225079869816;
   fWeightMatrix0to1[5][3] = 1.25768387190657;
   fWeightMatrix0to1[6][3] = -0.553690002764823;
   fWeightMatrix0to1[7][3] = -1.06004535439116;
   fWeightMatrix0to1[8][3] = -0.896246383723957;
   fWeightMatrix0to1[9][3] = -1.75790608927292;
   fWeightMatrix0to1[10][3] = 1.53849354690932;
   fWeightMatrix0to1[11][3] = 0.798459682020931;
   fWeightMatrix0to1[12][3] = -1.12617771412183;
   fWeightMatrix0to1[13][3] = 0.375664100502769;
   fWeightMatrix0to1[14][3] = -0.351922280606653;
   fWeightMatrix0to1[15][3] = -0.0080215144088839;
   fWeightMatrix0to1[0][4] = 1.7872627736432;
   fWeightMatrix0to1[1][4] = -0.151280924610941;
   fWeightMatrix0to1[2][4] = -0.128755135928129;
   fWeightMatrix0to1[3][4] = -0.711782674132639;
   fWeightMatrix0to1[4][4] = 1.08446287181415;
   fWeightMatrix0to1[5][4] = 0.287887495949642;
   fWeightMatrix0to1[6][4] = -0.286054617406424;
   fWeightMatrix0to1[7][4] = -1.70225728543983;
   fWeightMatrix0to1[8][4] = 0.943173375009179;
   fWeightMatrix0to1[9][4] = 0.641728311193985;
   fWeightMatrix0to1[10][4] = 1.10385470770862;
   fWeightMatrix0to1[11][4] = -0.00814607573411434;
   fWeightMatrix0to1[12][4] = -0.634429281910398;
   fWeightMatrix0to1[13][4] = -1.70974730271235;
   fWeightMatrix0to1[14][4] = 1.02525236024819;
   fWeightMatrix0to1[15][4] = -0.420986012533302;
   fWeightMatrix0to1[0][5] = -0.479844791138547;
   fWeightMatrix0to1[1][5] = -0.170308353181956;
   fWeightMatrix0to1[2][5] = -0.415169196226132;
   fWeightMatrix0to1[3][5] = -0.365966041598871;
   fWeightMatrix0to1[4][5] = -0.307447735315539;
   fWeightMatrix0to1[5][5] = -0.73183061924444;
   fWeightMatrix0to1[6][5] = -0.202478749636769;
   fWeightMatrix0to1[7][5] = 0.264271678184281;
   fWeightMatrix0to1[8][5] = -1.58280913420835;
   fWeightMatrix0to1[9][5] = -1.18178327632678;
   fWeightMatrix0to1[10][5] = -0.749667924769878;
   fWeightMatrix0to1[11][5] = -0.625954732875965;
   fWeightMatrix0to1[12][5] = 1.08872868858389;
   fWeightMatrix0to1[13][5] = -0.411135095640281;
   fWeightMatrix0to1[14][5] = -0.121282127095104;
   fWeightMatrix0to1[15][5] = 0.853767616575666;
   fWeightMatrix0to1[0][6] = -0.89790296094646;
   fWeightMatrix0to1[1][6] = -0.00833346678793898;
   fWeightMatrix0to1[2][6] = -0.887826709263223;
   fWeightMatrix0to1[3][6] = -0.145201970783371;
   fWeightMatrix0to1[4][6] = -1.26721528008627;
   fWeightMatrix0to1[5][6] = -1.56369869491309;
   fWeightMatrix0to1[6][6] = 0.572879649990985;
   fWeightMatrix0to1[7][6] = 0.907087658933093;
   fWeightMatrix0to1[8][6] = -0.61334004218859;
   fWeightMatrix0to1[9][6] = 1.27920976774112;
   fWeightMatrix0to1[10][6] = -0.374992353148806;
   fWeightMatrix0to1[11][6] = -0.374524586400998;
   fWeightMatrix0to1[12][6] = -0.138229228906575;
   fWeightMatrix0to1[13][6] = 0.308444293624313;
   fWeightMatrix0to1[14][6] = -0.713693616593411;
   fWeightMatrix0to1[15][6] = 0.165719261897202;
   fWeightMatrix0to1[0][7] = 1.15855239976613;
   fWeightMatrix0to1[1][7] = -1.24184061529813;
   fWeightMatrix0to1[2][7] = -2.16827927379339;
   fWeightMatrix0to1[3][7] = 1.96146679172684;
   fWeightMatrix0to1[4][7] = 0.519072095108865;
   fWeightMatrix0to1[5][7] = 0.223189543089627;
   fWeightMatrix0to1[6][7] = 4.92225301864948;
   fWeightMatrix0to1[7][7] = -2.61459297926206;
   fWeightMatrix0to1[8][7] = -0.423450571520333;
   fWeightMatrix0to1[9][7] = -0.269852887683505;
   fWeightMatrix0to1[10][7] = -0.0416506323259253;
   fWeightMatrix0to1[11][7] = 2.05240183191261;
   fWeightMatrix0to1[12][7] = 1.6748519984722;
   fWeightMatrix0to1[13][7] = -1.07143359651826;
   fWeightMatrix0to1[14][7] = 0.532364503254294;
   fWeightMatrix0to1[15][7] = -0.196507166436729;
   fWeightMatrix0to1[0][8] = -0.591877171132916;
   fWeightMatrix0to1[1][8] = -0.239879335420479;
   fWeightMatrix0to1[2][8] = -0.938534820657402;
   fWeightMatrix0to1[3][8] = -1.32856476960303;
   fWeightMatrix0to1[4][8] = 0.617720429395118;
   fWeightMatrix0to1[5][8] = -0.0649205456631453;
   fWeightMatrix0to1[6][8] = 0.475434751481799;
   fWeightMatrix0to1[7][8] = 0.161520451771508;
   fWeightMatrix0to1[8][8] = 1.26692254534728;
   fWeightMatrix0to1[9][8] = -1.36327378068243;
   fWeightMatrix0to1[10][8] = 0.284704118726994;
   fWeightMatrix0to1[11][8] = 1.04273073994362;
   fWeightMatrix0to1[12][8] = 1.05615397921249;
   fWeightMatrix0to1[13][8] = -0.00561990266829902;
   fWeightMatrix0to1[14][8] = -0.439714292701432;
   fWeightMatrix0to1[15][8] = -0.244394552850334;
   fWeightMatrix0to1[0][9] = -1.03980294574651;
   fWeightMatrix0to1[1][9] = 1.96357843202852;
   fWeightMatrix0to1[2][9] = 3.49857097281335;
   fWeightMatrix0to1[3][9] = 0.159767043828459;
   fWeightMatrix0to1[4][9] = -0.107108136764088;
   fWeightMatrix0to1[5][9] = 0.511174111582831;
   fWeightMatrix0to1[6][9] = -3.32173868387448;
   fWeightMatrix0to1[7][9] = -0.536639741463286;
   fWeightMatrix0to1[8][9] = 2.21813368324264;
   fWeightMatrix0to1[9][9] = -0.796193126424145;
   fWeightMatrix0to1[10][9] = -0.370033100044464;
   fWeightMatrix0to1[11][9] = 0.5506818344749;
   fWeightMatrix0to1[12][9] = 1.44087935155621;
   fWeightMatrix0to1[13][9] = -0.333473398415931;
   fWeightMatrix0to1[14][9] = 1.57855438179931;
   fWeightMatrix0to1[15][9] = -2.08756495881348;
   fWeightMatrix0to1[0][10] = 1.26023416928897;
   fWeightMatrix0to1[1][10] = 0.879098701467303;
   fWeightMatrix0to1[2][10] = 0.875188147541031;
   fWeightMatrix0to1[3][10] = 0.79640904805171;
   fWeightMatrix0to1[4][10] = 1.24205890430208;
   fWeightMatrix0to1[5][10] = 0.467799582644362;
   fWeightMatrix0to1[6][10] = -1.22061533393127;
   fWeightMatrix0to1[7][10] = -0.0698472653307661;
   fWeightMatrix0to1[8][10] = -0.106338336242784;
   fWeightMatrix0to1[9][10] = -0.826800272625414;
   fWeightMatrix0to1[10][10] = 2.0903671735493;
   fWeightMatrix0to1[11][10] = 0.893237914943161;
   fWeightMatrix0to1[12][10] = 0.148972664623988;
   fWeightMatrix0to1[13][10] = 0.35879124729731;
   fWeightMatrix0to1[14][10] = 1.19754872836561;
   fWeightMatrix0to1[15][10] = -1.41636573958578;
   fWeightMatrix0to1[0][11] = -2.02483620265233;
   fWeightMatrix0to1[1][11] = -2.02394357615742;
   fWeightMatrix0to1[2][11] = -1.84371730825935;
   fWeightMatrix0to1[3][11] = -2.11196985093305;
   fWeightMatrix0to1[4][11] = -2.08199311719304;
   fWeightMatrix0to1[5][11] = -1.90187574023684;
   fWeightMatrix0to1[6][11] = 5.3235949411534;
   fWeightMatrix0to1[7][11] = 1.39463574035998;
   fWeightMatrix0to1[8][11] = -1.80275507150392;
   fWeightMatrix0to1[9][11] = 0.27583278863972;
   fWeightMatrix0to1[10][11] = -0.900783837270008;
   fWeightMatrix0to1[11][11] = 0.287850462748661;
   fWeightMatrix0to1[12][11] = -1.8943062010332;
   fWeightMatrix0to1[13][11] = 1.41193340228278;
   fWeightMatrix0to1[14][11] = -2.5177212046211;
   fWeightMatrix0to1[15][11] = 1.85145825405186;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = -0.121692740496404;
   fWeightMatrix1to2[0][1] = -0.433564477345936;
   fWeightMatrix1to2[0][2] = 0.22990957171426;
   fWeightMatrix1to2[0][3] = 0.0142477856785191;
   fWeightMatrix1to2[0][4] = -0.00507685424157258;
   fWeightMatrix1to2[0][5] = -0.115618892258463;
   fWeightMatrix1to2[0][6] = -0.411238099789671;
   fWeightMatrix1to2[0][7] = -0.0355688000445772;
   fWeightMatrix1to2[0][8] = 0.012537653396264;
   fWeightMatrix1to2[0][9] = -0.00100577515535999;
   fWeightMatrix1to2[0][10] = 0.0538960145683007;
   fWeightMatrix1to2[0][11] = -0.0147284821826315;
   fWeightMatrix1to2[0][12] = 0.0402128712661292;
   fWeightMatrix1to2[0][13] = 0.0246941510362194;
   fWeightMatrix1to2[0][14] = 0.427474792648826;
   fWeightMatrix1to2[0][15] = 0.110770979159767;
   fWeightMatrix1to2[0][16] = 0.401704417302672;
}

inline double ReadMLP::GetMvaValue__( const std::vector<double>& inputValues ) const
{
   if (inputValues.size() != (unsigned int)fLayerSize[0]-1) {
      std::cout << "Input vector needs to be of size " << fLayerSize[0]-1 << std::endl;
      return 0;
   }

   for (int l=0; l<fLayers; l++)
      for (int i=0; i<fLayerSize[l]; i++) fWeights[l][i]=0;

   for (int l=0; l<fLayers-1; l++)
      fWeights[l][fLayerSize[l]-1]=1;

   for (int i=0; i<fLayerSize[0]-1; i++)
      fWeights[0][i]=inputValues[i];

   // layer 0 to 1
   for (int o=0; o<fLayerSize[1]-1; o++) {
      for (int i=0; i<fLayerSize[0]; i++) {
         double inputVal = fWeightMatrix0to1[o][i] * fWeights[0][i];
         fWeights[1][o] += inputVal;
      }
      fWeights[1][o] = ActivationFnc(fWeights[1][o]);
   }
   // layer 1 to 2
   for (int o=0; o<fLayerSize[2]; o++) {
      for (int i=0; i<fLayerSize[1]; i++) {
         double inputVal = fWeightMatrix1to2[o][i] * fWeights[1][i];
         fWeights[2][o] += inputVal;
      }
      fWeights[2][o] = OutputActivationFnc(fWeights[2][o]);
   }

   return fWeights[2][0];
}

double ReadMLP::ActivationFnc(double x) const {
   // hyperbolic tan
   return tanh(x);
}
double ReadMLP::OutputActivationFnc(double x) const {
   // identity
   return x;
}
   
// Clean up
inline void ReadMLP::Clear() 
{
   // nothing to clear
}
   inline double ReadMLP::GetMvaValue( const std::vector<double>& inputValues ) const
   {
      // classifier response value
      double retval = 0;

      // classifier response, sanity check first
      if (!IsStatusClean()) {
         std::cout << "Problem in class \"" << fClassName << "\": cannot return classifier response"
                   << " because status is dirty" << std::endl;
         retval = 0;
      }
      else {
         if (IsNormalised()) {
            // normalise variables
            std::vector<double> iV;
            int ivar = 0;
            for (std::vector<double>::const_iterator varIt = inputValues.begin();
                 varIt != inputValues.end(); varIt++, ivar++) {
               iV.push_back(NormVariable( *varIt, fVmin[ivar], fVmax[ivar] ));
            }
            Transform( iV, -1 );
            retval = GetMvaValue__( iV );
         }
         else {
            std::vector<double> iV;
            int ivar = 0;
            for (std::vector<double>::const_iterator varIt = inputValues.begin();
                 varIt != inputValues.end(); varIt++, ivar++) {
               iV.push_back(*varIt);
            }
            Transform( iV, -1 );
            retval = GetMvaValue__( iV );
         }
      }

      return retval;
   }

//_______________________________________________________________________
inline void ReadMLP::InitTransform_1()
{
   // Normalization transformation, initialisation
   fMin_1[0][0] = 0.0704614147544;
   fMax_1[0][0] = 3.18576622009;
   fMin_1[1][0] = 0.0706459358335;
   fMax_1[1][0] = 3.29995322227;
   fMin_1[2][0] = 0.0704614147544;
   fMax_1[2][0] = 3.29995322227;
   fMin_1[0][1] = 0.402354240417;
   fMax_1[0][1] = 4.99822092056;
   fMin_1[1][1] = 0.383513450623;
   fMax_1[1][1] = 4.99817085266;
   fMin_1[2][1] = 0.383513450623;
   fMax_1[2][1] = 4.99822092056;
   fMin_1[0][2] = 0.531068503857;
   fMax_1[0][2] = 4.98002052307;
   fMin_1[1][2] = 0.400089144707;
   fMax_1[1][2] = 4.99972915649;
   fMin_1[2][2] = 0.400089144707;
   fMax_1[2][2] = 4.99972915649;
   fMin_1[0][3] = 0.491473555565;
   fMax_1[0][3] = 5.70285654068;
   fMin_1[1][3] = 0.00575727969408;
   fMax_1[1][3] = 5.97906684875;
   fMin_1[2][3] = 0.00575727969408;
   fMax_1[2][3] = 5.97906684875;
   fMin_1[0][4] = 0.430928468704;
   fMax_1[0][4] = 3.19814586639;
   fMin_1[1][4] = 0.400032073259;
   fMax_1[1][4] = 3.19927811623;
   fMin_1[2][4] = 0.400032073259;
   fMax_1[2][4] = 3.19927811623;
   fMin_1[0][5] = 0.000368591747247;
   fMax_1[0][5] = 3.1415438652;
   fMin_1[1][5] = 1.38563655128e-05;
   fMax_1[1][5] = 3.14159226418;
   fMin_1[2][5] = 1.38563655128e-05;
   fMax_1[2][5] = 3.14159226418;
   fMin_1[0][6] = 5.07232713699;
   fMax_1[0][6] = 94.8096237183;
   fMin_1[1][6] = 5.00042486191;
   fMax_1[1][6] = 99.9998016357;
   fMin_1[2][6] = 5.00042486191;
   fMax_1[2][6] = 99.9998016357;
   fMin_1[0][7] = 24.7375221252;
   fMax_1[0][7] = 695.843139648;
   fMin_1[1][7] = 22.2296066284;
   fMax_1[1][7] = 699.396728516;
   fMin_1[2][7] = 22.2296066284;
   fMax_1[2][7] = 699.396728516;
   fMin_1[0][8] = 0;
   fMax_1[0][8] = 249.219848633;
   fMin_1[1][8] = 0;
   fMax_1[1][8] = 249.994110107;
   fMin_1[2][8] = 0;
   fMax_1[2][8] = 249.994110107;
   fMin_1[0][9] = 64.9815139771;
   fMax_1[0][9] = 397.740020752;
   fMin_1[1][9] = 24.6344547272;
   fMax_1[1][9] = 399.925476074;
   fMin_1[2][9] = 24.6344547272;
   fMax_1[2][9] = 399.925476074;
   fMin_1[0][10] = 3.62327671051;
   fMax_1[0][10] = 299.99798584;
   fMin_1[1][10] = 0.32002851367;
   fMax_1[1][10] = 299.745605469;
   fMin_1[2][10] = 0.32002851367;
   fMax_1[2][10] = 299.99798584;
}

//_______________________________________________________________________
inline void ReadMLP::Transform_1( std::vector<double>& iv, int cls) const
{
   // Normalization transformation
   if (cls < 0 || cls > 2) {
   if (2 > 1 ) cls = 2;
      else cls = 2;
   }
   const int nVar = 11;

   // get indices of used variables

   // define the indices of the variables which are transformed by this transformation
   std::vector<int> indicesGet;
   std::vector<int> indicesPut;

   indicesGet.push_back( 0);
   indicesGet.push_back( 1);
   indicesGet.push_back( 2);
   indicesGet.push_back( 3);
   indicesGet.push_back( 4);
   indicesGet.push_back( 5);
   indicesGet.push_back( 6);
   indicesGet.push_back( 7);
   indicesGet.push_back( 8);
   indicesGet.push_back( 9);
   indicesGet.push_back( 10);
   indicesPut.push_back( 0);
   indicesPut.push_back( 1);
   indicesPut.push_back( 2);
   indicesPut.push_back( 3);
   indicesPut.push_back( 4);
   indicesPut.push_back( 5);
   indicesPut.push_back( 6);
   indicesPut.push_back( 7);
   indicesPut.push_back( 8);
   indicesPut.push_back( 9);
   indicesPut.push_back( 10);

   std::vector<double> dv(nVar);
   for (int ivar=0; ivar<nVar; ivar++) dv[ivar] = iv[indicesGet.at(ivar)];
   for (int ivar=0;ivar<11;ivar++) {
      double offset = fMin_1[cls][ivar];
      double scale  = 1.0/(fMax_1[cls][ivar]-fMin_1[cls][ivar]);
      iv[indicesPut.at(ivar)] = (dv[ivar]-offset)*scale * 2 - 1;
   }
}

//_______________________________________________________________________
inline void ReadMLP::InitTransform()
{
   InitTransform_1();
}

//_______________________________________________________________________
inline void ReadMLP::Transform( std::vector<double>& iv, int sigOrBgd ) const
{
   Transform_1( iv, sigOrBgd );
}

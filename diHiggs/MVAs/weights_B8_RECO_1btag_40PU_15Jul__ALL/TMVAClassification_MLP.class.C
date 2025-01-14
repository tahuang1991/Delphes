// Class: ReadMLP
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : MLP::MLP
TMVA Release   : 4.1.2         [262402]
ROOT Release   : 5.32/04       [335876]
Creator        : lpernie
Date           : Fri Jul 15 14:32:01 2016
Host           : Linux login01.brazos.tamu.edu 2.6.32-431.23.3.el6.x86_64 #1 SMP Thu Jul 31 17:20:51 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /home/lpernie/Hhh/delphes/diHiggs/MVAs
Training events: 193623
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
dR_l1l2                       dR_l1l2                       dR_l1l2                       dR_l1l2                                                         'F'    [0.0706944391131,3.29995322227]
dR_b1b2                       dR_b1b2                       dR_b1b2                       dR_b1b2                                                         'F'    [0.391943722963,4.99817085266]
dR_bl                         dR_bl                         dR_bl                         dR_bl                                                           'F'    [0.400014162064,4.99432611465]
dR_l1l2b1b2                   dR_l1l2b1b2                   dR_l1l2b1b2                   dR_l1l2b1b2                                                     'F'    [0.00669882446527,5.97856473923]
MINdR_bl                      MINdR_bl                      MINdR_bl                      MINdR_bl                                                        'F'    [0.400001376867,3.19927811623]
dphi_l1l2b1b2                 dphi_l1l2b1b2                 dphi_l1l2b1b2                 dphi_l1l2b1b2                                                   'F'    [1.97242206923e-05,3.14159226418]
mass_l1l2                     mass_l1l2                     mass_l1l2                     mass_l1l2                                                       'F'    [5.00751638412,99.9998550415]
mass_b1b2                     mass_b1b2                     mass_b1b2                     mass_b1b2                                                       'F'    [22.2296066284,698.690368652]
mass_trans                    mass_trans                    mass_trans                    mass_trans                                                      'F'    [0,249.967041016]
MT2                           MT2                           MT2                           MT2                                                             'F'    [22.3099918365,399.925476074]
pt_b1b2                       pt_b1b2                       pt_b1b2                       pt_b1b2                                                         'F'    [0.326862245798,299.927642822]
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
      fVmax[1] = 1;
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
      fVmax[9] = 0.99999988079071;
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
   fWeightMatrix0to1[0][0] = 0.0470473689268939;
   fWeightMatrix0to1[1][0] = -0.282283464903775;
   fWeightMatrix0to1[2][0] = -0.369656223129777;
   fWeightMatrix0to1[3][0] = 0.819203390922356;
   fWeightMatrix0to1[4][0] = -1.47823423435963;
   fWeightMatrix0to1[5][0] = -0.535045756636212;
   fWeightMatrix0to1[6][0] = 0.490238463497779;
   fWeightMatrix0to1[7][0] = 1.01678069718135;
   fWeightMatrix0to1[8][0] = -0.578627693301593;
   fWeightMatrix0to1[9][0] = -1.18127927954594;
   fWeightMatrix0to1[10][0] = -1.77976292403776;
   fWeightMatrix0to1[11][0] = -0.332915433606124;
   fWeightMatrix0to1[12][0] = -0.664048267551072;
   fWeightMatrix0to1[13][0] = -0.319190351397979;
   fWeightMatrix0to1[14][0] = -1.24149367552664;
   fWeightMatrix0to1[15][0] = 0.557967852033418;
   fWeightMatrix0to1[0][1] = -0.350823616870395;
   fWeightMatrix0to1[1][1] = 1.5729548411072;
   fWeightMatrix0to1[2][1] = 0.915706931448962;
   fWeightMatrix0to1[3][1] = 1.38237856706168;
   fWeightMatrix0to1[4][1] = -0.447966947407894;
   fWeightMatrix0to1[5][1] = -1.24873940870259;
   fWeightMatrix0to1[6][1] = -0.149058827704658;
   fWeightMatrix0to1[7][1] = -0.169553271143138;
   fWeightMatrix0to1[8][1] = -0.977430043560439;
   fWeightMatrix0to1[9][1] = -0.259196573711159;
   fWeightMatrix0to1[10][1] = 0.754595355883338;
   fWeightMatrix0to1[11][1] = -1.31436746066277;
   fWeightMatrix0to1[12][1] = -2.35491115944464;
   fWeightMatrix0to1[13][1] = 0.573306446670866;
   fWeightMatrix0to1[14][1] = -0.884222214631008;
   fWeightMatrix0to1[15][1] = 1.26934065977017;
   fWeightMatrix0to1[0][2] = -0.199000672380516;
   fWeightMatrix0to1[1][2] = 0.708455197257813;
   fWeightMatrix0to1[2][2] = 0.642700057665841;
   fWeightMatrix0to1[3][2] = 0.371528620856651;
   fWeightMatrix0to1[4][2] = -0.907010695551903;
   fWeightMatrix0to1[5][2] = 0.766709280487283;
   fWeightMatrix0to1[6][2] = -0.22468073936092;
   fWeightMatrix0to1[7][2] = 0.723218418768615;
   fWeightMatrix0to1[8][2] = 1.45098128885434;
   fWeightMatrix0to1[9][2] = 0.77051923802158;
   fWeightMatrix0to1[10][2] = 1.58471183457189;
   fWeightMatrix0to1[11][2] = 0.728932078499363;
   fWeightMatrix0to1[12][2] = 0.294786703725375;
   fWeightMatrix0to1[13][2] = -1.48720328075481;
   fWeightMatrix0to1[14][2] = 0.214954340236497;
   fWeightMatrix0to1[15][2] = -0.534735360453727;
   fWeightMatrix0to1[0][3] = 1.53020430463991;
   fWeightMatrix0to1[1][3] = 0.605952590300485;
   fWeightMatrix0to1[2][3] = 0.363474693553686;
   fWeightMatrix0to1[3][3] = 1.7655759311157;
   fWeightMatrix0to1[4][3] = -1.21509720308291;
   fWeightMatrix0to1[5][3] = 0.79783937181369;
   fWeightMatrix0to1[6][3] = 0.254820747108056;
   fWeightMatrix0to1[7][3] = -1.12889836408296;
   fWeightMatrix0to1[8][3] = -0.878419886624149;
   fWeightMatrix0to1[9][3] = -1.70697988976147;
   fWeightMatrix0to1[10][3] = 1.77170750749198;
   fWeightMatrix0to1[11][3] = 0.841728205375714;
   fWeightMatrix0to1[12][3] = -0.749941749112595;
   fWeightMatrix0to1[13][3] = 0.431036180876859;
   fWeightMatrix0to1[14][3] = -0.365375635737832;
   fWeightMatrix0to1[15][3] = 0.0195231436241136;
   fWeightMatrix0to1[0][4] = 1.63641416666111;
   fWeightMatrix0to1[1][4] = -0.087118819423505;
   fWeightMatrix0to1[2][4] = 0.0290132284575008;
   fWeightMatrix0to1[3][4] = -0.539434333846816;
   fWeightMatrix0to1[4][4] = 1.01253415047188;
   fWeightMatrix0to1[5][4] = 0.775844273990529;
   fWeightMatrix0to1[6][4] = -0.0712944366170224;
   fWeightMatrix0to1[7][4] = -1.69317201162564;
   fWeightMatrix0to1[8][4] = 0.950546128726049;
   fWeightMatrix0to1[9][4] = 0.517764506324729;
   fWeightMatrix0to1[10][4] = 1.05620943182955;
   fWeightMatrix0to1[11][4] = -0.0439027524075345;
   fWeightMatrix0to1[12][4] = 0.0887436386876401;
   fWeightMatrix0to1[13][4] = -1.57731585701717;
   fWeightMatrix0to1[14][4] = 0.605509244796078;
   fWeightMatrix0to1[15][4] = -0.546958808198309;
   fWeightMatrix0to1[0][5] = 0.211264083315998;
   fWeightMatrix0to1[1][5] = -0.178348455824554;
   fWeightMatrix0to1[2][5] = -0.179076156731685;
   fWeightMatrix0to1[3][5] = -0.306735113963076;
   fWeightMatrix0to1[4][5] = -0.339515147727613;
   fWeightMatrix0to1[5][5] = -0.249402547291734;
   fWeightMatrix0to1[6][5] = -0.736364081489782;
   fWeightMatrix0to1[7][5] = 0.0214420126756145;
   fWeightMatrix0to1[8][5] = -1.21081378124285;
   fWeightMatrix0to1[9][5] = -1.08423437319065;
   fWeightMatrix0to1[10][5] = -0.594412307202621;
   fWeightMatrix0to1[11][5] = -0.194614297465246;
   fWeightMatrix0to1[12][5] = 0.718327728905555;
   fWeightMatrix0to1[13][5] = -0.188290474783865;
   fWeightMatrix0to1[14][5] = 0.278081396834666;
   fWeightMatrix0to1[15][5] = 0.867981630758929;
   fWeightMatrix0to1[0][6] = -0.787376110368224;
   fWeightMatrix0to1[1][6] = 0.195908669043356;
   fWeightMatrix0to1[2][6] = -0.411315964752144;
   fWeightMatrix0to1[3][6] = -0.145701257250873;
   fWeightMatrix0to1[4][6] = -1.13448959448003;
   fWeightMatrix0to1[5][6] = -1.55134181204766;
   fWeightMatrix0to1[6][6] = 0.926012866378071;
   fWeightMatrix0to1[7][6] = 1.12159259090769;
   fWeightMatrix0to1[8][6] = -0.690307877309708;
   fWeightMatrix0to1[9][6] = 1.20454725385443;
   fWeightMatrix0to1[10][6] = 0.486244281201484;
   fWeightMatrix0to1[11][6] = -0.367166897370091;
   fWeightMatrix0to1[12][6] = -0.165595361587657;
   fWeightMatrix0to1[13][6] = 0.258979578983283;
   fWeightMatrix0to1[14][6] = -0.238520192523653;
   fWeightMatrix0to1[15][6] = 0.303660769115981;
   fWeightMatrix0to1[0][7] = 1.22882195158786;
   fWeightMatrix0to1[1][7] = -2.34826314942548;
   fWeightMatrix0to1[2][7] = -3.60769977752036;
   fWeightMatrix0to1[3][7] = 1.94992879395813;
   fWeightMatrix0to1[4][7] = 0.503546531289264;
   fWeightMatrix0to1[5][7] = 0.399177512547151;
   fWeightMatrix0to1[6][7] = 5.45848098237283;
   fWeightMatrix0to1[7][7] = -2.56332849634324;
   fWeightMatrix0to1[8][7] = -0.557969437873128;
   fWeightMatrix0to1[9][7] = -0.222279471595328;
   fWeightMatrix0to1[10][7] = -0.0256844148845814;
   fWeightMatrix0to1[11][7] = 1.88531734681236;
   fWeightMatrix0to1[12][7] = 0.443258398037896;
   fWeightMatrix0to1[13][7] = -0.998613920344912;
   fWeightMatrix0to1[14][7] = 0.748715570518612;
   fWeightMatrix0to1[15][7] = -0.135192350091865;
   fWeightMatrix0to1[0][8] = -0.515253610014691;
   fWeightMatrix0to1[1][8] = 0.00967302409242483;
   fWeightMatrix0to1[2][8] = -0.658954981798819;
   fWeightMatrix0to1[3][8] = -1.2640357314239;
   fWeightMatrix0to1[4][8] = 0.568877709276854;
   fWeightMatrix0to1[5][8] = -0.119477293408598;
   fWeightMatrix0to1[6][8] = 0.590006854999822;
   fWeightMatrix0to1[7][8] = 0.381340928702033;
   fWeightMatrix0to1[8][8] = 1.39858263383185;
   fWeightMatrix0to1[9][8] = -1.26119784396621;
   fWeightMatrix0to1[10][8] = 0.426817464926158;
   fWeightMatrix0to1[11][8] = 0.555856962913519;
   fWeightMatrix0to1[12][8] = -0.499284576535524;
   fWeightMatrix0to1[13][8] = -0.157902435327265;
   fWeightMatrix0to1[14][8] = 0.00947672735373065;
   fWeightMatrix0to1[15][8] = -0.38485317898598;
   fWeightMatrix0to1[0][9] = -0.698049731810114;
   fWeightMatrix0to1[1][9] = 2.67188540714817;
   fWeightMatrix0to1[2][9] = 4.35928207044758;
   fWeightMatrix0to1[3][9] = 0.455465478082195;
   fWeightMatrix0to1[4][9] = -0.0660280782418912;
   fWeightMatrix0to1[5][9] = 0.492703064417339;
   fWeightMatrix0to1[6][9] = -3.40799273697617;
   fWeightMatrix0to1[7][9] = -0.362776382522755;
   fWeightMatrix0to1[8][9] = 2.18014907074676;
   fWeightMatrix0to1[9][9] = -0.817387426947112;
   fWeightMatrix0to1[10][9] = 0.127716983025529;
   fWeightMatrix0to1[11][9] = 0.528424799358241;
   fWeightMatrix0to1[12][9] = 2.17105079807586;
   fWeightMatrix0to1[13][9] = -0.365031167944005;
   fWeightMatrix0to1[14][9] = 1.4401453335718;
   fWeightMatrix0to1[15][9] = -1.82446948764005;
   fWeightMatrix0to1[0][10] = 1.81110922592689;
   fWeightMatrix0to1[1][10] = 1.00214691189489;
   fWeightMatrix0to1[2][10] = 0.998087564895245;
   fWeightMatrix0to1[3][10] = 1.06409781036805;
   fWeightMatrix0to1[4][10] = 1.23045665636468;
   fWeightMatrix0to1[5][10] = 0.54482976566461;
   fWeightMatrix0to1[6][10] = -1.36760358245977;
   fWeightMatrix0to1[7][10] = -0.158498201328594;
   fWeightMatrix0to1[8][10] = 0.0846015358141744;
   fWeightMatrix0to1[9][10] = -0.858175626097483;
   fWeightMatrix0to1[10][10] = 1.45192094185629;
   fWeightMatrix0to1[11][10] = 0.942046963918155;
   fWeightMatrix0to1[12][10] = 0.398672269522267;
   fWeightMatrix0to1[13][10] = -0.352124857015456;
   fWeightMatrix0to1[14][10] = 0.228998241440226;
   fWeightMatrix0to1[15][10] = -1.50652365499328;
   fWeightMatrix0to1[0][11] = -1.4749135871789;
   fWeightMatrix0to1[1][11] = -2.23254841127417;
   fWeightMatrix0to1[2][11] = -2.3770237225;
   fWeightMatrix0to1[3][11] = -2.09206134821502;
   fWeightMatrix0to1[4][11] = -1.92661741775402;
   fWeightMatrix0to1[5][11] = -2.62486611342815;
   fWeightMatrix0to1[6][11] = 5.43739948897047;
   fWeightMatrix0to1[7][11] = 0.740868901597365;
   fWeightMatrix0to1[8][11] = -1.65559716601437;
   fWeightMatrix0to1[9][11] = 0.359599989407367;
   fWeightMatrix0to1[10][11] = -1.19240837453825;
   fWeightMatrix0to1[11][11] = -0.123905377117982;
   fWeightMatrix0to1[12][11] = -2.243444225415;
   fWeightMatrix0to1[13][11] = 1.71683912349252;
   fWeightMatrix0to1[14][11] = -2.67778940807782;
   fWeightMatrix0to1[15][11] = 1.81250566102623;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = 0.0427674605681762;
   fWeightMatrix1to2[0][1] = -0.510015278247689;
   fWeightMatrix1to2[0][2] = 0.348450109862089;
   fWeightMatrix1to2[0][3] = 0.0138803606918158;
   fWeightMatrix1to2[0][4] = 0.000119182023228004;
   fWeightMatrix1to2[0][5] = -0.341729070677327;
   fWeightMatrix1to2[0][6] = -0.441019684263775;
   fWeightMatrix1to2[0][7] = 0.00942978333258097;
   fWeightMatrix1to2[0][8] = -0.0322190821661402;
   fWeightMatrix1to2[0][9] = 0.000860588726446374;
   fWeightMatrix1to2[0][10] = 0.0744461742594709;
   fWeightMatrix1to2[0][11] = 0.0175552902096545;
   fWeightMatrix1to2[0][12] = -0.232461997486189;
   fWeightMatrix1to2[0][13] = 0.145113697320479;
   fWeightMatrix1to2[0][14] = 0.787017545790468;
   fWeightMatrix1to2[0][15] = 0.0381474747573433;
   fWeightMatrix1to2[0][16] = 0.4178963883304;
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
   fMin_1[0][0] = 0.0706944391131;
   fMax_1[0][0] = 3.25721549988;
   fMin_1[1][0] = 0.0718527138233;
   fMax_1[1][0] = 3.29995322227;
   fMin_1[2][0] = 0.0706944391131;
   fMax_1[2][0] = 3.29995322227;
   fMin_1[0][1] = 0.401573121548;
   fMax_1[0][1] = 4.66858625412;
   fMin_1[1][1] = 0.391943722963;
   fMax_1[1][1] = 4.99817085266;
   fMin_1[2][1] = 0.391943722963;
   fMax_1[2][1] = 4.99817085266;
   fMin_1[0][2] = 0.518788516521;
   fMax_1[0][2] = 4.55215978622;
   fMin_1[1][2] = 0.400014162064;
   fMax_1[1][2] = 4.99432611465;
   fMin_1[2][2] = 0.400014162064;
   fMax_1[2][2] = 4.99432611465;
   fMin_1[0][3] = 0.119358405471;
   fMax_1[0][3] = 5.61826848984;
   fMin_1[1][3] = 0.00669882446527;
   fMax_1[1][3] = 5.97856473923;
   fMin_1[2][3] = 0.00669882446527;
   fMax_1[2][3] = 5.97856473923;
   fMin_1[0][4] = 0.410843193531;
   fMax_1[0][4] = 3.19498634338;
   fMin_1[1][4] = 0.400001376867;
   fMax_1[1][4] = 3.19927811623;
   fMin_1[2][4] = 0.400001376867;
   fMax_1[2][4] = 3.19927811623;
   fMin_1[0][5] = 0.00949874892831;
   fMax_1[0][5] = 3.14159083366;
   fMin_1[1][5] = 1.97242206923e-05;
   fMax_1[1][5] = 3.14159226418;
   fMin_1[2][5] = 1.97242206923e-05;
   fMax_1[2][5] = 3.14159226418;
   fMin_1[0][6] = 5.05352163315;
   fMax_1[0][6] = 94.7827072144;
   fMin_1[1][6] = 5.00751638412;
   fMax_1[1][6] = 99.9998550415;
   fMin_1[2][6] = 5.00751638412;
   fMax_1[2][6] = 99.9998550415;
   fMin_1[0][7] = 24.0999298096;
   fMax_1[0][7] = 663.692810059;
   fMin_1[1][7] = 22.2296066284;
   fMax_1[1][7] = 698.690368652;
   fMin_1[2][7] = 22.2296066284;
   fMax_1[2][7] = 698.690368652;
   fMin_1[0][8] = 0.0457026585937;
   fMax_1[0][8] = 249.678604126;
   fMin_1[1][8] = 0;
   fMax_1[1][8] = 249.967041016;
   fMin_1[2][8] = 0;
   fMax_1[2][8] = 249.967041016;
   fMin_1[0][9] = 58.7978210449;
   fMax_1[0][9] = 399.101257324;
   fMin_1[1][9] = 22.3099918365;
   fMax_1[1][9] = 399.925476074;
   fMin_1[2][9] = 22.3099918365;
   fMax_1[2][9] = 399.925476074;
   fMin_1[0][10] = 1.4910145998;
   fMax_1[0][10] = 299.927642822;
   fMin_1[1][10] = 0.326862245798;
   fMax_1[1][10] = 299.8175354;
   fMin_1[2][10] = 0.326862245798;
   fMax_1[2][10] = 299.927642822;
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

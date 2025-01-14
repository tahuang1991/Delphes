// Class: ReadMLP
// Automatically generated by MethodBase::MakeClass
//

/* configuration options =====================================================

#GEN -*-*-*-*-*-*-*-*-*-*-*- general info -*-*-*-*-*-*-*-*-*-*-*-

Method         : MLP::MLP
TMVA Release   : 4.1.2         [262402]
ROOT Release   : 5.32/04       [335876]
Creator        : lpernie
Date           : Fri Jul 15 14:31:07 2016
Host           : Linux login01.brazos.tamu.edu 2.6.32-431.23.3.el6.x86_64 #1 SMP Thu Jul 31 17:20:51 UTC 2014 x86_64 x86_64 x86_64 GNU/Linux
Dir            : /home/lpernie/Hhh/delphes/diHiggs/MVAs
Training events: 193479
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
dR_l1l2                       dR_l1l2                       dR_l1l2                       dR_l1l2                                                         'F'    [0.0702899098396,3.29992628098]
dR_b1b2                       dR_b1b2                       dR_b1b2                       dR_b1b2                                                         'F'    [0.391943722963,4.99817085266]
dR_bl                         dR_bl                         dR_bl                         dR_bl                                                           'F'    [0.400014162064,4.99432611465]
dR_l1l2b1b2                   dR_l1l2b1b2                   dR_l1l2b1b2                   dR_l1l2b1b2                                                     'F'    [0.00442243833095,5.99961948395]
MINdR_bl                      MINdR_bl                      MINdR_bl                      MINdR_bl                                                        'F'    [0.400007665157,3.19964170456]
dphi_l1l2b1b2                 dphi_l1l2b1b2                 dphi_l1l2b1b2                 dphi_l1l2b1b2                                                   'F'    [6.74796683597e-05,3.14159226418]
mass_l1l2                     mass_l1l2                     mass_l1l2                     mass_l1l2                                                       'F'    [5.00372505188,99.99949646]
mass_b1b2                     mass_b1b2                     mass_b1b2                     mass_b1b2                                                       'F'    [22.2296066284,699.155944824]
mass_trans                    mass_trans                    mass_trans                    mass_trans                                                      'F'    [0,249.99508667]
MT2                           MT2                           MT2                           MT2                                                             'F'    [22.3099918365,399.818786621]
pt_b1b2                       pt_b1b2                       pt_b1b2                       pt_b1b2                                                         'F'    [0.326862245798,299.870025635]
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
      fVmax[0] = 0.99999988079071;
      fVmin[1] = -1;
      fVmax[1] = 1;
      fVmin[2] = -1;
      fVmax[2] = 1;
      fVmin[3] = -1;
      fVmax[3] = 1;
      fVmin[4] = -1;
      fVmax[4] = 0.99999988079071;
      fVmin[5] = -1;
      fVmax[5] = 1;
      fVmin[6] = -1;
      fVmax[6] = 0.99999988079071;
      fVmin[7] = -1;
      fVmax[7] = 1;
      fVmin[8] = -1;
      fVmax[8] = 0.99999988079071;
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
   fWeightMatrix0to1[0][0] = -1.17156244573662;
   fWeightMatrix0to1[1][0] = -0.475669346019133;
   fWeightMatrix0to1[2][0] = -0.763721505744722;
   fWeightMatrix0to1[3][0] = 1.30775969046304;
   fWeightMatrix0to1[4][0] = -1.30026612636786;
   fWeightMatrix0to1[5][0] = -1.0170002486144;
   fWeightMatrix0to1[6][0] = 0.722406003401096;
   fWeightMatrix0to1[7][0] = 0.992309980415813;
   fWeightMatrix0to1[8][0] = -0.679368677934858;
   fWeightMatrix0to1[9][0] = -1.180358724249;
   fWeightMatrix0to1[10][0] = -0.871488239314106;
   fWeightMatrix0to1[11][0] = 1.84051732298965;
   fWeightMatrix0to1[12][0] = -0.635598078451536;
   fWeightMatrix0to1[13][0] = -0.347033099902438;
   fWeightMatrix0to1[14][0] = -0.990548902990724;
   fWeightMatrix0to1[15][0] = 0.647057445980099;
   fWeightMatrix0to1[0][1] = -1.93558264548013;
   fWeightMatrix0to1[1][1] = 0.784426696296758;
   fWeightMatrix0to1[2][1] = 0.82320063952641;
   fWeightMatrix0to1[3][1] = 1.44420804014946;
   fWeightMatrix0to1[4][1] = -0.467346560331141;
   fWeightMatrix0to1[5][1] = -0.56938440972379;
   fWeightMatrix0to1[6][1] = -0.674933923937032;
   fWeightMatrix0to1[7][1] = -0.257663553233514;
   fWeightMatrix0to1[8][1] = -0.862561799259273;
   fWeightMatrix0to1[9][1] = -0.26460937161751;
   fWeightMatrix0to1[10][1] = 1.02297717994644;
   fWeightMatrix0to1[11][1] = 0.576735599135572;
   fWeightMatrix0to1[12][1] = -1.61781752178722;
   fWeightMatrix0to1[13][1] = 0.260362792491478;
   fWeightMatrix0to1[14][1] = 0.696786683041164;
   fWeightMatrix0to1[15][1] = 1.29717838308076;
   fWeightMatrix0to1[0][2] = -0.820526952822052;
   fWeightMatrix0to1[1][2] = 0.273988803832347;
   fWeightMatrix0to1[2][2] = 0.629103536235362;
   fWeightMatrix0to1[3][2] = -0.0697010759299527;
   fWeightMatrix0to1[4][2] = -0.817694123258459;
   fWeightMatrix0to1[5][2] = 1.62345588253642;
   fWeightMatrix0to1[6][2] = 0.10929123979835;
   fWeightMatrix0to1[7][2] = 0.748559872931418;
   fWeightMatrix0to1[8][2] = 1.36410178634843;
   fWeightMatrix0to1[9][2] = 0.711548798845929;
   fWeightMatrix0to1[10][2] = 1.78246521035659;
   fWeightMatrix0to1[11][2] = 0.0693682280064084;
   fWeightMatrix0to1[12][2] = -0.295240853390278;
   fWeightMatrix0to1[13][2] = -1.38484182061407;
   fWeightMatrix0to1[14][2] = 0.927689026228364;
   fWeightMatrix0to1[15][2] = -0.278626819403335;
   fWeightMatrix0to1[0][3] = 0.82802591231077;
   fWeightMatrix0to1[1][3] = 0.743189119580813;
   fWeightMatrix0to1[2][3] = 0.908654071202602;
   fWeightMatrix0to1[3][3] = 1.52065181361263;
   fWeightMatrix0to1[4][3] = -1.02034163498769;
   fWeightMatrix0to1[5][3] = 1.15164105266829;
   fWeightMatrix0to1[6][3] = -0.885874511838729;
   fWeightMatrix0to1[7][3] = -1.11629571757066;
   fWeightMatrix0to1[8][3] = -0.964170104241431;
   fWeightMatrix0to1[9][3] = -1.78136379296483;
   fWeightMatrix0to1[10][3] = 1.61250325697574;
   fWeightMatrix0to1[11][3] = 0.086605704651708;
   fWeightMatrix0to1[12][3] = -1.47181982434729;
   fWeightMatrix0to1[13][3] = 0.353927011426175;
   fWeightMatrix0to1[14][3] = -0.0342621372818088;
   fWeightMatrix0to1[15][3] = 0.322577458621205;
   fWeightMatrix0to1[0][4] = 1.06756634463298;
   fWeightMatrix0to1[1][4] = 0.0419287558999319;
   fWeightMatrix0to1[2][4] = -0.00533673496393715;
   fWeightMatrix0to1[3][4] = -0.907897694554828;
   fWeightMatrix0to1[4][4] = 1.29148323051642;
   fWeightMatrix0to1[5][4] = 0.0920854696085523;
   fWeightMatrix0to1[6][4] = -0.153376737355088;
   fWeightMatrix0to1[7][4] = -1.67447059020004;
   fWeightMatrix0to1[8][4] = 0.906005519008849;
   fWeightMatrix0to1[9][4] = 0.451912366272011;
   fWeightMatrix0to1[10][4] = 1.11075061637641;
   fWeightMatrix0to1[11][4] = -0.757137980515514;
   fWeightMatrix0to1[12][4] = -1.38138337245197;
   fWeightMatrix0to1[13][4] = -1.58995021212918;
   fWeightMatrix0to1[14][4] = 0.835831805332887;
   fWeightMatrix0to1[15][4] = -0.0116055953309391;
   fWeightMatrix0to1[0][5] = -0.0518667251787077;
   fWeightMatrix0to1[1][5] = -0.0828962241355916;
   fWeightMatrix0to1[2][5] = -0.145037060390171;
   fWeightMatrix0to1[3][5] = -0.496424302107071;
   fWeightMatrix0to1[4][5] = 0.0560154170615697;
   fWeightMatrix0to1[5][5] = -0.426597663730467;
   fWeightMatrix0to1[6][5] = -0.1159580170183;
   fWeightMatrix0to1[7][5] = 0.14728831248098;
   fWeightMatrix0to1[8][5] = -1.5624310621002;
   fWeightMatrix0to1[9][5] = -1.05179411046153;
   fWeightMatrix0to1[10][5] = -0.712801389103221;
   fWeightMatrix0to1[11][5] = -0.18419339132036;
   fWeightMatrix0to1[12][5] = 1.37919126622254;
   fWeightMatrix0to1[13][5] = -0.28625744950979;
   fWeightMatrix0to1[14][5] = -0.483810942492044;
   fWeightMatrix0to1[15][5] = 1.27384586578816;
   fWeightMatrix0to1[0][6] = -0.280613945495003;
   fWeightMatrix0to1[1][6] = 0.480060277378066;
   fWeightMatrix0to1[2][6] = -0.3156142724615;
   fWeightMatrix0to1[3][6] = -0.202804352134903;
   fWeightMatrix0to1[4][6] = -1.06154523051312;
   fWeightMatrix0to1[5][6] = -1.21908349542907;
   fWeightMatrix0to1[6][6] = 0.773441927206314;
   fWeightMatrix0to1[7][6] = 0.987711564220798;
   fWeightMatrix0to1[8][6] = -0.642700392049894;
   fWeightMatrix0to1[9][6] = 1.34565984468323;
   fWeightMatrix0to1[10][6] = -0.280465771867034;
   fWeightMatrix0to1[11][6] = -0.214382455846367;
   fWeightMatrix0to1[12][6] = -0.0528545733537057;
   fWeightMatrix0to1[13][6] = 0.366246693423899;
   fWeightMatrix0to1[14][6] = -1.10487345886101;
   fWeightMatrix0to1[15][6] = 0.349562629527605;
   fWeightMatrix0to1[0][7] = 0.845475779961167;
   fWeightMatrix0to1[1][7] = -1.55013406662262;
   fWeightMatrix0to1[2][7] = -2.54556193613606;
   fWeightMatrix0to1[3][7] = 1.88246523663031;
   fWeightMatrix0to1[4][7] = 0.631488590186456;
   fWeightMatrix0to1[5][7] = 0.165107440625724;
   fWeightMatrix0to1[6][7] = 6.32492116315939;
   fWeightMatrix0to1[7][7] = -2.70394630450171;
   fWeightMatrix0to1[8][7] = -0.580860143765768;
   fWeightMatrix0to1[9][7] = -0.462308221920843;
   fWeightMatrix0to1[10][7] = -0.034495076748769;
   fWeightMatrix0to1[11][7] = -0.470209373933286;
   fWeightMatrix0to1[12][7] = 0.924667816068383;
   fWeightMatrix0to1[13][7] = -1.02332209047232;
   fWeightMatrix0to1[14][7] = -0.678820289911364;
   fWeightMatrix0to1[15][7] = -0.135671372348456;
   fWeightMatrix0to1[0][8] = -0.496829257366268;
   fWeightMatrix0to1[1][8] = -0.0670943595861408;
   fWeightMatrix0to1[2][8] = -0.609856851624522;
   fWeightMatrix0to1[3][8] = -1.64845571836105;
   fWeightMatrix0to1[4][8] = 0.69315184130671;
   fWeightMatrix0to1[5][8] = 0.0255415536746762;
   fWeightMatrix0to1[6][8] = 0.756546907190286;
   fWeightMatrix0to1[7][8] = 0.429811758216377;
   fWeightMatrix0to1[8][8] = 1.3675921601619;
   fWeightMatrix0to1[9][8] = -1.28576206421807;
   fWeightMatrix0to1[10][8] = 0.15459659613013;
   fWeightMatrix0to1[11][8] = 0.185213548697138;
   fWeightMatrix0to1[12][8] = 1.15174290870568;
   fWeightMatrix0to1[13][8] = -0.0795714786386138;
   fWeightMatrix0to1[14][8] = -0.153310704116962;
   fWeightMatrix0to1[15][8] = -0.221281582037302;
   fWeightMatrix0to1[0][9] = -0.0925205004189362;
   fWeightMatrix0to1[1][9] = 2.06486991010696;
   fWeightMatrix0to1[2][9] = 3.67259975998925;
   fWeightMatrix0to1[3][9] = -0.252145241350874;
   fWeightMatrix0to1[4][9] = -0.0145022386843848;
   fWeightMatrix0to1[5][9] = 0.795714128207973;
   fWeightMatrix0to1[6][9] = -3.36957624094686;
   fWeightMatrix0to1[7][9] = -0.371557638196655;
   fWeightMatrix0to1[8][9] = 2.11746528766984;
   fWeightMatrix0to1[9][9] = -1.00401718207627;
   fWeightMatrix0to1[10][9] = -0.349231194005893;
   fWeightMatrix0to1[11][9] = -0.959941688685655;
   fWeightMatrix0to1[12][9] = 0.969829697160704;
   fWeightMatrix0to1[13][9] = -0.506249182966308;
   fWeightMatrix0to1[14][9] = 1.73685868939326;
   fWeightMatrix0to1[15][9] = -1.61998449499893;
   fWeightMatrix0to1[0][10] = 0.150446423506957;
   fWeightMatrix0to1[1][10] = 0.668806772694271;
   fWeightMatrix0to1[2][10] = 0.834604218980868;
   fWeightMatrix0to1[3][10] = 0.660364234787832;
   fWeightMatrix0to1[4][10] = 1.20419485579672;
   fWeightMatrix0to1[5][10] = 0.425827652866713;
   fWeightMatrix0to1[6][10] = -1.3777249880736;
   fWeightMatrix0to1[7][10] = 0.021046729868608;
   fWeightMatrix0to1[8][10] = -0.203797229554298;
   fWeightMatrix0to1[9][10] = -1.02855186562843;
   fWeightMatrix0to1[10][10] = 2.29180788009011;
   fWeightMatrix0to1[11][10] = -0.186915731712112;
   fWeightMatrix0to1[12][10] = -0.340934492166543;
   fWeightMatrix0to1[13][10] = 0.244519646963337;
   fWeightMatrix0to1[14][10] = 1.23581259442792;
   fWeightMatrix0to1[15][10] = -1.62266970935957;
   fWeightMatrix0to1[0][11] = -3.05736257554259;
   fWeightMatrix0to1[1][11] = -2.22931708609365;
   fWeightMatrix0to1[2][11] = -2.02012237657203;
   fWeightMatrix0to1[3][11] = -1.69938824500056;
   fWeightMatrix0to1[4][11] = -2.31469530251978;
   fWeightMatrix0to1[5][11] = -2.12569086028119;
   fWeightMatrix0to1[6][11] = 5.68223759410972;
   fWeightMatrix0to1[7][11] = 0.974835558226453;
   fWeightMatrix0to1[8][11] = -1.60291166266002;
   fWeightMatrix0to1[9][11] = 0.575615696880136;
   fWeightMatrix0to1[10][11] = -0.923521727112484;
   fWeightMatrix0to1[11][11] = 2.87033885875369;
   fWeightMatrix0to1[12][11] = -0.563254342653107;
   fWeightMatrix0to1[13][11] = 1.65690061860027;
   fWeightMatrix0to1[14][11] = -1.96643254765074;
   fWeightMatrix0to1[15][11] = 1.55921835190074;
   // weight matrix from layer 1 to 2
   fWeightMatrix1to2[0][0] = -0.410913926305586;
   fWeightMatrix1to2[0][1] = -0.461600236364173;
   fWeightMatrix1to2[0][2] = 0.192118707565787;
   fWeightMatrix1to2[0][3] = -0.00155123302294642;
   fWeightMatrix1to2[0][4] = -0.0173881580333048;
   fWeightMatrix1to2[0][5] = -0.126074807308905;
   fWeightMatrix1to2[0][6] = -0.432890687337824;
   fWeightMatrix1to2[0][7] = -0.0104731231874945;
   fWeightMatrix1to2[0][8] = -0.00183536019485599;
   fWeightMatrix1to2[0][9] = -0.00133491392492999;
   fWeightMatrix1to2[0][10] = 0.053106664649522;
   fWeightMatrix1to2[0][11] = -0.760681278704935;
   fWeightMatrix1to2[0][12] = -0.00132985170488683;
   fWeightMatrix1to2[0][13] = 0.071727957474035;
   fWeightMatrix1to2[0][14] = -0.063456769043049;
   fWeightMatrix1to2[0][15] = -0.00151186649562864;
   fWeightMatrix1to2[0][16] = 0.300224554208817;
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
   fMin_1[0][0] = 0.0702899098396;
   fMax_1[0][0] = 3.21356654167;
   fMin_1[1][0] = 0.0706459358335;
   fMax_1[1][0] = 3.29992628098;
   fMin_1[2][0] = 0.0702899098396;
   fMax_1[2][0] = 3.29992628098;
   fMin_1[0][1] = 0.397854149342;
   fMax_1[0][1] = 4.63783788681;
   fMin_1[1][1] = 0.391943722963;
   fMax_1[1][1] = 4.99817085266;
   fMin_1[2][1] = 0.391943722963;
   fMax_1[2][1] = 4.99817085266;
   fMin_1[0][2] = 0.486945778131;
   fMax_1[0][2] = 4.73591423035;
   fMin_1[1][2] = 0.400014162064;
   fMax_1[1][2] = 4.99432611465;
   fMin_1[2][2] = 0.400014162064;
   fMax_1[2][2] = 4.99432611465;
   fMin_1[0][3] = 0.330653727055;
   fMax_1[0][3] = 5.65737962723;
   fMin_1[1][3] = 0.00442243833095;
   fMax_1[1][3] = 5.99961948395;
   fMin_1[2][3] = 0.00442243833095;
   fMax_1[2][3] = 5.99961948395;
   fMin_1[0][4] = 0.42992991209;
   fMax_1[0][4] = 3.19964170456;
   fMin_1[1][4] = 0.400007665157;
   fMax_1[1][4] = 3.19927811623;
   fMin_1[2][4] = 0.400007665157;
   fMax_1[2][4] = 3.19964170456;
   fMin_1[0][5] = 0.0188469719142;
   fMax_1[0][5] = 3.1415617466;
   fMin_1[1][5] = 6.74796683597e-05;
   fMax_1[1][5] = 3.14159226418;
   fMin_1[2][5] = 6.74796683597e-05;
   fMax_1[2][5] = 3.14159226418;
   fMin_1[0][6] = 5.13194799423;
   fMax_1[0][6] = 96.6272277832;
   fMin_1[1][6] = 5.00372505188;
   fMax_1[1][6] = 99.99949646;
   fMin_1[2][6] = 5.00372505188;
   fMax_1[2][6] = 99.99949646;
   fMin_1[0][7] = 22.6960010529;
   fMax_1[0][7] = 687.840087891;
   fMin_1[1][7] = 22.2296066284;
   fMax_1[1][7] = 699.155944824;
   fMin_1[2][7] = 22.2296066284;
   fMax_1[2][7] = 699.155944824;
   fMin_1[0][8] = 0;
   fMax_1[0][8] = 249.99508667;
   fMin_1[1][8] = 0;
   fMax_1[1][8] = 249.994110107;
   fMin_1[2][8] = 0;
   fMax_1[2][8] = 249.99508667;
   fMin_1[0][9] = 59.3439483643;
   fMax_1[0][9] = 398.157775879;
   fMin_1[1][9] = 22.3099918365;
   fMax_1[1][9] = 399.818786621;
   fMin_1[2][9] = 22.3099918365;
   fMax_1[2][9] = 399.818786621;
   fMin_1[0][10] = 6.38805055618;
   fMax_1[0][10] = 299.870025635;
   fMin_1[1][10] = 0.326862245798;
   fMax_1[1][10] = 299.8175354;
   fMin_1[2][10] = 0.326862245798;
   fMax_1[2][10] = 299.870025635;
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

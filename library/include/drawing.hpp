
#ifndef TRIANGULATOR_DRAWING_H_INCLUDED
#define TRIANGULATOR_DRAWING_H_INCLUDED

class ScreenParameters{
public:

  // ################
  // Data structures:
  // ################
  
  // Dimension of the ambient space:
  int Damb;
  
  // Distance of the screen from the coordinate origin of
  // the ambient space:
  double d;

  // Scaling parameters for the X and Y axes of the screen
  // coordinate system:
  double sx;
  double sy;

  // Distance of the observer's eye from the screen:
  double sz;

  // Angles determining the orientation of the screen origin
  // with respect to the coordinate system of ambient space,
  // with their domain specification:
  vector<double> alpha;
  vector<double> alphaMin;
  vector<double> alphaMax;

  // Angles determining the orientation of the X axis of the
  // screen coordinate system, with their domain specification:
  vector<double> beta;
  vector<double> betaMin;
  vector<double> betaMax;

  // Angles determining the orientation of the Y axis of the
  // screen coordinate system, with their domain specification:
  vector<double> gamma;
  vector<double> gammaMin;
  vector<double> gammaMax;

  // #############################
  // Constructors and destructors:
  // #############################
  
  // Constructor, for a given dimension of ambient space:
  ScreenParameters(int dimension);

  // Destructor:
  ~ScreenParameters();
};

#ifndef EMBDATA_STRUCT
#define EMBDATA_STRUCT

struct EmbData {
  KSimplex *simplex;
  vector<double> x;
};

#endif

#ifndef EDGEDATA_STRUCT
#define EDGEDATA_STRUCT

struct EdgeData {
  KSimplex *simplex1;
  KSimplex *simplex2;
};

#endif

#ifndef SCREENCOORDS_STRUCT
#define SCREENCOORDS_STRUCT

struct ScreenCoords {
  KSimplex *simplex;
  int X;
  int Y;
  double Z;
};

#endif

// A setup function that calls the constructor based on the
// topology of the given complex:
ScreenParameters* setup_screen_parameters(SimpComp *simpComp);

bool initialize_drawing_coordinates(SimpComp *simpComp);

bool reset_intrinsic_coordinates(SimpComp *simpComp);

void recenter_intrinsic_coordinates(SimpComp *simpComp);

void evaluate_embedding_coordinates(SimpComp *simpComp);

double evaluate_coordinate_distance(KSimplex *vertex1, KSimplex *vertex2, SimpComp *simpComp);

double evaluate_coordinate_edge_length(KSimplex *edge, SimpComp *simpComp);

double evaluate_potential(SimpComp *simpComp);

double evaluate_inverse_distance_potential(SimpComp *simpComp);

double evaluate_inverse_edge_potential(SimpComp *simpComp);

double evaluate_spring_distance_potential(SimpComp *simpComp);

double evaluate_spring_edge_potential(SimpComp *simpComp);

void shake_intrinsic_coordinates(SimpComp *simpComp);

void store_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &dest);

void restore_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &source);

void evaluate_potential_minimum(SimpComp *simpComp);

vector<EmbData> extract_embedding_data(SimpComp *simpComp);

vector<EdgeData> extract_edge_data(SimpComp *simpComp);

vector<ScreenCoords> evaluate_perspective_projection(vector<EmbData> embcoords, ScreenParameters *scrparams);


#endif



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

// The structure used to read coordinates of vertices during the
// calculation of their projection on the screen. 
struct EmbData {
  KSimplex *simplex;
  vector<double> x;
};

// The structure used to read which vertices should be connected
// with edges when drawing the simplicial complex on the screen. 
struct EdgeData {
  KSimplex *simplex1;
  KSimplex *simplex2;
  bool boundary;
};

// The structure used to store the calculated projection coordinates
// of vertices, that are to be drawn on the screen
struct ScreenCoords {
  KSimplex *simplex;
  int X;
  int Y;
  double Z;
};

// The structure for the data describing bounding spheres of simplices
// within a complex, used in evaluations of the potentials
struct BoundingSphere {
  KSimplex *simplex;
  string topology;
  vector<double> center;
  double radius;
};

// ########################
// Initialization functions
// ########################

// A setup function that calls the constructor based on the
// topology of the given complex, and instantiates all relevant
// parameters that describe the position of the screen in the
// ambient space
ScreenParameters* setup_screen_parameters(SimpComp *simpComp);

// Colorize all vertices of the simplicial complex with the drawing
// coordinates color, and set up random initial values for intrinsic
// coordinates.
bool initialize_drawing_coordinates(SimpComp *simpComp);

// For linear topology, translate all intrinsic coordinates by a
// calculated amount, so that they are evenly distributed around
// the coordinate origin.
void recenter_intrinsic_coordinates(SimpComp *simpComp);

// ############################################
// Embedding coordinates manipulation functions
// ############################################

// Once convenient intrinsic coordinates have been obtained, evaluate
// the embedding coordinates from them, based on the canonical way to
// embed a manifold of a given topology into an ambient Euclidean space
void evaluate_embedding_coordinates(SimpComp *simpComp);

// Calculate the distance between two sets of intrinsic coordinates,
// based on the topology of the simplicial complex
double evaluate_coordinate_distance(vector<double> q1, vector<double> q2, string topology);

// Calculate the distance between two sets of intrinsic or embedding
// coordinates, based on the topology of the simplicial complex and
// the "embedding" boolean
double evaluate_coordinate_distance(vector<double> q1, vector<double> q2, string topology, bool embedding);

// Calculate the distance between two vertices in the complex, using
// intrinsic coordinates
double evaluate_coordinate_distance(KSimplex *vertex1, KSimplex *vertex2, SimpComp *simpComp);

// Calculate the distance between two vertices in the complex, using
// embedding coordinates
double evaluate_embedding_distance(KSimplex *vertex1, KSimplex *vertex2);

// Caclulate the length of an edge, using intrinsic coordinates
double evaluate_coordinate_edge_length(KSimplex *edge, SimpComp *simpComp);

// Caclulate the length of an edge, using embedding coordinates
double evaluate_embedding_edge_length(KSimplex *edge);

// Evaluate the data od the intrinsic bounding sphere of a given simplex
BoundingSphere evaluate_intrinsic_bounding_sphere(KSimplex *simplex, SimpComp *simpComp);

// Evaluate the data od the embedding bounding sphere of a given simplex
BoundingSphere evaluate_embedding_bounding_sphere(KSimplex *simplex, SimpComp *simpComp);

// Evaluate the data od the bounding sphere of a given simplex, using either
// intrinsic or embedding coordinates, based on the "embedding" boolean
BoundingSphere evaluate_bounding_sphere(KSimplex *simplex, SimpComp *simpComp, bool embedding);

// ##########
// Potentials
// ##########

// The master function that evaluates the potential, for a given set of
// intrinsic and/or embedding coordinates. The actual potential function
// that is evaluated depends on the topology and the setup of global
// variables. Several different potentials can be turned on or off
double evaluate_potential(SimpComp *simpComp);

// Evaluate the linear well potential (definition provided in the
// implementation source for this function)
double evaluate_linear_well_potential(SimpComp *simpComp);

// Evaluate the inverse distance potential (definition provided in the
// implementation source for this function)
double evaluate_inverse_distance_potential(SimpComp *simpComp);

// Evaluate the inverse intrinsic bounding sphere potential (definition
// provided in the implementation source for this function)
double evaluate_inverse_intrinsic_bounding_sphere_potential(SimpComp *simpComp);

// Evaluate the inverse embedding bounding sphere potential (definition
// provided in the implementation source for this function)
double evaluate_inverse_embedding_bounding_sphere_potential(SimpComp *simpComp);

// Evaluate the inverse bounding sphere potential (definition provided
// in the implementation source for this function). The evaluation is
// performed either using intrinsic or embedding bounding sphere, based
// on the "embedding" boolean
double evaluate_inverse_bounding_sphere_potential(SimpComp *simpComp, bool embedding);

// Evaluate the inverse edge potential (definition provided in the
// implementation source for this function)
double evaluate_inverse_edge_potential(SimpComp *simpComp);

// Evaluate the intrinsic spring distance potential (definition provided
// in the implementation source for this function)
double evaluate_spring_distance_potential(SimpComp *simpComp);

// Evaluate the intrinsic edge spring potential (definition provided in
// the implementation source for this function)
double evaluate_spring_edge_potential(SimpComp *simpComp);

// #########################
// Minimization of potential
// #########################

// Master function that searches for the global minimum of the potential,
// starting from a number of random initial configurations of intrinsic
// and/or embedding coordinates, and applying a Monte Carlo steepest
// descent method to evaluate a minimum, for each configuration. The best
// configuration of intrinsinc and embedding coordinates is kept in the
// complex as a result
void evaluate_potential_minimum(SimpComp *simpComp);

// Re-randomize all intrinsic coordinates of the vertices in the
// complex. Used for the sampling of different arrangements of the
// vertices when calculating the global potential minimum
bool reset_intrinsic_coordinates(SimpComp *simpComp);

// Choose a slight variation of the intrinsic coordinates, to try various
// directions for the steepest descent toward a potential minimum
void shake_intrinsic_coordinates(SimpComp *simpComp);

// Helper function to save a temprorary copy of intrinsic coordinates of
// the vertices in the complex
void store_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &dest);

// Helper function to retrieve the intrinsic coordinates of the vertices
// from a saved copy
void restore_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &source);

// ##################################
// Projection on the screen functions
// ##################################

// Master function that evaluates the screen coordinates of all vertices,
// given their embedding coordinates and parameters describing the position
// of the screen in ambient space
vector<ScreenCoords> evaluate_perspective_projection(vector<EmbData> embcoords, ScreenParameters *scrparams);

// Helper function which extracts the embedding coordinates of vertices,
// to be projected on the screen
vector<EmbData> extract_embedding_data(SimpComp *simpComp);

// Helper function which extracts the information which vertices should
// be connected with edges, when drawing the complex on the screen
vector<EdgeData> extract_edge_data(SimpComp *simpComp);

#endif


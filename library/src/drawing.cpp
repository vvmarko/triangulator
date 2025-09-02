
#include "triangulator.hpp"

ScreenParameters::ScreenParameters(int dimension){
  int i;

  // The constructor for the ScreenParameters class is nontrivial,
  // since it needs to set up the default values of the parameters
  // depending on the dimension of the ambient space. This is
  // especially important for the angles, whose existance, number
  // and domains depend on the dimension, in addition to their
  // default values.
  //
  // The "trivial" constructor is deliberately not implemented,
  // because it is conceptually wrong to instantiate a member of
  // the ScreenParameter class, without doing the work that is being
  // done below.
  
  // First set the ambient dimension
  Damb = dimension;

  // Set default distance and scaling parameters
  d = 50.0;
  sx = 1.0;
  sy = 1.0;
  sz = 10.0;

  // Clear all angles, before actually setting them up
  alpha.clear();
  alphaMin.clear();
  alphaMax.clear();
  beta.clear();
  betaMin.clear();
  betaMax.clear();
  gamma.clear();
  gammaMin.clear();
  gammaMax.clear();
  
  // Angles must be set according to the ambient dimension,
  // with special treatment for Damb = 1,2.
  if(Damb == 1){
    // In Damb=1, alpha angles do not make sense, but the two constant
    // ones can in fact formally be defined (for completeness sake, since
    // they are not really useful):
    alpha.push_back(M_PI/2);
    alphaMin.push_back(M_PI/2);
    alphaMax.push_back(M_PI/2);

    alpha.push_back(0.0);
    alphaMin.push_back(0.0);
    alphaMax.push_back(0.0);

    // In Damb=1, beta and gamma angles are not even defined, so we leave
    // them completely empty.
  }

  if(Damb == 2){
    // In Damb=2, alpha angles have one free angle (known as the
    // "polar angle" of 2D polar coordinates), with the domain [0,2*pi):
    // We choose the default value zero for the polar angle:
    alpha.push_back(M_PI/2);
    alphaMin.push_back(M_PI/2);
    alphaMax.push_back(M_PI/2);

    alpha.push_back(0.0);
    alphaMin.push_back(0.0);
    alphaMax.push_back(2*M_PI);

    alpha.push_back(0.0);
    alphaMin.push_back(0.0);
    alphaMax.push_back(0.0);

    // In Damb=2, the trivial beta angles can be defined, for completeness:
    beta.push_back(M_PI/2);
    betaMin.push_back(M_PI/2);
    betaMax.push_back(M_PI/2);

    beta.push_back(0.0);
    betaMin.push_back(0.0);
    betaMax.push_back(0.0);

    // In Damb=2, the gamma angles are not even defined, so we leave
    // them completely empty.
  }

  if(Damb >=3){
    // The case Damb>=3 is already generic, with all angles being defined.
    // Generically, polar angles have the domain [0,pi], and the default
    // value pi/2, while azimuthal angles have domain [0,2*pi), and the
    // default value zero.

    // There are Damb-1 free alpha angles, and two fixed ones
    alpha.push_back(M_PI/2); // fixed angle
    alphaMin.push_back(M_PI/2);
    alphaMax.push_back(M_PI/2);

    for(i = 1; i < Damb - 1; i++) {
      alpha.push_back(M_PI/2); // Damb-2 polar angles
      alphaMin.push_back(0.0);
      alphaMax.push_back(M_PI);
    }
    
    alpha.push_back(0.0); // one azimuthal angle
    alphaMin.push_back(0.0);
    alphaMax.push_back(2*M_PI);

    alpha.push_back(0.0); // fixed angle
    alphaMin.push_back(0.0);
    alphaMax.push_back(0.0);

    // There are Damb-2 free beta angles, and two fixed ones
    beta.push_back(M_PI/2); // fixed angle
    betaMin.push_back(M_PI/2);
    betaMax.push_back(M_PI/2);

    for(i = 1; i < Damb - 2; i++) {
      beta.push_back(M_PI/2); // Damb-3 polar angles
      betaMin.push_back(0.0);
      betaMax.push_back(M_PI);
    }
    
    beta.push_back(0.0); // one azimuthal angle
    betaMin.push_back(0.0);
    betaMax.push_back(2*M_PI);

    beta.push_back(0.0); // fixed angle
    betaMin.push_back(0.0);
    betaMax.push_back(0.0);
    
    // There are Damb-3 free beta angles, and two fixed ones
    gamma.push_back(M_PI/2); // fixed angle
    gammaMin.push_back(M_PI/2);
    gammaMax.push_back(M_PI/2);

    for(i = 1; i < Damb - 3; i++) {
      gamma.push_back(M_PI/2); // Damb-4 polar angles
      gammaMin.push_back(0.0);
      gammaMax.push_back(M_PI);
    }

    if( Damb > 3 ){ // In D=3 the azimuthal angle does not exist
      gamma.push_back(0.0); // one azimuthal angle
      gammaMin.push_back(0.0);
      gammaMax.push_back(2*M_PI);
    }

    gamma.push_back(0.0); // fixed angle
    gammaMin.push_back(0.0);
    gammaMax.push_back(0.0);
  }

  if(Damb < 1){
    log_report(LOG_ERROR,"You are trying to instantiate screen parameters for a non-positive dimension! Fix your code!");
  }
}

ScreenParameters::~ScreenParameters(){
}



bool initialize_drawing_coordinates(SimpComp* simpComp){
  bool outcome;
  DrawingCoordinatesColor *color;
  int i;
  int D = simpComp->D;

  // Initialize random number generator
  srand((unsigned int)time(NULL));

  // Colorize the complex
  outcome = DrawingCoordinatesColor::colorize_entire_complex(simpComp);
  if(!outcome){
    log_report(LOG_ERROR,"Could not add DrawingCoordinatesColor to the complex, something is wrong! Aborting...");
    return false;
  }

  if(simpComp->topology == "linear"){
    // Go through all vertices of the complex:
    for(auto &vertex : simpComp->elements[0]){
      // Find a pointer to the color
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);

      // The intrinsic coordinates may have already been initialized for this vertex, in
      // some previous invocation of initialize_drawing_coordinates() function. If that is
      // the case, we wish to retain the current values of all coordinates for this vertex,
      // and we initialize them only if they have not been initialized before. In this way,
      // after adding new simplices to the complex, one can simply invoke the function
      // initialize_drawing_coordinates() again, to initialize the coordinates for the newly
      // added vertices, while skipping the old ones.

      // Verify that the intrinsic coordinates for this vertex have not been initialized:
      if( color->q.size() == 0 ){

        // A D-dimensional linear space can be embedded into a D-dimensional Euclidean space
        // Set Damb to be equal to the dimension of the complex, for linear topology:
        color->Damb = D;

        for(i = 0; i < D; i++){
          // Set initial values to zero for embedding coordinates:
          color->x.push_back(0.0);

          // Set the domain values for intrinsic coordinates:
          //        color->qMin.push_back(std::numeric_limits<double>::lowest()); // Absolute domain limits supported by the hardware
          //        color->qMax.push_back(std::numeric_limits<double>::max());
          color->qMin.push_back(-DOMAIN_LINEAR_TOPOLOGY); // Reasonable limits compatible with typical screen resolutions
          color->qMax.push_back(DOMAIN_LINEAR_TOPOLOGY);

          // Set initial random values for intrinsic coordinates:
          color->q.push_back(color->qMin[i] + (color->qMax[i] / RAND_MAX - color->qMin[i] / RAND_MAX) * rand());
        }
      }
    }
  }

  if(simpComp->topology == "sphere"){
    // Go through all vertices of the complex:
    for(auto &vertex : simpComp->elements[0]){
      // Find a pointer to the color
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);

      // Verify that the intrinsic coordinates for this vertex have not been initialized:
      if( color->q.size() == 0 ){
      
        // A D-dimensional sphere can be embedded into a (D+1)-dimensional Euclidean space
        // Set Damb to be equal to the dimension of the complex plus one, for sphere topology:
        color->Damb = D + 1;

        // Set initial values to zero for embedding coordinates:
        for(i = 0; i < color->Damb; i++) color->x.push_back(0.0);

        // For intrinsic coordinates on the sphere, we use the so-called extended spherical
        // coordinates, a total of D+2 of them, defined in the following way:
        //
        // q[0] is a constant with a value pi/2,
        // q[1] ... q[D-1] are regular spherical coordinates with the domain [0,pi],
        // q[D] is the final regular spherical coordinate, with the domain [0,2*pi),
        // q[D+1] is a constant with a value 0.
        //
        // This convention immensely simplifies all trigonometric equations that need to be
        // implemented, since we can put everything in a loop from 0 to D+1 (inclusively),
        // and not have to worry about the initial and final coordinates as special cases.
        // These extended spherical coordinates will be systematically used throughout the
        // whole library.

        // Set the intrinsic coordinate q[0]:
        color->qMin.push_back(M_PI/2); // M_PI is the value of number pi defined in <math.h>
        color->qMax.push_back(M_PI/2);
        color->q.push_back(M_PI/2);

        // Set the intrinsic coordinates q[1] ... q[D-1]:
        for(i = 1; i < D; i++){ // It is important that the counter starts from 1 instead of 0!
          color->qMin.push_back(0.0);
          color->qMax.push_back(M_PI);
          // Set initial random values for intrinsic coordinates
          color->q.push_back(color->qMin[i] + (color->qMax[i] / RAND_MAX - color->qMin[i] / RAND_MAX) * rand());
        }

        // Set the intrinsic coordinate q[D], which has different domain from the others:
        color->qMin.push_back(0.0);
        color->qMax.push_back(2*M_PI);
        // Set initial random value from this domain
        color->q.push_back(color->qMin[D] + (color->qMax[D] / RAND_MAX - color->qMin[D] / RAND_MAX) * rand());

        // Finally, set the intrinsic coordinate q[D+1]:
        color->qMin.push_back(0.0);
        color->qMax.push_back(0.0);
        color->q.push_back(0.0);
      }
    }
  }
  // At this point, all values of all coordinates in DrawingCoordinatesColor class
  // have been successfully initialized, for all vertices in the simplicial complex.
  return true;
}

void recenter_intrinsic_coordinates(SimpComp *simpComp){
  int dim;
  DrawingCoordinatesColor *color;
  vector<double> minq;
  vector<double> maxq;
  int i;
  bool outcome;

  // In linear topology, the intrinsic coordinates are initially assigned
  // completely randomly within the allowed domain, which means they can
  // end up off-center with respect to the coordinate origin. Even after
  // they are recalculated via the minimization of the potential, they
  // may remain bunched up around some non-origin point in the domain.
  // The purpose of this function is to perform a translation of the
  // whole complex along each coordinate axis in order to put the complex
  // around the coordinate origin.
  //
  // This function is typically executed after the minimization of the
  // potential, but immediately before the evaluation of the embedding
  // coordinates. The function evaluate_embedding_coordinates() invokes
  // this function automatically as needed.

  // First initialize the drawing coordinates for the complex,
  // to make sure that all vertices are colored properly. If
  // they already were colored previously, the coloring data
  // will be preserved.
  outcome = initialize_drawing_coordinates(simpComp);
  if(!outcome){
    log_report(LOG_ERROR,"Could not initialize drawing coordinates, something is wrong, aborting.");
    return;
  }
  
  // Recentering the intrinsic coordinates should ever be done
  // only for linear topology. For the sphere coordinates are
  // already centered properly, and probably for any other
  // topologies out there.
  if(simpComp->topology == "linear"){

    // Initialization of min and max arrays with zeros was done
    // just to build the vectors. Now we initialize their proper
    // values, based on the first vertex in the complex.

    // Determine the dimension of the intrinsic space
    color = DrawingCoordinatesColor::find_pointer_to_color(simpComp->elements[0][0]);
    dim = color->q.size();

    // Initialize the min and max helper arrays using the first
    // vertex as a base point
    minq.clear();
    maxq.clear();
    for(i = 0; i < dim; i++){
      maxq.push_back(color->q[i]);
      minq.push_back(color->q[i]);
    }

    // First step is to determine the current bounding box of the
    // complex, and put its coordinates into min and max arrays.
      
    // Go through all vertices of the complex
    for(auto &vertex : simpComp->elements[0]){
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);
      // Go through all the coordinates of the current vertex
      for(i = 0; i < dim; i++){
        // If the coordinate is larger than max or smaller than
        // min, readjust max and min
        if(color->q[i] > maxq[i]) maxq[i] = color->q[i];
        if(color->q[i] < minq[i]) minq[i] = color->q[i];
      }
    }

    // Given the max and min arrays now contain the outer bounding box
    // along all coordinate axes, we move the whole complex so that it
    // becomes centered around the coordinate origin. This is done by
    // translating all coordinates along each axis by the amount
    // necessary to center the bounding box around the origin.

    // Go through all vertices of the complex
    for(auto &vertex : simpComp->elements[0]){
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);
      // Translate each coordinate of the current vertex
      for(i = 0; i < dim; i++){
        color->q[i] -= (maxq[i] + minq[i]) / 2;
      }
    }
  }
}


void evaluate_embedding_coordinates(SimpComp *simpComp){
  DrawingCoordinatesColor *color;
  bool outcome;
  int i,j;

  // First initialize the drawing coordinates for the complex,
  // to make sure that all vertices are colored properly. If
  // they already were colored previously, the coloring data
  // will be preserved.
  outcome = initialize_drawing_coordinates(simpComp);
  if(!outcome){
    log_report(LOG_ERROR,"Could not initialize drawing coordinates, something is wrong, aborting.");
    return;
  }

  if(simpComp->topology == "linear"){
    // First we recenter the whole complex to sit around the
    // coordinate origin
    recenter_intrinsic_coordinates(simpComp);
    // Go through all vertices of the complex
    for(auto vertex : simpComp->elements[0]){
      // Find the drawing coordinates color of the vertex
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);
      // Copy intrinsic coordinates to embedding coordinates
      for(i = 0; i < simpComp->D; i++) color->x[i] = color->q[i];
    }
  }

  if(simpComp->topology == "sphere"){
    // Go through all vertices of the complex
    for(auto vertex : simpComp->elements[0]){
      // Find the drawing coordinates color of the vertex
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);
      // Evaluate the embedding coordinates from intrinsic coordinates
      // using the formula (LaTeX notation):
      //
      // x_{i-1} = R \cos q_i \prod_{j=0}^{i-1} \sin q_j ,  i = 1,...,D+1.
      // 
      for(i = 1; i < simpComp->D + 2; i++){
        color->x[i-1] = SPHERE_DRAWING_RADIUS * cos(color->q[i]);
        for(j = 0; j < i; j++) color->x[i-1] = color->x[i-1] * sin(color->q[j]);
      }
    }
  }
}

double evaluate_coordinate_distance(KSimplex *vertex1, KSimplex *vertex2, SimpComp *simpComp){
  DrawingCoordinatesColor *color1, *color2;
  double temp,sum;
  int i,j;

  // Find drawing coordinates colors of the two vertices
  color1 = DrawingCoordinatesColor::find_pointer_to_color(vertex1);
  color2 = DrawingCoordinatesColor::find_pointer_to_color(vertex2);

  // In the case of linear topology, we evaluate the distance between
  // two vertices using the ordinary Euclidean definition of distance,
  // i.e. the following formula:
  //
  // L = \sqrt{ \sum_{i=0}^{D-1} ( q^1_i - q^2_i )^2 } .
  //
  if(simpComp->topology == "linear"){
    sum = 0;
    for(i = 0; i < simpComp->D; i++){
      temp = color1->q[i] - color2->q[i];
      sum += temp * temp;
    }
    return sqrt(sum);
  }
  // In the case of spherical topology, we evaluate the distance between
  // two vertices using the formula for the arclength between two points
  // on a D-dimensional sphere:
  //
  // L = R \arccos{ \sum_{i=1}^{D+1} \cos q^1_i \cos q^2_i \prod_{j=0}^{i-1} \sin q^1_j \sin q^2_j }
  //
  if(simpComp->topology == "sphere"){
    sum = 0;
    for(i = 1; i <= simpComp->D + 1; i++){
      temp = cos(color1->q[i]) * cos(color2->q[i]);
      for(j = 0; j <= i - 1; j++) temp = temp * sin(color1->q[j]) * sin(color2->q[j]);
      sum += temp;
    }
    if( sum < -1.0 ) sum = -0.9; // Just to make sure sum does not veer outside the segment [-1,1],
    if( sum > 1.0 ) sum = 0.9;   // because arccos is not defined outside that segment...
    return SPHERE_DRAWING_RADIUS * acos(sum);
  }
  // We do not know how to evaluate distance for the topologies other than the ones given
  // above, we return a dummy unit value:
  return 1.0;
}

double evaluate_coordinate_edge_length(KSimplex *edge, SimpComp *simpComp){
  
  // Find the two vertices of the given edge
  KSimplex *vertex1 = edge->neighbors->elements[0][0];
  KSimplex *vertex2 = edge->neighbors->elements[0][1];

  return evaluate_coordinate_distance(vertex1,vertex2,simpComp);
}

double evaluate_potential(SimpComp *simpComp){
  string top;
  double pot;
  
  // Different choices of the potential are suitable for different
  // topologies, so we evaluate it taking into account the topology
  // of the simplicial complex. If the topology is not recognized,
  // it defaults to the basic spring potential, which is the
  // least bad option... :-)

  top = simpComp->topology;
  
  if(top == "linear"){
    // For any linear manifold, one typically uses the spring
    // potential. However, if used alone, it is prone to various
    // foldings and overlaps. These can be avoided by further
    // adding an inverse distance potential between non-nearest-
    // neighbor vertices (i.e. vertices which are not connected
    // with an edge). This pushes non-nearest-neighbor vertices as
    // far apart as possible, until they are balanced by the spring
    // tension between the nearest-neighbor vertices. In most cases
    // this "unfolds" the complex by stretching it, and hopefully
    // eliminates any overlaps.

    pot = evaluate_spring_potential(simpComp);
    pot += evaluate_inverse_distance_potential(simpComp);
    // The inverse distance potential took into accout also the
    // nearest-neighbors, so we have to subtract their contribution
    pot -= evaluate_inverse_edge_potential(simpComp);
    return pot;
  }

  if(top == "sphere"){
    // Since any D-dimensional sphere is a compact manifold, the
    // inverse distance potential on a sphere is bounded, and is
    // therefore the best choice, since it will force all vertices
    // of the complex to be as far as possible from each other,
    // distributing them "evenly" over the sphere.

    return evaluate_inverse_distance_potential(simpComp);
  }

  // If we do not recognize the topology, fall back to the
  // spring potential
  return evaluate_spring_potential(simpComp);
}
  
double evaluate_inverse_distance_potential(SimpComp *simpComp){
  KSimplex *vi;
  KSimplex *vj;
  double sum,dij;
  int i,j,N;
  
  // We evaluate the inverse distance potential according to the
  // formula
  //
  // V = \sum_{i=0}^{N-2} \sum_{j=i+1}^{N-1} const / d_{ij} .
  //
  // Here N is the number of vertices in the complex, const is the
  // interaction constant between the vertices, and d_{ij} is the
  // coordinate distance between vertices i and j.
  //
  sum = 0.0;
  N = simpComp->elements[0].size();
  
  for(i = 0; i <= N-2; i++){
    for(j = i+1; j <= N-1; j++){
      vi = simpComp->elements[0][i];
      vj = simpComp->elements[0][j];
      dij = evaluate_coordinate_distance(vi,vj,simpComp);
      if(dij == 0.0) dij = 0.001; // Small hack to avoid accidental division by zero
      sum += POTENTIAL_INVERSE_DISTANCE_INTERACTION / dij;
    }
  }
  return sum;
}

double evaluate_inverse_edge_potential(SimpComp *simpComp){
  double sum,di;
  
  // We evaluate the inverse edge potential according to the
  // formula
  //
  // V = \sum_{i=0}^{E-1} const / d_i .
  //
  // Here E is the number of edges in the complex, const is the
  // interaction constant across the edge, and d_i is the
  // coordinate length of the given edge.
  //
  sum = 0.0;
  for(auto &edge : simpComp->elements[1]){
    di = evaluate_coordinate_edge_length(edge,simpComp);
    if(di == 0.0) di = 0.001; // Small hack to avoid accidental division by zero
    sum += POTENTIAL_INVERSE_DISTANCE_INTERACTION / di;
  }
  return sum;
}

double evaluate_spring_potential(SimpComp *simpComp){
  double sum, Ledge;
  
  // We evaluate the spring potential according to the formula
  //
  // V = \sum_{i=0}^{E-1} c_1 ( L_i - c_2 )^2 .
  //
  // Here E is the number of edges in the complex, c_1 is the spring coefficient,
  // c_2 is the spring length (both c_1 and c_2 are constants), and L_i is the
  // coordinate length of the given edge.
  //
  sum = 0.0;
  for(auto &edge : simpComp->elements[1]){
    Ledge = evaluate_coordinate_edge_length(edge, simpComp);
    sum += POTENTIAL_SPRING_COEFFICIENT * (Ledge - POTENTIAL_SPRING_SIZE) * (Ledge - POTENTIAL_SPRING_SIZE);
  }
  return sum;
}

void shake_intrinsic_coordinates(SimpComp *simpComp){
  DrawingCoordinatesColor *color;
 
  // The aim is to randomly wiggle all intrinsic coordinates
  // of all vertices in the simplicial complex, by a given step.
  // The step can be positive, negative or zero, chosen by
  // random, with frequency 1/3 for each case.

  // Traverse all vertices, and all intrinsic coordinates of
  // each vertex
  for(auto vertex : simpComp->elements[0]){
    color = DrawingCoordinatesColor::find_pointer_to_color(vertex);
    for(long unsigned int i = 0; i < color->q.size(); i++){

      // pick a random number
      unsigned int randValue = rand();

      // if random is less than 1/3 of its domain,
      // step the coordinate to the left:
      if( randValue < RAND_MAX/3 ){
        color->q[i] -= POTENTIAL_SHAKE_STEP;
        // make sure the coordinate remains inside its domain:
        if(color->q[i] < color->qMin[i])
          color->q[i] = color->qMin[i];
      }

      // if random is greater than 2/3 of its domain,
      // step the coordinate to the right:
      if( randValue > (RAND_MAX/3)*2 ){
        color->q[i] += POTENTIAL_SHAKE_STEP;
        // make sure the coordinate remains inside its domain:
        if(color->q[i] > color->qMax[i])
          color->q[i] = color->qMax[i];
      }

      // if random is between 1/3 and 2/3 of its domain,
      // step the coordinate by zero, i.e. do not change
      // the coordinate, i.e. do nothing
    }
  }    
}

// Copy all drawing coordinates of each vertex in the complex
// into a destination vector of DrawingCoordinateColor elements
void store_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &dest){
  DrawingCoordinatesColor *color;

  // Empty the destination vector of any previous data
  // it may have contained
  dest.clear();

  // Traverse all vertices in the complex, and find the
  // DrawingCoordinatesColor for each vertex
  for(auto vertex : simpComp->elements[0]){
    color = DrawingCoordinatesColor::find_pointer_to_color(vertex);

    // Copy all data from DrawingCoordinatesColor, by value,
    // to a new element in the destination vector
    dest.push_back(*color); // Absolutely magical C++ notation :-)
  }    
}


// Copy the intrinsic coordinates data from a source vector of
// DrawingCoordinateColor elements into appropriate colors of
// each vertex in the complex
void restore_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &source){
  DrawingCoordinatesColor *color;
  KSimplex *vertex;

  // Traverse all vertices in the complex, and find the
  // DrawingCoordinatesColor for each vertex
  for(unsigned int i = 0; i < simpComp->elements[0].size(); i++){
    vertex = simpComp->elements[0][i];
    color = DrawingCoordinatesColor::find_pointer_to_color(vertex);

    // Populate the intrinsic coordinates for the vertex with
    // values from the source
    //
    // NB: We are not interested in copying other source data,
    // such as the domain, embedding coordinates and Damb. The
    // domain and Damb are always the same, while embedding
    // coordinates will have to be recalculated later anyway,
    // so we only want to copy the intrinsic coordinates.
    //
    for(long unsigned int k = 0; k < source[i].q.size(); k++) color->q[k] = source[i].q[k];
  }
}

void evaluate_potential_minimum(SimpComp *simpComp){
  vector<DrawingCoordinatesColor> minPotentialColors;
  double potential, minPotential;
  bool outcome;

  // The minimum of the potential is evaluated using the Monte Carlo
  // method. We start from a random point in the space of intrinsic
  // coordinates, evaluate the potential, randomly shake the intrinsic
  // coordinates to reach some nearby point, and evaluate the potential
  // again. If the new value is smaller than the old value, we have
  // moved closer to a minimum, and we start over from the new point.
  // If the new value is larger than the old value, we abandon the new
  // point, revert back to the previous point, and start over.
  //
  // The loop has two exits:
  // One: given a candidate minimum point, we may repeatedly fail to
  // find a better point in its neighborhood. We make an attept
  // POTENTIAL_MAX_TEST_COORDINATES number of times, and if all attempts
  // fail, the current candidate is likely very close to a local minimum.
  //
  // Two: the candidate minimum point lies on some large slope, and we
  // repeatedly always find a better candidate, running down the slope.
  // Depending on the nature of the potential function, the slope may be
  // infinite (i.e. no global minimum), or otherwise very large and too
  // expensive to traverse all the way down. To keep the search
  // reasonably fast, after POTENTIAL_MAX_ITERATION_NUMBER attempts we
  // give up on trying to find a local minimum, we break the loop, and
  // finish with the best point found so far.
  //
  // The user may tweak the values of the two POTENTIAL_MAX_* constants,
  // to optimize the algorithm for their usecase and available computing
  // time.
  //
  // Note that the above procedure may not find the *global* minimum of
  // the potential (which may even not exist, depending on the potential
  // function), but rather only a *local* minimum, closest to our initial
  // starting point, which was random. A global minimum could be looked
  // for by repeating the whole procedure many times over, with many
  // random choices of the initial point and keeping the best one, in the
  // hope that one of the attempts will land close to the global minimum.
  // However, given that the purpose of the whole exercise is to draw a
  // graph on the screen in a way that "looks nice", we are happy with
  // finding just some local minimum, it will do just fine, and the
  // global one is not really necessary.

  
  // First, initialize the drawing coordinates for the complex,
  // to make sure that all vertices are colored properly. If
  // they already were colored previously, the coloring data
  // will be preserved.
  outcome = initialize_drawing_coordinates(simpComp);
  if(!outcome){
    log_report(LOG_ERROR,"Could not initialize drawing coordinates, something is wrong, aborting.");
    return;
  }

  // Initialize the starting value for the candidate minimum
  // of the spring potential
  minPotential = potential = evaluate_potential(simpComp);

  // Make a copy of initial drawing coordinates that correspond to
  // the candidate minimum of the potential
  store_drawing_coordinates(simpComp, minPotentialColors);

  // Initialize loop counters
  int iIter = 0;
  int iShake = 0;

  // Start the main loop
  while( (iIter < POTENTIAL_MAX_ITERATION_NUMBER) && (iShake < POTENTIAL_MAX_TEST_COORDINATES) ){
    
    // Pick a random new point nearby
    shake_intrinsic_coordinates(simpComp);
    iShake++;

    // Evaluate the potential at the new point
    potential = evaluate_potential(simpComp);

    // If it is better than before, store the new point as the
    // candidate minimum; otherwise, revert to the previous point
    // and start over
    if(potential < minPotential){
      minPotential = potential;
      store_drawing_coordinates(simpComp, minPotentialColors);
      iShake = 0;
      iIter++;
    }else{
      restore_drawing_coordinates(simpComp, minPotentialColors);
    }
  }
  restore_drawing_coordinates(simpComp, minPotentialColors);
  // Debugging cout, remove it:
  //  cout << "Min found potential = " << minPotential << ", iIter = " << iIter << ", iShake = " << iShake << endl;
}


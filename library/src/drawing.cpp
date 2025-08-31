
#include "triangulator.hpp"


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
          color->qMin.push_back(-20.0); // Reasonable limits compatible with typical screen resolutions
          color->qMax.push_back(20.0);

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

double evaluate_coordinate_length(KSimplex *edge, SimpComp *simpComp){
  DrawingCoordinatesColor *color1, *color2;
  double temp,sum;
  int i,j;
  
  // Find the two vertices of the given edge
  KSimplex *vertex1 = edge->neighbors->elements[0][0];
  KSimplex *vertex2 = edge->neighbors->elements[0][1];
    
  // Find drawing coordinates colors of the two vertices
  color1 = DrawingCoordinatesColor::find_pointer_to_color(vertex1);
  color2 = DrawingCoordinatesColor::find_pointer_to_color(vertex2);

  // In the case of linear topology, we evaluate the length of an edge using the
  // ordinary Euclidean distance between vertices, i.e. the following formula:
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
  // In the case of spherical topology, we evaluate the length of an edge using the
  // formula for the arclength distance between two points on a sphere:
  //
  // L = R \arccos{ \sum_{i=1}^{D+1} \cos q^1_i \cos q^2_i \prod_{j=0}^{i-1} \sin q^1_j \sin q^2_j }
  //
  if(simpComp->topology == "sphere"){
    sum = 0;
    for(i = 1; i < simpComp->D + 2; i++){
      temp = cos(color1->q[i]) * cos(color2->q[i]);
      for(j = 0; j < i; j++) temp = temp * sin(color1->q[j]) * sin(color2->q[j]);
      sum += temp;
    }
    if( sum < -1 ) sum = -1.0; // Just to make sure sum does not veer outside the segment [-1,1],
    if( sum > 1 ) sum = 1.0;   // because arccos is not defined outside that segment...
    return SPHERE_DRAWING_RADIUS * acos(sum);
  }
  // We should never reach this part, but just to satisfy the
  // compiler, we return some dummy value:
  return 1.0;
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
    Ledge = evaluate_coordinate_length(edge, simpComp);
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
    for(int i = 0; i < simpComp->D; i++){

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
    for(int k = 0; k < simpComp->D; k++) color->q[k] = source[i].q[k];
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
  minPotential = potential = evaluate_spring_potential(simpComp);

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
    potential = evaluate_spring_potential(simpComp);

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
  // Debugging cout, remove it:
  cout << "Min found potential = " << minPotential << ", iIter = " << iIter << ", iShake = " << iShake << endl;
}


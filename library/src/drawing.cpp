
#include "triangulator.hpp"

ScreenParameters::ScreenParameters(int dimension){
  int i;

  // The constructor for the ScreenParameters class is nontrivial,
  // since it needs to set up the default values of the parameters
  // depending on the dimension of the ambient space. This is
  // especially important for the angles, whose existence, number
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
  d = 1000.0;
  sx = 1.0;
  sy = 1.0;
  sz = 1000.0;

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

    alpha.push_back(0.01);
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
    
    alpha.push_back(0.01); // one azimuthal angle
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
    
    beta.push_back(0.01); // one azimuthal angle
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
      gamma.push_back(0.01); // one azimuthal angle
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

ScreenParameters* setup_screen_parameters(SimpComp *simpComp){
  ScreenParameters *params = nullptr;
  if(simpComp->topology == "linear") params = new(nothrow) ScreenParameters(simpComp->D);
  if(simpComp->topology == "sphere") params = new(nothrow) ScreenParameters(simpComp->D + 1);
  if(params == nullptr){
    log_report(LOG_ERROR,"Cannot set up screen parameters, this topology is not recognized! Fix your code!");
  }
  return(params);
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
          color->qMin.push_back(-(triangulator_global::linear_topology_drawing_domain_size)); // Reasonable limits compatible with typical screen resolutions
          color->qMax.push_back(triangulator_global::linear_topology_drawing_domain_size);

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


bool reset_intrinsic_coordinates(SimpComp* simpComp){
  DrawingCoordinatesColor *color;
  long unsigned int i;

  if(simpComp->topology == "linear"){
    // Go through all vertices of the complex:
    for(auto &vertex : simpComp->elements[0]){
      // Find a pointer to the color
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);

      // Verify that the intrinsic coordinates for this vertex have already been initialized:
      if( color->q.size() != 0 ){

        for(i = 0; i < color->q.size(); i++){
          // Set initial values to zero for embedding coordinates:
          color->x[i] = 0.0;

          // Set random values for intrinsic coordinates:
          color->q[i] = (color->qMin[i] + (color->qMax[i] / RAND_MAX - color->qMin[i] / RAND_MAX) * rand());
        }
      }
    }
  }

  if(simpComp->topology == "sphere"){
    // Go through all vertices of the complex:
    for(auto &vertex : simpComp->elements[0]){
      // Find a pointer to the color
      color = DrawingCoordinatesColor::find_pointer_to_color(vertex);

      // Verify that the intrinsic coordinates for this vertex have already been initialized:
      if( color->q.size() != 0 ){
      
        // Set initial values to zero for embedding coordinates:
        for(i = 0; i < color->x.size(); i++) color->x[i] = 0.0;

        // Set the intrinsic coordinates q[1] ... q[D-1]:
        for(i = 1; i < color->q.size() - 1; i++){ // It is important that the counter starts from 1 instead of 0!
          // Set initial random values for intrinsic coordinates
          color->q[i] = (color->qMin[i] + (color->qMax[i] / RAND_MAX - color->qMin[i] / RAND_MAX) * rand());
        }
      }
    }
  }
  // At this point, all values of all coordinates in DrawingCoordinatesColor class
  // have been successfully initialized, for all vertices in the simplicial complex.
  return true;
}

vector<EmbData> extract_embedding_data(SimpComp *simpComp){
  vector<EmbData> coords;
  EmbData temp;
  long unsigned int i;
  DrawingCoordinatesColor *color;

  coords.clear();
  evaluate_embedding_coordinates(simpComp);
  for(auto &vertex : simpComp->elements[0]){
    temp.simplex = vertex;
    temp.x.clear();
    color = DrawingCoordinatesColor::find_pointer_to_color(vertex);
    for(i = 0; i < color->x.size(); i++) temp.x.push_back(color->x[i]);
    coords.push_back(temp);
  }
  return(coords);
}

vector<EdgeData> extract_edge_data(SimpComp *simpComp){
  vector <EdgeData> edges;
  EdgeData temp;
  int D = simpComp->D;
  edges.clear();

  // Traverse over all edges in the simplicial complex
  for(auto &edge : simpComp->elements[1]){
    // For each edge, remember its two neighboring vertices
    temp.simplex1 = edge->neighbors->elements[0][0];
    temp.simplex2 = edge->neighbors->elements[0][1];

    // Figure out if the edge is part of the boundary
    // Begin by assuming that it is not
    temp.boundary = false;
    // In D=1 the edge is never a boundary
    // In D=2 the edge might itself be a boundary
    if( edge->is_a_boundary() ) temp.boundary = true;
    // For D>2 the edge might be a part of a (D-1)-simplex
    // which is itself a boundary. If such a simplex exists,
    // the edge is considered part of the boundary
    if(D > 2){
      for( auto it : edge->neighbors->elements[D-1] ){
        if( it->is_a_boundary() ) temp.boundary = true;
      }
    }
    // Add obtained edge data to the vector
    edges.push_back(temp);
  }
  // Return the vector with data for all edges
  return(edges);
}

vector<ScreenCoords> evaluate_perspective_projection(vector<EmbData> embcoords, ScreenParameters *scrparams){

  // TODO: This function could be made multithreaded, since it
  // involves a lot of floating point calculations, does not
  // depend too much on dynamic memory access, can potentially
  // be vectorizable by splitting the embcoords domain into
  // multiple independent pieces, and the goal is to execute
  // it as fast as possible...
  
  int dimension;
  ScreenCoords temp;
  vector<ScreenCoords> drawingdata;

  drawingdata.clear();
  dimension = embcoords[0].x.size();

  double d = scrparams->d;
  double sx = scrparams->sx;
  double sy = scrparams->sy;
  double sz = scrparams->sz;

  // In a one-dimensional ambient space the general formulas are degenerate,
  // but one can work out that the X coordinate should be evaluated as
  //
  // XX = (x / sx) * sz / (sz + d)
  //
  // while the distance from the screen should be
  //
  // ZZ = d^2 ( 1 + x^2 / (sz + d)^2 )
  //
  if(dimension == 1){
    for(auto it : embcoords){
      temp.simplex = it.simplex;
      double XX = it.x[0] * sz / ( sx * (sz + d) );
      double ZZ = sqrt(d*d *( 1+ ( it.x[0] * it.x[0] ) / ((sz + d)*(sz + d)) ) );
      temp.X = static_cast<int>(round(XX));
      temp.Y = 0;
      temp.Z = ZZ;
      drawingdata.push_back(temp);
    }
  }

  // In a two-dimensional ambient space the general formulas are degenerate,
  // but one can work out that the X and Y coordinates should be evaluated as
  //
  // XX = (1 / sx) * (-x sin alpha + y cos alpha) * sz / (sz + d)
  // YY = (1 / sy) * ( x cos alpha + y sin alpha) * sz / (sz + d)
  //
  // while the distance from the screen should be
  //
  // ZZ = d^2 ( 1 + (x^2 + y^2) / (sz + d)^2 )
  //
  if(dimension == 2){
    for(auto it : embcoords){
      temp.simplex = it.simplex;
      double sinalpha = sin(scrparams->alpha[1]);
      double cosalpha = cos(scrparams->alpha[1]);
      double XX = (-it.x[0] * sinalpha + it.x[1] * cosalpha) * sz / ( sx * (sz + d) );
      double YY = (it.x[0] * cosalpha + it.x[1] * sinalpha) * sz / ( sy * (sz + d) );
      double ZZ = sqrt(d*d *( 1+ ( it.x[0] * it.x[0] + it.x[1] * it.x[1] ) / ((sz + d)*(sz + d)) ) );
      temp.X = static_cast<int>(round(XX));
      temp.Y = static_cast<int>(round(YY));
      temp.Z = ZZ;
      drawingdata.push_back(temp);
    }
  }

  // In cases of dimension 3 and higher, we employ the general algorithm
  if(dimension > 2){

    // Begin by reading all screen parameters into local variables,
    // for easier access
    vector<double> sinalpha; // array to hold sin(alpha)
    sinalpha.resize(scrparams->alpha.size());
    vector<double> cosalpha; // array to hold cos(alpha)
    cosalpha.resize(scrparams->alpha.size());
    vector<double> sinbeta; // same for beta and gamma
    sinbeta.resize(scrparams->beta.size());
    vector<double> cosbeta;
    cosbeta.resize(scrparams->beta.size());
    vector<double> singamma;
    singamma.resize(scrparams->gamma.size());
    vector<double> cosgamma;
    cosgamma.resize(scrparams->gamma.size());

    // Evaluate all necessary trigonometry functions
    int size = scrparams->alpha.size();
    for(int i = 0; i < size; i++){
      double angle = scrparams->alpha[i];
      sinalpha[i] = sin( angle );
      cosalpha[i] = cos( angle );
    }
    size = scrparams->beta.size();
    for(int i = 0; i < size; i++){
      double angle = scrparams->beta[i];
      sinbeta[i] = sin( angle );
      cosbeta[i] = cos( angle );
    }
    size = scrparams->gamma.size();
    for(int i = 0; i < size; i++){
      double angle = scrparams->gamma[i];
      singamma[i] = sin( angle );
      cosgamma[i] = cos( angle );
    }

    // Evaluate the array c (of size Damb), according to the formula:
    //
    // c_{i-1} = d * \cos alpha_i \prod_{j=0}^{i-1} \sin alpha_j ,
    //
    // where i = 1, ..., Damb.

    vector<double> c;
    c.resize(dimension);
    for(int i = 1; i <= dimension; i++){
      c[i-1] = d * cosalpha[i];
      for(int j = 0; j <= i-1; j++) c[i-1] *= sinalpha[j];
    }

    // Evaluate the array eX (of size Damb-1), according to the formula:
    //
    // eX_{i-1} = sx * \cos beta_i \prod_{j=0}^{i-1} \sin beta_i ,
    //
    // where i = 1, ..., Damb-1.
    
    vector<double> eX;
    eX.resize(dimension-1);
    for(int i = 1; i <= dimension-1; i++){
      eX[i-1] = sx * cosbeta[i];
      for(int j = 0; j <= i-1; j++) eX[i-1] *= sinbeta[j];
    }

    // Evaluate the array eY (of size Damb-2), according to the formula:
    //
    // eY_{i-1} = sy * \cos gamma_i \prod_{j=0}^{i-1} \sin gamma_i ,
    //
    // where i = 1, ..., Damb-2.
    
    vector<double> eY;
    eY.resize(dimension-2);
    for(int i = 1; i <= dimension-2; i++){
      eY[i-1] = sy * cosgamma[i];
      for(int j = 0; j <= i-1; j++) eY[i-1] *= singamma[j];
    }

    // Evaluate the array normN (of size Damb-1), according to the formula:
    //
    // normN_{i-1} = \sqrt{arg1_i + \cos^2 alpha_i * arg2_i} ,
    //
    // where i = 1, ..., Damb-1, and args are given as:
    //
    // arg1_i = \prod_{j=1}^i \sin^2 alpha_j ,
    //
    // arg2_i = \sum_{j=i+1}^{Damb} \cos^2 \alpha_j \prod_{k=0,k!=i}^{j-1} \sin^2 alpha_k . 

    vector<double> normN;
    normN.resize(dimension-1);
    for(int i = 1; i <= dimension-1; i++){
      double arg1 = 1.0;
      for(int j = 1; j <= i; j++) arg1 *= sinalpha[j] * sinalpha[j];
      double arg2 = 0.0;
      for(int j = i+1; j <= dimension; j++){
        double prodsines = 1.0;
        for(int k = 0; k <= j-1; k++){
          if(k!=i) prodsines *= sinalpha[k] * sinalpha[k];
        }
        arg2 += cosalpha[j] * cosalpha[j] * prodsines;
      }
      normN[i-1] = sqrt(arg1 + cosalpha[i] * cosalpha[i] * arg2);
    }
    
    // Evaluate the array normM (of size Damb-2), according to the formula:
    //
    // normM_{i-1} = \sqrt{arg1_i + \cos^2 beta_i * arg2_i} ,
    //
    // where i = 1, ..., Damb-2, and args are given as:
    //
    // arg1_i = \prod_{j=1}^i \sin^2 beta_j ,
    //
    // arg2_i = \sum_{j=i+1}^{Damb-1} \cos^2 beta_j \prod_{k=0,k!=i}^{j-1} \sin^2 beta_k . 

    vector<double> normM;
    normM.resize(dimension-2);
    for(int i = 1; i <= dimension-2; i++){
      double arg1 = 1.0;
      for(int j = 1; j <= i; j++) arg1 *= sinbeta[j] * sinbeta[j];
      double arg2 = 0.0;
      for(int j = i+1; j <= dimension-1; j++){
        double prodsines = 1.0;
        for(int k = 0; k <= j-1; k++){
          if(k!=i) prodsines *= sinbeta[k] * sinbeta[k];
        }
        arg2 += cosbeta[j] * cosbeta[j] * prodsines;
      }
      normM[i-1] = sqrt(arg1 + cosbeta[i] * cosbeta[i] * arg2);
    }
    
    // Evaluate the rectangular matrix u (of size (Damb-1) x Damb), according to the formula:
    //
    // for j < i: u_{i-1,j-1} = 0 ,
    //
    // for j = i: u_{i-1,j-1} = - 1/N_{i-1} \prod_{k=1}^{i} \sin alpha_k ,
    //
    // for j > i: u_{i-1,j-1} = 1/N_{i-1} \cos alpha_i \cos alpha_j \prod_{k=0,k!=i}^{j-1} \sin alpha_k .
    //
    // Here i = 1, ..., Damb-1, while j = 1, ..., Damb.

    vector<vector<double>> u(dimension-1, vector<double>(dimension));
    for(int i = 1; i <= dimension-1; i++)
      for(int j = 1; j <= dimension; j++){
        if(j < i) u[i-1][j-1] = 0.0;
        if(j == i){
          double prodsines = 1.0;
          for(int k = 1; k <= i; k++) prodsines *= sinalpha[k];
          u[i-1][j-1] = - prodsines / normN[i-1];
        }
        if(j > i){
          double prodsines = 1.0;
          for(int k = 0; k <= j-1; k++){
            if(k!=i) prodsines *= sinalpha[k];
          }
          u[i-1][j-1] = prodsines * cosalpha[i] * cosalpha[j] / normN[i-1];
        }
      }

    // Evaluate the rectangular matrix v (of size (Damb-2) x (Damb-1) ), according to the formula:
    //
    // for j < i: v_{i-1,j-1} = 0 ,
    //
    // for j = i: v_{i-1,j-1} = - 1/M_{i-1} \prod_{k=1}^{i} \sin beta_k ,
    //
    // for j > i: v_{i-1,j-1} = 1/M_{i-1} \cos beta_i \cos beta_j \prod_{k=0,k!=i}^{j-1} \sin beta_k .
    //
    // Here i = 1, ..., Damb-2, while j = 1, ..., Damb-1.

    vector<vector<double>> v(dimension-2, vector<double>(dimension-1));
    for(int i = 1; i <= dimension-2; i++)
      for(int j = 1; j <= dimension-1; j++){
        if(j < i) v[i-1][j-1] = 0.0;
        if(j == i){
          double prodsines = 1.0;
          for(int k = 1; k <= i; k++) prodsines *= sinbeta[k];
          v[i-1][j-1] = - prodsines / normM[i-1];
        }
        if(j > i){
          double prodsines = 1.0;
          for(int k = 0; k <= j-1; k++){
            if(k!=i) prodsines *= sinbeta[k];
          }
          v[i-1][j-1] = prodsines * cosbeta[i] * cosbeta[j] / normM[i-1];
        }
      }

    // Evaluate the following products of matrices:
    //
    // uTeX = u^T . eX ,
    //
    // vTeY = v^T . eY , (this one is an intermediary step)
    //
    // uTvTeY = u^t . vTeY .
    //
    // Here exponent T stands for matrix transpose, so that the dimensions match:
    //
    // eX has dimensions (Damb-1) x 1,
    //
    // u^T has dimensions Damb x (Damb-1),
    //
    // eY has dimensions (Damb-2) x 1,
    //
    // v^T has dimensions (Damb-1) x (Damb-2).
    //
    // The resulting matrices are vectors:
    //
    // uTeX has dimensions Damb x 1,
    //
    // vTeY has dimensions (Damb-1) x 1,
    //
    // uTvTey has dimensions Damb x 1.

    vector<double> uTeX;
    uTeX.resize(dimension);
    for(int i = 0; i < dimension; i++){
      uTeX[i] = 0.0;
      for(int j = 0; j < dimension-1; j++) uTeX[i] += u[j][i] * eX[j];
    }

    vector<double> vTeY;
    vTeY.resize(dimension-1);
    for(int i = 0; i < dimension-1; i++){
      vTeY[i] = 0.0;
      for(int j = 0; j < dimension-2; j++) vTeY[i] += v[j][i] * eY[j];
    }
        
    vector<double> uTvTeY;
    uTvTeY.resize(dimension);
    for(int i = 0; i < dimension; i++){
      uTvTeY[i] = 0.0;
      for(int j = 0; j < dimension-1; j++) uTvTeY[i] += u[j][i] * vTeY[j];
    }

    // From this point on, calculations depend on the position of each
    // vertex. All positions are tabulated in embcoords, so from now on
    // we loop the remaining calculations through embcoords...

    for(auto it : embcoords){

      // Given a coordinate vector x (of dimension Damb), and the previously
      // obtained vectors c, uTeX, uTvTeY, we now evaluate four fundamental scalar
      // products:
      //
      // vX = x . uTeX , vY = x . uTvTeY , vc = x . c , v2 = x . x
      //
      // The dimensions of all vectors is Damb. The scalars vX, vY, vc, v2 will be
      // crucial in further calculations.

      double vX = 0.0;
      double vY = 0.0;
      double vc = 0.0;
      double v2 = 0.0;
      for(int i = 0; i < dimension; i++){
        vX += it.x[i] * uTeX[i];
        vY += it.x[i] * uTvTeY[i];
        vc += it.x[i] * c[i];
        v2 += it.x[i] * it.x[i];
      }

      // Evaluate the parameter F, given as
      //
      // F = numerator / denominator,
      //
      // numerator = sz [sz + d - vc / d] ,
      //
      // denominator = sx^2 sy^2 (sz + d) [sz + d - vc / d] + sx^2 sy^2 v2 - sy^2 vX^2 - sx^2 vY^2 .

      double bracket = sz + d - (vc / d);
      double numerator = sz * bracket;
      double denominator = sx * sx * sy * sy * (sz + d) * bracket;
      denominator += sx * sx * sy * sy * v2;
      denominator -= sy * sy * vX * vX;
      denominator -= sx * sx * vY * vY;
      double F = numerator/denominator;

      // Finally, we calculate the screen coordinates XX, YY and the distance of the
      // vertex from the screen ZZ, according to the formulas:
      //
      // XX = F sy^2 vX , YY = F sx^2 vY ,
      //
      // ZZ = \sqrt{ c^2 + XX^2 + YY^2 + v2 - 2(vc + XX vX + YY vY)} .

      double XX = F * sy * sy * vX;
      double YY = F * sx * sx * vY;
      double c2 = d * d;
      double ZZ = sqrt( abs(c2 + (XX * XX) + (YY * YY) + v2 - 2 * (vc + (XX * vX) + (YY * vY))) );

      // We are done!! Collect all results and save them into drawingdata
      temp.simplex = it.simplex;
      temp.X = static_cast<int>(round(XX));
      temp.Y = static_cast<int>(round(YY));
      temp.Z = ZZ;
      drawingdata.push_back(temp);
    }
  }
  
  if( (dimension <= 0) ){
    log_report(LOG_ERROR,"The dimension of the embedding space must be positive! Fix your code!");
    temp.simplex = nullptr;
    temp.X = 0;
    temp.Y = 0;
    temp.Z = 0.0;
    drawingdata.push_back(temp);
  }
  return(drawingdata);
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
        color->x[i-1] = (triangulator_global::sphere_topology_drawing_radius) * cos(color->q[i]);
        for(j = 0; j < i; j++) color->x[i-1] = color->x[i-1] * sin(color->q[j]);
      }
    }
  }
}

double evaluate_coordinate_distance(KSimplex *vertex1, KSimplex *vertex2, SimpComp *simpComp){
  DrawingCoordinatesColor *color1, *color2;
  double distance;
 
  // Find drawing coordinates colors of the two vertices
  color1 = DrawingCoordinatesColor::find_pointer_to_color(vertex1);
  color2 = DrawingCoordinatesColor::find_pointer_to_color(vertex2);

  distance = evaluate_coordinate_distance(color1->q, color2->q, simpComp->topology);
  return distance;
}

double evaluate_embedding_distance(KSimplex *vertex1, KSimplex *vertex2){
  DrawingCoordinatesColor *color1, *color2;
  double distance;

  // This function assumes that the embedding coordinates have been evaluated,
  // otherwise it will probably return zero for the distance...
  
  // Find drawing coordinates colors of the two vertices
  color1 = DrawingCoordinatesColor::find_pointer_to_color(vertex1);
  color2 = DrawingCoordinatesColor::find_pointer_to_color(vertex2);

  distance = evaluate_coordinate_distance(color1->x, color2->x, "linear", true);
  return distance;
}

double evaluate_coordinate_distance(vector<double> q1, vector<double> q2, string topology){
  // Wrapper for implicit evaluation of the distance using intrinsic
  // coordinates, rather than embedding ones

  return evaluate_coordinate_distance(q1,q2,topology,false);
}

double evaluate_coordinate_distance(vector<double> q1, vector<double> q2, string topology, bool embedding){
  double temp,sum;
  unsigned long int i,j;

  // In the case of linear topology or in the case of embedding coordinates,
  // we evaluate the distance between two points with coordinates q1 and q2
  // using the ordinary Euclidean definition of distance, i.e. the following
  // formula:
  //
  // L = \sqrt{ \sum_{i=0}^{D-1} ( q^1_i - q^2_i )^2 } .
  //
  if( (topology == "linear")||(embedding == true) ){
    sum = 0;
    for(i = 0; i < q1.size(); i++){
      temp = q1[i] - q2[i];
      sum += temp * temp;
    }
    return sqrt(sum);
  }
  // In the case of spherical topology, we evaluate the distance between
  // two points using the formula for the arclength on a D-dimensional sphere:
  //
  // L = R \arccos{ \sum_{i=1}^{D+1} \cos q^1_i \cos q^2_i \prod_{j=0}^{i-1} \sin q^1_j \sin q^2_j }
  //
  // Note: this does not make sense for embedding coordinates
  if( (topology == "sphere")&&(embedding == false) ){
    sum = 0;
    for(i = 1; i < q1.size(); i++){
      temp = cos(q1[i]) * cos(q2[i]);
      for(j = 0; j <= i - 1; j++) temp = temp * sin(q1[j]) * sin(q2[j]);
      sum += temp;
    }
    if( sum < -1.0 ) sum = -0.9; // Just to make sure sum does not veer outside the segment [-1,1],
    if( sum > 1.0 ) sum = 0.9;   // because arccos is not defined outside that segment...
    return (triangulator_global::sphere_topology_drawing_radius) * acos(sum);
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

double evaluate_embedding_edge_length(KSimplex *edge){

  // This function assumes that the embedding coordinates have been evaluated,
  // otherwise it will probably return zero for the distance...
  
  // Find the two vertices of the given edge
  KSimplex *vertex1 = edge->neighbors->elements[0][0];
  KSimplex *vertex2 = edge->neighbors->elements[0][1];

  return evaluate_embedding_distance(vertex1,vertex2);
}

double evaluate_potential(SimpComp *simpComp){
  string top;
  double pot;
  long unsigned int bitmask;
  
  // Different choices of the potential are suitable for different
  // topologies, so we evaluate it taking into account the topology
  // of the simplicial complex. If the topology is not recognized,
  // it defaults to the basic spring potential, which is the
  // least bad option... :-)

  // Initialize topology, bitmask and potential
  top = simpComp->topology;
  bitmask = 0;
  pot = 0.0;
  
  // Set default potential bitmask for linear topology
  if(top == "linear") bitmask = triangulator_global::potential_bitmask_linear_topology;

  // Set default potential bitmask for sphere topology
  if(top == "sphere") bitmask = triangulator_global::potential_bitmask_sphere_topology;

  // If we do not recognize the topology, or the bitmask is out of
  // bounds, fall back to the simple spring edge potential
  if( (bitmask == 0) || (bitmask > 127) ) bitmask = 1;

  // We evaluate each bit in the bitmask: if it equals "1" we evaluate
  // the corresponding potential, while if it equals "0" we skip it
  if(bitmask &  1) pot += evaluate_spring_edge_potential(simpComp);
  if(bitmask &  2) pot += evaluate_spring_distance_potential(simpComp);
  if(bitmask &  4) pot += evaluate_inverse_edge_potential(simpComp);
  if(bitmask &  8) pot += evaluate_inverse_distance_potential(simpComp);
  if(bitmask & 16) pot += evaluate_linear_well_potential(simpComp);
  if(bitmask & 32) pot += evaluate_inverse_embedding_bounding_sphere_potential(simpComp);
  if(bitmask & 64) pot += evaluate_inverse_intrinsic_bounding_sphere_potential(simpComp);
  //  if(bitmask & 128) pot += 0.0; // not implemented yet
  //  if(bitmask & 256) pot += 0.0; // not implemented yet
  //  if(bitmask & 512) pot += 0.0; // not implemented yet
  //  if(bitmask & 1024) pot += 0.0; // not implemented yet
  //  if(bitmask & 2048) pot += 0.0; // not implemented yet

  return pot;
}

double evaluate_inverse_intrinsic_bounding_sphere_potential(SimpComp *simpComp){
  // Wrapper function for the choice of intrinsic boundary sphere
  return evaluate_inverse_bounding_sphere_potential(simpComp,false);
}

double evaluate_inverse_embedding_bounding_sphere_potential(SimpComp *simpComp){
  // Wrapper function for the choice of embedding boundary sphere
  return evaluate_inverse_bounding_sphere_potential(simpComp,true);
}

double evaluate_inverse_bounding_sphere_potential(SimpComp *simpComp, bool embedding){
  BoundingSphere s;
  vector<double> radius;
  double sum,dij;
  int i,j,N,D;

  // We evaluate the inverse bounding sphere potential according to the
  // formula
  //
  // V = \sum_{i=0}^{N-2} \sum_{j=i+1}^{N-1} const / d_{ij} .
  //
  // Here N is the number of cells (D-simplices) in the complex, const is
  // the interaction constant between the cells, and d_{ij} is the
  // distance between the centers of the bounding spheres of two cells
  // (i and j). This distance is equal to the sum of the radii of the
  // two spheres
  sum = 0.0;
  D = simpComp->D;
  N = simpComp->elements[D].size();
  radius.clear();

  // If we are to evaluate the embedding bounding sphere potential,
  // before we proceed it is necessary to update the values of all
  // embedding coordinates in the complex, according to the current
  // values of the intrinsic coordinates
  if(embedding == true) evaluate_embedding_coordinates(simpComp);
  
  // Evaluate and collect the radii of all cells into a vector
  for(i = 0; i < N; i++){
    s = evaluate_bounding_sphere(simpComp->elements[D][i], simpComp, embedding);
    radius.push_back(s.radius);
  }

  // Evaluate the potential according to the formula above
  for(i = 0; i <= N-2; i++){
    for(j = i+1; j <= N-1; j++){
      dij = radius[i] + radius[j];
      if(dij == 0.0) dij = 0.001; // Small hack to avoid accidental division by zero
      sum += (triangulator_global::potential_inverse_bounding_sphere_interaction) / dij;
    }
  }
  return sum;
}

double evaluate_linear_well_potential(SimpComp *simpComp){
  KSimplex *vi;
  double sum,di;
  int i,N;
  DrawingCoordinatesColor *color;
  vector<double> origin;

  // We evaluate the linear well potential according to the
  // formula
  //
  // V = \sum_{i=0}^{N-1} const * d_i .
  //
  // Here N is the number of vertices in the complex, const is the
  // interaction constant (slope coefficient of the well), and d_i
  // is the coordinate distance of vertex i from the origin of the
  // coordinate system.
  //
  sum = 0.0;
  if(simpComp->topology == "linear"){
    origin.clear();
    for(i = 0; i < simpComp->D; i++) origin.push_back(0.0);
    N = simpComp->elements[0].size();
  
    for(i = 0; i <= N-1; i++){
      vi = simpComp->elements[0][i];
      color = DrawingCoordinatesColor::find_pointer_to_color(vi);
      di = evaluate_coordinate_distance(color->x, origin, "linear", true);
      sum += (triangulator_global::potential_linear_well_interaction) * di;
    }
  }
  return sum;
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
      sum += (triangulator_global::potential_inverse_distance_interaction) / dij;
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
    sum += (triangulator_global::potential_inverse_distance_interaction) / di;
  }
  return sum;
}


double evaluate_spring_distance_potential(SimpComp *simpComp){
  KSimplex *vi;
  KSimplex *vj;
  double sum,dij;
  int i,j,N;

  // We evaluate the spring potential between any two vertices, according
  // to the formula
  //
  // V = \sum_{i=0}^{N-2} \sum_{j=i+1}^{N-1} c_1 ( d_{ij} - c_2 )^2 .
  //
  // Here N is the number of vertices in the complex,  c_1 is the spring
  // interaction constant, c_2 is the spring length, and d_{ij} is the
  // coordinate distance between vertices i and j.
  sum = 0.0;
  N = simpComp->elements[0].size();
  
  for(i = 0; i <= N-2; i++){
    for(j = i+1; j <= N-1; j++){
      vi = simpComp->elements[0][i];
      vj = simpComp->elements[0][j];
      dij = evaluate_coordinate_distance(vi,vj,simpComp);
      sum += (triangulator_global::potential_spring_non_edge_interaction)
             * (dij - (triangulator_global::potential_spring_non_edge_length))
             * (dij - (triangulator_global::potential_spring_non_edge_length));
    }
  }
  return sum;
}
  
double evaluate_spring_edge_potential(SimpComp *simpComp){
  double sum, Ledge;
  
  // We evaluate the spring potential along the edges of the simplicial complex,
  // according to the formula
  //
  // V = \sum_{i=0}^{E-1} c_1 ( L_i - c_2 )^2 .
  //
  // Here E is the number of edges in the complex, c_1 is the spring interaction
  // constant, c_2 is the spring length, and L_i is the coordinate length of the
  // given edge.
  //
  sum = 0.0;
  for(auto &edge : simpComp->elements[1]){
    Ledge = evaluate_coordinate_edge_length(edge, simpComp);
    sum += (triangulator_global::potential_spring_edge_interaction)
           * (Ledge - (triangulator_global::potential_spring_edge_length))
           * (Ledge - (triangulator_global::potential_spring_edge_length));
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
        color->q[i] -= (triangulator_global::potential_shake_step);
        // make sure the coordinate remains inside its domain:
        if(color->q[i] < color->qMin[i])
          color->q[i] = color->qMin[i];
      }

      // if random is greater than 2/3 of its domain,
      // step the coordinate to the right:
      if( randValue > (RAND_MAX/3)*2 ){
        color->q[i] += (triangulator_global::potential_shake_step);
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
  vector<DrawingCoordinatesColor> globalMinPotentialColors;
  double potential, minPotential, globalMinPotential;
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
  // find a better point in its neighborhood. We make an attept a certain
  // number of times (controlled by the global variable
  // triangulator_global::potential_max_test_coordinates), and if all
  // attempts fail, the current candidate is likely very close to a local
  // minimum.
  //
  // Two: the candidate minimum point lies on some large slope, and we
  // repeatedly always find a better candidate, running down the slope.
  // Depending on the nature of the potential function, the slope may be
  // infinite (i.e. no global minimum), or otherwise very large and too
  // expensive to traverse all the way down. To keep the search
  // reasonably fast, after a certain number of attempts (controlled by
  // the global variable
  // triangulator_global::potential_max_iteration_number), we give up on
  // trying to find a local minimum, we break the loop, and finish with
  // the best point found so far.
  //
  // The user may tweak the values of the two global variables as needed,
  // in order to optimize the algorithm for their usecase and available
  // computing time.
  //
  // Note that the above procedure may not find the *global* minimum of
  // the potential (which may even not exist, depending on the potential
  // function), but rather only a *local* minimum, closest to our initial
  // starting point, which was random. A global minimum is then looked
  // for by repeating the whole above procedure many times over, with many
  // random choices of the initial point and keeping the best one, in the
  // hope that one of the attempts will land close to the global minimum.
  // The number of attempts to find a global minimum is controlled by the
  // global variable triangulator_global::potential_global_retries_number.
  
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
  // of the global potential
  globalMinPotential = evaluate_potential(simpComp);

  // Make a copy of initial drawing coordinates that correspond to
  // the candidate global minimum of the potential
  store_drawing_coordinates(simpComp, globalMinPotentialColors);

  // Main loop for seeking global minimum
  for(int retries = 0; retries < triangulator_global::potential_global_retries_number; retries++){

    // Pick a random starting point for intrinsic coordinates
    reset_intrinsic_coordinates(simpComp);

    // Initialize the starting value for the candidate minimum
    // of the local potential
    minPotential = potential = evaluate_potential(simpComp);

    // Make a copy of initial drawing coordinates that correspond to
    // the candidate local minimum of the potential
    store_drawing_coordinates(simpComp, minPotentialColors);

    // Initialize loop counters for finding a local minimum
    int iIter = 0;
    int iShake = 0;

    // Start the main loop for local minimum
    while( (iIter < (triangulator_global::potential_max_iteration_number))
           && (iShake < (triangulator_global::potential_max_test_coordinates)) ){
    
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
    } // end of loop for local minimum

    // Evaluate the local minimum that was found
    restore_drawing_coordinates(simpComp, minPotentialColors);
    minPotential = evaluate_potential(simpComp);
    
    // Test if the new local minimum is better than the global one so far
    if(minPotential < globalMinPotential){
      // If it is, set it as the new global one
      globalMinPotential = minPotential;
      store_drawing_coordinates(simpComp, globalMinPotentialColors);
    }
  } // end of the loop for global minimum

  // We have hopefully found the global minimum, set the intrinsic coordinates
  // to that point
  restore_drawing_coordinates(simpComp, globalMinPotentialColors);
}

BoundingSphere evaluate_intrinsic_bounding_sphere(KSimplex *simplex, SimpComp *simpComp){

  return evaluate_bounding_sphere(simplex,simpComp,false);
}

BoundingSphere evaluate_embedding_bounding_sphere(KSimplex *simplex, SimpComp *simpComp){

  return evaluate_bounding_sphere(simplex,simpComp,true);
}

BoundingSphere evaluate_bounding_sphere(KSimplex *simplex, SimpComp *simpComp, bool embedding){

  BoundingSphere bsphere;
  KSimplex *extremalVertex1;
  KSimplex *extremalVertex2;
  KSimplex *extremalEdge;
  double maxdistance = 0.0;
  double tempdistance = 0.0;
  DrawingCoordinatesColor *color1;
  DrawingCoordinatesColor *color2;
  double middle;
  
  // Initialize the bounding sphere data
  bsphere.simplex = nullptr;
  bsphere.topology = "";
  bsphere.center.clear();
  bsphere.radius = 0.0;
  extremalEdge = nullptr;

  // If the complex is not defined report an error
  if(simpComp == nullptr){
    log_report(LOG_ERROR,"You are trying to evaluate a boundary sphere in a nullptr complex! Fix your code!");
    return bsphere;
  }

  // We can assign the topology info to the boundary sphere data
  bsphere.topology = simpComp->topology;
  
  // If the simplex is not defined report an error
  if(simplex == nullptr){
    log_report(LOG_ERROR,"You are trying to evaluate a boundary sphere of a nullptr! Fix your code!");
    return bsphere;
  }

  // We can assign the simplex to the boundary sphere data
  bsphere.simplex = simplex;
  
  // If the simplex is a vertex, assign its coordinates as the center of the bounding sphere, assign
  // the radius of the sphere to be zero, and return it with a warning that a bounding sphere around
  // a single vertex does not make much sense...
  if(simplex->k == 0){
    log_report(LOG_WARN,"You are trying to evaluate a boundary sphere of a single vertex. While this");
    log_report(LOG_WARN,"is technically legal, it does not make much sense, since the radius is zero.");
    color1 = DrawingCoordinatesColor::find_pointer_to_color(simplex);
    if(embedding == false){
      for(long unsigned int i = 0; i < color1->q.size(); i++) bsphere.center.push_back(color1->q[i]);
    } else {
      for(long unsigned int i = 0; i < color1->x.size(); i++) bsphere.center.push_back(color1->x[i]);
    }
    return bsphere;
  }

  // Find the edge in the simplex which has a maximum distance between its vertices, and remember
  // this distance

  // If the simplex is itself an edge, the bounding box exists only around its two vertices,
  // so the maximum distance is the edge distance, while the extremal edge is the edge itself
  if(simplex->k == 1){
    if(embedding == false){
      maxdistance =  evaluate_coordinate_edge_length(simplex, simpComp);
    } else {
      maxdistance =  evaluate_embedding_edge_length(simplex);
    }
    extremalEdge = simplex;
  }

  // If the simplex is a triangle or higher, go through its edges and find the maximal one
  if(simplex->k > 1){
    for(auto edge : simplex->neighbors->elements[1]){
      if(embedding == false){
        tempdistance =  evaluate_coordinate_edge_length(edge, simpComp);
      } else {
        tempdistance =  evaluate_embedding_edge_length(edge);
      }
      if(tempdistance > maxdistance){
        maxdistance = tempdistance;
        extremalEdge = edge;
      }
    }
  }

  // Evaluate the radius of the sphere as the half-distance of the extremal edge.
  // Also, there might be other vertices outside the sphere, but they can be at
  // most radius * \sqrt{3} away, so we increase the radius to this value. The
  // bounding sphere obtained in this way is not the minimal one, but it should
  // do a good enough job nevertheless...
  bsphere.radius = maxdistance * sqrt(3) / 2;

  // Find the two vertices of the extremal edge and their drawing coordinate colors
  extremalVertex1 = extremalEdge->neighbors->elements[0][0];
  extremalVertex2 = extremalEdge->neighbors->elements[0][1];
  color1 = DrawingCoordinatesColor::find_pointer_to_color(extremalVertex1);
  color2 = DrawingCoordinatesColor::find_pointer_to_color(extremalVertex2);

  // Evaluate the center of the bounding sphere as a midpoint between the two
  // extremal vertices
  if(embedding == false){
    for(unsigned long int i = 0; i < color1->q.size(); i++){
      middle = ( color1->q[i] + color2->q[i] ) * 0.5;
      bsphere.center.push_back(middle);
    }
  } else {
    for(unsigned long int i = 0; i < color1->x.size(); i++){
      middle = ( color1->x[i] + color2->x[i] ) * 0.5;
      bsphere.center.push_back(middle);
    }
  }

  // We are done, return the bounding sphere data
  return bsphere;
}


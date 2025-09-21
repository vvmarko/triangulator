
#include "triangulator.hpp"

namespace triangulator_global{
  // Vector of all instantiated simplicial complexes:
  vector<SimpComp*> seededComplexes;

  // Default logging parameters:
  string logFilename = "logfile.html";
  int logLevel = LOG_WARN;

  // Default values for drawing parameters:
  double potential_spring_edge_interaction = POTENTIAL_SPRING_EDGE_INTERACTION;
  double potential_spring_edge_length = POTENTIAL_SPRING_EDGE_LENGTH;
  double potential_spring_non_edge_interaction = POTENTIAL_SPRING_NON_EDGE_INTERACTION;
  double potential_spring_non_edge_length = POTENTIAL_SPRING_NON_EDGE_LENGTH;
  double potential_inverse_distance_interaction = POTENTIAL_INVERSE_DISTANCE_INTERACTION;
  double potential_inverse_bounding_sphere_interaction = POTENTIAL_INVERSE_BOUNDING_SPHERE_INTERACTION;
  double potential_shake_step = POTENTIAL_SHAKE_STEP;
  int potential_global_retries_number = POTENTIAL_GLOBAL_RETRIES_NUMBER;
  int potential_max_iteration_number = POTENTIAL_MAX_ITERATION_NUMBER;
  int potential_max_test_coordinates = POTENTIAL_MAX_TEST_COORDINATES;
  double sphere_topology_drawing_radius = SPHERE_TOPOLOGY_DRAWING_RADIUS;
  double linear_topology_drawing_domain_size = LINEAR_TOPOLOGY_DRAWING_DOMAIN_SIZE;
  
}


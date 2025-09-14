
#ifndef TRIANGULATOR_GLOBAL_VARS_H_INCLUDED
#define TRIANGULATOR_GLOBAL_VARS_H_INCLUDED

using namespace std;

namespace triangulator_global{
  // Vector of all instantiated simplicial complexes:
  extern vector<SimpComp*> seededComplexes;

  // Default logging parameters:
  extern string logFilename;
  extern int logLevel;

  // Default values for drawing parameters:
  extern double potential_spring_edge_interaction;
  extern double potential_spring_edge_length;
  extern double potential_spring_non_edge_interaction;
  extern double potential_spring_non_edge_length;
  extern double potential_inverse_distance_interaction;
  extern double potential_shake_step;
  extern int potential_global_retries_number;
  extern int potential_max_iteration_number;
  extern int potential_max_test_coordinates;
  extern double sphere_topology_drawing_radius;
  extern double linear_topology_drawing_domain_size;
  
}

#endif


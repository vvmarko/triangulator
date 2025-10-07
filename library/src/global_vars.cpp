
#include "triangulator.hpp"

namespace triangulator_global{
  // Vector of all instantiated simplicial complexes:
  vector<SimpComp*> seededComplexes;

  // Default logging parameters:
  string logFilename = "logfile.html";
  int logLevel = LOG_WARN;

  // Parameters governing the potential minimization function:
  int potential_global_retries_number = 50;
  int potential_max_iteration_number = 1000;
  int potential_max_test_coordinates = 50;
  double potential_shake_step = 0.2;
  long unsigned int potential_bitmask_linear_topology = 1; // spring edge
  long unsigned int potential_bitmask_sphere_topology = 1; // spring edge

  // Default values for the potential bitmasks:
  //
  // 0000 0000 0000 =    0 : no potential (defaults back to spring edge)
  // 0000 0000 0001 =    1 : spring edge potential
  // 0000 0000 0010 =    2 : sping distance potential
  // 0000 0000 0100 =    4 : inverse edge potenital
  // 0000 0000 1000 =    8 : inverse distance potential
  // 0000 0001 0000 =   16 : linear well potential
  // 0000 0010 0000 =   32 : inverse embedding bounding sphere potential
  // 0000 0100 0000 =   64 : inverse intrinsic bounding sphere potential
  // 0000 1000 0000 =  128 : not implemented yet
  // 0001 0000 0000 =  256 : not implemented yet
  // 0010 0000 0000 =  512 : not implemented yet
  // 0100 0000 0000 = 1024 : not implemented yet
  // 1000 0000 0000 = 2048 : not implemented yet

  // Overall scale parameters for linear and sphere topologies:
  double sphere_topology_drawing_radius = 200.0;
  double linear_topology_drawing_domain_size = 200.0;

  // Default constants for various potentials:
  double potential_spring_edge_interaction = 1.0;
  double potential_spring_edge_length = 50.0;
  double potential_spring_non_edge_interaction = 1.0;
  double potential_spring_non_edge_length = 200.0;
  double potential_inverse_distance_interaction = 1000.0;
  double potential_inverse_bounding_sphere_interaction = 1000.0;
  double potential_linear_well_interaction = 1.0;
  
}


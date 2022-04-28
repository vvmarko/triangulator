
SimpComp* seed_single_edge(string name);
SimpComp* seed_single_triangle(string name);
SimpComp* seed_single_tetrahedron(string name);

void seed_KSimplices(SimpComp* simpComp, int k);

// Seed a single simplex or sphere of dimension d:
SimpComp* seed_single_simplex_or_sphere(int D, int sphere);

SimpComp* seed_single_simplex(int D);

SimpComp* seed_sphere(int D);
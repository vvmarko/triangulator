
// Build k-simplex based on given vertices:
KSimplex* build_KSimplex(SimpComp* simpComp, set<KSimplex*> &s);

void seed_KSimplices(SimpComp* simpComp, int k);

// Seed a single simplex or sphere of dimension d:
SimpComp* seed_single_simplex_or_sphere(int D, int sphere);

SimpComp* seed_single_simplex(int D);

SimpComp* seed_sphere(int D);

// Seed a single sphere of dimension d
// by seeding a simplex of dimension d+1, and deleting only it:
SimpComp* seed_sphere_intuitively(int D);

KSimplex* build_simplex_one_level_up_with_vertex(SimpComp* simpComp, int k, KSimplex* small, KSimplex *vertex);


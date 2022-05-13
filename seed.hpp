
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

// Pachner move 1 to 4:
KSimplex* Pachner_move_1_to_4(KSimplex *kSimplex, SimpComp *simpComp);

// Pachner move 4 to 1:
KSimplex* Pachner_move_4_to_1(KSimplex *kSimplex, SimpComp *simpComp);

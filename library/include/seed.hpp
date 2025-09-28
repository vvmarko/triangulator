
#ifndef TRIANGULATOR_SEED_H_INCLUDED
#define TRIANGULATOR_SEED_H_INCLUDED

void seed_KSimplices(SimpComp* simpComp, int k);

// Seed a simplicial complex containing a single D-dimensional simplex:
SimpComp* seed_single_simplex_worker(int D, string name);

SimpComp* seed_single_simplex(int D, string name);

SimpComp* seed_sphere(int D, string name);

// Seed a sphere of dimension D by seeding a complex containing a
// single simplex of dimension D+1, and deleting its interior:
SimpComp* seed_sphere_worker(int D, string name);

KSimplex* build_simplex_one_level_up_with_vertex(SimpComp* simpComp, KSimplex* simpsmall, KSimplex *vertex);

KSimplex* build_simplex_one_level_up(SimpComp *simpComp, KSimplex* simpsmall);

KSimplex* attach_new_simplex_to_boundary( KSimplex *boundsimp , SimpComp *G);

//Unseed a specific complex - delete it from the memory and update pointers in seededComplexes vector
void unseed_complex(SimpComp *simpComp);

//Unseed all complexes - delete them from the memory and empty seededComplexes vector
void unseed_everything();

#endif


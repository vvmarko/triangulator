

// Main function that performs a Pachner move over a k-simplex simp within the
// simplicial complex simpComp
KSimplex* Pachner_move(KSimplex *simp, SimpComp *simpComp);

// Adds appropriate labels to the colors of Pachner sphere, in order to factorize
// it into internal and external piece, with respect to the given level
KSimplex* factorize_Pachner_sphere(SimpComp *simpComp, int level);

// Verifies if the numbers of neighbors at every level is the same between simp
// and its matching simplex in the Pachner sphere
bool simple_check_for_Pachner_compatibility(KSimplex *simp);

// A detailed recursive algorithm that establishes 1-to-1 correspondence between
// the neighborhood of simp and the internal piece of the sphere
bool detailed_check_for_Pachner_compatibility(KSimplex *simp, SimpComp *PachnerSphere);

// Once everything has been verified, this function actually deletes the old
// neighborgood of simp and constructs a copy of the external piece of the
// Pachner sphere in its place, thereby performing the Pachner move
bool perform_the_Pachner_move(SimpComp *simpComp, KSimplex *simp, SimpComp *PachnerSphere);



// Old code...

// Connect k-simplex neighbors based on given vertices, assuming all sub-simplices exist:
//KSimplex* build_KSimplex(SimpComp* simpComp, set<KSimplex*> &s);

// Pachner move 1 to 4:
//KSimplex* Pachner_move_1_to_4(KSimplex *kSimplex, SimpComp *simpComp);

// Pachner move 4 to 1:
//KSimplex* Pachner_move_4_to_1(KSimplex *kSimplex, SimpComp *simpComp);

// Pachner move 3 to 2:
//KSimplex* Pachner_move_3_to_2(KSimplex *simp, SimpComp *simpComp);

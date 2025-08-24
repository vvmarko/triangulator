

// Main function that performs a Pachner move over a k-simplex simp within the
// simplicial complex simpComp
KSimplex* Pachner_move(KSimplex *simp, SimpComp *simpComp);

// Adds appropriate labels to the colors of Pachner sphere, in order to factorize
// it into internal and external piece, with respect to the given level
KSimplex* factorize_Pachner_sphere(SimpComp *simpComp, int level);

// Tests if simp is a subneighbor or otherwise part of a boundary.
// If it is, returns false (test passes if it is not on the boundary since
// that is a necessary condition for a Pachner move...)
bool not_on_boundary_check_for_Pachner_compatibility(KSimplex *simp);

// Verifies if the numbers of neighbors at every level is the same between simp
// and its matching simplex in the Pachner sphere
bool simple_check_for_Pachner_compatibility(KSimplex *simp);

// A detailed recursive algorithm that establishes 1-to-1 correspondence between
// the neighborhood of simp and the internal piece of the sphere
bool detailed_check_for_Pachner_compatibility(KSimplex *simp, SimpComp *PachnerSphere);

// A recursive algorithm that tries to establish 1-to-1 matching between the simplices
// in the manifoldlist and the simplices in the spherelist, according to the permutation
// perm, and then do the same for appropriate lists of simplices one level below
bool assign_matching_recursively( vector<KSimplex*> manifoldlist , vector<KSimplex*> spherelist, vector<int> perm );

// Tests if an established 1-to-1 matching between neighborhood of simp and
// the internal part of the PachnerSphere preserves all neighbor relations
bool test_for_isomorphism( SimpComp *PachnerSphere );

// Once everything has been verified, this function actually deletes the old
// neighborgood of simp and constructs a copy of the external piece of the
// Pachner sphere in its place, thereby performing the Pachner move
bool perform_the_Pachner_move(SimpComp *simpComp, SimpComp *PachnerSphere);



// Connect k-simplex neighbors based on given vertices, assuming all sub-simplices exist:
KSimplex* build_KSimplex(SimpComp* simpComp, set<KSimplex*> &s);

// Pachner move 1 to 4:
KSimplex* Pachner_move_1_to_4(KSimplex *kSimplex, SimpComp *simpComp);

// Pachner move 4 to 1:
KSimplex* Pachner_move_4_to_1(KSimplex *kSimplex, SimpComp *simpComp);

// Pachner move 3 to 2:
KSimplex* Pachner_move_3_to_2(KSimplex *simp, SimpComp *simpComp);
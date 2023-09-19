
#include <algorithm>
#include "triangulator.hpp"

// ####################################
//           Pachner moves
// ####################################



KSimplex* Pachner_move(KSimplex *simp, SimpComp *simpComp){

  bool outcome;
  
  // First we do some sanity checks, whether simp is an element of simpComp, etc...

  if(simpComp==nullptr){
    log_report(LOG_ERROR,"The provided complex is nullptr, it does not exist!!");
    return nullptr;
  }
  if(simp==nullptr){
    log_report(LOG_ERROR,"The provided simplex is nullptr, it does not exist!!");
    return nullptr;
  }
  if(!(simpComp->is_an_element(simp))){
    log_report(LOG_ERROR,"This simplex does not belong to the given complex!!");
    return nullptr;
  }

  // Then we test whether simp is part of a boundary. If it is part of the boundary
  // (i.e. if the test fails), Pachner move is forbidden.

  outcome = not_on_boundary_check_for_Pachner_compatibility( simp );
  if (!outcome) {
    log_report(LOG_WARN,"Pachner move cannot be performed on a simplex that belongs to the boundary, skipping...");
    return nullptr;
  }
  
  // Preparatory steps. Initialize the Pachner sphere, colorize simp and the sphere.

  int D = simp->D;
  int level = simp->k;

  SimpComp *PachnerSphere = seed_sphere(D, "Pachner sphere");
  if(PachnerSphere==nullptr){
    log_report(LOG_ERROR,"Unable to seed the Pachner sphere, nullptr generated, aborting the Pachner move!!");
    return nullptr;
  }

  outcome = PachnerColor::colorize_single_simplex(simp);
  if(!outcome){
    log_report(LOG_ERROR,"Unable to colorize the simplex, something is very wrong, aborting the Pachner move!!");
    return nullptr;
  }

  outcome = PachnerColor::colorize_entire_complex(PachnerSphere);
  if(!outcome){
    log_report(LOG_ERROR,"Unable to colorize the Pachner sphere, something is very wrong, aborting the Pachner move!!");
    PachnerColor::remove_color_from_simplex(simp);
    return nullptr;
  }
  
  // Connect simp to the first k-simplex of the Pachner sphere, and make the connection immutable.

  PachnerColor *simpPachnerColor = PachnerColor::find_pointer_to_color( simp );
  PachnerColor *sphereKSimplexColor = PachnerColor::find_pointer_to_color( PachnerSphere->elements[level][0] );
  simpPachnerColor->matchingSimplex = PachnerSphere->elements[level][0];
  simpPachnerColor->immutable = true;
  sphereKSimplexColor->matchingSimplex = simp;
  sphereKSimplexColor->immutable = true;
  sphereKSimplexColor->internalSimplex = true;

  // Split the Pachner sphere into internal and external parts, evaluate the antipode.

  KSimplex *sphereAntipode = factorize_Pachner_sphere(PachnerSphere, level);
  if(sphereAntipode==nullptr){
    log_report(LOG_ERROR,"Received nullptr for the Pachner sphere antipode simplex, something is very broken, aborting the Pachner move!!");
    PachnerColor::remove_color_from_complex(simpComp);
    unseed_complex(PachnerSphere);
    return nullptr;
  }
  
  // Simplistic check of the compatibility --- simp must have the same number of
  // neighbors as its matching simplex from the Pachner sphere.

  outcome = simple_check_for_Pachner_compatibility(simp);
  if (!outcome) {
    log_report(LOG_WARN,"Pachner move cannot be performed on this simplex, skipping...");
    PachnerColor::remove_color_from_simplex(simp);
    unseed_complex(PachnerSphere);
    return nullptr;
  }

  // In-depth recursive check of the compatibility --- establish the 1-to-1 correspondence
  // between all neighbors of simp and the internal part of the Pachner sphere.

  outcome = detailed_check_for_Pachner_compatibility(simp, PachnerSphere);
  if (!outcome) {
    log_report(LOG_WARN,"Pachner move cannot be performed on this simplex, skipping...");
    PachnerColor::remove_color_from_complex(simpComp);
    unseed_complex(PachnerSphere);
    return nullptr;
  }
  
  // If all checks have passed, Pachner move can be performed. Delete the old structure in
  // simpComp, and recreate in its place a copy of the external structure of the Pachner sphere.

  outcome = perform_the_Pachner_move(simpComp, simp, PachnerSphere);
  if (!outcome) {
    log_report(LOG_PANIC,"Pachner move operation failed, the complex is probably in an inconsistent state, panic!!");
    PachnerColor::remove_color_from_complex(simpComp);
    unseed_complex(PachnerSphere);
    return nullptr;
  }

  // Pachner move is done. Evaluate the antipode, remove the PachnerColor from all simplices,
  // and delete the Pachner sphere.

  sphereKSimplexColor = PachnerColor::find_pointer_to_color( sphereAntipode );
  KSimplex *antipode = sphereKSimplexColor->matchingSimplex;
  PachnerColor::remove_color_from_complex(simpComp);
  unseed_complex(PachnerSphere);
  
  // Return the antipode k-simplex.

  return antipode;
}

KSimplex* factorize_Pachner_sphere(SimpComp *simpComp, int level)
{
  int D = simpComp->D;
  KSimplex* simp = simpComp->elements[level][0];
  int antipodeLevel = D - level;
  PachnerColor *pcolor;
  int k;
  PachnerColor *psubcolor;

  // Factorization of the Pachner sphere:
  //
  // Label all simplices as internal or external (or both)
  // according to their relation to simp:
  // - if a D-simplex is a neighbor of simp, it and all its
  //   subneighbors are called internal,
  // - if a D-simplex is not a neighbor of simp, it and all its
  //   subneighbors are called external.
  // A D-simplex is exclusively either internal or external,
  // while some simplices of lower level may be both.
  //
  // Note: if simp itself is a simplex of level D, the first
  // point above will fail, since by convention a simplex is
  // never a neighbor of itself. So level==D case must be treated
  // specially: simp and its subneighbors should be labeled as
  // internal, while all other D-simplices (and their subneighbors)
  // as external.
  for(auto &it : simpComp->elements[D]){
    pcolor = PachnerColor::find_pointer_to_color(it);
    if ( (simp->find_neighbor(it)) || (simp==it) ){ // Note: simp==it covers the case level==D
      pcolor->internalSimplex = true; // Colorize D-simplex as internal,
      for(k = 0; k <= D-1; k++){      // go through all of its subneighbors,
        for(auto &sub : it->neighbors->elements[k]){
          psubcolor = PachnerColor::find_pointer_to_color(sub);
          psubcolor->internalSimplex = true; // and colorize each as internal.
        }
      }
    }
    else{
      pcolor->externalSimplex = true; // Colorize D-simplex as external,
      for(k = 0; k <= D-1; k++){      // go through all of its subneighbors,
        for(auto &sub : it->neighbors->elements[k]){
          psubcolor = PachnerColor::find_pointer_to_color(sub);
          psubcolor->externalSimplex = true; // and colorize each as external.
        }
      }
    }
  }
  
  // Determining the sphere antipode simplex:
  //
  // The sphere antipode simplex is a unique simplex at level
  //
  //        antipodeLevel = D - level of simp
  //
  // that is a subneighbor of only external D-simplices, or equivalently,
  // that is labeled as an external but not as an internal simplex.
  // The algorithm therefore tests for the appropriate coloring to find it.
  for(auto &it : simpComp->elements[antipodeLevel]){
    pcolor = PachnerColor::find_pointer_to_color(it);
    if ( (pcolor->internalSimplex == false) && (pcolor->externalSimplex == true) )
      return it;
  }
  // For every simp, the sphere antipode simplex is guaranteed to exist and be
  // unique in the Pachner sphere (that is a theorem), so if we have finished the
  // above for loop without exiting by "return"ing the sphere antipode, something
  // went very very wrong. The code below should never ever execute:
  log_report(LOG_ERROR,"The sphere antipode calculation failed when factorizing the Pachner sphere. This should never happen, something is very very wrong!!! Returning nullptr for the sphere antipode.");
  return nullptr;
}

bool not_on_boundary_check_for_Pachner_compatibility(KSimplex *simp)
{
  int D = simp->D;  
  if ( simp->k == D ) return true; // D-simplex is never part of a boundary, by definition.
  if ( simp->is_a_boundary() ) return false; // simp may itself be a boundary if simp->k == D-1.
  for (auto &it : simp->neighbors->elements[D-1] ){ // In cases when simp->k < D-1, simp may
    if (it->is_a_boundary() ) return false;         // be a subneighbor of some boundary (D-1)-simplex.
  }
  return true; // If none of the above is the case, simp is not part of any boundary.
}

bool simple_check_for_Pachner_compatibility(KSimplex *simp)
{
  // Detailed check for compatibility of the neighborhood of simp for a Pachner move
  // can be implemented more simply if it can assume that both the simp and its match
  // have identical number of neighbors, at every level. If this is not true, we know
  // that compatibility fails, and a detailed check is unnecessary. So this test is
  // very useful because it is simple and effective, and the algorithm for the
  // detailed check of compatibility can assume that this test has passed.

  int D = simp->D;
  PachnerColor *pcolor = PachnerColor::find_pointer_to_color(simp);
  KSimplex *match = pcolor->matchingSimplex;

  for (int i = 0; i <= D; i++){
    if( ( simp->neighbors->elements[i].size() ) != ( match->neighbors->elements[i].size() ) ) return false;
  }
  return true;
}

bool detailed_check_for_Pachner_compatibility(KSimplex *simp, SimpComp *PachnerSphere)
{
  return true;
}

bool perform_the_Pachner_move(SimpComp *simpComp, KSimplex *simp, SimpComp *PachnerSphere)
{
  return true;
}



/*

// Old code...
  
// Pachner move 1 to 4:
// Input: 3-simplex (1-2-3-4)
// Output: Created 0-simplex (5)
// Beginning structure:
// k=0: 1, 2, 3, 4
// k=1: (1-2), (1-3), (1-4), (2-3), (2-4), (3-4)
// k=2: (1-2-3), (1-2-4), (1-3-4), (2-3-4)
// k=3: (1-2-3-4)
// Final structure:
// k=0: 1, 2, 3, 4, 5
// k=1: (1-2), (1-3), (1-4), (2-3), (2-4), (3-4), (1-5), (2-5), (3-5), (4-5)
// k=2: (1-2-3), (1-2-4), (1-3-4), (2-3-4), (1-2-5), (1-3-5), (1-4-5), (2-3-5), (2-4-5), (3-4-5)
// k=3: (1-2-3-5), (1-2-4-5), (1-3-4-5), (2-3-4-5)
KSimplex* Pachner_move_1_to_4(KSimplex *kSimplex, SimpComp *simpComp){
    // For each level k, save length of old simpComp.elements[k], 
    // so that old vertices, edges, triangles,... can be found:
	size_t oldLength = simpComp->elements[0].size();

    // Adding vertex:
    KSimplex *vertex = simpComp->create_ksimplex(0);

	// For each level, except for vertices:
    for(int kTemp = 1; kTemp <= simpComp->D; kTemp++){
		// Save the length of this level for further reference:
		size_t nextLength = simpComp->elements[kTemp].size();
		// For old elements from previous level:
		for(size_t i = 0; i < oldLength; i++){
			// Take k-simplex:
			KSimplex* &simpsmall = simpComp->elements[kTemp-1][i];
			// Append vertex to it, and save it at current level:
			build_simplex_one_level_up_with_vertex(simpComp, simpsmall, vertex);
		}
		// Update the old length from current level:
		oldLength = nextLength;
	}
	// Delete initial k-simplex from level D:
	simpComp->remove_simplex(simpComp->elements[simpComp->D][0]);
	
	return vertex;
}

KSimplex* build_KSimplex(SimpComp* simpComp, set<KSimplex*> &s){
	// Create new k-simplex of the level that is equal to s.size()-1:
	KSimplex* newKSimplex = simpComp->create_ksimplex(s.size()-1);
	// Add all vertices to newKSimplex:
	for(auto &it : s)
		it->add_neighbor(newKSimplex);
	newKSimplex->reconstruct_neighbors_from_vertices(simpComp);
	return newKSimplex;
}

// Pachner move 4 to 1:
// Input: 0-simplex (5) to remove.
// Output: Resulting 3-simplex (1-2-3-4).
// Beginning structure:
// k=0: 1, 2, 3, 4, 5
// k=1: (1-2), (1-3), (1-4), (2-3), (2-4), (3-4), (1-5), (2-5), (3-5), (4-5)
// k=2: (1-2-3), (1-2-4), (1-3-4), (2-3-4), (1-2-5), (1-3-5), (1-4-5), (2-3-5), (2-4-5), (3-4-5)
// k=3: (1-2-3-5), (1-2-4-5), (1-3-4-5), (2-3-4-5)
// Final structure:
// k=0: 1, 2, 3, 4
// k=1: (1-2), (1-3), (1-4), (2-3), (2-4), (3-4)
// k=2: (1-2-3), (1-2-4), (1-3-4), (2-3-4)
// k=3: (1-2-3-4)
KSimplex* Pachner_move_4_to_1(KSimplex *kSimplex, SimpComp *simpComp){
	// For each level beyond D,
    // remove kSimplex where found:
    for(int kTemp = 0; kTemp < simpComp->D; kTemp++){
		// Store current row of k-simplices:
		auto &row = simpComp->elements[kTemp];
		// Parse all elements:
		size_t i = 0;
		while(i < row.size()){
			// Collect all vertices of current k-simplex row[i]:
			set<KSimplex*> s;
			row[i]->collect_vertices(s);
			// If it includes kSimplex:
			if( s.find(kSimplex) != s.end() ){
				// Remove row[i], and parse new row[i] after (don't change i):
				row[i] = row[row.size()-1];
				row.pop_back();
			}else{
				// Advance to the next element in a row:
				i++;
			}
		}
	}
	// Store last row of k-simplices:
	auto &row = simpComp->elements[simpComp->D];
	// Collect all vertices of all k-simplices of the row:
	set<KSimplex*> s;
	while(!row.empty()){
		// Start processing from the last element of a row for faster deleting:
		row[row.size()-1]->collect_vertices(s);
		// Delete each k-simplex from a row after collecting vertices:
		simpComp->remove_simplex(row[ row.size()-1 ]);
	}
	// Remove given kSimplex from the set:
    s.erase(kSimplex);

	// Reconstruct initial k-simplex based on the set at level D:
	build_KSimplex(simpComp, s);
	
	return simpComp->elements[simpComp->D][0];
}
// Pachner move 3 to 2 
// Input: 1-simplex (1-5) to perform a move on, and complex simpComp.
// Output: 2-simplex kSimplex (2-3-4) create by the move.
// Beginning structure:
// k=0: 1, 2, 3, 4, 5
// k=1: (1-2), (1-3), (1-4), (2-3), (2-4), (3-4), (2-5), (3-5), (4-5), (1-5)
// k=2: (1-2-3), (1-2-4), (1-3-4), (2-3-5), (2-4-5), (3-4-5), (1-2-5), (1-3-5), (1-4-5)
// k=3: (1-2-3-5), (1-2-4-5), (1-3-4-5)
// Final structure:
// k=0: 1, 2, 3, 4, 5
// k=1: (1-2), (1-3), (1-4), (2-3), (2-4), (3-4), (2-5), (3-5), (4-5)
// k=2: (1-2-3), (1-2-4), (1-3-4), (2-3-4), (2-3-5), (2-4-5), (3-4-5)
// k=3: (1-2-3-4), (2-3-4-5)
KSimplex* Pachner_move_3_to_2(KSimplex *kSimplex, SimpComp *simpComp){
//kSimplex->print("");

cout << endl << "Printing simpComp..." << endl;
simpComp->print_compact();

    int k = kSimplex->k;

// Collecting vertices:
    set<KSimplex*> sKSimplex;
    kSimplex->collect_vertices(sKSimplex);
cout << endl << "kSimplex vertices:" << endl;
for(auto &it : sKSimplex)
    cout << (static_cast<UniqueIDColor*>(it->colors[0]))->id << endl;


cout << endl << "for k == 2, add (2-3-4):" << endl << endl;
    set<KSimplex*> hold;

    for(auto &it : simpComp->elements[2]){
        //cout << "Printing simpComp->elements[2]:" << endl;
        set<KSimplex*> s;
        it->collect_vertices(s);
        
        if(std::includes(s.begin(), s.end(),
                         sKSimplex.begin(), sKSimplex.end())){
            // 
            // for(auto &tempKSimplex : s){
            //     tempKSimplex->print_compact(); cout << endl;
            // }
            // 
            set_difference(begin(s), end(s),  
                           begin(sKSimplex), end(sKSimplex),  
                           inserter(hold, end(hold)));
        }

        // 
        // KSimplex *simpsmall = simpComp->create_ksimplex(0);
        // Progress to further dimensions by adding new vertex and conntecting it:
        // for(int k = 1; k <= D+sphere; k++){
            // Seed a KSimplex of level k based on KSimplex of level k-1:
        //     simpsmall = build_simplex_one_level_up(simpComp, simpsmall);
        // }
        // 
        //cout << "Printing KSimplex:" << endl;
        //it->print();
    }

    //
    // cout << "Printing hold colors:" << endl;
    // for(auto &tempHold : hold)
    //    for(auto &color : tempHold->colors){
    //        cout << (static_cast<UniqueIDColor*> (color)->id) << endl;
    //    }
    // cout << "Printing hold colors end." << endl;
    // 
    // KSimplex *result = simpComp->create_ksimplex_from_vertices(hold);
    
// cout << endl << "Printing simpComp..." << endl;
// simpComp->print_compact();
    //


cout << endl << "for k == 3, add (1-2-3-4), (2-3-4-5):" << endl;

    for(auto &it : sKSimplex){
        set<KSimplex*> s = hold;
        s.insert(it);
        KSimplex *temp = simpComp->create_ksimplex_from_vertices(s);
    }

cout << endl << "Printing simpComp..." << endl;
simpComp->print_compact();


cout << endl << "for k == 1, remove (1-5):" << endl;
    kSimplex->delete_all_neighbors();
    auto &vec = simpComp->elements[k];
    vec.erase(std::remove(vec.begin(), vec.end(), kSimplex), vec.end());    
    delete(kSimplex);

cout << endl << "Printing simpComp..." << endl;
simpComp->print_compact();

	//KSimplex *temp = simpComp->find_vertices(s);

//cout << endl << "Printing simpComp..." << endl;
//simpComp->print_compact();

return result;
}


*/




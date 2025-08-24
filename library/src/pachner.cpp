
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
    log_report(LOG_WARN,"Pachner move cannot be performed on this simplex, failed simple check, skipping...");
    PachnerColor::remove_color_from_simplex(simp);
    unseed_complex(PachnerSphere);
    return nullptr;
  }

  // In-depth recursive check of the compatibility --- establish the 1-to-1 correspondence
  // between all neighbors of simp and the internal part of the Pachner sphere.

  outcome = detailed_check_for_Pachner_compatibility(simp, PachnerSphere);
  if (!outcome) {
    log_report(LOG_WARN,"Pachner move cannot be performed on this simplex, failed detailed check, skipping...");
    PachnerColor::remove_color_from_complex(simpComp);
    unseed_complex(PachnerSphere);
    return nullptr;
  }
  
  // If all checks have passed, Pachner move can be performed. Delete the old structure in
  // simpComp, and recreate in its place a copy of the external structure of the Pachner sphere.

  outcome = perform_the_Pachner_move(simpComp, PachnerSphere);
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
  // Detailed check for compatibility is performed by constructing a 1-to-1 isomorphism
  // between the internal part of the Pachner sphere and the corresponding wide
  // neighborhood of simp, simplex for simplex. This is done in three main steps:
  //
  // (1) Construct a candidate 1-to-1 matching, compatible with the immutable flags.
  // (2) Verify if this candidate matching is indeed an isomorphism, i.e. if all neighbors
  //     match for each matching pair of simplices.
  // (3) If step (2) is true we are done. If (2) is false we go back to (1) for another
  //     candidate matching, until we exhaust all possible permutations.
  //
  // Note that this is in essence a backtracking algorithm. We traverse the space of all
  // possible permutations of matching simplices, until we find one that is an isomorphism.
  // Two outcomes are possible. Either an isomorphism exists, and we find one matching that
  // establishes it (there may be many, we are happy with a first one found), or an isomorphism
  // does not exist, and we traverse through the full permutation space and eventually fail.
  //
  // Immutable flags somewhat reduce the space of permutations, since any permutation that
  // is not compatible with the preset match for simp must fail.
  //
  // Backtracking is slow and bulky, but it is the only algorithm (that I know of) which is
  // certain to find an isomorphism if it exists, and works for simplicial complexes of
  // arbitrary dimension.

  bool outcome;
  vector<KSimplex*> manifoldlist;
  vector<KSimplex*> spherelist;
  vector<int> perm;
  
  int D = simp->D;
  int n = simp->neighbors->elements[D].size();
  PachnerColor *pcolor = PachnerColor::find_pointer_to_color(simp);
  KSimplex *simpmatch = pcolor->matchingSimplex;

  // Initialize manifoldlist and spherelist with appropriate D-simplices, for the beginning
  // of the recursive matching. Also initialize perm as a unit permutation to try first.

  if(simp->k == D){ // Case when simp is itself a D-simplex
    manifoldlist.push_back(simp);
    spherelist.push_back(simpmatch);
    perm.push_back(0);
  } else { // Case when simp has D-simplices as neighbors
    for (int i = 0; i < n; i++){
      manifoldlist.push_back(simp->neighbors->elements[D][i]);
      spherelist.push_back(simpmatch->neighbors->elements[D][i]);
      perm.push_back(i);
    }
  }

  // Perform steps (1)-(3) from above: construct recursively a candidate 1-to-1
  // matching, test if it is an isomorphism, and if not try the next permutation
  // of perm.

  do{
    outcome = assign_matching_recursively( manifoldlist , spherelist , perm );
    if (outcome) outcome = test_for_isomorphism(PachnerSphere);
  } while ( ( !outcome ) && ( next_permutation(perm.begin(),perm.end()) ) );

  if (outcome) return true; // We have either found an isomorphism,
  else return false;        // or we have exhausted all permutations without
                            // finding an isomorphism, so it does not exist.
}

bool assign_matching_recursively( vector<KSimplex*> manifoldlist , vector<KSimplex*> spherelist, vector<int> perm )
{
  // Two things are done. First, we establish the matching between simplices in manifoldlist
  // and simplices in spherelist, 1-to-1, ordering them according to perm. This takes into
  // account the possible immutable flags that may be set. If there is a clash with the immmutable
  // flags, we fail to establish matching, otherwise we succeed.
  //
  // Second, if we are higher than level=0, we set up a newmanifoldlist and newspherelist, with
  // accompanying newperm, and call ourself recursively for level-1.
  //
  // If all lower levels succeeded in matching, we are successful. If any of the lower levels
  // fails in matching, we first try a next newperm, and after exhausting all permutations, we fail.

  int level = manifoldlist[0]->k;
  int n = perm.size();
  PachnerColor *manifcolor;
  PachnerColor *sphcolor;
  int i;
  bool outcome;
 
  // Try to establish matching between i-th simplex in manifoldlist and perm[i]-th simplex
  // in spherelist, while watching out for immutable flags.

  for(i=0; i<n; i++){
    manifcolor = PachnerColor::find_pointer_to_color(manifoldlist[i]);
    if (manifcolor == nullptr){ // Maybe this simplex has not been colorized yet with PachnerColor
      outcome = PachnerColor::colorize_single_simplex(manifoldlist[i]); // If so, colorize it.
      if (!outcome){
        log_report(LOG_ERROR,"Unable to colorize the simplex, something is very wrong, assign_matching_recursively() is failing, the Pachner move will probably fail!!");
        return false;
      } else {
        manifcolor = PachnerColor::find_pointer_to_color(manifoldlist[i]);
      }
    }

    // Simplices from the Pachner sphere have all already been colorized with PachnerColor,
    // so there is no need to check and colorize as was done for manifold simplices above.
    sphcolor = PachnerColor::find_pointer_to_color(spherelist[perm[i]]);
    
    // If either simplex is immutable and the other is not, matching is impossible and perm fails.
    if( (manifcolor->immutable == true) && (sphcolor->immutable == false) ) return false;
    if( (manifcolor->immutable == false) && (sphcolor->immutable == true) ) return false;
    // If neither simplex is immutable, we match them to each other.
    if( (manifcolor->immutable == false) && (sphcolor->immutable == false) ){
      manifcolor->matchingSimplex = spherelist[perm[i]];
      sphcolor->matchingSimplex = manifoldlist[i];
    }
    // If both simplices are immutable, matching has already been set, so we do nothing.
  }

  // At this point matching has been successfully performed. If we are at level==0,
  // there is nothing more to do, recursion ends successfully.
  if(level==0) return true;

  // Otherwise, we set up for recursion to level-1.

  vector<KSimplex*> newmanifoldlist;
  vector<KSimplex*> newspherelist;
  vector<int> newperm;
  KSimplex *current;

  // Build a newmanifoldlist from level-1 neighbors of all simplices in manifoldlist,
  // skipping any duplicates.

  for(auto &it : manifoldlist){
    n = it->neighbors->elements[level-1].size();
    for(i=0; i<n; i++){
      current = it->neighbors->elements[level-1][i];
      // If the current simplex is not already present in newmanifoldlist, add it in.
      if( std::find(newmanifoldlist.begin(), newmanifoldlist.end(), current) == newmanifoldlist.end() ){
        newmanifoldlist.push_back(current);
      }
    }
  }

  // Build a newspherelist from level-1 neighbors of all simplices in spherelist,
  // skipping any duplicates.

  for(auto &it : spherelist){
    n = it->neighbors->elements[level-1].size();
    for(i=0; i<n; i++){
      current = it->neighbors->elements[level-1][i];
      // If the current simplex is not already present in newspherelist, add it in.
      if( std::find(newspherelist.begin(), newspherelist.end(), current) == newspherelist.end() ){
        newspherelist.push_back(current);
      }
    }
  }

  // If the two lists have different number of elements, matching must fail
  if( newmanifoldlist.size() != newspherelist.size() ) return false;

  // And if they have the same number of elements, setup the initial newperm

  n = newmanifoldlist.size();
  for(i=0; i<n; i++) newperm.push_back(i);

  // Finally, recurse one level below...

  do{
    outcome = assign_matching_recursively( newmanifoldlist , newspherelist , newperm );
  } while ( ( !outcome ) && ( next_permutation(newperm.begin(),newperm.end()) ) );

  if (outcome) return true; // We have either established a matching for all levels below,
  else return false;        // or we have exhausted all permutations without managing to match.
}

bool test_for_isomorphism( SimpComp *PachnerSphere )
{

  // Given an established matching between the part of the manifold and the internal part
  // of the sphere, we want to test if this matching is an isomorphism, by verifying that
  // all neighbor tables are also equivalent. If two simplices in the internal part of the
  // sphere are neighbors, their coresponding matching simplices in the manifold must also
  // be neighbors. Otherwise the matching is not an isomorphism, and the test fails.

  KSimplex *manifsimp;
  KSimplex *nbmanifsimp;
  PachnerColor *sphcolor;
  PachnerColor *nbsphcolor;

  // Traverse all simplices in the sphere:
  for(auto &level : PachnerSphere->elements){
    for(auto &sphsimp : level){
      // test if the simplex is internal, otherwise skip it
      sphcolor = PachnerColor::find_pointer_to_color(sphsimp);
      if (sphcolor->internalSimplex) {
        // find the matching simplex in the manifold:       
        manifsimp = sphcolor->matchingSimplex;

        // Given the internal sphere simplex, traverse all simplices in its table of neighbors:
        for(auto &nblevel : sphsimp->neighbors->elements){
          for(auto &nbsphsimp : nblevel){
            // test if the neighbor is internal, otherwise skip it
            nbsphcolor = PachnerColor::find_pointer_to_color(nbsphsimp);
            if (nbsphcolor->internalSimplex) {
              // for each neighbor find its match in the manifold:
              nbmanifsimp = nbsphcolor->matchingSimplex;

              // Given the manifold matches of the internal sphere simplex and its neighbor,
              // the two matches must also be neighbors; if they are not, we fail:
              if (!(manifsimp->find_neighbor(nbmanifsimp))) return false;
            }
          }
        }
      }
    }
  }
  // If we reached this point, all neighbor tables in the internal part of the sphere correspond
  // to neighbor tables in the manifold, according to the matching. Therefore, the matching is
  // a proper isomorphism, and we succeed:
  return true;
}

bool perform_the_Pachner_move(SimpComp *simpComp, SimpComp *PachnerSphere)
{
  KSimplex *manifsimp;
  KSimplex *nbmanifsimp;
  PachnerColor *sphcolor;
  PachnerColor *manifcolor;
  PachnerColor *nbsphcolor;

  bool outcome;
  
  // Traverse all simplices in the sphere:
  for(auto &level : PachnerSphere->elements){
    for(auto &sphsimp : level){
      sphcolor = PachnerColor::find_pointer_to_color(sphsimp);
      
      // Step (1): delete all manifold simplices which match exclusively internal sphere simplices.

      // Test if the simplex is internal and not external
      if ( (sphcolor->internalSimplex) && (!(sphcolor->externalSimplex)) ) {
        // find the matching simplex in the manifold, and delete it:
        manifsimp = sphcolor->matchingSimplex;
        simpComp->remove_simplex(manifsimp);
        // disconnect the sphere from the deleted simplex:
        sphcolor->matchingSimplex = nullptr;
        sphcolor->immutable = false; // just to be sure...
      }

      // Step (2): create manifold simplices that correspond to exclusively external sphere simplices,
      // and match them to each other.

      // Test if the simplex is external and not internal
      if ( (sphcolor->externalSimplex) && (!(sphcolor->internalSimplex)) ) {
        // create the corresponding simplex in the manifold
        manifsimp = simpComp->create_ksimplex(sphsimp->k);
        if (manifsimp == nullptr){
          log_report(LOG_ERROR,"Could not create the manifold simplex, something is very wrong, aborting the Pachner move!!");
          return false;
        }
        // colorize the new simplex
        outcome = PachnerColor::colorize_single_simplex(manifsimp);
        if(!outcome){
          log_report(LOG_ERROR,"Unable to colorize the simplex, something is very wrong, aborting the Pachner move!!");
          return false;
        }
        // match the new simplex to the sphere simplex
        manifcolor = PachnerColor::find_pointer_to_color(manifsimp);
        manifcolor->matchingSimplex = sphsimp;
        sphcolor->matchingSimplex = manifsimp;
      }
    }
  }

  // Step (3): fill in the neighbor tables of all new manifold simplices, to match the neighbor
  // tables of the external part of the sphere.
  //
  // Note: in contrast to steps (1) and (2), step (3) cannot be put in the same loop, since neighbor tables
  // may depend on deletion of manifold simplices. So we first delete and create all simplices, and then
  // after that we update the neighbor tables.
  
  // Traverse all simplices in the sphere:
  for(auto &level : PachnerSphere->elements){
    for(auto &sphsimp : level){
      sphcolor = PachnerColor::find_pointer_to_color(sphsimp);

      // Test if the simplex is external and not internal
      if ( (sphcolor->externalSimplex) && (!(sphcolor->internalSimplex)) ) {
        // find the matching simplex in the manifold:       
        manifsimp = sphcolor->matchingSimplex;

        // Given the external sphere simplex, traverse all simplices in its table of neighbors:
        for(auto &nblevel : sphsimp->neighbors->elements){
          for(auto &nbsphsimp : nblevel){
            // for each neighbor find its match in the manifold:
            nbsphcolor = PachnerColor::find_pointer_to_color(nbsphsimp);
            nbmanifsimp = nbsphcolor->matchingSimplex;
            // Match of the neighbor must be a neighbor of the match:
            if( nbmanifsimp != nullptr ) manifsimp->add_neighbor(nbmanifsimp);
          }
        }
      }
    }
  }
  // If we have reached this point, everything went well, Pachner move was successful:
  return true; 
}


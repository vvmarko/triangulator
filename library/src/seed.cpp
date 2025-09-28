
#include "triangulator.hpp"

// ####################################
// Seed functions for various manifolds
// ####################################


std::ostream &operator<<(std::ostream &os, KSimplex &k) { 
	k.print_compact();
    return os;
}

// Connects new vertex v with k-simplices of up to dimension k:
// At each level k, old and new k-simplices are delimited using / sign:
// k=0: 1, 2, 3, / 4.
// k=1: 1-2, 1-3, 2-3, / 1-4, 2-4, 3-4.
// k=2: 1-2-3, / 1-2-4, 1-3-4, 2-3-4.
// k=3: / 1-2-3-4.
KSimplex* build_simplex_one_level_up_with_vertex(SimpComp* simpComp, KSimplex* simpsmall, KSimplex *vertex){
    int k = (simpsmall ? simpsmall->k + 1 : 0);
	set<KSimplex*> s;
	simpsmall->collect_vertices(s);
    s.insert(vertex);

	KSimplex *find = simpComp->find_vertices(s);
	if(find)
		return find;

    // Create big KSimplex that is by 1 dimension higher than simpsmall,
    // and is constructed by appending vertex to simpsmall:
	KSimplex* big = nullptr;
	if(k <= simpComp->D){
		big = simpComp->create_ksimplex(k);
		big->add_neighbor(simpsmall);
		big->add_neighbor(vertex);
	}
	if(k == 1)
		return big;

    // Populate neighbors of simplex at level k
    // by adding new vertex to old k-1 vertices belonging to simpsmall.
    // Store the size of k-1 vertices of simpsmall:
    int oldSize = simpsmall->neighbors->elements[0].size();

    // As vertex V is already added, continue from level 1 to k:
    for(int kTemp = 1; kTemp <= k; kTemp++){ // for each level
        // Before adding k-simplices at level kTemp,
        // store the number of old k-simplices at level kTemp:
        int nextOldSize = simpsmall->neighbors->elements[kTemp].size();
        // Initiate new kTemp-eders based on
        // old kTemp-1-eders and the new vertex v:
        for(int iKSimplex = 0; iKSimplex < oldSize; iKSimplex++){
            // Create temporary KSimplex by appending vertex to one of old k-simplices at level kTemp-1
            KSimplex* newKSimplex = build_simplex_one_level_up_with_vertex(simpComp, simpsmall->neighbors->elements[kTemp-1][iKSimplex], vertex);
			if(k <= simpComp->D)
	        	big->add_neighbor(newKSimplex);
        }
        // For next kTemp, use initial number of k-simplices at level kTemp:
        oldSize = nextOldSize;
    }
    // Return resulting KSimplex* that is simpsmall advanced by vertex:
    return big;
}

KSimplex* build_simplex_one_level_up(SimpComp *simpComp, KSimplex* simpsmall){
    // Seed a KSimplex of level k based on KSimplex of level k-1:
    int k = (simpsmall ? simpsmall->k + 1 : 0);
    // Create a new vertex, i.e. KSimplex(0):
    KSimplex *vertex = simpComp->create_ksimplex(0);
    if(k == 0)
        return vertex;

    // Connect new vertex with k-simplex simpsmall:
    return build_simplex_one_level_up_with_vertex(simpComp, simpsmall, vertex);
}

KSimplex* attach_new_simplex_to_boundary( KSimplex *boundsimp , SimpComp *G){
  // (1) Test for all the conditions that must be satisfied for succesfully attaching a new simplex
  // legality of the complex pointer:
  if(G==nullptr){
    log_report(LOG_ERROR,"The provided complex is nullptr, it does not exist!!");
    return nullptr;
  }
  // legality of the simplex pointer:
    if(boundsimp==nullptr){
    log_report(LOG_ERROR,"The provided simplex is nullptr, it does not exist!!");
    return nullptr;
  }
  // whether this simplex is an element of this complex:
  if(!(G->is_an_element(boundsimp))){
    log_report(LOG_ERROR,"This simplex does not belong to the given complex!!");
    return nullptr;
  }
  // whether the simplex has dimension D-1:
  int D = boundsimp->D;
  int k = boundsimp->k;
  if( k != D-1 ){
    log_report(LOG_ERROR,"This simplex does not have the dimension D-1, so I cannot attach another D-simplex to it!!");
    return nullptr;
  }
  // whether the simplex is part of the boundary of the complex:
  if(boundsimp->is_a_boundary() != true){
    log_report(LOG_ERROR,"This simplex is not on the boundary of the complex, cannot attach another D-simplex to it!!");
    return nullptr;
  }
  
  // (2) If all is well, attach a new D-simplex to this (D-1)-simplex, and verify that it was done
  KSimplex* newsimplex = build_simplex_one_level_up( G , boundsimp );
  if(newsimplex==nullptr){
    log_report(LOG_ERROR,"Something went wrong during the build, the result is nullptr. This should not happen, something is very wrong!!");
    return nullptr;
  }
  
  // (3) Colorize all (D-1)-simplices of the new D-simplex with the BoundaryColor, except for the old (D-1)-simplex
  bool test = true;
  for(auto &simp : newsimplex->neighbors->elements[D-1])
    if (simp!=boundsimp) test = BoundaryColor::colorize_single_simplex(simp);
  if(!test) log_report(LOG_ERROR,"I could not label the boundary of the attached D-simplex properly. This is very very bad, the structure of the complex is inconsistent. Fix your code!!");
  
  // (4) Remove the BoundaryColor from the old (D-1)-simplex
  BoundaryColor::remove_color_from_simplex(boundsimp);
  
  // (5) Finally, return the pointer to the newly built D-simpleks
  return newsimplex;
}


// Worker function for seeding a complex made of a single simplex
SimpComp* seed_single_simplex_worker(int D, string name){
    if(D <= 0){
        log_report(LOG_ERROR, "Not possible to seed for dimension lower or equal to 0");
        return nullptr;
    }
    // Initialize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name,D);
    if(simpComp==nullptr){
      log_report(LOG_PANIC,"PANIC!!! Cannot allocate memory!!!");
      return nullptr;
    }
    simpComp->topology = "linear";
    KSimplex *simpsmall = simpComp->create_ksimplex(0);

    // Progress to further dimensions by adding new vertex and conntecting it:
    for(int k = 1; k <= D; k++){
        // Seed a KSimplex of level k based on KSimplex of level k-1:
        simpsmall = build_simplex_one_level_up(simpComp, simpsmall);
    }

    // Add boundary color to the boundary simplices:
    BoundaryColor::colorize_simplices_at_level(simpComp, D-1);
    
    return simpComp;
}

SimpComp* seed_single_simplex(int D, string name){
    string s = "Seeding a simplicial complex " + name + ", containing a single D-simplex, D = ";
    s += to_string(D) + "...";
    log_report(LOG_INFO, s);
    SimpComp *simpComp = seed_single_simplex_worker(D, name);
    triangulator_global::seededComplexes.push_back(simpComp);
    return simpComp;
}

SimpComp* seed_sphere(int D, string name){
    string s = "Seeding a sphere " + name + ", D = " + to_string(D) + "...";
    log_report(LOG_INFO, s);
    SimpComp *simpComp = seed_sphere_worker(D, name);
    triangulator_global::seededComplexes.push_back(simpComp);
    return simpComp;
}

// Seed a sphere of dimension D, by seeding a single simplex of
// dimension D+1, and deleting its interior:
SimpComp* seed_sphere_worker(int D, string name){
    if(D <= 0){
        log_report(LOG_ERROR, "Not possible to seed for dimension lower or equal to 0");
        return nullptr;
    }
    // Initialize a simplicial complex with a single simplex of dimension D+1:
    SimpComp *simpComp = seed_single_simplex_worker(D+1, name);

    // Rename its topology
    simpComp->topology = "sphere";

    // Delete its single (D+1)-simplex
    simpComp->remove_simplex(simpComp->elements[D+1][0]);

    // Decrease its list of elements from the back
    simpComp->elements.pop_back();

    // Decrease  its dimension
    simpComp->D--;

    // Remove BoundaryColor, since a sphere has no boundary:
    for(auto &kSimplex : simpComp->elements[D]){
        for(auto pColor : kSimplex->colors)
            delete pColor;
        kSimplex->colors.pop_back();
    }

    // For each k-simplex from elements:
    for(unsigned i = 0; i < simpComp->elements.size(); i++){
        for(auto &kSimplex : simpComp->elements[i]){
            // Decrease the length of neighbors->elements:
            kSimplex->neighbors->elements.pop_back();
            // Decrease the dimension info for the neighbors complex:
            kSimplex->neighbors->D--;
            // Decrease the dimension of kSimplex itself:
            kSimplex->D--;
        }
    }

    return simpComp;
}

//Unseed a specific complex - delete it from the memory and update pointers in seededComplexes vector
void unseed_complex(SimpComp *simpComp){
    if(!simpComp){
        log_report(LOG_ERROR, "Not possible to unseed a nullptr complex");
        return;
    }

    unsigned long i = 0; // index of simpComp in seededComplexes
    auto &vec = triangulator_global::seededComplexes;
    // Find simpComp in seededComplexes:
    while( (i < vec.size()) && (vec[i] != simpComp) )
        i++;
    if(i < vec.size()){ // found
        delete vec[i]; // delete it
        // Remove it from vec:
        while(i < vec.size()-1){
	  vec[i] = vec[i+1];
	  i++;
	}
	vec.pop_back();
    }else{ // not found
        log_report(LOG_ERROR, "Not possible to unseed a complex");
    }
}

//Unseed all complexes - delete them from the memory and empty seededComplexes vector
void unseed_everything(){
    auto &vec = triangulator_global::seededComplexes;
    // Free memory occupied by each seeded complex:
    while(vec.size() > 0) unseed_complex(vec[0]);
    // This is probably an overkill:
    vec.clear();
}



#include "triangulator.hpp"

// ####################################
// Seed functions for various manifolds
// ####################################

/*
// For a given level k, function seed_KSimplices:
// Seeds a KSimplex of level k-1
// Add edges between 1 new vertex and each of old vertices
// Add triangles between each 2 new edges and each of old edges
// Add tethraeder between each 3 new triangles and each of old triangles
// ...
// Add k-aeder between each k-1 new k-1-eders and each of old k-1-eders
void seed_KSimplices(SimpComp* simpComp, int k){
    if(!k){
        // If k == 0, create a KSimplex(0):
        KSimplex *v = simpComp->create_ksimplex(0);
Color *c = new(nothrow) UniqueIDColor();
v->colors.push_back(c);
cout << "+addV0+" << endl;
        return;
    }
    // Seed a KSimplex of level k-1:
cout << "++++ Seeding seed_KSimplices " << k-1 << " starting" << endl;
    seed_KSimplices(simpComp, k-1);
cout << "---- Seeding seed_KSimplices " << k-1 << " finished" << endl;
    
    // Make a copy of old simpComp, 
    // so that old vertices, edges, triangles,... can be found:
    SimpComp *original = new(nothrow) SimpComp(*simpComp);
    // Adding vertex:
    KSimplex *v = simpComp->create_ksimplex(0);
Color *c = new(nothrow) UniqueIDColor();
v->colors.push_back(c);
cout << "+addV" << k << "+" << endl;
    // new k-1-eders:
    std::vector<KSimplex*> newKSimplices;
    newKSimplices.push_back(v); // adds
    // Once nRemove new KSimplices from level 0 are parsed,
    // and nAdded edges added,
    // remove nRemove of them, and consider as new only nAdded ones:
    size_t nRemove = 1;
    size_t nAdded = 0;
    for(int kTemp = 1; kTemp <= k; kTemp++){ // for each level
cout << "kTemp = " << kTemp << endl;
        // Find each set of kTemp simplices from all newKSimplices
        // Initially, first kTemp newKSimplices will be used.
        // Inidces will hold their kTemp positions.
        // Example for kTemp = 3, where used newKSimplices are marked with k:
        // kNewKSimplices:       k k k - - - - ... - - - -
        // includeNewKSimplices: 0 1 2
        // Indices will vary 
        // (by increasing last index that can be increased
        // and updating later ones to follow) until the end:
        // kNewKSimplices:       - - - - ... - - - - k   k  k
        // includeNewKSimplices:                    n-2 n-1 n
        // Initiate old k-1-eders to existing ones:
        std::vector<KSimplex*> oldKSimplices;
        for(auto &oldKSimplex : original->elements[kTemp-1])
            oldKSimplices.push_back(oldKSimplex);
        // Initially, first kTemp newKSimplices will be used:
        std::vector<size_t> includeNewKSimplices;
        for(int i = 0; i < kTemp; i++)
            includeNewKSimplices.push_back(i);
        
        // Indices will vary until the end:
        do{
            for(auto &oldKSimplex : oldKSimplices){
                // Add k-aeder:
                KSimplex *newKSimplex = simpComp->create_ksimplex(kTemp);
cout << "+add+" << endl;
Color *c = new(nothrow) UniqueIDColor();
newKSimplex->colors.push_back(c);
                newKSimplices.push_back(newKSimplex);
                nAdded++;
                for(auto &it : includeNewKSimplices){
                    newKSimplex->add_neighbor(newKSimplices[it]); //TODO: ADD ONLY COMMON NEIGHBOR
cout << "^";
                }
//                KSimplex *oldSimplex = find_common(oldKSimplices, newKSimplices, includeNewKSimplices);
                newKSimplex->add_neighbor(oldKSimplex);
            }
for(size_t m = 0; m < includeNewKSimplices.size(); m++)
    cout << "m" << includeNewKSimplices[m] << " ";
cout << " // newKSimplices.size()-nAdded = " << newKSimplices.size()-nAdded << endl;
            // Find last index that can be incremented and increment it
            // (do not count recently added ones, i.e. reduce by nAdded):
            if(includeNewKSimplices[ includeNewKSimplices.size() - 1 ] < newKSimplices.size() - nAdded - 1){
                includeNewKSimplices[ includeNewKSimplices.size() - 1 ]++;
            }else{
                int i = includeNewKSimplices.size() - 2;
                while( (i >= 0) && (includeNewKSimplices[i] + 1 == includeNewKSimplices[i+1]) )
                    i--;
                if(i >= 0){
                    // Increment the last index that can be incremented:
                    includeNewKSimplices[i]++;
                    // Set later indices right after:
                    int tempI = includeNewKSimplices[i] + 1;
                    for(size_t j = i+1; j < includeNewKSimplices.size(); j++)
                        includeNewKSimplices[j] = tempI++;
                }
            }
        }while(includeNewKSimplices[0] != newKSimplices.size() -nAdded - kTemp);
        newKSimplices.erase( newKSimplices.begin(), newKSimplices.begin() + nRemove );
        nRemove = nAdded;
        nAdded = 0;
    }
    // Delete vector of pointers,
    // but not KSimplex-es they point to:
//    original->elements.clear();
//    delete original; // save to delete the rest
}*/




std::ostream &operator<<(std::ostream &os, KSimplex &k) { 
	k.print_compact();
    return os;
}

// Function seed_single_simplex_advance_vertex
// connects new vertex v with k-simplices of up to dimension k:
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
  test = true;
  test = BoundaryColor::remove_color_from_simplex(boundsimp);
  if(!test) log_report(LOG_ERROR,"I could not remove the boundary label of the old (D-1)-simplex properly. This is very very bad, the structure of the complex is inconsistent. Fix your code!!");
  
  // (5) Finally, return the pointer to the newly built D-simpleks
  return newsimplex;
}


// Seed a single simplex or sphere of dimension d:
SimpComp* seed_single_simplex_or_sphere(int D, int sphere, string name){
    string s = "Creating general ";
    s += (sphere ? "sphere" : "simplicial complex");
    s += ", D = " + to_string(D) + "...";
    log_report(LOG_INFO, s);
    if(D <= 0){
        log_report(LOG_ERROR, "Not possible to seed for dimension lower or equal to 0");
        return nullptr;
    }
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name,D);
    if(simpComp==nullptr){
      log_report(LOG_PANIC,"PANIC!!! Cannot allocate memory!!!");
      return nullptr;
    }
    simpComp->topology = sphere ? "sphere" : "linear";
    KSimplex *simpsmall = simpComp->create_ksimplex(0);

    // Progress to further dimensions by adding new vertex and conntecting it:
    for(int k = 1; k <= D+sphere; k++){
        // Seed a KSimplex of level k based on KSimplex of level k-1:
        simpsmall = build_simplex_one_level_up(simpComp, simpsmall);
    }

    // If seeding simplicial complex, add boundary color:
    if(!sphere) BoundaryColor::colorize_simplices_at_level(simpComp, D-1);
    
    return simpComp;
}

SimpComp* seed_single_simplex(int D, string name){
    SimpComp *simpComp = seed_single_simplex_or_sphere(D, 0, name);
    triangulator_global::seededComplexes.push_back(simpComp);
    return simpComp;
}

SimpComp* seed_sphere(int D, string name){
    SimpComp *simpComp = seed_single_simplex_or_sphere(D, 1, name);
    triangulator_global::seededComplexes.push_back(simpComp);

cout << "Seed: " << triangulator_global::seededComplexes.size() << endl;

    return simpComp;
}

// Seed a single sphere of dimension d
// by seeding a simplex of dimension d+1, and deleting only it:
SimpComp* seed_sphere_intuitively(int D, string name){
    string s = "Creating general sphere " + name + ", D = " + to_string(D) + "...";
    log_report(LOG_INFO, s);
    if(D <= 0){
        log_report(LOG_ERROR, "Not possible to seed for dimension lower or equal to 0");
        return nullptr;
    }
    // Initilize simplicial complex of dimension D+1, and an empty k-simplex:
    SimpComp *simpComp = seed_single_simplex(D+1, name);
    simpComp->name = name;
    simpComp->topology = "sphere";

    // Delete the last created k-simplex after disconnecting neighbors:
    simpComp->remove_simplex(simpComp->elements[D+1][0]);
    // Remove BoundaryColor, as sphere doesn't have them:
    for(auto &kSimplex : simpComp->elements[D]){
        for(auto pColor : kSimplex->colors)
            delete pColor;
        kSimplex->colors.pop_back();
    }

    // For each k-simplex from elements:
    for(unsigned i = 0; i < simpComp->elements.size()-1; i++){
        for(auto &kSimplex : simpComp->elements[i]){
            // Decrease the length of matrix kSimplex->neighbors->elements:
            // kSimplex->neighbors->elements[D+1].empty(); // already disconnected
            kSimplex->neighbors->elements.pop_back();

            // Decrease the dimension of kSimplex:
            kSimplex->D--;
        }
    }

    // Decrease the length of matrix elements:
    simpComp->elements.pop_back();

    // Decrease the dimension of simpComp:
    simpComp->D--;

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
    for(auto &it : vec)
        if(it)
            delete it;
    vec.clear(); // delete all pointers
}


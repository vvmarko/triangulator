
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
Color *c = new UniqueIDColor();
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
    SimpComp *original = new SimpComp(*simpComp);
    // Adding vertex:
    KSimplex *v = simpComp->create_ksimplex(0);
Color *c = new UniqueIDColor();
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
Color *c = new UniqueIDColor();
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


/*
    e2 = tetrahedron->create_ksimplex(1);
    e3 = tetrahedron->create_ksimplex(1);
  ...
    // Add triangles (k-simplices, k=2)
    trianglev1v2v3 = tetrahedron->create_ksimplex(2);
    trianglev1v3v4 = tetrahedron->create_ksimplex(2);
...
    // Add tetrahedron (k-simplex, k=3)
    tetrahedron1 = tetrahedron->create_ksimplex(3);

    // Edges have vertices as neighbors and the opposite:
    e1->add_neighbor(v1);
    e1->add_neighbor(v2);
    e2->add_neighbor(v1);
    e2->add_neighbor(v3);
...
    // Triangles have edges (and vertices) as neighbors and the opposite:
    trianglev1v2v3->add_neighbor(e1);
...
    trianglev1v2v4->add_neighbor(e4);

    tetrahedron1->add_neighbor(trianglev1v2v3);
    tetrahedron1->add_neighbor(trianglev1v3v4);
...
*/

std::ostream &operator<<(std::ostream &os, KSimplex &k) { 
	k.print_compact();
    return os;
}

KSimplex* build_KSimplex(SimpComp* simpComp, set<KSimplex*> &s){
	// Create new k-simplex of the level that is equal to s.size():
	KSimplex* newKSimplex = simpComp->create_ksimplex(s.size()-1);
	// Add all vertices to newKSimplex:
	for(auto &it : s)
		it->add_neighbor(newKSimplex);
	newKSimplex->reconstruct_neighbors_from_vertices();
	return newKSimplex;
}

// Function seed_single_simplex_advance_vertex
// connects new vertex v with k-simplices of up to dimension k:
// At each level k, old and new k-simplices are delimited using / sign:
// k=0: 1, 2, 3, / 4.
// k=1: 1-2, 1-3, 2-3, / 1-4, 2-4, 3-4.
// k=2: 1-2-3, / 1-2-4, 1-3-4, 2-3-4.
// k=3: / 1-2-3-4.
KSimplex* build_simplex_one_level_up_with_vertex(SimpComp* simpComp, int k, KSimplex* small, KSimplex *vertex){
	set<KSimplex*> s;
	small->collect_vertices(s);
    s.insert(vertex);

	KSimplex *find = simpComp->find_vertices(s);
	if(find)
		return find;

    // Create big KSimplex that is by 1 dimension higher than small,
    // and is constructed by appending vertex to small:
	KSimplex* big = nullptr;
	if(k <= simpComp->D){
		big = simpComp->create_ksimplex(k);
		big->add_neighbor(small);
		big->add_neighbor(vertex);
	}
	if(k == 1)
		return big;

    // Populate neighbors of simplex at level k
    // by adding new vertex to old k-1 vertices belonging to small.
    // Store the size of k-1 vertices of small:
    int oldSize = small->neighbors->elements[0].size();

    // As vertex V is already added, continue from level 1 to k:
    for(int kTemp = 1; kTemp <= k; kTemp++){ // for each level
        // Before adding k-simplices at level kTemp,
        // store the number of old k-simplices at level kTemp:
        int nextOldSize = small->neighbors->elements[kTemp].size();
        // Initiate new kTemp-eders based on
        // old kTemp-1-eders and the new vertex v:
        for(int iKSimplex = 0; iKSimplex < oldSize; iKSimplex++){
            // Create temporary KSimplex by appending vertex to one of old k-simplices at level kTemp-1
            KSimplex* newKSimplex = build_simplex_one_level_up_with_vertex(simpComp, kTemp, small->neighbors->elements[kTemp-1][iKSimplex], vertex);
			if(k <= simpComp->D)
	        	big->add_neighbor(newKSimplex);
        }
        // For next kTemp, use initial number of k-simplices at level kTemp:
        oldSize = nextOldSize;
    }
    // Return resulting KSimplex* that is small advanced by vertex:
    return big;
}

KSimplex* build_simplex_one_level_up(SimpComp *simpComp, int k, KSimplex* small){
    // Seed a KSimplex of level k based on KSimplex of level k-1:
    // Create a new vertex, i.e. KSimplex(0):
    KSimplex *vertex = simpComp->create_ksimplex(0);
    if(k == 0)
        return vertex;

    // Connect new vertex with k-simplex small:
    return build_simplex_one_level_up_with_vertex(simpComp, k, small, vertex);
}

// Seed a single simplex or sphere of dimension d:
SimpComp* seed_single_simplex_or_sphere(int D, int sphere){
    string s = "Creating general ";
    s += (sphere ? "sphere" : "simplicial complex");
    s += ", D = " + to_string(D) + "...";
    log_report(LOG_INFO, s);
    if(D < 0){
        log_report(LOG_ERROR, "Not possible to seed for dimension lower than 0");
        return nullptr;
    }
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new SimpComp(D);
    KSimplex *small = nullptr;

    // Progress to further dimensions by adding new vertex and conntecting it:
    for(int k = 0; k <= D+sphere; k++){
        // Seed a KSimplex of level k based on KSimplex of level k-1:
        small = build_simplex_one_level_up(simpComp, k, small);
    }

    // If seeding simplicial complex, add boundary color:
	if(!sphere)
		for(auto &kSimplex : simpComp->elements[simpComp->D-1])
			kSimplex->colors.push_back(new BoundaryColor(true));
    
    return simpComp;
}

SimpComp* seed_single_simplex(int D){
    return seed_single_simplex_or_sphere(D, 0);
}

SimpComp* seed_sphere(int D){
    return seed_single_simplex_or_sphere(D, 1);
}

// Seed a single sphere of dimension d
// by seeding a simplex of dimension d+1, and deleting only it:
SimpComp* seed_sphere_intuitively(int D){
    string s = "Creating general sphere, D = " + to_string(D) + "...";
    log_report(LOG_INFO, s);
    if(D < 0){
        log_report(LOG_ERROR, "Not possible to seed for dimension lower than 0");
        return nullptr;
    }
    // Initilize simplicial complex of dimension D+1, and an empty k-simplex:
    SimpComp *simpComp = new SimpComp(D+1);
    KSimplex *small = nullptr;

    // Progress to further dimensions by adding new vertex and conntecting it:
    for(int k = 0; k <= D+1; k++){
        // Seed a KSimplex of level k based on KSimplex of level k-1:
        small = build_simplex_one_level_up(simpComp, k, small);
    }

    // Delete the last created k-simplex after disconnecting neighbors:
    simpComp->remove_simplex(simpComp->elements[D+1][0]);

    // For each k-simplex from elements:
    for(unsigned i = 0; i < simpComp->elements.size()-1; i++){
        for(auto &kSimplex : simpComp->elements[i]){
            // Decrease the length of matrix kSimplex->neighbors->elements:
            //kSimplex->neighbors->elements[D+1].empty(); // already disconnected
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
			KSimplex* &small = simpComp->elements[kTemp-1][i];
			// Append vertex to it, and save it at current level:
			build_simplex_one_level_up_with_vertex(simpComp, kTemp, small, vertex);
		}
		// Update the old length from current level:
		oldLength = nextLength;
	}
	// Delete initial k-simplex from level D:
	simpComp->remove_simplex(simpComp->elements[simpComp->D][0]);
	
	return vertex;
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



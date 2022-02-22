
#include "triangulator.hpp"

// ####################################
// Seed functions for various manifolds
// ####################################

SimpComp* seed_single_edge(string name){
    string s = "Creating simplicial complex - single edge: " + name + "...";
    log_report(LOG_INFO, s);
    
    // Initilize KComplex of dimension 1:
    SimpComp *edge = new SimpComp(name, 1);

    // Add 2 vertices (k=0) of the edge:
    KSimplex *v1 = edge->create_ksimplex(0);
    KSimplex *v2 = edge->create_ksimplex(0);

    // Create KSimplex (k=1) for the edge:
    KSimplex *e1 = edge->create_ksimplex(1);

    // Connect KSimplex-es:

    // Edge has both vertices as neighbors and the opposite:
    v1->add_neighbor(e1);
    v2->add_neighbor(e1);

    // Create and apply colors:
    Color *c1 = new BoundaryColor(true);
    Color *c2 = new BoundaryColor(true);
    v1->colors.push_back(c1);
    v2->colors.push_back(c2);

    return edge;
}

SimpComp* seed_single_triangle(string name){
    string s = "Creating simplicial complex - single triangle: " + name + "...";
    log_report(LOG_INFO, s);
    
    // Initilize KComplex of dimension 1:
    SimpComp *triangle = new SimpComp(name, 2);

    // Add 3 vertices (k=0) of the triangle:
    KSimplex *v1 = triangle->create_ksimplex(0);
    KSimplex *v2 = triangle->create_ksimplex(0);
    KSimplex *v3 = triangle->create_ksimplex(0);

    // Add 3 edges (k=1) of the triangle:
    KSimplex *e1 = triangle->create_ksimplex(1);
    KSimplex *e2 = triangle->create_ksimplex(1);
    KSimplex *e3 = triangle->create_ksimplex(1);

    // Create KSimplex (k=2) for the triangle:
    KSimplex *t1 = triangle->create_ksimplex(2);

    // Add vertices as neighbors of edges and the opposite:
    e1->add_neighbor(v1);
    e1->add_neighbor(v2);
    e2->add_neighbor(v1);
    e2->add_neighbor(v3);
    e3->add_neighbor(v2);
    e3->add_neighbor(v3);

    // Add edges (and their vertices) as neighbors of the triangle:
    t1->add_neighbor(e1);
    t1->add_neighbor(e2);
    t1->add_neighbor(e3);

    // Create and apply boundary colors:
    Color *c1 = new BoundaryColor(true);
    Color *c2 = new BoundaryColor(true);
    Color *c3 = new BoundaryColor(true);
    e1->colors.push_back(c1);
    e2->colors.push_back(c2);
    e3->colors.push_back(c3);

    // Create and apply unique ID colors:
    c1 = new UniqueIDColor();
    c2 = new UniqueIDColor();
    c3 = new UniqueIDColor();
    v1->colors.push_back(c1);
    v2->colors.push_back(c2);
    v3->colors.push_back(c3);
    c1 = new UniqueIDColor();
    c2 = new UniqueIDColor();
    c3 = new UniqueIDColor();
    e1->colors.push_back(c1);
    e2->colors.push_back(c2);
    e3->colors.push_back(c3);
    c1 = new UniqueIDColor();
    t1->colors.push_back(c1);

    return triangle;
}

SimpComp* seed_single_tetrahedron(string name)
{
    KSimplex *v1, *v2, *v3, *v4;
    KSimplex *e1, *e2, *e3, *e4, *e5, *e6;
    KSimplex *trianglev1v2v3;
    KSimplex *trianglev1v3v4;
    KSimplex *trianglev1v2v4;
    KSimplex *trianglev2v4v3;
    KSimplex *tetrahedron1;

    // Initialize simplicial complex of dimension 3
    SimpComp *tetrahedron = new SimpComp(name, 3);

    // Vertex v1 is located up
    // Vertex v2 is located left
    // Vertex v4 is located right
    // Vertex v3 is located down
    // Add vertices (k-simplices, k=0)
    v1 = tetrahedron->create_ksimplex(0);
    v2 = tetrahedron->create_ksimplex(0);
    v3 = tetrahedron->create_ksimplex(0);
    v4 = tetrahedron->create_ksimplex(0);

    // Add edges (k-simplices, k=1)
    e1 = tetrahedron->create_ksimplex(1);
    e2 = tetrahedron->create_ksimplex(1);
    e3 = tetrahedron->create_ksimplex(1);
    e4 = tetrahedron->create_ksimplex(1);
    e5 = tetrahedron->create_ksimplex(1);
    e6 = tetrahedron->create_ksimplex(1);

    // Add triangles (k-simplices, k=2)
    trianglev1v2v3 = tetrahedron->create_ksimplex(2);
    trianglev1v3v4 = tetrahedron->create_ksimplex(2);
    trianglev1v2v4 = tetrahedron->create_ksimplex(2);
    trianglev2v4v3 = tetrahedron->create_ksimplex(2);

    // Add tetrahedron (k-simplex, k=3)
    tetrahedron1 = tetrahedron->create_ksimplex(3);

    // Edges have vertices as neighbors and the opposite:
    e1->add_neighbor(v1);
    e1->add_neighbor(v2);
    e2->add_neighbor(v1);
    e2->add_neighbor(v3);
    e3->add_neighbor(v1);
    e3->add_neighbor(v4);
    e4->add_neighbor(v2);
    e4->add_neighbor(v4);
    e5->add_neighbor(v2);
    e5->add_neighbor(v3);
    e6->add_neighbor(v3);
    e6->add_neighbor(v4);

    // Triangles have edges (and vertices) as neighbors and the opposite:
    trianglev1v2v3->add_neighbor(e1);
    trianglev1v2v3->add_neighbor(e2);
    trianglev1v2v3->add_neighbor(e5);

    trianglev1v3v4->add_neighbor(e2);
    trianglev1v3v4->add_neighbor(e3);
    trianglev1v3v4->add_neighbor(e6);

    trianglev1v2v4->add_neighbor(e1);
    trianglev1v2v4->add_neighbor(e3);
    trianglev1v2v4->add_neighbor(e4);

    trianglev2v4v3->add_neighbor(e4);
    trianglev2v4v3->add_neighbor(e6);
    trianglev2v4v3->add_neighbor(e5);

    // Tetrahedron has triangles (and their lower level neighbors) 
    // as neighbors and the opposite:
    tetrahedron1->add_neighbor(trianglev1v2v3);
    tetrahedron1->add_neighbor(trianglev1v3v4);
    tetrahedron1->add_neighbor(trianglev1v2v4);
    tetrahedron1->add_neighbor(trianglev2v4v3);

    // Triangles have boundary color
    Color *c1 = new BoundaryColor(true);
    Color *c2 = new BoundaryColor(true);
    Color *c3 = new BoundaryColor(true);
    Color *c4 = new BoundaryColor(true);
    trianglev1v2v3->colors.push_back(c1);
    trianglev1v3v4->colors.push_back(c2);
    trianglev1v2v4->colors.push_back(c3);
    trianglev2v4v3->colors.push_back(c4);

    return tetrahedron;
}

// Function seed_KSimplices for level k does:
// Seed a KSimplex of level k-1
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
cout << "+addV+" << endl;

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
/*    // Delete vector of pointers,
    // but not KSimplex-es they point to:
    original->elements.clear();
    delete original; // save to delete the rest
*/
}

SimpComp* seed_single_SimpComp(string name, int d){
    string s = "Creating general simplicial complex " + name 
            + ", d = " + to_string(d) + "...";
    log_report(LOG_INFO, s);

    if(d < 0){
        log_report(LOG_ERROR, "Not possible to seed simplicial"
          "complex of dimension lower than 0");
        return nullptr;
    }
    
    // Initilize KComplex of dimension d:
    SimpComp *simpComp = new SimpComp(d);

    seed_KSimplices(simpComp, d);

    return simpComp;
}

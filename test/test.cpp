
#define TEST_FAILED_IF(condition, errorMessage) { \
  if(condition){ \
    cout << endl << "Error in " << __FUNCTION__ << ":" << endl; \
    test_failed(errorMessage); \
    return; \
  } \
}

void test_ok(string s = ""){
    if(s != "")
        cout << "Test " << s << " passed." << endl;
    else
        cout << "Ok." << endl;
}

void test_failed(string s = ""){
    if(s != "")
        cout << "Failed : " << s << "!" << endl << endl;
    else
        cout << "Failed!" << endl << endl;
}

// Test default constructor
void test_kSimplex_default_constructor(){
    KSimplex *k = new KSimplex(3, 2);

    TEST_FAILED_IF(!k, "KSimplex not constructed");

    TEST_FAILED_IF(k->k != 3, "KSimplex level not assigned well");

    TEST_FAILED_IF(k->D != 2, "KSimplex dimension not assigned well");

    TEST_FAILED_IF(!k->neighbors, "KSimplex neighbors not constructed");
    
    test_ok("KSimplex constructor");

    delete k;
}

// Test default destructor
void test_kSimplex_default_destructor(){
    KSimplex *k = new KSimplex(3, 2);
    k->~KSimplex(); // Explicit call to destructor

    if(k->colors.size()){
        test_failed("KSimplex colors not deleted");
        return;
    }

    test_ok("KSimplex destructor");
}

// Test adding neighbor as vertices and collecting vertices
void test_kSimplex_collect_vertices(){
    KSimplex *k = new KSimplex(0, 5);
    set<KSimplex*> s;
    k->collect_vertices(s);

    TEST_FAILED_IF(s.size() == 0, "KSimplex not adding itself in a set of vertices");

    auto it = s.begin();
    TEST_FAILED_IF(*it != k, "KSimplex not adding itself properly in a set of vertices");

    delete k;
    s.clear();

    k = new KSimplex(1, 5);
    KSimplex *kNeigh = new KSimplex(0, 5);
    k->add_neighbor(kNeigh);
    k->collect_vertices(s);
    it = s.begin();

    TEST_FAILED_IF(*it != kNeigh, "KSimplex not adding properly in a set of vertices");
    
    test_ok("KSimplex collect vertices");

    delete k;
    //delete kNeigh;
}

// Test finding neighbors
void test_kSimplex_find_neighbor(){
    KSimplex *k = new KSimplex(1, 5);
    KSimplex *kNeigh1 = new KSimplex(0, 5);
    KSimplex *kNeigh2 = new KSimplex(0, 5);
    k->add_neighbor(kNeigh1);
    k->add_neighbor(kNeigh2);

    TEST_FAILED_IF(!k->find_neighbor(kNeigh1), "KSimplex neighbor search not working");

    TEST_FAILED_IF(!k->find_neighbor(kNeigh2), "KSimplex neighbor search not working");
    

    test_ok("KSimplex find neighbors");

    delete k;
    //delete kNeigh1;
    //delete kNeigh2;
}

// Test getting uniqueID of a single color assigned to KSimplex
void test_kSimplex_get_uniqueID(){
    KSimplex *k = new KSimplex(2, 3);
    Color *c = new UniqueIDColor();
    k->colors.push_back(c);
    UniqueIDColor* uniqueC = k->get_uniqueID();

    TEST_FAILED_IF(c != uniqueC, "KSimplex get UniqueID not working");

    test_ok("KSimplex get UniqueID");

    UniqueIDColor::next_free_uid_number--;
    delete k;
}

void test_printing(){
    SimpComp *simpCompTest = seed_sphere(1, "1-sfera");
    UniqueIDColor::colorize_entire_complex(simpCompTest);

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    triangulator_global::seededComplexes[0]->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );

    string expectedStr = R""""(Printing SimpComp dvatet, topology = linear, D = 2
  Printing SimpComp dvatet elements, level = 0:
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , 
Simplices at level k = 2:   
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , 
Simplices at level k = 2:   
  Printing SimpComp dvatet elements, level = 1:
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
  Printing SimpComp dvatet elements, level = 2:
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
)"""";

    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "SimpComp printing");

    test_ok("SimpComp printing");

    //delete simpCompTest; TODO: FIX
}










void test_build_simplex_one_level_up(){
    string name = "test";
    int D = 2;
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name, D);

    KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    
    
    string expectedStr = R""""(Printing KSimplex:  k = 1, D = 2
Printing colors:
  Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    newKSimplex->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_build_simplex_one_level_up");

    test_ok("test_build_simplex_one_level_up");
}

void test_attach_new_simplex_to_boundary(){

    int D = 2;

    SimpComp *dvatet = seed_single_simplex(D, "dvatet");
    
    KSimplex *simpdvatet = dvatet->elements[D-1][0];
    
    attach_new_simplex_to_boundary(simpdvatet, dvatet);

    //    UniqueIDColor::colorize_entire_complex(dvatet);
    //    dvatet->print_detailed();
    
    //cout << endl << "Prvi:" << endl;
    Pachner_move(dvatet->elements[0][2], dvatet);
    //cout << endl << "Drugi:" << endl;
    Pachner_move(dvatet->elements[1][1], dvatet);
    //cout << endl << "Treci:" << endl;
    Pachner_move(dvatet->elements[2][0], dvatet);
    //cout << endl << "Cetvrti:" << endl;    

    string expectedStr = R""""(Printing KSimplex:  k = 1, D = 2
Printing colors:
  Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    simpdvatet->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_attach_new_simplex_to_boundary");

    test_ok("test_attach_new_simplex_to_boundary");
}

void test_seed_single_simplex(){

    int D = 1;
    string name = "Test";
    
    // Initilize simplicial complex of dimension D+1, and an empty k-simplex:
    SimpComp *simpComp = seed_single_simplex(D+1, name);

    string expectedStr = R""""(Printing SimpComp Test, topology = linear, D = 2
  Printing SimpComp Test elements, level = 0:
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , 
Simplices at level k = 2:   
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , 
Simplices at level k = 2:   
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , 
Simplices at level k = 2:   
  Printing SimpComp Test elements, level = 1:
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
Boundary = true
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex
  Printing SimpComp Test elements, level = 2:
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    simpComp->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_seed_single_simplex");

    test_ok("test_seed_single_simplex");
}



void test_seed_sphere(){

    int D = 2;

    SimpComp *sphere = seed_sphere(D, "Test sphere");
    if(sphere==nullptr){
        log_report(LOG_ERROR,"Unable to seed a sphere, nullptr generated, aborting test!");
    }

    string expectedStr = R""""(Printing SimpComp Test sphere, topology = sphere, D = 2
  Printing SimpComp Test sphere elements, level = 0:
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
  Printing SimpComp Test sphere elements, level = 1:
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
  Printing SimpComp Test sphere elements, level = 2:
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    sphere->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_seed_sphere");

    test_ok("test_seed_sphere");

}

void test_seed_sphere_intuitively(){

    int D = 2;

    SimpComp *sphere = seed_sphere_intuitively(D, "Test sphere");
    if(sphere==nullptr){
        log_report(LOG_ERROR,"Unable to seed a sphere, nullptr generated, aborting test!");
    }

    string expectedStr = R""""(Printing SimpComp Test sphere, topology = sphere, D = 2
  Printing SimpComp Test sphere elements, level = 0:
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
    Printing KSimplex:  k = 0, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   , , 
Simplices at level k = 2:   , , 
  Printing SimpComp Test sphere elements, level = 1:
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
    Printing KSimplex:  k = 1, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   Simplex, Simplex
  Printing SimpComp Test sphere elements, level = 2:
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
    Printing KSimplex:  k = 2, D = 2
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex, Simplex
Simplices at level k = 1:   Simplex, Simplex, Simplex
Simplices at level k = 2:   
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    sphere->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_seed_sphere_intuitively");

    test_ok("test_seed_sphere_intuitively");
}


void test_unseed_complex(){
    //string name = "test";
    //int D = 2;

    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    //SimpComp *simpComp = new(nothrow) SimpComp(name,D);

    //KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    //KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    

    //newKSimplex->print();




}


void test_unseed_everything(){


//    TEST_FAILED_IF(!k, "KSimplex not constructed");


}





// Collect pointers to my neighboring vertices (zero-level neighbors) into a set
// (if I am a vertex myself, but put me into the set --- this is convenient
// for vertices, despite that no simplex is a neighbor of itself)
void test_KSimplex_collect_vertices(){
    set<KSimplex*> s;

    int D = 2;

    SimpComp *sphere = seed_sphere_intuitively(D, "Test sphere");
    if(sphere==nullptr){
        log_report(LOG_ERROR,"Unable to seed a sphere, nullptr generated, aborting test!");
    }

    UniqueIDColor::colorize_simplices_at_level(sphere, 0);
     
    //sphere->print();
    
    // Otherwise, collect neighboring vertices into a set:
    sphere->elements[2][0]->collect_vertices(s);

    TEST_FAILED_IF(3 != s.size(), "test_seed_sphere_intuitively");

    string expectedStr = R""""(Printing KSimplex:  k = 0, D = 2
Printing colors:
  Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   (7-8), (7-9), (7-10)
Simplices at level k = 2:   (7-8-9), (7-8-10), (7-9-10)
Printing KSimplex:  k = 0, D = 2
Printing colors:
  Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   (7-8), (8-9), (8-10)
Simplices at level k = 2:   (7-8-9), (7-8-10), (8-9-10)
Printing KSimplex:  k = 0, D = 2
Printing colors:
  Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   (7-9), (8-9), (9-10)
Simplices at level k = 2:   (7-8-9), (7-9-10), (8-9-10)
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    for(auto &it : s)
        it->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_KSimplex_collect_vertices");

    test_ok("test_KSimplex_collect_vertices");
}



// Verify if a given simplex is my neighbor
// (true if it is found in my neighbor list, false otherwise)
void test_KSimplex_find_neighbor(){
    set<KSimplex*> s;

    int D = 2;

    SimpComp *sphere = seed_sphere_intuitively(D, "Test sphere");
    if(sphere==nullptr){
        log_report(LOG_ERROR,"Unable to seed a sphere, nullptr generated, aborting test!");
    }

    UniqueIDColor::colorize_simplices_at_level(sphere, 0);
     
    TEST_FAILED_IF(sphere->elements[1][0]->find_neighbor(sphere->elements[1][1]), "test_KSimplex_find_neighbor");

    test_ok("test_KSimplex_find_neighbor");
}


// #####################################
// Functions for manipulating neighbors:
// #####################################

// Establish a neighborhood relationship between a given simplex and myself
// NOTE: This is a powerful function, that does a lot of things:
//  - it adds the simplex into my list of neighbors (if it is not there already)
//  - it adds myself to the list of neighbors of that simplex (if I am not there already)
//  - if my level is smaller than the level of that simplex, add all my subsimplices as its neighbors
//  - if its level is smaller than mine, add all of its subsimplices as my neighbors
void test_KSimplex_add_neighbor(){
    // tested with:
    //test_kSimplex_find_neighbor();
  
}

// Disconnect a given kSimplex from my list of neighbors, and disconnect me from his list
// NOTE: This function is *not* a full inverse of add_neighbor(), since it does not
// disconnect the sub-neighbors of the two simplices (and it should not!)
void test_KSimplex_delete_neighbor(){
}

// Disconnect all simplices from my list of neighbors, and disconnect me from everyone else's lists
// NOTE: This function calls delete_neighbor() for all simplices in my list of neighbors, so
// the same caveats apply here as for the above function
void test_KSimplex_delete_all_neighbors(){
}

// Reconstruct my neighbors table if my neighbor vertices are known
// NOTE: This function traverses the entire given complex, and adds as my neighbors all
// other simplices which have compatible vertex structure. Namely, it finds my
// sub-neighbor if all vertices of a given simplex are a subset of my simplices, and it
// finds my super-neighbor if all my vertices are a subset of a given simplex.
// NOTE: It is important to emphasize that this function implicitly assumes that
// each simplex in the complex has its vertices already assigned in its appropriate
// list of neighbors, otherwise it will fail to find some (or all) neighbors, and in
// addition it may also find false-positives, and assign neighbors that should not be
// neighbors. The assumption about vertices is semantic (it depends on the type of the
// complex in question) and thus it is impossible to verify. Therefore, you have to
// make sure yourself that this assumption is satisfied for your complex, before
// calling this function.
void test_KSimplex_reconstruct_neighbors_from_vertices(){
    set<KSimplex*> s;

    int D = 2;

    SimpComp *sphere = seed_sphere_intuitively(D, "Test sphere");
    if(sphere==nullptr){
        log_report(LOG_ERROR,"Unable to seed a sphere, nullptr generated, aborting test!");
    }

    UniqueIDColor::colorize_simplices_at_level(sphere, 0);

    //sphere->print();
     
    sphere->elements[0][0]->delete_all_neighbors();

    //sphere->print();
     
    //reconstruct_neighbors_from_vertices();
    
    //TEST_FAILED_IF(sphere->elements[1][0]->find_neighbor(sphere->elements[1][1]), "test_KSimplex_reconstruct_neighbors_from_vertices");

    //test_ok("test_KSimplex_reconstruct_neighbors_from_vertices");
}


// ###################
// Printing functions:
// ###################

// Rudimentary printing of the simplex data to stdout
// (probably will be deprecated soon --- use print_detailed() instead)
// TODO: ja bih ovo prepravio u wrapper za print_detailed() ili sl.
void test_KSimplex_print(){
    string name = "test";
    int D = 2;
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name, D);

    KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    
    
    string expectedStr = R""""(Printing KSimplex:  k = 1, D = 2
Printing colors:
  Printing neighbors:
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   
)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    newKSimplex->print();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_KSimplex_print");

    test_ok("test_KSimplex_print");
}

// Prints the set of UniqueID colors of vertices of a given simplex,
// in a human-readable format, "50(2-13-44)" or "(2-13-44)" and similar, to stdout
// Works best if all vertices in the simplex are colored with UniqueIDColor
void test_KSimplex_print_compact(){
    string name = "test";
    int D = 2;
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name, D);

    KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    
    
    string expectedStr = R""""(simplex)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    newKSimplex->print_compact();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_KSimplex_print_compact");

    test_ok("test_KSimplex_print_compact");
}

// Detailed printing of the data of the simplex, in a nice human-readable
// tabular format, to stdout
// Prints the list of colors with their values, and a table of neighbors of a simplex
// Relies on print_compact(), and works best if the entire complex is colored
// with UniqueIDColor (see UniqueIDColor::colorize_entire_complex() for details)
void test_KSimplex_print_detailed(){
    string name = "test";
    int D = 2;
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name, D);

    KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    
    
    string expectedStr = R""""(Simplex ID: 

List of colors: 

Table of neighbors: 
---------------------------------------------
Simplices at level k = 0:   Simplex, Simplex
Simplices at level k = 1:   
Simplices at level k = 2:   
---------------------------------------------

)"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    newKSimplex->print_detailed();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_KSimplex_print_detailed");

    test_ok("test_KSimplex_print_detailed");
}

// Constructs a string for printing the HTML link for a simplex,
// with the syntax appropriate for the GUI
// (it is heavily used by the GUI, not useful otherwise)
void test_KSimplex_print_html(){
    string name = "test";
    int D = 2;
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name, D);

    KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    
    
    //newKSimplex->print_html();

    string expectedStr = R""""()"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    newKSimplex->print_html();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_KSimplex_print_html");

    test_ok("test_KSimplex_print_html");
}

// Constructs a string for printing the non-HTML name for a simplex,
// with the syntax appropriate for the GUI
// (it is heavily used by the GUI, not useful otherwise)
void test_KSimplex_print_non_html(){
    string name = "test";
    int D = 2;
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = new(nothrow) SimpComp(name, D);

    KSimplex *simpsmall = simpComp->create_ksimplex(0);
    
    KSimplex *newKSimplex = build_simplex_one_level_up(simpComp, simpsmall);    
    
    //newKSimplex->print_non_html();
    
    string expectedStr = R""""()"""";

    // Redirect cout:
    streambuf* oldCoutStreamBuf = cout.rdbuf();
    std::ostringstream testStr;
    cout.rdbuf( testStr.rdbuf() );

    // All cout output is redirected to testStr:
    newKSimplex->print_non_html();

    // Restoring original cout:
    cout.rdbuf( oldCoutStreamBuf );
 
    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "test_KSimplex_print_non_html");

    test_ok("test_KSimplex_print_non_html");
}

void test_TopologicalCoordinatesColor(){
    string name = "test_TopologicalCoordinatesColor";
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = seed_single_simplex(3, "triangle");
    //simpComp->print_compact();

    TopologicalCoordinatesColor::initQMinQMax(simpComp->D);
    TopologicalCoordinatesColor::colorize_entire_complex(simpComp);
    
    TopologicalCoordinatesColor::evaluate_spring_potential(simpComp);
    
    TopologicalCoordinatesColor::evaluate_potential_minimum(simpComp);
}

void test_EmbeddingCoordinatesColor(){
    string name = "test_EmbeddingCoordinatesColor";
    
    // Initilize simplicial complex of dimension D, and an empty k-simplex:
    SimpComp *simpComp = seed_single_simplex(3, "triangle");
    //simpComp->print_compact();

    TopologicalCoordinatesColor::initQMinQMax(simpComp->D);
    TopologicalCoordinatesColor::colorize_entire_complex(simpComp);
    
    TopologicalCoordinatesColor::evaluate_spring_potential(simpComp);
    
    TopologicalCoordinatesColor::evaluate_potential_minimum(simpComp);

    EmbeddingCoordinatesColor::evaluate_embedding_coordinates(simpComp);
    for(auto vertex : simpComp->elements[0]){
        Color* col = Color::find_pointer_to_color_type(vertex, TYPE_EMBEDDING_COORDINATES);
        col->print();
    }
}
    
void run_tests(){
    test_kSimplex_default_constructor();
    test_kSimplex_default_destructor();
    test_kSimplex_collect_vertices();
    test_kSimplex_find_neighbor();
    test_kSimplex_get_uniqueID();

    test_build_simplex_one_level_up();
    test_attach_new_simplex_to_boundary();
    test_seed_single_simplex();
    test_seed_sphere();
    test_seed_sphere_intuitively();
    test_unseed_complex();
    test_unseed_everything();

    test_printing();
    
    test_KSimplex_collect_vertices();
    test_KSimplex_find_neighbor();
    test_KSimplex_add_neighbor();
    test_KSimplex_delete_neighbor();
    test_KSimplex_delete_all_neighbors();
    test_KSimplex_reconstruct_neighbors_from_vertices();
    
    test_KSimplex_print();
    test_KSimplex_print_compact();
    test_KSimplex_print_detailed();
    test_KSimplex_print_html();
    test_KSimplex_print_non_html();

    test_TopologicalCoordinatesColor();

    test_EmbeddingCoordinatesColor();

}

void test(){
    //cout << endl << "---------------------------------------------------"
    //        << endl << "Starting tests/demos..." << endl;

    //SimpComp *simpComp = seed_single_simplex(3, "triangle");
    //simpComp->print_compact();

    //SimpComp *simpComp = seed_sphere(3, "sphere");
    //simpComp->print_compact();

    //save_complex_to_xml_file(triangle, "triangle.xml");
    //save_complex_to_xml_file(tetrahedron, "tetrahedron.xml");

    /*
	cout << endl << "Testing function reconstruct_neighbors_from_vertices" << endl;

 SimpComp *simpComp = seed_single_simplex(3, "tetraedar");
 UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
 simpComp->print_compact();
 // Print random k-simplex:
 cout << endl;
 int n=2;
 int m=3;
 simpComp->elements[n][m]->neighbors->print_compact();
 cout << endl;
 // Delete neighbors except vertices:
 for(int k = 0; k <= simpComp->D; k++)
   for(auto &kSimplex : simpComp->elements[k])
     for(int tempK = 1; tempK <= simpComp->D; tempK++)
       kSimplex->neighbors->elements[tempK].clear();
 // Print the same k-simplex:
 cout << endl;
 simpComp->elements[n][m]->neighbors->print_compact();
 cout << endl;
 simpComp->elements[n][m]->reconstruct_neighbors_from_vertices(simpComp);
 // simpComp->reconstruct_neighbors_from_vertices();
 // Print the same k-simplex:
 cout << endl;
 simpComp->elements[n][m]->neighbors->print_compact();
 cout << endl;
 cout << endl;
 // simpComp->print_detailed();
    */

    /*
    SimpComp *simpComp = seed_sphere(3, "simpCompFindByIDs");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    cout << endl << "Testing find_KSimplex by ID" << endl;
    KSimplex* temp = simpComp->find_KSimplex(2);
    if(temp)
        temp->print_compact();
    else
        cout << "2 not found!!!" << endl;
    cout << endl << "---------------------------------------------------" << endl;

    cout << endl << "Testing find_KSimplex by IDs" << endl;
	set<int> IDs {1, 2, 4};
	temp = simpComp->find_KSimplex(IDs);
    cout << endl << "---------------------------------------------------" << endl;
    temp->print();
    //*/
    
    /*
    cout << endl << "Testing deleting KSimplex with IDs 1, 2, 3, and 4" << endl;
    SimpComp *simpCompDeleteKSimplex = seed_sphere_intuitively(3, "sphere");
    UniqueIDColor::colorize_simplices_at_level(simpCompDeleteKSimplex, 0);
    simpCompDeleteKSimplex->print_compact();

    set<int> s = {1, 2, 3, 4};
    simpCompDeleteKSimplex->remove_simplex(s);
    simpCompDeleteKSimplex->print_compact();
    //*/
    
}

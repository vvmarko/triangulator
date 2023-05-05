
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

    string expectedStr = R""""(Printing SimpComp 1-sfera, topology = sphere, D = 1
  Printing SimpComp 1-sfera elements, level = 0:
    Printing KSimplex:  k = 0, D = 1
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   4(1-2), 5(1-3)
    Printing KSimplex:  k = 0, D = 1
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   4(1-2), 6(2-3)
    Printing KSimplex:  k = 0, D = 1
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   
Simplices at level k = 1:   5(1-3), 6(2-3)
  Printing SimpComp 1-sfera elements, level = 1:
    Printing KSimplex:  k = 1, D = 1
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   1, 2
Simplices at level k = 1:   
    Printing KSimplex:  k = 1, D = 1
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   1, 3
Simplices at level k = 1:   
    Printing KSimplex:  k = 1, D = 1
    Printing colors:
      Printing neighbors:
Simplices at level k = 0:   2, 3
Simplices at level k = 1:   
)"""";

    // Printing all redirected cout output:
    //cout << testStr.str();
    //cout << expectedStr;

    TEST_FAILED_IF(expectedStr != testStr.str(), "SimpComp printing");

    test_ok("SimpComp printing");

    //delete simpCompTest; TODO: FIX
}


void run_tests(){
    test_kSimplex_default_constructor();
    test_kSimplex_default_destructor();
    test_kSimplex_collect_vertices();
    test_kSimplex_find_neighbor();
    test_kSimplex_get_uniqueID();

    test_printing();
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

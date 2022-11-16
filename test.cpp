
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
	
    SimpComp *simpComp = seed_single_simplex(3, "sphere");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();

    // Print random k-simplex:
	cout << endl;
    simpComp->elements[1][0]->print("");
	cout << endl;

    // Delete neighbors except vertices:
	for(int k = 1; k <= simpComp->D; k++)
		for(auto &kSimplex : simpComp->elements[k])
			for(int tempK = 1; tempK <= simpComp->D; tempK++)
				kSimplex->neighbors->elements[tempK].clear();

    // Print the same k-simplex:
	cout << endl;
    simpComp->elements[1][0]->print("");
	cout << endl;

	simpComp->reconstruct_neighbors_from_vertices();

    // Print the same k-simplex:
	cout << endl;
    simpComp->elements[1][0]->print("");
	cout << endl;

	cout << endl;
    simpComp->print_compact();
    //*/

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
    simpCompDeleteKSimplex->delete_KSimplex(s);
    simpCompDeleteKSimplex->print_compact();
    //*/
    
}

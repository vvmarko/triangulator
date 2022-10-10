
#include "triangulator.hpp"

int main(){
    //save_complex_to_xml_file(triangle, "triangle.xml");
    //save_complex_to_xml_file(tetrahedron, "tetrahedron.xml");
    cout << endl << "---------------------------------------------------"
            << endl << "General seed example, compact printing:" << endl << endl;

    //SimpComp *simpComp = seed_single_simplex(3, "triangle");
    //simpComp->print_compact();

    //SimpComp *simpComp = seed_sphere(3, "sphere");
    //simpComp->print_compact();

	/*
	//Function reconstruct_neighbors_from_vertices demonstration:
	
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
    
    SimpComp *simpComp = seed_sphere_intuitively(3, "sphere");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    
    //SimpComp simpComp2(*simpComp);
    //simpComp2.print_compact();
	//for(auto &it : simpComp2.elements)
	//	it.clear();
	//delete simpComp;
	//cout << "Deleted." << endl;

	/*
	cout << "Simulatin Pancher Moves 1->4 and 4->1:" << endl;
    SimpComp *simpComp = seed_single_simplex(3, "PancherSimulation");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;

	Pachner_move_1_to_4(simpComp->elements[simpComp->D][0], simpComp);
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;
	
	Pachner_move_4_to_1(simpComp->elements[0][ simpComp->elements[0].size()-1 ], simpComp);
    simpComp->print_compact();
	//*/

    /*
    SimpComp *simpComp = seed_sphere(3, "simpCompFindByIDs");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
	cout << "Testing find_KSimplex by ID:" << endl;
    KSimplex* temp = simpComp->find_KSimplex(2);
    if(temp)
        temp->print_compact();
    else
        cout << "2 not found!!!" << endl;
    cout << endl << "---------------------------------------------------" << endl;

	cout << "Testing find_KSimplex by IDs:" << endl;
	set<int> IDs {1, 2, 4};
	temp = simpComp->find_KSimplex(IDs);
    cout << endl << "---------------------------------------------------" << endl;
    temp->print();
    //*/

	/*
	//cout << "Simulatin Pancher Moves 3->2 and 2->3:" << endl;
	cout << "Simulatin Pancher Move 3->2:" << endl;
    SimpComp *simpComp = seed_sphere(3, "PancherSimulation");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;
    
    
        




/*
	set<KSimplex*> &s;
	kSimplex->collect_vertices(s);
    s.insert(simpComp->elements[0][4]);
        	simpComp->find_vertices(s);


	simpComp->Pachner_move_3_to_2(simpComp->elements[simpComp->D][0], simpComp);
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;
	
	//Pachner_move_2_to_3(simpComp->elements[0][ simpComp->elements[0].size()-1 ], simpComp);
    //simpComp->print_compact();
	//*/
	
    return 0;
}


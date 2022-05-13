
#include "triangulator.hpp"

int main(){
    //save_complex_to_xml_file(triangle, "triangle.xml");
    //save_complex_to_xml_file(tetrahedron, "tetrahedron.xml");
    cout << endl << "---------------------------------------------------"
            << endl << "General seed example, compact printing:" << endl << endl;

    //SimpComp *simpComp = seed_single_simplex(3);
    //simpComp->print_compact();

    //SimpComp *simpComp = seed_sphere(3);
    //simpComp->print_compact();

	/*
	//Function reconstruct_neighbors_from_vertices demonstration:
	
    SimpComp *simpComp = seed_single_simplex(3);
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

	KSimplex::reconstruct_neighbors_from_vertices(simpComp);

    // Print the same k-simplex:
	cout << endl;
    simpComp->elements[1][0]->print("");
	cout << endl;

	cout << endl;
    simpComp->print_compact();
    //*/
    
    //SimpComp *simpComp = seed_sphere_intuitively(3);
    //simpComp->print_compact();
    
    //SimpComp simpComp2(*simpComp);
    //simpComp2.print_compact();
	//for(auto &it : simpComp2.elements)
	//	it.clear();
	//delete simpComp;
	//cout << "Deleted." << endl;
	
    SimpComp *simpComp = seed_single_simplex(3);
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;

	Pachner_move_1_to_4(simpComp->elements[simpComp->D][0], simpComp);
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
	

    return 0;
}


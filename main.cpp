
#include "triangulator.hpp"

int main(){
/*
    cout << endl << "Edge example:" << endl << endl;
    SimpComp *edge = seed_single_edge("First edge");
    edge->print();
 
    cout << endl << "Triangle example:" << endl << endl;
    SimpComp *triangle = seed_single_triangle("First triangle");
    triangle->print();
    
    cout << endl << "Tetrahedron example." << endl << endl;
    SimpComp *tetrahedron = seed_single_tetrahedron("First tetrahedron");
    
    cout << endl << "Triangle example, compact printing:" << endl << endl;
    triangle->print_compact();

    cout << endl << "Tetrahedron example, compact printing:" << endl << endl;
    tetrahedron->print_compact();

    cout << endl << "Tetrahedron example, coloring level 0:" << endl << endl;
    UniqueIDColor::colorize_simplices_at_level(tetrahedron, 0);
    tetrahedron->print_compact();

    cout << endl << "Tetrahedron example, coloring everything:" << endl << endl;
    UniqueIDColor::colorize_entire_complex(tetrahedron);
    tetrahedron->print_compact();

    save_complex_to_xml_file(triangle, "triangle.xml");
    save_complex_to_xml_file(tetrahedron, "tetrahedron.xml");
//*/

//*  
    cout << endl << "---------------------------------------------------"
            << endl << "General seed example, compact printing:" << endl << endl;

    SimpComp *simpComp = seed_single_simplex(3);
    simpComp->print_compact();

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
    
    return 0;
}


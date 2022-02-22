
#include "triangulator.hpp"

int main(){
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
    UniqueIDColor::colorize_all_simplices(tetrahedron, 0);
    tetrahedron->print_compact();

    cout << endl << "Tetrahedron example, coloring everyithing:" << endl << endl;
    UniqueIDColor::colorize_entire_complex(tetrahedron);
    tetrahedron->print_compact();
    
//    cout << endl << "General seed example, compact printing:" << endl << endl;
//    SimpComp *simpComp = seed_single_SimpComp("1. general SimpComp", 3);
//    simpComp->print_compact();

    return 0;
}


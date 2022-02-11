
#include "triangulator.hpp"

#include <iostream>
#include <string>

int main(){
    cout << endl << "Edge example:" << endl << endl;
    SimpComp *edge = seed_single_edge("First edge");
    edge->print();
 
    cout << endl << "Triangle example:" << endl << endl;
    SimpComp *triangle = seed_single_triangle("First triangle");
    triangle->print();

    cout << endl << "Triangle example, compact printing:" << endl << endl;
    triangle->print_compact();

    return 0;
}

#include "triangulator.hpp"

#include <iostream>
#include <string>

int main(){
    ScreenCoordinateColor c1(0, 0), c2(1, 1), c3(2, 0);
    //BoundaryColor c1(true), c2(true), c3(true);

    cout << endl << "Edge example:" << endl << endl;
    SimpComp *edge = seed_edge("First edge", c1, c2);
    edge->print();
 
    cout << endl << "Triangle example:" << endl << endl;
    SimpComp *triangle = seed_triangle("First triangle", c1, c2, c3);
    triangle->print();

    return 0;
}
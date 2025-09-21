
#include "triangulator.hpp"

#include <unistd.h>

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    int D = 3;
    long unsigned int i;
    BoundingSphere bs;
    
    //    SimpComp *sph = seed_single_simplex(D,"mycomplex");
    SimpComp *sph = seed_sphere(D,"mysphere");
    Pachner_move(sph->elements[D][0],sph);
    Pachner_move(sph->elements[D][0],sph);
    Pachner_move(sph->elements[D][0],sph);
    Pachner_move(sph->elements[D][0],sph);
    Pachner_move(sph->elements[D][0],sph);

    evaluate_potential_minimum(sph);

    cout << "Number of cells is: " << sph->elements[D].size() << endl << endl;

    for(auto it : sph->elements[D]){
      bs = evaluate_bounding_sphere(it, sph);

      cout << "Cell pointer: " << bs.simplex << endl;
      cout << "Topology: " << bs.topology << endl;
      cout << "Coordinates: ";
      for(i = 0; i < bs.center.size(); i++) cout << bs.center[i] << "   ";
      cout << endl;
      cout << "Radius: " << bs.radius << endl;
      cout << endl;
    }

    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}



#include "triangulator.hpp"

#include <unistd.h>

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    DrawingCoordinatesColor *col;
    int D = 2;
    
    //    SimpComp *sph = seed_single_simplex(D,"sphere");
    SimpComp *sph = seed_sphere(D,"sphere");
    
    Pachner_move(sph->elements[D][0],sph);
    Pachner_move(sph->elements[D][1],sph);
    Pachner_move(sph->elements[D][2],sph);
    Pachner_move(sph->elements[D][3],sph);
    Pachner_move(sph->elements[D][4],sph);

    UniqueIDColor::colorize_entire_complex(sph);
    sph->print_compact();
    cout << endl;

    initialize_drawing_coordinates(sph);
    evaluate_potential_minimum(sph);
    evaluate_embedding_coordinates(sph);

    cout << endl << "Spring potential value: " << evaluate_spring_potential(sph) << endl << endl;

    cout << "Edge lengths:";
    for(auto &it : sph->elements[1])
      cout << "  " << evaluate_coordinate_length(it,sph);
    cout << endl << endl;
    
    for(auto &it : sph->elements[0] ){
      col = DrawingCoordinatesColor::find_pointer_to_color(it);
      col->print();
      cout << endl;
    }

    
    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}

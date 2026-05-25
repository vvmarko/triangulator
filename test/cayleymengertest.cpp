#include "triangulator.hpp"

int main()
{
    cout << endl << "#######################################################" << endl;
    cout << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout << "#######################################################" << endl << endl;

    SimpComp* g = seed_sphere(2, "obicna 2-sfera");
    UniqueIDColor::colorize_entire_complex(g);
    VolumeSquaredColor::colorize_entire_complex(g);

    cout << "Cayley Menger matrix for one KSimplex k=2" << endl;
    print_vector_of_a_vector(cayley_menger_matrix(g->elements[2][0]));

    cout << endl << "Cayley Menger matrix for one KSimplex k=1" << endl;
    print_vector_of_a_vector(cayley_menger_matrix(g->elements[1][0]));

    cout << endl << "Cayley Menger matrix for one KSimplex k=0" << endl;
    print_vector_of_a_vector(cayley_menger_matrix(g->elements[0][0]));

    unseed_everything();

    cout << endl << "#################################################" << endl;
    cout << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout << "#################################################" << endl << endl;

    return 0;
}
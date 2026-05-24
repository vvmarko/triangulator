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

    cout << endl << "Number of neighboring vertices of vertex0 (g->elements[0][0]): " << g->elements[0][0]->neighbors->elements[0].size() << endl
        << "Number of neighboring vertices of edge0 (g->elements[1][0]): " << g->elements[1][0]->neighbors->elements[0].size() << endl
        << "Number of neighboring vertices of face0 (g->elements[2][0]): " << g->elements[2][0]->neighbors->elements[0].size() << endl;

    cout << endl << "Number of neighboring edges of edge0 (g->elements[1][0]): " << g->elements[1][0]->neighbors->elements[1].size() << endl;

    unseed_everything();

    cout << endl << "#################################################" << endl;
    cout << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout << "#################################################" << endl << endl;

    return 0;
}
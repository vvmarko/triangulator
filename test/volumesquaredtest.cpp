#include "triangulator.hpp"

int main()
{
    cout << endl << "#######################################################" << endl;
    cout << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout << "#######################################################" << endl << endl;

    unsigned int dim = 6;
    SimpComp* g = seed_sphere(dim, "obicna 6-sfera");
    UniqueIDColor::colorize_entire_complex(g);
    VolumeSquaredColor::colorize_entire_complex(g);

    for (unsigned int i = 0; i <= dim; i++)
    {
        cout << "Volume squared of one KSimplex k=" << i << ": "
            << evaluate_volume_squared(g->elements[i][0]) << endl;
    }

    unseed_everything();

    cout << endl << "#################################################" << endl;
    cout << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout << "#################################################" << endl << endl;

    return 0;
}
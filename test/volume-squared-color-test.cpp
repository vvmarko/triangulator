
#include "triangulator.hpp"

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    SimpComp *g = seed_sphere(2,"obicna 2-sfera");
    UniqueIDColor::colorize_entire_complex(g);
    VolumeSquaredColor::colorize_entire_complex(g);
    g->print_detailed();

    unseed_everything();

    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}


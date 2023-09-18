
#include "triangulator.hpp"


int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    int level = 3;
    SimpComp *pahsfera =  seed_sphere(3,"sfera");
    UniqueIDColor::colorize_entire_complex(pahsfera);
    PachnerColor::colorize_entire_complex(pahsfera);
    KSimplex *sphantip=factorize_Pachner_sphere(pahsfera,level);
    pahsfera->print_detailed();
    cout << endl << "input simplex ID: ";
    pahsfera->elements[level][0]->print_compact();
    cout << endl << "antipode simplex ID: ";
    sphantip->print_compact();
    cout << endl << endl;

    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}

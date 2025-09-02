
#include "triangulator.hpp"

#include <unistd.h>

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    int D = 2;
    
    //    SimpComp *sph = seed_single_simplex(D,"sphere");
    SimpComp *sph = seed_sphere(D,"sphere");
    
    ScreenParameters *sp = new(nothrow) ScreenParameters(sph->D + 1);

    cout << "Damb = " << sp->Damb << endl;
    cout << "d =    " << sp->d << endl;
    cout << "sx =   " << sp->sx << endl;
    cout << "sy =   " << sp->sy << endl;
    cout << "sz =   " << sp->sz << endl;
    cout << "Number of alpha angles: " << sp->alpha.size() << endl;
    cout << "Number of beta angles: " << sp->beta.size() << endl;
    cout << "Number of gamma angles: " << sp->gamma.size() << endl;

    free(sp);
    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}


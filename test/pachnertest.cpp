
#include "triangulator.hpp"


int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;


    int D = 5;
    int level = 5;
    int n = 0;
    SimpComp *sfera = seed_sphere(D,"sfera");
    SimpComp *tet = seed_single_simplex(D,"tet");
    SimpComp *pahsfera1 = seed_sphere(D,"ps1");
    SimpComp *pahsfera2 = seed_sphere(D,"ps2");

    KSimplex *simpsf = sfera->elements[level][n];
    KSimplex *simptet = tet->elements[level][n];

    PachnerColor::colorize_entire_complex(pahsfera1);
    PachnerColor::colorize_entire_complex(pahsfera2);
    PachnerColor::colorize_single_simplex(simpsf);
    PachnerColor::colorize_single_simplex(simptet);

    PachnerColor *simpPachnerColor = PachnerColor::find_pointer_to_color( simpsf );
    PachnerColor *sphereKSimplexColor = PachnerColor::find_pointer_to_color( pahsfera1->elements[level][0] );
    simpPachnerColor->matchingSimplex = pahsfera1->elements[level][0];
    simpPachnerColor->immutable = true;
    sphereKSimplexColor->matchingSimplex = simpsf;
    sphereKSimplexColor->immutable = true;
    sphereKSimplexColor->internalSimplex = true;

    simpPachnerColor = PachnerColor::find_pointer_to_color( simptet );
    sphereKSimplexColor = PachnerColor::find_pointer_to_color( pahsfera2->elements[level][0] );
    simpPachnerColor->matchingSimplex = pahsfera2->elements[level][0];
    simpPachnerColor->immutable = true;
    sphereKSimplexColor->matchingSimplex = simptet;
    sphereKSimplexColor->immutable = true;
    sphereKSimplexColor->internalSimplex = true;

    bool outcomesf = simple_check_for_Pachner_compatibility(simpsf);
    bool outcometet = simple_check_for_Pachner_compatibility(simptet);

    cout << endl;
    cout << "D = " << D << ", level = " << level << ", n = " << n << endl;
    cout << endl;
    cout << "Sphere test, should be true for all level and dimension:      " << outcomesf << endl;
    cout << "Tet test, should be false for level<D, and true for level==D: " << outcometet << endl << endl;

    /*
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
    */
    
    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}

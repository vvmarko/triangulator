
#include "triangulator.hpp"


int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    int D = 4;
    int i;
    KSimplex *simp1;
    KSimplex *simp2;
    SimpComp *sph = seed_sphere(D,"sphere");

    UniqueIDColor::colorize_entire_complex(sph);
    cout << endl << "Original complex:" << endl << endl;
    sph->print_compact();
    cout << endl << "===============================================" << endl;
    for(i=D;i>=0;i--){
      simp1 = sph->elements[i][0];
      cout << endl << "Complex after the Pachner move on the simplex ";
      simp1->print_compact();
      cout << endl << endl;
      simp2 = Pachner_move(simp1,sph);
      UniqueIDColor::append_color_to_entire_complex(sph);
      if(simp2==nullptr) {
	cout << "The Pachner move could not be performed on this simplex, skipping." << endl;
        cout << "===============================================" << endl;
      } else {
        sph->print_compact();
        cout << endl << "The new resulting antipode simplex is ";
        simp2->print_compact();
        cout << endl;
        cout << endl << "-----------------------------------------------" << endl;
        cout << endl << "Complex after the inverse Pachner move, on the simplex ";
        simp2->print_compact();
        cout << endl << endl;
        simp1 = Pachner_move(simp2,sph);
        UniqueIDColor::append_color_to_entire_complex(sph);
        sph->print_compact();
        cout << endl << "The new resulting antipode of the antipode simplex is ";
        simp1->print_compact();
        cout << endl;
        cout << endl << "===============================================" << endl;
      }
    }
    
    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}

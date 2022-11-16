
#include "triangulator.hpp"
#include "test.cpp"

int main(){

    test();

    cout << endl << "---------------------------------------------------" << endl << endl;





    //*
//cout << endl << "Testing preparing simpComp from 3D-sphere to Pachner 3->2 initial state:" << endl;
    SimpComp *simpComp = seed_sphere_intuitively(3, "sphere");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
//simpComp->print_compact();
    set<int> s1 = {1,2,3,4};
    simpComp->delete_KSimplex(s1);
    set<int> s2 = {2,3,4,5};
    simpComp->delete_KSimplex(s2);
    set<int> s3 = {2,3,4};
    simpComp->delete_KSimplex(s3);
//simpComp->print_compact();
    //*/
    



	/*
	cout << "Simulatin Pancher Moves 1->4 and 4->1:" << endl;
    SimpComp *simpComp = seed_single_simplex(3, "PancherSimulation");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;

	Pachner_move_1_to_4(simpComp->elements[simpComp->D][0], simpComp);
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;
	
	Pachner_move_4_to_1(simpComp->elements[0][ simpComp->elements[0].size()-1 ], simpComp);
    simpComp->print_compact();
	//*/


	/*
	//cout << "Simulatin Pancher Moves 3->2 and 2->3:" << endl;
	cout << "Simulatin Pancher Move 3->2:" << endl;
    SimpComp *simpComp = seed_sphere(3, "PancherSimulation");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;
    */  
    
        




    /*
    cout << endl << "Testing Pachner_move_3_to_2" << endl;
	Pachner_move_3_to_2(simpComp->elements[1][6], simpComp);
	
	//Pachner_move_2_to_3(simpComp->elements[0][ simpComp->elements[0].size()-1 ], simpComp);
    //simpComp->print_compact();
	//*/




	
    return 0;
}


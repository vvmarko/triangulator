
#include "triangulator.hpp"

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;


   SimpComp* sc1 = seed_sphere(2,"prvi");
   UniqueIDColor::colorize_entire_complex(sc1);
   SimpComp* sc2 = seed_single_simplex(3,"drugi");
   UniqueIDColor::colorize_entire_complex(sc2);
   SimpComp* sc3 = seed_sphere(4,"treci");
   UniqueIDColor::colorize_entire_complex(sc3);
   cout << "===========================" << endl;
   sc1->print_compact();
   cout << "===========================" << endl;
   sc2->print_compact();
   cout << "===========================" << endl;
   sc3->print_compact();
   cout << "===========================" << endl;
   cout << "next free uid number = " << UniqueIDColor::next_free_uid_number << endl;
   cout << "Number of complexes = " << triangulator_global::seededComplexes.size();
   
   unseed_complex(sc2);

   cout << endl << "Deleting complex 2..." << endl << endl;
   
   cout << "===========================" << endl;
   sc1->print_compact();
   cout << "===========================" << endl;
   sc3->print_compact();
   cout << "===========================" << endl;
   cout << "next free uid number = " << UniqueIDColor::next_free_uid_number << endl;
   cout << "Number of complexes = " << triangulator_global::seededComplexes.size();
   
   UniqueIDColor::relabel_everything();

   cout << endl << "Relabeling UniqueID colors..." << endl << endl;
   
   cout << "===========================" << endl;
   sc1->print_compact();
   cout << "===========================" << endl;
   sc3->print_compact();
   cout << "===========================" << endl;
   cout << "next free uid number = " << UniqueIDColor::next_free_uid_number << endl;
   cout << "Number of complexes = " << triangulator_global::seededComplexes.size();
   
   unseed_everything();
   
    /*
    triangulator_global::logLevel=LOG_DEBUG;
    triangulator_global::logFilename="logfile.html";

    log_report(LOG_PANIC,"Testing the log reporting facility, to see if it works and if it is readable...");
    log_report(LOG_ERROR,"Testing the log reporting facility, to see if it works and if it is readable...");
    log_report(LOG_WARN,"Testing the log reporting facility, to see if it works and if it is readable...");
    log_report(LOG_INFO,"Testing the log reporting facility, to see if it works and if it is readable...");
    log_report(LOG_DEBUG,"Testing the log reporting facility, to see if it works and if it is readable...");
    */


    /*
    SimpComp *g = seed_single_simplex(4,"simpleks");
    UniqueIDColor::colorize_entire_complex(g);
    g->print_compact();
    cout << endl << "===========================" << endl << endl;
    KSimplex *stranica = g->elements[3][1];
    attach_new_simplex_to_boundary( stranica , g );
    UniqueIDColor::append_color_to_entire_complex(g);
    g->print_compact();
    unseed_complex(g);
    */


    

    
    /*  
    SimpComp *g1 = seed_sphere(3,"obicna 2-sfera");
    UniqueIDColor::colorize_entire_complex(g1);
    g1->print_detailed();
    */
/*
    SimpComp *simpComp = seed_sphere(3, "3-sfera");
    UniqueIDColor::colorize_entire_complex(simpComp);
    //triangulator_global::seededComplexes[0]->print_detailed();





cout << "main: " << triangulator_global::seededComplexes.size() << endl;

    SimpComp *simpComp2 = seed_sphere(2, "2-sfera");
    UniqueIDColor::colorize_entire_complex(simpComp2);

cout << "main: " << triangulator_global::seededComplexes.size() << endl;

    //triangulator_global::seededComplexes[1]->print_detailed();

    unseed_everything();
*/  
    //    save_complex_to_xml_file(simpComp,"proba.xml");
    //    SimpComp *sc2 = read_complex_from_xml_file("proba.xml");
    //    save_complex_to_xml_file(sc2,"proba2.xml");
    //    sc2->print_detailed();
    
    /*
    string s=simpComp->print_html();
    cout << "<br><br>";
    cout << s;
    cout << "<br><br>";
    s=simpComp->elements[0][1]->neighbors->print_html();
    cout << s << endl;
    cout << "<br><br>";
    */
    
    /*
    SimpComp *simpComp = seed_sphere_intuitively(3, "moja_sfera");
    UniqueIDColor::colorize_entire_complex(simpComp);
    save_complex_to_xml_file(simpComp, "test1.xml");
    SimpComp *simpComp2 = read_complex_from_xml_file("test1.xml");
    save_complex_to_xml_file(simpComp2, "test2.xml");
    */

    /*
cout << endl << "Testing boundaries..." << endl << "seed_single_simplex(3,...);" << endl;
    SimpComp *simpComp = seed_single_simplex(3, "seed_single_simplex and 3");
    UniqueIDColor::colorize_simplices_at_level(simpComp, 0);
    if(simpComp->elements[3][0]->is_a_boundary())
        cout << "Jeste boundary elements[3][0]" << endl;
    else
        cout << "Nije boundary elements[3][0]" << endl;
    if(simpComp->elements[2][0]->is_a_boundary())
        cout << "Jeste boundary elements[2][0]" << endl;
    else
        cout << "Nije boundary elements[2][0]" << endl;
    */


    /*
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
    SimpComp *simpCompP32 = seed_sphere(3, "PancherSimulation");
    UniqueIDColor::colorize_simplices_at_level(simpCompP32, 0);
    simpComp->print_compact();
    cout << endl << "---------------------------------------------------" << endl;
    //*/  
    
        




    /*
    cout << endl << "Testing Pachner_move_3_to_2" << endl;
	Pachner_move_3_to_2(simpComp->elements[1][6], simpComp);
	
	//Pachner_move_2_to_3(simpComp->elements[0][ simpComp->elements[0].size()-1 ], simpComp);
    //simpComp->print_compact();
	//*/




    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}

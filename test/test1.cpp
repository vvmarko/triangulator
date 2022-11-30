#include "triangulator.hpp"

int main(){

  //    test();

  cout << endl << "#######################################################" << endl;
  cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
  cout         << "#######################################################" << endl << endl;

    SimpComp *simpComp = seed_sphere_intuitively(3, "moja_sfera");
    UniqueIDColor::colorize_entire_complex(simpComp);
    //save_complex_to_xml_file(simpComp, "test1.xml");
    //SimpComp *simpComp2 = read_complex_from_xml_file("test1.xml");
    //save_complex_to_xml_file(simpComp2, "test2.xml");
    simpComp->print_detailed();


  cout << endl << "#################################################" << endl;
  cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
  cout         << "#################################################" << endl << endl;
	
    return 0;
}
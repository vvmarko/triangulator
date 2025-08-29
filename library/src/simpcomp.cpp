
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include <algorithm>
#include "triangulator.hpp"

// TODO: ne vidim da bilo gde koristimo cout skupa integera,
//       da li nam ova funkcija uopste treba negde?
std::ostream &operator<<(std::ostream &os, set<int> &s) { 
	os << "(";
	bool first = true;
	for(auto &it : s){
		if(!first)
			os << ",";
		first = false;
		os << it;
	}
	os << ")";
    return os;
}

// #############################
// SimpComp class implementation
// #############################

// General information:
//
// Here are the implementations of the functions for the SimpComp class,
// declared in "simpcomp.hpp" file. See that file for the description
// of the purpose of each function, and see here (below) for how it
// works.


// ##########################################
// Constructors and destructors of a complex:
// ##########################################

// Default constructor
SimpComp::SimpComp(string SimpCompName, int dim):
        name {SimpCompName}, topology{""}, D{dim}{
    if (D<1){
      log_report(LOG_ERROR, "SimpComp(): SimpComp constructor called for dimension less than 1!! Probably a bug in seed function.");
      log_report(LOG_ERROR, "SimpComp(): If you see this message, the rest of the code will probably fail, fix your code!");
    }
// Grow vector "elements" to size D+1, containing vectors of pointers to simplices, for each level k = 0,...,D:
    for(int i = 0; i <= D; i++){
        vector<KSimplex*> listOfKSimplices;
        elements.push_back(listOfKSimplices);
    }
    log_report(LOG_DEBUG, "SimpComp(): Default constructor: initializing a new simplicial complex object");
    log_report(LOG_DEBUG, "SimpComp(): Name of the complex: " + SimpCompName);
    log_report(LOG_DEBUG, "SimpComp(): Dimension of the complex: "+to_string(D));
}


// No-name constructor
// Just call the default constructor with an empty name string
SimpComp::SimpComp(int dim):SimpComp("", dim){
}

// Default destructor
SimpComp::~SimpComp(){
    log_report(LOG_DEBUG, "~SimpComp(): Default destructor: deallocating an existing simplicial complex object");
    log_report(LOG_DEBUG, "~SimpComp(): Name of the complex: "+name);
    log_report(LOG_DEBUG, "~SimpComp(): Dimension of the complex: "+to_string(D));
    for(int i = 0; i <= D; i++){
      //        for(auto pKSimplex : elements[i]){
      //            delete(pKSimplex);
      //        }
      while(elements[i].size())
	    remove_simplex(elements[i][0]);
    }
}


// ###########################################
// Adding and removing simplices in a complex:
// ###########################################


// Creates a new simplex of level k, and adds it to the complex
KSimplex* SimpComp::create_ksimplex(int k){
    log_report(LOG_DEBUG, "create_ksimplex(): Creating KSimplex at level " + to_string(k) + " in complex "+ this->name );
    if ( (k >= 0) && (k <= D) ){
        // Creating new KSimplex at level k:
        KSimplex *newKSimplex = new(nothrow) KSimplex(k, D);
	if(newKSimplex==nullptr){
	  log_report(LOG_PANIC, "create_ksimplex(): PANIC!!! Cannot allocate memory!!");
          return nullptr;
	}
        // Add newly created k-simplex to the k-th list of elements of this simplicial complex:
        elements[k].push_back(newKSimplex);
        return newKSimplex;
    }else{
        log_report(LOG_ERROR, "create_ksimplex(): You have attempted to create a simplex of level " + to_string(k) + ", which is outside the [0,...,D] range.");
        log_report(LOG_ERROR, "create_ksimplex(): This is not allowed, since the dimension of the complex is " + to_string(D) + ". Nullptr created instead, fix your code!");
        return nullptr;
    }
}

// Given a set of vertices, creates a new simplex in the complex,
// and assigns those vertices as its neighbors
KSimplex* SimpComp::create_ksimplex_from_vertices(set<KSimplex*> &s){
  int level= (int) s.size()-1;
  log_report(LOG_DEBUG, "create_ksimplex_from_vertices(): Creating KSimplex using a set of vertices as its neighbors, at level " + to_string(level) + " in complex "+ this->name );
  if (level > 0){
    KSimplex *kSimplex = create_ksimplex((int) s.size()-1);
    for(auto &tempKSimplex : s){
        kSimplex->neighbors->elements[0].push_back(tempKSimplex);
    }
    return kSimplex;
  }
  if (level == 0){
    log_report(LOG_ERROR, "create_ksimplex_from_vertices(): You have attempted to create a simplex from a set of vertices, which contains only one vertex.");
    log_report(LOG_ERROR, "create_ksimplex_from_vertices(): This does not make sense. Nullptr created instead, fix your code!");
  }
  if (level < 0) {
    log_report(LOG_ERROR, "create_ksimplex_from_vertices(): You have attempted to create a simplex from a set of vertices, which is apparently empty.");
    log_report(LOG_ERROR, "create_ksimplex_from_vertices(): This does not make sense. Nullptr created instead, fix your code!");
  }
  return nullptr;  
}

// Removes a given simplex from a complex, by disconnecting it
// from all of its neighbors and then deleting it
void SimpComp::remove_simplex(KSimplex *kSimplex){
    if(!kSimplex){
      log_report(LOG_WARN, "remove_simplex(): You have tried to remove a simplex from complex " + name + ", but provided a nullptr instead of the simplex.");
      log_report(LOG_WARN, "remove_simplex(): I am skipping this, but you should not be removing nonexistent simplices, check your code.");
      return;
    }
    int level = kSimplex->k;
    if(level > D){
      log_report(LOG_ERROR, "remove_simplex(): You have tried to remove a simplex of level " + to_string(level) + " from complex " + name + " of dimension " + to_string(D) + ".");
      log_report(LOG_ERROR, "remove_simplex(): This simplex either does not belong to that complex, or the structure of the complex is corrupted. Fix your code!");
      return;
    }
    // Find the position of kSimplex in elements[level]:
    unsigned int i = 0;
    while( (i < elements[level].size()) && (elements[level][i] != kSimplex) )
        i++;
    // If not an element:
    if(i == elements[level].size()){
      log_report(LOG_WARN, "remove_simplex(): You have tried to remove a simplex from complex " + name + ", but apparently it is not an element of the complex.");
      log_report(LOG_WARN, "remove_simplex(): I am skipping this, but you should not be removing simplices that belong to other complexes from this complex, check your code.");
      return;
    }
    log_report(LOG_DEBUG, "remove_simplex(): Removing the simplex of level " + to_string(level) + " from complex "+ this->name );
    // If not last:
    if(i < elements[level].size() - 1){
        // Copy the pointer of the last kSimplex onto position i:
        elements[level][i] = elements[level][ elements[level].size() - 1 ];
    }
    // Remove the last element:
    elements[level].pop_back();
    // Disconnect kSimplex from all of its neighbors, and delete its neighbor lists:
    kSimplex->delete_all_neighbors();
    // Free the memory reserved by kSimplex:
    delete kSimplex;
}

// Removes a simplex with given UniqueIDs of its vertices, if it exists,
// by disconnecting it from all of its neighbors and then deleting it
void SimpComp::remove_simplex(set<int> IDs){
    int kFound = IDs.size() - 1;
    if(kFound > D){
      log_report(LOG_ERROR, "remove_simplex(): You have provided a set of " + to_string(kFound+1) + " UniqueIDs for vertices which should uniquely specify a");
      log_report(LOG_ERROR, "remove_simplex(): simplex of level " + to_string(kFound) + ", in order to remove it from complex " + name + " of dimension " + to_string(D) + ".");
      log_report(LOG_ERROR, "remove_simplex(): This simplex either does not belong to that complex, or the structure of the complex is corrupted. Skipping, fix your code!");
      return;
    }
    if(kFound < 0){
      log_report(LOG_ERROR, "remove_simplex(): You have provided an empty set of UniqueIDs for vertices which should uniquely specify a simplex, in order to remove it from a complex.");
      log_report(LOG_ERROR, "remove_simplex(): A simplex cannot be specified with an empty set of vertices. Skipping, fix your code!");
      return;
    }
    KSimplex* toDelete = find_KSimplex(IDs);
    if(!toDelete){
      log_report(LOG_WARN, "remove_simplex(): You have provided a set of UniqueIDs for vertices which should uniquely specify a simplex, in order to remove it from a complex.");
      log_report(LOG_WARN, "remove_simplex(): But such a simplex could not be found. I am skipping this, but you should not generate empty sets of UniqueID's, check your code.");
      return;
    }
    remove_simplex(toDelete);
}


// #################################
// Functions for sizes and counting:
// #################################

// Counts the number of simplices of a given level within the complex
int SimpComp::count_number_of_simplices(int level){
    return elements[level].size();
}


// ##########################################
// Verifying various properties of a complex:
// ##########################################

// Verify if all simplices of a given level are colored with UniqueIDColor
bool SimpComp::all_uniqueID(int level){
    size_t i = 0;
    if( (level > D) || (level < 0) )
        return false;
    while( (i < elements[level].size()) && (elements[level][i]->get_uniqueID()) )
        i++;
    return i == elements[level].size();
}


// #########################################################
// Finding and collecting simplices with various properties:
// #########################################################

// Collect all vertices (simplices of level=0) of a complex into a set
// (this is typically used over a complex of neighbors, see KSimplex class)
void SimpComp::collect_vertices(set<KSimplex*> &s){
    for(auto &kSimplex : elements[0]){
        s.insert(kSimplex);
    }
}

// Collect UniqueID numbers for all vertices (that have UniqueIDColor) into a set
// (similar to above, typically used over a complex of neighbors of a KSimplex)
void SimpComp::collect_vertices_IDs(set<int> &s){
    for(auto &it : elements[0]){
        UniqueIDColor *pColor = it->get_uniqueID();
        if(pColor)
            s.insert( static_cast<UniqueIDColor*>(pColor)->id );
    }
}

// Finds a simplex which contains precisely the given set of vertices, if it exists
KSimplex* SimpComp::find_vertices(set<KSimplex*> &s){
    int kFound = s.size() - 1;
    if( (kFound > D) || (kFound < 0) )
        return nullptr;
    for(auto &it : elements[kFound]){
		set<KSimplex*> sTemp;
        it->collect_vertices(sTemp);
        if(sTemp == s)
        	return it;
    }
    return nullptr;
}

// Finds a simplex with a given UniqueID number, if it exists
KSimplex* SimpComp::find_KSimplex(size_t id){
    for(auto &lvl: elements)
        for(auto &it : lvl){
            UniqueIDColor *pColor = it->get_uniqueID();
            if( (pColor) && (pColor->id == id) )
                return it;
	}
    return nullptr;
}

// Finds a simplex whose vertices are colored with UniqueIDs from a given set, if it exists
KSimplex* SimpComp::find_KSimplex(set<int> IDs){
    set<KSimplex*> sTemp;
    for(auto &id : IDs){
        KSimplex *temp = find_KSimplex(id);
        if(!temp)
            return nullptr;
        sTemp.insert(temp);
    }
    return find_vertices(sTemp);
}

// Verifies if a given simplex is an element of the complex or not
bool SimpComp::is_an_element(KSimplex* simp){

    if(simp==nullptr){
        log_report(LOG_WARN, "SimpComp::is_an_element(): You have provided a nullptr as a simplex, in order to verify if it belongs to a given complex. Nullptr is never an element of a complex.");
        log_report(LOG_WARN, "SimpComp::is_an_element(): I am skipping this, and answering that this simplex is not an element of the complex, but this input should never appear, check your code.");
        return false;
    }
    int level=simp->k;
    if( level > (this->D)){
        log_report(LOG_WARN, "SimpComp::is_an_element(): The level of the given simplex is greater than the dimension of the complex, so it cannot be an element of that complex.");
        log_report(LOG_WARN, "SimpComp::is_an_element(): I am skipping this, and answering that this simplex is not an element of the complex, but this should rarely happen (if ever), check your code.");
        return false;
    }    
    for(auto &it : this->elements[level])
        if( it == simp ) return true;

    return false;
}


// #####################################################################
// Reconstructing the neighbor relations between simplices in a complex:
// #####################################################################

// If all simplices have known vertex-neighbors, reconstruct all other neighbor relations
//
// IMPORTANT NOTE: This function works under the assumption that all simplices in the
// complex have appropriate vertex neighbors, i.e., neighbors->elements[0] is prefilled
// correctly, for every simplex. There is no easy way to validate this assumption within
// the function itself, so no validation is done --- you must take care yourself to
// satisfy this assumption before invoking the function. Otherwise its behavior is
// undefined --- the function *will do something*, but probably not what you expect.
// You have been warned...
//
// (this simply calls KSimplex::reconstruct_neighbors_from_vertices() for all simplices
// in the complex, see the corresponding KSimplex class function for further details)
bool SimpComp::reconstruct_neighbors_from_vertices(){
    for(int k = 0; k <= D; k++){
        for(auto &kSimplex : elements[k]){
            bool success = kSimplex->reconstruct_neighbors_from_vertices(this);
            if(!success)
	        return false;
        }
    }
    return true;
}


// ###################
// Printing functions:
// ###################

// Printing of the complex structure to stdout
// Wrapper for the print_detailed() function
void SimpComp::print(){
  this->print_detailed();
}


// Prints a total number of simplices at each level in a complex to stdout
// (probably will be deprecated soon --- use print_detailed() instead)
// TODO: mislim da nam ova f-ja ni za sta ne treba, obrisao bih je...
//void SimpComp::print_sizes(){
//    cout << endl << " --- Number of elements in " << name << " for each dimension ---" << endl;
//    for(int i = 0; i <= D; i++){
//        int n = count_number_of_simplices(i);
//        if(n){
//            cout << "Number of KSimplices at level " << i << ": " << n << endl; // counting edges
//        }else{
//            //cout << "Nothing on level " << i << "." << endl;
//        }
//    }
//    cout << endl;
//}


// Constructs a string with HTML code for printing all simplices in a complex,
// with the HTML syntax appropriate for the GUI
// (it is heavily used by the GUI, not useful otherwise)
string SimpComp::print_html(){
    string s = "";
    for(size_t k = 0; k < elements.size(); k++){
        s = s + "<b>Level " + to_string(k) + ":</b><br>";
        if(!elements[k].empty()){
            for(size_t i = 0; i < elements[k].size(); i++){
                s = s + " " + elements[k][i]->print_html();
            }
        }
        s = s + "<br>";
    }
    return s;
}


// Detailed printing of the whole simplicial complex structure, in a nice
// human-readable tabular format, to stdout
// Prints the name, dimension, topology and list of simplices at each level,
// and then for each simplex prints all colors with their values, and a list
// of neighbors of that simplex
// Relies on print_compact(), and works best if the entire complex is colored
// with UniqueIDColor (see UniqueIDColor::colorize_entire_complex() for details)
void SimpComp::print_detailed(){
    cout << endl;
    cout << "=================================================================" << endl;
    cout << "=================================================================" << endl;
    cout << "=======   _____________________________________________   =======" << endl;
    cout << "=======  /                                             \\  =======" << endl;
    cout << "=======  | Detailed printing of the simplicial complex |  =======" << endl;
    cout << "=======  \\_____________________________________________/  =======" << endl;
    cout << "=======                                                   =======" << endl;
    cout << "=================================================================" << endl;
    cout << "=================================================================" << endl;
    cout << endl;
    cout << "Name of the complex: " << name << endl;
    cout << "Dimension:           " << D << endl;
    cout << "Topology:            " << topology << endl;
    cout << "List of elements:" << endl;
    cout << "---------------------------------------------" << endl;
    print_compact();
    cout << "---------------------------------------------" << endl;
    cout << endl;
    for(size_t k = 0; k < elements.size(); k++){
        cout << "==============================================================" << endl;
        cout << "Printing details of simplices at level k = " << k << " of the complex" << endl;
        cout << "==============================================================" << endl << endl;
        for(size_t i = 0; i < elements[k].size(); i++) elements[k][i]->print_detailed();
        cout << endl;
    }
    cout << "=================================================================" << endl;
    cout << "=================================================================" << endl;
    cout << "=======   _____________________________________________   =======" << endl;
    cout << "=======  /                                             \\  =======" << endl;
    cout << "=======  | Finished printing of the simplicial complex |  =======" << endl;
    cout << "=======  \\_____________________________________________/  =======" << endl;
    cout << "=======                                                   =======" << endl;
    cout << "=================================================================" << endl;
    cout << "=================================================================" << endl;
    cout << endl;
}


// Prints a table of simplices in a complex, in a human-readable tabular
// format, to stdout
// Works best if the entire complex is colored with UniqueIDColor
void SimpComp::print_compact(){
  //    cout << endl << "Printing SimpComp " << name << ", D = " << D << endl;
    if(elements.empty())
        return;
    
    // vAll - all vertices are colored with UniqueIDColor:
    bool vAll = all_uniqueID(0);

    // sAll - all simplices at levels above zero are colored with UniqueIDColor:
    bool sAll = true;
    size_t level = 1;
    while( sAll && (level < elements.size()) )
        sAll &= all_uniqueID(level++);

    if(vAll){
        if(sAll){
            // vAll and sAll:
            // 14 (2-4-8), 15 (3-5-9), ...
            for(size_t k = 0; k < elements.size(); k++){
	      cout << "Simplices at level k = " << k << ":   ";
                if(!elements[k].empty()){
                    elements[k][0]->print_compact();
                    for(size_t i = 1; i < elements[k].size(); i++){
                        cout << ", ";
                        elements[k][i]->print_compact();
                    }
                    cout << endl;
                } else { cout << endl; }
            }
        }else{
            // vAll and not sAll:
            for(size_t k = 0; k < elements.size(); k++){
	      cout << "Simplices at level k = " << k << ":   ";
                if(!elements[k].empty()){
                    if(!k){ // print vertices:
                        // 14, 15, 18, ...
                        elements[k][0]->print_compact();
                        for(size_t i = 1; i < elements[k].size(); i++){
                            cout << ", ";
                            elements[k][i]->print_compact();
                        }
                        cout << endl;
                    }else{ // print simplices of level higher than zero:
                        // (2-4-8), (3-5-9), ...
                        bool first = true;
                        for(size_t i = 0; i < elements[k].size(); i++){
                            if(!first)
                                cout << ", ";
                            first = false;
                            elements[k][i]->neighbors->print_vertices_IDs_in_parentheses();
                        }
                        cout << endl;
                    }
                } else { cout << endl; }
            }
        }
    }else{
        // not vAll:
        // 8, 4, Simplex, 15, ...
        for(size_t k = 0; k < elements.size(); k++){
	  cout << "Simplices at level k = " << k << ":   ";
            bool first = true;
            for(size_t i = 0; i < elements[k].size(); i++){
                if(!first)
                    cout << ", ";
                first = false;
                UniqueIDColor* pColor = elements[k][i]->get_uniqueID();
                if(pColor){
                    cout << static_cast<UniqueIDColor*>(pColor)->id;
                }else{
                    cout << "Simplex";
                }
            }
            cout << endl;
        }
    }
}


// Helper function used by print_compact()
// Collect all vertices in a complex, and print their UniqueID numbers in parentheses, to stdout
// The output format looks like "(2-4-15-39)" or "(2-4-Simplex-Simplex)" or similar
// (it makes sense to apply it only to the neighbor structure of a KSimplex object)
void SimpComp::print_vertices_IDs_in_parentheses(){
  set<int> s;
  collect_vertices_IDs(s);
    if(s.size())
        print_set(s);
}


// Helper function used by print_vertices_IDs_in_parentheses()
// Given a set of UniqueID numbers, prints them in parentheses to stdout, padding
// them with the word "-Simplex" if some of the vertices do not have UniqueIDColor
// (it makes sense to apply it only to the neighbor structure of a KSimplex object)
void SimpComp::print_set(set<int> &s){
    int nNotUniqueID = elements[0].size() - s.size();
    cout << "(";
    bool first = true;
    for(auto itr = s.begin(); itr != s.end(); itr++){
        if(!first)
            cout << "-";
        first = false;
        cout << *itr;
    }
    for(int iSimp = 0; iSimp < nNotUniqueID; iSimp++)
        cout << "-Simplex";
    cout << ")";
}


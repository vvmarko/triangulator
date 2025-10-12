
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include "triangulator.hpp"

// #############################
// KSimplex class implementation
// #############################

// General information:
//
// Here are the implementations of the functions for the KSimplex class,
// declared in "ksimplex.hpp" file. See that file for the description
// of the purpose of each function, and see here (below) for how it
// works.


// ##########################################
// Constructors and destructors of a simplex:
// ##########################################

// Default constructor
KSimplex::KSimplex(int level, int dim){
    this->k = level;
    this->D = dim;
    neighbors = new(nothrow) SimpComp(dim);
}

// Default destructor
KSimplex::~KSimplex(){
  //  cout << "Deleting KSimplex... " << endl;
    for(auto pColor : colors)
        delete pColor;
    delete neighbors; // This is important for properly deallocating a simplex!
}

// #######################################################
// Finding and collecting various properties of a simplex:
// #######################################################

// Collect pointers to my neighboring vertices (zero-level neighbors) into a set
// (if I am a vertex myself, but put me into the set --- this is convenient
// for vertices, despite that no simplex is a neighbor of itself)
void KSimplex::collect_vertices(set<KSimplex*> &s){
    // If this k-simplex is a vertex, put it into the set:
    if(k == 0){
        s.insert(this);
		return;
	}
    // Otherwise, collect neighboring vertices into a set:
    for(auto &kSimplex : neighbors->elements[0]){
        s.insert(kSimplex);
    }
}

// Verify if a given simplex is my neighbor
// (true if it is found in my neighbor list, false otherwise)
bool KSimplex::find_neighbor(KSimplex *kSimplex){
    if(!kSimplex)
        return false;
    int k1 = kSimplex->k;
    for(auto &it : neighbors->elements[k1])
        if(it == kSimplex)
            return true;
    return false;
}

// Return a pointer to a first instance of my UniqueIDColor, if that color has
// been assigned to me
// (otherwise return nullptr)
UniqueIDColor* KSimplex::get_uniqueID(){
    for(auto pColor : colors)
        if(pColor->type == TYPE_UNIQUE_ID)
            return static_cast<UniqueIDColor*>(pColor);
    return nullptr;
}

// Verify if I live on the boundary of a complex
// (i.e. if the BoundaryColor is assigned to me)
bool KSimplex::is_a_boundary(){
  return Color::is_colorized_with_type(this,TYPE_BOUNDARY);
}

// Verify if I am an anchor for drawing
// (i.e. if the DrawingAnchorColor is assigned to me)
bool KSimplex::is_a_drawing_anchor(){
  return Color::is_colorized_with_type(this,TYPE_DRAWING_ANCHOR);
}


// #####################################
// Functions for manipulating neighbors:
// #####################################

// Establish a neighborhood relationship between a given simplex and myself
// NOTE: This is a powerful function, that does a lot of things:
//  - it adds the simplex into my list of neighbors (if it is not there already)
//  - it adds myself to the list of neighbors of that simplex (if I am not there already)
//  - if my level is smaller than the level of that simplex, add all my subsimplices as its neighbors
//  - if its level is smaller than mine, add all of its subsimplices as my neighbors
void KSimplex::add_neighbor(KSimplex *kSimplex){
  int k2;
    if(!kSimplex)
        return;
    int k1 = kSimplex->k; // extract dimension
    if (kSimplex==this){
      log_report(LOG_ERROR, "add_neighbor(): You have tried to add a simplex as a neighbor to itself! This is wrong and should never happen, skipping. Fix your code.");
      return;
    }
    if (k1==k){
      log_report(LOG_ERROR, "add_neighbor(): You have tried to connect two simplices of the same level as neighbors! This is wrong and should never happen, skipping. Fix your code.");
      return;
    }
    if(!find_neighbor(kSimplex)){ // if kSimplex is not already my neighbor
        neighbors->elements[k1].push_back(kSimplex); // add it as a neighbor
    }
    if(!kSimplex->find_neighbor(this)){ // if I am not already his neighbor
        kSimplex->neighbors->elements[k].push_back(this); // add me as his neigbhor as well
    }
    if(k1 && k>k1){ // if kSimplex is my subsimplex (k>k1) and is not a vertex (k1!=0), then add all his neighbor subsimplices (k1-1,...,0) as my neighbors:
      for (k2=0 ; k2<k1 ; k2++){
        for(auto &it : kSimplex->neighbors->elements[k2])
	  add_neighbor(it);
      }
    }
    if(k && k<k1){ // on the other hand, if I am a subsimplex of kSimplex (k<k1) and I am not a vertex (k!=0), then add all my neighbor subsimplices (k-1,...,0) as his neighbors:
      for (k2=0 ; k2<k ; k2++){
        for(auto &it : neighbors->elements[k2])
          kSimplex->add_neighbor(it);
      }
    }
}

// Disconnect a given kSimplex from my list of neighbors, and disconnect me from his list
// NOTE: This function is *not* a full inverse of add_neighbor(), since it does not
// disconnect the sub-neighbors of the two simplices (and it should not!)
void KSimplex::delete_neighbor(KSimplex* kSimplex){
    if(!kSimplex){
      log_report(LOG_WARN,"delete_neighbor(): You have asked me to remove nullptr from my list of neighbors.");
      log_report(LOG_WARN,"delete_neighbor(): I am skipping this, but it probably should not have happened, check your code.");
      return;
    }
    int level = kSimplex->k;
    // Find the position of kSimplex in neighbors->elements[level]:
    unsigned int i = 0;
    while( (i < neighbors->elements[level].size()) && (neighbors->elements[level][i] != kSimplex) )
        i++;
    // If not found, return silently (not an error!):
    if(i == neighbors->elements[level].size()) return;
    // If found but not last:
    if(i < neighbors->elements[level].size() - 1){
        // Copy the pointer of the last kSimplex onto position i:
        neighbors->elements[level][i] =
                neighbors->elements[level][ neighbors->elements[level].size() - 1 ];
    }
    // Remove the last element:
    neighbors->elements[level].pop_back();
    // Remove myself from the neighbors of kSimplex:
    kSimplex->delete_neighbor(this);
}

// Disconnect all simplices from my list of neighbors, and disconnect me from everyone else's lists
// NOTE: This function calls delete_neighbor() for all simplices in my list of neighbors, so
// the same caveats apply here as for the above function
void KSimplex::delete_all_neighbors(){
    for(long unsigned int i = 0; i < neighbors->elements.size(); i++){
      //        for(auto &it : neighbors->elements[i]){ // TODO: FIXME: Ovo mozda nece da obrise bas sve susede?
      //	  delete_neighbor(it); // Iterator prolazi kroz skup ciji elementi se menjaju u petlji, moze da preskoci neke od njih...
      //        }
      while(neighbors->elements[i].size())
	delete_neighbor(neighbors->elements[i][0]);
    }
}

// Reconstruct my neighbors table if my neighbor vertices are known
// NOTE: This function traverses the entire given complex, and adds as my neighbors all
// other simplices which have compatible vertex structure. Namely, it finds my
// sub-neighbor if all vertices of a given simplex are a subset of my simplices, and it
// finds my super-neighbor if all my vertices are a subset of a given simplex.
// NOTE: It is important to emphasize that this function implicitly assumes that
// each simplex in the complex has its vertices already assigned in its appropriate
// list of neighbors, otherwise it will fail to find some (or all) neighbors, and in
// addition it may also find false-positives, and assign neighbors that should not be
// neighbors. The assumption about vertices is semantic (it depends on the type of the
// complex in question) and thus it is impossible to verify. Therefore, you have to
// make sure yourself that this assumption is satisfied for your complex, before
// calling this function.
bool KSimplex::reconstruct_neighbors_from_vertices(SimpComp* simpComp){
	set<KSimplex*> s;
	collect_vertices(s);
	if(!s.size()){
          log_report(LOG_ERROR,"reconstruct_neighbors_from_vertices(): This simplex is not a vertex, but it also has no vertices assigned as its neighbors! Therefore I cannot reconstruct any other neighbors.");
          log_report(LOG_ERROR,"reconstruct_neighbors_from_vertices(): This should never happen --- every simplex (that is not a vertex itself) should have at least two vertices as its neighbors.");
          log_report(LOG_ERROR,"reconstruct_neighbors_from_vertices(): It appears that the structure of the complex " + simpComp->name + " is inconsistent. Fix your code!");
	  return false;
	}
	for(int tempK = 0; tempK <= simpComp->D; tempK++){
	    if(tempK!=k){ // skip level k in the loop, there must be no neighbors at that level
               for(auto &tempKSimplex : simpComp->elements[tempK]){
                   set<KSimplex*> tempS;
                   tempKSimplex->collect_vertices(tempS);
                   if( (subset(tempS, s)) || (subset(s, tempS)) )
                       add_neighbor(tempKSimplex);
	       }
	    }
	}
	return true;
}


// ###################
// Printing functions:
// ###################

// Printing of the simplex data to stdout
// Wrapper for print_detailed()
void KSimplex::print(){
  this->print_detailed();
}

// Prints the set of UniqueID colors of vertices of a given simplex,
// in a human-readable format, "50(2-13-44)" or "(2-13-44)" and similar, to stdout
// Works best if all vertices in the simplex are colored with UniqueIDColor
void KSimplex::print_compact(){
    UniqueIDColor* pColor = get_uniqueID();
    if(!pColor){
        cout << "simplex";
    }else{
        pColor->print_compact();
        if( k && (neighbors->elements[0].size()) )
	  neighbors->print_vertices_IDs_in_parentheses();
    }
}

// Detailed printing of the data of the simplex, in a nice human-readable
// tabular format, to stdout
// Prints the list of colors with their values, and a table of neighbors of a simplex
// Relies on print_compact(), and works best if the entire complex is colored
// with UniqueIDColor (see UniqueIDColor::colorize_entire_complex() for details)
void KSimplex::print_detailed(){
    UniqueIDColor* pColor = get_uniqueID();
    cout << "Simplex ID: ";
    if(pColor)
      pColor->print_compact();
    if( k && (neighbors->elements[0].size()) )
      neighbors->print_vertices_IDs_in_parentheses();
    cout << endl << endl;
    cout << "List of colors: " << endl;
    for(auto col : colors)
      cout << "color: " << get_color_name_from_type(col->type) << " (type " << col->type << "), value: " << col->get_color_value_as_str() << endl;
    cout << endl;
    cout << "Table of neighbors: " << endl;
    cout << "---------------------------------------------" << endl;
    neighbors->print_compact();
    cout << "---------------------------------------------" << endl << endl;
}

// Constructs a string for printing the HTML link for a simplex,
// with the syntax appropriate for the GUI
// (it is heavily used by the GUI, not useful otherwise)
string KSimplex::print_html(){
  string str = "";
    set<int> s;
    UniqueIDColor* pColor = get_uniqueID();
    if(!pColor){
      log_report(LOG_ERROR,"print_html(): HTML printing requires all simplices to have a UniqueID color! Apply UniqueIDColor::colorize_entire_complex() to your data.");
    }else{
    str = str + "[<a href=\"http://triangulatorgui.com/" + pColor->get_color_value_as_str() + ".html\">";
      if (k==0) {
	s.insert( static_cast<UniqueIDColor*>(pColor)->id );
      } else {
        neighbors->collect_vertices_IDs(s);
      }
      bool first = true;
      for(auto itr = s.begin(); itr != s.end(); itr++){
          if(!first)
              str = str + "-";
          first = false;
          str = str + to_string(*itr);
      }
      str = str + "</a>]";
    }
    return str;
}

// Constructs a string for printing the non-HTML name for a simplex,
// with the syntax appropriate for the GUI
// (it is heavily used by the GUI, not useful otherwise)
string KSimplex::print_non_html(){
  string str = "";
    set<int> s;
    UniqueIDColor* pColor = get_uniqueID();
    if(!pColor){
      log_report(LOG_ERROR,"print_non_html(): HTML-like printing requires all simplices to have a UniqueID color! Apply UniqueIDColor::colorize_entire_complex() to your data.");
    }else{
    str = str + "[";
      if (k==0) {
	s.insert( static_cast<UniqueIDColor*>(pColor)->id );
      } else {
        neighbors->collect_vertices_IDs(s);
      }
      bool first = true;
      for(auto itr = s.begin(); itr != s.end(); itr++){
          if(!first)
              str = str + "-";
          first = false;
          str = str + to_string(*itr);
      }
      str = str + "]";
    }
    return str;
}



// ##########################
// General support functions:
// ##########################

// Verify if a set s1 of simplices is a subset of the set s2 of simplices
// (an empty set s1 is a subset of any s2, even if s2 is also empty)
bool KSimplex::subset(set<KSimplex*> &s1, set<KSimplex*> &s2){
	for(auto x : s1){
		auto search = s2.find(x);
		if(search == s2.end())
        	return false;
	}
	return true;
}


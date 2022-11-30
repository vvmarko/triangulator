
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include "triangulator.hpp"

KSimplex::KSimplex(){
    k = 0;
    neighbors = new SimpComp(0);
}

KSimplex::KSimplex(int k, int D){
    this->k = k;
    this->D = D;
    neighbors = new SimpComp(D);
}

KSimplex::~KSimplex(){
	//cout << "Deleting KSimplex... ";
    for(auto pColor : colors)
        delete pColor;
}

bool KSimplex::find_neighbor(KSimplex *kSimplex){
    if(!kSimplex)
        return false;
    int k1 = kSimplex->k;
    for(auto &it : neighbors->elements[k1])
        if(it == kSimplex)
            return true;
    return false;
}

// By adding neighbor k1, function add_neighbor
// also adds all simplices of k1.
// Whenever a neighbor is added to current one,
// current one is also added to neighbor.
void KSimplex::add_neighbor(KSimplex *kSimplex){
  int k2;
    if(!kSimplex)
        return;
    int k1 = kSimplex->k; // extract dimension
    if (kSimplex==this){
      log_report(LOG_ERROR, "You have tried to add a simplex as a neighbor to itself! This is wrong and should never happen, skipping. Fix your code.");
      return;
    }
    if (k1==k){
      log_report(LOG_ERROR, "You have tried to connect two simplices of the same level as neighbors! This is wrong and should never happen, skipping. Fix your code.");
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


// Collects neighboring vertices into a set<int>:
void KSimplex::collect_vertices(set<KSimplex*> &s){
	// If this k-simplex is a vertex, return it:
    if(k == 0){
        s.insert(this);
		return;
	}
    
	// Otherwise, collect IDs of neighboring vertices:
    for(auto &kSimplex : neighbors->elements[0]){
        s.insert(kSimplex);
    }
}

void KSimplex::print(string space){
    cout << space << "Printing KSimplex: " << " k = " << k << ", D = " << D << endl;
    cout << space << "Printing colors:" << endl;
    for(auto &c : colors){
        c->print(space + "  ");
    }
	// Recursively prints neighbors:
    cout << space << "  Printing neighbors:" << endl;
    neighbors->print_compact();
}

UniqueIDColor* KSimplex::get_uniqueID(){
    for(auto pColor : colors)
        if(pColor->type == TYPE_UNIQUE_ID)
            return static_cast<UniqueIDColor*>(pColor);
    return nullptr;
}

void KSimplex::print_compact(){
    UniqueIDColor* pColor = get_uniqueID();
    if(!pColor){
        cout << "simplex";
    }else{
        pColor->print_compact();
        if( k && (neighbors->elements[0].size()) ){
            // empty ordered set container
            set<int> s;
            neighbors->print_vertices_IDs_in_parentheses(s);
        }
    }
}

void KSimplex::print_detailed(){
    UniqueIDColor* pColor = get_uniqueID();
    cout << "Simplex ID: ";
    if(pColor)
      pColor->print_compact();
    if( k && (neighbors->elements[0].size()) ){
            // empty ordered set container
            set<int> s;
            neighbors->print_vertices_IDs_in_parentheses(s);
    }
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

string KSimplex::print_html(){
  string str = "";
    set<int> s;
    UniqueIDColor* pColor = get_uniqueID();
    if(!pColor){
      log_report(LOG_ERROR,"HTML printing requires all simplices to have a UniqueID color! Apply UniqueIDColor::colorize_entire_complex() to your data.");
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


bool subset(set<KSimplex*> &s1, set<KSimplex*> &s2){
	for(auto x : s1){
		auto search = s2.find(x);
		if(search == s2.end())
        	return false;
	}
	return true;
}

// Delete given kSimplex from the list of neighbors of this k-simplex:
void KSimplex::delete_my_neighbor(KSimplex* kSimplex){
    int k = kSimplex->k;
    // Find the position of kSimplex in neighbors->elements[k]:
    unsigned int i = 0;
    while( (i < neighbors->elements[k].size()) && (neighbors->elements[k][i] != kSimplex) )
        i++;
    if(i == neighbors->elements[k].size())
        error("delete_neighbor: kSimplex not found on level "+to_string(k));
    // If not last:
    if(i < neighbors->elements[k].size() - 1){
        // Copy the pointer of the last kSimplex onto position i:
        neighbors->elements[k][i] =
                neighbors->elements[k][ neighbors->elements[k].size() - 1 ];
    }
    // Remove the last element:
    neighbors->elements[k].pop_back();
}

// Delete given kSimplex as neighbor, and delete this as kSimplex's neighbor:
void KSimplex::delete_neighbor(KSimplex* kSimplex){
    delete_my_neighbor(kSimplex);
    kSimplex->delete_my_neighbor(this);
}

void KSimplex::delete_all_neighbors(){
    for(int i = 0; i < neighbors->elements.size(); i++){
        for(auto &it : neighbors->elements[i]){
            delete_neighbor(it);
        }
    }
}

bool KSimplex::reconstruct_neighbors_from_vertices(){
	set<KSimplex*> s;
	collect_vertices(s);
	for(int tempK = 0; tempK < k; tempK++){
		for(auto &tempKSimplex : neighbors->elements[tempK]){
			set<KSimplex*> tempS;
			tempKSimplex->collect_vertices(tempS);
			if(subset(tempS, s))
				add_neighbor(tempKSimplex);
		}
	}
	return true;
}

bool KSimplex::is_a_boundary(){
    for(auto &color : colors)
        if(color->type == TYPE_BOUNDARY)
            return true;
    return false;
}

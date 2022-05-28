
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
    int k = kSimplex->k;
    for(auto &it : neighbors->elements[k])
        if(it == kSimplex)
            return true;
    return false;
}

// By adding neighbor k1, function add_neighbor
// also adds all simplices of k1.
// Whenever a neighbor is added to current one,
// current one is also added to neighbor.
void KSimplex::add_neighbor(KSimplex *kSimplex){
    if(!kSimplex)
        return;
    int k1 = kSimplex->k; // extract dimension
    if(!find_neighbor(kSimplex)){ // if doesn't exist already
        neighbors->elements[k1].push_back(kSimplex); // add it as a neighbor
    }
    if(!kSimplex->find_neighbor(this)) // add me as a neigbhor to it as well
        kSimplex->neighbors->elements[k].push_back(this);
    if(k1){ // recursivelly add neighbors at k-1,..0:
        for(auto &it : kSimplex->neighbors->elements[k1 - 1])
            add_neighbor(it);
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




//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include "triangulator.hpp"

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

SimpComp::SimpComp(int dim):
        name {""}, D{dim}{
    for(int i = 0; i <= D; i++){
//        list<KSimplex*> listaKSimpleksa;
        vector<KSimplex*> listaKSimpleksa;
        elements.push_back(listaKSimpleksa);
    }
    log_report(LOG_INFO, "initialize: Setting up everything for a new graph.");
}

SimpComp::SimpComp(string s, int dim):
        name {s}, D{dim}{
    for(int i = 0; i <= D; i++){
//        list<KSimplex*> listaKSimpleksa;
        vector<KSimplex*> listaKSimpleksa;
        elements.push_back(listaKSimpleksa);
    }
    log_report(LOG_INFO, "initialize: Setting up everything for a new graph.");
}

// Copy constructor - creating new SimpComp by copying existing one:
SimpComp::SimpComp(const SimpComp& simpComp){
	name = simpComp.name;
    D = simpComp.D;
    for(auto &row : simpComp.elements){
        vector<KSimplex*> newKSimplexList;
        for(auto &kSimplex : row)
            newKSimplexList.push_back(kSimplex);
        elements.push_back(newKSimplexList);
    }
}

SimpComp::~SimpComp(){
	//cout << "Deleting SimpComp... ";
    for(int i = 0; i <= D; i++){
        for(auto pKSimplex : elements[i]){
            delete(pKSimplex);
        }
    }
}

int SimpComp::count_number_of_simplexes(int level){
    return elements[level].size();
}

void SimpComp::print(string space){
    cout << space << "Printing SimpComp " << name << ", D = " << D << endl;
    for(size_t i = 0; i < elements.size(); i++){
        if(!elements[i].empty()){
            cout << space << "  Printing SimpComp " << name << " elements, level = " << i << ":" << endl;
            for(auto kSimplex : elements[i]){
                kSimplex->print(space + "    ");
            }
        }
    }
}

bool SimpComp::all_uniqueID(int level){
    size_t i = 0;
    if(level > D)
        return false;
    while( (i < elements[level].size()) && (elements[level][i]->get_uniqueID()) )
        i++;
    return i == elements[level].size();
}

// Collects neighboring vertices into a set<int>:
void SimpComp::collect_vertices(set<KSimplex*> &s){
    for(auto &kSimplex : elements[0]){
        s.insert(kSimplex);
    }
}

// Collects neighboring vertices IDs into a set<int>:
void SimpComp::collect_vertices_IDs(set<int> &s){
    for(auto &it : elements[0]){
        Color *pColor = it->get_uniqueID();
        if(pColor)
            s.insert( static_cast<UniqueIDColor*>(pColor)->id );
    }
}

// Finds a k-simplex with given vertices, if exists:
KSimplex* SimpComp::find_vertices(set<KSimplex*> &s){
	int kFind = s.size();
	if(kFind > D)
		return nullptr;
    for(auto &it : elements[kFind-1]){
		set<KSimplex*> sTemp;
        it->collect_vertices(sTemp);
        if(sTemp == s)
        	return it;
    }
    return nullptr;
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

bool SimpComp::reconstruct_neighbors_from_vertices(){
	for(int k = 1; k <= D; k++){
		for(auto &kSimplex : elements[k]){
			bool success = kSimplex->reconstruct_neighbors_from_vertices();
			if(!success)
				return false;
		}
	}
	return true;
}

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

void SimpComp::print_vertices_IDs_in_parentheses(set<int> &s){
    collect_vertices_IDs(s);
    if(s.size())
        print_set(s);
}

void SimpComp::print_compact(){
    cout << "Printing SimpComp " << name << ", D = " << D << endl;
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
                cout << "Simplices k = " << k << ":" << endl;
                if(!elements[k].empty()){
                    elements[k][0]->print_compact();
                    for(size_t i = 1; i < elements[k].size(); i++){
                        cout << ", ";
                        elements[k][i]->print_compact();
                    }
                    cout << endl;
                }
            }
        }else{
            // vAll and not sAll:
            for(size_t k = 0; k < elements.size(); k++){
                cout << "Simplices k = " << k << ":" << endl;
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

                            set<int> s; // empty ordered set container
                            UniqueIDColor* pColor = elements[k][i]->get_uniqueID();
                            if(pColor)
                                s.insert( static_cast<UniqueIDColor*>(pColor)->id );
                            elements[k][i]->neighbors->print_vertices_IDs_in_parentheses(s);
                        }
                        cout << endl;
                    }
                }
            }
        }
    }else{
        // not vAll:
        // 8, 4, Simplex, 15, ...
        for(size_t k = 0; k < elements.size(); k++){
            cout << "Simplices k = " << k << ":" << endl;
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

// Creating new KSimplex at level k:
KSimplex* SimpComp::create_ksimplex(int k){
    string s = "Creating KSimplex at level: " + k;
    log_report(LOG_INFO, s);
    if ( (k >= 0) && (k <= D) ){
        // Creating new KSimplex at level k:
        KSimplex *newKSimplex = new KSimplex(k, D);
        // Add newly created k-simplex to the this simplicial complex elements:
        elements[k].push_back(newKSimplex);
        return newKSimplex;
    }else{
    	cout << "k = " << k << "?!?" << endl;
        log_report(LOG_ERROR, "Adding KSimplex failed...");
        return nullptr;
    }
}

// Remove given simplex after disconnecting neighbors:
void SimpComp::remove_simplex(KSimplex *kSimplex){
    if(!kSimplex)
        error("remove_simplex: nullptr given.");
    int k = kSimplex->k;
    if(k > D)
        error("remove_simplex: k("+to_string(k)+")>D("+to_string(D)+")");

    // Find the position of kSimplex in neighbors->elements[k]:
    unsigned int i = 0;
    while( (i < elements[k].size()) && (elements[k][i] != kSimplex) )
        i++;
    if(i == elements[k].size())
        error("remove_simplex: kSimplex not found on level "+to_string(k));

    // If not last:
    if(i < elements[k].size() - 1){
        // Copy the pointer of the last kSimplex onto position i:
        elements[k][i] = elements[k][ elements[k].size() - 1 ];
    }

    // Remove the last element:
    elements[k].pop_back();

    // Delete all neigbhors from kSimplex:
    kSimplex->delete_all_neighbors();

    // Free the memory reserved by kSimplex:
    delete kSimplex;
}

void SimpComp::print_sizes(){
    cout << endl << " --- Number of elements in " << name << " for each dimension ---" << endl;
    for(int i = 0; i <= D; i++){
        int n = count_number_of_simplexes(i);
        if(n){
            cout << "Number of KSimplexes at level " << i << ": " << n << endl; // counting edges
        }else{
            //cout << "Nothing on level " << i << "." << endl;
        }
    }
    cout << endl;
}


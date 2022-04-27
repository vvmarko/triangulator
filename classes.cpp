
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
    for(auto pColor : colors)
        delete pColor;
    delete neighbors;
}

bool KSimplex::find_neighbor(KSimplex *k1){
    if(!k1)
        return false;
    int k = k1->k;
    for(auto &it : neighbors->elements[k])
        if(it == k1)
            return true;
    return false;
}

// By adding neighbor k1, function add_neighbor
// also adds all simplices of k1.
// Whenever a neighbor is added to current one,
// current one is also added to neighbor.
void KSimplex::add_neighbor(KSimplex *k1){
    if(!k1)
        return;
    int kK1 = k1->k; // extract dimension
    if(!find_neighbor(k1)) // if doesnt exist already
        neighbors->elements[kK1].push_back(k1); // add it as a neighbor
    if(!k1->find_neighbor(this)) // add me as a neigbhor to it as well
        k1->neighbors->elements[k].push_back(this);
    if(kK1){ // recursivelly add neighbors at k-1,..0:
        for(auto &it : k1->neighbors->elements[kK1 - 1])
            add_neighbor(it);
    }
}

// Collects neighboring vertices into a set<int>:
void KSimplex::collect_vertices(set<int> &s){
	// If this k-simplex is a vertex, return its ID:
    if(k == 0){
        Color *pColor = get_uniqueID();
        if(pColor)
            s.insert( static_cast<UniqueIDColor*>(pColor)->id );
		return;
	}
    
	// Otherwise, collect IDs of neighboring vertices:
    for(auto &it : neighbors->elements[0]){
        Color *pColor = it->get_uniqueID();
        if(pColor)
            s.insert( static_cast<UniqueIDColor*>(pColor)->id );
    }
}

void KSimplex::print(string space){
    cout << space << "Printing KSimplex: " << " k = " << k << ", D = " << D << endl;
//        cout << space << "Printing colors:" << endl;
    for(auto &c : colors){
        c->print(space + "  ");
    }
// Recursively prints neighbors:
//        cout << space << "  Printing neighbors:" << endl;
//        neighbors->print(space + "    ");
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
            neighbors->print_vertices_in_parentheses(s);
        }
    }
}

SimpComp::SimpComp(int dim):
        name {""}, D{dim}{
    for(int i = 0; i <= D; i++){
//            list<KSimplex*> listaKSimpleksa;
        vector<KSimplex*> listaKSimpleksa;
        elements.push_back(listaKSimpleksa);
    }
    log_report(LOG_INFO, "initialize: Setting up everything for a new graph.");
}

SimpComp::SimpComp(string s, int dim):
        name {s}, D{dim}{
    for(int i = 0; i <= D; i++){
//            list<KSimplex*> listaKSimpleksa;
        vector<KSimplex*> listaKSimpleksa;
        elements.push_back(listaKSimpleksa);
    }
    log_report(LOG_INFO, "initialize: Setting up everything for a new graph.");
}

/*
// Shallow copy, keeping original KSimplex pointers,
// without creating new KSimplex-es by copying existing ones:
SimpComp::SimpComp(const SimpComp& s){
    name = s.name;
    D = s.D;
    for(auto &pointers : elements){
        vector<KSimplex*> listaKSimpleksa;
        for(auto &kSimplex : pointers)
            listaKSimpleksa.push_back(kSimplex);
        elements.push_back(listaKSimpleksa);
    }
    vector< vector<KSimplex *> > elements;
}
*/
SimpComp::~SimpComp(){
    for(int i = 0; i <= D; i++){
        for(auto pKSimplex : elements[i])
            delete(pKSimplex);
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
void SimpComp::collect_vertices(set<int> &s){
    for(auto &it : elements[0]){
        Color *pColor = it->get_uniqueID();
        if(pColor)
            s.insert( static_cast<UniqueIDColor*>(pColor)->id );
    }
}

// Finds a k-simplex with given vertices, if exists:
KSimplex* SimpComp::find_vertices(set<int> &s){
	int kFind = s.size();
	if(kFind > D)
		return nullptr;
    for(auto &it : elements[kFind-1]){
		set<int> sTemp;
        it->collect_vertices(sTemp);
        if(sTemp == s)
        	return it;
    }
    return nullptr;
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

void SimpComp::print_vertices_in_parentheses(set<int> &s){
    collect_vertices(s);
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
                            elements[k][i]->neighbors->print_vertices_in_parentheses(s);
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
        KSimplex *newKSimplex = new KSimplex(k, D);
        elements[k].push_back(newKSimplex);
        return newKSimplex;
    }else{
        log_report(LOG_ERROR, "Adding KSimplex failed...");
        return nullptr;
    }
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


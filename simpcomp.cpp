
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include <algorithm>
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
	int kFound = s.size() - 1;
	if(kFound > D)
		return nullptr;
    for(auto &it : elements[kFound]){
		set<KSimplex*> sTemp;
        it->collect_vertices(sTemp);
        if(sTemp == s)
        	return it;
    }
    return nullptr;
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

// Finds a k-simplex with given ID, if exists:
KSimplex* SimpComp::find_KSimplex(size_t id){
    for(auto &it : elements[0]){
        for(auto pColor : it->colors)
            if(pColor->type == TYPE_UNIQUE_ID)
                if((static_cast<UniqueIDColor*>(pColor))->id == id)
                    return it;
    }
    return nullptr;
}

// Finds a k-simplex with given IDs, if exists:
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

// Deletes a k-simplex with given IDs, if exists:
void SimpComp::delete_KSimplex(set<int> IDs){
    int kFound = IDs.size() - 1;
	if(kFound > D)
		return;

    KSimplex* toDelete = find_KSimplex(IDs);
    if(!toDelete) error("No KSimplex to delete!");

    toDelete->delete_all_neighbors();

    auto &vec = elements[kFound];
    vec.erase(std::remove(vec.begin(), vec.end(), toDelete), vec.end());    

    delete(toDelete);
    toDelete = nullptr;
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


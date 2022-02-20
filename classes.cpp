
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include "classes.hpp"

void log_report(int type, string message){
    if(type <= logLevel){
        if(type == LOG_ERROR) cerr << "ERROR:   " << message << endl;
        if(type == LOG_WARN) cerr << "WARNING: " << message << endl;
        if(type == LOG_INFO) cerr << "INFO:    " << message << endl;
        if(type == LOG_DEBUG) cerr << "DEBUG:   " << message << endl;
    }
};

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

void KSimplex::add_neighbor_couple(KSimplex *k1){
    if(!k1)
        return;
    int kK1 = k1->k;
    if(!find_neighbor(k1))
        neighbors->elements[kK1].push_back(k1);
    if(!k1->find_neighbor(this))
        k1->neighbors->elements[k].push_back(this);
}

void KSimplex::add_neighbor(KSimplex *k1){
    if(!k1)
        return;
    int kK1 = k1->k;
    add_neighbor_couple(k1);
    if(kK1){
        for(auto &it : k1->neighbors->elements[kK1 - 1])
            add_neighbor(it);
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

void KSimplex::print_compact(){
    bool hasUniqueColor = false;
    for(auto pColor : colors){
        if(pColor->type == TYPE_UNIQUE_ID){
            hasUniqueColor = true;
            pColor->print_compact();
            if( k && (neighbors->elements[0].size()) ){
                // empty ordered set container
                set<int> s;
                int nNotUniqueID = neighbors->elements[0].size();
                for(auto &it : neighbors->elements[0]){
                    for(auto pC : it->colors){
                        if(pC->type == TYPE_UNIQUE_ID){
                            nNotUniqueID--;
                            s.insert( static_cast<UniqueIDColor*>(pC)->id );
                        }
                    }
                }
                if(s.size()){
                    cout << " (";
                    bool first = true;
                    for(auto itr = s.begin(); itr != s.end(); itr++){
                        if(!first)
                            cout << "-";
                        first = false;
                        cout << *itr;
                    }
                    for(int iSimp = 0; iSimp < nNotUniqueID; iSimp++) //TODO:FIX // MV: Fixed... :-)
                        cout << "-Simplex";
                    cout << ")";
                }
            }
        }
    }
    if(!hasUniqueColor)
        cout << "simplex";
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
    for(int i = 0; i < elements.size(); i++){
        if(!elements[i].empty()){
            cout << space << "  Printing SimpComp " << name << " elements, level = " << i << ":" << endl;
            for(auto kSimplex : elements[i]){
                kSimplex->print(space + "    ");
            }
        }
    }
}

void SimpComp::print_compact(){
    cout << "Printing SimpComp " << name << ", D = " << D << endl;
    for(int k = 0; k < elements.size(); k++){
        cout << "Simplices k = " << k << ":" << endl;
        if(!elements[k].empty()){
            elements[k][0]->print_compact();
            for(int i = 1; i < elements[k].size(); i++){
                cout << ", ";
                elements[k][i]->print_compact();
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

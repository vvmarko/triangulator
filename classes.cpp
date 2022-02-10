
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

#include "classes.hpp"

void log_report(int type, string message){
    if (type <= loglevel){
        if (type==1) cerr << "ERROR:   " << message << endl;
        if (type==2) cerr << "WARNING: " << message << endl;
        if (type==3) cerr << "INFO:    " << message << endl;
        if (type==4) cerr << "DEBUG:   " << message << endl;
    }
};

KSimplex::KSimplex(){
    k = 0;
    name = "p";
    neighbors = new SimpComp(0);
}

KSimplex::KSimplex(int k, int D, string name){
    this->k = k;
    this->D = D;
    this->name = name;
    neighbors = new SimpComp(D);
}

KSimplex::KSimplex(KSimplex *v1, KSimplex *v2){
    if((v1) && (v2)){
        name = v1->name + "-" + v2->name;
        this->D = v1->D;
        neighbors = new SimpComp(D);
        add_neighbor(v1);
        add_neighbor(v2);
    }
}

KSimplex::~KSimplex(){
    delete neighbors;
}

void KSimplex::add_neighbor(KSimplex *k1){
    if(!k1)
        return;
    int k = k1->k;
    neighbors->elements[k].push_back(k1);
}

void KSimplex::print(string space){
    cout << space << "Printing KSimplex " << name << ", k = " << k << ", D = " << D << endl;
//        cout << space << "Printing colors:" << endl;
    for(auto &c : colors){
        c->print(space + "  ");
    }
// Recursively prints neighbors:
//        cout << space << "  Printing neighbors:" << endl;
//        neighbors->print(space + "    ");
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

// Adding new KSimplex at level k:
KSimplex* SimpComp::add_ksimplex(int k, string name){
    if(loglevel >= LOG_INFO){
        cout << "Adding KSimplex at level :" << k << endl;
    }
    if ( (k >= 0) && (k <= D) ){
        KSimplex *newKSimplex = new KSimplex(k, D, name);
        elements[k].push_back(newKSimplex);
        return newKSimplex;
    }else{
        log_report(LOG_ERROR, "Adding KSimplex failed...");
        return nullptr;
    }
}

// Adding new KSimplex at level k:
KSimplex* SimpComp::add_ksimplex(KSimplex *newKSimplex){
    if(!newKSimplex)
        return nullptr;
    int k = newKSimplex->k;
    D = newKSimplex->D;
    if(loglevel >= LOG_INFO){
        cout << "Adding KSimplex at level :" << k << endl;
    }
    if ( (k >= 0) && (k <= D) ){
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

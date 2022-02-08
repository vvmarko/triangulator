
//TODO:
// - check arguments against graph levels, invalid values, null pointers, etc.

int loglevel = 2; //TODO
#define LOG_QUIET 0 // quiet
#define LOG_ERROR 1 // errors
#define LOG_WARN 2 // errors and warnings
#define LOG_INFO 3 // errors, warnings and info
#define LOG_DEBUG 4 // errors, warnings, info and debug

// Special colors will have type between MIN_SPECIAL and MAX_SPECIAL:
#define MIN_SPECIAL 0
#define MAX_SPECIAL 127
// Built-in colors will have type between MIN_BUILT_IN and MAX_BUILT_IN:
#define MIN_BUILT_IN MAX_SPECIAL + 1
#define MAX_BUILT_IN 1023
// User defined colors will have type between MIN_USER_DEFINED and MAX_USER_DEFINED:
#define MIN_USER_DEFINED MAX_BUILT_IN + 1
#define MAX_USER_DEFINEED 65535

#define TYPE_BOUNDARY 0
#define TYPE_SCREEN_COORDINATE 1
#define TYPE_COUNTER 140
#define TYPE_UNSIGNED_DOUBLE 265
#define TYPE_TRIPLE_OF_INTEGERS 440
#define TYPE_COORDINATES_FOR_EMBEDDING 813


void log_report(int type, string message){
    if (type <= loglevel){
        if (type==1) cerr << "ERROR:   " << message << endl;
        if (type==2) cerr << "WARNING: " << message << endl;
        if (type==3) cerr << "INFO:    " << message << endl;
        if (type==4) cerr << "DEBUG:   " << message << endl;
    }
};

class Color{
public:
    int type;
    virtual void print(string space = ""){
//        cout << space << "Color type = " << type << endl;
    }
};
class BoundaryColor : public Color{
public:
    BoundaryColor(bool boundary){
        type = TYPE_BOUNDARY;
        this->boundary = boundary;
    }
    void print(string space = ""){
        Color::print(space);
        cout << space << "Boundary = " << (boundary?"true":"false") << endl;
    }
    bool boundary; // whether the KSimplex of this color represents a boundary
};

class ScreenCoordinateColor : public Color{
public:
    ScreenCoordinateColor(){
        type = TYPE_SCREEN_COORDINATE;
    }
    ScreenCoordinateColor(int x, int y){
        type = TYPE_SCREEN_COORDINATE;
        this->x = x;
        this->y = y;
    }
    void print(string space = ""){
        Color::print(space);
        cout << space << "coordinate(" << x << ", " << y << ")" << endl;
    }
    int x, y; // screen coordinates
};



class KSimplex;
class SimpComp{
public:
    SimpComp(int dim);
    SimpComp(string s, int dim);
    ~SimpComp();
    int count_number_of_simplexes(int level);
    void print(string space = "");
    // Adding new KSimplex at level k:
    KSimplex* add_ksimplex(int k, string name = "");
    // Adding new KSimplex at level k:
    KSimplex* add_ksimplex(KSimplex *newKSimplex);
    void print_sizes();

    string name;
    int D;
    // An element at each level
    // is a list or vector of KSimplex pointers to KSimplex on that level:
//    vector< list<KSimplex *> > elements;
    vector< vector<KSimplex *> > elements;
};

// K simpleks of given dimension and level k, with colors:
class KSimplex{
public:
    KSimplex(){
        k = 0;
        name = "p";
        neighbors = new SimpComp(0);
    }
    KSimplex(int k, int D, string name = ""){
        this->k = k;
        this->D = D;
        this->name = name;
        neighbors = new SimpComp(D);
    }
    KSimplex(KSimplex *v1, KSimplex *v2){
        if((v1) && (v2)){
            name = v1->name + "-" + v2->name;
            this->D = v1->D;
            neighbors = new SimpComp(D);
            add_neighbor(v1);
            add_neighbor(v2);
        }
    }
    ~KSimplex(){
        delete neighbors;
    }

    void add_neighbor(KSimplex *k1){
        if(!k1)
            return;
        int k = k1->k;
        neighbors->elements[k].push_back(k1);
    }
    
    void print(string space = ""){
        cout << space << "Printing KSimplex " << name << ", k = " << k << ", D = " << D << endl;
//        cout << space << "Printing colors:" << endl;
        for(auto &c : colors){
            c->print(space + "  ");
        }
// Recursively prints neighbors:
//        cout << space << "  Printing neighbors:" << endl;
//        neighbors->print(space + "    ");
    }

    int k; // level
    int D; // dimension
    string name;
    SimpComp *neighbors;
    vector<Color *> colors; // colors of this KSimplex
};




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
/*
// Find n-th simplex in the simplexlist:
KSimplex* SimpComp::find_simplex(int n, int level){
    auto &lista = elements[level];
    if(elements.size()<n){
        log_report(LOG_ERROR, "find_simplex: Null pointer returned! No elements at required level");
        return nullptr;
    }
    if(n < 1){ // TODO: check 1 or 0
        log_report(LOG_ERROR, "find_simplex: Null pointer returned! Invalid index given.");
        return nullptr;
    }

    if (lista.size() > n){
        auto it = next(lista.begin(), n);
        return *it;
    }else{
        log_report(LOG_ERROR, "find_simplex: Null pointer vertex returned! List of elements shorter than expected!");
        return nullptr;
    }
}
*/    



#include <iostream>
#include <vector>
#include <list>

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

#include "color.cpp"


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
    KSimplex();
    KSimplex(int k, int D, string name = "");
    KSimplex(KSimplex *v1, KSimplex *v2);
    ~KSimplex();
    void add_neighbor(KSimplex *k1);    
    void print(string space = "");

    int k; // level
    int D; // dimension
    string name;
    SimpComp *neighbors;
    vector<Color *> colors; // colors of this KSimplex
};

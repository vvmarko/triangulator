
#include <iostream>
#include <set>
#include <vector>
#include <list>

#include "constants.hpp"
#include "color.cpp"

class KSimplex;
class SimpComp{
public:
    SimpComp(int dim);
    SimpComp(string s, int dim);
//    SimpComp(const SimpComp& s);
    ~SimpComp();
    int count_number_of_simplexes(int level);
    void print(string space = "");
    void print_compact();
    // Creating new KSimplex at level k:
    KSimplex* create_ksimplex(int k);
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
    KSimplex(int k, int D);
    ~KSimplex();
    // Searches for k1 in neighbor elements:
    bool find_neighbor(KSimplex *k1);
    // Adds k1 as neigbor and this as k1's neighbor:
    void add_neighbor_couple(KSimplex *k1);
    // Adds k1 and its lower level neigbors as neighbors and the opposite:
    void add_neighbor(KSimplex *k1);    
    void print(string space = "");
    void print_compact();

    int k; // level
    int D; // dimension
    SimpComp *neighbors;
    vector<Color *> colors; // colors of this KSimplex
};


// ######################
// Main class definitions
// ######################


class KSimplex;
class SimpComp{
public:
    SimpComp(int dim);
    SimpComp(string s, int dim);
//    SimpComp(const SimpComp& s);
    ~SimpComp();
    int count_number_of_simplexes(int level);
    void print(string space = "");
    // All KSimplex-es at level colored with UniqueIDColor:
    bool all_uniqueID(int level);
    void print_compact();
    // Creating new KSimplex at level k:
    KSimplex* create_ksimplex(int k);
    void print_sizes();
    // Collects all vertices into a set<int>:
    void collect_vertices(set<int> &s);

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
    // Adds k1 and its lower level neigbors as neighbors and the opposite:
    void add_neighbor(KSimplex *k1);    
    void print(string space = "");
    UniqueIDColor* get_uniqueID();
    void print_compact();
    // Collects neighboring vertices into a set<int>:
    void collect_vertices(set<int> &s);

    int k; // level
    int D; // dimension
    SimpComp *neighbors;
    vector<Color *> colors; // colors of this KSimplex
};


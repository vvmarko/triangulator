
// ######################
// Main class definitions
// ######################

std::ostream &operator<<(std::ostream &os, set<int> &s);

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
    // Collects all vertices into a set<int>:
    void collect_vertices(set<KSimplex*> &s);
    // Collects all vertices IDs into a set<int>:
    void collect_vertices_IDs(set<int> &s);
	// Finds a k-simplex with given vertices, if exists:
	KSimplex* find_vertices(set<KSimplex*> &s);
    void print_set(set<int> &s);
    void print_vertices_IDs_in_parentheses(set<int> &s);
    void print_compact();
    // Creating new KSimplex at level k:
    KSimplex* create_ksimplex(int k);
    // Remove given simplex after disconnecting neighbors:
    void remove_simplex(KSimplex* kSimplex);
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
    // Adds k1 and its lower level neigbors as neighbors and the opposite:
    void add_neighbor(KSimplex *k1);    
	// Collects neighboring vertices into a set<int>:
	void collect_vertices(set<KSimplex*> &s);
    void print(string space = "");
    // Searches for the first unique ID color:
    UniqueIDColor* get_uniqueID();
    void print_compact();
	static bool reconstruct_neighbors_from_vertices(SimpComp *simpComp);
    // Delete given kSimplex from the list of neighbors of this k-simplex:
    void delete_my_neighbor(KSimplex* kSimplex);
    // Delete given kSimplex as neighbor, and delete this as kSimplex's neighbor:
    void delete_neighbor(KSimplex* kSimplex);
    void delete_all_neighbors();
    
    int k; // level
    int D; // dimension
    SimpComp *neighbors;
    vector<Color *> colors; // colors of this KSimplex
};

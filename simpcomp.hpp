
// #########################
// SimpComp class definition
// #########################

std::ostream &operator<<(std::ostream &os, set<int> &s); // MV: Sta je ovo i cemu sluzi? 

class KSimplex;
class SimpComp{
public:
    SimpComp(int dim);
    SimpComp(string SimpCompName, int dim);
    SimpComp(const SimpComp& simpComp);
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
    bool reconstruct_neighbors_from_vertices();
    // Finds a k-simplex with given given ID, if exists:
    KSimplex* find_KSimplex(size_t id);
    // Finds a k-simplex with given IDs, if exists:
    KSimplex* find_KSimplex(set<int> IDs);
    // Deletes a k-simplex with given IDs, if exists:
    void delete_KSimplex(set<int> IDs);
    void print_set(set<int> &s);
    void print_vertices_IDs_in_parentheses(set<int> &s);
    void print_compact();
    void print_detailed();
    string print_html();
    // Creating new KSimplex at level k:
    KSimplex* create_ksimplex(int k);
    // Creates new KSimplex given a set of vertices:
    KSimplex* create_ksimplex_from_vertices(set<KSimplex*> &s);
    // Remove given simplex after disconnecting neighbors:
    void remove_simplex(KSimplex* kSimplex);
    void print_sizes();

    string name;
    string topology;
    int D;
    // An element at each level
    // is a list or vector of KSimplex pointers to KSimplex on that level:
//    vector< list<KSimplex *> > elements;
    vector< vector<KSimplex *> > elements;
};


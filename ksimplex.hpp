
// #########################
// KSimplex class definition
// #########################

// k-simplex of given dimension and level k, with colors:
class SimpComp;
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
    // Reconstruct the neighbors table if neighbor vertices are known:
    bool reconstruct_neighbors_from_vertices(SimpComp *simpComp);
    // Determines whether BoundaryColor color exists:
    bool is_a_boundary();
    // Print a k-simplex by listing IDs of its vertices:
    void print_compact();
    // Print a k-simplex by listing all of its details and neighbors:
    void print_detailed();
    // Print a k-simplex in HTML format, for gui:
    string print_html();
    // Delete given kSimplex from the list of neighbors of this k-simplex:
    void delete_my_neighbor(KSimplex* kSimplex);
    // Delete given kSimplex as neighbor, and delete this as kSimplex's neighbor:
    void delete_neighbor(KSimplex* kSimplex);
    // Delete all neighbors:
    void delete_all_neighbors();
    
    int k; // level
    int D; // dimension
    SimpComp *neighbors;
    vector<Color *> colors; // colors of this KSimplex
};

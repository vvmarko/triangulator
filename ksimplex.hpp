
// #########################
// KSimplex class definition
// #########################

// General information:
//
// Class KSimplex is the second fundamental class of the whole
// Triangulator library and derived userspace apps. It contains
// the basic description of an individual simplex as an element
// of a given simplicial complex, by keeping track of its level
// (i.e. its intrinsic dimension), the dimension of the corresponding
// simplicial complex, a list of pointers to the colors assigned to it,
// and lists of pointers to all k-simplices which are its neighbors
// (for all levels k=0,...,D).
//
// As a rule, objects of the SimpComp class should be allocated
// and deallocated using various high-level functions, such as seed
// functions, Pachner move functions, or functions that attach a new
// simplex to the boundary of a given complex, or yet higher-level
// functions that automatically "grow" a complex to a given size etc.
// You should *never* need to instantiate a SimpComp object using a
// low-level constructor, nor delete it using a low-level destructor.
// Even if you are doing something low-level, such as writing a new
// seed function, you should instantiate new simplices by invoking
// SimpComp::create_ksimplex() and similar, and delete existing
// simplices by invoking SimpComp::remove_simplex(). See the file
// "simpcomp.hpp" for details.
//
// Other than that, you will be working with, using and manipulating
// KSimplex objects all the time --- the whole purpose of the
// Triangulator library is to facilitate various tools for that.

class SimpComp; // A necessary forward declaration of the SimpComp class

class KSimplex{
public:

  // ###############################
  // Data structures of the simplex:
  // ###############################

  // Level of the simplex (its intrinsic dimension):
    int k;
  // Dimension of the complex containing this simplex:
    int D;
  // A small simplicial complex of all neighbors of the simplex:
    SimpComp *neighbors;
  // A list of pointers to the colors assigned to the simplex:
    vector<Color *> colors; // colors of this KSimplex


  // ##########################################
  // Constructors and destructors of a simplex:
  // ##########################################

  // Default constructor, specifying level and dimension
    KSimplex(int k, int D);

  // Dummy constructor
  // TODO: FIXME: Ovo ne bi smelo nigde i nikad da se koristi, predlazem da ga obrisemo...
    KSimplex();

  // Default destructor
    ~KSimplex();


  // #######################################################
  // Finding and collecting various properties of a simplex:
  // #######################################################

  // Collect pointers to my neighboring vertices (zero-level neighbors) into a set
  // (if I am a vertex myself, but put me into the set --- this is convenient
  // for vertices, despite that no simplex is a neighbor of itself)
    void collect_vertices(set<KSimplex*> &s);

  // Verify if a given simplex is my neighbor
  // (true if it is found in my neighbor list, false otherwise)
    bool find_neighbor(KSimplex *kSimplex);

  // Return a pointer to a first instance of my UniqueIDColor, if that color has
  // been assigned to me
  // (otherwise return nullptr)
    UniqueIDColor* get_uniqueID();

  // Verify if I live on the boundary of a complex
  // (i.e. if the BoundaryColor is assigned to me)
    bool is_a_boundary();


  // #####################################
  // Functions for manipulating neighbors:
  // #####################################

  // Establish a neighborhood relationship between a given simplex and myself
  // NOTE: This is a powerful function, that does a lot of things:
  //  - it adds the simplex into my list of neighbors (if it is not there already)
  //  - it adds myself to the list of neighbors of that simplex (if I am not there already)
  //  - if my level is smaller than the level of that simplex, add all my subsimplices as its neighbors
  //  - if its level is smaller than mine, add all of its subsimplices as my neighbors
    void add_neighbor(KSimplex *kSimplex);

  // Disconnect a given kSimplex from my list of neighbors, and disconnect me from his list
  // NOTE: This function is *not* a full inverse of add_neighbor(), since it does not
  // disconnect the sub-neighbors of the two simplices (and it should not!)
    void delete_neighbor(KSimplex* kSimplex);

  // Disconnect all simplices from my list of neighbors, and disconnect me from everyone else's lists
  // NOTE: This function calls delete_neighbor() for all simplices in my list of neighbors, so
  // the same caveats apply here as for the above function
    void delete_all_neighbors();

  // Reconstruct my neighbors table if my neighbor vertices are known
  // NOTE: This function traverses the entire given complex, and adds as my neighbors all
  // other simplices which have compatible vertex structure. Namely, it finds my
  // sub-neighbor if all vertices of a given simplex are a subset of my simplices, and it
  // finds my super-neighbor if all my vertices are a subset of a given simplex.
  // NOTE: It is important to emphasize that this function implicitly assumes that
  // each simplex in the complex has its vertices already assigned in its appropriate
  // list of neighbors, otherwise it will fail to find some (or all) neighbors, and in
  // addition it may also find false-positives, and assign neighbors that should not be
  // neighbors. The assumption about vertices is semantic (it depends on the type of the
  // complex in question) and thus it is impossible to verify. Therefore, you have to
  // make sure yourself that this assumption is satisfied for your complex, before
  // calling this function.
    bool reconstruct_neighbors_from_vertices(SimpComp *simpComp);
  

  // ###################
  // Printing functions:
  // ###################

  // Rudimentary printing of the simplex data to stdout
  // (probably will be deprecated soon --- use print_detailed() instead)
  // TODO: ja bih ovo prepravio u wrapper za print_detailed() ili sl.
    void print(string space = "");

  // Prints the set of UniqueID colors of vertices of a given simplex,
  // in a human-readable format, "50(2-13-44)" or "(2-13-44)" and similar, to stdout
  // Works best if all vertices in the simplex are colored with UniqueIDColor
    void print_compact();

  // Detailed printing of the data of the simplex, in a nice human-readable
  // tabular format, to stdout
  // Prints the list of colors with their values, and a table of neighbors of a simplex
  // Relies on print_compact(), and works best if the entire complex is colored
  // with UniqueIDColor (see UniqueIDColor::colorize_entire_complex() for details)
    void print_detailed();

  // Constructs a string for printing the HTML link for a simplex,
  // with the syntax appropriate for the GUI
  // (it is heavily used by the GUI, not useful otherwise)
    string print_html();

  // ##########################
  // General support functions:
  // ##########################

  // Verify if a set s1 of simplices is a subset of the set s2 of simplices
  // (an empty set s1 is a subset of any s2, even if s2 is also empty)
  bool subset(set<KSimplex*> &s1, set<KSimplex*> &s2);

};


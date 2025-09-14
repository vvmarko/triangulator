
#ifndef TRIANGULATOR_SIMPCOMP_H_INCLUDED
#define TRIANGULATOR_SIMPCOMP_H_INCLUDED

// #########################
// SimpComp class definition
// #########################

// General information:
//
// Class SimpComp is the first fundamental class of the whole
// Triangulator library and derived userspace apps. It contains
// the basic description of a simplicial complex corresponding to
// a given manifold, by keeping track of its name, topology,
// dimension, and lists of pointers to all k-simplices which are
// elements of the complex (for all levels k=0,...,D).
//
// As a rule, objects of the SimpComp class should be allocated
// and deallocated using various high-level seed- and unseed-
// functions (see the file "seed.hpp" for details). You should not
// instantiate a SimpComp object using a low-level constructor,
// unless you know what you are doing (for example, writing a
// new seed function, or something to that effect).
//
// Other than that, you will be working with, using and manipulating
// SimpComp objects all the time --- the whole purpose of the
// Triangulator library is to facilitate various tools for that.


// TODO: ovo bismo trebali da premestimo negde drugo, nije mu mesto ovde...
std::ostream &operator<<(std::ostream &os, set<int> &s);

class KSimplex; // A necessary forward declaration of the KSimplex class

class SimpComp{
public:

  // ##########################################
  // Data structures of the simplicial complex:
  // ##########################################

  // Name of the complex (user-specified):
    string name;
  // Topology of the complex (specified automatically):
    string topology;
  // Dimension of the complex (user-specified):
    int D;
  // Vectors of pointers to k-simplices, arranged into
  // a vector of levels k = 0,...,D:
    vector< vector<KSimplex *> > elements;

  
  // ##########################################
  // Constructors and destructors of a complex:
  // ##########################################

  // Default constructor, specifying name and dimension
    SimpComp(string SimpCompName, int dim);

  // No-name constructor (used for neighbor structure in the KSimplex class)
    SimpComp(int dim);

  // Default destructor
    ~SimpComp();


  // ###########################################
  // Adding and removing simplices in a complex:
  // ###########################################

  // Creates a new simplex of level k, and adds it to the complex
    KSimplex* create_ksimplex(int k);

  // Given a set of vertices, creates a new simplex in the complex,
  // and assigns those vertices as its neighbors
    KSimplex* create_ksimplex_from_vertices(set<KSimplex*> &s);
  
  // Removes a given simplex from a complex, by disconnecting it
  // from all of its neighbors and then deleting it
    void remove_simplex(KSimplex* kSimplex);

  // Removes a simplex with given UniqueIDs of its vertices, if it exists,
  // by disconnecting it from all of its neighbors and then deleting it
    void remove_simplex(set<int> IDs);

  
  // #################################
  // Functions for sizes and counting:
  // #################################
  
  // Counts the number of simplices of a given level within the complex
    int count_number_of_simplices(int level);

  
  // ##########################################
  // Verifying various properties of a complex:
  // ##########################################
  
  // Verify if all simplices of a given level are colored with UniqueIDColor
    bool all_uniqueID(int level);

  
  // #########################################################
  // Finding and collecting simplices with various properties:
  // #########################################################

  // Collect all vertices (simplices of level=0) of a complex into a set
  // (this is typically used over a complex of neighbors, see KSimplex class)
    void collect_vertices(set<KSimplex*> &s);

  // Collect UniqueID numbers for all vertices (that have UniqueIDColor) into a set
  // (similar to above, typically used over a complex of neighbors of a KSimplex)
    void collect_vertices_IDs(set<int> &s);

  // Finds a simplex which contains precisely the given set of vertices, if it exists
    KSimplex* find_vertices(set<KSimplex*> &s);

  // Finds a simplex with a given UniqueID number, if it exists
    KSimplex* find_KSimplex(size_t id);

  // Finds a simplex whose vertices are colored with UniqueIDs from a given set, if it exists
    KSimplex* find_KSimplex(set<int> IDs);

  // Verifies if a given simplex is an element of the complex or not
    bool is_an_element(KSimplex* simp);


  // #####################################################################
  // Reconstructing the neighbor relations between simplices in a complex:
  // #####################################################################

  // If all simplices have known vertex-neighbors, reconstruct all other neighbor relations
  //
  // IMPORTANT NOTE: This function works under the assumption that all simplices in the
  // complex have appropriate vertex neighbors, i.e., neighbors->elements[0] is prefilled
  // correctly, for every simplex. There is no easy way to validate this assumption within
  // the function itself, so no validation is done --- you must take care yourself to
  // satisfy this assumption before invoking the function. Otherwise its behavior is
  // undefined --- the function *will do something*, but probably not what you expect.
  // You have been warned...
  //
  // (this simply calls KSimplex::reconstruct_neighbors_from_vertices() for all simplices
  // in the complex, see the corresponding KSimplex class function for further details)
  bool reconstruct_neighbors_from_vertices();
  

  // ###################
  // Printing functions:
  // ###################

  // Printing of the complex structure to stdout
  // Wrapper for the print_detailed() function
    void print();

  // Constructs a string with HTML code for printing all simplices in a complex,
  // with the HTML syntax appropriate for the GUI
  // (it is heavily used by the GUI, not useful otherwise)
    string print_html();

  // Detailed printing of the whole simplicial complex structure, in a nice
  // human-readable tabular format, to stdout
  // Prints the name, dimension, topology and list of simplices at each level,
  // and then for each simplex prints all colors with their values, and a list
  // of neighbors of that simplex
  // Relies on print_compact(), and works best if the entire complex is colored
  // with UniqueIDColor (see UniqueIDColor::colorize_entire_complex() for details)
    void print_detailed();

  // Prints a table of simplices in a complex, in a human-readable tabular
  // format, to stdout
  // Works best if the entire complex is colored with UniqueIDColor
    void print_compact();

  // Helper function used by print_compact()
  // Collect all vertices in a complex, and print their UniqueID numbers in parentheses, to stdout
  // The output format looks like "(2-4-15-39)" or "(2-4-Simplex-Simplex)" or similar
  // (it makes sense to apply it only to the neighbor structure of a KSimplex object)
    void print_vertices_IDs_in_parentheses();

  // Helper function used by print_vertices_IDs_in_parentheses()
  // Given a set of UniqueID numbers, prints them in parentheses to stdout, padding
  // them with the word "-Simplex" if some of the vertices do not have UniqueIDColor
  // (it makes sense to apply it only to the neighbor structure of a KSimplex object)
  void print_set(set<int> &s);
  
};

#endif


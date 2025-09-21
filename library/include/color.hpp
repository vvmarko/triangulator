
#ifndef TRIANGULATOR_COLOR_H_INCLUDED
#define TRIANGULATOR_COLOR_H_INCLUDED


// ######################
// Color class definition
// ######################

// General information:
//
// Class Color is the third fundamental class of the whole
// Triangulator library and derived userspace apps. It contains
// the basic description of the colors and labels corresponding to
// a given simplex, by keeping track of the color type, value, and
// various other parameters and constants associated to each
// particular color.

// The vast majority of usecases of the library revolves around
// manipulating colors. Users are welcome and encouraged to define
// and use their own custom colors, in addition to the ones that are
// already provided by default. See below for the template for
// user-defined colors and requirements for defining them.

// As a rule, objects of the Color class should be allocated
// and deallocated using various high-level functions such as
// colorize_single_simplex() and remove_color_from_simplex().
// You should not instantiate a Color object using a low-level
// constructor, unless you know what you are doing (for example,
// writing a new colorize_single_simplex() function for a newly
// defined color, or something to that effect).
//
// Other than that, you will be working with, using and manipulating
// Color objects all the time --- the whole purpose of the
// Triangulator library is to facilitate various tools for that.


// ####################################################
// Definitions of the various constants and parameters:
// ####################################################

// #######################
// Color type definitions:
// #######################

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
#define TYPE_PACHNER 1
#define TYPE_UNIQUE_ID 128
#define TYPE_DRAWING_COORDINATES 129

// ################################################################
// Parameters foc evaluating topological and embedding coordinates:
// ################################################################

#define POTENTIAL_SPRING_EDGE_INTERACTION 1.0
#define POTENTIAL_SPRING_EDGE_LENGTH 50.0
#define POTENTIAL_SPRING_NON_EDGE_INTERACTION 1.0
#define POTENTIAL_SPRING_NON_EDGE_LENGTH 200.0
#define POTENTIAL_INVERSE_DISTANCE_INTERACTION 1000.0
#define POTENTIAL_INVERSE_BOUNDING_SPHERE_INTERACTION 1000.0
#define POTENTIAL_SHAKE_STEP 0.2
#define POTENTIAL_GLOBAL_RETRIES_NUMBER 50
#define POTENTIAL_MAX_ITERATION_NUMBER 1000
#define POTENTIAL_MAX_TEST_COORDINATES 50
#define SPHERE_TOPOLOGY_DRAWING_RADIUS 200.0
#define LINEAR_TOPOLOGY_DRAWING_DOMAIN_SIZE 200.0

// #########################################
// Forward declarations of neccesary classes
// #########################################

class KSimplex;
class SimpComp;

// #####################################
// Definition of the parent Color class:
// #####################################

class Color{
public:

  // ##########################################
  // Data structures of the parent Color class:
  // ##########################################

  // Type of the color (corresponding to the TYPE_ constant from
  // the table above):
    int type;
  // NB: All other data structures of the color are defined in child
  // colors, rather than here. Type is the only common parameter for
  // all colors.

  // ########################################
  // Constructors and destructors of a color:
  // ########################################

  // Default constructor does not exist for the parent class, one should
  // only ever instantiate child color objects.
  
  // Default destructor:
    virtual ~Color();

  // ########################################
  // Adding and removing colors in a simplex:
  // ########################################

  // Given a color and its value in a string, add the color to a given
  // simplex and convert the string into the color value (this function
  // is used for loading the simplicial complex from a file)
    static bool colorize_simplex_from_string(KSimplex* simp, const int color_type, const string& color_value);

  // Remove all instances of the given color from a given simplex, erasing
  // the respective color values
    static void remove_color_type_from_simplex(KSimplex* simp, int typecode);

  // Go through all simplices of a given level in the complex, and for each
  // remove all instances of the given color, as above
    static void remove_color_type_from_level(SimpComp* G, int level, int typecode);

  // Go through all simplices in the whole complex, and remove all instances
  // of the color from all of them, as above
    static void remove_color_type_from_complex(SimpComp* G, int typecode);

  // ######################################
  // String transliteration of color value:
  // ######################################
  
  // Encode the value of the color into a string
  // NB: This is an abstract virtual function, in the sense that every
  // child Color class must provide its own implementation of this
  // function, depending on the nature of the color data
    virtual string get_color_value_as_str() = 0;
//    virtual string get_color_value_as_str() const;

  // Decode the string into a color value, and set it
  // NB: This is an abstract virtual function, in the sense that every
  // child Color class must provide its own implementation of this
  // function, depending on the nature of the color data
    virtual void set_color_value_from_str(const string& source) = 0;

  // ##############################################
  // Finding colors and testing for their presence:
  // ##############################################
  
  // Check if a given simplex is colored with a given color
    static bool is_colorized_with_type(KSimplex* simp, int typecode);
  
  // Find a pointer to a given color within a given simplex
    static Color* find_pointer_to_color_type(KSimplex* simp, int typecode);

  // ###################
  // Printing functions:
  // ###################

  // Printing of the color name, type and value to stdout, in a
  // human-readable format
  // Makes use of get_color_name_from_type() to figure out the color
  // name, and of the virtual function get_color_value_as_str() to
  // convert the color value into a human-readable string.
    void print();

};


// ######################################
// Functions external to the Color class:
// ######################################

// Assigns a human-readable name to a given color type, in correspondence
// with the definitions in the TYPE_ table above
string get_color_name_from_type(int color_type);


// ##############################
// Child color --- Boundary color
// ##############################

// The boundary color is a special color that is internally used by the
// library. It marks simplices of level D-1 that represent the boundary
// of a simplicial complex. The built-in functions in the library
// manipulate the boundary color automatically, and the only two cases
// where a user may deal with it are (a) testing if a given simplex is
// on the boundary of a complex, and (b) defining the boundary of a
// complex when implementing a new seed function.

// The boundary color contains no data or value. The mere presence of
// the color in a given simplex is information enough that this simplex
// is part of the boundary.

class BoundaryColor : public Color{
public:

  // Constructor:
    BoundaryColor();
  
  // Destructor:
    ~BoundaryColor();

  // Add boundary color to a given simplex:
    static bool colorize_single_simplex(KSimplex* simp);

  // Add boundary color to all simplices of a given level:
    static bool colorize_simplices_at_level(SimpComp* G, int level);

  // Remove boundary color from the simplex:
    static void remove_color_from_simplex(KSimplex* simp);

  // Obligatory implementations of string codec functions for boundary color:
    string get_color_value_as_str() override;
    void set_color_value_from_str(const string& source) override;
};


// #############################
// Child color --- Pachner color
// #############################

// The Pachner color is a special color that is internally used by the
// library. It is being used exclusively by the Pachner_move() function
// and the functions that it calls internally, and should never be used
// or manipulated by the user, except for the purpose of debugging the
// Pachner_move() function itself. The Pachner color is temporary in
// its usage --- it is being added to some simplices automatically at
// the beginning of the Pachner_move(), and is automatically removed
// from all simplices by the end of the Pachner_move().

// You most probably do not want to interact with Pachner color, ever.

// The Pachner color contains the pointer to a matching simplex from
// another simplicial complex, and is being used to build the
// isomorphism between the wider neighborhood of the target simplex
// and the internal part of the Pachner sphere to be replaced. It also
// contains booleans that classify the sphere simplices into internal
// and/or external, and one immutable boolean that serves as a
// write-protect flag against changing the isomorphism relation of the
// corresponding pair of simplices.

class PachnerColor : public Color{
public:

  // Data structures:
  // ################
  
  // Pointer to the matching simplex for the isomorphism:
    KSimplex *matchingSimplex = nullptr;

  // Booleans labelling the simplex as internal and/or external:
    bool internalSimplex = false;
    bool externalSimplex = false;

  // Write-protect boolean for the simplex with the fixed match:
    bool immutable = false;

  // Functions:
  // ##########
  
  // Constructor:
    PachnerColor();

  // Destructor:
    ~PachnerColor();

  // Add color to a single simplex, to all simplices of a given level
  // and to all simplices in the complex:
    static bool colorize_single_simplex(KSimplex* simp);
    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static bool colorize_entire_complex(SimpComp* G);

  // Remove color from a single simplex, from all simplices of a given
  // level, and from all simplices in a complex:
    static void remove_color_from_simplex(KSimplex* simp);
    static void remove_color_from_level(SimpComp* G, int level);
    static void remove_color_from_complex(SimpComp* G);

  // verify if a simplex contains the Pachner color:
    static bool is_colorized(KSimplex* simp);

  // find a pointer to the Pachner color for a given simplex:
    static PachnerColor* find_pointer_to_color(KSimplex* simp);

  // Obligatory implementations of string codec functions for Pachner color
  // (though it is unlikely that saving to file or reading from a file will
  // ever happen in the middle of the execution of a Pachner_move() operation):
    string get_color_value_as_str() override;
    void set_color_value_from_str(const string& source) override;

};


// ##############################
// Child color --- UniqueID color
// ##############################

// UniqueID color is one convenient colors built-in and integrated into the
// library. Its purpose is to assign a unique numerical identifier (in
// monotonically  ascending order) to any simplex that is colored. It is
// extensively used by various printing functions and GUI functions, to
// identify simplices in a human-readable form. The uniqueness of the
// identifier is preserved across multiple instantiated simplicial complexes.

// Sometimes some colored simplices get deleted. By default, their numerical
// identifier (a UniqueID number) is not reused. This can sometimes lead to
// big gaps in the numeration of simplices, and ever larger values for new
// simplices. To mitigate this effect, a relabel_everything() function is
// provided, which resets all UniqueID color identifiers for all colored
// simplices, and re-enumerates them with a new numerical identifier,
// starting back from 1. This changes the numeration of all colored simplices
// in all complexes, resetting the identifiers to eliminate any gaps. Use this
// function with care.

class UniqueIDColor : public Color{
public:

  // Data structures:
  // ################

  // Next unused identifier (global for the color):
    static unsigned long next_free_uid_number;

  // A given UniqueID identifier for this simplex:
    unsigned long id;

  // Functions:
  // ##########

  // Constructor:
    UniqueIDColor();
  
  // Constructor with a specified value of the identifier
  // (used for reading from a file):
    UniqueIDColor(unsigned long uid);
  
  // Destructor:
    ~UniqueIDColor();

  // Add color to a single simplex, to all simplices of a given level
  // and to all simplices in the complex:
    static bool colorize_single_simplex(KSimplex* simplex);
    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static bool colorize_entire_complex(SimpComp* G);

  // Append color to a single simplex, to all simplices of a given
  // level, and to all simplices in a complex, if they have not already
  // been colored:
    static bool append_color_to_single_simplex(KSimplex* simp);
    static bool append_color_to_simplices_at_level(SimpComp* G, int level);
    static bool append_color_to_entire_complex(SimpComp* G);

  // verify if a simplex contains the UniqueID color:
    static bool is_colorized(KSimplex* simp);

  // Reset all UniqueID identifiers monotonically starting from 1, resetting
  // all identifiers, for all colored simplices across all instantiated
  // complexes in memory (use with care):
    static bool relabel_everything(void);

  // Printing the UniqueID number in a compact form:
    void print_compact();

  // Obligatory implementations of string codec functions for UniqueID color
    string get_color_value_as_str() override;
    void set_color_value_from_str(const string& source) override;

};


// #########################################
// Child color --- Drawing coordinates color
// #########################################

// Drawing coordinates color is a built-in color that is used by
// various functions of the library, for the purpose of drawing the
// wireframe graph of the simplicial complex on the computer screen.
// The idea is to start from assigning some intrinsic coordinates
// adapted to the topology of the simplicial complex. The simplicial
// complex is then embedded as a hypersurface into a Euclidean space
// of higher dimension, and intrinsic coordinates are used to
// evaluate the embedding coordinates in the big ambient space.
// Finally, the embedding coordinates are used to project the complex
// onto a two-dimensional plane of the computer screen, and the
// wireframe of the complex can then be drawn on the screen.

// The drawing coordinates color is assigned to vertices of the
// complex, while all other simplices are ignored. For a simplicial
// complex of dimension D embedded into an ambient space of dimension
// Damb (where D <= Damb <= 2D), the drawing coordinates color keeps
// track of D intrinsic cooordinates, their domain (i.e. their
// maximum and minimum values), and Damb embedding coordinates,
// including the value of Damb.

// The user is allowed to manipulate the values of both intrinsic
// and embedding coordinates (within their domains of course), but
// for best visual results we recommend that they always be evaluated
// automatically. Note that there are functions which will erase and
// modify any manual changes made to either intrinsic or to embedding
// coordinates.

class DrawingCoordinatesColor : public Color{
public:

  // Data structures:
  // ################

  // Array of D topological coordinates:
    vector<double> q;

  // Arrays defining the domain of topological coordinates,
  // namely minimal and maximal possible values for each of the
  // D coordinates:
    vector<double> qMin;
    vector<double> qMax;

  // Array of Damb topological coordinates:
    vector<double> x;

  // Dimension of the ambient embedding space:
    int Damb;
  
  // Functions:
  // ##########

  // Constructor:
    DrawingCoordinatesColor();

  // Destructor:
    ~DrawingCoordinatesColor();

  // Adding DrawingCoordinatesColor to a single simplex, and to a complex:
    static bool colorize_single_simplex(KSimplex* simp);
    static bool colorize_entire_complex(SimpComp* simpComp);

  // Adding DrawingCoordinatesColor to a single simplex, using values from a string:
    static bool colorize_single_simplex(KSimplex* simp, const string& source);

  // verify if a simplex contains the DrawingCoordinates color:
    static bool is_colorized(KSimplex* simp);
  
  // find a pointer to the Pachner color for a given simplex:
    static DrawingCoordinatesColor* find_pointer_to_color(KSimplex* simp);
  
  // Printing the coordinates to the screen in human-readable form:
    void print();

  // Small helper function to convert a float/double to a string, with a fixed number
  // of decimal places
    string double_to_string_with_precision(double number, int decdigits);
  
  // Obligatory implementations of string codec functions for drawing coordinates color:
    string get_color_value_as_str() override;
    void set_color_value_from_str(const string& source) override;
  
};

#endif


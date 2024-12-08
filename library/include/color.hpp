
#ifndef _TRIANGULATOR_COLOR_HPP_
#define _TRIANGULATOR_COLOR_HPP_

// ######################
// Color type definitions
// ######################

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
#define TYPE_SCREEN_COORDINATES 128
#define TYPE_UNIQUE_ID 129
#define TYPE_TOPOLOGICAL_COORDINATES 130

#endif

#include <memory>
#include <math.h>

// ######################
// Forward declarations of neccesary classes
// ######################

class KSimplex;
class SimpComp;

// #######################
// Color class definitions
// #######################

// Colors define features of K-simplices:
class Color{
public:

    virtual ~Color();

    virtual void print();
    virtual void print_compact();

    virtual string get_color_value_as_str() const;
    virtual void set_color_value_from_str(const string& source) = 0;
    static bool colorize_simplex_from_string(KSimplex* simp, const int color_type, const string& color_value);
    static bool is_colorized_with_type(KSimplex* simp, int typecode);
    static Color* find_pointer_to_color_type(KSimplex* simp, int typecode);
    static void remove_color_type_from_simplex(KSimplex* simp, int typecode);
    static void remove_color_type_from_level(SimpComp* G, int level, int typecode);
    static void remove_color_type_from_complex(SimpComp* G, int typecode);
  
    int type;
};

string get_color_name_from_type(int color_type);

// Whether the KSimplex of this color represents a boundary:
class BoundaryColor : public Color{
public:
    BoundaryColor();
    ~BoundaryColor();
    static bool colorize_single_simplex(KSimplex* simp);
    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static void remove_color_from_simplex(KSimplex* simp);
    void print();
    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source) override;
};


// The color used by the Pachner move algorithm:
class PachnerColor : public Color{
public:
    PachnerColor();
    ~PachnerColor();

    static bool colorize_single_simplex(KSimplex* simp);
    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static bool colorize_entire_complex(SimpComp* G);
    static void remove_color_from_simplex(KSimplex* simp);
    static void remove_color_from_level(SimpComp* G, int level);
    static void remove_color_from_complex(SimpComp* G);
    static bool is_colorized(KSimplex* simp);
    static PachnerColor* find_pointer_to_color(KSimplex* simp);

    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source) override;
  
    KSimplex *matchingSimplex = nullptr;
    bool internalSimplex = false;
    bool externalSimplex = false;
    bool immutable = false;
};


// Unique ID of KSimplex:
class UniqueIDColor : public Color{
public:
    UniqueIDColor();
    ~UniqueIDColor();
    UniqueIDColor(unsigned long uid);
    void print(string space = "");
    void print_compact();

    static bool colorize_single_simplex(KSimplex* simplex);
    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static bool colorize_entire_complex(SimpComp* G);
    static bool is_colorized(KSimplex* simp);
    static bool append_color_to_single_simplex(KSimplex* simp);
    static bool append_color_to_simplices_at_level(SimpComp* G, int level);
    static bool append_color_to_entire_complex(SimpComp* G);
    static bool relabel_everything(void);
  
    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source) override;

    static unsigned long next_free_uid_number;
    unsigned long id;
};

// Drawing coordinates:
class ScreenCoordinatesColor : public Color{
public:
    ScreenCoordinatesColor();
    ~ScreenCoordinatesColor();
    ScreenCoordinatesColor(int x, int y);
    void print();
    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source);

    int x, y; // screen coordinates
};

// Whether the KSimplex of this color represents a boundary:
class TopologicalCoordinatesColor : public Color{
public:
    TopologicalCoordinatesColor();
    ~TopologicalCoordinatesColor();
    static void initQMinQMax(int D);
    static bool colorize_simplex(SimpComp* simp);
    bool colorize_vertex();
    
    static double evaluate_edge_length(KSimplex *edge, SimpComp *simp);
    static double evaluate_spring_potential(SimpComp *simp);
    
    void print();
    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source) override;
        
    static vector<double> qMin;
    static vector<double> qMax;
    vector<double> q;
};


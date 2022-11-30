
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
#define TYPE_SCREEN_COORDINATE 128
#define TYPE_UNIQUE_ID 129

#endif

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
    virtual void print(string space = "");
    virtual void print_compact();

    virtual string get_color_value_as_str() const;
    virtual void set_color_value_from_str(const string& source) = 0;
    static void colorize_node_from_string(KSimplex* ks, const int color_type, const string& color_value);

    int type;
};

string get_color_name_from_type(int color_type);

// Whether the KSimplex of this color represents a boundary:
class BoundaryColor : public Color{
public:
    BoundaryColor();
    void print(string space = "");
    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source) override;
};

// Unique ID of KSimplex:
class UniqueIDColor : public Color{
public:
    UniqueIDColor();
    UniqueIDColor(unsigned long uid);
    void print(string space = "");
    void print_compact();

    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static bool colorize_entire_complex(SimpComp* G);

    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source) override;

    static unsigned long next_free_uid_number;
    unsigned long id; // whether the KSimplex of this color represents a boundary
};

// Drawing coordinates:
class ScreenCoordinateColor : public Color{
public:
    ScreenCoordinateColor();
    ScreenCoordinateColor(int x, int y);
    void print(string space = "");

    string get_color_value_as_str() const;
    void set_color_value_from_str(const string& source);

    int x, y; // screen coordinates
};


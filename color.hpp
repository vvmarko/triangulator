
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


// #######################
// Color class definitions
// #######################


class Color{
public:
    virtual void print(string space = "");
    virtual void print_compact();

    int type;
};

class BoundaryColor : public Color{
public:
    BoundaryColor(bool boundary);
    void print(string space = "");

    bool boundary; // whether the KSimplex of this color represents a boundary
};

class UniqueIDColor : public Color{
public:
    UniqueIDColor();
    void print(string space = "");
    void print_compact();

    static bool colorize_simplices_at_level(SimpComp* G, int level);
    static bool colorize_entire_complex(SimpComp* G);

    static unsigned long next_free_uid_number;
    unsigned long id; // whether the KSimplex of this color represents a boundary
};

class ScreenCoordinateColor : public Color{
public:
    ScreenCoordinateColor();
    ScreenCoordinateColor(int x, int y);
    void print(string space = "");

    int x, y; // screen coordinates
};


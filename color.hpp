
#include <string>
#include <iostream>

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

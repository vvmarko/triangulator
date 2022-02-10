
#include "color.hpp"

void Color::print(string space){
//    cout << space << "Color type = " << type << endl;
}

BoundaryColor::BoundaryColor(bool boundary){
    type = TYPE_BOUNDARY;
    this->boundary = boundary;
}
void BoundaryColor::print(string space){
    Color::print(space);
    cout << space << "Boundary = " << (boundary?"true":"false") << endl;
}

ScreenCoordinateColor::ScreenCoordinateColor(){
    type = TYPE_SCREEN_COORDINATE;
}

ScreenCoordinateColor::ScreenCoordinateColor(int x, int y){
    type = TYPE_SCREEN_COORDINATE;
    this->x = x;
    this->y = y;
}

void ScreenCoordinateColor::print(string space){
    Color::print(space);
    cout << space << "coordinate(" << x << ", " << y << ")" << endl;
}

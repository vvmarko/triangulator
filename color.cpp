
#include "triangulator.hpp"

unsigned long UniqueIDColor::next_free_uid_number = 1;

void Color::print(string space){
//    cout << space << "Color type = " << type << endl;
}

void Color::print_compact(){
}

BoundaryColor::BoundaryColor(bool boundary){
    type = TYPE_BOUNDARY;
    this->boundary = boundary;
}
void BoundaryColor::print(string space){
    Color::print(space);
    cout << space << "Boundary = " << (boundary?"true":"false") << endl;
}

UniqueIDColor::UniqueIDColor(){
    type = TYPE_UNIQUE_ID;
    id = next_free_uid_number++;
}

void UniqueIDColor::print(string space){
    cout << space << "id = " << id << endl;
}

void UniqueIDColor::print_compact(){
    cout << id;
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

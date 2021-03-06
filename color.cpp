
#include "triangulator.hpp"

unsigned long UniqueIDColor::next_free_uid_number = 1;

void Color::print(string space){
//    cout << space << "Color type = " << type << endl;
}

void Color::print_compact(){
}

string Color::get_color_value_as_str() const
{
    return "";
}

bool Color::set_color_value_from_str(const string& source)
{
    return false;
}

BoundaryColor::BoundaryColor(bool boundary){
    type = TYPE_BOUNDARY;
    this->boundary = boundary;
}
void BoundaryColor::print(string space){
    Color::print(space);
    cout << space << "Boundary = " << (boundary?"true":"false") << endl;
}

string BoundaryColor::get_color_value_as_str() const
{
    if (this->boundary) return "1";
    return "0";
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

bool UniqueIDColor::colorize_simplices_at_level(SimpComp* G, int level)
{
    for (auto simplex : G->elements[level]) {
        simplex->colors.push_back(new UniqueIDColor());
    }
    return true;
}

bool UniqueIDColor::colorize_entire_complex(SimpComp* G)
{
    for (int level = 0; level <= G->D; level++) {
        UniqueIDColor::colorize_simplices_at_level(G, level);
    }
    return true;
}

string UniqueIDColor::get_color_value_as_str() const
{
    return to_string(this->id);
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

string ScreenCoordinateColor::get_color_value_as_str() const
{
    return "(" + to_string(this->x) + "," + to_string(y) + ")";
}

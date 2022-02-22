
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

bool UniqueIDColor::colorize_all_simplices(SimpComp* G, int level)
{
    bool colorization_successful = false, already_has_unique_id;

    for (auto simplex : G->elements[level]) {
        already_has_unique_id = false;
        for (auto cl : simplex->colors)
            if (cl->type == TYPE_UNIQUE_ID)
                already_has_unique_id = true;

        if (!already_has_unique_id) {
            simplex->colors.push_back(new UniqueIDColor());
            colorization_successful = true;
        }
    }
    return colorization_successful;
}

bool UniqueIDColor::colorize_entire_complex(SimpComp* G)
{
    bool colorization_successful = false, current_colorization_successful = false;
    for (int level = 0; level <= G->D; level++) {
        current_colorization_successful = UniqueIDColor::colorize_all_simplices(G, level);
        if (!colorization_successful)
            colorization_successful = current_colorization_successful;
    }
    return colorization_successful;
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


#include "triangulator.hpp"

unsigned long UniqueIDColor::next_free_uid_number = 1;

Color::~Color()
{
}

void Color::print(){
//    cout << "Color type = " << type << endl;
}

void Color::print_compact(){
}

string Color::get_color_value_as_str() const
{
    return "";
}

/**
 * @brief Creates new colour and gets its value from string, adding it to the
 * given KSimplex.
 * 
 * @details Uses switch structure to set the right colour class from
 * color_type integer. New types should be added to the switch as they are
 * created.
 * 
 * @param ks KSimplex to add the colour to.
 * @param color_type Type of the colour to be created.
 * @param color_value Value of the colour to be created.
 */
void Color::colorize_node_from_string(KSimplex* ks, const int color_type, const string& color_value)
{
    // This is a horrible solution. We should think of some other way of doing this
    Color* color;
    // NOTE: This seems as a bad solution.
    switch(color_type) {
    case TYPE_BOUNDARY:
        color = new BoundaryColor();
        ks->colors.push_back(static_cast<BoundaryColor*>(color));
        break;
    case TYPE_UNIQUE_ID:
        ks->colors.push_back(new UniqueIDColor());
        break;
    case TYPE_SCREEN_COORDINATE:
        color = new ScreenCoordinateColor();
        static_cast<ScreenCoordinateColor *>(color) -> set_color_value_from_str(color_value);
        ks->colors.push_back(color);
        break;
    default:
        return;
        // TODO: Color not implemented.
    }
}

string get_color_name_from_type(int color_type)
{
switch(color_type) {
    case TYPE_BOUNDARY:
      return "Boundary";
    case TYPE_UNIQUE_ID:
      return "UniqueID";
    case TYPE_SCREEN_COORDINATE:
      return "ScreenCoordinate";
    default:
      return "Unknown color name, type " + to_string(color_type);
    }
}


BoundaryColor::BoundaryColor(){
    type = TYPE_BOUNDARY;
}

BoundaryColor::~BoundaryColor(){
}

void BoundaryColor::print(){
  //    Color::print();
    cout << "Boundary = true" << endl;
}

string BoundaryColor::get_color_value_as_str() const
{
    return "true";
}

void BoundaryColor::set_color_value_from_str(const string& source)
{
  if (source=="true") return; // This is a dummy command, do not remove
}

UniqueIDColor::UniqueIDColor(){
    type = TYPE_UNIQUE_ID;
    id = next_free_uid_number++;
}

UniqueIDColor::~UniqueIDColor(){
}

UniqueIDColor::UniqueIDColor(unsigned long uid){
    type = TYPE_UNIQUE_ID;
    id = uid;
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

void UniqueIDColor::set_color_value_from_str(const string& source) {
    id = stoi(source);
}

ScreenCoordinateColor::ScreenCoordinateColor(){
    type = TYPE_SCREEN_COORDINATE;
}

ScreenCoordinateColor::~ScreenCoordinateColor(){
}

ScreenCoordinateColor::ScreenCoordinateColor(int xx, int yy){
    type = TYPE_SCREEN_COORDINATE;
    this->x = xx;
    this->y = yy;
}

void ScreenCoordinateColor::print(){
  //    Color::print();
    cout << "coordinate(" << x << ", " << y << ")" << endl;
}

string ScreenCoordinateColor::get_color_value_as_str() const
{
    return "(" + to_string(this->x) + "," + to_string(y) + ")";
}

void ScreenCoordinateColor::set_color_value_from_str(const string& source)
{
    int divider = source.find(",");
    this->x = stoi(source.substr(1, divider));
    this->y = stoi(source.substr(divider + sizeof(","), source.length() - sizeof(")")));
}


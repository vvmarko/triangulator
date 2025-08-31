
#include "triangulator.hpp"

unsigned long UniqueIDColor::next_free_uid_number = 1;

Color::~Color()
{
}


void Color::print(){
  cout << "color: " << get_color_name_from_type(type) << " (type " << type << "), value: " << this->get_color_value_as_str() << endl;
}

//string Color::get_color_value_as_str() const
//{
//    return "";
//}

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

bool Color::colorize_simplex_from_string(KSimplex* simp, const int color_type, const string& color_value)
{
    // This is a horrible solution. We should think of some other way of doing this
    switch(color_type) {
    case TYPE_BOUNDARY:
        return BoundaryColor::colorize_single_simplex(simp);
    case TYPE_PACHNER:
        return PachnerColor::colorize_single_simplex(simp);
    case TYPE_UNIQUE_ID:
        return UniqueIDColor::colorize_single_simplex(simp);
    case TYPE_DRAWING_COORDINATES:
      return DrawingCoordinatesColor::colorize_single_simplex(simp,color_value);
    default:
      log_report(LOG_ERROR,"Color::colorize_simplex_from_string : Color type " + to_string(color_type) + " not recognized! Fix your code!");
      return false;
    }
}

bool Color::is_colorized_with_type(KSimplex* simp, int typecode)
{
    for(auto &color : simp->colors)
        if(color->type == typecode)
            return true;
    return false;
}

Color* Color::find_pointer_to_color_type(KSimplex* simp, int typecode)
{
    for(auto &color : simp->colors)
        if(color->type == typecode)
            return color;
    return nullptr;
}


void Color::remove_color_type_from_simplex(KSimplex* simp, int typecode)
{
    unsigned int i, size;

    while( Color::is_colorized_with_type(simp,typecode) ){
        i = 0;
        while( simp->colors[i]->type != typecode )
            i++;
        delete simp->colors[i];
        size = simp->colors.size(); 
        if(i < size - 1) simp->colors[i] = simp->colors[size-1];
        simp->colors.pop_back();
    }
}

void Color::remove_color_type_from_level(SimpComp* G, int level, int typecode){
    for (auto simp : G->elements[level]) Color::remove_color_type_from_simplex(simp, typecode);
}

void Color::remove_color_type_from_complex(SimpComp* G, int typecode){
    for (int level = 0; level <= G->D; level++) Color::remove_color_type_from_level(G, level, typecode);
}

string get_color_name_from_type(int color_type){
    switch(color_type) {
        case TYPE_BOUNDARY:
            return "Boundary";
        case TYPE_PACHNER:
            return "Pachner";
        case TYPE_UNIQUE_ID:
            return "UniqueID";
        case TYPE_DRAWING_COORDINATES:
            return "DrawingCoordinates";
        default:
            return "Unknown color name, type " + to_string(color_type);
    }
}

BoundaryColor::BoundaryColor(){
    type = TYPE_BOUNDARY;
}

BoundaryColor::~BoundaryColor(){
}

bool BoundaryColor::colorize_single_simplex(KSimplex* simp)
{
  bool outcome = true;
  BoundaryColor* color = new(nothrow) BoundaryColor();
  if (color==nullptr) {
    outcome = false;
    log_report(LOG_PANIC,"BoundaryColor::colorize_single_simplex : PANIC!!! CANNOT ALLOCATE MEMORY!!!");
  }
  else simp->colors.push_back(color);
  return outcome;
}

bool BoundaryColor::colorize_simplices_at_level(SimpComp* G, int level)
{
  bool outcome = true;
  bool temp;
  for (auto simplex : G->elements[level]) {
    temp = BoundaryColor::colorize_single_simplex(simplex);
    if (!temp) outcome = false;
    }
  return outcome;
}

void BoundaryColor::remove_color_from_simplex(KSimplex* simp)
{
  Color::remove_color_type_from_simplex(simp, TYPE_BOUNDARY);
}

string BoundaryColor::get_color_value_as_str()
{
    return "true";
}

void BoundaryColor::set_color_value_from_str(const string& source)
{
  if (source=="true") return; // This is a dummy command, do not remove
}









PachnerColor::PachnerColor(){
    type = TYPE_PACHNER;
}

PachnerColor::~PachnerColor(){
}

bool PachnerColor::colorize_single_simplex(KSimplex* simp)
{
  bool outcome = true;
  PachnerColor* color = new(nothrow) PachnerColor();
  if (color==nullptr) {
    outcome = false;
    log_report(LOG_PANIC,"PachnerColor::colorize_single_simplex : PANIC!!! CANNOT ALLOCATE MEMORY!!!");
  }
  else simp->colors.push_back(color);
  return outcome;
}

bool PachnerColor::colorize_simplices_at_level(SimpComp* G, int level)
{
  bool outcome = true;
  bool temp;
  for (auto simplex : G->elements[level]) {
    temp = PachnerColor::colorize_single_simplex(simplex);
    if (!temp) outcome = false;
    }
  return outcome;
}

bool PachnerColor::colorize_entire_complex(SimpComp* G)
{
  bool outcome=true;
  bool temp;
  for (int level = 0; level <= G->D; level++) {
       temp = PachnerColor::colorize_simplices_at_level(G, level);
       if (!temp) outcome = false;
    }
  return outcome;
}

void PachnerColor::remove_color_from_simplex(KSimplex* simp)
{
  Color::remove_color_type_from_simplex(simp, TYPE_PACHNER);
}

void PachnerColor::remove_color_from_level(SimpComp* G, int level)
{
  Color::remove_color_type_from_level(G, level, TYPE_PACHNER);
}

void PachnerColor::remove_color_from_complex(SimpComp* G)
{
  Color::remove_color_type_from_complex(G, TYPE_PACHNER);
}

bool PachnerColor::is_colorized(KSimplex* simp)
{
  return Color::is_colorized_with_type(simp,TYPE_PACHNER);
}

PachnerColor* PachnerColor::find_pointer_to_color(KSimplex* simp)
{
  Color *temp = Color::find_pointer_to_color_type(simp, TYPE_PACHNER);
  return static_cast<PachnerColor*>(temp);
}


string PachnerColor::get_color_value_as_str()
{
  string s="internal: ";
  if (internalSimplex) s = s + "true, external: ";
  else s = s + "false, external: ";
  if (externalSimplex) s = s + "true, immutable: ";
  else s = s + "false, immutable: ";
  if (immutable) s = s + "true.";
  else s = s + "false.";
  return s;
}

void PachnerColor::set_color_value_from_str(const string& source)
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

void UniqueIDColor::print_compact(){
    cout << id;
}


bool UniqueIDColor::colorize_single_simplex(KSimplex* simp)
{
  bool outcome = true;
  UniqueIDColor* color = new(nothrow) UniqueIDColor();
  if (color==nullptr) {
    outcome = false;
    log_report(LOG_PANIC,"UniqueIDColor::colorize_single_simplex : PANIC!!! CANNOT ALLOCATE MEMORY!!!");
  }
  else simp->colors.push_back(color);
  return outcome;
}

bool UniqueIDColor::colorize_simplices_at_level(SimpComp* G, int level)
{
  bool outcome = true;
  bool temp;
  for (auto simplex : G->elements[level]) {
    temp = UniqueIDColor::colorize_single_simplex(simplex);
    if (!temp) outcome = false;
    }
  return outcome;
}

bool UniqueIDColor::colorize_entire_complex(SimpComp* G)
{
  bool outcome=true;
  bool temp;
  for (int level = 0; level <= G->D; level++) {
       temp = UniqueIDColor::colorize_simplices_at_level(G, level);
       if (!temp) outcome = false;
    }
  return outcome;
}

bool UniqueIDColor::is_colorized(KSimplex* simp)
{
  return Color::is_colorized_with_type(simp,TYPE_UNIQUE_ID);
}

bool UniqueIDColor::append_color_to_single_simplex(KSimplex* simp)
{
  bool outcome=true;
  if (UniqueIDColor::is_colorized(simp)) return true;
  else outcome = UniqueIDColor::colorize_single_simplex(simp);
  return outcome;
}

bool UniqueIDColor::append_color_to_simplices_at_level(SimpComp* G, int level)
{
  bool outcome = true;
  bool temp;
  for (auto simplex : G->elements[level]) {
    temp = UniqueIDColor::append_color_to_single_simplex(simplex);
    if (!temp) outcome = false;
    }
  return outcome;
}

bool UniqueIDColor::append_color_to_entire_complex(SimpComp* G)
{
  bool outcome=true;
  bool temp;
  for (int level = 0; level <= G->D; level++) {
       temp = UniqueIDColor::append_color_to_simplices_at_level(G, level);
       if (!temp) outcome = false;
    }

  return outcome;
}

bool UniqueIDColor::relabel_everything( void )
{
  bool outcome = true;
  unsigned long N=1;
  for (auto simpComp : triangulator_global::seededComplexes) {
    for (auto scelem : simpComp->elements) {
      for (auto ksimplex : scelem) {
	for (auto col : ksimplex->colors) {
	  if (col->type == TYPE_UNIQUE_ID) {
	    static_cast<UniqueIDColor*>(col)->id = N;
	    N++;
	  }
        }
      }
    }
  }
  UniqueIDColor::next_free_uid_number = N;

  return outcome;
}

string UniqueIDColor::get_color_value_as_str()
{
    return to_string(this->id);
}

void UniqueIDColor::set_color_value_from_str(const string& source) {
    id = stoi(source);
}








DrawingCoordinatesColor::DrawingCoordinatesColor(){
    type = TYPE_DRAWING_COORDINATES;
}

DrawingCoordinatesColor::~DrawingCoordinatesColor(){
}

bool DrawingCoordinatesColor::colorize_single_simplex(KSimplex* simp)
{
  // Check if the simplex is a vertex or not
  if(simp->k != 0){
    log_report(LOG_INFO,"This simplex is not a vertex, DrawingCoordinatesColor is meant only for vertices, skipping...");
    return true;
  }

  // Check if the simplex is already colorized
  if( DrawingCoordinatesColor::is_colorized(simp) ) return true;

  // Colorize the simplex
  DrawingCoordinatesColor* color = new(nothrow) DrawingCoordinatesColor();
  if (color==nullptr){
    log_report(LOG_PANIC,"DrawingCoordinatesColor::colorize_single_simplex : PANIC!!! CANNOT ALLOCATE MEMORY!!!");
    return false;
  }
  simp->colors.push_back(color);
  return true;
}

bool DrawingCoordinatesColor::colorize_single_simplex(KSimplex* simp, const string& source)
{
  bool outcome;
  DrawingCoordinatesColor *color;

  // Check if the simplex is a vertex or not
  if(simp->k != 0){
    log_report(LOG_INFO,"This simplex is not a vertex, DrawingCoordinatesColor is meant only for vertices, skipping...");
    return true;
  }
  
  // Colorize the simplex
  outcome = DrawingCoordinatesColor::colorize_single_simplex(simp);
  if (!outcome) {
    log_report(LOG_PANIC,"DrawingCoordinatesColor::colorize_single_simplex : PANIC!!! Failed to colorize the simplex!!!");
    return false;
  }
  
  // Find the pointer to the color
  color = DrawingCoordinatesColor::find_pointer_to_color(simp);
  if (color == nullptr){
    log_report(LOG_PANIC,"Could not assign DrawingCoordinatesColor to a simplex!!!");
    return false;
  }

  // Fill in the color values from the string
  color->set_color_value_from_str(source);
  return true;
}

bool DrawingCoordinatesColor::colorize_entire_complex(SimpComp* simpComp){
  bool outcome=true;
  bool temp;
  
  for(auto &simp : simpComp->elements[0]){
    temp = DrawingCoordinatesColor::colorize_single_simplex(simp);
    if (!temp) outcome = false;
  }
  return outcome;
}

bool DrawingCoordinatesColor::is_colorized(KSimplex* simp)
{
  return Color::is_colorized_with_type(simp,TYPE_DRAWING_COORDINATES);
}

DrawingCoordinatesColor* DrawingCoordinatesColor::find_pointer_to_color(KSimplex* simp)
{
  Color *temp = Color::find_pointer_to_color_type(simp, TYPE_DRAWING_COORDINATES);
  return static_cast<DrawingCoordinatesColor*>(temp);
}

void DrawingCoordinatesColor::print(){
  long unsigned int i;

  cout << "Intrinsic coordinates:  ";
  for(i = 0; i < q.size(); i++) cout << q[i] << "  ";
  cout << endl << "Embedding coordinates:  ";
  for(i = 0; i < x.size(); i++) cout << x[i] << "  ";
  cout << endl << "Ambient space dimension: Damb = " << Damb << endl;
}

string DrawingCoordinatesColor::get_color_value_as_str()
{
  // Serialize vector q, by to_string(q[i]), or optimized?
  // Serialize vectors qMin and qMax?
  // Serialize vector x
  // Serialize Damb?
  return "(TopologicalCoordinatesColor)"; // TODO
}

void DrawingCoordinatesColor::set_color_value_from_str(const string& source) // TODO
{
  if (source=="true") return; // This is a dummy command, do not remove
  // deserialize vector q
  // deserialize vectors qMin and qMax?
  // deserialize vector x
  // deserialize Damb?
}


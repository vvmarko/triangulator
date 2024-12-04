
#include "triangulator.hpp"

unsigned long UniqueIDColor::next_free_uid_number = 1;
vector<double> TopologicalCoordinateColor::qMin;
vector<double> TopologicalCoordinateColor::qMax;

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

bool Color::colorize_simplex_from_string(KSimplex* simp, const int color_type, const string& color_value)
{
    bool outcome;
    Color* color;

    // This is a horrible solution. We should think of some other way of doing this
    switch(color_type) {
    case TYPE_BOUNDARY:
        return BoundaryColor::colorize_single_simplex(simp);
    case TYPE_PACHNER:
        return PachnerColor::colorize_single_simplex(simp);
    case TYPE_UNIQUE_ID:
        return UniqueIDColor::colorize_single_simplex(simp);
    case TYPE_SCREEN_COORDINATE: // Fix this case
        color = new(nothrow) ScreenCoordinateColor();
        if (color == nullptr) outcome = false;
        static_cast<ScreenCoordinateColor *>(color) -> set_color_value_from_str(color_value);
        simp->colors.push_back(color);
        return outcome;
    default:
      log_report(LOG_ERROR,"Color::colorize_node_from_string : Color type " + to_string(color_type) + " not recognized! Fix your code!");
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


string PachnerColor::get_color_value_as_str() const
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

void UniqueIDColor::print(string space){
    cout << space << "id = " << id << endl;
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




TopologicalCoordinateColor::TopologicalCoordinateColor(){
    type = TYPE_TOPOLOGICAL_COORDINATE;
}

TopologicalCoordinateColor::~TopologicalCoordinateColor(){
}

void TopologicalCoordinateColor::initQMinQMax(int D){
    if(qMin.size()==0){
        for(int i = 0; i < D; i++){
            qMin.push_back(5); // TODO
            qMax.push_back(15);
        }
    }
}

bool TopologicalCoordinateColor::colorize_simplex(SimpComp* simp){
    TopologicalCoordinateColor::initQMinQMax(simp->D);
    srand((unsigned int)time(NULL));
    // For all vertices:
    for(auto &kSimplex : simp->elements[0]){
        TopologicalCoordinateColor *c = new TopologicalCoordinateColor();
        c->colorize_vertex();
        c->print();
        kSimplex->colors.push_back(c);
    }

    return true;
}

bool TopologicalCoordinateColor::colorize_vertex(){
    for(unsigned int i = 0; i < qMin.size(); i++)
        q.push_back(qMin[i] + (qMax[i] - qMin[i]) * rand() / RAND_MAX); // TODO

    return true;
}
    
double TopologicalCoordinateColor::evaluate_edge_length(KSimplex *edge, SimpComp *simp){
    Color *col;
    TopologicalCoordinateColor *color1, *color2;
cout << "Edge: " << endl;
    KSimplex *vertex1 = edge->neighbors->elements[0][0];
    KSimplex *vertex2 = edge->neighbors->elements[0][1];

cout << "  vertex1: ";
    col = Color::find_pointer_to_color_type(vertex1, TYPE_TOPOLOGICAL_COORDINATE);
    if(col)
        color1 = (TopologicalCoordinateColor*) col;
    else
        return 0;
cout << " color1: ";
    color1->print();

cout << "  vertex2: ";
    col = Color::find_pointer_to_color_type(vertex2, TYPE_TOPOLOGICAL_COORDINATE);
    if(col)
        color2 = (TopologicalCoordinateColor*) col;
    else
        return 0;
cout << " color2: ";
    color2->print();

    double sum = 0;
    for(int i = 0; i < simp->D; i++)
        sum += pow((color1->q[i] - color2->q[i]), 2);

    return sqrt(sum);
}

double TopologicalCoordinateColor::evaluate_spring_potential(SimpComp *simp){
    double const1 = 0.1; // TODO
    double const2 = 5;

    double sum = 0;
    // For all edges:
    for(auto &kSimplex : simp->elements[1]){
        double Le = TopologicalCoordinateColor::evaluate_edge_length(kSimplex, simp);
cout << "Le = " << Le << endl;
        sum += const1 * pow(Le - const2, 2);
    }
cout << "V = " << sum << endl;

    return sum;
}

void TopologicalCoordinateColor::print(){
    Color::print();
cout << "TopologicalCoordinateColor: ";
    for(unsigned int i = 0; i < q.size(); i++){
        cout << q[i] << "  ";
    }
    cout << endl;
}

string TopologicalCoordinateColor::get_color_value_as_str() const
{
    // Serialize vector q, by to_string(q[i]), or optimized? TODO
    // Serialize vectors qMin and qMax? // TODO

    return "(TopologicalCoordinateColor)"; // TODO
}

void TopologicalCoordinateColor::set_color_value_from_str(const string& source) // TODO
{
    cout << source; // remove
    // deserialize vector q // TODO
    // deserialize vectors qMin and qMax? // TODO
}


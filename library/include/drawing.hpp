
bool initialize_drawing_coordinates(SimpComp *simpComp);

void evaluate_embedding_coordinates(SimpComp *simpComp);

double evaluate_coordinate_length(KSimplex *edge, SimpComp *simpComp);

double evaluate_spring_potential(SimpComp *simpComp);

void shake_intrinsic_coordinates(SimpComp *simpComp);

void store_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &dest);

void restore_drawing_coordinates(SimpComp *simpComp, vector<DrawingCoordinatesColor> &source);

void evaluate_potential_minimum(SimpComp *simpComp);




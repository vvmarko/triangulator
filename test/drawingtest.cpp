
#include "triangulator.hpp"

#include <unistd.h>

int main(){
    cout << endl << "#######################################################" << endl;
    cout         << "############ BEGINNING OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#######################################################" << endl << endl;

    int D = 4;
    vector<EmbData> coords;
    vector<ScreenCoords> drawingdata;
    vector<EdgeData> edgedata;
    long unsigned int i;
    ScreenParameters *sp;

    
    //    SimpComp *sph = seed_single_simplex(D,"sphere");
    SimpComp *sph = seed_sphere(D,"sphere");
    //    Pachner_move(sph->elements[D][0],sph);
    //    Pachner_move(sph->elements[D][0],sph);
    //    Pachner_move(sph->elements[D][0],sph);
    //    Pachner_move(sph->elements[D][0],sph);
    //    Pachner_move(sph->elements[D][0],sph);

    evaluate_potential_minimum(sph);
    sp = setup_screen_parameters(sph);
    
    coords = extract_embedding_data(sph);

    edgedata = extract_edge_data(sph);

    cout << "Number of edges is: " << edgedata.size() << endl << endl;
    
    drawingdata = evaluate_perspective_projection(coords,sp);
    
    for(i = 0; i < drawingdata.size(); i++){
      cout << "Vertex pointer: " << drawingdata[i].simplex << endl;
      cout << "X, Y and Z coordinates: ";
      cout << drawingdata[i].X << "   ";
      cout << drawingdata[i].Y << "   ";
      cout << drawingdata[i].Z;
      cout << endl << endl;
    }
    free(sp);


    /*
    ScreenParameters *sp = new(nothrow) ScreenParameters(sph->D + 1);

    cout << "Damb = " << sp->Damb << endl;
    cout << "d =    " << sp->d << endl;
    cout << "sx =   " << sp->sx << endl;
    cout << "sy =   " << sp->sy << endl;
    cout << "sz =   " << sp->sz << endl;
    cout << "Number of alpha angles: " << sp->alpha.size() << endl;
    cout << "Number of beta angles: " << sp->beta.size() << endl;
    cout << "Number of gamma angles: " << sp->gamma.size() << endl;

    free(sp);
    */
    
    unseed_everything();
    
    cout << endl << "#################################################" << endl;
    cout         << "############ END OF THE MAIN FUNCTION ###########" << endl;
    cout         << "#################################################" << endl << endl;

    return 0;
}


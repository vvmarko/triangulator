
//TODO:
// - functions defined here should be reimplemented within SimpComp class
// - add checking whether KSimplex at level k consists of given at k-1 based on is_a_triangle
// - add KSimplex should be implemented based on add vertex, edge, and triangle

#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include <random>
#include <list>
#include <ctime>

#include <vector>
#include <string>

#define VERBOSE true

using namespace std;

#include "classes.cpp"

void temp(){
    //SimpComp G;

    int D = 2; // dimension
    //special function for sphere, torus, ball, rectangle, etc. are needed:
    //G = seed_simplex(D); // generates triangle, 3 edges, and 3 vertices

    //edge = get_random_edge(G);

    //zakaci_nov_trougao_na_edge(edge, G);
    //packner_move(G,...);
    
    //set_vertices_id_color(G, c);

    //da_li_se_3edges_sklapaju_u_trougao(G, edges);
    //duzine ivica, uglovi, povrsine...
    
    //definisi koordinatni sistem na osnovu triangle + vertex
    //isto za 2. trougao
    //transformisi iz jednog u drugi koordinatni sistem

    //path integrali - integral N dim. => suma umesto int. za graf.    

    //save_graph(G, f); // save graph to file f
}



/*
// Find a vertex common to two edges, if any,
// and if the edges are not the same:
KSimplex* find_common_vertex(KSimplex *e1, KSimplex *e2){ // given two edges
    KSimplex *v;

    v = nullptr;
    if(!e1){ log_report(LOG_WARN, "find_common_vertex: Null vertex pointer returned! The first given edge was already null."); return v; }
    if(!e2){ log_report(LOG_WARN, "find_common_vertex: Null vertex pointer returned! The second given edge was already null."); return v; }
    if(e1 == e2) { log_report(LOG_WARN, "find_common_vertex: Cannot find the common vertex between the edge and itself!"); return v; }

    KSimplex* e1_v1 = e1->vertex1();
    KSimplex* e1_v2 = e1->vertex2();
    KSimplex* e2_v1 = e2->vertex1();
    KSimplex* e2_v2 = e2->vertex2();

    if(e1_v1 == e2_v1) v = e1_v1;
    if(e1_v1 == e2_v2) v = e1_v1;
    if(e1_v2 == e2_v1) v = e1_v2;
    if(e1_v2 == e2_v2) v = e1_v2;

    if(!v) log_report(LOG_WARN, "find_common_vertex: Null pointer vertex returned! Apparently the two edges have no common vertex.");

    return v;
};

// Given a vertex, if it is adjacent to an edge,
// find the other vertex adjacent to the same edge
KSimplex* find_other_vertex(KSimplex *v, KSimplex *e){
    KSimplex *otherV;

    otherV = nullptr;

    if(!e){ log_report(LOG_WARN, "find_other_vertex: Null vertex pointer returned! The given edge was already null."); return otherV; }
    if(!v){ log_report(LOG_WARN, "find_other_vertex: Null vertex pointer returned! The given vertex was already null."); return otherV; }

    KSimplex* e_v1 = e->vertex1();
    KSimplex* e_v2 = e->vertex2();

    if(v == e_v1) otherV = e_v2;
    if(v == e_v2) otherV = e_v1;
    if(!otherV) log_report(LOG_WARN, "find_other_vertex: Null pointer vertex returned! The given vertex is not adjacent to the given edge.");

    return otherV;
};

// Given two vertices, find their common edge, if any
KSimplex* find_common_edge(KSimplex *v1, KSimplex *v2){
    KSimplex *commonE;
    KSimplex *vTemp;

    commonE = nullptr;
    if(!v1){ log_report(LOG_WARN, "find_common_edge: Null edge pointer returned! The first given vertex was already null."); return commonE; }
    if(!v2){ log_report(LOG_WARN, "find_common_edge: Null edge pointer returned! The second given vertex was already null."); return commonE; }

    for(auto &it : v1->neighbors[1]){
        vTemp = find_other_vertex(v1, it);
        if(v2 == vTemp) commonE = it;
// return *it; ???
    }
    if(!commonE) log_report(LOG_DEBUG, "find_common_edge: Null edge pointer returned! The two vertices do not share a common edge.");

    return commonE;
}; 

// Given an edge, if it is adjacent to a vertex
// and a triangle, find the other edge adjacent to
// the same vertex and the same triangle
KSimplex* find_other_edge(KSimplex *v, KSimplex *e, KSimplex *t){
    KSimplex *edg1, *edg2, *edg3, *otherEdg;
    KSimplex *v1, *v2, *v3;

    otherEdg = nullptr;
    if(!t){ log_report(LOG_WARN, "find_other_edge: Null edge pointer returned! The given triangle was already null."); return otherEdg; }
    if(!e){ log_report(LOG_WARN, "find_other_edge: Null edge pointer returned! The given edge was already null."); return otherEdg; }
    if(!v){ log_report(LOG_WARN, "find_other_edge: Null edge pointer returned! The given vertex was already null."); return otherEdg; }

    edg1 = t->edge1();
    edg2 = t->edge2();
    edg3 = t->edge3();
    v1 = nullptr;
    v2 = nullptr;
    v3 = nullptr;
    if(edg1 != e) v1 = find_common_vertex(edg1, e);
    if(edg2 != e) v2 = find_common_vertex(edg2, e);
    if(edg3 != e) v3 = find_common_vertex(edg3, e);
    if(v1 == v) otherEdg = edg1;
    if(v2 == v) otherEdg = edg2;
    if(v3 == v) otherEdg = edg3;
    if(!otherEdg) log_report(LOG_WARN, "find_other_edge: Null edge pointer returned! Vertex, edge and  triangle are not adjacent.");

    return otherEdg;
};

// Test if three edges form a triangle
bool is_a_triangle(KSimplex *e1, KSimplex *e2, KSimplex *e3){
    KSimplex *v12, *va, *vb, *vc;
    bool triang;

    triang = false;
    if(!e1){ log_report(LOG_WARN, "is_a_triangle: False returned! The first given edge was already null."); return triang; }
    if(!e2){ log_report(LOG_WARN, "is_a_triangle: False returned! The second given edge was already null."); return triang; }
    if(!e3){ log_report(LOG_WARN, "is_a_triangle: False returned! The third given edge was already null."); return triang; }

    v12 = find_common_vertex(e1, e2);
    if(!v12) return triang;

    va = find_other_vertex(v12, e1);
    if(!va) return triang;
    vb = find_other_vertex(va, e3);
    if(!vb) return triang;
    vc = find_other_vertex(vb, e2);
    if(vc == v12) triang=true;

    return triang;
};
*/

//############################################
//############################################
//
//  Creation and deletion functions for
//  edge and triangle
//
//############################################
//############################################

SimpComp* seed_edge(string name, Color &c1, Color &c2){
    if(loglevel >= LOG_INFO){
        cout << "Adding edge " << name << "..." << endl;
    }
    
    // Initilize KComplex of dimension 1:
    SimpComp *edge = new SimpComp(name, 1);

    // Add 2 vertices (k=0) of the edge:
    KSimplex *v1 = edge->add_ksimplex(0, "v1");
    v1->colors.push_back(&c1);
    
    KSimplex *v2 = edge->add_ksimplex(0, "v2");
    v2->colors.push_back(&c2);

    // Create KSimplex (k=1) for the edge:
    KSimplex *e1 = edge->add_ksimplex(1, "e1");

    // Connect KSimplex-es:

    // Edge has both vertices as neighbors at level 0:
    e1->neighbors->elements[0].push_back(v1);
    e1->neighbors->elements[0].push_back(v2);

    // Both vertices have edge as neighbors at level 1:
    v1->neighbors->elements[1].push_back(e1);
    v2->neighbors->elements[1].push_back(e1);

    return edge;
}

SimpComp* seed_triangle(string name, Color &c1, Color &c2, Color &c3){
    if(loglevel >= LOG_INFO){
        cout << "Adding triangle " << name << "..." << endl;
    }
    
    // Initilize KComplex of dimension 1:
    SimpComp *triangle = new SimpComp(name, 2);

    // Add 3 vertices (k=0) of the triangle:
    KSimplex *v1 = triangle->add_ksimplex(0, "v1");
    v1->colors.push_back(&c1);
    
    KSimplex *v2 = triangle->add_ksimplex(0, "v2");
    v2->colors.push_back(&c2);

    KSimplex *v3 = triangle->add_ksimplex(0, "v3");
    v3->colors.push_back(&c3);

    // Add 3 edges (k=1) of the triangle:
    KSimplex *e1 = triangle->add_ksimplex(1, "e1");
    KSimplex *e2 = triangle->add_ksimplex(1, "e2");
    KSimplex *e3 = triangle->add_ksimplex(1, "e3");

    // Connect edge e1 with vertices as neighbors at level 0:
    e1->neighbors->elements[0].push_back(v1);
    e1->neighbors->elements[0].push_back(v2);
    // Both vertices have edge e1 as neighbors at level 1:
    v1->neighbors->elements[1].push_back(e1);
    v2->neighbors->elements[1].push_back(e1);

    // Connect edge e2 with vertices as neighbors at level 0:
    e2->neighbors->elements[0].push_back(v2);
    e2->neighbors->elements[0].push_back(v3);
    // Both vertices have edge e2 as neighbors at level 1:
    v2->neighbors->elements[1].push_back(e2);
    v3->neighbors->elements[1].push_back(e2);

    // Connect edge e3 with vertices as neighbors at level 0:
    e3->neighbors->elements[0].push_back(v1);
    e3->neighbors->elements[0].push_back(v3);
    // Both vertices have edge e3 as neighbors at level 1:
    v1->neighbors->elements[1].push_back(e3);
    v3->neighbors->elements[1].push_back(e3);

    // Create KSimplex (k=2) for the triangle:
    KSimplex *t1 = triangle->add_ksimplex(2, "t1");

    // Connect triangle t1 with vertices:
    v1->neighbors->elements[2].push_back(t1);
    v2->neighbors->elements[2].push_back(t1);
    v3->neighbors->elements[2].push_back(t1);
    t1->neighbors->elements[0].push_back(v1);
    t1->neighbors->elements[0].push_back(v2);
    t1->neighbors->elements[0].push_back(v3);

    // Connect triangle t1 with edges:
    e1->neighbors->elements[2].push_back(t1);
    e2->neighbors->elements[2].push_back(t1);
    e3->neighbors->elements[2].push_back(t1);
    t1->neighbors->elements[1].push_back(e1);
    t1->neighbors->elements[1].push_back(e2);
    t1->neighbors->elements[1].push_back(e3);

    return triangle;
}

int main(){
    ScreenCoordinateColor c1(0, 0), c2(1, 1), c3(2, 0);
    //BoundaryColor c1(true), c2(true), c3(true);

    cout << endl << "Edge example:" << endl << endl;
    SimpComp *edge = seed_edge("First edge", c1, c2);
    edge->print();
 
    cout << endl << "Triangle example:" << endl << endl;
    SimpComp *triangle = seed_triangle("First triangle", c1, c2, c3);
    triangle->print();

    return 0;
}
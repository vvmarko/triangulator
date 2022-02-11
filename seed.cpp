
#include <iostream>
#include <vector>
#include <list>

//############################################
//############################################
//
//  Creation and deletion functions for
//  edge and triangle
//
//############################################
//############################################

SimpComp* seed_single_edge(string name){
    string s = "Creating simplicial complex - single edge: " + name + "...";
    log_report(LOG_INFO, s);
    
    // Initilize KComplex of dimension 1:
    SimpComp *edge = new SimpComp(name, 1);

    // Add 2 vertices (k=0) of the edge:
    KSimplex *v1 = edge->create_ksimplex(0);
    KSimplex *v2 = edge->create_ksimplex(0);

    // Create KSimplex (k=1) for the edge:
    KSimplex *e1 = edge->create_ksimplex(1);

    // Connect KSimplex-es:

    // Edge has both vertices as neighbors at level 0:
    e1->add_neighbor(v1);
    e1->add_neighbor(v2);

    // Both vertices have edge as neighbors at level 1:
    v1->add_neighbor(e1);
    v2->add_neighbor(e1);

    // Create and apply colors:
    Color *c1 = new BoundaryColor(true);
    Color *c2 = new BoundaryColor(true);
    v1->colors.push_back(c1);
    v2->colors.push_back(c2);

    return edge;
}

SimpComp* seed_single_triangle(string name){
    string s = "Creating simplicial complex - single triangle: " + name + "...";
    log_report(LOG_INFO, s);
    
    // Initilize KComplex of dimension 1:
    SimpComp *triangle = new SimpComp(name, 2);

    // Add 3 vertices (k=0) of the triangle:
    KSimplex *v1 = triangle->create_ksimplex(0);
    KSimplex *v2 = triangle->create_ksimplex(0);
    KSimplex *v3 = triangle->create_ksimplex(0);

    // Add 3 edges (k=1) of the triangle:
    KSimplex *e1 = triangle->create_ksimplex(1);
    KSimplex *e2 = triangle->create_ksimplex(1);
    KSimplex *e3 = triangle->create_ksimplex(1);

    // Create KSimplex (k=2) for the triangle:
    KSimplex *t1 = triangle->create_ksimplex(2);

    // Connect edge e1 with vertices as neighbors at level 0:
    e1->add_neighbor(v1);
    e1->add_neighbor(v2);
    // Both vertices have edge e1 as neighbors at level 1:
    v1->add_neighbor(e1);
    v2->add_neighbor(e1);

    // Connect edge e2 with vertices as neighbors at level 0:
    e2->add_neighbor(v2);
    e2->add_neighbor(v3);
    // Both vertices have edge e2 as neighbors at level 1:
    v2->add_neighbor(e2);
    v3->add_neighbor(e2);

    // Connect edge e3 with vertices as neighbors at level 0:
    e3->add_neighbor(v1);
    e3->add_neighbor(v3);
    // Both vertices have edge e3 as neighbors at level 1:
    v1->add_neighbor(e3);
    v3->add_neighbor(e3);

    // Connect triangle t1 with vertices:
    v1->add_neighbor(t1);
    v2->add_neighbor(t1);
    v3->add_neighbor(t1);
    t1->add_neighbor(v1);
    t1->add_neighbor(v2);
    t1->add_neighbor(v3);

    // Connect triangle t1 with edges:
    e1->add_neighbor(t1);
    e2->add_neighbor(t1);
    e3->add_neighbor(t1);
    t1->add_neighbor(e1);
    t1->add_neighbor(e2);
    t1->add_neighbor(e3);

    // Create and apply boundary colors:
    Color *c1 = new BoundaryColor(true);
    Color *c2 = new BoundaryColor(true);
    Color *c3 = new BoundaryColor(true);
    e1->colors.push_back(c1);
    e2->colors.push_back(c2);
//    e3->colors.push_back(c3);

    // Create and apply unique ID colors:
    c1 = new UniqueIDColor();
    c2 = new UniqueIDColor();
    c3 = new UniqueIDColor();
    v1->colors.push_back(c1);
//    v2->colors.push_back(c2);
    v3->colors.push_back(c3);
    c1 = new UniqueIDColor();
    c2 = new UniqueIDColor();
    c3 = new UniqueIDColor();
    e1->colors.push_back(c1);
    e2->colors.push_back(c2);
    e3->colors.push_back(c3);
    c1 = new UniqueIDColor();
    t1->colors.push_back(c1);

    return triangle;
}

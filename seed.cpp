
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

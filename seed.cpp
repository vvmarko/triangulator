
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

    // Edge has both vertices as neighbors and the opposite:
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

    // Both vertices have edge e1 as neighbors and the opposite:
    v1->add_neighbor(e1);
    v2->add_neighbor(e1);

    // Both vertices have edge e2 as neighbors and the opposite:
    v2->add_neighbor(e2);
    v3->add_neighbor(e2);

    // Both vertices have edge e3 as neighbors and the opposite:
    v1->add_neighbor(e3);
    v3->add_neighbor(e3);

    // Connect triangle t1 with vertices:
    v1->add_neighbor(t1);
    v2->add_neighbor(t1);
    v3->add_neighbor(t1);

    // Connect triangle t1 with edges:
    e1->add_neighbor(t1);
    e2->add_neighbor(t1);
    e3->add_neighbor(t1);

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

SimpComp* seed_single_tetrahedron(string name)
{
    KSimplex *v1, *v2, *v3, *v4;
    KSimplex *e1, *e2, *e3, *e4, *e5, *e6;
    KSimplex *trianglev1v2v3;
    KSimplex *trianglev1v3v4;
    KSimplex *trianglev1v2v4;
    KSimplex *trianglev2v4v3;
    KSimplex *tetrahedron1;

    // Initialize simplicial complex of dimension 3
    SimpComp *tetrahedron = new SimpComp(name, 3);

    // Vertex v1 is located up
    // Vertex v2 is located left
    // Vertex v4 is located right
    // Vertex v3 is located down
    // Add vertices (k-simplices, k=0)
    v1 = tetrahedron->create_ksimplex(0);
    v2 = tetrahedron->create_ksimplex(0);
    v3 = tetrahedron->create_ksimplex(0);
    v4 = tetrahedron->create_ksimplex(0);

    // Add edges (k-simplices, k=1)
    e1 = tetrahedron->create_ksimplex(1);
    e2 = tetrahedron->create_ksimplex(1);
    e3 = tetrahedron->create_ksimplex(1);
    e4 = tetrahedron->create_ksimplex(1);
    e5 = tetrahedron->create_ksimplex(1);
    e6 = tetrahedron->create_ksimplex(1);

    // Add triangles (k-simplices, k=2)
    trianglev1v2v3 = tetrahedron->create_ksimplex(2);
    trianglev1v3v4 = tetrahedron->create_ksimplex(2);
    trianglev1v2v4 = tetrahedron->create_ksimplex(2);
    trianglev2v4v3 = tetrahedron->create_ksimplex(2);

    // Add tetrahedron (k-simplex, k=3)
    tetrahedron1 = tetrahedron->create_ksimplex(3);

    // Vertices have edges as neighbors and the opposite:
    v1->add_neighbor(e1);
    v2->add_neighbor(e1);

    v1->add_neighbor(e2);
    v3->add_neighbor(e2);

    v1->add_neighbor(e3);
    v4->add_neighbor(e3);

    v2->add_neighbor(e4);
    v4->add_neighbor(e4);

    v2->add_neighbor(e5);
    v3->add_neighbor(e5);

    v3->add_neighbor(e6);
    v4->add_neighbor(e6);

    // Vertices and edges have triangles as neighbors and the opposite
    v1->add_neighbor(trianglev1v2v3);
    v2->add_neighbor(trianglev1v2v3);
    v3->add_neighbor(trianglev1v2v3);
    e1->add_neighbor(trianglev1v2v3);
    e2->add_neighbor(trianglev1v2v3);
    e5->add_neighbor(trianglev1v2v3);

    v1->add_neighbor(trianglev1v3v4);
    v3->add_neighbor(trianglev1v3v4);
    v4->add_neighbor(trianglev1v3v4);
    e2->add_neighbor(trianglev1v3v4);
    e3->add_neighbor(trianglev1v3v4);
    e6->add_neighbor(trianglev1v3v4);

    v1->add_neighbor(trianglev1v2v4);
    v2->add_neighbor(trianglev1v2v4);
    v4->add_neighbor(trianglev1v2v4);
    e1->add_neighbor(trianglev1v2v4);
    e3->add_neighbor(trianglev1v2v4);
    e4->add_neighbor(trianglev1v2v4);

    v2->add_neighbor(trianglev2v4v3);
    v4->add_neighbor(trianglev2v4v3);
    v3->add_neighbor(trianglev2v4v3);
    e4->add_neighbor(trianglev2v4v3);
    e6->add_neighbor(trianglev2v4v3);
    e5->add_neighbor(trianglev2v4v3);

    // All have tetrahedron as neighbors and the opposite
    v1->add_neighbor(tetrahedron1);
    v2->add_neighbor(tetrahedron1);
    v3->add_neighbor(tetrahedron1);
    v4->add_neighbor(tetrahedron1);

    e1->add_neighbor(tetrahedron1);
    e2->add_neighbor(tetrahedron1);
    e3->add_neighbor(tetrahedron1);
    e4->add_neighbor(tetrahedron1);
    e5->add_neighbor(tetrahedron1);
    e6->add_neighbor(tetrahedron1);

    trianglev1v2v3->add_neighbor(tetrahedron1);
    trianglev1v3v4->add_neighbor(tetrahedron1);
    trianglev1v2v4->add_neighbor(tetrahedron1);
    trianglev2v4v3->add_neighbor(tetrahedron1);

    // Triangles have boundary color
    Color *c1 = new BoundaryColor(true);
    Color *c2 = new BoundaryColor(true);
    Color *c3 = new BoundaryColor(true);
    Color *c4 = new BoundaryColor(true);
    trianglev1v2v3->colors.push_back(c1);
    trianglev1v3v4->colors.push_back(c2);
    trianglev1v2v4->colors.push_back(c3);
    trianglev2v4v3->colors.push_back(c4);

    return tetrahedron;
}

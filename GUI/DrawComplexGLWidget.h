#pragma once

#include "TriangulatorGUIsystemHeaders.h"

class SimpCompItem;

class DrawComplexGLWidget :
    public QOpenGLWidget
{
private:
    QStatusBar *drawComplexStatusBar = NULL;
public:
    DrawComplexGLWidget(QWidget* parent) : QOpenGLWidget(parent) { }

    SimpCompItem *item;

    std::vector<ScreenCoords> drawingdata;
    std::vector<EdgeData> edgedata;
    double enveloping_radius;

    int m_posAttr, m_colAttr, m_matrixUniform;
    QOpenGLShaderProgram *m_program;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void create_circleTriangleFan (GLfloat *vertices, int subdivs, double radius);
    void create_rodTriangleFan (GLfloat *vertices1, GLfloat *vertices2, int x1, int y1, int x2, int y2, double radius1, double radius2);
    void draw_triangleFan(QOpenGLFunctions *f, GLfloat *vertices, int numVertices, bool boundary);
    KSimplex* find_nearest_vertex_to_mouse_position(int posx, int posy );
    void paintGL() override;
public:
    void setDrawComplexStatusBar (QStatusBar *drawComplex);
protected:
    void leaveEvent(QEvent *event) override;
    void enterEvent(QEnterEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

};


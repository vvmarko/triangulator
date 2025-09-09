#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <math.h>
#include <QStatusBar>
#include <QMouseEvent>
#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>
#include "SimpCompTableModel.h"
#include "Inspector.h"
#include "Utils.h"

//struct ScreenCoords;

#ifndef SCREENCOORDS_STRUCT
#define SCREENCOORDS_STRUCT

struct ScreenCoords {
  KSimplex *simplex;
  int X;
  int Y;
  double Z;
};

#endif

#ifndef EDGEDATA_STRUCT
#define EDGEDATA_STRUCT

struct EdgeData {
  KSimplex *simplex1;
  KSimplex *simplex2;
};

#endif

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
    void draw_triangleFan(QOpenGLFunctions *f, GLfloat *vertices, int numVertices);
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


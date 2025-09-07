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

    int m_posAttr, m_colAttr, m_matrixUniform;
    QOpenGLShaderProgram *m_program;

//protected:
//    GLfloat vertex1_x_pos;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void create_circleTriangleFan (GLfloat *vertices, int subdivs, double radius);
    void draw_lines(QOpenGLFunctions *f, GLfloat *vertices, int numVertices);
    void draw_triangleFan(QOpenGLFunctions *f, GLfloat *vertices, int numVertices);
    void create_simpcomp_edge(GLfloat *vertices);
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


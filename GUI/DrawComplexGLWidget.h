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

class DrawComplexGLWidget :
    public QOpenGLWidget
{
private:
    QStatusBar *drawComplexStatusBar = NULL;
public:
    DrawComplexGLWidget(QWidget* parent) : QOpenGLWidget(parent) { }

    SimpCompItem *item;

    int m_posAttr, m_colAttr, m_matrixUniform;
    QOpenGLShaderProgram *m_program;

protected:
    GLfloat vertex1_x_pos;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void create_test (GLfloat *vertices, GLfloat *colors);
    void create_circleTriangles (GLfloat *vertices, int subdivs);
    void create_circleTriangleFan (GLfloat *vertices, int subdivs);
    void create_circle (GLfloat *vertices, int subdivs);
    void draw_lines(QOpenGLFunctions *f, GLfloat *vertices, int numVertices);
    void draw_triangleFan(QOpenGLFunctions *f, GLfloat *vertices, int numVertices);
    void draw_triangles(QOpenGLFunctions *f, GLfloat *vertices, int numVertices);
    void draw_testTriangle(QOpenGLFunctions *f, GLfloat *vertices, int numVertices, GLfloat *colors);
    void create_simpcomp_line(GLfloat *vertices);
    void paintGL() override;
public:
    void set_vertex1_x_pos(GLfloat value);
    void setDrawComplexStatusBar (QStatusBar *drawComplex);
protected:
    void leaveEvent(QEvent *event) override;
    void enterEvent(QEnterEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;

};


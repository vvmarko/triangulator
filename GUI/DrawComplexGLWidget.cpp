#include "DrawComplexGLWidget.h"

#include "triangulator.hpp"

void DrawComplexGLWidget::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    //f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    static const char *vertexShaderSource =
            /*"attribute highp vec4 posAttr;\n"
                "uniform highp mat4 matrix;\n"
                "void main(void)\n"
                "{\n"
                "   gl_Position = matrix * posAttr;\n"
                "}"; */
        "attribute highp vec4 posAttr;\n"
        "attribute lowp vec4 colAttr;\n"
        "varying lowp vec4 col;\n"
        "uniform highp mat4 matrix;\n"
        "void main() {\n"
        "   col = colAttr;\n"
        "   gl_Position = matrix * posAttr;\n"
        "}\n";

    static const char *fragmentShaderSource =
        "varying lowp vec4 col;\n"
        "void main() {\n"
        "   gl_FragColor = col;\n"
        "}\n";
    /*"uniform mediump vec4 color;\n"
        "void main(void)\n"
        "{\n"
        "   gl_FragColor = color;\n"
        "}"; */

    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    Q_ASSERT(m_posAttr != -1);
    m_colAttr = m_program->attributeLocation("colAttr");
    Q_ASSERT(m_colAttr != -1);
    m_matrixUniform = m_program->uniformLocation("matrix");
    Q_ASSERT(m_matrixUniform != -1);

    vertex1_x_pos = 115;
}

void DrawComplexGLWidget::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings:
    //m_projection.setToIdentity();
    //m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void DrawComplexGLWidget::create_test (GLfloat *vertices, GLfloat *colors)
{
    static const GLfloat verticesTriangle[] = {
             0.0f,  0.707f,
             -0.5f, -0.5f,
             0.5f, -0.5f
        };

    static const GLfloat colorsTriangle[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
        };

    for (int i = 0; i < 6; i++)
    {
        vertices[i] = verticesTriangle[i];
    }

    for (int i = 0; i < 9; i++)
    {
        colors[i] = colorsTriangle[i];
    }

}

void DrawComplexGLWidget::create_circleTriangles (GLfloat *vertices, int subdivs)
{
    GLfloat fSubDivs = (GLfloat)subdivs;
    double diff = 2 * M_PI / fSubDivs;
    double radius = 4;

    for (int i = 0; i < subdivs; i ++)
    {
        vertices[i * 6] = 0.0f;
        vertices[i * 6 + 1] = 0.0f;
        vertices[i * 6 + 2] = radius * cos((double)i * diff);
        vertices[i * 6 + 3] = radius * sin((double)i * diff);
        vertices[i * 6 + 4] = radius * cos((double)(i + 1) * diff);
        vertices[i * 6 + 5] = radius * sin((double)(i + 1) * diff);
    }
}

void DrawComplexGLWidget::create_circleTriangleFan (GLfloat *vertices, int subdivs)
{
    GLfloat fSubDivs = (GLfloat)subdivs;
    double diff = 2 * M_PI / fSubDivs;
    double radius = 4;

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    for (int i = 0; i <= subdivs; i ++)
    {
        vertices[2 + i * 2] = radius * cos((double)i * diff);
        vertices[2 + i * 2 + 1] = radius * sin((double)i * diff);
    }
}

void DrawComplexGLWidget::create_circle (GLfloat *vertices, int subdivs)
{
    GLfloat fSubDivs = (GLfloat)subdivs;
    double diff = 2 * M_PI / fSubDivs;
    double radius = 4;

    for (int i = 0; i < subdivs; i ++)
    {
        vertices[i * 4] = radius * cos((double)i * diff);
        vertices[i * 4 + 1] = radius * sin((double)i * diff);
        vertices[i * 4 + 2] = radius * cos((double)(i + 1) * diff);
        vertices[i * 4 + 3] = radius * sin((double)(i + 1) * diff);
    }
}

void DrawComplexGLWidget::draw_lines(QOpenGLFunctions *f, GLfloat *vertices, int numVertices)
{
    GLfloat *colors = new GLfloat[numVertices * 3];

    int i;

    GLfloat color = (GLfloat)0.0;

    int numVertices3 = numVertices * 3;

    for (i = 0; i < numVertices3; i++)
    {
        colors[i] = color;
    }

    /*QColor color1(255, 255, 255, 255);

    m_program->enableAttributeArray(m_posAttr);
    m_program->setAttributeArray(m_posAttr, vertices, 2);
    //program.setUniformValue(m_matrixUniform, pmvMatrix);
    m_program->setUniformValue(m_colAttr, color1);
    m_program->bind();

    f->glDrawArrays(GL_LINES, 0, numVertices);

    m_program->disableAttributeArray(m_posAttr); */

    f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    f->glEnableVertexAttribArray(m_posAttr);
    f->glEnableVertexAttribArray(m_colAttr);

    f->glDrawArrays(GL_LINES, 0, numVertices);

    f->glDisableVertexAttribArray(m_colAttr);
    f->glDisableVertexAttribArray(m_posAttr);

    delete[] colors;
}

void DrawComplexGLWidget::draw_triangleFan(QOpenGLFunctions *f, GLfloat *vertices, int numVertices)
{
    GLfloat *colors = new GLfloat[numVertices * 3];

    int i;

    GLfloat color = (GLfloat)0.0;

    int numVertices3 = numVertices * 3;

    for (i = 0; i < numVertices3; i++)
    {
        colors[i] = color;
    }

    f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    f->glEnableVertexAttribArray(m_posAttr);
    f->glEnableVertexAttribArray(m_colAttr);

    f->glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);

    f->glDisableVertexAttribArray(m_colAttr);
    f->glDisableVertexAttribArray(m_posAttr);

    delete[] colors;
}

void DrawComplexGLWidget::draw_triangles(QOpenGLFunctions *f, GLfloat *vertices, int numVertices)
{
    GLfloat *colors = new GLfloat[numVertices * 3];

    int i;

    GLfloat color = (GLfloat)0.0;

    int numVertices3 = numVertices * 3;

    for (i = 0; i < numVertices3; i++)
    {
        colors[i] = color;
    }

    f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    f->glEnableVertexAttribArray(m_posAttr);
    f->glEnableVertexAttribArray(m_colAttr);

    f->glDrawArrays(GL_TRIANGLES, 0, numVertices);

    f->glDisableVertexAttribArray(m_colAttr);
    f->glDisableVertexAttribArray(m_posAttr);

    delete[] colors;
}

void DrawComplexGLWidget::draw_testTriangle(QOpenGLFunctions *f, GLfloat *vertices, int numVertices, GLfloat *colors)
{
    f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    f->glEnableVertexAttribArray(m_posAttr);
    f->glEnableVertexAttribArray(m_colAttr);

    f->glDrawArrays(GL_TRIANGLES, 0, 3);

    f->glDisableVertexAttribArray(m_colAttr);
    f->glDisableVertexAttribArray(m_posAttr);
}

void DrawComplexGLWidget::create_simpcomp_line(GLfloat *vertices)
{
    vertices[0] = vertex1_x_pos;
    vertices[1] = 145.0f;
    vertices[2] = 325.0f;
    vertices[3] = 225.0f;
}

void DrawComplexGLWidget::paintGL()
{
    // Draw the scene:
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    //f->glDisable(GL_DEPTH_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;
    //matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    //m_program->setUniformValue(m_matrixUniform, matrix);
    //GLfloat vertices[6], colors[9];
    //create_test(vertices, colors);
    //draw_testTriangle(f, vertices, 3, colors);

    QRect rect;

    /*if (this->width() > this->height()) {
        rect = QRect(0, 0, 100 * this->width() / this->height(), 100);
    } else {
        rect = QRect(0, 0, 100, 100 * this->height() / this->width());
    } */

    rect = QRect(0, 0, this->width(), this->height());

    matrix.setToIdentity();
    matrix.ortho(rect);
    //m_program->setUniformValue(m_matrixUniform, matrix);
    m_program->setUniformValue(m_matrixUniform, matrix);

    GLfloat vertices_line[4 * 2];
    create_simpcomp_line(vertices_line);
    draw_lines(f, vertices_line, 2);

    /* matrix.translate(20, 20, 0);
    m_program->setUniformValue(m_matrixUniform, matrix);

    draw_lines(f, vertices_line, 2); */

    GLfloat vertices_circle[2 * 10 + 2 + 2]; // last vertex is repeated twice

    create_circleTriangleFan(vertices_circle, 10);

    matrix.setToIdentity();
    matrix.ortho(rect);
    matrix.translate(vertex1_x_pos, 145, 0);
    m_program->setUniformValue(m_matrixUniform, matrix);
    draw_triangleFan(f, vertices_circle, 2 + 10);

    matrix.setToIdentity();
    matrix.ortho(rect);
    matrix.translate(325, 225, 0);
    m_program->setUniformValue(m_matrixUniform, matrix);
    draw_triangleFan(f, vertices_circle, 2 + 10);

}

void DrawComplexGLWidget::set_vertex1_x_pos(GLfloat value)
{
    vertex1_x_pos = value;
}

void DrawComplexGLWidget::setDrawComplexStatusBar (QStatusBar *drawComplex)
{
    drawComplexStatusBar = drawComplex;
}

void DrawComplexGLWidget::leaveEvent(QEvent *event)
{
    if (drawComplexStatusBar != NULL)
    {
        drawComplexStatusBar->showMessage("0, 0");
    }
}

void DrawComplexGLWidget::enterEvent(QEnterEvent *e)
{
    std::string s = std::to_string((int)e->position().x());
    std::string s1 = std::to_string((int)e->position().y());
    if (drawComplexStatusBar != NULL)
    {
            drawComplexStatusBar->showMessage((s + ", " + s1).c_str());
    }
}

void DrawComplexGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    std::string s = std::to_string((int)e->position().x());
    std::string s1 = std::to_string((int)e->position().y());
    if (drawComplexStatusBar != NULL)
    {
        drawComplexStatusBar->showMessage((s + ", " + s1).c_str());
    }
}

void DrawComplexGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    std::string s = std::to_string((int)e->position().x());
    std::string s1 = std::to_string((int)e->position().y());

    QVBoxLayout *layout = new QVBoxLayout();
    QLabel *label = new QLabel();
    label->setText((s + ", " + s1).c_str());
    layout->addWidget(label);

    Inspector *inspector = new Inspector(item->simpComp->elements[0][0], item);

    Utils::openWindowOnRandomPos(inspector);

    inspector->show();
}

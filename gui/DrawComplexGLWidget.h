#pragma once

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <math.h>

class DrawComplexGLWidget :
	public QOpenGLWidget
{
public:
    DrawComplexGLWidget(QWidget* parent) : QOpenGLWidget(parent) { }

    int m_posAttr, m_colAttr, m_matrixUniform;
    QOpenGLShaderProgram *m_program;

protected:
    void initializeGL() override
    {
        // Set up the rendering context, load shaders and other resources, etc.:
        QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();        
        f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);       

        static const char *vertexShaderSource =
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
    }

    void resizeGL(int w, int h) override
    {
        // Update projection matrix and other size related settings:
        //m_projection.setToIdentity();
        //m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);        
    }

    void create_circle (GLfloat *vertices) {
        for (int i = 0; i < 20; i ++) {
            vertices[i * 6] = 0.5f;
            vertices[i * 6 + 1] = 0.5f;
            vertices[i * 6 + 2] = 0.5f + cos((float)i * 2 * M_PI / 20.0);
            vertices[i * 6 + 3] = 0.5f + sin((float)i * 2 * M_PI / 20.0);
            vertices[i * 6 + 4] = 0.5f + cos((float)(i + 1) * 2 * M_PI / 20.0);
            vertices[i * 6 + 5] = 0.5f + sin((float)(i + 1) * 2 * M_PI / 20.0);
        }
    }

    void paintGL() override
    {
        // Draw the scene:
        QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
        f->glClear(GL_COLOR_BUFFER_BIT);

        static const GLfloat vertices[] = {
                 0.0f,  0.707f,
                -0.5f, -0.5f,
                 0.5f, -0.5f
            };

        static const GLfloat colors[] = {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
            };

        GLfloat vertices_circle[2 * 120];
        GLfloat col_circle[2 * 180];

        create_circle(vertices_circle);

        for (int i = 0; i < 120; i++) {
            vertices_circle[120 + i] = vertices_circle[i] + 100.0f;
        }

        for (int i = 0; i < 2 * 180; i++) {
            col_circle[i] = 1.0f;
        }

        f->glDisable(GL_DEPTH_BUFFER_BIT);

        m_program->bind();

        QMatrix4x4 matrix;
        matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        matrix.translate(0, 0, -2);

        m_program->setUniformValue(m_matrixUniform, matrix);

        f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
        f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

        f->glEnableVertexAttribArray(m_posAttr);
        f->glEnableVertexAttribArray(m_colAttr);

        f->glDrawArrays(GL_TRIANGLES, 0, 3);

        f->glDisableVertexAttribArray(m_colAttr);
        f->glDisableVertexAttribArray(m_posAttr);

        f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices_circle);
        f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, col_circle);

        f->glEnableVertexAttribArray(m_posAttr);
        f->glEnableVertexAttribArray(m_colAttr);

        f->glDrawArrays(GL_TRIANGLES, 0, 120);

        f->glDisableVertexAttribArray(m_colAttr);
        f->glDisableVertexAttribArray(m_posAttr);

    }
};


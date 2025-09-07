#include "DrawComplexGLWidget.h"

#include "triangulator.hpp"

void DrawComplexGLWidget::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    //f->glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    f->glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

void DrawComplexGLWidget::resizeGL(int w, int h)
{
    // Update projection matrix and other size related settings:
    //m_projection.setToIdentity();
    //m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}


void DrawComplexGLWidget::create_circleTriangleFan (GLfloat *vertices, int subdivs, double radius)
{
    GLfloat fSubDivs = (GLfloat)subdivs;
    double diff = 2 * M_PI / fSubDivs;

    if(radius < 4) radius = 4; // cutoff on the smallness of the fat vertex size

    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    for (int i = 0; i <= subdivs; i ++)
    {
        vertices[2 + i * 2] = radius * cos((double)i * diff);
        vertices[2 + i * 2 + 1] = radius * sin((double)i * diff);
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

    f->glVertexAttribPointer(m_posAttr, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    f->glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

    f->glEnableVertexAttribArray(m_posAttr);
    f->glEnableVertexAttribArray(m_colAttr);

    // This function actually does the actual drawing of lines on the screen:
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

    // This function actually does the actual drawing of polygons on the screen:
    f->glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);

    f->glDisableVertexAttribArray(m_colAttr);
    f->glDisableVertexAttribArray(m_posAttr);

    delete[] colors;
}

void DrawComplexGLWidget::create_simpcomp_edge(GLfloat *vertices)
{
    // The array of vertices is constructed by the sequence:
    //
    // v1x,v1y,v2x,v2y,v3x,v3y,v4x,v4y,...
    //
    // This will draw a line between v1 and v2, another line
    // between v3 and v4, etc...

    int i=0;
    for(auto edge : edgedata){
        for(auto vertex : drawingdata){
            if(edge.simplex1 == vertex.simplex){
                vertices[i] = static_cast<GLfloat>(vertex.X + (this->width()/2));
                i++;
                // y-axis has an extra minus, to orient it upwards
                vertices[i] = static_cast<GLfloat>(-vertex.Y + (this->height()/2));
                i++;
            }
            if(edge.simplex2 == vertex.simplex){
                vertices[i] = static_cast<GLfloat>(vertex.X + (this->width()/2));
                i++;
                // y-axis has an extra minus, to orient it upwards
                vertices[i] = static_cast<GLfloat>(-vertex.Y + (this->height()/2));
                i++;
            }
        }
    }
}

void DrawComplexGLWidget::paintGL()
{
    int x,y;
    // Draw the scene:
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    m_program->bind();

    QMatrix4x4 matrix;

    QRect rect;

    rect = QRect(0, 0, this->width(), this->height());

    // Create the array of vertices for drawing a set of edges --- we have two
    // vertices times two coordinates per edge, i.e. 4 GLfloat numbers per edge:
    GLfloat vertices_edge[4 * edgedata.size()];
    create_simpcomp_edge(vertices_edge);

    // Draw the set of edges defined by their array of vertices:
    matrix.setToIdentity();
    matrix.ortho(rect);
    m_program->setUniformValue(m_matrixUniform, matrix);
    draw_lines(f, vertices_edge, 2 * edgedata.size());

    // Create the array containing data for drawing a single "node" as a
    // regular 10-polygon (which is a good approximation to a real circle for
    // small radii):
    GLfloat node[2 * 10 + 2 + 2]; // last vertex is repeated twice

    // Now draw one node for every vertex in the simplicial complex:
    for(auto it : drawingdata){
        // Evaluate the radius of a given node
        double radius = 400.0/std::abs(it.Z);

        // Populate the array for drawing a node
        create_circleTriangleFan(node, 10, radius);

        // Draw the node, by setting the center of the polygon at the
        // coordinates of the vertex, and then drawing it on the screen:
        x = it.X + (this->width())/2;
        y = -it.Y + (this->height())/2; // minus to orient the y-axis upwards
        matrix.setToIdentity();
        matrix.ortho(rect);
        matrix.translate(x, y, 0);
        m_program->setUniformValue(m_matrixUniform, matrix);
        draw_triangleFan(f, node, 2 + 10);
    }
}

void DrawComplexGLWidget::setDrawComplexStatusBar (QStatusBar *drawComplex)
{
    drawComplexStatusBar = drawComplex;
}

void DrawComplexGLWidget::leaveEvent(QEvent *event)
{
    if (drawComplexStatusBar != NULL)
    {
        drawComplexStatusBar->showMessage("Click near a vertex to inspect it...");
    }
}

void DrawComplexGLWidget::enterEvent(QEnterEvent *e)
{
    KSimplex *nearestvertex = find_nearest_vertex_to_mouse_position( e->position().x() , e->position().y() );

    if (drawComplexStatusBar != NULL){
            drawComplexStatusBar->showMessage(("Click to inspect vertex " + nearestvertex->print_non_html() + " ...").c_str());
    }
}

void DrawComplexGLWidget::mouseMoveEvent(QMouseEvent *e)
{
    KSimplex *nearestvertex = find_nearest_vertex_to_mouse_position( e->position().x() , e->position().y() );

    if (drawComplexStatusBar != NULL){
        drawComplexStatusBar->showMessage(("Click to inspect vertex " + nearestvertex->print_non_html() + " ...").c_str());
    }
}

KSimplex* DrawComplexGLWidget::find_nearest_vertex_to_mouse_position(int posx, int posy ){
    int xsep, ysep;
    double mindistance;
    KSimplex *nearestvertex;

    // Translate current mouse position to vertex coordinates
    int x = posx - (this->width()/2);
    int y = -(posy - (this->height()/2)); // minus to orient the y-axis upwards

    // Setup initial minimum distance and nearset vertex
    xsep = std::abs(drawingdata[0].X - x);
    ysep = std::abs(drawingdata[0].Y - y);
    mindistance = std::sqrt( xsep*xsep + ysep*ysep );
    nearestvertex = drawingdata[0].simplex;

    // Go through all vertices and find the closest one to the mouse
    for(auto it : drawingdata){
        xsep = std::abs(it.X - x);
        ysep = std::abs(it.Y - y);
        double tempdistance = std::sqrt( xsep*xsep + ysep*ysep );
        if(tempdistance < mindistance){
            mindistance = tempdistance;
            nearestvertex = it.simplex;
        }
    }
    return nearestvertex;
}

void DrawComplexGLWidget::mouseReleaseEvent(QMouseEvent *e)
{
    KSimplex *nearestvertex = find_nearest_vertex_to_mouse_position( e->position().x() , e->position().y() );

    Inspector *inspector = new Inspector(nearestvertex, item);

    Utils::openWindowOnRandomPos(inspector);

    inspector->show();
}

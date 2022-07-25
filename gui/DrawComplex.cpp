#include "DrawComplex.h"
#include "MainWindow.h"
#include <QSlider>

DrawComplex::DrawComplex(MainWindow *mainWnd, QWidget *parent)
	: QWidget(parent)
{    
	ui.setupUi(this);

    this->mainWnd = mainWnd;

    connect(ui.horizontalSlider, &QSlider::valueChanged, this, &DrawComplex::sliderValueChanged);
}

void DrawComplex::sliderValueChanged(int value) {
    ui.openGLWidget->set_vertex1_x_pos(115 + value);
    ui.openGLWidget->repaint();
}

DrawComplex::~DrawComplex()
{
}

void DrawComplex::closeEvent (QCloseEvent* event)
{
    mainWnd->drawComplexWndClosed(this);
}

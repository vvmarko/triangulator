#include "DrawComplex.h"
#include "MainWindow.h"
#include <QSlider>
#include <QSpinBox>

DrawComplex::DrawComplex(MainWindow *mainWnd, QWidget *parent)
	: QWidget(parent)
{    
	ui.setupUi(this);

    this->mainWnd = mainWnd;

    connect(ui.horizontalSlider, &QSlider::valueChanged, this, &DrawComplex::sliderValueChanged);
    connect(ui.spinBox, &QSpinBox::valueChanged, this, &DrawComplex::spinBoxValueChanged);
}

void DrawComplex::sliderValueChanged(int value) {
    ui.openGLWidget->set_vertex1_x_pos(115 + value);
    ui.openGLWidget->repaint();
    ui.spinBox->setValue(value);
}

void DrawComplex::spinBoxValueChanged(int value) {
    ui.openGLWidget->set_vertex1_x_pos(115 + value);
    ui.openGLWidget->repaint();
    ui.horizontalSlider->setValue(value);
}


DrawComplex::~DrawComplex()
{
}

void DrawComplex::closeEvent (QCloseEvent* event)
{
    mainWnd->drawComplexWndClosed(this);
}

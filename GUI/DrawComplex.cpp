#include "DrawComplex.h"
#include "MainWindow.h"
#include <QSlider>
#include <QSpinBox>
#include "SimpCompTableModel.h"

#include "triangulator.hpp"

DrawComplex::DrawComplex(MainWindow *mainWnd, SimpCompItem *item, QWidget *parent)
	: QWidget(parent)
{    
	ui.setupUi(this);

    statusBar = new QStatusBar();

    this->layout()->addWidget (statusBar);

    statusBar->showMessage("0, 0");

    this->mainWnd = mainWnd;
    this->item = item;
    ui.openGLWidget->item = item;

    ui.openGLWidget->setDrawComplexStatusBar(statusBar);    

    connect(ui.horizontalSlider, &QSlider::valueChanged, this, &DrawComplex::sliderValueChanged);
    connect(ui.spinBox, &QSpinBox::valueChanged, this, &DrawComplex::spinBoxValueChanged);

    item->childWindows.push_back(this);
}

void DrawComplex::sliderValueChanged(int value)
{
    ui.openGLWidget->set_vertex1_x_pos(115 + value);
    ui.openGLWidget->repaint();
    ui.spinBox->setValue(value);
}

void DrawComplex::spinBoxValueChanged(int value)
{
    ui.openGLWidget->set_vertex1_x_pos(115 + value);
    ui.openGLWidget->repaint();
    ui.horizontalSlider->setValue(value);
}

DrawComplex::~DrawComplex()
{
}

DrawComplexGLWidget *DrawComplex::getGLWidget()
{
    return ui.openGLWidget;
}

void DrawComplex::closeEvent (QCloseEvent* event)
{    
    item->drawComplexXcoordinate = this->x();
    item->drawComplexYcoordinate = this->y();

    mainWnd->drawComplexWndClosed(this);

    bool erased = false;

    if (ui.openGLWidget->item->removeWindowFromChildWindowsOnClose)
    {
      for (unsigned int i = 0; i < ui.openGLWidget->item->childWindows.size() && !erased; i++)
      {
         if (ui.openGLWidget->item->childWindows[i] == ui.openGLWidget)
         {
             ui.openGLWidget->item->childWindows.erase(ui.openGLWidget->item->childWindows.begin() + i);
             erased = true;
         }
      }
    }
}

void DrawComplex::setStatusMessage(QString s)
{
    statusBar->showMessage(s);
}

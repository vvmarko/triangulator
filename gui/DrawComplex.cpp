#include "DrawComplex.h"
#include "MainWindow.h"

DrawComplex::DrawComplex(MainWindow *mainWnd, QWidget *parent)
	: QWidget(parent)
{    
	ui.setupUi(this);

    this->mainWnd = mainWnd;
}

DrawComplex::~DrawComplex()
{
}

void DrawComplex::closeEvent (QCloseEvent* event)
{
    mainWnd->drawComplexWndClosed(this);
}

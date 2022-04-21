#include "MainWindow.h"
//#include "PrintComplex.h"
#include "SeedComplex.h"

void MainWindow::newFile() {
	//PrintComplex* printDialog = new PrintComplex();

	//printDialog->show();
	SeedComplex* seedDialog = new SeedComplex();

	seedDialog->show();
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionNew, &QAction::triggered, this, &MainWindow::newFile);
}

MainWindow::~MainWindow()
{
}

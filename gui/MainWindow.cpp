#include "MainWindow.h"
#include "PrintComplex.h"

void MainWindow::newFile() {
	PrintComplex* printDialog = new PrintComplex();

	printDialog->show();
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

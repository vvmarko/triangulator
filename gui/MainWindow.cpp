#include "MainWindow.h"
#include "SeedComplex.h"

void MainWindow::newFile() {
	SeedComplex* seedDialog = new SeedComplex(&items, ui.tblComplexes);

	seedDialog->show();
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionNew, &QAction::triggered, this, &MainWindow::newFile);
	
	SimpCompTableModel *model = new SimpCompTableModel(&items);

	ui.tblComplexes->setModel(model);
	ui.tblComplexes->show();
}

MainWindow::~MainWindow()
{
}

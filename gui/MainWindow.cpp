#include "MainWindow.h"
#include "SeedComplex.h"
#include "DrawComplex.h"
#include <QPushButton>
#include "SimpCompTableModel.h"
#include <QMessageBox>

void MainWindow::newFile() {
    SeedComplex* seedDialog = new SeedComplex(this, &items, ui.tblComplexes);

    seedDialog->show();    
}

void MainWindow::tblItemClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    DrawComplex* complexDialog = new DrawComplex();

    complexDialog->show();
}

void MainWindow::createItemWidget(int row) {
    QPushButton *btnDrawComplex = new QPushButton("Crtaj");

    connect(btnDrawComplex, &QPushButton::released, this, &MainWindow::tblItemClick);

    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 2), btnDrawComplex);
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

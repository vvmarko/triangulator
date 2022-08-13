#include "MainWindow.h"
#include "SeedComplex.h"
#include "DrawComplex.h"
#include <QPushButton>
#include "SimpCompTableModel.h"

void MainWindow::newFile() {
    SeedComplex* seedDialog = new SeedComplex(this, &items, ui.tblComplexes);

    seedDialog->show();    
}

void MainWindow::drawComplexWndClosed(DrawComplex *wnd) {
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].drawComplex == wnd) {
            items[i].drawComplex = NULL;
        }
    }
}

void MainWindow::tblItemClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    if (items[i].drawComplex == NULL) {
        items[i].drawComplex = new DrawComplex(this);
        //connect(items[i].drawComplex, &DrawComplex::destroyed, this, &MainWindow::drawComplexWndClosed);
        items[i].drawComplex->show();
    } else {        
        items[i].drawComplex->close();
        delete items[i].drawComplex;
        items[i].drawComplex = NULL;
    }
}

void MainWindow::createItemWidget(int row) {
    QPushButton *btnDrawComplex = new QPushButton("Crtaj");

    connect(btnDrawComplex, &QPushButton::released, this, &MainWindow::tblItemClick);

    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 2), btnDrawComplex);
}

void MainWindow::quit()
{
    QApplication::quit();
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	connect(ui.actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);

    SimpCompTableModel *model = new SimpCompTableModel(&items);

    ui.tblComplexes->setModel(model);
    ui.tblComplexes->show();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QApplication::quit();
}

MainWindow::~MainWindow()
{
}

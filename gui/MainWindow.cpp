#include "MainWindow.h"
#include "SeedComplex.h"
#include "DrawComplex.h"
#include <QPushButton>
#include "SimpCompTableModel.h"
#include "LogViewer.h"
#include "PrintComplex.h"

void MainWindow::newFile() {
    SeedComplex* seedDialog = new SeedComplex(this, &items, ui.tblComplexes);

    seedDialog->show();    
}

void MainWindow::openLogFile() {    
    if (!viewLogFileVisible)
    {
        logViewerDialog = new LogViewer(this, "");

        logViewerDialog->show();
        viewLogFileVisible = true;
    } else
    {
        logViewerDialog->close();
        viewLogFileVisible = false;
        delete logViewerDialog;
        logViewerDialog = NULL;
    }
}

void MainWindow::drawComplexWndClosed(DrawComplex *wnd) {
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].drawComplex == wnd) {
            items[i].drawComplex = NULL;            
        }
    }
}

void MainWindow::printComplexWndClosed(PrintComplex *wnd) {
    for (int i = 0; i < items.size(); i++)
    {
        if (items[i].printComplex == wnd) {
            items[i].printComplex = NULL;            
        }
    }
}

void MainWindow::viewLogFileClosed()
{
    viewLogFileVisible = false;
    delete logViewerDialog;
    logViewerDialog = NULL;
}

void MainWindow::tblItemDrawComplexClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    if (items[i].drawComplex == NULL) {
        items[i].drawComplex = new DrawComplex(this, &(items[i]));
        //connect(items[i].drawComplex, &DrawComplex::destroyed, this, &MainWindow::drawComplexWndClosed);
        items[i].drawComplex->show();        
    } else {        
        items[i].drawComplex->close();
        delete items[i].drawComplex;
        items[i].drawComplex = NULL;
    }
}

void MainWindow::tblItemPrintComplexClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    if (items[i].printComplex == NULL) {
        items[i].printComplex = new PrintComplex(this, "", false, &(items[i]));
        //connect(items[i].drawComplex, &DrawComplex::destroyed, this, &MainWindow::drawComplexWndClosed);                                
        items[i].printComplex->show();
    } else {                        
        items[i].printComplex->close();
        delete items[i].printComplex;
        items[i].printComplex = NULL;
    }
}

void MainWindow::tblItemDeleteRowClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();    

    items[i].removeWindowFromChildWindowsOnClose = false;

    int j = 0;

    for (QWidget *w : items[i].childWindows)
    {
        w->close();
        if (items[i].printComplex == w)
        {
            //items[i].printComplex->close();
            delete items[i].printComplex;
        }
        else if (items[i].drawComplex == w)
        {
            //items[i].drawComplex->close();
            delete items[i].drawComplex;
        }
        else
        {
            delete (Inspector *)w;
        }
        items[i].childWindows[j] = NULL;
        j++;

    }

    items.erase(items.begin() + i);

    SimpCompTableModel* model = new SimpCompTableModel(&items);

    SimpCompTableModel* prevModel;

    prevModel = (SimpCompTableModel*)ui.tblComplexes->model();

    ui.tblComplexes->setModel(model);

    delete prevModel;

    for (int i = 0; i < items.size(); i++)
    {
        createItemWidget(i);
    }

    ui.tblComplexes->show();
}

void MainWindow::createItemWidget(int row) {
    QPushButton *btnPrintComplex = new QPushButton("Listaj");
    QPushButton *btnDrawComplex = new QPushButton("Crtaj");    
    QPushButton *btnDeleteRow = new QPushButton("Obriši");

    connect(btnPrintComplex, &QPushButton::released, this, &MainWindow::tblItemPrintComplexClick);
    connect(btnDrawComplex, &QPushButton::released, this, &MainWindow::tblItemDrawComplexClick);    
    connect(btnDeleteRow, &QPushButton::released, this, &MainWindow::tblItemDeleteRowClick);

    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 2), btnPrintComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 3), btnDrawComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 4), btnDeleteRow);
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
    connect(ui.actionView_Log_File, &QAction::triggered, this, &MainWindow::openLogFile);
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);

    SimpCompTableModel *model = new SimpCompTableModel(&items);

    ui.tblComplexes->setModel(model);
    ui.tblComplexes->show();

    viewLogFileVisible = false;
    logViewerDialog = NULL;
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    QApplication::quit();
}

MainWindow::~MainWindow()
{
}

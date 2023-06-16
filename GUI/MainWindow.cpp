
#include "MainWindow.h"
#include "SeedComplex.h"
#include "DrawComplex.h"
#include <QPushButton>
#include "SimpCompTableModel.h"
#include "LogViewer.h"
#include "PrintComplex.h"
#include <QFileDialog>
#include "Utils.h"
#include <QMessageBox>

#include "triangulator.hpp"

void MainWindow::newFile() {
    SeedComplex* seedDialog = new SeedComplex(this, &items, ui.tblComplexes);

    seedDialog->show();    
}

void MainWindow::openFile() {
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
        tr("Open Simplicial Complex"), "", tr("SimpComp Files (*.xml)"));

    for (const QString& fileName : fileNames)
    {
        SimpCompItem i;

        i.simpComp = read_complex_from_xml_file(fileName.toStdString());

        items.push_back(i);
    }

    updateSimpCompTableModel();

    ui.tblComplexes->show();
}

void MainWindow::openLogFile() {    
    if (!viewLogFileVisible)
    {
        logViewerDialog = new LogViewer(this, "");

        Utils::openWindowOnRandomPos(logViewerDialog);

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
        Utils::openWindowOnRandomPos(items[i].drawComplex);
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
        items[i].printComplex = new PrintComplex(this, QString::fromStdString(items[i].simpComp->print_html()),
                                                 &(items[i]));
        items[i].printComplex->setWindowTitle(QString::fromStdString("Elements of complex " + items[i].simpComp->name));
        Utils::openWindowOnRandomPos(items[i].printComplex);
        items[i].printComplex->show();
    } else {
        items[i].printComplex->close();
        delete items[i].printComplex;
        items[i].printComplex = NULL;
    }
}

void MainWindow::tblItemSaveComplexAsClick() {
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Simplicial Complex"), "", tr("SimpComp Files (*.xml)"));
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    if (fileName.lastIndexOf('.') != -1)
    {
        QString extension;

        extension = fileName.right(fileName.length() - fileName.lastIndexOf('.') - 1);
        if (extension != "xml") {
            QMessageBox msgBox(QMessageBox::Critical, "Error", "File extension must be .xml",
                               QMessageBox::Ok);

            msgBox.exec();

            return;
        }
    }
        else
        {
            fileName += ".xml";
        }

    save_complex_to_xml_file(items[i].simpComp, fileName.toStdString());

    //QFile *f = new QFile(fileName);
    //f->open(QIODeviceBase::NewOnly);
    //QTextStream t(f);

    //t << "aaa\n";
    //f->close();
}

void MainWindow::tblItemDeleteRowClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();    

    QMessageBox msgBox(QMessageBox::Question, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No)
    {
        return;
    }

    items[i].removeWindowFromChildWindowsOnClose = false;

    int j = 0;

    for (QWidget *w : items[i].childWindows)
    {
        w->close();
        if (items[i].printComplex == w)
        {            
            delete items[i].printComplex;
        }
        else if (items[i].drawComplex == w)
        {            
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

    updateSimpCompTableModel();

    ui.tblComplexes->show();
}

void MainWindow::createItemWidget(int row) {
    QPushButton *btnPrintComplex = new QPushButton("List elements");
    QPushButton *btnDrawComplex = new QPushButton("Crtaj");
    QPushButton *btnSaveComplexAs = new QPushButton("Save as");
    QPushButton *btnDeleteRow = new QPushButton("Obriši");

    connect(btnPrintComplex, &QPushButton::released, this, &MainWindow::tblItemPrintComplexClick);
    connect(btnDrawComplex, &QPushButton::released, this, &MainWindow::tblItemDrawComplexClick);    
    connect(btnSaveComplexAs, &QPushButton::released, this, &MainWindow::tblItemSaveComplexAsClick);
    connect(btnDeleteRow, &QPushButton::released, this, &MainWindow::tblItemDeleteRowClick);

    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 3), btnPrintComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 4), btnDrawComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 5), btnSaveComplexAs);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 6), btnDeleteRow);
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
    connect(ui.actionOpen, &QAction::triggered, this, &MainWindow::openFile);   
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

void MainWindow::updateSimpCompTableModel()
{
    SimpCompTableModel* model = new SimpCompTableModel(&items);

    SimpCompTableModel* prevModel;

    prevModel = (SimpCompTableModel*)ui.tblComplexes->model();

    ui.tblComplexes->setModel(model);

    delete prevModel;

    for (int i = 0; i < items.size(); i++)
    {
        createItemWidget(i);
    }
}

MainWindow::~MainWindow()
{
}


#include "MainWindow.h"
#include "SeedComplex.h"
#include "RenameComplex.h"
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
        logViewerDialog = new LogViewer(this);

        if( (logWindowXcoordinate!=-1)||(logWindowYcoordinate!=-1) )
            logViewerDialog->move(logWindowXcoordinate,logWindowYcoordinate);
        else Utils::openWindowOnRandomPos(logViewerDialog);

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
    for (unsigned long i = 0; i < items.size(); i++)
    {
        if (items[i].drawComplex == wnd) {
            items[i].drawComplex = NULL;            
        }
    }
}

void MainWindow::printComplexWndClosed(PrintComplex *wnd) {
    for (unsigned long i = 0; i < items.size(); i++)
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
        items[i].drawComplex = new DrawComplex(this, items[i].simpComp, &(items[i]));
        items[i].drawComplex->setWindowTitle(QString::fromStdString("Triangulator visualize complex: " + items[i].simpComp->name));

        if( (items[i].drawComplexXcoordinate!=-1)||(items[i].drawComplexYcoordinate!=-1) )
            items[i].drawComplex->move(items[i].drawComplexXcoordinate,items[i].drawComplexYcoordinate);
        else Utils::openWindowOnRandomPos(items[i].drawComplex);

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
        items[i].printComplex->setWindowTitle(QString::fromStdString("Triangulator catalogue for complex: " + items[i].simpComp->name));

        if( (items[i].printComplexXcoordinate!=-1)||(items[i].printComplexYcoordinate!=-1) )
            items[i].printComplex->move(items[i].printComplexXcoordinate,items[i].printComplexYcoordinate);
        else Utils::openWindowOnRandomPos(items[i].printComplex);

        items[i].printComplex->show();
    } else {
        items[i].printComplex->close();
        delete items[i].printComplex;
        items[i].printComplex = NULL;
    }
}

void MainWindow::tblItemOpenToolsClick(int index) {

    QComboBox *btn = (QComboBox *)sender();
    int row = ui.tblComplexes->indexAt(btn->pos()).row();

    btn->setCurrentIndex(0); // reset the status of the ComboBox

    if (index==0) return;
    if (index==1) return;
    if (index==2) tblItemSaveComplexAsClick(row);
    if (index==3) tblItemRenameComplexClick(row);
    if (index==4) tblItemDeleteRowClick(row);
}

void MainWindow::tblItemOpenActionsClick(int index) {

    QComboBox *btn = (QComboBox *)sender();
//    int row = ui.tblComplexes->indexAt(btn->pos()).row();

    btn->setCurrentIndex(0); // reset the status of the ComboBox

    if (index==0) return;
    if (index==1) return;
    if (index==2) return; // call Pachner grow
    if (index==3) return; // call Boundary grow
    if (index==4) return; // call Random grow
    if (index==5) return; // call Colorize simplices at level
    if (index==6) return; // call Colorize entire complex
}

void MainWindow::tblItemSaveComplexAsClick(int row) {
    QString simpcompname=QString::fromStdString(items[row].simpComp->name);
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Simplicial Complex"), simpcompname, tr("SimpComp Files (*.xml)"));

    if (fileName.lastIndexOf('.') != -1)
    {
        QString extension;

        extension = fileName.right(fileName.length() - fileName.lastIndexOf('.') - 1);
        if (extension != "xml") fileName += ".xml";
    }
    else
    {
        fileName+=".xml";
    }
    save_complex_to_xml_file(items[row].simpComp, fileName.toStdString());
}

void MainWindow::tblItemRenameComplexClick(int row) {
    RenameComplex* renameDialog = new RenameComplex(this, items[row], ui.tblComplexes);

    renameDialog->show();
}


void MainWindow::tblItemDeleteRowClick(int row) {
    QMessageBox msgBox(QMessageBox::Question, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);

    if (msgBox.exec() == QMessageBox::No) return;

    items[row].removeWindowFromChildWindowsOnClose = false;

    int j = 0;

    for (QWidget *w : items[row].childWindows)
    {
        w->close();
        if (items[row].printComplex == w)
        {            
            delete items[row].printComplex;
        }
        else if (items[row].drawComplex == w)
        {            
            delete items[row].drawComplex;
        }
        else
        {
            delete (Inspector *)w;
        }
        items[row].childWindows[j] = NULL;
        j++;
    }
    unseed_complex(items[row].simpComp);
    items.erase(items.begin() + row);

    updateSimpCompTableModel();

    ui.tblComplexes->show();
}

void MainWindow::createItemWidget(int row) {
    QPushButton *btnPrintComplex = new QPushButton("Catalogue");
    QPushButton *btnDrawComplex = new QPushButton("Visualize");
    QComboBox *cmbTools = new QComboBox();
    QComboBox *cmbActions = new QComboBox();

    cmbTools->addItem("Tools...");
    cmbTools->insertSeparator(1);
    cmbTools->addItem("Save as");
    cmbTools->addItem("Rename");
    cmbTools->addItem("Delete");

    cmbActions->addItem("Actions...");
    cmbActions->insertSeparator(1);
    cmbActions->addItem("Pachner grow");
    cmbActions->addItem("Bounday grow");
    cmbActions->addItem("Random grow");
    cmbActions->addItem("Colorize simplices at level");
    cmbActions->addItem("Colorize entire complex");

    connect(btnPrintComplex, &QPushButton::released, this, &MainWindow::tblItemPrintComplexClick);
    connect(btnDrawComplex, &QPushButton::released, this, &MainWindow::tblItemDrawComplexClick);    
    connect(cmbTools, SIGNAL(activated(int)), this, SLOT(tblItemOpenToolsClick(int)));
    connect(cmbActions, SIGNAL(activated(int)), this, SLOT(tblItemOpenActionsClick(int)));

    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 3), btnPrintComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 4), btnDrawComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 5), cmbTools);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 6), cmbActions);

}

void MainWindow::quit()
{
    QApplication::quit();
}

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

    setWindowTitle("Triangulator GUI");

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    triangulator_global::logLevel=LOG_INFO;

    QString qendMessage = "<b>Triangulator GUI session, finished on: ";

    QDateTime dt = QDateTime::currentDateTime();
    QString formattedTime = dt.toString("dd.MM.yyyy hh:mm:ss");
    qendMessage = qendMessage + formattedTime + "</b><br>";
    std::string endMessage = qendMessage.toStdString();

    log_report(LOG_INFO,endMessage);

    unseed_everything();

    event->accept();
}

void MainWindow::updateSimpCompTableModel()
{
    SimpCompTableModel* model = new SimpCompTableModel(&items);

    SimpCompTableModel* prevModel;

    prevModel = (SimpCompTableModel*)ui.tblComplexes->model();

    ui.tblComplexes->setModel(model);

    delete prevModel;

    for (unsigned long i = 0; i < items.size(); i++)
    {
        createItemWidget(i);
    }
}

MainWindow::~MainWindow()
{
}


#include "triangulator.hpp"
#include "TriangulatorGUI.h"

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
        viewLogFileClosed();
    }
}

void MainWindow::drawComplexWndClosed(DrawComplex *wnd)
{
    if(wnd != nullptr) wnd->close();
}

void MainWindow::printComplexWndClosed(PrintComplex *wnd)
{
    if(wnd != nullptr) wnd->close();
}

void MainWindow::viewLogFileClosed()
{
    viewLogFileVisible = false;
    delete logViewerDialog;
    logViewerDialog = nullptr;
}

void MainWindow::tblItemDrawComplexClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    if (items[i].drawComplex == nullptr) {
        items[i].drawComplex = new DrawComplex(this, items[i].simpComp, &(items[i]));
        items[i].drawComplex->setWindowTitle(QString::fromStdString("Triangulator visualize complex: " + items[i].simpComp->name));

        if( (items[i].drawComplexXcoordinate!=-1)||(items[i].drawComplexYcoordinate!=-1) )
            items[i].drawComplex->move(items[i].drawComplexXcoordinate,items[i].drawComplexYcoordinate);
        else Utils::openWindowOnRandomPos(items[i].drawComplex);

        items[i].drawComplex->show();
    } else drawComplexWndClosed(items[i].drawComplex);
}

void MainWindow::tblItemPrintComplexClick() {
    QPushButton *btn = (QPushButton *)sender();
    int i = ui.tblComplexes->indexAt(btn->pos()).row();

    if (items[i].printComplex == nullptr) {
        items[i].printComplex = new PrintComplex(this, QString::fromStdString(items[i].simpComp->print_html()),
                                                 &(items[i]));
        items[i].printComplex->setWindowTitle(QString::fromStdString("Triangulator catalogue for complex: " + items[i].simpComp->name));

        if( (items[i].printComplexXcoordinate!=-1)||(items[i].printComplexYcoordinate!=-1) )
            items[i].printComplex->move(items[i].printComplexXcoordinate,items[i].printComplexYcoordinate);
        else Utils::openWindowOnRandomPos(items[i].printComplex);

        items[i].printComplex->show();
    } else printComplexWndClosed(items[i].printComplex);
}

void MainWindow::tblItemOpenToolsClick(int index) {

    QComboBox *btn = (QComboBox *)sender();
    int row = ui.tblComplexes->indexAt(btn->pos()).row();

    btn->setCurrentIndex(0); // reset the status of the ComboBox

    if (index==0) return;
    if (index==1) return;
    // Tools: Save as
    if (index==2) tblItemSaveComplexAsClick(row);
    // Tools: Rename
    if (index==3) tblItemRenameComplexClick(row);
    // Tools: Delete
    if (index==4) tblItemDeleteRowClick(row);
}

void MainWindow::tblItemOpenActionsClick(int index) {

    QComboBox *btn = (QComboBox *)sender();
//    int row = ui.tblComplexes->indexAt(btn->pos()).row();

    btn->setCurrentIndex(0); // reset the status of the ComboBox

    if (index==0) return;
    if (index==1) return;
    // Actions: Pachner grow
    if (index==2) { notImplementedYetMessage(); return; } // call Pachner grow
    // Actions: Boundary grow
    if (index==3) { notImplementedYetMessage(); return; } // call Boundary grow
    // Actions: Random grow
    if (index==4) { notImplementedYetMessage(); return; } // call Random grow
    // Actions: Colorize simplices at level
    if (index==5) { notImplementedYetMessage(); return; } // call Colorize simplices at level
    // Actions: Colorize entire complex
    if (index==6) { notImplementedYetMessage(); return; } // call Colorize entire complex
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

void MainWindow::tblItemDeleteRow(int row)
{
    drawComplexWndClosed(items[row].drawComplex);
    printComplexWndClosed(items[row].printComplex);

    while(items[row].childWindows.empty() == false)
    {
        Inspector *w = items[row].childWindows[0];
        w->close();
    }

    unseed_complex(items[row].simpComp);
    items.erase(items.begin() + row);

    updateSimpCompTableModel();
    ui.tblComplexes->show();
}

void MainWindow::tblItemDeleteRowClick(int row) {
    QMessageBox msgBox(QMessageBox::Question, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
    if (msgBox.exec() == QMessageBox::No) return;
    tblItemDeleteRow(row);
}

void MainWindow::unseedAllComplexes() {
    QMessageBox msgBox(QMessageBox::Question, "Confirm", "Are you sure?", QMessageBox::Yes | QMessageBox::No);
    if (msgBox.exec() == QMessageBox::No) return;

    int row = 0;

    while(items.empty() == false){
        row = items.size() - 1;
        tblItemDeleteRow(row);
    }
}

void MainWindow::notImplementedYetMessage() {
    QMessageBox msgBox;
    msgBox.setText("Not implemented yet. Fix me!");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Triangulator GUI");
    msgBox.exec();
}

void MainWindow::AboutMessage() {
    QMessageBox msgBox;
    QString message = "Triangulator GUI is a part of the Triangulator software project, ";
            message += "see<br><br>";
            message += "<a href=\"https://github.com/vvmarko/triangulator\">https://github.com/vvmarko/triangulator</a><br><br>";
            message += "for details.<br><br>";
            message += "Its purpose is to showcase the capabilities of the Triangulator library ";
            message += "of routines for creation, manipulation and various numerical simulations ";
            message += "involving simplicial complexes and triangulations of manifolds.<br><br>";
            message += "The GUI provides an interactive way to construct simplicial complexes, ";
            message += "manipulate their structure, draw them on the screen, and inspect the ";
            message += "properties of individual simplices present in the complex.<br><br>";
            message += "This is version 0.5, work in progress... :-)<br><br>";
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setWindowTitle("About Triangulator GUI");
    msgBox.exec();
}


void MainWindow::DocumentationMessage() {
    QMessageBox msgBox;
    QString message = "Triangulator GUI is a part of the Triangulator software project, ";
            message += "see<br><br>";
            message += "<a href=\"https://github.com/vvmarko/triangulator\">https://github.com/vvmarko/triangulator</a><br><br>";
            message += "for details.<br><br>";
            message += "THe official documentation is yet to be written, it is on our TODO list. ";
            message += "In the meantime, you are welcome to read the source code, we try to keep it reasonably ";
            message += "well commented.<br><br>";
            message += "Use the Source, Luke! :-)<br><br>";
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setTextFormat(Qt::RichText);
    msgBox.setText(message);
    msgBox.setWindowTitle("About Triangulator GUI");
    msgBox.exec();
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

    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 3), btnPrintComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 4), btnDrawComplex);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 5), cmbTools);
    ui.tblComplexes->setIndexWidget(ui.tblComplexes->model()->index(row, 6), cmbActions);

    // #################
    // Table row buttons
    // #################

    // Catalogue
    connect(btnPrintComplex, &QPushButton::released, this, &MainWindow::tblItemPrintComplexClick);

    // Visualize
    connect(btnDrawComplex, &QPushButton::released, this, &MainWindow::tblItemDrawComplexClick);

    // Tools
    connect(cmbTools, SIGNAL(activated(int)), this, SLOT(tblItemOpenToolsClick(int)));

    // Actions
    connect(cmbActions, SIGNAL(activated(int)), this, SLOT(tblItemOpenActionsClick(int)));
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

    SimpCompTableModel *model = new SimpCompTableModel(&items);

    ui.tblComplexes->setModel(model);
    ui.tblComplexes->show();

    viewLogFileVisible = false;
    logViewerDialog = NULL;

    // #########
    // Main menu
    // #########

    // File menu:
    // Seed new complex
    connect(ui.actionNew, &QAction::triggered, this, &MainWindow::newFile);
    // Open complex from file
    connect(ui.actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    // Open session from file
    connect(ui.actionOpen_session, &QAction::triggered, this, &MainWindow::notImplementedYetMessage);
    // Save current session
    connect(ui.actionSave_session, &QAction::triggered, this, &MainWindow::notImplementedYetMessage);
    // Unseed everything
    connect(ui.actionUnseed_everything, &QAction::triggered, this, &MainWindow::unseedAllComplexes);
    // Quit
    connect(ui.actionQuit, &QAction::triggered, this, &MainWindow::quit);

    // View menu:
    // View log file
    connect(ui.actionView_Log_File, &QAction::triggered, this, &MainWindow::openLogFile);

    // Actions menu:
    // Relabel UniqueID colors
    connect(ui.actionRelabel_UniqueID_colors, &QAction::triggered, this, &MainWindow::notImplementedYetMessage);
    // Find simplices with color type
    connect(ui.actionFind_simplices_with_color_type, &QAction::triggered, this, &MainWindow::notImplementedYetMessage);
    // Find simplices with color value
    connect(ui.actionFind_simplices_with_color_value, &QAction::triggered, this, &MainWindow::notImplementedYetMessage);

    // Settings menu:
    // Options
    connect(ui.actionOptions, &QAction::triggered, this, &MainWindow::notImplementedYetMessage);

    // Help menu:
    // Documentation
    connect(ui.actionDocumentation, &QAction::triggered, this, &MainWindow::DocumentationMessage);
    // About
    connect(ui.actionAbout, &QAction::triggered, this, &MainWindow::AboutMessage);
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

    unseedAllComplexes();
    unseed_everything();

    if (viewLogFileVisible == true)
    {
        logViewerDialog->close();
        viewLogFileClosed();
    }

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

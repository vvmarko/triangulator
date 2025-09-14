#pragma once

#include "TriangulatorGUIsystemHeaders.h"
#include "ui_MainWindow.h"

class LogViewer;
class DrawComplex;
class PrintComplex;
class SimpCompItem;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

    void createItemWidget(int row);

    void drawComplexWndClosed(DrawComplex *wnd);
    void printComplexWndClosed(PrintComplex *wnd);
    void viewLogFileClosed();

    void updateSimpCompTableModel();

    void notImplementedYetMessage();

    int logWindowXcoordinate = -1;
    int logWindowYcoordinate = -1;

public slots:
    void tblItemOpenToolsClick(int index);
    void tblItemOpenActionsClick(int index);

protected:
    void closeEvent(QCloseEvent *e) override;

private:
	Ui::MainWindow ui;

    bool viewLogFileVisible;
    LogViewer* logViewerDialog;

	void newFile();
    void openFile();    
    void openLogFile();
    void quit();

    void tblItemPrintComplexClick();    
    void tblItemDrawComplexClick();    
    void tblItemSaveComplexAsClick(int row);
    void tblItemRenameComplexClick(int row);
    void tblItemDeleteRowClick(int row);

    void unseedAllComplexes();

    void DocumentationMessage();
    void AboutMessage();

    std::vector<SimpCompItem> items;
};

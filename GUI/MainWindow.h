#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "SimpCompTableModel.h"

class LogViewer;

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

public slots:
    void tblItemOpenToolsClick(int index);
    void tblItemOpenActionsClick(int index);

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


    std::vector<SimpCompItem> items;

    void closeEvent(QCloseEvent *e) override;    
};

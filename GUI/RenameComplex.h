#pragma once

#include "SimpCompTableModel.h"
#include "TriangulatorGUIsystemHeaders.h"
#include "ui_RenameComplex.h"

class MainWindow;

class RenameComplex : public QWidget
{
    Q_OBJECT

public:
    RenameComplex(MainWindow *mainWnd, SimpCompItem item, QTableView *table, QWidget *parent = Q_NULLPTR);
    ~RenameComplex();
private:
    Ui::RenameComplex ui;
		
    void handleAccepted();
    void handleRejected();

    MainWindow *mainWnd;
    SimpCompItem item;
    QTableView *table;	
};

#pragma once

#include "TriangulatorGUIsystemHeaders.h"
#include "ui_SeedComplex.h"

class MainWindow;
class SimpCompItem;

class SeedComplex : public QWidget
{
	Q_OBJECT

public:
    SeedComplex(MainWindow *mainWnd, std::vector<SimpCompItem> *items, QTableView *table, QWidget *parent = Q_NULLPTR);
	~SeedComplex();

private:
	Ui::SeedComplex ui;
		
	void handleAccepted();
	void handleRejected();

    MainWindow *mainWnd;
    std::vector<SimpCompItem>* items = 0;
	QTableView* table;	
};

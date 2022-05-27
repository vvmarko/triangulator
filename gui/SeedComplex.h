#pragma once

#include <QWidget>
#include "ui_SeedComplex.h"
#include "SimpCompTableModel.h"
#include <qtableview.h>

using namespace std;

class SeedComplex : public QWidget
{
	Q_OBJECT

public:
	SeedComplex(vector<SimpCompItem> *items, QTableView *table, QWidget *parent = Q_NULLPTR);
	~SeedComplex();

private:
	Ui::SeedComplex ui;
		
	void handleAccepted();
	void handleRejected();

	vector<SimpCompItem>* items = 0;
	QTableView* table;	
};

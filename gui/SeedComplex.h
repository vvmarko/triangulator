#pragma once

#include <QWidget>
#include "ui_SeedComplex.h"

class SeedComplex : public QWidget
{
	Q_OBJECT

public:
	SeedComplex(QWidget *parent = Q_NULLPTR);
	~SeedComplex();

private:
	Ui::SeedComplex ui;
		
	void handleAccepted();
	void handleRejected();
};

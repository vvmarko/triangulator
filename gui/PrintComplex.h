#pragma once

#include <QWidget>
#include "ui_PrintComplex.h"

class PrintComplex : public QWidget
{
	Q_OBJECT

public:
	PrintComplex(QWidget *parent = Q_NULLPTR);
	~PrintComplex();

private:
	Ui::PrintComplex ui;
};

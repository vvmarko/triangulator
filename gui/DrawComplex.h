#pragma once

#include <QWidget>
#include "ui_DrawComplex.h"

class DrawComplex : public QWidget
{
	Q_OBJECT

public:
	DrawComplex(QWidget *parent = Q_NULLPTR);
	~DrawComplex();

private:
	Ui::DrawComplex ui;
};

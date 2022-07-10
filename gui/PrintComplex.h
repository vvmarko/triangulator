#pragma once

#include <QWidget>
#include "ui_PrintComplex.h"
using namespace std;

class PrintComplex : public QWidget
{
	Q_OBJECT

public:
    PrintComplex(QString displayStr, bool displayLink, QWidget *parent = Q_NULLPTR);
	~PrintComplex();

private:
	Ui::PrintComplex ui;

    void anchorClicked(const QUrl &url);
};

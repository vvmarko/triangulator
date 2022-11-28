#pragma once

#include <QWidget>
#include "ui_PrintComplex.h"
using namespace std;

class MainWindow;
class SimpCompItem;

class PrintComplex : public QWidget
{
	Q_OBJECT

public:
    PrintComplex(MainWindow *mainWnd, QString displayStr, bool displayLink, SimpCompItem *item, QWidget *parent = Q_NULLPTR);
	~PrintComplex();

    SimpCompItem *item;    

    MainWindow *mainWnd;

private:
	Ui::PrintComplex ui;

    void anchorClicked(const QUrl &url);    

    void closeEvent(QCloseEvent *e) override;

};

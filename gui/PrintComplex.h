#pragma once

#include <QWidget>
#include "ui_PrintComplex.h"
using namespace std;

class MainWindow;

class PrintComplex : public QWidget
{
	Q_OBJECT

public:
    PrintComplex(MainWindow *mainWnd, QString displayStr, bool displayLink, QWidget *parent = Q_NULLPTR);
	~PrintComplex();

    PrintComplex *parent;

    std::vector<PrintComplex*> childWindows;

    MainWindow *mainWnd;

    void closeWindowAndChildWindows(PrintComplex *window);

private:
	Ui::PrintComplex ui;

    void anchorClicked(const QUrl &url);    

    void closeEvent(QCloseEvent *e) override;

};

#pragma once

#include "TriangulatorGUIsystemHeaders.h"
#include "ui_PrintComplex.h"

class MainWindow;
class SimpCompItem;

class PrintComplex : public QWidget
{
	Q_OBJECT

public:
    PrintComplex(MainWindow *mainWnd, QString displayStr, SimpCompItem *item, QWidget *parent = Q_NULLPTR);
	~PrintComplex();

    void refreshCatalog(SimpCompItem *item);

    SimpCompItem *item;    

    MainWindow *mainWnd;

    Ui::PrintComplex ui;

private:

    void anchorClicked(const QUrl &url);    

    void closeEvent(QCloseEvent *e) override;

};

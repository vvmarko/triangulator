#pragma once

#include <QWidget>
#include "ui_DrawComplex.h"
#include <QStatusBar>
#include <QMouseEvent>

class MainWindow;
class SimpCompItem;

class DrawComplex : public QWidget
{
	Q_OBJECT

public:
    DrawComplex(MainWindow *mainWnd, SimpCompItem *item, QWidget *parent = Q_NULLPTR);
	~DrawComplex();    

    DrawComplexGLWidget *getGLWidget();

private:
	Ui::DrawComplex ui;

    MainWindow *mainWnd;
    SimpCompItem *item;

    QStatusBar *statusBar;   

    void closeEvent(QCloseEvent* event) override;

    void sliderValueChanged(int value);
    void spinBoxValueChanged(int value);

protected:
    void setStatusMessage(QString s);
};

#pragma once

#include <QWidget>
#include "ui_DrawComplex.h"

class MainWindow;

class DrawComplex : public QWidget
{
	Q_OBJECT

public:
    DrawComplex(MainWindow *mainWnd, QWidget *parent = Q_NULLPTR);
	~DrawComplex();

private:
	Ui::DrawComplex ui;

    MainWindow *mainWnd;

    void closeEvent(QCloseEvent* event) override;

    void sliderValueChanged(int value);
    void spinBoxValueChanged(int value);
};

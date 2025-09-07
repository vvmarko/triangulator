#pragma once

#include <QWidget>
#include "DrawComplexGLWidget.h"
#include <QSpinBox>
#include <QSlider>
#include "ui_DrawComplex.h"
#include <QStatusBar>
#include <QMouseEvent>

class MainWindow;
class SimpCompItem;
class ScreenParameters;

//class KSimplex;
//class SimpComp;

#ifndef EMBDATA_STRUCT
#define EMBDATA_STRUCT

struct EmbData {
  KSimplex *simplex;
  std::vector<double> x;
};

#endif


class DrawComplex : public QWidget
{
	Q_OBJECT

public:
    DrawComplex(MainWindow *mainWnd, SimpComp *simpComp, SimpCompItem *item, QWidget *parent = Q_NULLPTR);
	~DrawComplex();    

    DrawComplexGLWidget *getGLWidget();

private:
	Ui::DrawComplex ui;

    MainWindow *mainWnd;
    SimpComp *simpComp;
    SimpCompItem *item;
    ScreenParameters *scrparam;
    std::vector<EmbData> coords;

    QStatusBar *statusBar;
    QSlider *sliderd;
    QSpinBox *spinboxd;
    QSlider *slidersx;
    QSpinBox *spinboxsx;
    QSlider *slidersy;
    QSpinBox *spinboxsy;
    QSlider *slidersz;
    QSpinBox *spinboxsz;
    std::vector<QSlider*> sliderAlpha;
    std::vector<QSlider*> sliderBeta;
    std::vector<QSlider*> sliderGamma;

    std::vector<QDoubleSpinBox*> spinboxAlpha;
    std::vector<QDoubleSpinBox*> spinboxBeta;
    std::vector<QDoubleSpinBox*> spinboxGamma;

    void closeEvent(QCloseEvent* event) override;

    void sliderdValueChanged(int value);
    void spinboxdValueChanged(int value);
    void slidersxValueChanged(int value);
    void spinboxsxValueChanged(int value);
    void slidersyValueChanged(int value);
    void spinboxsyValueChanged(int value);
    void sliderszValueChanged(int value);
    void spinboxszValueChanged(int value);

    void sliderAlphaValueChanged(int value, int counter);
    void spinboxAlphaValueChanged(double value, int counter);
    void sliderBetaValueChanged(int value, int counter);
    void spinboxBetaValueChanged(double value, int counter);
    void sliderGammaValueChanged(int value, int counter);
    void spinboxGammaValueChanged(double value, int counter);

protected:
    void setStatusMessage(QString s);
};

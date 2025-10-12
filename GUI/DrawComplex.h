#pragma once

#include "TriangulatorGUIsystemHeaders.h"

class SimpComp;
class KSimplex;
class MainWindow;
class SimpCompItem;
class ScreenParameters;

#include "ui_DrawComplex.h"

class DrawComplex : public QWidget
{
    Q_OBJECT

public:
    DrawComplex(MainWindow *mainWnd, SimpComp *simpComp, SimpCompItem *item, QWidget *parent = Q_NULLPTR);
    ~DrawComplex();    

    void refreshVisualizer();

    DrawComplexGLWidget *getGLWidget();

private:
    Ui::DrawComplex ui;

    MainWindow *mainWnd;
    SimpComp *simpComp;
    SimpCompItem *item;
    ScreenParameters *scrparam;
    std::vector<EmbData> coords;
    std::vector<EmbData> coordLines;
    double coordinateAxisLength = 100.0;
  
    double enveloping_radius;
    double default_scale;

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

/*  // We comment out the sz user interface, since we do not want the user to manipulate
    // it. Instead we use the hard-coded value sz = 20, since that is an optimal value for
    // the graphics rendering. We leave the commented-out code here, in case someone wants
    // to try it out anyway...

    void sliderszValueChanged(int value);
    void spinboxszValueChanged(int value);
*/

    void sliderAlphaValueChanged(int value, int counter);
    void spinboxAlphaValueChanged(double value, int counter);
    void sliderBetaValueChanged(int value, int counter);
    void spinboxBetaValueChanged(double value, int counter);
    void sliderGammaValueChanged(int value, int counter);
    void spinboxGammaValueChanged(double value, int counter);

    double find_enveloping_sphere_radius(std::vector<EmbData> coords);

protected:
    void setStatusMessage(QString s);
};

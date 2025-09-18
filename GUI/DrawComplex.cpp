
#include "triangulator.hpp"
#include "TriangulatorGUI.h"

DrawComplex::DrawComplex(MainWindow *cmainWnd, SimpComp *csimpComp, SimpCompItem *citem, QWidget *parent)
	: QWidget(parent)
{    
    // Housekeeping
    this->mainWnd = cmainWnd;
    this->simpComp = csimpComp;
    this->item = citem;

    // Initialize screen parameters and embedding coordinates
    this->scrparam = setup_screen_parameters(simpComp);
    this->coords = extract_embedding_data(simpComp);

    // Set up basic user interface and add self to list of open windows
	ui.setupUi(this);

    // Add status bar to the user interface
    statusBar = new QStatusBar();
    this->layout()->addWidget (statusBar);
    statusBar->showMessage("Click near a vertex to inspect it...");

    // #################################################
    // Populate user interface with four scaling sliders
    // #################################################

    // Label for the parameter d
    QLabel *labelDistance = new QLabel("  Screen distance:");
    ui.verticalLayout->addWidget(labelDistance);
    labelDistance->show();

    // Horizontal layout for the parameter d
    QHBoxLayout *hboxd = new QHBoxLayout();
    ui.verticalLayout->addLayout(hboxd);

    // Slider for the parameter d
    QSlider *csliderd = new QSlider(Qt::Horizontal);
    this->sliderd = csliderd;
    sliderd->setRange(0, 100); // Sets the minimum and maximum values
    sliderd->setValue(0); // Sets the current value
    sliderd->setSingleStep(1);
    sliderd->setPageStep(10);
    sliderd->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    sliderd->setFixedWidth(250);
    hboxd->addWidget(sliderd);
    sliderd->show();

    // Spinbox for the parameter d
    QSpinBox *cspinboxd = new QSpinBox();
    this->spinboxd = cspinboxd;
    spinboxd->setRange(0,100);
    spinboxd->setValue(0);
    spinboxd->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    spinboxd->setFixedWidth(55);
    hboxd->addWidget(spinboxd);
    spinboxd->show();

    // Synchronize the values of the slider and the spinbox
    connect(sliderd, &QSlider::valueChanged, this, &DrawComplex::sliderdValueChanged);
    connect(spinboxd, &QSpinBox::valueChanged, this, &DrawComplex::spinboxdValueChanged);

/*  // We comment out the sz user interface, since we do not want the user to manipulate
    // it. Instead we use the hard-coded value sz = 20, since that is an optimal value for
    // the graphics rendering. We leave the commented-out code here, in case someone wants
    // to try it out anyway...

    // Label for the parameter sz
    QLabel *labelEye = new QLabel("  Eye distance:");
    ui.verticalLayout->addWidget(labelEye);
    labelEye->show();

    // Horizontal layout for the parameter sz
    QHBoxLayout *hboxsz = new QHBoxLayout();
    ui.verticalLayout->addLayout(hboxsz);

    // Slider for the parameter sz
    QSlider *cslidersz = new QSlider(Qt::Horizontal);
    this->slidersz = cslidersz;
    slidersz->setRange(1, 100); // Sets the minimum and maximum values
    slidersz->setValue(2); // Sets the current value
    slidersz->setSingleStep(1);
    slidersz->setPageStep(10);
    slidersz->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    slidersz->setFixedWidth(250);
    hboxsz->addWidget(slidersz);
    slidersz->show();

    // Spinbox for the parameter sz
    QSpinBox *cspinboxsz = new QSpinBox();
    this->spinboxsz = cspinboxsz;
    spinboxsz->setRange(1,100);
    spinboxsz->setValue(2);
    spinboxsz->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    spinboxsz->setFixedWidth(55);
    hboxsz->addWidget(spinboxsz);
    spinboxsz->show();

    // Synchronize the values of the slider and the spinbox
    connect(slidersz, &QSlider::valueChanged, this, &DrawComplex::sliderszValueChanged);
    connect(spinboxsz, &QSpinBox::valueChanged, this, &DrawComplex::spinboxszValueChanged);
*/

    // Label for the parameter sx
    QLabel *labelScalex = new QLabel("  Horizontal scale:");
    ui.verticalLayout->addWidget(labelScalex);
    labelScalex->show();

    // Horizontal layout for the parameter sx
    QHBoxLayout *hboxsx = new QHBoxLayout();
    ui.verticalLayout->addLayout(hboxsx);

    // Slider for the parameter sx
    QSlider *cslidersx = new QSlider(Qt::Horizontal);
    this->slidersx = cslidersx;
    slidersx->setRange(-100, 100); // Sets the minimum and maximum values
    slidersx->setValue(0); // Sets the current value
    slidersx->setSingleStep(1);
    slidersx->setPageStep(10);
    slidersx->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    slidersx->setFixedWidth(250);
    hboxsx->addWidget(slidersx);
    slidersx->show();

    // Spinbox for the parameter sx
    QSpinBox *cspinboxsx = new QSpinBox();
    this->spinboxsx = cspinboxsx;
    spinboxsx->setRange(-100,100);
    spinboxsx->setValue(0);
    spinboxsx->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    spinboxsx->setFixedWidth(55);
    hboxsx->addWidget(spinboxsx);
    spinboxsx->show();

    // Synchronize the values of the slider and the spinbox
    connect(slidersx, &QSlider::valueChanged, this, &DrawComplex::slidersxValueChanged);
    connect(spinboxsx, &QSpinBox::valueChanged, this, &DrawComplex::spinboxsxValueChanged);

    // Label for the parameter sy
    QLabel *labelScaley = new QLabel("  Vertical scale:");
    ui.verticalLayout->addWidget(labelScaley);
    labelScaley->show();

    // Horizontal layout for the parameter sy
    QHBoxLayout *hboxsy = new QHBoxLayout();
    ui.verticalLayout->addLayout(hboxsy);

    // Slider for the parameter sy
    QSlider *cslidersy = new QSlider(Qt::Horizontal);
    this->slidersy = cslidersy;
    slidersy->setRange(-100, 100); // Sets the minimum and maximum values
    slidersy->setValue(0); // Sets the current value
    slidersy->setSingleStep(1);
    slidersy->setPageStep(10);
    slidersy->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    slidersy->setFixedWidth(250);
    hboxsy->addWidget(slidersy);
    slidersy->show();

    // Spinbox for the parameter sy
    QSpinBox *cspinboxsy = new QSpinBox();
    this->spinboxsy = cspinboxsy;
    spinboxsy->setRange(-100,100);
    spinboxsy->setValue(0);
    spinboxsy->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    spinboxsy->setFixedWidth(55);
    hboxsy->addWidget(spinboxsy);
    spinboxsy->show();

    // Synchronize the values of the slider and the spinbox
    connect(slidersy, &QSlider::valueChanged, this, &DrawComplex::slidersyValueChanged);
    connect(spinboxsy, &QSpinBox::valueChanged, this, &DrawComplex::spinboxsyValueChanged);

    // ################################################
    // Populate user interface with alpha angle sliders
    // ################################################

    this->sliderAlpha.clear();
    this->spinboxAlpha.clear();
    if(scrparam->alpha.size() >2){ // set up slider only for nontrivial angles
        for(unsigned long int i = 1; i < scrparam->alpha.size() - 1; i++){ // skip first and final angle, they are constants

            // Label for the parameter alpha_i
            QString str;
            QLabel *labelAlpha = new QLabel("  Alpha angle " + str.setNum(i) + ":");
            ui.verticalLayout->addWidget(labelAlpha);
            labelAlpha->show();

            // Horizontal layout for the parameter alpha_i
            QHBoxLayout *hboxAlpha = new QHBoxLayout();
            ui.verticalLayout->addLayout(hboxAlpha);

            // Slider for the parameter alpha_i
            QSlider *sliderAlphaTemp = new QSlider(Qt::Horizontal);
            this->sliderAlpha.push_back(sliderAlphaTemp);
            sliderAlphaTemp->setRange(0,100); // Sets the minimum and maximum values
            sliderAlphaTemp->setValue(static_cast<int>(std::round(scrparam->alpha[i]*100.0/scrparam->alphaMax[i]))); // Sets the current value
            sliderAlphaTemp->setSingleStep(1);
            sliderAlphaTemp->setPageStep(10);
            sliderAlphaTemp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            sliderAlphaTemp->setFixedWidth(250);
            hboxAlpha->addWidget(sliderAlphaTemp);
            sliderAlphaTemp->show();

            // Spinbox for the parameter alpha_i
            QDoubleSpinBox *spinboxAlphaTemp = new QDoubleSpinBox();
            this->spinboxAlpha.push_back(spinboxAlphaTemp);
            spinboxAlphaTemp->setRange(scrparam->alphaMin[i],scrparam->alphaMax[i]);
            spinboxAlphaTemp->setValue(scrparam->alpha[i]);
            spinboxAlphaTemp->setSingleStep(0.05);
            spinboxAlphaTemp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            spinboxAlphaTemp->setFixedWidth(55);
            hboxAlpha->addWidget(spinboxAlphaTemp);
            spinboxAlphaTemp->show();

            // Synchronize the values of the slider and the spinbox
            connect(this->sliderAlpha[i-1], &QSlider::valueChanged, this, [this, i](int signalValue) {
                this->sliderAlphaValueChanged(signalValue, i);
            });
            connect(this->spinboxAlpha[i-1], &QDoubleSpinBox::valueChanged, this, [this, i](double signalValue) {
                this->spinboxAlphaValueChanged(signalValue, i);
            });
        }
    }


    // ###############################################
    // Populate user interface with beta angle sliders
    // ###############################################

    this->sliderBeta.clear();
    this->spinboxBeta.clear();
    if(scrparam->beta.size() >2){ // set up slider only for nontrivial angles
        for(unsigned long int i = 1; i < scrparam->beta.size() - 1; i++){ // skip first and final angle, they are constants

            // Label for the parameter beta_i
            QString str;
            QLabel *labelBeta = new QLabel("  Beta angle " + str.setNum(i) + ":");
            ui.verticalLayout->addWidget(labelBeta);
            labelBeta->show();

            // Horizontal layout for the parameter beta_i
            QHBoxLayout *hboxBeta = new QHBoxLayout();
            ui.verticalLayout->addLayout(hboxBeta);

            // Slider for the parameter beta_i
            QSlider *sliderBetaTemp = new QSlider(Qt::Horizontal);
            this->sliderBeta.push_back(sliderBetaTemp);
            sliderBetaTemp->setRange(0,100); // Sets the minimum and maximum values
            sliderBetaTemp->setValue(static_cast<int>(std::round(scrparam->beta[i]*100.0/scrparam->betaMax[i]))); // Sets the current value
            sliderBetaTemp->setSingleStep(1);
            sliderBetaTemp->setPageStep(10);
            sliderBetaTemp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            sliderBetaTemp->setFixedWidth(250);
            hboxBeta->addWidget(sliderBetaTemp);
            sliderBetaTemp->show();

            // Spinbox for the parameter beta_i
            QDoubleSpinBox *spinboxBetaTemp = new QDoubleSpinBox();
            this->spinboxBeta.push_back(spinboxBetaTemp);
            spinboxBetaTemp->setRange(scrparam->betaMin[i],scrparam->betaMax[i]);
            spinboxBetaTemp->setValue(scrparam->beta[i]);
            spinboxBetaTemp->setSingleStep(0.05);
            spinboxBetaTemp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            spinboxBetaTemp->setFixedWidth(55);
            hboxBeta->addWidget(spinboxBetaTemp);
            spinboxBetaTemp->show();

            // Synchronize the values of the slider and the spinbox
            connect(this->sliderBeta[i-1], &QSlider::valueChanged, this, [this, i](int signalValue) {
                this->sliderBetaValueChanged(signalValue, i);
            });
            connect(this->spinboxBeta[i-1], &QDoubleSpinBox::valueChanged, this, [this, i](double signalValue) {
                this->spinboxBetaValueChanged(signalValue, i);
            });
        }
    }


    // ################################################
    // Populate user interface with gamma angle sliders
    // ################################################

    this->sliderGamma.clear();
    this->spinboxGamma.clear();
    if(scrparam->gamma.size() >2){ // set up slider only for nontrivial angles
        for(unsigned long int i = 1; i < scrparam->gamma.size() - 1; i++){ // skip first and final angle, they are constants

            // Label for the parameter gamma_i
            QString str;
            QLabel *labelGamma = new QLabel("  Gamma angle " + str.setNum(i) + ":");
            ui.verticalLayout->addWidget(labelGamma);
            labelGamma->show();

            // Horizontal layout for the parameter gamma_i
            QHBoxLayout *hboxGamma = new QHBoxLayout();
            ui.verticalLayout->addLayout(hboxGamma);

            // Slider for the parameter gamma_i
            QSlider *sliderGammaTemp = new QSlider(Qt::Horizontal);
            this->sliderGamma.push_back(sliderGammaTemp);
            sliderGammaTemp->setRange(0,100); // Sets the minimum and maximum values
            sliderGammaTemp->setValue(static_cast<int>(std::round(scrparam->gamma[i]*100.0/scrparam->gammaMax[i]))); // Sets the current value
            sliderGammaTemp->setSingleStep(1);
            sliderGammaTemp->setPageStep(10);
            sliderGammaTemp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            sliderGammaTemp->setFixedWidth(250);
            hboxGamma->addWidget(sliderGammaTemp);
            sliderGammaTemp->show();

            // Spinbox for the parameter gamma_i
            QDoubleSpinBox *spinboxGammaTemp = new QDoubleSpinBox();
            this->spinboxGamma.push_back(spinboxGammaTemp);
            spinboxGammaTemp->setRange(scrparam->gammaMin[i],scrparam->gammaMax[i]);
            spinboxGammaTemp->setValue(scrparam->gamma[i]);
            spinboxGammaTemp->setSingleStep(0.05);
            spinboxGammaTemp->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
            spinboxGammaTemp->setFixedWidth(55);
            hboxGamma->addWidget(spinboxGammaTemp);
            spinboxGammaTemp->show();

            // Synchronize the values of the slider and the spinbox
            connect(this->sliderGamma[i-1], &QSlider::valueChanged, this, [this, i](int signalValue) {
                this->sliderGammaValueChanged(signalValue, i);
            });
            connect(this->spinboxGamma[i-1], &QDoubleSpinBox::valueChanged, this, [this, i](double signalValue) {
                this->spinboxGammaValueChanged(signalValue, i);
            });
        }
    }

    // Fill the space below the sliders with strechable space
    ui.verticalLayout->insertStretch(-1,0);

    // Introduce some gui-useful defaults for screen parameters
    this->enveloping_radius = find_enveloping_sphere_radius(this->coords);
    this->scrparam->d = this->enveloping_radius;
    // For setting the default scale below, 500 is the initial height of the
    // drawing viewport (specified in file DrawComplex.ui), while 10 is the
    // scaling parameter that nicely fills in the viewport for the hard-coded
    // value sz = 20. Changing any of these parameters will likely only make
    // the picture worse...
    this->scrparam->sz = 20.0; // Hard-coded value for sz, the optimal one
    if (scrparam->Damb < 3){
        this->default_scale = 2 * enveloping_radius / 500;
    } else {
        this->default_scale = 2 * (scrparam->sz) / 500;
    }
//    this->default_scale = 2 * enveloping_radius / (500 * 10);
    this->scrparam->sx = default_scale;
    this->scrparam->sy = default_scale;

    // Set up the drawing widget
    ui.openGLWidget->item = item;
    ui.openGLWidget->edgedata = extract_edge_data(simpComp);
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->enveloping_radius = this->enveloping_radius;
    ui.openGLWidget->setDrawComplexStatusBar(statusBar);
}

void DrawComplex::sliderdValueChanged(int value)
{
    scrparam->d = enveloping_radius + (value * 10.0);
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxd->setValue(value);
}

void DrawComplex::spinboxdValueChanged(int value)
{
    scrparam->d = enveloping_radius + (value * 10.0);
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    sliderd->setValue(value);
}

void DrawComplex::slidersxValueChanged(int value)
{
    // We use the function
    //
    // f(x) = 1/2 ( -x/10 + \sqrt{4 + (x/10)^2} )
    //
    // to smoothly interpolate the slider input [-100,100] to the actual scaling
    // parameter [10, 1/10], such that slider input 0 gives no scaling (f(0)=1)...
    double scale = ( -(value/10.0) + sqrt(4 + (value/10.0) * (value/10.0)) )/2;
    scrparam->sx = default_scale * scale;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxsx->setValue(value);
}

void DrawComplex::spinboxsxValueChanged(int value)
{
    // We use the function
    //
    // f(x) = 1/2 ( -x/10 + \sqrt{4 + (x/10)^2} )
    //
    // to smoothly interpolate the slider input [-100,100] to the actual scaling
    // parameter [10, 1/10], such that slider input 0 gives no scaling (f(0)=1)...
    double scale = ( -(value/10.0) + sqrt(4 + (value/10.0) * (value/10.0)) )/2;
    scrparam->sx = default_scale * scale;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    slidersx->setValue(value);
}

void DrawComplex::slidersyValueChanged(int value)
{
    // We use the function
    //
    // f(x) = 1/2 ( -x/10 + \sqrt{4 + (x/10)^2} )
    //
    // to smoothly interpolate the slider input [-100,100] to the actual scaling
    // parameter [10, 1/10], such that slider input 0 gives no scaling (f(0)=1)...
    double scale = ( -(value/10.0) + sqrt(4 + (value/10.0) * (value/10.0)) )/2;
    scrparam->sy = default_scale * scale;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxsy->setValue(value);
}

void DrawComplex::spinboxsyValueChanged(int value)
{
    // We use the function
    //
    // f(x) = 1/2 ( -x/10 + \sqrt{4 + (x/10)^2} )
    //
    // to smoothly interpolate the slider input [-100,100] to the actual scaling
    // parameter [10, 1/10], such that slider input 0 gives no scaling (f(0)=1)...
    double scale = ( -(value/10.0) + sqrt(4 + (value/10.0) * (value/10.0)) )/2;
    scrparam->sy = default_scale * scale;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    slidersy->setValue(value);
}

/*  // We comment out the sz user interface, since we do not want the user to manipulate
    // it. Instead we use the hard-coded value sz = 20, since that is an optimal value for
    // the graphics rendering. We leave the commented-out code here, in case someone wants
    // to try it out anyway...

void DrawComplex::sliderszValueChanged(int value)
{
    scrparam->sz = value * 10.0;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxsz->setValue(value);
}

void DrawComplex::spinboxszValueChanged(int value)
{
    scrparam->sz = value * 10.0;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    slidersz->setValue(value);
}
*/

void DrawComplex::sliderAlphaValueChanged(int value, int i)
{
    scrparam->alpha[i] = value * (scrparam->alphaMax[i]) / 100.0;
    if(scrparam->alpha[i] == 0.0) scrparam->alpha[i] = 0.01;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxAlpha[i-1]->setValue(scrparam->alpha[i]);
    // Counting members in the spinboxAlpha vector starts from zero,
    // while the counting of corresponding angles in the vector alpha
    // starts from one (alpha[0] is a constant angle that we ignore).
    // Hence "i-1" in the assignment above.
}

void DrawComplex::spinboxAlphaValueChanged(double value, int i)
{
    scrparam->alpha[i] = value;
    if(scrparam->alpha[i] == 0.0) scrparam->alpha[i] = 0.01;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    int num = static_cast<int>(std::round(value * 100.0 / scrparam->alphaMax[i]));
    sliderAlpha[i-1]->setValue(num);
    // Counting members in the spinboxAlpha vector starts from zero,
    // while the counting of corresponding angles in the vector alpha
    // starts from one (alpha[0] is a constant angle that we ignore).
    // Hence "i-1" in the assignment above.
}


void DrawComplex::sliderBetaValueChanged(int value, int i)
{
    scrparam->beta[i] = value * (scrparam->betaMax[i]) / 100.0;
    if(scrparam->beta[i] == 0.0) scrparam->beta[i] = 0.01;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxBeta[i-1]->setValue(scrparam->beta[i]);
    // Counting members in the spinboxBeta vector starts from zero,
    // while the counting of corresponding angles in the vector beta
    // starts from one (beta[0] is a constant angle that we ignore).
    // Hence "i-1" in the assignment above.
}

void DrawComplex::spinboxBetaValueChanged(double value, int i)
{
    scrparam->beta[i] = value;
    if(scrparam->beta[i] == 0.0) scrparam->beta[i] = 0.01;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    int num = static_cast<int>(std::round(value * 100.0 / scrparam->betaMax[i]));
    sliderBeta[i-1]->setValue(num);
    // Counting members in the spinboxAlpha vector starts from zero,
    // while the counting of corresponding angles in the vector alpha
    // starts from one (alpha[0] is a constant angle that we ignore).
    // Hence "i-1" in the assignment above.
}

void DrawComplex::sliderGammaValueChanged(int value, int i)
{
    scrparam->gamma[i] = value * (scrparam->gammaMax[i]) / 100.0;
    if(scrparam->gamma[i] == 0.0) scrparam->gamma[i] = 0.01;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    spinboxGamma[i-1]->setValue(scrparam->gamma[i]);
    // Counting members in the spinboxGamma vector starts from zero,
    // while the counting of corresponding angles in the vector gamma
    // starts from one (gamma[0] is a constant angle that we ignore).
    // Hence "i-1" in the assignment above.
}

void DrawComplex::spinboxGammaValueChanged(double value, int i)
{
    scrparam->gamma[i] = value;
    if(scrparam->gamma[i] == 0.0) scrparam->gamma[i] = 0.01;
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
    int num = static_cast<int>(std::round(value * 100.0 / scrparam->gammaMax[i]));
    sliderGamma[i-1]->setValue(num);
    // Counting members in the spinboxAlpha vector starts from zero,
    // while the counting of corresponding angles in the vector alpha
    // starts from one (alpha[0] is a constant angle that we ignore).
    // Hence "i-1" in the assignment above.
}

void DrawComplex::refreshVisualizer()
{
    this->coords = extract_embedding_data(simpComp);
    ui.openGLWidget->edgedata = extract_edge_data(simpComp);
    ui.openGLWidget->drawingdata = evaluate_perspective_projection(coords,scrparam);
    ui.openGLWidget->repaint();
}

double DrawComplex::find_enveloping_sphere_radius(std::vector<EmbData> ccoords)
{
    double temp;
    double max = 0.0;

    // Initialize max as a radius-squared of the first set of coordinates
    for(long unsigned int i = 0; i < ccoords[0].x.size(); i++) max += ccoords[0].x[i] * ccoords[0].x[i];

    // Go through all coordinates, evaluate radius-squared, and remember the biggest one
    for(auto &it : ccoords){
        temp = 0.0;
        for(long unsigned int j = 0; j < it.x.size(); j++) temp += it.x[j] * it.x[j];
        if(temp > max) max = temp;
    }
    // Return the biggest radius
    return sqrt(max);
}

DrawComplex::~DrawComplex()
{
}

DrawComplexGLWidget *DrawComplex::getGLWidget()
{
    return ui.openGLWidget;
}

void DrawComplex::closeEvent (QCloseEvent* event)
{    
    item->drawComplexXcoordinate = this->x();
    item->drawComplexYcoordinate = this->y();
    if (item->drawComplex != nullptr) item->drawComplex = nullptr;

    delete scrparam;

    event->accept();
}

void DrawComplex::setStatusMessage(QString s)
{
    statusBar->showMessage(s);
}

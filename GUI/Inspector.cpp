
#include "triangulator.hpp"
#include "TriangulatorGUI.h"

void Inspector::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().right(url.fileName().indexOf("http://triangulatorgui.com/"));
    QString str2 = str1.left(str1.indexOf(".html"));

    KSimplex *csimplex = item->simpComp->find_KSimplex(str2.toInt());

    Inspector *inspector = new Inspector(csimplex, item);
    Utils::openWindowOnRandomPos(inspector);

    inspector->show();    
}

void Inspector::activateBtnAttachNewSimplex() {
    KSimplex* newSimplex = attach_new_simplex_to_boundary(simplex , simpComp);
    UniqueIDColor::append_color_to_entire_complex(simpComp);
    evaluate_potential_minimum(simpComp);
    evaluate_embedding_coordinates(simpComp);

    if (item->printComplex != nullptr) {
        item->printComplex->refreshCatalog();
    }
    if (item->drawComplex != nullptr) {
        item->drawComplex->refreshVisualizer();
    }
    refreshAllInspectors();

    if(newSimplex != nullptr){
        Inspector *newinspector = new Inspector(newSimplex, item);
        Utils::openWindowOnRandomPos(newinspector);
        newinspector->show();
    }
}

void Inspector::activateBtnApplyPachnerMove() {
    KSimplex* newSimplex = Pachner_move(simplex , simpComp);
    UniqueIDColor::append_color_to_entire_complex(simpComp);
    evaluate_potential_minimum(simpComp);
    evaluate_embedding_coordinates(simpComp);

    if (item->printComplex != nullptr) {
        item->printComplex->refreshCatalog();
    }
    if (item->drawComplex != nullptr) {
        item->drawComplex->refreshVisualizer();
    }

    if(newSimplex != nullptr){
        Inspector *newinspector = new Inspector(newSimplex, item);
        Utils::openWindowOnRandomPos(newinspector);
        newinspector->show();
    }
    this->close();
    refreshAllInspectors();
}

void resizeLabelToFitContents(QLabel *label) {
    QRect r = QFontMetrics(label->font()).boundingRect( label->text() + "  ");

//    label->adjustSize();
    label->setMinimumWidth(r.width());
    label->setMaximumWidth(r.width());
}

Inspector::Inspector(KSimplex *csimplex, SimpCompItem *citem, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    // Initialize the inspector window and ui
    ui->setupUi(this);    
    setWindowTitle("Triangulator inspector");
    resizeLabelToFitContents(ui->lSimplexNameLbl);
    resizeLabelToFitContents(ui->lLevelLbl);
    resizeLabelToFitContents(ui->lElementOfComplexLbl);
    resizeLabelToFitContents(ui->lDimensionLbl);

    // Housekeeping
    this->simpComp = citem->simpComp;
    this->simplex = csimplex;
    this->item = citem;
    item->childWindows.push_back(this);

    // Display basic data about the simplex
    std::string simplexMark = simplex->print_non_html();
    std::string level = to_string(simplex->k);
    std::string complexName = simpComp->name;
    std::string dimension = to_string(simpComp->D);
    ui->lSimplexMark->setText(QString::fromStdString(simplexMark));
    ui->lLevel->setText(QString::fromStdString(level));
    ui->lElementOfComplex->setText(QString::fromStdString(complexName));
    ui->lDimension->setText(QString::fromStdString(dimension));

    // Display the white field with neighbor data in HTML syntax
    std::string contentsWhiteField = simplex->neighbors->print_html();
    ui->tbNeighbors->setText(QString::fromStdString(contentsWhiteField));
    ui->tbNeighbors->setOpenLinks(false);

    // Set the background of the color list widget to be the same as the
    // background for other members of the Inspector window, for example
    // the label widget
    QPalette labelpalette = ui->label->palette();
    QPalette tbColorspalette = ui->tbColors->palette();
    QColor activewindowcolor = labelpalette.color(QPalette::Active,QPalette::Window);
    QColor inactivewindowcolor = labelpalette.color(QPalette::Inactive,QPalette::Window);
    tbColorspalette.setColor(QPalette::Active,QPalette::Base,activewindowcolor);
    tbColorspalette.setColor(QPalette::Inactive,QPalette::Base,inactivewindowcolor);
    ui->tbColors->setPalette(tbColorspalette);

    // Prepare the string containing the color data in HTML syntax
    QString str = "<table>";
    for(auto col : simplex->colors)
    {
       std::string colorName = get_color_name_from_type(col->type) + " (type " + to_string(col->type) + "):   ";
       std::string colorValue = col->get_color_value_as_str();
       str += "<tr><td><b>" + QString::fromStdString(colorName) + "</b></td><td>";
       str += QString::fromStdString(colorValue) + "</td></tr>";
    }
    str+="</table>";

    // Display the string with color data in the color list widget
    ui->tbColors->setText(str);

    // React on clicking on the link in the neighbor table
    connect(ui->tbNeighbors, &QTextBrowser::anchorClicked, this, &Inspector::anchorClicked);

    // React on clicking the three buttons at the bottom
    connect(ui->btnAttachNewSimplex,  &QPushButton::clicked, this, &Inspector::activateBtnAttachNewSimplex);
    connect(ui->btnApplyPachnerMove,  &QPushButton::clicked, this, &Inspector::activateBtnApplyPachnerMove);
    connect(ui->btnAddNewColor,  &QPushButton::clicked, this, &Inspector::notImplementedYetMessage);
}

void Inspector::refreshAllInspectors()
{
    for(auto insp : item->childWindows){
        if(insp != nullptr)
        {
            // Refresh the simplex label
            std::string simplexMark = insp->simplex->print_non_html();
            insp->ui->lSimplexMark->setText(QString::fromStdString(simplexMark));

            // Refresh the white field with neighbor data in HTML syntax
            std::string contentsWhiteField = insp->simplex->neighbors->print_html();
            insp->ui->tbNeighbors->setText(QString::fromStdString(contentsWhiteField));
            insp->ui->tbNeighbors->setOpenLinks(false);

            // Prepare the string containing the color data in HTML syntax
            QString str = "<table>";
            for(auto col : insp->simplex->colors)
            {
               std::string colorName = get_color_name_from_type(col->type) + " (type " + to_string(col->type) + "):   ";
               std::string colorValue = col->get_color_value_as_str();
               str += "<tr><td><b>" + QString::fromStdString(colorName) + "</b></td><td>";
               str += QString::fromStdString(colorValue) + "</td></tr>";
            }
            str+="</table>";

            // Refresh the color data in the color list widget
            insp->ui->tbColors->setText(str);
        }
    }
}

void Inspector::notImplementedYetMessage()
{
    QMessageBox msgBox;
    msgBox.setText("Not implemented yet. Fix me!");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle("Triangulator GUI");
    msgBox.exec();
}

Inspector::~Inspector()
{
    delete ui;
}

void Inspector::closeEvent (QCloseEvent* event)
{
    bool erased = false;
    event->accept();

    for (unsigned long i = 0; i < item->childWindows.size() && !erased; i++)
        if (item->childWindows[i] == this)
        {
            item->childWindows.erase(item->childWindows.begin() + i);
            erased = true;
        }
}

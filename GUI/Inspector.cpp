#include "Inspector.h"
#include "ui_Inspector.h"
#include "SimpCompTableModel.h"
#include "Utils.h"
#include "MainWindow.h"
#include <QMessageBox>

#include "triangulator.hpp"

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

    //    w->updateSimpCompTableModel();

    if(newSimplex!=nullptr){
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

    //    w->updateSimpCompTableModel();

    if(newSimplex!=nullptr){
        Inspector *newinspector = new Inspector(newSimplex, item);
        Utils::openWindowOnRandomPos(newinspector);
        newinspector->show();
    }
    this->close();
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
    ui->setupUi(this);    

    setWindowTitle("Triangulator inspector");

    resizeLabelToFitContents(ui->lSimplexNameLbl);
    resizeLabelToFitContents(ui->lLevelLbl);
    resizeLabelToFitContents(ui->lElementOfComplexLbl);
    resizeLabelToFitContents(ui->lDimensionLbl);

    SimpComp *csimpComp = citem->simpComp;
    this->simpComp = csimpComp;
    this->simplex = csimplex;

//    UniqueIDColor* idColor = simplex->get_uniqueID();
    std::string simplexMark =  simplex->print_non_html();
    ui->lSimplexMark->setText (QString::fromStdString(simplexMark));
    std::string level = to_string(simplex->k);
    std::string complexName = simpComp->name;

    std::string dimension = to_string(simpComp->D);

    std::string contentsWhiteField = simplex->neighbors->print_html();
    ui->lLevel->setText (QString::fromStdString(level));
    ui->lElementOfComplex->setText (QString::fromStdString(complexName));
    ui->lDimension->setText (QString::fromStdString(dimension));

    ui->tbNeighbors->setText(QString::fromStdString(contentsWhiteField));
    // ui->lCoords->setText(coords);

    QString str = "<table>";
    for(auto col : simplex->colors)
    {
       std::string colorName = get_color_name_from_type(col->type) + " (type " + to_string(col->type) + "):   ";
       std::string colorValue = col->get_color_value_as_str();
       str += "<tr><td><b>" + QString::fromStdString(colorName) + "</b></td><td>";
       str += QString::fromStdString(colorValue) + "</td></tr>";
    }
    str+="</table>";

    // Set the background of the color list widget to be the same as the
    // background for other members of the Inspector window, like the label
    // widget.
    QPalette labelpalette = ui->label->palette();
    QPalette tbColorspalette = ui->tbColors->palette();
    QColor activewindowcolor = labelpalette.color(QPalette::Active,QPalette::Window);
    QColor inactivewindowcolor = labelpalette.color(QPalette::Inactive,QPalette::Window);
    tbColorspalette.setColor(QPalette::Active,QPalette::Base,activewindowcolor);
    tbColorspalette.setColor(QPalette::Inactive,QPalette::Base,inactivewindowcolor);

    ui->tbColors->setPalette(tbColorspalette);

    ui->tbColors->setText(str);

    this->item = citem;

    ui->tbNeighbors->setOpenLinks(false);
    connect(ui->tbNeighbors, &QTextBrowser::anchorClicked, this, &Inspector::anchorClicked);
    connect(ui->btnAttachNewSimplex,  &QPushButton::clicked, this, &Inspector::activateBtnAttachNewSimplex);
    connect(ui->btnApplyPachnerMove,  &QPushButton::clicked, this, &Inspector::activateBtnApplyPachnerMove);
    connect(ui->btnAddNewColor,  &QPushButton::clicked, this, &Inspector::notImplementedYetMessage);

    item->childWindows.push_back(this);

}

void Inspector::notImplementedYetMessage() {
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

    if (item->removeWindowFromChildWindowsOnClose)
    {
      for (unsigned long i = 0; i < item->childWindows.size() && !erased; i++)
        if (item->childWindows[i] == this)
        {
            item->childWindows.erase(item->childWindows.begin() + i);
            erased = true;
        }
    }
    if(event == nullptr) return; // This is a dummy command to satisfy the compiler, do not remove
}

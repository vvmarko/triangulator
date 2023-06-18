#include "Inspector.h"
#include "ui_Inspector.h"
#include "SimpCompTableModel.h"
#include "Utils.h"

#include "triangulator.hpp"

void Inspector::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().right(url.fileName().indexOf("http://triangulatorgui.com/"));
    QString str2 = str1.left(str1.indexOf(".html"));

    KSimplex *simplex = item->simpComp->find_KSimplex(str2.toInt());

    Inspector *inspector = new Inspector(simplex, item);
    Utils::openWindowOnRandomPos(inspector);

    inspector->show();    
}

void resizeLabelToFitContents(QLabel *label) {
    QRect r = QFontMetrics(label->font()).boundingRect( label->text() + "  ");

//    label->adjustSize();
    label->setMinimumWidth(r.width());
    label->setMaximumWidth(r.width());
}

Inspector::Inspector(KSimplex *simplex, SimpCompItem *item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);    

    resizeLabelToFitContents(ui->lSimplexNameLbl);
    resizeLabelToFitContents(ui->lLevelLbl);
    resizeLabelToFitContents(ui->lElementOfComplexLbl);
    resizeLabelToFitContents(ui->lDimensionLbl);

    SimpComp *simpComp = item->simpComp;
//    this->simpComp = simpComp;
    this->simplex = simplex;

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

    ui->tbColors->setText(str);

    this->item = item;

    ui->tbNeighbors->setOpenLinks(false);
    connect(ui->tbNeighbors, &QTextBrowser::anchorClicked, this, &Inspector::anchorClicked);

    item->childWindows.push_back(this);

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

}

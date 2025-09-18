
#include "triangulator.hpp"
#include "TriangulatorGUI.h"

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().right(url.fileName().indexOf("http://triangulatorgui.com/"));
    QString str2 = str1.left(str1.indexOf(".html"));

    KSimplex *simplex = item->simpComp->find_KSimplex(str2.toInt());

    Inspector *inspector = new Inspector(simplex, item);

    Utils::openWindowOnRandomPos(inspector);

    inspector->show();
}

PrintComplex::PrintComplex(MainWindow *cmainWnd, QString displayStr, SimpCompItem *citem, QWidget *parent)
    : QWidget(parent)
{
    this->mainWnd = cmainWnd;
    this->item = citem;

    ui.setupUi(this);

    // links must be of the form <a href=\"http://triangulatorgui.com/123.html\">123</a>

    ui.textBrowser->setHtml(displayStr);
    ui.textBrowser->setOpenLinks(false);    
    ui.textBrowser->setTextInteractionFlags(ui.textBrowser->textInteractionFlags() | Qt::LinksAccessibleByMouse);    

    connect(ui.textBrowser, &QTextBrowser::anchorClicked, this, &PrintComplex::anchorClicked);
}


void PrintComplex::refreshCatalog()
{
    QString displayStr = QString::fromStdString(item->simpComp->print_html());
    ui.textBrowser->setHtml(displayStr);
    ui.textBrowser->setOpenLinks(false);
}

void PrintComplex::closeEvent (QCloseEvent* event)
{
    item->printComplexXcoordinate = this->x();
    item->printComplexYcoordinate = this->y();

    event->accept();

    if (item->printComplex != nullptr) item->printComplex = nullptr;
}

PrintComplex::~PrintComplex()
{
}

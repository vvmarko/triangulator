
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
    ui.setupUi(this);

    // links must be like <a href=\"http://triangulatorgui.com/123.html\">123</a>

    ui.textBrowser->setHtml(displayStr);
    ui.textBrowser->setOpenLinks(false);    
    ui.textBrowser->setTextInteractionFlags(ui.textBrowser->textInteractionFlags() | Qt::LinksAccessibleByMouse);    

    connect(ui.textBrowser, &QTextBrowser::anchorClicked, this, &PrintComplex::anchorClicked);

    this->mainWnd = cmainWnd;
    this->item = citem;

    item->childWindows.push_back(this);
}


void PrintComplex::refreshCatalog(SimpCompItem *citem) {

    if (citem->printComplex != NULL) {
        QString displayStr = QString::fromStdString(citem->simpComp->print_html());

        citem->printComplex->ui.textBrowser->setHtml(displayStr);
        citem->printComplex->ui.textBrowser->setOpenLinks(false);

//        citem->.printComplex->show();
    }
}



void PrintComplex::closeEvent (QCloseEvent* event)
{
    item->printComplexXcoordinate = this->x();
    item->printComplexYcoordinate = this->y();

    bool erased = false;

    mainWnd->printComplexWndClosed(this);

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

PrintComplex::~PrintComplex()
{
}

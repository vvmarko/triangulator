#include "PrintComplex.h"
#include "MainWindow.h"
#include "Inspector.h"
#include "Utils.h"

#include "triangulator.hpp"

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().right(url.fileName().indexOf("http://triangulatorgui.com/"));
    QString str2 = str1.left(str1.indexOf(".html"));

    KSimplex *simplex = item->simpComp->find_KSimplex(str2.toInt());

    Inspector *inspector = new Inspector(simplex, item);

    Utils::openWindowOnRandomPos(inspector);

    inspector->show();
}

PrintComplex::PrintComplex(MainWindow *mainWnd, QString displayStr, SimpCompItem *item, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    // links must be like <a href=\"http://triangulatorgui.com/123.html\">123</a>

    ui.textBrowser->setHtml(displayStr);

    ui.textBrowser->setOpenLinks(false);    
    ui.textBrowser->setTextInteractionFlags(ui.textBrowser->textInteractionFlags() | Qt::LinksAccessibleByMouse);    
    connect(ui.textBrowser, &QTextBrowser::anchorClicked, this, &PrintComplex::anchorClicked);   

    this->mainWnd = mainWnd;
    this->item = item;

    item->childWindows.push_back(this);

}

void PrintComplex::closeEvent (QCloseEvent* event)
{
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

}

PrintComplex::~PrintComplex()
{
}

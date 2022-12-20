#include "PrintComplex.h"
#include "MainWindow.h"
#include "Inspector.h"
#include "Utils.h"

using namespace std;

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().left(url.fileName().indexOf(".html"));

    Inspector *inspector = new Inspector("123", "123", item);

    Utils::openWindowOnRandomPos(inspector);

    inspector->show();
}

PrintComplex::PrintComplex(MainWindow *mainWnd, QString displayStr, SimpCompItem *item, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    // links must be like <a href=\"http://abc.com/123.html\">123</a>
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
      for (int i = 0; i < item->childWindows.size() && !erased; i++)
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

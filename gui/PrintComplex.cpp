#include "PrintComplex.h"
#include "MainWindow.h"
#include "Inspector.h"

using namespace std;

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().left(url.fileName().indexOf(".html"));

    //PrintComplex *printComplex = new PrintComplex(NULL, str1, true, item);
    //printComplex->setGeometry(this->pos().x() + 20, this->pos().y() + 50,
    //                          printComplex->width(), printComplex->height());
    //printComplex->show();
    Inspector *inspector = new Inspector("123", "123", item);
    //printComplex->parent = this;
    //childWindows.push_back(printComplex);        
    inspector->show();
    //ui.textBrowser->setHtml(str1);    
}

PrintComplex::PrintComplex(MainWindow *mainWnd, QString displayStr, bool displayLink, SimpCompItem *item, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    if (!displayLink)
    {
        ui.textBrowser->setHtml(displayStr + "\n[<a href=\"http://abc.com/123.html\">123</a>]\n123\n12345678901234567890 12345678901234567890 1234567890 123456789012345 678901234567890");
    } else
    {
        ui.textBrowser->setHtml(displayStr);
    }

    ui.textBrowser->setOpenLinks(false);    
    ui.textBrowser->setTextInteractionFlags(ui.textBrowser->textInteractionFlags() | Qt::LinksAccessibleByMouse);    
    connect(ui.textBrowser, &QTextBrowser::anchorClicked, this, &PrintComplex::anchorClicked);
    //this->parent = NULL;

    this->mainWnd = mainWnd;
    this->item = item;

    item->childWindows.push_back(this);

}

/*void PrintComplex::closeWindowAndChildWindows(PrintComplex *window)
{    
    if (window->childWindows.size() == 0)
    {
        window->close();
    }
    else
    {
        for (int i = 0; i < window->childWindows.size(); i++)
        {
            closeWindowAndChildWindows(childWindows[i]);
        }
    }
}*/

void PrintComplex::closeEvent (QCloseEvent* event)
{
    bool erased = false;

    /*for(PrintComplex *p:childWindows)
    {
        closeWindowAndChildWindows(p);
    }

    // remove widget from parent's child widgets
    if (parent) {
        for (int i = 0; i < parent->childWindows.size() && !erased; i++)
            if (parent->childWindows[i] == this)
            {
                parent->childWindows.erase(parent->childWindows.begin() + i);
                erased = true;
            }
     } else {
        mainWnd->printComplexWndClosed(this);
    }*/

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

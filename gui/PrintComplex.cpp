//#include "PrintComplex.h"
#include "MainWindow.h"

using namespace std;

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().left(url.fileName().indexOf(".html"));

    PrintComplex *printComplex = new PrintComplex(NULL, str1, true);
    printComplex->setGeometry(this->pos().x() + 20, this->pos().y() + 50,
                              printComplex->width(), printComplex->height());
    printComplex->show();
    printComplex->parent = this;
    childWindows.push_back(printComplex);
    //ui.textBrowser->setHtml(str1);
}

PrintComplex::PrintComplex(MainWindow *mainWnd, QString displayStr, bool displayLink, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    if (!displayLink) {
        ui.textBrowser->setHtml(displayStr + "\n[<a href=\"http://abc.com/123.html\">123</a>]\n123\n12345678901234567890 12345678901234567890 1234567890 123456789012345 678901234567890");
    } else
    {
        ui.textBrowser->setHtml(displayStr);
    }

    ui.textBrowser->setOpenLinks(false);    
    ui.textBrowser->setTextInteractionFlags(ui.textBrowser->textInteractionFlags() | Qt::LinksAccessibleByMouse);    
    connect(ui.textBrowser, &QTextBrowser::anchorClicked, this, &PrintComplex::anchorClicked);
    this->parent = NULL;

    this->mainWnd = mainWnd;
}

void PrintComplex::closeWindowAndChildWindows(PrintComplex *window)
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
}

void PrintComplex::closeEvent (QCloseEvent* event)
{
    bool erased = false;

    for(PrintComplex *p:childWindows)
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
    }
}

PrintComplex::~PrintComplex()
{
}

#include "PrintComplex.h"

using namespace std;

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().left(url.fileName().indexOf(".html"));

    PrintComplex *printComplex = new PrintComplex(str1, true);
    printComplex->setGeometry(this->pos().x() + 20, this->pos().y() + 50,
                              printComplex->width(), printComplex->height());
    printComplex->show();
    //ui.textBrowser->setHtml(str1);
}

PrintComplex::PrintComplex(QString displayStr, bool displayLink, QWidget *parent)
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

}

PrintComplex::~PrintComplex()
{
}

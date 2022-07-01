#include "PrintComplex.h"

using namespace std;

void PrintComplex::anchorClicked(const QUrl &url) {
    QString str1 = url.fileName().left(url.fileName().indexOf(".html"));

    ui.textBrowser->setHtml(str1);
}

PrintComplex::PrintComplex(QString complexType, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

    ui.textBrowser->setHtml(complexType + "\n[<a href=\"http://abc.com/123.html\">123</a>]\n123\n12345678901234567890 12345678901234567890 1234567890 123456789012345 678901234567890");

    ui.textBrowser->setOpenLinks(false);    
    ui.textBrowser->setTextInteractionFlags(ui.textBrowser->textInteractionFlags() | Qt::LinksAccessibleByMouse);    
    connect(ui.textBrowser, &QTextBrowser::anchorClicked, this, &PrintComplex::anchorClicked);

}

PrintComplex::~PrintComplex()
{
}

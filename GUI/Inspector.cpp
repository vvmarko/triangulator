#include "Inspector.h"
#include "ui_Inspector.h"
#include "SimpCompTableModel.h"

void Inspector::anchorClicked(const QUrl &url) {
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

Inspector::Inspector(QString text, QString coords, SimpCompItem *item, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Inspector)
{
    ui->setupUi(this);
    ui->textBrowser->setText(text);
    ui->lCoords->setText(coords);

    this->item = item;

    connect(ui->textBrowser, &QTextBrowser::anchorClicked, this, &Inspector::anchorClicked);

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
      for (int i = 0; i < item->childWindows.size() && !erased; i++)
        if (item->childWindows[i] == this)
        {
            item->childWindows.erase(item->childWindows.begin() + i);
            erased = true;
        }
    }

}

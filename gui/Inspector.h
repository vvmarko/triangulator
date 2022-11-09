#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QWidget>

class SimpCompItem;

namespace Ui {
class Inspector;
}

class Inspector : public QWidget
{
    Q_OBJECT

public:
    explicit Inspector(QString text, QString coords, SimpCompItem *item, QWidget *parent = nullptr);
    ~Inspector();

    SimpCompItem *item;

private:
    Ui::Inspector *ui;

    void anchorClicked(const QUrl &url);

    void closeEvent(QCloseEvent* event) override;

};

#endif // INSPECTOR_H

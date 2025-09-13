#ifndef INSPECTOR_H
#define INSPECTOR_H

#include <QWidget>

class SimpCompItem;
class SimpComp;
class KSimplex;

namespace Ui {
class Inspector;
}

class Inspector : public QWidget
{
    Q_OBJECT

public:
    explicit Inspector(KSimplex *simplex, SimpCompItem *item, QWidget *parent = nullptr);
    ~Inspector();

    SimpCompItem *item;
    SimpComp *simpComp;
    KSimplex *simplex;

private:
    Ui::Inspector *ui;

    void anchorClicked(const QUrl &url);
    void activateBtnAttachNewSimplex();
    void activateBtnApplyPachnerMove();

    void notImplementedYetMessage();

    void closeEvent(QCloseEvent* event) override;

};

#endif // INSPECTOR_H

#pragma once

#include "TriangulatorGUIsystemHeaders.h"
#include "ui_Inspector.h"

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

    void refreshAllInspectors();
    SimpCompItem *item;
    SimpComp *simpComp;
    KSimplex *simplex;
    Ui::Inspector *ui;

private:

    void anchorClicked(const QUrl &url);
    void activateBtnAttachNewSimplex();
    void activateBtnApplyPachnerMove();

    void notImplementedYetMessage();

    void closeEvent(QCloseEvent* event) override;

};


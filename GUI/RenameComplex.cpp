
#include "triangulator.hpp"

#include "RenameComplex.h"
#include "PrintComplex.h"
#include "SimpCompTableModel.h"
#include "MainWindow.h"

void RenameComplex::handleAccepted() {

    std::string newName = ui.newName->text().toStdString();
    item.simpComp->name = newName;

    mainWnd->updateSimpCompTableModel();

    this->close();   
}

void RenameComplex::handleRejected() {
    this->close();
}

RenameComplex::RenameComplex(MainWindow *cmainWnd, SimpCompItem citem, QTableView *ctable, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    this->mainWnd = cmainWnd;
    this->item = citem;
    this->table = ctable;

    ui.oldName->setText (QString::fromStdString(item.simpComp->name));

    connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &RenameComplex::handleAccepted);
    connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &RenameComplex::handleRejected);
}

RenameComplex::~RenameComplex()
{
}

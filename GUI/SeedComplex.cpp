
#include "triangulator.hpp"

#include "SeedComplex.h"
#include "PrintComplex.h"
#include "SimpCompTableModel.h"
#include "MainWindow.h"

void SeedComplex::handleAccepted() {
    SimpCompItem i;

    std::string name = ui.leName->text().toStdString();
    int D = atoi(ui.leDimension->text().toStdString().c_str());
    if (ui.cbType->itemText(ui.cbType->currentIndex()) == "Line")
    {
        i.simpComp = seed_single_simplex(1, name);
    }
    else if (ui.cbType->itemText(ui.cbType->currentIndex()) == "Triangle")
    {
        i.simpComp = seed_single_simplex(2, name);
    }
    else if (ui.cbType->itemText(ui.cbType->currentIndex()) == "Tetrahedron")
    {
        i.simpComp = seed_single_simplex(3, name);
    }
    else if (ui.cbType->itemText(ui.cbType->currentIndex()) == "D-Simplex")
    {
        i.simpComp = seed_single_simplex(D, name);
    }
    else if (ui.cbType->itemText(ui.cbType->currentIndex()) == "D-Sphere")
    {
        i.simpComp = seed_sphere(D, name);
    }


    if (ui.cbType->itemText(ui.cbType->currentIndex()) != "Please select...")
    {
        UniqueIDColor::colorize_entire_complex(i.simpComp);
        evaluate_potential_minimum(i.simpComp);
        evaluate_embedding_coordinates(i.simpComp);
        items->push_back(i);
        mainWnd->updateSimpCompTableModel();
        table->show();
    }
    this->close();   
}

void SeedComplex::handleRejected() {
    this->close();
}

SeedComplex::SeedComplex(MainWindow *cmainWnd, std::vector<SimpCompItem> *citems, QTableView *ctable, QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.cbType->addItem("Please select...");
    ui.cbType->insertSeparator(1);
    ui.cbType->addItem("Line");
    ui.cbType->addItem("Triangle");
    ui.cbType->addItem("Tetrahedron");
    ui.cbType->addItem("D-Simplex");
    ui.cbType->addItem("D-Sphere");

    connect(ui.btnOK, &QPushButton::clicked, this, &SeedComplex::handleAccepted);
    connect(ui.btnCancel, &QPushButton::clicked, this, &SeedComplex::handleRejected);

    this->mainWnd = cmainWnd;
    this->items = citems;
    this->table = ctable;
}

SeedComplex::~SeedComplex()
{
}

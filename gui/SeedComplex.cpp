
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
    UniqueIDColor::colorize_entire_complex(i.simpComp);
    items->push_back(i);

    mainWnd->updateSimpCompTableModel();

	table->show();
	this->close();   
}

void SeedComplex::handleRejected() {
	this->close();
}

SeedComplex::SeedComplex(MainWindow *mainWnd, std::vector<SimpCompItem> *items, QTableView *table, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.cbType->addItem("");
	ui.cbType->addItem("Line");
	ui.cbType->addItem("Triangle");
	ui.cbType->addItem("Tetrahedron");
	ui.cbType->addItem("D-Simplex");
	ui.cbType->addItem("D-Sphere");
	
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &SeedComplex::handleAccepted);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &SeedComplex::handleRejected);

    this->mainWnd = mainWnd;
	this->items = items;
	this->table = table;	
}

SeedComplex::~SeedComplex()
{
}

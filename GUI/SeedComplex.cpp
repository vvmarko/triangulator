#include "SeedComplex.h"
#include "PrintComplex.h"
#include "SimpCompTableModel.h"
#include "MainWindow.h"

void SeedComplex::handleAccepted() {			
	SimpCompItem i;

	i.name = ui.leName->text().toStdString();
	i.d = atoi(ui.leDimension->text().toStdString().c_str());    
    items->push_back(i);

    PrintComplex* print = new PrintComplex(mainWnd, ui.cbType->currentText(), false, (&items->back()), NULL);

    items->back().printComplex = print;

    print->show();

    mainWnd->updateSimpCompTableModel();

	table->show();
	this->close();   
}

void SeedComplex::handleRejected() {
	this->close();
}

SeedComplex::SeedComplex(MainWindow *mainWnd, vector<SimpCompItem> *items, QTableView *table, QWidget *parent)
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

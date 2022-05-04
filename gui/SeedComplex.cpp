#include "SeedComplex.h"
#include "PrintComplex.h"

void SeedComplex::handleAccepted() {		
	PrintComplex* print = new PrintComplex(ui.cbType->currentText());

	print->show();
	this->close();
}

void SeedComplex::handleRejected() {
	this->close();
}

SeedComplex::SeedComplex(QWidget *parent)
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
}

SeedComplex::~SeedComplex()
{
}

#include "SeedComplex.h"
#include "PrintComplex.h"

void SeedComplex::handleNewLine() {
	ui.leType->setText("Line");
}

void SeedComplex::handleNewTriangle() {
	ui.leType->setText("Triangle");
}

void SeedComplex::handleNewTetrahedron() {
	ui.leType->setText("Tetrahedron");
}

void SeedComplex::handleNewDSimplex() {
	ui.leType->setText("D-Simplex");
}

void SeedComplex::handleNewDSphere() {
	ui.leType->setText("D-Sphere");
}

void SeedComplex::handleAccepted() {
	PrintComplex* print = new PrintComplex();

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
	connect(ui.btnTetrahedron, &QPushButton::released, this, &SeedComplex::handleNewTetrahedron);
	connect(ui.btnLine, &QPushButton::released, this, &SeedComplex::handleNewLine);
	connect(ui.btnTriangle, &QPushButton::released, this, &SeedComplex::handleNewTriangle);
	connect(ui.btnDSimplex, &QPushButton::released, this, &SeedComplex::handleNewDSimplex);
	connect(ui.btnDSphere, &QPushButton::released, this, &SeedComplex::handleNewDSphere);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &SeedComplex::handleAccepted);
	connect(ui.buttonBox, &QDialogButtonBox::rejected, this, &SeedComplex::handleRejected);
}

SeedComplex::~SeedComplex()
{
}

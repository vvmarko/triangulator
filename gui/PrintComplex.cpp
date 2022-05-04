#include "PrintComplex.h"

PrintComplex::PrintComplex(QString complexType, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.textEdit->setText(complexType + "\n123\n123\n12345678901234567890 12345678901234567890 1234567890 123456789012345 678901234567890");
}

PrintComplex::~PrintComplex()
{
}

#include "PrintComplex.h"

PrintComplex::PrintComplex(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.textEdit->setText("123\n123\n12345678901234567890123456789012345678901234567890123456789012345678901234567890");	
}

PrintComplex::~PrintComplex()
{
}

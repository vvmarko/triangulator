#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;    
    w.statusBar()->showMessage("Status");
    /*QLabel* status = new QLabel();
    status->setText("Status");
    status->setLineWidth(0);
    w.statusBar()->setStyleSheet("QStatusBar::item{ border: 0px solid black };");
    w.statusBar()->addWidget(status);*/
    
    w.show();
    return a.exec();
}

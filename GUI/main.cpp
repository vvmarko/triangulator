#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QFile>
#include <QDateTime>
#include <QString>

#include "triangulator.hpp"

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

    triangulator_global::logLevel=LOG_INFO;

    QFile logf(QString::fromStdString(triangulator_global::logFilename));
    logf.remove();

    QString qstartMessage = "<b>Triangulator GUI session, started on: ";

    QDateTime dt = QDateTime::currentDateTime();
    QString formattedTime = dt.toString("dd.MM.yyyy hh:mm:ss");
    qstartMessage = qstartMessage + formattedTime + "</b><br>";
    std::string startMessage = qstartMessage.toStdString();

    log_report(LOG_INFO,startMessage);

    return a.exec();
}

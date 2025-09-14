
#include "triangulator.hpp"
#include "TriangulatorGUI.h"

void Utils::openWindowOnRandomPos(QWidget *w)
{
    //QSize size = qApp->screens()[0]->size();
    QRect rect = QApplication::primaryScreen()->availableGeometry();
    int height = rect.height();
    int width = rect.width();

    quint32 x = QRandomGenerator::global()->generateDouble() * (width - w->frameGeometry().width());
    quint32 y = QRandomGenerator::global()->generateDouble() * (height - w->frameGeometry().height());
    w->move(x, y);
}



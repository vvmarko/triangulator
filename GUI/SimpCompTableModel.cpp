
#include "triangulator.hpp"

#include "SimpCompTableModel.h"

using namespace std;

SimpCompItem::SimpCompItem()
{
    simpComp = NULL;
    drawComplex = NULL;
    printComplex = NULL;
    removeWindowFromChildWindowsOnClose = true;
}

SimpCompTableModel::SimpCompTableModel(vector<SimpCompItem> *items, QObject* parent)
    : QAbstractTableModel(parent)
{
    this->items = items;
}

int SimpCompTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return items->size();
}

int SimpCompTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 7; // name, dimension, topology, print complex, draw complex, save as, delete row
}

QVariant SimpCompTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)        
        switch (index.column())
        {
            case 0 :
              return QString::fromStdString(items->at(index.row()).simpComp->name);
            case 1 :
              return QString::number(items->at(index.row()).simpComp->D);
            case 2 :
              return QString::fromStdString(items->at(index.row()).simpComp->topology);
            case 3 :
              return QString("");
            case 4 :
              return QString("");
            case 5 :
              return QString("");
            case 6 :
              return QString("");
        }

    return QVariant();
}

QVariant SimpCompTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
          case 0:
              return QString("Name");
          case 1:
              return QString("Dimension");
          case 2:
              return QString("Topology");
        }
    }
    return QVariant();
}

void SimpCompTableModel::addItem(SimpCompItem i)
{
    items->push_back(i);
}

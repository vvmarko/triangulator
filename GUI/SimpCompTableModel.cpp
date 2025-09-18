
#include "triangulator.hpp"
#include "TriangulatorGUI.h"

using namespace std;

SimpCompItem::SimpCompItem()
{
    simpComp = nullptr;
    drawComplex = nullptr;
    printComplex = nullptr;
}

SimpCompTableModel::SimpCompTableModel(vector<SimpCompItem> *citems, QObject* parent)
    : QAbstractTableModel(parent)
{
    this->items = citems;
}

int SimpCompTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return items->size();
}

int SimpCompTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return 7; // name, dimension, topology, catalogue, visualize, tools, actions
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
              return QString("Catalogue");
            case 4 :
              return QString("Visualize");
            case 5 :
              return QString("Tools");
            case 6 :
              return QString("Actions");
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
          case 3:
              return QString("Catalogue");
          case 4:
              return QString("Visualize");
          case 5:
              return QString("Tools");
          case 6:
              return QString("Actions");
        }
    }
    return QVariant();
}

void SimpCompTableModel::addItem(SimpCompItem i)
{
    items->push_back(i);
}

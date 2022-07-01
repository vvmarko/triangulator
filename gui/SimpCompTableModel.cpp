
#include "SimpCompTableModel.h"

using namespace std;

SimpCompItem::SimpCompItem()
{    
    d = 0;
}

SimpCompItem::SimpCompItem(string name, int d)
{
    this->name = name;
    this->d = d;
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
    return 3;
}

QVariant SimpCompTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)        
        switch (index.column())
        {
            case 0 :
              return QString::fromStdString(items->at(index.row()).name);
            case 1 :
              return QString::number(items->at(index.row()).d);
            case 2 :
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
        }
    }
    return QVariant();
}

void SimpCompTableModel::addItem(SimpCompItem i)
{
    items->push_back(i);
}

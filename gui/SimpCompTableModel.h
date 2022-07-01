#pragma once

// mymodel.h
#include <QAbstractTableModel>

//using namespace std;

class SimpCompItem
{
public:
    SimpCompItem();
    SimpCompItem(std::string name, int d);

    std::string name;
    int d;
};

class SimpCompTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    SimpCompTableModel(std::vector<SimpCompItem>* items, QObject* parent = nullptr);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addItem(SimpCompItem i);    
private:
    std::vector<SimpCompItem> *items;
};

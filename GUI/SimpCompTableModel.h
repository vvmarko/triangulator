#pragma once

#include "TriangulatorGUIsystemHeaders.h"

class SimpComp;
class DrawComplex;
class PrintComplex;
class Inspector;

class SimpCompItem
{
public:
    SimpCompItem();    

    DrawComplex *drawComplex; // each simplicial complex has its own Visualizer window
    int drawComplexXcoordinate = -1;
    int drawComplexYcoordinate = -1;

    PrintComplex *printComplex; // each simplicial complex has its own Catalogue window
    int printComplexXcoordinate = -1;
    int printComplexYcoordinate = -1;

    SimpComp *simpComp;

    std::vector<Inspector*> childWindows; // each simplicial complex has its own set of Inspector windows
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

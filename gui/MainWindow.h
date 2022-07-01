#pragma once

#include <QMainWindow>
#include "ui_MainWindow.h"
#include "SimpCompTableModel.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

    void createItemWidget(int row);

private:
	Ui::MainWindow ui;

	void newFile();

    void tblItemClick();

    std::vector<SimpCompItem> items;

};

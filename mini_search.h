#pragma once
#pragma execution_character_set("UTF-8")

#include <QtWidgets/QMainWindow>
#include <ui_mini_search.h>
#include "dialog.h"
#include <QTextStream>
#include "test.h"
#include <QAbstractButton>
#include <QPushButton>

namespace Ui {
	class mini_search;
}

class mini_search : public QMainWindow
{
	Q_OBJECT

public:
	mini_search(QWidget *parent = 0);
	~mini_search();

private slots:
	void on_pushButton_clicked();

private:
	Ui::mini_searchClass ui;
	Dialog* dialog;
};


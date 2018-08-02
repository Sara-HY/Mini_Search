#pragma once
#pragma execution_character_set("UTF-8")

#include <QDialog>
#include "test.h"
#include <QModelIndex>

namespace Ui {
	class Dialog;
}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	void show_word(COREDATA* pcoredata, DOCFEEDBACK* pdocfeedback);
	Dialog(QWidget *parent = 0);
	~Dialog();

public slots:
	void push_clicked(QModelIndex index);

private:
	Ui::Dialog *ui;
};

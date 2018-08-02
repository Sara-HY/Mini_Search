#pragma once
#pragma execution_character_set("UTF-8")

#include "mini_search.h"
#include "ui_mini_search.h"
#include "dialog.h"
#include <QMessageBox>
#include <QAbstractButton>
#include <QDebug>

extern "C"
{
#include "test.h"
};

#ifdef __cplusplus
extern "C"
{
#endif // _cplusplus
	DOCFEEDBACK* query_and(COREDATA* pcoredata, char* src);
	DOCFEEDBACK* query_or(COREDATA* pcoredata, char* src);
	DOCFEEDBACK* query(COREDATA* pcoredata, char* src);
	COREDATA* pcoredata = build();
}

mini_search::mini_search(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	COREDATA* pcoredata = build(); 
}

mini_search::~mini_search()
{

}

void mini_search::on_pushButton_clicked()
{
	QString str = NULL;
	QByteArray ba = NULL;
	char* ch = NULL;
	DOCFEEDBACK *pdocfeedback = NULL;

    str = ui.lineEdit->text();
	ba = str.toLocal8Bit();
	ch = ba.data();

	if (strcmp(ch, "") != 0)
	{
		if (ui.radioButton->isChecked() == true)
		{
			ui.radioButton_2->released();
			pdocfeedback = query_and(pcoredata, ch);
		}

		if (ui.radioButton_2->isChecked() == true)
		{
			pdocfeedback = query_or(pcoredata, ch);
		}

		if (ui.radioButton->isChecked() == false && ui.radioButton_2->isChecked() == false)
		{
			pdocfeedback = query(pcoredata, ch);
		}

		if (pdocfeedback == NULL)
		{
			QMessageBox::information(this, "Results", "Sorry,Not Found these words!");
		}
		else
		{
			dialog = new Dialog;
			dialog->show_word(pcoredata, pdocfeedback);
			dialog->show();
		}
	}
	return;
}





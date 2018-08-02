#pragma once
#pragma execution_character_set("UTF-8")

#include "dialog.h"
#include "ui_dialog.h"
#include <QStandardItemModel>
#include <QModelIndex>
#include <QMessageBox>
#include <QtWidgets>

Dialog::Dialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::Dialog)
{
	ui->setupUi(this);
	connect(ui->tableView, SIGNAL(clicked(QModelIndex)), this, SLOT(push_clicked(QModelIndex)));
}

void Dialog::show_word(COREDATA* pcoredata, DOCFEEDBACK* pdocfeedback){

	QString tmp;
	int doc_id, i;
	DOCFEEDBACK* ptr;

	ui->tableView->setMouseTracking(true);

	QStandardItemModel  *model = new QStandardItemModel();
	//准备数据模型
	model->setColumnCount(4);
	model->setHeaderData(0, Qt::Horizontal, QString("doc_id"));
	model->setHeaderData(1, Qt::Horizontal, QString("score"));
	model->setHeaderData(2, Qt::Horizontal, QString("html_url"));
	model->setHeaderData(3, Qt::Horizontal, QString("html_title"));
	ui->tableView->setModel(model);

	//表头信息显示居中
	ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

	//设置表格的各列的宽度值
	ui->tableView->setColumnWidth(0, 10);
	ui->tableView->setColumnWidth(1, 100);
	ui->tableView->setColumnWidth(2, 400);
	ui->tableView->setColumnWidth(3, 400);

	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

	ui->tableView->resizeColumnToContents(0);
	ui->tableView->resizeColumnToContents(1);
	ui->tableView->resizeColumnToContents(2);
	ui->tableView->resizeColumnToContents(3);


	//设置表格的单元为只读属性，即不能编辑
	ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

	if (pdocfeedback)
	{
		for (ptr = pdocfeedback, i = 0; ptr != NULL; ptr = ptr->pnext, i++)
		{
			doc_id = ptr->docid;
			model->setItem(i, 0, new QStandardItem(QString::number(doc_id)));
			model->setItem(i, 1, new QStandardItem(QString::number(ptr->score)));
			model->setItem(i, 2, new QStandardItem(QObject::tr(pcoredata->pdocset->pdocinfo[doc_id].url)));
			model->setItem(i, 3, new QStandardItem(QObject::tr(pcoredata->pdocset->pdocinfo[doc_id].title)));
			model->item(i, 0)->setTextAlignment(Qt::AlignCenter);
			model->item(i, 1)->setTextAlignment(Qt::AlignCenter);
			model->item(i, 2)->setTextAlignment(Qt::AlignCenter);
			model->item(i, 3)->setTextAlignment(Qt::AlignCenter);
		}
	}

}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::push_clicked(QModelIndex index)
{
	QString str = index.data().toString();
	//QMessageBox::information(NULL, "", str);
	ShellExecuteA(NULL, "open", str.toStdString().c_str(), "", "", SW_SHOW);
}
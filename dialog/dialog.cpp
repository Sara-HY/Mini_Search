#include "dialog.h"
#include "ui_dialog.h"
#include <QDialog>
#include <QStandardItemModel>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    QStandardItemModel  *model = new QStandardItemModel();

    //准备数据模型
    model->setColumnCount(4);
    model->setHeaderData(0,Qt::Horizontal,QString("doc_id"));
    model->setHeaderData(1,Qt::Horizontal,QString("score"));
    model->setHeaderData(2,Qt::Horizontal,QString("html_url"));
    model->setHeaderData(3,Qt::Horizontal,QString("html_title"));
    ui->tableView->setModel(model);

    //表头信息显示居中
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //设置表格的各列的宽度值
    ui->tableView->setColumnWidth(0,10);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,400);
    ui->tableView->setColumnWidth(3,400);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->tableView->resizeColumnToContents(0);
    ui->tableView->resizeColumnToContents(1);
    ui->tableView->resizeColumnToContents(2);
    ui->tableView->resizeColumnToContents(3);

    //设置选中时为整行选中
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表格的单元为只读属性，即不能编辑
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);


      model->setItem(0, 0, new QStandardItem("      "));
      model->setItem(0, 1, new QStandardItem("   "));
      model->setItem(0, 2, new QStandardItem("                                0"));
      model->setItem(0, 3, new QStandardItem("                                         0"));
      model->item(0, 0)->setTextAlignment(Qt::AlignCenter);
      model->item(0, 1)->setTextAlignment(Qt::AlignCenter);
      model->item(0, 2)->setTextAlignment(Qt::AlignCenter);
      model->item(0, 3)->setTextAlignment(Qt::AlignCenter);


}

Dialog::~Dialog()
{
    delete ui;
}


#include "historyrecorditem.h"
#include "ui_historyrecorditem.h"

HistoryRecordItem::HistoryRecordItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryRecordItem)
{
    ui->setupUi(this);
}

HistoryRecordItem::~HistoryRecordItem()
{
    delete ui;
}

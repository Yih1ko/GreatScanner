#include "historypage.h"
#include "ui_historypage.h"
#include <qpainter.h>

HistoryPage::HistoryPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryPage)
{
    ui->setupUi(this);

    // 批量构造25条固定格式的记录
    for (int i = 0; i < 25; ++i) {
        SearchMetaData<Page::PAGE_HISTORY> newdata =
            { "upload"
            , "file_" + std::to_string(i) + ".txt"
            , static_cast<uint64_t>(100 * i)
            , QDateTime(QDate(2025, 7, 30), QTime(21, 22))};
        ui->historyTable->appendRecord(newdata);
    }
}

HistoryPage::~HistoryPage()
{
    delete ui;
}

void HistoryPage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(),  QColor(248, 250, 255)); // RGB颜色
    // 可选：调用基类绘制其他内容（如边框）
    QWidget::paintEvent(event);
}

// void HistoryPage::on_DeleteAllBtn_clicked()
// {
//     ui->historyTable->selectAll();
//     ui->historyTable->deleteAllSelected();
// }

void HistoryPage::on_BackUpBtn_clicked()
{
    ui->historyTable->uploadAllSelected();
}

void HistoryPage::on_SelectAll_clicked()
{
    bool hasSelectedItems = !ui->historyTable->getSelectedRecords().isEmpty();

    if(hasSelectedItems) {
        // 如果已有选中项，则取消全选
        ui->SelectAll->setChecked(false);
        ui->historyTable->deselectAll();
    } else {
        // 如果没有选中项，则全选
        ui->historyTable->selectAll();
    }
}

void HistoryPage::on_DeleteAllSelectedBtn_clicked()
{
    ui->historyTable->deleteAllSelected();
}


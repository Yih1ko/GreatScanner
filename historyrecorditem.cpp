#include "historyrecorditem.h"
#include "ui_historyrecorditem.h"
#include <QDateTime>
#include <QPainter>

HistoryRecordItem::HistoryRecordItem(QWidget *parent,
                                     SearchMetaData<Page::PAGE_HISTORY> recordMetaData)
    : QWidget(parent)
    , ui(new Ui::HistoryRecordItem)
    , m_recordMetaData(recordMetaData)
{
    ui->setupUi(this);

    // ui->FileTypePLabel
    ui->FileNameLabel->setText(QString::fromStdString(recordMetaData.fileName));
    ui->SizeLabel->setText(QString::fromStdString(std::to_string(recordMetaData.fileSize/1024)));
    ui->DateLabel->setText(recordMetaData.date.toString("yyyy 年MM月dd日 hh:mm:ss"));

    // 设置初始选中状态
    ui->CheckBox->setChecked(m_isSelected);
}

HistoryRecordItem::~HistoryRecordItem()
{
    delete ui;
}

void HistoryRecordItem::setSelected(bool selected)
{
    if (m_isSelected != selected) {
        m_isSelected = selected;
        ui->CheckBox->setChecked(selected);
        emit selectionChanged(selected);
    }
}

void HistoryRecordItem::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.fillRect(rect(),  QColor(255, 255, 255)); // RGB颜色
    // 可选：调用基类绘制其他内容（如边框）
    QWidget::paintEvent(event);
}

void HistoryRecordItem::on_CheckBox_clicked()
{
    m_isSelected = ui->CheckBox->isChecked();
    emit selectionChanged(m_isSelected);
}

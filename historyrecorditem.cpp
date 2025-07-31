#include "historyrecorditem.h"
#include "ui_historyrecorditem.h"
#include <QDateTime>
#include <QPainter>
#include <QEnterEvent>

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

    // 根据悬浮状态选择颜色
    QColor bgColor = m_isHovered ? QColor(0xf8, 0xf8, 0xf8) : QColor(255, 255, 255);
    painter.fillRect(rect(),  bgColor);

    QWidget::paintEvent(event);
}

void HistoryRecordItem::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    update();  // 触发重绘
    QWidget::enterEvent(event);
}

void HistoryRecordItem::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    update();  // 触发重绘
    QWidget::leaveEvent(event);
}

void HistoryRecordItem::on_CheckBox_clicked()
{
    m_isSelected = ui->CheckBox->isChecked();
    emit selectionChanged(m_isSelected);
}

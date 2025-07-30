#include "historytable.h"
#include "ui_historytable.h"
#include <QScrollBar>
#include <algorithm>

HistoryTable::HistoryTable(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HistoryTable)
{
    ui->setupUi(this);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_layout = new QVBoxLayout(ui->RecordAera);
    m_layout->setAlignment(Qt::AlignTop);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
}

HistoryTable::~HistoryTable()
{
    delete ui;
}

void HistoryTable::appendRecord(SearchMetaData<Page::PAGE_HISTORY> &recordMetaData)
{
    HistoryRecordItem* newItem = new HistoryRecordItem(nullptr, recordMetaData);
    newItem->setFixedHeight(60);
    ui->RecordAera->setMinimumHeight(m_layout->count() * 60);
    m_layout->addWidget(newItem);

    // 连接选中状态变化信号
    QObject::connect(newItem, &HistoryRecordItem::selectionChanged,
                     [this, newItem](bool selected) {
                         if (selected) {
                             if (!m_selectedRecords.contains(newItem))  {
                                 m_selectedRecords.append(newItem);
                             }
                         } else {
                             m_selectedRecords.removeAll(newItem);
                         }
                     });
}

QVector<HistoryRecordItem*> HistoryTable::getSelectedRecords() const
{
    return m_selectedRecords;
}

void HistoryTable::selectAll()
{
    for (int i = 0; i < m_layout->count(); ++i) {
        if (auto item = qobject_cast<HistoryRecordItem*>(m_layout->itemAt(i)->widget())) {
            item->setSelected(true);
        }
    }
}

void HistoryTable::deselectAll()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    for (int i = 0; i < m_layout->count(); ++i) {
        if (auto item = qobject_cast<HistoryRecordItem*>(m_layout->itemAt(i)->widget())) {
            item->setSelected(false);
        }
    }
    QApplication::restoreOverrideCursor();
}

void HistoryTable::deleteAllSelected()
{
    // 使用临时副本防止迭代器失效
    auto selectedCopy = m_selectedRecords;

    for (auto item : selectedCopy) {
        m_layout->removeWidget(item);
        item->deleteLater();
        m_selectedRecords.removeAll(item);
    }

    // 更新容器高度
    ui->RecordAera->setMinimumHeight(m_layout->count() * 60);

    // 如果没有记录项了，隐藏滚动条
    if (m_layout->count() == 0) {
        ui->scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
}

void HistoryTable::uploadAllSelected()
{
    // for (auto item : m_selectedRecords) {
    //     qDebug() << "Uploading record:" << item->getMetadata().title;
    // }
    qDebug() << "Total" << m_selectedRecords.size()  << "records uploaded";
}

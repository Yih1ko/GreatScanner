#ifndef HISTORYTABLE_H
#define HISTORYTABLE_H

#include <QWidget>
#include "historyrecorditem.h"
#include <QVector>
#include <QVBoxLayout>

namespace Ui {
class HistoryTable;
}

class HistoryTable : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryTable(QWidget *parent = nullptr);
    ~HistoryTable();
    void appendRecord(SearchMetaData<Page::PAGE_HISTORY>& recordMetaData);
    QVector<HistoryRecordItem*> getSelectedRecords() const;
    void selectAll();
    void deselectAll();

public slots:
    void deleteAllSelected();
    void uploadAllSelected();

private:
    Ui::HistoryTable *ui;
    QVector<HistoryRecordItem*> m_selectedRecords;
    QVBoxLayout *m_layout = nullptr;
};

#endif // HISTORYTABLE_H

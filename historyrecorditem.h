#ifndef HISTORYRECORDITEM_H
#define HISTORYRECORDITEM_H

#include <QWidget>

namespace Ui {
class HistoryRecordItem;
}

class HistoryRecordItem : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryRecordItem(QWidget *parent = nullptr);
    ~HistoryRecordItem();

private:
    Ui::HistoryRecordItem *ui;
};

#endif // HISTORYRECORDITEM_H

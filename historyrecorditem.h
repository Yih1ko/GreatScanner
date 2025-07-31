#ifndef HISTORYRECORDITEM_H
#define HISTORYRECORDITEM_H

#include <QWidget>
#include "search/SearchDef.h"

namespace Ui {
class HistoryRecordItem;
}

class HistoryRecordItem : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryRecordItem(QWidget *parent = nullptr,
                               SearchMetaData<Page::PAGE_HISTORY> record = {});
    ~HistoryRecordItem();

    bool isSelected() const { return m_isSelected; }
    void setSelected(bool selected);
    void setHovered(bool isHovered){m_isHovered = isHovered;}
    SearchMetaData<Page::PAGE_HISTORY> getMetadata() const { return m_recordMetaData; }
protected:
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
signals:
    void selectionChanged(bool selected);

private slots:
    void on_CheckBox_clicked();

private:
    Ui::HistoryRecordItem *ui;
    SearchMetaData<Page::PAGE_HISTORY> m_recordMetaData;
    bool m_isSelected = false;
    bool m_isHovered = false;
};

#endif // HISTORYRECORDITEM_H

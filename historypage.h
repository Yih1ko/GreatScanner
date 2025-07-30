#ifndef HISTORYPAGE_H
#define HISTORYPAGE_H

#include <QWidget>
#include <QVector>

namespace Ui {
class HistoryPage;
}

class HistoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryPage(QWidget *parent = nullptr);
    ~HistoryPage();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    // void on_DeleteAllBtn_clicked();

    void on_BackUpBtn_clicked();

    void on_SelectAll_clicked();

    void on_DeleteAllSelectedBtn_clicked();

private:
    Ui::HistoryPage *ui;
};

#endif // HISTORYPAGE_H

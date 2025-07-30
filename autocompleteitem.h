#ifndef AUTOCOMPLETEITEM_H
#define AUTOCOMPLETEITEM_H

#include <QWidget>

namespace Ui {
class AutoCompleteItem;
}

class AutoCompleteItem : public QWidget
{
    Q_OBJECT

public:
    explicit AutoCompleteItem(QWidget *parent = nullptr);
    ~AutoCompleteItem();
    void setText(QString text);
    QString getText();

signals:
    void selected(QString);

private:
    Ui::AutoCompleteItem *ui;
};

#endif // AUTOCOMPLETEITEM_H

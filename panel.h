#ifndef PANEL_H
#define PANEL_H

#include "textlabel.h"
#include "cardgroup.h"
#include <QVBoxLayout>


class Panel : public QWidget
{
    Q_OBJECT
public:
    explicit Panel(CardGroup* cardGroup, QString title = "", QWidget *parent = nullptr);
    explicit Panel(int pixelSize, CardGroup* cardGroup, QString title = "", QWidget *parent = nullptr);
    ~Panel();

    void setTitleFont(const QString &family, bool italic = false);
    CardView* getCard(const QString& str);
signals:

private:
    TextLabel* m_title;
    CardGroup* m_cardGroup;

    void doLayout(){
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(m_title);
        layout->addWidget(m_cardGroup);
        layout->setAlignment(Qt::AlignTop);
    }
};

#endif // PANEL_H

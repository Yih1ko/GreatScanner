#include "panel.h"
#include <QFont>

Panel::Panel(CardGroup *cardGroup, QString title, QWidget *parent)
    : QWidget(parent)
{
    if (!cardGroup) {
        qFatal("CardGroup pointer must not be null.");
        return;
    }
    setAttribute(Qt::WA_TranslucentBackground); //默认背景透明

    setObjectName(title);

    m_title = new TextLabel(title, this);
    m_title->setTextStyle(TextStyle::Subtitle);

    m_cardGroup = cardGroup;
    m_cardGroup->setBackgroundColor(Qt::white);

    setTitleFont("黑体");

    doLayout();
}

Panel::Panel(int pixelSize, CardGroup *cardGroup, QString title, QWidget *parent)
    : QWidget(parent)
{
    if (!cardGroup) {
        qFatal("CardGroup pointer must not be null.");
        return;
    }
    setAttribute(Qt::WA_TranslucentBackground); //默认背景透明

    setObjectName(title);

    m_title = new TextLabel(title, pixelSize, this);
    m_title->setTextStyle(TextStyle::Subtitle);

    m_cardGroup = cardGroup;

    doLayout();
}

Panel::~Panel()
{

}

void Panel::setTitleFont(const QString &family, bool italic)
{
    m_title->setTextFont(QFont(family, italic));
}

CardView *Panel::getCard(const QString &str)
{
    return m_cardGroup->getCardView(str);
}

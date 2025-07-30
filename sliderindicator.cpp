#include "sliderindicator.h"
#include "SliderIndicator.h"
#include <qpainter.h>

SliderIndicator::SliderIndicator(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(3);
    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
}

void SliderIndicator::activateAt(QWidget *target)
{
    if (!target) return;

    const int margin = 5; // 留白
    const QPoint pos = target->mapTo(parentWidget(), QPoint(0,0));
    QRect endRect(
        pos.x() ,
        pos.y() + target->height() + margin-2,
        target->width() - margin*2,
        height()
        );

    m_animation->stop();
    m_animation->setStartValue(geometry());
    m_animation->setEndValue(endRect);
    m_animation->start();
}

void SliderIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_color);
    painter.drawRoundedRect(rect(),  m_borderRadius, m_borderRadius);
}

#include "myrect.h"
#include <QPainter>
#include <QDebug>
#include <QPen>

MyRect::MyRect()
{

}

MyRect::MyRect(QRectF rect) : m_rect(rect.x(), rect.y(), rect.width(), rect.height())
{

}

void MyRect::setText(QString str)
{
    this->m_str = str;
}

void MyRect::setLocation(qreal x, qreal y)
{
    //设置左边界的坐标，右边界保持不变
    m_rect.setX(x);
    m_rect.setY(y - 21);
    //设置长度为正,相当于设置右边界
    m_rect.setWidth(71);
    m_rect.setHeight(21);
}

void MyRect::drawMe(QPainter &painter)
{


    painter.save();
    painter.setPen(QPen());
    painter.setBrush(QColor(0, 0, 0, 150));
    painter.drawRect(m_rect);
    //设置白色文字
    painter.setPen(QColor(255, 255, 255));
    //居中画出文字
    painter.drawText(m_rect, Qt::AlignCenter, m_str);
    painter.restore();

}

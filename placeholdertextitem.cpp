#include "PlaceholderTextItem.h"
#include <QPainter>
#include <cmath>

PlaceholderTextItem::PlaceholderTextItem(const QPolygonF& polygon, const QString& text, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_polygon(polygon), m_text(text)
{
    m_boundingRect = m_polygon.boundingRect();
    setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
}

QRectF PlaceholderTextItem::boundingRect() const {
    return m_boundingRect;
}

void PlaceholderTextItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
    // 绘制边框
    painter->setPen(QPen(Qt::green, 1.5, Qt::DashLine));
    painter->drawPolygon(m_polygon);

    const int charCount = m_text.length();
    if (charCount == 0) return;

    const qreal rectWidth = m_boundingRect.width();
    const qreal rectHeight = m_boundingRect.height();
    const qreal charWidth = rectWidth / charCount;

    // 绘制字符占位符
    for (int i = 0; i < charCount; ++i) {
        QRectF charRect(
            m_boundingRect.left()  + i * charWidth,
            m_boundingRect.top(),
            charWidth,
            rectHeight
            );

        // 绘制选择高亮
        if (m_selectionStart >= 0 && i >= m_selectionStart && i < m_selectionEnd) {
            painter->setBrush(QBrush(QColor(100, 150, 255, 150))); // 半透明蓝色
            painter->setPen(Qt::NoPen);
            painter->drawRect(charRect);
        }

        // 绘制占位符边框（仅在未选中时显示）
        if (m_selectionStart < 0 || i < m_selectionStart || i >= m_selectionEnd) {
            painter->setPen(QPen(QColor(200, 200, 200, 50), 0.5));
            painter->setBrush(Qt::NoBrush);
            painter->drawRect(charRect);
        }
    }
}

void PlaceholderTextItem::setSelection(int start, int end) {
    m_selectionStart = qBound(-1, start, m_text.length());
    m_selectionEnd = qBound(-1, end, m_text.length());
    update();
}

QString PlaceholderTextItem::selectedText() const {
    if (m_selectionStart >= 0 && m_selectionEnd > m_selectionStart) {
        return m_text.mid(m_selectionStart,  m_selectionEnd - m_selectionStart);
    }
    return QString();
}

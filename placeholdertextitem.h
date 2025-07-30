#ifndef PLACEHOLDERTEXTITEM_H
#define PLACEHOLDERTEXTITEM_H

#include <QGraphicsObject>
#include <QPolygonF>

class PlaceholderTextItem : public QGraphicsObject {
    Q_OBJECT
public:
    PlaceholderTextItem(const QPolygonF& polygon, const QString& text, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override;

    void setSelection(int start, int end);
    QString selectedText() const;
    QString fullText() const { return m_text; }
    int selectionStart() const { return m_selectionStart; }
    int selectionEnd() const { return m_selectionEnd; }

private:
    QPolygonF m_polygon;
    QString m_text;
    QRectF m_boundingRect;
    int m_selectionStart = -1;
    int m_selectionEnd = -1;
};
#endif // PLACEHOLDERTEXTITEM_H

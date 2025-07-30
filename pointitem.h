// PointItem.h
#ifndef POINTITEM_H
#define POINTITEM_H

#include <QAbstractGraphicsShapeItem>

class PointItem : public QObject, public QAbstractGraphicsShapeItem {
    Q_OBJECT

public:
    explicit PointItem(QGraphicsItem* parent, const QPointF& pos);
    ~PointItem();

    // 获取当前点位置（基于父项坐标系）
    QPointF getPoint() const { return m_point; }
    // 设置点位置
    void setPoint(const QPointF& p);

signals:
    void positionChanged();
    void selected(PointItem* item, bool state);
    void dragEnded();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:
    QPointF m_point;
    QPointF m_oldPos; //记录旧位置
};

#endif // POINTITEM_H

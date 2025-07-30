#ifndef FRAME_H
#define FRAME_H

#include "pointitem.h"
#include <QStack>

class Frame : public QGraphicsObject
{

    Q_OBJECT

public:
    explicit Frame(QGraphicsItem* parent = nullptr);
    ~Frame();

    // 图形项类型标识
    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    // 控制点访问
    QList<PointItem*> controlPoints() const { return m_points; }
    QPolygonF getPoints() const{
        QPolygonF polygon;
        for (PointItem* point : m_points) {
            polygon << point->getPoint();
        }
        return polygon;
    };

public slots:
    void handleDragEnd();
    void updateShape();

signals:
    void pointSelectionChanged(PointItem* point);

private:

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    // 新增坐标变化处理
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    // 新增鼠标释放事件
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void restoreToLastConvex();

    QStack<QPolygonF> m_history;    //记录时刻点的位置(是凸四边形时)
    QList<PointItem*> m_points;  // 四个控制点
    QPolygonF m_polygon;         // 当前形状多边形
    QPointF m_dragStartPos; // 记录拖拽起始位置
    bool m_isDragging = false;  // 正在拖拽
};

#endif // FRAME_H

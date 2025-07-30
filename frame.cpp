#include "frame.h"
#include <qgraphicssceneevent.h>
#include <qpainter.h>
#include <qtimer.h>
/////////////////////////////////////////////////////////////////////////////////////////
/// \brief 辅助函数isConvex: 检测是否为凸四边形
/// \param polygon
/// \return
///
bool isConvex(const QPolygonF& polygon) {
    if (polygon.size()  != 4) return false;

    // 计算每条边的法向量方向
    QVector<QPointF> edges;
    for (int i = 0; i < 4; ++i) {
        int nextIndex = (i + 1) % 4;
        edges << polygon[nextIndex] - polygon[i];
    }

    // 检查所有法向量方向是否一致
    bool clockwise = false;
    for (int i = 0; i < 4; ++i) {
        QPointF edge1 = edges[i];
        QPointF edge2 = edges[(i + 1) % 4];
        double crossProduct = edge1.x() * edge2.y() - edge1.y() * edge2.x();
        if (crossProduct > 0) {
            clockwise = true;
        } else if (crossProduct < 0) {
            clockwise = false;
            break;
        }
    }

    return clockwise;
}
//////////////////////////////////////////////////////////////////////////////////////
Frame::Frame(QGraphicsItem* parent)
    : QGraphicsObject(parent)
{
    //setCacheMode(QGraphicsItem::DeviceCoordinateCache); //启用缓存

    setAcceptHoverEvents(true);

    // 初始化四个控制点
    for (int i = 0; i < 4; ++i) {
        auto point = new PointItem(this, QPointF());
        m_points.append(point);
    }

    // 初始位置（矩形）
    m_points[0]->setPoint(QPointF(-50, -50));
    m_points[1]->setPoint(QPointF(50, -50));
    m_points[2]->setPoint(QPointF(50, 50));
    m_points[3]->setPoint(QPointF(-50, 50));

    for (auto point : m_points) {
        //更新形状
        connect(point, &PointItem::positionChanged,
                this, &Frame::updateShape);
        //单选PointItem
        connect(point, &PointItem::selected,
                [this, point](bool state){
                    if(state) emit pointSelectionChanged(point);
                    // 清除其他点的选中状态(单选)
                    for(auto p : m_points) {
                        if(p != point) p->setSelected(false);
                    }
                });
        //结束拖拽后检查形状
        connect(point, &PointItem::dragEnded,
                this, &Frame::handleDragEnd);
    }

    // 样式设置
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
}

Frame::~Frame()
{
    qDeleteAll(m_points);
}

QRectF Frame::boundingRect() const
{
    return m_polygon.boundingRect().adjusted(-5,  -5, 5, 5);
}

void Frame::paint(QPainter* painter,
                  const QStyleOptionGraphicsItem* option,
                  QWidget* widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->setRenderHint(QPainter::Antialiasing); // 抗锯齿

    static QPen normalPen(Qt::blue, 1.5);
    static QPen selectedPen(Qt::red, 2.0);
    static QBrush fillBrush(QColor(100, 100, 255, 30));

    painter->setPen(isSelected() ? selectedPen : normalPen);
    painter->setBrush(fillBrush);
    painter->drawPolygon(m_polygon);

    if (isSelected()) {
        const QRectF rect = boundingRect();
        const QPointF center = rect.center();

        painter->setPen(QPen(Qt::yellow, 1, Qt::DashLine));
        painter->drawRect(rect);

        painter->setPen(QPen(Qt::yellow, 1));
        painter->drawLine(
            QLineF(center.x() - 5, center.y(),
                   center.x() + 5, center.y())
            );
        painter->drawLine(
            QLineF(center.x(), center.y() - 5,
                   center.x(), center.y() + 5)
            );
    }
}

void Frame::handleDragEnd()
{
    // 只有在拖拽结束后才进行凸四边形检测
    updateShape();
    qDebug()<<"拖拽结束";

    // 检测是否为凸四边形
    if (isConvex(m_polygon)) {
        // 如果是凸四边形，记录当前状态
        m_history.push(m_polygon);
        qDebug()<<"已记录当前凸四边形状态";
    } else {
        // 如果是凹四边形，恢复到最近的凸四边形状态
        qDebug()<<"是凹四边形,尝试恢复凸四边形状态";
        restoreToLastConvex();
    }
}

void Frame::updateShape()
{
    //直接填充 m_polygon
    m_polygon = QPolygonF({
        m_points[0]->getPoint(),
        m_points[1]->getPoint(),
        m_points[2]->getPoint(),
        m_points[3]->getPoint()
    });

    prepareGeometryChange();
    update();
    //emit geometryChanged(m_polygon); //for debugging
}


void Frame::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    setSelected(true);
    QGraphicsObject::mousePressEvent(event);
}

QVariant Frame::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionHasChanged && scene()) {
        static bool updateScheduled = false;
        if (!updateScheduled) {
            updateScheduled = true;
            QTimer::singleShot(0, [this]() {
                updateShape();
                updateScheduled = false;
            });
        }
    }
    return QGraphicsObject::itemChange(change, value);
}

void Frame::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        for (auto point : m_points) {
            point->setPoint(point->getPoint());
        }
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

void Frame::restoreToLastConvex()
{
    if (m_history.isEmpty())  {
        // 如果没有历史记录，则使用默认矩形
        m_polygon.clear();
        m_polygon << QPointF(-50, -50);
        m_polygon << QPointF(50, -50);
        m_polygon << QPointF(50, 50);
        m_polygon << QPointF(-50, 50);
    } else {
        // 恢复到最近的凸四边形状态
        m_polygon = m_history.top();
    }

    // 更新控制点位置
    for (int i = 0; i < 4; ++i) {
        if(m_points[i]->pos()!= m_polygon[i]){
            m_points[i]->setPoint(m_polygon[i]);
        }
    }

    prepareGeometryChange();
    update();
    qDebug()<<"恢复凸四边形状态成功";
}

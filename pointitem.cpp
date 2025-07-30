#include "pointitem.h"
#include "movepointcommand.h"
#include "undo_stack_singleton.h"
#include <qcursor.h>
#include <QStyle>
#include <qgraphicssceneevent.h>
#include <QStyleOptionGraphicsItem>
#include <qpainter.h>
#include <qpen.h>

PointItem::PointItem(QGraphicsItem* parent, const QPointF& pos)
    : QAbstractGraphicsShapeItem(parent)
    , m_point(pos)
{
    setPos(pos);
    setFlags(QGraphicsItem::ItemIsSelectable |
             QGraphicsItem::ItemIsMovable |
             QGraphicsItem::ItemIsFocusable |
             QGraphicsItem::ItemAcceptsInputMethod|
             QGraphicsItem::ItemSendsGeometryChanges);
    setCursor(QCursor(Qt::PointingHandCursor));
}

PointItem::~PointItem()
{
}

void PointItem::setPoint(const QPointF& p)
{
    m_point = p;
    setPos(p);
    emit positionChanged();
}

QRectF PointItem::boundingRect() const
{
    return QRectF(-4, -4, 8, 8);
}

void PointItem::paint(QPainter* painter,
                      const QStyleOptionGraphicsItem* option,
                      QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // ======== 透明配置 ========
    // 基础透明属性 (数值越小越透明)
    constexpr int normalAlpha = 100;  // 未选中透明度 (0~255)
    constexpr int selectedAlpha = 180; // 选中时透明度 (更显著)

    // 将预定义颜色转换为带透明度的RGBA格式
    const QColor normalColor = QColor(Qt::green).darker(200);  // 深绿色
    const QColor selectedColor = QColor(Qt::red).lighter(150); // 亮红色

    // ======== 状态判断 ========
    const bool isSelected = option->state & QStyle::State_Selected;

    // ======== 透明度与颜色设置 ========
    QColor fillColor = isSelected
                           ? selectedColor
                           : normalColor;
    fillColor.setAlpha(isSelected  ? selectedAlpha : normalAlpha);

    QPen pen(fillColor, 1.5);
    pen.setCosmetic(true);   // 保持像素线宽不随视图缩放变化

    // ======== 绘制操作 ========
    const qreal radius = isSelected ? 5 : 3;

    painter->setRenderHint(QPainter::Antialiasing); // 开启抗锯齿
    painter->setPen(pen);
    painter->setBrush(QBrush(fillColor, Qt::SolidPattern));
    painter->drawEllipse(QPointF(0, 0), radius, radius);
}

void PointItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton) {
        // 限制在父项边界内
        QRectF validArea = parentItem()->boundingRect();
        QPointF newPos = parentItem()->mapFromScene(event->scenePos());

        newPos.setX(qBound(validArea.left(),  newPos.x(), validArea.right()));
        newPos.setY(qBound(validArea.top(),  newPos.y(), validArea.bottom()));

        setPoint(newPos);
    }
}

void PointItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 标记事件已处理
        event->accept();
        // 触发选中信号
        emit selected(this, true);
        //记录旧位置
        m_oldPos = m_point;
    } else {
        QAbstractGraphicsShapeItem::mousePressEvent(event);
    }
}

void PointItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        // 拖拽结束时触发信号
        emit dragEnded();

        // 创建并执行命令
        MovePointCommand *command = new MovePointCommand(this, m_oldPos, m_point/* 新位置 */, nullptr);
        UndoStackSingleton::getInstance()->getUndoStack()->push(command);
    }
    QAbstractGraphicsShapeItem::mouseReleaseEvent(event);
}

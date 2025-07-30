#include "cardgroup.h"

CardGroup::CardGroup(QWidget *parent)
    : QWidget(parent)
{
    // 初始化布局
    m_flowLayout = new FlowLayout(this);
    m_flowLayout->setContentsMargins(5, 5, 5, 5); // 内边距
    m_flowLayout->setAnimationEnabled(1); // 启用布局动画

    // 设置样式
    setAttribute(Qt::WA_TranslucentBackground); //默认背景透明
    setStyleSheet("border: none;"); // 去掉默认边框

    // 初始化边框属性
    m_borderRadius = 25;
    m_borderColor = QColor(200, 200, 200);
    m_borderWidth = 1;
    m_backgroundColor = Qt::transparent; // 初始化背景颜色
}

CardGroup::~CardGroup()
{
    delete m_flowLayout;
}

// 添加 CardView 的公共接口
void CardGroup::addCardView(CardView* cardView)
{
    m_flowLayout->addWidget(cardView);
    m_cardMap.insert(cardView->getDesBelow(), cardView);
}

CardView *CardGroup::getCardView(const QString &str)
{
    return m_cardMap.value(str);
}

// 边框属性的Setter实现
void CardGroup::setBorderRadius(int radius)
{
    m_borderRadius = radius;
    update(); // 刷新界面
}

void CardGroup::setBorderColor(const QColor &color)
{
    m_borderColor = color;
    update();
}

void CardGroup::setBorderWidth(int width)
{
    m_borderWidth = width;
    update();
}

// 背景颜色设置接口实现
void CardGroup::setBackgroundColor(const QColor &color)
{
    m_backgroundColor = color;
    update();
}

// 绘制圆角边框
void CardGroup::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 优化后的绘制路径
    QPainterPath path;
    const qreal halfPen = m_borderWidth / 2.0;
    QRectF drawRect = rect().adjusted(halfPen, halfPen, -halfPen, -halfPen);

    path.addRoundedRect(drawRect,  m_borderRadius, m_borderRadius);

    // 一次性绘制背景和边框
    painter.fillPath(path,  m_backgroundColor);
    painter.strokePath(path,  QPen(m_borderColor, m_borderWidth));
}

// 处理窗口大小变化
// void CardGroup::resizeEvent(QResizeEvent *event)
// {
//     // QWidget::resizeEvent(event);
//     // m_flowLayout->setGeometry(rect());
// }

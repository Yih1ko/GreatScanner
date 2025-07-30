#include "flowlayout.h"
#include <QWidget>
#include <QPropertyAnimation>
#include <qtimer.h>

FlowLayout::FlowLayout(QWidget* parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent),
    m_hSpacing(hSpacing),
    m_vSpacing(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpacing(hSpacing),
    m_vSpacing(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    qDeleteAll(m_items);
}

//====== 核心功能实现 ======
void FlowLayout::addItem(QLayoutItem* item)
{
    m_items.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    return (m_hSpacing >= 0) ? m_hSpacing : smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
    return (m_vSpacing >= 0) ? m_vSpacing : smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

void FlowLayout::setAnimationEnabled(bool enabled)
{
    m_isAnimation = enabled;
    if (!enabled && m_animationGroup) {
        m_animationGroup->clear(); // 禁用时清除动画
    }
}

//====== 必须实现的虚函数 ======
Qt::Orientations FlowLayout::expandingDirections() const
{
    return Qt::Horizontal; // 仅水平方向扩展
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    return doLayout(QRect(0, 0, width, 0), true);
}

int FlowLayout::count() const
{
    return m_items.size();
}

QLayoutItem* FlowLayout::itemAt(int index) const
{
    return m_items.value(index);
}

QLayoutItem* FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < m_items.size())
        return m_items.takeAt(index);
    return nullptr;
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem* item : qAsConst(m_items)) {
        size = size.expandedTo(item->minimumSize());
    }

    // 获取内容边距（左、上、右、下）
    QMargins margins = contentsMargins();
    int horizontalMargin = margins.left()  + margins.right();
    int verticalMargin = margins.top()  + margins.bottom();

    return size + QSize(horizontalMargin, verticalMargin);
}

void FlowLayout::setGeometry(const QRect& rect)
{
    if (geometry() == rect) // 如果相等，直接返回
        return;
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

//====== 内部工具函数 ======
int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QWidget* parent = parentWidget();
    if (!parent) return -1;
    return parent->style()->pixelMetric(pm, nullptr, parent);
}

int FlowLayout::doLayout(const QRect& rect, bool testOnly) const
{
    int left, top, right, bottom;
    getContentsMargins(&left, &top, &right, &bottom);
    QRect effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
    int x = effectiveRect.x();
    int y = effectiveRect.y();
    int lineHeight = 0;

    for (QLayoutItem* item : std::as_const(m_items))
    {
        const QWidget* wid = item->widget();
        int spaceX = horizontalSpacing();
        if (spaceX == -1)
        {
            spaceX = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Horizontal);
        }
        int spaceY = verticalSpacing();
        if (spaceY == -1)
        {
            spaceY = wid->style()->layoutSpacing(QSizePolicy::PushButton, QSizePolicy::PushButton, Qt::Vertical);
        }

        int nextX = x + item->sizeHint().width() + spaceX;
        if (nextX - spaceX > effectiveRect.right() && lineHeight > 0)
        {
            x = effectiveRect.x();
            y = y + lineHeight + spaceY;
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }
        if (!m_lastGeometryMap.contains(item))
        {
            m_lastGeometryMap.insert(item, QPoint(x, y));
        }
        if (!testOnly)
        {
            if ((item->geometry().x() == 0 && item->geometry().y() == 0))
            {
                item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
            }
            else if (item->geometry().x() != x || item->geometry().y() != y)
            {
                if (m_isAnimation)
                {
                    // 阻止多重动画
                    if (m_lastGeometryMap[item] == QPoint(x, y))
                    {
                        lineHeight = qMax(lineHeight, item->sizeHint().height());
                        return y + lineHeight - rect.y() + bottom;
                    }
                    QPropertyAnimation* geometryAnimation = new QPropertyAnimation(item->widget(), "geometry");
                    geometryAnimation->setStartValue(item->widget()->geometry());
                    geometryAnimation->setEndValue(QRect(QPoint(x, y), item->sizeHint()));
                    geometryAnimation->setDuration(300);
                    geometryAnimation->setEasingCurve(QEasingCurve::InOutSine);
                    geometryAnimation->start(QAbstractAnimation::DeleteWhenStopped);
                    m_lastGeometryMap[item] = QPoint(x, y);
                }
                else
                {
                    item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));
                }
            }
        }
        x = nextX;
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    return y + lineHeight - rect.y() + bottom;
}




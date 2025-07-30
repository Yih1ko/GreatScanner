#include "simplebutton.h"
#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>

SimpleButton::SimpleButton(QWidget *parent)
{

}

void SimpleButton::setBorderRadius(int radius)
{
    m_borderRadius = radius;
}

void SimpleButton::mousePressEvent(QMouseEvent *event)
{
    // 鼠标按下时背景变灰色
    if (event->button() == Qt::LeftButton) {
        m_pressed = true;
        update(); // 刷新界面以显示灰色背景
    }
    QWidget::mousePressEvent(event);
}

void SimpleButton::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标释放时背景恢复透明
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
        update(); // 刷新界面以恢复透明背景
        emit clicked();
    }
    QWidget::mouseReleaseEvent(event);
}

void SimpleButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿

    // 绘制圆角矩形背景
    QPainterPath path;
    path.addRoundedRect(rect(),  m_borderRadius, m_borderRadius);
    painter.setClipPath(path);

    // 根据鼠标状态设置背景颜色
    // if (m_pressed) {
    //     painter.fillRect(rect(),  QColor(230, 230, 230)); // 灰色背景
    // } else {
    //     painter.fillRect(rect(),  Qt::transparent); // 透明背景
    // }

    // 让子部件正常绘制
    QWidget::paintEvent(event);
}

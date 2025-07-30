#include "customgraphicsview.h"
#include <qevent.h>
#include <qscrollbar.h>

// 键盘按下事件（检测空格键）
void CustomGraphicsView::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        m_spacePressed = true;
        // setCursor(Qt::OpenHandCursor); // 改为手形光标
    }
    QGraphicsView::keyPressEvent(event);
}

// 键盘释放事件（检测空格键）
void CustomGraphicsView::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Space && !event->isAutoRepeat()) {
        m_spacePressed = false;
        if (!m_dragging) {
            // setCursor(Qt::ArrowCursor); // 恢复默认光标
        }
    }
    QGraphicsView::keyReleaseEvent(event);
}

// 鼠标按下事件（启动拖拽逻辑）
void CustomGraphicsView::mousePressEvent(QMouseEvent* event) {
    if (m_spacePressed && event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_lastDragPos = event->pos(); // 记录起点
        // setCursor(Qt::ClosedHandCursor); // 改为抓紧手形
        event->accept(); // 事件已处理
        return;
    }
    QGraphicsView::mousePressEvent(event); // 默认处理（如选择图元）
}

// 鼠标移动事件（视图平移）
void CustomGraphicsView::mouseMoveEvent(QMouseEvent* event) {
    if (m_dragging) {
        QPoint delta = event->pos() - m_lastDragPos; // 计算位移
        horizontalScrollBar()->setValue( // 水平滚动条调整
            horizontalScrollBar()->value() - delta.x()
            );
        verticalScrollBar()->setValue( // 垂直滚动条调整
            verticalScrollBar()->value() - delta.y()
            );
        m_lastDragPos = event->pos(); // 更新记录点
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
}

// 鼠标释放事件（结束拖拽）
void CustomGraphicsView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
        // setCursor(m_spacePressed ? Qt::OpenHandCursor : Qt::ArrowCursor);
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

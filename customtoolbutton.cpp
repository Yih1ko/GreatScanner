#include "customtoolbutton.h"
#include <qevent.h>
#include <qscrollbar.h>
#include <qtimer.h>

void CustomToolButton::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 启动长按定时器
        startLongPressTimer();
    }
    QToolButton::mousePressEvent(event);
}

void CustomToolButton::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        // 停止长按定时器
        stopLongPressTimer();
    }
    QToolButton::mouseReleaseEvent(event);
}

void CustomToolButton::mouseMoveEvent(QMouseEvent *event) {
    // 如果鼠标移动到按钮外，则停止长按
    if (!rect().contains(event->pos())) {
        stopLongPressTimer();
    }
    QToolButton::mouseMoveEvent(event);
}

void CustomToolButton::startLongPressTimer() {
    if (!longPressTimer) {
        longPressTimer = new QTimer(this);
        longPressTimer->setInterval(100); // 初始延迟 100ms
        connect(longPressTimer, &QTimer::timeout, this, &CustomToolButton::onLongPressTimeout);
        longPressTimer->start();
    }
}

void CustomToolButton::stopLongPressTimer() {
    if (longPressTimer) {
        longPressTimer->stop();
        delete longPressTimer;
        longPressTimer = nullptr;
    }
}

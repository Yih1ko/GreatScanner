#include "infinitedial.h"
#include <qevent.h>

void InfiniteDial::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        int delta = event->pos().x() - lastPos.x();
        setValue(value() + delta);
        lastPos = event->pos();
    }
    QDial::mouseMoveEvent(event);
}

void InfiniteDial::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPos = event->pos();
    }
    QDial::mousePressEvent(event);
}

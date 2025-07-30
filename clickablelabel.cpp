// ClickableLabel.cpp
#include "clickablelabel.h"
#include <QMouseEvent>
#include <QApplication>

ClickableLabel::ClickableLabel(QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent, f), m_pressed(false)
{

}

ClickableLabel::ClickableLabel(const QString& text, QWidget* parent, Qt::WindowFlags f)
    : QLabel(text, parent, f), m_pressed(false)
{

}

void ClickableLabel::mousePressEvent(QMouseEvent* event)
{
    m_pressed = true;
    setStyleSheet(R"(
        QLabel{
            background-color: rgba(197, 227, 255, 50);
        }
    )");
    QLabel::mousePressEvent(event);
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event)
{
    if(m_pressed && rect().contains(event->pos())) {
        emit clicked();
        emit clicked(event->pos());
        emit clicked(event->button());
    }
    setStyleSheet(R"(
        QLabel{
            background-color: rgba(255, 255, 255, 0);
        }
    )");
    m_pressed = false;
    QLabel::mouseReleaseEvent(event);
}

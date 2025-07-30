#ifndef SIMPLEBUTTON_H
#define SIMPLEBUTTON_H

#include <QObject>
#include <QPushButton>

class SimpleButton : public QPushButton
{
    Q_OBJECT
public:
    SimpleButton(QWidget *parent = nullptr);
    void setBorderRadius(int radius = 6);
protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    bool m_pressed;
    int m_borderRadius;
};

#endif // SIMPLEBUTTON_H

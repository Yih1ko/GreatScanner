#ifndef INFINITEDIAL_H
#define INFINITEDIAL_H

#include <QDial>

class InfiniteDial : public QDial {
    Q_OBJECT
public:
    InfiniteDial(QWidget *parent = nullptr) : QDial(parent) {
        setWrapping(true);
        setMinimum(0);
        setMaximum(360);
    }

protected:
    void mouseMoveEvent(QMouseEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

private:
    QPoint lastPos;
};

#endif // INFINITEDIAL_H

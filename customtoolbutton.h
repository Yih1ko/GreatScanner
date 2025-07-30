#ifndef CUSTOMTOOLBUTTON_H
#define CUSTOMTOOLBUTTON_H

#include <QToolButton>

class CustomToolButton : public QToolButton {
    Q_OBJECT

public:
    explicit CustomToolButton(QWidget *parent = nullptr)
        : QToolButton(parent), longPressTimer(nullptr) {}

protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void onLongPressTimeout() {
        // 发射自定义信号或执行操作
        emit longPressed();
    }

private:
    void startLongPressTimer();

    void stopLongPressTimer();

signals:
    void longPressed();

private:
    QTimer *longPressTimer;
};

#endif // CUSTOMTOOLBUTTON_H

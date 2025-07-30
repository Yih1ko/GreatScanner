#ifndef SLIDERINDICATOR_H
#define SLIDERINDICATOR_H

#include <QWidget>
#include <QPropertyAnimation>

class SliderIndicator : public QWidget {
    Q_OBJECT
public:
    explicit SliderIndicator(QWidget *parent = nullptr);

public slots:
    void activateAt(QWidget *target); // 外部按钮调用此槽更新位置

protected:
    void paintEvent(QPaintEvent *) override;
    void updatePosition(QWidget *target);

private:
    QPropertyAnimation *m_animation;
    QColor m_color = QColor("#252525");
    int m_borderRadius = 3;
};
#endif // SLIDERINDICATOR_H

#ifndef DIFFUSION_BUTTON_H
#define DIFFUSION_BUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QPropertyAnimation>

class Diffusion_button : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged FINAL)
    Q_PROPERTY(int opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)

public:
    explicit Diffusion_button(QWidget* parent = nullptr);
    void draw_disappearing_circle();
    void execute_animation();
    QPoint mouse_coordinates;
    QPropertyAnimation* animation1;
    QPropertyAnimation* animation3;
    void reset_animation();
    void setBorderRadius(int newBorderRadius);
    void setShadowOffset(int dx, int dy);    // 偏移量控制
    void setShadowBlur(int radius);          // 模糊度控制
    void setShadowColor(QColor color);       // 颜色控制

public:
    int radius() const;
    void setRadius(int newRadius);
    int opacity() const;
    void setOpacity(int newOpacity);

signals:
    void radiusChanged();
    void opacityChanged();

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    int m_radius = 0;
    int m_opacity = 255;
    int m_borderRadius = 10;
    QGraphicsDropShadowEffect* m_shadow;
};
#endif // DIFFUSION_BUTTON_H

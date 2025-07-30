#include "diffusion_button.h"

Diffusion_button::Diffusion_button(QWidget* parent) : QPushButton{ parent }
{
    this->setCursor(Qt::PointingHandCursor);
    this->resize(130, 42);

    m_shadow = new QGraphicsDropShadowEffect(this);

    m_shadow->setOffset(0, 8);
    m_shadow->setBlurRadius(20);
    m_shadow->setColor(QColor(0, 0, 0, 100));
    this->setGraphicsEffect(m_shadow);

    animation3 = new QPropertyAnimation(this, "radius");
    animation3->setDuration(400);
    animation3->setStartValue(m_radius);
    animation3->setEndValue(this->width());
    animation3->setEasingCurve(QEasingCurve::Linear);
    animation3->setDirection(QAbstractAnimation::Forward);

    animation1 = new QPropertyAnimation(this, "opacity");
    animation1->setDuration(400);
    animation1->setStartValue(m_opacity);
    animation1->setEndValue(0);
    animation1->setEasingCurve(QEasingCurve::Linear);
    animation1->setDirection(QAbstractAnimation::Forward);

    connect(animation3, &QPropertyAnimation::finished, this, &Diffusion_button::reset_animation);
    connect(animation1, &QPropertyAnimation::finished, this, &Diffusion_button::reset_animation);
}

void Diffusion_button::draw_disappearing_circle()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    QBrush brush(QColor(135, 206, 235, m_opacity));
    painter.setBrush(brush);
    painter.drawEllipse(mouse_coordinates, m_radius, m_radius);
}

void Diffusion_button::execute_animation()
{
    animation3->start();
    animation1->start();
}

void Diffusion_button::reset_animation()
{
    m_radius = 0;
    m_opacity = 255;
}

void Diffusion_button::setBorderRadius(int newBorderRadius)
{
    m_borderRadius = newBorderRadius;
    update();
}

void Diffusion_button::setShadowOffset(int dx, int dy)
{
    m_shadow->setOffset(dx, dy);
    update();  // 强制界面刷新
}

void Diffusion_button::setShadowBlur(int radius)
{
    m_shadow->setBlurRadius(radius);
    update();
}

void Diffusion_button::setShadowColor(QColor color)
{
    m_shadow->setColor(color);
    update();
}

void Diffusion_button::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setViewport(0, 0, width(), height());
    painter.setWindow(0, 0, width(), height());

    QPainterPath path;
    path.addRoundedRect(0, 0, width(), height(), m_borderRadius, m_borderRadius);
    painter.setClipPath(path);
    painter.setPen(Qt::NoPen);

    QBrush Brush(QColor(255, 255, 255, 255));
    painter.setBrush(Brush);
    painter.drawRect(0, 0, width(), height());
    this->draw_disappearing_circle();

    // 添加文本绘制
    painter.setPen(Qt::black);  // 设置文字颜色
    painter.setFont(this->font());
    painter.drawText(rect(),  Qt::AlignCenter, text());
}

void Diffusion_button::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        mouse_coordinates = event->pos();
        emit clicked();
        update();
        execute_animation();
    }
}

int Diffusion_button::radius() const
{
    return m_radius;
}

void Diffusion_button::setRadius(int newRadius)
{
    if (m_radius == newRadius)
        return;
    m_radius = newRadius;
    update();
    emit radiusChanged();
}

int Diffusion_button::opacity() const
{
    return m_opacity;
}

void Diffusion_button::setOpacity(int newOpacity)
{
    if (m_opacity == newOpacity)
        return;
    m_opacity = newOpacity;
    update();
    emit opacityChanged();
}

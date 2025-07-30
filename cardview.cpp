#include "cardview.h"
#include "global.h"
#include <QDebug>

CardView::CardView(QWidget* parent)
    : QWidget(parent),
    m_borderRadius(5),
    m_textHeight(10),
    m_horizontalPixmapRatio(1),
    m_verticalPixmapRatio(1),
    m_shadowBorderWidth(6),
    m_hoverGradient(nullptr),
    m_pressGradient(nullptr),
    m_isPressAnimationFinished(true),
    m_pressRadius(0),
    m_hoverOpacity(0),
    m_pressOpacity(0),
    m_isWithBelowText(true)
{
    setAttribute(Qt::WA_TranslucentBackground);//设置背景透明
    setMouseTracking(true);
    setObjectName("CardView");
    setStyleSheet("#CardView{background-color:transparent;}"); //默认背景色

    m_hoverGradient = new QRadialGradient();
    m_hoverGradient->setRadius(170);
    m_hoverGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 40));
    m_hoverGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 0));

    m_pressGradient = new QRadialGradient();
    m_pressGradient->setRadius(170);
    m_pressGradient->setColorAt(0, QColor(0xFF, 0xFF, 0xFF, 0));
    m_pressGradient->setColorAt(1, QColor(0xFF, 0xFF, 0xFF, 40));
}

CardView::~CardView()
{
    delete m_hoverGradient;
    delete m_pressGradient;
}

void CardView::setBorderRadius(int radius)
{
    if (m_borderRadius != radius) {
        m_borderRadius = radius;
        update();
    }
}

int CardView::getBorderRadius() const
{
    return m_borderRadius;
}

void CardView::setCardPixmap(const QPixmap& pixmap)
{
    //if (m_cardPixmap != pixmap) {
        m_cardPixmap = pixmap;
        update();
    //}
}

QPixmap CardView::getCardPixmap() const
{
    return m_cardPixmap;
}

void CardView::setDesBelow(const QString &text)
{
    if (m_desBelow != text) {
        m_desBelow = text;
        update();
    }
}

QString CardView::getDesBelow() const
{
    return m_desBelow;
}

// void CardView::setTextHeight(int height)
// {
//     if (m_textHeight != height) {
//         m_textHeight = height;
//         update();
//     }
// }

int CardView::getTextHeight() const
{
    return m_textHeight;
}

void CardView::setHorizontalPixmapRatio(qreal ratio)
{
    if (ratio > 0 && ratio <= 1 && m_horizontalPixmapRatio != ratio) {
        m_horizontalPixmapRatio = ratio;
        update();
    }
}

qreal CardView::getHorizontalPixmapRatio() const
{
    return m_horizontalPixmapRatio;
}

void CardView::setVerticalPixmapRatio(qreal ratio)
{
    if (ratio > 0 && ratio <= 1 && m_verticalPixmapRatio != ratio) {
        m_verticalPixmapRatio = ratio;
        update();
    }
}

qreal CardView::getVerticalPixmapRatio() const
{
    return m_verticalPixmapRatio;
}

void CardView::setPressRadius(qreal radius)
{
    if (m_pressRadius != radius) {
        m_pressRadius = radius;
        update();
    }
}

qreal CardView::getPressRadius() const
{
    return m_pressRadius;
}

void CardView::setHoverOpacity(qreal opacity)
{
    if (m_hoverOpacity != opacity) {
        m_hoverOpacity = opacity;
        update();
    }
}

qreal CardView::getHoverOpacity() const
{
    return m_hoverOpacity;
}

void CardView::setPressOpacity(qreal opacity)
{
    if (m_pressOpacity != opacity) {
        m_pressOpacity = opacity;
        update();
    }
}

qreal CardView::getPressOpacity() const
{
    return m_pressOpacity;
}

void CardView::setWithBelowText(bool isWithBelowText)
{
    m_isWithBelowText = isWithBelowText;
}

bool CardView::getWithBelowText() const
{
    return m_isWithBelowText;
}

qreal CardView::getLongestDistance(QPoint point)
{
    qreal topLeftDis = distance(point, QPoint(0, 0));
    qreal topRightDis = distance(point, QPoint(width(), 0));
    qreal bottomLeftDis = distance(point, QPoint(0, height()));
    qreal bottomRightDis = distance(point, QPoint(width(), height()));

    std::vector<qreal> disVector{topLeftDis, topRightDis, bottomLeftDis, bottomRightDis};
    std::sort(disVector.begin(),  disVector.end());

    return disVector[3];
}

qreal CardView::distance(QPoint point1, QPoint point2)
{
    return std::sqrt((point1.x() - point2.x()) * (point1.x() - point2.x()) +
                     (point1.y() - point2.y()) * (point1.y() - point2.y()));
}

void CardView::startHoverOpacityAnimation(bool isVisible)
{
    QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "hoverOpacity");
    connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        update();
    });

    opacityAnimation->setDuration(155);
    opacityAnimation->setStartValue(m_hoverOpacity);
    opacityAnimation->setEndValue(isVisible ? 1 : 0);
    opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

bool CardView::event(QEvent* event)
{
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        // 判断点击位置是否在图片或文字区域内
        QRect cont;

        if(m_isWithBelowText){
            cont.setLeft(m_shadowBorderWidth+ m_textHeight/2);
            cont.setTop(m_shadowBorderWidth);
            cont.setWidth(width() - 2 * m_shadowBorderWidth- m_textHeight);
            cont.setHeight(height() - 2 * m_shadowBorderWidth - m_textHeight);
        }else{
            cont.setLeft(m_shadowBorderWidth+ m_textHeight/2);
            cont.setTop(m_shadowBorderWidth);
            cont.setWidth(width() - 2 * m_shadowBorderWidth);
            cont.setHeight(height() - 2 * m_shadowBorderWidth);
        }

        bool isClickOnContent = cont.contains(mouseEvent->pos())  || cont.contains(mouseEvent->pos());
        if (!isClickOnContent) {
            return QWidget::event(event);
        }

        QPropertyAnimation* opacityAnimation = new QPropertyAnimation(this, "pressOpacity");
        connect(opacityAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            update();
        });
        connect(opacityAnimation, &QPropertyAnimation::finished, this, [=]() {
            m_isPressAnimationFinished = true;
        });

        opacityAnimation->setDuration(300);
        opacityAnimation->setEasingCurve(QEasingCurve::InQuad);
        opacityAnimation->setStartValue(1);
        opacityAnimation->setEndValue(0);
        opacityAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        QPropertyAnimation* pressAnimation = new QPropertyAnimation(this, "pressRadius");
        connect(pressAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
            m_pressGradient->setRadius(value.toReal());
        });

        pressAnimation->setDuration(300);
        pressAnimation->setEasingCurve(QEasingCurve::InQuad);
        pressAnimation->setStartValue(30);
        pressAnimation->setEndValue(getLongestDistance(mouseEvent->pos()) * 1.8);
        pressAnimation->start(QAbstractAnimation::DeleteWhenStopped);

        m_isPressAnimationFinished = false;
        m_pressGradient->setFocalPoint(mouseEvent->pos());
        m_pressGradient->setCenter(mouseEvent->pos());
        startHoverOpacityAnimation(false);
        break;
    }

    case QEvent::MouseButtonRelease: {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        // 判断点击位置是否在图片或文字区域内
        QRect cont;

        if(m_isWithBelowText){
            cont.setLeft(m_shadowBorderWidth+ m_textHeight/2);
            cont.setTop(m_shadowBorderWidth);
            cont.setWidth(width() - 2 * m_shadowBorderWidth- m_textHeight);
            cont.setHeight(height() - 2 * m_shadowBorderWidth - m_textHeight);
        }else{
            cont.setLeft(m_shadowBorderWidth+ m_textHeight/2);
            cont.setTop(m_shadowBorderWidth);
            cont.setWidth(width() - 2 * m_shadowBorderWidth);
            cont.setHeight(height() - 2 * m_shadowBorderWidth);
        }
        bool isClickOnContent = cont.contains(mouseEvent->pos())  || cont.contains(mouseEvent->pos());
        if (!isClickOnContent) {
            break;
        }
        emit cardClicked();
        break;
    }

    case QEvent::MouseMove: {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (m_hoverOpacity < 1 && m_isPressAnimationFinished) {
            startHoverOpacityAnimation(true); // 显示悬浮效果
        }

        if (m_isPressAnimationFinished) {
            m_hoverGradient->setCenter(mouseEvent->pos()); // 更新悬浮渐变的中心点
            m_hoverGradient->setFocalPoint(mouseEvent->pos()); // 更新悬浮渐变的焦点
        }
        update();
        break;
    }

    case QEvent::Enter: {
        startHoverOpacityAnimation(true);
        break;
    }

    case QEvent::Leave: {
        startHoverOpacityAnimation(false);
        break;
    }

    default:
        break;
    }

    return QWidget::event(event);
}

void CardView::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing  | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    QRect foregroundRect;//QRect(int left, int top, int width, int height) noexcept;

    if(m_isWithBelowText){
        // 减少图片区域的高度以腾出空间
        foregroundRect.setLeft(m_shadowBorderWidth+ m_textHeight/2);
        foregroundRect.setTop(m_shadowBorderWidth);
        foregroundRect.setWidth(width() - 2 * m_shadowBorderWidth- m_textHeight);
        foregroundRect.setHeight(height() - 2 * m_shadowBorderWidth - m_textHeight);
    }else{
        foregroundRect.setLeft(m_shadowBorderWidth+ m_textHeight/2);
        foregroundRect.setTop(m_shadowBorderWidth);
        foregroundRect.setWidth(width() - 2 * m_shadowBorderWidth);
        foregroundRect.setHeight(height() - 2 * m_shadowBorderWidth);
    }

    // 绘制带有圆角的阴影
    {
        painter.save();   // 保存当前状态
        QPainterPath path;
        path.setFillRule(Qt::WindingFill);
        QColor color = QColor(84, 84, 84);
        for (int i = 0; i < foregroundRect.top();  i++)
        {
            path.addRoundedRect(foregroundRect.left()-  i, foregroundRect.top()  - i, rect().width() - (foregroundRect.top()  - i) * 2 - m_textHeight, rect().height() - (foregroundRect.top()  - i) * 2 - m_textHeight, m_borderRadius + i, m_borderRadius + i);
            int alpha = 1 * (foregroundRect.top()  - i + 1);
            color.setAlpha(alpha  > 255 ? 255 : alpha);
            painter.setPen(color);
            painter.drawPath(path);
        }
        painter.restore();   // 恢复到保存的状态
    }

    // {
    //     painter.save();
    //     drawEffectShadow(&painter, foregroundRect, 20, m_borderRadius);
    //     painter.restore();
    // }

    // 绘制前景内容（图片）
    {
        painter.save();  // 保存当前状态

        QPainterPath path;
        path.addRoundedRect(foregroundRect,  m_borderRadius, m_borderRadius);
        painter.setClipPath(path);

        // 图片绘制
        if (!m_cardPixmap.isNull())  {
            QRect pixSourceRect = m_cardPixmap.rect();
            qreal horizontalOffset = m_cardPixmap.width()  * (1 - m_horizontalPixmapRatio) / 2;
            qreal verticalOffset = m_cardPixmap.height()  * (1 - m_verticalPixmapRatio) / 2;

            pixSourceRect.adjust(horizontalOffset,  verticalOffset, -horizontalOffset, -verticalOffset);
            painter.drawPixmap(foregroundRect,  m_cardPixmap, pixSourceRect);
        }
        painter.restore();  // 恢复到保存的状态
    }


    // 新增：绘制文字描述
    {
        if(m_isWithBelowText){
            painter.save();
            //文字区域的位置
            int textY = foregroundRect.bottom()+5;  // 在图片下方留一点间距

            // 文字区域的宽度与图片一致
            int textWidth = foregroundRect.width();

            // 计算文字的实际大小
            QFontMetrics fontMetrics(painter.font());
            int textLength = fontMetrics.boundingRect(m_desBelow).width();

            // 如果文字过长，截断并添加"..."
            if (textLength > textWidth) {
                m_desBelow = fontMetrics.elidedText(m_desBelow,  Qt::ElideRight, textWidth);
            }

            // 绘制文字
            painter.setPen(QColor(Qt::black));   // 设置文字颜色
            painter.drawText(foregroundRect.left(),  textY, textWidth, m_textHeight,
                             Qt::AlignCenter | Qt::TextSingleLine, m_desBelow);
            painter.restore();
        }
    }


    // 绘制悬浮效果
    {
        painter.save();  // 保存当前状态
        if (m_isPressAnimationFinished) {
            painter.setOpacity(m_hoverOpacity);
            painter.setBrush(*m_hoverGradient);
            painter.drawEllipse(m_hoverGradient->center(),  m_hoverGradient->radius(), m_hoverGradient->radius());
        } else {
            painter.setOpacity(m_pressOpacity);
            painter.setBrush(*m_pressGradient);
            painter.drawEllipse(m_pressGradient->center(),  m_pressRadius, m_pressRadius / 1.1);
        }
        painter.restore();
    }

    painter.restore();
}

#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <QWidget>
#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QPropertyAnimation>
#include <QPainter>
#include <QPainterPath>
#include <QRadialGradient>
#include <QMouseEvent>
#include <QFontMetrics>
#include <QColor>
#include <QRect>
#include <QList>
#include <cmath>

class CardView : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QPixmap cardPixmap READ getCardPixmap WRITE setCardPixmap)
    Q_PROPERTY(qreal horizontalPixmapRatio READ getHorizontalPixmapRatio WRITE setHorizontalPixmapRatio)
    Q_PROPERTY(qreal verticalPixmapRatio READ getVerticalPixmapRatio WRITE setVerticalPixmapRatio)
    Q_PROPERTY(qreal pressRadius READ getPressRadius WRITE setPressRadius)
    Q_PROPERTY(qreal hoverOpacity READ getHoverOpacity WRITE setHoverOpacity)
    Q_PROPERTY(qreal pressOpacity READ getPressOpacity WRITE setPressOpacity)

public:
    explicit CardView(QWidget* parent = nullptr);
    ~CardView();

    void setBorderRadius(int radius);
    int getBorderRadius() const;

    void setCardPixmap(const QPixmap& pixmap);
    QPixmap getCardPixmap() const;

    void setDesBelow(const QString& text);
    QString getDesBelow() const;

    //void setTextHeight(int height);
    int getTextHeight() const;

    void setHorizontalPixmapRatio(qreal ratio);
    qreal getHorizontalPixmapRatio() const;

    void setVerticalPixmapRatio(qreal ratio);
    qreal getVerticalPixmapRatio() const;

    void setPressRadius(qreal radius);
    qreal getPressRadius() const;

    void setHoverOpacity(qreal opacity);
    qreal getHoverOpacity() const;

    void setPressOpacity(qreal opacity);
    qreal getPressOpacity() const;

    void setWithBelowText(bool isWithBelowText);
    bool getWithBelowText() const;

Q_SIGNALS:
    void cardClicked();

protected:
    bool event(QEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    int m_borderRadius;
    QPixmap m_cardPixmap;
    QString m_desBelow;
    int m_textHeight;
    qreal m_horizontalPixmapRatio;
    qreal m_verticalPixmapRatio;
    int m_shadowBorderWidth;
    QRadialGradient* m_hoverGradient;
    QRadialGradient* m_pressGradient;
    bool m_isPressAnimationFinished;
    qreal m_pressRadius;
    qreal m_hoverOpacity;
    qreal m_pressOpacity;
    bool m_isWithBelowText;

    qreal getLongestDistance(QPoint point);
    qreal distance(QPoint point1, QPoint point2);
    void startHoverOpacityAnimation(bool isVisible);
};

#endif // CARDVIEW_H

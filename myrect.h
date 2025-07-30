#ifndef MYRECT_H
#define MYRECT_H

#include <QObject>
#include <QRectF>
#include <QString>
#include <QVector>

class QPainter;

/**
 * @brief 显示当前宽度和高度的矩形类
 */
class MyRect : public QObject
{
    Q_OBJECT
public:
    MyRect();
    explicit MyRect(QRectF rect);

    void setText(QString str);
    void drawMe(QPainter &painter);
    void setLocation(qreal x, qreal y);

private:
    QRectF m_rect;  // 保存的矩形区域
    QString m_str;  // 显示的文本
};

#endif // MYRECT_H

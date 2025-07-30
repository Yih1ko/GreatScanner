#ifndef COLORBLOCK_H
#define COLORBLOCK_H

#include <QWidget>
#include <QColor>
#include <QPainterPath>
#include <QMouseEvent>

class ColorBlock : public QWidget
{
    Q_OBJECT
public:
    explicit ColorBlock(QWidget *parent = nullptr);

    // 设置控件模式
    enum Mode { ColorMode, PickerMode };
    void setMode(Mode mode);

    // 颜色设置/获取
    void setColor(const QColor &color);
    QColor color() const;

    // 选中状态设置/获取
    void setSelected(bool selected);
    bool isSelected() const;

signals:
    // 点击时发送颜色信号
    void colorSelected(const QColor &color);
    // 新增：选中状态变化信号
    void selectionChanged(bool selected);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    // 绘制彩色选择器背景
    void drawPickerBackground(QPainter &painter);

    Mode m_mode = ColorMode;  // 当前模式
    QColor m_color = Qt::black;  // 存储的颜色值
    QPainterPath m_clipPath;  // 圆形裁剪路径
    bool m_selected = false;  // 选中状态
    bool m_hasPicked = false;   //颜色选取状态标识
};

#endif // COLORBLOCK_H

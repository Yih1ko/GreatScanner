#ifndef TEXTLABEL_H
#define TEXTLABEL_H

#include <QLabel>       // 基础 QLabel 类
#include <QFont>        // 字体设置
#include <QPainter>     // 绘制工具
#include <QTimer>       // 定时器（目前未使用）
#include "Def.h"

// 自定义文本标签类，继承自 QLabel
class TextLabel : public QLabel
{
    Q_OBJECT  // 必须宏，启用 Qt 的元对象系统（信号和槽）

    // Q_PROPERTY 宏用于将类的方法暴露为 Qt 属性，支持绑定和外部访问
    Q_PROPERTY(bool IsWrapAnywhere READ getIsWrapAnywhere WRITE setIsWrapAnywhere)   // 是否启用任意位置换行
    Q_PROPERTY(int TextPixelSize READ getTextPixelSize WRITE setTextPixelSize)       // 文本像素大小
    Q_PROPERTY(int TextPointSize READ getTextPointSize WRITE setTextPointSize)       // 文本点大小
    Q_PROPERTY(TextStyle TextStyle READ getTextStyle WRITE setTextStyle)             // 文本样式

public:
    // 构造函数
    explicit TextLabel(QWidget* parent = nullptr);          // 默认构造函数
    explicit TextLabel(QString text, QWidget* parent = nullptr); // 带初始文本的构造函数
    explicit TextLabel(QString text, int pixelSize, QWidget* parent = nullptr); // 带文本和像素大小的构造函数

    ~TextLabel() override;                                   // 析构函数

    // 设置是否启用任意位置换行
    void setIsWrapAnywhere(bool isWrapAnywhere);
    bool getIsWrapAnywhere() const;

    // 文本像素大小
    void setTextPixelSize(int size);
    int getTextPixelSize() const;

    // 文本点大小
    void setTextPointSize(int size);
    int getTextPointSize() const;

    // 文本样式
    void setTextStyle(TextStyle textStyle = TextStyle::NoStyle);
    TextStyle getTextStyle() const;

    // 设置字体（不改变字体大小）
    void setTextFont(const QFont &font);

    //设置不透明度
    void setOpacity(double value);

signals:
    void clicked();

protected:
    // 重写绘制事件，用于自定义文本绘制
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    bool m_isWrapAnywhere{false};   // 是否启用任意位置换行，默认关闭
    qreal m_textSpacing{0.5};       // 文本间距，默认 0.5
    TextStyle m_textStyle{TextStyle::NoStyle}; // 当前文本样式，默认无样式
};

#endif // TEXTLABEL_H

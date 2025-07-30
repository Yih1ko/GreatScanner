#include "textlabel.h"
#include <QPainter>
#include <QFont>
#include <QTimer>

// 默认构造函数
TextLabel::TextLabel(QWidget* parent)
    : QLabel(parent)
{
    setObjectName("TextLabel");      // 设置对象名称，便于外部引用
    setStyleSheet("#TextLabel{background-color:transparent;}");  // 设置背景透明

    QFont textFont = font();         // 获取当前字体
    textFont.setFamily("PingFang SC");  // 设置默认字体为 PingFang SC
    textFont.setLetterSpacing(QFont::AbsoluteSpacing,   m_textSpacing);  // 设置字符间距
    textFont.setPixelSize(28);         // 设置默认像素大小为 28
    setFont(textFont);               // 应用字体设置
    setWordWrap(true);               // 启用文字换行

    setTextStyle();
}

// 带初始文本的构造函数
TextLabel::TextLabel(QString text, QWidget* parent)
    : TextLabel(parent)              // 调用默认构造函数初始化
{
    QFont textFont = font();         // 获取当前字体
    textFont.setFamily("PingFang SC");  // 设置默认字体为 PingFang SC
    textFont.setLetterSpacing(QFont::AbsoluteSpacing,   m_textSpacing);  // 设置字符间距
    textFont.setPixelSize(28);         // 设置默认像素大小为 28
    setFont(textFont);               // 应用字体设置
    setWordWrap(true);               // 启用文字换行
    setText(text);                   // 设置初始文本

    setTextStyle();
}

// 带文本和像素大小的构造函数
TextLabel::TextLabel(QString text, int pixelSize, QWidget* parent)
    : TextLabel(text, parent)        // 调用带文本的构造函数初始化
{
    QFont font = this->font();       // 获取当前字体
    font.setPixelSize(pixelSize);     // 设置指定的像素大小
    setFont(font);                   // 应用字体设置
}

// 析构函数
TextLabel::~TextLabel()
{
    // 无需特殊清理操作，默认析构即可
}

// 设置是否启用任意位置换行
void TextLabel::setIsWrapAnywhere(bool isWrapAnywhere)
{
    setWordWrap(isWrapAnywhere);     // 启用或禁用文字换行
    m_isWrapAnywhere = isWrapAnywhere;  // 更新成员变量状态
}

// 获取是否启用任意位置换行
bool TextLabel::getIsWrapAnywhere() const
{
    return m_isWrapAnywhere;         // 返回当前状态
}

// 设置文本像素大小
void TextLabel::setTextPixelSize(int size)
{
    QFont font = this->font();       // 获取当前字体
    font.setPixelSize(size);          // 设置新的像素大小
    setFont(font);                   // 应用字体设置
}

// 获取文本像素大小
int TextLabel::getTextPixelSize() const
{
    return this->font().pixelSize();  // 返回当前像素大小
}

// 设置文本点大小
void TextLabel::setTextPointSize(int size)
{
    QFont font = this->font();       // 获取当前字体
    font.setPointSize(size);          // 设置新的点大小
    setFont(font);                   // 应用字体设置
}

// 获取文本点大小
int TextLabel::getTextPointSize() const
{
    return this->font().pointSize(); // 返回当前点大小
}

// 设置文本样式（根据枚举值调整字体属性）
void TextLabel::setTextStyle(TextStyle textStyle)
{
    m_textStyle = textStyle;         // 更新当前样式

    QFont textFont = font();         // 获取当前字体

    switch (textStyle)               // 根据样式调整字体属性
    {
    case TextStyle::NoStyle:
        // 清除所有样式，恢复默认字体属性
        textFont.setWeight(QFont::Normal);    // 恢复正常粗细
        textFont.setStyle(QFont::StyleNormal);  // 恢复正常风格
        break;
    case TextStyle::Caption:
        textFont.setPixelSize(12);            // 设置像素大小为 12
        break;
    case TextStyle::Body:
        textFont.setPixelSize(13);            // 设置像素大小为 13
        break;
    case TextStyle::BodyStrong:
        textFont.setPixelSize(13);            // 设置像素大小为 13
        textFont.setWeight(QFont::DemiBold);   // 设置为半加粗
        break;
    case TextStyle::Subtitle:
        textFont.setPixelSize(20);            // 设置像素大小为 20
        textFont.setWeight(QFont::DemiBold);   // 设置为半加粗
        break;
    case TextStyle::Title:
        textFont.setPixelSize(28);            // 设置像素大小为 28
        textFont.setWeight(QFont::DemiBold);   // 设置为半加粗
        break;
    case TextStyle::TitleLarge:
        textFont.setPixelSize(40);            // 设置像素大小为 40
        textFont.setWeight(QFont::DemiBold);   // 设置为半加粗
        break;
    case TextStyle::Display:
        textFont.setPixelSize(48);            // 设置像素大小为 48
        textFont.setWeight(QFont::DemiBold);   // 设置为半加粗
        break;
    }

    setFont(textFont);                  // 应用新的字体设置
}

// 获取当前文本样式
TextStyle TextLabel::getTextStyle() const
{
    return m_textStyle;                // 返回当前样式枚举值
}

void TextLabel::setTextFont(const QFont &font)
{
    QFont currentFont = font;           // 创建一个副本
    int currentPixelSize = getTextPixelSize(); // 获取当前的像素大小

    currentFont.setPixelSize(currentPixelSize);  // 保持像素大小不变
    setFont(currentFont);                   // 应用新的字体设置
}

void TextLabel::setOpacity(double value)
{
    // 将 double 类型的 value 转换为 0~255 的范围
    // 其中 value 范围建议在 0~100 之间
    // 例如：
    // - value = 0 → opacity = 0 (完全透明)
    // - value = 10 → opacity = 25.5 (约 10% 不透明)
    // - value = 100 → opacity = 255 (完全不透明)
    int opacityValue = static_cast<int>(value * 2.55);  // 将 0~100 转换为 0~255

    // 确保不透明度值在有效范围内
    if (opacityValue < 0)
        opacityValue = 0;
    else if (opacityValue > 255)
        opacityValue = 255;

    // 将不透明度值转换为 0.0~1.0 的范围
    qreal opacity = opacityValue / 255.0;

    // 设置控件的不透明度
    setWindowOpacity(opacity);
}

void TextLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
    QLabel::mousePressEvent(event); // 继续处理默认事件
}

// 重写绘制事件，用于自定义文本绘制逻辑
void TextLabel::paintEvent(QPaintEvent* event)
{
    if (wordWrap() && m_isWrapAnywhere)  // 如果启用了文字换行和任意位置换行
    {
        QPainter painter(this);          // 创建 QPainter 对象，用于绘制内容

        painter.save();                   // 保存当前绘制状态

        painter.setRenderHints(           // 设置抗锯齿渲染提示，提高文字质量
            QPainter::Antialiasing |
            QPainter::TextAntialiasing);

        painter.setPen(Qt::black);        // 设置绘制颜色为黑色

        painter.drawText(                 // 绘制文本，支持换行和自适应布局
            rect(),                     // 在整个控件区域内绘制
            Qt::AlignLeft |              // 左对齐
                Qt::AlignVCenter |           // 垂直居中对齐
                Qt::TextWordWrap |           // 启用单词换行
                Qt::TextWrapAnywhere,        // 允许在任何位置换行（包括单词内部）
            text());                    // 绘制当前文本内容

        painter.restore();                // 恢复之前的绘制状态
    }
    else
    {
        QLabel::paintEvent(event);       // 使用默认 QLabel 的绘制逻辑
    }
}

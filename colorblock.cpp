#include "colorblock.h"
#include <QPainter>
#include <QColorDialog>
#include <QConicalGradient>

ColorBlock::ColorBlock(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(18, 18);  // 默认尺寸
    setAttribute(Qt::WA_TranslucentBackground); // 确保透明背景
}

// ======== 公开方法 ========
void ColorBlock::setMode(Mode mode) {
    m_mode = mode;
    update();
}

void ColorBlock::setColor(const QColor &color) {
    m_color = color;
    m_hasPicked = true;  // 标记已选取颜色
    update();
}

QColor ColorBlock::color() const {
    return m_color;
}

void ColorBlock::setSelected(bool selected) {
    if (m_selected != selected) {
        m_selected = selected;
        update(); // 触发重绘
        emit selectionChanged(m_selected); // 通知选中状态变化
    }
}

bool ColorBlock::isSelected() const {
    return m_selected;
}

// ======== 事件处理 ========
void ColorBlock::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // === 核心修改: 模式判断逻辑 ===
    if (m_mode == ColorMode) {
        // 单色模式: 始终显示存储颜色
        painter.setBrush(m_color);
    } else {
        // 拾色器模式: 根据选取状态切换
        if (m_hasPicked) {
            painter.setBrush(m_color);   // 显示选取的颜色
        } else {
            drawPickerBackground(painter);  // 显示彩虹背景
        }
    }

    // 绘制圆形主体
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(rect().adjusted(1,  1, -1, -1));

    // === 边框绘制优化 ===
    QPen borderPen;
    if (m_selected) {
        borderPen = QPen(QColor(65, 105, 225), 2);  // 选中状态
    } else if (m_mode == PickerMode && !m_hasPicked) {
        borderPen = QPen(Qt::white, 1);  // 未选中的拾色器特殊边框
    } else {
        borderPen = QPen(Qt::lightGray, 1);  // 默认边框
    }
    painter.setPen(borderPen);
    painter.setBrush(Qt::NoBrush);
    painter.drawEllipse(rect());
}

void ColorBlock::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        setSelected(true);  // 更新选中状态

        if (m_mode == PickerMode) {
            // 核心交互: 打开颜色对话框
            QColor newColor = QColorDialog::getColor(
                m_hasPicked ? m_color : Qt::white,  // 智能初始化
                this,
                "选择颜色",
                QColorDialog::ShowAlphaChannel
                );

            if (newColor.isValid())  {
                setColor(newColor);  // 更新颜色并标记已选取
                emit colorSelected(newColor);
            }
        } else {
            // 颜色模式直接发射当前颜色
            emit colorSelected(m_color);
        }
    }
}

void ColorBlock::resizeEvent(QResizeEvent *) {
    // 创建圆形裁剪路径（缩小1px避免边框被裁剪）
    QRectF pathRect = rect().adjusted(1, 1, -1, -1);
    m_clipPath = QPainterPath();
    m_clipPath.addEllipse(pathRect);
}

// ======== 拾色器渲染 ========
void ColorBlock::drawPickerBackground(QPainter &painter) {
    // 径向渐变（彩虹色）
    QRadialGradient grad(rect().center(), width()/2);
    grad.setColorAt(0.0,  Qt::white);
    grad.setColorAt(0.2,  Qt::red);
    grad.setColorAt(0.4,  Qt::yellow);
    grad.setColorAt(0.6,  Qt::green);
    grad.setColorAt(0.8,  Qt::blue);
    grad.setColorAt(1.0,  Qt::black);

    painter.setBrush(grad);

    // 亮度渐变叠加
    QLinearGradient maskGrad(QPoint(0, 0), QPoint(width(), height()));
    maskGrad.setColorAt(0,   QColor(255, 255, 255, 150));
    maskGrad.setColorAt(1,   QColor(0, 0, 0, 150));
    painter.setBrush(maskGrad);
}

#include "markeditor.h"
#include "ui_markeditor.h"
#include <QPainter>
#include <QPainterPath>
#include <QLinearGradient>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include "clickablelabel.h"

MarkEditor::MarkEditor(QWidget *parent)
    :  QWidget(parent),
    ui(new Ui::MarkEditor)
{
    m_markMeta = {"WaterMark", "Arial", QColor(Qt::black), 160, 20};
    ui->setupUi(this);
    initUI();
    initSlots();
}

MarkEditor::~MarkEditor()
{
    delete ui;
}


void MarkEditor::initUI()
{
    // 窗口属性设置
    setWindowFlags(Qt::FramelessWindowHint );
    setAttribute(Qt::WA_TranslucentBackground);

    // 计算窗口初始位置（屏幕中央）
    QRect screenGeo = QApplication::primaryScreen()->geometry();
    int x = (screenGeo.width()  - width()) / 2;
    int y = (screenGeo.height()  - height()) / 3;
    move(x, y);

    // 设置控件
    ui->Color_1->setColor(Qt::black);
    ui->Color_2->setColor(Qt::white);
    ui->Color_3->setColor(Qt::yellow);
    ui->Color_4->setColor(Qt::red);
    ui->Color_5->setColor(Qt::green);
    ui->Color_6->setColor(Qt::blue);
    ui->Color_triger->setMode(ColorBlock::PickerMode);

    // 初始化颜色块列表
    m_colorBlocks << ui->Color_1 << ui->Color_2 << ui->Color_3
                  << ui->Color_4 << ui->Color_5 << ui->Color_6
                  << ui->Color_triger;

    // 设置内边距
    if (layout()) {
        layout()->setContentsMargins(12, 18, 12, 12);
    }
}

void MarkEditor::initSlots()
{
    // 连接所有颜色块的selectionChanged信号
    for (ColorBlock* block : m_colorBlocks) {
        connect(block, &ColorBlock::selectionChanged,
                this, &MarkEditor::onColorBlockSelected);
    }

    // 设置默认选中第一个颜色块
    if (!m_colorBlocks.isEmpty())  {
        m_colorBlocks.first()->setSelected(true);
    }

    QObject::connect(ui->OK, QOverload<>::of(&ClickableLabel::clicked), [this](){
        //透传水印元数据
        m_markMeta.m_opacity=ui->OpacitySetter->value();
        m_markMeta.m_size = ui->SizeSetter->value();
        m_markMeta.m_text = ui->markTextEdit->toPlainText();
        markMeta(m_markMeta);
        qDebug()<<m_markMeta.m_color<<m_markMeta.m_opacity<<m_markMeta.m_size<<m_markMeta.m_text;
        //关闭widget
        this->hide();
    });
    QObject::connect(ui->Cancel, QOverload<>::of(&ClickableLabel::clicked), [this](){
        //透传原始水印元数据
        markMeta(m_markMeta);
        qDebug()<<m_markMeta.m_color<<m_markMeta.m_opacity<<m_markMeta.m_size<<m_markMeta.m_text;
        //关闭widget
        this->hide();
    });
}

void MarkEditor::onColorBlockSelected(bool selected)
{
    if (!selected) return; // 只处理选中状态

    ColorBlock* senderBlock = qobject_cast<ColorBlock*>(sender());
    if (!senderBlock) return;

    // 遍历所有颜色块，取消非当前块的选中状态
    for (ColorBlock* block : m_colorBlocks) {
        if (block != senderBlock && block->isSelected()) {
            block->setSelected(false);
        }
    }

    // 更新当前选中的颜色
    m_markMeta.m_color = senderBlock->color();
}

// 核心绘图实现
void MarkEditor::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,  true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform,  true);

    // 1. 绘制圆角矩形背景
    QRect mainRect = rect().adjusted(1, 1, -1, -1);
    QPainterPath path;
    path.addRoundedRect(mainRect,  12, 12);

    // 创建玻璃质感渐变
    QLinearGradient gradient(0, 0, 0, height());
    gradient.setColorAt(0.0,  QColor(255, 255, 255, 245));
    gradient.setColorAt(0.5,  QColor(248, 248, 248, 245));
    gradient.setColorAt(1.0,  QColor(242, 242, 242, 245));

    // 填充圆角矩形
    painter.fillPath(path,  gradient);

    // 2. 绘制边框
    QPen borderPen;
    borderPen.setColor(QColor(200,  200, 200, 80));
    borderPen.setWidthF(1.2);
    painter.setPen(borderPen);
    painter.drawPath(path);

    // 3. 绘制高光线
    QLinearGradient highlightGrad(0, 1, 0, 6);
    highlightGrad.setColorAt(0,  QColor(255, 255, 255, 150));
    highlightGrad.setColorAt(1,  Qt::transparent);

    QPainterPath highlightPath;
    highlightPath.addRoundedRect(QRectF(2,  2, width()-4, 8), 5, 5);
    painter.fillPath(highlightPath,  highlightGrad);
}

// 鼠标事件实现拖动
void MarkEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPosition().toPoint() - frameGeometry().topLeft();
        event->accept();
    }
    QWidget::mousePressEvent(event);
}

void MarkEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPosition().toPoint() - m_dragPosition);

        // 添加边界吸附效果
        QRect screenGeo = QApplication::primaryScreen()->geometry();
        QPoint pos = event->globalPosition().toPoint() - m_dragPosition;

        if (qAbs(pos.x() - screenGeo.left())  < 20)
            pos.setX(screenGeo.left());
        else if (qAbs(pos.x() + width() - screenGeo.right())  < 20)
            pos.setX(screenGeo.right()  - width());

        if (qAbs(pos.y() - screenGeo.top())  < 20)
            pos.setY(screenGeo.top());
        else if (qAbs(pos.y() + height() - screenGeo.bottom())  < 20)
            pos.setY(screenGeo.bottom()  - height());

        move(pos);
        event->accept();
    }
    QWidget::mouseMoveEvent(event);
}

void MarkEditor::mouseReleaseEvent(QMouseEvent *event)
{
    m_dragging = false;
    QWidget::mouseReleaseEvent(event);
}

// 窗口大小变化时更新
void MarkEditor::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update();  // 触发重绘
}

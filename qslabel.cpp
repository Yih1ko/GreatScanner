#include "qslabel.h"
#include "controlwidget.h"
#include "screen.h"
#include <QPalette>
#include <QPainter>
#include <algorithm>

QSLabel::QSLabel(QWidget* parent) : QLabel(parent)
{
    setMouseTracking(true);
setCursor(Qt::SizeAllCursor); // 默认光标
    // 初始化状态
    ispressed = false;
    isdrawline = false;
    isdrawrectangle = false;
    isdrawround = false;
    istextedit = false;
    isdrawarrow = false;
    isreturnedit = false;

    // 初始化文本编辑框
    m_plaintextedit = new QTextEdit(this);
    m_plaintextedit->hide();
    m_plaintextedit->resize(60, 40);
    currentResizingText = new myText; // 初始化当前调整文本为空
    currentResizingText->isSelected = false;
    currentResizingText->isResizing = false;  // 初始化
    currentResizingText->resizeEdge = 0;

    // 设置透明背景
    QPalette palette = m_plaintextedit->palette();
    palette.setBrush(QPalette::Base,  QBrush(QColor(255, 0, 0, 0)));
    m_plaintextedit->setPalette(palette);

    // 样式设置
    m_plaintextedit->setStyleSheet(
        "QTextEdit{ border: 1px solid #dadada; }"
        "QTextEdit{ font-family:'Microsoft YaHei'; font-size:14px; color:#ff0000; }"
        );

    connect(m_plaintextedit, &QTextEdit::textChanged, this, &QSLabel::ontextchanged);
}

// 文本变化时的自适应调整
void QSLabel::ontextchanged()
{
    if (m_plaintextedit->toPlainText().size() < 10) {
        m_plaintextedit->resize(m_plaintextedit->toPlainText().size() * 10 + 50, 40);
    } else {
        m_plaintextedit->resize(
            m_plaintextedit->toPlainText().size() * 15 + 20,
            m_plaintextedit->document()->size().rheight() + 10
            );
    }
}

// 鼠标移动事件处理
void QSLabel::mouseMoveEvent(QMouseEvent *event)
{
    QPoint currentPos = event->position().toPoint();

    // 更新文本调整时的光标样式
    updateCursorForTextResize(currentPos);

    // 处理文本矩形框调整
    if (ispressed && currentResizingText) {
        QPoint delta = currentPos - resizeStartPoint;
        resizeTextRect(currentResizingText, delta, currentResizingText->resizeEdge);
        resizeStartPoint = currentPos; // 关键更新点
        update();
        return;
    }

    if (isdrawline || isdrawrectangle || isdrawround || istextedit || isdrawarrow) {
        setCursor(Qt::CrossCursor);
    }

    if (ispressed) {
        if (isdrawline) {
            endPoint = event->pos();
            myLine *line = new myLine;
            line->startPoint = startPoint;
            line->endPoint = endPoint;
            lines.push_back(line);
            startPoint = endPoint;
        }
        else if (isdrawrectangle || isdrawround || isdrawarrow) {
            endPoint = event->pos();
        }
        update();
    }
}

// 鼠标按下事件处理
void QSLabel::mousePressEvent(QMouseEvent *event)
{
    startPoint = event->position().toPoint();
    endPoint = startPoint;
    ispressed = true;

    // if (istextedit) {
    //     if (!m_plaintextedit->toPlainText().isEmpty()) {
    //         myText *text = new myText;
    //         text->mRect = QRect(
    //             QPoint(m_plaintextedit->x(), m_plaintextedit->y()),
    //             QSize(m_plaintextedit->width(), m_plaintextedit->height())
    //             );
    //         text->mText = m_plaintextedit->toPlainText();
    //         texts.push_back(text);
    //         update();
    //     }
    //     m_plaintextedit->move(startPoint);
    //     m_plaintextedit->show();
    //     m_plaintextedit->clear();
    // }
    // 首先检查是否在调整文本矩形框
    int edge = 0;
    for (auto* text : texts) {
        if (isNearTextEdge(startPoint, text, edge)) {
            text->isResizing = true;
            text->resizeEdge = edge;
            currentResizingText = text;
            resizeStartPoint = startPoint;
            originalResizeRect = text->mRect;
            return; // 处理文本调整，不进行其他操作
        }
    }

    // 取消所有文本的选中状态
    for (auto* text : texts) {
        text->isSelected = false;
    }

    if (istextedit) {
        if (!m_plaintextedit->toPlainText().isEmpty()) {
            myText *text = new myText;
            text->mRect = QRect(
                QPoint(m_plaintextedit->x(), m_plaintextedit->y()),
                QSize(m_plaintextedit->width(), m_plaintextedit->height())
                );
            text->mText = m_plaintextedit->toPlainText();
            text->isSelected = false; // 新文本默认不选中
            text->isResizing = false;
            texts.push_back(text);
            update();
        }
        m_plaintextedit->move(startPoint);
        m_plaintextedit->show();
        m_plaintextedit->clear();
    }
}

// 鼠标释放事件处理
void QSLabel::mouseReleaseEvent(QMouseEvent *event)
{
    ispressed = false;

    // 结束文本矩形框调整
    if (currentResizingText) {
        currentResizingText->isResizing = false;
        currentResizingText = nullptr;
        return;
    }

    if (isdrawrectangle) {
        endPoint = event->pos();
        myRectangle *rectangle = new myRectangle;
        rectangle->startPoint = startPoint;
        rectangle->endPoint = endPoint;
        rectangles.push_back(rectangle);
        actionVec.push_back("rectangles");
        update();
    }
    else if (isdrawround) {
        endPoint = event->pos();
        myRound *round = new myRound;
        round->startPoint = startPoint;
        round->endPoint = endPoint;
        rounds.push_back(round);
        actionVec.push_back("rounds");
        update();
    }
    else if (isdrawarrow) {
        endPoint = event->pos();
        myArrow *arrow = new myArrow;
        arrow->startPoint = startPoint;
        arrow->endPoint = endPoint;
        arrows.push_back(arrow);
        actionVec.push_back("arrows");
        update();
    }
}

// 绘制事件处理
void QSLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制背景图像
    QPixmap pix = QPixmap::fromImage(selectimage);
    pix.setDevicePixelRatio(devicePixelRatioF());
    painter.drawPixmap(0,  0, pix);

    // 设置绘图样式
    QPen pen(Qt::red, 2);
    painter.setPen(pen);

    // 绘制所有线段
    for (auto* line : lines) {
        painter.drawLine(line->startPoint,  line->endPoint);
    }

    // 绘制所有矩形
    for (auto* rectangle : rectangles) {
        int x = std::min(rectangle->startPoint.x(), rectangle->endPoint.x());
        int y = std::min(rectangle->startPoint.y(), rectangle->endPoint.y());
        painter.drawRect(x,  y,
                         abs(rectangle->endPoint.x() - rectangle->startPoint.x()),
                         abs(rectangle->endPoint.y() - rectangle->startPoint.y()));
    }

    // 绘制所有圆形
    for (auto* round : rounds) {
        int x = std::min(round->startPoint.x(), round->endPoint.x());
        int y = std::min(round->startPoint.y(), round->endPoint.y());
        painter.drawEllipse(x,  y,
                            abs(round->endPoint.x() - round->startPoint.x()),
                            abs(round->endPoint.y() - round->startPoint.y()));
    }

    // 绘制所有箭头
    for (auto* arrow : arrows) {
        drawarrow(arrow->startPoint, arrow->endPoint, painter);
    }

    // 绘制所有文本
    for (auto* text : texts) {
        // 绘制文本矩形框（选中状态高亮）
        if (text->isSelected || text->isResizing) {
            QPen highlightPen(Qt::blue, 1, Qt::DashLine);
            painter.setPen(highlightPen);
            painter.drawRect(text->mRect);

            // 重置为文本绘制颜色
            pen.setColor(Qt::red);
            painter.setPen(pen);
        }

        painter.drawText(text->mRect,   Qt::TextWrapAnywhere, text->mText);
    }

    // 绘制当前正在绘制的图形
    int xx = std::min(startPoint.x(), endPoint.x());
    int yy = std::min(startPoint.y(), endPoint.y());
    if (isdrawrectangle) {
        painter.drawRect(xx,  yy, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
    }
    else if (isdrawround) {
        painter.drawEllipse(xx,  yy, abs(endPoint.x() - startPoint.x()), abs(endPoint.y() - startPoint.y()));
    }
    else if (isdrawarrow) {
        drawarrow(startPoint, endPoint, painter);
    }
}

// 绘制箭头实现
void QSLabel::drawarrow(QPoint startpoint, QPoint endpoint, QPainter &p)
{
    const double par = 15.0;  // 箭头大小参数
    double slopy = atan2((endpoint.y() - startpoint.y()), (endpoint.x() - startpoint.x()));
    double cosy = cos(slopy);
    double siny = sin(slopy);

    // 计算箭头三个顶点
    QPoint point1 = QPoint(
        endpoint.x() + int(-par * cosy - (par / 2.0 * siny)),
        endpoint.y() + int(-par * siny + (par / 2.0 * cosy))
        );
    QPoint point2 = QPoint(
        endpoint.x() + int(-par * cosy + (par / 2.0 * siny)),
        endpoint.y() - int(par / 2.0 * cosy + par * siny)
        );

    QPoint points[3] = { endpoint, point1, point2 };

    // 绘制箭头三角形
    p.setRenderHint(QPainter::Antialiasing,  true);
    QPen drawtrianglepen(Qt::red, 1);
    p.setPen(drawtrianglepen);
    p.drawPolygon(points,  3);

    // 绘制箭头身体
    int offsetx = int(par * siny / 3);
    int offsety = int(par * cosy / 3);
    QPoint point3 = QPoint(
        endpoint.x() + int(-par * cosy - (par / 2.0 * siny)) + offsetx,
        endpoint.y() + int(-par * siny + (par / 2.0 * cosy)) - offsety
        );
    QPoint point4 = QPoint(
        endpoint.x() + int(-par * cosy + (par / 2.0 * siny) - offsetx),
        endpoint.y() - int(par / 2.0 * cosy + par * siny) + offsety
        );
    QPoint arrbodypoints[3] = { startpoint, point3, point4 };
    p.drawPolygon(arrbodypoints,  3);
}

// 设置显示图像
void QSLabel::setimagetolabel(const QImage &image)
{
    selectimage = image;
    QPixmap pix = QPixmap::fromImage(image);
    pix.setDevicePixelRatio(devicePixelRatioF());
    this->setPixmap(pix);

    // 重置绘制状态
    endPoint = QPoint(0, 0);
    startPoint = QPoint(0, 0);
}

// 以下为各种绘图模式的设置方法（实现逻辑相同，仅调整格式）
void QSLabel::setdrawlineenable() {
    settextedittovector();
    isdrawline = true;
    isdrawrectangle = isdrawround = istextedit = isdrawarrow = isreturnedit = false;
    m_plaintextedit->hide();
}

void QSLabel::setrectangleenable() {
    settextedittovector();
    isdrawrectangle = true;
    isdrawline = isdrawround = istextedit = isdrawarrow = isreturnedit = false;
    m_plaintextedit->hide();
}

void QSLabel::setdrawarrowenable() {
    settextedittovector();
    isdrawarrow = true;
    isdrawline = isdrawrectangle = isdrawround = istextedit = isreturnedit = false;
    m_plaintextedit->hide();
}

void QSLabel::setroundenable() {
    settextedittovector();
    isdrawround = true;
    isdrawline = isdrawrectangle = istextedit = isdrawarrow = isreturnedit = false;
    m_plaintextedit->hide();
}

void QSLabel::settexteditenable() {
    settextedittovector();
    istextedit = true;
    isdrawline = isdrawrectangle = isdrawround = isdrawarrow = isreturnedit = false;
    m_plaintextedit->hide();
}

// 保存当前文本编辑内容
void QSLabel::settextedittovector()
{
    if (istextedit && !m_plaintextedit->toPlainText().isEmpty()) {
        myText *text = new myText;
        text->mRect = QRect(
            QPoint(m_plaintextedit->x(), m_plaintextedit->y()),
            QSize(m_plaintextedit->width(), m_plaintextedit->height())
            );
        text->mText = m_plaintextedit->toPlainText();
        text->isSelected = false;
        texts.push_back(text);
        update();
    }
}

// 撤销上一步操作
void QSLabel::setReturnEditEnable() {
    isreturnedit = true;
    if (!actionVec.empty())  {
        QString str = actionVec.back();
        actionVec.pop_back();
        if (str == "rectangles") {
            rectangles.pop_back();
        } else if (str == "rounds") {
            rounds.pop_back();
        } else if (str == "arrows") {
            arrows.pop_back();
        }
    }
    m_plaintextedit->hide();
}

// 获取合成后的结果图像
QImage QSLabel::resultimage()
{
    //获取设备像素比
    qreal dpr = devicePixelRatioF();
    if (qFuzzyIsNull(dpr) || dpr < 1.0) dpr = 1.0;
    QSize physicalSize = selectimage.size()  * dpr;

    QPixmap combinedPixmap(physicalSize);
    combinedPixmap.fill(Qt::transparent);
    combinedPixmap.setDevicePixelRatio(dpr);  //设置DPI缩放

    QPainter painter(&combinedPixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 绘制原始图像
    painter.drawPixmap(0,  0, QPixmap::fromImage(selectimage));
    //坐标系转换（逻辑→物理）
    painter.scale(dpr,  dpr); // ✅ 关键转换
    // 绘制所有图形元素
    QPen pen(Qt::red, 2* dpr);// ✅ 关键转换
    painter.setPen(pen);

    for (auto* line : lines) {
        painter.drawLine(line->startPoint,  line->endPoint);
    }

    for (auto* rect : rectangles) {
        int x = std::min(rect->startPoint.x(), rect->endPoint.x());
        int y = std::min(rect->startPoint.y(), rect->endPoint.y());
        painter.drawRect(x,  y,
                         abs(rect->endPoint.x() - rect->startPoint.x()),
                         abs(rect->endPoint.y() - rect->startPoint.y()));
    }

    for (auto* round : rounds) {
        int x = std::min(round->startPoint.x(), round->endPoint.x());
        int y = std::min(round->startPoint.y(), round->endPoint.y());
        painter.drawEllipse(x,  y,
                            abs(round->endPoint.x() - round->startPoint.x()),
                            abs(round->endPoint.y() - round->startPoint.y()));
    }

    for (auto* arrow : arrows) {
        drawarrow(arrow->startPoint, arrow->endPoint, painter);
    }

    for (auto* text : texts) {
        QFont font = painter.font();
        font.setPixelSize(14  * dpr); // ✅ 字体物理像素适配
        painter.setFont(font);
        painter.drawText(text->mRect,  Qt::TextWrapAnywhere, text->mText);
    }

    // 计算裁剪尺寸（防越界）
    int targetW = qMin(
        combinedPixmap.width(),
        qRound(combinedPixmap.width()  / dpr)
        );
    int targetH = qMin(
        combinedPixmap.height(),
        qRound(combinedPixmap.height()  / dpr)
        );

    // 执行精准裁剪
    QRect cutArea(0, 0, targetW, targetH);
    QPixmap croppedPix = combinedPixmap.copy(cutArea);

    return croppedPix.toImage();
}

// 判断鼠标是否在文本矩形框边缘附近
bool QSLabel::isNearTextEdge(const QPoint &point, myText *text, int &edge)
{
    const int edgeThreshold = 8; // 边缘检测阈值
    QRect rect = text->mRect;

    // 检测上边缘
    if (qAbs(point.y() - rect.top())  <= edgeThreshold &&
        point.x() >= rect.left()  && point.x() <= rect.right())  {
        edge = 1; // 上边缘
        return true;
    }

    // 检测下边缘
    if (qAbs(point.y() - rect.bottom())  <= edgeThreshold &&
        point.x() >= rect.left()  && point.x() <= rect.right())  {
        edge = 2; // 下边缘
        return true;
    }

    // 检测左边缘
    if (qAbs(point.x() - rect.left())  <= edgeThreshold &&
        point.y() >= rect.top()  && point.y() <= rect.bottom())  {
        edge = 3; // 左边缘
        return true;
    }

    // 检测右边缘
    if (qAbs(point.x() - rect.right())  <= edgeThreshold &&
        point.y() >= rect.top()  && point.y() <= rect.bottom())  {
        edge = 4; // 右边缘
        return true;
    }

    edge = 0;
    return false;
}

// 根据文本矩形框边缘位置更新鼠标样式
void QSLabel::updateCursorForTextResize(const QPoint &point)
{
    // 只在不绘图模式下检测
    if (isdrawline || isdrawrectangle || isdrawround || istextedit || isdrawarrow) {
        return;
    }

    int edge = 0;
    for (auto* text : texts) {
        if (isNearTextEdge(point, text, edge)) {
            switch (edge) {
            case 1: // 上边缘
            case 2: // 下边缘
                setCursor(Qt::SizeVerCursor);
                return;
            case 3: // 左边缘
            case 4: // 右边缘
                setCursor(Qt::SizeHorCursor);
                return;
            }
        }
    }

    // 没有在边缘则恢复默认光标
    setCursor(Qt::ArrowCursor);
}

// 调整文本矩形框大小
void QSLabel::resizeTextRect(myText *text, const QPoint &delta, int edge)
{
    QRect newRect = originalResizeRect/*text->mRect*/;

    switch (edge) {
    case 1: // 上边缘
        newRect.setTop(newRect.top()  + delta.y());
        // 确保高度不小于最小值
        if (newRect.height()  < 20) {
            newRect.setTop(newRect.bottom()  - 20);
        }
        break;
    case 2: // 下边缘
        newRect.setBottom(newRect.bottom()  + delta.y());
        if (newRect.height()  < 20) {
            newRect.setBottom(newRect.top()  + 20);
        }
        break;
    case 3: // 左边缘
        newRect.setLeft(newRect.left()  + delta.x());
        if (newRect.width()  < 50) {
            newRect.setLeft(newRect.right()  - 50);
        }
        break;
    case 4: // 右边缘
        newRect.setRight(newRect.right()  + delta.x());
        if (newRect.width()  < 50) {
            newRect.setRight(newRect.left()  + 50);
        }
        break;
    }

    text->mRect = newRect;
}

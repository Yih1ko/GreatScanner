#include "imgeditor.h"
#include "movepointcommand.h"
#include "rotateimgcommand.h"
#include "ui_imgeditor.h"

void cropImage(const std::string& inputPath
               , const std::string& outputPath
               , const std::vector<cv::Point2f>& cvCropPoints) {
    // 读取输入图像
    cv::Mat src = cv::imread(inputPath);
    if (src.empty()) {
        throw std::runtime_error("无法读取输入图像");
    }

    // 转换为BGRA格式
    cv::Mat srcBgra;
    cv::cvtColor(src, srcBgra, cv::COLOR_BGR2BGRA);

    // 确定正方形的边长
    int width = src.cols;
    int height = src.rows;
    int squareSize = std::max(width, height);

    // 定义目标点的位置
    std::vector<cv::Point2f> dstPoints = {
        cv::Point2f(0, 0),
        cv::Point2f(squareSize, 0),
        cv::Point2f(squareSize, squareSize),
        cv::Point2f(0, squareSize)
    };

    // 确保输入点和目标点的类型一致
    if (cvCropPoints.size() != 4 || dstPoints.size() != 4) {
        throw std::runtime_error("输入和目标点的数量必须为4个");
    }

    // 计算透视变换矩阵
    cv::Mat perspectiveMatrix = cv::findHomography(cvCropPoints, dstPoints, cv::RANSAC, 3);
    if (perspectiveMatrix.empty()) {
        throw std::runtime_error("无法计算透视变换矩阵");
    }

    // 创建结果图像并初始化Alpha通道为0
    cv::Mat resultBgra(squareSize, squareSize, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    // 应用透视变换
    cv::warpPerspective(srcBgra, resultBgra, perspectiveMatrix, cv::Size(squareSize, squareSize));

    // 保存结果图像
    if (!cv::imwrite(outputPath, resultBgra)) {
        throw std::runtime_error("无法保存输出图像");
    } else {
        qDebug() << "已成功保存!";
    }
}

void RotatePoints(std::vector<cv::Point2f>& cvPoints,
                  float rotationAngle,
                  const std::string& imagePath) {
    // 将角度从度数转换为弧度
    float theta = rotationAngle * (M_PI / 180.0f);

    // 读取图片以获取尺寸
    cv::Mat image = cv::imread(imagePath);
    if (image.empty()) {
        std::cerr << "Error: Could not open or find the image!" << std::endl;
        return;
    }

    // 计算背景图的中心坐标
    int width = image.cols;
    int height = image.rows;
    float centerX = width / 2.0f;
    float centerY = height / 2.0f;

    // 计算cosθ和sinθ
    float cosTheta = cos(theta);
    float sinTheta = sin(theta);

    // 遍历每个点并调整其坐标
    for (auto& point : cvPoints) {
        // 计算相对于背景图中心的偏移量
        float dx = point.x - centerX;
        float dy = point.y - centerY;

        // 应用旋转变换矩阵
        float newDx = dx * cosTheta - dy * sinTheta;
        float newDy = dx * sinTheta + dy * cosTheta;

        // 计算旋转后的绝对坐标
        point.x = newDx + centerX;
        point.y = newDy + centerY;
    }
}


ImgEditor::ImgEditor(QWidget *parent, QString imgPath)
    : QWidget(parent)
    , ui(new Ui::ImgEditor)
    , m_scene(new QGraphicsScene(this))
    , m_scaleFactor(1.0) // 初始缩放
    ,m_lastDialValue(-1) // 初始化为非法值
    ,m_currentScale(1.0)
    // , m_currentXScale(1.0)
    // , m_currentYScale(1.0)
    , m_imgPath(imgPath)
{
    ui->setupUi(this);

    // 去除边框并设置为顶层窗口
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    // 启用透明背景
    setAttribute(Qt::WA_TranslucentBackground);

    // 连接QUndoStack的信号到按钮的槽
    connect(UndoStackSingleton::GetInstance().getUndoStack(), &QUndoStack::canUndoChanged, this, &ImgEditor::updateUndoButtonState);
    connect(UndoStackSingleton::GetInstance().getUndoStack(), &QUndoStack::canRedoChanged, this, &ImgEditor::updateRedoButtonState);

    //初始化按钮状态
    updateUndoButtonState();
    updateRedoButtonState();

    // 初始化图形视图
    ui->graphicsView->setScene(m_scene);

    // 设置场景背景
    m_scene->setBackgroundBrush(QColor(50, 50, 50));
    // 加载图片并创建QGraphicsPixmapItem
    QPixmap backgroundPixmap(imgPath);
    if (!backgroundPixmap.isNull()) {
        //根据图片设定场景大小
        m_scene->setSceneRect(0, 0, backgroundPixmap.width(), backgroundPixmap.height());
        ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        backgroundItem = new QGraphicsPixmapItem(backgroundPixmap);

        // 计算中心偏移
        const QRectF sceneRect = m_scene->sceneRect();
        const QSize pixmapSize = backgroundPixmap.size();

        // 设置图元位置
        backgroundItem->setPos(
            sceneRect.center().x() - pixmapSize.width() / 2.0,
            sceneRect.center().y() - pixmapSize.height() / 2.0
            );

        m_scene->addItem(backgroundItem);
        backgroundItem->setZValue(-1);
        //图片变换中心
        backgroundItem->setTransformOriginPoint(backgroundPixmap.width() / 2, backgroundPixmap.height() / 2);
        //get图片中心
        imageCenter = backgroundPixmap.rect().center();
    } else {
        qWarning() << "Failed to load background image!";
    }

    // 创建并添加四边形项
    quad = new Frame();
    m_scene->addItem(quad);
    // 居中显示
    quad->setPos(m_scene->sceneRect().center());
    // 将视图中心对准场景中心
    ui->graphicsView->centerOn(quad);

    connect(quad, &Frame::geometryChanged, [](const QPolygonF& poly) {
        Q_UNUSED(poly)
        //qDebug() << "Frame updated to:" << poly;
    });

    // 监听点选择状态变化
    connect(quad, &Frame::pointSelectionChanged,
            this, [this](PointItem* point) {
                onPointSelected(point, true); // 手动传递状态参数
            });

    // 在初始化view_scale后设置初始值
    m_lastDialValue = ui->view_scale->value();

    // 连接信号时需要进行参数类型转换（重要！）
    connect(ui->view_scale, &InfiniteDial::valueChanged,
            this, [this](int value) {
                viewScale(value);
            });

    connect(ui->rBtn, &CustomToolButton::longPressed, this, [=]() {
        // 处理长按事件
        qDebug() << "Long press detected!";
        on_rBtn_clicked();
    });
    connect(ui->lBtn, &CustomToolButton::longPressed, this, [=]() {
        // 处理长按事件
        qDebug() << "Long press detected!";
        on_lBtn_clicked();
    });
    connect(ui->upBtn, &CustomToolButton::longPressed, this, [=]() {
        // 处理长按事件
        qDebug() << "Long press detected!";
        on_upBtn_clicked();
    });
    connect(ui->downBtn, &CustomToolButton::longPressed, this, [=]() {
        // 处理长按事件
        qDebug() << "Long press detected!";
        on_downBtn_clicked();
    });
}

ImgEditor::~ImgEditor()
{
    delete ui;
    delete m_scene;
}

void ImgEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 开启抗锯齿

    QPainterPath path;
    int radius = 10; // 圆角半径
    QRect rect = this->rect().adjusted(0, 0, -1, -1);
    path.addRoundedRect(rect,  radius, radius);

    // 设置背景颜色（带透明度）
    QBrush brush(QColor(255, 255, 255, 170)); // 白色半透明
    painter.fillPath(path,  brush);

    //绘制边框（可选）
    QPen pen(QColor(200, 200, 200), 1);
    painter.strokePath(path,  pen);
}

void ImgEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_offset = event->pos();
    }
}

void ImgEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        move(event->globalPos() - m_offset);
    }
}

void ImgEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
    }
}

void ImgEditor::onPointSelected(PointItem *item, bool state)
{
    if (state) {
        m_selectedPoint = item;
        qDebug() << "Point selected at:" << item->getPoint();
    } else {
        if(m_selectedPoint == item) { // 同一项取消选择时才清除
            m_selectedPoint = nullptr;
        }
    }
}

void ImgEditor::moveSelectedPoint(qreal dx, qreal dy)
{
    if(m_selectedPoint && m_selectedPoint->scene()) {
        // 记录旧位置
        QPointF oldPos = m_selectedPoint->getPoint();

        // 计算新位置
        QPointF newPos = oldPos + QPointF(dx, dy);

        // 创建并执行命令
        MovePointCommand *command = new MovePointCommand(m_selectedPoint, oldPos, newPos, nullptr);
        UndoStackSingleton::GetInstance().getUndoStack()->push(command);

        // 更新点的位置
        m_selectedPoint->setPoint(newPos);

        // 结束移动后检查形状
        quad->handleDragEnd();
    }
}

void ImgEditor::updateUndoButtonState()
{
    ui->undoBtn->setEnabled(UndoStackSingleton::GetInstance().getUndoStack()->canUndo());
}

void ImgEditor::updateRedoButtonState()
{
    ui->redoBtn->setEnabled(UndoStackSingleton::GetInstance().getUndoStack()->canRedo());
}


void ImgEditor::on_undoBtn_clicked() {
    UndoStackSingleton::GetInstance().getUndoStack()->undo();
}

void ImgEditor::on_redoBtn_clicked() {
    UndoStackSingleton::GetInstance().getUndoStack()->redo();
}


void ImgEditor::on_upBtn_clicked()
{
    moveSelectedPoint(0, -0.5);
}


void ImgEditor::on_downBtn_clicked()
{
    moveSelectedPoint(0, 0.5);
}


void ImgEditor::on_lBtn_clicked()
{
    moveSelectedPoint(-0.5, 0);
}


void ImgEditor::on_rBtn_clicked()
{
    moveSelectedPoint(0.5, 0);
}

// 视图缩放函数实现
void ImgEditor::viewScale(int value) {
    // 第一次调用时初始化初始值
    if (m_lastDialValue == -1) {
        m_lastDialValue = value;
        return;
    }

    // 处理数值回绕问题
    int delta = value - m_lastDialValue;
    if (delta > 300) { // 反向环回情况（比如350->10）
        delta -= 360;
    } else if (delta < -300) { // 正向环回情况（比如10->350）
        delta += 360;
    }

    // 计算缩放增量
    double scaleFactorChange = 1 + delta * SCALE_FACTOR_PER_DEGREE;

    // 获取当前视图的变换矩阵
    QTransform currentTransform = ui->graphicsView->transform();
    double currentScale = currentTransform.m11(); // 当前缩放因子

    // 更新缩放因子
    currentScale *= scaleFactorChange;

    // 设定缩放范围限制
    currentScale = qBound(MIN_SCALE, currentScale, MAX_SCALE);

    // 创建新的变换矩阵
    QTransform newTransform;
    newTransform.scale(currentScale, currentScale);

    // 保持视图中心点不变
    QPointF viewCenter = ui->graphicsView->viewport()->rect().center();
    QPointF sceneCenterBefore = ui->graphicsView->mapToScene(viewCenter.toPoint());

    // 应用新的变换矩阵
    ui->graphicsView->setTransform(newTransform);

    // 计算中心点偏移并调整视图位置
    QPointF sceneCenterAfter = ui->graphicsView->mapToScene(viewCenter.toPoint());
    QPointF centerOffset = sceneCenterBefore - sceneCenterAfter;
    ui->graphicsView->translate(centerOffset.x(), centerOffset.y());

    // 更新记录值
    m_lastDialValue = value;
}

//裁剪并导出
void ImgEditor::handleCrop() {
    // 先得到4个点在图片中的像素位置储存在cvCropPoints
    std::vector<cv::Point2f> cvCropPoints;
    auto points = quad->controlPoints();

    for (PointItem* point : points) {
        QPointF scenePos = point->mapToScene(QPointF(0, 0)); // 获取PointItems在场景中的位置
        cvCropPoints.push_back(cv::Point2f(scenePos.x(), scenePos.y()));
    }

    // 获取背景图的中心坐标
    QRectF sceneRect = m_scene->sceneRect();
    QPointF backgroundCenter(
        sceneRect.center().x(),
        sceneRect.center().y()
        );

    // 传递背景图的中心坐标到RotatePoints函数
    RotatePoints(cvCropPoints
                 , -ui->rotate->value()/*角度传相反数因为背景转45度，对应Frame(前景)的角度是-45度*/
                 , m_imgPath.toStdString());

    // 调用裁剪函数
    cropImage(
        m_imgPath.toStdString(),
        imgEditor_output_path.toStdString(),
        cvCropPoints
        );
}

// 正值为图片向右旋转，负值为向左
void ImgEditor::on_rotate_valueChanged(int value) {
    //更新当前值
    backgroundItem->setRotation(value);
}

// OK!
void ImgEditor::on_OKBtn_clicked()
{
    handleCrop();
}

//旋转滑块按下
void ImgEditor::on_rotate_sliderPressed()
{
    //记录旧值
    m_startAngle = ui->rotate->value();
}


void ImgEditor::on_rotate_sliderReleased()
{
    // 获取新值
    int newAngle = ui->rotate->value();

    // 创建撤销命令
    RotateImgCommand* command = new RotateImgCommand(ui->rotate, m_startAngle, newAngle);

    // 推送命令到撤销堆栈
    UndoStackSingleton::GetInstance().getUndoStack()->push(command);

    // 更新背景项的旋转角度
    backgroundItem->setRotation(newAngle);
}

void ImgEditor::on_closeBtn_clicked()
{
    close();
}


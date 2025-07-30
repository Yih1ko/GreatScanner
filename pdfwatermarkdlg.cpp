#include "pdfwatermarkdlg.h"
#include "ui_pdfwatermarkdlg.h"
#include <QPdfDocument>
#include <QMessageBox>
#include <QFileDialog>
#include <QPdfPageNavigator>
#include <QPainter>
#include <QStandardPaths>
#include <QScreen>
#include <QTransform>
#include <QPdfPageRenderer>
#include <QPdfWriter>
#include <QProgressDialog>

PdfWaterMarkDlg::PdfWaterMarkDlg(QWidget *parent, QString docPath)
    : QWidget(parent, Qt::Window)
    , ui(new Ui::PdfWaterMarkDlg)
    , m_pdfDocument(new QPdfDocument(this)) // 初始化PDF文档
    ,m_pageRenderer(new QPdfPageRenderer(this)) // 初始化渲染器
    ,m_docPath(docPath)
{
    ui->setupUi(this);
    m_pageRenderer->setDocument(m_pdfDocument); // 绑定文档
    m_pageRenderer->setRenderMode(QPdfPageRenderer::RenderMode::MultiThreaded); // 启用多线程

    initUI();
    initSlots();
}

PdfWaterMarkDlg::~PdfWaterMarkDlg()
{
    delete ui;
}

void PdfWaterMarkDlg::clearMarks() {
    if (m_currentMark.m_text.isEmpty())  return; // 已无水印时跳过
    m_currentMark = MarkMeta(); // 重置水印数据

    // 强制清除预览图片并刷新UI
    if (m_imageLabel) {
        m_imageLabel->clear();  // 立即清空当前显示的图片
        m_imageLabel->update(); // 触发重绘事件
    }

    // 重新渲染无水印预览（异步）
    if (m_pdfDocument->pageCount() > 0) {
        QSizeF pageSize = m_pdfDocument->pagePointSize(0);
        m_pageRenderer->requestPage(0, QSize(pageSize.width()  * 2, pageSize.height()  * 2));
    }
}

void PdfWaterMarkDlg::initUI()
{
    //确保父容器有布局
        if (!ui->previewWidget->layout()) {
        QVBoxLayout *layout = new QVBoxLayout(ui->previewWidget);
        layout->setContentsMargins(0, 0, 0, 0);  // 移除边距
    }

    // 仅保留图像显示区域
    m_imageLabel = new QLabel(this);
    m_imageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_imageLabel->setAlignment(Qt::AlignCenter);
    m_imageLabel->setStyleSheet("background-color: #f0f0f0; border: 1px solid #cccccc;");

    ui->previewWidget->layout()->addWidget(m_imageLabel);
    loadPdf();
    // 添加水印编辑器
    m_markEditor = new MarkEditor(this);
    m_markEditor->setAttribute(Qt::WA_DeleteOnClose); // 关闭时自动销毁
    m_markEditor->hide();
}

void PdfWaterMarkDlg::initSlots()
{
    QObject::connect(ui->savePdf, QOverload<>::of(&ClickableLabel::clicked), this, &PdfWaterMarkDlg::savePdf);
    QObject::connect(m_markEditor, &MarkEditor::markMeta, this, [this](MarkMeta data){
        this->updateMark(data);
    });
    QObject::connect(ui->editMark, QOverload<>::of(&ClickableLabel::clicked), this, [this](){
        m_markEditor->show();
    });
    QObject::connect(ui->cleanMark, QOverload<>::of(&ClickableLabel::clicked), this, &PdfWaterMarkDlg::clearMarks);
    // 连接页面渲染完成的信号
    connect(m_pageRenderer, &QPdfPageRenderer::pageRendered, this, &PdfWaterMarkDlg::handlePageRendered);
}


void PdfWaterMarkDlg::loadPdf() {
    if (!QFile::exists(m_docPath)) {
        QMessageBox::warning(this, "文件错误", "PDF文件不存在!");
        return;
    }

    QPdfDocument::Error error = m_pdfDocument->load(m_docPath);
    if (error != QPdfDocument::Error::None) {
        QMessageBox::warning(this, "加载错误",
                             "无法加载PDF文档: "/* + m_pdfDocument->error()*/);
        return;
    }

    // 请求渲染首页用于预览
    if (m_pdfDocument->pageCount() > 0) {
        // 获取屏幕DPI缩放比例
        qreal dpiScale = this->devicePixelRatioF();

        // 计算高分辨率尺寸 (DPI感知)
        if (m_pdfDocument->pageCount() > 0) {
            QSizeF pageSize = m_pdfDocument->pagePointSize(0);
            QSize renderSize(
                static_cast<int>(pageSize.width()  * dpiScale * 2),
                static_cast<int>(pageSize.height()  * dpiScale * 2)
                );
            m_pageRenderer->requestPage(0, renderSize);
        }
    }
}

void PdfWaterMarkDlg::renderWatermarkedPreview() {
    if (!m_imageLabel || !m_pageCache.contains(0))  return;
    // 直接使用缓存图像副本
    QImage resultImage = m_pageCache.value(0).copy();
    qreal dpiScale = m_imageLabel->devicePixelRatioF();
    QSizeF pageSize = m_pdfDocument->pagePointSize(0);
    QPainter painter(&resultImage);
    painter.setRenderHints(QPainter::Antialiasing  | QPainter::TextAntialiasing);
    if (!m_currentMark.m_text.isEmpty())  {
        // 精确坐标转换（避免双重缩放）
        painter.save();
        painter.scale(
            resultImage.width()  / pageSize.width(),
            resultImage.height()  / pageSize.height()
            );
        renderWatermarkToPainter(painter, pageSize);
        painter.restore();
    }
    // 设置设备像素比
    resultImage.setDevicePixelRatio(dpiScale);
    m_imageLabel->setPixmap(QPixmap::fromImage(resultImage));
}

void PdfWaterMarkDlg::renderWatermarkToPainter(QPainter &painter, const QSizeF &pageSize)
{
    if (m_currentMark.m_text.isEmpty())  return;

    QFont font(m_currentMark.m_fontFamily, m_currentMark.m_size);
    font.setHintingPreference(QFont::PreferFullHinting);  // 增强文本清晰度
    font.setStyleStrategy(QFont::PreferAntialias);
    font.setBold(true);

    // 使用浮点精度字体测量
    QFontMetricsF fontMetrics(font);
    QRectF textRect = fontMetrics.boundingRect(m_currentMark.m_text);

    // 计算旋转参数
    const qreal angle = -45.0;
    const qreal radians = qDegreesToRadians(angle);

    // 计算旋转后实际占位区域
    const qreal rotatedWidth = textRect.width()  * qAbs(qCos(radians))
                               + textRect.height()  * qAbs(qSin(radians));
    const qreal rotatedHeight = textRect.width()  * qAbs(qSin(radians))
                                + textRect.height()  * qAbs(qCos(radians));

    // 动态步长计算（基础步长+安全间距）
    const qreal stepBase = qMax(rotatedWidth, rotatedHeight) * 1.5/* * 1.2*/;

    // 设置绘图参数
    painter.save();
    painter.setFont(font);
    painter.setPen(QColor(m_currentMark.m_color));
    painter.setOpacity(m_currentMark.m_opacity  / 100.0);
    painter.translate(pageSize.width()  / 2, pageSize.height()  / 2);
    painter.rotate(angle);

    // 交错布局参数
    qreal rowOffset = 0;
    bool alternateRow = false;

    // 计算覆盖范围
    const qreal rangeX = pageSize.width()  ;
    const qreal rangeY = pageSize.height() ;

    // 交错式循环绘制
    for (qreal y = -rangeY; y < rangeY; y += stepBase) {
        for (qreal x = -rangeX; x < rangeX; x += stepBase) {
            QPointF pos(x + rowOffset, y);
            painter.drawText(pos,  m_currentMark.m_text);
        }
        // 切换行偏移量
        alternateRow = !alternateRow;
        rowOffset = alternateRow ? stepBase/2 : 0;
    }

    painter.restore();
}

// int PdfWaterMarkDlg::getSourceDpiFromMuPdf(const QString& path) {

// }

void PdfWaterMarkDlg::updateMark(MarkMeta data)
{
    //应用MarkMeta中的参数来更新预览
    m_currentMark = data;
    renderWatermarkedPreview(); // 渲染预览
}

void PdfWaterMarkDlg::savePdf()
{
    QString savePath = QFileDialog::getSaveFileName(
        this, "保存PDF文件",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) +
            "/" + QFileInfo(m_docPath).baseName() + "_watermarked.pdf",
        "PDF文件 (*.pdf)"
        );
    if (savePath.isEmpty())
        return;
    QPdfWriter pdfWriter(savePath);
    pdfWriter.setCreator("PDF Watermark Tool");
    pdfWriter.setResolution(300);
    QProgressDialog progress("正在保存PDF...", "取消", 0, m_pdfDocument->pageCount(), this);
    progress.setMinimumDuration(0);
    QPainter painter(&pdfWriter);
    if (!painter.isActive())  {
        QMessageBox::critical(this, "错误", "无法创建PDF文件，请检查路径是否有效。");
        return;
    }
    const int pageCount = m_pdfDocument->pageCount();
    // 统一使用设备DPI比例
    const qreal dpiScale = this->devicePixelRatioF();
    const int renderDpi = qRound(300 * dpiScale); // 根据屏幕DPI缩放渲染分辨率
    for (int pageIndex = 0; pageIndex < pageCount; ++pageIndex) {
        progress.setValue(pageIndex);
        if (progress.wasCanceled())  {
            painter.end();
            QFile::remove(savePath);
            return;
        }
        QSizeF pageSize = m_pdfDocument->pagePointSize(pageIndex);
        if (pageSize.isEmpty())  continue;
        pdfWriter.setPageSize(QPageSize(pageSize, QPageSize::Point));
        if (pageIndex > 0) {
            pdfWriter.newPage();
        }
        painter.resetTransform();
        // Render to image
        QSize imageSize(static_cast<int>(pageSize.width() * renderDpi / 72.0),
                        static_cast<int>(pageSize.height() * renderDpi / 72.0));
        QImage pageImage = m_pdfDocument->render(pageIndex, imageSize);
        if (!pageImage.isNull()) {
            painter.drawImage(QRectF(0, 0, pageSize.width(), pageSize.height()),
                              pageImage,
                              QRectF(0, 0, pageImage.width(), pageImage.height()));
        }
        // Add watermark
        if (!m_currentMark.m_text.isEmpty())  {
            painter.save();
            painter.setOpacity(m_currentMark.m_opacity / 100.0);
            renderWatermarkToPainter(painter, pageSize);
            painter.restore();
        }
        QCoreApplication::processEvents();
    }
    painter.end();
    progress.setValue(pageCount);
    if (QFile::exists(savePath)) {
        QMessageBox::information(this, "保存成功",
                                 QString("文件已保存:\n%1\n大小: %2 KB")
                                     .arg(savePath)
                                     .arg(QFileInfo(savePath).size() / 1024));
    } else {
        QMessageBox::critical(this, "保存失败", "文件保存失败，请检查磁盘空间和权限。");
    }
}

void PdfWaterMarkDlg::handlePageRendered(int pageNumber, QSize size, const QImage &image) {
    if (pageNumber != 0) return; // 只缓存预览页
    m_pageCache[pageNumber] = image.copy();  // 存储原始图像的独立副本

    if (pageNumber == 0) {
        renderWatermarkedPreview(); // 立即触发预览更新
        if (m_imageLabel) m_imageLabel->update(); // 强制UI刷新
    }
}



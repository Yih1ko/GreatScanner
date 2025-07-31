#include "srcselector.h"
#include "Def.h"
#include <QProcess>
#include "ExcelReconstructor.h"
#include "LayoutReconstructor.h"
#include "main_window.h"
#include "ui_srcselector.h"
#include <QGraphicsDropShadowEffect>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QImageCapture>
#include <QMediaDevices>
#include <QStandardPaths>
#include <QVideoSink>
#include "ocrclient.h"
#include "ocrtextdlg.h"
#include <QTemporaryFile>
#include <qjsondocument.h>
#include "cvutils.h"

SrcSelector::SrcSelector(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SrcSelector)
{
    // 设置为独立窗口
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    //setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时对象销毁
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);

    // 阴影效果设置
    auto imgShadow = new QGraphicsDropShadowEffect(this);
    imgShadow->setBlurRadius(15);
    imgShadow->setColor(Qt::darkGray);
    ui->ImportImg->setGraphicsEffect(imgShadow);

    auto okShadow = new QGraphicsDropShadowEffect(this);
    okShadow->setBlurRadius(15);
    okShadow->setColor(Qt::darkGray);
    ui->OK->setGraphicsEffect(okShadow);

    // 初始化摄像头
    initCamera();

    // 连接信号
    connect(ui->ImportImg, QOverload<>::of(&ClickableLabel::clicked), [this]{
        QString filePath = QFileDialog::getOpenFileName(
            this,
            tr("选择图片"),
            QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),//日后为配置项
            tr("图片文件 (*.png *.jpg *.jpeg *.bmp)")
            );

        if (!filePath.isEmpty()) {
            m_path = filePath;
            m_funcMap[m_mod]();
            this->hide();
        }
    });

    connect(ui->OK, QOverload<>::of(&ClickableLabel::clicked), this, &SrcSelector::captureImage);
    connect(OcrClient::GetInstance().get(), &OcrClient::sig_RespOcrRes, this, &SrcSelector::dlgGate, Qt::UniqueConnection);//打开对应Dlg
    if (m_imageCapture) {
        connect(m_imageCapture, &QImageCapture::imageCaptured,
                this, &SrcSelector::handleImageCaptured);
    }
    //注册回调
    registFuncs();
    SET_MICA_BACKGROUND(this);
}

SrcSelector::~SrcSelector()
{
    // 断开与OcrClient的信号连接
    disconnect(OcrClient::GetInstance().get(), nullptr, this, nullptr);
    if (m_camera) {
        m_camera->stop();
        delete m_camera;
    }

    if (m_captureSession) {
        delete m_captureSession;
    }

    delete ui;
}

void SrcSelector::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}

void SrcSelector::setMod(ipc::IpcId mod)
{
    m_mod = mod;
}

void SrcSelector::captureImage()
{
    if (!m_camera || m_isCapturing || !m_imageCapture) return;

    if (!m_camera->isActive()) {
        m_camera->start();
    }

    m_isCapturing = true;
    ui->OK->setEnabled(false); // 禁用拍照按钮防止重复点击
    m_imageCapture->capture();
}

void SrcSelector::handleImageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id);
    m_capturedImage = preview;
    saveCapturedImage();
}

void SrcSelector::handleCameraError(QCamera::Error error)
{
    Q_UNUSED(error);
    ui->OK->setEnabled(false);
    ui->OK->setToolTip(tr("摄像头错误: %1").arg(m_camera->errorString()));
}

void SrcSelector::dlgGate(QByteArray data)
{
    qDebug()<<"mod是"<<m_mod;
    if(m_mod == ipc::IpcId::Ocr_Default){
        OcrTextDlg* dlg = new OcrTextDlg(m_mainWindow);
        dlg->show();
        dlg->showOcrRes(data);
    }else if(m_mod == ipc::IpcId::Ocr_Word){
        //选择路径
        QString defaultName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".docx";
        QString savePath = QFileDialog::getSaveFileName(m_mainWindow,
                                                        tr("Save OCR Document"),
                                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + defaultName,
                                                        tr("Word Documents (*.docx)"));

        if (savePath.isEmpty())  return;
        //生成docx
        LayoutReconstructor res;
        res(nlohmann::json::parse(data.constData(),  data.constData()  + data.size()), savePath.toStdString());
    }else if(m_mod == ipc::Ocr_Execl){
        //选择路径
        QString defaultName = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".xlsx";
        QString savePath = QFileDialog::getSaveFileName(m_mainWindow,
                                                        tr("Save OCR Table"),
                                                        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + defaultName,
                                                        tr("Excel Tables (*.xlsx)"));

        if (savePath.isEmpty())  return;
        //生成excel-获取表结构
        auto textInfo = nlohmann::json::parse(data.constData(),  data.constData()  + data.size());
        CvUtils::tableStructure structureRec;
        json structureInfo = structureRec(textInfo);
        //std::cout<<structureInfo.dump(2)<<std::endl;
        //生成excel-生成并保存
        ExcelReconstructor res;
        res(structureInfo, savePath.toStdString());
    }
}

void SrcSelector::initCamera()
{
    const QList<QCameraDevice> cameras = QMediaDevices::videoInputs();

    if (cameras.isEmpty()) {
        ui->OK->setEnabled(false);
        ui->OK->setToolTip(tr("未检测到可用摄像头"));
        return;
    }

    // 创建摄像头对象
    m_camera = new QCamera(cameras.first());
    connect(m_camera, &QCamera::errorOccurred, this, &SrcSelector::handleCameraError);

    // 创建图像捕获对象
    m_imageCapture = new QImageCapture(this);

    // 创建媒体捕获会话
    m_captureSession = new QMediaCaptureSession(this);
    m_captureSession->setCamera(m_camera);
    m_captureSession->setImageCapture(m_imageCapture);

    // 创建视频预览组件
    m_videoItem = new QGraphicsVideoItem;
    m_scene = new QGraphicsScene(this);
    m_scene->addItem(m_videoItem);

    // 关键修改：应用水平翻转变换
    QTransform transform;
    transform.scale(-1, 1); // 水平翻转
    m_videoItem->setTransform(transform);
    m_videoItem->setTransformOriginPoint(m_videoItem->boundingRect().center());

    ui->Preview->setScene(m_scene);
    ui->Preview->setRenderHint(QPainter::Antialiasing);
    ui->Preview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->Preview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 关键连接：视频输出到预览组件
    m_captureSession->setVideoOutput(m_videoItem->videoSink());

    // 动态调整预览尺寸
    connect(m_videoItem, &QGraphicsVideoItem::nativeSizeChanged, [this](const QSizeF &size) {
        m_videoItem->setSize(size);
        ui->Preview->fitInView(m_videoItem, Qt::KeepAspectRatio);
    });

    // 启动摄像头预览
    m_camera->start();
}

void SrcSelector::saveCapturedImage()
{
    QString saveDir = "D:\\Qt_Projects\\GreatScanner\\savePictures"/*QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)*/;//日后做成配置项
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString filePath = QString("%1/Capture_%2.png").arg(saveDir).arg(timestamp);

    // 添加水平翻转处理
    QImage flippedImage = m_capturedImage.mirrored(false, false);

    if (flippedImage.save(filePath, "PNG")) {
        m_path = filePath;
        try{
            m_funcMap[m_mod]();//执行回调: {0:普通ocr文字提取, }
        }catch(const std::exception& e){
            qDebug()<<e.what();
        }
        m_camera->stop();
        this->hide();
    } else {
        QMessageBox::warning(this,
                             tr("保存失败"),
                             tr("无法保存图片到: %1").arg(filePath));
        ui->OK->setEnabled(true);
        m_isCapturing = false;
    }
}

void SrcSelector::releaseCameraResources()
{

}

void SrcSelector::registFuncs()
{
    m_funcMap.push_back(
        [this](){
            qDebug()<<"调用ipc::IpcId::Ocr_Default提取文字模块PostOcrTask";
            OcrClient::GetInstance()->PostOcrTask(
                ipc::IpcId::Ocr_Default,
                m_path.toStdString().c_str());
        });
    m_funcMap.push_back(
        [this](){
            qDebug()<<"调用ipc::IpcId::Ocr_Word转Word模块PostOcrTask";
            OcrClient::GetInstance()->PostOcrTask(
                ipc::IpcId::Ocr_Word,
                m_path.toStdString().c_str());
        });
    m_funcMap.push_back(
        [this](){
            qDebug()<<"调用ipc::IpcId::Ocr_Execl转Execl模块PostOcrTask";
            OcrClient::GetInstance()->PostOcrTask(
                ipc::IpcId::Ocr_Execl,
                m_path.toStdString().c_str());
        });
    m_funcMap.push_back(
        [this](){

        });
    m_funcMap.push_back(
        [this](){

        });
    m_funcMap.push_back(
        [this](){

        });
    m_funcMap.push_back(
        [this](){

        });
}

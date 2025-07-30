#ifndef SRCSELECTOR_H
#define SRCSELECTOR_H

#include "Def.h"
#include <QWidget>
#include <QCamera>
#include <QMediaCaptureSession>
#include <QGraphicsVideoItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImageCapture>  // 添加头文件
#include <QVector>
#include <qprocess.h>

namespace Ui {
class SrcSelector;
}
class MainWindow;
class SrcSelector : public QWidget
{
    Q_OBJECT

public:
    explicit SrcSelector(QWidget *parent = nullptr);
    ~SrcSelector();
    // 添加父窗口指针
    void setMainWindow(MainWindow* mainWindow);
    void setMod(ipc::IpcId mod);

signals:
    void getPath(const QString& path);

private slots:
    void captureImage();
    void handleImageCaptured(int id, const QImage &preview);
    void handleCameraError(QCamera::Error error);  // 添加摄像头错误处理
    void dlgGate(QByteArray data);

private:
    Ui::SrcSelector *ui;
    void initCamera();
    void saveCapturedImage();
    void releaseCameraResources();
    QString m_path = "";
    QCamera *m_camera = nullptr;
    QMediaCaptureSession* m_captureSession = nullptr;  // 初始化指针
    QImageCapture *m_imageCapture = nullptr;
    QImage m_capturedImage;
    bool m_isCapturing = false;
    QGraphicsVideoItem *m_videoItem = nullptr;
    QGraphicsScene *m_scene = nullptr;
    std::vector<std::function<void()>> m_funcMap;
    MainWindow* m_mainWindow = nullptr; // 新增成员
    ipc::IpcId m_mod;
    QScopedPointer<QProcess> m_pythonProcess; // 使用智能指针管理生命周期
    void registFuncs();
};

#endif // SRCSELECTOR_H

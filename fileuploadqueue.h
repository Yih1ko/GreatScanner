#ifndef FILEUPLOADQUEUE_H
#define FILEUPLOADQUEUE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QFileInfo>
#include "fileuploader.h"
#include "singleton.h"

//队列上传器
class FileUploadQueue : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(FileUploadQueue)
public:

    ~FileUploadQueue();

    void addFile(quint16 id, const QString &filePath);  // 添加文件到队列

signals:
    void sig_appendTask(quint16 id, const QString &filePath);
    void uploadTotalProgressChanged(int progress);  // 整体进度
    void oneFileUploaded();                         // 一个文件上传完成
    void allFilesUploaded();                        // 所有文件上传完成

private slots:
    void onSingleFileUploaded();  // 单个文件上传完成处理

private:
    QList<std::pair<quint16 ,QString>> m_uploadQueue;      // 文件上传队列(id, 文件路径)
    int m_currentIndex;                // 当前处理的文件索引
    qint64 m_totalSize;                // 所有文件的总大小
    qint64 m_uploadedSize;             // 已上传的总大小
    FileUploader *m_uploader;          // 文件上传器实例

    explicit FileUploadQueue(/*QObject *parent = nullptr*/);
    void startNextUpload();            // 开始下一个文件的上传
};

#endif // FILEUPLOADQUEUE_H

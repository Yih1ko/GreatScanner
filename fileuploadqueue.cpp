#include "fileuploadqueue.h"
#include <QDebug>
#include <QFileInfo>

Q_SINGLETON_CREATE_CPP(FileUploadQueue)

FileUploadQueue::FileUploadQueue(/*QObject *parent*/) :
    // QObject(parent),
    m_currentIndex(-1),
    m_totalSize(0),
    m_uploadedSize(0),
    m_uploader(nullptr)
{
    m_uploader = new FileUploader(this);
    connect(m_uploader, &FileUploader::uploadCompleted, this, &FileUploadQueue::onSingleFileUploaded);
}

FileUploadQueue::~FileUploadQueue()
{
    delete m_uploader;
}

void FileUploadQueue::addFile(quint16 id, const QString &filePath)
{
    // 计算文件大小并添加到队列
    QFileInfo fileInfo(filePath);
    m_totalSize += fileInfo.size();
    m_uploadQueue.append(std::make_pair(id, filePath));

    //信号：新文件上传任务加入
    emit sig_appendTask(id, filePath);

    // 如果当前没有正在上传的文件，则立即开始上传第一个文件
    if (m_currentIndex == -1) {
        startNextUpload();
    }
}

void FileUploadQueue::startNextUpload()
{
    if (m_uploadQueue.isEmpty())  {
        return;
    }

    // 取出队列中的第一个文件
    m_currentIndex = 0;
    auto currentFilePath = m_uploadQueue.takeFirst();

    // 重置已上传大小
    m_uploadedSize = 0;

    // 开始上传当前文件
    m_uploader->uploadFile(currentFilePath.first, currentFilePath.second);
}

void FileUploadQueue::onSingleFileUploaded()
{
    //发送上传了一个文件信号
    emit oneFileUploaded();
    // 更新已上传大小
    QFileInfo fileInfo(m_uploadQueue.value(m_currentIndex).second);
    m_uploadedSize += fileInfo.size();

    // 计算整体进度
    int totalProgress = (m_uploadedSize * 100) / m_totalSize;
    //发射当前整体进度
    emit uploadTotalProgressChanged(totalProgress*0.9);

    // 处理下一个文件
    if (!m_uploadQueue.isEmpty())  {
        startNextUpload();
    } else {
        // 队列为空，发射完成信号
        emit allFilesUploaded();
    }
}

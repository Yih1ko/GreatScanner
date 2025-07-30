#include "fileuploader.h"
#include <QDebug>
#include <QFileInfo>
#include <qeventloop.h>

FileUploader::FileUploader(QObject *parent) : QObject(parent)
{
    m_file = nullptr;
    _seq = 0;
}

FileUploader::~FileUploader()
{
    if (m_file && m_file->isOpen()) {
        m_file->close();
    }
    delete m_file;
}

void FileUploader::uploadFile(quint16 id, const QString &filePath)
{
    m_fileName = filePath;
    m_file = new QFile(filePath);

    if (!m_file->open(QIODevice::ReadOnly)) {
        qWarning() << "Could not open file:" << m_file->errorString();
        return;
    }

    // 保存当前文件指针位置
    qint64 originalPos = m_file->pos();
    QCryptographicHash hash(QCryptographicHash::Md5);
    if (!hash.addData(m_file))  {
        qWarning() << "Failed to read data from file:" << m_fileName;
        return;
    }

    m_fileMd5 = hash.result().toHex();  // 返回十六进制字符串

    // 恢复文件指针到原来的位置
    m_file->seek(originalPos);

    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    _total_size = fileInfo.size();

    if (_total_size % MAX_FILE_LEN) {
        _last_seq = (_total_size / MAX_FILE_LEN) + 1;
    } else {
        _last_seq = _total_size / MAX_FILE_LEN;
    }

    _seq = 0;

    while (!m_file->atEnd()) {
        //每次读取2048字节发送
        QByteArray buffer = m_file->read(MAX_FILE_LEN);
        QJsonObject jsonObj;
        // 将文件内容转换为 Base64 编码（可选）
        QString base64Data = buffer.toBase64();

        ++_seq;
        jsonObj["md5"] = m_fileMd5;
        jsonObj["name"] = fileName;
        //添加id操作细节的json项...
        jsonObj["seq"] = _seq;
        jsonObj["trans_size"] = buffer.size()  + (_seq - 1) * MAX_FILE_LEN;
        jsonObj["total_size"] = _total_size;

        if (buffer.size()  + (_seq - 1) * MAX_FILE_LEN == _total_size) {
            jsonObj["last"] = 1;
        } else {
            jsonObj["last"] = 0;
        }

        jsonObj["data"] = base64Data;
        jsonObj["last_seq"] = _last_seq;
        QJsonDocument doc(jsonObj);
        auto send_data = doc.toJson();
        //开始发送
        TcpClient::getInstance()->sendMsg(id, send_data);

        // 发送完成后等待500ms
        startDelay(500);

        // 发射进度信号
        int progress = (buffer.size()  + (_seq - 1) * MAX_FILE_LEN) * 100 / _total_size;
        emit uploadProgressChanged(progress);
    }

    // 关闭文件
    m_file->close();
    emit uploadCompleted();
}

void FileUploader::startDelay(int msecs)
{
    qDebug() << "Starting delay...";
    QEventLoop loop;
    QTimer timer;
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    timer.start(msecs);
    loop.exec();
    qDebug() << "Delay finished!";
}

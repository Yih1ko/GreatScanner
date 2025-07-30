#ifndef FILEUPLOADER_H
#define FILEUPLOADER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QCryptographicHash>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include "tcpclient.h"
#include "Def.h"

//单个文件上传器
class FileUploader : public QObject
{
    Q_OBJECT

public:
    explicit FileUploader(QObject *parent = nullptr);
    ~FileUploader();

    void uploadFile(quint16 id, const QString &filePath);

signals:
    void uploadProgressChanged(int progress);   //进度
    void uploadCompleted();                     //发送完了

private slots:
    void startDelay(int msecs);

private:
    QString m_fileName;
    QString m_fileMd5;
    QFile *m_file;
    int _seq;
    int _total_size;
    int _last_seq;
};

#endif // FILEUPLOADER_H

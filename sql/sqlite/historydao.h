// historydao.h
#ifndef HISTORYDAO_H
#define HISTORYDAO_H

#include "basedao.h"
#include <QDateTime>

struct FileInfo {
    qint64 fileId = 0;
    qint64 userId = 0;
    QString fileHash;
    QString storagePath;
    QDateTime uploadTime;
    QString logicalName;
    QString processType;

    bool isValid() const { return fileId > 0; }
};

class HistoryDao : public BaseDao<FileInfo>
{
public:
    explicit HistoryDao(QObject *parent = nullptr)
        : BaseDao<FileInfo>(parent) {}

    // 实现基类纯虚函数
    QString tableName() const override { return "file_info"; }
    QString primaryKey() const override { return "file_id"; }

    FileInfo mapToEntity(const QVariantMap &row) const override;

    QVariantMap entityToMap(const FileInfo &entity) const override;

    // 业务特定方法
    QList<FileInfo> findByUser(qint64 userId);

    QList<FileInfo> paginateByUploadTime(qint64 userId,
                                         int pageSize,
                                         int pageNumber);

    FileInfo findByHash(qint64 userId, const QString &fileHash);

    bool exists(qint64 userId, const QString &fileHash);
};

#endif // HISTORYDAO_H

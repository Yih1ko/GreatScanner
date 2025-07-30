#include "historydao.h"


FileInfo HistoryDao::mapToEntity(const QVariantMap &row) const {
    FileInfo info;
    if (row.isEmpty())  return info;

    info.fileId  = row.value("file_id").toLongLong();
    info.userId  = row.value("user_id").toLongLong();
    info.fileHash  = row.value("file_hash").toString();
    info.storagePath  = row.value("storage_path").toString();
    info.uploadTime  = QDateTime::fromString(
        row.value("upload_time").toString(),  Qt::ISODate);
    info.logicalName  = row.value("logical_name").toString();
    info.processType  = row.value("process_type").toString();

    return info;
}

QVariantMap HistoryDao::entityToMap(const FileInfo &entity) const {
    QVariantMap map;
    if (entity.fileId  > 0) {
        map["file_id"] = entity.fileId;
    }
    map["user_id"] = entity.userId;
    map["file_hash"] = entity.fileHash;
    map["storage_path"] = entity.storagePath;
    map["upload_time"] = entity.uploadTime.toString(Qt::ISODate);
    map["logical_name"] = entity.logicalName;
    map["process_type"] = entity.processType;

    return map;
}

QList<FileInfo> HistoryDao::findByUser(qint64 userId) {
    QVariantMap params;
    params["user_id"] = userId;

    return paginate("user_id = :user_id",
                    params,
                    "upload_time DESC",
                    0, 0); // 获取全部
}

QList<FileInfo> HistoryDao::paginateByUploadTime(qint64 userId, int pageSize, int pageNumber)
{
    QVariantMap params;
    params["user_id"] = userId;

    int offset = (pageNumber - 1) * pageSize;
    return paginate("user_id = :user_id",
                    params,
                    "upload_time DESC",
                    pageSize,
                    offset);
}

FileInfo HistoryDao::findByHash(qint64 userId, const QString &fileHash) {
    QString sql = "SELECT * FROM file_info "
                  "WHERE user_id = :user_id AND file_hash = :file_hash";

    QVariantMap params;
    params["user_id"] = userId;
    params["file_hash"] = fileHash;

    QVariantMap row = SqliteMgr::instance()->queryOne(sql, params);
    return mapToEntity(row);
}

bool HistoryDao::exists(qint64 userId, const QString &fileHash) {
    QString sql = "SELECT 1 FROM file_info "
                  "WHERE user_id = :user_id AND file_hash = :file_hash "
                  "LIMIT 1";

    QVariantMap params;
    params["user_id"] = userId;
    params["file_hash"] = fileHash;

    return !SqliteMgr::instance()->queryOne(sql, params).isEmpty();
}

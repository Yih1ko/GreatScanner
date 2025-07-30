// basedao.h
#ifndef BASEDAO_H
#define BASEDAO_H

#include "sqlitemgr.h"
#include <QObject>
#include <QVariantMap>
#include <QList>

template <typename T>
class BaseDao : public QObject
{
public:
    explicit BaseDao(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~BaseDao() {}

    // 必须由子类实现的接口
    virtual QString tableName() const = 0;
    virtual QString primaryKey() const = 0;
    virtual T mapToEntity(const QVariantMap &row) const = 0;
    virtual QVariantMap entityToMap(const T &entity) const = 0;

    // 基础CRUD操作
    bool insert(T &entity) {
        QVariantMap data = entityToMap(entity);
        QStringList fields = data.keys();
        QStringList placeholders;

        for (const QString &field : fields) {
            placeholders.append(":"  + field);
        }

        QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                          .arg(tableName())
                          .arg(fields.join(",  "))
                          .arg(placeholders.join(",  "));

        if (SqliteMgr::instance()->execute(sql, data)) {
            // 获取自增ID
            QVariantMap result = SqliteMgr::instance()->queryOne(
                "SELECT last_insert_rowid() AS id");
            if (!result.isEmpty())  {
                data[primaryKey()] = result["id"];
                entity = mapToEntity(data);
                return true;
            }
        }
        return false;
    }

    bool update(const T &entity) {
        QVariantMap data = entityToMap(entity);
        QStringList updates;

        for (auto it = data.constBegin();  it != data.constEnd();  ++it) {
            if (it.key()  != primaryKey()) {
                updates.append(QString("%1  = :%1").arg(it.key()));
            }
        }

        QString sql = QString("UPDATE %1 SET %2 WHERE %3 = :%3")
                          .arg(tableName())
                          .arg(updates.join(",  "))
                          .arg(primaryKey());

        return SqliteMgr::instance()->execute(sql, data);
    }

    bool removeById(const QVariant &id) {
        QString sql = QString("DELETE FROM %1 WHERE %2 = :id")
        .arg(tableName())
            .arg(primaryKey());

        QVariantMap params;
        params["id"] = id;
        return SqliteMgr::instance()->execute(sql, params);
    }

    T findById(const QVariant &id) {
        QString sql = QString("SELECT * FROM %1 WHERE %2 = :id")
        .arg(tableName())
            .arg(primaryKey());

        QVariantMap params;
        params["id"] = id;

        QVariantMap row = SqliteMgr::instance()->queryOne(sql, params);
        return mapToEntity(row);
    }

    QList<T> findAll(const QString &orderBy = "") {
        QString sql = QString("SELECT * FROM %1").arg(tableName());
        if (!orderBy.isEmpty())  {
            sql += " ORDER BY " + orderBy;
        }

        QList<QVariantMap> rows = SqliteMgr::instance()->query(sql);
        QList<T> result;
        for (const auto &row : rows) {
            result.append(mapToEntity(row));
        }
        return result;
    }

protected:
    // 分页查询通用实现
    QList<T> paginate(const QString &condition,
                      const QVariantMap &params,
                      const QString &orderBy,
                      int limit,
                      int offset)
    {
        QString sql = QString("SELECT * FROM %1").arg(tableName());

        if (!condition.isEmpty())  {
            sql += " WHERE " + condition;
        }

        if (!orderBy.isEmpty())  {
            sql += " ORDER BY " + orderBy;
        }

        sql += " LIMIT :limit OFFSET :offset";

        QVariantMap finalParams = params;
        finalParams["limit"] = limit;
        finalParams["offset"] = offset;

        QList<QVariantMap> rows = SqliteMgr::instance()->query(sql, finalParams);
        QList<T> result;
        for (const auto &row : rows) {
            result.append(mapToEntity(row));
        }
        return result;
    }
};

#endif // BASEDAO_H

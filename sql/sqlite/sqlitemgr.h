// sqlitemgr.h
#ifndef SQLITEMGR_H
#define SQLITEMGR_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QList>
#include <QMutex>

class SqliteMgr : public QObject
{
    Q_OBJECT

public:
    // 单例模式获取实例
    static SqliteMgr* instance();

    // 初始化数据库连接
    bool init(const QString& dbName, const QString& connectionName = "qt_sql_default_connection");

    // 执行SQL语句（无返回值）
    bool execute(const QString& sql, const QVariantMap& params = QVariantMap());

    // 查询多条记录
    QList<QVariantMap> query(const QString& sql, const QVariantMap& params = QVariantMap());

    // 查询单条记录
    QVariantMap queryOne(const QString& sql, const QVariantMap& params = QVariantMap());

    // 开启事务
    bool beginTransaction();

    // 提交事务
    bool commitTransaction();

    // 回滚事务
    bool rollbackTransaction();

    // 关闭数据库连接
    void close();

private:
    explicit SqliteMgr(QObject *parent = nullptr);
    ~SqliteMgr();

    QSqlDatabase m_db;
    static SqliteMgr* m_instance;
    static QMutex m_mutex;
};

#endif // SQLITEMGR_H

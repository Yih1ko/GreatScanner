#include "sqlitemgr.h"
#include <QSqlRecord>

SqliteMgr* SqliteMgr::m_instance = nullptr;
QMutex SqliteMgr::m_mutex;

SqliteMgr::SqliteMgr(QObject *parent) : QObject(parent)
{
}

SqliteMgr::~SqliteMgr()
{
    if(m_db.isOpen())  {
        m_db.close();
    }
}

SqliteMgr* SqliteMgr::instance()
{
    QMutexLocker locker(&m_mutex);
    if (m_instance == nullptr) {
        m_instance = new SqliteMgr();
    }
    return m_instance;
}

bool SqliteMgr::init(const QString &dbName, const QString &connectionName)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    m_db.setDatabaseName(dbName);

    if (!m_db.open())  {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }

    return true;
}

bool SqliteMgr::execute(const QString &sql, const QVariantMap &params)
{
    QSqlQuery query(m_db);
    query.prepare(sql);

    for(auto it = params.begin();  it != params.end();  ++it) {
        query.bindValue(":"  + it.key(),  it.value());
    }

    if(!query.exec())  {
        qCritical() << "Execute SQL failed:" << query.lastError().text();
        qCritical() << "SQL:" << sql;
        return false;
    }

    return true;
}

QList<QVariantMap> SqliteMgr::query(const QString &sql, const QVariantMap &params)
{
    QList<QVariantMap> result;
    QSqlQuery query(m_db);
    query.prepare(sql);

    for(auto it = params.begin();  it != params.end();  ++it) {
        query.bindValue(":"  + it.key(),  it.value());
    }

    if(!query.exec())  {
        qCritical() << "Query SQL failed:" << query.lastError().text();
        qCritical() << "SQL:" << sql;
        return result;
    }

    while(query.next())  {
        QVariantMap record;
        for(int i = 0; i < query.record().count();  ++i) {
            record.insert(query.record().fieldName(i),  query.value(i));
        }
        result.append(record);
    }

    return result;
}

QVariantMap SqliteMgr::queryOne(const QString &sql, const QVariantMap &params)
{
    QVariantMap result;
    QList<QVariantMap> list = query(sql, params);
    if(!list.isEmpty())  {
        result = list.first();
    }
    return result;
}

bool SqliteMgr::beginTransaction()
{
    return m_db.transaction();
}

bool SqliteMgr::commitTransaction()
{
    return m_db.commit();
}

bool SqliteMgr::rollbackTransaction()
{
    return m_db.rollback();
}

void SqliteMgr::close()
{
    if(m_db.isOpen())  {
        m_db.close();
    }
}

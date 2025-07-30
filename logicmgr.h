#ifndef LOGICMGR_H
#define LOGICMGR_H
#include "singleton.h"
#include <functional>
#include <QMap>
#include <QJsonObject>
#include <QObject>
#include <QThread>

class LogicWorker: public QObject{
    Q_OBJECT
public:
    LogicWorker(QObject * parent = nullptr);
    void InitHandlers();
private:
    QMap<quint32, std::function<void(QJsonObject obj)> >_handlers;
signals:
    void sig_trans_size(int trans_size);
public slots:
    void slot_logic_process(quint16 msgid, QJsonObject obj);
};

//对于服务器回包的处理
class LogicMgr : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(LogicMgr)
public:
    ~LogicMgr();
private:
    LogicMgr();
    LogicWorker* m_worker;
    QThread* m_workThread;
signals:
    void sig_logic_process(quint16 msgid, QJsonObject obj);
    void sig_trans_size(int trans_size);
};

#endif // LOGICMGR_H

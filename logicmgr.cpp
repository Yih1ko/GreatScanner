#include "logicmgr.h"
#include "Def.h"
#include <QDebug>
#include "tcpclient.h"
#include <QDateTime>

Q_SINGLETON_CREATE_CPP(LogicMgr)

LogicMgr::~LogicMgr()
{
    m_workThread->quit();
    m_workThread->wait();
    m_workThread->deleteLater();
    m_worker->deleteLater();
}

LogicMgr::LogicMgr():QObject(nullptr)
{
    m_worker = new LogicWorker();
    m_workThread = new QThread(this);
    //worker移动到子线程处理消息
    m_worker->moveToThread(m_workThread);

    //连接信号和槽函数
    connect(this, &LogicMgr::sig_logic_process, m_worker,
            &LogicWorker::slot_logic_process);
    connect(m_worker, &LogicWorker::sig_trans_size,
            this, &LogicMgr::sig_trans_size);
    //启动线程
    m_workThread->start();
}

LogicWorker::LogicWorker(QObject* parent):QObject(parent)
{
    InitHandlers();
}

void LogicWorker::InitHandlers()
{
    //注册测试消息
    _handlers[ID_TEST_MSG_RSP] = [this](QJsonObject obj){
        auto err = obj["error"].toInt();
        if(err != SUCCESS){
            qDebug() << "test msg rsp err is " << err;
            return;
        }

        auto data = obj["data"].toString();
        qDebug() << "recv data is " << data;
    };

    //注册上传消息
    _handlers[ID_TEST_UPLOAD_FILE_RSP] = [this](QJsonObject obj){
        auto err = obj["error"].toInt();
        if(err != SUCCESS){
            qDebug() << "upload msg rsp err is " << err;
            return;
        }

        auto name = obj["name"].toString();
        auto total_size = obj["total_size"].toInt();
        auto trans_size = obj["trans_size"].toInt();

        qDebug() << "recv : " << name << " file trans_size is " << trans_size;
        emit sig_trans_size(trans_size);
    };
}

void LogicWorker::slot_logic_process(quint16 msgid, QJsonObject obj)
{
    if(!_handlers.contains(msgid)){
        qDebug() << "msgid: " << msgid << " not found";
        return;
    }
    _handlers[msgid](obj);
}



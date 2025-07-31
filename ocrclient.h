#ifndef OCRCLIENT_H
#define OCRCLIENT_H
//实则是跑在客户端的OcrServer，只不过Client好理解

#include <qobject.h>
#include <assert.h>
#include <iostream>
#include "Def.h"
#include "_singleton.h"
#include "ipc/include/ShmIPCServer.h"
#include "ipc/include/IShmChannelNotify.h"
#include <Qqueue>
#include "ipc/processctrl.h"
#include <stdexcept>

struct TaskNode {
public:
    TaskNode(ipc::IpcId ipcId, QByteArray data): m_ipcId(ipcId), m_data(data){}
    TaskNode(){}
    ipc::IpcId m_ipcId;
    QByteArray m_data;
};

class ShmChannelNotifyServer : public QObject, public ipc::IShmChannelNotify
{
    Q_OBJECT
public:
    ShmChannelNotifyServer() {}
    ~ShmChannelNotifyServer() {}

    void OnDataRecived(const char* buf, int size) override
    {
        std::cout << "OnDataRecived: msgSize=" << size << std::endl;
        respData(QByteArray(buf, size));
    }
    void OnWriteSuccess(int id) override
    {
        std::cout << "OnWriteSuccess: id=" << id << std::endl;
    }
    void OnWriteFailed(int id) override
    {
        std::cout << "OnWriteFailed: id=" << id << std::endl;
    }

signals:
    void respData(QByteArray);
};

class OcrClient: public QObject, public _Singleton<OcrClient>, public std::enable_shared_from_this<OcrClient>
{
    Q_OBJECT
public:
    ~OcrClient();
    void init();
    static void DestoryInstance();
signals:
    void sig_RespOcrRes(QByteArray data);
public slots:
    void PostOcrTask(ipc::IpcId ipcId, QByteArray data);
    void slot_RespOcrRes(QByteArray data);
private:
    friend class _Singleton<OcrClient>;
    OcrClient();

    ShmChannelNotifyServer* m_notify;
    ipc::ShmIPCServer* m_server;
    QQueue<TaskNode> m_taskQueue;
    std::thread m_workThread;
    std::mutex m_mtx;
    std::atomic<bool> m_bStop;
    std::condition_variable m_cv;
    //ProcessCtrl m_processCtrl;
};

#endif // OCRCLIENT_H

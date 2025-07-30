#include "ocrclient.h"
#include <qdebug.h>
#include "QImage"

void OcrClient::PostOcrTask(ipc::IpcId ipcId, QByteArray data)
{
    if (m_bStop.load())  return; // 停止后拒绝新任务
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_taskQueue.push_back(TaskNode(ipcId, data));
    }
    m_cv.notify_one();
}

void OcrClient::slot_RespOcrRes(QByteArray data)
{
    emit sig_RespOcrRes(data);
}

OcrClient::~OcrClient() {
    {
        std::lock_guard<std::mutex> lock(m_mtx);
        m_bStop = true;
        m_taskQueue.clear();   // 清空残留任务
    }
    m_cv.notify_all();         // 确保唤醒线程

    // 先停止server，这样SendMsg可能会失败返回，但我们已经设置m_bStop，工作线程会退出
    if (m_server) {
        //m_server->Stop();          // 停止服务
        // delete m_server;
        // m_server = nullptr;
    }

    if (m_workThread.joinable())   {
        m_workThread.join();    // 等待线程安全退出
    }

    delete m_notify;
    m_notify = nullptr;
}

void OcrClient::init()
{

}

void OcrClient::DestoryInstance()
{
    _Singleton<OcrClient>::DestroyInstance();
}

OcrClient::OcrClient() {
    m_notify = new ShmChannelNotifyServer;
    m_server = new ipc::ShmIPCServer(m_notify);

    int ret = m_server->Init();
    if (0 != ret)
    {
        qDebug()<<"IPC-OCR服务初始化失败!";
    }
    m_server->Start();
    qDebug()<<"IPC-OCR服务初始化成功!";
    QObject::connect(m_notify, &ShmChannelNotifyServer::respData, this, &OcrClient::slot_RespOcrRes);
    m_workThread = std::thread([this]() {
        while (true) {
            TaskNode task;
            {
                std::unique_lock<std::mutex> lock(m_mtx);
                m_cv.wait(lock,  [this]() {
                    return m_bStop || !m_taskQueue.empty();
                });

                if (m_bStop) {
                    return;
                }
                task = m_taskQueue.front();
                m_taskQueue.pop_front();  // 弹出任务
            }
            if (m_bStop) {
                return;
            }
            if (m_server) {
                // 发送消息，即使失败也不退出线程
                int ret = m_server->SendMsg(task.m_ipcId, task.m_data, task.m_data.size());
            }
        }
    });
}

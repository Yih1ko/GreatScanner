#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonObject>
#include "singleton.h"


class TcpClient : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(TcpClient)
public:
    ~TcpClient();

    //删除拷贝构造和拷贝赋值
    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    //连接到服务器
    void connectToServer(const QString& host, quint16 port);

    //断开连接
    void disconnectFromServer();

    //公共发送接口
    void sendMsg(quint16 id, QByteArray data);

    bool isConnected();

private:
    //构造函数变为私有
    explicit TcpClient(QObject *parent = nullptr);

    //QTcpSocket类，用来通信的客户端
    QTcpSocket* m_socket;

    //缓存收到的数据
    QByteArray m_buffer;

    void processData();
signals:
    void sig_send_msg(quint16 id, QByteArray data);
    void sig_connected(bool);
    //透传服务器回包的信号
    void sig_logic_process(quint16 id, QJsonObject obj);
    //监听网络错误的信号
    void sig_net_error(QString);
public slots:

    //连接成功触发的槽函数
    void slot_connected();
    //tcp缓冲区有数据可读，触发读取消息槽函数
    void slot_ready_read();
    //捕获到断开连接触发的槽函数
    void slot_disconnected();
    //捕获到出错信号触发的槽函数
    void slot_error_occured(QAbstractSocket::SocketError socketError);
    //发送消息槽函数
    void slot_send_msg(quint16 id, QByteArray data);

};

#endif // TCPCLIENT_H

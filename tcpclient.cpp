#include "tcpclient.h"
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include "Def.h"
#include <QFile>
#include <QCryptographicHash>

Q_SINGLETON_CREATE_CPP(TcpClient)

TcpClient::~TcpClient()
{

}

TcpClient::TcpClient(QObject *parent) : QObject(parent),
    m_socket(new QTcpSocket(this))
{
    //连接 连接成功信号和槽函数
    connect(m_socket, &QTcpSocket::connected, this, &TcpClient::slot_connected);
    //连接 读数据信号和槽函数
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::slot_ready_read);
    //连接 断开连接信号和槽函数
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::slot_disconnected);
    //连接 出错信号和槽函数
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TcpClient::slot_error_occured);

    //连接 发送数据信号和槽函数
    connect(this, &TcpClient::sig_send_msg, this, &TcpClient::slot_send_msg);
}

void TcpClient::processData()
{
    while(m_buffer.size() >= TCP_HEAD_LEN){
        //先取出六字节头部
        auto head_byte = m_buffer.left(TCP_HEAD_LEN);
        QDataStream stream(head_byte);
        //设置为大端模式
        stream.setByteOrder(QDataStream::BigEndian);
        //读取ID
        quint16 msg_id;
        stream >> msg_id;
        //读取长度
        quint32 body_length;
        stream >> body_length;
        if(m_buffer.size() >= TCP_HEAD_LEN+body_length){
            //完整的消息体已经接受
            QByteArray body = m_buffer.mid(TCP_HEAD_LEN,body_length);
            //去掉完整的消息包
            m_buffer = m_buffer.mid(TCP_HEAD_LEN+body_length);
            // 解析服务器发过来的消息
            QJsonDocument jsonDoc = QJsonDocument::fromJson(body);
            if(jsonDoc.isNull()){
                qDebug() << "Failed to create JSON doc.";
                this->m_socket->close();
                return;
            }

            if(!jsonDoc.isObject()){
                qDebug() << "JSON is not an object.";
                this->m_socket->close();
                return;
            }
            //qDebug() << "receive data is " << body;
            // 获取 JSON 对象
            QJsonObject jsonObject = jsonDoc.object();
            emit sig_logic_process(msg_id, jsonObject); //将服务器的回包透传给外层逻辑(LogicMgr处理)
        }else{
            //消息未完全接受，所以中断
            break;
        }
    }
}

//发送数据槽函数
void TcpClient::slot_send_msg(quint16 id, QByteArray body)
{
    //如果连接异常则直接返回
    if(m_socket->state() != QAbstractSocket::ConnectedState){
        emit sig_net_error(QString("断开连接无法发送"));
        return;
    }

    //获取body的长度
    quint32 bodyLength = body.size();

    //创建字节数组
    QByteArray data;
    //绑定字节数组
    QDataStream stream(&data, QIODevice::WriteOnly);
    //设置大端模式
    stream.setByteOrder(QDataStream::BigEndian);
    //写入ID
    stream << id;
    //写入长度
    stream << bodyLength;
    //写入包体
    data.append(body);

    //发送消息
    m_socket->write(data);
}

void TcpClient::connectToServer(const QString &host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

//断开连接函数
void TcpClient::disconnectFromServer()
{
    if(m_socket->state() != QAbstractSocket::ConnectedState){
        return;
    }

    m_socket->disconnectFromHost(); // 优雅地关闭连接

    // 或者使用 socket->close(); 立即关闭连接
    if (m_socket->waitForDisconnected(3000)) {

    } else {
        emit sig_net_error(QString("关闭错误%1").arg(m_socket->errorString()));
    }
}

void TcpClient::sendMsg(quint16 id,QByteArray data)
{
    //发送信号，统一交给槽函数处理，这么做的好处是多线程安全
    emit sig_send_msg(id, data);
}

bool TcpClient::isConnected()
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}


//连接成功槽函数
void TcpClient::slot_connected()
{
    emit sig_connected(true);
}

//接受服务器发送的信息
void TcpClient::slot_ready_read()
{
    //读取所有数据
    QByteArray data = m_socket->readAll();

    //将数据缓存起来
    m_buffer.append(data);

    //处理收到的数据
    processData();
}

//捕获连接断开信号
void TcpClient::slot_disconnected()
{
    //连接断开,发送信号通知主界面弹窗
    emit sig_net_error(QString("连接断开"));
}

void TcpClient::slot_error_occured(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    //产生错误, 发送主界面弹窗
    emit sig_net_error(QString("网络错误: %1 ！").arg(m_socket->errorString()));
    //出错则判断连接状态并重置
    if(m_socket->state() != QAbstractSocket::ConnectedState){
        return;
    }

    //出错就关闭连接
    m_socket->close();
}


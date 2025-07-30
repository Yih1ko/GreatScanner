#include "httpclient.h"

HttpClient::HttpClient() {
    //依赖信号槽机制处理http请求和完成信号
    connect(this, &HttpClient::sig_http_finish, this, &HttpClient::slot_http_finish);
}

void HttpClient::PostHttpReq(QUrl url, QJsonObject json, ReqId reqId, Modules mod)
{
    QByteArray data = QJsonDocument(json).toJson();
    QNetworkRequest request(url);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(data.length()));

    auto self = shared_from_this();

    QNetworkReply* reply = m_manager.post(request, data);

    //链接响应完成信号
    connect(reply, &QNetworkReply::finished, [reply, self, reqId, mod](){
        //处理错误先
        if(reply->error() != QNetworkReply::NoError){
            qDebug() << reply->errorString();
            //透传错误信号
            emit self->sig_http_finish(reqId, "", ErrorCodes::Error_Network, mod);
            reply->deleteLater();
            return;
        }

        QString res = reply->readAll();
        emit self->sig_http_finish(reqId, res, ErrorCodes::SUCCESS, mod);
        reply->deleteLater();
        return;
    });
}

HttpClient::~HttpClient()
{

}

void HttpClient::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod)
{
    qDebug()<<"http finished, recv body is:"<<res;
    //注册模块
    if(mod == Modules::REGISTMOD){
        //发送信号通知指定模块的http响应结束了
        emit sig_reg_mod_finish(id, res, err, mod);
    }else if(mod == Modules::LOGINMOD){
        emit sig_login_mod_finish(id, res, err, mod);
    }else if(mod == Modules::RESETMOD){
        emit sig_reset_mod_finish(id, res, err, mod);
    }
}

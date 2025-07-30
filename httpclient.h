#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include "_singleton.h"
#include <QObject>
#include "Def.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

class HttpClient : public QObject, public _Singleton<HttpClient>, public std::enable_shared_from_this<HttpClient>
{
    Q_OBJECT
public:
    void PostHttpReq(QUrl url, QJsonObject json, ReqId reqId, Modules mod);
    ~HttpClient();
private:
    friend class _Singleton<HttpClient>;
    QNetworkAccessManager m_manager;
    HttpClient();
private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err, Modules mod);//注册模块
    void sig_login_mod_finish(ReqId id, QString res, ErrorCodes err, Modules mod);//登录模块
    void sig_reset_mod_finish(ReqId id, QString res, ErrorCodes err, Modules mod);//密码重置模块
};

#endif // HTTPCLIENT_H

#ifndef LOGINORSIGNUPWIDGET_H
#define LOGINORSIGNUPWIDGET_H

#include "Def.h"
#include <QWidget>
#include <QMap>

namespace Ui {
class LoginOrSignUpWidget;
}

class LoginOrSignUpWidget : public QWidget
{
    Q_OBJECT

    // 页面索引常量
    enum PageIndex {
        LOGIN_PAGE = 0,
        SIGNUP_PAGE = 1,
        RESET_PAGE = 2
    };

public:
    explicit LoginOrSignUpWidget(QWidget *parent = nullptr);
    ~LoginOrSignUpWidget();

signals:
    void loginSuccessfully();
private slots:
    void handleHttpResponse(ReqId id, QString res, ErrorCodes err);
    void on_ExecuteBtn_clicked();
    void on_GetCodeBtn_clicked();
    void on_GetCodeBtn_2_clicked();

private:
    Ui::LoginOrSignUpWidget *ui;
    QTimer* m_loginValidationTimer;  // 登录输入验证延时器
    QTimer* m_signupValidationTimer; // 注册输入验证延时器
    QTimer* m_ResetValidationTimer; // 密码重置输入验证延时器

    // 网络请求处理器映射表 <ReqId, 处理函数>
    QMap<ReqId, std::function<void(const QJsonObject&)>> m_requestHandlers;

    // 初始化方法组
    void initializeUI();
    void setupConnections();
    void configureValidation();
    void loadStyleResources();
    void initHandlers();

    // 输入验证方法
    bool validateLoginForm();
    bool validateSignupForm();
    bool validateResetForm();
    void showStatusMessage(const QString& msg);

    // 静态验证规则
    static bool isValidEmailFormat(const QString& email);
    static bool isStrongPassword(const QString& password);

    // 网络请求方法
    void sendLoginRequest();
    void sendSignupRequest();
    void sendSignupGetCodeRequest();
    void sendResetRequest();
    void sendResetGetCodeRequest();
};

#endif // LOGINORSIGNUPWIDGET_H

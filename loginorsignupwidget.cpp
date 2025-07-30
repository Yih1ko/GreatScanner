#include "loginorsignupwidget.h"
#include "ui_loginorsignupwidget.h"
#include "httpclient.h"
#include <qgraphicseffect.h>
#include <qtimer.h>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>

// 常量定义
constexpr int INPUT_VALIDATION_DELAY = 500; // 输入验证延时(ms)
constexpr int STATUS_ANIMATION_DURATION = 800; // 状态提示动画时长
const QUrl LOGIN_API_ENDPOINT = QUrl("http://localhost:8080/user_login");
const QUrl SIGNUP_API_ENDPOINT = QUrl("http://localhost:8080/user_register");
const QUrl SIGNUP_GETCODE_API_ENDPOINT = QUrl("http://localhost:8080/user_register_send_code");
const QUrl RESET_API_ENDPOINT = QUrl("http://localhost:8080/reset_pwd");
const QUrl RESET_GETCODE_API_ENDPOINT = QUrl("http://localhost:8080/reset_send_code");

LoginOrSignUpWidget::LoginOrSignUpWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginOrSignUpWidget)
{
    ui->setupUi(this);

    // 初始化流程
    initializeUI();
    setupConnections();
    configureValidation();
    loadStyleResources();
    initHandlers();
}

LoginOrSignUpWidget::~LoginOrSignUpWidget()
{
    delete ui;
}

void LoginOrSignUpWidget::initializeUI()
{
    // 窗口属性设置
    setAttribute(Qt::WA_TranslucentBackground);
    SET_MICA_BACKGROUND(this);
    setFixedSize(350, 580);
    setWindowTitle("GreatScanner");

    ui->ExecuteBtn->setRadius(0);

    // 初始页面设置
    ui->stackedWidget->setCurrentIndex(LOGIN_PAGE);
    QTimer::singleShot(0, [this]{
        ui->Slider->activateAt(ui->LoginBtn);
    });
    QTimer::singleShot(100, this, [this]{
        ui->StatusTip->setProperty("originalPos", ui->StatusTip->pos());
    });
}

void LoginOrSignUpWidget::setupConnections()
{
    // 页面切换
    connect(ui->LoginBtn, QOverload<>::of(&ClickableLabel::clicked), [this] {
        m_loginValidationTimer->stop(); // 停止登录验证定时器
        ui->stackedWidget->setCurrentIndex(LOGIN_PAGE);
        ui->Slider->activateAt(ui->LoginBtn);
        validateLoginForm();
    });

    connect(ui->SignUpBtn, QOverload<>::of(&ClickableLabel::clicked), [this] {
        m_signupValidationTimer->stop(); // 停止注册验证定时器
        ui->stackedWidget->setCurrentIndex(SIGNUP_PAGE);
        ui->Slider->activateAt(ui->SignUpBtn);
        validateSignupForm();
    });

    connect(ui->ResetPwdBtn, QOverload<>::of(&ClickableLabel::clicked), [this] {
        m_ResetValidationTimer->stop(); // 停止重置密码验证定时器
        ui->stackedWidget->setCurrentIndex(RESET_PAGE);
        ui->Slider->activateAt(ui->LoginBtn);
        validateSignupForm();
    });

    // 网络响应
    auto client = HttpClient::GetInstance().get();
    connect(client, &HttpClient::sig_login_mod_finish,
            this, &LoginOrSignUpWidget::handleHttpResponse);
    connect(client, &HttpClient::sig_reg_mod_finish,
            this, &LoginOrSignUpWidget::handleHttpResponse);
    connect(client, &HttpClient::sig_reset_mod_finish,
            this, &LoginOrSignUpWidget::handleHttpResponse);
}

void LoginOrSignUpWidget::configureValidation()
{
    // 初始化验证定时器
    m_loginValidationTimer = new QTimer(this);
    m_loginValidationTimer->setSingleShot(true);
    connect(m_loginValidationTimer, &QTimer::timeout,
            this, &LoginOrSignUpWidget::validateLoginForm);

    m_signupValidationTimer = new QTimer(this);
    m_signupValidationTimer->setSingleShot(true);
    connect(m_signupValidationTimer, &QTimer::timeout,
            this, &LoginOrSignUpWidget::validateSignupForm);

    m_ResetValidationTimer = new QTimer(this);
    m_ResetValidationTimer->setSingleShot(true);
    connect(m_ResetValidationTimer, &QTimer::timeout,
            this, &LoginOrSignUpWidget::validateResetForm);

    // 登录表单输入监听
    auto startLoginValidation = [this] {
        m_loginValidationTimer->stop();
        m_loginValidationTimer->start(INPUT_VALIDATION_DELAY);
    };
    connect(ui->EmailEdit_2, &QLineEdit::textChanged, startLoginValidation);
    connect(ui->PwdEdit_2, &QLineEdit::textChanged, startLoginValidation);

    // 注册表单输入监听
    auto startSignupValidation = [this] {
        m_signupValidationTimer->stop();
        m_signupValidationTimer->start(INPUT_VALIDATION_DELAY);
    };
    connect(ui->EmailEdit, &QLineEdit::textChanged, startSignupValidation);
    connect(ui->PwdEdit, &QLineEdit::textChanged, startSignupValidation);
    connect(ui->NameEdit, &QLineEdit::textChanged, startSignupValidation);

    // 密码重置表单输入监听
    auto startResetValidation = [this] {
        m_ResetValidationTimer->stop();
        m_ResetValidationTimer->start(INPUT_VALIDATION_DELAY);
    };
    connect(ui->EmailEdit_3, &QLineEdit::textChanged, startResetValidation);
    connect(ui->PwdEdit_3, &QLineEdit::textChanged, startResetValidation);
    connect(ui->CodeEdit, &QLineEdit::textChanged, startResetValidation);
}

void LoginOrSignUpWidget::loadStyleResources()
{
    // 加载样式表
    setStyleSheet(R"(
        * {
        outline: none;
        }

        QDialog {
        background: #D6DBE9;
        }

        QLineEdit {
        border: 1px solid #A0A0A0; /* 边框宽度为1px，颜色为#A0A0A0 */
        border-radius: 5px; /* 边框圆角 */
        padding-left: 5px; /* 文本距离左边界有5px */
        background-color: #F2F2F2; /* 背景颜色 */
        color: #5d5d5d ; /* 文本颜色 */
        selection-background-color: #A0A0A0; /* 选中文本的背景颜色 */
        selection-color: #F2F2F2; /* 选中文本的颜色 */
        font-family: "萍方-繁"; /* 文本字体族 */
        font-size: 10pt; /* 文本字体大小 */
        }

        QLineEdit:hover { /* 鼠标悬浮在QLineEdit时的状态 */
        border: 1px solid #5b72bd ;
        border-radius: 5px;
        background-color: #F2F2F2;
        color: #5b72bd ;
        selection-background-color: #5b72bd ;
        selection-color: #F2F2F2;
        }

        QLineEdit[echoMode="2"] { /* QLineEdit有输入掩码时的状态 */
        lineedit-password-character: 9679;
        lineedit-password-mask-delay: 2000;
        }

        QLineEdit:disabled { /* QLineEdit在禁用时的状态 */
        border: 1px solid #CDCDCD;
        background-color: #CDCDCD;
        color: #B4B4B4;
        }

        QLineEdit:read-only { /* QLineEdit在只读时的状态 */
        background-color: #CDCDCD;
        color: #F2F2F2;
        }
    )");
}

void LoginOrSignUpWidget::initHandlers()
{
    //注册用户注册-获取验证码回调
    m_requestHandlers.insert(ReqId::ID_REG_USER_GETCODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::SUCCESS){
            showStatusMessage("✓ 验证码已发送至邮箱，请在3min内使用");
            return;
        }
        if(error == ErrorCodes::EmailExist){
            showStatusMessage("邮箱已被注册");
            return;
        }
        if(error == ErrorCodes::RedisSetCodeFailed){
            showStatusMessage("验证码发送失败");
            return;
        }
    });
    //注册用户注册回调
    m_requestHandlers.insert(ReqId::ID_REG_USER, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::SUCCESS){
            showStatusMessage("✓ 注册成功");
            return;
        }
        if(error == ErrorCodes::RegFailed){
            showStatusMessage("未知错误");
            return;
        }
        if(error == ErrorCodes::EmailExist){
            showStatusMessage("此邮箱已被注册");
            return;
        }
        if(error == ErrorCodes::EmailNotMatchCode){
            showStatusMessage("验证码错误");
            return;
        }
    });

    //注册用户登录回调
    m_requestHandlers.insert(ReqId::ID_LOGIN, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::SUCCESS){
            showStatusMessage("✓ 登录成功，正在跳转...");
            emit loginSuccessfully();
            this->hide();
            return;
        }
        if(error == ErrorCodes::PasswdInvalid){
            showStatusMessage("登录失败, 邮箱或密码错误");
            return;
        }
    });

    //注册密码重置-获取验证码回调
    m_requestHandlers.insert(ReqId::ID_RESET_GETCODE, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error==ErrorCodes::SUCCESS){
            showStatusMessage("✓ 验证码已发送至邮箱，请在3min内使用");
            return;
        }
        if(error == ErrorCodes::EmailNotExist){
            showStatusMessage("邮箱未注册");
            return;
        }
        if(error == ErrorCodes::RedisSetCodeFailed){
            showStatusMessage("验证码发送失败");
            return;
        }
    });

    //注册密码重置回调
    m_requestHandlers.insert(ReqId::ID_RESET, [this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error==ErrorCodes::SUCCESS){
            showStatusMessage("✓ 密码重置成功，可以返回登录咯~");
            return;
        }
        if(error == ErrorCodes::EmailNotMatch){
            showStatusMessage("邮箱或密码错误");
            return;
        }
        if(error == ErrorCodes::EmailNotMatchCode){
            showStatusMessage("验证码错误");
            return;
        }
    });
}

// 输入验证逻辑实现
bool LoginOrSignUpWidget::validateLoginForm() {
    const QString email = ui->EmailEdit_2->text().trimmed();
    const QString password = ui->PwdEdit_2->text().trimmed();
    bool isValid = true;

    // 邮箱校验
    if (email.isEmpty())  {
        showStatusMessage("邮箱不能为空");
        isValid = false;
    } else if (!isValidEmailFormat(email)) {
        showStatusMessage("邮箱格式无效");
        isValid = false;
    }

    // 密码校验
    if (password.isEmpty())  {
        showStatusMessage("密码不能为空");
        isValid = false;
    } else if (!isStrongPassword(password)) {
        showStatusMessage("密码需至少8位且包含字母");
        isValid = false;
    }

    if (isValid) showStatusMessage("");
    return isValid;
}

bool LoginOrSignUpWidget::validateSignupForm() {
    const QString email = ui->EmailEdit->text().trimmed();
    const QString password = ui->PwdEdit->text().trimmed();
    const QString username = ui->NameEdit->text().trimmed();
    const QString code = ui->CodeEdit_2->text().trimmed();
    bool isValid = true;

    // 用户名校验
    if (username.isEmpty())  {
        showStatusMessage("用户名不能为空");
        isValid = false;
    } else if (username.length()  < 2 || username.length()  > 20) {
        showStatusMessage("用户名需2-20个字符");
        isValid = false;
    }

    // 邮箱校验
    if (email.isEmpty())  {
        showStatusMessage("邮箱不能为空");
        isValid = false;
    } else if (!isValidEmailFormat(email)) {
        showStatusMessage("邮箱格式无效");
        isValid = false;
    }

    // 密码校验
    if (password.isEmpty())  {
        showStatusMessage("密码不能为空");
        isValid = false;
    } else if (!isStrongPassword(password)) {
        showStatusMessage("密码需至少8位且包含字母");
        isValid = false;
    }

    //验证码检验
    if(code.isEmpty()){
        showStatusMessage("验证码不能为空");
        isValid = false;
    }

    if (isValid) showStatusMessage("");
    return isValid;
}

bool LoginOrSignUpWidget::validateResetForm() {
    const QString email = ui->EmailEdit_3->text().trimmed();
    const QString password = ui->PwdEdit_3->text().trimmed();
    const QString code = ui->CodeEdit->text().trimmed();
    bool isValid = true;

    // 邮箱校验
    if (email.isEmpty())  {
        showStatusMessage("邮箱不能为空");
        isValid = false;
    } else if (!isValidEmailFormat(email)) {
        showStatusMessage("邮箱格式无效");
        isValid = false;
    }

    // 密码校验
    if (password.isEmpty())  {
        showStatusMessage("密码不能为空");
        isValid = false;
    } else if (!isStrongPassword(password)) {
        showStatusMessage("密码需至少8位且包含字母");
        isValid = false;
    }

    if (code.isEmpty())  {
        showStatusMessage("验证码不能为空");
        isValid = false;
    }

    if (isValid) showStatusMessage("");
    return isValid;
}

// 静态验证方法
bool LoginOrSignUpWidget::isValidEmailFormat(const QString& email)
{
    const QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

bool LoginOrSignUpWidget::isStrongPassword(const QString& password)
{
    const QRegularExpression regex(R"(^(?=.*[a-zA-Z]).{8,}$)");
    return regex.match(password).hasMatch();
}

// 网络请求处理
void LoginOrSignUpWidget::on_ExecuteBtn_clicked() {
    const int currentPage = ui->stackedWidget->currentIndex();
    bool isValid = false;

    // 执行严格校验
    switch (currentPage) {
    case LOGIN_PAGE:  isValid = validateLoginForm(); break;
    case SIGNUP_PAGE: isValid = validateSignupForm(); break;
    case RESET_PAGE:  isValid = validateResetForm(); break;
    }

    if (!isValid) {
        // 若已有错误提示，避免重复动画
        if (ui->StatusTip->text().isEmpty()) {
            showStatusMessage("请完善表单信息");
        }
        return;
    }

    // 发送请求
    switch (currentPage) {
    case LOGIN_PAGE:  sendLoginRequest();  break;
    case SIGNUP_PAGE: sendSignupRequest(); break;
    case RESET_PAGE:  sendResetRequest();  break;
    }
}

void LoginOrSignUpWidget::sendLoginRequest()
{
    QJsonObject payload {
        {"email", ui->EmailEdit_2->text().trimmed()},
        {"password", ui->PwdEdit_2->text().trimmed()}
    };
    HttpClient::GetInstance()->PostHttpReq(LOGIN_API_ENDPOINT, payload, ID_LOGIN, LOGINMOD);
}

void LoginOrSignUpWidget::sendSignupRequest()
{
    QJsonObject payload {
        {"email", ui->EmailEdit->text().trimmed()},
        {"password", ui->PwdEdit->text().trimmed()},
        {"username", ui->NameEdit->text().trimmed()},
        {"verify_code", ui->CodeEdit_2->text().trimmed()}
    };
    HttpClient::GetInstance()->PostHttpReq(SIGNUP_API_ENDPOINT, payload, ID_REG_USER, REGISTMOD);
}

void LoginOrSignUpWidget::sendSignupGetCodeRequest()
{
    if(ui->EmailEdit->text().isEmpty()){
        showStatusMessage("请输入邮箱");
        return;
    }
    QJsonObject payload {
        {"email", ui->EmailEdit->text().trimmed()},
    };
    HttpClient::GetInstance()->PostHttpReq(SIGNUP_GETCODE_API_ENDPOINT, payload, ID_REG_USER_GETCODE, REGISTMOD);
}

void LoginOrSignUpWidget::sendResetRequest()
{
    QJsonObject payload {
        {"email", ui->EmailEdit_3->text().trimmed()},
        {"password", ui->PwdEdit_3->text().trimmed()},
        {"verify_code", ui->CodeEdit->text().trimmed()}
    };
    HttpClient::GetInstance()->PostHttpReq(RESET_API_ENDPOINT, payload, ID_RESET, RESETMOD);
}

void LoginOrSignUpWidget::sendResetGetCodeRequest()
{
    if(ui->EmailEdit_3->text().isEmpty()){
        showStatusMessage("请输入邮箱");
        return;
    }
    QJsonObject payload {
        {"email", ui->EmailEdit_3->text().trimmed()},
    };
    HttpClient::GetInstance()->PostHttpReq(RESET_GETCODE_API_ENDPOINT, payload, ID_RESET_GETCODE, RESETMOD);
}

// 状态提示动画
void LoginOrSignUpWidget::showStatusMessage(const QString& msg)
{
    QMetaObject::invokeMethod(this, [this, msg] {
        // 停止所有进行中的动画
        QList<QAbstractAnimation*> activeAnims = ui->StatusTip->findChildren<QAbstractAnimation*>();
        foreach(QAbstractAnimation* anim, activeAnims) {
            anim->stop();
            anim->deleteLater();
        }

        // 动态样式配置
        const QString color = msg.contains("✓") ? "#2e7d32" : "#c62828";
        ui->StatusTip->setStyleSheet(QString("color: %1; font: 10pt '萍方-繁'; padding: 4px;").arg(color));
        ui->StatusTip->setText(msg);

        // 初始化位置与透明度
        // 强制重置位置
        const QPoint originalPos = ui->StatusTip->property("originalPos").toPoint();
        if(originalPos.isNull()) {
            ui->StatusTip->setProperty("originalPos", ui->StatusTip->pos());
        }
        ui->StatusTip->move(ui->StatusTip->property("originalPos").toPoint());

        QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(ui->StatusTip);
        ui->StatusTip->setGraphicsEffect(opacityEffect);
        opacityEffect->setOpacity(0.0);

        // 复合动画组
        QParallelAnimationGroup *animGroup = new QParallelAnimationGroup(this);

        // 淡入动画
        QPropertyAnimation *fadeIn = new QPropertyAnimation(opacityEffect, "opacity");
        fadeIn->setDuration(500);
        fadeIn->setEasingCurve(QEasingCurve::OutQuint);
        fadeIn->setStartValue(0.0);
        fadeIn->setEndValue(1.0);

        // 平滑位移动画
        QPropertyAnimation *moveDown = new QPropertyAnimation(ui->StatusTip, "pos");
        moveDown->setDuration(500);
        moveDown->setEasingCurve(QEasingCurve::OutQuint);
        moveDown->setStartValue(QPoint(originalPos.x(), originalPos.y() - 10));
        moveDown->setEndValue(originalPos);

        animGroup->addAnimation(fadeIn);
        animGroup->addAnimation(moveDown);
        animGroup->start(QAbstractAnimation::DeleteWhenStopped);

        // 安全自动隐藏（带智能指针保护）
        QPointer<QGraphicsOpacityEffect> safeEffect(opacityEffect);
        QTimer::singleShot(3000, this, [this, safeEffect] {
            if (!safeEffect || !ui->StatusTip) return;

            QPropertyAnimation *fadeOut = new QPropertyAnimation(safeEffect, "opacity");
            fadeOut->setDuration(STATUS_ANIMATION_DURATION);
            fadeOut->setStartValue(1.0);
            fadeOut->setEndValue(0.0);
            fadeOut->setEasingCurve(QEasingCurve::InQuint);
            fadeOut->start(QAbstractAnimation::DeleteWhenStopped);
        });
    }, Qt::QueuedConnection);
}

// 网络响应处理
void LoginOrSignUpWidget::handleHttpResponse(ReqId id, QString res, ErrorCodes err)
{
    // 统一错误处理
    if(err != SUCCESS) {
        showStatusMessage(QString("网络错误: %1").arg(static_cast<int>(err)));
        return;
    }

    // JSON解析
    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(res.toUtf8(),  &jsonError);

    if(id == ReqId::ID_REG_USER || id == ReqId::ID_LOGIN){
        if(jsonError.error  != QJsonParseError::NoError) {
            showStatusMessage("响应数据解析失败");
            return;
        }

        // 处理器分发
        if(m_requestHandlers.contains(id))  {
            m_requestHandlers[id](doc.object());
        }
    }
}


void LoginOrSignUpWidget::on_GetCodeBtn_clicked()
{
    sendResetGetCodeRequest();
}


void LoginOrSignUpWidget::on_GetCodeBtn_2_clicked()
{
    sendSignupGetCodeRequest();
}


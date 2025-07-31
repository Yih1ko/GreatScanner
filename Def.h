#ifndef DEF_H
#define DEF_H

#include <string>
#define _NO_BYTE_DEFINE
#include <Windows.h>
#include <Dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#ifdef _WIN32
#include <dwmapi.h>

// 标准云母效果（主窗口适用）
#define SET_MICA_BACKGROUND(window) \
do { \
            HWND hwnd = reinterpret_cast<HWND>((window)->winId()); \
            if (IsWindow(hwnd)) { \
                MARGINS margins = { -1, 0, 0, 0 }; \
                DwmExtendFrameIntoClientArea(hwnd, &margins); \
                const DWM_SYSTEMBACKDROP_TYPE backdropType = DWMSBT_MAINWINDOW; \
                DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, \
                                                                           &backdropType, sizeof(backdropType)); \
    } \
} while(0)

// 备选云母效果（子窗口/浮动面板适用）
#define SET_MICA_ALT(window) \
    do { \
            HWND hwnd = reinterpret_cast<HWND>((window)->winId()); \
            if (IsWindow(hwnd)) { \
                MARGINS margins = { -1, 0, 0, 0 }; \
                DwmExtendFrameIntoClientArea(hwnd, &margins); \
                const DWM_SYSTEMBACKDROP_TYPE backdropType = DWMSBT_TABBEDWINDOW; \
                DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, \
                                                                           &backdropType, sizeof(backdropType)); \
                COLORREF color = 0x00F2F2F2;   /* 浅灰底色增强对比 */ \
                DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &color, sizeof(color)); \
    } \
} while(0)

#else
#define SET_MICA_BACKGROUND(window) (void)0
#define SET_MICA_ALT(window) (void)0
#endif

//InfoCard文本位置
enum TextPositon{
    UpperLeft, //CardView上方
    LowerLeft,//CardView下方
    RightDes//右侧描述
};

// 文本样式枚举
enum TextStyle
{
    NoStyle = 0x0000,    // 无样式
    Caption = 0x0001,    // 标题样式
    Body = 0x0002,      // 正文样式
    BodyStrong = 0x0003, // 加粗正文样式
    Subtitle = 0x0004,   // 副标题样式
    Title = 0x0005,     // 标题样式
    TitleLarge = 0x0006, // 大标题样式
    Display = 0x0007     // 显示样式（通常用于大号字体）
};


//TCP包头长度
#define TCP_HEAD_LEN 6
//TCP ID长度
#define TCP_ID_LEN 2
//TCP 长度字段的长度
#define TCP_LEN_LEN 4
//最大文件长度
#define MAX_FILE_LEN 2048


//Tcp消息id
enum MsgId {
    ID_TEST_MSG_REQ = 1001,       //测试消息
    ID_TEST_MSG_RSP = 1002,       //测试消息回包
    ID_TEST_UPLOAD_FILE_REQ = 1003,    //发送文件请求
    ID_TEST_UPLOAD_FILE_RSP = 1004,    //发送文件回复
};

//Http请求id
enum ReqId {
    ID_REG_USER = 1040,  //注册用户
    ID_LOGIN = 1041,     //用户登录
    ID_RESET = 1042,      //密码重置
    ID_RESET_GETCODE = 1043, //密码重置-获取验证码
    ID_REG_USER_GETCODE = 1044  //用户注册-获取验证码
};

//所属网络模块
enum Modules {
    REGISTMOD = 0,      //注册模块
    LOGINMOD  = 1,      //登录模块
    RESETMOD  = 2,       //密码重置模块
};

//错误码(RSP是tcp的)
enum ErrorCodes {
    SUCCESS = 0,          //TCP正常回包 & http正常
    RSP_INVALID_ID = 1,       //TCP消息id不正确

    Error_Network = 1019,        //网络错误
    Error_Json = 1020,  //Json解析错误
    EmailExist = 1021,       //邮箱用户已经存在
    RegFailed = 1022,       //注册失败
    PasswdErr = 1023,    //密码错误
    EmailNotMatch = 1024,  //邮箱不匹配密码
    PasswdUpFailed = 1025,  //更新密码失败
    PasswdInvalid = 1026,   //密码更新失败
    EmailNotExist = 1027,   //对于[密码重置]邮箱不存在
    UidInvalid = 1028,  //uid无效
    RedisSetCodeFailed = 1029, //redis设置验证码失败
    EmailSendFailed = 1030, //发送邮件失败
    EmailNotMatchCode = 1031, //邮箱验证码不匹配
};

namespace  ipc {
    enum IpcId {
        Ocr_Default = 0,//ocr to text
        Ocr_Word = 1,   //ocr to word文档
        Ocr_Execl = 2,   //ocr to excel表格
        Ocr_ScreenShot = 4,//截图转文字
        Ocr_ShutDown    //关闭ocr服务进程
    };
    enum ErrorCodes {
        Success = 0,           // 成功
        ImageLoadFailure = 1,  // 图像加载失败
        InvalidImage = 2,      // 无效图像输入
        Mismatch = 3,          // 检测/识别结果数量不匹配
        ModelFailure = 4,     // 模型运行错误
    };
}

enum class HotOperations{
    SCREENSHOT,

};

#endif // DEF_H

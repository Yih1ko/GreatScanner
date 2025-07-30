#include "Systray.h"
#include <qapplication.h>
#include "ocrclient.h"

#pragma execution_character_set("utf-8")    // qt支持显示中文


Systray::Systray(QWidget *parent) : QSystemTrayIcon(parent), m_pParent(parent) {

    initSysTray();		// 初始化托盘
    addSysTrayMenu();	// 初始化菜单

    this->show();		// 显示托盘
}

Systray::~Systray() {
}


// 事件触发
void Systray::onIconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {		// 点击触发
        m_pParent->activateWindow();
        m_pParent->showNormal();

    } else if (reason == QSystemTrayIcon::Context) {	// 右键触发
        trayMenu->exec(QCursor::pos());
    }
}



// 初始化
void Systray::initSysTray() {
    setToolTip(tr(":/GreatScanner Client"));		// 设置工具提示
    setIcon(QIcon(tr(":/icons/cat.jpg")));	// 设置托盘图标

    // 点击托盘所作的操作
    connect(this, &Systray::activated, this, &Systray::onIconActivated);

    // 点击消息框后显示主页面
    connect(this, &Systray::messageClicked, [this] {
        m_pParent->activateWindow();
        m_pParent->showNormal();
    });

    // 显示系统托盘消息框
    connect(this, &Systray::showTrayMessage, this, &Systray::showMessage);
}



// 菜单
void Systray::addSysTrayMenu() {
    trayMenu = new QMenu(m_pParent);		// 新建菜单，指定父对象
    // 新建菜单子项
    QAction *action1 = new QAction(trayMenu);
    QAction *action2 = new QAction(trayMenu);

    action1->setIcon(QIcon(":/icons/cat.jpg"));	// 设置菜单图标
    action1->setText(QString("Show"));		// 设置带单文本
    action2->setIcon(QIcon(":/icons/quit.png"));
    action2->setText(QString("Quit"));


    // 添加菜单子项
    trayMenu->addAction(action1);
    trayMenu->addAction(action2);

    // 显示窗体
    connect(action1, &QAction::triggered, [this](bool) {
        m_pParent->activateWindow();
        m_pParent->showNormal();
    });

    // 退出
    connect(action2, &QAction::triggered, []() {
        qApp->quit(); // 退出整个应用程序
    });
}


// 消息框
void Systray::showMessage() {
    QSystemTrayIcon::showMessage("Notice",				// 消息窗口标题
                                 "The form is minimized！",// 消息内容
                                 QSystemTrayIcon::MessageIcon::Information,// 消息窗口图标
                                 1000);					// 消息窗口显示时长(经测试，win10系统下，没有效果)
}

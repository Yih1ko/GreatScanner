#pragma once

#include <QSystemTrayIcon>
#include <QWidget>
#include <QMenu>

class Systray : public QSystemTrayIcon {
    Q_OBJECT

public:
    Systray(QWidget *parent);
    ~Systray();


private slots:
    void onIconActivated(QSystemTrayIcon::ActivationReason reason);	// 处理事件函数

private:
    void initSysTray();		// 初始化托盘
    void addSysTrayMenu();	// 添加菜单
    void showMessage();		// 显示消息框


signals:
    void showTrayMessage();		// 触发显示系统托盘消息框

private:
    QWidget *m_pParent;	// 父类，用于显示窗体
    QMenu *trayMenu;	// 菜单
};

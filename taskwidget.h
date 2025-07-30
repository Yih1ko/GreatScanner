#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include "roundprogressbar.h"
#include "infocard.h"
#include "fileuploadqueue.h"
#include "tasklist.h"
#include <QScrollArea>
#include <QScrollBar>
#include <QQueue>
#include <QListWidget>
#include <qapplication.h>

class GlobalEventFilter : public QObject {
    Q_OBJECT
public:
    explicit GlobalEventFilter(QObject *parent = nullptr) : QObject(parent) {}

protected:
    bool eventFilter(QObject *watched, QEvent *event) override {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            // 获取当前活动窗口
            QWidget *activeWindow = qApp->activeWindow();
            if (activeWindow) {
                // 获取鼠标全局位置
                QPoint pos = mouseEvent->globalPos();
                // 判断点击位置是否在TaskList之外
                if (!activeWindow->rect().contains(activeWindow->mapFromGlobal(pos))) {
                    // 获取当前活动窗口中的TaskList实例
                    TaskList *taskList = activeWindow->findChild<TaskList *>("TaskList");
                    if (taskList) {
                        taskList->clearSelection();
                    }
                }
            }
        }
        return QObject::eventFilter(watched, event);
    }
};

//任务(文件)上传客户端
class TaskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TaskWidget(QWidget *parent = nullptr);
    // void pauseUpload();     //暂停上传
signals:
    void sig_allDone(bool);

public slots:
    //上传进度槽函数
    void slot_uploadProgress(int progress);
    //队尾新增任务卡片
    void slot_appendTask(quint16 id, const QString &filePath);
    //弹出最顶端的任务卡片
    void slot_popTask();

private:
    RoundProgressBar* m_progressBar;        //进度条
    TaskList* m_taskList;                //上传队列界面
    //GlobalEventFilter *m_eventFilter;
    // FileUploadQueue::getInstance();         //队列上传器(单例)

    //bool m_pausing;                     //暂停中(未实现)

    void initUI();
    void initSlots();


};

#endif // TASKWIDGET_H

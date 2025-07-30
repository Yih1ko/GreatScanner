#include "tasklist.h"

TaskList::TaskList(QWidget *parent)
{
    setObjectName("TaskList");
    setSizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);
    setContentsMargins(0, 0, 0, 0);
    setSpacing(0);
    //禁用默认滚动条
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setStyleSheet(R"(
        QScrollBar:vertical {
            background: transparent; /* 将轨道背景设置为透明 */
            width: 5px; /* 滚动条宽度，根据需要调整 */
            margin: 0px; /* 移除滚动条与滑块之间的间距 */
        }

        QScrollBar::handle:vertical {
            background: rgb(217, 217, 217); /* 滑块颜色 */
            min-height: 10px; /* 滑块最小高度，根据需要调整 */
            border-radius: 2px; /* 滑块边缘圆角，调整以形成椭圆形状 */
        }

        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px; /* 移除上下按钮 */
            border: none; /* 移除边框 */
            background: transparent; /* 背景透明 */
        }

        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
            background: none; /* 页面滚动部分背景透明 */
        }

        QListWidget {
            background: transparent;
            border: none;
        }

        QListWidget::item:selected {
            background-color: transparent/*white*/;
            border: none;
            outline: none;
        }

        QListWidget::item:hover {
            background-color: transparent;
            border: none;
            outline: none;
        }

        QListWidget::focus {
            border: none;
            outline: none;
        }
    )");

    //安装事件过滤
    this->viewport()->installEventFilter(this);
}

void TaskList::popRow()
{
    // 获取最顶端的项
    QListWidgetItem* topItem = item(0);
    if (topItem != nullptr) {
        // 获取关联的widget
        QWidget* topWidget = itemWidget(topItem);
        if (topWidget != nullptr) {
            // 删除widget并断开关联
            delete topWidget;
            removeItemWidget(topItem);
        }

        // 移除并删除项
        takeItem(0);
        delete topItem;

        qDebug() << "Top item and its widget have been deleted successfully."<<"count:"<<count();
    } else {
        qDebug() << "No items to delete.";
    }
}

bool TaskList::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否是鼠标悬浮进入或离开
    if (watched == this->viewport()) {
        if (event->type() == QEvent::Enter) {
            // 鼠标悬浮，显示滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if (event->type() == QEvent::Leave) {
            // 鼠标离开，隐藏滚动条
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }

    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        //动态加载内容(暂未实现)

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched, event);
}

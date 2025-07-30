#ifndef TASKLIST_H
#define TASKLIST_H

#include <QListWidget>
#include <QWheelEvent>
#include <QScrollBar>
#include <QEvent>
#include <QMap>
#include "tcpclient.h"
#include "rightbtnmenu.h"

/*需求:
 * TaskList滚动条样式实现
 * TaskList滚动条是否显示取决于鼠标是否位于TaskList中
*/
class TaskList : public QListWidget
{
    Q_OBJECT
public:
    TaskList(QWidget* parent = nullptr);
public slots:
    void popRow();
    //void
protected:
    bool eventFilter(QObject* watched, QEvent *event) override;

private:
    RightBtnMenu *m_contextMenu;    //右键菜单
    QAction *m_pauseAction;         //右键菜单-暂停传输
    QAction *m_attributeAction;         //右键菜单-task属性

    QMap<int, bool> m_isPausingMap;     //index对应task是否暂停
};

#endif // TASKLIST_H

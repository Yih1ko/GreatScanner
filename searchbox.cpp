#include "searchbox.h"
#include <QFocusEvent>
#include <QApplication>
#include <QMouseEvent>
#include "autocomplete.h"

SearchBox::SearchBox(QWidget *parent)
    : QLineEdit(parent)
{
    // 安装事件过滤器
    installEventFilter(this);

    // 设置焦点检查定时器（50ms延迟足够处理焦点切换）
    connect(&m_focusCheckTimer, &QTimer::timeout, this, &SearchBox::checkFocusState);
    m_focusCheckTimer.setSingleShot(true);
}

void SearchBox::setAutoComplete(AutoComplete* autoCompleteWidget)
{
    m_autoComplete = autoCompleteWidget;
    if (m_autoComplete) {
        // 为自动补全窗口安装事件过滤器
        m_autoComplete->installEventFilter(this);
    }
}

void SearchBox::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);

    // 处理首次聚焦
    if (m_isFirstFocus) {
        m_isFirstFocus = false;
        emit reactivated();  // 强制触发首次激活
        return;
    }

    // 正常重新激活逻辑
    if (m_hasLostFocus) {
        emit reactivated();
        m_hasLostFocus = false;
    }
}
void SearchBox::focusOutEvent(QFocusEvent *event) {
    QLineEdit::focusOutEvent(event);
    m_focusCheckTimer.start(50);
    m_hasLostFocus = true;  // 记录失焦状态
}

void SearchBox::checkFocusState()
{
    QWidget* focusedWidget = QApplication::focusWidget();

    // 检查焦点是否在搜索框或自动补全窗口上
    bool focusInSearchBox = (focusedWidget == this);
    bool focusInAutoComplete = (m_autoComplete &&
                                (focusedWidget == m_autoComplete ||
                                 m_autoComplete->isAncestorOf(focusedWidget)));

    // 如果焦点既不在搜索框也不在自动补全窗口，则视为真正失焦
    if (!focusInSearchBox && !focusInAutoComplete) {
        emit lostFocus();
        m_hasLostFocus = true;
        qDebug() << "SearchBox失焦 - autocomplete隐藏";
    }
}

bool SearchBox::eventFilter(QObject *watched, QEvent *event)
{
    // 处理自动补全窗口的鼠标按下事件
    if (event->type() == QEvent::MouseButtonPress &&
        watched == m_autoComplete)
    {
        // 点击自动补全窗口时保持搜索框焦点
        this->setFocus();
        return true;  // 事件已处理
    }

    return QLineEdit::eventFilter(watched, event);
}

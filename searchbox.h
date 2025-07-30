#ifndef SEARCHBOX_H
#define SEARCHBOX_H

#include <QLineEdit>
#include <QWidget>
#include <QTimer>

class AutoComplete;  // 前置声明

class SearchBox : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchBox(QWidget *parent = nullptr);

    // 设置自动补全窗口指针
    void setAutoComplete(AutoComplete* autoCompleteWidget);

signals:
    void lostFocus();       // 失焦信号（点击外部时触发）
    void reactivated();     // 重新激活信号（仅触发一次）

protected:
    void focusOutEvent(QFocusEvent *event) override;
    void focusInEvent(QFocusEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    bool m_hasLostFocus = false;
    bool m_isFirstFocus = true;
    AutoComplete* m_autoComplete = nullptr;  // 自动补全窗口指针
    QTimer m_focusCheckTimer;                 // 焦点检查定时器

private slots:
    void checkFocusState();  // 焦点状态检查
};

#endif // SEARCHBOX_H

#ifndef GLOBALKEYEVENTMGR_H
#define GLOBALKEYEVENTMGR_H

#include <QObject>
#include <windows.h>
#include <QHash>
#include <QAbstractNativeEventFilter>
#include <QCoreApplication>

class GlobalKeyEventMgr : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    explicit GlobalKeyEventMgr(QObject *parent = nullptr);
    ~GlobalKeyEventMgr();
    bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) override;
signals:
    void triggered(QString operation);
private:
    QStringList m_operations;
    QVector<QVector<unsigned int>> m_keyCombs;
    int m_hotKeyId{1};
    QHash<int, QString> m_hotkeyIdToOperation; // 热键ID与操作映射
    bool m_running = true;

    /**
 * @brief 将Qt::Key枚举转换为Windows虚拟键码（VK_*）
 * @param qtKey Qt的按键枚举值
 * @return 对应的Windows虚拟键码，若未映射则返回0
 */
    static unsigned int qtKeyToWinVK(int qtKey);
    UINT toModifierConstant(unsigned int vk);
    void initOperations();
};

#endif // GLOBALKEYEVENTMGR_H

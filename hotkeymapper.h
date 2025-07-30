// #ifndef HOTKEYMAPPER_H
// #define HOTKEYMAPPER_H

// #include <QHash>
// #include <QVector>
// #include <QString>
// #include <QStringList>
// #include <QFile>
// #include <QJsonDocument>
// #include <QJsonObject>
// #include <QJsonArray>
// #include <QDebug>
// #include <initializer_list>
// #include <QMutex>
// #include <QMutexLocker>
// #include <QFuture>
// #include <QtConcurrent>
// #include <atomic>
// #include "_singleton.h"
// #include "hotkeycallbackregistry.h"

// class HotkeyMapper: public QObject, public _Singleton<HotkeyMapper>, public std::enable_shared_from_this<HotkeyMapper>{
//     Q_OBJECT
// public:
//     ~HotkeyMapper(){}
//     HotkeyCallbackRegistry& callbackRegistry() { return m_callbackRegistry; }
//     // 同步加载配置文件（线程安全）
//     bool loadConfig(const QString& configPath) {
//         QMutexLocker locker(&m_mutex);
//         return loadConfigInternal(configPath);
//     }

//     // 异步加载配置文件（非阻塞）
//     QFuture<bool> loadConfigAsync(const QString& configPath) {
//         std::weak_ptr<HotkeyMapper> weakSelf = shared_from_this();
//         return QtConcurrent::run([weakSelf, configPath] {
//             if (auto self = weakSelf.lock())  {
//                 QMutexLocker locker(&self->m_mutex);
//                 bool success = self->loadConfigInternal(configPath);
//                 self->m_configLoaded.store(success,  std::memory_order_release);
//                 return success;
//             }
//             qWarning() << "对象已销毁，操作取消";
//             return false;
//         });
//     }

//     // 保存配置到文件（线程安全）
//     bool saveConfig(const QString& configPath) const {
//         QMutexLocker locker(&m_mutex);

//         QFile file(configPath);
//         if (!file.open(QIODevice::WriteOnly))  {
//             qWarning() << "无法打开配置文件用于写入:" << configPath;
//             return false;
//         }

//         // 原子读取配置加载状态
//         if (!m_configLoaded/*.load(std::memory_order_acquire)*/)  {
//             qWarning() << "配置未加载，保存操作被拒绝";
//             return false;
//         }

//         QJsonObject rootObject;
//         for (auto it = m_operationMappings.begin();  it != m_operationMappings.end();  ++it) {
//             QJsonArray keyArray;
//             for (const QString& key : it.value())  {
//                 keyArray.append(key);
//             }
//             rootObject[it.key()] = keyArray;
//         }

//         QJsonDocument doc(rootObject);
//         return file.write(doc.toJson())  > 0;
//     }

//     // 获取操作对应的键枚举数组（线程安全）
//     QVector<Qt::Key> getKeyEnums(const QString& operation) const {
//         QMutexLocker locker(&m_mutex);

//         // 原子检查配置状态
//         if (!m_configLoaded.load(std::memory_order_acquire))  {
//             qWarning() << "配置未加载，操作被拒绝";
//             return QVector<Qt::Key>();
//         }

//         auto it = m_operationMappings.find(operation);
//         if (it == m_operationMappings.end())  {
//             qWarning() << "未找到操作映射:" << operation;
//             return QVector<Qt::Key>();
//         }

//         const QStringList& keyStrings = it.value();
//         QVector<Qt::Key> result;
//         result.reserve(keyStrings.size());

//         for (const QString& keyStr : keyStrings) {
//             QString normalized = keyStr.simplified().toCaseFolded();
//             auto keyIt = m_keyMappings.find(normalized);
//             if (keyIt != m_keyMappings.end())  {
//                 result.append(keyIt.value());
//             } else {
//                 qWarning() << "未找到键映射:" << keyStr;
//                 result.append(Qt::Key_unknown);
//             }
//         }
//         return result;
//     }

//     // 获取所有已定义的操作（线程安全）
//     QStringList getDefinedOperations() const {
//         QMutexLocker locker(&m_mutex);

//         // 检查配置状态
//         if (!m_configLoaded.load(std::memory_order_acquire))  {
//             return QStringList();
//         }
//         return m_operationMappings.keys();
//     }

//     // 检查配置是否已加载（无锁原子操作）
//     bool isConfigLoaded() const {
//         return m_configLoaded.load(std::memory_order_acquire);
//     }

//     // 设置键映射（线程安全）
//     template<typename... Args>
//     void setKeyMapping(const QString& operation, Args... keys) {
//         QStringList keyList;
//         for (const auto& key : {keys...}) {
//             keyList.append(QString(key));
//         }
//         setKeyMapping(operation, keyList);
//     }

//     void setKeyMapping(const QString& operation, const QStringList& keys) {
//         QMutexLocker locker(&m_mutex);
//         setKeyMappingInternal(operation, keys);
//     }

//     void setKeyMapping(const QString& operation, std::initializer_list<const char*> keys) {
//         QStringList keyList;
//         for (const auto& key : keys) {
//             keyList.append(QString(key));
//         }
//         setKeyMapping(operation, keyList);
//     }

//     // 重新加载当前配置文件（热更新）
//     bool reloadConfig() {
//         QMutexLocker locker(&m_mutex);

//         // 原子检查配置状态
//         if (!m_configLoaded.load(std::memory_order_acquire))  {
//             qWarning() << "配置未加载，重载操作被拒绝";
//             return false;
//         }

//         if (m_currentConfigPath.isEmpty())  {
//             qWarning() << "未记录配置文件路径，无法重新加载";
//             return false;
//         }

//         qDebug() << "正在热更新配置文件:" << m_currentConfigPath;
//         return loadConfigInternal(m_currentConfigPath);
//     }

//     // 异步重新加载
//     QFuture<bool> reloadConfigAsync() {
//         std::weak_ptr<HotkeyMapper> weakSelf = shared_from_this();
//         return QtConcurrent::run([weakSelf] {
//             if (auto self = weakSelf.lock()){
//                 QMutexLocker locker(&self->m_mutex);
//                 bool success = self->reloadConfig();
//                 self->m_configLoaded.store(success,  std::memory_order_release);
//                 return success;
//             }
//             return false;
//         });
//     }
// signals:  // 新增信号
//     void shortcutChanged(const QString& operation);

// private:
//     friend class _Singleton<HotkeyMapper>;
//     explicit HotkeyMapper(/*const QString& configPath = ""*/): m_configLoaded(false)
//     {
//         initDefaultKeyMappings();
//         // m_configLoaded.store(false,  std::memory_order_release);
//         // QMutexLocker locker(&m_mutex);
//         // if (!configPath.isEmpty())  {
//         //     loadConfigInternal(configPath); // 直接调用内部实现
//         // }
//     }
//     // 内部加载实现
//     bool loadConfigInternal(const QString& configPath) {
//         QFile file(configPath);
//         if (!file.open(QIODevice::ReadOnly))  {
//             qWarning() << "无法打开配置文件:" << configPath;
//             return false;
//         }

//         QJsonParseError parseError;
//         QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),  &parseError);
//         if (parseError.error  != QJsonParseError::NoError) {
//             qWarning() << "JSON解析错误:" << parseError.errorString();
//             return false;
//         }

//         if (!doc.isObject())  {
//             qWarning() << "配置文件格式错误: 根元素应为对象";
//             return false;
//         }

//         parseJsonConfig(doc.object());

//         // 记录当前配置文件路径
//         m_currentConfigPath = configPath;

//         // 原子标记配置已加载
//         m_configLoaded.store(true,  std::memory_order_release);
//         return true;
//     }

//     // 内部设置键映射
//     void setKeyMappingInternal(const QString& operation, const QStringList& keys) {
//         if (keys.isEmpty())  {
//             qWarning() << "操作" << operation << "的键序列为空，将被忽略";
//             return;
//         }

//         for (const QString& key : keys) {
//             QString normalized = key.simplified().toCaseFolded();
//             if (!m_keyMappings.contains(normalized))  {
//                 qWarning() << "键映射无效:" << key << "，操作" << operation << "的映射未设置";
//                 m_operationMappings.remove(operation);  // 清除残留项
//                 return;
//             }
//         }

//         m_operationMappings[operation] = keys;
//         m_callbackRegistry.bindShortcut(operation,  keys);
//         emit shortcutChanged(operation);
//         qDebug() << "已更新操作映射:" << operation << "->" << keys;

//         emit shortcutChanged(operation);
//     }

//     // 初始化默认键映射
//     void initDefaultKeyMappings() {
//         // 字母键
//         for (char c = 'a'; c <= 'z'; ++c) {
//             QString key = QString(c);
//             m_keyMappings[key] = static_cast<Qt::Key>(Qt::Key_A + (c - 'a'));
//         }

//         // 数字键
//         for (char c = '0'; c <= '9'; ++c) {
//             QString key = QString(c);
//             m_keyMappings[key] = static_cast<Qt::Key>(Qt::Key_0 + (c - '0'));
//         }

//         // 功能键
//         const QVector<QString> fKeys = {"f1", "f2", "f3", "f4", "f5", "f6",
//                                         "f7", "f8", "f9", "f10", "f11", "f12"};
//         for (int i = 0; i < fKeys.size();  ++i) {
//             m_keyMappings[fKeys[i]] = static_cast<Qt::Key>(Qt::Key_F1 + i);
//         }

//         // 特殊键
//         m_keyMappings.insert("ctrl",  Qt::Key_Control);
//         m_keyMappings.insert("control",  Qt::Key_Control);
//         m_keyMappings.insert("alt",  Qt::Key_Alt);
//         m_keyMappings.insert("shift",  Qt::Key_Shift);
//         m_keyMappings.insert("tab",  Qt::Key_Tab);
//         m_keyMappings.insert("enter",  Qt::Key_Return);
//         m_keyMappings.insert("return",  Qt::Key_Return);
//         m_keyMappings.insert("escape",  Qt::Key_Escape);
//         m_keyMappings.insert("space",  Qt::Key_Space);
//         m_keyMappings.insert("backspace",  Qt::Key_Backspace);
//         m_keyMappings.insert("delete",  Qt::Key_Delete);
//         m_keyMappings.insert("insert",  Qt::Key_Insert);
//         m_keyMappings.insert("home",  Qt::Key_Home);
//         m_keyMappings.insert("end",  Qt::Key_End);
//         m_keyMappings.insert("pageup",  Qt::Key_PageUp);
//         m_keyMappings.insert("pagedown",  Qt::Key_PageDown);
//         m_keyMappings.insert("up",  Qt::Key_Up);
//         m_keyMappings.insert("down",  Qt::Key_Down);
//         m_keyMappings.insert("left",  Qt::Key_Left);
//         m_keyMappings.insert("right",  Qt::Key_Right);
//         m_keyMappings.insert("capslock",  Qt::Key_CapsLock);
//         m_keyMappings.insert("numlock",  Qt::Key_NumLock);
//         m_keyMappings.insert("scrolllock",  Qt::Key_ScrollLock);
//         m_keyMappings.insert("printscreen",  Qt::Key_Print);
//         m_keyMappings.insert("pause",  Qt::Key_Pause);
//         m_keyMappings.insert("menu",  Qt::Key_Menu);
//         m_keyMappings.insert("meta",  Qt::Key_Meta);
//         m_keyMappings.insert("super",  Qt::Key_Super_L);
//     }

//     // 解析JSON配置
//     void parseJsonConfig(const QJsonObject& config) {
//         // 预分配内存提高性能
//         m_operationMappings.reserve(config.size());

//         for (auto it = config.begin();  it != config.end();  ++it) {
//             QString operation = it.key();
//             QJsonValue value = it.value();

//             if (value.isArray())  {
//                 QJsonArray array = value.toArray();
//                 QStringList keyList;
//                 keyList.reserve(array.size());

//                 for (const QJsonValue& item : array) {
//                     if (item.isString())  {
//                         keyList.append(item.toString());
//                     } else {
//                         qWarning() << operation << "含非字符串键值";
//                     }
//                 }

//                 if (!keyList.isEmpty())  {
//                     m_operationMappings[operation] = keyList;
//                 } else {
//                     qWarning() << "操作" << operation << "的键序列为空，将被忽略";
//                 }
//             } else {
//                 qWarning() << "操作" << operation << "的值不是数组，将被忽略";
//             }
//         }
//     }

//     // 线程安全机制
//     mutable QMutex m_mutex; // 互斥锁保护共享数据
//     std::atomic<bool> m_configLoaded{false}; // 原子标志位
//     QString m_currentConfigPath;

//     // 键映射数据
//     QHash<QString, Qt::Key> m_keyMappings;
//     QHash<QString, QStringList> m_operationMappings;

//     HotkeyCallbackRegistry m_callbackRegistry;
// };
// #endif // HOTKEYMAPPER_H

//暂时只用这个单线程版(多线程未经测试优化)：
#ifndef HOTKEYMAPPER_H
#define HOTKEYMAPPER_H

#include <QHash>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <initializer_list>
#include "_singleton.h"

class HotkeyMapper: public QObject, public _Singleton<HotkeyMapper> {
    Q_OBJECT
public:
    ~HotkeyMapper() {}

    // 同步加载配置文件（单线程版）
    bool loadConfig(const QString& configPath) {
        QFile file(configPath);
        if (!file.open(QIODevice::ReadOnly))  {
            qWarning() << "无法打开配置文件:" << configPath;
            return false;
        }

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(),  &parseError);
        if (parseError.error  != QJsonParseError::NoError) {
            qWarning() << "JSON解析错误:" << parseError.errorString();
            return false;
        }

        if (!doc.isObject())  {
            qWarning() << "配置文件格式错误: 根元素应为对象";
            return false;
        }

        parseJsonConfig(doc.object());
        m_currentConfigPath = configPath;
        m_configLoaded = true;  // 直接设置状态
        return true;
    }

    // 保存配置到文件
    bool saveConfig(const QString& configPath) const {
        if (!m_configLoaded) {
            qWarning() << "配置未加载，保存操作被拒绝";
            return false;
        }

        QFile file(configPath);
        if (!file.open(QIODevice::WriteOnly))  {
            qWarning() << "无法打开配置文件用于写入:" << configPath;
            return false;
        }

        QJsonObject rootObject;
        for (auto it = m_operationMappings.begin();  it != m_operationMappings.end();  ++it) {
            QJsonArray keyArray;
            for (const QString& key : it.value())  {
                keyArray.append(key);
            }
            rootObject[it.key()] = keyArray;
        }

        QJsonDocument doc(rootObject);
        return file.write(doc.toJson())  > 0;
    }

    // 获取操作对应的键枚举数组
    QVector<Qt::Key> getKeyEnums(const QString& operation) const {
        if (!m_configLoaded) {
            qWarning() << "配置未加载，操作被拒绝";
            return QVector<Qt::Key>();
        }

        auto it = m_operationMappings.find(operation);
        if (it == m_operationMappings.end())  {
            qWarning() << "未找到操作映射:" << operation;
            return QVector<Qt::Key>();
        }

        const QStringList& keyStrings = it.value();
        QVector<Qt::Key> result;
        result.reserve(keyStrings.size());

        for (const QString& keyStr : keyStrings) {
            QString normalized = keyStr.simplified().toCaseFolded();
            auto keyIt = m_keyMappings.find(normalized);
            if (keyIt != m_keyMappings.end())  {
                result.append(keyIt.value());
            } else {
                qWarning() << "未找到键映射:" << keyStr;
                result.append(Qt::Key_unknown);
            }
        }
        return result;
    }

    // 获取所有已定义的操作
    QStringList getDefinedOperations() const {
        if (!m_configLoaded) {
            return QStringList();
        }
        return m_operationMappings.keys();
    }

    // 检查配置是否已加载
    bool isConfigLoaded() const { return m_configLoaded; }

    // 设置键映射
    template<typename... Args>
    void setKeyMapping(const QString& operation, Args... keys) {
        QStringList keyList;
        for (const auto& key : {keys...}) {
            keyList.append(QString(key));
        }
        setKeyMapping(operation, keyList);
    }

    void setKeyMapping(const QString& operation, const QStringList& keys) {
        if (keys.isEmpty())  {
            qWarning() << "操作" << operation << "的键序列为空，将被忽略";
            return;
        }

        for (const QString& key : keys) {
            QString normalized = key.simplified().toCaseFolded();
            if (!m_keyMappings.contains(normalized))  {
                qWarning() << "键映射无效:" << key << "，操作" << operation << "的映射未设置";
                m_operationMappings.remove(operation);
                return;
            }
        }

        m_operationMappings[operation] = keys;

        emit shortcutChanged(operation);
        qDebug() << "已更新操作映射:" << operation << "->" << keys;
    }

    void setKeyMapping(const QString& operation, std::initializer_list<const char*> keys) {
        QStringList keyList;
        for (const auto& key : keys) {
            keyList.append(QString(key));
        }
        setKeyMapping(operation, keyList);
    }

    // 重新加载当前配置文件（热更新）
    bool reloadConfig() {
        if (!m_configLoaded || m_currentConfigPath.isEmpty())  {
            qWarning() << "配置未加载或路径为空，重载操作被拒绝";
            return false;
        }
        qDebug() << "正在热更新配置文件:" << m_currentConfigPath;
        return loadConfig(m_currentConfigPath);
    }

signals:
    void shortcutChanged(const QString& operation);

private:
    friend class _Singleton<HotkeyMapper>;
    explicit HotkeyMapper() : m_configLoaded(false) {
        initDefaultKeyMappings();
    }

    // 初始化默认键映射
    void initDefaultKeyMappings() {
        // 字母键
        for (char c = 'a'; c <= 'z'; ++c) {
            QString key = QString(c);
            m_keyMappings[key] = static_cast<Qt::Key>(Qt::Key_A + (c - 'a'));
        }

        // 数字键
        for (char c = '0'; c <= '9'; ++c) {
            QString key = QString(c);
            m_keyMappings[key] = static_cast<Qt::Key>(Qt::Key_0 + (c - '0'));
        }

        // 功能键
        const QVector<QString> fKeys = {"f1", "f2", "f3", "f4", "f5", "f6",
                                        "f7", "f8", "f9", "f10", "f11", "f12"};
        for (int i = 0; i < fKeys.size();  ++i) {
            m_keyMappings[fKeys[i]] = static_cast<Qt::Key>(Qt::Key_F1 + i);
        }

        // 特殊键
        m_keyMappings.insert("ctrl",  Qt::Key_Control);
        m_keyMappings.insert("control",  Qt::Key_Control);
        m_keyMappings.insert("alt",  Qt::Key_Alt);
        m_keyMappings.insert("shift",  Qt::Key_Shift);
        m_keyMappings.insert("tab",  Qt::Key_Tab);
        m_keyMappings.insert("enter",  Qt::Key_Return);
        m_keyMappings.insert("return",  Qt::Key_Return);
        m_keyMappings.insert("escape",  Qt::Key_Escape);
        m_keyMappings.insert("space",  Qt::Key_Space);
        m_keyMappings.insert("backspace",  Qt::Key_Backspace);
        m_keyMappings.insert("delete",  Qt::Key_Delete);
        m_keyMappings.insert("insert",  Qt::Key_Insert);
        m_keyMappings.insert("home",  Qt::Key_Home);
        m_keyMappings.insert("end",  Qt::Key_End);
        m_keyMappings.insert("pageup",  Qt::Key_PageUp);
        m_keyMappings.insert("pagedown",  Qt::Key_PageDown);
        m_keyMappings.insert("up",  Qt::Key_Up);
        m_keyMappings.insert("down",  Qt::Key_Down);
        m_keyMappings.insert("left",  Qt::Key_Left);
        m_keyMappings.insert("right",  Qt::Key_Right);
        m_keyMappings.insert("capslock",  Qt::Key_CapsLock);
        m_keyMappings.insert("numlock",  Qt::Key_NumLock);
        m_keyMappings.insert("scrolllock",  Qt::Key_ScrollLock);
        m_keyMappings.insert("printscreen",  Qt::Key_Print);
        m_keyMappings.insert("pause",  Qt::Key_Pause);
        m_keyMappings.insert("menu",  Qt::Key_Menu);
        m_keyMappings.insert("meta",  Qt::Key_Meta);
        m_keyMappings.insert("super",  Qt::Key_Super_L);
    }

    // 解析JSON配置
    void parseJsonConfig(const QJsonObject& config) {
        m_operationMappings.clear();// 清空旧映射
        m_operationMappings.reserve(config.size());

        for (auto it = config.begin();  it != config.end();  ++it) {
            QString operation = it.key();
            QJsonValue value = it.value();

            if (value.isArray())  {
                QJsonArray array = value.toArray();
                QStringList keyList;
                keyList.reserve(array.size());

                for (const QJsonValue& item : array) {
                    if (item.isString())  {
                        keyList.append(item.toString());
                    } else {
                        qWarning() << operation << "含非字符串键值";
                    }
                }

                if (!keyList.isEmpty())  {
                    m_operationMappings[operation] = keyList;
                } else {
                    qWarning() << "操作" << operation << "的键序列为空，将被忽略";
                }
            } else {
                qWarning() << "操作" << operation << "的值不是数组，将被忽略";
            }
        }
    }

    // 成员变量
    bool m_configLoaded = false;
    QString m_currentConfigPath;

    // 键映射数据
    QHash<QString, Qt::Key> m_keyMappings;
    QHash<QString, QStringList> m_operationMappings;
};
#endif // HOTKEYMAPPER_H

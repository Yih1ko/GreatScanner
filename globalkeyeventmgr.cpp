#include "globalkeyeventmgr.h"
#include "hotkeymapper.h"

GlobalKeyEventMgr::GlobalKeyEventMgr(QObject *parent)
    : QObject{parent}
{
    qApp->installNativeEventFilter(this);
    qDebug()<<"GlobalKeyEventMgr init";
    initOperations();
}

GlobalKeyEventMgr::~GlobalKeyEventMgr() {
    qApp->removeNativeEventFilter(this);
    for(auto it = m_hotkeyIdToOperation.begin();  it != m_hotkeyIdToOperation.end();  ++it) {
        UnregisterHotKey(nullptr, it.key());
    }
}

bool GlobalKeyEventMgr::nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result) {
    if (eventType == "windows_generic_MSG") {
        MSG* msg = static_cast<MSG*>(message);
        if(msg->message == WM_HOTKEY) {
            const int hotkeyId = static_cast<int>(msg->wParam);
            if(const QString op = m_hotkeyIdToOperation.value(hotkeyId);  !op.isEmpty()){
                emit triggered(op);
                if (result) { // 检查空指针
                    *result = 0;
                }
                return true;
            }
        }
    }
    return false;
}

unsigned int GlobalKeyEventMgr::qtKeyToWinVK(int qtKey) {
    // 字母和数字键
    if ((qtKey >= Qt::Key_A && qtKey <= Qt::Key_Z) ||
        (qtKey >= Qt::Key_0 && qtKey <= Qt::Key_9)) {
        return toupper(qtKey);
    }

    // 功能键 F1-F24
    if (qtKey >= Qt::Key_F1 && qtKey <= Qt::Key_F24) {
        return VK_F1 + (qtKey - Qt::Key_F1);
    }

    // 扩展键位映射表
    static const QHash<int, int> keyMap = {
        // ==== 基础控制键 ====
        {Qt::Key_Escape, VK_ESCAPE},
        {Qt::Key_Tab, VK_TAB},
        {Qt::Key_Backspace, VK_BACK},
        {Qt::Key_Return, VK_RETURN},
        {Qt::Key_Enter, VK_RETURN},
        {Qt::Key_Shift, VK_SHIFT},
        {Qt::Key_Control, VK_CONTROL},
        {Qt::Key_Alt, VK_MENU},
        {Qt::Key_Space, VK_SPACE},
        {Qt::Key_CapsLock, VK_CAPITAL},
        {Qt::Key_NumLock, VK_NUMLOCK},
        {Qt::Key_ScrollLock, VK_SCROLL},
        {Qt::Key_Print, VK_SNAPSHOT},
        {Qt::Key_Pause, VK_PAUSE},

        // ==== 方向键 ====
        {Qt::Key_Left, VK_LEFT},
        {Qt::Key_Right, VK_RIGHT},
        {Qt::Key_Up, VK_UP},
        {Qt::Key_Down, VK_DOWN},

        // ==== 编辑键 ====
        {Qt::Key_Insert, VK_INSERT},
        {Qt::Key_Delete, VK_DELETE},
        {Qt::Key_Home, VK_HOME},
        {Qt::Key_End, VK_END},
        {Qt::Key_PageUp, VK_PRIOR},
        {Qt::Key_PageDown, VK_NEXT},

        // ==== 符号键 ====
        {Qt::Key_Comma, VK_OEM_COMMA},        // ,
        {Qt::Key_Period, VK_OEM_PERIOD},      // .
        {Qt::Key_Minus, VK_OEM_MINUS},        // -
        {Qt::Key_Plus, VK_OEM_PLUS},          // +
        {Qt::Key_Slash, VK_OEM_2},            // /
        {Qt::Key_Backslash, VK_OEM_5},        // '\'
        {Qt::Key_Semicolon, VK_OEM_1},        // ;
        {Qt::Key_QuoteLeft, VK_OEM_3},        // `
        {Qt::Key_Apostrophe, VK_OEM_7},       // '
        {Qt::Key_BracketLeft, VK_OEM_4},      // [
        {Qt::Key_BracketRight, VK_OEM_6},     // ]
        {Qt::Key_Equal, VK_OEM_PLUS},         // =（需Shift）
        {Qt::Key_AsciiTilde, VK_OEM_3},       // ~
        {Qt::Key_Question, VK_OEM_2},         // ?
        {Qt::Key_Colon, VK_OEM_1},            // :（需Shift）
        {Qt::Key_QuoteDbl, VK_OEM_7},         // "（需Shift）
        {Qt::Key_BraceLeft, VK_OEM_4},        // {
        {Qt::Key_BraceRight, VK_OEM_6},       // }
        {Qt::Key_Bar, VK_OEM_5},              // |

        // ==== 左右修饰键 ====
        {Qt::Key_AltGr, VK_RMENU},            // 右Alt
        {Qt::Key_Meta, VK_LWIN},              // Win键
        {Qt::Key_Super_L, VK_LWIN},           // 左Win
        {Qt::Key_Super_R, VK_RWIN},           // 右Win
        {Qt::Key_Control, VK_LCONTROL},       // 左Ctrl
        {Qt::Key_Shift, VK_LSHIFT},           // 左Shift

        // ==== 多媒体键 ====
        {Qt::Key_MediaPlay, VK_MEDIA_PLAY_PAUSE},
        {Qt::Key_MediaStop, VK_MEDIA_STOP},
        {Qt::Key_MediaPrevious, VK_MEDIA_PREV_TRACK},
        {Qt::Key_MediaNext, VK_MEDIA_NEXT_TRACK},
        {Qt::Key_VolumeDown, VK_VOLUME_DOWN},
        {Qt::Key_VolumeUp, VK_VOLUME_UP},
        {Qt::Key_VolumeMute, VK_VOLUME_MUTE},

        // ==== 浏览器/应用键 ====
        {Qt::Key_LaunchMail, VK_LAUNCH_MAIL},
        {Qt::Key_LaunchMedia, VK_LAUNCH_MEDIA_SELECT},
        {Qt::Key_Back, VK_BROWSER_BACK},
        {Qt::Key_Forward, VK_BROWSER_FORWARD},
        {Qt::Key_Refresh, VK_BROWSER_REFRESH},
        {Qt::Key_Search, VK_BROWSER_SEARCH},
        {Qt::Key_Favorites, VK_BROWSER_FAVORITES},
        {Qt::Key_HomePage, VK_BROWSER_HOME},

        // ==== 其他特殊键 ====
        {Qt::Key_Help, VK_HELP},
        {Qt::Key_Cancel, VK_CANCEL},
        {Qt::Key_Context1, VK_APPS}          // 右键菜单键
    };

    return keyMap.value(qtKey,  0); // 未匹配时返回0
}

UINT GlobalKeyEventMgr::toModifierConstant(unsigned int vk) {
    switch (vk) {
    case VK_CONTROL: return MOD_CONTROL;
    case VK_MENU:    return MOD_ALT;      // VK_MENU 是 Alt 键
    case VK_SHIFT:   return MOD_SHIFT;
    case VK_LSHIFT:  return MOD_SHIFT;
    case VK_RSHIFT:  return MOD_SHIFT;
    case VK_LWIN:    return MOD_WIN;
    case VK_RWIN:    return MOD_WIN;
    default:         return 0;            // 非修饰键返回 0
    }
}

void GlobalKeyEventMgr::initOperations() {
    if (!HotkeyMapper::GetInstance()->isConfigLoaded()) {
        qWarning() << "Hotkey config not loaded, skip init operations";
        return;
    }

    // 清空旧注册
    for (auto it = m_hotkeyIdToOperation.begin();  it != m_hotkeyIdToOperation.end();  ++it) {
        UnregisterHotKey(nullptr, it.key());
    }
    m_hotkeyIdToOperation.clear();
    m_hotKeyId = 1; // 重置ID

    // 遍历所有已定义操作
    QStringList allOps = HotkeyMapper::GetInstance()->getDefinedOperations();
    for (const QString& op : allOps) {
        QVector<Qt::Key> qtKeys = HotkeyMapper::GetInstance()->getKeyEnums(op);
        if (qtKeys.isEmpty())  {
            qWarning() << "操作" << op << "的键序列无效，跳过注册";
            continue;
        }

        UINT fsModifiers = 0;
        UINT vk = 0;
        bool valid = true;

        // 解析键序列：识别修饰键和主键
        for (const Qt::Key& key : qtKeys) {
            const unsigned int winVk = qtKeyToWinVK(key);
            if (winVk == 0) {
                qWarning() << "操作" << op << "包含无效键:" << key << "，跳过注册";
                valid = false;
                break;
            }

            // 检查是否为修饰键（Ctrl/Alt/Shift/Win）
            const UINT mod = toModifierConstant(winVk);
            if (mod != 0) {
                fsModifiers |= mod; // 累积修饰键
            } else {
                if (vk != 0) {
                    qWarning() << "操作" << op << "包含多个非修饰键，仅最后一个有效";
                }
                vk = winVk; // 最后一个非修饰键作为主键
            }
        }

        // 校验组合有效性
        if (!valid || vk == 0 || fsModifiers == 0) {
            qWarning() << "操作" << op << "的键序列无效（需至少1个修饰键+1个主键），跳过注册";
            continue;
        }

        // 注册热键
        const int currentId = m_hotKeyId;
        if (RegisterHotKey(nullptr, currentId, fsModifiers, vk)) {
            m_hotkeyIdToOperation.insert(currentId,  op);
            m_hotKeyId++;
            qDebug() << "成功注册热键：" << op << "，ID:" << currentId
                     << "，修饰键:" << QString::number(fsModifiers, 16)
                     << "，主键:" << QString::number(vk, 16);
        } else {
            DWORD err = GetLastError();
            qWarning() << "注册热键失败（错误码:" << err << "），操作：" << op;
            // 回滚ID（避免无效ID占用）
            m_hotKeyId--;
        }
    }
}

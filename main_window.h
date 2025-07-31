#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include "autocomplete.h"
#include "search/searchengine.h"
#include "search/SearchDef.h"
#include "systray.h"
#include <QMainWindow>
#include <qstackedwidget.h>
#include <QCloseEvent>
#include "globalkeyeventmgr.h"
#include "screen.h"

constexpr int PAGE_TOOLS= 0;
constexpr int PAGE_FILE_HISTORY = 1 ;
constexpr int PAGE_BACKUP = 2;
constexpr int PAGE_SETTINGS = 3;
constexpr int INVALID_PAGE = -1;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    auto systray(){return m_stray;}
    ~MainWindow();

private slots:
    void openGitHub();

    void refreshClient();

    void on_ToolsPage_clicked();

    void on_SettingsPage_clicked();

    void on_BackUpPage_clicked();

    void on_FileHistoryPage_clicked();

    void operationGate(const QString& operation);

    void on_Search_textChanged(const QString &arg1);

    void handlePageSearchRes(const MetaDataVariant &res);

protected:
    // 重写关闭事件
    void closeEvent(QCloseEvent *event) override;
    void moveEvent(QMoveEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
private:
    Ui::MainWindow *ui;
    void initSlots();
    void initUI();
    void initToolsPageResults(); //初始化工具页面的所有可能搜索结果
    void processToolsSearch(const std::string& query,
                            QList<QPair<QString, MetaDataVariant>>& results);

    Systray* m_stray = nullptr;

    GlobalKeyEventMgr *m_keyMgr = nullptr;

    Screen* m_screen = nullptr;

    AutoComplete* m_completer = nullptr;
    SearchEngine<Page::PAGE_TOOLS> m_toolsSearchEngine;
    SearchEngine<Page::PAGE_HISTORY> m_historySearchEngine;
    SearchEngine<Page::PAGE_BACKUP> m_backupSearchEngine;
    SearchEngine<Page::PAGE_SETTINGS> m_settingsSearchEngine;
    std::atomic<bool> m_isSearching{false}; // 防止重复搜索
    QTimer m_searchTimer; // 防抖定时器
};

#endif // MAIN_WINDOW_H

#include "main_window.h"
#include "./ui_main_window.h"
#include <QDesktopServices>
#include <QUrl>
#include "clickablelabel.h"
#include "toolspage.h"
#include "ocrclient.h"
#include "globalkeyeventmgr.h"
#include "search/SearchDef.h"
#include <QtConcurrent>
#include "historypage.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_keyMgr(new GlobalKeyEventMgr(this))// 初始化全局快捷键管理器
    , m_completer(new AutoComplete(this))
{
    ui->setupUi(this);
    initUI();
    initSlots();
    m_searchTimer.setSingleShot(true);
    m_searchTimer.setInterval(300); // 300ms防抖
    // 托盘
    m_stray = new Systray(this);

    // 连接快捷键触发信号
    connect(m_keyMgr, &GlobalKeyEventMgr::triggered,
            this, &MainWindow::operationGate);

    initToolsPageResults();

    SET_MICA_BACKGROUND(this);
}

MainWindow::~MainWindow()
{
    OcrClient::DestoryInstance();
    if(m_completer){
        delete m_completer;
    }
}

void MainWindow::openGitHub()
{
    QUrl githubUrl("https://github.com/Yih1ko/GreatScanner");
    QDesktopServices::openUrl(githubUrl);
}

void MainWindow::refreshClient()
{

}

void MainWindow::initSlots()
{
    connect(ui->git_label, QOverload<>::of(&ClickableLabel::clicked), this, &MainWindow::openGitHub);
    connect(ui->git_label_2, QOverload<>::of(&ClickableLabel::clicked), this, &MainWindow::openGitHub);
    connect(ui->Refresh, QOverload<>::of(&ClickableLabel::clicked), this, &MainWindow::refreshClient);
    connect(ui->Search, &SearchBox::lostFocus, [this](){
        m_completer->hide();
    });
    connect(ui->Search, &SearchBox::reactivated, [this](){
        m_completer->show();
    });
    //用户点击搜索得到的结果autocompleteitem时，调用对应槽函数handlePageSearchRes
    connect(m_completer, &AutoComplete::selectedItem, this, &MainWindow::handlePageSearchRes);
    connect(&m_searchTimer, &QTimer::timeout, this, [this](){
        if (!ui->Search->text().isEmpty()) {
            on_Search_textChanged(ui->Search->text());
        }
    });
}

void MainWindow::initUI()
{
    setWindowTitle(tr("GreatScanner"));
    setWindowIcon(QIcon(":/icons/cat.jpg"));
    setAttribute(Qt::WA_TranslucentBackground); //默认背景透明
    ui->ContentArea->setContentsMargins(0, 0, 0, 0);
    /******************************************************************************************/
    //插入页面(目前仅实现了PAGE_TOOLS)
    ToolsPage* toolsPage = new ToolsPage(this, ui->ContentArea); // 传入this指针
    ui->ContentArea->insertWidget(PAGE_TOOLS, toolsPage);
    HistoryPage* historyPage = new HistoryPage(this);
    ui->ContentArea->insertWidget(PAGE_FILE_HISTORY, historyPage);
    /******************************************************************************************/
    ui->Search->setAutoComplete(m_completer);
    // for(int i=0;i<30;i++){
    // // 添加工具页面结果测试
    // SearchMetaData<Page::PAGE_TOOLS> toolData(QString("Header %1").arg(i).toStdString(), QString("Function %1").arg(i).toStdString());
    // m_completer->appendResult(QString("Header %1").arg(i), toolData);
    // }
    QTimer::singleShot(100, [this]() {
        m_completer->updatePosition(ui->Search);
        m_completer->hide();
    });


    ui->Search->setObjectName("searchBox");
    m_completer->setObjectName("autoComplete");
}

void MainWindow::initToolsPageResults()
{
    // 添加工具页面的搜索项
    const std::vector<std::pair<QString, QString>> tools = {
        {tr("扫描证件"), "扫描证件"},
        {tr("提取文字"), "提取文字"},
        {tr("识别公式"), "识别公式"},
        {tr("拍照计数"), "拍照计数"},
        {tr("电子签名"), "电子签名"},
        {tr("添加水印"), "添加水印"},
        {tr("转Word"), "转Word"},
        {tr("转Excel"), "转Excel"},
        {tr("转Pdf"), "转Pdf"}
    };

    RecordId id = 1;
    for (const auto& [header, function] : tools) {
        // 使用emplace避免临时对象
        m_toolsSearchEngine.insertRecord(
            header.toStdString(),
            id++,
            SearchMetaData<Page::PAGE_TOOLS>(
                header.toStdString(),
                function.toStdString()
                )
            );
    }
}

void MainWindow::processToolsSearch(const std::string &query, QList<QPair<QString, MetaDataVariant> > &results)
{
    // 使用reserve预分配内存
    results.reserve(20);

    // 合并搜索和去重
    std::unordered_set<RecordId> combinedIds;
    const auto prefixIds = m_toolsSearchEngine.getPrefixIds(query);
    const auto keywordIds = m_toolsSearchEngine.searchKeywords(query);

    combinedIds.insert(prefixIds.begin(), prefixIds.end());
    combinedIds.insert(keywordIds.begin(), keywordIds.end());

    // 直接获取元数据避免多次查找
    for (RecordId id : combinedIds) {
        const auto& metaData = m_toolsSearchEngine.getMetaData(id);
        results.append({
            QString::fromStdString(metaData.header),
            MetaDataVariant(metaData)
        });
    }
}

void MainWindow::operationGate(const QString &operation)
{
    if(operation == "SCREENSHOT"){
        m_screen = new Screen();
        m_screen->show();
    }
}


void MainWindow::on_ToolsPage_clicked()
{
    if(ui->ContentArea->currentIndex() == PAGE_TOOLS) return;
    ui->ContentArea->setCurrentIndex(PAGE_TOOLS);
}


void MainWindow::on_SettingsPage_clicked()
{
    if(ui->ContentArea->currentIndex() == PAGE_SETTINGS) return;
    ui->ContentArea->setCurrentIndex(PAGE_SETTINGS);
}


void MainWindow::on_BackUpPage_clicked()
{
    if(ui->ContentArea->currentIndex() == PAGE_BACKUP) return;
    ui->ContentArea->setCurrentIndex(PAGE_BACKUP);
}


void MainWindow::on_FileHistoryPage_clicked()
{
    if(ui->ContentArea->currentIndex() == PAGE_FILE_HISTORY) return;
    ui->ContentArea->setCurrentIndex(PAGE_FILE_HISTORY);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // 关闭所有子窗口
    for (QWidget* widget : findChildren<QWidget*>()) {
        if (widget->isWindow() && widget != this) {
            widget->close();
        }
    }
    m_completer->close();
    event->ignore(); // 忽略默认关闭行为
    emit m_stray->showTrayMessage();
    hide();
}

void MainWindow::moveEvent(QMoveEvent *event)
{
    QMainWindow::moveEvent(event);
    if (m_completer) m_completer->updatePosition(ui->Search);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    if (m_completer) m_completer->updatePosition(ui->Search);
}

//对接真正的搜索接口
void MainWindow::on_Search_textChanged(const QString &arg1)
{
    //根据当前页面来进行搜索，例如目前ContentArea的index为PAGE_TOOLS，则用对应的搜索器来搜索
    //当搜索匹配到前缀搜索和分词搜索时再整合(如果分词和前缀搜索结果一致则视为一个结果)并且show all the results in m_completer
    if (arg1.isEmpty()) {
        m_completer->clearResults();
        m_completer->hide();
        m_searchTimer.stop();
        return;
    }

    // 避免频繁搜索
    if (m_isSearching.exchange(true)) return;

    // 使用QtConcurrent避免阻塞UI线程
    QtConcurrent::run([this, query = arg1.toStdString()]() {
        QList<QPair<QString, MetaDataVariant>> results;
        const int currentPage = ui->ContentArea->currentIndex();

        // 根据当前页面选择搜索引擎
        switch (currentPage) {
        case PAGE_TOOLS:
            processToolsSearch(query, results);
            break;
        case PAGE_FILE_HISTORY:
            //processHistorySearch(query, results);
            break;
        case PAGE_BACKUP:
            //processBackupSearch(query, results);
            break;
        case PAGE_SETTINGS:
            //processSettingsSearch(query, results);
            break;
        default:
            break;
        }

        // 更新UI
        QMetaObject::invokeMethod(this, [this, results]() {
            m_completer->clearResults();
            for (const auto& [text, data] : results) {
                m_completer->appendResult(text, data);
            }
            if (!results.isEmpty()) {
                m_completer->show();
            } else {
                m_completer->hide();
            }
            m_isSearching = false;
        });
    });

    // 使用防抖机制
    m_searchTimer.start();
}

void MainWindow::handlePageSearchRes(const MetaDataVariant &res)
{
    // 使用visit处理不同类型的元数据
    std::visit([this](auto&& arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, SearchMetaData<Page::PAGE_TOOLS>>) {
            ToolsPage* toolsPage = qobject_cast<ToolsPage*>(ui->ContentArea->widget(PAGE_TOOLS));
            // 获取功能标识并执行对应操作
            const QString function = QString::fromStdString(arg.function);
            toolsPage->handleCardAction(function);
        }
    }, res);
}

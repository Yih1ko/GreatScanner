#include "toolspage.h"
#include "cardview.h"
#include "cardgroup.h"
#include <QScroller>
#include <QGuiApplication>
#include "srcselector.h"
#include "ocrclient.h"
#include "ocrtextdlg.h"
#include "pdfwatermarkdlg.h"
#include "QFileDialog"
#include <QStandardPaths>

constexpr int ToolsPage::CARD_SIZE;
constexpr int ToolsPage::CARD_RADIUS;

ToolsPage::ToolsPage(MainWindow* mainWindow, QWidget *parent)
    : QWidget(parent), m_mainWindow(mainWindow)
{
    // 滚动区域设置
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setFrameShape(QFrame::NoFrame);

    // 滚动内容容器
    m_scrollContent = new QWidget(m_scrollArea);
    m_mainLayout = new QVBoxLayout(m_scrollContent);
    m_mainLayout->setAlignment(Qt::AlignTop);
    m_mainLayout->setContentsMargins(0, 12, 12, 12);
    m_mainLayout->setSpacing(8);  // 增加卡片间距

    m_scrollArea->setWidget(m_scrollContent);

    // 主布局
    QVBoxLayout* outerLayout = new QVBoxLayout(this);
    outerLayout->addWidget(m_scrollArea);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // 启用触摸滚动
    QScroller::grabGesture(m_scrollArea->viewport(), QScroller::TouchGesture);

    initUI();

    // 样式表提取为常量
    const QString styleSheet = R"(
        ToolsPage, QScrollArea > QWidget > QWidget {
            background-color: rgb(248, 250, 255);
        }
        QScrollBar:vertical {
            background: transparent;
            width: 5px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #D9D9D9;
            min-height: 10px;
            border-radius: 2px;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical {
            height: 0px;
            border: none;
            background: transparent;
        }
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: none;
        }
    )";
    setStyleSheet(styleSheet);
}

ToolsPage::~ToolsPage()
{

}

Panel* ToolsPage::getPanel(const QString &key) const
{
    return m_panels.value(key,  nullptr);
}

void ToolsPage::closeEvent(QCloseEvent *event)
{
    event->accept();
}

void ToolsPage::initUI()
{
    clearLayout();

    // 卡片组配置数据
    struct CardGroupConfig {
        QString groupName;
        QStringList cards;
    };

    const QList<CardGroupConfig> groups = {
        {"扫描服务", {"扫描证件", "提取文字", "识别公式", "拍照计数"}},
        {"文档编辑", {"电子签名", "添加水印"}},
        {"格式转换", {"转Word", "转Excel", "转Pdf"}}
    };

    for (const auto& group : groups) {
        CardGroup* cardGroup = new CardGroup(m_scrollContent);
        for (const auto& text : group.cards)  {
            CardView* card = new CardView(cardGroup);
            configureCard(card, text, text + "_icon");
            cardGroup->addCardView(card);
        }
        addPanel(new Panel(cardGroup, group.groupName),  group.groupName);
    }

    m_mainLayout->addStretch();
}

void ToolsPage::addPanel(Panel *panel, const QString& key)
{
    m_mainLayout->addWidget(panel);
    m_panels.insert(key,  panel);
}

void ToolsPage::configureCard(CardView *card, const QString &description, const QString &iconName)
{
    card->setFixedSize(CARD_SIZE, CARD_SIZE);
    card->setBorderRadius(CARD_RADIUS);

    QPixmap pixmap(":/icons/" + iconName);
    if (!pixmap.isNull())  {
        card->setCardPixmap(pixmap);
        card->setHorizontalPixmapRatio(1);
        card->setVerticalPixmapRatio(1);
    }

    card->setDesBelow(description);

    connect(card, &CardView::cardClicked, this, [this, description]() {
        handleCardAction(description);
    });
}

void ToolsPage::clearLayout()
{
    qDeleteAll(m_scrollContent->findChildren<QWidget*>(QString(), Qt::FindDirectChildrenOnly));
    m_panels.clear();
}

void ToolsPage::handleCardAction(const QString &action)
{
    static const QHash<QString, std::function<void()>> actionMap = {
        {"扫描证件", [this]{
             /* 扫描证件实现 */
             //this->showSrcSelector();
         }},
        {"提取文字", [this]{
             /* 提取文字实现 */
             this->showSrcSelector(ipc::IpcId::Ocr_Default);
         }},
        {"识别公式", [this]{
             /* 识别公式实现 */
             //this->showSrcSelector();
         }},
        {"拍照计数", [this]{
             /* 拍照计数实现 */
             //this->showSrcSelector();
         }},
        {"电子签名", []{
             /* 电子签名实现 */

         }},
        {"添加水印", [this]{
             /* Pdf添加水印实现 */
             // 文件选择对话框
             QString filePath = QFileDialog::getOpenFileName(
                 this,
                 tr("选择PDF文件"),
                 QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
                 tr("PDF文件 (*.pdf)")
                 );

             if (!filePath.isEmpty())  {
                 // 关闭现有对话框
                 if (m_watermarkDlg) {
                     m_watermarkDlg->close();
                     m_watermarkDlg->deleteLater();
                 }

                 // 创建水印对话框
                 m_watermarkDlg = new PdfWaterMarkDlg(this, filePath);
                 m_watermarkDlg->setWindowTitle(tr("PDF水印工具"));
                 m_watermarkDlg->setAttribute(Qt::WA_DeleteOnClose);
                 m_watermarkDlg->show();
             }
         }},
        {"转Word", [this]{
             /* 转Word实现 */
             this->showSrcSelector(ipc::IpcId::Ocr_Word);
         }},
        {"转Excel", [this]{
             /* 转Excel实现 */
             this->showSrcSelector(ipc::IpcId::Ocr_Execl);
         }},
        {"转Pdf", [this]{
             /* 转Pdf实现 */
             //this->showSrcSelector();
         }}
    };

    if (actionMap.contains(action))  {
        actionMap.value(action)();
    }
}

void ToolsPage::showSrcSelector(ipc::IpcId mod) {
    // 安全销毁旧实例
    if (m_activeSelector) {
        // 关键修复：断开旧信号连接，避免悬空指针回调
        disconnect(m_activeSelector, &SrcSelector::destroyed, this, nullptr);
        m_activeSelector->deleteLater();
        m_activeSelector = nullptr; // 立即置空指针
    }

    auto selector = new SrcSelector(this);
    m_activeSelector = selector;
    m_activeSelector->setMainWindow(m_mainWindow);

    // 设置关闭时自动销毁 (关键!)
    m_activeSelector->setAttribute(Qt::WA_DeleteOnClose);

    // 监听销毁信号 (关键!)
    connect(m_activeSelector, &SrcSelector::destroyed, this, [this]() {
        m_activeSelector = nullptr; // 窗口销毁时自动置空指针
    });

    // 其他初始化
    m_activeSelector->setParent(this, Qt::Window);
    m_activeSelector->setWindowTitle(tr("图像采集器"));
    m_activeSelector->setMod(mod);
    m_activeSelector->show();
}


#ifndef TOOLSPAGE_H
#define TOOLSPAGE_H

#include <QWidget>
#include <QMap>
#include <QVBoxLayout>
#include <QScrollArea>
#include "panel.h"
#include "pdfwatermarkdlg.h"
#include "srcselector.h"
#include "main_window.h"
class CardView;
class CardGroup;

class ToolsPage : public QWidget
{
    Q_OBJECT
public:
    explicit ToolsPage(MainWindow* mainWindow, QWidget *parent = nullptr);
    ~ToolsPage();
    Panel* getPanel(const QString& key) const;

public slots:
    void handleCardAction(const QString &action);
protected:
    void closeEvent(QCloseEvent *event) override;  // 重写关闭事件

private:
    void initUI();
    void addPanel(Panel *panel, const QString& key);
    void clearLayout();
    void configureCard(CardView *card, const QString &description, const QString &iconName);
    //void handleCardAction(const QString &action);
    MainWindow* m_mainWindow; // 保存主窗口指针
    void showSrcSelector(ipc::IpcId mod);   // 统一的窗口显示方法
    QMap<QString, Panel*> m_panels;
    QVBoxLayout* m_mainLayout = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QWidget* m_scrollContent = nullptr;
    SrcSelector* m_activeSelector = nullptr;
    PdfWaterMarkDlg* m_watermarkDlg = nullptr;

    static constexpr int CARD_SIZE = 100;
    static constexpr int CARD_RADIUS = 15;
};

#endif // TOOLSPAGE_H

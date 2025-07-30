#include "autocomplete.h"
#include "searchbox.h"
#include "ui_autocomplete.h"
#include <qgraphicseffect.h>
#include <qlineedit.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include "autocompleteitem.h"

AutoComplete::AutoComplete(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AutoComplete)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Tool |/*Qt::Window | */Qt::FramelessWindowHint/* | Qt::Popup*/);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_layout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    m_layout->setAlignment(Qt::AlignTop); // 标签顶部对齐
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
}

AutoComplete::~AutoComplete()
{
    delete ui;
}

void AutoComplete::appendResult(const QString &text, const MetaDataVariant &metaData)
{
    AutoCompleteItem* newItem = new AutoCompleteItem();
    newItem->setText(text);
    newItem->setFixedHeight(40); // 固定高度
    ui->scrollAreaWidgetContents->setMinimumHeight(m_layout->count() * 40);
    m_layout->addWidget(newItem);

    QObject::connect(newItem, &AutoCompleteItem::selected, this, [this, metaData](){
        emit selectedItem(metaData);
        // std::visit([](auto&& arg) {
        //     using T = std::decay_t<decltype(arg)>;  // 获取实际类型

        //     // 处理工具页面数据
        //     if constexpr (std::is_same_v<T, SearchMetaData<Page::PAGE_TOOLS>>) {
        //         qDebug() << "工具页面选择: "
        //                  << QString::fromStdString(arg.header)
        //                  << " - 功能: "
        //                  << QString::fromStdString(arg.function);
        //     }
        // }, metaData);
    });
}

void AutoComplete::clearResults()
{
    QLayoutItem* child;
    while ((child = m_layout->takeAt(0)) != nullptr) {
        delete child->widget(); // 删除部件
        delete child; // 删除布局项
    }
}



void AutoComplete::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿

    // 创建圆角矩形路径
    QPainterPath path;
    path.addRoundedRect(rect(),  10, 10);
    painter.setClipPath(path);

    // 绘制背景（可自定义颜色/渐变）
    painter.fillPath(path,  QBrush(Qt::white));  // 白色背景
}



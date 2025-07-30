#include "infocard.h"
#include "global.h"

InfoCard::InfoCard(QWidget *parent)
    : QWidget(parent)
{
    // 初始化组件
    initUI();
}

InfoCard::~InfoCard()
{

}

//设置卡片图标
void InfoCard::setCardIcon(const QIcon &icon)
{
    // 将 QIcon 转换为 QPixmap 并设置给 CardView
    if (!icon.isNull())  {
        QPixmap pixmap = icon.pixmap(icon.actualSize(QSize(64,  64)));
        setCardIcon(pixmap);
    }
}

//设置卡片图标
void InfoCard::setCardIcon(const QPixmap &pixmap)
{
    // 设置 CardView 的图标 pixmap
    if (!pixmap.isNull())  {
        m_card->setCardPixmap(pixmap);
        m_card->setHorizontalPixmapRatio(1); // 设置图片宽度比例
        m_card->setVerticalPixmapRatio(1);   // 设置图片高度比例
        m_card->setBorderRadius(10);    // 卡片圆角半径
    }
}

//设置卡片圆角
void InfoCard::setBorderRadius(int radius)
{
    // 设置圆角半径
    m_borderRadius = radius;
    update(); // 刷新界面以应用更改
}

//设置卡片内部文件信息
void InfoCard::setFileInfo(QFileInfo &info)
{
    m_info = info;
}
QFileInfo InfoCard::getFileInfo() const
{
    return m_info;
}

//更新左侧的主要信息
void InfoCard::updateCon(TextPositon textPos, QString text)
{
    //update the text of m_upperLeft or m_lowerLeft or m_midright;
    switch (textPos) {
    case TextPositon::UpperLeft:{
        m_upperLeft->setText(text);
    }
    break;
    case TextPositon::LowerLeft:{
        m_lowerLeft->setText(text);
    }
    break;
    default:
        break;
    }
}

//更新左侧的主要信息
void InfoCard::updateCon(TextPositon textPos, double opacity)
{
    //update the opacity of m_upperLeft or m_lowerLeft or m_midright;
    switch (textPos) {
    case TextPositon::UpperLeft:{
        m_upperLeft->setOpacity(opacity);
    }
    break;
    case TextPositon::LowerLeft:{
        m_lowerLeft->setOpacity(opacity);
    }
    break;
    default:
        break;
    }
}

//添加新的右侧描述文
void InfoCard::appendDes(QString text) {
    TextLabel* des = new TextLabel(text, this);
    des->setTextPixelSize(14);
    des->setContentsMargins(0, 0, 0, 0);
    des->setFixedWidth(75); // 固定列宽

    m_rightDes.push_back(des);
    m_desContentLayout->addWidget(des);

    QSpacerItem* spacer1 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_desContentLayout->addItem(spacer1);
}
//根据索引更新右侧描述文
void InfoCard::updateDes(int index, QString text)
{
    if(index >= m_rightDes.size()){
        return;
    }
    TextLabel* des = m_rightDes[index];
    des->setText(text);
}

QString InfoCard::getDes(int index) const
{
    if(index >= m_rightDes.size()){
        return "";
    }
    return m_rightDes[index]->text();
}

//0代表左侧Con，1+才是右侧描述
std::tuple<QString, QString, QString> InfoCard::getInfo(int index) const
{
    if(index<0 || index>=m_rightDes.size()){
        return std::make_tuple(
            "",
            "",
            ""
            );
    }
    if(index==0){
        return std::make_tuple(
            "",
            m_lowerLeft->text(),
            m_upperLeft->text()
            );
    }
    if(!m_rightDes[index-1]){
        return std::make_tuple(
            m_rightDes[index-1]->text(),
            m_lowerLeft->text(),
            m_upperLeft->text()
            );
    }
    return std::make_tuple(
        "",
        "",
        ""
        );
}

int InfoCard::getLen() const
{
    return m_rightDes.size();
}

void InfoCard::setWithBelowText(bool withBelow = false)
{
    m_card->setWithBelowText(withBelow);
}

//鼠标按下事件
void InfoCard::mousePressEvent(QMouseEvent *event)
{
    // 鼠标按下时背景变灰色
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        m_pressed = true;
        update(); // 刷新界面以显示灰色背景
    }
    QWidget::mousePressEvent(event);
}

//鼠标松开事件
void InfoCard::mouseReleaseEvent(QMouseEvent *event)
{
    // 鼠标释放时背景恢复透明
    if (event->button() == Qt::LeftButton) {
        m_pressed = false;
        update(); // 刷新界面以恢复透明背景
        emit clicked();
    }
    if(event->button() == Qt::RightButton){
        m_pressed = false;
        update(); // 刷新界面以恢复透明背景
        emit rightButtonClicked();
    }
    QWidget::mouseReleaseEvent(event);
}

bool InfoCard::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
    //暂时没用处
}

void InfoCard::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
}

void InfoCard::leaveEvent(QEvent *event)
{
    m_hovered = false;
}

//绘制主逻辑
void InfoCard::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);  // 开启抗锯齿

    // 绘制圆角矩形背景
    QPainterPath path;
    path.addRoundedRect(rect(),  m_borderRadius, m_borderRadius);
    painter.setClipPath(path);

    // 根据鼠标状态设置背景颜色
    if (m_pressed) {
        painter.fillRect(rect(),  QColor(233, 233, 233)); // 灰色背景
    } else if(m_hovered){
        painter.fillRect(rect(),  Qt::transparent); // 透明背景
        drawEffectShadow(&painter, rect(), 8, m_borderRadius);
    } else {
        painter.fillRect(rect(),  Qt::transparent); // 透明背景
    }

    // 让子部件正常绘制
    QWidget::paintEvent(event);
}

//初始化ui
void InfoCard::initUI()
{
    // 设置圆角半径
    setBorderRadius(kCardBorderRadius);
    // 背景颜色透明
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    setContentsMargins(0, 0, 0, 0);
    // 鼠标按下时的背景颜色
    m_pressed = false;
    //鼠标悬浮
    setAttribute(Qt::WA_Hover, true);
    installEventFilter(this);
    m_hovered = false;
    //默认没有卡下文字


    // 创建主布局
    m_mainLayout = new QHBoxLayout(this);
    m_mainLayout->setContentsMargins(0, 3, 10, 3); // 设置主布局的边距
    //m_mainLayout->setSpacing(kMainLayoutSpacing); // 设置主布局中各子布局之间的间距

    // 创建左侧内容布局
    QHBoxLayout* layoutOfCon = new QHBoxLayout(this);
    layoutOfCon->setAlignment(Qt::AlignLeft);//
    layoutOfCon->setContentsMargins(0, 0, 0, 0);
    layoutOfCon->setSpacing(kCardContentSpacing);

    // 创建CardView并设置属性
    m_card = new CardView(this);
    m_card->setFixedSize(kCardSize, kCardSize);
    m_card->setBorderRadius(kCardBorderRadius);
    m_card->setWithBelowText(false);

    // 创建左侧文本布局
    QVBoxLayout* textLayout = new QVBoxLayout(this);
    textLayout->setContentsMargins(0, kUpperTextLabelSpacing, 0, kLowerTextLabelSpacing); // 上下各增加 10 像素的间隔
    textLayout->setSpacing(0);

    m_upperLeft = new TextLabel(this);
    m_upperLeft->setTextStyle(TextStyle::Body);

    m_lowerLeft = new TextLabel(this);
    m_lowerLeft->setTextStyle(TextStyle::Body);

    textLayout->addWidget(m_upperLeft);
    textLayout->addWidget(m_lowerLeft);

    // 将CardView和文本布局添加到左侧内容布局
    layoutOfCon->addWidget(m_card);
    layoutOfCon->addLayout(textLayout);

    // 创建右侧描述布局
    m_layoutOfDes = new QHBoxLayout(this);
    m_layoutOfDes->setAlignment(Qt::AlignRight);//
    m_layoutOfDes->setContentsMargins(0, 0, 0, 0);
    m_layoutOfDes->setSpacing(kRightDesSpacing);

    // 创建一个新的水平布局用于容纳所有描述文本
    m_desContentLayout = new QHBoxLayout(this);
    m_desContentLayout->setContentsMargins(0, 0, 0, 0);
    //m_desContentLayout->setSpacing(kRightDesSpacing);

    // 创建菜单按钮
    m_menuDot = new QPushButton(this);
    m_menuDot->setFixedSize(kCardSize / 3, kRightDesSpacing + 3);
    m_menuDot->setIcon(QIcon(":/icons/216更多-竖向.png"));

    // 应用样式表
    m_menuDot->setStyleSheet(R"(
        QPushButton {
            border: none;           /* 无边框 */
            background-color: transparent; /* 透明背景 */
            border-radius: 4px;     /* 圆角（可选） */
        }

        QPushButton:hover {
            background-color: #e0e0e0; /* 悬停时浅灰色背景 */
        }

        QPushButton:pressed {
            background-color: #c9c9c9; /* 按下时深灰色背景 */
        }
    )");

    // 将描述内容布局和按钮添加到右侧描述布局
    m_layoutOfDes->addLayout(m_desContentLayout);
    m_layoutOfDes->addWidget(m_menuDot, 0, Qt::AlignRight);

    // 将左侧内容布局添加到主布局
    m_mainLayout->addLayout(layoutOfCon);

    // 添加 Spacer
    QSpacerItem* mainSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
    m_mainLayout->addItem(mainSpacer);

    //右侧描述布局添加到主布局
    m_mainLayout->addLayout(m_layoutOfDes);
}

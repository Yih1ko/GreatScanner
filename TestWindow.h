#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "autocomplete.h"
#include "roundprogressbar.h"
#include "infocard.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QColorDialog>
#include <QLabel>
#include <QComboBox>
#include <qlineedit.h>

class ProgressBarTest : public QWidget {
    Q_OBJECT
public:
    ProgressBarTest(QWidget *parent = nullptr) : QWidget(parent) {
        initUI();
        initConnections();
        // 初始更新所有参数
        updateProgressBar();
    }

private:
    void initUI() {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // 进度条展示区域
        progressBar = new RoundProgressBar(this);
        progressBar->setFixedSize(300, 300);
        mainLayout->addWidget(progressBar, 0, Qt::AlignCenter);

        // 参数控制面板
        QGridLayout *controlLayout = new QGridLayout();

        // 当前值控制
        addControlRow(controlLayout, 0, "当前值:", valueSlider = createSlider(0, 100), valueSpin = createSpinBox(0, 100));
        addControlRow(controlLayout, 1, "最小值:", minSpin = createSpinBox(-1000, 1000));
        addControlRow(controlLayout, 2, "最大值:", maxSpin = createSpinBox(-1000, 1000));

        // 角度和方向控制
        addControlRow(controlLayout, 3, "起始角度:", angleSpin = createSpinBox(0, 360));
        directionCombo = new QComboBox();
        directionCombo->addItems({"顺时针", "逆时针"});
        controlLayout->addWidget(new QLabel("方向:"), 3, 2);
        controlLayout->addWidget(directionCombo, 3, 3);

        // 宽度控制
        addControlRow(controlLayout, 4, "外圈宽度:", outterWidthSpin = createDoubleSpinBox(1, 50));
        addControlRow(controlLayout, 5, "内圈宽度:", innerWidthSpin = createDoubleSpinBox(1, 50));

        // 颜色控制按钮
        QHBoxLayout *colorLayout = new QHBoxLayout();
        addColorButton(colorLayout, "外圈颜色", &outterColorBtn, QColor(233, 248, 248));
        addColorButton(colorLayout, "内圈颜色", &startColorBtn, QColor(49, 177, 190));
        addColorButton(colorLayout, "渐变色终点", &endColorBtn, QColor(133, 243, 244));
        controlLayout->addLayout(colorLayout, 6, 0, 1, 4);

        // 功能开关
        QHBoxLayout *switchLayout = new QHBoxLayout();
        addSwitchCheckbox(switchLayout, "显示文字", defaultTextCheck, true);
        addSwitchCheckbox(switchLayout, "显示圆点", decorateDotCheck, true);
        addSwitchCheckbox(switchLayout, "渐变色", linearColorCheck, true);
        controlLayout->addLayout(switchLayout, 7, 0, 1, 4);

        mainLayout->addLayout(controlLayout);
    }

    void initConnections() {
        connect(valueSlider, &QSlider::valueChanged, this, [=](int val){
            progressBar->setValue(val);
            valueSpin->setValue(val);
        });
        connect(valueSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [=](int val){
            valueSlider->setValue(val);
        });

        // 范围设置
        connect(minSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProgressBarTest::updateRange);
        connect(maxSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, &ProgressBarTest::updateRange);
    }

    void updateProgressBar() {
        // 更新控制标志位
        quint32 flags = 0;
        if (defaultTextCheck->isChecked()) flags |= RoundProgressBar::defaultText;
        if (decorateDotCheck->isChecked()) flags |= RoundProgressBar::decorateDot;
        if (linearColorCheck->isChecked()) flags |= RoundProgressBar::linearColor;
        flags |= RoundProgressBar::outterCircle | RoundProgressBar::animation;
        progressBar->setControlFlags(flags);
    }

private slots:
    void updateRange() {
        progressBar->setRange(minSpin->value(), maxSpin->value());
        valueSlider->setRange(minSpin->value(), maxSpin->value());
        valueSpin->setRange(minSpin->value(), maxSpin->value());
    }

private:
    // 控件生成辅助函数
    QSlider* createSlider(int min, int max) {
        QSlider *slider = new QSlider(Qt::Horizontal);
        slider->setRange(min, max);
        return slider;
    }

    QSpinBox* createSpinBox(int min, int max) {
        QSpinBox *spin = new QSpinBox;
        spin->setRange(min, max);
        return spin;
    }

    QDoubleSpinBox* createDoubleSpinBox(double min, double max) {
        QDoubleSpinBox *spin = new QDoubleSpinBox;
        spin->setRange(min, max);
        spin->setSingleStep(0.5);
        return spin;
    }

    void addControlRow(QLayout *layout, int row, const QString &label, QWidget *control1, QWidget *control2 = nullptr) {
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(new QLabel(label));
        hLayout->addWidget(control1);
        if (control2) hLayout->addWidget(control2);
        static_cast<QGridLayout*>(layout)->addLayout(hLayout, row, 0, 1, 2);
    }

    void addColorButton(QLayout *layout, const QString &text, QPushButton **btn, const QColor &color) {
        *btn = new QPushButton(text);
        (*btn)->setStyleSheet(QString("background-color: %1").arg(color.name()));
        connect(*btn, &QPushButton::clicked, this, [=](){
            QColor newColor = QColorDialog::getColor(color, this);
            if (newColor.isValid())  {
                (*btn)->setStyleSheet(QString("background-color: %1").arg(newColor.name()));
                if (text == "外圈颜色") progressBar->setOutterColor(newColor);
                else if (text == "内圈颜色") progressBar->setInnerColor(newColor);
                else progressBar->setInnerColor(startColorBtn->palette().button().color(), newColor);
            }
        });
        layout->addWidget(*btn);
    }

    void addSwitchCheckbox(QLayout *layout, const QString &text, QCheckBox *&checkBox, bool checked) {
        checkBox = new QCheckBox(text);
        checkBox->setChecked(checked);
        connect(checkBox, &QCheckBox::stateChanged, this, &ProgressBarTest::updateProgressBar);
        layout->addWidget(checkBox);
    }

    // 控件成员
    RoundProgressBar *progressBar;
    QSlider *valueSlider;
    QSpinBox *valueSpin, *minSpin, *maxSpin, *angleSpin;
    QDoubleSpinBox *outterWidthSpin, *innerWidthSpin;
    QComboBox *directionCombo;
    QPushButton *outterColorBtn, *startColorBtn, *endColorBtn;
    QCheckBox *defaultTextCheck, *decorateDotCheck, *linearColorCheck;
};


//---------------------------------------------------------------------------------------------------------------------------

#include <QIcon>
class InfoCardTest : public QWidget
{
    Q_OBJECT
public:
    explicit InfoCardTest(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        // 创建 InfoCard 实例
        m_infoCard = new InfoCard(this);
        m_infoCard->setBorderRadius(13); // 设置圆角半径

        // 创建测试按钮
        m_testBtn = new QPushButton("Run Test", this);
        connect(m_testBtn, &QPushButton::clicked, this, &InfoCardTest::testAllFeatures);

        // 设置布局
        QVBoxLayout* layout = new QVBoxLayout(this);
        layout->addWidget(m_testBtn);
        layout->addWidget(m_infoCard);
        setLayout(layout);

        // 设置窗口标题和大小
        setWindowTitle("InfoCard Test");
        resize(400, 300);
    }

private slots:
    void testAllFeatures()
    {
        qDebug() << "=== Start Testing InfoCard Features ===";

        // 1. 测试设置卡片图标 (使用 QIcon)
        QIcon icon(":/image/MicaBase.png");  // 图标路径
        m_infoCard->setCardIcon(icon);
        qDebug() << "Test 1: Set Card Icon using QIcon - Done";

        // 2. 测试设置卡片图标 (使用 QPixmap)
        QPixmap pixmap(":/image/MicaBase.png");  // 假设这是一个有效的 pixmap 路径
        m_infoCard->setCardIcon(pixmap);
        qDebug() << "Test 2: Set Card Icon using QPixmap - Done";

        // 3. 测试更新左侧内容 (文本)
        m_infoCard->updateCon(TextPositon::UpperLeft, "example");
        m_infoCard->updateCon(TextPositon::LowerLeft, "图片");
        qDebug() << "Test 3: Update Left Content - Done";

        // // 4. 测试更新左侧内容 (不透明度)
        // m_infoCard->updateCon(TextPositon::UpperLeft, 0.8); // 设置左上角文本不透明度为 80%
        // m_infoCard->updateCon(TextPositon::LowerLeft, 0); // 设置左下角文本不透明度为 60%
        // qDebug() << "Test 4: Update Opacity of Left Content - Done";

        // 5. 测试添加右侧描述
        m_infoCard->appendDes("Des 1");
        m_infoCard->appendDes("Des 2");
        m_infoCard->appendDes("Des 3");
        qDebug() << "Test 5: Append Descriptions - Done";

        // 6. 测试更新右侧描述
        m_infoCard->updateDes(0, "new Des 1");
        m_infoCard->updateDes(2, "new Des 3");
        qDebug() << "Test 6: Update Descriptions - Done";

        // // 7. 测试设置空图标
        // QIcon nullIcon;
        // m_infoCard->setCardIcon(nullIcon);
        // qDebug() << "Test 7: Set Null Icon - Done";

        // // 8. 测试添加大量描述
        // for (int i = 0; i < 5; ++i) {
        //     m_infoCard->appendDes(QString("des %1").arg(i+1));
        // }
        // qDebug() << "Test 8: Add Multiple Descriptions - Done";

        qDebug() << "=== All Tests Completed ===";
    }  // 测试所有功能

private:
    InfoCard* m_infoCard;  // 测试用的 InfoCard 实例
    QPushButton* m_testBtn; // 测试按钮
};

//------------------------------------------------------------------------------------------------------
//弃用listwidget

//------------------------------------------------------------------------------------------------------
#include "notification.h"

class NotifacationTest : public QWidget{
    Q_OBJECT
public:
    NotifacationTest(QWidget* parent=nullptr){
        resize(500, 360);

        nf = new Notification(this);

        QPushButton* btn1 = new QPushButton("普通", this);
        QPushButton* btn2 = new QPushButton("成功", this);
        QPushButton* btn3 = new QPushButton("错误", this);
        QPushButton* btn4 = new QPushButton("警告", this);
        QPushButton* btn5 = new QPushButton("信息", this);

        connect(btn1, &QPushButton::clicked, this, [&](){
            nf->Push(NotifyType::Notify_Type_None,
                     NotifyPosition::Pos_Top_Right,
                     "标准提示",
                     "你好，世界！Hellow world!"
                     );
        });
        connect(btn2, &QPushButton::clicked, this, [&](){
            nf->Push(NotifyType::Notify_Type_Success,
                     NotifyPosition::Pos_Top_Left,
                     "成功提示",
                     "你好，世界！Hellow world!");
        });
        connect(btn3, &QPushButton::clicked, this, [&](){
            nf->Push(NotifyType::Notify_Type_Error,
                     NotifyPosition::Pos_Bottom_Left,
                     "错误提示",
                     "你好，世界！Hellow world!");
        });
        connect(btn4, &QPushButton::clicked, this, [&](){
            nf->Push(NotifyType::Notify_Type_Warning,
                     NotifyPosition::Pos_Bottom_Right,
                     "警告提示",
                     "你好，世界！Hellow world!");
        });
        connect(btn5, &QPushButton::clicked, this, [&](){
            nf->Push(NotifyType::Notify_Type_Information,
                     NotifyPosition::Pos_Top_Right,
                     "信息提示",
                     "你好，世界！Hellow world!");
        });

        btn1->move(200, 50);
        btn2->move(200, 100);
        btn3->move(200, 150);
        btn4->move(200, 200);
        btn5->move(200, 250);
    };
private:
    Notification* nf;
};


#include "autocomplete.h"
#include "search/searchengine.h"

// class AutoCompleteTest : public QWidget {
//     Q_OBJECT
// public:
//     explicit AutoCompleteTest(QWidget *parent = nullptr)
//         : QWidget(parent)
//     {
//         // 初始化UI
//         setupUI();
//         // 初始化搜索数据
//         initSearchData();
//         // 连接信号槽
//         connectSignals();
//     }

// private:
//     void setupUI() {
//         QVBoxLayout *layout = new QVBoxLayout(this);

//         // 创建输入框
//         m_inputEdit = new QLineEdit(this);
//         m_inputEdit->setPlaceholderText("输入工具名称...");
//         layout->addWidget(m_inputEdit);

//         // 创建搜索按钮
//         QPushButton *searchBtn = new QPushButton("搜索", this);
//         layout->addWidget(searchBtn);

//         // 结果显示区域
//         m_resultLabel = new QLabel("选择结果将显示在这里", this);
//         m_resultLabel->setAlignment(Qt::AlignCenter);
//         m_resultLabel->setStyleSheet("QLabel { background-color: #f0f8ff; padding: 10px; border-radius: 5px; }");
//         layout->addWidget(m_resultLabel);

//         // 创建自动完成组件
//         m_autoComplete = new AutoComplete(this);
//         m_searchAutoComplete = new AutoComplete(this);

//         // 设置样式
//         this->setStyleSheet("QWidget { font-family: 'Segoe UI'; }"
//                             "QLineEdit { padding: 8px; font-size: 14px; border-radius: 4px; border: 1px solid #ccc; }"
//                             "QPushButton { padding: 8px; background-color: #4CAF50; color: white; border: none; border-radius: 4px; }"
//                             "QPushButton:hover { background-color: #45a049; }");
//         this->resize(500, 300);
//     }

//     void initSearchData() {
//         // 添加测试数据到搜索引擎
//         m_searchEngine.insertRecord("文本处理", 1, SearchMetaData<Page::PAGE_TOOLS>("文本工具", "文本处理"));
//         m_searchEngine.insertRecord("图像编辑", 2, SearchMetaData<Page::PAGE_TOOLS>("图像工具", "图像编辑"));
//         m_searchEngine.insertRecord("文件压缩", 3, SearchMetaData<Page::PAGE_TOOLS>("文件工具", "文件压缩"));
//         m_searchEngine.insertRecord("PDF转换", 4, SearchMetaData<Page::PAGE_TOOLS>("文档工具", "PDF转换"));
//         m_searchEngine.insertRecord("屏幕截图", 5, SearchMetaData<Page::PAGE_TOOLS>("系统工具", "屏幕截图"));
//         m_searchEngine.insertRecord("OCR识别", 6, SearchMetaData<Page::PAGE_TOOLS>("图像工具", "OCR识别"));
//         m_searchEngine.insertRecord("视频转换", 7, SearchMetaData<Page::PAGE_TOOLS>("视频工具", "视频转换"));
//         m_searchEngine.insertRecord("音频编辑", 8, SearchMetaData<Page::PAGE_TOOLS>("音频工具", "音频编辑"));
//         m_searchEngine.insertRecord("数据恢复", 9, SearchMetaData<Page::PAGE_TOOLS>("文件工具", "数据恢复"));
//         m_searchEngine.insertRecord("网络分析", 10, SearchMetaData<Page::PAGE_TOOLS>("网络工具", "网络分析"));
//     }

//     void connectSignals() {
//         // 输入框文本变化时触发自动补全
//         connect(m_inputEdit, &QLineEdit::textChanged, this, &AutoCompleteTest::onTextChanged);

//         // 自动补全选择处理
//         connect(m_autoComplete, &AutoComplete::completerSelected, this, [this](const QString &text) {
//             m_resultLabel->setText(QString("选择了: %1").arg(text));
//             m_inputEdit->setText(text);
//         });

//         // 搜索结果选择处理
//         connect(m_searchAutoComplete, &AutoComplete::itemSelected, this, [this](const QString &group, const QString &function) {
//             m_resultLabel->setText(QString("选择了: %1 > %2").arg(group, function));
//             m_inputEdit->setText(function);
//         });
//     }

// private slots:
//     void onTextChanged(const QString &text) {
//         if (text.isEmpty())  {
//             m_autoComplete->hide();
//             m_searchAutoComplete->hide();
//             return;
//         }

//         //前缀匹配的自动补全
//         auto prefixIds = m_searchEngine.getPrefixIds(m_inputEdit->text().toStdString());
//         QStringList items;
//         for (const auto &id : prefixIds) {
//             items << QString::fromStdString(m_searchEngine.getMetaData(id).function);
//         }
//         m_autoComplete->setItems(items);

//         // 显示在输入框下方
//         QPoint pos = m_inputEdit->mapToGlobal(QPoint(0, m_inputEdit->height()));
//         m_autoComplete->move(pos);
//         m_autoComplete->setFixedWidth(m_inputEdit->width());

//         //关键词搜索（分词匹配）
//         auto keywordIds = m_searchEngine.searchKeywords(text.toStdString());
//         QList<QPair<QString, QString>> results;
//         for (const auto &id : keywordIds) {
//             const auto &meta = m_searchEngine.getMetaData(id);
//             results.append(qMakePair(
//                 QString::fromStdString(meta.header),
//                 QString::fromStdString(meta.function)
//                 ));
//         }
//         m_searchAutoComplete->setSearchResults(results);

//         // 显示在输入框下方（在自动补全下方）
//         if (!results.isEmpty())  {
//             QPoint searchPos = pos + QPoint(0, m_autoComplete->height());
//             m_searchAutoComplete->move(searchPos);
//             m_searchAutoComplete->setFixedWidth(m_inputEdit->width());
//         }
//     }

// private:
//     QLineEdit *m_inputEdit;
//     QLabel *m_resultLabel;
//     AutoComplete *m_autoComplete;
//     AutoComplete *m_searchAutoComplete;
//     SearchEngine<Page::PAGE_TOOLS> m_searchEngine;
// };

#endif // MAINWINDOW_H

#include "globalkeyeventmgr.h"
#include "loginorsignupwidget.h"
#include "main_window.h"
#include <QApplication>
#include "cardgroup.h"
#include "ocrviewer.h"
#include "panel.h"
#include "pdfwatermarkdlg.h"
#include "screen.h"
#include "srcselector.h"
#include "ocrclient.h"
#include "ocrtextdlg.h"
#include "markeditor.h"
#include "hotkeymapper.h"
#include "localkeyeventmgr.h"
#include "TestWindow.h"
#include "autocomplete.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // LocalKeyEventMgr local;
    // qApp->installEventFilter(&local);

    //加载快捷键配置
    HotkeyMapper::GetInstance()->loadConfig("D:/Qt_Projects/GreatScanner/configs/ShortCut.json");

    //LoginOrSignUpWidget login;
    //SET_MICA_BACKGROUND(&login);
    //login.show();
    MainWindow main;
    main.show();

    // AutoComplete completer;

    // for(int i=0;i<30;i++){
    //     // 添加工具页面结果测试
    //     SearchMetaData<Page::PAGE_TOOLS> toolData(QString("Header %1").arg(i).toStdString(), QString("Function %1").arg(i).toStdString());
    //     completer.appendResult(QString("Header %1").arg(i), toolData);
    // }
    // completer.show();

    // MarkEditor m;
    // m.show();
    // Screen s;
    // s.show();

    // PdfWaterMarkDlg p(nullptr, "C:\\Users\\田卫军\\Desktop\\Web开发框架课堂实验4——Spring Boot安全服务.pdf");
    // p.show();

    // QProcess checkPython;
    // checkPython.start("D:\\pythonsoft\\python.exe",  {"--version"});
    // if (!checkPython.waitForFinished(1000)  || checkPython.exitCode()  != 0) {
    //     qDebug()<<"Python environment not found";
    // }
    // SrcSelector s;
    // s.show();
    // QObject::connect(&s, &SrcSelector::getPath, [](const QString& path){
    //     OcrClient::GetInstance()->PostOcrTask(ipc::IpcId::Ocr_Default, path.toStdString().c_str());
    // });
    // OcrClient::DestoryInstance();

    //Test Ocr/Ipc operations:
    //for(int i=0;i<4;i++){
        // OcrClient::GetInstance()->PostOcrTask(ipc::IpcId::Ocr_Default
        //                                       , "D:\\Qt_Projects\\GreatScanner\\savePictures\\Capture_20250611_151520.png");
    //}
    //OcrClient::DestoryInstance();

    // QObject::connect(&login, &LoginOrSignUpWidget::loginSuccessfully, [&]() {
    //     login.close();    // 关闭登录窗口
    //     main.show();      // 显示主窗口
    // });

    // OcrTextDlg dlg;
    // dlg.show();

    //OCRViewer Test
    // nlohmann::json data = nlohmann::json::parse(R"(
    //         {"detected_count":4,"image_path":"D:/Qt_Projects/GreatScanner/savePictures/Capture_20250616_110825.png","text_objects":[{"box":[[468,377],[806,377],[806,407],[468,407]],"text":"你带来的冰淇淋般的甜量"},{"box":[[462,330],[806,329],[806,363],[462,365]],"text":"夏季，在我心中蔓延的是"},{"box":[[536,259],[714,255],[716,295],[537,299]],"text":"S"},{"box":[[581,186],[674,197],[669,247],[576,236]],"text":"fep"}]}
    //     )");
    // OCRViewer viewer;
    // viewer.setWindowTitle("OCR 文字选择工具 - 2025年6月16日");
    // viewer.resize(1200,  800);
    // viewer.show();
    // viewer.loadData(data);


    /*ImgEditor Test*/
    //loadConfig();
    // ImgEditor w(nullptr, imgEditor_input_path);
    // w.show();
    // SET_MICA_BACKGROUND(&w);

    //eApp->init();

    /* CardView Test */
    // // 创建主窗口
    // QWidget mainWindow;
    // mainWindow.setWindowTitle("Single Card Example");
    // mainWindow.resize(400,  600);
    // mainWindow.setStyleSheet("background-color:  #ffffff;");

    // // 创建垂直布局
    // QVBoxLayout *layout = new QVBoxLayout(&mainWindow);
    // layout->setContentsMargins(20, 20, 20, 20);
    // layout->setAlignment(Qt::AlignCenter);

    // // 创建 CardView 实例
    // CardView *card = new CardView;
    // card->setFixedSize(200, 100); // 设置卡片大小
    // card->setBorderRadius(15);    // 设置圆角半径

    // // 设置卡片背景图片
    // QPixmap pixmap(":/image/MicaBase.png");  // 替换为你的图片路径
    // if (!pixmap.isNull())  {
    //     card->setCardPixmap(pixmap);
    //     card->setHorizontalPixmapRatio(1); // 设置图片宽度比例
    //     card->setVerticalPixmapRatio(1);   // 设置图片高度比例
    // }

    // card->setDesBelow("test test test test test test test");
    // int cnt = 0;
    // // 连接点击信号
    // CardView::connect(card, &CardView::cardClicked, [&cnt](void) {
    //     qDebug() << "Card clicked!" <<cnt++;
    // });

    // //将卡片添加到布局中
    // layout->addWidget(card);

    // // 显示主窗口
    // mainWindow.show();

    /*CardViews with FlowLayout Test*/
    // QWidget w;
    // w.resize(400, 600);
    // FlowLayout* flowLayout = new FlowLayout(&w, 0, 5, 5);
    // flowLayout->setContentsMargins(30, 0, 0, 0);
    // flowLayout->setAnimationEnabled(true);
    // for(int i=0;i<8;++i){
    //     CardView *card = new CardView;
    //     card->setFixedSize(200, 100); // 设置卡片大小
    //     card->setBorderRadius(15);    // 设置圆角半径

    //     // 设置卡片背景图片
    //     QPixmap pixmap(":/image/MicaBase.png");  // 替换为你的图片路径
    //     if (!pixmap.isNull())  {
    //         card->setCardPixmap(pixmap);
    //         card->setHorizontalPixmapRatio(1); // 设置图片宽度比例
    //         card->setVerticalPixmapRatio(1);   // 设置图片高度比例
    //     }

    //     card->setDesBelow("test");
    //     flowLayout->addWidget(card);
    // }
    // w.setLayout(flowLayout);
    // w.show();

    /*CardGroup，Panel(带标题的CardGroup-Widget) Test*/
    //创建CardGroup对象
    // CardGroup* cardGroup = new CardGroup;
    // {
    //     // 添加多个CardView
    //     for (int i = 0; i < 10; ++i) {
    //         CardView *card = new CardView;
    //         card->setFixedSize(150, 150); // 设置卡片大小
    //         card->setBorderRadius(15);    // 设置圆角半径

    //         // 设置卡片背景图片
    //         QPixmap pixmap(":/image/MicaBase.png");  // 图片路径
    //         if (!pixmap.isNull())  {
    //             card->setCardPixmap(pixmap);
    //             card->setHorizontalPixmapRatio(1); // 设置图片宽度比例
    //             card->setVerticalPixmapRatio(1);   // 设置图片高度比例
    //         }

    //         card->setDesBelow("test");

    //         //连接点击信号
    //         CardView::connect(card, &CardView::cardClicked, [i]() {
    //             qDebug() << "Card" << i+1 << "clicked!";
    //         });

    //         cardGroup->addCardView(card);
    //     }
    // }
    // cardGroup->setBorderRadius(10);
    // Panel panel(cardGroup, "测试标题");
    // panel.resize(500, 720);
    // panel.show();

    /*InfoCard Test*/
    // InfoCardTest window;
    // window.show();

    /*RoundProgressBar Test*/
    // ProgressBarTest window;
    // window.show();

    /*ListView Test(弃用)*/

    /*TaskWidget Test(a better ListView)*/
    // TaskWidget w;
    // w.slot_appendTask(MsgId::ID_TEST_UPLOAD_FILE_REQ, ":/image/MicaBase.png");
    // w.slot_popTask();
    // w.show();

    /*Notifacation Test*/
    // NotifacationTest nft;
    // nft.show();

    /*LoginOrSignUpWidget Test*/
    // LoginOrSignUpWidget w;
    // w.show();
    // SET_MICA_BACKGROUND(&w);

    // qDebug() << "Active top-level windows:"
    //          << QApplication::topLevelWindows().count();
    return a.exec();
}


// #include <iostream>

// #include "minidocx.hpp"
// using namespace docx;
// int main()
// {
//     Document doc;

//     auto p1 = doc.AppendParagraph("Hello, World!", 12, "Times New Roman");

//     auto p4 = doc.AppendParagraph();
//     p4.SetAlignment(Paragraph::Alignment::Centered);

//     auto p4r1 = p4.AppendRun("Sample text here...", 12, "Arial");
//     p4r1.AppendLineBreak();
//     p4r1.SetCharacterSpacing(Pt2Twip(2));

//     auto p4r2 = p4.AppendRun("And another line of sample text here...");
//     p4r2.SetFontSize(14);
//     p4r2.SetFont("Times New Roman");
//     p4r2.SetFontColor("FF0000");
//     p4r2.SetFontStyle(Run::Bold | Run::Italic);

//     doc.Save("D:\\Qt_Projects\\GreatScanner\\saveDocxes\\test.docx");
//     return 0;
// }

#include "controlwidget.h"
#include "ocrtextdlg.h"
#include "ui_controlwidget.h"
#include <QPoint>
#include "screen.h"
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <qfileinfo.h>
#include "ocrclient.h"
#include <QDir>
#include <QUuid>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "opencv2/opencv.hpp"

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget),
    screen(NULL)
{
    ui->setupUi(this);
    connectSignalAndSlot();
}

void ControlWidget::connectSignalAndSlot()
{
    connect(OcrClient::GetInstance().get(), &OcrClient::sig_RespOcrRes, this, [this](QByteArray data){
        OcrTextDlg* dlg = new OcrTextDlg(this);
        auto img = screen->getGrabImage();
        dlg->showOcrRes(data, img);
        dlg->show();
    });
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(cancelBtn_slot()));
    connect(ui->saveBtn, SIGNAL(clicked()), this, SLOT(saveBtn_slot()));
    connect(ui->finishBtn, SIGNAL(clicked()), this, SLOT(finishBtn_slot()));
}
//撤销截图按钮
void ControlWidget::cancelBtn_slot()
{
    if( screen )
    {
        screen->close();
        screen->Exit();
    }
}
//保存截图按钮
void ControlWidget::saveBtn_slot()
{
    screen->savePixmap();
    cancelBtn_slot();
}

//完成按钮将截图保存到剪贴板
void ControlWidget::finishBtn_slot()
{
    QClipboard *board = QApplication::clipboard();
    board->setImage(screen->getGrabImage());
    cancelBtn_slot();
}


//保存Screen类的引用
void ControlWidget::setScreenQuote(Screen *screen)
{
    this->screen = screen;
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::on_drawLineBtn_clicked()
{

    screen->drawline();
}



void ControlWidget::on_textEditBtn_clicked()
{
    screen->textedit();
}

void ControlWidget::on_rectangleBtn_clicked()
{
    screen->drawrectang();
}

void ControlWidget::on_drawRoundBtn_clicked()
{
    screen->drawround();
}

void ControlWidget::on_arrowBtn_clicked()
{
    screen->drawarrow();
}

void ControlWidget::on_mosaicBtn_clicked()
{
    //暂不实现
}

void ControlWidget::on_returneditBtn_clicked()
{
    screen->returnEdit();
}

//恢复默认指针功能Qt::SizeAllCursor
void ControlWidget::on_moveCursorBtn_clicked()
{

}


void ControlWidget::on_ocrBtn_clicked()
{
    // 获取截图并转换为QImage
    auto image  = screen->getGrabImage().convertToFormat(QImage::Format_RGB888);

    // 转换为cv::Mat并编码为PNG
    cv::Mat mat(image.height(),  image.width(),  CV_8UC3,
                const_cast<uchar*>(image.bits()),  image.bytesPerLine());

    // 编码为内存中的PNG
    std::vector<uchar> buffer;
    cv::imencode(".png", mat, buffer);

    // 构造JSON数据包
    QJsonObject packet;
    packet["type"] = "image_data";
    packet["format"] = "png";
    packet["data"] = QString(QByteArray(
                                 reinterpret_cast<const char*>(buffer.data()),
                                 buffer.size()
                                 ).toBase64());

    // 发送任务
    QJsonDocument doc(packet);
    try{
        OcrClient::GetInstance()->PostOcrTask(
            ipc::IpcId::Ocr_ScreenShot,
            doc.toJson(QJsonDocument::Compact)
            );
    }catch(const std::exception& e){
        qDebug()<<e.what();
    }


    qDebug() << "直接发送图像数据完成，buffer大小:" << buffer.size()  << "字节";

    // cancelBtn_slot();//退出
}



#include "ocrtextdlg.h"
#include "ui_ocrtextdlg.h"
#include <QString>
#include <qclipboard.h>

OcrTextDlg::OcrTextDlg(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OcrTextDlg)
{
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    //setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时对象销毁
    setWindowTitle("Ocr结果");
    ui->setupUi(this);

    QObject::connect(ui->ocrViewer, &OCRViewer::textReady, [this](const QString& formattedText){
        ui->ocrOutput->append(formattedText);
    });
}

OcrTextDlg::~OcrTextDlg()
{
    delete ui;
}

void OcrTextDlg::showOcrRes(QByteArray data)
{
    m_OcrResp = json::parse(data.toStdString().c_str());
    ui->ocrViewer->loadData(m_OcrResp);
}

void OcrTextDlg::showOcrRes(QByteArray data, QImage &image)
{
    m_OcrResp = json::parse(data.toStdString().c_str());
    ui->ocrViewer->loadData(m_OcrResp, image);
}

void OcrTextDlg::on_copyBtn_clicked()
{
    QApplication::clipboard()->setText(ui->ocrOutput->toPlainText());
}


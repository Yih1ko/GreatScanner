#ifndef OCRTEXTDLG_H
#define OCRTEXTDLG_H

#include <QWidget>
#include "json.hpp"
#include "QImage"

using json = nlohmann::json;
namespace Ui {
class OcrTextDlg;
}

class OcrTextDlg : public QWidget
{
    Q_OBJECT

public:
    explicit OcrTextDlg(QWidget *parent = nullptr);
    ~OcrTextDlg();

private:
    Ui::OcrTextDlg *ui;
    json m_OcrResp;

public slots:
    void showOcrRes(QByteArray data);
    void showOcrRes(QByteArray data, QImage &image);
private slots:
    void on_copyBtn_clicked();
};

#endif // OCRTEXTDLG_H

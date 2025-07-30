#ifndef PDFWATERMARKDLG_H
#define PDFWATERMARKDLG_H

#include <QWidget>
#include <QPdfDocument>
#include <QPdfPageRenderer>
#include <QLabel>
#include "markeditor.h"
// #include <mupdf/fitz.h>

namespace Ui {
class PdfWaterMarkDlg;
}

class PdfWaterMarkDlg : public QWidget
{
    Q_OBJECT

public:
    explicit PdfWaterMarkDlg(QWidget *parent = nullptr, QString docPath = "");
    ~PdfWaterMarkDlg();

public slots:
    void clearMarks();

private:
    Ui::PdfWaterMarkDlg *ui;
    MarkEditor* m_markEditor = nullptr;
    QPdfDocument *m_pdfDocument = nullptr;
    QPdfPageRenderer *m_pageRenderer = nullptr;
    QLabel *m_imageLabel = nullptr;
    QString m_docPath;
    MarkMeta m_currentMark;
    QHash<int, QImage> m_pageCache; // 页面缓存
    void initUI();
    void initSlots();
    void loadPdf();
    void renderWatermarkedPreview();
    void renderWatermarkToPainter(QPainter &painter, const QSizeF &pageSize);
    //int getSourceDpiFromMuPdf(const QString& path);

private slots:
    void updateMark(MarkMeta data);
    void savePdf();
    void handlePageRendered(int pageNumber, QSize size, const QImage &image);

signals:
    void pdfLoaded(bool success);
};

#endif // PDFWATERMARKDLG_H

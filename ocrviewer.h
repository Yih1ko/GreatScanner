#ifndef OCRVIEWER_H
#define OCRVIEWER_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <qapplication.h>
#include <qevent.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "placeholdertextitem.h"
#include "json.hpp"
#include <QImage>

class OCRViewer : public QGraphicsView {
    Q_OBJECT
public:
    OCRViewer(QWidget* parent = nullptr);

signals:
    void textReady(const QString& formattedText);  // 格式化文本就绪信号

public slots:
    void loadData(const nlohmann::json& jsonData);
    void loadData(const nlohmann::json& jsonData, QImage &image);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    int charIndexAtPosition(PlaceholderTextItem* item, const QPointF& scenePos);
    void clearAllSelections();
    QString formatAllText() const;  // 格式化全部文本
    void updateAreaSelection(const QRectF& rect);
    void applySelectionToItem(PlaceholderTextItem* item, const QRectF& selectRect);

    QList<PlaceholderTextItem*> getItemsInSelection() const;
    QList<PlaceholderTextItem*> m_textItems; // 所有文本项集合
    QPointF m_selectionStartPos;             // 区域选择起点
    QRectF m_currentSelectionRect;           // 当前选择区域
    bool m_isAreaSelecting = false;          // 区域选择标志
    PlaceholderTextItem* m_currentItem = nullptr;
    QPointF m_startPos;
    int m_selectionAnchor = -1;
    bool m_isSelecting = false;
};

#endif // OCRVIEWER_H

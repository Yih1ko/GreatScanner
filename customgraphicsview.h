#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include <QGraphicsView>

class CustomGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    using QGraphicsView::QGraphicsView;
    //explicit CustomGraphicsView(QWidget *parent = nullptr);

protected:
    // 重写事件处理函数
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_spacePressed = false;   // 空格键按下状态
    bool m_dragging = false;       // 是否正在拖拽
    QPoint m_lastDragPos;          // 拖拽起点坐标
};

#endif // CUSTOMGRAPHICSVIEW_H

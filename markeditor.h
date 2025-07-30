#ifndef MARKEDITOR_H
#define MARKEDITOR_H

#include "colorblock.h"
#include <QWidget>
#include <QMouseEvent>

struct MarkMeta {
    QString m_text;
    QString m_fontFamily = "Arial"; // 添加字体成员
    QColor m_color = Qt::gray;
    int m_size = 48;
    int m_opacity = 30; // 0-100
};
namespace Ui {
class MarkEditor;
}

class MarkEditor : public QWidget
{
    Q_OBJECT

public:
    explicit MarkEditor(QWidget *parent = nullptr);
    ~MarkEditor();

signals:
    void markMeta(MarkMeta);

protected:
    void paintEvent(QPaintEvent *event) override;  // 重写绘制事件
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;  // 处理窗口大小变化

private:
    Ui::MarkEditor *ui;
    MarkMeta m_markMeta;
    void initUI();
    void initSlots();
    QPoint m_dragPosition;
    bool m_dragging = false;
    bool m_selected = false;
    QList<ColorBlock*> m_colorBlocks;
private slots:
    // 处理颜色块选中状态变化
    void onColorBlockSelected(bool selected);
};

#endif // MARKEDITOR_H

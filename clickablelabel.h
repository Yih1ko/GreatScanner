
#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags());
    explicit ClickableLabel(const QString& text,
                            QWidget* parent = nullptr,
                            Qt::WindowFlags f = Qt::WindowFlags());

signals:
    // 基础点击信号
    void clicked();
    // 带坐标参数的点击信号
    void clicked(QPoint pos);
    // 带鼠标按键参数的点击信号
    void clicked(Qt::MouseButton button);

protected:
    // 鼠标事件处理
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    bool m_pressed; // 跟踪按下状态
};

#endif // CLICKABLELABEL_H

#ifndef IMGEDITOR_H
#define IMGEDITOR_H

#include <QWidget>
#include "frame.h"
#include <QUndoStack>
#include <QUndoCommand>
#include "undo_stack_singleton.h"

constexpr double MIN_SCALE = 0.1;
constexpr double MAX_SCALE = 10.0;

static const double SCALE_FACTOR_PER_DEGREE = 0.003; // 每度变化因子

namespace Ui {
class ImgEditor;
}

class ImgEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ImgEditor(QWidget *parent = nullptr, QString imgPath = "");
    ~ImgEditor();

private slots:

    void on_unDoBtn_clicked();

    void on_reDoBtn_clicked();

    void onPointSelected(PointItem* item, bool state);

    void on_upBtn_clicked();

    void on_downBtn_clicked();

    void on_lBtn_clicked();

    void on_rBtn_clicked();

    void viewScale(int value);

    void on_rotate_valueChanged(int value);

    void handleCrop();

    void on_OKBtn_clicked();

    void on_rotate_sliderReleased();

    void on_rotate_sliderPressed();

public slots:
    void moveSelectedPoint(qreal dx, qreal dy);

private:
    Ui::ImgEditor *ui;
    PointItem* m_selectedPoint = nullptr; // 当前选中点
    QGraphicsScene* m_scene;
    double m_scaleFactor;  // 记录当前缩放比例（初始1.0）
    int m_lastDialValue;   // 记录旋钮上一次值（用于计算增量）
    double m_currentScale; // 当前缩放因子
    // double m_currentXScale; // 当前X轴缩放因子
    // double m_currentYScale; // 当前Y轴缩放因子
    Frame* quad; //裁剪框
    QString m_imgPath;//背景图位置
    QGraphicsPixmapItem *backgroundItem;//背景图
    QPointF imageCenter;//背景图中心点
    int m_startAngle;//起始旋转角度
};

#endif // IMGEDITOR_H

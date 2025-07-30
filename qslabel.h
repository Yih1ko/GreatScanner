#ifndef QSLABEL_H
#define QSLABEL_H

#include <QLabel>
#include <QWidget>
#include <QTextEdit>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRgb>

class Screen;

// 图形元素结构体定义
typedef struct myLine {
    QPoint startPoint;
    QPoint endPoint;
} myLine;

typedef struct myRectangle {
    QPoint startPoint;
    QPoint endPoint;
} myRectangle;

typedef struct myRound {
    QPoint startPoint;
    QPoint endPoint;
} myRound;

typedef struct myArrow {
    QPoint startPoint;
    QPoint endPoint;
} myArrow;

typedef struct myText {
    QString mText;
    QRect mRect;
    bool isSelected;  // 新增：选中状态
    bool isResizing;  // 新增：调整大小状态
    int resizeEdge;   // 新增：当前调整的边 (0=无,1=上,2=下,3=左,4=右)
} myText;

/**
 * @brief 支持图形绘制的自定义QLabel
 */
class QSLabel : public QLabel
{
    Q_OBJECT
public:
    explicit QSLabel(QWidget *parent);

    // 鼠标事件重写
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    // 绘图模式设置
    void setdrawlineenable();
    void setrectangleenable();
    void setdrawarrowenable();
    void setroundenable();
    void settexteditenable();
    void setReturnEditEnable();

    // 功能方法
    void drawarrow(QPoint startpoint, QPoint endpoint, QPainter &p);
    void setimagetolabel(const QImage &image);
    void settextedittovector();
    QImage resultimage();

    // 文本矩形框调整相关方法
    bool isNearTextEdge(const QPoint &point, myText *text, int &edge);
    void updateCursorForTextResize(const QPoint &point);
    void resizeTextRect(myText *text, const QPoint &delta, int edge);

public slots:
    void ontextchanged();

private:
    // 图形元素存储
    QVector<myLine*> lines;
    QVector<myRectangle*> rectangles;
    QVector<myRound*> rounds;
    QVector<myText*> texts;
    QVector<myArrow*> arrows;
    QVector<QString> actionVec;

    // 状态控制
    QPoint startPoint;
    QPoint endPoint;
    QImage selectimage;
    QTextEdit *m_plaintextedit;
    QString text;
    bool ispressed;
    bool isdrawline;
    bool isdrawrectangle;
    bool isdrawround;
    bool istextedit;
    bool isdrawarrow;
    bool isreturnedit;
    Screen *screen;

    //文本调整状态
    myText *currentResizingText; // 当前正在调整的文本
    QPoint resizeStartPoint;     // 调整开始时的鼠标位置
    QRect originalResizeRect;    // 调整开始时的矩形

    // 坐标转换辅助方法
    QPoint logicalToPhysical(const QPoint& logicalPoint) {
        return logicalPoint * devicePixelRatioF();
    }

    QPoint physicalToLogical(const QPoint& physicalPoint) {
        return physicalPoint / devicePixelRatioF();
    }
};

#endif // QSLABEL_H

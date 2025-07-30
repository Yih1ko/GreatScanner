#ifndef INFOCARD_H
#define INFOCARD_H

#include "Def.h"
#include "simplebutton.h"
#include "textlabel.h"
#include "cardview.h"
#include <QHoverEvent>
#include <QEvent>
#include <QIcon>
#include <QPixmap>
#include <QVector>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <tuple>
#include <QFileInfo>


class InfoCard : public QWidget
{
    Q_OBJECT
public:
    explicit InfoCard(QWidget *parent = nullptr);
    ~InfoCard();
    void setBorderRadius(int radius);
    void setFileInfo(QFileInfo &info);
    QFileInfo getFileInfo() const;
    QString getDes(int index) const;
    std::tuple<QString, QString, QString> getInfo(int index) const;
    int getLen() const;
public slots:
    void setCardIcon(const QIcon& icon);
    void setCardIcon(const QPixmap& pixmap);
    void updateCon(TextPositon textPos, QString text);
    void updateCon(TextPositon textPos, double opacity);
    void appendDes(QString text);
    void updateDes(int index, QString text);
    void setWithBelowText(bool withBelow);
    //void drawDropShadow(bool yes);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    bool eventFilter(QObject *obj, QEvent* event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

signals:
    void clicked();
    void rightButtonClicked();
    void desPositionsUpdated(const QVector<QPoint>& positions);

private:
    //内容:
    CardView* m_card;   //图标(封面缩略图widget)
    TextLabel* m_upperLeft; //一般储存操作类型-文件名
    TextLabel* m_lowerLeft; //一般储存文件类型
    QFileInfo m_info;  //储存内部的文件本身信息

    //管理:
    QPushButton* m_menuDot;
    QHBoxLayout* m_mainLayout;
    QHBoxLayout* m_layoutOfDes;
    QHBoxLayout* m_desContentLayout;
    QVector<TextLabel*> m_rightDes; //一般描述(index->TextLabel*)

    int m_borderRadius;
    bool m_pressed;
    bool m_hovered;

    void initUI();

    static const int kMainLayoutSpacing = 110;     // 主布局间距
    static const int kCardContentSpacing = 10;      // 内容布局间距
    static const int kUpperTextLabelSpacing = 7;         // 上文本标签间距
    static const int kLowerTextLabelSpacing = 9;         // 下文本标签间距
    static const int kCardSize = 55;                // 卡片大小
    static const int kCardBorderRadius = 8;        // 卡片圆角半径
    static const int kRightDesSpacing = 25;         // 右侧描述间距
};

#endif // INFOCARD_H

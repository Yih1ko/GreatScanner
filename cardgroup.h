#ifndef CARDGROUP_H
#define CARDGROUP_H

#include "flowlayout.h"
#include "cardview.h"

class CardGroup : public QWidget
{
    Q_OBJECT

public:
    explicit CardGroup(QWidget *parent = nullptr);
    ~CardGroup();

    // 添加 CardView 的公共接口
    void addCardView(CardView* cardView);
    CardView* getCardView(const QString& str);
    FlowLayout* layout() const { return m_flowLayout; } // 提供布局访问接口(用于添加CardView)

    // 边框属性的Setter
    void setBorderRadius(int radius);
    void setBorderColor(const QColor &color);
    void setBorderWidth(int width);

    // 背景颜色设置接口
    void setBackgroundColor(const QColor &color);

protected:
    void paintEvent(QPaintEvent *event) override;
    //void resizeEvent(QResizeEvent *event) override;

private:
    FlowLayout *m_flowLayout;
    int m_borderRadius;
    QColor m_borderColor;
    int m_borderWidth;
    QColor m_backgroundColor; // 新增背景颜色成员变量
    QHash<QString, CardView*> m_cardMap;
};

#endif // CARDGROUP_H

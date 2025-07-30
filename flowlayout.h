#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QStyle>
#include <QList>
#include <QHash>
#include <qparallelanimationgroup.h>
#include <qpropertyanimation.h>

class FlowLayout : public QLayout
{
    Q_OBJECT
public:
    // 构造函数
    explicit FlowLayout(QWidget* parent,
                        int margin = -1,
                        int hSpacing = -1,
                        int vSpacing = -1);
    explicit FlowLayout(int margin = -1,
                        int hSpacing = -1,
                        int vSpacing = -1);
    ~FlowLayout();

    // 核心接口
    void addItem(QLayoutItem* item) override;
    int horizontalSpacing() const;
    int verticalSpacing() const;
    void setAnimationEnabled(bool enabled);

    // 必须重写的 QLayout 虚函数
    Qt::Orientations expandingDirections() const override;
    bool hasHeightForWidth() const override;
    int heightForWidth(int width) const override;
    int count() const override;
    QLayoutItem* itemAt(int index) const override;
    QSize minimumSize() const override;
    void setGeometry(const QRect& rect) override;
    QSize sizeHint() const override;
    QLayoutItem* takeAt(int index) override;

private:
    // 内部工具函数
    int smartSpacing(QStyle::PixelMetric pm) const;
    int doLayout(const QRect& rect, bool testOnly) const;

    // 成员变量
    QList<QLayoutItem*> m_items;
    mutable QHash<QLayoutItem*, QPoint> m_lastGeometryMap; // 关键点9：必须使用 QPoint 记录位置
    bool m_isAnimation = true;
    int m_hSpacing;
    int m_vSpacing;

    mutable QParallelAnimationGroup* m_animationGroup = nullptr; // 动画组
    mutable QHash<QLayoutItem*, QPropertyAnimation*> m_activeAnimations; // 动画复用池
    bool m_animating = false;
};

#endif // FLOWLAYOUT_H

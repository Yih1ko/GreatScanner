#ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <QWidget>
#include "search/SearchDef.h"
#include "searchbox.h"
#include "ui_autocomplete.h"
#include <QHash>

namespace Ui {
class AutoComplete;
}

class AutoComplete : public QWidget
{
    Q_OBJECT

public:
    explicit AutoComplete(QWidget *parent = nullptr);
    ~AutoComplete();
    void appendResult(const QString& text, const MetaDataVariant& metaData);
    void clearResults();
    void updatePosition(SearchBox *anchorWidget){
        QPoint globalPos = anchorWidget->mapToGlobal(QPoint(0, anchorWidget->height()));
        // 移除父窗口映射逻辑
        move(globalPos); // 直接使用全局坐标
        setFixedWidth(anchorWidget->width()); // 保持宽度一致
    }

signals:
    void selectedItem(MetaDataVariant);
    void shown();  // 新增
    void hidden(); // 新增

protected:
    void paintEvent(QPaintEvent *event) override;

    void showEvent(QShowEvent* event) override {
        QWidget::showEvent(event);
        emit shown();
    }

    void hideEvent(QHideEvent* event) override {
        QWidget::hideEvent(event);
        emit hidden();
    }
private:
    Ui::AutoComplete *ui;
    QVBoxLayout *m_layout = nullptr;

    void updatePosition();
};

#endif // AUTOCOMPLETE_H

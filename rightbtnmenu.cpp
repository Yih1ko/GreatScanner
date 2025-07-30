#include "rightbtnmenu.h"

RightBtnMenu::RightBtnMenu(QWidget *parent)
{
    setStyleSheet(R"(
    QMenu {
        background-color: #f0f0f0;
        border: 1px solid #999;
        border-radius: 5px;
        padding: 5px;
    }
    QMenu::item {
        background-color: transparent;
        color: #333;
        padding: 8px 16px;
    }
    QMenu::item:selected {
        background-color: #4CAF50;
        color: white;
    }
    QMenu::separator {
        height: 1px;
        background-color: #ccc;
        margin: 4px 0;
    }
)");
}

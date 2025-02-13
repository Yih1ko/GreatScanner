/********************************************************************************
** Form generated from reading UI file 'imageeditor.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEEDITOR_H
#define UI_IMAGEEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImgEditor
{
public:
    QHBoxLayout *horizontalLayout_2;
    QGraphicsView *graphicsView;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    QToolButton *lBtn;
    QToolButton *rBtn;
    QToolButton *upBtn;
    QToolButton *downBtn;
    QWidget *widget_4;
    QGridLayout *gridLayout_2;
    QLabel *scaleXLabel;
    QSlider *scaleX;
    QLabel *scaleYLabel;
    QSlider *scaleY;
    QLabel *rotateLabel;
    QSlider *rotate;
    QWidget *widget_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *unDoBtn;
    QPushButton *reDoBtn;
    QPushButton *OKBtn;

    void setupUi(QWidget *ImgEditor)
    {
        if (ImgEditor->objectName().isEmpty())
            ImgEditor->setObjectName("ImgEditor");
        ImgEditor->resize(400, 300);
        horizontalLayout_2 = new QHBoxLayout(ImgEditor);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        graphicsView = new QGraphicsView(ImgEditor);
        graphicsView->setObjectName("graphicsView");

        horizontalLayout_2->addWidget(graphicsView);

        widget = new QWidget(ImgEditor);
        widget->setObjectName("widget");
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName("verticalLayout");
        widget_2 = new QWidget(widget);
        widget_2->setObjectName("widget_2");
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        lBtn = new QToolButton(widget_2);
        lBtn->setObjectName("lBtn");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imageEditor/\345\267\246.png"), QSize(), QIcon::Normal, QIcon::Off);
        lBtn->setIcon(icon);

        gridLayout->addWidget(lBtn, 2, 0, 1, 1);

        rBtn = new QToolButton(widget_2);
        rBtn->setObjectName("rBtn");
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/imageEditor/\345\217\263.png"), QSize(), QIcon::Normal, QIcon::Off);
        rBtn->setIcon(icon1);

        gridLayout->addWidget(rBtn, 2, 2, 1, 1);

        upBtn = new QToolButton(widget_2);
        upBtn->setObjectName("upBtn");
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/imageEditor/\344\270\212.png"), QSize(), QIcon::Normal, QIcon::Off);
        upBtn->setIcon(icon2);

        gridLayout->addWidget(upBtn, 0, 1, 1, 1);

        downBtn = new QToolButton(widget_2);
        downBtn->setObjectName("downBtn");
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/imageEditor/\344\270\213.png"), QSize(), QIcon::Normal, QIcon::Off);
        downBtn->setIcon(icon3);

        gridLayout->addWidget(downBtn, 3, 1, 1, 1);


        verticalLayout->addWidget(widget_2);

        widget_4 = new QWidget(widget);
        widget_4->setObjectName("widget_4");
        gridLayout_2 = new QGridLayout(widget_4);
        gridLayout_2->setObjectName("gridLayout_2");
        scaleXLabel = new QLabel(widget_4);
        scaleXLabel->setObjectName("scaleXLabel");

        gridLayout_2->addWidget(scaleXLabel, 0, 0, 1, 1);

        scaleX = new QSlider(widget_4);
        scaleX->setObjectName("scaleX");
        scaleX->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(scaleX, 0, 1, 1, 1);

        scaleYLabel = new QLabel(widget_4);
        scaleYLabel->setObjectName("scaleYLabel");

        gridLayout_2->addWidget(scaleYLabel, 1, 0, 1, 1);

        scaleY = new QSlider(widget_4);
        scaleY->setObjectName("scaleY");
        scaleY->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(scaleY, 1, 1, 1, 1);

        rotateLabel = new QLabel(widget_4);
        rotateLabel->setObjectName("rotateLabel");

        gridLayout_2->addWidget(rotateLabel, 2, 0, 1, 1);

        rotate = new QSlider(widget_4);
        rotate->setObjectName("rotate");
        rotate->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_2->addWidget(rotate, 2, 1, 1, 1);


        verticalLayout->addWidget(widget_4);

        widget_3 = new QWidget(widget);
        widget_3->setObjectName("widget_3");
        horizontalLayout = new QHBoxLayout(widget_3);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(3, -1, 3, -1);
        unDoBtn = new QPushButton(widget_3);
        unDoBtn->setObjectName("unDoBtn");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/imageEditor/undo_no.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon4.addFile(QString::fromUtf8(":/imageEditor/undo_yes.png"), QSize(), QIcon::Normal, QIcon::On);
        unDoBtn->setIcon(icon4);

        horizontalLayout->addWidget(unDoBtn);

        reDoBtn = new QPushButton(widget_3);
        reDoBtn->setObjectName("reDoBtn");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/imageEditor/redo_no.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon5.addFile(QString::fromUtf8(":/imageEditor/redo_yes.png"), QSize(), QIcon::Normal, QIcon::On);
        reDoBtn->setIcon(icon5);

        horizontalLayout->addWidget(reDoBtn);


        verticalLayout->addWidget(widget_3);

        OKBtn = new QPushButton(widget);
        OKBtn->setObjectName("OKBtn");

        verticalLayout->addWidget(OKBtn);


        horizontalLayout_2->addWidget(widget);


        retranslateUi(ImgEditor);

        QMetaObject::connectSlotsByName(ImgEditor);
    } // setupUi

    void retranslateUi(QWidget *ImgEditor)
    {
        ImgEditor->setWindowTitle(QCoreApplication::translate("ImgEditor", "Form", nullptr));
        lBtn->setText(QString());
        rBtn->setText(QString());
        upBtn->setText(QString());
        downBtn->setText(QString());
        scaleXLabel->setText(QCoreApplication::translate("ImgEditor", "\346\213\211\344\274\270x", nullptr));
        scaleYLabel->setText(QCoreApplication::translate("ImgEditor", "\346\213\211\344\274\270y", nullptr));
        rotateLabel->setText(QCoreApplication::translate("ImgEditor", "\346\227\213\350\275\254\357\274\232", nullptr));
        unDoBtn->setText(QString());
        reDoBtn->setText(QString());
        OKBtn->setText(QCoreApplication::translate("ImgEditor", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImgEditor: public Ui_ImgEditor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEEDITOR_H

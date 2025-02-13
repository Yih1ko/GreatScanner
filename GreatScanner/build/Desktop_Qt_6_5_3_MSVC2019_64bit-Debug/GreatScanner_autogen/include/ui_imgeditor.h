/********************************************************************************
** Form generated from reading UI file 'imgeditor.ui'
**
** Created by: Qt User Interface Compiler version 6.5.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMGEDITOR_H
#define UI_IMGEDITOR_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "customgraphicsview.h"
#include "customtoolbutton.h"
#include "infinitedial.h"

QT_BEGIN_NAMESPACE

class Ui_ImgEditor
{
public:
    QHBoxLayout *horizontalLayout_2;
    CustomGraphicsView *graphicsView;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QGridLayout *gridLayout_2;
    QWidget *Operation;
    QVBoxLayout *verticalLayout;
    QWidget *Preview;
    QVBoxLayout *verticalLayout_4;
    QLabel *label;
    QWidget *widget_2;
    QGridLayout *gridLayout;
    CustomToolButton *rBtn;
    CustomToolButton *lBtn;
    CustomToolButton *upBtn;
    QSpacerItem *horizontalSpacer;
    CustomToolButton *downBtn;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_4;
    InfiniteDial *view_scale;
    QWidget *Trans;
    QVBoxLayout *verticalLayout_2;
    QFrame *line;
    QLabel *label_2;
    QWidget *widget_3;
    QGridLayout *gridLayout_3;
    QSlider *rotate;
    QLabel *rotateLabel;
    QWidget *Step;
    QHBoxLayout *horizontalLayout;
    QPushButton *unDoBtn;
    QPushButton *reDoBtn;
    QPushButton *OKBtn;

    void setupUi(QWidget *ImgEditor)
    {
        if (ImgEditor->objectName().isEmpty())
            ImgEditor->setObjectName("ImgEditor");
        ImgEditor->resize(862, 443);
        horizontalLayout_2 = new QHBoxLayout(ImgEditor);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        graphicsView = new CustomGraphicsView(ImgEditor);
        graphicsView->setObjectName("graphicsView");

        horizontalLayout_2->addWidget(graphicsView);

        dockWidget = new QDockWidget(ImgEditor);
        dockWidget->setObjectName("dockWidget");
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dockWidget->sizePolicy().hasHeightForWidth());
        dockWidget->setSizePolicy(sizePolicy);
        dockWidget->setMinimumSize(QSize(200, 431));
        dockWidget->setFeatures(QDockWidget::DockWidgetFeature::DockWidgetFloatable|QDockWidget::DockWidgetFeature::DockWidgetMovable);
        dockWidget->setWindowTitle(QString::fromUtf8(""));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName("dockWidgetContents");
        gridLayout_2 = new QGridLayout(dockWidgetContents);
        gridLayout_2->setObjectName("gridLayout_2");
        gridLayout_2->setContentsMargins(6, -1, 6, -1);
        Operation = new QWidget(dockWidgetContents);
        Operation->setObjectName("Operation");
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Operation->sizePolicy().hasHeightForWidth());
        Operation->setSizePolicy(sizePolicy1);
        verticalLayout = new QVBoxLayout(Operation);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, -1);
        Preview = new QWidget(Operation);
        Preview->setObjectName("Preview");
        verticalLayout_4 = new QVBoxLayout(Preview);
        verticalLayout_4->setSpacing(0);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(0, 0, 0, -1);
        label = new QLabel(Preview);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_4->addWidget(label);

        widget_2 = new QWidget(Preview);
        widget_2->setObjectName("widget_2");
        sizePolicy1.setHeightForWidth(widget_2->sizePolicy().hasHeightForWidth());
        widget_2->setSizePolicy(sizePolicy1);
        gridLayout = new QGridLayout(widget_2);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 4, 0, 0);
        rBtn = new CustomToolButton(widget_2);
        rBtn->setObjectName("rBtn");
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(rBtn->sizePolicy().hasHeightForWidth());
        rBtn->setSizePolicy(sizePolicy2);
        rBtn->setMinimumSize(QSize(30, 25));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/imageEditor/\345\217\263.png"), QSize(), QIcon::Normal, QIcon::Off);
        rBtn->setIcon(icon);

        gridLayout->addWidget(rBtn, 1, 3, 1, 1);

        lBtn = new CustomToolButton(widget_2);
        lBtn->setObjectName("lBtn");
        sizePolicy2.setHeightForWidth(lBtn->sizePolicy().hasHeightForWidth());
        lBtn->setSizePolicy(sizePolicy2);
        lBtn->setMinimumSize(QSize(30, 25));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/imageEditor/\345\267\246.png"), QSize(), QIcon::Normal, QIcon::Off);
        lBtn->setIcon(icon1);

        gridLayout->addWidget(lBtn, 1, 1, 1, 1);

        upBtn = new CustomToolButton(widget_2);
        upBtn->setObjectName("upBtn");
        sizePolicy2.setHeightForWidth(upBtn->sizePolicy().hasHeightForWidth());
        upBtn->setSizePolicy(sizePolicy2);
        upBtn->setMinimumSize(QSize(0, 25));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/imageEditor/\344\270\212.png"), QSize(), QIcon::Normal, QIcon::Off);
        upBtn->setIcon(icon2);

        gridLayout->addWidget(upBtn, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        downBtn = new CustomToolButton(widget_2);
        downBtn->setObjectName("downBtn");
        sizePolicy2.setHeightForWidth(downBtn->sizePolicy().hasHeightForWidth());
        downBtn->setSizePolicy(sizePolicy2);
        downBtn->setMinimumSize(QSize(0, 25));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/imageEditor/\344\270\213.png"), QSize(), QIcon::Normal, QIcon::Off);
        downBtn->setIcon(icon3);

        gridLayout->addWidget(downBtn, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 4, 1, 1);


        verticalLayout_4->addWidget(widget_2);

        widget = new QWidget(Preview);
        widget->setObjectName("widget");
        sizePolicy1.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy1);
        horizontalLayout_4 = new QHBoxLayout(widget);
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        view_scale = new InfiniteDial(widget);
        view_scale->setObjectName("view_scale");
        sizePolicy1.setHeightForWidth(view_scale->sizePolicy().hasHeightForWidth());
        view_scale->setSizePolicy(sizePolicy1);
        view_scale->setMinimumSize(QSize(120, 120));

        horizontalLayout_4->addWidget(view_scale);


        verticalLayout_4->addWidget(widget);


        verticalLayout->addWidget(Preview);

        Trans = new QWidget(Operation);
        Trans->setObjectName("Trans");
        sizePolicy1.setHeightForWidth(Trans->sizePolicy().hasHeightForWidth());
        Trans->setSizePolicy(sizePolicy1);
        verticalLayout_2 = new QVBoxLayout(Trans);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, -1, 0, -1);
        line = new QFrame(Trans);
        line->setObjectName("line");
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_2->addWidget(line);

        label_2 = new QLabel(Trans);
        label_2->setObjectName("label_2");
        sizePolicy2.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy2);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout_2->addWidget(label_2);

        widget_3 = new QWidget(Trans);
        widget_3->setObjectName("widget_3");
        gridLayout_3 = new QGridLayout(widget_3);
        gridLayout_3->setObjectName("gridLayout_3");
        gridLayout_3->setContentsMargins(0, -1, 0, -1);
        rotate = new QSlider(widget_3);
        rotate->setObjectName("rotate");
        rotate->setMinimum(-180);
        rotate->setMaximum(180);
        rotate->setSingleStep(0);
        rotate->setOrientation(Qt::Orientation::Horizontal);

        gridLayout_3->addWidget(rotate, 4, 1, 1, 1);

        rotateLabel = new QLabel(widget_3);
        rotateLabel->setObjectName("rotateLabel");

        gridLayout_3->addWidget(rotateLabel, 4, 0, 1, 1);


        verticalLayout_2->addWidget(widget_3);


        verticalLayout->addWidget(Trans);

        Step = new QWidget(Operation);
        Step->setObjectName("Step");
        sizePolicy1.setHeightForWidth(Step->sizePolicy().hasHeightForWidth());
        Step->setSizePolicy(sizePolicy1);
        horizontalLayout = new QHBoxLayout(Step);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(3, -1, 3, -1);
        unDoBtn = new QPushButton(Step);
        unDoBtn->setObjectName("unDoBtn");
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/imageEditor/undo_yes.png"), QSize(), QIcon::Normal, QIcon::Off);
        unDoBtn->setIcon(icon4);

        horizontalLayout->addWidget(unDoBtn);

        reDoBtn = new QPushButton(Step);
        reDoBtn->setObjectName("reDoBtn");
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/imageEditor/redo_yes.png"), QSize(), QIcon::Normal, QIcon::Off);
        reDoBtn->setIcon(icon5);

        horizontalLayout->addWidget(reDoBtn);


        verticalLayout->addWidget(Step);

        OKBtn = new QPushButton(Operation);
        OKBtn->setObjectName("OKBtn");

        verticalLayout->addWidget(OKBtn);


        gridLayout_2->addWidget(Operation, 0, 0, 1, 1);

        dockWidget->setWidget(dockWidgetContents);

        horizontalLayout_2->addWidget(dockWidget);


        retranslateUi(ImgEditor);

        QMetaObject::connectSlotsByName(ImgEditor);
    } // setupUi

    void retranslateUi(QWidget *ImgEditor)
    {
        ImgEditor->setWindowTitle(QCoreApplication::translate("ImgEditor", "ImgEditor", nullptr));
        label->setText(QCoreApplication::translate("ImgEditor", "\351\242\204\350\247\210\350\260\203\346\225\264", nullptr));
        rBtn->setText(QString());
        lBtn->setText(QString());
        upBtn->setText(QString());
        downBtn->setText(QString());
        label_2->setText(QCoreApplication::translate("ImgEditor", "\350\203\214\346\231\257\345\233\276\347\211\207\347\274\226\350\276\221", nullptr));
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

#endif // UI_IMGEDITOR_H

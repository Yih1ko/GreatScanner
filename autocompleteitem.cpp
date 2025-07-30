#include "autocompleteitem.h"
#include "ui_autocompleteitem.h"

AutoCompleteItem::AutoCompleteItem(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AutoCompleteItem)
{
    ui->setupUi(this);
    QObject::connect(ui->resultLabel, QOverload<>::of(&ClickableLabel::clicked), [this](){
        emit selected(ui->resultLabel->text());
    });
}

AutoCompleteItem::~AutoCompleteItem()
{
    delete ui;
}

void AutoCompleteItem::setText(QString text)
{
    ui->resultLabel->setText(text);
}

QString AutoCompleteItem::getText()
{
    return ui->resultLabel->text();
}

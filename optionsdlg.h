#ifndef OPTIONSDLG_H
#define OPTIONSDLG_H

#include <QWidget>

namespace Ui {
class OptionsDlg;
}

class OptionsDlg : public QWidget
{
    Q_OBJECT

public:
    explicit OptionsDlg(QWidget *parent = nullptr);
    ~OptionsDlg();

private:
    Ui::OptionsDlg *ui;
};

#endif // OPTIONSDLG_H

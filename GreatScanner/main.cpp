#include <QApplication>
#include "imgeditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ImgEditor w(nullptr, "D:\\Qt projects\\GreatScanner\\imageEditor\\test\\t1.png");
    w.show();
    return a.exec();
}


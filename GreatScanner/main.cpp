#include "imgeditor.h"

void loadConfig(){
    QSettings settings("D:\\Qt projects\\GreatScanner\\config.ini", QSettings::IniFormat);
    //配置内容：
    // For ImgEditor:
    imgEditor_input_path = settings.value("ImgEditor/input").toString();
    imgEditor_output_path = settings.value("ImgEditor/output").toString();
    qDebug() << "imgEditor_input_path:"<< imgEditor_input_path << "\nimgEditor_output_path:" << imgEditor_output_path;

    //For ...
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loadConfig();
    ImgEditor w(nullptr, imgEditor_input_path);
    w.show();
    return a.exec();
}


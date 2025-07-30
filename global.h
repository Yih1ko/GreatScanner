#ifndef GLOBAL_H
#define GLOBAL_H
#include <qpainter.h>
#include <qpainterpath.h>
#include <QString>
/*储存全局辅助函数或者全局变量***********************************************************************************************************************************************/

//全局变量
extern QString imgEditor_input_path;       //图片背景选择源
extern QString imgEditor_output_path;       //裁切后图片保存路径


//高性能阴影
extern void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

#endif // GLOBAL_H

#ifndef GLOBAL_H
#define GLOBAL_H

// Qt核心模块
#include <QObject>          // 基础对象模型
#include <QDebug>           // 调试输出
#include <QVector>          // Qt向量容器
#include <QPointF>          // 浮点坐标点
#include <QDateTime>        // 日期时间处理
#include <QImage>           // 图像处理
#include <QStack>           // Qt栈容器

// 图形视图框架
#include <QGraphicsView>    // 图形视图组件（已包含QGraphicsObject）
#include <QStyleOptionGraphicsItem> // 图形项样式选项
#include <QAbstractGraphicsShapeItem> // 抽象图形形状项
#include <QGraphicsSceneMouseEvent> // 场景鼠标事件

// GUI组件
#include <QToolButton>      // 工具按钮
#include <QSlider>          // 滑块组件
#include <QDial>            // 旋钮控件
#include <QPainter>         // 绘图工具

// 事件系统
#include <QMouseEvent>      // 鼠标事件处理

// 工具模块
#include <QTimer>           // 定时器
#include <QScrollBar>       // 滚动条
#include <QApplication>     // 应用对象

// 持久化存储
#include <QSettings>        // 应用配置存储
#include <QDir>             // 目录操作

// 标准库
#include <vector>           // STL向量容器
#include <string>           // 字符串处理
#include <iostream>         // 标准输入输出流
#include <map>         // 红黑树map
#include <unordered_map>         // 哈希表map
#include <unordered_set>         // 哈希集合
#include <stdexcept>         // 异常处理

// OpenCV
#include <opencv2/opencv.hpp>  // 包含核心模块（core/imgproc/highgui等）

// 撤销框架
#include <QUndoStack>       // 撤销栈
#include <QUndoCommand>     // 撤销命令基类

/******************************************************************************************************************************************************/

//全局变量
extern QString imgEditor_input_path;       //图片背景选择源
extern QString imgEditor_output_path;       //裁切后图片保存路径

#endif // GLOBAL_H

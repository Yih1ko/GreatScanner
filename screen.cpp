#include "screen.h"
#include <QPainter>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPoint>
#include <QDebug>
#include <QScreen>
#include <QGuiApplication>
#include <QFileDialog>
#include <QRectF>
#include "controlwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTime>
#include"myrect.h"

Screen::Screen(QWidget *parent)
    : QWidget(parent, Qt::Window | Qt::FramelessWindowHint /*| Qt::WindowStaysOnTopHint*/),
      widthInfoRect(QRectF(0, 0, 0, 0)),
      control(NULL)
{
    setAttribute(Qt::WA_DeleteOnClose); // 自动释放资源
    showFullScreen();//将窗口部件全屏显示
    //截取全屏
    QScreen *scrPix = QGuiApplication::primaryScreen();
    // 获取设备像素比
    m_ratio = this->devicePixelRatioF();
    qDebug() << "Device Pixel Ratio:" << this->devicePixelRatioF();
    qDebug() << "Screen DPI:" << QGuiApplication::primaryScreen()->logicalDotsPerInch();
    pixmap = scrPix->grabWindow(0);

    //全局路径的初始化，一个全屏闭合回路
    globalPath.lineTo(pixmap.width(), 0);
    globalPath.lineTo(pixmap.width(), pixmap.height());
    globalPath.lineTo(0, pixmap.height());
    globalPath.lineTo(0, 0);

    oncePress = true;
    //想到了添加一个bool labelimageShow()的函数的方法，找时间试一下
    labelimage=new QSLabel(this);
    Qt::WindowFlags nType = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint;
    labelimage->setWindowFlags(nType);

    setMouseTracking(true);
    type = Type::NO;
    qDebug() << "window rect:" << this->geometry();
    qDebug() << "screen size:" << QGuiApplication::primaryScreen()->size();
}

void Screen::paintEvent(QPaintEvent *e)//将上一个函数得到的全屏灰色图绘制在painter上，并添加宽高信息矩形和边角拖曳小正方形
{


    QPainter paint(this);
    // paint.drawPixmap(0, 0, pixmap);
    // 绘制适配DPI的pixmap
    QPixmap scaledPix = pixmap;
    scaledPix.setDevicePixelRatio(m_ratio);
    paint.drawPixmap(0,  0, scaledPix);
    //初始化画笔操作
    paint.setPen("#2e86ff"/*Qt::blue*/);
    paint.setBrush(QColor(0, 0, 0, 100));
    //设置路径
    QPainterPath path = getPath();

    paint.drawPath(path);
    //绘制选取左上角显示宽高的区域
    widthInfoRect.drawMe(paint);
    //绘制四个小正方形
    drawControlArea(paint);
}
//绘制正方形
void Screen::drawControlArea(QPainter &painter)
{
    //计算四个小正方形
    // rect1.setX(movePoint.x() - 3);
    // rect1.setY(pressedPoint.y() - 3);
    // rect1.setWidth(6);
    // rect1.setHeight(6);
    // rect2.setX(pressedPoint.x() - 3);
    // rect2.setY(pressedPoint.y() - 3);
    // rect2.setWidth(6);
    // rect2.setHeight(6);
    // rect3.setX(pressedPoint.x() - 3);
    // rect3.setY(movePoint.y() - 3);
    // rect3.setWidth(6);
    // rect3.setHeight(6);
    // rect4.setX(movePoint.x() - 3);
    // rect4.setY(movePoint.y() - 3);
    // rect4.setWidth(6);
    // rect4.setHeight(6);
    // painter.save();
    // painter.setBrush(Qt::blue);
    // painter.drawRect(rect1);
    // painter.drawRect(rect2);
    // painter.drawRect(rect3);
    // painter.drawRect(rect4);
    // painter.restore();
    QRect area = normalizedRect();  // 使用归一化坐标

    rect1 = QRectF(area.topRight()  - QPoint(3, 3), QSize(6, 6));
    rect2 = QRectF(area.topLeft()  - QPoint(3, 3), QSize(6, 6));
    rect3 = QRectF(area.bottomLeft()  - QPoint(3, 3), QSize(6, 6));
    rect4 = QRectF(area.bottomRight()  - QPoint(3, 3), QSize(6, 6));

    painter.setBrush(QColor("#2e86ff")/*Qt::blue*/);
    painter.drawRects({rect1,  rect2, rect3, rect4});
}



//得到选区之外的路径
QPainterPath Screen::getPath()   //这个函数得到的是开始截图时候得到的白色选区
{
    //选取路径
    QPainterPath path;
    path.moveTo(pressedPoint.x(), pressedPoint.y());
    path.lineTo(movePoint.x(), pressedPoint.y());
    path.lineTo(movePoint.x(), movePoint.y());
    path.lineTo(pressedPoint.x(), movePoint.y());
    path.lineTo(pressedPoint.x(), pressedPoint.y());
    return globalPath.subtracted(path);    //从灰色全屏路径中扣掉这个白色路径区域，然后作为截取的图片？
}


void Screen::mousePressEvent(QMouseEvent *e)
{
    if( e->button() & Qt::LeftButton && oncePress)   //如果鼠标左键第一次按下
    {
       pressedPoint = e->pos();
    } else
    {
        oldPoint = e->pos();
    }
}

void Screen::mouseReleaseEvent(QMouseEvent *e)    //只有已经按下鼠标按键，才会存在鼠标释放行为，释放鼠标后会立刻出现控制面板
{
    oncePress = false;
    if( !control )        //如果未出现截图操作控件
    {
        qDebug() << "new ControlWidget";
        control = new QWidget(this);         //新建一个窗口控件(目前未实现自适应)
        controlUi = new ControlWidget(control);  //新建控制窗口
        QHBoxLayout *layout = new QHBoxLayout(control);    //在control上建立水平布局
        layout->addWidget(controlUi);         //将控制窗口应用于水平布局
        layout->setContentsMargins(0, 0, 0, 0);
        control->setObjectName("control");
        control->setStyleSheet("QWidget#control{\
                               background-color: #eaecf0;}");
        controlUi->setScreenQuote(this);
    }
    //设置控制面板的位置
    control->setGeometry(movePoint.x() - 543, movePoint.y() + 6, 543, 25);
    control->show();
}

void Screen::mouseMoveEvent(QMouseEvent *e)
{
    // 转换逻辑坐标->物理坐标
    QPoint tempPoint = e->pos();     //当前鼠标位置
    setselectimagelabel(e);
    repaint();

    if(e->buttons() & Qt::LeftButton)   //如果左键按下
    {
        //选取左上角的信息更改
        int x = qAbs(movePoint.x() - pressedPoint.x());//movepoint即终点坐标
        int y = qAbs(movePoint.y() - pressedPoint.y());
        widthInfoRect.setText(tr("%1 * %2").arg(x).arg(y));   //将截图选区的长宽信息显示在widthinforect中
        if( comparePoint(pressedPoint, movePoint)) {
            widthInfoRect.setLocation(pressedPoint.x(), pressedPoint.y());
            //计算选区矩形
            rect.setX(pressedPoint.x());
            rect.setY(pressedPoint.y());
            rect.setWidth(movePoint.x() - pressedPoint.x());
            rect.setHeight(movePoint.y() - pressedPoint.y());
        } else {
            widthInfoRect.setLocation(movePoint.x(), movePoint.y());
            rect.setX(movePoint.x());
            rect.setY(movePoint.y());
            rect.setWidth(pressedPoint.x() - movePoint.x());
            rect.setHeight(pressedPoint.y() - movePoint.y());
        }
        if( oncePress )    //如果是第一次按下时候，
        {
            movePoint = e->pos();   //终点即为鼠标此时位置
        //此时的移动代表选区的变化
        } else
        {

            if( control )  //如果控制面板已经出现，则第二次以后的每一次鼠标按键都会使其暂时隐藏
                control->hide();
            int moveX = tempPoint.x() - oldPoint.x();  //鼠标移动的x距离
            int moveY = tempPoint.y() - oldPoint.y();   //鼠标移动的y距离
            switch( type )         //选区角落四个小矩形的位置
            {
            case Type::RECT1:               //意思是第一次选区之后，分别拖动四个角落的小矩形时候，截图选区的变化
                pressedPoint.setY(pressedPoint.y() + moveY);  //右上角的矩形
                movePoint.setX(movePoint.x() + moveX);
                break;
            case Type::RECT2:
                pressedPoint.setX(pressedPoint.x() + moveX);    //左上角的矩形
                pressedPoint.setY(pressedPoint.y() + moveY);
                break;
            case Type::RECT3:
                pressedPoint.setX(pressedPoint.x() + moveX);  //左下角的矩形
                movePoint.setY(movePoint.y() + moveY);
                break;
            case Type::RECT4:
                movePoint.setX(movePoint.x() + moveX);     //右下角的矩形
                movePoint.setY(movePoint.y() + moveY);
                break;
            case Type::RECT:                  //指的是当鼠标在截图选区中按下左键然后拖动时候，截图选区的整体位置变化
            {

                int tempPressX = pressedPoint.x() + moveX;
                int tempPressY = pressedPoint.y() + moveY;
                int tempMoveX = movePoint.x() + moveX;
                int tempMoveY = movePoint.y() + moveY;
                int deskWidth = pixmap.width();
                int deskHeight = pixmap.height();
                if( tempPressX < 0 )
                {
                    tempPressX = 0;
                    tempMoveX = movePoint.x();
                }
                if( tempPressX > deskWidth)
                {
                    tempPressX = deskHeight;
                }
                if(tempPressY < 0)
                {
                    tempPressY = 0;
                    tempMoveY = movePoint.y();
                }
                if( tempPressY > deskHeight)
                {

                    tempPressY = deskHeight;
                }
                if( tempMoveX < 0)
                {
                    tempMoveX = 0;
                }
                if( tempMoveX > deskWidth)
                {
                    tempMoveX = deskWidth;
                    tempPressX = pressedPoint.x();
                }
                if( tempMoveY < 0)
                {
                    tempMoveY = 0;
                }
                if( tempMoveY > deskHeight)
                {
                    tempMoveY = deskHeight;
                    tempPressY = pressedPoint.y();

                }
                pressedPoint.setX(tempPressX);    //鼠标在截图区域中拖动结束后，选区的位置
                pressedPoint.setY(tempPressY);
                movePoint.setX(tempMoveX);
                movePoint.setY(tempMoveY);
                break;
            }
            case Type::NO:
                break;
            default:
                break;
            }
            oldPoint = tempPoint;    //更新位置坐标信息
        }

    } else                          //指的是当鼠标位于四个角落小矩形中和选区中时候鼠标的图标变化
    {
        Type r = pointInWhere(e->pos());
        if( r == Type::RECT1)
        {
            type = Type::RECT1;
            setCursor(Qt::SizeBDiagCursor);
        } else if( r == Type::RECT2)
        {
            type = Type::RECT2;
            setCursor(Qt::SizeFDiagCursor);
        } else if( r == Type::RECT3)
        {
            type = Type::RECT3;
            setCursor(Qt::SizeBDiagCursor);
        } else if( r == Type::RECT4)
        {
            type = Type::RECT4;
            setCursor(Qt::SizeFDiagCursor);
        } else if( r == Type::RECT)
        {
            type = Type::RECT;
            setCursor(Qt::SizeAllCursor);
        } else
        {
            type = Type::NO;
            setCursor(Qt::ArrowCursor);
        }
    }
    // repaint();
}

bool Screen::compareRect(QRectF &r1, QRectF &r2)
{
    return false;
}

//计算此时鼠标移动在哪个选区上面
Type Screen::pointInWhere(QPoint p)
{
    if( pointInRect(p, rect1))
        return Type::RECT1;
    else if( pointInRect(p, rect2))
        return Type::RECT2;
    else if( pointInRect(p, rect3))
        return Type::RECT3;
    else if( pointInRect(p, rect4))
        return Type::RECT4;
    else if( pointInRect(p, rect))
        return Type::RECT;
    else
        return Type::NO;

}

//判断点是否在矩形之内
bool Screen::pointInRect(QPoint &p, QRectF &r)
{
    if( p.x() > r.x() && p.x() < r.x() + r.width() &&
            p.y() > r.y() && p.y() < r.y() + r.height())
        return true;
    return false;
}

QRect Screen::normalizedRect() const
{

    int x1 = qMin(pressedPoint.x(), movePoint.x());
    int y1 = qMin(pressedPoint.y(), movePoint.y());
    int x2 = qMax(pressedPoint.x(), movePoint.x());
    int y2 = qMax(pressedPoint.y(), movePoint.y());
    return QRect(x1, y1, x2 - x1, y2 - y1);
}

void Screen::keyPressEvent(QKeyEvent *e)    //键盘事件，包括esc退出截图，回车键完成截图，返回键完成截图
{
    int key = e->key();
    switch( key )
    {
    case Qt::Key_Escape :
        close();
        break;
    case Qt::Key_Enter:
        if( controlUi )
        {
            controlUi->finishBtn_slot();
        }
        break;
    case Qt::Key_Return :
        if( controlUi )
        {
            controlUi->finishBtn_slot();
        }
        break;
    default:
        break;
    }
}

//如果p1的x和y坐标都小于p2的x和y坐标返回true
bool Screen::comparePoint(QPoint &p1, QPoint &p2)      //点p1的位置坐标是否小于p2点
{
    if(p1.x() < p2.x() && p1.y() < p2.y())
        return true;
    return false;
}
void Screen::setselectimagelabel(QMouseEvent* event)
{
    QRect area = normalizedRect();
    // 使用物理坐标获取图像
    QRect physicalArea = logicalToPhysicalRect(area);
    QPixmap tempPix = pixmap.copy(physicalArea);

    // 设置正确的像素比
    tempPix.setDevicePixelRatio(m_ratio);
    QImage selectimage = tempPix.toImage();

    // 直接使用逻辑坐标定位（无需屏幕位置计算）
    labelimage->setimagetolabel(selectimage);
    labelimage->setFixedSize(area.size());
    labelimage->move(area.topLeft());  // 使用归一化的逻辑坐标
    labelimage->show();
}

//保存截取出来的图片
void Screen::savePixmap()
{
    qDebug()<<"aaaaaaaaa";
    //生成图片名称
    QString picName;
    QTime time;
    //获取当前系统时间，用做伪随机数的种子
    time = QTime::currentTime();
    srand(time.msec() + time.second() * 1000);
    //随机字符串
    QString randStr;
    randStr.setNum(rand());
    picName.append(randStr);

    QString filename=QFileDialog::getSaveFileName(this,QStringLiteral("保存截图"),picName,"JPEG Files(*.jpg)");

    if(filename.length()>0){
        QImage pimage=labelimage->resultimage();
        pimage.save(filename, "jpg");
    }
}

QImage Screen::getGrabImage()   //返回截到的图片
{
    return labelimage->resultimage();
}

void Screen::drawline()
{

    labelimage->setdrawlineenable();

}

void Screen::textedit()
{
    labelimage->settexteditenable();
}

void Screen::drawarrow()
{
    labelimage->setdrawarrowenable();
}

void Screen::returnEdit() {
    labelimage->setReturnEditEnable();
}

void Screen::drawround()
{
    labelimage->setroundenable();
}

void Screen::drawrectang()
{
    labelimage->setrectangleenable();
}

void Screen::Exit()
{
    if(labelimage)
    {
        labelimage->close();
    }
}

Screen::~Screen()
{
    qDebug()<<"m_screen deleted";
}

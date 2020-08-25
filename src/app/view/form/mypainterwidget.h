#ifndef MYPAINTERWIDGET_H
#define MYPAINTERWIDGET_H

#include <QWidget>
#include<QPoint>
#include"frmtcpclient.h"
namespace Ui {
class MyPainterWidget;
}
typedef struct{
    int number;//序号，标记
    QPoint point[1000];
    int validnum;//有效点数
    int nowframenum;//当前帧号
} MyPoints;
class MyPainterWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyPainterWidget(QWidget *parent = 0);
    ~MyPainterWidget();
    void paintEvent(QPaintEvent *);
    void draw_point(int const x, int const y, QColor const c, Frame_Content& frame);
    void clearPath();

    void drawRulerTop(QPainter *painter);
    void drawLeftScale(QPainter *painter);
    void drawLeftText(QPainter *painter);
public slots:
    void slot_timerOut_mTimer();
    void slot_FrameContent(Frame_Content&);
private:
    Ui::MyPainterWidget *ui;
    // 添加成员变量
    QPixmap Pix;
    QTimer *m_timer;
    MyPoints mypath[100];


    double minValue;                //最小值
    double maxValue;                //最大值
    double value;                   //目标值
    int precision;                  //精确度,小数点后几位

    int longStep;                   //长线条等分步长
    int shortStep;                  //短线条等分步长
    int space;                      //间距

    bool animation;                 //是否启用动画显示
    double animationStep;           //动画显示时步长

    QColor bgColorStart;            //背景渐变开始颜色
    QColor bgColorEnd;              //背景渐变结束颜色
    QColor lineColor;               //线条颜色

    QColor pointerColor;            //指示器颜色
    int pointerWidth;               //指示器宽度
    int scaleStyle;

};

#endif // MYPAINTERWIDGET_H

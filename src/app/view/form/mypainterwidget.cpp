#include "mypainterwidget.h"
#include "ui_mypainterwidget.h"
#include <QPainter>
#include <QTime>
#include <QTimer>
#include "quiwidget.h"
MyPainterWidget::MyPainterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyPainterWidget)
{
    ui->setupUi(this);
    setFixedSize(500,700); //固定大小
    setWindowTitle("Hello DevDiv!");
    // 初始化QPixmap
    Pix = QPixmap(this->width(), this->height());

    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
    int n = qrand() % 5;    //产生5以内的随机数
    m_timer= new QTimer(this);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slot_timerOut_mTimer()));
    m_timer->start(50);
    //初始化数据结构


    minValue = 0-App::XWidth/2;
    maxValue = App::XWidth/2;
    value = 0;

    precision = 0;
    longStep = 10;
    shortStep = 1;
    space = 0;

    animation = false;
    animationStep = 0.5;

    bgColorStart = QColor(100, 100, 100);
    bgColorEnd = QColor(60, 60, 60);
    lineColor = Qt::gray;

    pointerColor = QColor(100, 184, 255, 180);
    pointerWidth = 3;
    scaleStyle=0;

}

MyPainterWidget::~MyPainterWidget()
{
    delete ui;
}
void MyPainterWidget::slot_timerOut_mTimer(){
//    draw_point(0,0,QColor(10,20,20), 10);

}
void MyPainterWidget::slot_FrameContent(Frame_Content& frame){
    qDebug()<<"slot_FrameContent";
    draw_point(0,0,QColor(10,20,20), frame);
}
void MyPainterWidget::draw_point(int const x, int const y, QColor const c, Frame_Content& frame){
    Pix.fill(Qt::black);
    // 画一条直线
    QPainter painter(&Pix);             // 创建QPainter一个对象
    QPen pen;
    //pen.setStyle(Qt::DashLine);
    painter.save();
    painter.setPen(Qt::NoPen);
    QLinearGradient bgGradient(QPointF(0, 0), QPointF(0, height()));
    bgGradient.setColorAt(0.0, bgColorStart);
    bgGradient.setColorAt(1.0, bgColorEnd);
    painter.setBrush(bgGradient);
    painter.drawRect(rect());
    painter.restore();




    painter.save(); //保存坐标系状态
    painter.translate(0,App::ZeroPointY*this->height()); //将点（100，100）设为原点
    drawLeftScale(&painter);
    drawRulerTop(&painter);
    painter.restore();

    painter.save(); //保存坐标系状态
    painter.translate(this->width()*App::ZeroPointX,App::ZeroPointY*this->height()); //将点（100，100）设为原点

    pen.setWidth(2);
    for(int j=0;j<frame.num_obj;j++){
        int id=0;
        memcpy(&id,frame.objectData[j].id,sizeof(int));
//        qDebug()<<QString("clusterId:%1 clusterNum:%2 distLat:%3 distLong:%4 dynProp:%5 measState:%6 objClass:%7  id[0]:%8 id[1]:%9 id[2]:%10 id[3]:%11 id:%12")
//                  .arg(frame.objectData[j].clusterId,4,10,QChar('0'))
//                  .arg(frame.objectData[j].clusterNum,4,10,QChar('0'))
//                  .arg(frame.objectData[j].distLat)
//                  .arg(frame.objectData[j].distLong)
//                  .arg(frame.objectData[j].dynProp,2,16,QChar('0'))
//                  .arg(frame.objectData[j].measState,2,16,QChar('0'))
//                  .arg(frame.objectData[j].objClass,2,16,QChar('0'))
//                  .arg(frame.objectData[j].id[0],2,16,QChar('0'))
//                  .arg(frame.objectData[j].id[1],2,16,QChar('0'))
//                  .arg(frame.objectData[j].id[2],2,16,QChar('0'))
//                  .arg(frame.objectData[j].id[3],2,16,QChar('0'))
//                  .arg(id,8,16,QChar('0'));
        //坐标转换
        float x=frame.objectData[j].distLat;
        float y=frame.objectData[j].distLong;
        float xReal=(this->width()*x)/ App::XWidth;
        float yReal=-(this->height()*y)/App::YHeight;
        //float xReal=(this->width()/2)+a;
        //float yReal=this->height()-b;
        //
        //判断轨迹是否合法
        if(mypath[id].validnum>=2){
            QPointF a=mypath[id].point[mypath[id].validnum-1];
            QPointF b=mypath[id].point[mypath[id].validnum-2];
            //条件1 方向不一致
            if(((a.y()-b.y()>0)&&(yReal-a.y()<0))||((a.y()-b.y()<0)&&(yReal-a.y()>0))){
                //清空
                mypath[id].validnum=0;
            }
            //条件2 x y变动较大
            if(qAbs(yReal-a.y())>30||qAbs(xReal-a.x())>30){
                mypath[id].validnum=0;
            }
        }



        //赋值到轨迹
        mypath[id].number=id;
        mypath[id].point[mypath[id].validnum]=QPoint(xReal,yReal);
        mypath[id].validnum+=1;
        mypath[id].nowframenum=frame.frame;
        if(id%2==0){
            pen.setColor(Qt::red);           // 设置画笔为黄色
        }else{
            pen.setColor(Qt::green);           // 设置画笔为黄色
        }
        painter.setPen(pen);

        QPainterPath path;
        int startx=0;
        int starty=0;
        for(int i=0;i<mypath[id].validnum;i++){
            startx=mypath[id].point[i].x();
            starty=mypath[id].point[i].y();
            if(i==0){
                path.moveTo(startx,starty);
            }else{
                path.lineTo(startx,starty);
            }
        }
        painter.drawText(QRect(startx,starty,200,20),QString("%1-%2-%3").arg(id).arg(frame.objectData[j].dynProp).arg(frame.objectData[j].objClass));

        painter.drawPath(path);

    }
    painter.restore(); //恢复以前的坐标系状态
    update();
}
void MyPainterWidget::paintEvent(QPaintEvent *)
{
    QPainter Painter(this);



    Painter.drawPixmap(0, 0, this->width(), this->height(),Pix);
}

void MyPainterWidget::drawLeftScale(QPainter *painter)
{
    double minValue = 0;
    double maxValue = App::YHeight;
    int longStep = 50;
    int shortStep = 5;

    QPen pen;
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    pen.setColor(lineColor);
    painter->save();
    painter->setPen(pen);

    //绘制纵向标尺线
    double initLeftX = space;
    double initY = space;

    //左侧纵向标尺线
    painter->drawLine(QPointF(initLeftX, initY), QPointF(initLeftX, height() - space));

    //绘制纵向标尺刻度
    double length = height() - 2 * space;
    //计算每一格移动多少
    double increment = length / (maxValue - minValue);
    //长线条短线条长度
    int longLineLen = 1000;
    int shortLineLen = 7;

    //根据范围值绘制刻度值及刻度值
    for (int i = minValue; i <= maxValue; i = i + shortStep) {
        QString strValue = QString("%1").arg(i);
        if (i % longStep == 0) {
            painter->drawLine(QPointF(initLeftX + longLineLen, initY), QPointF(initLeftX, initY));
            QPointF textLeftPot(initLeftX + 10 + shortLineLen, initY+ 4);
            painter->drawText(textLeftPot, strValue);
        } else {
            if (i % (longStep / 2) == 0) {
                shortLineLen = 7;
            } else {
                shortLineLen = 4;
            }

            painter->drawLine(QPointF(initLeftX + shortLineLen, initY), QPointF(initLeftX, initY));
        }

        initY -= increment * shortStep;
    }

    painter->restore();
}

void MyPainterWidget::drawRulerTop(QPainter *painter)
{


    painter->save();
    painter->setPen(lineColor);

    double initX = space;

    //绘制横向标尺上部分底部线
    double initTopY = space;
    QPointF lineTopLeftPot = QPointF(initX, initTopY);
    QPointF lineTopRightPot = QPointF(width() - initX, initTopY);
    painter->drawLine(lineTopLeftPot, lineTopRightPot);

    //绘制上部分及下部分横向标尺刻度
    double length = width() - 2 * space;
    //计算每一格移动多少
    double increment = length / (maxValue - minValue);
    //长线条短线条长度
    int longLineLen = 15;
    int shortLineLen = 10;

    //根据范围值绘制刻度值及刻度值 长线条需要移动10像素 短线条需要移动5像素
    for (int i = minValue; i <= maxValue; i = i + shortStep) {
        if (i % longStep == 0) {
            QPointF topPot = QPointF(initX, initTopY-3000);
            QPointF bottomPot = QPointF(initX, initTopY + longLineLen);
            painter->drawLine(topPot, bottomPot);

            //如果间距小于5则第一个值和最后一个值不要绘制
            if (space <= 5 && (i == minValue || i == maxValue)) {
                initX += increment * shortStep;
                continue;
            }

            QString strValue = QString("%1").arg((double)i, 0, 'f', precision);
            double textWidth = fontMetrics().width(strValue);
            double textHeight = fontMetrics().height();

            QPointF textPot = QPointF(initX - textWidth / 2, initTopY + textHeight + longLineLen);
            painter->drawText(textPot, strValue);
        } else {
            if (i % (longStep / 2) == 0) {
                shortLineLen = 10;
            } else {
                shortLineLen = 6;
            }

            QPointF topPot = QPointF(initX, initTopY);
            QPointF bottomPot = QPointF(initX, initTopY + shortLineLen);
            painter->drawLine(topPot, bottomPot);
        }

        initX += increment * shortStep;
    }

    painter->restore();
}

void MyPainterWidget::clearPath(){
     memset(mypath,0,100*sizeof (MyPoints));
}

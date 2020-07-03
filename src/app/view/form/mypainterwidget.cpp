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
    pen.setColor(Qt::red);           // 设置画笔为黄色
    pen.setWidth(3);
    painter.setPen(pen);
    painter.save(); //保存坐标系状态
    painter.translate(this->width()*App::ZeroPointX,App::ZeroPointY*this->height()); //将点（100，100）设为原点
    //
    //painter.setBrush(Qt::gray);

    painter.drawRect(-10,-10,20,20);
    painter.drawRect(-5,-5,10,10);

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

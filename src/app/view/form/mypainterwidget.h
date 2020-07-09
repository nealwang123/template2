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
public slots:
    void slot_timerOut_mTimer();
    void slot_FrameContent(Frame_Content&);
private:
    Ui::MyPainterWidget *ui;
    // 添加成员变量
    QPixmap Pix;
    QTimer *m_timer;
    MyPoints mypath[100];

};

#endif // MYPAINTERWIDGET_H

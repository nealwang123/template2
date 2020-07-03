#include "usbdevice.h"

USBDevice::USBDevice()
{
    m_exitStateThread1=0;
    m_exitStateThread2=0;
}
USBDevice::~USBDevice(){
    m_exitStateThread1=0;
    m_exitStateThread2=0;
}
int USBDevice::setDeviceAttrubute(QStringList&list){
    this->m_device_attribute=list;
    qDebug()<<"USBDevice::setDeviceAttrubute list.length():"<<list.length()<<list;
    if(list.length()<=7){
        return -1;
    }
    m_index=list.at(0).toInt();
    m_ConnectType=list.at(2);
    m_ConnectPara=list.at(3);
    m_AStepAngle1=list.at(4);
    m_ADiv1=list.at(5);
    m_ACircleRatio1=list.at(6);
    m_Acc1=list.at(7);
    m_Dec1=list.at(8);
    m_SpeedMax1=list.at(9);
    m_AStepAngle2=list.at(10);
    m_ADiv2=list.at(11);
    m_ACircleRatio2=list.at(12);
    m_Acc2=list.at(13);
    m_Dec2=list.at(14);
    m_SpeedMax2=list.at(15);


    return 0;
}
/*******************************************************
* 打开设备，步骤5个，异常返回-
*/
int USBDevice::open(){
    qDebug()<<"usb open function!";
    qDebug()<<"usb open sucessfully!!!";

    int state=0;
    state=MT_Init();
    if(state!=0){
        return -10;
    }
    if(m_ConnectType=="usb"){
        state=MT_Open_USB();
    }else if(m_ConnectType=="com"){
        state=MT_Open_UART(m_ConnectPara.toLatin1().data());
    }
    if(state!=0){
        return -20;
    }

    int acc,dec,max;
    acc=MT_Help_Step_Circle_Real_To_Steps(m_AStepAngle1.toDouble(),m_ADiv1.toInt(),m_ACircleRatio1.toDouble(),m_Acc1.toDouble());
    dec=MT_Help_Step_Circle_Real_To_Steps(m_AStepAngle1.toDouble(),m_ADiv1.toInt(),m_ACircleRatio1.toDouble(),m_Acc1.toDouble());
    max=MT_Help_Step_Circle_Real_To_Steps(m_AStepAngle1.toDouble(),m_ADiv1.toInt(),m_ACircleRatio1.toDouble(),m_Acc1.toDouble());

    state=MT_Set_Axis_Acc(0,acc);
    if(state!=0){
        return -41;
    }
    MT_Set_Axis_Dec(0,dec);
    if(state!=0){
        return -42;
    }
    MT_Set_Axis_Position_V_Max(0,max);
    if(state!=0){
        return -43;
    }
    //参数初始化
    state=MT_Set_Axis_Mode_Position(0);
    if(state!=0){
        return -30;
    }

    acc=MT_Help_Step_Circle_Real_To_Steps(m_AStepAngle2.toDouble(),m_ADiv2.toInt(),m_ACircleRatio2.toDouble(),m_Acc2.toDouble());
    dec=MT_Help_Step_Circle_Real_To_Steps(m_AStepAngle2.toDouble(),m_ADiv2.toInt(),m_ACircleRatio2.toDouble(),m_Acc2.toDouble());
    max=MT_Help_Step_Circle_Real_To_Steps(m_AStepAngle2.toDouble(),m_ADiv2.toInt(),m_ACircleRatio2.toDouble(),m_Acc2.toDouble());
    state=MT_Set_Axis_Acc(1,acc);
    if(state!=0){
        return -51;
    }
    MT_Set_Axis_Dec(1,dec);
    if(state!=0){
        return -52;
    }
    MT_Set_Axis_Position_V_Max(1,max);
    if(state!=0){
        return -53;
    }
    state=MT_Set_Axis_Mode_Position(1);

    //启动另一个状态线程
    star();
    return 0;
}
void USBDevice::star()//启动函数是需要在原有代码基础上增加
{
    m_exitStateThread2=1;
    QtConcurrent::run(this,&USBDevice::stateThread);//多线程执行死循环启动，可以带参数，具体格式可以查阅网上其它资料
}

void USBDevice::stateThread(){
    int i=0;
//    qDebug()<<" USBDevice::sliderStateThread()"<<i<<QThread::currentThreadId() << QThread::currentThread()<<m_exitState;
    zmq::context_t context2 (1);
    zmq::socket_t publisher (context2, ZMQ_PUB);

    publisher.bind("tcp://*:6666");
    char* zipcode;
    double xPos, yPos;
    int count=0;
    //  Get values that will fool the boss
    zipcode     = (char *)"usbState";

    while(m_exitStateThread2){
        qDebug()<<"usbState thread:"<< QThread::currentThread()<<"m_exitStateThread2"<<m_exitStateThread2;
        //获取转台状态
        int iAbs_0,iAbs_1;
        int state=MT_Get_Axis_Software_P_Now(0,&iAbs_0);
        state=MT_Get_Axis_Software_P_Now(0,&iAbs_1);

        xPos = MT_Help_Step_Circle_Steps_To_Real(m_AStepAngle1.toDouble(),m_ADiv1.toInt(),m_ACircleRatio1.toDouble(),iAbs_0);
        yPos = MT_Help_Step_Circle_Steps_To_Real(m_AStepAngle2.toDouble(),m_ADiv2.toInt(),m_ACircleRatio2.toDouble(),iAbs_1);

        zmq::message_t message(40);//此处需设置大于所有数据
        snprintf ((char *) message.data(), 40 ,"%s %-10.2f %10.2f %6d", zipcode, xPos, yPos,count);
        //发布状态
        //  Send message to all subscribers
        publisher.send(message);
        cout<<"pub:"<<count<<endl;
        s_console((char *) message.data());
        count++;
        s_sleep(1000);
    }
}
int USBDevice::close(){
    if(m_ConnectType=="usb"){
        MT_Close_USB();
    }else if(m_ConnectType=="com"){
        MT_Close_UART();
    }
    MT_DeInit();
    return 0;
}


//订阅控制命令，执行控制命令
void USBDevice::processHandle(){//处理线程

//    return;
    zmq::context_t context (1);
    //  Socket to talk to server
    std::cout << "Collecting control command  client...\n" << std::endl;
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.bind("tcp://*:5555");

    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = "usbControl";//s
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));

    //  Process 100 updates
    while(m_exitStateThread1){
        std::cout <<" USBDevice::processHandle() once control command!"<<m_exitStateThread1<<" "<<m_exitStateThread2<<endl;
        zmq::message_t update;
        string zipcode;
        double xPos, yPos;
        int stop;
        bool ret=subscriber.recv(&update);//阻塞了
        std::istringstream iss(static_cast<char*>(update.data()));
        iss >>zipcode >> xPos >> yPos>>stop;
        QString str;
        str="filter: '"+QString::fromStdString(zipcode)+"' xPos:"+QString::number(xPos) +"yPos:"+QString::number(yPos)+" stopStr "+QString::number(stop);
        qDebug() <<str;
        if(stop==1){
            MT_Set_Axis_Position_Stop(0);
            s_sleep(5);
            MT_Set_Axis_Position_Stop(1);
            s_sleep(5);
        }else{
            //执行转台运动
            gotoAbs(0,m_AStepAngle1.toDouble(),m_ADiv1.toInt(),m_ACircleRatio1.toDouble(),xPos);
            s_sleep(5);
            gotoAbs(1,m_AStepAngle2.toDouble(),m_ADiv2.toInt(),m_ACircleRatio2.toDouble(),yPos);
            s_sleep(50);
        }
    }
}
//
int USBDevice::gotoAbs(int obj,double AStepAngle,int ADiv,double AcircleRatio,double AValue){
    qDebug()<<"USBDevice::gotoAbs"<<"AStepAngle"<<AStepAngle<<"ADiv"<<ADiv<<"AcircleRatio"<<AcircleRatio<<"AValue"<<AValue;
    int value=MT_Help_Step_Circle_Real_To_Steps(AStepAngle,ADiv,AcircleRatio,AValue);
    int state=MT_Set_Axis_Position_P_Target_Abs(obj,value);
    if(state!=0){
        return -1;
    }
    return 0;
}

int USBDevice::initZmq(){
    return 0;
}

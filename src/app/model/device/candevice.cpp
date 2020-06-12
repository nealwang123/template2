#include "candevice.h"

CanDevice::CanDevice()
{
    qDebug()<<"构造函数CanDevice::CanDevice()";
}
/*
*解析配置参数，赋值到对应变量中，此处和sqlite数据库对应字段顺序紧密相关；
*/
int CanDevice::setDeviceAttrubute(QStringList&list){
    this->m_device_attribute=list;
    qDebug()<<"setDeviceAttrubute list.length():"<<list.length();
    if(list.length()<=7){
        qDebug()<<"list.length() error!"<<list.length();
        return -1;
    }
    this->m_tableIndex=list.at(0).toInt();//0

    this->m_canDeviceType=list.at(2).toInt();//2
    this->m_canDeviceIndex=list.at(3).toInt();//3
    this->m_canChannelIndex=list.at(4).toInt();//4

    this->m_baudRate=list.at(5);//5
    this->m_dbcFile=list.at(6);//6

    this->m_workMode=list.at(8);//8
    this->m_filterMode=list.at(9);//9
    this->m_accCode=list.at(10);//10
    this->m_accMask=list.at(11);//11
    this->m_filterFlage=list.at(12);//12
    this->m_canDeviceName=list.at(13);//13
    qDebug()<<"CanDevice::setDeviceAttrubute";
    return 0;
}
/*
*封装对库的一些操作，
*/
int CanDevice::open(){
    qDebug()<<"CanDevice::open()"<<m_canDeviceType<<m_canDeviceIndex;
    //open 返回值=1，表示操作成功；=0表示操作失败；=-1表示USB-CAN设备不存在或USB掉线
    int dwRel = VCI_OpenDevice(m_canDeviceType, m_canDeviceIndex, 0);
    if(dwRel==-1){
        qDebug()<<"VCI_OpenDevice failed:device not exist or usb down!";
        return -1;
    }else if(dwRel==0){
        qDebug()<<"VCI_OpenDevice failed:operation failed!";
    }else{
        qDebug()<<"VCI_OpenDevice sucessed!";
    }

    if(initCan()!=0){
        qDebug()<<"VCI_InitCAN failed";
        return -1;
    }
    qDebug()<<"VCI_InitCAN sucessed";
    if(startCan()!=0){
        qDebug()<<"VCI_StartCan failed";
        return -1;
    }
    qDebug()<<"VCI_StartCan sucessed";
    return 0;
}

int CanDevice::initCan(){
    //init
    VCI_INIT_CONFIG vic;
    /*
    *验收码。SJA1000的帧过滤验收码。对经过屏蔽码过滤为“有关位”进行匹配，全部匹
    *配成功后，此帧可以被接收。否则不接收。详见VCI_InitCAN。
    */
    vic.AccCode=0x80000008;
    /*
    *屏蔽码 SJA1000的帧过滤屏蔽码。对接收的CAN帧ID进行过滤，对应位为0的是“有
    *关位”，对应位为1的是“无关位”。屏蔽码推荐设置为0xFFFFFFFF，即全部接收。
    */
    vic.AccMask=0xFFFFFFFF;//0xFFFFFFFF
    /*
    *示例：
    *若把AccCode的值设为0x24600000（即0x123左移21位的结果），
    *AccMask的值设为0x00000000，则只有CAN信息帧ID为0x123的报
    *文能够被接受(AccMask的值0x00000000表示所有位均为相关位)。若把 AccCode 的值设为0x24600000，
    *AccMask的值设为0x600000（0x03左移21位的结果），则只有CAN信息帧ID为
    *0x120～0x123的报文都能够被接受(AccMask的值 0x600000 表示除了bit0~bit1其他位
    *(bit2~bit10)均为相关位)。
    *注：本滤波器设置示例以标准帧为例，高11位有效；
    *若为扩展帧，则ID为29位，AccCode和AccMask设置时高29位对扩展帧有效！
    */

    /*
    * 滤波模式
    *0|1 接收所有类型 滤波器同时对标准帧与扩展帧过滤！
    *2   只接收标准帧 滤波器只对标准帧过滤，扩展帧将直接被滤除。
    *3   只接收扩展帧 滤波器只对扩展帧过滤，标准帧将直接被滤除。
    */
    vic.Filter=1;//this->m_filterMode.toInt();
    /*
    * CAN波特率 Timing0(BTR0) Timing1(BTR1)
    * 10 Kbps    0x31            0x1C
    * 20 Kbps    0x18            0x1C
    * 40 Kbps    0x87            0xFF
    * 50 Kbps    0x09            0x1C
    * 80 Kbps    0x83            0xFF
    * 100 Kbps   0x04            0x1C
    * 125 Kbps   0x03            0x1C
    * 200 Kbps   0x81            0xFA
    * 250 Kbps   0x01            0x1C
    * 400 Kbps   0x80            0xFA
    * 500 Kbps   0x00            0x1C
    * 666 Kbps   0x80            0xB6
    * 800 Kbps   0x00            0x16
    * 1000 Kbps  0x00            0x14
    * 33.33 Kbps 0x09            0x6F
    * 66.66 Kbps 0x04            0x6F
    * 83.33 Kbps 0x03            0x6F
    */
    if(this->m_baudRate=="500k"){
        vic.Timing0=0x00;
        vic.Timing1=0x1C;
    }else if(this->m_baudRate=="1000k"){
        vic.Timing0=0x00;
        vic.Timing1=0x14;
    }

    /*
    * 工作模式
    * 0-正常工作
    * 1-仅监听模式
    * 2-自发自收测试模式
    */
    vic.Mode=this->m_workMode.toInt();

    int dwRel=0;
    dwRel = VCI_InitCAN(m_canDeviceType, m_canDeviceIndex, m_canChannelIndex, &vic);
    if(dwRel!=1){
        return -1;
    }
    return 0;
}
int CanDevice::startCan(){
    if(VCI_StartCAN(m_canDeviceType,m_canDeviceIndex,m_canChannelIndex)!=1)return -1;
    return 0;
}
int CanDevice::readBoardInfo(){
    VCI_BOARD_INFO vbi;
    int dwRel;
    dwRel = VCI_ReadBoardInfo(m_canDeviceType, m_canDeviceIndex, &vbi);
    if(dwRel != 1)
    {
    qDebug()<<"readBoardInfo failed!";
    return FALSE;
    }
}
int CanDevice::getReceiveNum(){
    VCI_GetReceiveNum(m_canDeviceType,m_canDeviceIndex,m_canChannelIndex);
    return 0;
}
int CanDevice::clearBuffer(){
    VCI_ClearBuffer(m_canDeviceType,m_canDeviceIndex,m_canChannelIndex);
    return 0;
}
int CanDevice::resetCAN(){
    VCI_ResetCAN(m_canDeviceType,m_canDeviceIndex,m_canChannelIndex);
    return 0;
}
/*
*封装对库的一些操作
*/
int CanDevice::close(){
    qDebug()<<"CanDevice::close()";
    VCI_CloseDevice(m_canDeviceType, m_canChannelIndex);
    return 0;
}
int CanDevice::readData(int len){

    int ret=VCI_Receive(m_canDeviceType,m_canDeviceIndex,m_canChannelIndex,vco,len,-1);
    return ret;
}
int CanDevice::sendData(QString msg){
    VCI_CAN_OBJ sendobj;
    QStringList list=msg.split(" ");
    sendobj.ID=list.at(5).toInt(nullptr,16);
    sendobj.DataLen=list.at(6).toInt(nullptr,16);
    qDebug()<<"list.len()========="<<list.length();
    for(int i=0;i<sendobj.DataLen;i++){
        sendobj.Data[i]=list.at(7+i).toInt(nullptr,16);
    }

    int ret=VCI_Transmit(m_canDeviceType,m_canDeviceIndex,m_canChannelIndex,&sendobj,1);
    return ret;
}
//线程1
void CanDevice::processHandle(){//处理线程
    qDebug()<<"CanDevice::processHandle()...."<<QThread::currentThread()<<"["<<m_canDeviceType<<"] "<<"["<<m_canDeviceIndex<<"] "<<"["<<m_canChannelIndex<<"]";
    //  Prepare our context and publisher
    zmq::context_t context (1);
    zmq::socket_t publisher (context, ZMQ_PUB);
    publisher.connect("tcp://localhost:7777");
    resetCAN();
    while(m_exitStateThread1){
        int lRel=readData(1000);
        if(lRel > 0){ /* 数据处理 */
//            qDebug()<<"CanDevice::processHandle()"<<QThread::currentThread()<<m_canDeviceType<<m_canDeviceIndex<<m_canChannelIndex<<lRel;
            for(int i=0;i<lRel;i++){
                QString data;
                data.clear();
                QString temp;
                for(int j=0;j<vco[i].DataLen;j++){
                    temp=QString("%1").arg(vco[i].Data[j],2,16,QChar('0'));
                    data.append(temp);
                    data.append(" ");
                }
                //TOPC 4  0  0  time  ID DataLen Data........................  index
                //TOPC %1 %2 %3  %4   %5 %6      %7 %8 %9 %10 %11 %12 %13 %14  %15
                QString header=QString("TOPC %1 %2 %3").arg(m_canDeviceType).arg(m_canDeviceIndex).arg(m_canChannelIndex);
                QString senddata=QString(" %1 %2 %3 %4 %5")
                        .arg(vco[i].TimeStamp,8,16,QChar('0'))
                        .arg(vco[i].ID,8,16,QChar('0'))
                        .arg(vco[i].DataLen,4,10,QChar('0'))
                        .arg(data)
                        .arg(i,4,10,QChar('0'));
                header=header+senddata;
//                qDebug()<<header;
                s_send(publisher,header.toStdString());
            }

        }else if(lRel == -1){/* USB-CAN设备不存在或USB掉线，可以调用VCI_CloseDevice并重新VCI_OpenDevice。如此可以达到USB-CAN设备热插拔的效果。 */
            //todo,设备掉线,异常处理
            qDebug()<<"CanDevice::processHandle() error:"<<QThread::currentThread()<<lRel;
        }
        QThread::msleep(50);
    }
    qDebug()<<"CanDevice::processHandle() exit!";
}
//线程2启动函数
void CanDevice::star()//启动函数是需要在原有代码基础上增加
{
    qDebug()<<"QtConcurrent::run(this,&CanDevice::stateThread);//多线程执行死循环启动:";
    m_exitStateThread2=1;
    QtConcurrent::run(this,&CanDevice::stateThread);//多线程执行死循环启动，可以带参数，具体格式可以查阅网上其它资料
}
//线程2
/*
* 功能定位：
* 1启动zeromq订阅功能
* 2从指定端口订阅CANControl主题消息
* 3收到消息下发can控制指令
*/
void CanDevice::stateThread(){
    qDebug()<<"CanDevice::stateThread():"<<QThread::currentThread();
    zmq::context_t context (1);
    //  Socket to talk to server
    std::cout << "Collecting CAN control command  client...\n" << std::endl;
    zmq::socket_t subscriber (context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:7888");

    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = "TORADAR ";//s
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
    while (m_exitStateThread2) {
//        //  Read address
        std::string toRadarMsg = s_recv(subscriber);
//        //  Read identify
//        std::string identify = s_recv(subscriber);
//        //  Read message contents
//        std::string contents = s_recv(subscriber);
//        std::cout << "[" << address << "] "<< "[" << identify << "] " << contents << std::endl;
        std::cout << "[command:]" <<m_exitStateThread2<<toRadarMsg<< std::endl;
        sendData(QString::fromStdString(toRadarMsg));
    }
    qDebug()<<"CanDevice::stateThread() end!!";
}
int CanDevice::initZmq(){

}

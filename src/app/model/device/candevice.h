#ifndef CANDEVICE_H
#define CANDEVICE_H

#include"abstractdevice.h"
#include<windef.h>
#include<windows.h>
#include"ControlCan.h"
//#include"MT_API.h"

class CanDevice : public AbstractDevice
{
public:
    CanDevice();
    int initCan();
    int startCan();
    int readBoardInfo();
    int getReceiveNum();
    int clearBuffer();
    int resetCAN();


protected:
    virtual int setDeviceAttrubute(QStringList&list);
    virtual int open();
    virtual int close();
    //线程1
    virtual void processHandle();//处理线程
    //线程2
    virtual void star();
    virtual void stateThread();
    virtual int initZmq();
    int readData(int len=2500);
    int sendData(QString msg);
private:

    int m_tableIndex;//序号
    //操作设备所需关键参数，不同设备不同
    int m_canDeviceType; //设备类型/* USBCAN-2A或USBCAN-2C或CANalyst-II */
    int m_canDeviceIndex;//设备号
    int m_canChannelIndex;//通道号
    QString m_baudRate;// 波特率
    QString m_dbcFile;//dbc 文件地址
    QString m_workMode;//工作模式
    QString m_filterMode;//滤波模式
    QString m_accCode;//验收码
    QString m_accMask;//屏蔽码 默认全F
    QString m_filterFlage;//过滤器开关
    QString m_canDeviceName;//can设备名称
    VCI_CAN_OBJ vco[2500];//can接收数据结构

//    //zmq add
//    zmq::context_t context;
//    zmq::socket_t publisher;

};

#endif // CANDEVICE_H

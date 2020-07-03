#ifndef USBDEVICE_H
#define USBDEVICE_H

#include"abstractdevice.h"
#include<windef.h>
#include<windows.h>
//#include"MT_API.h"

class USBDevice : public AbstractDevice
{
public:
    USBDevice();
    ~USBDevice();
protected:
    virtual int setDeviceAttrubute(QStringList&list);
    virtual int open();
    virtual int close();
    virtual int initZmq();
    //线程1
    virtual void processHandle();//处理线程
    //线程2
    virtual void star();
    virtual void stateThread();

private:

    int gotoAbs(int obj,double AStepAngle,int ADiv,double AcircleRatio,double AValue);
    //USB设备属性
    int m_index;
    QString m_ConnectType;
    QString m_ConnectPara;
    QString m_AStepAngle1;
    QString m_ADiv1;
    QString m_ACircleRatio1;
    QString m_Acc1;
    QString m_Dec1;
    QString m_SpeedMax1;
    QString m_AStepAngle2;
    QString m_ADiv2;
    QString m_ACircleRatio2;
    QString m_Acc2;
    QString m_Dec2;
    QString m_SpeedMax2;

};

#endif // USBDEVICE_H

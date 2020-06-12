#ifndef WINPCAPDEVICE_H
#define WINPCAPDEVICE_H
/*
*Winpcap 设备控件
*/
#include"abstractdevice.h"
class WinPcapDevice : public AbstractDevice
{
public:
    WinPcapDevice();
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
};

#endif // WINPCAPDEVICE_H

#ifndef NETDEVICE_H
#define NETDEVICE_H

#include"abstractdevice.h"
class NetDevice : public AbstractDevice
{
public:
    NetDevice();
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

#endif // NETDEVICE_H

#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H
#include"abstractdevice.h"

class SerialDevice : public AbstractDevice
{
public:
    SerialDevice();
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

#endif // SERIALDEVICE_H

#ifndef ABSTRACTDEVICE_H
#define ABSTRACTDEVICE_H
#include <QObject>
#include<QMutex>
#include<QDebug>
#include<QtConcurrent>
#include"mythread.h"
#include"zhelpers.hpp"
#include<iostream>
using namespace std;
class AbstractDevice: public QObject
{
    Q_OBJECT
public:
    explicit AbstractDevice(QObject *parent = nullptr);
    ~AbstractDevice(){}//基类写为虚析构函数
    virtual int setDeviceAttrubute(QStringList&list)=0;
    virtual int open()=0;
    virtual int close()=0;
    int startDeviceProcess();
    int stopDeviceProcess();



    virtual int initZmq()=0;

protected:
//    void timerEvent(QTimerEvent *event);
    int setExitState(int threadIndex,int);
    //线程1
    virtual void processHandle()=0;//处理线程
    //线程2
    virtual void star()=0;
    virtual void stateThread()=0;

    QStringList m_device_attribute;
    QMutex mutex;
    int m_exitStateThread1;

    int m_exitStateThread2;

signals:
    void threadid();
    void sig_startProcess();
public slots:
    void slot_ThreadID();
    void slot_ProcessHandle();



};

#endif // ABSTRACTDEVICE_H

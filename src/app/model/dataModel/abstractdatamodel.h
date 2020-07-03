#ifndef ABSTRACTDATAMODEL_H
#define ABSTRACTDATAMODEL_H

#include <QObject>
#include<QMutex>
#include<QDebug>
#include<QtConcurrent>
#include"mythread.h"
#include"zhelpers.hpp"
#include<iostream>
class AbstractDataModel : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDataModel(QObject *parent = nullptr);
    ~AbstractDataModel(){}//基类写为虚析构函数int setExitState(int threadIndex,int);
    //线程1
    virtual void processHandle()=0;//处理线程
    //线程2
    virtual void star()=0;
    virtual void stateThread()=0;
    int startDeviceProcess();
    int stopDeviceProcess();
    int setExitState(int threadIndex,int);
    virtual void handleMsgToRadar(QString str)=0;;
protected:
    QMutex mutex;
    int m_exitStateThread1;
    int m_exitStateThread2;
signals:
    void threadid();
    void sig_startProcess();
    void sig_msgToPC(QString);
    void sig_stateInfo(QString);

public slots:
    void slot_ThreadID();
    void slot_ProcessHandle();
    void slot_msgToRadar(QString );
private:

};

#endif // ABSTRACTDATAMODEL_H

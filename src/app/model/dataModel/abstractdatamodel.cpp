#include "abstractdatamodel.h"

AbstractDataModel::AbstractDataModel(QObject *parent) : QObject(parent)
{
    qDebug()<<"AbstractDataModel construct!"<<QThread::currentThread();
    MyThread *thread = new MyThread;
    this->moveToThread(thread);
    thread->start();
    //测试多线程
    connect(this,SIGNAL(threadid()),this,SLOT(slot_ThreadID()),Qt::QueuedConnection);
    emit threadid();

    connect(this,SIGNAL(sig_startProcess()),this,SLOT(slot_ProcessHandle()),Qt::QueuedConnection);
    m_exitStateThread1=0;
    m_exitStateThread2=0;
}
int AbstractDataModel::setExitState(int threadIndex,int exi){
    QMutexLocker locker(&mutex);
    if(threadIndex==0){
        this->m_exitStateThread1=exi;
    }else if(threadIndex==1){
        this->m_exitStateThread2=exi;
    }
    return 0;
}
int AbstractDataModel::startDeviceProcess(){
    qDebug()<<"AbstractDataModel::startDeviceProcess()";
    setExitState(0,1);
    QThread::msleep(50);
    setExitState(1,1);
    emit sig_startProcess();
    return 0;
}
int AbstractDataModel::stopDeviceProcess(){
    qDebug()<<"AbstractDataModel::stopDeviceProcess()";
    setExitState(0,0);
    QThread::msleep(50);
    setExitState(1,0);
    return 0;
}
void AbstractDataModel::slot_ThreadID(){
    qDebug()<<"AbstractDataModel::slot_ThreadID():"<<QThread::currentThread();
}
void AbstractDataModel::slot_ProcessHandle(){
    this->star();
    this->processHandle();
}
//线程1 响应控制指令转发给雷达
void AbstractDataModel::slot_msgToRadar(QString str){

    this->handleMsgToRadar(str);

}

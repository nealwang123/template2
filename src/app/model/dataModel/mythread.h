#ifndef MYTHREAD_H
#define MYTHREAD_H

#include<QObject>
#include<QThread>
class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject *parent = 0): QThread(parent){

    }
signals:

public slots:
};

#endif // MYTHREAD_H

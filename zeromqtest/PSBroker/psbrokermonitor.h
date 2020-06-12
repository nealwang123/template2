#ifndef PSBROKERMONITOR_H
#define PSBROKERMONITOR_H

#include <QObject>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include <QDebug>
#include "zhelpers.hpp"
class PSBrokerMonitor : public QThread
{
public:
    PSBrokerMonitor(int brokerType=0,int exitFlage=0);
    void run();
    int setType(QString type);
    int setExitFlage(int exit);
private:
    int m_brokerType;
    int m_exitFlage;
};

#endif // PSBROKERMONITOR_H

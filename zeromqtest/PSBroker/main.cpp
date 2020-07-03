#include "psbrokerwidget.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    PSBrokerMonitor *broker=new PSBrokerMonitor(1,1);
    broker->start();
    return a.exec();

}

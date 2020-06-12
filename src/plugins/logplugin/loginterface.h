#ifndef LOGINTERFACE_H
#define LOGINTERFACE_H

#include <QObject>
#include "../../pluginmanager/plugininterface.h"

class LogInterface:public PluginInterface
{
public:
    virtual ~LogInterface() {}
    virtual void log(QString)=0;
};
#endif // LOGINTERFACE_H

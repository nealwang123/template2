#ifndef LOGPLUGIN_H
#define LOGPLUGIN_H

#include <loginterface.h>


class LogPlugin : public QObject,public LogInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.LogPlugin" FILE "logplugin.json")
#endif // QT_VERSION >= 0x050000

    Q_INTERFACES(PluginInterface)
public:
    LogPlugin(QObject *parent = 0);

     void log(QString);
};

#endif // LOGPLUGIN_H

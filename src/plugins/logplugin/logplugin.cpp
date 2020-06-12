#include "logplugin.h"
#include <QDebug>

LogPlugin::LogPlugin(QObject *parent) :
    QObject(parent)
{
}

void LogPlugin::log(QString str)
{
    qDebug()<<"logplugin:"<<str;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(logplugin, LogPlugin)
#endif // QT_VERSION < 0x050000

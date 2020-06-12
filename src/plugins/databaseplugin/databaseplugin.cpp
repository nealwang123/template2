#include "databaseplugin.h"
#include <QDebug>

DatabasePlugin::DatabasePlugin(QObject *parent) :
    QObject(parent)
{
}

void DatabasePlugin::insert(QString str)
{
    qDebug()<<"DatabasePlugin:"<<str;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(databaseplugin, DatabasePlugin)
#endif // QT_VERSION < 0x050000

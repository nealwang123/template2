#ifndef DATABASEPLUGIN_H
#define DATABASEPLUGIN_H

#include <QGenericPlugin>
#include <databaseinterface.h>

class DatabasePlugin : public QObject,public DatabaseInterface
{
    Q_OBJECT
#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.DatabasePlugin" FILE "databaseplugin.json")
#endif // QT_VERSION >= 0x050000
    Q_INTERFACES(PluginInterface)

public:
    DatabasePlugin(QObject *parent = 0);

    void insert(QString);
};

#endif // DATABASEPLUGIN_H

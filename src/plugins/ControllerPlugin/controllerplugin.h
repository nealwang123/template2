#ifndef CONTROLLERPLUGIN_H
#define CONTROLLERPLUGIN_H


#include <QObject>
#include <QWidget>
#include <QSharedMemory>
#include <QTimer>
#include "controllerplugin_global.h"
#include "../../src/plugininterface.h"
class CONTROLLERPLUGINSHARED_EXPORT ControllerPlugin:public QObject,PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "controllerplugin.json")
    Q_INTERFACES(PluginInterface)
public:
    ControllerPlugin();
    QString name()const;
    QString iconPath() const;
    QWidget *widget(QWidget *parent) const;
    AUTOROAD_WIDGETTYPE autoroad_Widget() const;
    Qt::DockWidgetArea autoroad_DockWidgetArea() const;

public slots:
    void slot_sendMsg2Manager(int);
    virtual void recMsgfromManager(PluginMetaData metadate)  Q_DECL_OVERRIDE;

    virtual void slot_writeShareMemory();
    /*virtual*/ void slot_readShareMemory();
signals:
    virtual void sendMsg2Manager(PluginMetaData metadate) Q_DECL_OVERRIDE;
public:
    QSharedMemory *m_pShareMemory;
    QTimer *m_readShareMemory_timer;

};

#endif // CONTROLLERPLUGIN_H

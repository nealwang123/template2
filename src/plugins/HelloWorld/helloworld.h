#ifndef HELLOWORLD_H 
#define HELLOWORLD_H 

#include <QObject>
#include <QWidget>
#include <QSharedMemory>
#include <QTimer>
#include "helloworld_global.h"
#include "../../src/plugininterface.h"


class HELLOWORLDSHARED_EXPORT HelloWorld:public QObject,PluginInterface
{
   Q_OBJECT
   Q_PLUGIN_METADATA(IID PluginInterface_iid FILE "helloworld.json")
   Q_INTERFACES(PluginInterface)


public:
    HelloWorld();
    QString name() const;
    QString iconPath() const;
    QWidget *widget(QWidget *parent) const;
    AUTOROAD_WIDGETTYPE autoroad_Widget() const;
    Qt::DockWidgetArea autoroad_DockWidgetArea() const;
     
public slots:
    virtual void recMsgfromManager(PluginMetaData)  Q_DECL_OVERRIDE;

    virtual void slot_writeShareMemory();
    /*virtual*/ void slot_readShareMemory();
signals:
    void sig_recMsgfromManager(int);
    virtual void sendMsg2Manager(PluginMetaData) Q_DECL_OVERRIDE;
public:
    QSharedMemory *m_pShareMemory;
    QTimer * m_readShareMemory_timer;
};
#endif // TESTPLUGIN_H

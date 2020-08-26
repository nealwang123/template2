#ifndef AbstractDeviceManager_H
#define AbstractDeviceManager_H

#include <QObject>
#include"abstractdevice.h"
#include"abstractdevicefactory.h"
#include"quiwidget.h"
#include"app.h"
#include<QDebug>
#include"mythread.h"
class AbstractDeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDeviceManager(QObject *parent = nullptr);
    virtual ~AbstractDeviceManager(){}//基类写为虚析构函数
    int readConfig();
    int InitDeviceFactory(AbstractDeviceFactory* a);
    int InitDevice();
    int UnInitDevice();

    int startProcess();//启动设备处理线程
    int stopProcess();//停止设备处理线程
protected:
    virtual int initSql();//初始化数据库
    virtual int unInitSql();//关闭数据库相关
    virtual int setAttrubute();//设置设备相关属性
    virtual int Open();//打开设备
    virtual int Close();//关闭设备


    QString m_DeviceManagerName;
    //设备工厂基类对象
    AbstractDeviceFactory* m_device_factory;
    AbstractDevice* m_device_temp;
    //对设备需修改
    QList<AbstractDevice*> m_device_list;
    //存储配置数据数据库
    QSqlDatabase _db;
    //存储多个同类设备配置数据
    QMap<int,QStringList> m_listMap;

signals:

public slots:

private:


};

#endif // AbstractDeviceManager_H

#ifndef ABSTRACTDEVICEFACTORY_H
#define ABSTRACTDEVICEFACTORY_H

#include <QObject>
#include<QStringList>
#include"abstractdevice.h"
class AbstractDeviceFactory : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDeviceFactory(QObject *parent = nullptr);
    virtual AbstractDevice* createDeviceMethod()=0;
    ~AbstractDeviceFactory(){}//基类写为虚析构函数

signals:

public slots:
};

#endif // ABSTRACTDEVICEFACTORY_H

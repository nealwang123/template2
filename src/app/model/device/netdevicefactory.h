#ifndef NETDEVICEFACTORY_H
#define NETDEVICEFACTORY_H

#include"abstractdevicefactory.h"
#include"NetDevice.h"
class NetDeviceFactory : public AbstractDeviceFactory
{
public:
    NetDeviceFactory();
    virtual AbstractDevice* createDeviceMethod();
};

#endif // NETDEVICEFACTORY_H

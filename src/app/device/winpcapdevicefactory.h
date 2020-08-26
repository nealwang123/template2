#ifndef WINPCAPDEVICEFACTORY_H
#define WINPCAPDEVICEFACTORY_H
#include"abstractdevicefactory.h"
#include"winpcapdevice.h"
class WinPcapDeviceFactory : public AbstractDeviceFactory
{
public:
    WinPcapDeviceFactory();
    virtual AbstractDevice* createDeviceMethod();
};

#endif // WINPCAPDEVICEFACTORY_H

#ifndef USBDEVICEFACTORY_H
#define USBDEVICEFACTORY_H
#include"abstractdevicefactory.h"
#include"usbdevice.h"
class USBDeviceFactory : public AbstractDeviceFactory
{
public:
    USBDeviceFactory();
    virtual AbstractDevice* createDeviceMethod();
};

#endif // USBDEVICEFACTORY_H

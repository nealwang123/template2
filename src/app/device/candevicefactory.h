#ifndef CANDEVICEFACTORY_H
#define CANDEVICEFACTORY_H

#include"abstractdevicefactory.h"
#include"candevice.h"
class CANDeviceFactory : public AbstractDeviceFactory
{
public:
    CANDeviceFactory();
    virtual AbstractDevice* createDeviceMethod();
};

#endif // CANDEVICEFACTORY_H

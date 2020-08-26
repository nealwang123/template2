#ifndef SERIALDEVICEFACTORY_H
#define SERIALDEVICEFACTORY_H

#include"abstractdevicefactory.h"
#include"serialdevice.h"
class SerialDeviceFactory : public AbstractDeviceFactory
{
public:
    SerialDeviceFactory();
    virtual AbstractDevice* createDeviceMethod();
};

#endif // SERIALDEVICEFACTORY_H

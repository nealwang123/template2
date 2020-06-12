#include "serialdevicefactory.h"

SerialDeviceFactory::SerialDeviceFactory()
{

}
AbstractDevice* SerialDeviceFactory::createDeviceMethod(){
    return new SerialDevice();
}

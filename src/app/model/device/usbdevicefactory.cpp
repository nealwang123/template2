#include "usbdevicefactory.h"
USBDeviceFactory::USBDeviceFactory()
{

}
AbstractDevice* USBDeviceFactory::createDeviceMethod(){
    return new USBDevice();
}

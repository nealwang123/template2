#include "candevicefactory.h"

CANDeviceFactory::CANDeviceFactory()
{

}
AbstractDevice* CANDeviceFactory::createDeviceMethod(){
    return new CanDevice();
}

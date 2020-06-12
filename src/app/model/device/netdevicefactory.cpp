#include "netdevicefactory.h"

NetDeviceFactory::NetDeviceFactory()
{

}
AbstractDevice* NetDeviceFactory::createDeviceMethod(){
    return new NetDevice();
}

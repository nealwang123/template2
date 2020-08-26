#include "winpcapdevicefactory.h"

WinPcapDeviceFactory::WinPcapDeviceFactory()
{

}
AbstractDevice* WinPcapDeviceFactory::createDeviceMethod(){
    return new WinPcapDevice();
}

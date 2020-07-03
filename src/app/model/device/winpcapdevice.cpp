#include "winpcapdevice.h"

WinPcapDevice::WinPcapDevice()
{
    qDebug()<<"WinPcapDevice::WinPcapDevice()";
}
int WinPcapDevice::setDeviceAttrubute(QStringList&list){
    return 0;
}
int WinPcapDevice::open(){
    return 0;
}
int WinPcapDevice::close(){
    return 0;
}
//线程1
 void WinPcapDevice::processHandle(){}//处理线程
//线程2
void WinPcapDevice::star(){}
 void WinPcapDevice::stateThread(){}
int WinPcapDevice::initZmq(){
    return 0;
}

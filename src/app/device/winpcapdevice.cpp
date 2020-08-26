#include "winpcapdevice.h"

WinPcapDevice::WinPcapDevice()
{
    qDebug()<<"WinPcapDevice::WinPcapDevice()";
}
int WinPcapDevice::setDeviceAttrubute(QStringList&list){}
int WinPcapDevice::open(){}
int WinPcapDevice::close(){}
//线程1
 void WinPcapDevice::processHandle(){}//处理线程
//线程2
void WinPcapDevice::star(){}
 void WinPcapDevice::stateThread(){}
int WinPcapDevice::initZmq(){}

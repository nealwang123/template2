#include "canapi.h"
/// <summary>
/// 设备连接
/// </summary>
/// <returns></returns>
bool CANApi::OpenDevice(int devicetype,int deviceindex,int ch){
    ECANStatus eCANStatus = (ECANStatus)VCI_OpenDevice(devicetype,deviceindex,ch);

    if (eCANStatus == _STATUS_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// <summary>
/// 设备关闭
/// </summary>
/// <returns></returns>
bool CANApi::CloseDevice(int devicetype,int deviceindex){
    ECANStatus eCANStatus = (ECANStatus)VCI_CloseDevice(devicetype,deviceindex);

    if (eCANStatus == _STATUS_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// <summary>
/// CAN设备参数初始化
/// </summary>
/// <returns></returns>
bool CANApi::CANInit(int devicetype,int deviceindex,int ch,int baudrate){

    VCI_INIT_CONFIG iNIT_CONFIG ;
    iNIT_CONFIG.AccCode = 0;
    iNIT_CONFIG.AccMask = 0xFFFFFF;
    iNIT_CONFIG.Filter = 0;iNIT_CONFIG.Mode = 0;

    switch(baudrate)
    {
        case 0://500Kbps
            {
        //87.5
        //                iNIT_CONFIG.Timing0 =0x00;
        //                iNIT_CONFIG.Timing1 =0x1C;
        //62.5
                        iNIT_CONFIG.Timing0 =0x40;
                        iNIT_CONFIG.Timing1 =0x58;
//        //81.3
//                        iNIT_CONFIG.Timing0 =0x00;
//                        iNIT_CONFIG.Timing1 =0x2B;
//        //75

            }
            break;
        case 1://10Kbps
            {
                iNIT_CONFIG.Timing0 =0x31;
                iNIT_CONFIG.Timing1 =0x1C;
            }
            break;
        case 2://20Kbps
            {
                iNIT_CONFIG.Timing0 =0x18;
                iNIT_CONFIG.Timing1 =0x1C;
            }
            break;
        case 3://40Kbps
            {
                iNIT_CONFIG.Timing0 =0x87;
                iNIT_CONFIG.Timing1 =0xFF;
            }
            break;
        case 4://50Kbps
            {
                iNIT_CONFIG.Timing0 =0x09;
                iNIT_CONFIG.Timing1 =0x1C;
            }
            break;
        case 5://80Kbps
            {
                iNIT_CONFIG.Timing0 =0x83;
                iNIT_CONFIG.Timing1 =0xFF;
            }
            break;
        case 6://100Kbps
            {
                iNIT_CONFIG.Timing0 =0x04;
                iNIT_CONFIG.Timing1 =0x1C;
            }
            break;
        case 7://125Kbps
            {
                iNIT_CONFIG.Timing0 =0x03;
                iNIT_CONFIG.Timing1 =0x1C;
            }
            break;
        case 8://200Kbps
            {
                iNIT_CONFIG.Timing0 =0x81;
                iNIT_CONFIG.Timing1 =0xFA;
            }
            break;
        case 9://250Kbps
            {
                iNIT_CONFIG.Timing0 =0x01;
                iNIT_CONFIG.Timing1 =0x1C;
            }
            break;
        case 10://400Kbps
            {
                iNIT_CONFIG.Timing0 =0x80;
                iNIT_CONFIG.Timing1 =0xFA;
            }
            break;
        case 11://500Kbps
            {

            }
            break;
        case 12://666Kbps
            {
                iNIT_CONFIG.Timing0 =0x80;
                iNIT_CONFIG.Timing1 =0xB6;
            }
            break;
        case 13://800Kbps
            {
                iNIT_CONFIG.Timing0 =0x00;
                iNIT_CONFIG.Timing1 =0x16;
            }
            break;
        case 14://1000Kbps
            {
                iNIT_CONFIG.Timing0 =0x00;
                iNIT_CONFIG.Timing1 =0x14;
            }
            break;
    }
    ECANStatus eCANStatus = (ECANStatus)VCI_InitCAN(devicetype,deviceindex,ch, &iNIT_CONFIG);

    if (eCANStatus == _STATUS_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// <summary>
/// 启动CAN通信
/// </summary>
/// <returns></returns>
bool CANApi::StartCan(int devicetype,int deviceindex,int ch){

    ECANStatus eCANStatus = (ECANStatus)VCI_StartCAN(devicetype,deviceindex,ch);

    if (eCANStatus == _STATUS_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}
/// <summary>
/// 发送一帧的数据
/// </summary>
/// <param name="frame">发送经过UDS网络层之后的数据帧</param>
/// <returns></returns>
ECANStatus CANApi::SendOneFrame(byte remoteflag, byte externflag, VCI_CAN_OBJ frame[],int devicetype,int deviceindex,int ch){
    VCI_CAN_OBJ cAN_OBJ[1];

//  lock (Lock_Object)//锁定主要用于多线程资源的共享，在同一时刻只允许一个线程对资源进行访问
//  {
    memcpy(cAN_OBJ,frame,sizeof (VCI_CAN_OBJ));
    //cAN_OBJ = frame;
    cAN_OBJ[0].SendType = 0;
    cAN_OBJ[0].ExternFlag = externflag;
    cAN_OBJ[0].RemoteFlag = remoteflag;



    ECANStatus eCANStatus = (ECANStatus)VCI_Transmit(devicetype, deviceindex, ch, cAN_OBJ, 1);

    if (eCANStatus != _STATUS_OK)
    {
        return _STATUS_ERR;
    }
    else
    {
        return _STATUS_OK;
    }
  //}
}
ECANStatus  CANApi::RecvFrames(VCI_CAN_OBJ obj[], int count,int devicetype,int deviceindex,int ch){
    return (ECANStatus)VCI_Receive(devicetype,deviceindex,ch,obj,count,0);
}
ECANStatus  CANApi::SendFrames(VCI_CAN_OBJ[], int count){
    return _STATUS_OK;
}

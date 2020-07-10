#ifndef CANAPI_H
#define CANAPI_H

//#include<windef.h>
#include<windows.h>
#include"ControlCAN.h"
#include<QObject>
typedef unsigned char byte;
enum ECANStatus
{
    _STATUS_ERR      = 0x00000,
    _STATUS_OK     =   0x00001,
    _STATUS_TIME_OUT =0x00002
};

class CANApi
{
public:
    const static uint SEND_CAN_ID_Self1 = 0x3E8;//
    const static uint RECV_CAN_ID_MIN = 0;
    const static uint RECV_CAN_ID_MAX = 9999;
    /// <summary>
    /// 设备连接
    /// </summary>
    /// <returns></returns>
    static bool OpenDevice(int devicetype,int deviceindex,int ch);
    /// <summary>
    /// 设备关闭
    /// </summary>
    /// <returns></returns>
    static bool CloseDevice(int devicetype,int deviceindex);
    /// <summary>
    /// CAN设备参数初始化
    /// </summary>
    /// <returns></returns>
    static bool CANInit(int devicetype,int deviceindex,int ch,int baudrate);
    /// <summary>
    /// 启动CAN通信
    /// </summary>
    /// <returns></returns>
    static bool StartCan(int devicetype,int deviceindex,int ch);
    /// <summary>
    /// 发送一帧的数据
    /// </summary>
    /// <param name="frame">发送经过UDS网络层之后的数据帧</param>
    /// <returns></returns>
    static ECANStatus SendOneFrame(byte remoteflag, byte externflag, VCI_CAN_OBJ frame[]);
    static ECANStatus RecvFrames(VCI_CAN_OBJ[], int count);
    static ECANStatus SendFrames(VCI_CAN_OBJ[],int count);
};

#endif // CANAPI_H

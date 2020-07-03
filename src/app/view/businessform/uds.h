#ifndef UDS_H
#define UDS_H

#include <QObject>
//#include<windef.h>
#include<windows.h>
#include"ControlCAN.h"
typedef quint8 byte;
enum ECANStatus:uint
{
    _STATUS_ERR      = 0x00000,
    _STATUS_OK     =   0x00001,
    _STATUS_TIME_OUT =0x00002
};
/// UDS协议数据帧
/// </summary>
struct UDSDataFrame//UDS数据帧
{
    uint canID;//CAN通信ID地址
    int longth; //数据长度
    byte* data  ; //传输数据
};
/// <summary>
/// UDS接收数据帧
/// </summary>
struct UDSReceiveFrame
{
public:
    byte frameType;              //帧类型
    int canID;                   //接收到的canID
    byte FS;                     //FC返回的状态，继续，等待，超限
    byte BS;                     //每次发送CF的个数，最后一次可能小于BS值
    byte STmin;                  //发送两个CF帧之间的最小间隔
    byte* receivedData;       //要发送数据的总长度
    int receivedDataCount;     //接收的数据量
    int expectCFSerialNumber;    //期望的连续帧序列号
    int length;                  //数据长度
};

/// <summary>
/// 发送数据，，数据发送时使用两个事件，AutoResetEvent
/// </summary>
class SendFrame//发送数据
{
public:
    int SID; //服务标识符
    int offset;      //已发送字节数
    int byteAmount;   //总的字节数据量
    byte* array;      //要发送的数据
    //AutoResetEvent waitEvent = new AutoResetEvent(false);    //多帧传送等待用事件 FF发完等待FC帧
    //public AutoResetEvent waitEvent = new AutoResetEvent(false);//等待响应事件
};
/// <summary>
/// 接收数据帧和发送数据帧的声明
/// </summary>
class Session//会话
{
public:
    UDSReceiveFrame receiveFrame ;//接收数据帧
    SendFrame sendframe ;//发送数据帧
};
/// <summary>
/// 连续帧
/// </summary>
class SerialFrame//连续帧
{
    int length;//数据长度
    bool isFisrstFrame;//是否为首帧标志位
    int serial;//序列号
    byte data[8] ;//数据
};
class UDS : public QObject
{
    Q_OBJECT
public:
    explicit UDS(QObject *parent = nullptr);
    const static uint SEND_CAN_ID = 0x7A1;//
    const static uint FUNCTION_ID = 0x7DF;
    const static uint RECEIVE_CAN_ID=0x7A9;
    const static uint Timeout = 2500;
    const uint TIME_OUT = 2;
    const int INTERVAL= 30;
    const static bool mConnected = false;
    const static int REC_MSG_BUF_MAX = 0x2710;//接收数据帧缓冲区最大存储量

    Session *session ;
    QList<SerialFrame> serialFrameGlabol;
    static VCI_CAN_OBJ gRecMsgBuf[];//接收缓冲区
    static uint gRecMsgBufHead;//接收缓冲区首地址
    static uint gRecMsgBufTail;//接收缓冲区尾地址

    /// <summary>
    /// 接收和发送函数
    /// </summary>
    /// <param name="data"></data可以是很多的数据不一定是单帧数据可以实多针数据/>
    /// <param name="can_id"></param>
    /// <returns></returns>
    ECANStatus SendAndReceive(byte data[],uint can_id);
    /// <summary>
    /// 发送数据，包括单帧和多帧数据
    /// </summary>
    /// <param name="data"></param>
    /// <param name="canID"></param>
    /// <returns></returns>
    ECANStatus sendData(byte data[], uint canID);//包括发送数据和CAN地址信息
    /// <summary>
    /// 接收数据处理
    /// </summary>
    void ReceiveDataProc();
signals:

public slots:

};
class SeedKey//发送解锁密钥
{
    static byte seedbyte[4];//种子
    static byte keybyte[4];//安全密钥

    const static uint32_t MASK = 0x968ABC63;//MASK
    static void  keyResult() {
        //UInt32 i  = (uint)(seedbyte[0] | seedbyte[1] << 8 | seedbyte[2] << 16 | seedbyte[3] << 24);
    uint32_t i = (uint32_t)(seedbyte[3]| seedbyte[2] << 8 | seedbyte[1] << 16 | seedbyte[0] << 24);
    uint32_t r = ApplicationSW(i, MASK);
    //int转byte
    //keybytetemp = System.BitConverter.GetBytes(r);
    //反转
    keybyte[0] = (r>>24);
    keybyte[1] = (r>>16);
    keybyte[2] = (r>>8);
    keybyte[3] = (r>>0);

        //Console.WriteLine();

    }
    static uint32_t ApplicationSW(uint32_t seed, uint32_t MASK)
    {
        uint32_t i = 0;
        uint32_t key = 0;
        if (seed != 0)
        {
            for (i = 0; i < 34; i++)
            {
                if ((seed & 0x20000000)!=0)
                {
                    seed = seed << 1;
                    seed = ~seed;
                    seed = seed ^ MASK;
                }
                else
                {
                    seed = seed << 1;
                }
            }
            key = seed;
        }
        return key;
    }
    //neusoft source code

    ///MASK：96 8A BC 63
};
#endif // UDS_H

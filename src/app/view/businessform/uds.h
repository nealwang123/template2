#ifndef UDS_H
#define UDS_H

#include <QObject>
#include <QDebug>
#include <QtConcurrent>
#include "quiwidget.h"
#include "canapi.h"
#include "mythread.h"
#include"DbcReader/dbcreader.h"
/// UDS协议数据帧
/// </summary>
struct UDSDataFrame//UDS数据帧
{
    uint canID;//CAN通信ID地址
    int longth; //数据长度
    byte* data  ; //传输数据
};
struct CAN_ERR_INFO{
    uint ErrCode;
    byte Passive_ErrData[50];
    byte ArLost_ErrData;
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
    byte receivedData[1024+2];       //要发送数据的总长度
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
    quint8 SID; //服务标识符
    int offset;      //已发送字节数
    int byteAmount;   //总的字节数据量
    byte array[1026];      //要发送的数据
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
public:
    int length;//数据长度
    bool isFisrstFrame;//是否为首帧标志位
    int serial;//序列号
    byte data[8] ;//数据
};


 enum FrameType:byte
{
    N_PDU_TYPE_SF = 0x00,//单帧类型
    N_PDU_TYPE_FF = 0x10,//首帧类型
    N_PDU_TYPE_CF = 0x20,//连续帧类型
    N_PDU_TYPE_FC = 0x30//流控帧类型
};
 enum WORKMODE:int
{
    FACTORY = 0x00,//工厂模式
    CONSUMER = 0x01,//客户模式
    UDSUPDATE = 0x02,//UDS升级模式
    ALIGN=0x03,//安装标定模式
};
 class SeedKey//发送解锁密钥
 {
 public :
     byte seedbyte[4];//种子
     byte keybyte[4];//安全密钥

     uint32_t MASK = 0x968ABC63;//MASK
     void  keyResult() {
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
     uint32_t ApplicationSW(uint32_t seed, uint32_t MASK)
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
class UDS : public QObject
{
    Q_OBJECT
public:
    static UDS *Instance();
    explicit UDS(QObject *parent = nullptr);
    const static uint SEND_CAN_ID = 0x7A1;//uds send
    const static uint FUNCTION_ID = 0x7DF;
    const static uint RECEIVE_CAN_ID=0x7A9;//uds recv
    const static uint Timeout = 2500;
    const uint TIME_OUT = 2;
    const int INTERVAL= 30;

    const static int REC_MSG_BUF_MAX = 0x2710;//接收数据帧缓冲区最大存储量

    Session *session ;
    QList<SerialFrame> serialFrameGlabol;
    VCI_CAN_OBJ gRecMsgBuf[REC_MSG_BUF_MAX];//接收缓冲区
    uint gRecMsgBufHead;//接收缓冲区首地址
    uint gRecMsgBufTail;//接收缓冲区尾地址
    static bool mConnected;
    /// <summary>
    /// 接收和发送函数
    /// </summary>
    /// <param name="data"></data可以是很多的数据不一定是单帧数据可以实多针数据/>
    /// <param name="can_id"></param>
    /// <returns></returns>
    ECANStatus SendAndReceive(uint can_id,byte data[],int dataLength);
    /// <summary>
    /// 发送数据，包括单帧和多帧数据
    /// </summary>
    /// <param name="data"></param>
    /// <param name="canID"></param>
    /// <returns></returns>
    ECANStatus sendData(uint canID,byte data[],int dataLength );//包括发送数据和CAN地址信息
    /// <summary>
    /// 接收数据处理
    /// </summary>
    void ReceiveDataProc();
    //线程1
    void processHandle();//处理线程
    //线程2
    void star();
    void stateThread();
    void startHandleThread();

    int setExitState(int threadIndex,int exi);
    int FrameNum(byte data[],int dataLength);
    ECANStatus sendFirstFrame(byte data[], int dataLength,UDSDataFrame frame);
    bool udsSleep(int sec);
    int getNextState();
    void setNextState();
    SeedKey seedkey;
    QString getSeedKey();
    quint8 getdataVerify();
    quint8 getconsistencyVerify();
    void setWorkMode(int mode);
    int getWorkMode();
    //非uds带交互发送
    ECANStatus NormalSendAndReceive(uint can_id,byte data[],int dataLength);
private:
    static QScopedPointer<UDS> self;
    int m_exitStateThread1;
    int m_exitStateThread2;
    QMutex mutex;
    int m_waitforNext;
    quint8 dataVerify;
    quint8 consistencyVerify;
    int workmode;
    QByteArray m_recvArray;
    QString respHead;
    DBCReader *dbcloader;

signals:
    void emitEventRecv(VCI_CAN_OBJ obj);
    void emitEOLInfo(QString respHead,QByteArray array);
    void emitTemperatureTest(int id,QString rawdata,QStringList list);
    void installAlignData(QString data);
public slots:

};

class CAN_SEND_FRAME
{
public:
    uint FrameNumber;
    byte DATA[1024+2];


};

class BinRecordBlock
{
public:
    QList<byte> data;//字符列表
};
class FileParse
    {
        public:


        const int INTERVAL = 30;//固定延时时间为30ms。





        //public static int all_K_byte = 0;
        //static uint DownloadProgress = 0x00;

        const int REC_MSG_BUF_MAX = 0x100;//接收数据帧缓冲区最大存储量

        static VCI_CAN_OBJ gRecMsgBuf[0x100];//接收缓冲区

        static uint gRecMsgBufHead;//接收缓冲区首地址

        static uint gRecMsgBufTail;//接收缓冲区尾地址


        bool ParseBinFile(QString path, QList<BinRecordBlock>& binBlockList)
        {

            QFileInfo *fileInfo = new QFileInfo(path);

            qDebug()<<"Bin Size:" + QString::number(fileInfo->size());
            QFile *file = new QFile;
            /*
            * 读取Bin文件
            */
            QByteArray arry;
            file->setFileName(path);
            if(file->open(QIODevice::ReadOnly)){
                QDataStream BinFileData(file);
                char *pBuff = new char[fileInfo->size()];
                BinFileData.readRawData(pBuff,static_cast<int>(fileInfo->size()));
                arry = QByteArray(pBuff,static_cast<int>(fileInfo->size()));

                //qDebug()<< mFileInfo.BinFileRawData;
                file->close();
            }else {
                qDebug()<<"无法读取,请检查BIN文件路径!";
                return false;
            }

            binBlockList.clear();

            BinRecordBlock block;
            qDebug()<<"arry"<<arry.size();
            for(int i=0;i<arry.size();i++){
                block.data.append(arry[i]);

            }

            binBlockList.append(block);
            qDebug()<<"binBlockList"<<binBlockList.at(0).data.size()<<"count"<<binBlockList.at(0).data.count();
            return true;
        }
//        bool checkSum(QString data, int checkNum)
//        {
//            //校验
//            int sum = 0;
//            for (int i = 0; i < data.Length; i = i + 2)
//            {
//                byte value = byte.Parse(data.Substring(i, 2), System.Globalization.NumberStyles.HexNumber);
//                sum += value;
//            }

//            //取低字节，1位
//            byte lowSum = (byte)sum;
//            if (((0x100 - lowSum) == checkNum)|| ((0x100 - lowSum) == 0x100))
//            {
//                return true;
//            }
//            else
//            {
//                return false;
//            }
//        }


    };

#endif // UDS_H

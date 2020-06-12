#ifndef DBCREADER_H
#define DBCREADER_H
#include"dbcreader_global.h"
#include <QObject>
#include <windows.h>
#include <windef.h>
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>
#include <QMap>
#include <QRegExp>
#include <QMutex>
#include <QScopedPointer>
#include <QtEndian>
#define DateType int
#define BitNum 32
#define TR(str)   ((str))  //解决Debug中文乱码,失败
#define _MAX_FILE_PATH_ 1000 //最长文件路径
#define _DBC_NAME_LENGTH_ 127 //名称最长长度
#define _DBC_COMMENT_MAX_LENGTH_ 200 //注释最长长度
#define _DBC_UNIT_MAX_LENGTH_ 10 //单位最长长度
#define _DBC_SIGNAL_MULTI_GROUP_ 255 //一个消息含有复用分组最大值
#define _DBC_SIGNAL_MAX_COUNT_ 255 //一个消息含有的信号的最大数目
#define EPSINON 0.000001f
/***
 * 以下结构 DBCSignal 定义了一个信号，用户着重关注 nValue 项，这个是信号值，作为
 * 解析结果返回，也可以修改该值发送出去，一个消息 DBCMessage 可能会包含多个信号。
 ***/
struct DBCSignal
{

    quint32 nStartBit; // 起始位
    quint32 nLen; // 位长度
    double nFactor; // 转换因子
    double nOffset; // 转换偏移实际值=原始值*nFactor+nOffset
    double nMin; // 最小值
    double nMax; // 最大值
    double nValue; //实际值
    quint64 nRawValue;//原始值
    bool is_signed; //1:有符号数据, 0:无符号
    bool is_motorola;//是否摩托罗拉格式
    quint8 multiplexer_type;//see 'multiplexer type' above 0\1\2
    quint8 multiplexer_value;//复用信号内部ID
    char unit[_DBC_UNIT_MAX_LENGTH_+1];//单位
    QString strName; //名称
    QString strComment; //注释
    QString strValDesc; //值描述
};

/*
 * 以下结构 DBCMessage 定义了一个消息，消息跟帧 ID 对应，一个 DBC 文件当中含有多个消息。
 */
struct  DBCMessage {
    quint32 nID;//MSGID
    quint8 nExtend; //1:扩展帧, 0:标准帧
    quint32 nSize; //消息占的字节数目
    QString strName; //名称
    QString strComment; //注释
    DBCSignal vSignals[_DBC_SIGNAL_MULTI_GROUP_+1][_DBC_SIGNAL_MAX_COUNT_+1]; //信号集合,
    int nSignalCount[_DBC_SIGNAL_MULTI_GROUP_+1];
    //逆向转换使用
    QMap<QString,int> signalToindex;//信号名称转下标/////xxy addi
};

/*
 * 结构 FileInfo 作为初始化参数，strFilePath 指明 DBC 文件的绝对路径，type 指明 DBC 文件的协议类型，
 *PROTOCOL_J1939 代表 J1939 协议，其它协议选择 PROTOCOL_OTHER。
*/
struct FileInfo
{
    char strFilePath[_MAX_FILE_PATH_+1]; //dbc 文件路径
    quint8 type; //dbc 的协议类型, j1939 选择 PROTOCOL_J1939, 其他协议选择 PROTOCOL_OTHER
};
//class DBCReader:public QObject
class DBCREADERSHARED_EXPORT DBCReader
{
//    SINGLETON(DBCReader) // [2]

public:
    explicit DBCReader();
    ~DBCReader();
//    typedef void (*OnSend)(void* ctx, void* pObj);
//    typedef void (*OnMultiTransDone)(void* ctx, DBCMessage* pMsg, quint8* data, quint16 nLen, quint8 nDirection);
    //加载DBC文件
    int loadDBCFile(QString filePath);
    //解析数据，传入ID、Data地址，返回解析结果
    QStringList parseMSGWithData(quint32 ID,BYTE Data[],quint8 m_id=0,bool m_isMulti=0);
    bool parseMSGWithValue(quint32 ID,QString strName,QString strValue,BYTE Data[],quint8 m_id=0,bool m_isMulti=0);
    void debugDBCStruct(QString str);
private:
    void fillDbcMsg(QStringList& sigList,DBCMessage& dbcMsg,int sigCount,int offset);
    quint64 parseDataCrossByte(int startBit, int len, int start2end, BYTE Data[]);
    int parseSignedData(int flage, int bitlen, quint64 rawValue);
    quint8 mybitset(quint8 count);

private:
    QMap <int,DBCMessage*> DBCMap;
//QMap <int,DBCMessage> DBCMapHanTeng;
};

#endif // DBCREADER_H

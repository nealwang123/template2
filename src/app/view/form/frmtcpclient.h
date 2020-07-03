#ifndef FRMTCPCLIENT_H
#define FRMTCPCLIENT_H

#include <QWidget>
#include <QtNetwork>
#include <QtConcurrent>
#include <QFuture>
namespace Ui {
class frmTcpClient;
}
#define MINSIZE 8
#define MSG_TYPE_ID 0x53444154
#define MSG_TYPE_FILE_START 0x52525357
#define MSG_TYPE_FILE_SENDING 0x52525358
#define MSG_TYPE_FILE_END 0x52525359

typedef struct _ObjectData { // not float
    uint8_t clusterNum;
    uint8_t dynProp;
    uint8_t measState;
    uint8_t objClass;
    uint8_t id[4];
    // 0x60B
    float distLong;
    float distLat;
    float vrelLong;
    float vrelLat;
    float RCS;
    // 0x60C
    float DistLongRms;
    float VrelLongRms;
    float DistLatRms;
    float VrelLatRms;
    float ArelLatRms;
    float ArelLongRms;
    float OrientationRms;
    float ProbExist;
    // 0x60D
    float ArelLong;
    float ArelLat;
    float OrientationAngle;
    float Length;
    float Width;
    // not direct 408
    float posYMin;
    float posYMax;
    unsigned char clusterId;
    unsigned char used;
} ObjectData;

typedef struct {
    uint8_t RTC_Hours; /*!< Specifies the RTC Time Hour. This parameter must be set to a value in the 0-12 range if the RTC_HourFormat_12 is selected or 0-23 range if the RTC_HourFormat_24 is selected. */
    uint8_t RTC_Minutes; /*!< Specifies the RTC Time Minutes. This parameter must be set to a value in the 0-59 range. */
    uint8_t RTC_Seconds; /*!< Specifies the RTC Time Seconds.This parameter must be set to a value in the 0-59 range. */
    uint8_t RTC_H12; /*!< Specifies the RTC AM/PM Time. This parameter can be a value of @ref RTC_AM_PM_Definitions */
}RTC_TimeTypeDef;

typedef struct{
    RTC_TimeTypeDef RTC_TimeNowStruct;
    uint32_t mt;
    uint16_t num_obj;
    uint16_t frame;
    ObjectData objectData[100];
}Frame_Content;

class frmTcpClient : public QWidget
{
    Q_OBJECT

public:
    explicit frmTcpClient(QWidget *parent = 0);
    ~frmTcpClient();
    void handleMsg();
//void com_send_SDAT(void) {
//    RTC_GetTime(RTC_Format_BIN,&RTC_TimeNowStruct);
//                           //Ö±½ÓÊä³ö408Êý¾Ý
//    uint32_t len = sizeof(RTC_TimeNowStruct)+sizeof(uint32_t)+2*sizeof(uint16_t)+sizeof(ObjectData)* (uint32_t)numObject;
//    memset(&com_tx,0,COM_TX_PACKET_SIZE);
//    com_tx.u.first.header[0] = 'S';
//    com_tx.u.first.header[1] = 'D';
//    com_tx.u.first.header[2] = 'A';
//    com_tx.u.first.header[3] = 'T';
//    memcpy(com_tx.u.first.length,(uint8_t*)(&len), sizeof(len));
//    //Ô•Ê¼Êý¾Ý°´ÕÕÐ¡¶Ë´«Êä
//    memcpy(com_tx.u.first.data, (uint8_t*)(&RTC_TimeNowStruct), sizeof(RTC_TimeNowStruct));
//    memcpy(com_tx.u.first.data+sizeof(RTC_TimeNowStruct), (uint8_t*)(&MsTick), sizeof(uint32_t));
//    memcpy(com_tx.u.first.data+sizeof(RTC_TimeNowStruct)+sizeof(uint32_t), (uint8_t*) (&numObject), sizeof(uint16_t));
//    memcpy(com_tx.u.first.data+sizeof(RTC_TimeNowStruct)+sizeof(uint32_t)+sizeof(uint16_t), (uint8_t*)(&measCounter), sizeof(uint16_t));
//    memcpy(com_tx.u.first.data+sizeof(RTC_TimeNowStruct)+sizeof(uint32_t)+2*sizeof(uint16_t), (uint8_t*)(objectData), sizeof(ObjectData)*(uint32_t)numObject);
//    Write_Multiple_SOCK_Data_Buffer((unsigned char *)(&com_tx), 4+4+len);
//    //¶à¿Í»§¶Ë
//}
private:
    Ui::frmTcpClient *ui;

    bool isOk;
    QTcpSocket *tcpSocket;
    QTimer *timer;
    //图片名字
    QByteArray m_fileName;
    //接收到的数据
    QByteArray m_recvData;
    //实际图片数据大小
    qint64 m_DataSize;
    //接收图片数据大小
    qint64 m_checkSize;
    //缓存上一次或多次的未处理的数据
    //这个用来处理，重新粘包
    QByteArray m_buffer;
    Frame_Content m_frame;


private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void changeTimer();
    void append(int type, const QString &data, bool clear = false);

    void connected();
    void disconnected();
    void readData();
    void slot_readmesg();
    void sendData(const QString &data);

private slots:
    void on_btnConnect_clicked();
    void on_btnSave_clicked();
    void on_btnClear_clicked();
    void on_btnSend_clicked();

signals:
    void sig_FrameContent(Frame_Content& frame);
};

#endif // FRMTCPCLIENT_H

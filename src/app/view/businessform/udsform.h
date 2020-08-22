#ifndef UDSFORM_H
#define UDSFORM_H
#include <QWidget>
#include <QtSql>
#include<QByteArray>
#include "quiwidget.h"
#include "dbdelegate.h"
#include "canapi.h"
#include "uds.h"
#include "temperaturedelegate.h"
#include"algorithmparadelegate.h"
#include <QStandardItemModel>
#include<QtEndian>
#include"olinedeviceform.h"
#include"mainform.h"
#include"initialphaseform.h"
#include"onlineburnform.h"
#include"candebugform.h"
#include"xmlreader.h"
#define SQLDATAINDEX 7
namespace Ui {
class UDSForm;
}

class BurnInfo{
public:
    static QString SN_Head;
    static QString SN_Year;
    static QString SN_MD;
    static QString SN_Tail;
    static QString HW_A;
    static QString HW_B;
};

class ReadInfo{
public:
    static QString SN_Head;
    static QString SN_Year;
    static QString SN_MD;
    static QString SN_Tail;
    static QString HW_A;
    static QString HW_B;
};


class UDSForm : public QWidget
{
    Q_OBJECT

public:
    explicit UDSForm(QWidget *parent = 0);
    ~UDSForm();
    bool createConnection();
    void startDemarcation(QStringList list);
    //function:替换数据功能函数
    void replaceStr(int rowIndex,int index,int len,QString replaceStr);
    void getDataByIndex(int index,QString &send_data,QString &recv_data);
    void setData(int row,int clown,QString data);
    void addDataToView(int rowIndex, VCI_CAN_OBJ cAN_OBJ1);
    ECANStatus writeBinBlockData(QList<BinRecordBlock> BinBlockList, int type);
    ECANStatus splitalldata_addSID(byte* alldata,int alldatalen, QList<CAN_SEND_FRAME>& cansendframe);
    void sendCommandByIndex(int index);
    void recorveyState();
    ECANStatus SendAndReceive(uint can_id,byte data[],int dataLength);
    ECANStatus eolSendCommandOnce();
    float Byte2Float(QByteArray byte);
    int judgeResult();

    void initOnLineTableView();
private:
    Ui::UDSForm *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    MySubClassedSqlTableModel *model;      //数据表格模型

    QSqlDatabase _db;
    QStringList m_deviceInfoList;
    QTimer* demarcationTimer;
    int demarcationIndex;
    UDS uDS;
    QMap<int,QString>listItem;
    QStringList listcommandValue;
    QStringList listcommandKey;
    int commandIndex;
    QString selfSendStr;
    FileParse fileParse;
    bool m_ParseFile;
    //整块block数据bin文件
    static QList<BinRecordBlock> binRecordBlocks_DriverList ;


    //定时器
    QTimer *sendCommandTimer;
    //延时置灰定时器
    QTimer *disableTimer;

    //can发送指令
    QStringList m_list_canSendCommand;
    QStringList m_list_canSendDiscrib;
    int m_eolcommandIndex;
    QString m_eolselfSendStr;
    QStringList sendSeqList;
    //定时器
    QTimer *m_eolsendCommandTimer;
    int m_modelIndex;
    QStringList m_compare_para;
    quint8 m_eol_Result;

    //高低温测试
    TemperatureDelegate* tempwidget;


    //算法参数窗口
    AlgorithmParaDelegate* algowiget;

    //安装标定
    MainForm installAlign;

    //初相校准
    InitialPhaseForm initialphaseform;
    //在线升级程序
    OnlineBurnForm onlineburnform;
    //
    //QVector<CLASS*> mClasses;   //模拟数据
    //在线设备列表
    OlineDeviceForm *onlineDeviceForm;
    int onlineDeviceFlage;
    QTimer *onlineDeviceTimer;
    int onlineDeviceTimerCounter;
    int onlineDeviceNum;
    QMap<int,ItemData>mapOnlineDevice;
    QMap<int,ItemData>mapAllDevice;

    //模式标识
    int guiworkmode;
    //can调试输出窗口
    CanDebugForm debugform;
    QTimer *calTimeoutTimer;
    int m_calTimeoutTimes;
    int failTimes;
    M_CANConfig m_CanConfig;
    //算法参数列表
    QList<AlgoPara > senddata;


    //射频或自定义指令表
    int m_commandIndex;
    QString m_selfSendStr;
    QStringList m_list_canSendCommand_RF;
    QStringList m_list_canSendDiscrib_RF;


private slots:
    void initForm(QString fileName);
    void slot_demarcationTimer();
    void slot_eolsendCommandTimer();
    void slot_UDSFrameRecv(VCI_CAN_OBJ cAN_OBJ1);
    void slot_EOLInfo(QString respHead,QByteArray array);
    void slot_OnlineBurnInfo(QString respHead,QByteArray array);
    void slot_TemperatureTest(int id,QString ,QStringList list);
    void slot_onlineDeviceTimer();
    void slot_disableTimer();
    void slot_calTimeoutTimer();
private slots:
    void on_btnAdd_clicked();
    void on_btnSave_clicked();
    void on_btnDelete_clicked();
    void on_btnReturn_clicked();
    void on_btnClear_clicked();
    void on_pushButton_clicked();

    void on_cBox_command_activated(int index);
    void on_pushButton_released();
    void on_button_Connect_released();
    void on_buttonChooseFile_released();
    void on_buttonDownload_released();
    void on_cBox_Continue_stateChanged(int arg1);
    void on_buttonSingleTest_released();
    void on_checkBox_released();
    void on_buttonSendcan1_released();
    void on_cBoxcansend_activated(int index);
    void on_lineEdit_PW_editingFinished();
    void on_pushButton_2_released();
    void on_comboBox_activated(int index);
    void on_lineEdit_HV_editingFinished();
    void on_button_InFactory_released();
    void on_button_queryMode_released();
    void on_button_burnInfo_released();
    void on_button_queryInfo_released();
    //void on_button_AlgoQuery_released();
    void on_button_Consumer_released();
    void on_pushButton_onlineTest_released();
    void on_pushButton_3_released();
    void on_button_export_released();

    void on_tabWidget_currentChanged(int index);
    void on_lineEdit_Delay_editingFinished();
    void on_tabWidget_tabBarClicked(int index);
    void on_button_Calibration_1_released();
    void on_button_Calibration_2_released();
    void on_button_Calibration_3_released();
    void on_button_Calibration_4_released();
    void on_button_Calibration_5_released();
    void on_button_Connect_3_released();
    void on_button_Connect_2_released();
    void on_cBox_DeviceIndex_2_activated(int index);
    void on_cBox_DeviceIndex_activated(int index);
    void on_pushButton_4_released();
    void on_pushButton_6_released();
    void on_pushButton_import_released();
    void on_pushButton_export_released();
    void on_toolButton_released();
    void on_cBoxcansend_2_activated(int index);
    void on_cBoxCanSendDiscrib_activated(int index);
    void on_pushButton_selfsend_released();
    void on_button_Calibration_1_pressed();
};

#endif // UDSFORM_H

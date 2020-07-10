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
    void eolSendCommandOnce();

private:
    Ui::UDSForm *ui;

    QList<QString> columnNames; //列名集合
    QList<int> columnWidths;    //列宽集合
    QSqlTableModel *model;      //数据表格模型

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

    //can发送指令
    QStringList m_list_canSendCommand;
    QStringList m_list_canSendDiscrib;
    int m_eolcommandIndex;
    QString m_eolselfSendStr;
    //定时器
    QTimer *m_eolsendCommandTimer;
    int m_modelIndex;
    QStringList m_compare_para;
    quint8 m_eol_Result;

    //高低温测试
    TemperatureDelegate* tempwidget;
private slots:
    void initForm(QString fileName);
    void slot_demarcationTimer();
    void slot_eolsendCommandTimer();
    void slot_UDSFrameRecv(VCI_CAN_OBJ cAN_OBJ1);
    void slot_EOLInfo(QString respHead,QByteArray array);
    void slot_TemperatureTest(int id,QString ,QStringList list);
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
};

#endif // UDSFORM_H

#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
//#include "qlite/litewindow.h"
#include"dllTest/dlltest.h"
#include"DbcReader/dbcreader.h"
//#include"aggregation/aggregate.h"
#include"frmmain.h"

#include"demarcationdelegate.h"
#include"canconfigdelegate.h"
#include<QTimer>
#include"zhelpers.hpp"
#include<QStandardItemModel>
namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();
    void initForm();
    void initFile();
    //多线程
    void star();
    void recThread();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();
    void slot_commandTimer();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

private:
    Ui::MainForm *ui;
    DBCReader *dbc;
    frmMain *frm;
    QTimer * commandTimer;
    zmq::context_t context;
    zmq::socket_t subscriber;
    zmq::socket_t publisher;
    int m_exitStateThread2;

    DemarcationDelegate* demarcationForm ;
    CANConfigDelegate* canConfigForm ;
    QMutex mutex;
    QTimer* recvTimer;

    //计数，等同标定测试步骤
    int m_getResultCount;
    int m_sendFrameIndex;
    QString m_senddata,m_recvdata;

    //标定结果
    QStandardItemModel* model;
    QMap<int,QString> m_AlignErrorStates;
public slots:
    void changeStyle(const QString &qssFile);
    void slot_msgToPC(QString );
    void slot_RecvTest();
    void slot_stateInfoDataModel(QString str);
signals:
    void sig_msgToRadar(QString );
    void changeStyle();
    void unInitDevice();
    void initDevice();

};

#endif // MAINFORM_H

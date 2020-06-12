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

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();
    void slot_commandTimer();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainForm *ui;
    DBCReader *dbc;
    frmMain *frm;
    QTimer * commandTimer;
    zmq::context_t context;
    zmq::socket_t publisher;

    DemarcationDelegate* demarcationForm ;
    CANConfigDelegate* canConfigForm ;

public slots:
    void changeStyle(const QString &qssFile);

signals:
    void changeStyle();
    void unInitDevice();
    void initDevice();

};

#endif // MAINFORM_H

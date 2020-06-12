#include "frmdbtcpdata.h"
#include "ui_frmdbtcpdata.h"
#include "quiwidget.h"
#include "dbtcpdatathread.h"

frmDbTcpData::frmDbTcpData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDbTcpData)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmDbTcpData::~frmDbTcpData()
{
    delete ui;
}

void frmDbTcpData::initForm()
{
    ui->line1->setFixedHeight(1);
    ui->line2->setFixedHeight(1);
    ui->line3->setFixedHeight(1);

    maxCount = 50;
    currentCount = 0;

    connect(DbTcpDataThread::Instance(), SIGNAL(debug(QString)), this, SLOT(debug(QString)));
}

void frmDbTcpData::initConfig()
{
    ui->txtHostNameLocal->setText(App::HostNameLocal);
    ui->txtPortLocal->setText(QString::number(App::PortLocal));
    ui->txtDbNameLocal->setText(App::DbNameLocal);
    ui->txtUserNameLocal->setText(App::UserNameLocal);
    ui->txtUserPwdLocal->setText(App::UserPwdLocal);

    ui->txtHostNameNet->setText(App::HostNameNet);
    ui->txtPortNet->setText(QString::number(App::PortNet));
    ui->txtDbNameNet->setText(App::DbNameNet);
    ui->txtUserNameNet->setText(App::UserNameNet);
    ui->txtUserPwdNet->setText(App::UserPwdNet);

    connect(ui->txtHostNameLocal, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtPortLocal, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDbNameLocal, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserNameLocal, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserPwdLocal, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    connect(ui->txtHostNameNet, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtPortNet, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDbNameNet, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserNameNet, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserPwdNet, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmDbTcpData::saveConfig()
{
    App::HostNameLocal = ui->txtHostNameLocal->text();
    App::PortLocal = ui->txtPortLocal->text().toInt();
    App::DbNameLocal = ui->txtDbNameLocal->text();
    App::UserNameLocal = ui->txtUserNameLocal->text();
    App::UserPwdLocal = ui->txtUserPwdLocal->text();

    App::HostNameNet = ui->txtHostNameNet->text();
    App::PortNet = ui->txtPortNet->text().toInt();
    App::DbNameNet = ui->txtDbNameNet->text();
    App::UserNameNet = ui->txtUserNameNet->text();
    App::UserPwdNet = ui->txtUserPwdNet->text();

    App::writeConfig();
}

void frmDbTcpData::test()
{
    static int deviceLogID = 1;
    int count = 20;

    //启动数据库事务
    QSqlDatabase::database("dbLocal").transaction();

    for (int i = 0; i < count; i++) {
        int temp1 = rand() % 50;
        int temp2 = rand() % 10;
        int temp3 = rand() % 80;
        int temp4 = rand() % 10;
        double powerValue = rand() % 5;

        if (temp1 < 10) {
            temp1 += 10;
        }

        if (temp3 < 10) {
            temp3 += 10;
        }

        if (powerValue == 0) {
            powerValue = 1;
        }

        double tempValue = (double)temp1 + (double)temp2 / 10;
        double dampValue = (double)temp3 + (double)temp4 / 10;

        QString sql = QString("insert into DeviceLog(DeviceLogID,DeviceID,DeviceName,DeviceAddr,GroupName,TempValue,DampValue,PowerValue,SaveTime,Type)"
                              " values('%1','%4','WIFI端口-%4','%4','默认区域','%2','%3','%4','%5','运行记录')")
                      .arg(deviceLogID).arg(tempValue).arg(dampValue).arg(powerValue).arg(DATETIME);
        DbTcpDataThread::Instance()->appendSqlLocal(sql);

        deviceLogID++;
    }

    //提交数据库事务
    bool ok = QSqlDatabase::database("dbLocal").commit();
    if (!ok) {
        QSqlDatabase::database("dbLocal").rollback();
    }

    debug(QString("随机插入 %1 条数据 %2").arg(count).arg(ok ? "成功" : "失败"));
}

void frmDbTcpData::debug(const QString &msg)
{
    if (currentCount >= maxCount) {
        ui->txtMain->clear();
        currentCount = 0;
    }

    if (msg.startsWith("重连") || msg.startsWith("提交事务失败")) {
        qDebug() << DATETIME << msg;
    }

    ui->txtMain->append(QString("[%1] %2").arg(DATETIME).arg(msg));
    currentCount++;
}

void frmDbTcpData::on_btnStart_clicked()
{
    ui->frame->setEnabled(false);

    if (App::DbNameLocal.endsWith(".db")) {
        DbTcpDataThread::Instance()->setConnInfoLocal(DbTcpDataThread::DbType_Sqlite, "dbLocal", App::HostNameLocal, App::PortLocal,
                QUIHelper::appPath() + App::DbNameLocal, App::UserNameLocal, App::UserPwdLocal);
    } else {
        DbTcpDataThread::Instance()->setConnInfoLocal(DbTcpDataThread::DbType_MySql, "dbLocal", App::HostNameLocal, App::PortLocal,
                App::DbNameLocal, App::UserNameLocal, App::UserPwdLocal);
    }

    DbTcpDataThread::Instance()->setConnInfoNet(DbTcpDataThread::DbType_MySql, "dbNet", App::HostNameNet, App::PortNet,
            App::DbNameNet, App::UserNameNet, App::UserPwdNet);

    bool dbOkLocal = DbTcpDataThread::Instance()->openDbLocal();
    debug(QString("打开本地数据库%1").arg(dbOkLocal ? "成功" : "失败"));

    bool dbOkNet = DbTcpDataThread::Instance()->openDbNet();
    debug(QString("打开远程数据库%1").arg(dbOkNet ? "成功" : "失败"));

    DbTcpDataThread::Instance()->start();

    //启动定时器模拟设备数据
    QTimer *timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(test()));
    timer->start();
}

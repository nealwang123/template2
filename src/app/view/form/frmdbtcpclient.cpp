#include "frmdbtcpclient.h"
#include "ui_frmdbtcpclient.h"
#include "quiwidget.h"
#include "dbtcpclientthread.h"

frmDbTcpClient::frmDbTcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDbTcpClient)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();

}

frmDbTcpClient::~frmDbTcpClient()
{
    delete ui;
}

void frmDbTcpClient::initForm()
{
    maxCount = 50;
    currentCount = 0;

    connect(DbTcpClientThread::Instance(), SIGNAL(debug(QString)), this, SLOT(debug(QString)));
}

void frmDbTcpClient::initConfig()
{
    ui->txtDeviceCountClient->setText(QString::number(App::DeviceCountClient));
    ui->txtHostNameClient->setText(App::HostNameClient);
    ui->txtPortClient->setText(QString::number(App::PortClient));
    ui->txtDbNameClient->setText(App::DbNameClient);
    ui->txtUserNameClient->setText(App::UserNameClient);
    ui->txtUserPwdClient->setText(App::UserPwdClient);

    connect(ui->txtDeviceCountClient, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtHostNameClient, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtPortClient, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDbNameClient, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserNameClient, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserPwdClient, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmDbTcpClient::saveConfig()
{
    App::DeviceCountClient = ui->txtDeviceCountClient->text().toInt();
    App::HostNameClient = ui->txtHostNameClient->text();
    App::PortClient = ui->txtPortClient->text().toInt();
    App::DbNameClient = ui->txtDbNameClient->text();
    App::UserNameClient = ui->txtUserNameClient->text();
    App::UserPwdClient = ui->txtUserPwdClient->text();

    App::writeConfig();
}

void frmDbTcpClient::test()
{
    int deviceCount = ui->txtDeviceCountClient->text().toInt();
    QString strDate = DATETIME;
    int logCountAll = 100;

    //随机产生温湿度数据
    for (int i = 1; i <= deviceCount; i++) {
        int temp1 = rand() % 50;
        int temp2 = rand() % 10;
        int temp3 = rand() % 80;
        int temp4 = rand() % 10;
        double powerValue = rand() % 5;
        int logCountNoRead = rand() % 100;

        if (temp1 < 10) {
            temp1 += 10;
        }

        if (temp3 < 10) {
            temp3 += 10;
        }

        double tempValue = (double)temp1 + (double)temp2 / 10;
        double dampValue = (double)temp3 + (double)temp4 / 10;

        QString sql = QString("update DeviceLogTemp set TempValue='%1',DampValue='%2',PowerValue='%3',SaveTime='%4',LogCountAll='%5',LogCountNoRead='%6' where DeviceID='%7'")
                      .arg(tempValue).arg(dampValue).arg(powerValue).arg(strDate).arg(logCountAll).arg(logCountNoRead).arg(i);
        DbTcpClientThread::Instance()->append(sql);
    }
}

void frmDbTcpClient::debug(const QString &msg)
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

void frmDbTcpClient::on_btnStart_clicked()
{
    ui->frame->setEnabled(false);
    qApp->processEvents();

    DbTcpClientThread::Instance()->setConnInfo(DbTcpClientThread::DbType_MySql, "dbClient", App::HostNameClient, App::PortClient,
            App::DbNameClient, App::UserNameClient, App::UserPwdClient);

    QList<int> listDeviceID;    //设备编号队列
    QStringList listDeviceName; //设备名称队列
    QList<int> listDeviceAddr;  //设备地址队列
    QStringList listGroupName;  //设备分组队列

    for (int i = 1; i <= App::DeviceCountClient; i++) {
        listDeviceID.append(i);
        listDeviceName.append(QString("串口设备%1").arg(i));
        listDeviceAddr.append(i);
        listGroupName.append("默认区域");
    }

    DbTcpClientThread::Instance()->setListDeviceID(listDeviceID);
    DbTcpClientThread::Instance()->setListDeviceName(listDeviceName);
    DbTcpClientThread::Instance()->setListDeviceAddr(listDeviceAddr);
    DbTcpClientThread::Instance()->setListGroupName(listGroupName);

    bool dbOk = DbTcpClientThread::Instance()->openDb();
    debug(QString("打开数据库%1").arg(dbOk ? "成功" : "失败"));

    int interval = ui->txtIntervalClient->text().toInt();
    DbTcpClientThread::Instance()->setCheckInterval((interval / 1000) * 10);
    DbTcpClientThread::Instance()->initDb();
    DbTcpClientThread::Instance()->start();

    //启动定时器模拟设备数据
    QTimer *timer = new QTimer(this);
    timer->setInterval(interval);
    connect(timer, SIGNAL(timeout()), this, SLOT(test()));
    timer->start();
}

void frmDbTcpClient::on_txtDeviceCountClient_textChanged(const QString &arg1)
{
    int interval = arg1.toInt() * 50;
    interval = interval < 1000 ? 1000 : interval;
    ui->txtIntervalClient->setText(QString::number(interval));
}

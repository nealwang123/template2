#include "frmdbtcpserver.h"
#include "ui_frmdbtcpserver.h"
#include "quiwidget.h"
#include "dbtcpserverthread.h"
#include "frmdevicepanel.h"

frmDbTcpServer::frmDbTcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDbTcpServer)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmDbTcpServer::~frmDbTcpServer()
{
    foreach (DbTcpServerThread *server, servers) {
        server->stop();
        server->quit();
        server->wait();
        server->deleteLater();
    }

    delete ui;
}

void frmDbTcpServer::initForm()
{
    maxCount = 20;
    currentCount = 0;

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setColumnWidth(0, 175);
    ui->treeWidget->header()->setVisible(true);
    ui->treeWidget->setHeaderLabel(" 温湿度设备列表");
    ui->treeWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void frmDbTcpServer::initConfig()
{
    ui->txtDeviceCountServer->setText(QString::number(App::DeviceCountServer));
    ui->txtHostNameServer->setText(App::HostNameServer);
    ui->txtPortServer->setText(QString::number(App::PortServer));
    ui->txtDbNameServer->setText(App::DbNameServer);
    ui->txtUserNameServer->setText(App::UserNameServer);
    ui->txtUserPwdServer->setText(App::UserPwdServer);

    connect(ui->txtDeviceCountServer, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtHostNameServer, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtPortServer, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDbNameServer, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserNameServer, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserPwdServer, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmDbTcpServer::saveConfig()
{
    App::DeviceCountServer = ui->txtDeviceCountServer->text().toInt();
    App::HostNameServer = ui->txtHostNameServer->text();
    App::PortServer = ui->txtPortServer->text().toInt();
    App::DbNameServer = ui->txtDbNameServer->text();
    App::UserNameServer = ui->txtUserNameServer->text();
    App::UserPwdServer = ui->txtUserPwdServer->text();

    App::writeConfig();
}

void frmDbTcpServer::loadServer()
{
    QString tableName = "DeviceLogTemp";
    QStringList columnName;
    columnName << "DeviceLogID" << "DeviceID" << "DeviceName" << "DeviceAddr" << "GroupName"
               << "TempValue" << "DampValue" << "PowerValue" << "SaveTime" << "LogCountAll" << "LogCountNoRead" << "Type";
    QStringList keyName;
    keyName << "DeviceName" << "GroupName";

    //读取配置文件,根据配置文件对应的远程数据库信息进行实例化采集数据线程
    QFile file(qApp->applicationDirPath() + "/db.txt");

    if (file.open(QFile::ReadOnly)) {
        int id = 1;
        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.trimmed();

            if (!line.isEmpty()) {
                QStringList list = line.split(",");
                if (list.count() != 5) {
                    continue;
                }

                QString name = list.at(2);//QString("dbServer%1").arg(id);
                DbTcpServerThread *server = new DbTcpServerThread(this);
                server->setObjectName(name);
                server->setProperty("IP", list.at(0));
                server->setProperty("DbName", list.at(2));
                server->setConnInfo(DbTcpServerThread::DbType_MySql, name, list.at(0), list.at(1).toInt(), list.at(2), list.at(3), list.at(4));
                server->setTableName(tableName);
                server->setColumnName(columnName);
                server->setKeyName(keyName);

                connect(server, SIGNAL(debug(QString)), this, SLOT(debug(QString)));
                connect(server, SIGNAL(dataReceive(QStringList, QStringList)),
                        this, SLOT(dataReceive(QStringList, QStringList)));

                debug(QString("打开数据库 %1 %2").arg(list.at(2)).arg(server->openDb() ? "成功" : "失败"));
                server->start();

                servers.append(server);
                id++;
            }
        }

        file.close();
    }
}

void frmDbTcpServer::loadData()
{
    int id = 1;
    ui->treeWidget->clear();

    //初始化面板
    QList<QWidget *> widgets;

    //读取远程设备信息,加载设备面板
    foreach (DbTcpServerThread *server, servers) {
        //添加父节点
        QTreeWidgetItem *itemGroup = new QTreeWidgetItem
                (ui->treeWidget, QStringList(server->objectName()));
        itemGroup->setIcon(0, QIcon(":/imageTest/tree_parent.png"));

        QStringList resultData = server->getData("select DeviceName,GroupName from DeviceInfo");
        foreach (QString str, resultData) {
            QStringList list = str.split("|");
            frmDevicePanel *frm = new frmDevicePanel;
            frm->setDeviceID(id);
            frm->setDeviceName(list.at(0));
            frm->setGroupName(list.at(1));
            frm->setDeviceOnLine(true);
            frm->setFixedHeight(frm->sizeHint().height() + 10);
            frm->setFixedWidth(frm->sizeHint().width() + 10);
            frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
            widgets.append(frm);

            deviceNames.append(list.at(0));
            groupNames.append(list.at(1));
            devices.append(frm);
            id++;

            //添加子节点
            QString text = QString("%1").arg(list.at(0));
            QTreeWidgetItem *itemDevice = new QTreeWidgetItem(itemGroup, QStringList(text));
            itemDevice->setIcon(0, QIcon(":/imageTest/tree_child.png"));
        }
    }

    ui->widgetPanel->setAutoWidth(false);
    ui->widgetPanel->setWidget(widgets, App::DeviceColumn);
    ui->widgetPanel->setMargin(3);
    ui->widgetPanel->setSpace(3);

    ui->treeWidget->expandAll();
}

void frmDbTcpServer::changeStyle()
{
    foreach (frmDevicePanel *device, devices) {
        bool tempAlarm = (device->getTempUpperAlarm() || device->getTempLimitAlarm());
        bool dampAlarm = (device->getDampUpperAlarm() || device->getDampLimitAlarm());
        device->changeStatus("温度", tempAlarm);
        device->changeStatus("湿度", dampAlarm);
        ui->widgetPanel->setStyleSheet(QString("QFrame#boxPanel{border-width:0px;background:%1;}"
                                               "QFrame#gboxDevicePanel,QFrame#gboxDeviceTitle{padding:3px;}")
                                       .arg(QUIConfig::PanelColor));
    }
}

void frmDbTcpServer::debug(const QString &msg)
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

void frmDbTcpServer::dataReceive(const QStringList &keyData, const QStringList &resultData)
{
    int count = keyData.count();
    int deviceCount = groupNames.count();

    for (int i = 0; i < count; i++) {
        QStringList list = keyData.at(i).split("|");
        for (int j = 0; j < deviceCount; j++) {
            if (deviceNames.at(j) == list.at(0) && groupNames.at(j) == list.at(1)) {
                QStringList list = resultData.at(i).split("|");
                frmDevicePanel *frm = devices.at(j);
                frm->setCurrentTempDamp(list.at(5).toDouble(), list.at(6).toDouble());
                frm->setPowerValue(list.at(7).toInt());
                frm->setCount(list.at(9).toInt(), list.at(10).toInt());
                qApp->processEvents();
                break;
            }
        }
    }
}

void frmDbTcpServer::on_btnStart_clicked()
{
    ui->treeWidget->clear();

    //初始化面板
    QList<QWidget *> widgets;    

    //添加父节点
    QTreeWidgetItem *itemGroup = new QTreeWidgetItem
            (ui->treeWidget, QStringList("本地设备"));
    itemGroup->setIcon(0, QIcon(":/imageTest/tree_parent.png"));

    for (int i = 1; i <= App::DeviceCountServer; i++) {
        frmDevicePanel *frm = new frmDevicePanel;
        frm->setDeviceID(i);
        frm->setDeviceName(QString("串口设备%1").arg(i));
        frm->setDeviceAddr(i);
        frm->setGroupName("默认区域");
        frm->setDeviceOnLine(true);
        frm->setFixedHeight(frm->sizeHint().height() + 10);
        frm->setMinimumWidth(frm->sizeHint().width() + 10);
        frm->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        widgets.append(frm);

        deviceNames.append(QString("串口设备%1").arg(i));
        groupNames.append("默认区域");
        devices.append(frm);

        //添加子节点
        QString text = QString("%1").arg(QString("串口设备%1").arg(i));
        QTreeWidgetItem *itemDevice = new QTreeWidgetItem(itemGroup, QStringList(text));
        itemDevice->setIcon(0, QIcon(":/imageTest/tree_child.png"));
    }

    ui->widgetPanel->setAutoWidth(true);
    ui->widgetPanel->setWidget(widgets, App::DeviceColumn);
    ui->widgetPanel->setMargin(3);
    ui->widgetPanel->setSpace(3);

    ui->treeWidget->expandAll();
    ui->frame->setEnabled(false);
    qApp->processEvents();

    QString tableName = "DeviceLogTemp";
    QStringList columnName;
    columnName << "DeviceLogID" << "DeviceID" << "DeviceName" << "DeviceAddr" << "GroupName"
               << "TempValue" << "DampValue" << "PowerValue" << "SaveTime" << "LogCountAll" << "LogCountNoRead" << "Type";
    QStringList keyName;
    keyName << "DeviceName" << "GroupName";

    connect(DbTcpServerThread::Instance(), SIGNAL(debug(QString)), this, SLOT(debug(QString)));
    connect(DbTcpServerThread::Instance(), SIGNAL(dataReceive(QStringList, QStringList)),
            this, SLOT(dataReceive(QStringList, QStringList)));

    DbTcpServerThread::Instance()->setConnInfo(DbTcpServerThread::DbType_MySql, "dbServer", App::HostNameServer, App::PortServer,
                                               App::DbNameServer, App::UserNameServer, App::UserPwdServer);
    DbTcpServerThread::Instance()->setTableName(tableName);
    DbTcpServerThread::Instance()->setColumnName(columnName);
    DbTcpServerThread::Instance()->setKeyName(keyName);

    bool dbOk = DbTcpServerThread::Instance()->openDb();
    debug(QString("打开数据库%1").arg(dbOk ? "成功" : "失败"));

    int interval = ui->txtIntervalServer->text().toInt();
    DbTcpServerThread::Instance()->setReadInterval(interval);
    DbTcpServerThread::Instance()->setCheckInterval((interval / 1000) * 10);
    DbTcpServerThread::Instance()->start();
}

void frmDbTcpServer::on_btnLoad_clicked()
{
    QTimer::singleShot(0, this, SLOT(loadServer()));
    QTimer::singleShot(2000, this, SLOT(loadData()));
    ui->frame->setEnabled(false);
}

void frmDbTcpServer::on_txtDeviceCountServer_textChanged(const QString &arg1)
{
    int interval = arg1.toInt() * 50;
    interval = interval < 1000 ? 1000 : interval;
    ui->txtIntervalServer->setText(QString::number(interval));
}

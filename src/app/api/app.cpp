#include "app.h"
#include "quiwidget.h"

#ifdef __arm__
//QString App::FontName = "Droid Sans Fallback";
//int App::FontSize = 13;
QString App::FontName = "WenQuanYi Micro Hei";
int App::FontSize = 18;
#else
QString App::FontName = "Microsoft Yahei";
int App::FontSize = 9;
#endif

QString App::ConfigFile = "config.ini";

QString App::StyleName = ":/qss/psblack.css";
QString App::LocalDBType = "Sqlite";
QString App::LocalDBIP = "127.0.0.1";
int App::LocalDBPort = 3306;
QString App::LocalDBName = "/tcms.db";
QString App::LocalUserName = "root";
QString App::LocalUserPwd = "root";

double App::PlotMinValue = 0.0;
double App::PlotMaxValue = 100.0;
double App::PlotDefaultValue = 0.0;
double App::TempUpperBuffer = 0.1;
double App::TempLimitBuffer = 0.1;
double App::DampUpperBuffer = 1.0;
double App::DampLimitBuffer = 1.0;

int App::DeviceCountClient = 50;
QString App::DbNameClient = "tcms";
QString App::HostNameClient = "127.0.0.1";
int App::PortClient = 3306;
QString App::UserNameClient = "root";
QString App::UserPwdClient = "root";

int App::DeviceCountServer = 50;
QString App::DbNameServer = "tcms";
QString App::HostNameServer = "127.0.0.1";
int App::PortServer = 3306;
QString App::UserNameServer = "root";
QString App::UserPwdServer = "root";
int App::DeviceColumn = 6;

QString App::DbNameLocal = "tcms";
QString App::HostNameLocal = "127.0.0.1";
int App::PortLocal = 3306;
QString App::UserNameLocal = "root";
QString App::UserPwdLocal = "root";

QString App::DbNameNet = "tcms";
QString App::HostNameNet = "127.0.0.1";
int App::PortNet = 3306;
QString App::UserNameNet = "root";
QString App::UserPwdNet = "root";

QString App::DbNameClean = "tcms";
QString App::HostNameClean = "127.0.0.1";
int App::PortClean = 3306;
QString App::UserNameClean = "root";
QString App::UserPwdClean = "root";
int App::CleanCount = 100;
int App::CleanInterval = 5;

QString App::DbNameData = "tcms";
QString App::HostNameData = "127.0.0.1";
int App::PortData = 3306;
QString App::UserNameData = "root";
QString App::UserPwdData = "root";
int App::DataCount = 100;
int App::DataInterval = 1000;

QString App::SqliteDBName = "/tcms.db";
QString App::SqliteSql = "/tcms.sql";
QString App::MysqlDBIP = "127.0.0.1";
int App::MysqlDBPort = 3306;
QString App::MysqlDBName = "tcms";
QString App::MysqlUserName = "root";
QString App::MysqlUserPwd = "root";
QString App::MysqlSql = "/tcms.sql";

void App::readConfig()
{
    if (!checkConfig()) {
        return;
    }
    qDebug()<<"App::ConfigFile"<<App::ConfigFile;
    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("AppConfig");
    App::StyleName = set.value("StyleName").toString();
    App::LocalDBType = set.value("LocalDBType").toString();
    App::LocalDBIP = set.value("LocalDBIP").toString();
    App::LocalDBPort = set.value("LocalDBPort").toInt();
    App::LocalDBName = set.value("LocalDBName").toString();
    App::LocalUserName = set.value("LocalUserName").toString();
    App::LocalUserPwd = set.value("LocalUserPwd").toString();
    App::LocalUserPwd = QUIHelper::getXorEncryptDecrypt(App::LocalUserPwd, 255);
    set.endGroup();

    set.beginGroup("ValueConfig");
    App::PlotMinValue = set.value("PlotMinValue").toDouble();
    App::PlotMaxValue = set.value("PlotMaxValue").toDouble();
    App::PlotDefaultValue = set.value("PlotDefaultValue").toDouble();
    App::TempUpperBuffer = set.value("TempUpperBuffer").toDouble();
    App::TempLimitBuffer = set.value("TempLimitBuffer").toDouble();
    App::DampUpperBuffer = set.value("DampUpperBuffer").toDouble();
    App::DampLimitBuffer = set.value("DampLimitBuffer").toDouble();
    set.endGroup();

    set.beginGroup("ClientDbConfig");
    App::DeviceCountClient = set.value("DeviceCountClient").toInt();
    App::DbNameClient = set.value("DbNameClient").toString();
    App::HostNameClient = set.value("HostNameClient").toString();
    App::PortClient = set.value("PortClient").toInt();
    App::UserNameClient = set.value("UserNameClient").toString();
    App::UserPwdClient = set.value("UserPwdClient").toString();
    set.endGroup();

    set.beginGroup("ServerDbConfig");
    App::DeviceCountServer = set.value("DeviceCountServer").toInt();
    App::DbNameServer = set.value("DbNameServer").toString();
    App::HostNameServer = set.value("HostNameServer").toString();
    App::PortServer = set.value("PortServer").toInt();
    App::UserNameServer = set.value("UserNameServer").toString();
    App::UserPwdServer = set.value("UserPwdServer").toString();
    App::DeviceColumn = set.value("DeviceColumn").toInt();
    set.endGroup();

    set.beginGroup("LocalDbConfig");
    App::DbNameLocal = set.value("DbNameLocal").toString();
    App::HostNameLocal = set.value("HostNameLocal").toString();
    App::PortLocal = set.value("PortLocal").toInt();
    App::UserNameLocal = set.value("UserNameLocal").toString();
    App::UserPwdLocal = set.value("UserPwdLocal").toString();
    set.endGroup();

    set.beginGroup("NetDbConfig");
    App::DbNameNet = set.value("DbNameNet").toString();
    App::HostNameNet = set.value("HostNameNet").toString();
    App::PortNet = set.value("PortNet").toInt();
    App::UserNameNet = set.value("UserNameNet").toString();
    App::UserPwdNet = set.value("UserPwdNet").toString();
    set.endGroup();

    set.beginGroup("CleanDbConfig");
    App::DbNameClean = set.value("DbNameClean").toString();
    App::HostNameClean = set.value("HostNameClean").toString();
    App::PortClean = set.value("PortClean").toInt();
    App::UserNameClean = set.value("UserNameClean").toString();
    App::UserPwdClean = set.value("UserPwdClean").toString();
    App::CleanCount = set.value("CleanCount").toInt();
    App::CleanInterval = set.value("CleanInterval").toInt();
    set.endGroup();

    set.beginGroup("DataDbConfig");
    App::DbNameData = set.value("DbNameData").toString();
    App::HostNameData = set.value("HostNameData").toString();
    App::PortData = set.value("PortData").toInt();
    App::UserNameData = set.value("UserNameData").toString();
    App::UserPwdData = set.value("UserPwdData").toString();
    App::DataCount = set.value("DataCount").toInt();
    App::DataInterval = set.value("DataInterval").toInt();
    set.endGroup();
    qDebug()<<"App::DbNameData"<<App::DbNameData;
    set.beginGroup("DbSqlConfig");
    App::SqliteDBName = set.value("SqliteDBName").toString();
    App::SqliteSql = set.value("SqliteSql").toString();
    App::MysqlDBIP = set.value("MysqlDBIP").toString();
    App::MysqlDBPort = set.value("MysqlDBPort").toInt();
    App::MysqlDBName = set.value("MysqlDBName").toString();
    App::MysqlUserName = set.value("MysqlUserName").toString();
    App::MysqlUserPwd = set.value("MysqlUserPwd").toString();
    App::MysqlSql = set.value("MysqlSql").toString();
    set.endGroup();
}

void App::writeConfig()
{
    QSettings set(App::ConfigFile, QSettings::IniFormat);

    set.beginGroup("AppConfig");
    set.setValue("StyleName", App::StyleName);
    set.setValue("LocalDBType", App::LocalDBType);
    set.setValue("LocalDBIP", App::LocalDBIP);
    set.setValue("LocalDBPort", App::LocalDBPort);
    set.setValue("LocalDBName", App::LocalDBName);
    set.setValue("LocalUserName", App::LocalUserName);
    set.setValue("LocalUserPwd", QUIHelper::getXorEncryptDecrypt(App::LocalUserPwd, 255));
    set.endGroup();

    set.beginGroup("ValueConfig");
    set.setValue("PlotMinValue", App::PlotMinValue);
    set.setValue("PlotMaxValue", App::PlotMaxValue);
    set.setValue("PlotDefaultValue", App::PlotDefaultValue);
    set.setValue("TempUpperBuffer", App::TempUpperBuffer);
    set.setValue("TempLimitBuffer", App::TempLimitBuffer);
    set.setValue("DampUpperBuffer", App::DampUpperBuffer);
    set.setValue("DampLimitBuffer", App::DampLimitBuffer);
    set.endGroup();

    set.beginGroup("ClientDbConfig");
    set.setValue("DeviceCountClient", App::DeviceCountClient);
    set.setValue("DbNameClient", App::DbNameClient);
    set.setValue("HostNameClient", App::HostNameClient);
    set.setValue("PortClient", App::PortClient);
    set.setValue("UserNameClient", App::UserNameClient);
    set.setValue("UserPwdClient", App::UserPwdClient);
    set.endGroup();

    set.beginGroup("ServerDbConfig");
    set.setValue("DeviceCountServer", App::DeviceCountServer);
    set.setValue("DbNameServer", App::DbNameServer);
    set.setValue("HostNameServer", App::HostNameServer);
    set.setValue("PortServer", App::PortServer);
    set.setValue("UserNameServer", App::UserNameServer);
    set.setValue("UserPwdServer", App::UserPwdServer);
    set.setValue("DeviceColumn", App::DeviceColumn);
    set.endGroup();

    set.beginGroup("LocalDbConfig");
    set.setValue("DbNameLocal", App::DbNameLocal);
    set.setValue("HostNameLocal", App::HostNameLocal);
    set.setValue("PortLocal", App::PortLocal);
    set.setValue("UserNameLocal", App::UserNameLocal);
    set.setValue("UserPwdLocal", App::UserPwdLocal);
    set.endGroup();

    set.beginGroup("NetDbConfig");
    set.setValue("DbNameNet", App::DbNameNet);
    set.setValue("HostNameNet", App::HostNameNet);
    set.setValue("PortNet", App::PortNet);
    set.setValue("UserNameNet", App::UserNameNet);
    set.setValue("UserPwdNet", App::UserPwdNet);
    set.endGroup();

    set.beginGroup("CleanDbConfig");
    set.setValue("DbNameClean", App::DbNameClean);
    set.setValue("HostNameClean", App::HostNameClean);
    set.setValue("PortClean", App::PortClean);
    set.setValue("UserNameClean", App::UserNameClean);
    set.setValue("UserPwdClean", App::UserPwdClean);
    set.setValue("CleanCount", App::CleanCount);
    set.setValue("CleanInterval", App::CleanInterval);
    set.endGroup();

    set.beginGroup("DataDbConfig");
    set.setValue("DbNameData", App::DbNameData);
    set.setValue("HostNameData", App::HostNameData);
    set.setValue("PortData", App::PortData);
    set.setValue("UserNameData", App::UserNameData);
    set.setValue("UserPwdData", App::UserPwdData);
    set.setValue("DataCount", App::DataCount);
    set.setValue("DataInterval", App::DataInterval);
    set.endGroup();

    set.beginGroup("DbSqlConfig");
    set.setValue("SqliteDBName", App::SqliteDBName);
    set.setValue("SqliteSql", App::SqliteSql);
    set.setValue("MysqlDBIP", App::MysqlDBIP);
    set.setValue("MysqlDBPort", App::MysqlDBPort);
    set.setValue("MysqlDBName", App::MysqlDBName);
    set.setValue("MysqlUserName", App::MysqlUserName);
    set.setValue("MysqlUserPwd", App::MysqlUserPwd);
    set.setValue("MysqlSql", App::MysqlSql);
    set.endGroup();
}

void App::newConfig()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
#endif

    writeConfig();
}

bool App::checkConfig()
{
    //如果配置文件大小为0,则以初始值继续运行,并生成配置文件
    QFile file(App::ConfigFile);

    if (file.size() == 0) {
        qDebug()<<"new config file:file.size()==0";
        newConfig();
        return false;
    }

    //如果配置文件不完整,则以初始值继续运行,并生成配置文件
    if (file.open(QFile::ReadOnly)) {
        bool ok = true;

        while (!file.atEnd()) {
            QString line = file.readLine();
            line = line.replace("\r", "");
            line = line.replace("\n", "");
            QStringList list = line.split("=");

            if (list.count() == 2) {
                if (list.at(1) == "") {
                    ok = false;
                    break;
                }
            }
        }

        if (!ok) {
            qDebug()<<"new config file:file not complate!";
            newConfig();
            return false;
        }
    } else {
        qDebug()<<"new config file:file.open() failed!";
        newConfig();
        return false;
    }

    return true;
}

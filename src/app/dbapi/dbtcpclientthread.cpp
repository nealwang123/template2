#include "dbtcpclientthread.h"

QScopedPointer<DbTcpClientThread> DbTcpClientThread::self;
DbTcpClientThread *DbTcpClientThread::Instance()
{
    if (self.isNull()) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbTcpClientThread);
        }
    }

    return self.data();
}

DbTcpClientThread::DbTcpClientThread(QObject *parent) : QThread(parent)
{
    stopped = false;

    dbOkNet = false;
    lastCheckTimeNet = QDateTime::currentDateTime();
    checkInterval = 30;

    dbTypeNet = DbType_MySql;
    connNameNet = "dbClient";
    hostNameNet = "127.0.0.1";
    portNet = 3306;
    dbNameNet = "tcms_liu_mysql";
    userNameNet = "root";
    userPwdNet = "root";

    maxCount = 1000;
    listSql.clear();
}

DbTcpClientThread::~DbTcpClientThread()
{
    this->stop();
    this->wait(1000);
}

void DbTcpClientThread::run()
{
    while (!stopped) {
        //定时执行一次校验数据库连接是否正常
        QDateTime now = QDateTime::currentDateTime();

        if (lastCheckTimeNet.secsTo(now) >= checkInterval) {
            checkDb();
            lastCheckTimeNet = now;
            continue;
        }

        //如果数据库连接正常则处理数据,不正常则重连数据库
        if (dbOkNet) {
            checkData();
            msleep(100);
        } else {
            openDb();
            emit debug(QString("重连数据库%1").arg(dbOkNet ? "成功" : "失败"));
            msleep(3000);
        }
    }

    stopped = false;
}

bool DbTcpClientThread::openDb()
{
    //可以自行增加其他数据库的支持
    if (dbTypeNet == DbType_MySql) {
        dbConnNet = QSqlDatabase::addDatabase("QMYSQL", connNameNet);
        dbConnNet.setHostName(hostNameNet);
        dbConnNet.setPort(portNet);
        dbConnNet.setDatabaseName(dbNameNet);
        dbConnNet.setUserName(userNameNet);
        dbConnNet.setPassword(userPwdNet);
    }

    dbOkNet = dbConnNet.open();
    return dbOkNet;
}

bool DbTcpClientThread::checkDb()
{
    QDateTime dtStart = QDateTime::currentDateTime();

    QString sql = "select 1";
    QSqlQuery query(dbConnNet);
    dbOkNet = query.exec(sql);

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);
    emit debug(QString("检查数据库连接(共 %1 条/用时 %2 秒)").arg(1).arg(QString::number(ms / 1000, 'f', 3)));

    return dbOkNet;
}

void DbTcpClientThread::closeDb()
{
    dbConnNet.close();
    QSqlDatabase::removeDatabase(connNameNet);
    dbOkNet = false;
    emit debug("关闭数据库");
}

void DbTcpClientThread::initDb()
{
    //首先判断对应设备是否已经存在数据,不存在则新增
    int count = listDeviceID.count();

    if (count > 0) {
        if (dbOkNet) {
            QDateTime dtStart = QDateTime::currentDateTime();

            //首先清空该表所有数据
            QString sql = "delete from DeviceLogTemp";
            QSqlQuery query(dbConnNet);
            query.exec(sql);

            //新建初始值数据
            for (int i = 0; i < count; i++) {
                int deviceID = listDeviceID.at(i);
                QString deviceName = listDeviceName.at(i);
                int deviceAddr = listDeviceAddr.at(i);
                QString groupName = listGroupName.at(i);

                sql = "insert into DeviceLogTemp(DeviceLogID,DeviceID,DeviceName,DeviceAddr,";
                sql += "GroupName,TempValue,DampValue,PowerValue,SaveTime,LogCountAll,LogCountNoRead,Type) values('";
                sql += QString::number(i + 1) + "','";
                sql += QString::number(deviceID) + "','";
                sql += deviceName + "','";
                sql += QString::number(deviceAddr) + "','";
                sql += groupName + "','";
                sql += QString::number(20.0) + "','";
                sql += QString::number(50.0) + "','";
                sql += QString::number(1) + "','";
                sql += QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "','";
                sql += QString::number(0) + "','";
                sql += QString::number(0) + "','";
                sql += QString("运行记录") + "')";

                query.clear();
                query.exec(sql);
            }

            QDateTime dtEnd = QDateTime::currentDateTime();
            double ms = dtStart.msecsTo(dtEnd);
            emit debug(QString("远程清空并新建数据(共 %1 条/用时 %2 秒)").arg(count).arg(QString::number(ms / 1000, 'f', 3)));
        }
    }
}

void DbTcpClientThread::stop()
{
    stopped = true;
}

void DbTcpClientThread::append(const QString &sql)
{
    mutex.lock();

    //只有不一样的sql语句才需要处理
    if (!listSql.contains(sql)) {
        //如果达到最大数量,则移除最后一条,追加到末尾
        if (listSql.count() >= maxCount) {
            listSql.removeLast();
        }

        listSql.append(sql);
    }

    mutex.unlock();
}

void DbTcpClientThread::clear()
{
    mutex.lock();
    listSql.clear();
    mutex.unlock();
}

bool DbTcpClientThread::checkData()
{
    mutex.lock();
    QStringList sqls = listSql;
    mutex.unlock();

    int count = sqls.count();
    if (count == 0) {
        return false;
    }

    //取出队列中的所有sql语句,一次性事务提交到远程数据库
    if (dbOkNet) {
        QDateTime dtStart = QDateTime::currentDateTime();
        dbConnNet.transaction();

        //逐个执行sql语句
        foreach (QString sql, sqls) {
            QSqlQuery query(dbConnNet);
            query.exec(sql);
        }

        //提交成功后将sql语句队列清空
        //提交失败则需要回滚事务
        dbOkNet = dbConnNet.commit();

        if (dbOkNet) {
            clear();
            QDateTime dtEnd = QDateTime::currentDateTime();
            double ms = dtStart.msecsTo(dtEnd);
            emit debug(QString("远程实时更新数据(共 %1 条/用时 %2 秒)").arg(count).arg(QString::number(ms / 1000, 'f', 3)));
        } else {
            dbConnNet.rollback();
            emit debug(QString("提交事务失败: %1").arg(dbConnNet.lastError().text()));
        }
    }

    return dbOkNet;
}

void DbTcpClientThread::setConnInfo(DbType dbType, const QString &connName, const QString &hostName, int port,
                                    const QString &dbName, const QString &userName, const QString &userPwd)
{
    this->dbTypeNet = dbType;
    this->connNameNet = connName;
    this->hostNameNet = hostName;
    this->portNet = port;
    this->dbNameNet = dbName;
    this->userNameNet = userName;
    this->userPwdNet = userPwd;
}

void DbTcpClientThread::setCheckInterval(int checkInterval)
{
    if (checkInterval > 5 && this->checkInterval != checkInterval) {
        this->checkInterval = checkInterval;
    }
}

void DbTcpClientThread::setListDeviceID(const QList<int> &listDeviceID)
{
    this->listDeviceID = listDeviceID;
}

void DbTcpClientThread::setListDeviceName(const QStringList &listDeviceName)
{
    this->listDeviceName = listDeviceName;
}

void DbTcpClientThread::setListDeviceAddr(const QList<int> &listDeviceAddr)
{
    this->listDeviceAddr = listDeviceAddr;
}

void DbTcpClientThread::setListGroupName(const QStringList &listGroupName)
{
    this->listGroupName = listGroupName;
}

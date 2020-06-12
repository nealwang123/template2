#include "dbtcpserverthread.h"

QScopedPointer<DbTcpServerThread> DbTcpServerThread::self;
DbTcpServerThread *DbTcpServerThread::Instance()
{
    if (self.isNull()) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbTcpServerThread);
        }
    }

    return self.data();
}

DbTcpServerThread::DbTcpServerThread(QObject *parent) : QThread(parent)
{
    stopped = false;

    dbOkNet = false;
    lastCheckTimeNet = QDateTime::currentDateTime();
    checkInterval = 30;

    dbTypeNet = DbType_MySql;
    connNameNet = "dbServer";
    hostNameNet = "127.0.0.1";
    portNet = 3306;
    dbNameNet = "tcms_liu_mysql";
    userNameNet = "root";
    userPwdNet = "root";

    readInterval = 1000;    
    tableName = "DeviceLogTemp";
    columnName << "DeviceLogID" << "DeviceID" << "DeviceName" << "DeviceAddr" << "GroupName"
               << "TempValue" << "DampValue" << "PowerValue" << "SaveTime" << "LogCountAll" << "LogCountNoRead" << "Type";
    keyName << "DeviceName" << "GroupName";
    keyIndex << 2 << 4;
}

DbTcpServerThread::~DbTcpServerThread()
{
    this->stop();
    this->wait(1000);
}

void DbTcpServerThread::run()
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
            msleep(readInterval);
        } else {
            openDb();
            emit debug(QString("重连数据库%1").arg(dbOkNet ? "成功" : "失败"));
            msleep(3000);
        }
    }

    stopped = false;
}

bool DbTcpServerThread::openDb()
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

bool DbTcpServerThread::checkDb()
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

void DbTcpServerThread::closeDb()
{
    dbConnNet.close();
    QSqlDatabase::removeDatabase(connNameNet);
    dbOkNet = false;
    emit debug("关闭数据库");
}

void DbTcpServerThread::stop()
{
    stopped = true;
}

bool DbTcpServerThread::checkData()
{
    int count = columnName.count();
    if (count == 0 || tableName.isEmpty()) {
        return false;
    }

    if (dbOkNet) {
        QDateTime dtStart = QDateTime::currentDateTime();

        QString sql = QString("select %1 from %2").arg(columnName.join(",")).arg(tableName);
        QSqlQuery query(dbConnNet);
        dbOkNet = query.exec(sql);

        if (!dbOkNet) {
            QString msg = QString("执行查询出错 原因: %1").arg(query.lastError().text());
            emit debug(msg);
            return false;
        }

        int recordCount = query.record().count();

        //返回结果的列数必须和设定的列数完全一致
        if (recordCount == count) {
            int dataCount = 0;
            QStringList keyData;
            QStringList resultData;

            while (query.next()) {
                //将查询结果对应的关键字结果存入字符串链表,中间分隔符 |
                QStringList key;
                for (int i = 0; i < keyName.count(); i++) {
                    int index = keyIndex.at(i);
                    key.append(query.value(index).toString());
                }

                keyData.append(key.join("|"));

                //将所有查询结果的数据存入字符串链表,中间分隔符 |
                QStringList result;
                for (int i = 0; i < count; i++) {
                    result.append(query.value(i).toString());
                }

                resultData.append(result.join("|"));

                dataCount++;
            }

            emit dataReceive(keyData, resultData);

            QDateTime dtEnd = QDateTime::currentDateTime();
            double ms = dtStart.msecsTo(dtEnd);
            emit debug(QString("远程实时获取数据(共 %1 条/用时 %2 秒)").arg(dataCount).arg(QString::number(ms / 1000, 'f', 3)));
        } else {
            QString msg = QString("获取数据出错,字段数量有误: %1").arg(recordCount);
            emit debug(msg);
        }
    }

    return dbOkNet;
}

bool DbTcpServerThread::execSql(const QString &sql)
{
    if (dbOkNet) {
        QSqlQuery query(dbConnNet);
        dbOkNet = query.exec(sql);

        if (!dbOkNet) {
            QString msg = QString("执行查询出错 原因: %1").arg(query.lastError().text());
            emit debug(msg);
        }
    }

    return dbOkNet;
}

QStringList DbTcpServerThread::getData(const QString &sql)
{
    QStringList resultData;

    if (dbOkNet) {
        QSqlQuery query(dbConnNet);
        dbOkNet = query.exec(sql);

        //执行成功则将对应结果数据按照给定的字段顺序打包
        if (dbOkNet) {
            int recordCount = query.record().count();

            //至少有一个字段
            if (recordCount >= 1) {
                while (query.next()) {
                    QStringList result;
                    for (int i = 0; i < recordCount; i++) {
                        result.append(query.value(i).toString());
                    }

                    resultData.append(result.join("|"));
                }
            }
        } else {
            QString msg = QString("执行查询出错 原因: %1").arg(query.lastError().text());
            emit debug(msg);
        }
    }

    return resultData;
}

void DbTcpServerThread::setConnInfo(DbType dbType, const QString &connName, const QString &hostName, int port,
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

void DbTcpServerThread::setReadInterval(int readInterval)
{
    if (readInterval >= 1000 && this->readInterval != readInterval) {
        this->readInterval = readInterval;
    }
}

void DbTcpServerThread::setCheckInterval(int checkInterval)
{
    if (checkInterval > 5 && this->checkInterval != checkInterval) {
        this->checkInterval = checkInterval;
    }
}

void DbTcpServerThread::setTableName(const QString &tableName)
{
    if (this->tableName != tableName) {
        this->tableName = tableName;
    }
}

void DbTcpServerThread::setColumnName(const QStringList &columnName)
{
    if (this->columnName != columnName) {
        this->columnName = columnName;
    }
}

void DbTcpServerThread::setKeyName(const QStringList &keyName)
{
    if (this->keyName != keyName) {
        this->keyName = keyName;

        //将对应的关键字段的索引找出
        keyIndex.clear();
        for (int i = 0; i < keyName.count(); i++) {
            keyIndex << columnName.indexOf(keyName.at(i));
        }
    }
}

#include "dbtcpdatathread.h"

QScopedPointer<DbTcpDataThread> DbTcpDataThread::self;
DbTcpDataThread *DbTcpDataThread::Instance()
{
    if (self.isNull()) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbTcpDataThread);
        }
    }

    return self.data();
}

DbTcpDataThread::DbTcpDataThread(QObject *parent) : QThread(parent)
{
    stopped = false;

    dbOkLocal = false;
    lastCheckTimeLocal = QDateTime::currentDateTime();

    dbOkNet = false;
    lastCheckTimeNet = QDateTime::currentDateTime();

    checkInterval = 30;

    dbTypeLocal = DbType_MySql;
    connNameLocal = "dbLocal";
    hostNameLocal = "127.0.0.1";
    portLocal = 3306;
    dbNameLocal = "tcms_liu_mysql";
    userNameLocal = "root";
    userPwdLocal = "root";

    dbTypeNet = DbType_MySql;
    connNameNet = "dbNet";
    hostNameNet = "127.0.0.1";
    portNet = 3306;
    dbNameNet = "tcms_liu_mysql";
    userNameNet = "root";
    userPwdNet = "root";

    maxCount = 1000;
    listSql.clear();
}

DbTcpDataThread::~DbTcpDataThread()
{
    this->stop();
    this->wait(1000);
}

void DbTcpDataThread::run()
{
    while(!stopped) {
        //优先执行排队等待执行的sql语句
        if (sqlLocal.count() > 0) {
            mutex.lock();
            QString sql = sqlLocal.takeFirst();
            mutex.unlock();

            execSqlLocal(sql);
            msleep(10);
            continue;
        }

        if (sqlNet.count() > 0) {
            mutex.lock();
            QString sql = sqlNet.takeFirst();
            mutex.unlock();

            execSqlNet(sql);
            msleep(10);
            continue;
        }

        //定时执行一次校验数据库连接是否正常
        QDateTime now = QDateTime::currentDateTime();
        if (lastCheckTimeLocal.secsTo(now) >= checkInterval) {
            checkDbLocal();
            msleep(10);
            lastCheckTimeLocal = now;
            continue;
        }

        if (lastCheckTimeNet.secsTo(now) >= checkInterval) {
            checkDbNet();
            msleep(10);
            lastCheckTimeNet = now;
            continue;
        }

        //如果数据库连接正常则处理数据,不正常则重连数据库
        if (dbOkLocal) {
            checkDataLocal();
            msleep(10);
        } else {
            openDbLocal();
            emit debug(QString("重连本地数据库%1").arg(dbOkLocal ? "成功" : "失败"));
            msleep(3000);
        }

        //如果数据库连接正常则处理数据,不正常则重连数据库
        if (dbOkNet) {
            checkDataNet();
            msleep(10);
        } else {
            openDbNet();
            emit debug(QString("重连远程数据库%1").arg(dbOkNet ? "成功" : "失败"));
            msleep(3000);
        }

        msleep(100);
    }

    stopped = false;
}

bool DbTcpDataThread::openDbLocal()
{
    //可以自行增加其他数据库的支持
    if (dbTypeLocal == DbType_Sqlite) {
        dbConnLocal = QSqlDatabase::addDatabase("QSQLITE", connNameLocal);
        dbConnLocal.setDatabaseName(dbNameLocal);
    } else if (dbTypeLocal == DbType_MySql) {
        dbConnLocal = QSqlDatabase::addDatabase("QMYSQL", connNameLocal);
        dbConnLocal.setHostName(hostNameLocal);
        dbConnLocal.setPort(portLocal);
        dbConnLocal.setDatabaseName(dbNameLocal);
        dbConnLocal.setUserName(userNameLocal);
        dbConnLocal.setPassword(userPwdLocal);
    }

    dbOkLocal = dbConnLocal.open();
    return dbOkLocal;
}

bool DbTcpDataThread::checkDbLocal()
{
    QDateTime dtStart = QDateTime::currentDateTime();

    QString sql = "select 1";
    QSqlQuery query(dbConnLocal);
    dbOkLocal = query.exec(sql);

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);
    emit debug(QString("检查数据库连接(共 %1 条/用时 %2 秒)").arg(1).arg(QString::number(ms / 1000, 'f', 3)));

    return dbOkLocal;
}

void DbTcpDataThread::closeDbLocal()
{
    dbConnLocal.close();
    QSqlDatabase::removeDatabase(connNameLocal);
    dbOkLocal = false;
    emit debug("关闭数据库");
}

bool DbTcpDataThread::openDbNet()
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

bool DbTcpDataThread::checkDbNet()
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

void DbTcpDataThread::closeDbNet()
{
    dbConnNet.close();
    QSqlDatabase::removeDatabase(connNameNet);
    dbOkNet = false;
    emit debug("关闭数据库");
}

void DbTcpDataThread::stop()
{
    stopped = true;
}

void DbTcpDataThread::append(const QString &sql)
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

void DbTcpDataThread::clear()
{
    mutex.lock();
    listSql.clear();
    listID.clear();
    mutex.unlock();
}

void DbTcpDataThread::appendSqlLocal(const QString &sql)
{
    if (dbOkLocal) {
        QString strSql = sql;
        strSql = strSql.replace("'", "''");
        strSql = QString("insert into TempSql(StrSql) values('%1')").arg(strSql);
        QSqlQuery query(dbConnLocal);
        dbOkLocal = query.exec(strSql);
    }
}

void DbTcpDataThread::appendExecSqlLocal(const QString &sql)
{
    mutex.lock();
    sqlLocal.append(sql);
    mutex.unlock();
}

void DbTcpDataThread::appendExecSqlNet(const QString &sql)
{
    mutex.lock();
    sqlNet.append(sql);
    mutex.unlock();
}

bool DbTcpDataThread::execSqlLocal(const QString &sql)
{
    if (dbOkLocal) {
        QSqlQuery query(dbConnLocal);
        dbOkLocal = query.exec(sql);
    }

    return dbOkLocal;
}

bool DbTcpDataThread::execSqlNet(const QString &sql)
{
    if (dbOkNet) {
        QSqlQuery query(dbConnNet);
        dbOkNet = query.exec(sql);
    }

    return dbOkNet;
}

bool DbTcpDataThread::checkDataLocal()
{
    //处理流程
    //外部产生的各种sql语句插入到临时sql表中
    //线程运行,实时检查数据库连接,失败则重连数据库

    //第一步:检查本地数据库临时sql语句表是否有数据,有则查询出来添加到sql语句队列
    //第二步:检查远程数据库是否连接成功,成功则将该sql语句执行到远程数据库
    //第三步:检查本地数据库是否连接成功,成功则将sql语句插入到本地临时sql表
    //只有成功执行到了远程数据库或者插入到本地数据库才将sql语句移除

    if (dbOkLocal) {
        QString sql = QString("select ID,StrSql from TempSql limit %1").arg(maxCount);
        QSqlQuery query(dbConnLocal);
        dbOkLocal = query.exec(sql);
        while (query.next()) {
            QString id = query.value(0).toString();

            if (!listID.contains(id)) {
                listID.append(id);
                append(query.value(1).toString());
            }
        }
    }

    return dbOkLocal;
}

bool DbTcpDataThread::checkDataNet()
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

        //提交成功后 删除已经同步的sql语句,同时将sql语句队列清空
        //提交失败则需要回滚事务
        dbOkNet = dbConnNet.commit();

        if (dbOkNet) {
            QSqlQuery query(dbConnLocal);
            QString sql = QString("delete from TempSql where ID in ('%1')").arg(listID.join("','"));
            dbOkLocal = query.exec(sql);

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

void DbTcpDataThread::setConnInfoLocal(DbType dbType, const QString &connName, const QString &hostName, int port,
                                       const QString &dbName, const QString &userName, const QString &userPwd)
{
    this->dbTypeLocal = dbType;
    this->connNameLocal = connName;
    this->hostNameLocal = hostName;
    this->portLocal = port;
    this->dbNameLocal = dbName;
    this->userNameLocal = userName;
    this->userPwdLocal = userPwd;
}

void DbTcpDataThread::setConnInfoNet(DbType dbType, const QString &connName, const QString &hostName, int port,
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

void DbTcpDataThread::setCheckInterval(int checkInterval)
{
    if (checkInterval > 5 && this->checkInterval != checkInterval) {
        this->checkInterval = checkInterval;
    }
}

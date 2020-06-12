#include "dbexecsqlthread.h"

DbExecSqlThread::DbExecSqlThread(QObject *parent) : QThread(parent)
{
    connName = "qt_sql_default_connection";
    sqls.clear();

    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
}

void DbExecSqlThread::run()
{
    //计算用时
    QDateTime dtStart = QDateTime::currentDateTime();

    //启动数据库事务
    QSqlDatabase::database(connName).transaction();

    int count = sqls.count();
    for (int i = 0; i < count; i++) {
        QSqlQuery query(QSqlDatabase::database(connName));
        query.exec(sqls.at(i));
    }

    //提交数据库事务
    bool ok = QSqlDatabase::database(connName).commit();
    if (!ok) {
        QSqlDatabase::database(connName).rollback();
    }

    QDateTime dtEnd = QDateTime::currentDateTime();
    double msec = dtStart.msecsTo(dtEnd);
    emit finsh(ok, count, msec);
}

void DbExecSqlThread::setConnName(const QString &connName)
{
    this->connName = connName;
}

void DbExecSqlThread::setSqls(const QStringList &sqls)
{
    this->sqls = sqls;
}

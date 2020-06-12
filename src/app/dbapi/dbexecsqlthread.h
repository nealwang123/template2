#ifndef DBEXECSQLTHREAD_H
#define DBEXECSQLTHREAD_H

/**
 * 批量执行sql语句线程类
 * 1:可设置对应数据库连接名称和sql语句
 * 2:执行结果提供信号传出执行是否成功,执行条数,用时
 */

#include <QtGui>
#include <QtSql>

class DbExecSqlThread : public QThread
{
    Q_OBJECT
public:
    explicit DbExecSqlThread(QObject *parent = 0);

private:
    QString connName;   //数据库连接名称
    QStringList sqls;   //要执行的sql语句集合

protected:
    void run();

signals:
    void finsh(bool ok, quint32 count, double msec);

public slots:
    //设置数据库连接名称
    void setConnName(const QString &connName);

    //设置要执行的sql语句集合
    void setSqls(const QStringList &sqls);
};

#endif // DBEXECSQLTHREAD_H

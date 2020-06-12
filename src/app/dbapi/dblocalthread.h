#ifndef DBLOCALTHREAD_H
#define DBLOCALTHREAD_H

/**
 * 本地数据库打开、关闭、检查、重连
 * 1:支持数据库自动重连
 * 2:可设置检查数据库时间间隔
 */

#include <QtGui>
#include <QtSql>

class DbLocalThread : public QThread
{
    Q_OBJECT
public:
    enum DbType {
        DbType_Sqlite = 0,      //sqlite数据库
        DbType_MySql = 1,       //mysql数据库
        DbType_SqlServer = 3,   //sqlserver数据库
        DbType_Access = 4,      //access数据库
        DbType_PostgreSQL = 5   //postgresql数据库
    };

    static DbLocalThread *Instance();
    explicit DbLocalThread(QObject *parent = 0);
    ~DbLocalThread();

protected:
    void run();

private:
    static QScopedPointer<DbLocalThread> self;

    volatile bool stopped;          //停止线程标志位

    bool dbOkLocal;                 //本地数据库是否正常
    QSqlDatabase dbConnLocal;       //本地数据库连接对象
    QDateTime lastCheckTimeLocal;   //本地最后一次测试数据库连接时间
    int checkInterval;              //检查数据库连接间隔

    DbType dbTypeLocal;             //本地数据库类型
    QString connNameLocal;          //本地数据库连接名称
    QString hostNameLocal;          //本地数据库地址
    int portLocal;                  //本地数据库端口
    QString dbNameLocal;            //本地数据库名称
    QString userNameLocal;          //本地数据库用户名
    QString userPwdLocal;           //本地数据库用户密码

signals:
    //打印信息信号
    void debug(const QString &msg);

public slots:
    //打开数据库
    bool openDb();

    //检查数据库连接
    bool checkDb();

    //关闭数据库
    void closeDb();

    //停止线程
    void stop();

    //设置数据库连接信息
    void setConnInfo(DbType dbType, const QString &connName, const QString &hostName, int port,
                     const QString &dbName, const QString &userName, const QString &userPwd);

    //设置检查数据库间隔
    void setCheckInterval(int checkInterval);
};

#endif // DBLOCALTHREAD_H

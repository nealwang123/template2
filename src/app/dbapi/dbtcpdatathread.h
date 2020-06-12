#ifndef DBTCPDATATHREAD_H
#define DBTCPDATATHREAD_H

/**
 * 本地数据同步到远程数据库线程
 * 1:可设置远程mysql数据库信息
 * 2:可设置本地mysql数据库信息
 * 3:自动重连数据库,包括本地数据库和远程数据库
 * 4:可设置检测连接间隔
 * 5:所有消息都发送信号,可在信号槽函数中打印输出
 * 6:支持优先执行sql语句
 */

#include <QtGui>
#include <QtSql>

class DbTcpDataThread : public QThread
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

    static DbTcpDataThread *Instance();
    explicit DbTcpDataThread(QObject *parent = 0);
    ~DbTcpDataThread();

protected:
    void run();

private:
    static QScopedPointer<DbTcpDataThread> self;

    QMutex mutex;                   //互斥变量锁
    volatile bool stopped;          //停止线程标志位

    bool dbOkLocal;                 //本地数据库是否正常
    QSqlDatabase dbConnLocal;       //本地数据库连接对象
    QDateTime lastCheckTimeLocal;   //本地最后一次测试数据库连接时间

    bool dbOkNet;                   //远程数据库是否正常
    QSqlDatabase dbConnNet;         //远程数据库连接对象
    QDateTime lastCheckTimeNet;     //远程最后一次测试数据库连接时间

    int checkInterval;              //检查数据库连接间隔

    DbType dbTypeLocal;             //本地数据库类型
    QString connNameLocal;          //本地数据库连接名称
    QString hostNameLocal;          //本地数据库地址
    int portLocal;                  //本地数据库端口
    QString dbNameLocal;            //本地数据库名称
    QString userNameLocal;          //本地数据库用户名
    QString userPwdLocal;           //本地数据库用户密码

    DbType dbTypeNet;               //远程数据库类型
    QString connNameNet;            //远程数据库连接名称
    QString hostNameNet;            //远程数据库地址
    int portNet;                    //远程数据库端口
    QString dbNameNet;              //远程数据库名称
    QString userNameNet;            //远程数据库用户名
    QString userPwdNet;             //远程数据库用户密码

    int maxCount;                   //sql语句队列中最大的数量
    QStringList listSql;            //sql语句队列
    QStringList listID;             //sql语句队列编号,用于删除本地sql语句表

    QStringList sqlLocal;           //本地数据库优先执行的sql语句
    QStringList sqlNet;             //远程数据库优先执行的sql语句

signals:
    //打印信息信号
    void debug(const QString &msg);

public slots:
    //打开本地数据库
    bool openDbLocal();

    //检查本地数据库连接
    bool checkDbLocal();

    //关闭本地数据库
    void closeDbLocal();

    //打开远程数据库
    bool openDbNet();

    //检查远程数据库连接
    bool checkDbNet();

    //关闭远程数据库
    void closeDbNet();

    //停止线程
    void stop();

    //添加sql语句
    void append(const QString &sql);
    //清空sql语句
    void clear();

    //插入临时sql语句到本地数据库(外部使用都使用这个函数)
    void appendSqlLocal(const QString &sql);

    //将优先执行的sql语句添加到队列
    void appendExecSqlLocal(const QString &sql);
    void appendExecSqlNet(const QString &sql);

    //直接对数据库执行sql语句
    bool execSqlLocal(const QString &sql);
    bool execSqlNet(const QString &sql);

    //执行动作
    bool checkDataLocal();
    bool checkDataNet();

    //设置数据库连接信息
    void setConnInfoLocal(DbType dbType, const QString &connName, const QString &hostName, int port,
                          const QString &dbName, const QString &userName, const QString &userPwd);
    void setConnInfoNet(DbType dbType, const QString &connName, const QString &hostName, int port,
                        const QString &dbName, const QString &userName, const QString &userPwd);

    //设置检查数据库间隔
    void setCheckInterval(int checkInterval);
};

#endif // DBTCPDATATHREAD_H

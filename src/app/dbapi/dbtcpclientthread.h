#ifndef DBTCPCLIENTTHREAD_H
#define DBTCPCLIENTTHREAD_H

/**
 * 远程数据实时同步线程客户端,负责上传数据
 * 1:可设置远程mysql数据库信息
 * 2:自动重连数据库
 * 3:可设置检测连接间隔
 * 4:自动重置远程设备信息
 * 5:所有消息都发送信号,可在信号槽函数中打印输出
 */

#include <QtGui>
#include <QtSql>

class DbTcpClientThread : public QThread
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

    static DbTcpClientThread *Instance();
    explicit DbTcpClientThread(QObject *parent = 0);
    ~DbTcpClientThread();

protected:
    void run();

private:
    static QScopedPointer<DbTcpClientThread> self;

    QMutex mutex;                   //互斥变量锁
    volatile bool stopped;          //停止线程标志位

    bool dbOkNet;                   //远程数据库是否正常
    QSqlDatabase dbConnNet;         //远程数据库连接对象
    QDateTime lastCheckTimeNet;     //远程最后一次测试数据库连接时间
    int checkInterval;              //检查数据库连接间隔

    DbType dbTypeNet;               //远程数据库类型
    QString connNameNet;            //远程数据库连接名称
    QString hostNameNet;            //远程数据库地址
    int portNet;                    //远程数据库端口
    QString dbNameNet;              //远程数据库名称
    QString userNameNet;            //远程数据库用户名
    QString userPwdNet;             //远程数据库用户密码

    int maxCount;                   //sql语句队列中最大的数量
    QStringList listSql;            //sql语句队列

    QList<int> listDeviceID;        //设备编号队列
    QStringList listDeviceName;     //设备名称队列
    QList<int> listDeviceAddr;      //设备地址队列
    QStringList listGroupName;      //设备分组队列

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

    //初始化数据库
    void initDb();

    //停止线程
    void stop();

    //添加sql语句
    void append(const QString &sql);
    //清空sql语句
    void clear();

    //执行动作
    bool checkData();

    //设置数据库连接信息
    void setConnInfo(DbType dbType, const QString &connName, const QString &hostName, int port,
                     const QString &dbName, const QString &userName, const QString &userPwd);

    //设置检查数据库间隔
    void setCheckInterval(int checkInterval);

    //设置初始化用的数据
    void setListDeviceID(const QList<int> &listDeviceID);
    void setListDeviceName(const QStringList &listDeviceName);
    void setListDeviceAddr(const QList<int> &listDeviceAddr);
    void setListGroupName(const QStringList &listGroupName);
};

#endif // DBTCPCLIENTTHREAD_H

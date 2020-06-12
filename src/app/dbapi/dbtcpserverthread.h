#ifndef DBTCPSERVERTHREAD_H
#define DBTCPSERVERTHREAD_H

/**
 * 远程数据实时同步线程服务端,负责下载数据
 * 1:可设置远程mysql数据库信息
 * 2:自动重连数据库
 * 3:可设置检测连接间隔
 * 4:可设置要查询的表名
 * 5:所有消息都发送信号,可在信号槽函数中打印输出
 * 6:返回数据结果严格按照字段顺序
 * 7:返回的数据同时包含关键字字段数据
 * 8:提供接口可对远程数据库执行sql语句
 * 9:提供接口可对远程数据库获取对应表数据
 */

#include <QtGui>
#include <QtSql>

class DbTcpServerThread : public QThread
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

    static DbTcpServerThread *Instance();
    explicit DbTcpServerThread(QObject *parent = 0);
    ~DbTcpServerThread();

protected:
    void run();

private:
    static QScopedPointer<DbTcpServerThread> self;

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

    int readInterval;               //获取数据间隔
    QString tableName;              //要查询的表名
    QStringList columnName;         //要查询的数据字段
    QStringList keyName;            //要查询的关键字数据字段
    QList<int> keyIndex;            //对应关键字字段索引

signals:
    //打印信息信号
    void debug(const QString &msg);
    //数据到来信号
    void dataReceive(const QStringList &keyData, const QStringList &resultData);

public slots:
    //打开数据库
    bool openDb();

    //检查数据库连接
    bool checkDb();

    //关闭数据库
    void closeDb();

    //停止线程
    void stop();

    //执行动作
    bool checkData();

    //执行sql语句,无返回结果
    bool execSql(const QString &sql);
    //获取对应表数据
    QStringList getData(const QString &sql);

    //设置数据库连接信息
    void setConnInfo(DbType dbType, const QString &connName, const QString &hostName, int port,
                     const QString &dbName, const QString &userName, const QString &userPwd);

    //设置获取数据间隔,单位毫秒
    void setReadInterval(int readInterval);
    //设置检查数据库间隔,单位秒
    void setCheckInterval(int checkInterval);

    //设置要查询的表名
    void setTableName(const QString &tableName);
    //设置要查询的字段名
    void setColumnName(const QStringList &columnName);
    //设置要查询的关键字段名
    void setKeyName(const QStringList &keyName);
};

#endif // DBTCPSERVERTHREAD_H

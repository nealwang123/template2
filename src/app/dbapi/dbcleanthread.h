#ifndef DBCLEANTHREAD_H
#define DBCLEANTHREAD_H

/**
 * 自动清理数据类
 * 1:可设置要清理的对应数据库连接名称和表名
 * 2:可设置条件字段
 * 3:可设置排序字段
 * 4:可设置最大保留的记录数
 * 5:可设置执行自动清理的间隔
 * 6:后期支持多个数据库和多个表
 * 7:建议条件字段用数字类型的主键,速度极快
 * 8:增加统计用字段名称设置
 * 9:增加自动清理文件夹,超过大小自动删除文件夹中早期文件
 */

#include <QtGui>
#include <QtSql>

class DbCleanThread : public QThread
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

    static DbCleanThread *Instance();
    explicit DbCleanThread(QObject *parent = 0);
    ~DbCleanThread();

protected:
    void run();

private:
    static QScopedPointer<DbCleanThread> self;

    volatile bool stopped;          //停止线程标志位
    volatile bool isBusy;           //是否繁忙
    QDateTime lastTime;             //最后一次检测时间

    QString connName;               //数据库连接名称
    QString tableName;              //表名
    QString countName;              //统计表行数用字段
    QString whereColumnName;        //条件字段
    QString orderSql;               //排序字段及排序规则
    int maxCount;                   //最大记录数
    int interval;                   //执行间隔

    QString dirPath;                //监听的文件夹
    QStringList dirFileFilter;      //监听的文件夹过滤类型
    int dirMaxSize;                 //最大大小,单位MB
    DbType dbType;                  //数据库类型

signals:
    //清理完成,包括执行成功与否,清理的记录条数,开销时间
    void cleanFinsh(bool ok, int cleanCount, double msec);

    //删除文件夹完成信号
    void cleanDir(const QString &dirPath);

public slots:
    //设置数据库连接名称
    void setConnName(const QString &connName);

    //设置表名
    void setTableName(const QString &tableName);

    //设置条件字段
    void setWhereColumnName(const QString &whereColumnName);

    //设置排序字段及排序规则
    void setOrderSql(const QString &orderSql);

    //设置最大保留的记录数
    void setMaxCount(int maxCount);

    //设置执行自动清理的间隔
    void setInterval(int interval);

    //综合设置
    void setPar(const QString &connName, const QString &tableName,
                const QString &countName, const QString &whereColumnName,
                const QString &orderSql, int maxCount, int interval);

    //设置监听的文件夹
    void setDirPath(const QString &dirPath);

    //设置监听的文件夹的类型
    void setDirFileFilter(const QStringList &dirFileFilter);

    //设置监听文件夹最大大小
    void setDirMaxSize(int dirMaxSize);

    //设置数据库类型
    void setDbType(const DbType &dbType);

    //删除指定文件夹
    void deleteDirectory(const QString &path);

    //停止线程
    void stop();

    //执行清理操作
    void clean();
};

#endif // DBCLEANTHREAD_H

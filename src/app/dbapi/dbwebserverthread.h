#ifndef DBWEBSERVERTHREAD_H
#define DBWEBSERVERTHREAD_H

/**
 * 远程数据实时同步线程服务端,负责下载数据
 * 1:通过web方式访问网页获取返回值
 * 2:可以拓展支持更多种支持方式
 * 3:可设置检测连接间隔
 * 4:可设置要查询的表名
 * 5:所有消息都发送信号,可在信号槽函数中打印输出
 * 6:返回数据结果严格按照字段顺序
 * 7:返回的数据同时包含关键字字段数据
 */

#include <QtGui>
#include <QtNetwork>

class DbWebServerThread : public QThread
{
    Q_OBJECT
public:
    static DbWebServerThread *Instance();
    explicit DbWebServerThread(QObject *parent = 0);
    ~DbWebServerThread();

protected:
    void run();

private:
    static QScopedPointer<DbWebServerThread> self;

    volatile bool stopped;      //停止线程标志位
    QString webUrl;             //要访问的web地址

    int readInterval;           //获取数据间隔
    QString tableName;          //要查询的表名
    QStringList columnName;     //要查询的数据字段
    QStringList keyName;        //要查询的关键字数据字段
    QList<int> keyIndex;        //对应关键字字段索引

signals:
    //打印信息信号
    void debug(const QString &msg);
    //数据到来信号
    void dataReceive(const QStringList &keyData, const QStringList &resultData);

public slots:
    //停止线程
    void stop();

    //执行动作
    void checkData();

    //获取对应表数据
    QStringList getData(const QString &url);

    //设置要访问的web地址
    void setWebUrl(const QString &webUrl);

    //设置获取数据间隔,单位毫秒
    void setReadInterval(int readInterval);
    //设置要查询的表名
    void setTableName(const QString &tableName);
    //设置要查询的字段名
    void setColumnName(const QStringList &columnName);
    //设置要查询的关键字段名
    void setKeyName(const QStringList &keyName);
};

#endif // DBWEBSERVERTHREAD_H

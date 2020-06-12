#ifndef SAVERUNTIME_H
#define SAVERUNTIME_H

#include <QObject>
#include <QMutex>
#include <QDateTime>

class QTimer;

class SaveRunTime : public QObject
{
    Q_OBJECT
public:
    explicit SaveRunTime(QObject *parent = 0);
    static SaveRunTime *Instance()
    {
        static QMutex mutex;

        if (!self) {
            QMutexLocker locker(&mutex);

            if (!self) {
                self = new SaveRunTime;
            }
        }

        return self;
    }

private:
    static SaveRunTime *self;
    QString path;       //日志文件路径
    QString name;       //日志文件名称

    int lastID;
    int saveInterval;
    QDateTime startTime;
    QString logFile;
    QTimer *timerSave;

private:
    void getDiffValue(const QDateTime &startTime, const QDateTime &endTime, int &day, int &hour, int &minute);

signals:

public slots:
    void start();       //启动服务
    void stop();        //停止服务
    void initLog();     //初始化日志文件
    void appendLog();   //追加一条记录到日志文件
    void saveLog();     //保存运行时间到日志文件

    void setPath(const QString &path);
    void setName(const QString &name);
    void setSaveInterval(int saveInterval);
};

#endif // SAVERUNTIME_H

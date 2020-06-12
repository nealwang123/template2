#ifndef SAVELOG_H
#define SAVELOG_H

#include <QObject>
#include <QMutex>

class SaveLog : public QObject
{
	Q_OBJECT
public:
    explicit SaveLog(QObject *parent = 0);
    static SaveLog *Instance()
	{
		static QMutex mutex;

		if (!self) {
			QMutexLocker locker(&mutex);

			if (!self) {
                self = new SaveLog;
			}
		}

		return self;
	}	

private:
    static SaveLog *self;
    QString path;       //日志文件路径
    QString name;       //日志文件名称

public:
    QString getPath()   const;
    QString getName()   const;

public slots:
    void start();
    void stop();

    void setPath(const QString &path);
    void setName(const QString &name);
};

#endif // SAVELOG_H

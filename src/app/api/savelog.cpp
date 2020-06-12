#include "savelog.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qapplication.h"
#include "qdatetime.h"
#include "qstringlist.h"

QFile *logFile = new QFile;
QString logFileName;

//输出所有打印调试信息到日志文件
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
void Log(QtMsgType type, const char *msg)
#else
void Log(QtMsgType type, const QMessageLogContext &, const QString &msg)
#endif
{
    // 加锁
    static QMutex mutex;
    mutex.lock();
    QString content;

    switch (type) {
    case QtDebugMsg:
        content = QString("%1").arg(msg);
        break;

    case QtWarningMsg:
        content = QString("%1").arg(msg);
        break;

    case QtCriticalMsg:
        content = QString("%1").arg(msg);
        break;

    case QtFatalMsg:
        content = QString("%1").arg(msg);
        break;
    }

    //方法改进:之前每次输出日志都打开文件,改成只有当日期改变时才新建和打开文件
    QString fileName = QString("%1/%2_log_%3.txt")
            .arg(SaveLog::Instance()->getPath())
            .arg(SaveLog::Instance()->getName())
            .arg(QDate::currentDate().toString("yyyy-MM-dd"));

    if (fileName != logFileName) {
        logFileName = fileName;

        if (logFile->isOpen()) {
            logFile->close();
        }

        logFile->setFileName(logFileName);
        logFile->open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
    }

    QTextStream logStream(logFile);
    logStream << content << "\n";
    mutex.unlock();
}

SaveLog *SaveLog::self = NULL;
SaveLog::SaveLog(QObject *parent) : QObject(parent)
{
    path = qApp->applicationDirPath();
    QString str = qApp->applicationFilePath();
    QStringList list = str.split("/");
    name = list.at(list.count() - 1).split(".").at(0);
}

QString SaveLog::getPath() const
{
    return this->path;
}

QString SaveLog::getName() const
{
    return this->name;
}

//安装日志钩子,输出调试信息到文件,便于调试
void SaveLog::start()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    qInstallMsgHandler(Log);
#else
    qInstallMessageHandler(Log);
#endif
}

//卸载日志钩子
void SaveLog::stop()
{
#if (QT_VERSION <= QT_VERSION_CHECK(5,0,0))
    qInstallMsgHandler(0);
#else
    qInstallMessageHandler(0);
#endif
}

void SaveLog::setPath(const QString &path)
{
    if (this->path != path) {
        this->path = path;
    }
}

void SaveLog::setName(const QString &name)
{
    if (this->name != name) {
        this->name = name;
    }
}

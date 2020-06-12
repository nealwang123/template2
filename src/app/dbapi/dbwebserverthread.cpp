#include "dbwebserverthread.h"

QScopedPointer<DbWebServerThread> DbWebServerThread::self;
DbWebServerThread *DbWebServerThread::Instance()
{
    if (self.isNull()) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new DbWebServerThread);
        }
    }

    return self.data();
}

DbWebServerThread::DbWebServerThread(QObject *parent) : QThread(parent)
{
    stopped = false;

    webUrl = "http://114.215.135.238/getdevicelog.php";

    readInterval = 1000;
    tableName = "DeviceLogTemp";
    columnName << "DeviceLogID" << "DeviceID" << "DeviceName" << "DeviceAddr" << "GroupName"
               << "TempValue" << "DampValue" << "PowerValue" << "SaveTime" << "LogCountAll" << "LogCountNoRead" << "Type";
    keyName << "DeviceName" << "GroupName";
    keyIndex << 2 << 4;
}

DbWebServerThread::~DbWebServerThread()
{
    this->stop();
    this->wait(1000);
}

void DbWebServerThread::run()
{
    while (!stopped) {
        checkData();
        msleep(readInterval);
    }

    stopped = false;
}

void DbWebServerThread::stop()
{
    stopped = true;
}

void DbWebServerThread::checkData()
{
    int count = columnName.count();

    if (count == 0 || tableName.isEmpty()) {
        return;
    }

    QDateTime dtStart = QDateTime::currentDateTime();

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(webUrl)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();

    //取出对应的关键字数据
    QStringList resultData = QString::fromUtf8(responseData).split(";");
    QStringList keyData;
    int dataCount = resultData.count();

    for (int i = 0; i < dataCount; i++) {
        //将查询结果对应的关键字结果存入字符串链表,中间分隔符 |
        QStringList key;
        QString str = resultData.at(i);
        for (int j = 0; j < keyName.count(); j++) {
            int index = keyIndex.at(j);
            key.append(str.split("|").at(index));
        }

        keyData.append(key.join("|"));
    }

    emit dataReceive(keyData, resultData);

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);
    emit debug(QString("远程实时获取数据(共 %1 条/用时 %2 秒)").arg(dataCount).arg(QString::number(ms / 1000, 'f', 3)));
}

QStringList DbWebServerThread::getData(const QString &url)
{
    QStringList resultData;

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    resultData = QString::fromUtf8(responseData).split(";");

    return resultData;
}

void DbWebServerThread::setWebUrl(const QString &webUrl)
{
    if (this->webUrl != webUrl) {
        this->webUrl = webUrl;
    }
}

void DbWebServerThread::setReadInterval(int readInterval)
{
    if (readInterval >= 1000 && this->readInterval != readInterval) {
        this->readInterval = readInterval;
    }
}

void DbWebServerThread::setTableName(const QString &tableName)
{
    if (this->tableName != tableName) {
        this->tableName = tableName;
    }
}

void DbWebServerThread::setColumnName(const QStringList &columnName)
{
    if (this->columnName != columnName) {
        this->columnName = columnName;
    }
}

void DbWebServerThread::setKeyName(const QStringList &keyName)
{
    if (this->keyName != keyName) {
        this->keyName = keyName;

        //将对应的关键字段的索引找出
        keyIndex.clear();
        for (int i = 0; i < keyName.count(); i++) {
            keyIndex << columnName.indexOf(keyName.at(i));
        }
    }
}

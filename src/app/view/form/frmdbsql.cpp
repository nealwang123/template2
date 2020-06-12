#include "frmdbsql.h"
#include "ui_frmdbsql.h"
#include "qsqldatabase.h"
#include "qsqlquery.h"
#include "qfile.h"
#include "qdebug.h"
#include "quiwidget.h"

frmDbSql::frmDbSql(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDbSql)
{
    ui->setupUi(this);
    this->initConfig();
}

frmDbSql::~frmDbSql()
{
    delete ui;
}

void frmDbSql::initConfig()
{
    ui->txtSqliteDBName->setText(App::SqliteDBName);
    connect(ui->txtSqliteDBName, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtSqliteSql->setText(App::SqliteSql);
    connect(ui->txtSqliteSql, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtMysqlDBIP->setText(App::MysqlDBIP);
    connect(ui->txtMysqlDBIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtMysqlDBPort->setText(QString::number(App::MysqlDBPort));
    connect(ui->txtMysqlDBPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtMysqlDBName->setText(App::MysqlDBName);
    connect(ui->txtMysqlDBName, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtMysqlUserName->setText(App::MysqlUserName);
    connect(ui->txtMysqlUserName, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtMysqlUserPwd->setText(App::MysqlUserPwd);
    connect(ui->txtMysqlUserPwd, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtMysqlSql->setText(App::MysqlSql);
    connect(ui->txtMysqlSql, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmDbSql::saveConfig()
{
    App::SqliteDBName = ui->txtSqliteDBName->text();
    App::SqliteSql = ui->txtSqliteSql->text();
    App::MysqlDBIP = ui->txtMysqlDBIP->text();
    App::MysqlDBPort = ui->txtMysqlDBPort->text().toInt();
    App::MysqlDBName = ui->txtMysqlDBName->text();
    App::MysqlUserName = ui->txtMysqlUserName->text();
    App::MysqlUserPwd = ui->txtMysqlUserPwd->text();
    App::MysqlSql = ui->txtMysqlSql->text();
    App::writeConfig();
}

void frmDbSql::on_btnSqliteConnect_clicked()
{
    {
        QString dbFile = QUIHelper::appPath() + App::SqliteDBName;
        QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE", "sqlitedb");
        dbConn.setDatabaseName(dbFile);

        if (dbConn.open()) {
            QUIHelper::showMessageBoxInfo("连接数据库成功:"+App::SqliteDBName);
        } else {
            QUIHelper::showMessageBoxError("连接数据库失败:"+App::SqliteDBName);
        }

        dbConn.close();
    }

    QSqlDatabase::removeDatabase("sqlitedb");
}

void frmDbSql::on_btnSqliteSql_clicked()
{
    QFile file(QUIHelper::appPath() + App::SqliteSql);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QUIHelper::showMessageBoxError("数据库脚本文件打开失败!");
        return;
    }

    QString dbFile = QUIHelper::appPath() + App::SqliteDBName;

    //先删除本地数据库文件
    QFile f(dbFile);
    f.remove();
    QUIHelper::sleep(100);

    QDateTime dtStart = QDateTime::currentDateTime();

    {
        QSqlDatabase dbConn = QSqlDatabase::addDatabase("QSQLITE", "sqlitedb");
        dbConn.setDatabaseName(dbFile);

        if (!dbConn.open()) {
            QUIHelper::showMessageBoxError("连接数据库失败!");
            return;
        }

        QSqlQuery query(QSqlDatabase::database("sqlitedb"));

        QString sql = "BEGIN TRANSACTION;";
        qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);

        while (!file.atEnd()) {
            sql = QString::fromUtf8(file.readLine());
            sql = sql.replace("\n", "");
            if (sql.startsWith("--") || sql.length() < 5) {
                continue;
            }
            qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);
        }

        sql = "COMMIT TRANSACTION;";
        qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);

        dbConn.close();
    }

    QSqlDatabase::removeDatabase("sqlitedb");

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);

    QUIHelper::showMessageBoxInfo(QString("数据库脚本执行成功,总共用时 %1 秒!").arg(QString::number(ms / 1000, 'f', 1)));
}

void frmDbSql::on_btnMysqlConnect_clicked()
{
    //先判断对应的数据库服务器IP地址是否存在,否则会卡很久
    if (!QUIHelper::ipLive(App::MysqlDBIP, App::MysqlDBPort)) {
        QUIHelper::showMessageBoxError("数据库服务器不存在,请检查服务器IP地址和端口是否正确!");
        ui->txtMysqlDBIP->setFocus();
        return;
    }

    {
        QSqlDatabase dbConn = QSqlDatabase::addDatabase("QMYSQL", "mysqldb");
        dbConn.setHostName(App::MysqlDBIP);
        dbConn.setPort(App::MysqlDBPort);
        dbConn.setDatabaseName("mysql");
        dbConn.setUserName(App::MysqlUserName);
        dbConn.setPassword(App::MysqlUserPwd);

        if (dbConn.open()) {
            QUIHelper::showMessageBoxInfo("连接数据库成功!");
        } else {
            QUIHelper::showMessageBoxError("连接数据库失败!");
        }

        dbConn.close();
    }

    QSqlDatabase::removeDatabase("mysqldb");
}

void frmDbSql::on_btnMysqlSql_clicked()
{
    //先判断对应的数据库服务器IP地址是否存在,否则会卡很久
    if (!QUIHelper::ipLive(App::MysqlDBIP, App::MysqlDBPort)) {
        QUIHelper::showMessageBoxError("数据库服务器不存在,请检查服务器IP地址和端口是否正确!");
        ui->txtMysqlDBIP->setFocus();
        return;
    }

    QDateTime dtStart = QDateTime::currentDateTime();

    {
        QSqlDatabase dbConn = QSqlDatabase::addDatabase("QMYSQL", "mysqldb");
        dbConn.setHostName(App::MysqlDBIP);
        dbConn.setPort(App::MysqlDBPort);
        dbConn.setDatabaseName("mysql");
        dbConn.setUserName(App::MysqlUserName);
        dbConn.setPassword(App::MysqlUserPwd);

        if (!dbConn.open()) {
            QUIHelper::showMessageBoxError("连接数据库失败!");
            return;
        }

        QSqlQuery query(QSqlDatabase::database("mysqldb"));

        //第一步,删除原有数据库
        QString sql = QString("DROP DATABASE IF EXISTS %1;").arg(App::MysqlDBName);
        qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);

        //第二步,新建数据库
        sql = QString("CREATE DATABASE %1;").arg(App::MysqlDBName);
        qDebug() << TIMEMS << "sql:" << sql << "result:" << query.exec(sql);

        dbConn.close();

        QFile file(QUIHelper::appPath() + App::MysqlSql);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QUIHelper::showMessageBoxError("数据库脚本文件打开失败!");
            return;
        }

        //第三步,切换到新建的数据库库并执行建表语句
        dbConn.setHostName(App::MysqlDBIP);
        dbConn.setPort(App::MysqlDBPort);
        dbConn.setDatabaseName(App::MysqlDBName);
        dbConn.setUserName(App::MysqlUserName);
        dbConn.setPassword(App::MysqlUserPwd);

        if (!dbConn.open()) {
            QUIHelper::showMessageBoxError("连接数据库失败!");
            return;
        }
        QSqlQuery query2(QSqlDatabase::database("mysqldb"));

        sql = "BEGIN;";
        qDebug() << TIMEMS << "sql:" << sql << "result:" << query2.exec(sql);

        while (!file.atEnd()) {
            sql = QString::fromUtf8(file.readLine());
            sql = sql.replace("\n", "");
            if (sql.startsWith("--") || sql.length() < 5 || sql.toUpper().startsWith("DROP INDEX IF EXISTS")) {
                continue;
            }

            int index = sql.indexOf("INTEGER PRIMARY KEY AUTOINCREMENT");
            if (index > 0) {
                sql = sql.replace("INTEGER PRIMARY KEY AUTOINCREMENT", "INTEGER (11) NOT NULL PRIMARY KEY AUTO_INCREMENT");
            }

            qDebug() << TIMEMS << "sql:" << sql << "result:" << query2.exec(sql);
        }

        sql = "COMMIT;";
        qDebug() << TIMEMS << "sql:" << sql << "result:" << query2.exec(sql);

        dbConn.close();
    }
    QSqlDatabase::removeDatabase("mysqldb");

    QDateTime dtEnd = QDateTime::currentDateTime();
    double ms = dtStart.msecsTo(dtEnd);

    QUIHelper::showMessageBoxInfo(QString("数据库脚本执行成功,总共用时 %1 秒!").arg(QString::number(ms / 1000, 'f', 1)));
}

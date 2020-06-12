#include "frmdbclean.h"
#include "ui_frmdbclean.h"
#include "quiwidget.h"
#include "dbcleanthread.h"

frmDbClean::frmDbClean(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDbClean)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmDbClean::~frmDbClean()
{
    delete ui;
}

void frmDbClean::initForm()
{
    maxCount = 10;
    currentCount = 0;
    ui->widgetDb->setEnabled(true);
    ui->widgetServer->setEnabled(false);

    ui->tableMain->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableMain->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableMain->horizontalHeader()->setStretchLastSection(true);
    ui->tableMain->verticalHeader()->setDefaultSectionSize(25);

    connect(DbCleanThread::Instance(), SIGNAL(cleanFinsh(bool, int, double)), this, SLOT(cleanFinsh(bool, int, double)));
}

void frmDbClean::initConfig()
{
    ui->txtHostNameClean->setText(App::HostNameClean);
    ui->txtPortClean->setText(QString::number(App::PortClean));
    ui->txtDbNameClean->setText(App::DbNameClean);
    ui->txtUserNameClean->setText(App::UserNameClean);
    ui->txtUserPwdClean->setText(App::UserPwdClean);
    ui->txtCleanCount->setText(QString::number(App::CleanCount));
    ui->txtCleanInterval->setText(QString::number(App::CleanInterval));

    connect(ui->txtHostNameClean, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtPortClean, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDbNameClean, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserNameClean, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserPwdClean, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtCleanCount, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtCleanInterval, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmDbClean::saveConfig()
{
    App::HostNameClean = ui->txtHostNameClean->text();
    App::PortClean = ui->txtPortClean->text().toInt();
    App::DbNameClean = ui->txtDbNameClean->text();
    App::UserNameClean = ui->txtUserNameClean->text();
    App::UserPwdClean = ui->txtUserPwdClean->text();

    App::CleanCount = ui->txtCleanCount->text().toInt();
    App::CleanCount = App::CleanCount < 100 ? 100 : App::CleanCount;

    App::CleanInterval = ui->txtCleanInterval->text().toInt();
    App::CleanInterval = App::CleanInterval < 1 ? 1 : App::CleanInterval;

    if (isVisible()) {
        DbCleanThread::Instance()->setMaxCount(App::CleanCount);
        DbCleanThread::Instance()->setInterval(App::CleanInterval);
    }

    App::writeConfig();
}

void frmDbClean::loadData()
{
    columnNames.clear();
    columnNames << "记录序号" << "设备编号" << "设备名称" << "设备地址" << "区域名称" << "温度值" << "湿度值" << "电量" << "保存时间" << "记录类型" << "备注";

    columnWidths.clear();
    columnWidths << 80 << 60 << 100 << 60 << 100 << 60 << 60 << 40 << 160 << 80 << 80;

    model = new QSqlTableModel(this, QSqlDatabase::database("dbClean"));
    model->setTable("DeviceLog");
    model->setSort(0, Qt::DescendingOrder);
    ui->tableMain->setModel(model);
    ui->tableMain->setProperty("model", true);

    for (int i = 0; i < columnNames.count(); i++) {
        model->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
        ui->tableMain->setColumnWidth(i, columnWidths.at(i));
    }

    on_btnLoad_clicked();
}

void frmDbClean::debug(const QString &msg)
{
    if (currentCount >= maxCount) {
        ui->txtMain->clear();
        currentCount = 0;
    }

    if (msg.startsWith("重连") || msg.startsWith("提交事务失败")) {
        qDebug() << DATETIME << msg;
    }

    ui->txtMain->append(QString("[%1] %2").arg(DATETIME).arg(msg));
    currentCount++;
}

void frmDbClean::cleanFinsh(bool ok, int cleanCount, double msec)
{
    QString msg = QString("自动清理数据 %1 条数据 %2  用时 %3 秒").arg(cleanCount)
                  .arg(ok ? "成功" : "失败").arg(QString::number(msec / 1000, 'f', 3));
    debug(msg);

    if (ok) {
        on_btnLoad_clicked();
    }
}

void frmDbClean::on_btnConn_clicked()
{
    //打开数据库
    QSqlDatabase dbConn;
    {
        if (App::DbNameClean.endsWith(".db")) {
            dbConn = QSqlDatabase::addDatabase("QSQLITE", "dbClean");
            dbConn.setDatabaseName(QUIHelper::appPath() + App::DbNameClean);
            DbCleanThread::Instance()->setDbType(DbCleanThread::DbType_Sqlite);
        } else {
            dbConn = QSqlDatabase::addDatabase("QMYSQL", "dbClean");
            dbConn.setHostName(App::HostNameClean);
            dbConn.setPort(App::PortClean);
            dbConn.setDatabaseName(App::DbNameClean);
            dbConn.setUserName(App::UserNameClean);
            dbConn.setPassword(App::UserPwdClean);
            DbCleanThread::Instance()->setDbType(DbCleanThread::DbType_MySql);
        }
    }

    bool dbOk = dbConn.open();
    debug(QString("打开数据库%1").arg(dbOk ? "成功" : "失败"));

    if (!dbOk) {
        QSqlDatabase::removeDatabase("dbClean");
        return;
    }

    ui->widgetDb->setEnabled(false);
    ui->widgetServer->setEnabled(true);
    QTimer::singleShot(1000, this, SLOT(loadData()));

    DbCleanThread::Instance()->setPar("dbClean", "DeviceLog", "DeviceLogID", "DeviceLogID", "DeviceLogID desc",
                                      App::CleanCount, App::CleanInterval);

    QTimer::singleShot(3000, this, SLOT(on_btnStart_clicked()));
}

void frmDbClean::on_btnStart_clicked()
{
    if (ui->btnStart->text() == "启动服务") {
        DbCleanThread::Instance()->start();
        ui->btnStart->setText("停止服务");
    } else {
        DbCleanThread::Instance()->stop();
        ui->btnStart->setText("启动服务");
    }
}

void frmDbClean::on_btnLoad_clicked()
{
    //先查询记录编号最大数,超过1000万条就不加载表格数据,超过100万条就不显示具体记录行数
    QString sql = "select max(DeviceLogID) from DeviceLog";
    QSqlQuery query(QSqlDatabase::database("dbClean"));
    query.exec(sql);
    query.next();
    int count = query.value(0).toInt();
    ui->labCount->setText(QString("最大记录号 %1").arg(count));
    qApp->processEvents();

    if (count > (100 * 10000)) {
        QUIHelper::showMessageBoxError("数据量超过100万条, 不加载数据!", 5);
    } else if (count > (10 * 10000)) {
        QUIHelper::showMessageBoxError("数据量超过10万条, 不显示具体行数!", 5);
        model->select();
    } else {
        model->select();

        sql = "select count(*) from DeviceLog";
        query.clear();
        query.exec(sql);
        query.next();
        count = query.value(0).toInt();
        //count = model->rowCount();//mysql 数据库可以直接用这个
        ui->labCount->setText(QString("共 %1 条数据").arg(count));
    }
}

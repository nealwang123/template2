#include "frmdatacreate.h"
#include "ui_frmdatacreate.h"
#include "quiwidget.h"
#include "dbexecsqlthread.h"

frmDataCreate::frmDataCreate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDataCreate)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
}

frmDataCreate::~frmDataCreate()
{
    delete ui;
}

QString frmDataCreate::getSerialNumber()
{
    static int count = 0;
    count++;
    QString serialNumber = QString("%1%2").arg(QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")).arg(count, 3, 10, QChar('0'));
    return serialNumber;
}

void frmDataCreate::initForm()
{
    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(dataCreate()));

    maxCount = 10;
    currentCount = 0;
    ui->widgetDb->setEnabled(true);
    ui->widgetServer->setEnabled(false);

    ui->tableMain->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableMain->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableMain->horizontalHeader()->setStretchLastSection(true);
    ui->tableMain->verticalHeader()->setDefaultSectionSize(25);
}

void frmDataCreate::initConfig()
{
    ui->txtHostNameData->setText(App::HostNameData);
    ui->txtPortData->setText(QString::number(App::PortData));
    ui->txtDbNameData->setText(App::DbNameData);
    ui->txtUserNameData->setText(App::UserNameData);
    ui->txtUserPwdData->setText(App::UserPwdData);
    ui->txtDataCount->setText(QString::number(App::DataCount));
    ui->txtDataInterval->setText(QString::number(App::DataInterval));
    //
    qDebug()<<"App::DbNameData================="<<App::DbNameData;

    connect(ui->txtHostNameData, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtPortData, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDbNameData, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserNameData, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtUserPwdData, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDataCount, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
    connect(ui->txtDataInterval, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));
}

void frmDataCreate::saveConfig()
{
    App::HostNameData = ui->txtHostNameData->text();
    App::PortData = ui->txtPortData->text().toInt();
    App::DbNameData = ui->txtDbNameData->text();
    App::UserNameData = ui->txtUserNameData->text();
    App::UserPwdData = ui->txtUserPwdData->text();

    App::DataCount = ui->txtDataCount->text().toInt();
    App::DataCount = App::DataCount < 10 ? 10 : App::DataCount;

    App::DataInterval = ui->txtDataInterval->text().toInt();
    App::DataInterval = App::DataInterval < 1000 ? 1000 : App::DataInterval;

    if (timer->interval() != App::DataInterval) {
        timer->setInterval(App::DataInterval);
    }

    App::writeConfig();
}

void frmDataCreate::loadData()
{
    model = new QSqlTableModel(this, QSqlDatabase::database("dbData"));
    model->setTable(tableName);

    if (App::DbNameData.toUpper() == "/TA.DB") {
        columnNames.clear();
        columnNames << "防区号" << "防区名称" << "设备IP" << "日志类型" << "事件内容" << "触发时间" << "告警详情" << "告警数据" << "告警图像";

        columnWidths.clear();
        columnWidths << 70 << 120 << 120 << 80 << 150 << 160 << 160 << 160 << 160;

        model->setSort(5, Qt::DescendingOrder);
    } else if (App::DbNameData.toUpper() == "/VMS.DB") {
        columnNames.clear();
        columnNames << "编号" << "访客单号" << "照片类型" << "进入门" << "进入时间" << "进入照片";
        columnNames << "离开门" << "离开时间" << "离开照片" << "姓名" << "性别" << "出生年月" << "地址";

        columnWidths.clear();
        columnWidths << 60 << 180 << 70 << 80 << 150 << 70 << 80 << 150 << 70 << 80 << 60 << 80 << 150;

        model->setSort(0, Qt::DescendingOrder);
    } else {
        columnNames.clear();
        columnNames << "记录序号" << "设备编号" << "设备名称" << "设备地址" << "区域名称" << "温度值" << "湿度值" << "电量" << "保存时间" << "记录类型" << "备注";

        columnWidths.clear();
        columnWidths << 80 << 60 << 100 << 60 << 100 << 60 << 60 << 40 << 160 << 80 << 80;

        model->setSort(0, Qt::DescendingOrder);
    }

    ui->tableMain->setModel(model);
    ui->tableMain->setProperty("model", true);

    for (int i = 0; i < columnNames.count(); i++) {
        model->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
        ui->tableMain->setColumnWidth(i, columnWidths.at(i));
    }

    on_btnLoad_clicked();
}

void frmDataCreate::dataCreate()
{
    QStringList sqls;

    if (App::DbNameData.toUpper() == "/TA.DB") {
        QStringList logContents;
        logContents << "防区开路告警" << "防区丢失告警" << "防区开路故障" << "防区丢失故障" << "防区恢复正常" << "防区旁路";
        for (int i = 0; i < App::DataCount; i++) {
            int temp1 = rand() % 10;
            int temp2 = rand() % 6;

            QString defenceID = QString("%1").arg(temp1 + 10, 3, 10, QChar('0'));
            QString defenceName = QString("防区%1").arg(defenceID);
            QString logContent = logContents.at(temp2);

            QString sql = QString("insert into LogInfo(DefenceID,DefenceName,DeviceIP,LogType,LogContent,TriggerTime)"
                                  " values('%1','%2','192.168.1.255','主机上报','%3','%4')")
                          .arg(defenceID).arg(defenceName).arg(logContent).arg(DATETIME);

            sqls.append(sql);
        }
    } else if (App::DbNameData.toUpper() == "/VMS.DB") {
        for (int i = 0; i < App::DataCount; i++) {
            QString sql = "insert into VisitorInfo(SerialNumber,PhotoType,EnterDoor,EnterTime,EnterPhoto,"
                          "VisitorName,VisitorSex,VisitorNation,VisitorBirthday,VisitorAddress,VisitorCardID,"
                          "VisitorNumber,VisitorCompany,VisitorTel,VisitorCarNumber,VisitorType,CardType,CardNumber,"
                          "PeopleName,PeopleSection,PeopleNum,PeopleTel,PeopleID,PeopleOther) values('";
            sql += getSerialNumber() + "','";
            sql += QString("%1").arg("拍照") + "','";
            sql += QString("%1").arg("前门") + "','";
            sql += QString("%1").arg(DATETIME) + "','";
            sql += QString("%1").arg("") + "','";
            sql += QString("%1").arg("张三") + "','";
            sql += QString("%1").arg("男") + "','";
            sql += QString("%1").arg("汉族") + "','";
            sql += QString("%1").arg("1990-01-10") + "','";
            sql += QString("%1").arg("某省某县") + "','";
            sql += QString("%1").arg("1111111111111111111") + "','";
            sql += QString("%1").arg("1") + "','";
            sql += QString("%1").arg("**科技") + "','";
            sql += QString("%1").arg("111111") + "','";
            sql += QString("%1").arg("1111111") + "','";
            sql += QString("%1").arg("访客") + "','";
            sql += QString("%1").arg("身份证") + "','";
            sql += QString("%1").arg("22928254") + "','";
            sql += QString("%1").arg("校长") + "','";
            sql += QString("%1").arg("校长") + "','";
            sql += QString("%1").arg("021-88888888") + "','";
            sql += QString("%1").arg("13888888888") + "','";
            sql += QString("%1").arg("888") + "','";
            sql += QString("%1").arg("备注") + "')";

            sqls.append(sql);
        }
    } else {
        for (int i = 0; i < App::DataCount; i++) {
            int temp1 = rand() % 50;
            int temp2 = rand() % 10;
            int temp3 = rand() % 80;
            int temp4 = rand() % 10;
            double powerValue = rand() % 5;

            if (temp1 < 10) {
                temp1 += 10;
            }

            if (temp3 < 10) {
                temp3 += 10;
            }

            if (powerValue == 0) {
                powerValue = 1;
            }

            double tempValue = (double)temp1 + (double)temp2 / 10;
            double dampValue = (double)temp3 + (double)temp4 / 10;

            deviceLogID++;
            QString sql = QString("insert into DeviceLog(DeviceLogID,DeviceID,DeviceName,DeviceAddr,GroupName,TempValue,DampValue,PowerValue,SaveTime,Type)"
                                  " values(%1,'%4','默认端口-%4','%4','默认区域','%2','%3','%4','%5','运行记录')")
                          .arg(deviceLogID).arg(tempValue).arg(dampValue).arg(powerValue).arg(DATETIME);

            sqls.append(sql);
        }
    }

    DbExecSqlThread *dbExecSqlThread = new DbExecSqlThread(this);
    connect(dbExecSqlThread, SIGNAL(finsh(bool, quint32, double)), this, SLOT(finsh(bool, quint32, double)));
    dbExecSqlThread->setConnName("dbData");
    dbExecSqlThread->setSqls(sqls);
    dbExecSqlThread->start();

    timer->setInterval(App::DataInterval);
}

void frmDataCreate::debug(const QString &msg)
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

void frmDataCreate::finsh(bool ok, quint32 count, double msec)
{
    debug(QString("随机插入 %1 条数据 %2  用时 %3 秒").arg(count)
          .arg(ok ? "成功" : "失败").arg(QString::number(msec / 1000, 'f', 3)));

    //如果上一次执行时间超过定时器的间隔则定时器间隔加大
    if (msec > timer->interval()) {
        timer->setInterval(msec + 1000);
    }
}

void frmDataCreate::on_btnConn_clicked()
{
    //打开数据库
    QSqlDatabase dbConn;
    {
        if (App::DbNameData.endsWith(".db")) {
            dbConn = QSqlDatabase::addDatabase("QSQLITE", "dbData");
            dbConn.setDatabaseName(QUIHelper::appPath() + App::DbNameData);
        } else {
            dbConn = QSqlDatabase::addDatabase("QMYSQL", "dbData");
            dbConn.setHostName(App::HostNameData);
            dbConn.setPort(App::PortData);
            dbConn.setDatabaseName(App::DbNameData);
            dbConn.setUserName(App::UserNameData);
            dbConn.setPassword(App::UserPwdData);
        }
    }

    bool dbOk = dbConn.open();
    debug(QString("打开数据库%1").arg(dbOk ? "成功" : "失败"));

    if (!dbOk) {
        QSqlDatabase::removeDatabase("dbData");
        return;
    }

    if (App::DbNameData.toUpper() == "/TA.DB") {
        tableName = "LogInfo";
        countName = "rowid";
    } else if (App::DbNameData.toUpper() == "/VMS.DB") {
        tableName = "VisitorInfo";
        countName = "ID";
    } else {
        tableName = "DeviceLog";
        countName = "DeviceLogID";
    }

    //查询最大记录编号,防止重复
    QString sql = QString("select max(%1) from %2").arg(countName).arg(tableName);
    QSqlQuery query(QSqlDatabase::database("dbData"));
    query.exec(sql);
    query.next();
    deviceLogID = query.value(0).toInt();

    ui->widgetDb->setEnabled(false);
    ui->widgetServer->setEnabled(true);
    QTimer::singleShot(1000, this, SLOT(loadData()));

    on_btnStart_clicked();
}

void frmDataCreate::on_btnStart_clicked()
{
    if (ui->btnStart->text() == "启动服务") {
        timer->start();
        ui->btnStart->setText("停止服务");
    } else {
        timer->stop();
        ui->btnStart->setText("启动服务");
    }
}

void frmDataCreate::on_btnLoad_clicked()
{
    //先查询记录编号最大数,超过1000万条就不加载表格数据,超过100万条就不显示具体记录行数
    QString sql = QString("select max(%1) from %2").arg(countName).arg(tableName);
    QSqlQuery query(QSqlDatabase::database("dbData"));
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

        sql = QString("select count(*) from %1").arg(tableName);
        query.clear();
        query.exec(sql);
        query.next();
        count = query.value(0).toInt();
        //count = model->rowCount();//mysql 数据库可以直接用这个
        ui->labCount->setText(QString("共 %1 条数据").arg(count));
    }
}

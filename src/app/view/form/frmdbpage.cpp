#include "frmdbpage.h"
#include "ui_frmdbpage.h"
#include "quiwidget.h"
#include "dbpage.h"
#include "dbdelegate.h"

frmDbPage::frmDbPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmDbPage)
{
    ui->setupUi(this);
    this->initForm();
}

frmDbPage::~frmDbPage()
{
    delete ui;
}

void frmDbPage::initForm()
{
    columnNames.clear();
    columnWidths.clear();

    if (App::LocalDBName.toUpper() == "/TA.DB") {
        tableName = "LogInfo";
        countName = "rowid";

        columnNames.append("防区号");
        columnNames.append("防区名称");
        columnNames.append("设备IP");
        columnNames.append("日志类型");
        columnNames.append("事件内容");
        columnNames.append("触发时间");
        columnNames.append("告警详情");
        columnNames.append("告警数据");
        columnNames.append("告警图像");

        columnWidths.append(70);
        columnWidths.append(120);
        columnWidths.append(120);
        columnWidths.append(80);
        columnWidths.append(150);
        columnWidths.append(160);
        columnWidths.append(160);
        columnWidths.append(160);
        columnWidths.append(160);
    } else {
        tableName = "DeviceLog";
        countName = "DeviceLogID";

        columnNames.append("记录序号");
        columnNames.append("设备编号");
        columnNames.append("设备名称");
        columnNames.append("设备地址");
        columnNames.append("区域名称");
        columnNames.append("温度值");
        columnNames.append("湿度值");
        columnNames.append("电量");
        columnNames.append("保存时间");
        columnNames.append("记录类型");
        columnNames.append("备注");

        columnWidths.append(80);
        columnWidths.append(60);
        columnWidths.append(100);
        columnWidths.append(60);
        columnWidths.append(100);
        columnWidths.append(60);
        columnWidths.append(60);
        columnWidths.append(40);
        columnWidths.append(160);
        columnWidths.append(80);
        columnWidths.append(80);
    }

    //加载远程数据库服务器名称列表
    QStringList serverName;
    serverName << "qt_sql_default_connection" << "dbClean" << "dbClient" << "dbServer" << "dbLocal" << "dbNet" << "dbData";
    ui->cboxServerName->addItems(serverName);

    //设置需要显示数据的表格和翻页的按钮
    dbPage = new DbPage(this);
    //设置所有列居中显示
    dbPage->setAllCenter(true);
    dbPage->setControl(ui->tableMain, ui->labPageCount, ui->labPageCurrent, ui->labResultCount, ui->labResultCurrent, ui->labResult, 0,
                       ui->btnFirst, ui->btnPre, ui->btnNext, ui->btnLast, countName, ui->cboxServerName->currentText());
    ui->tableMain->horizontalHeader()->setStretchLastSection(true);
    ui->tableMain->verticalHeader()->setDefaultSectionSize(25);

#if 1
    //要插入的列索引,可自行更改
    int column = 10;

    //增加新列用于显示校验列
    dbPage->setInsertColumnIndex(column);
    dbPage->setInsertColumnName("告警");
    dbPage->setInsertColumnWidth(50);

    //增加一列第10列,对第6列数据进行校验,校验规则 >20 即温度值>20则显示异常
    DbDelegate *dbDelegate = new DbDelegate(this);
    //设置校验列
    dbDelegate->setCheckColumn(column);
    //设置校验参照列,即从那一列取值
    dbDelegate->setCheckTarget(6);
    //设置校验规则
    dbDelegate->setCheckType(">");
    //设置校验值
    dbDelegate->setCheckValue("20");
    //如果不需要显示校验失败的图标则设置该图标为空即可
    //check->setCheckNoImage("");
    ui->tableMain->setItemDelegateForColumn(column, dbDelegate);
#endif
}

void frmDbPage::on_cboxServerName_activated(const QString &arg1)
{
    dbPage->setConnName(arg1);
    on_btnSelect_clicked();
}

void frmDbPage::on_btnSelect_clicked()
{
    ui->labResult->setText("正在查询...");

    //绑定数据到表格
    QString sql = "where 1=1";
    dbPage->setTableName(tableName);
    dbPage->setOrderSql(QString("%1 %2").arg(countName).arg("asc"));
    dbPage->setWhereSql(sql);
    dbPage->setResultCurrent(50);
    dbPage->setColumnNames(columnNames);
    dbPage->setColumnWidths(columnWidths);
    dbPage->select();
}

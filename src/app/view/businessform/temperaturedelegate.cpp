#include "temperaturedelegate.h"
#include "ui_temperaturedelegate.h"

TemperatureDelegate::TemperatureDelegate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TemperatureDelegate)
{
    ui->setupUi(this);
    createConnection();
    this->initForm("TemperatureTest");
}

TemperatureDelegate::~TemperatureDelegate()
{
    delete ui;
}
int TemperatureDelegate::addRow(){
    int count = model->rowCount();
    model->insertRow(count);
    ui->tableMain->setCurrentIndex(model->index(count, 0));
    return count;
}
void TemperatureDelegate::setData(int row,int col,QString data){
    model->setData(model->index(row, col), data);
}

bool TemperatureDelegate:: createConnection()
{

    if (QSqlDatabase::contains("qt_sql_default_connection_")){
        _db = QSqlDatabase::database("qt_sql_default_connection_");
    }else{
        //建立和sqlite数据的连接
        _db = QSqlDatabase::addDatabase("QSQLITE","qt_sql_default_connection_");
        //设置数据库文件的名字
//         _db.setDatabaseName(QUIHelper::appPath() + App::LocalDBName);
        _db.setDatabaseName(QUIHelper::appPath() +"/targetDb.db");
    }
   QString path=QUIHelper::appPath() + App::LocalDBName;
   //自定义文件
   path=QUIHelper::appPath() + "/targetDb.db";
   bool ret;
   if(QFile::exists(path))
   {
       // 存在则打开
      ret=_db.open();
   }else{
       // 不存在打开并创建数据库表 补充：SQLite是基于文件的数据库，当打开时如果数据库文件不存在将自动创建一个
       ret=-1;
//        ret=_db.open();
   //        QSqlQuery query;
   //        // set为sqlite关键字，不能使用
   //        bool bRet = query.exec("create table init(" \
   //                               "no   INT    PRIMARY KEY NOT NULL,"\
   //                               "name TEXT   NOT NULL,"\
   //                               "content TEXT,"\
   //                               "description TEXT"\
   //                               ");");
   //        if(!bRet) {
   //            _db.close();
   //            QFile::remove(path);
   //        }else{
   //            // 此处演示了三种不同insert插入方法
   //            query.exec("insert into init values" \
   //                       "(1, '启动界面背景', 'images/background.jpg', '请使用1920*1080的图片，图片格式可以为png、jpg。');");
   //            query.exec("insert into init       " \
   //                       "(no,name,content,description) values(2, '欢迎视频', '','进入启动界面后，循环播放的视频,必须为mp4格式，如果为空则循环播放欢迎音乐');");
   //            query.exec("insert into init       " \
   //                       "(no,name,description) values (3, '欢迎音乐','进去启动界面后，循环播放的音频文件，可选择mp3,wav格式');");
   //        }

   }
   if(ret == false){
       qDebug()<<tr("TemperatureDelegate连接数据失败")<<endl;
       return false;
   }
   qDebug()<<tr("TemperatureDelegate连接数据库成功")<<endl;
   return true;

}
void TemperatureDelegate::initForm(QString fileName)
{
   ui->tableMain->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableMain->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->tableMain->horizontalHeader()->setStretchLastSection(true);
   ui->tableMain->verticalHeader()->setDefaultSectionSize(25);
   ui->tableMain->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);

   model = new QSqlTableModel(this,_db);
   model->setEditStrategy(QSqlTableModel::OnManualSubmit);
   //设置表名
//    model->setTable("PortInfo");
//    =;
   model->setTable(fileName);
   model->setSort(0,Qt::AscendingOrder);
   model->select();
   ui->tableMain->setModel(model);
   ui->tableMain->setProperty("model", true);

   columnNames.clear();

//    columnNames << "端口编号" << "端口名称" << "连接类型" << "通讯方式" << "串口号" << "波特率" << "IP地址" << "通讯端口" << "采集周期(秒)" << "通讯超时(次)";
//   columnNames << "序号" << "字段个数" << "设备类型" << "设备编号"<< "通道号" << "波特率"<< "dbc路径" << "用途策略" << "工作模式"<<"滤波模式"
//               <<"AccCode"<<"AccMask"<<"滤波使能"<<"设备名称"<<"备注" ;
   columnNames<<"序号"<<"雷达编号"<<"原始数据"<<"帧ID"<<"VCO温度"<<"seq1"<< "seq2"
               <<"seq3"<<"详细结果"<<"结果"<<"日期"<<"温度";
   columnWidths.clear();
   columnWidths <<50<<60<<160<<60<<60<<60<<60<<60<<120<<60<<140<<60;
   if(columnNames.length()!=columnWidths.length()){
       QUIHelper::showMessageBoxError(QString("columnWidths!=columnNames:columnWidths=%1 columnNames=%2").arg(columnWidths.length()).arg(columnNames.length()));
       return;
   }
   for (int i = 0; i < columnNames.count(); i++) {
       model->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
       ui->tableMain->setColumnWidth(i, columnWidths.at(i));
   }
   qDebug()<<columnNames;
   qDebug()<<columnWidths;

//    //端口编号委托
//    QStringList portID;
//    for (int i = 1; i <= 255; i++) {
//        portID.append(QString::number(i));
//    }

//    DbDelegate *d_cbox_portID = new DbDelegate(this);
//    d_cbox_portID->setDelegateType("QComboBox");
//    d_cbox_portID->setDelegateValue(portID);
//    ui->tableMain->setItemDelegateForColumn(0, d_cbox_portID);

//    //连接类型委托
//    QStringList connectType;
//    connectType << "Com_RS232" << "Com_Tcp_Client" << "Com_Tcp_Server" << "Tcp_Client" << "Tcp_Server";

//    DbDelegate *d_cbox_connectType = new DbDelegate(this);
//    d_cbox_connectType->setDelegateType("QComboBox");
//    d_cbox_connectType->setDelegateValue(connectType);
//    ui->tableMain->setItemDelegateForColumn(2, d_cbox_connectType);

//    //通讯方式委托
//    QStringList connectMode;
//    connectMode << "采集" << "上报";

//    DbDelegate *d_cbox_connectMode = new DbDelegate(this);
//    d_cbox_connectMode->setDelegateType("QComboBox");
//    d_cbox_connectMode->setDelegateValue(connectMode);
//    ui->tableMain->setItemDelegateForColumn(3, d_cbox_connectMode);

//    //端口号委托
//    QStringList comName;
//    for (int i = 1; i <= 20; i++) {
//        comName << QString("COM%1").arg(i);
//    }

//    DbDelegate *d_cbox_comName = new DbDelegate(this);
//    d_cbox_comName->setDelegateType("QComboBox");
//    d_cbox_comName->setDelegateEdit(true);
//    d_cbox_comName->setDelegateValue(comName);
//    ui->tableMain->setItemDelegateForColumn(4, d_cbox_comName);
}
void TemperatureDelegate::on_btnAdd_clicked()
{

       int count = model->rowCount();
       model->insertRow(count);

       int prePortID = model->index(count - 1, 0).data().toString().replace("0x","").toInt(nullptr,16)+1;

       QString prePortName = model->index(count - 1, 1).data().toString();
       QString preConnectType = model->index(count - 1, 2).data().toString();
       QString preConnectMode = model->index(count - 1, 3).data().toString();
       QString preComName = model->index(count - 1, 4).data().toString();
       QString preBaudRate = model->index(count - 1, 5).data().toString();
       QString preTcpIP = model->index(count - 1, 6).data().toString();
       QString preTcpPort = model->index(count - 1, 7).data().toString();
       int preReadInterval = model->index(count - 1, 8).data().toInt();
       int preReadTimeout = model->index(count - 1, 9).data().toInt();

       QStringList list = preTcpIP.split(".");
       if (list.count() == 4) {
           preTcpIP = QString("%1.%2.%3.%4").arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3).toInt() + 1);
       }

       if (count == 0) {
           prePortID = 1;
           prePortName = "默认端口1";
           preConnectType = "Tcp_Client";
           preConnectMode = "采集";
           preTcpIP = "192.168.1.53";
           preTcpPort = 502;
           preReadInterval = 2;
           preReadTimeout = 3;
       }

       //设置新增加的行默认值
       model->setData(model->index(count, 0), QString("0x%1").arg(prePortID,3,16,QChar('0')));
       model->setData(model->index(count, 1), prePortName);
       model->setData(model->index(count, 2), preConnectType);
       model->setData(model->index(count, 3), preConnectMode);
       model->setData(model->index(count, 4), preComName);
       model->setData(model->index(count, 5), preBaudRate);
       model->setData(model->index(count, 6), preTcpIP);
       model->setData(model->index(count, 7), preTcpPort);
       model->setData(model->index(count, 8), preReadInterval);
       model->setData(model->index(count, 9), preReadTimeout);

       ui->tableMain->setCurrentIndex(model->index(count, 0));

}

void TemperatureDelegate::on_btnSave_clicked()
{
   //启用数据库事务加快执行速度
   model->database().transaction();

   if (model->submitAll()) {
       model->database().commit();
   } else {
       model->database().rollback();
       QUIHelper::showMessageBoxError("保存设备信息失败,设备信息不能为空,请重新填写!");
   }
}

void TemperatureDelegate::on_btnDelete_clicked()
{
    if (ui->tableMain->currentIndex().row() < 0) {
        QUIHelper::showMessageBoxError("请选择要删除的内容!");
        return;
    }
    if (ui->tableMain->currentIndex().row() == 0) {
        QUIHelper::showMessageBoxError("第一行不能删除!");
        return;
    }


   int row = ui->tableMain->currentIndex().row();
   model->removeRow(row);
   model->submitAll();
   ui->tableMain->setCurrentIndex(model->index(model->rowCount() - 1, 0));
}

void TemperatureDelegate::on_btnReturn_clicked()
{
   model->revertAll();
}

void TemperatureDelegate::on_btnClear_clicked()
{
   if (model->rowCount() <= 0) {
       return;
   }

   if (QUIHelper::showMessageBoxQuestion("确定要清空所有端口信息吗?") == QMessageBox::Yes) {
       QString sql = "delete from PortInfo";
       QSqlQuery query;
       query.exec(sql);
       model->select();
   }
}

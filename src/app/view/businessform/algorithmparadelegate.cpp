#include "algorithmparadelegate.h"
#include "ui_algorithmparadelegate.h"
static bool createConnectionDateBase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");; //创建QSQLdatabase对象，指定QT使用的哪一种SQL。
    db.setDatabaseName("testSQL.db");
    if(!db.open())
    {
        QMessageBox::critical(NULL,"数据库打开失败","打开数据库失败！",
                              QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;
    query.exec("create table TEST_SQL (id int(1) primary key,name char(200),age int(1),chenked int (1))");

    query.exec("insert into TEST_SQL values(1,'测试1',18,0)");
    query.exec("insert into TEST_SQL values(2,'测试2',19,0)");
    query.exec("insert into TEST_SQL values(3,'测试3',20,0)");
    return true;
}
AlgorithmParaDelegate::AlgorithmParaDelegate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlgorithmParaDelegate)
{
    ui->setupUi(this);
    createConnection();
    this->initForm("Algorithm");

}

AlgorithmParaDelegate::~AlgorithmParaDelegate()
{
    delete ui;
}
int AlgorithmParaDelegate::addRow(){
    int count = model->rowCount();
    on_btnAdd_clicked();
    return count;
}
int AlgorithmParaDelegate::getRow(){
    return model->rowCount();
}
int AlgorithmParaDelegate::getType(int row){
    return model->record(row).value(TYPEINDEX).toInt();
}
void AlgorithmParaDelegate::setData(int row,int col,QString data){
    bool ret=false;
    if(col==0){
        ret=model->setData(model->index(row, col), data,Qt::CheckStateRole);
    }else{
        ret=model->setData(model->index(row, col), data);

    }
}

bool AlgorithmParaDelegate:: createConnection()
{

    if (QSqlDatabase::contains("qt_sql_default_connection_")){
        _db = QSqlDatabase::database("qt_sql_default_connection_");
        qDebug()<<"QSqlDatabase::contains";
    }else{
        //建立和sqlite数据的连接
        _db = QSqlDatabase::addDatabase("QSQLITE","qt_sql_default_connection_");
        //设置数据库文件的名字
//         _db.setDatabaseName(QUIHelper::appPath() + App::LocalDBName);
        _db.setDatabaseName(QUIHelper::appPath() +"/targetDb.db");
        qDebug()<<"!QSqlDatabase::contains";
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
       qDebug()<<tr("AlgorithmParaDelegate连接数据失败")<<endl;
       return false;
   }
   qDebug()<<tr("AlgorithmParaDelegate连接数据库成功")<<endl;
   return true;

}
void AlgorithmParaDelegate::initForm(QString fileName)
{
   ui->tableMain->setSelectionBehavior(QAbstractItemView::SelectRows);
   ui->tableMain->setSelectionMode(QAbstractItemView::SingleSelection);
   ui->tableMain->horizontalHeader()->setStretchLastSection(true);
   ui->tableMain->verticalHeader()->setDefaultSectionSize(25);
   ui->tableMain->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
   model = new QSqlTableModel(this,_db);
   qDebug()<<"_db"<<_db.connectionNames();
   model->setEditStrategy(QSqlTableModel::OnManualSubmit);
   //设置表名
   model->setTable(fileName);
   model->setSort(0,Qt::AscendingOrder);
   model->select();
   ui->tableMain->setModel(model);
   ui->tableMain->setProperty("model", true);

   columnNames.clear();
   columnNames<<"ID"<<"使能"<<"索引"<<"读原始值"<<"读真实值"<<"校对内容"<<"类型"<< "描述";
   columnWidths.clear();
   columnWidths <<30<<60<<60<<100<<60<<100<<20<<100;
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
       QStringList connectType;
       connectType << "true" << "false";

    DbDelegate *d_cbox_portID = new DbDelegate(this);
    d_cbox_portID->setDelegateType("QCheckBox");
    d_cbox_portID->setDelegateValue(connectType);
    ui->tableMain->setItemDelegateForColumn(1, d_cbox_portID);

//    //连接类型委托
//    QStringList connectType;
//    connectType << "true" << "false";

//    DbDelegate *d_cbox_connectType = new DbDelegate(this);
//    d_cbox_connectType->setDelegateType("QCheckBox");
//    d_cbox_connectType->setDelegateValue(connectType);
//    ui->tableMain->setItemDelegateForColumn(5, d_cbox_connectType);

//    //通讯方式委托
//    QStringList connectMode;
//    connectMode << "采集" << "上报";

//    DbDelegate *d_cbox_connectMode = new DbDelegate(this);
//    d_cbox_connectMode->setDelegateType("QComboBox");
//    d_cbox_connectMode->setDelegateValue(connectMode);
//    ui->tableMain->setItemDelegateForColumn(6, d_cbox_connectMode);

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
void AlgorithmParaDelegate::on_btnAdd_clicked()
{

       int count = model->rowCount();
       model->insertRow(count);

       int preID = model->index(count - 1, 0).data().toString().toInt(nullptr,10)+1;
       QString preEnable = model->index(count - 1, 1).data().toString();
       int preIndex = model->index(count - 1, 2).data().toString().toInt(nullptr,10)+2;
       QString preRaw = model->index(count - 1, 3).data().toString();
       QString preReal = model->index(count - 1, 4).data().toString();
       QString preCompare = model->index(count - 1, 5).data().toString();
       QString preType = model->index(count - 1, 6).data().toString();
       QString preDescription = "描述，待更新";

       if (count == 0) {
           preID = 0;
           preEnable = "false";
           preIndex = 0;
           preRaw= "00 00 00 00";
           preReal = "0";
           preCompare = "0";
           preType = "0";
           preDescription = "描述，待更新";
       }

       //设置新增加的行默认值
       model->setData(model->index(count, 0), QString("%1").arg(preID));
       model->setData(model->index(count, 1), preEnable);
       model->setData(model->index(count, 2), QString("%1").arg(preIndex));
       model->setData(model->index(count, 3), preRaw);
       model->setData(model->index(count, 4), preReal);
       model->setData(model->index(count, 5), preCompare);
       model->setData(model->index(count, 6), preType);
       model->setData(model->index(count, 7), preDescription);


       ui->tableMain->setCurrentIndex(model->index(count, 0));

}

void AlgorithmParaDelegate::on_btnSave_clicked()
{
   bool ret=false;
   //启用数据库事务加快执行速度
   model->database().transaction();

   if (model->submitAll()) {
       ret=model->database().commit();
   } else {
       ret=model->database().rollback();
       QUIHelper::showMessageBoxError("保存信息失败,信息不能为空,请重新填写!");
   }
   qDebug()<<"ret=========="<<ret;
}

void AlgorithmParaDelegate::on_btnDelete_clicked()
{
   if (ui->tableMain->currentIndex().row() < 0) {
       QUIHelper::showMessageBoxError("请选择要删除的设备!");
       return;
   }
   int row = ui->tableMain->currentIndex().row();
   model->removeRow(row);
   model->submitAll();
   ui->tableMain->setCurrentIndex(model->index(model->rowCount() - 1, 0));
}

void AlgorithmParaDelegate::on_btnReturn_clicked()
{
   model->revertAll();
}

void AlgorithmParaDelegate::on_btnClear_clicked()
{
    if (ui->tableMain->currentIndex().row() < 0) {
        QUIHelper::showMessageBoxError("请选择要删除的内容!");
        return;
    }
    if (ui->tableMain->currentIndex().row() == 0) {
        QUIHelper::showMessageBoxError("第一行不能删除!");
        return;
    }


    if (QUIHelper::showMessageBoxQuestion("确定要清空所有端口信息吗?") == QMessageBox::Yes) {
       QString sql = "delete from PortInfo";
       QSqlQuery query;
       query.exec(sql);
       model->select();
    }
}

QList<AlgoPara > AlgorithmParaDelegate::debugData(){
    QList<AlgoPara > list;
    list.clear();
    AlgoPara para;
    //遍历所有，查找可更新参数表
    for(int i=0;i<model->rowCount();i++){
        QString test= model->index(i,SENDENABLEINDEX).data().toString();
        QString index=model->index(i,SENDINDEX).data().toString();
        QString sendstr=model->index(i,REALINDEX).data().toString();
        QString type= model->index(i,TYPEINDEX).data().toString();
        QString description = model->index(i,TYPEINDEX+1).data().toString();

        para.enable=test;
        para._index=index.toInt();
        para._type=type.toInt();
        para.realstr=sendstr;
        para.description=description;
        list.append(para);
    }
    return list;
    //更新参数
}

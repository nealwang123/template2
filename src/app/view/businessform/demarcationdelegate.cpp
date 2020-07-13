#include "demarcationdelegate.h"
#include "ui_demarcationdelegate.h"
#include "quiwidget.h"
#include "dbdelegate.h"

DemarcationDelegate::DemarcationDelegate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DemarcationDelegate)
{
    ui->setupUi(this);
    createConnection();
    this->initForm("demarcation");
    demarcationTimer=new QTimer(this);
    connect(demarcationTimer,&QTimer::timeout,this,&DemarcationDelegate::slot_demarcationTimer);

}

DemarcationDelegate::~DemarcationDelegate()
{
    delete ui;
}

void DemarcationDelegate::slot_demarcationTimer(){

}
void DemarcationDelegate::startDemarcation(QStringList list){
    demarcationIndex=0;
    qDebug()<<"model->rowCount():"<<model->rowCount();
    for(int i=0;i<model->rowCount();i++){
        QString idstr=model->record(i).value("ID(Hex)").toString();
        QString dirstr=model->record(i).value("传输方向").toString();
        QString headstr=model->record(i).value("协议头").toString();
        QString datastr=model->record(i).value("数据").toString();
        QString tailstr=model->record(i).value("协议尾").toString();
        QString data;
        data.clear();
        if(headstr!="null"){
            data.append(headstr);
            data.append(" ");
        }
        data.append(datastr);
        data.append(" ");
        qDebug()<<"data"<<data<<"len"<<data.split(" ").length();
        int len=data.split(" ").length()-1;
        for(int count =0;count<(8-(len));count++){

            data.append(tailstr);
            data.append(" ");
        }

        //TOPC 4  0  0  time  ID DataLen Data.  index
        //TOPC %1 %2 %3  %4   %5 %6      %7   %8
        qDebug()<<QString("TORADAR %1 %2 %3 %4 %5 %6 %7 %8")
                  .arg(list.at(0))
                  .arg(list.at(1))
                  .arg(list.at(2))
                  .arg(0)
                  .arg(idstr)
                  .arg(8)//
                  .arg(data)
                  .arg(0);
    }


}
/********************
*
*******************/
 bool DemarcationDelegate:: createConnection()
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
        qDebug() << "DemarcationDelegate连接数据失败！";
        return false;
    }
    qDebug() << "DemarcationDelegate连接数据库成功";
    return true;

}
void DemarcationDelegate::initForm(QString fileName)
{
    //更新配置信息
    ui->lineEdit->setText(App::AlignAngle);
    ui->lineEdit_2->setText(App::AlignDistance);
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
    columnNames << "索引" << "描述" << "传输方向" << "步骤"<< "类型" << "ID(Hex)"<< "协议头" << "数据" << "协议尾"<<"回执"<<"延时"<<"校验结果" ;
    columnWidths.clear();
    columnWidths <<35<<40<<40<<120<<60<<60<<60<<120<< 60<<60<<60<<60 ;

    for (int i = 0; i < columnNames.count(); i++) {
        model->setHeaderData(i, Qt::Horizontal, columnNames.at(i));
        ui->tableMain->setColumnWidth(i, columnWidths.at(i));
    }

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

void DemarcationDelegate::on_btnAdd_clicked()
{

//        int count = model->rowCount();
//        model->insertRow(count);

//        int prePortID = model->index(count - 1, 0).data().toString().replace("0x","").toInt(nullptr,16)+1;

//        QString prePortName = model->index(count - 1, 1).data().toString();
//        QString preConnectType = model->index(count - 1, 2).data().toString();
//        QString preConnectMode = model->index(count - 1, 3).data().toString();
//        QString preComName = model->index(count - 1, 4).data().toString();
//        QString preBaudRate = model->index(count - 1, 5).data().toString();
//        QString preTcpIP = model->index(count - 1, 6).data().toString();
//        QString preTcpPort = model->index(count - 1, 7).data().toString();
//        int preReadInterval = model->index(count - 1, 8).data().toInt();
//        int preReadTimeout = model->index(count - 1, 9).data().toInt();

//        QStringList list = preTcpIP.split(".");
//        if (list.count() == 4) {
//            preTcpIP = QString("%1.%2.%3.%4").arg(list.at(0)).arg(list.at(1)).arg(list.at(2)).arg(list.at(3).toInt() + 1);
//        }

//        if (count == 0) {
//            prePortID = 1;
//            prePortName = "默认端口1";
//            preConnectType = "Tcp_Client";
//            preConnectMode = "采集";
//            preTcpIP = "192.168.1.53";
//            preTcpPort = 502;
//            preReadInterval = 2;
//            preReadTimeout = 3;
//        }

//        //设置新增加的行默认值
//        model->setData(model->index(count, 0), QString("0x%1").arg(prePortID,3,16,QChar('0')));
//        model->setData(model->index(count, 1), prePortName);
//        model->setData(model->index(count, 2), preConnectType);
//        model->setData(model->index(count, 3), preConnectMode);
//        model->setData(model->index(count, 4), preComName);
//        model->setData(model->index(count, 5), preBaudRate);
//        model->setData(model->index(count, 6), preTcpIP);
//        model->setData(model->index(count, 7), preTcpPort);
//        model->setData(model->index(count, 8), preReadInterval);
//        model->setData(model->index(count, 9), preReadTimeout);

//        ui->tableMain->setCurrentIndex(model->index(count, 0));

}

void DemarcationDelegate::on_btnSave_clicked()
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

void DemarcationDelegate::on_btnDelete_clicked()
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

void DemarcationDelegate::on_btnReturn_clicked()
{
    model->revertAll();
}

void DemarcationDelegate::on_btnClear_clicked()
{
//    if (model->rowCount() <= 0) {
//        return;
//    }

//    if (QUIHelper::showMessageBoxQuestion("确定要清空所有端口信息吗?") == QMessageBox::Yes) {
//        QString sql = "delete from PortInfo";
//        QSqlQuery query;
//        query.exec(sql);
//        model->select();
//    }
}

void DemarcationDelegate::on_pushButton_clicked()
{
    //异常提示
    if(ui->lineEdit->text().toDouble()<-3||ui->lineEdit->text().toDouble()>3){
        QUIHelper::showMessageBoxError("标定参数范围有误!（-3,3）");
    }
    //更新配置信息
    App::AlignAngle=ui->lineEdit->text();
    App::AlignDistance=ui->lineEdit_2->text();
    App::writeConfig();
    QString angle=QString("%1").arg((int)(ui->lineEdit->text().toDouble()*100),4,16,QChar('0'));
    QString dis=QString("%1").arg((int)(ui->lineEdit_2->text().toDouble()*100),4,16,QChar('0'));

    //角度信息更新
    if(angle.length()>4){//负数补码问题
        angle=angle.right(4);
    }
    angle.insert(2," ");

    //距离信息更新
    dis.insert(2," ");

    replaceStr(2,9,5,angle);
    replaceStr(4,3,5,dis);
    on_btnSave_clicked();
}
/*
* function:替换数据功能函数
* 参数：
* 1：rowIndex 行号
* 2：index 替换起始位置
* 3：len 替换长度
* 4：replaceStr 替换字符串
*/
void DemarcationDelegate::replaceStr(int rowIndex,int index,int len,QString replaceStr){

    QString str1=model->record(rowIndex).value("数据").toString();
    str1.replace(index,len,replaceStr);
    qDebug()<<"str1.replace"<<str1;
    model->setData(model->index(rowIndex, SQLDATAINDEX), str1);
}
/*
 *function:获取待写入数据
 * 参数：
 * 1：index index数据
 * 2:send_data 发送数据
 * 3:recv_data 待接收数据
 */
void DemarcationDelegate::getDataByIndex(int index,QString &send_data,QString &recv_data){
    QString head1=model->record(index).value("协议头").toString();
    QString data1=model->record(index).value("数据").toString();
    QString tail1=model->record(index).value("协议尾").toString();
    send_data.clear();
    if(head1!="null"){
        send_data=head1;
    }
    send_data.append(" ");
    send_data.append(data1);
    int nowlen1=send_data.split(" ").length();
    if(nowlen1<8){
        for(int i=0;i<8-nowlen1;i++){
            send_data.append(" ");
            send_data.append(tail1);
        }
    }

    QString head2=model->record(index+1).value("协议头").toString();
    QString data2=model->record(index+1).value("数据").toString();
    QString tail2=model->record(index+1).value("协议尾").toString();
    recv_data.clear();
    if(head2!="null"){
        recv_data=head2;
    }
    recv_data.append(" ");
    recv_data.append(data2);
    int nowlen2=recv_data.split(" ").length();
    if(nowlen2<8){
        for(int i=0;i<8-nowlen2;i++){
            recv_data.append(" ");
            recv_data.append(tail2);
        }
    }

//    qDebug()<<"send_data:"<<send_data<<"recv_data:"<<recv_data;
}
void DemarcationDelegate::setData(int row,int clown,QString data){
    model->setData(model->index(row, clown), data);
}

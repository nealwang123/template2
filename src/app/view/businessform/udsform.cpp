#include "udsform.h"
#include "ui_udsform.h"
#include "quiwidget.h"
#include "dbdelegate.h"
//整块block数据bin文件
QList<BinRecordBlock> UDSForm::binRecordBlocks_DriverList ;
UDSForm::UDSForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UDSForm)
{
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ&");
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    ui->setupUi(this);
    createConnection();
    this->initForm("UDSBurnData");
    demarcationTimer=new QTimer(this);
    connect(demarcationTimer,&QTimer::timeout,this,&UDSForm::slot_demarcationTimer);
    connect(&uDS,&UDS::emitEventRecv,this,&UDSForm::slot_UDSFrameRecv,Qt::QueuedConnection);
    sendCommandTimer=new QTimer(this);
    sendCommandTimer->setSingleShot(true);
    connect(sendCommandTimer,&QTimer::timeout,this,&UDSForm::slot_demarcationTimer);
    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit->setCalendarPopup(true);  // 正确

    //testing 日期
    qDebug()<<ui->dateEdit->text();
    QByteArray array=QUIHelper::hexStrToByteArray(ui->dateEdit->text());
    qDebug()<<"array size:"<<array.size()<<QString("%1 %2 %3 %4")
                .arg(array[0],2,16,QChar('0'))
                .arg(array[1],2,16,QChar('0'))
                .arg(array[2],2,16,QChar('0'))
                .arg(array[3],2,16,QChar('0'));
    //testing 诊断仪编号
    qDebug()<<ui->lineEdit_SerialNum->text();
    QByteArray arraysn=QUIHelper::asciiStrToByteArray(ui->lineEdit_SerialNum->text());
    qDebug()<<"arraysn size:"<<arraysn.size()<<QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
                .arg(arraysn[0],2,16,QChar('0'))
                .arg(arraysn[1],2,16,QChar('0'))
                .arg(arraysn[2],2,16,QChar('0'))
                .arg(arraysn[3],2,16,QChar('0'))
                .arg(arraysn[4],2,16,QChar('0'))
                .arg(arraysn[5],2,16,QChar('0'))
                .arg(arraysn[6],2,16,QChar('0'))
                .arg(arraysn[7],2,16,QChar('0'))
                .arg(arraysn[8],2,16,QChar('0'))
                .arg(arraysn[9],2,16,QChar('0'));
}

UDSForm::~UDSForm()
{
    delete ui;
}
void UDSForm::slot_UDSFrameRecv(VCI_CAN_OBJ cAN_OBJ1){
    //qDebug()<<"slot_UDSFrameRecv";
    qDebug()<<QString("ID:%1  Data:%2 %3 %4 %5 %6 %7 %8 %9")
              .arg(cAN_OBJ1.ID,4,16,QChar('0'))
              .arg(cAN_OBJ1.Data[0],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[1],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[2],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[3],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[4],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[5],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[6],2,16,QChar('0'))
              .arg(cAN_OBJ1.Data[7],2,16,QChar('0'));
    //addDataToView(0,cAN_OBJ1);
}
void UDSForm::addDataToView(int rowIndex, VCI_CAN_OBJ cAN_OBJ1)
{
    int count = model->rowCount();
    model->insertRow(count);
    QString dirStr = "";
    if (cAN_OBJ1.ID == UDS::SEND_CAN_ID){
        dirStr = "发送";
    }else if (cAN_OBJ1.ID == UDS::RECEIVE_CAN_ID){
        dirStr = "接收";
    }else if (cAN_OBJ1.ID == UDS::FUNCTION_ID){
        dirStr = "其它";
    }
    //设置新增加的行默认值
    model->setData(model->index(count, 0), 0);
    model->setData(model->index(count, 1), dirStr);
    model->setData(model->index(count, 2), QString("0x%1").arg(cAN_OBJ1.ID, 16));
    model->setData(model->index(count, 3), "DATA");
    QString frameTypeStr;
    if (cAN_OBJ1.ExternFlag == 1){
        frameTypeStr = "扩展帧";
    }else{
        frameTypeStr = "标准帧";
    }
    model->setData(model->index(count, 4), frameTypeStr);
    model->setData(model->index(count, 5), cAN_OBJ1.DataLen);
    model->setData(model->index(count, 6), QString("%1 %2 %3 %4 %5 %6 %7 %8")
            .arg(cAN_OBJ1.Data[0],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[1],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[2],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[3],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[4],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[5],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[6],2,16,QChar('0'))
            .arg(cAN_OBJ1.Data[7],2,16,QChar('0')));
    model->setData(model->index(count, 7), count);
    ui->tableMain->setCurrentIndex(model->index(count, 0));
}

void UDSForm::startDemarcation(QStringList list){
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
 bool UDSForm:: createConnection()
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
void UDSForm::initForm(QString fileName)
{
    //更新配置信息
//    ui->lineEdit->setText(App::AlignAngle);
//    ui->lineEdit_2->setText(App::AlignDistance);
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
//    columnNames << "索引" << "描述" << "传输方向" << "步骤"<< "类型" << "ID(Hex)"<< "协议头" << "数据" << "协议尾"<<"回执"<<"延时"<<"校验结果" ;
    columnNames << "序号" << "名称" << "帧ID" << "类型"<< "格式" << "DLC"<< "数据" << "帧数" << "方向" ;
    columnWidths.clear();
    columnWidths <<40<<40<<50<<40<<50<<40<<170<<40<<40<<40<<40<<40 ;

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

    //配置指令列表

    listItem.insert(1, "10 03");
    listItem.insert(2, "85 02");
    listItem.insert(3, "28 03 03");
    listItem.insert(4, "10 02");
    listItem.insert(5, "27 01");
    listItem.insert(6, "27 02 xx xx xx xx");
    listItem.insert(7, "2E F1 98 xx xx xx xx xx xx xx xx xx xx");
    listItem.insert(8, "2E F1 99 xx xx xx xx");
    listItem.insert(9, "31 01 FF 00 01");
    listItem.insert(10, "34 01");
    listItem.insert(11, "36 **");
    listItem.insert(12, "37");
    listItem.insert(13, "31 01 02 02");
    listItem.insert(14, "31 01 FF 01");
    listItem.insert(15, "11 01");

    listcommandKey.append("APP指令1");
    listcommandKey.append("2APP指令2");
    listcommandKey.append("3APP指令3");
    listcommandKey.append("4Boot指令1");
    listcommandKey.append("5请求Seed");
    listcommandKey.append("6发送Key");
    listcommandKey.append("7写入数据");
    listcommandKey.append("8刷新日期");
    listcommandKey.append("9擦除Block");
    listcommandKey.append("10请求下载");
    listcommandKey.append("11发送数据");
    listcommandKey.append("12请求退出");
    listcommandKey.append("13数据校验");
    listcommandKey.append("14一致性检查");
    listcommandKey.append("15ECU复位");
    QMap<int, QString>::iterator iter = listItem.begin();
    QStringList listcommandValue;
    while (iter != listItem.end())
    {
        qDebug() << "Iterator " << iter.key() << ":" << iter.value(); // 迭代器
        listcommandValue.append(iter.value());

        iter++;
    }


    ui->cBox_command->clear();
    ui->cBox_command->addItems(listcommandValue);

    //
    QUIHelper::readFileList("","cansend.txt",m_list_canSendCommand,m_list_canSendDiscrib);
    ui->cBoxcansend->clear();
    ui->cBoxcansend->addItems(m_list_canSendCommand);
    ui->labelCanSendDiscrib->setText(m_list_canSendDiscrib.at(0));
}

void UDSForm::on_btnAdd_clicked()
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

void UDSForm::on_btnSave_clicked()
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

void UDSForm::on_btnDelete_clicked()
{
//    if (ui->tableMain->currentIndex().row() < 0) {
//        QUIHelper::showMessageBoxError("请选择要删除的设备!");
//        return;
//    }

//    int row = ui->tableMain->currentIndex().row();
//    model->removeRow(row);
//    model->submitAll();
//    ui->tableMain->setCurrentIndex(model->index(model->rowCount() - 1, 0));
}

void UDSForm::on_btnReturn_clicked()
{
    model->revertAll();
}

void UDSForm::on_btnClear_clicked()
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

void UDSForm::on_pushButton_clicked()
{
//    //异常提示
//    if(ui->lineEdit->text().toDouble()<-3||ui->lineEdit->text().toDouble()>3){
//        QUIHelper::showMessageBoxError("标定参数范围有误!（-3,3）");
//    }
//    //更新配置信息
//    App::AlignAngle=ui->lineEdit->text();
//    App::AlignDistance=ui->lineEdit_2->text();
//    App::writeConfig();
//    QString angle=QString("%1").arg((int)(ui->lineEdit->text().toDouble()*100),4,16,QChar('0'));
//    QString dis=QString("%1").arg((int)(ui->lineEdit_2->text().toDouble()*100),4,16,QChar('0'));

//    //角度信息更新
//    if(angle.length()>4){//负数补码问题
//        angle=angle.right(4);
//    }
//    angle.insert(2," ");

//    //距离信息更新
//    dis.insert(2," ");

//    replaceStr(2,9,5,angle);
//    replaceStr(4,3,5,dis);
//    on_btnSave_clicked();
}
/*
* function:替换数据功能函数
* 参数：
* 1：rowIndex 行号
* 2：index 替换起始位置
* 3：len 替换长度
* 4：replaceStr 替换字符串
*/
void UDSForm::replaceStr(int rowIndex,int index,int len,QString replaceStr){

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
void UDSForm::getDataByIndex(int index,QString &send_data,QString &recv_data){
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
void UDSForm::setData(int row,int clown,QString data){
    model->setData(model->index(row, clown), data);
}




void UDSForm::on_button_Connect_released()
{

    if(ui->button_Connect->text()=="连接"){//连接设备
        bool result = CANApi::OpenDevice();
        if(result==true)
        {
            ui->textBrowser_Debug->append("设备连接成功");
        }
        else
        {
           QUIHelper::showMessageBoxError("设备连接失败");
           return;
        }

        result = CANApi::CANInit(ui->cBox_Baud->currentIndex());
        if (result == true)
        {
            ui->textBrowser_Debug->append("设备初始化成功");
        }
        else
        {
            QUIHelper::showMessageBoxError("设备初始化失败");
            return;
        }

        result = CANApi::StartCan();
        if (result == true)
        {
            ui->textBrowser_Debug->append("CAN通信启动成功");
            UDS::mConnected = true;//证明设备当前处于连接状态
            uDS.startHandleThread();//启动CAN帧数据的接收
        }
        else
        {
            QUIHelper::showMessageBoxError("CAN通信启动失败");
            return;
        }
        ui->button_Connect->setText("关闭");
    }else if(ui->button_Connect->text()=="关闭"){//关闭设备
        bool result = CANApi::CloseDevice();

        if (result == true)
        {
            ui->textBrowser_Debug->append("设备关闭成功");
            UDS::mConnected = false;//证明设备当前处于连接状态
        }
        else
        {
            QUIHelper::showMessageBoxError("设备关闭失败");
            return;
        }
        //uDS.setExitState(1,0);//启动CAN帧数据的接收
        ui->button_Connect->setText("连接");
    }
}

void UDSForm::on_buttonChooseFile_released()
{
    ui->lineEditChooseFile->setText(QUIHelper::getFileName("BIN Files(*.bin)"));
    //已经成功对相关文件进行解析。
    m_ParseFile = fileParse.ParseBinFile(ui->lineEditChooseFile->text(), UDSForm::binRecordBlocks_DriverList);
    qDebug()<<"UDSForm::binRecordBlocks_DriverList.length:"<<UDSForm::binRecordBlocks_DriverList.length();
    qDebug()<<"UDSForm::binRecordBlocks_DriverList:"<<UDSForm::binRecordBlocks_DriverList.at(0).data.length();
    if (m_ParseFile == true)
    {

        ui->textBrowser_Debug->append("BIN应用程序解析成功");

    }
    else
    {
        QUIHelper::showMessageBoxError("BIN文件解析失败，请再次解析");
    }
}

void UDSForm::on_buttonDownload_released()
{
    if (m_ParseFile == true)
    {
        ECANStatus eCANStatus = writeBinBlockData(UDSForm::binRecordBlocks_DriverList, 0);//文件解析成功

        if (eCANStatus == _STATUS_ERR|| eCANStatus == _STATUS_TIME_OUT)
        {
            ui->textBrowser_Debug->append(QString("<font color=green>数据下载不成功 Code:%1").arg(eCANStatus));
            //ui->textBrowser_Debug->append("数据下载不成功，Code"+ eCANStatus);
            //状态恢复
            ui->pushButton->setText("开始升级");
            ui->pushButton->setEnabled(true);
            return;
        }
        else
        {
            ui->pushButton->setText("开始升级");
            ui->pushButton->setEnabled(true);
            ui->textBrowser_Debug->append(QString("<font color=green>%1").arg("数据下载成功"));
            //ui->textBrowser_Debug->append("数据下载成功");
        }
        //状态恢复
    }
    else
    {
        ui->pushButton->setText("开始升级");
        ui->pushButton->setEnabled(true);
        QString str="文件解析失败，请重新解析！";
        ui->textBrowser_Debug->append(QString("<font color=red>%1").arg(str));
        //ui->textBrowser_Debug->append("文件解析失败，请重新解析！");
        return;
    }
}
void UDSForm::recorveyState(){
    if(ui->cBox_BootState->isChecked()){
        commandIndex=3;
    }else{
        commandIndex=0;
    }
    ui->cBox_Continue->setCheckState(Qt::Checked);
    ui->cBox_Continue->setChecked(true);
}
ECANStatus UDSForm::writeBinBlockData(QList<BinRecordBlock> BinBlockList, int type)
{
    //声明一个布尔类型
    ECANStatus result = _STATUS_ERR;
    //实例化字符型列表
    QList<byte> allData ;
    int EndAddress = 0x00000000;
    //获取整个数据块的末尾地址
    EndAddress =(BinBlockList.at(0).data.count());
    //声明一个数据块
    byte *alldata = new byte[(int)EndAddress];
    /*
    //获取整个数据块的末尾地址
    EndAddress = (uint)(HexBlockList[HexBlockList.Count() - 1].address + HexBlockList[HexBlockList.Count() - 1].data.Count() - HexBlockList[0].address);
    */
    //将数据块中的所有元素赋值为0
    for (uint i = 0; i < (int)EndAddress; i++)
    {
        alldata[i] = 0;
    }

    int alldatalen=0;
    //将所有数据整合到一个列表中

    alldatalen = BinBlockList.at(0).data.count();
    for (int j = 0; j < alldatalen; j++)
    {
        alldata[j] = (quint8)BinBlockList.at(0).data.at(j);
    }


    //将整个列表再拆分成若干个CAN发送帧数据
    QList<CAN_SEND_FRAME> cansendframe ;

    result = splitalldata_addSID(alldata, alldatalen,cansendframe);

    if (result != _STATUS_OK)
    {
        return _STATUS_ERR;
    }
    ui->progressBar->setValue(0);

    //拆分完成所有数据帧之后，需要按照顺序进行相应数据的发送。

    for (int i = 0; i < cansendframe.count(); i++)
    {
        QByteArray arr;
        arr.resize(1026);
        for(int j=0;j<1026;j++) {
            arr[j]=cansendframe[i].DATA[j];
        }
        ui->progressBar->setValue(100*i/cansendframe.count());
        qDebug()<<"send:"<<(QUIHelper::byteArrayToHexStr(arr))<<"i="<<i<<"cansendframe.count()="<<cansendframe.count();
        result=uDS.SendAndReceive( UDS::SEND_CAN_ID,cansendframe[i].DATA,1026);
        if (result != _STATUS_OK)
        {
            return result;
        }
    }
    //请求退出
    //requestExit();
    sendCommandByIndex(11);
    //数据校验
    //dataVerify();
    sendCommandByIndex(12);
    //一致性检查
    //consistencyVerify();
    sendCommandByIndex(13);
    //获取校验结果
    if(uDS.getdataVerify()!=0||uDS.getconsistencyVerify()!=0){
        QUIHelper::showMessageBoxError(QString("数据校验：%1;一致性校验:%2")
                                       .arg(uDS.getdataVerify(),2,16,QChar('0'))
                                       .arg(uDS.getconsistencyVerify(),2,16,QChar('0'))
                                       );
    }else{
        //复位ECU
        //resetECU();
        sendCommandByIndex(14);
    }
    //烧写完成
    ui->progressBar->setValue(100);
    //状态恢复
    /*
    result = uDS.SendAndReceive(cansendframe[0].DATA, UDS.SEND_CAN_ID);
    Console.WriteLine(Can_Card_Call.byteToHexStr(cansendframe[0].DATA));
    if (result != ECANStatus.STATUS_OK)
    {

        return result;
    }
    */

    return _STATUS_OK;
    //}
    //return ECANStatus.STATUS_OK;
}
void UDSForm::sendCommandByIndex(int index){
    ui->cBox_command->setCurrentIndex(index);
    on_cBox_command_activated(index);
    on_buttonSingleTest_released();
}
ECANStatus UDSForm::splitalldata_addSID(byte* alldata,int alldatalen, QList<CAN_SEND_FRAME>& cansendframe)
{
    uint dataindex = 0;
    CAN_SEND_FRAME One_send_Frame;
    //qDebug()<<"sizeof(alldata)::"<<sizeof(alldata);
    int alldatasize=alldatalen;
    qDebug()<<"alldatalen="<<alldatalen;
    //将所有的数据整合为以1024字节为单位的类里面
    if (alldatasize % 1024 == 0)//如果整个数据块正好为8的倍数，则一共有dataindex个数据帧需要发送
    {
        dataindex = (uint)(alldatasize/1024);
        for (int i = 0; i < dataindex; i++)
        {
            One_send_Frame.FrameNumber = (uint)i;
            memcpy(((One_send_Frame.DATA)+2),(alldata+i*1024),1024);
            //Array.Copy(alldata, i * 1024, One_send_Frame.DATA, 2, 1024);
            One_send_Frame.DATA[0] = 0x36;
            One_send_Frame.DATA[1] = (byte)((One_send_Frame.FrameNumber+1)%256);
            cansendframe.append(One_send_Frame);//将获取到的每一帧添加到发送帧列表中

        }
    }
    else//如果不是1024的整数倍，则多添加一个1024字节，将剩下不足1024字节的数据放置在相应的最后一个1024中。
    {
        dataindex = (uint)(alldatasize/1024) + 1;

        for (int i = 0; i < dataindex; i++)
        {
            One_send_Frame.FrameNumber = (uint)i;
            //qDebug()<<"One_send_Frame.FrameNumber="<<One_send_Frame.FrameNumber<<" "<<((One_send_Frame.FrameNumber+1)%255);
            if ((alldatasize - i * 1024) >= 1024)//如果后面还剩余大于1024，则直接复制1024字节到one_send_frame.data
            {
                memcpy(((One_send_Frame.DATA)+2),(alldata+i*1024),1024);
                //Array.Copy(alldata, i * 1024, One_send_Frame.DATA, 2, 1024);
                One_send_Frame.DATA[0] = 0x36;
                One_send_Frame.DATA[1] = (quint8)((One_send_Frame.FrameNumber+1)%256);
//                qDebug()<<"header:"<<QString("%1 %2")
//                        .arg(One_send_Frame.DATA[0],2,16,QChar('0'))
//                        .arg(One_send_Frame.DATA[1],2,16,QChar('0'));
//                qDebug()<<"header:"<<QString::number(One_send_Frame.DATA[0],16)<<" "<<QString::number(One_send_Frame.DATA[1],16);
            }
            else
            {
                memcpy(((One_send_Frame.DATA)+2),(alldata+i*1024),(alldatasize - i * 1024));
                //Array.Copy(alldata, i * 1024, One_send_Frame.DATA, 2, alldata.Count() - i * 1024);
                One_send_Frame.DATA[0] = 0x36;
                One_send_Frame.DATA[1] = (quint8)((One_send_Frame.FrameNumber +1)%256);
            }


            cansendframe.append(One_send_Frame);//将获取到的每一帧添加到发送帧列表中
        }
    }
    return _STATUS_OK;
}

void UDSForm::on_cBox_Continue_stateChanged(int arg1)
{
    if(ui->cBox_BootState->isChecked()){
        commandIndex=3;
    }else{
        commandIndex=0;
    }
    ui->cBox_command->setCurrentIndex(commandIndex);
    on_cBox_command_activated(commandIndex);

}
void UDSForm::on_cBox_command_activated(int index)
{
    qDebug()<<"on_cBox_command_activated"<<index;
    commandIndex = ui->cBox_command->currentIndex();
    if (ui->cBox_command->currentIndex() == 5){//发送key
        qDebug()<<"uDS.getSeedKey():"<<uDS.getSeedKey();
        selfSendStr = "27 02 " + uDS.getSeedKey();
        //ui->cBox_command->setCurrentText(selfSendStr);
    }else if (ui->cBox_command->currentIndex() == 6){//写入数据

        //testing 诊断仪编号
        qDebug()<<ui->lineEdit_SerialNum->text();
        QByteArray arraysn=QUIHelper::asciiStrToByteArray(ui->lineEdit_SerialNum->text());
        qDebug()<<"arraysn size:"<<arraysn.size()<<QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
                    .arg(arraysn[0],2,16,QChar('0'))
                    .arg(arraysn[1],2,16,QChar('0'))
                    .arg(arraysn[2],2,16,QChar('0'))
                    .arg(arraysn[3],2,16,QChar('0'))
                    .arg(arraysn[4],2,16,QChar('0'))
                    .arg(arraysn[5],2,16,QChar('0'))
                    .arg(arraysn[6],2,16,QChar('0'))
                    .arg(arraysn[7],2,16,QChar('0'))
                    .arg(arraysn[8],2,16,QChar('0'))
                    .arg(arraysn[9],2,16,QChar('0'));
       selfSendStr = "2E F1 98 " + QString("%1 %2 %3 %4 %5 %6 %7 %8 %9 %10")
               .arg(arraysn[0],2,16,QChar('0'))
               .arg(arraysn[1],2,16,QChar('0'))
               .arg(arraysn[2],2,16,QChar('0'))
               .arg(arraysn[3],2,16,QChar('0'))
               .arg(arraysn[4],2,16,QChar('0'))
               .arg(arraysn[5],2,16,QChar('0'))
               .arg(arraysn[6],2,16,QChar('0'))
               .arg(arraysn[7],2,16,QChar('0'))
               .arg(arraysn[8],2,16,QChar('0'))
               .arg(arraysn[9],2,16,QChar('0'));

    }else if (ui->cBox_command->currentIndex() == 7){//写入刷新时间
        //testing 日期
        qDebug()<<ui->dateEdit->text();
        QByteArray array=QUIHelper::hexStrToByteArray(ui->dateEdit->text());
        qDebug()<<"array size:"<<array.size()<<QString("%1 %2 %3 %4")
                    .arg(array[0],2,16,QChar('0'))
                    .arg(array[1],2,16,QChar('0'))
                    .arg(array[2],2,16,QChar('0'))
                    .arg(array[3],2,16,QChar('0'));
        selfSendStr = "2E F1 99 " + QString("%1 %2 %3 %4")
                .arg(array[0],2,16,QChar('0'))
                .arg(array[1],2,16,QChar('0'))
                .arg(array[2],2,16,QChar('0'))
                .arg(array[3],2,16,QChar('0'));
    }else if (ui->cBox_command->currentIndex() == 10){//下载数据

    }else{//
        selfSendStr = ui->cBox_command->currentText();
    }
    ui->label_SelfSendLab->setText(listcommandKey[ui->cBox_command->currentIndex()]);
}
void UDSForm::on_pushButton_released()
{
    if(ui->pushButton->text()=="开始升级"){
        recorveyState();
        slot_demarcationTimer();
        ui->pushButton->setText("升级...");
        ui->pushButton->setEnabled(false);
    }else{

    }

//    if(ui->cBox_Continue->isChecked()){
//        commandIndex++;
//        if(commandIndex<8){
//            sendCommandTimer->start(1000);
//        }else{
//            commandIndex=0;
//        }
//    }

}
void UDSForm::slot_demarcationTimer(){
    //定时判断是否可以进行下一条指令
//    if(uDS.getNextState()==1){
//        ui->textBrowser_Debug->append("正响应");
//    }else if(uDS.getNextState()==0){
//        ui->textBrowser_Debug->append("暂无响应");
//    }else if(uDS.getNextState()==-1){
//        ui->textBrowser_Debug->append("负响应");
//    }

    ui->cBox_command->setCurrentIndex(commandIndex);
    on_cBox_command_activated(commandIndex);
    on_buttonSingleTest_released();
    commandIndex++;
    if(commandIndex<10){
        sendCommandTimer->start(1000);
    }else{
        if(ui->cBox_BootState->isChecked()){
            commandIndex=3;
        }else{
            commandIndex=0;
        }
        //可以开始发送bin文件
        on_buttonDownload_released();
    }
}
void UDSForm::on_buttonSingleTest_released()
{
    QString strc;
    if(ui->cBox_command->currentText().contains("xx")){
        strc=selfSendStr.trimmed();
    }else{
        strc=ui->cBox_command->currentText().trimmed();
    }

    QByteArray arry=QUIHelper::hexStrToByteArray(strc);
    ECANStatus result=uDS.SendAndReceive(UDS::SEND_CAN_ID,(byte*)arry.data(),arry.count());
    if ( result== _STATUS_OK){
        ui->textBrowser_Debug->append("Sucess:服务名:"+listcommandKey[commandIndex]+"状态：OK");
    }else if ( result== _STATUS_ERR){
        ui->textBrowser_Debug->append("Failed:服务名:"+listcommandKey[commandIndex]+"状态：ERR");
    }else if ( result== _STATUS_TIME_OUT){
        ui->textBrowser_Debug->append("Failed:服务名:"+listcommandKey[commandIndex]+"状态：TIMEOUT");
    }
}

void UDSForm::on_checkBox_released()
{
    if(ui->checkBox->isChecked()){
        ui->buttonSingleTest->setEnabled(true);
        ui->buttonDownload->setEnabled(true);
        ui->cBox_Continue->setEnabled(true);
    }else{
        ui->buttonSingleTest->setEnabled(false);
        ui->buttonDownload->setEnabled(false);
        ui->cBox_Continue->setEnabled(false);
    }
}
//can指令测试，非UDS数据帧发送
void UDSForm::on_buttonSendcan1_released()
{
    //获取指令
    QString cmd=ui->cBoxcansend->currentText();
    //生成指令
    QByteArray array=QUIHelper::hexStrToByteArray(cmd);
    uDS.setWorkMode(FACTORY);
    //发送指令
    SendAndReceive(CANApi::SEND_CAN_ID_Self1,(byte*)array.data(),array.size());
}
//非uds
ECANStatus UDSForm::SendAndReceive(uint can_id,byte data[],int dataLength){
    int n=0;
    if (dataLength % 8 == 0){
        n=dataLength/8;
    }else{
        n=(dataLength/8)+1;
    }
    VCI_CAN_OBJ *obj=new VCI_CAN_OBJ[n];
    memset(obj,0,sizeof (VCI_CAN_OBJ));
    for (int i=0;i<n;i++) {
        obj[i].ID=can_id;
        if ((dataLength - i * 8) >= 8){
            memcpy(obj[i].Data,(data+i*8),8);
        }else{
            memcpy(obj[i].Data,(data+i*8),(dataLength - i*8));
        }
    }
    VCI_Transmit(4,0,0,obj,n);
    if(uDS.udsSleep(2000)){
        qDebug()<<"指令正常完成！";
    }else{
        qDebug()<<"指令超时！";
        return _STATUS_TIME_OUT;
    }
    return _STATUS_OK;
}

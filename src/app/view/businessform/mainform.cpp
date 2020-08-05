#include "mainform.h"
#include "ui_mainform.h"
#include"ControlCAN.h"
MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
//          initFile();
//          qDebug()<<"dbc loaded...";
    //    DllTest d;
    //    Aggregation *agg;

    //    frm=new frmMain();
    //    frm->show();

    initForm();
    commandTimer=new QTimer();
    connect(commandTimer, SIGNAL(timeout()), this, SLOT(slot_commandTimer()));
//    commandTimer->setSingleShot(true);
//    context =zmq::context_t(1);
//    publisher = zmq::socket_t(context, ZMQ_PUB);
//    publisher.connect("tcp://localhost:7777");
    recvTimer=new QTimer();
    connect(recvTimer,SIGNAL(timeout()),this,SLOT(slot_RecvTest()));
    m_sendFrameIndex=0;


    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->verticalHeader()->setDefaultSectionSize(25);
    ui->tableView->setEditTriggers(QAbstractItemView::CurrentChanged | QAbstractItemView::DoubleClicked);
    //initAlignModle
    model = new QStandardItemModel(this);
    model->setColumnCount(5);
    model->setHeaderData(0,Qt::Horizontal,"ERROR");
    model->setHeaderData(1,Qt::Horizontal,"Horizontal");
    model->setHeaderData(2,Qt::Horizontal,"Vertical");
    model->setHeaderData(3,Qt::Horizontal,"Power");
    model->setHeaderData(4,Qt::Horizontal,"Align Date");
    /*设置行字段名*/
//    model->setRowCount(3);
//    model->setHeaderData(0,Qt::Vertical, "记录一");
//    model->setHeaderData(1,Qt::Vertical, "记录二");
//    model->setHeaderData(2,Qt::Vertical, "记录三");
//    model->setItem(0, 0, new QStandardItem("-1"));
//    model->setItem(0, 1, new QStandardItem("1"));
//    model->setItem(0, 2, new QStandardItem("25"));
//    model->setItem(0, 3, new QStandardItem("25"));


    ui->tableView->setModel(model);
    // column width
    ui->tableView->setColumnWidth(0, 160);
    ui->tableView->setColumnWidth(1, 60);
    ui->tableView->setColumnWidth(2, 60);
    ui->tableView->setColumnWidth(3, 60);
    ui->tableView->setColumnWidth(4, 160);

//    00000:Normal
//    00001:No target
//    00010:Target distance error
//    00011:Multiple targets
//    00100:Target moved
//    00101:Axis not adjusted
//    00110:Horizontal Angle error
//    00111:Radar error
//    01000:Radar dirty
//    01001:Trmperature error
//    01010:Voltage errot
//    01011:Communication error
//    01100:Axis error(upwards)
//    01101:Axis error(downwards)
//    01110:Vehicle speed error
//    01111:Other
//    10000:Timeout
//    10001:Target parameters error
//    10010:Specification values undefined
    m_AlignErrorStates.insert(0b00000,"Normal");
    m_AlignErrorStates.insert(0b00001,"No target");
    m_AlignErrorStates.insert(0b00010,"Target distance error");
    m_AlignErrorStates.insert(0b00011,"Multiple targets");
    m_AlignErrorStates.insert(0b00100,"Target moved");
    m_AlignErrorStates.insert(0b00101,"Axis not adjusted");
    m_AlignErrorStates.insert(0b00111,"Radar error");
    m_AlignErrorStates.insert(0b01000,"Radar dirty");
    m_AlignErrorStates.insert(0b01001,"Trmperature error");
    m_AlignErrorStates.insert(0b01010,"Voltage errot");
    m_AlignErrorStates.insert(0b01011,"Communication error");
    m_AlignErrorStates.insert(0b01100,"Axis error(upwards)");
    m_AlignErrorStates.insert(0b01101,"Axis error(downwards)");
    m_AlignErrorStates.insert(0b01110,"Vehicle speed error");
    m_AlignErrorStates.insert(0b01111,"Other");
    m_AlignErrorStates.insert(0b10000,"Timeout");
    m_AlignErrorStates.insert(0b10001,"Target parameters error");
     m_AlignErrorStates.insert(0b10010,"Specification values undefined");


}
MainForm::~MainForm()
{
    delete ui;
}
void MainForm::slot_RecvTest(){
    static int  flage=0;
    int nDeviceType = 4; /* USBCAN-2A或USBCAN-2C或CANalyst-II */
    int nDeviceInd = 0; /* 第1个设备 */
    int nCANInd = 0; /* 第1个通道 */
    DWORD dwRel;
    VCI_INIT_CONFIG vic;
    VCI_CAN_OBJ obj[2500];
    if(flage==0){
        dwRel = VCI_OpenDevice(nDeviceType, nDeviceInd, 0);
        if(dwRel != 1){
            qDebug()<<"打开设备失败！";
        }
        vic.AccCode=0x80000008;
        vic.AccMask=0xFFFFFFFF;
        vic.Filter=1;
        vic.Timing0=0x00;
        vic.Timing1=0x1C;
        vic.Mode=0;
        dwRel = VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &vic);
        if(dwRel !=1) {
            VCI_CloseDevice(nDeviceType, nDeviceInd);
            qDebug()<<"VCI_InitCAN failed!!";
        }
//        int  bRel = VCI_UsbDeviceReset(nDeviceType, nDeviceInd, 0);
        if(VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd) !=1)
        {
            VCI_CloseDevice(nDeviceType, nDeviceInd);
            qDebug()<<"VCI_StartCAN failed!!";
        }
        flage++;
    }
    int ret0=VCI_GetReceiveNum(nDeviceType, nDeviceInd, nCANInd);
    int ret=VCI_Receive(nDeviceType, nDeviceInd, nCANInd,obj,2500,0);
    qDebug()<<"ret======="<<ret0<<"  "<<ret;
}
void MainForm::initForm(){

//   DemarcationDelegate* d1 = new DemarcationDelegate(this);
//   DemarcationDelegate* d2 = new DemarcationDelegate(this);
//   DemarcationDelegate* d3 = new DemarcationDelegate(this);
    demarcationForm = new DemarcationDelegate;
    canConfigForm = new CANConfigDelegate;
    //stackedWidget默认有多个窗口
    ui->stackedWidget->addWidget(demarcationForm);
    ui->stackedWidget->addWidget(canConfigForm);
    qDebug()<<"ui->stackedWidget->currentIndex()::"<<ui->stackedWidget->currentIndex()<<ui->stackedWidget->count();
    ui->stackedWidget->setCurrentIndex(0);
    QPixmap pixmap(QString(":/imageTest/buttonpilot_gray.png"));
    ui->label_state->setFixedSize(pixmap.size());
    ui->label_state->setPixmap(pixmap);
    ui->label_state->setScaledContents(true);
}
void MainForm::initFile(){
    dbc=new DBCReader;
    dbc->loadDBCFile("./Neusoft_Radar_CAN_protocol_TSMTC_V1.1.dbc");
    qDebug()<<"loadDBCFile end";
//    dbc->debugDBCStruct(" ");
//    dbc.loadDBCFile("./test.dbc");
//    dbc->debugDBCStruct("");

}

void MainForm::changeStyle(const QString &qssFile)
{
    QFile file(qssFile);

    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        QUIConfig::PanelColor = qss.mid(20, 7);
        QUIConfig::TextColor = qss.mid(49, 7);
        file.close();
    }

    App::StyleName = qssFile;
    App::writeConfig();
    emit changeStyle();

//    this->changeBtnStyle();
}
void MainForm::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
void MainForm::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainForm::on_pushButton_4_clicked()
{
    if(commandTimer->isActive()){
        commandTimer->stop();
    }
    //状态复位
    m_sendFrameIndex=0;
    m_getResultCount=0;
    for(int i=0;i<18;i++){//结果清空
        demarcationForm->setData(i,11," ");

    }
    qDebug()<<"m_sendFrameIndex:"<<m_sendFrameIndex;
    commandTimer->start(200);

}


//
void MainForm::slot_commandTimer(){
    if(m_sendFrameIndex>=18){
        m_sendFrameIndex=0;
        m_getResultCount=0;
        commandTimer->stop();
        ui->textBrowser->append("一次安装标定动作完成！！！");

        return;
    }
    demarcationForm->getDataByIndex(m_sendFrameIndex,m_senddata,m_recvdata);
    QString header=QString("TORAD %1 %2 %3").arg(4).arg(0).arg(0);
    QString senddata=QString(" %1 %2 %3 %4 %5")
            .arg(0,8,16,QChar('0'))
            .arg(0x7A1,8,16,QChar('0'))
            .arg(8,4,10,QChar('0'))
            .arg(m_senddata)
            .arg(m_sendFrameIndex,4,10,QChar('0'));
    header=header+senddata;
    //从数据库获取数据，发送指令
    if(0)
        emit sig_msgToRadar(header);
    qDebug()<<"m_sendFrameIndex:"<<m_sendFrameIndex<<"m_senddata:"<<m_senddata;
    QByteArray arry=QUIHelper::hexStrToByteArray(m_senddata);
    //直接发送
    VCI_CAN_OBJ *obj=new VCI_CAN_OBJ[1];
    memset(obj,0,sizeof (VCI_CAN_OBJ));
    obj[0].ID=0x7A1;
    obj[0].SendType = 0;
    obj[0].ExternFlag = 0;
    obj[0].RemoteFlag = 0;
    obj[0].DataLen=8;
    memcpy(obj[0].Data,arry.data(),8);
    qDebug()<<" obj[0].ID"<<QString("%1").arg(obj[0].ID,4,16,QChar('0'))<<"obj[0].DataLen"<<obj[0].DataLen;
    int ret=CANApi::SendOneFrame(0, 0, &obj[0]);
//    int ret=VCI_Transmit(4,0,0,&obj[0],1);

    qDebug()<<"ret=="<<ret;

}
//断开设备
void MainForm::on_pushButton_5_clicked()
{
    QMutexLocker locker(&mutex);
    m_exitStateThread2=0;
    emit(unInitDevice());

}

void MainForm::on_pushButton_6_clicked()
{
    emit(initDevice());
}

//线程2启动函数
void MainForm::star()//启动函数是需要在原有代码基础上增加
{
    qDebug()<<"QtConcurrent::run(this,&DemarcationDelegate::stateThread);//多线程执行死循环启动:";
    context =zmq::context_t(1);
    subscriber = zmq::socket_t(context, ZMQ_SUB);
    subscriber.connect("tcp://localhost:7888");
    //  Subscribe to zipcode, default is NYC, 10001
    const char *filter = "TOPC ";//s
    subscriber.setsockopt(ZMQ_SUBSCRIBE, filter, strlen (filter));
    m_exitStateThread2=1;
    QtConcurrent::run(this,&MainForm::recThread);//多线程执行死循环启动，可以带参数，具体格式可以查阅网上其它资料
}
void MainForm::recThread(){

    while(m_exitStateThread2){
        std::string toPCMsg = s_recv(subscriber);
//        //  Read identify
//        std::string identify = s_recv(subscriber);
//        //  Read message contents
//        std::string contents = s_recv(subscriber);
//        std::cout << "[" << address << "] "<< "[" << identify << "] " << contents << std::endl;
        std::cout << "[topc:]" <<m_exitStateThread2<<toPCMsg<< std::endl;

    }
}
/*
 * function:雷达反馈数据更新到界面
 */
void MainForm::slot_msgToPC(QString str){
    qDebug()<<"MainForm::slot_msgToPC:"<<str;

    static QString err_resultstr;
//    qDebug()<<"str.len"<<str.length()<<str.mid(34,23);
    QStringList list=str.split(" ");
    qDebug()<<"list==="<<list;
    if(list.at(5).toInt(nullptr,16)==0x7A9){
        QString data=str.mid(34,23);

        //循环替换 xx
        while(1){
            int index=m_recvdata.indexOf("xx");
            if(index!=-1){//查找待替换位置
                qDebug()<<"replace xx with real data!!!";
                m_recvdata.replace(index,2,data.mid(index,2));

            }else{
                break;
            }
        }
        qDebug()<<"real recv:"<<data.trimmed().toUpper();
        qDebug()<<"sql save:"<<m_recvdata.trimmed().toUpper();
        QString state;
        int step=1+(m_sendFrameIndex/2);
        state=" STEP ";
        state+=QString("%1").arg(step,2,10,QChar('0'));

        quint8 hor_alig_angle=0;
        quint8 ver_measurid_angle=0;
        quint8 alig_power_h=0;
        quint8 alig_power_l=0;
        qint16 alig_power=0;
        if(data.trimmed().toUpper()==m_recvdata.trimmed().toUpper()){//指令格式正常
            if(step==5){//查询标定状态
                //判断结果
                QString result=data.trimmed().mid(12,2);
                qDebug()<<"result:"<<result;
                if((result!="02"&&result!="03")&&m_getResultCount<App::AlignCount){
                    if(result=="00"){
                        state+="Not Executed";
                    }else if(result=="01"){
                        state+="Aligning";
                    }
                    m_getResultCount++;
                    ui->textBrowser->append((str+state).toUpper());
                    QThread::msleep(10);
                    return;//退出
                }else{//可继续下一步骤
                    if(result=="02"){
                        state+="Align Success";
                    }else if(result=="03"){
                        state+="Align Failure";
                    }
                    m_getResultCount=0;
                }
            }else if(step==6){//查询错误码
                quint8 err_result=data.trimmed().mid(12,2).toInt(nullptr,16);
                err_result=(err_result>>3);//
                err_resultstr=QString("%1 ").arg(err_result,6,2,QChar('0'));
                if(m_AlignErrorStates.contains(err_result)){
                    err_resultstr.append(m_AlignErrorStates.find(err_result).value());
                }
            }else if(step==7){//查询标定结果
                hor_alig_angle=data.trimmed().mid(15,2).toInt();
                ver_measurid_angle=data.trimmed().mid(18,2).toInt();
                alig_power_h=data.trimmed().mid(21,2).toInt();
            }else if(step==8){
                alig_power_l=data.trimmed().mid(0,2).toInt();
                alig_power=((quint16)alig_power_h)<<8+alig_power_l;
                int count = model->rowCount();
                model->insertRow(count);

                model->setData(model->index(count, 0), err_resultstr);
                //设置新增加的行默认值
                model->setData(model->index(count, 1), hor_alig_angle*0.1);
                model->setData(model->index(count, 2), ver_measurid_angle*0.1);
                model->setData(model->index(count, 3), (alig_power>>7)-69.2);
                QDateTime dateTime(QDateTime::currentDateTime());
                QString qStr = dateTime.toString("yyy-MM-dd hh:mm::ss ddd");
                model->setData(model->index(count, 4), qStr);
                ui->tableView->setCurrentIndex(model->index(count, 0));
            }
            state+=" PASS";
            demarcationForm->setData(m_sendFrameIndex,11,"PASS");

            demarcationForm->setData(m_sendFrameIndex+1,11,"PASS");

            QModelIndex index = model->index(m_sendFrameIndex+1, 0, QModelIndex());
            ui->tableView->setCurrentIndex(index);
            ui->tableView->selectRow(m_sendFrameIndex+1);
            m_sendFrameIndex+=2;

        }else{//指令格式异常
            ui->textBrowser->append("command format error!");
        }
        ui->textBrowser->append((str+state).toUpper());
    }else if(list.at(5).toInt(nullptr,16)==0x4E0){
        static int count=0;
        QString firmware_data=str.mid(34,23);
//        qDebug()<<"firmware_data"<<firmware_data;
        ui->lineEdit->setText(firmware_data.right(5).replace(2,1,""));
        if(count%2==0){
            QPixmap pixmap(QString(":/imageTest/buttonpilot_gray.png"));
            ui->label_state->setFixedSize(pixmap.size());
            ui->label_state->setPixmap(pixmap);
            ui->label_state->setScaledContents(true);
        }else if(count%2==1){
            QPixmap pixmap(QString(":/imageTest/buttonpilot_green.png"));
            ui->label_state->setFixedSize(pixmap.size());
            ui->label_state->setPixmap(pixmap);
            ui->label_state->setScaledContents(true);
        }
        count++;
    }

}
void MainForm::slot_stateInfoDataModel(QString str){

}
void MainForm::on_pushButton_7_clicked()
{
    recvTimer->start(100);
}

void MainForm::on_pushButton_4_released()
{

}

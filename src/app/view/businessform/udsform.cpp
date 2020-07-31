#include "udsform.h"
#include "ui_udsform.h"
#include "quiwidget.h"
#include "dbdelegate.h"

QString BurnInfo::SN_Head = "";
QString BurnInfo::SN_Year = "";
QString BurnInfo::SN_MD = "";
QString BurnInfo::SN_Tail = "";
QString BurnInfo::HW_A = "";
QString BurnInfo::HW_B = "";

//整块block数据bin文件
QList<BinRecordBlock> UDSForm::binRecordBlocks_DriverList ;
UDSForm::UDSForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UDSForm)
{
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ&");
    qRegisterMetaType<VCI_CAN_OBJ>("VCI_CAN_OBJ");
    ui->setupUi(this);
    qDebug()<<"UDSBurnData open:"<<createConnection();
    QSqlQuery query;
    qDebug()<<query.exec("SELECT * FROM UDSBurnData;");
    while(query.next())
    {
        QString id = query.value(0).toString();
        QString name = query.value(1).toString();
        QString score = query.value(2).toString();
        QString classs = query.value(3).toString();

        qDebug()<<id<<name<<score<<classs;
    }

    this->initForm("UDSBurnData");
    demarcationTimer=new QTimer(this);
    connect(demarcationTimer,&QTimer::timeout,this,&UDSForm::slot_demarcationTimer);
    connect(UDS::Instance(),&UDS::emitEventRecv,this,&UDSForm::slot_UDSFrameRecv,Qt::QueuedConnection);
    connect(UDS::Instance(),&UDS::emitEOLInfo,this,&UDSForm::slot_EOLInfo,Qt::QueuedConnection);
    connect(UDS::Instance(),&UDS::emitOnlineBurnInfo,this,&UDSForm::slot_OnlineBurnInfo,Qt::QueuedConnection);

    connect(UDS::Instance(),&UDS::emitTemperatureTest,this,&UDSForm::slot_TemperatureTest,Qt::QueuedConnection);
    connect(UDS::Instance(),&UDS::installAlignData,&installAlign,&MainForm::slot_msgToPC,Qt::QueuedConnection);

    sendCommandTimer=new QTimer(this);
    sendCommandTimer->setSingleShot(true);
    connect(sendCommandTimer,&QTimer::timeout,this,&UDSForm::slot_demarcationTimer);
    //初始化eol定时器
    m_eolsendCommandTimer=new QTimer(this);
    m_eolsendCommandTimer->setSingleShot(true);
    connect(m_eolsendCommandTimer,&QTimer::timeout,this,&UDSForm::slot_eolsendCommandTimer);
    m_eolcommandIndex=0;

    ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit->setCalendarPopup(true);  // 正确
    ui->dateEdit_SN->setDateTime(QDateTime::currentDateTime());
    ui->dateEdit_SN->setCalendarPopup(true);  // 正确

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

   qDebug()<< model->record(0).value(1).toString()<<model->record(0).value(2).toString();
   ui->lineEdit_User->setPlaceholderText("请输入用户名");
   ui->lineEdit_PW->setPlaceholderText("请输入密码");
   QPixmap pixmap(QString(":/imageTest/buttongray.png"));
   ui->label_state->setFixedSize(64,64);
   QPixmap fitpixmap = pixmap.scaled(64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
   ui->label_state->setPixmap(fitpixmap);

   {
       //高低温测试窗口初始化
       tempwidget=new TemperatureDelegate();
       QHBoxLayout *lapv=new QHBoxLayout;
       lapv->addWidget(tempwidget);
       ui->widget_Temp->setLayout(lapv);
       ui->lineEdit_Delay->setText(QString::number(App::OnlineCaStatDelay));
   }

   //算法参数
   algowiget=new AlgorithmParaDelegate();
   QHBoxLayout *laAlgo=new QHBoxLayout;
   laAlgo->addWidget(algowiget);
   ui->widget_Algorithm->setLayout(laAlgo);

   //ui->lineEdit_HV->text()
    ui->lineEdit_HV->setText(App::HWVersion);
    initOnLineTableView();

    {//init onlineDeviceForm
        onlineDeviceForm=new OlineDeviceForm();
        onlineDeviceForm->show();
        QVBoxLayout *laOnline=new QVBoxLayout;
        laOnline->addWidget(onlineDeviceForm);
        ui->onlineDevice_frame->setLayout(laOnline);
    }
    {
        onlineDeviceFlage=0;
        onlineDeviceTimerCounter=0;
        onlineDeviceTimer=new QTimer();
        onlineDeviceTimer->setSingleShot(true);
        connect(onlineDeviceTimer,&QTimer::timeout,this,&UDSForm::slot_onlineDeviceTimer);

        disableTimer=new QTimer();
        connect(disableTimer,&QTimer::timeout,this,&UDSForm::slot_disableTimer);
        disableTimer->setSingleShot(true);
    }


    {//安装标定界面
        QVBoxLayout *laAlign=new QVBoxLayout;
        laAlign->addWidget(&installAlign);
        ui->widget_installAlign->setLayout(laAlign);

    }
    {//初相校准界面
        QVBoxLayout* initial=new QVBoxLayout;
        initial->addWidget(&initialphaseform);
        ui->widget_Initial->setLayout(initial);
    }

    {//设置成在线更新程序
        UDS::Instance()->setWorkMode(ONLINEUPDATE);
        QVBoxLayout* onlineBurn=new QVBoxLayout;
        onlineBurn->addWidget(&onlineburnform);
        ui->widget_onlineBurn->setLayout(onlineBurn);
    }

}

UDSForm::~UDSForm()
{
    delete ui;
}
void UDSForm::slot_disableTimer(){
    //清空设备列表
    onlineDeviceForm->resetData();
    //更新在线列表，遍历对比
    QMap<int,ItemData>::const_iterator i;
    for (i = mapAllDevice.constBegin(); i != mapAllDevice.constEnd(); ++i) {
        qDebug()<<"i.key()"<< i.key()<<"i.value().id"<< i.value().id;
        int id=i.key();
        int radarstate=i.value().state;
        QString datastr=i.value().date;
        if(mapOnlineDevice.contains(id)){
            QMap<int,ItemData>::const_iterator iter= mapOnlineDevice.find(id);
            {
                onlineDeviceForm->addData(id,iter.value().date,2);

            }
        }else{
            onlineDeviceForm->addData(id,datastr,0);
        }
    }
}
/*
 *定时器执行槽函数
 */
void UDSForm::slot_onlineDeviceTimer(){

    if(onlineDeviceTimerCounter==0){//首次更新
        ui->lineEdit_Temp_first->setText(QString("%1").arg(onlineDeviceNum,4,10,QChar('0')));
        ui->lineEdit_Temp_realtime->setText(QString("%1").arg(onlineDeviceNum,4,10,QChar('0')));
        ui->lineEdit_Temp_dead->setText(QString("%1").arg((0),4,10,QChar('0')));
        mapAllDevice=mapOnlineDevice;
    }else{
        ui->lineEdit_Temp_realtime->setText(QString("%1").arg(onlineDeviceNum,4,10,QChar('0')));
        ui->lineEdit_Temp_dead->setText(QString("%1").arg((ui->lineEdit_Temp_first->text().toInt(nullptr,10)-onlineDeviceNum),4,10,QChar('0')));
    }

    //清空设备列表
    onlineDeviceForm->resetData();
    //更新在线列表，遍历对比
    QMap<int,ItemData>::const_iterator i;

    for (i = mapAllDevice.constBegin(); i != mapAllDevice.constEnd(); ++i) {
        qDebug()<<"i.key()"<< i.key()<<"i.value().id"<< i.value().id;
        int id=i.key();
        int radarstate=i.value().state;
        QString datastr=i.value().date;
        if(mapOnlineDevice.contains(id)){
           QMap<int,ItemData>::const_iterator iter= mapOnlineDevice.find(id);

            {
                if(radarstate==0){//异常
                    onlineDeviceForm->addData(id,iter.value().date,2);
                }else if(radarstate==1){//在线
                    //统计在线数据
                    onlineDeviceForm->addData(id,iter.value().date,1);
                }
            }
        }else{
            onlineDeviceForm->addData(id,datastr,0);
        }
    }


    onlineDeviceTimerCounter++;
    onlineDeviceFlage=0;

    //启动置灰定时器
    int delayms=ui->lineEdit_Delay->text().toInt();
    disableTimer->start(delayms);
}
void UDSForm::initOnLineTableView(){

}
void UDSForm::slot_TemperatureTest(int id,QString raw,QStringList list){
    if(ui->pushButton_2->text()!="记录中..."){
        ui->label_TempState->setText("未记录温度数据...");
        return;
    }
    static int cansavecount=0;
    if(id==0x7D0){
        ui->label_TempState->setText("模式可能异常，请确保进入工厂模式...");
        if(cansavecount==0){
            QUIHelper::showMessageBoxError("收到非预期数据，模式可能异常，请确保进入工厂模式...");
            cansavecount++;
        }

        return;
    }else{
        ui->label_TempState->setText("记录中...");
        cansavecount=0;
    }
    QStringList templist=list;
    if(templist.length()!=8){
        QUIHelper::showMessageBoxError("温度测试列表长度异常！");
        return;
    }


    qDebug()<<QString("id:%1 data:%2").arg(id).arg(raw);
    qDebug()<<list;
    QString swVer=templist.at(0).split("|").at(1);
    QString frameID=templist.at(6).split("|").at(2);
    QString vcoTemp=templist.at(3).split("|").at(2);
    QString flagRunSeq01=templist.at(5).split("|").at(2);
    QString flagRunSeq02=templist.at(4).split("|").at(2);
    QString flagRunSeq03=templist.at(1).split("|").at(2);
    //QString vcoTemp=templist.at(3).split("|").at(2).right(1);
    int count=tempwidget->addRow();
    qDebug()<<"";
    tempwidget->setData(count,0,QString("%1").arg(count,5,10,QChar('0')));
    tempwidget->setData(count,1,QString("%1").arg(id,4,10,QChar('0')));
    tempwidget->setData(count,2,raw);
    tempwidget->setData(count,3,QString("%1").arg(qRound(frameID.toFloat()),5,10,QChar('0')));
    tempwidget->setData(count,4,vcoTemp);
    tempwidget->setData(count,5,QString("%1").arg(qRound(flagRunSeq01.toDouble()),4,2,QChar('0')));
    tempwidget->setData(count,6,QString("%1").arg(qRound(flagRunSeq02.toDouble()),8,2,QChar('0')));
    tempwidget->setData(count,7,QString("%1").arg(qRound(flagRunSeq03.toDouble()),4,2,QChar('0')));
    QString tempstr=QString("%1%2%3").arg(qRound(flagRunSeq01.toDouble()),4,2,QChar('0'))
                            .arg(qRound(flagRunSeq02.toDouble()),8,2,QChar('0'))
                            .arg(qRound(flagRunSeq03.toDouble()),4,2,QChar('0'));
    tempwidget->setData(count,8,tempstr);
    tempwidget->setData(count,11,ui->lineEdit_Temp->text());
    int radarstate=0;
    //判定
    if(tempstr=="0000000000000000"){
        tempwidget->setData(count,9,"PASS");
        radarstate=1;
    }else{
        tempwidget->setData(count,9,"FAIL");
        radarstate=0;
    }
    QString timeStr=QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    tempwidget->setData(count,10,QString("%1").arg(timeStr));
    //保存到sql数据库
    tempwidget->on_btnSave_clicked();


    //启动10s定时器，统计最大数量
    if(onlineDeviceFlage==0){
        onlineDeviceFlage=1;
        {//清空
            onlineDeviceNum=0;
            mapOnlineDevice.clear();
        }
        int delayms=ui->lineEdit_Delay->text().toInt();
        onlineDeviceTimer->start(delayms);
    }

    QString datastr=QString("帧号:%1 vco:%2")
            .arg(qRound(frameID.toFloat()),5,10,QChar('0'))
            .arg(vcoTemp);
    if(onlineDeviceFlage==1){

        //更新到在线map
        ItemData itemdata;
        itemdata.id=id;
        itemdata.date=QString("帧ID:%1 VCO:%2")
                .arg(qRound(frameID.toFloat()),5,10,QChar('0'))
                .arg(vcoTemp);
        itemdata.state=radarstate;
        mapOnlineDevice.insert(id,itemdata);

        onlineDeviceForm->updateTime();
        onlineDeviceNum++;
    }


}
void UDSForm::slot_OnlineBurnInfo(QString respHead,QByteArray array){
    qDebug()<<"----------------------slot_OnlineBurnInfo----------------------"<<array.size();
    if(respHead=="BOOT"){
        QUIHelper::showMessageBoxInfo(QString("进入BOOT升级流程!\r\n固件版本：%1").arg(QUIHelper::byteArrayToAsciiStr(array)),2);
        onlineburnform.displayStr(QString("进入BOOT升级流程! 固件版本：%1\n").arg(QUIHelper::byteArrayToAsciiStr(array)));
    }else if(respHead=="GBYE"){
        QUIHelper::showMessageBoxInfo("固件升级成功,待重启反馈版本号！",2);
        onlineburnform.displayStr("固件升级成功,待重启反馈版本号！");
        onlineburnform.updateDone();
    }else if(respHead=="SW"){
        onlineburnform.displayStr(QString("软件版本号：%1%2").arg((quint8)array[6],2,16,QChar('0')).arg((quint8)array[7],2,16,QChar('0')));
    }
}
void UDSForm::slot_EOLInfo(QString respHead,QByteArray array){
    qDebug()<<"----------------------slot_EOLInfo----------------------"<<array.size();
//    qDebug()<<QString("Head:%1  hexContext:%2 AsciiContext:%3 m_modelIndex:%4")
//              .arg(respHead)
//              .arg(QUIHelper::byteArrayToHexStr(array))
//              .arg(QUIHelper::byteArrayToAsciiStr(array))
//              .arg(m_modelIndex);
    //str=="RRCF"||str=="EOLS"||str=="RRSN"||str=="RRSV"||str=="RRHV"||str=="RRBV"||str=="EOLR"
    QString tempstr;
    int testItem=0;
    if(guiworkmode==FACTORY||guiworkmode==CONSUMER){
        ui->label_out->setText(respHead);
        if(respHead=="DONE"){//正常反馈

        }else if(respHead=="RRCF"){//读取模式

            if(QUIHelper::byteArrayToHexStr(array)=="5A 5A 5A 5A"){
                tempstr="客户模式";
            } else if(QUIHelper::byteArrayToHexStr(array)=="50 5A 5A 5A"){
                tempstr="工厂模式";
            }else {//
                tempstr="未知模式";
            }
            ui->pushButton_workMode->setText(tempstr);
            model->setData(model->index(m_modelIndex, 4),tempstr);
            ui->tableMain->setCurrentIndex(model->index(m_modelIndex, 0));
            //testItem=
        }else if(respHead=="EOLR"){//进入EOL模式反馈

        }else if(respHead=="EOLS"){//EOL子模式

        }else if(respHead=="RRSN"){//SN获取
            tempstr=QUIHelper::byteArrayToAsciiStr(array).remove(QRegExp("\\s"));
            ui->lineEdit_SNoutput->setText(tempstr);
            qDebug()<<"save SN"<<model->setData(model->index(m_modelIndex, 5), tempstr);
        }else if(respHead=="RRSV"){//软件版本获取
            tempstr=QUIHelper::byteArrayToHexStr(array).remove(QRegExp("\\s"));
            ui->lineEdit_SWOutput->setText(tempstr);
            qDebug()<<"save SV"<<model->setData(model->index(m_modelIndex, 6), tempstr);
        }else if(respHead=="RRHV"){//硬件版本获取
            tempstr=QUIHelper::byteArrayToHexStr(array).remove(QRegExp("\\s"));
            ui->lineEdit_HWOutput->setText(tempstr);
            qDebug()<<"save HV"<<model->setData(model->index(m_modelIndex, 7), tempstr);
        }else if(respHead=="RRBV"){//boot版本获取
            tempstr=QUIHelper::byteArrayToHexStr(array).remove(QRegExp("\\s"));
            ui->lineEdit_BootOutput->setText(tempstr);
            qDebug()<<"save BV"<<model->setData(model->index(m_modelIndex, 8), tempstr);
        }else if(respHead=="PARA"){//算法参数版本获取
            //ui->lineEdit_BootOutput->setText(QUIHelper::byteArrayToHexStr(array));
            //add data to table
            tempstr=QUIHelper::byteArrayToHexStr(array);

            int rowcount=algowiget->getRow();

            if(array.size()==512){
                qDebug()<<"合法数据";
            }
            for(int i=0;i<rowcount;i++){
                int type=algowiget->getType(i);
                QString temp=QString("%1 %2 %3 %4").arg((quint8)array.at(i*4+0),2,16,QChar('0'))
                                    .arg((quint8)array.at(i*4+1),2,16,QChar('0'))
                                    .arg((quint8)array.at(i*4+2),2,16,QChar('0'))
                                    .arg((quint8)array.at(i*4+3),2,16,QChar('0'));
                algowiget->setData(i,2,temp);
                //type
                if(type==1){
                    qDebug()<<"float:"<<QUIHelper::Byte2Float(QUIHelper::hexStrToByteArray(temp));
                    //
                    algowiget->setData(i,3,QString::number(QUIHelper::Byte2Float(QUIHelper::hexStrToByteArray(temp)),'g',6));
                }else{
                    algowiget->setData(i,3,QString("%1")
                                       .arg(QUIHelper::byteToInt(QUIHelper::hexStrToByteArray(temp)))
                                            );
                }


            }
            model->setData(model->index(m_modelIndex, 9), "获得参数");
            //保存到算法参数sql数据库
            algowiget->on_btnSave_clicked();
        }
        qDebug()<<"m_eolcommandIndex:"<<m_eolcommandIndex;
        if(m_eolcommandIndex>=12){
            int ret=judgeResult();
            model->setData(model->index(m_modelIndex, 2),QString("%1").arg(ret,8,2,QChar('0')));
            QString resultpara=model->record(0).value(2).toString();
            QString result=model->record(m_modelIndex).value(2).toString();

            //qDebug()<<"resultpara"<<resultpara<<"result"<<result;
            //qDebug()<<QString("resultpara：%1").arg(resultpara.toInt(nullptr,2),8,2,QChar('0'))<<QString("result：%1").arg(result.toInt(nullptr,2),8,2,QChar('0'));

            if((resultpara.toInt(nullptr,2)&result.toInt(nullptr,2))==resultpara.toInt(nullptr,2)){
    //            model->setData(model->index(m_modelIndex, 1), QBrush(QColor(255, 0, 0)), Qt::TextColorRole);
                //设置单元格文本颜色，张三的数据设置为红色
                model->data(model->index(m_modelIndex, 1),Qt::BackgroundRole);
                model->setData(model->index(m_modelIndex, 1),"PASS");
            }else{
                model->data(model->index(m_modelIndex, 1),Qt::BackgroundRole);
                model->setData(model->index(m_modelIndex, 1),"FAIL");
            }

        }
        //更新数据库
        on_btnSave_clicked();
    }else if(guiworkmode==INITIALPHASE){//初相校准模式更新
        if(respHead=="DONE"){//正常反馈

        }else if(respHead=="RRCF"){//读取模式

            if(QUIHelper::byteArrayToHexStr(array)=="5A 5A 5A 5A"){
                tempstr="客户模式";
            } else if(QUIHelper::byteArrayToHexStr(array)=="50 5A 5A 5A"){
                tempstr="工厂模式";
            }else {//
                tempstr="未知模式";
            }
            //
            initialphaseform.displayWorkmode(tempstr);
        }else if(respHead=="PARA"){//算法参数版本获取
            //ui->lineEdit_BootOutput->setText(QUIHelper::byteArrayToHexStr(array));
            //add data to table
            tempstr=QUIHelper::byteArrayToHexStr(array);

            int rowcount=algowiget->getRow();

            if(array.size()==512){
                qDebug()<<"合法数据";
            }
            float para_cal[12];

            for(int i=0;i<rowcount;i++){
                int type=algowiget->getType(i);
                QString temp=QString("%1 %2 %3 %4").arg((quint8)array.at(i*4+0),2,16,QChar('0'))
                                    .arg((quint8)array.at(i*4+1),2,16,QChar('0'))
                                    .arg((quint8)array.at(i*4+2),2,16,QChar('0'))
                                    .arg((quint8)array.at(i*4+3),2,16,QChar('0'));
                algowiget->setData(i,2,temp);
                //type
                if(type==1){
                    float a=QUIHelper::Byte2Float(QUIHelper::hexStrToByteArray(temp));
                    qDebug()<<"float:"<<a;
                    //
                    algowiget->setData(i,3,QString::number(a,'g',6));
                    if(i>=1&&i<=12){
                        para_cal[i-1]=a;
                    }
                }else{
                    algowiget->setData(i,3,QString("%1")
                                       .arg(QUIHelper::byteToInt(QUIHelper::hexStrToByteArray(temp)))
                                            );
                }


            }
            model->setData(model->index(m_modelIndex, 9), "获得参数");
            //保存到算法参数sql数据库
            algowiget->on_btnSave_clicked();


            //
        }else if(respHead=="PARA"){//远距

        }else if(respHead=="PARA"){//近距

        }
    }
}

int UDSForm::judgeResult(){
    //获取首行内容
    QString workmodepara=model->record(0).value(4).toString();
    QString snpara=model->record(m_modelIndex).value(3).toString();
    QString swpara=model->record(0).value(6).toString();
    QString hwpara=model->record(0).value(7).toString();
    QString bootpara=model->record(0).value(8).toString();
    QString algopara=model->record(0).value(9).toString();
    //获取当前行
    QString workmode=model->record(m_modelIndex).value(4).toString();
    QString sn=model->record(m_modelIndex).value(5).toString();
    QString sw=model->record(m_modelIndex).value(6).toString();
    QString hw=model->record(m_modelIndex).value(7).toString();
    QString boot=model->record(m_modelIndex).value(8).toString();
    QString algo=model->record(m_modelIndex).value(9).toString();
    quint8 result=0;
    qDebug()<<"swpara"<<swpara<<"sw"<<sw;
    qDebug()<<"hwpara"<<hwpara<<"hw"<<hw;
    if(workmodepara==workmode){
        result|=0b00100000;
    }
    if(snpara==sn){
        result|=0b00010000;
    }
    if(swpara==sw){
        result|=0b00001000;
    }
    if(hwpara==hw){
        result|=0b00000100;
    }
    if(bootpara==boot){
        result|=0b00000010;
    }
    if(algopara==algo){
        result|=0b00000001;
    }
    return result;
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

    model = new MySubClassedSqlTableModel(this,_db);
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
//    columnNames << "序号" << "名称" << "帧ID" << "类型"<< "格式" << "DLC"<< "数据" << "帧数" << "方向" ;
    columnNames << "序号" <<"详细结果"<<"结果"<< "写入SN"<< "模式" << "序列号"<< "软件版本" << "硬件版本" <<"Boot版本"<< "雷达参数" << "用户" << "时间" ;
    columnWidths.clear();
    columnWidths <<40<<50<<70<<130<<70<<130<<130<<130<<80<<80<<160<<160 ;

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

    //通讯方式委托
//    QStringList connectMode;
//    connectMode << "工厂模式" << "客户模式";
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
    listItem.insert(13, "31 01 02 02 01");
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
        QUIHelper::showMessageBoxError("保存信息失败,信息不能为空,请重新填写!");
    }
    //保持滚动条在底部
    ui->tableMain->scrollToBottom();
}

void UDSForm::on_btnDelete_clicked()
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

void UDSForm::on_btnReturn_clicked()
{
    model->revertAll();
}

void UDSForm::on_btnClear_clicked()
{
    if (model->rowCount() <= 0) {
        return;
    }

    int result=QUIHelper::showMessageBoxQuestion("确定要清空所有生产测试数据吗?");
    if ( result== QMessageBox::Yes) {
        QString sql = "delete FROM UDSBurnData where 序号 !=0";
        QSqlQuery query(_db);
        bool value =query.exec(sql);
        value=model->select();
    }else{
    }
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
        bool result = CANApi::OpenDevice(4,0,0);
        if(result==true)
        {
            ui->textBrowser_Debug->append("设备连接成功");
        }
        else
        {
           QUIHelper::showMessageBoxError("设备连接失败");
           return;
        }

        result = CANApi::CANInit(4,0,0,ui->cBox_Baud->currentIndex());
        if (result == true)
        {
            ui->textBrowser_Debug->append("设备初始化成功");
        }
        else
        {
            QUIHelper::showMessageBoxError("设备初始化失败");
            return;
        }

        result = CANApi::StartCan(4,0,0);
        if (result == true)
        {
            ui->textBrowser_Debug->append("CAN通信启动成功");
            UDS::mConnected = true;//证明设备当前处于连接状态
            UDS::Instance()->startHandleThread();//启动CAN帧数据的接收
        }
        else
        {
            QUIHelper::showMessageBoxError("CAN通信启动失败");
            return;
        }
        ui->button_Connect->setText("关闭");
    }else if(ui->button_Connect->text()=="关闭"){//关闭设备
        bool result = CANApi::CloseDevice(4,0);

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
        //UDS::Instance()->setExitState(1,0);//启动CAN帧数据的接收
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
        arr.resize(1024+2);
        for(int j=0;j<(1024+2);j++) {
            arr[j]=cansendframe[i].DATA[j];
        }
        ui->progressBar->setValue(100*i/cansendframe.count());
        qDebug()<<"send:"<<(QUIHelper::byteArrayToHexStr(arr))<<"i="<<i<<"cansendframe.count()="<<cansendframe.count();
        result=UDS::Instance()->SendAndReceive( UDS::SEND_CAN_ID,cansendframe[i].DATA,1024+2);
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
    if(UDS::Instance()->getdataVerify()!=0||UDS::Instance()->getconsistencyVerify()!=0){
        QUIHelper::showMessageBoxError(QString("数据校验：%1;一致性校验:%2")
                                       .arg(UDS::Instance()->getdataVerify(),2,16,QChar('0'))
                                       .arg(UDS::Instance()->getconsistencyVerify(),2,16,QChar('0'))
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
    result = UDS::Instance()->SendAndReceive(cansendframe[0].DATA, UDS::Instance()->SEND_CAN_ID);
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
        qDebug()<<"UDS::Instance()->getSeedKey():"<<UDS::Instance()->getSeedKey();
        selfSendStr = "27 02 " + UDS::Instance()->getSeedKey();
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
        UDS::Instance()->setWorkMode(UDSUPDATE);
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
void UDSForm::slot_eolsendCommandTimer(){
    static int count =0;
    //定时
    ui->cBoxcansend->setCurrentIndex(m_eolcommandIndex);
    on_cBoxcansend_activated(m_eolcommandIndex);
    ECANStatus ret=eolSendCommandOnce();
    if(ui->cBox_eolContinue->isChecked()){
        if(ret==_STATUS_ERR){
            count++;
            qDebug()<<"重发计数:"<<count;
            if(count>5){//发送五次指令
                m_eolcommandIndex=0;

                ui->buttonSendcan1->setEnabled(true);
                ui->label_out->setText("重发超限！请重测");
            }
        }else{
            m_eolcommandIndex++;
        }
        if(m_eolcommandIndex<16){
            if(m_eolcommandIndex==2)
                m_eolsendCommandTimer->start(4000);
            else if(m_eolcommandIndex==6)
                m_eolsendCommandTimer->start(4000);
            else
                m_eolsendCommandTimer->start(500);
        }else{
            m_eolcommandIndex=0;
            //索引加1
            //ui->lineEdit_SNTail->setText(QString("%1").arg(ui->lineEdit_SNTail->text().toInt()+1,4,10,QChar('0')));
            ui->buttonSendcan1->setEnabled(true);
        }
    }
}
void UDSForm::slot_demarcationTimer(){
    //定时
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
    ECANStatus result=UDS::Instance()->SendAndReceive(UDS::SEND_CAN_ID,(byte*)arry.data(),arry.count());
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
    //判断设备连接状态
    if(ui->button_Connect->text()!="关闭"){
        QUIHelper::showMessageBoxError("请确保设备连接正常，并再次尝试！");
        return;
    }
    //判断合法性，获取参数真值
    {
        if((ui->lineEdit_SNHead->text().length()!=4)&&
           (ui->dateEdit_SN->text().length()!=8)&&
           (ui->lineEdit_SNTail->text().length()!=4)&&
           (ui->lineEdit_HV->text().length()!=16))
        {
            QUIHelper::showMessageBoxError("写入信息格式有误，请检查确认后继续操作！");
            return;
        }
        BurnInfo::SN_Head=ui->lineEdit_SNHead->text();
        BurnInfo::SN_Year=ui->dateEdit_SN->text().left(4);
        BurnInfo::SN_MD=ui->dateEdit_SN->text().right(4);
        BurnInfo::SN_Tail=ui->lineEdit_SNTail->text();
        BurnInfo::HW_A=ui->lineEdit_HV->text().left(8);
        BurnInfo::HW_B=ui->lineEdit_HV->text().right(8);
    }
    ui->buttonSendcan1->setEnabled(false);
    m_eolsendCommandTimer->start(1000);
    //此处可添加can连接判定
    if(ui->cBox_eolContinue->isChecked()){//且为自动发送添加记录到表中
        m_compare_para.clear();
        for (int i=0;i<model->record(0).count();i++) {
            m_compare_para.append(model->record(0).value(i).toString());
        }
        m_modelIndex=model->rowCount();
        model->insertRow(m_modelIndex);
        //更新序号
        model->setData(model->index(m_modelIndex, 0),m_modelIndex+1);
        //更新写入sn到数据库
        model->setData(model->index(m_modelIndex, 3),QString("%1%2%3")
                       .arg(ui->lineEdit_SNHead->text())
                       .arg(ui->dateEdit_SN->text())
                       .arg(ui->lineEdit_SNTail->text()));
        //更新用户
        model->setData(model->index(m_modelIndex, 10),QString("%1")
                       .arg(ui->lineEdit_User->text())
                       );
        //更新开始时间到数据库
        model->setData(model->index(m_modelIndex, 11),QString("%1")
                       .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                       );
    }

}
ECANStatus UDSForm::eolSendCommandOnce(){
    //生成指令
    QByteArray array=QUIHelper::hexStrToByteArray(m_eolselfSendStr);
    qDebug()<<"m_eolselfSendStr:"<<m_eolselfSendStr<<"array.size()"<<array.size();
    UDS::Instance()->setWorkMode(FACTORY);
    QPixmap pixmap(QString(":/imageTest/buttongray.png"));
    ui->label_state->setFixedSize(64,64);
    QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->label_state->setPixmap(fitpixmap);
    ui->label_state->setScaledContents(true);
    //发送指令
    ECANStatus ret= UDS::Instance()->NormalSendAndReceive(CANApi::SEND_CAN_ID_Self1,(byte*)array.data(),array.size());
    if(ret==_STATUS_OK){
        QPixmap pixmap(QString(":/imageTest/buttongreen.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);
    }else if(ret==_STATUS_ERR){
        QPixmap pixmap(QString(":/imageTest/buttonred.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);
    }else{
        QPixmap pixmap(QString(":/imageTest/buttonyellow.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);
    }
    return ret;
}


void UDSForm::on_cBoxcansend_activated(int index)
{
    m_eolcommandIndex=ui->cBoxcansend->currentIndex();

    if(ui->cBoxcansend->currentIndex()==2){//SRHV
        qDebug()<<QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_Head));
        m_eolselfSendStr="53 52 48 56 00 00 00 04 "+QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_Head));
    }else if(ui->cBoxcansend->currentIndex()==3){//SMYE
        qDebug()<<QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_Year));
        m_eolselfSendStr="53 4D 59 45 00 00 00 04 "+QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_Year));
    }else if(ui->cBoxcansend->currentIndex()==4){//SMMD
        qDebug()<<QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_MD));
        m_eolselfSendStr="53 4D 4D 44 00 00 00 04 "+QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_MD));
    }else if(ui->cBoxcansend->currentIndex()==5){//SRSN
        qDebug()<<QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_Tail));
        m_eolselfSendStr="53 52 53 4E 00 00 00 04 "+QUIHelper::byteArrayToHexStr(QUIHelper::asciiStrToByteArray(BurnInfo::SN_Tail));
    }else if(ui->cBoxcansend->currentIndex()==6){//SHVA
        qDebug()<<QUIHelper::byteArrayToHexStr(QUIHelper::hexStrToByteArray(BurnInfo::HW_A));
        m_eolselfSendStr="53 48 56 41 00 00 00 04 "+QUIHelper::byteArrayToHexStr(QUIHelper::hexStrToByteArray(BurnInfo::HW_A));
    }else if(ui->cBoxcansend->currentIndex()==7){//SHVB
        qDebug()<<QUIHelper::byteArrayToHexStr(QUIHelper::hexStrToByteArray(BurnInfo::HW_B));
        m_eolselfSendStr="53 48 56 42 00 00 00 04 "+QUIHelper::byteArrayToHexStr(QUIHelper::hexStrToByteArray(BurnInfo::HW_B));
    }else{
        m_eolselfSendStr=ui->cBoxcansend->currentText();
    }

    ui->labelCanSendDiscrib->setText(m_list_canSendDiscrib.at(index));
}

void UDSForm::on_lineEdit_PW_editingFinished()
{
    //判断权限
    if(ui->lineEdit_PW->text()=="admin"){
        ui->cBox_eolContinue->setEnabled(true);
        ui->checkBox_autoadd->setEnabled(true);
        ui->btnDelete->setEnabled(true);
        ui->btnSave->setEnabled(true);
        ui->buttonSendcan1->setEnabled(true);
        ui->button_AlgoQuery->setEnabled(true);
        ui->button_Consumer->setEnabled(true);
        ui->comboBox->setEnabled(true);
        ui->btnClear->setEnabled(true);
        //
        tempwidget->setChildEnable(true);
        //高低温首次计数清零按钮
        ui->pushButton_3->setEnabled(true);
    }
}

void UDSForm::on_pushButton_2_released()
{
    int result=QUIHelper::showMessageBoxQuestion("请确认是否输入温度："
                                                 "确认：继续后续测试，取消：返回录入温度！");
    if (result==QMessageBox::Yes){

    }else if(result==QMessageBox::No){
        return;
    }
    if(ui->pushButton_2->text()=="开始记录"){

        ui->pushButton_2->setText("记录中...");

    }else{
        ui->pushButton_2->setText("开始记录");
    }
}
//单项信息交互
void UDSForm::on_comboBox_activated(int index)
{
    QList<QStringList> commandPair;
    commandPair.clear();
    commandPair.append(QString("工厂模式_1").split("_"));//0
    commandPair.append(QString("模式查询_0").split("_"));//1
    commandPair.append(QString("写入信息_6,7,2,3,4,5").split("_"));//2
    commandPair.append(QString("读取信息_8,9,10,11,12").split("_"));//3
    commandPair.append(QString("写入序列号_2,3,4,5").split("_"));//4
    commandPair.append(QString("写入硬件号_6,7").split("_"));//5
    commandPair.append(QString("进入EOL模式_8").split("_"));//6
    commandPair.append(QString("查询序列号_9").split("_"));//7
    commandPair.append(QString("查询软件版本_10").split("_"));//8
    commandPair.append(QString("查询boot版本_11").split("_"));//9
    commandPair.append(QString("查询硬件版本_12").split("_"));//10
    commandPair.append(QString("查询参数_13").split("_"));//11
    commandPair.append(QString("退出EOL模式_15").split("_"));//12
    commandPair.append(QString("客户模式_16").split("_"));//13
    //判断合法性，获取参数真值
    {
        if((ui->lineEdit_SNHead->text().length()!=4)&&
           (ui->dateEdit_SN->text().length()!=8)&&
           (ui->lineEdit_SNTail->text().length()!=4)&&
           (ui->lineEdit_HV->text().length()!=16))
        {
            QUIHelper::showMessageBoxError("写入信息格式有误，请检查确认后继续操作！");
            return;
        }
        BurnInfo::SN_Head=ui->lineEdit_SNHead->text();
        BurnInfo::SN_Year=ui->dateEdit_SN->text().left(4);
        BurnInfo::SN_MD=ui->dateEdit_SN->text().right(4);
        BurnInfo::SN_Tail=ui->lineEdit_SNTail->text();
        BurnInfo::HW_A=ui->lineEdit_HV->text().left(8);
        BurnInfo::HW_B=ui->lineEdit_HV->text().right(8);
    }
    if(index==1){
        ui->pushButton_workMode->setText("");
    }
    //添加到记录
    if(index==3){
        int result= QUIHelper::showMessageBoxQuestion("是否添加新记录？");
        if (result==QMessageBox::Yes){
            m_modelIndex=model->rowCount();
            model->insertRow(m_modelIndex);
            model->setData(model->index(m_modelIndex,0),m_modelIndex);

            //更新写入sn到数据库
            model->setData(model->index(m_modelIndex, 3),QString("%1%2%3")
                           .arg(ui->lineEdit_SNHead->text())
                           .arg(ui->dateEdit_SN->text())
                           .arg(ui->lineEdit_SNTail->text()));
            //更新模式
            model->setData(model->index(m_modelIndex, 4),QString("%1")
                           .arg(ui->pushButton_workMode->text())
                           );
            //更新用户
            model->setData(model->index(m_modelIndex, 10),QString("%1")
                           .arg(ui->lineEdit_User->text())
                           );
            //更新开始时间到数据库
            model->setData(model->index(m_modelIndex, 11),QString("%1")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                           );

        }else if(result==QMessageBox::No){
            m_modelIndex=model->rowCount()-1;
            model->setData(model->index(m_modelIndex,0),m_modelIndex);


            //更新写入sn到数据库
            model->setData(model->index(m_modelIndex, 3),QString("%1%2%3")
                           .arg(ui->lineEdit_SNHead->text())
                           .arg(ui->dateEdit_SN->text())
                           .arg(ui->lineEdit_SNTail->text()));
            //更新模式
            model->setData(model->index(m_modelIndex, 4),QString("%1")
                           .arg(ui->pushButton_workMode->text())
                           );
            //更新用户
            model->setData(model->index(m_modelIndex, 10),QString("%1")
                           .arg(ui->lineEdit_User->text())
                           );
            //更新开始时间到数据库
            model->setData(model->index(m_modelIndex, 11),QString("%1")
                           .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"))
                           );
        }
    }
    //
    if(index<commandPair.length()){
        sendSeqList=commandPair.at(index).at(1).split(",");

        for(int i=0;i<sendSeqList.length();i++){
            int ind=sendSeqList.at(i).toInt();

            if(ind<=16&&ind>=0){
                //发送指令
                qDebug()<<"ind=="<<ind;
                ui->cBoxcansend->setCurrentIndex(ind);
                on_cBoxcansend_activated(ind);
                QThread::msleep(30);
                eolSendCommandOnce();
            }else{

            }
        }
    }


    //需要延时等待的指令禁止点击
    if(index==0||index==2){
        //禁用下拉列表
        ui->comboBox->setEnabled(false);
        //全部按钮禁用
        ui->button_InFactory->setEnabled(false);
        ui->button_queryMode->setEnabled(false);
        ui->button_burnInfo->setEnabled(false);
        ui->button_queryInfo->setEnabled(false);
        //延时
        QUIHelper::sleep(1000);
        ui->label_out->setText("雷达复位，请耐心等待3s");
        QUIHelper::sleep(1000);
        ui->label_out->setText("雷达复位，请耐心等待2s");
        QUIHelper::sleep(1000);
        ui->label_out->setText("雷达复位，请耐心等待1s");
        QUIHelper::sleep(1000);
        ui->label_out->setText("复位完成，可继续其它操作");
        //启用下拉列表
        ui->comboBox->setEnabled(true);
        //启用下拉列表
        ui->button_InFactory->setEnabled(true);
        ui->button_queryMode->setEnabled(true);
        ui->button_burnInfo->setEnabled(true);
        ui->button_queryInfo->setEnabled(true);
        //

    }

}

void UDSForm::on_lineEdit_HV_editingFinished()
{
    App::HWVersion=ui->lineEdit_HV->text();
    App::writeConfig();
}
//切换到工厂模式
void UDSForm::on_button_InFactory_released()
{
    ui->comboBox->setCurrentIndex(0);
    on_comboBox_activated(0);
}
//查询当前模式
void UDSForm::on_button_queryMode_released()
{

    ui->comboBox->setCurrentIndex(1);
    on_comboBox_activated(1);
}
//写入信息
void UDSForm::on_button_burnInfo_released()
{
    //添加查重功能
    QString sn=QString("%1%2%3").arg(ui->lineEdit_SNHead->text())
    .arg(ui->dateEdit_SN->text())
    .arg(ui->lineEdit_SNTail->text());
    QString sql = QString("select * FROM UDSBurnData where 写入信息 == %1;").arg(sn);
    QSqlQuery query(_db);
    bool value =query.exec(sql);
    qDebug()<<"查询value:"<<value;
    if(query.next()){//开始就先执行一次next()函数，那么query指向结果集的第一条记录
        int ret=QUIHelper::showMessageBoxQuestion(QString("查询到SN记录已存在,请确认是否继续写入！"));
        if (ret==QMessageBox::Yes){

        }else{
            return;
        }
    }

    ui->comboBox->setCurrentIndex(2);
    on_comboBox_activated(2);
}
//获取信息
void UDSForm::on_button_queryInfo_released()
{
    QString sn=QString("%1%2%3").arg(ui->lineEdit_SNHead->text())
    .arg(ui->dateEdit_SN->text())
    .arg(ui->lineEdit_SNTail->text());
    QString sql = QString("select * FROM UDSBurnData where 写入信息 == %1;").arg(sn);
    QSqlQuery query(_db);
    bool value =query.exec(sql);
    qDebug()<<"查询value:"<<value;
    if(query.next()){//开始就先执行一次next()函数，那么query指向结果集的第一条记录
        int ret=QUIHelper::showMessageBoxQuestion(QString("查询到SN记录已存在,请确认是否继续写入！"));
        if (ret==QMessageBox::Yes){

        }else{
            return;
        }
    }
    ui->comboBox->setCurrentIndex(3);
    on_comboBox_activated(3);
}

void UDSForm::on_button_AlgoQuery_released()
{
    ui->comboBox->setCurrentIndex(11);
    on_comboBox_activated(11);
}

void UDSForm::on_button_Consumer_released()
{
    ui->comboBox->setCurrentIndex(13);
    on_comboBox_activated(13);
}

void UDSForm::on_pushButton_onlineTest_released()
{
    qsrand(time(NULL));
    int n = qrand() % 100;    //产生100以内的随机数
    onlineDeviceForm->addData(n,"222222222",0);
}

void UDSForm::on_pushButton_3_released()
{
    onlineDeviceTimerCounter=0;
    //清空设备列表
    onlineDeviceForm->resetData();

}

void UDSForm::on_button_export_released()
{
    QString file;
    //开始计算用时
    QTime time;
    time.start();
    int MaxCount1=10000;
    QStringList content;
    for (int i = 0; i < MaxCount1; i++) {
        QStringList list;
        for (int j = 0; j < 12; j++) {
            QString text =model->index(i, j).data().toString();
            list.append(text);
        }

        content.append(list.join(";"));
    }

    QList<QString> columnNames;
    QList<int> columnWidths;
    columnNames << "序号" <<"详细结果"<<"结果"<< "写入"<< "模式" << "序列号"<< "软件版本" << "硬件版本" <<"Boot版本"<< "雷达参数" << "用户" << "时间" ;
    columnWidths <<40<<50<<70<<130<<70<<130<<130<<130<<80<<80<<160<<160 ;

    file = qApp->applicationDirPath() +QString("/输出信息/生产测试%1_%2.xls")
            .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm"))
            .arg(ui->lineEdit_remark->text());
    ExcelAPI::Instance()->saveExcel(file, "生产测试信息", "所有信息", QString("%1 导出信息").arg(DATETIME), columnNames, columnWidths, content, true, false, 3, "==", "高低温测试");

    double ms = time.elapsed();
    ui->labInfo1->setText(QString("导出 %1 条数据成功,用时 %2 秒").arg(MaxCount1).arg(QString::number(ms / 1000, 'f', 2)));
    //如果打开excel会有警告提示,要去掉警告提示请双击运行源码下的 excel禁止提示.reg
    //qDebug() << "用时" << ExcelThread::Instance()->getTakeTime() << "毫秒";
    QString url = QString("file:///%1").arg(file);
    QDesktopServices::openUrl(QUrl(url, QUrl::TolerantMode));
}


void UDSForm::on_tabWidget_currentChanged(int index)
{
    guiworkmode=UDS::Instance()->getWorkMode();
    if(index==5){
        if(guiworkmode!=ALIGN){
            QUIHelper::showMessageBoxInfo("切换至整车EOL标定功能");
            UDS::Instance()->setWorkMode(ALIGN);
            guiworkmode=ALIGN;
        }
    }else if(index==1){
        if(guiworkmode!=UDSUPDATE){
            QUIHelper::showMessageBoxInfo("切换至UDS升级功能");
            UDS::Instance()->setWorkMode(UDSUPDATE);
            guiworkmode=UDSUPDATE;
        }
    }else if(index==2||index==3||index==4){
        if(guiworkmode!=FACTORY&&guiworkmode!=CONSUMER){
            QUIHelper::showMessageBoxInfo("切换至生产测试功能");
            UDS::Instance()->setWorkMode(FACTORY);
            guiworkmode=FACTORY;
        }
    }else if(index==6){
        if(guiworkmode!=INITIALPHASE){
            QUIHelper::showMessageBoxInfo("切换至初相校准功能");
            UDS::Instance()->setWorkMode(INITIALPHASE);
            guiworkmode=INITIALPHASE;
        }
    }
    qDebug()<<"guiworkmode"<<guiworkmode<<"UDS::Instance()->setWorkMode"<<guiworkmode;
}

void UDSForm::on_lineEdit_Delay_editingFinished()
{
    QUIHelper::showMessageBoxInfo("请确保输入内容为数字，建议范围1000-3000");
    App::OnlineCaStatDelay=ui->lineEdit_Delay->text().toInt();
    App::writeConfig();
}

void UDSForm::on_tabWidget_tabBarClicked(int index)
{

}

#include "onlineburnform.h"
#include "ui_onlineburnform.h"
QList<HexRecordBlock> OnlineBurnForm::hexRecordBlocks_DriverList ;
OnlineBurnForm::OnlineBurnForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineBurnForm)
{
    ui->setupUi(this);
    QUIHelper::readFileList("","InstructionFileSet/FirmwareUpdate.txt",m_list_canSendCommand,m_list_canSendDiscrib);
    ui->cBoxcansend->clear();
    ui->cBoxcansend->addItems(m_list_canSendCommand);
    ui->cBoxCanSendDiscrib->addItems(m_list_canSendDiscrib);

    QPixmap pixmap(QString(":/imageTest/buttongray.png"));
    ui->label_state->setFixedSize(64,64);
    QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
    ui->label_state->setPixmap(fitpixmap);
    ui->label_state->setScaledContents(true);
    {//
        qDebug()<<"App::OnlineBurnfile"<<App::OnlineBurnfile;
        QString str("请先确保can连接状态正常\n"
                    "步骤1：重启雷达，三秒内点击‘初始化’按钮发送初始化指令，进入升级模式；\n"
                    "   正常情况会返回Boot版本提示框！\n"
                    "步骤2：选择文件（若输入框已存在且路径正常无需重复选择）；\n"
                    "步骤3：点击‘开始升级’开始升级，正常情况下升级结束会弹窗提示；\n");
        displayStr(str,1);
        m_ParseFile=false;
        if(QUIHelper::fileIsExist(App::OnlineBurnfile)){
            ui->lineEditChooseFile->setText(App::OnlineBurnfile);
            //已经成功对相关文件进行解析。
            m_ParseFile = fileParse.ParseHexFile(ui->lineEditChooseFile->text(), hexRecordBlocks_DriverList);
            if (m_ParseFile == true){
                displayStr("Hex应用程序解析成功");
                ui->pushButton_update->setEnabled(true);
            }else{
                QUIHelper::showMessageBoxError("Hex文件解析失败，请再次解析");
            }
        }


    }
    burnTimer=new QTimer(this);
    connect(burnTimer,&QTimer::timeout,this,&OnlineBurnForm::slot_burnTimer);
    burnTimer->setSingleShot(true);
    connect(ui->textBrowser_Debug,&QTextBrowser::textChanged,this,&OnlineBurnForm::slot_autoScroll);
}
void OnlineBurnForm::slot_autoScroll(){
    ui->textBrowser_Debug->moveCursor(QTextCursor::End);
}
OnlineBurnForm::~OnlineBurnForm()
{
    delete ui;
}
void OnlineBurnForm::slot_burnTimer(){
    QUIHelper::showMessageBoxError("固件升级失败！",3);
    displayStr("固件升级失败！",1);
}

void OnlineBurnForm::on_cBoxcansend_activated(int index)
{
    ui->cBoxCanSendDiscrib->setCurrentIndex(index);
    m_commandIndex=ui->cBoxcansend->currentIndex();
    m_selfSendStr=ui->cBoxcansend->currentText();
    ui->cBoxCanSendDiscrib->setCurrentIndex(index);
    QByteArray array=QUIHelper::hexStrToByteArray(m_selfSendStr);
    //qDebug()<<"m_selfSendStr:"<<m_selfSendStr<<"array.size()"<<array.size();
    ECANStatus ret=UDS::Instance()->NormalSend(CANApi::SEND_CAN_ID_Self1,(byte*)array.data(),array.size(),200); //ptr就是主窗口的指针，可以用来操作主窗口
    if(ret==_STATUS_OK){

        QPixmap pixmap(QString(":/imageTest/buttongreen.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);

    }else if(ret==_STATUS_ERR){
        displayStr("发送指令异常！\n",1);
        QPixmap pixmap(QString(":/imageTest/buttonred.png"));
        ui->label_state->setFixedSize(64,64);
        QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
        ui->label_state->setPixmap(fitpixmap);
        ui->label_state->setScaledContents(true);

    }
}

void OnlineBurnForm::on_cBoxCanSendDiscrib_activated(int index)
{
    ui->cBoxcansend->setCurrentIndex(index);
    qDebug()<<"on_cBoxCanSendDiscrib_activated";

}
//load hex file
void OnlineBurnForm::on_toolButton_released()
{
    ui->lineEditChooseFile->setText(QUIHelper::getFileName("Hex Files(*.hex)"));
    //已经成功对相关文件进行解析。
    m_ParseFile = fileParse.ParseHexFile(ui->lineEditChooseFile->text(), hexRecordBlocks_DriverList);
    if (m_ParseFile == true){
        App::OnlineBurnfile=ui->lineEditChooseFile->text();
        App::writeConfig();
        displayStr("Hex应用程序解析成功!\r\n");
        ui->pushButton_update->setEnabled(true);
    }else{
        QUIHelper::showMessageBoxError("Hex文件解析失败，请再次解析");
        displayStr("Hex文件解析失败，请再次解析!\r\n",1);
    }
}
ECANStatus OnlineBurnForm::writehexBlockData(QList<HexRecordBlock> HexBlockList, int type)
{
    //声明一个布尔类型
    ECANStatus result = _STATUS_ERR;
    //实例化字符型列表
    QList<byte> allData ;
    int EndAddress = 0x00000000;
    //获取整个数据块的末尾地址
    EndAddress =(HexBlockList.at(0).data.count());
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

    alldatalen = HexBlockList.at(0).data.count();
    for (int j = 0; j < alldatalen; j++)
    {
        alldata[j] = (quint8)HexBlockList.at(0).data.at(j);
    }
    //46 49 4C 45 xx xx xx xx,FILE指令
    byte data[8]={0x46,0x49,0x4C,0x45,0x00,0x00,0x00,0x00};
    data[4]=(byte)(alldatalen>>24);
    data[5]=(byte)(alldatalen>>16);
    data[6]=(byte)(alldatalen>>8);
    data[7]=(byte)(alldatalen);
    qDebug()<<"##"<<QString("len:%1 %2 %3 %4 %5 %6 %7 %8")
                .arg(data[0],2,16,QChar('0'))
                .arg(data[1],2,16,QChar('0'))
                .arg(data[2],2,16,QChar('0'))
                .arg(data[3],2,16,QChar('0'))
                .arg(data[4],2,16,QChar('0'))
                .arg(data[5],2,16,QChar('0'))
                .arg(data[6],2,16,QChar('0'))
                .arg(data[7],2,16,QChar('0'));
    //先发送长度
    result=UDS::Instance()->NormalSend(CANApi::SEND_CAN_ID_Self1,data,8,500);
    int count=0;
    //将整个列表再拆分成若干个CAN发送帧数据
    QList<CAN_Standard_FRAME> cansendframe ;
    CAN_Standard_FRAME One_send_Frame;
    while(alldatalen>0){
        if(alldatalen>=8){
            One_send_Frame.validlen=8;
            One_send_Frame.FrameNumber=count;
            memcpy(One_send_Frame.DATA,alldata+count*8,8);
            alldatalen-=8;
        }else{
            One_send_Frame.validlen=alldatalen;
            One_send_Frame.FrameNumber=count;
            memcpy(One_send_Frame.DATA,alldata+count*8,alldatalen);
            alldatalen=0;
        }
        cansendframe.append(One_send_Frame);

        count++;
    }
    qDebug()<<"cansendframe.length():"<<cansendframe.length();

    ui->progressBar->setValue(0);


    //拆分完成所有数据帧之后，需要按照顺序进行相应数据的发送。

    for (int i = 0; i < cansendframe.count(); i++)
    {
        QByteArray arr;
        arr.resize(8);
        for(int j=0;j<(8);j++) {
            arr[j]=cansendframe[i].DATA[j];
        }
        ui->progressBar->setValue(100*i/cansendframe.count());
        qDebug()<<"send:"<<(QUIHelper::byteArrayToHexStr(arr))<<"i="<<i<<"cansendframe.count()="<<cansendframe.count();
        result=UDS::Instance()->NormalSend(CANApi::SEND_CAN_ID_Self1,cansendframe[i].DATA,cansendframe[i].validlen,0);
        if(i%800==0){
            QPixmap pixmap(QString(":/imageTest/buttongreen.png"));
            ui->label_state->setFixedSize(64,64);
            QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
            ui->label_state->setPixmap(fitpixmap);
            ui->label_state->setScaledContents(true);
        }else if(i%800==400){
            QPixmap pixmap(QString(":/imageTest/buttongray.png"));
            ui->label_state->setFixedSize(64,64);
            QPixmap fitpixmap = pixmap.scaled(64,64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // 饱满填充
            ui->label_state->setPixmap(fitpixmap);
            ui->label_state->setScaledContents(true);
        }
        if (result != _STATUS_OK)
        {
            return result;
        }
    }
    //烧写完成
    ui->progressBar->setValue(100);
    return _STATUS_OK;
}

void OnlineBurnForm::on_pushButton_released()
{

    ui->textBrowser_Debug->clear();
    //int ret=QUIHelper::showMessageBoxQuestion(QString("请确认进入固件更新模式！"));
    UDS::Instance()->setWorkMode(ONLINEUPDATE);
//    if (ret==QMessageBox::Yes){
//        UDS::Instance()->setWorkMode(ONLINEUPDATE);
//    }else{
//        displayStr("未进入固件更新模式！\n",1);
//        return;
//    }
    displayStr("上位机进入固件更新模式！\n");
    displayStr("发送初始化指令！\n");
    ui->cBoxcansend->setCurrentIndex(0);
    on_cBoxcansend_activated(0);



}
void OnlineBurnForm::stringToHtmlFilter(QString &str)
{
   //注意这几行代码的顺序不能乱，否则会造成多次替换
   str.replace("&","&amp;");
   str.replace(">","&gt;");
   str.replace("<","&lt;");
   str.replace("\"","&quot;");
   str.replace("\'","&#39;");
   str.replace(" ","&nbsp;");
   str.replace("\n","<br>");
   str.replace("\r","<br>");
}
void OnlineBurnForm::stringToHtml(QString& str,QColor crl)
{
    QByteArray array;
    array.append(crl.red());
    array.append(crl.green());
    array.append(crl.blue());
    QString strC(array.toHex());
    //str = QString("<span style=\"color:#%1;\">%2</span>").arg(strC).arg(str);
    str = QString("<span style=\"color:#%1; font-size:14px\">%2</span>").arg(strC).arg(str);
}


void OnlineBurnForm::on_pushButton_update_released()
{
     if(!m_ParseFile){
         QUIHelper::showMessageBoxError("升级固件异常，请检查路径！");
         return;
     }
     displayStr("开始升级！\n");
     ECANStatus ret=writehexBlockData(hexRecordBlocks_DriverList,0);
     if(ret==_STATUS_OK){
        QString str("数据发送完毕!");
        displayStr("数据发送完毕!\n");
        burnTimer->start(5000);

     }else{
         QUIHelper::showMessageBoxError("升级中途结束，数据发送出现异常，需重新升级！",3);
     }
}
void OnlineBurnForm::displayStr(QString str,int index){
    stringToHtmlFilter(str);
    QColor clrR;
    if(index==0)
        clrR= QColor(26,145,114);
    else if(index==1)
        clrR= QColor(255,0,0);
    stringToHtml(str,clrR);
    ui->textBrowser_Debug->insertHtml(str);
}
void OnlineBurnForm::updateDone(){
    if(burnTimer->isActive()){
        burnTimer->stop();
    }
}

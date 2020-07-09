#include "frmtcpclient.h"
#include "ui_frmtcpclient.h"
#include "quiwidget.h"
#include<QtEndian>
frmTcpClient::frmTcpClient(QWidget *parent) : QWidget(parent), ui(new Ui::frmTcpClient)
{
    ui->setupUi(this);
    this->initForm();
    this->initConfig();
    m_buffer.clear();
    qRegisterMetaType<Frame_Content>("Frame_Content&");
}

frmTcpClient::~frmTcpClient()
{
    delete ui;
}

void frmTcpClient::initForm()
{
    isOk = false;
    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(connected()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
//    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(slot_readmesg()));

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(on_btnSend_clicked()));

    ui->cboxInterval->addItems(App::Intervals);
    ui->cboxData->addItems(App::Datas);
}

void frmTcpClient::initConfig()
{
    ui->ckHexSend->setChecked(App::HexSendTcpClient);
    connect(ui->ckHexSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckHexReceive->setChecked(App::HexReceiveTcpClient);
    connect(ui->ckHexReceive, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAscii->setChecked(App::AsciiTcpClient);
    connect(ui->ckAscii, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckDebug->setChecked(App::DebugTcpClient);
    connect(ui->ckDebug, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->ckAutoSend->setChecked(App::AutoSendTcpClient);
    connect(ui->ckAutoSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

    ui->cboxInterval->setCurrentIndex(ui->cboxInterval->findText(QString::number(App::IntervalTcpClient)));
    connect(ui->cboxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

    ui->txtServerIP->setText(App::TcpServerIP);
    connect(ui->txtServerIP, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    ui->txtServerPort->setText(QString::number(App::TcpServerPort));
    connect(ui->txtServerPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

    this->changeTimer();
}

void frmTcpClient::saveConfig()
{
    App::HexSendTcpClient = ui->ckHexSend->isChecked();
    App::HexReceiveTcpClient = ui->ckHexReceive->isChecked();
    App::AsciiTcpClient = ui->ckAscii->isChecked();
    App::DebugTcpClient = ui->ckDebug->isChecked();
    App::AutoSendTcpClient = ui->ckAutoSend->isChecked();
    App::IntervalTcpClient = ui->cboxInterval->currentText().toInt();
    App::TcpServerIP = ui->txtServerIP->text().trimmed();
    App::TcpServerPort = ui->txtServerPort->text().trimmed().toInt();
    App::writeConfig();

    this->changeTimer();
}

void frmTcpClient::changeTimer()
{
    timer->setInterval(App::IntervalTcpClient);
    if (App::AutoSendTcpClient) {
        if (!timer->isActive()) {
            timer->start();
        }
    } else {
        if (timer->isActive()) {
            timer->stop();
        }
    }
}

void frmTcpClient::append(int type, const QString &data, bool clear)
{
    static int currentCount = 0;
    static int maxCount = 100;

    if (clear) {
        ui->txtMain->clear();
        currentCount = 0;
        return;
    }

    if (currentCount >= maxCount) {
        ui->txtMain->clear();
        currentCount = 0;
    }

    if (ui->ckShow->isChecked()) {
        return;
    }

    //过滤回车换行符
    QString strData = data;
    strData = strData.replace("\r", "");
    strData = strData.replace("\n", "");

    //不同类型不同颜色显示
    QString strType;
    if (type == 0) {
        strType = "发送";
        ui->txtMain->setTextColor(QColor("darkgreen"));
    } else {
        strType = "接收";
        ui->txtMain->setTextColor(QColor("red"));
    }

    strData = QString("时间[%1] %2: %3").arg(TIMEMS).arg(strType).arg(strData);
    ui->txtMain->append(strData);
    currentCount++;
}

void frmTcpClient::connected()
{
    isOk = true;
    ui->btnConnect->setText("断开");
    append(0, "服务器连接");
}

void frmTcpClient::disconnected()
{
    isOk = false;
    tcpSocket->abort();
    ui->btnConnect->setText("连接");
    append(1, "服务器断开");
}

void frmTcpClient::handleMsg(){
    //缓冲区没有数据，直接无视
    if( tcpSocket->bytesAvailable() <= 0 )
    {
        return;
    }
    //临时获得从缓存区取出来的数据，但是不确定每次取出来的是多少。
    QByteArray buffer;
    //如果是信号readyRead触发的，使用readAll时会一次把这一次可用的数据全总读取出来
    //所以使用while(m_tcpClient->bytesAvailable())意义不大，其实只执行一次。
    buffer = tcpSocket->readAll();
    //上次缓存加上这次数据
    /*
    上面有讲到混包的三种情况，数据A、B，他们过来时有可能是A+B、B表示A包+B包中一部分数据，
    然后是B包剩下的数据，或者是A、A+B表示A包一部分数据，然后是A包剩下的数据与B包组合。
    这个时候，我们解析时肯定会残留下一部分数据，并且这部分数据对于下一包会有效，所以我们
    要和下一包组合起来。
    */
    m_buffer.append(buffer);
    quint32 type_id, mesg_len;
    int totalLen = m_buffer.size();
    //不够包头的数据直接就不处理。
    if( totalLen < MINSIZE )//
    {
        return;
    }
    //与QDataStream绑定，方便操作。
    QDataStream packet(m_buffer);
    packet.setByteOrder(QDataStream::BigEndian);

    //packet >> type_id >> mesg_len;
    //totalLen = m_buffer.size();
    while(totalLen >= 4){

        packet >> type_id;
        if(type_id!=MSG_TYPE_ID){
            m_buffer.remove(0,4);
            totalLen-=4;
        }else{
            break;
        }

    }
    //不够包头的数据直接就不处理。
    if( totalLen < MINSIZE )//
    {
        return;
    }
    packet >> mesg_len;    //小端转换
    int len=qFromBigEndian(mesg_len);
    qDebug()<<QString("type_id:0x%1 mesg_len:0x%2 len:0x%3 totalLen:%4").arg(type_id,8,16,QChar('0'))
              .arg(mesg_len,8,16,QChar('0'))
              .arg(len,8,10,QChar('0'))
              .arg(totalLen,8,10,QChar('0'));

    while(1)
    {
        //如果不够长度等够了在来解析
        if(totalLen <len)
        {
            break;
        }
        //qDebug() << __FUNCTION__  << QThread::currentThreadId() << QThread::currentThread();
        //数据足够多，且满足我们定义的包头的几种类型
        switch(type_id)
        {
            case MSG_TYPE_ID:
                memcpy(&m_frame,m_buffer.data()+8,len);
//                memcpy(&m_frame.RTC_TimeNowStruct,m_buffer+8,sizeof(RTC_TimeTypeDef));
//                memcpy(&m_frame.mt,m_buffer+8+sizeof(RTC_TimeTypeDef),sizeof(uint32_t));
//                memcpy(&m_frame.num_obj,m_buffer+8+sizeof(RTC_TimeTypeDef)+4,sizeof(uint16_t));
//                memcpy(&m_frame.frame,m_buffer+8+sizeof(RTC_TimeTypeDef)+4+2,sizeof(uint16_t));
//                qDebug()<<QString("RTC:%1 %2 %3 %4 mt:%5 num_obj:%6 framecount:%7 len:%8 size:%9")
//                        .arg(m_frame.RTC_TimeNowStruct.RTC_Hours,4,10,QChar('0'))
//                        .arg(m_frame.RTC_TimeNowStruct.RTC_Minutes,4,16,QChar('0'))
//                        .arg(m_frame.RTC_TimeNowStruct.RTC_Seconds,4,16,QChar('0'))
//                        .arg(m_frame.RTC_TimeNowStruct.RTC_H12,4,16,QChar('0'))
//                        .arg(m_frame.mt,8,16,QChar('0'))
//                        .arg(m_frame.num_obj,8,16,QChar('0'))
//                        .arg(m_frame.frame,8,16,QChar('0'))
//                        .arg(len,8,16,QChar('0'))
//                        .arg(sizeof(m_frame),8,16,QChar('0'));
                emit(sig_FrameContent(m_frame));
            break;


            case MSG_TYPE_FILE_START:
            {
                packet >> m_fileName;
            }
            break;


            case MSG_TYPE_FILE_SENDING:
            {
//                QByteArray tmpdata;
//                packet >> tmpdata;
//                //这里我把所有的数据都缓存在内存中，因为我们传输的文件不大，最大才几M;
//                //大家可以这里收到一个完整的数据包，就往文件里面写入，即使保存。
//                m_recvData.append(tmpdata);
//                //这个可以最后拿来校验文件是否传完，或者是否传的完整。
//                m_checkSize += tmpdata.size();
//                //打印提示，或者可以连到进度条上面。
//                emit sig_displayMesg(QString("recv: %1").arg(m_checkSize));
            }
            break;


            case MSG_TYPE_FILE_END:
            {
//                packet >> m_DataSize;
//                saveImage();
//                clearData();
            }
                break;


            default:
            break;
        }
        //缓存多余的数据
        buffer = m_buffer.right(totalLen - len-8);
        //更新长度
        totalLen = buffer.size();
        //更新多余数据
        m_buffer = buffer;
    }
}
//接收消息
void frmTcpClient::slot_readmesg()
{
    //qDebug() << __FUNCTION__  << QThread::currentThreadId() << QThread::currentThread();
    QFuture<void> f1 =QtConcurrent::run(this,&frmTcpClient::handleMsg);

}
void frmTcpClient::readData()
{
    QByteArray data = tcpSocket->readAll();
    if (data.length() <= 0) {
        return;
    }

    QString buffer;
    if (App::HexReceiveTcpClient) {
        buffer = QUIHelper::byteArrayToHexStr(data);
    } else if (App::AsciiTcpClient) {
        buffer = QUIHelper::byteArrayToAsciiStr(data);
    } else {
        buffer = QString(data);
    }

    append(1, buffer);

    //自动回复数据,可以回复的数据是以;隔开,每行可以带多个;所以这里不需要继续判断
    if (App::DebugTcpClient) {
        int count = App::Keys.count();
        for (int i = 0; i < count; i++) {
            if (App::Keys.at(i) == buffer) {
                sendData(App::Values.at(i));
                break;
            }
        }
    }
}

void frmTcpClient::sendData(const QString &data)
{
    QByteArray buffer;
    if (App::HexSendTcpClient) {
        buffer = QUIHelper::hexStrToByteArray(data);
    } else if (App::AsciiTcpClient) {
        buffer = QUIHelper::asciiStrToByteArray(data);
    } else {
        buffer = data.toLatin1();
    }

    tcpSocket->write(buffer);
    append(0, data);
}

void frmTcpClient::on_btnConnect_clicked()
{
    if (ui->btnConnect->text() == "连接") {
        tcpSocket->abort();
        tcpSocket->connectToHost(App::TcpServerIP, App::TcpServerPort);
    } else {
        tcpSocket->abort();
    }
}

void frmTcpClient::on_btnSave_clicked()
{
    QString data = ui->txtMain->toPlainText();
    if (data.length() <= 0) {
        return;
    }

    QString fileName = QString("%1/%2.txt").arg(QUIHelper::appPath()).arg(STRDATETIME);
    QFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(data.toUtf8());
        file.close();
    }

    on_btnClear_clicked();
}

void frmTcpClient::on_btnClear_clicked()
{
    append(0, "", true);
}

void frmTcpClient::on_btnSend_clicked()
{
    if (!isOk) {
        return;
    }

    QString data = ui->cboxData->currentText();
    if (data.length() <= 0) {
        return;
    }

    sendData(data);
}

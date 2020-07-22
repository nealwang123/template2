#include "uds.h"
bool UDS::mConnected = false;
QScopedPointer<UDS> UDS::self;
UDS *UDS::Instance()
{
    if (self.isNull()) {
        QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new UDS);
        }
    }

    return self.data();
}
UDS::UDS(QObject *parent) : QObject(parent)
{
    qDebug()<<"AbstractDevice construct!"<<QThread::currentThread();
    MyThread *thread = new MyThread;
    this->moveToThread(thread);
    thread->start();
    workmode=FACTORY;

    dbcloader=new DBCReader;
    dbcloader->loadDBCFile("./高低温测试DBC.dbc");
    qDebug()<<"dbcloader loadDBCFile end";

}
/// <summary>
/// 接收和发送函数
/// </summary>
/// <param name="data"></data可以是很多的数据不一定是单帧数据可以实多针数据/>
/// <param name="can_id"></param>
/// <returns></returns>
ECANStatus UDS::SendAndReceive(uint can_id,byte data[],int dataLength){
    ECANStatus result = _STATUS_ERR ;

    session=new Session();

    session->sendframe.byteAmount =dataLength;
    //
    memcpy(session->sendframe.array,&data[0],session->sendframe.byteAmount);
    session->sendframe.offset = 0;
    session->sendframe.SID = data[0];

    //表示相关数据的发送
    result = sendData(can_id,data,dataLength);//发送数据，包括单帧和多帧数据

    //判断数据发送的结果
    if (result != _STATUS_OK)
    {
        return _STATUS_ERR;
    }
    else
    {
        return _STATUS_OK;
    }
}
/// <summary>
/// 计算要发送的帧的数量
/// </summary>
/// <param name="data">表示要发送的数据</param>
/// <returns></returns>
int UDS::FrameNum(byte data[],int dataLength)
{
    int frmAmount = 0;
    if ((dataLength + 1) % 7 == 0)//如果发送数据可以被7整除，则发送整除数个连续帧
    {
        frmAmount = (dataLength + 1) / 7;
    }
    else
    {
        frmAmount = (dataLength + 1) / 7 + 1;//如果数据被能被整数个连续帧发送，则需要发送整除数+1个数据。
    }

    return frmAmount;
}
/// <summary>
/// 发送首帧数据
/// </summary>
/// <param name="data"></param>
/// <param name="frame"></param>
/// <returns></returns>
ECANStatus UDS::sendFirstFrame(byte data[], int dataLength,UDSDataFrame frame)
{
    VCI_CAN_OBJ cAN_OBJs[1];
    cAN_OBJs[0].Data[0] = (byte)((byte)N_PDU_TYPE_FF | ((ushort)dataLength/sizeof (byte)) >> 8);
    cAN_OBJs[0].Data[1] = (byte)dataLength/sizeof(byte);
    cAN_OBJs[0].DataLen = 8;
    cAN_OBJs[0].ID = SEND_CAN_ID;
    memcpy(cAN_OBJs[0].Data+2,data,6);
    //Array.Copy(data, 0, cAN_OBJs[0].data, 2, 6);  //FF装载6个字节数据
    emitEventRecv(cAN_OBJs[0]);
    return CANApi::SendOneFrame(0, 0, cAN_OBJs);//发送一个单帧
}
/// <summary>
/// 发送数据，包括单帧和多帧数据
/// </summary>
/// <param name="data"></param>
/// <param name="canID"></param>
/// <returns></returns>
ECANStatus UDS::sendData(uint canID,byte data[],int dataLength ){
    workmode=UDSUPDATE;
    uint TimeOut = Timeout * 2;
    //step1：清空连续帧
    serialFrameGlabol.clear();

    //step2：发送单帧
    if (dataLength <= 7)//发送单帧数据
    {
        //声明一个单数据帧
        VCI_CAN_OBJ frame[1];
        frame[0].ID = canID;
        memset(frame[0].Data,0,8);
        frame[0].Data[0] = (byte)((byte)(N_PDU_TYPE_SF) | (byte)dataLength);

        memcpy(frame[0].Data+1,data,dataLength);
       // Array.Copy(data, 0, frame[0].Data, 1, data.Length);//用于矩阵中数组的复制操作
        frame[0].DataLen = 8;/// (byte)data.Length;

        setNextState();
        //进行单帧的发送
        if (CANApi::SendOneFrame(0, 0, frame) != _STATUS_OK)
        {
            return _STATUS_ERR;
        }
        emitEventRecv(frame[0]);

        //Console.WriteLine("sendData:"+ QUIHelper::byteArrayToHexStr(frame[0].Data));

        if (udsSleep((int)TimeOut*2)){
            //雷达响应
            if(m_waitforNext==1){//正响应
                qDebug()<<"正响应，可以继续下一条指令";
            }else if(m_waitforNext==-1){//负响应
                qDebug()<<"负响应，不可以继续下一条指令";
            }
        }else{
            qDebug()<<"指令超时";
            return _STATUS_TIME_OUT;
        }


    }
    else //step3：多帧发送
    {
        //step4：计算传输本数据一共要发送多少个连续帧数据。
        int amount = FrameNum(data,dataLength);
        //step5：将数据整理成一帧一帧的数据，保存到帧列表中
        for (int i = 0; i < amount; i++)
        {
            //实例化连续帧
            SerialFrame serialFrame;
            memset(serialFrame.data,0,8);
            //step6：首帧判断
            if (i == 0)//判断是否为首帧
            {
                //判断为首帧
                serialFrame.isFisrstFrame = true;//判断为首帧
                                                 //获取要发送的数据
                //Array.Copy(session->sendframe.array, session->sendframe.offset, serialFrame.data, 0, 6);
                memcpy(serialFrame.data,session->sendframe.array+session->sendframe.offset,6);
                //将发送的数据量偏移更新为+6后的
                session->sendframe.offset += 6;
                //连续帧首帧的数据长度
                serialFrame.length = 6;
            }
            else
            {
                //设置首帧标志位位false
                serialFrame.isFisrstFrame = false;
                //获取剩下的要连续帧发送的数据量的七个字节或者剩下不够七个字节的部分
                int length = (session->sendframe.byteAmount - session->sendframe.offset) < 7 ? session->sendframe.byteAmount - session->sendframe.offset : 7;
                //将七个字节要发送的数据量转移到指定的存储空间
                memcpy(serialFrame.data,session->sendframe.array+session->sendframe.offset,length);
                //Array.Copy(session->sendframe.array, session->sendframe.offset, serialFrame->data, 0, length);
                //设置发送数据的偏移量
                session->sendframe.offset += length;
                //表示连续帧要发送的数据长度
                serialFrame.length = length;//表示该连续帧要发送的数据的长度
            }
            //step7：将数据块分成若干个数据帧并保存到连续帧List中
            serialFrameGlabol.append(serialFrame);//向List中添加一个连续帧

        }
        //step8：对连续帧的每个帧进行编号
        for (int i = 0; i < serialFrameGlabol.count(); i++){
            serialFrameGlabol[i].serial = i % 16;//将List<>中的序号进行编号
//            qDebug()<<QString("serialFrame:%1 %2 %3 %4 %5 %6 %7 %8 %8")
//                      .arg(serialFrameGlabol[i].data[0],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[1],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[2],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[3],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[4],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[5],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[6],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].data[7],2,16,QChar('0'))
//                    .arg(serialFrameGlabol[i].serial,2,16,QChar('0'));
        }

        int count = 0;  //发送数据帧数量
                        //实例化一个单数据帧对象
        UDSDataFrame frame;
        //CAN地址获取
        frame.canID = canID;
        setNextState();
        //step9：首帧发送
        if (sendFirstFrame(data,dataLength, frame) != _STATUS_OK)
        {
            return _STATUS_ERR;
        }
        //到此处，则完成首帧数据的发送
        count++;

        //step10，阻塞当前线程TimeOut的时间或者设置set（）后，前面两者谁先到达，先执行谁，最晚不超过TimeOut
        //不管是哪个线程，只要执行到此就会执行等待
        //Console.WriteLine("TimeOut1");
        //QUIHelper::sleep(1000);
        //Thread.Sleep(100); //毫秒
        if (udsSleep((int)TimeOut*2))//阻塞当前线程运行时间为2s，并确定阻塞当前
        //if(true)
        {                                                 //当前线程2s的命令被成功执行
            //Console.WriteLine("TimeOut2");                                              //step11：计算首帧之后剩下的要发送的连续帧的数量
            int BS = serialFrameGlabol.count() - count;
            qDebug()<<"BS=========="<<BS<<"count:"<<count;
            while (count < serialFrameGlabol.count() )
            {
                //step11：判断流控帧传回的数量
                if (session->receiveFrame.BS != 0)
                {
                    //BS = (serialFrameGlabol.count()  - count) < session->receiveFrame.BS ? (serialFrameGlabol.count() - count) : session->receiveFrame.BS;
                }

                //每次发送BS个帧，等待FC返回
                for (int j = 0; j < BS; j++)
                {
                    //CF帧第一个循环从1编号，后续从0~F 编号
                    //实例化一个单帧
                    VCI_CAN_OBJ cAN_OBJs[1] ;
                    cAN_OBJs[0].ID= canID;
                    cAN_OBJs[0].DataLen = 8;
                    //开辟8个字节空间
                    //cAN_OBJs[0].Data = new byte[8];
                    //获取数据
                    cAN_OBJs[0].Data[0] = (byte)( (byte)N_PDU_TYPE_CF | (byte)serialFrameGlabol[count].serial);
                    memcpy(cAN_OBJs[0].Data+1,serialFrameGlabol[count].data,serialFrameGlabol[count].length);
                    //Array.Copy(serialFrameGlabol[count].data, 0, cAN_OBJs[0].data, 1, serialFrameGlabol[count].length);
                    emitEventRecv(cAN_OBJs[0]);
                    setNextState();
                    //连续帧中单帧数据的发送
                    if (CANApi::SendOneFrame(0, 0, cAN_OBJs) != _STATUS_OK)
                    {
                        return _STATUS_ERR;
                    }
                    //如果成功了则表示发送成功的数量增1
                    count++;
                    //主线程阻塞STmin 的时间。
                    //QThread.Sleep(session.receiveFrame.STmin);//连续帧发送之间等待时间间隔
                    //QThread::msleep(session->receiveFrame.STmin);
                    if(j==0)
                        qDebug()<<"session->receiveFrame.STmin"<<session->receiveFrame.STmin;
                }

                if (count != serialFrameGlabol.count())
                {

                    if (udsSleep((int)TimeOut*2))
                    //if(true)
                    {
                        qDebug()<<"count=====" + QString::number(count) +"Glabol.Count" +QString::number(serialFrameGlabol.count());
                        continue;
                    }
                    else
                    {
                        return _STATUS_TIME_OUT;
                    }
                }else{
                    if (udsSleep((int)TimeOut*3))
                    //if(true)
                    {
                        qDebug()<<"count=====" + QString::number(count) +"Glabol.Count" +QString::number(serialFrameGlabol.count());
                        continue;
                    }
                    else
                    {
                        return _STATUS_TIME_OUT;
                    }
                }
            }
        }
        else
        {
            return _STATUS_TIME_OUT;
        }
    }
    return _STATUS_OK;
}//包括发送数据和CAN地址信息

int UDS::getNextState(){
    return m_waitforNext;
}
void UDS::setNextState(){
    m_waitforNext=0;
}

/// <summary>
/// 接收数据处理
/// </summary>
void UDS::ReceiveDataProc(){

    CAN_ERR_INFO cAN_ERR_INFO;

//    try
//    {
        //VCI_ResetCAN(4,0,0);
        //始终不停循环的接收CAN卡发送过来的数据帧。
        while (true)//表示连续不断的循环
        {

            //step1:首先需要判断当前设备处于连接状态
            if (!mConnected) break;


            //step3:获取本次接收到的数据帧数

            VCI_CAN_OBJ mmsg ;//实例化数据帧结构体

            int ReceiveNum = 0;//接收信息帧数量
            gRecMsgBufHead = 0;
            do
            {
                uint mlen = 1;
                //receive(设备类型，设备索引号，第几路can通道，接收结构体首地址指针，接收长度，等待时间)
                if (!((CANApi::RecvFrames(&mmsg, 1) == _STATUS_OK) & (mlen > 0)))//如果接收不正常且接收数据长度为0，则跳出执行代码
                {
                    break;
                }
                //否则，将获取到的相关数据转移到接收缓存区的指定区域

//                gRecMsgBuf[gRecMsgBufHead].ID = mmsg.ID;//获取数据帧的id地址
//                gRecMsgBuf[gRecMsgBufHead].DataLen = mmsg.DataLen;//获取数据帧的接收长度
//                memcpy(gRecMsgBuf[gRecMsgBufHead].Data,mmsg.Data,8);
//                //gRecMsgBuf[gRecMsgBufHead].data = mmsg.data;//获取数据帧的相关数据
//                gRecMsgBuf[gRecMsgBufHead].ExternFlag = mmsg.ExternFlag;//检测该数据帧是否为扩展帧
//                gRecMsgBuf[gRecMsgBufHead].RemoteFlag = mmsg.RemoteFlag;//检测该数据帧是否为远程帧
//                gRecMsgBuf[gRecMsgBufHead].TimeStamp = mmsg.TimeStamp;//获取接收数据帧的时间
//                memcpy(gRecMsgBuf[gRecMsgBufHead].Reserved,mmsg.Reserved,8);
//                //gRecMsgBuf[gRecMsgBufHead].Reserved = mmsg.Reserved;//系统保留
//                gRecMsgBuf[gRecMsgBufHead].TimeFlag = mmsg.TimeFlag;//获取接收数据帧的标志位

                memcpy(&gRecMsgBuf[gRecMsgBufHead],&mmsg,sizeof (VCI_CAN_OBJ));
                gRecMsgBufHead += 1;//接收缓冲区首地址自增1
                if (gRecMsgBufHead >= REC_MSG_BUF_MAX)//如果超出接收缓冲区的最大值，则进行清零操作
                {
                    gRecMsgBufHead = 0;
                }
                ReceiveNum++;//接收数据帧的个数增加1
            }
            while (ReceiveNum < 500);//缓冲区一次只能接收500帧数据


            //step4：对接收到的相应数据帧进行处理
            if (ReceiveNum <= 0)//如果没有检测到数据，则获取CAN卡故障信息
            {
                //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码，
                //千万不能省略这一步（即使你可能不想知道错误码是什么）
               //CANApi::VCI_ReadErrInfo(4, 0, 0, cAN_ERR_INFO);
            }
            else//表示已经成功接收到信息
            {
                //qDebug()<<"ReceiveNum::"<<ReceiveNum;

                //step6：对获取到的数据帧进行具体的单帧数据信息解析
                for (int i = 0; i < ReceiveNum; i++)
                {
                    //if (i >= gRecMsgBuf.Length) break;//如果接收到的数据帧长度为0，则跳出循环

                    //提取出一帧数据
                    VCI_CAN_OBJ ReceiveOneFrame;

                    memcpy(&ReceiveOneFrame,&gRecMsgBuf[i],sizeof (VCI_CAN_OBJ));
                    //
//                    QString temp,data;

//                    for(int j=0;j<ReceiveOneFrame.DataLen;j++){
//                        temp=QString("%1").arg(ReceiveOneFrame.Data[j],2,16,QChar('0'));
//                        data.append(temp);
//                        data.append(" ");
//                    }
//                    QTime now=QTime::fromMSecsSinceStartOfDay(qRound(ReceiveOneFrame.TimeStamp*0.1));

//                    qDebug()<<QString("ID:0x%1 Data:%2 time:%3 %5%6").arg(ReceiveOneFrame.ID,4,16,QChar('0'))
//                              .arg(data)
//                              .arg(ReceiveOneFrame.TimeStamp,8,16,QChar('0'))
//                              .arg(now.toString("hh:mm:ss.zzz."))
//                              .arg(QString::number(ReceiveOneFrame.TimeStamp%10));
                    //安装标定模式，进入扩展会话模式
                    if(workmode==ALIGN&&(ReceiveOneFrame.ID==0x7A9||ReceiveOneFrame.ID==0x4E0)){
                        QString temp,data;
                        for(int j=0;j<ReceiveOneFrame.DataLen;j++){
                            temp=QString("%1").arg(ReceiveOneFrame.Data[j],2,16,QChar('0'));
                            data.append(temp);
                            data.append(" ");
                        }
                        QString header=QString("TOPC %1 %2 %3").arg(4).arg(0).arg(0);
                        QString senddata=QString(" %1 %2 %3 %4 %5")
                                .arg(ReceiveOneFrame.TimeStamp,8,16,QChar('0'))
                                .arg(ReceiveOneFrame.ID,8,16,QChar('0'))
                                .arg(ReceiveOneFrame.DataLen,4,10,QChar('0'))
                                .arg(data)
                                .arg(i,4,10,QChar('0'));
                        header=header+senddata;

                        emit(installAlignData(header));

                    }
                    //工厂模式或者客户模式下
                    else if(((ReceiveOneFrame.ID>=0)&&(ReceiveOneFrame.ID<=9999)&&(workmode==FACTORY||workmode==CONSUMER))){
                        qDebug()<<"__ReceiveOneFrame.ID"<<QString("%1").arg(ReceiveOneFrame.ID,4,16,QChar('0'))<<QString("Data: %1 %2 %3 %4 %5 %6 %7 %8").arg(ReceiveOneFrame.Data[0],2,16,QChar('0')).arg(ReceiveOneFrame.Data[1],2,16,QChar('0')).arg(ReceiveOneFrame.Data[2],2,16,QChar('0')).arg(ReceiveOneFrame.Data[3],2,16,QChar('0')).arg(ReceiveOneFrame.Data[4],2,16,QChar('0')).arg(ReceiveOneFrame.Data[5],2,16,QChar('0')).arg(ReceiveOneFrame.Data[6],2,16,QChar('0')).arg(ReceiveOneFrame.Data[7],2,16,QChar('0'));
                        QByteArray b ;
                        b.resize(ReceiveOneFrame.DataLen);
                        b=QByteArray((const char*)ReceiveOneFrame.Data);
                        QString str=QUIHelper::byteArrayToAsciiStr(b);
                        str=str.left(4);
                        static qint32 lastlen=0;
                        if(str=="DONE"||str=="RRCF"||str=="EOLS"||str=="RRSN"||str=="RRSV"||str=="RRHV"||str=="RRBV"||str=="EOLR"||str=="PARA"){
                            respHead=str;
                            lastlen=(qint32)(ReceiveOneFrame.Data[4]<<24)|(ReceiveOneFrame.Data[5]<<16)|(ReceiveOneFrame.Data[6]<<8)|(ReceiveOneFrame.Data[7]<<0);
                            qDebug()<<"respHead"<<respHead<<"lastlen"<<lastlen;
                            m_recvArray.clear();
                            m_recvArray.resize(lastlen);
                            if(lastlen==0){
                                //获取完有效数据发送到界面
                                emit(emitEOLInfo(respHead,m_recvArray));
                                qDebug()<<"1获取到有效数据。。。。。。";
                                m_waitforNext=1;
                            }

                        }else if(str=="GBYE"){//异常指令，相当于uds异常帧，帧重发
                            m_waitforNext=-1;
                        }else{//非限定字符串
                            if(lastlen>0){//限定字符串后续有效内容
                                qDebug()<<"lastlen"<<lastlen<<"ReceiveOneFrame.DataLen"<<ReceiveOneFrame.DataLen<<" (lastlen-ReceiveOneFrame.DataLen)"<<(lastlen-ReceiveOneFrame.DataLen);
                                if((lastlen-ReceiveOneFrame.DataLen)>=0){
                                    qDebug()<<"找到连续帧";
                                    static int index=0;
    //                                QByteArray bar;
    //                                bar.resize(ReceiveOneFrame.DataLen);
                                    for(int count=0;count<ReceiveOneFrame.DataLen;count++){
                                        m_recvArray[index]=ReceiveOneFrame.Data[count];
                                        index++;
                                    }
                                   // m_recvArray=QByteArray((const char*)ReceiveOneFrame.Data);
                                    //m_recvArray.append(bar);
                                    lastlen-=ReceiveOneFrame.DataLen;
                                    qDebug()<<"lastlen运算后"<<lastlen;
                                    if(lastlen==0){
                                        //获取完有效数据发送到界面
                                        emit(emitEOLInfo(respHead,m_recvArray));
                                        qDebug()<<"2获取到有效数据。。。。。。"<<"respHead"<<respHead<<"m_recvArray.size()"<<m_recvArray.size()<<" "<<QUIHelper::byteArrayToAsciiStr(m_recvArray);
                                        m_waitforNext=1;
                                        qDebug()<<"m_waitforNext"<<m_waitforNext;
                                        index=0;
                                    }
                                }
                            }else{//lastlen==0执行内容，非限定字符串
                                if(((ReceiveOneFrame.Data[0]&0xFF)==0x02)){//高低温测试
                                    qDebug()<<lastlen<<"非工厂模式限定字符串 ReceiveOneFrame.Data[0]:"<<QString::number(ReceiveOneFrame.Data[0])<<QString::number(ReceiveOneFrame.Data[0]&0x0F)<<QString::number((ReceiveOneFrame.Data[0]&0x0F)==0x02);
                                    QStringList list;
                                    list=dbcloader->parseMSGWithData(0x01,ReceiveOneFrame.Data);
                                    QString tempstr="";
                                    for(int i=0;i<ReceiveOneFrame.DataLen;i++){
                                        tempstr.append(QString("%1 ").arg(ReceiveOneFrame.Data[i],2,16,QChar('0')));
                                    }
                                    //发送数据到界面
                                    emit(emitTemperatureTest(ReceiveOneFrame.ID,tempstr.trimmed(),list));
                                }
                            }

                        }
                    }
                    //step7：判断接收到的数据帧的ID地址是否与设定地址相同
                    else if (RECEIVE_CAN_ID == ReceiveOneFrame.ID&&(workmode==UDSUPDATE))
                    {
                        //step8：检查登陆日志文件
                        //logmanager.Writelog(logmanager.ArrayToString(frame.Data), RECEIVE_CAN_ID.ToString("X3") + "  接收");

                        //step9：获取当前接收报文帧的帧类型
                        session->receiveFrame.frameType = (byte)(ReceiveOneFrame.Data[0] & 0xF0);
                        qDebug()<<"session->receiveFrame.frameType:"<<QString::number(session->receiveFrame.frameType,16);
                        //接收流控帧
                        if (session->receiveFrame.frameType == N_PDU_TYPE_FC)//接收到流控帧
                        {
                            session->receiveFrame.FS = (byte)(ReceiveOneFrame.Data[0] & 0x0F);//获取接收到的流状态
                            session->receiveFrame.BS = ReceiveOneFrame.Data[1];//获取接收到的可发送帧数量
                            session->receiveFrame.STmin = ReceiveOneFrame.Data[2];//获取发送连续帧之间的时间间隔

                            //接收到流控帧之后才能进行连续帧的发送，set（）方法是继续执行该线程的意思。
                            //session->sendframe.waitEvent.Set();//上位机可以发送连续帧了
                            m_waitforNext=1;
                        }
                        //首帧
                        else if (session->receiveFrame.frameType ==N_PDU_TYPE_FF)
                        {
                            //获取接收数据的长度
                            session->receiveFrame.length = (ReceiveOneFrame.Data[0] & 0x0F) << 8 | ReceiveOneFrame.Data[1];
                            //开辟连续帧接收的数据量大小的空间
//                            session->receiveFrame.receivedData = new byte[session->receiveFrame.length];
                            //初始化接收数据的数据量
                            session->receiveFrame.receivedDataCount = 0;
                            //获取接收到的6个字节的数据
                            memcpy(ReceiveOneFrame.Data+2*sizeof (byte), session->receiveFrame.receivedData, 6);
                            //更新已经接收的数据量
                            session->receiveFrame.receivedDataCount += 6;//接收的数据量
                            //获取期望连续帧的序列号
                            session->receiveFrame.expectCFSerialNumber = 1;//期望的连续帧序列号

                            //发送FC帧
                            UDSDataFrame FCframe ;//= new UDSDataFrame();//其中UDSDataFrame是单帧的意思
                            FCframe.canID = SEND_CAN_ID;//发送客户端地址
                            FCframe.data = new byte[8];//开辟8个字节的数据控件
                            FCframe.data[0] = (byte)(N_PDU_TYPE_FC | 0x00);//获取流控帧类型
                            FCframe.data[1] = 0x00;//数据帧数量
                            FCframe.data[2] = 0x14;//发送连续帧之间的时间间隔(BS)
                            //接收到单帧数据后，进行流控帧数据的发送
                            //Can_Card_Call.SendOneFrame(0,0, FCframe);//发送流控制帧
                            //session->sendframe.waitEvent.Set();//上位机可以发送连续帧了
                        }
                        //连续帧
                        else if (session->receiveFrame.frameType ==N_PDU_TYPE_CF)//如果接收到的是连续帧
                        {
                            //判断连续帧序列号是否正确
                            if (session->receiveFrame.expectCFSerialNumber % 16 == (0x0F & ReceiveOneFrame.Data[0]))
                            {
                                //判断接收连续帧的数量。
                                int length = (session->receiveFrame.length- session->receiveFrame.receivedDataCount) < 7 ?
                                    (session->receiveFrame.length - session->receiveFrame.receivedDataCount) : 7;
                                //获取连续帧中接收到的数据

                                //Array.Copy(ReceiveOneFrame.Data, 1, session->receiveFrame.receivedData, session->receiveFrame.receivedDataCount, length);
                                //已接收数据量更新为这次接收后的数据量
                                session->receiveFrame.receivedDataCount += length;
                                //期望下次接收到的连续帧的序列号
                                session->receiveFrame.expectCFSerialNumber++;
                            }

                            //判断连续帧中想要接收的数据已经全部接收完成
                            if (session->receiveFrame.length == session->receiveFrame.receivedDataCount)
                            {
                                //全部数据已经接收完毕，通知上位机，反馈已经送达
                                //通知上位机，反馈已经送达
                                if (session->sendframe.SID + 0x40 == session->receiveFrame.receivedData[0] ||
                                    session->receiveFrame.receivedData[0] == 0x7F)
                                {
                                    //session->sendframe.waitEvent.Set();//
                                    m_waitforNext=1;
                                }
                                else
                                {
                                    //超时
                                }
                            }
                            else
                            {
                                //失败问题
                            }
                        }
                        //单帧
                        else if (session->receiveFrame.frameType ==N_PDU_TYPE_SF)//如果接收到的是单帧数据
                        {
                            //数据长度
                            session->receiveFrame.length = ReceiveOneFrame.Data[0] & 0x0F;
                            //开辟相应长度的内存空间
                            //session->receiveFrame.receivedData = new byte[session->receiveFrame.length];//开辟数据空间

                            //获取接收到的单帧的数据
                            memcpy(session->receiveFrame.receivedData,ReceiveOneFrame.Data+1,session->receiveFrame.length);
                            //Array.Copy(ReceiveOneFrame.data, 1, session->receiveFrame.receivedData, 0, session->receiveFrame.length);//获取数据

                            qDebug()<<"session->sendframe.SID"+QString::number(session->sendframe.SID,16)<<"session SID:0X" + QString::number(session->receiveFrame.receivedData[0],16) + " session datalen:" + QString::number(session->receiveFrame.length,16);
                            //判断接收到的服务标识符是否等于预期标识符
                            if ((quint8)(session->sendframe.SID + 0x40) == (quint8)session->receiveFrame.receivedData[0])
                            {

                                //种子获取
                                if (session->receiveFrame.receivedData[0]==0x67&& session->receiveFrame.length==6)
                                {
                                    seedkey.seedbyte[0] = session->receiveFrame.receivedData[2];
                                    seedkey.seedbyte[1] = session->receiveFrame.receivedData[3];
                                    seedkey.seedbyte[2] = session->receiveFrame.receivedData[4];
                                    seedkey.seedbyte[3] = session->receiveFrame.receivedData[5];
                                    //Console.WriteLine("种子获取:"+CAN_Card.Can_Card_Call.byteToHexStr(SeedKey.seedbyte));
                                    seedkey.keyResult();
                                    qDebug()<<("Key:" + QString("%1 %2 %3 %4")
                                               .arg(seedkey.keybyte[0],2,16,QChar('0'))
                                               .arg(seedkey.keybyte[1],2,16,QChar('0'))
                                               .arg(seedkey.keybyte[2],2,16,QChar('0'))
                                               .arg(seedkey.keybyte[3],2,16,QChar('0')));
                                }else if(session->receiveFrame.receivedData[0]==0x71&& session->receiveFrame.length==6){
                                    if(session->receiveFrame.receivedData[2]==0x02&&session->receiveFrame.receivedData[3]==0x02){
                                        dataVerify=session->receiveFrame.receivedData[5];
                                        qDebug()<<("dataVerify:" + QString("%1")
                                                   .arg(dataVerify,2,16,QChar('0'))
                                                   );
                                    }else if(session->receiveFrame.receivedData[2]==0xFF&&session->receiveFrame.receivedData[3]==0x01){
                                        consistencyVerify=session->receiveFrame.receivedData[4];
                                        qDebug()<<("consistencyVerify:" + QString("%1")
                                                   .arg(consistencyVerify,2,16,QChar('0'))
                                                   );
                                    }


                                }
                                //session->sendframe.waitEvent.Set();
                                qDebug()<<"肯定响应！！！！";
                                m_waitforNext=1;

                            }

                            //服务否定响应
                            else if (session->receiveFrame.receivedData[0] == 0x7F)
                            {
                                //36下载时的特殊反馈，ECU正在处理数据，稍后会发送肯定或者否定的反馈
                                if (session->receiveFrame.receivedData[2] == 0x78)
                                {

                                    //emitEventRecv(ReceiveOneFrame);
                                    continue;
                                }
                                else
                                {
                                    //session->sendframe.waitEvent.Set();
                                    m_waitforNext=-1;

                                }
                            }
                            ///Thread.Sleep(100);

                        }
                        ///此处必须放到事件Set函数之后，否则界面线程阻塞情况下此处也会阻塞
                        emitEventRecv(ReceiveOneFrame);
                    }
                }
            }
        }
//    }
//    catch (Exception ee)
//    {
//    }

}
quint8 UDS::getdataVerify(){
    return dataVerify;
}
quint8 UDS::getconsistencyVerify(){
    return consistencyVerify;
}
QString UDS::getSeedKey(){
    return QString("%1 %2 %3 %4")
            .arg(seedkey.keybyte[0],2,16,QChar('0'))
            .arg(seedkey.keybyte[1],2,16,QChar('0'))
            .arg(seedkey.keybyte[2],2,16,QChar('0'))
            .arg(seedkey.keybyte[3],2,16,QChar('0'));
}
int UDS::setExitState(int threadIndex,int exi){
    QMutexLocker locker(&mutex);
    if(threadIndex==0){
        this->m_exitStateThread1=exi;
    }else if(threadIndex==1){
        this->m_exitStateThread2=exi;
    }
    return 0;
}
void UDS::startHandleThread(){
    //启动接收线程
    this->star();
}
//线程2启动函数
void UDS::star()//启动函数是需要在原有代码基础上增加
{
    qDebug()<<"QtConcurrent::run(this,&UDS::stateThread);//多线程执行死循环启动:";
    m_exitStateThread2=1;
    QtConcurrent::run(this,&UDS::stateThread);//多线程执行死循环启动，可以带参数，具体格式可以查阅网上其它资料
}
//线程2
/*
* 功能定位：接收线程
*/
void UDS::stateThread(){
    qDebug()<<"UDS::stateThread():"<<QThread::currentThread();
    qDebug()<< "[command:]" <<m_exitStateThread2;
    ReceiveDataProc();
    qDebug()<<"CanDevice::stateThread() end thread!!";
}
bool UDS::udsSleep(int sec)
{

    QTime dieTime = QTime::currentTime().addMSecs(sec);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        //qDebug()<<"延时等待。。。"<<m_waitforNext;
        if(m_waitforNext==1){//正响应
            qDebug()<<"延时等待结束 正响应";
            return true;
        }else if(m_waitforNext==-1){//负响应
            qDebug()<<"延时等待结束 负响应";
            return false;
        }


    }
    return false;
}
void UDS::setWorkMode(int mode){

    workmode=mode;
}
int  UDS::getWorkMode(){

    return workmode;
}
//非uds
ECANStatus UDS::NormalSendAndReceive(uint can_id,byte data[],int dataLength){
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
        obj[i].SendType = 0;
        obj[i].ExternFlag = 0;
        obj[i].RemoteFlag = 0;
        if ((dataLength - i * 8) >= 8){
            obj[i].DataLen=8;
            memcpy(obj[i].Data,(data+i*8),8);
        }else{
            //obj[i].DataLen=dataLength - i*8;
            obj[i].DataLen=(dataLength - i*8);//(dataLength - i*8);//;
            memcpy(obj[i].Data,(data+i*8),(dataLength - i*8));
        }
        qDebug()<<" obj[i].ID"<<QString("%1").arg(obj[i].ID,4,16,QChar('0'))<<"obj[i].DataLen"<<obj[i].DataLen;
        setNextState();
        int ret=VCI_Transmit(4,0,0,&obj[i],1);
        //QThread::msleep(1);
        qDebug()<<"ret=="<<ret;
    }
    //int ret=VCI_Transmit(4,0,0,obj,n);
    //int ret=VCI_Transmit(4,0,0,obj,n);
    qDebug()<<"uDS.get"<<getNextState();
    if(udsSleep(2000)){
        qDebug()<<"指令正常完成！";
    }else{

        if(getNextState()==-1){
            qDebug()<<"指令异常！ _STATUS_ERR";
            return _STATUS_ERR;
        }
        qDebug()<<"指令异常！ _STATUS_TIME_OUT";
        return _STATUS_TIME_OUT;
    }
    return _STATUS_OK;
}
